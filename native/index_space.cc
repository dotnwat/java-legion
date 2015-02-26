#include <jni.h>

#include <legion.h>

#include "include/org_legion_IndexSpace.h"
#include "portal.h"

/*
 * Class:     org_legion_IndexSpace
 * Method:    disposeInternal
 * Signature: (J)V
 */
void Java_org_legion_IndexSpace_disposeInternal(JNIEnv *env,
    jobject jobj, jlong jhandle)
{
  delete reinterpret_cast<IndexSpace*>(jhandle);
  IndexSpaceJni::setHandle(env, jobj, NULL);
}
