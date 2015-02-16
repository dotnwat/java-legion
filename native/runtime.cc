#include <jni.h>

#include <legion.h>

#include "include/org_legion_Runtime.h"
#include "include/org_legion_TaskLauncher.h"
#include "include/org_legion_Future.h"
#include "include/org_legion_Task.h"

using namespace LegionRuntime::HighLevel;

enum TaskID {
  TASK_WRAPPER_ID,
  TOP_LEVEL_TASK_WRAPPER_ID,
};

static JavaVM *jvm;

jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
  jvm = vm;

  JNIEnv *env;
  int ret = jvm->GetEnv((void**)&env, JNI_VERSION_1_6);
  assert(ret == JNI_OK);

  return JNI_VERSION_1_6;
}

static JNIEnv *getJniEnv(void) {
  JNIEnv *env;
  jint rs = jvm->AttachCurrentThread(reinterpret_cast<void **>(&env), NULL);
  assert(rs == JNI_OK);
  return env;
}


/*
 *
 */
template<class PTR, class DERIVED> class LegionNativeClass {
 public:
  static jclass getJClass(JNIEnv *env, const char *jclazz_name) {
    jclass jclazz = env->FindClass(jclazz_name);
    assert(jclazz != NULL);
    return jclazz;
  }

  static jfieldID getHandleFieldID(JNIEnv *env) {
    static jfieldID fid = env->GetFieldID(
        DERIVED::getJClass(env), "nativeHandle", "J");
    assert(fid != NULL);
    return fid;
  }

  static void setHandle(JNIEnv *env, jobject jobj, PTR ptr) {
    env->SetLongField(jobj, getHandleFieldID(env), reinterpret_cast<jlong>(ptr));
  }
};

/*
 * Virtualizes the task_id
 */
class TaskLauncherWrapper {
 public:

  TaskLauncherWrapper() {
    launcher.task_id = TASK_WRAPPER_ID;
  }

  TaskLauncher launcher;

  int task_id;
  size_t arg_size;
  char *arg_data;
};

struct TaskArgumentWrapper {
  int task_id;
  size_t size;
  char data[];
};


/*
 *
 */
class TaskLauncherJni : public LegionNativeClass<TaskLauncherWrapper*, TaskLauncherJni> {
 public:
  static jclass getJClass(JNIEnv *env) {
    return LegionNativeClass::getJClass(env, "org/legion/TaskLauncher");
  }
};

/*
 *
 */
class FutureJni : public LegionNativeClass<Future*, FutureJni> {
 public:
  static jclass getJClass(JNIEnv *env) {
    return LegionNativeClass::getJClass(env, "org/legion/Future");
  }
};

void task_wrapper(const Task *task, const std::vector<PhysicalRegion>& regions,
    Context ctx, HighLevelRuntime *runtime)
{
  JNIEnv *env = getJniEnv();

  TaskArgumentWrapper *args = (TaskArgumentWrapper*)task->args;
  int task_id = args->task_id;

  // create array of pointers to physical regions
  unsigned nregions = regions.size();
  jlongArray jregions = env->NewLongArray(nregions);
  for (unsigned i = 0; i < nregions; i++) {
    jlong addr = (jlong)&regions[i];
    env->SetLongArrayRegion(jregions, i, 1, &addr);
  }

  // cache these!
  jclass rt_clazz = env->FindClass("org/legion/Runtime");
  assert(rt_clazz);
  jmethodID task_wrapper_mid = env->GetStaticMethodID(rt_clazz, "task_wrapper", "(J[JJJI)V");
  assert(task_wrapper_mid);

  /*
   * Invoke the Java task wrapper.
   *
   * TODO:
   *  - Are there failure modes other than fail-stop? What exceptions might
   *  make sense to allow through?
   */
  env->CallStaticVoidMethod(rt_clazz, task_wrapper_mid, (jlong)task,
      jregions, (jlong)ctx, (jlong)runtime, (jint)task_id);
  if (env->ExceptionOccurred()) {
    env->ExceptionDescribe();
    env->ExceptionClear();
    assert(0);
  }

  jvm->DetachCurrentThread();
}

void top_level_task_wrapper(const Task *task,
    const std::vector<PhysicalRegion>& regions,
    Context ctx, HighLevelRuntime *runtime)
{
  JNIEnv *env = getJniEnv();

  // create array of pointers to physical regions
  unsigned nregions = regions.size();
  jlongArray jregions = env->NewLongArray(nregions);
  for (unsigned i = 0; i < nregions; i++) {
    jlong addr = (jlong)&regions[i];
    env->SetLongArrayRegion(jregions, i, 1, &addr);
  }

  // cache these!
  jclass rt_clazz = env->FindClass("org/legion/Runtime");
  assert(rt_clazz);
  jmethodID task_wrapper_mid = env->GetStaticMethodID(rt_clazz,
      "top_level_task_wrapper", "(J[JJJ)V");
  assert(task_wrapper_mid);

  env->CallStaticVoidMethod(rt_clazz, task_wrapper_mid, (jlong)task,
      jregions, (jlong)ctx, (jlong)runtime);
  if (env->ExceptionOccurred()) {
    env->ExceptionDescribe();
    env->ExceptionClear();
    assert(0);
  }

  jvm->DetachCurrentThread();
}

/*
 * Class:     org_legion_Runtime
 * Method:    hlr_execute_task
 * Signature: (JJJ)J
 */
