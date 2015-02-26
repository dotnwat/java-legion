package org.legion;

/**
 *
 */
public class IndexAllocator extends LegionObject {

  IndexAllocator(long handle) {
    nativeHandle = handle;
  }


  @Override
  protected void disposeInternal() {
    assert(isInitialized());
    disposeInternal(nativeHandle);
  }

  private native void disposeInternal(long handle);
}
