package org.legion;

public class Future extends LegionObject {

  Future(long handle) {
    nativeHandle = handle;
  }

  @Override
  protected void disposeInternal() {
    assert(isInitialized());
    disposeInternal(nativeHandle);
  }

  public void waitOnComplete() {
    waitOnComplete(nativeHandle);
  }

  // ???
  private native void disposeInternal(long handle);
  private native void waitOnComplete(long handle);
}
