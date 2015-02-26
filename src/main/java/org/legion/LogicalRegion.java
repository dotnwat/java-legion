package org.legion;

/**
 *
 */
public class LogicalRegion extends LegionObject {
  LogicalRegion(long handle) {
    nativeHandle = handle;
  }

  public IndexSpace getIndexSpace() {
    long isHandle = getIndexSpace(nativeHandle);
    return new IndexSpace(isHandle);
  }

  public FieldSpace getFieldSpace() {
    long fsHandle = getFieldSpace(nativeHandle);
    return new FieldSpace(fsHandle);
  }

  @Override
  protected void disposeInternal() {
    assert(isInitialized());
    disposeInternal(nativeHandle);
  }

  private native void disposeInternal(long handle);
  private native long getIndexSpace(long handle);
  private native long getFieldSpace(long handle);
}