jlong Java_org_legion_Runtime_hlr_1execute_1task(JNIEnv *env, jobject jobj,
    jlong jrt, jlong jctx, jlong jlauncher)
{
  HighLevelRuntime *runtime = reinterpret_cast<HighLevelRuntime*>(jrt);
  Context ctx = reinterpret_cast<Context>(jctx);
  TaskLauncherWrapper *launcher = reinterpret_cast<TaskLauncherWrapper*>(jlauncher);

  // so much memory leak terriblenss
  TaskArgumentWrapper *task_args = (TaskArgumentWrapper*)malloc(sizeof(*task_args) + launcher->arg_size);
  task_args->task_id = launcher->task_id;
  task_args->size = launcher->arg_size;
  memcpy(&task_args->data[0], launcher->arg_data, task_args->size);

  launcher->launcher.argument = TaskArgument(task_args,
      sizeof(*task_args) + launcher->arg_size);

  Future *result = new Future;
  *result = runtime->execute_task(ctx, launcher->launcher);

  return reinterpret_cast<jlong>(result);
}

void Java_org_legion_Runtime_start(JNIEnv *env, jclass jrt, jobjectArray jargs)
{
  const unsigned argc = env->GetArrayLength(jargs);
  char **argv = new char*[argc];
  for (unsigned i = 0; i < argc; i++) {
    jstring jargv = static_cast<jstring>(env->GetObjectArrayElement(jargs, i));
    const char *jargvp = env->GetStringUTFChars(jargv, NULL);
    argv[i] = strdup(jargvp);
    env->ReleaseStringUTFChars(jargv, jargvp);
  }

  HighLevelRuntime::register_legion_task<task_wrapper>(
      TASK_WRAPPER_ID,
      Processor::LOC_PROC,
      true,  // single
      false); // index

  HighLevelRuntime::register_legion_task<top_level_task_wrapper>(
      TOP_LEVEL_TASK_WRAPPER_ID,
      Processor::LOC_PROC,
      true,  // single
      false); // index

  HighLevelRuntime::set_top_level_task_id(TOP_LEVEL_TASK_WRAPPER_ID);

  HighLevelRuntime::start(argc, argv);

  // clean up
  for (unsigned i = 0; i < argc; i++)
    delete [] argv[i];
  delete [] argv;
}

/*
 * Class:     org_legion_TaskLauncher
 * Method:    newTaskLauncher
 * Signature: ()V
 */
void Java_org_legion_TaskLauncher_newTaskLauncher(JNIEnv *env, jobject jobj)
{
  TaskLauncherWrapper *l = new TaskLauncherWrapper();
  TaskLauncherJni::setHandle(env, jobj, l);
}

/*
 * Class:     org_legion_TaskLauncher
 * Method:    disposeInternal
 * Signature: (J)V
 */
void Java_org_legion_TaskLauncher_disposeInternal(JNIEnv *env, jobject jobj,
    jlong jhandle)
{
  delete reinterpret_cast<TaskLauncher*>(jhandle);
  TaskLauncherJni::setHandle(env, jobj, NULL);
}

/*
 * Class:     org_legion_TaskLauncher
 * Method:    setTaskId
 * Signature: (JI)V
 */
void Java_org_legion_TaskLauncher_setTaskId(JNIEnv *env, jobject jobj,
    jlong jhandle, jint jtaskId)
{
  TaskLauncherWrapper *l = reinterpret_cast<TaskLauncherWrapper*>(jhandle);
  l->task_id = static_cast<int>(jtaskId);
}

/*
 * Class:     org_legion_TaskLauncher
 * Method:    setTaskArg
 * Signature: (J[B)V
 */
void Java_org_legion_TaskLauncher_setTaskArg(JNIEnv *env, jobject jobj,
    jlong jhandle, jbyteArray jarg)
{
  TaskLauncherWrapper *l = reinterpret_cast<TaskLauncherWrapper*>(jhandle);

  jsize nelms = env->GetArrayLength(jarg);
  l->arg_data = new char[nelms]; // FIXME; memory leak never freed
  jbyte *data = env->GetByteArrayElements(jarg, NULL);
  memcpy(l->arg_data, (char*)data, static_cast<size_t>(nelms));
  l->arg_size = static_cast<size_t>(nelms);
  env->ReleaseByteArrayElements(jarg, data, JNI_ABORT);
}

/*
 * Class:     org_legion_Future
 * Method:    disposeInternal
 * Signature: (J)V
 */
void Java_org_legion_Future_disposeInternal(JNIEnv *env, jobject jobj, jlong jhandle)
{
  delete reinterpret_cast<Future*>(jhandle);
  FutureJni::setHandle(env, jobj, NULL);
}

/*
 * Class:     org_legion_Future
 * Method:    waitOnComplete
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_org_legion_Future_waitOnComplete(JNIEnv *env, jobject jobj, jlong jhandle)
{
  Future *future = reinterpret_cast<Future*>(jhandle);
  future->get_void_result();
}

/*
 * Class:     org_legion_Task
 * Method:    getArgs
 * Signature: (J)[B
 */
jbyteArray Java_org_legion_Task_getArgs(JNIEnv *env, jobject jobj, jlong jhandle)
{
  const Task *task = reinterpret_cast<const Task*>(jhandle);

  TaskArgumentWrapper *args = (TaskArgumentWrapper*)task->args;

  jbyteArray ret = env->NewByteArray(args->size);
  env->SetByteArrayRegion(ret, 0, args->size, (const jbyte*)args->data);

  return ret;
}
