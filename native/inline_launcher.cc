#include <jni.h>

#include <legion.h>

#include "include/org_legion_InlineLauncher.h"
#include "portal.h"

/*
 * Class:     org_legion_InlineLauncher
 * Method:    disposeInternal
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_org_legion_InlineLauncher_disposeInternal
  (JNIEnv *env, jobject jobj, jlong jhandle)
{
  delete reinterpret_cast<InlineLauncher*>(jhandle);
  InlineLauncherJni::setHandle(env, jobj, NULL);
}

/*
 * Class:     org_legion_InlineLauncher
 * Method:    newInlineLauncher
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_legion_InlineLauncher_newInlineLauncher
  (JNIEnv *env, jobject jobj)
{
  InlineLauncher *il = new InlineLauncher;
  InlineLauncherJni::setHandle(env, jobj, il);
}

/*
 * Class:     org_legion_InlineLauncher
 * Method:    setRegionRequirement
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_org_legion_InlineLauncher_setRegionRequirement
  (JNIEnv *env, jobject jobj, jlong jhandle, jlong jreq)
{
  InlineLauncher *il = reinterpret_cast<InlineLauncher*>(jhandle);
  RegionRequirement *req = reinterpret_cast<RegionRequirement*>(jreq);
  il->requirement = *req;
}

