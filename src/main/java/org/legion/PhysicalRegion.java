package org.legion;

/**
 *
 */
public class PhysicalRegion {
  long handle;

  PhysicalRegion(long handle) {
    this.handle = handle;
  }

  public void waitForValid() {
    waitForValid(handle);
  }

  public LogicalRegion getLogicalRegion() {
    long lrHandle = getLogicalRegion(handle);
    return new LogicalRegion(lrHandle);
  }

  private native void waitForValid(long handle);
  private native long getLogicalRegion(long handle);
}
