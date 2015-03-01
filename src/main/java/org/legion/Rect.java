package org.legion;

/**
 *
 */
public class Rect extends LegionObject {

  /**
   *
   */
  public Rect(Point low, Point high) {
    newRect(low.nativeHandle, high.nativeHandle);
  }

  @Override
  protected void disposeInternal() {
    assert(isInitialized());
    disposeInternal(nativeHandle);
  }

  private native void disposeInternal(long handle);

  private native void newRect(long low, long high);
}
