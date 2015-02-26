package org.legion;

/**
 *
 */
public class IndexAllocator extends LegionObject {

  IndexAllocator(long handle) {
    nativeHandle = handle;
  }

  /**
   *
   */
  public IndexSpace getIndexSpace() {
    long is = getIndexSpace(nativeHandle);
    return new IndexSpace(is);
  }

  /**
   *
   */
  public Pointer allocate() {
    return allocate(1);
  }

  /**
   *
   */
  public Pointer allocate(int count) {
    long p = allocate(nativeHandle, count);
    return new Pointer(p);
  }

  @Override
  protected void disposeInternal() {
    assert(isInitialized());
    disposeInternal(nativeHandle);
  }

  private native void disposeInternal(long handle);
  private native long getIndexSpace(long handle);
  private native long allocate(long handle, int count);
}
