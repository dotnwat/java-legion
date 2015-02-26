#include <jni.h>

#include <legion.h>

#include "include/org_legion_FieldAllocator.h"
#include "portal.h"

/*
 * Class:     org_legion_FieldAllocator
 * Method:    disposeInternal
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_org_legion_FieldAllocator_disposeInternal
  (JNIEnv *env, jobject jobj, jlong jhandle)
{
  delete reinterpret_cast<FieldAllocator*>(jhandle);
  FieldAllocatorJni::setHandle(env, jobj, NULL);
}

/*
 * Class:     org_legion_FieldAllocator
 * Method:    allocateField
 * Signature: (JII)V
 */
JNIEXPORT void JNICALL Java_org_legion_FieldAllocator_allocateField
  (JNIEnv *env, jobject jobj, jlong jhandle, jint jsize, jint jfieldId)
{
  FieldAllocator *fa = reinterpret_cast<FieldAllocator*>(jhandle);
  fa->allocate_field((size_t)jsize, (int)jfieldId);
}
