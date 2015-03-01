#include <jni.h>

#include <legion.h>

#include "include/org_legion_Point.h"
#include "portal.h"

/*
 * Class:     org_legion_Point
 * Method:    disposeInternal
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_org_legion_Point_disposeInternal
  (JNIEnv *env, jobject jobj, jlong jhandle)
{
  PointWithRank *pp = reinterpret_cast<PointWithRank*>(jhandle);
  switch (pp->rank) {
    case 1:
      delete reinterpret_cast<PointWithRankImpl<1>*>(jhandle);
      break;
    case 2:
      delete reinterpret_cast<PointWithRankImpl<2>*>(jhandle);
      break;
    case 3:
      delete reinterpret_cast<PointWithRankImpl<3>*>(jhandle);
      break;
    default:
      assert(0);
  }
}

/*
 * Class:     org_legion_Point
 * Method:    newPoint
 * Signature: ([I)V
 */
JNIEXPORT void JNICALL Java_org_legion_Point_newPoint
  (JNIEnv *env, jobject jobj, jintArray jcoord)
{
  jsize len = env->GetArrayLength(jcoord);
  jint *coord = env->GetIntArrayElements(jcoord, 0);

  switch (len) {
    case 1:
    {
      PointWithRankImpl<1> *p = new PointWithRankImpl<1>(coord);
      PointJni::setHandle(env, jobj, p);
      break;
    }
    case 2:
    {
      PointWithRankImpl<2> *p = new PointWithRankImpl<2>(coord);
      PointJni::setHandle(env, jobj, p);
      break;
    }
    case 3:
    {
      PointWithRankImpl<3> *p = new PointWithRankImpl<3>(coord);
      PointJni::setHandle(env, jobj, p);
      break;
    }
    default:
      assert(0);
  }

  env->ReleaseIntArrayElements(jcoord, coord, 0);
}
