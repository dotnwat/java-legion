#include <jni.h>

#include <legion.h>

#include "include/org_legion_PhysicalRegion.h"
#include "portal.h"

/*
 * Class:     org_legion_PhysicalRegion
 * Method:    waitForValid
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_org_legion_PhysicalRegion_waitForValid
  (JNIEnv *env, jobject jobj, jlong jhandle)
{
  PhysicalRegion *pr = reinterpret_cast<PhysicalRegion*>(jhandle);
  pr->wait_until_valid();
}

/*
 * Class:     org_legion_PhysicalRegion
 * Method:    getLogicalRegion
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_org_legion_PhysicalRegion_getLogicalRegion
  (JNIEnv *env, jobject jobj, jlong jhandle)
{
  PhysicalRegion *pr = reinterpret_cast<PhysicalRegion*>(jhandle);

  LogicalRegion *lr = new LogicalRegion;
  *lr = pr->get_logical_region();

  return reinterpret_cast<jlong>(lr);
}
