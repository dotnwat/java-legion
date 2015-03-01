package org.legion;

public class Point extends LegionObject {
  public Point(int... coord) {
    newPoint(coord);
  }

  @Override
  protected void disposeInternal() {
    assert(isInitialized());
    disposeInternal(nativeHandle);
  }

  private native void disposeInternal(long handle);

  private native void newPoint(int[] coord);
}
