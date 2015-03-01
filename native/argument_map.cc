#include <jni.h>

#include <legion.h>

#include "include/org_legion_ArgumentMap.h"
#include "portal.h"

/*
 * Class:     org_legion_ArgumentMap
 * Method:    disposeInternal
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_org_legion_ArgumentMap_disposeInternal
  (JNIEnv *env, jobject jobj, jlong jhandle)
{
  delete reinterpret_cast<ArgumentMap*>(jhandle);
  ArgumentMapJni::setHandle(env, jobj, NULL);
}

/*
 * Class:     org_legion_ArgumentMap
 * Method:    newArgumentMap
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_legion_ArgumentMap_newArgumentMap
  (JNIEnv *env, jobject jobj)
{
  ArgumentMap *am = new ArgumentMap;
  ArgumentMapJni::setHandle(env, jobj, am);
}
