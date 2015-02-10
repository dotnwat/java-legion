package org.legion;

public class TaskLauncher extends LegionObject {

  public TaskLauncher(int taskId) {
    newTaskLauncher();
    setTaskId(nativeHandle, taskId);
  }

  @Override
  protected void disposeInternal() {
    assert(isInitialized());
    disposeInternal(nativeHandle);
  }

  private native void disposeInternal(long handle);

  private native void newTaskLauncher();

  private native void setTaskId(long handle, int taskId);
}
