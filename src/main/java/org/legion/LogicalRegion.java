package org.legion;

/**
 *
 */
public class LogicalRegion extends LegionObject {
  LogicalRegion(long handle) {
    nativeHandle = handle;
  }

  @Override
  protected void disposeInternal() {
    assert(isInitialized());
    disposeInternal(nativeHandle);
  }

  private native void disposeInternal(long handle);
}
