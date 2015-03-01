package org.legion;

/**
 *
 */
public class IndexLauncher extends LegionObject {

  /**
   *
   */
  public IndexLauncher(int taskId, Domain domain, byte[] global_arg,
      ArgumentMap arg_map) {
    newIndexLauncher(taskId, domain.nativeHandle, global_arg,
        arg_map.nativeHandle);
  }

  /**
   *
   */
  public void addRegionRequirement(RegionRequirement req) {
    addRegionRequirement(nativeHandle, req.nativeHandle);
  }

  @Override
  protected void disposeInternal() {
    assert(isInitialized());
    disposeInternal(nativeHandle);
  }

  private native void disposeInternal(long handle);
  private native void newIndexLauncher(int taskId, long domain,
      byte[] global_arg, long arg_map);
  private native void addRegionRequirement(long handle, long rr);
}
