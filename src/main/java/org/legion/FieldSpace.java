package org.legion;

/**
 *
 */
public class FieldSpace extends LegionObject {
  FieldSpace(long handle) {
    nativeHandle = handle;
  }

  @Override
  protected void disposeInternal() {
    assert(isInitialized());
    disposeInternal(nativeHandle);
  }

  private native void disposeInternal(long handle);
};
