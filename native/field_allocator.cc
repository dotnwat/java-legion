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

/*
 * Class:     org_legion_FieldAllocator
 * Method:    freeField
 * Signature: (JI)V
 */
JNIEXPORT void JNICALL Java_org_legion_FieldAllocator_freeField
  (JNIEnv *env, jobject jobj, jlong jhandle, jint jfieldId)
{
  FieldAllocator *fa = reinterpret_cast<FieldAllocator*>(jhandle);
  fa->free_field(static_cast<int>(jfieldId));
}

/*
 * Class:     org_legion_FieldAllocator
 * Method:    getFieldSpace
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_org_legion_FieldAllocator_getFieldSpace
  (JNIEnv *env, jobject jobj, jlong jhandle)
{
  FieldAllocator *fa = reinterpret_cast<FieldAllocator*>(jhandle);

  FieldSpace *fs = new FieldSpace;
  *fs = fa->get_field_space();

  return reinterpret_cast<jlong>(fs);
}
