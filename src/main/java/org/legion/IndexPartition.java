
package org.legion;

/**
 *
 */
public class IndexPartition extends LegionObject {

  IndexPartition(long handle) {
    nativeHandle = handle;
  }

  @Override
  protected void disposeInternal() {
    assert(isInitialized());
    disposeInternal(nativeHandle);
  }

  private native void disposeInternal(long handle);
}
