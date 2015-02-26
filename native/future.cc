#include <jni.h>

#include <legion.h>

#include "include/org_legion_Future.h"
#include "portal.h"

/*
 * Class:     org_legion_Future
 * Method:    disposeInternal
 * Signature: (J)V
 */
void Java_org_legion_Future_disposeInternal(JNIEnv *env, jobject jobj, jlong jhandle)
{
  delete reinterpret_cast<Future*>(jhandle);
  FutureJni::setHandle(env, jobj, NULL);
}

/*
 * Class:     org_legion_Future
 * Method:    waitOnComplete
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_org_legion_Future_waitOnComplete(JNIEnv *env, jobject jobj, jlong jhandle)
{
  Future *future = reinterpret_cast<Future*>(jhandle);
  future->get_void_result();
}
