package org.legion;

public class TaskLauncher extends LegionObject {

  public TaskLauncher(int taskId, byte[] arg) {
    this(taskId);
    setTaskArg(nativeHandle, arg);
  }

  public TaskLauncher(int taskId) {
    newTaskLauncher();
    setTaskId(nativeHandle, taskId);
  }

  /**
   *
   */
  public void addRegionRequirement(RegionRequirement req) {
    addRegionRequirement(nativeHandle, req.nativeHandle);
  }

  @Override
  protected void disposeInternal() {
    assert(isInitialized());
    disposeInternal(nativeHandle);
  }

  private native void disposeInternal(long handle);

  private native void newTaskLauncher();

  /* FIXME: we shoudl be using the Legion constructors here, like in
   * IndexLauncher */
  private native void setTaskId(long handle, int taskId);
  private native void setTaskArg(long handle, byte[] arg);
  private native void addRegionRequirement(long handle, long rr);
}
