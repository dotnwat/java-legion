#include <jni.h>

#include <legion.h>

#include "include/org_legion_RegionRequirement.h"
#include "portal.h"

using namespace LegionRuntime::HighLevel;

/*
 * Class:     org_legion_RegionRequirement
 * Method:    disposeInternal
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_org_legion_RegionRequirement_disposeInternal
  (JNIEnv *env, jobject jobj, jlong jhandle)
{
  delete reinterpret_cast<RegionRequirement*>(jhandle);
  RegionRequirementJni::setHandle(env, jobj, NULL);
}

/*
 * Class:     org_legion_RegionRequirement
 * Method:    newRegionRequirement
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_legion_RegionRequirement_newRegionRequirement
  (JNIEnv *env, jobject jobj)
{
  RegionRequirement *req = new RegionRequirement;
  RegionRequirementJni::setHandle(env, jobj, req);
}

/*
 * Class:     org_legion_RegionRequirement
 * Method:    setRegion
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_org_legion_RegionRequirement_setRegion
  (JNIEnv *env, jobject jobj, jlong jhandle, jlong jregion)
{
  RegionRequirement *req = reinterpret_cast<RegionRequirement*>(jhandle);
  LogicalRegion *region = reinterpret_cast<LogicalRegion*>(jregion);
  req->region = *region;
}

/*
 * Class:     org_legion_RegionRequirement
 * Method:    setParent
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_org_legion_RegionRequirement_setParent
  (JNIEnv *env, jobject jobj, jlong jhandle, jlong jparent)
{
  RegionRequirement *req = reinterpret_cast<RegionRequirement*>(jhandle);
  LogicalRegion *parent = reinterpret_cast<LogicalRegion*>(jparent);
  req->parent = *parent;
}

/*
 * Class:     org_legion_RegionRequirement
 * Method:    setPrivilege
 * Signature: (JI)V
 */
JNIEXPORT void JNICALL Java_org_legion_RegionRequirement_setPrivilege
  (JNIEnv *env, jobject jobj, jlong jhandle, jint jpriv)
{
  RegionRequirement *req = reinterpret_cast<RegionRequirement*>(jhandle);
  switch (static_cast<int>(jpriv)) {
    case 0:
      req->privilege = NO_ACCESS;
      break;
    case 1:
      req->privilege = READ_ONLY;
      break;
    case 2:
      req->privilege = READ_WRITE;
      break;
    case 3:
      req->privilege = WRITE_ONLY;
      break;
    case 4:
      req->privilege = WRITE_DISCARD;
      break;
    case 5:
      req->privilege = REDUCE;
      break;
    default:
      assert(0);
  }
}

/*
 * Class:     org_legion_RegionRequirement
 * Method:    setCoherence
 * Signature: (JI)V
 */
JNIEXPORT void JNICALL Java_org_legion_RegionRequirement_setCoherence
  (JNIEnv *env, jobject jobj, jlong jhandle, jint jcoherence)
{
  RegionRequirement *req = reinterpret_cast<RegionRequirement*>(jhandle);
  switch (static_cast<int>(jcoherence)) {
    case 0:
      req->prop = EXCLUSIVE;
      break;
    case 1:
      req->prop = ATOMIC;
      break;
    case 2:
      req->prop = SIMULTANEOUS;
      break;
    case 3:
      req->prop = RELAXED;
      break;
    default:
      assert(0);
  }
}
