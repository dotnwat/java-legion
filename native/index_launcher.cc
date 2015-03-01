#include <jni.h>

#include <legion.h>

#include "include/org_legion_IndexLauncher.h"
#include "portal.h"

/*
 * Class:     org_legion_IndexLauncher
 * Method:    disposeInternal
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_org_legion_IndexLauncher_disposeInternal
  (JNIEnv *env, jobject jobj, jlong jhandle)
{
  delete reinterpret_cast<IndexLauncher*>(jhandle);
  IndexLauncherJni::setHandle(env, jobj, NULL);
}

/*
 * Class:     org_legion_IndexLauncher
 * Method:    newIndexLauncher
 * Signature: (IJ[BJ)V
 */
JNIEXPORT void JNICALL Java_org_legion_IndexLauncher_newIndexLauncher
  (JNIEnv *env, jobject jobj, jint jtaskId, jlong jdomain,
   jbyteArray jglobal_arg, jlong jarg_map)
{
  Domain *domain = reinterpret_cast<Domain*>(jdomain);
  ArgumentMap *arg_map = reinterpret_cast<ArgumentMap*>(jarg_map);

  IndexLauncherWrapper *l = new IndexLauncherWrapper;

  if (jglobal_arg) {
    jsize nelms = env->GetArrayLength(jglobal_arg);
    l->arg_data = new char[nelms]; // FIXME; memory leak never freed
    jbyte *data = env->GetByteArrayElements(jglobal_arg, NULL);
    memcpy(l->arg_data, (char*)data, static_cast<size_t>(nelms));
    l->arg_size = static_cast<size_t>(nelms);
    env->ReleaseByteArrayElements(jglobal_arg, data, JNI_ABORT);
  } else {
    l->arg_data = NULL;
    l->arg_size = 0;
  }

  // want to use the c++ constructors in legion.h so that we get all the
  // variable initialization combinations
  IndexLauncher il(TASK_WRAPPER_ID, *domain,
      TaskArgument(NULL, 0), // overwritten during launch
      *arg_map);
  l->launcher = il; // FIXME: make this setup smoother
  l->task_id = jtaskId;

  IndexLauncherJni::setHandle(env, jobj, l);
}

/*
 * Class:     org_legion_IndexLauncher
 * Method:    addRegionRequirement
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_org_legion_IndexLauncher_addRegionRequirement
  (JNIEnv *env, jobject jobj, jlong jhandle, jlong jrr)
{
  IndexLauncherWrapper *il = reinterpret_cast<IndexLauncherWrapper*>(jhandle);
  RegionRequirement *rr = reinterpret_cast<RegionRequirement*>(jrr);
  il->launcher.add_region_requirement(*rr);
}
