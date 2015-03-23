package org.legion;

public abstract class LegionObject {

  protected long nativeHandle;
  protected boolean owningHandle;

  protected LegionObject() {
    nativeHandle = 0;
    owningHandle = true;
  }

  public final synchronized void dispose() {
    if (owningHandle && isInitialized())
      disposeInternal();
    nativeHandle = 0;
    owningHandle = false;
  }

  protected abstract void disposeInternal();

  protected boolean isInitialized() {
    return nativeHandle != 0;
  }

  @Override
  protected void finalize() throws Throwable {
    dispose();
    super.finalize();
  }

}
