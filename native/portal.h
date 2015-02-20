#ifndef PORTAL_H
#define PORTAL_H

#include <legion.h>
using namespace LegionRuntime::HighLevel;

enum TaskID {
  TASK_WRAPPER_ID,
  TOP_LEVEL_TASK_WRAPPER_ID,
};



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

class IndexSpaceJni : public LegionNativeClass<IndexSpace*, IndexSpaceJni> {
 public:
  static jclass getJClass(JNIEnv *env) {
    return LegionNativeClass::getJClass(env, "org/legion/IndexSpace");
  }
};

class FieldSpaceJni : public LegionNativeClass<FieldSpace*, FieldSpaceJni> {
 public:
  static jclass getJClass(JNIEnv *env) {
    return LegionNativeClass::getJClass(env, "org/legion/FieldSpace");
  }
};

class FieldAllocatorJni : public LegionNativeClass<FieldAllocator*, FieldAllocatorJni> {
 public:
  static jclass getJClass(JNIEnv *env) {
    return LegionNativeClass::getJClass(env, "org/legion/FieldAllocator");
  }
};

class LogicalRegionJni : public LegionNativeClass<LogicalRegion*, LogicalRegionJni> {
 public:
  static jclass getJClass(JNIEnv *env) {
    return LegionNativeClass::getJClass(env, "org/legion/LogicalRegion");
  }
};

class RegionRequirementJni : public LegionNativeClass<RegionRequirement*, RegionRequirementJni> {
 public:
  static jclass getJClass(JNIEnv *env) {
    return LegionNativeClass::getJClass(env, "org/legion/RegionRequirement");
  }
};

#endif
