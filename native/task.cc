#include <jni.h>

#include <legion.h>

#include "include/org_legion_Task.h"
#include "portal.h"

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
 * Class:     org_legion_Task
 * Method:    getRegionRequirement
 * Signature: (JI)J
 */
JNIEXPORT jlong JNICALL Java_org_legion_Task_getRegionRequirement
  (JNIEnv *env, jobject jobj, jlong jhandle, jint jindex)
{
  const Task *task = reinterpret_cast<const Task*>(jhandle);
  return reinterpret_cast<jlong>(&task->regions[jindex]);
}
