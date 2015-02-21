package org.legion;

/**
 *
 */
public class RegionRequirement extends LegionObject {

  /**
   *
   */
  public RegionRequirement(final LogicalRegion region,
      final PrivilegeMode priv, final CoherenceProperty coherence,
      final LogicalRegion parent) {
    newRegionRequirement(region.nativeHandle, priv.getValue(),
        coherence.getValue(), parent.nativeHandle);
  }

  /**
   *
   */
  public void addField(int fid) {
    addField(fid, true);
  }

  /**
   *
   */
  public void addField(int fid, boolean instance) {
    addField(nativeHandle, fid, instance);
  }

  @Override
  protected void disposeInternal() {
    assert(isInitialized());
    disposeInternal(nativeHandle);
  }

  private native void disposeInternal(long handle);

  private native void newRegionRequirement(long region, long priv,
      long coherence, long parent);

  private native void setRegion(long handle, long regionHandle);
  private native void setParent(long handle, long regionHandle);
  private native void setPrivilege(long handle, int priv);
  private native void setCoherence(long handle, int coherence);
  private native void addField(long handle, int fid, boolean instance);
}
