#include <jni.h>

#include <legion.h>

#include "include/org_legion_FieldSpace.h"
#include "portal.h"

/*
 * Class:     org_legion_FieldSpace
 * Method:    disposeInternal
 * Signature: (J)V
 */
void Java_org_legion_FieldSpace_disposeInternal(JNIEnv *env,
    jobject jobj, jlong jhandle)
{
  delete reinterpret_cast<FieldSpace*>(jhandle);
  FieldSpaceJni::setHandle(env, jobj, NULL);
}
