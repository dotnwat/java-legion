#include <jni.h>

#include <legion.h>

#include "include/org_legion_Runtime.h"
#include "include/org_legion_Task.h"
#include "portal.h"

using namespace LegionRuntime::HighLevel;

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
 * Method:    executeTask
 * Signature: (JJJ)J
 */
jlong Java_org_legion_Runtime_executeTask(JNIEnv *env, jobject jobj,
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

/*
 * Class:     org_legion_Runtime
 * Method:    createIndexSpace
 * Signature: (JJJ)J
 */
jlong Java_org_legion_Runtime_createIndexSpace(JNIEnv *env,
    jobject jobj, jlong jrt, jlong jctx, jlong jmax_elems)
{
  HighLevelRuntime *runtime = reinterpret_cast<HighLevelRuntime*>(jrt);
  Context ctx = reinterpret_cast<Context>(jctx);

  IndexSpace *is = new IndexSpace;
  *is = runtime->create_index_space(ctx, static_cast<size_t>(jmax_elems));

  return reinterpret_cast<jlong>(is);
}

/*
 * Class:     org_legion_Runtime
 * Method:    createFieldSpace
 * Signature: (JJ)J
 */
JNIEXPORT jlong JNICALL Java_org_legion_Runtime_createFieldSpace(JNIEnv *env,
    jobject jobj, jlong jrt, jlong jctx)
{
  HighLevelRuntime *runtime = reinterpret_cast<HighLevelRuntime*>(jrt);
  Context ctx = reinterpret_cast<Context>(jctx);

  FieldSpace *fs = new FieldSpace;
  *fs = runtime->create_field_space(ctx);

  return reinterpret_cast<jlong>(fs);
}

/*
 * Class:     org_legion_Runtime
 * Method:    createFieldAllocator
 * Signature: (JJJ)J
 */
JNIEXPORT jlong JNICALL Java_org_legion_Runtime_createFieldAllocator
  (JNIEnv *env, jobject jobj, jlong jrt, jlong jctx, jlong jfs)
{
  HighLevelRuntime *runtime = reinterpret_cast<HighLevelRuntime*>(jrt);
  Context ctx = reinterpret_cast<Context>(jctx);
  FieldSpace *fs = reinterpret_cast<FieldSpace*>(jfs);

  FieldAllocator *fa = new FieldAllocator;
  *fa = runtime->create_field_allocator(ctx, *fs);

  return reinterpret_cast<jlong>(fa);
}

/*
 * Class:     org_legion_Runtime
 * Method:    createLogicalRegion
 * Signature: (JJJJ)J
 */
JNIEXPORT jlong JNICALL Java_org_legion_Runtime_createLogicalRegion
  (JNIEnv *env, jobject jobj, jlong jrt, jlong jctx, jlong jis, jlong jfs)
{
  HighLevelRuntime *runtime = reinterpret_cast<HighLevelRuntime*>(jrt);
  Context ctx = reinterpret_cast<Context>(jctx);
  IndexSpace *is = reinterpret_cast<IndexSpace*>(jis);
  FieldSpace *fs = reinterpret_cast<FieldSpace*>(jfs);

  LogicalRegion *lr = new LogicalRegion;
  *lr = runtime->create_logical_region(ctx, *is, *fs);

  return reinterpret_cast<jlong>(lr);
}

/*
 * Class:     org_legion_Runtime
 * Method:    mapRegion
 * Signature: (JJJ)J
 */
JNIEXPORT jlong JNICALL Java_org_legion_Runtime_mapRegion
  (JNIEnv *env, jobject jobj, jlong jrt, jlong jctx, jlong jil)
{
  HighLevelRuntime *runtime = reinterpret_cast<HighLevelRuntime*>(jrt);
  Context ctx = reinterpret_cast<Context>(jctx);
  InlineLauncher *il = reinterpret_cast<InlineLauncher*>(jil);

  PhysicalRegion *pr = new PhysicalRegion;
  *pr = runtime->map_region(ctx, *il);

  return reinterpret_cast<jlong>(pr); }

/*
 * Class:     org_legion_Runtime
 * Method:    createIndexAllocator
 * Signature: (JJJ)J
 */
JNIEXPORT jlong JNICALL Java_org_legion_Runtime_createIndexAllocator
  (JNIEnv *env, jobject jobj, jlong jrt, jlong jctx, jlong jis)
{
  HighLevelRuntime *runtime = reinterpret_cast<HighLevelRuntime*>(jrt);
  Context ctx = reinterpret_cast<Context>(jctx);
  IndexSpace *is = reinterpret_cast<IndexSpace*>(jis);

  IndexAllocator *ia = new IndexAllocator;
  *ia = runtime->create_index_allocator(ctx, *is);

  return reinterpret_cast<jlong>(ia);
}

/*
 * Class:     org_legion_Runtime
 * Method:    createIndexPartition
 * Signature: (JJJJJZ)J
 */
JNIEXPORT jlong JNICALL Java_org_legion_Runtime_createIndexPartition
  (JNIEnv *env, jobject jobj, jlong jrt, jlong jctx, jlong jis,
   jlong jdomain, jlong jdomaincoloring, jboolean jdisjoint)
{
  HighLevelRuntime *runtime = reinterpret_cast<HighLevelRuntime*>(jrt);
  Context ctx = reinterpret_cast<Context>(jctx);
  IndexSpace *is = reinterpret_cast<IndexSpace*>(jis);
  Domain *d = reinterpret_cast<Domain*>(jdomain);
  DomainColoring *dc = reinterpret_cast<DomainColoring*>(jdomaincoloring);

  IndexPartition *ip = new IndexPartition;
  *ip = runtime->create_index_partition(ctx, *is, *d, *dc, jdisjoint);

  return reinterpret_cast<jlong>(ip);
}

/*
 * Class:     org_legion_Runtime
 * Method:    getLogicalPartition
 * Signature: (JJJJ)J
 */
JNIEXPORT jlong JNICALL Java_org_legion_Runtime_getLogicalPartition
  (JNIEnv *env, jobject jobj, jlong jrt, jlong jctx, jlong jlr, jlong jip)
{
  HighLevelRuntime *runtime = reinterpret_cast<HighLevelRuntime*>(jrt);
  Context ctx = reinterpret_cast<Context>(jctx);
  LogicalRegion *lr = reinterpret_cast<LogicalRegion*>(jlr);
  IndexPartition *ip = reinterpret_cast<IndexPartition*>(jip);

  LogicalPartition *lp = new LogicalPartition;
  *lp = runtime->get_logical_partition(ctx, *lr, *ip);

  return reinterpret_cast<jlong>(lp);
}
