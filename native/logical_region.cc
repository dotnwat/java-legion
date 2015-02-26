#include <jni.h>

#include <legion.h>

#include "include/org_legion_LogicalRegion.h"
#include "portal.h"

/*
 * Class:     org_legion_LogicalRegion
 * Method:    disposeInternal
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_org_legion_LogicalRegion_disposeInternal
  (JNIEnv *env, jobject jobj, jlong jhandle)
{
  delete reinterpret_cast<LogicalRegion*>(jhandle);
  LogicalRegionJni::setHandle(env, jobj, NULL);
}
