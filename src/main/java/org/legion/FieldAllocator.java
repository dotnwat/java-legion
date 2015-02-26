package org.legion;

/**
 *
 */
public class FieldAllocator extends LegionObject {

  FieldAllocator(long handle) {
    nativeHandle = handle;
  }

  /**
   *
   */
  public void allocateField(int size, int fieldId) {
    allocateField(nativeHandle, size, fieldId);
  }

  @Override
  protected void disposeInternal() {
    assert(isInitialized());
    disposeInternal(nativeHandle);
  }

  private native void disposeInternal(long handle);
  private native void allocateField(long handle, int size, int fieldId);
};
