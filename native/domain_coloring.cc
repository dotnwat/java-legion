#include <jni.h>

#include <legion.h>

#include "include/org_legion_DomainColoring.h"
#include "portal.h"

/*
 * Class:     org_legion_DomainColoring
 * Method:    disposeInternal
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_org_legion_DomainColoring_disposeInternal
  (JNIEnv *env, jobject jobj, jlong jhandle)
{
  delete reinterpret_cast<DomainColoring*>(jhandle);
  DomainColoringJni::setHandle(env, jobj, NULL);
}

/*
 * Class:     org_legion_DomainColoring
 * Method:    newDomainColoring
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_legion_DomainColoring_newDomainColoring
  (JNIEnv *env, jobject jobj)
{
  DomainColoring *dc = new DomainColoring;
  DomainColoringJni::setHandle(env, jobj, dc);
}

/*
 * Class:     org_legion_DomainColoring
 * Method:    setColor
 * Signature: (JIJ)V
 */
JNIEXPORT void JNICALL Java_org_legion_DomainColoring_setColor
  (JNIEnv *env, jobject jobj, jlong jdc, jint jcolor, jlong jdomain)
{
  DomainColoring *dc = reinterpret_cast<DomainColoring*>(jdc);
  Domain *d = reinterpret_cast<Domain*>(jdomain);
  (*dc)[static_cast<int>(jcolor)] = *d;
}
