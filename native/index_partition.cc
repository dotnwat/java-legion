#include <jni.h>

#include <legion.h>

#include "include/org_legion_IndexPartition.h"
#include "portal.h"

/*
 * Class:     org_legion_IndexPartition
 * Method:    disposeInternal
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_org_legion_IndexPartition_disposeInternal
  (JNIEnv *env, jobject jobj, jlong jhandle)
{
  delete reinterpret_cast<IndexPartition*>(jhandle);
  IndexPartitionJni::setHandle(env, jobj, NULL);
}
