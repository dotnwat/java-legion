package org.legion;

public class FutureMap extends LegionObject {

  FutureMap(long handle) {
    nativeHandle = handle;
  }

  @Override
  protected void disposeInternal() {
    assert(isInitialized());
    disposeInternal(nativeHandle);
  }

  // ???
  private native void disposeInternal(long handle);
}
