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
 * Signature: (JJJJ)V
 */
JNIEXPORT void JNICALL Java_org_legion_RegionRequirement_newRegionRequirement__JJJJ
  (JNIEnv *env, jobject jobj, jlong jregion, jlong jpriv, jlong jcoherence, jlong jparent)
{
  RegionRequirement *req = new RegionRequirement(
      *reinterpret_cast<LogicalRegion*>(jregion),
      getPrivilegeMode(jpriv),
      getCoherenceProperty(jcoherence),
      *reinterpret_cast<LogicalRegion*>(jparent));
  RegionRequirementJni::setHandle(env, jobj, req);
}

/*
 * Class:     org_legion_RegionRequirement
 * Method:    newRegionRequirement
 * Signature: (JIJJJ)V
 */
JNIEXPORT void JNICALL Java_org_legion_RegionRequirement_newRegionRequirement__JIJJJ
  (JNIEnv *env, jobject jobj, jlong jpartition, jint jprojectionId, jlong jpriv,
   jlong jcoherence, jlong jparent)
{
  RegionRequirement *req = new RegionRequirement(
      *reinterpret_cast<LogicalPartition*>(jpartition),
      jprojectionId,
      getPrivilegeMode(jpriv),
      getCoherenceProperty(jcoherence),
      *reinterpret_cast<LogicalRegion*>(jparent));
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
  req->privilege = getPrivilegeMode(jpriv);
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
  req->prop = getCoherenceProperty(jcoherence);
}

/*
 * Class:     org_legion_RegionRequirement
 * Method:    addField
 * Signature: (JIZ)V
 */
JNIEXPORT void JNICALL Java_org_legion_RegionRequirement_addField
  (JNIEnv *env, jobject jobj, jlong jhandle, jint jfid, jboolean jinstance)
{
  RegionRequirement *req = reinterpret_cast<RegionRequirement*>(jhandle);
  req->add_field(
      static_cast<FieldID>(jfid),
      static_cast<bool>(jinstance));
}
