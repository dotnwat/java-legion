package org.legion;

/**
 *
 */
public class ArgumentMap extends LegionObject {

  /**
   *
   */
  public ArgumentMap() {
    newArgumentMap();
  }

  @Override
  protected void disposeInternal() {
    assert(isInitialized());
    disposeInternal(nativeHandle);
  }

  private native void disposeInternal(long handle);
  private native void newArgumentMap();
}
