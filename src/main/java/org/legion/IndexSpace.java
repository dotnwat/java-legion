package org.legion;

/**
 *
 */
public class IndexSpace extends LegionObject {
  IndexSpace(long handle) {
    nativeHandle = handle;
  }

  @Override
  protected void disposeInternal() {
    assert(isInitialized());
    disposeInternal(nativeHandle);
  }

  private native void disposeInternal(long handle);
};
