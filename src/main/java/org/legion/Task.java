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

  private native byte[] getArgs(long handle);
}
