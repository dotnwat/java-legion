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
    // FIXME: return field id?
  }

  /**
   *
   */
  public void freeField(int fieldId) {
    freeField(nativeHandle, fieldId);
  }

  /**
   *
   */
  public FieldSpace getFieldSpace() {
    long fs = getFieldSpace(nativeHandle);
    return new FieldSpace(fs);
  }

  @Override
  protected void disposeInternal() {
    assert(isInitialized());
    disposeInternal(nativeHandle);
  }

  private native void disposeInternal(long handle);
  private native void allocateField(long handle, int size, int fieldId);
  private native void freeField(long fa, int fieldId);
  private native long getFieldSpace(long fa);
};
