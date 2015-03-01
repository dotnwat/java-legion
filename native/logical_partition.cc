#include <jni.h>

#include <legion.h>

#include "include/org_legion_LogicalPartition.h"
#include "portal.h"

/*
 * Class:     org_legion_LogicalPartition
 * Method:    disposeInternal
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_org_legion_LogicalPartition_disposeInternal
  (JNIEnv *env, jobject jobj, jlong jhandle)
{
  delete reinterpret_cast<LogicalPartition*>(jhandle);
  LogicalPartitionJni::setHandle(env, jobj, NULL);
}
