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

/*
 * Class:     org_legion_IndexAllocator
 * Method:    getIndexSpace
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_org_legion_IndexAllocator_getIndexSpace
  (JNIEnv *env, jobject jobj, jlong jhandle)
{
  IndexAllocator *ia = reinterpret_cast<IndexAllocator*>(jhandle);

  IndexSpace *is = new IndexSpace; // FIXME: common constructor?
  *is = ia->get_index_space();

  return reinterpret_cast<jlong>(is);
}

/*
 * Class:     org_legion_IndexAllocator
 * Method:    allocate
 * Signature: (JI)J
 */
JNIEXPORT jlong JNICALL Java_org_legion_IndexAllocator_allocate
  (JNIEnv *env, jobject jobj, jlong jhandle, jint jcount)
{
  IndexAllocator *ia = reinterpret_cast<IndexAllocator*>(jhandle);

  ptr_t *p = new ptr_t;
  *p = ia->alloc(static_cast<jint>(jcount));

  return reinterpret_cast<jlong>(p);
}
