package org.legion;

import java.util.ArrayList;

public interface TaskFunction {
  public void task(final Task task, final ArrayList<PhysicalRegion> regions,
      final Context ctx, final Runtime rt);
}
