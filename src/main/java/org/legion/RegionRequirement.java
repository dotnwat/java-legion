package org.legion;

/**
 *
 */
public class RegionRequirement extends LegionObject {

  public RegionRequirement(final LogicalRegion region,
      final PrivilegeMode priv, final CoherenceProperty coherence,
      final LogicalRegion parent) {
    newRegionRequirement();
    setRegion(nativeHandle, region.nativeHandle);
    setPrivilege(nativeHandle, priv.getValue());
    setCoherence(nativeHandle, coherence.getValue());
    setParent(nativeHandle, parent.nativeHandle);
  }

  @Override
  protected void disposeInternal() {
    assert(isInitialized());
    disposeInternal(nativeHandle);
  }

  private native void disposeInternal(long handle);

  private native void newRegionRequirement();

  private native void setRegion(long handle, long regionHandle);
  private native void setParent(long handle, long regionHandle);
  private native void setPrivilege(long handle, int priv);
  private native void setCoherence(long handle, int coherence);
}
