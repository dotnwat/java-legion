package org.legion;

/**
 *
 */
public class DomainColoring extends LegionObject {
  public DomainColoring() {
    newDomainColoring();
  }

  /**
   *
   */
  public void setColor(int color, Domain domain) {
    setColor(nativeHandle, color, domain.nativeHandle);
  }

  @Override protected void disposeInternal() {
    assert(isInitialized());
    disposeInternal(nativeHandle);
  }

  private native void disposeInternal(long handle);
  private native void newDomainColoring();
  private native void setColor(long handle, int color, long domain);
}
