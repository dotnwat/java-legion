package org.legion;

public interface TaskFunction {
  public void task(final Task task, final Context ctx, final Runtime rt);
}
