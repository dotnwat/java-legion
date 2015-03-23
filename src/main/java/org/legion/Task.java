package org.legion;

/**
 *
 */
public class Task {
  private long handle;

  Task(long handle) {
    this.handle = handle;
  }

  public byte[] getArgs() {
    return getArgs(handle);
  }

  /*
   *
   */
  public RegionRequirement getRegionRequirement(int index) {
    long rrHandle = getRegionRequirement(handle, index);
    return new RegionRequirement(rrHandle);
  }

  private native byte[] getArgs(long handle);
  private native long getRegionRequirement(long handle, int index);
}
