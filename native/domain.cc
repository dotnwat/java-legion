#include <jni.h>

#include <legion.h>

#include "include/org_legion_Domain.h"
#include "portal.h"

/*
 * Class:     org_legion_Domain
 * Method:    disposeInternal
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_org_legion_Domain_disposeInternal
  (JNIEnv *env, jobject jobj, jlong jhandle)
{
  delete reinterpret_cast<Domain*>(jhandle);
  DomainJni::setHandle(env, jobj, NULL);
}

/*
 * Class:     org_legion_Domain
 * Method:    newDomainFromRect
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_org_legion_Domain_newDomainFromRect
  (JNIEnv *env, jobject jobj, jlong jrect)
{
  RectWithRank *rr = reinterpret_cast<RectWithRank*>(jrect);
  switch (rr->rank) {
    case 1:
      {
        Domain *d = new Domain;
        *d = Domain::from_rect<1>(
            reinterpret_cast<RectWithRankImpl<1>*>(jrect)->rect);
        return reinterpret_cast<jlong>(d);
      }
    case 2:
      {
        Domain *d = new Domain;
        *d = Domain::from_rect<2>(
            reinterpret_cast<RectWithRankImpl<2>*>(jrect)->rect);
        return reinterpret_cast<jlong>(d);
      }
    case 3:
      {
        Domain *d = new Domain;
        *d = Domain::from_rect<3>(
            reinterpret_cast<RectWithRankImpl<3>*>(jrect)->rect);
        return reinterpret_cast<jlong>(d);
      }
    default:
      assert(0);
  }
}
