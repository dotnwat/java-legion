#include <jni.h>

#include <legion.h>

#include "include/org_legion_IndexAllocator.h"
#include "portal.h"

/*
 * Class:     org_legion_IndexAllocator
 * Method:    disposeInternal
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_org_legion_IndexAllocator_disposeInternal
  (JNIEnv *env, jobject jobj, jlong jhandle)
{
  delete reinterpret_cast<IndexAllocator*>(jhandle);
  IndexAllocatorJni::setHandle(env, jobj, NULL);
}
