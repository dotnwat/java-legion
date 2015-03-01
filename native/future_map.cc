#include <jni.h>

#include <legion.h>

#include "include/org_legion_FutureMap.h"
#include "portal.h"

/*
 * Class:     org_legion_FutureMap
 * Method:    disposeInternal
 * Signature: (J)V
 */
void Java_org_legion_FutureMap_disposeInternal(JNIEnv *env, jobject jobj, jlong jhandle)
{
  delete reinterpret_cast<FutureMap*>(jhandle);
  FutureJni::setHandle(env, jobj, NULL);
}
