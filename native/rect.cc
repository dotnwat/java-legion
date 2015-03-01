#include <jni.h>

#include <legion.h>

#include "include/org_legion_Rect.h"
#include "portal.h"

/*
 * Class:     org_legion_Rect
 * Method:    disposeInternal
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_org_legion_Rect_disposeInternal
  (JNIEnv *env, jobject jobj, jlong jhandle)
{
  RectWithRank *rr = reinterpret_cast<RectWithRank*>(jhandle);
  switch (rr->rank) {
    case 1:
      delete reinterpret_cast<RectWithRankImpl<1>*>(jhandle);
      break;
    case 2:
      delete reinterpret_cast<RectWithRankImpl<2>*>(jhandle);
      break;
    case 3:
      delete reinterpret_cast<RectWithRankImpl<3>*>(jhandle);
      break;
    default:
      assert(0);
  }
}

/*
 * Class:     org_legion_Rect
 * Method:    newRect
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_org_legion_Rect_newRect
  (JNIEnv *env, jobject jobj, jlong jlow, jlong jhigh)
{
  PointWithRank *prlow = reinterpret_cast<PointWithRank*>(jlow);
  PointWithRank *prhigh = reinterpret_cast<PointWithRank*>(jhigh);
  assert(prlow->rank == prhigh->rank);

  switch (prlow->rank) {
    case 1:
    {
      RectWithRankImpl<1> *r = new RectWithRankImpl<1>(
          reinterpret_cast<PointWithRankImpl<1>*>(jlow)->point,
          reinterpret_cast<PointWithRankImpl<1>*>(jhigh)->point);
      RectJni::setHandle(env, jobj, r);
      break;
    }
    case 2:
    {
      RectWithRankImpl<2> *r = new RectWithRankImpl<2>(
          reinterpret_cast<PointWithRankImpl<2>*>(jlow)->point,
          reinterpret_cast<PointWithRankImpl<2>*>(jhigh)->point);
      RectJni::setHandle(env, jobj, r);
      break;
    }
    case 3:
    {
      RectWithRankImpl<3> *r = new RectWithRankImpl<3>(
          reinterpret_cast<PointWithRankImpl<3>*>(jlow)->point,
          reinterpret_cast<PointWithRankImpl<3>*>(jhigh)->point);
      RectJni::setHandle(env, jobj, r);
      break;
    }
    default:
      assert(0);
  }
}
