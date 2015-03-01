package org.legion;

/**
 *
 */
public class LogicalPartition extends LegionObject {
  LogicalPartition(long handle) {
    nativeHandle = handle;
  }

  @Override
  protected void disposeInternal() {
    assert(isInitialized());
    disposeInternal(nativeHandle);
  }

  private native void disposeInternal(long handle);
}
