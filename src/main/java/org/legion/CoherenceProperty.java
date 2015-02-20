package org.legion;

public enum CoherenceProperty {
  EXCLUSIVE((int)    0),
  ATOMIC((int)       1),
  SIMULTANEOUS((int) 2),
  RELAXED((int)      3);

  private final int value;

  private CoherenceProperty(int value) {
    this.value = value;
  }

  int getValue() {
    return value;
  }
}
