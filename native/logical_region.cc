#include <jni.h>

#include <legion.h>

#include "include/org_legion_LogicalRegion.h"
#include "portal.h"

/*
 * Class:     org_legion_LogicalRegion
 * Method:    disposeInternal
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_org_legion_LogicalRegion_disposeInternal
  (JNIEnv *env, jobject jobj, jlong jhandle)
{
  delete reinterpret_cast<LogicalRegion*>(jhandle);
  LogicalRegionJni::setHandle(env, jobj, NULL);
}

/*
 * Class:     org_legion_LogicalRegion
 * Method:    getIndexSpace
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_org_legion_LogicalRegion_getIndexSpace
  (JNIEnv *env, jobject jobj, jlong jhandle)
{
  LogicalRegion *lr = reinterpret_cast<LogicalRegion*>(jhandle);

  IndexSpace *is = new IndexSpace;
  *is = lr->get_index_space();

  return reinterpret_cast<jlong>(is);
}

/*
 * Class:     org_legion_LogicalRegion
 * Method:    getFieldSpace
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_org_legion_LogicalRegion_getFieldSpace
  (JNIEnv *env, jobject jobj, jlong jhandle)
{
  LogicalRegion *lr = reinterpret_cast<LogicalRegion*>(jhandle);

  FieldSpace *fs = new FieldSpace;
  *fs = lr->get_field_space();

  return reinterpret_cast<jlong>(fs);
}
