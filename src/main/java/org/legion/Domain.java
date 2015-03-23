package org.legion;

public class Domain extends LegionObject {

  Domain(long handle) {
    nativeHandle = handle;
  }

  public Domain(Rect rect) {
    long dHandle = newDomainFromRect(rect.nativeHandle);
    nativeHandle = dHandle;
  }

  @Override
  protected void disposeInternal() {
    assert(isInitialized());
    disposeInternal(nativeHandle);
  }

  private native void disposeInternal(long handle);
  private native long newDomainFromRect(long rect);
}
