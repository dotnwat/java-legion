package org.legion;

/**
 *
 */
public class InlineLauncher extends LegionObject {

  /**
   *
   */
  public InlineLauncher(RegionRequirement req) {
    newInlineLauncher();
    setRegionRequirement(req);
  }

  /**
   *
   */
  public void setRegionRequirement(RegionRequirement req) {
    setRegionRequirement(nativeHandle, req.nativeHandle);
  }

  @Override
  protected void disposeInternal() {
    assert(isInitialized());
    disposeInternal(nativeHandle);
  }

  private native void disposeInternal(long handle);

  private native void newInlineLauncher();
  private native void setRegionRequirement(long handle, long reqHandle);
}
