package org.legion;

public enum PrivilegeMode {
  NO_ACCESS((int)     0),
  READ_ONLY((int)     1),
  READ_WRITE((int)    2),
  WRITE_ONLY((int)    3),
  WRITE_DISCARD((int) 4),
  REDUCE((int)        5);

  private final int value;

  private PrivilegeMode(int value) {
    this.value = value;
  }

  int getValue() {
    return value;
  }
}
