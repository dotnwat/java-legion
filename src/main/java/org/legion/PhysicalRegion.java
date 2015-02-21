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

  private native void waitForValid(long handle);
}
