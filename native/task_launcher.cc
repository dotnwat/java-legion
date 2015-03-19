#include <jni.h>

#include <legion.h>

#include "include/org_legion_TaskLauncher.h"
#include "portal.h"

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

  assert(jarg); // FIXME: set size/data=0/null and check during arg packaging
  jsize nelms = env->GetArrayLength(jarg);
  l->arg_data = new char[nelms]; // FIXME; memory leak never freed
  jbyte *data = env->GetByteArrayElements(jarg, NULL);
  memcpy(l->arg_data, (char*)data, static_cast<size_t>(nelms));
  l->arg_size = static_cast<size_t>(nelms);
  env->ReleaseByteArrayElements(jarg, data, JNI_ABORT);
}

/*
 * Class:     org_legion_TaskLauncher
 * Method:    addRegionRequirement
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_org_legion_TaskLauncher_addRegionRequirement
  (JNIEnv *env, jobject jobj, jlong jhandle, jlong jrr)
{
  TaskLauncherWrapper *tl = reinterpret_cast<TaskLauncherWrapper*>(jhandle);
  RegionRequirement *rr = reinterpret_cast<RegionRequirement*>(jrr);
  tl->launcher.add_region_requirement(*rr);
}
