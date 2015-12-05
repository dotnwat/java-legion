import java.util.ArrayList;

import org.legion.Runtime;
import org.legion.TaskFunction;
import org.legion.Context;
import org.legion.Future;
import org.legion.TaskLauncher;
import org.legion.Task;
import org.legion.IndexSpace;
import org.legion.FieldSpace;
import org.legion.FieldAllocator;
import org.legion.LogicalRegion;
import org.legion.RegionRequirement;
import org.legion.PrivilegeMode;
import org.legion.CoherenceProperty;
import org.legion.InlineLauncher;
import org.legion.PhysicalRegion;

public class LegionHelloWorld {

  static final int TOP_ID   = 1;
  static final int HELLO_ID = 2;

  static class MyTask implements TaskFunction {
    public void task(Task task, ArrayList<PhysicalRegion> regions,
        Context ctx, Runtime rt) {
      System.out.println("Hello world, from sub-task");
      System.out.println(new String(task.getArgs()));

      IndexSpace is = rt.createIndexSpace(ctx, 1000);
      FieldSpace fs = rt.createFieldSpace(ctx);
      FieldAllocator fa = rt.createFieldAllocator(ctx, fs);
      fa.allocateField(1024, 1);

      LogicalRegion lr = rt.createLogicalRegion(ctx, is, fs);

      RegionRequirement req = new RegionRequirement(lr,
          PrivilegeMode.READ_WRITE, CoherenceProperty.EXCLUSIVE,
          lr);
      req.addField(1);

      InlineLauncher il = new InlineLauncher(req);

      PhysicalRegion pr = rt.mapRegion(ctx, il);
      pr.waitForValid();
    }
  }

  public static void main(String[] args) {

    Runtime.register_task(HELLO_ID, new MyTask());

    Runtime.register_task(TOP_ID, new TaskFunction() {
      public void task(Task task, ArrayList<PhysicalRegion> regions,
          Context ctx, Runtime rt) {
        TaskLauncher launcher = new TaskLauncher(HELLO_ID,
            "This is an argument".getBytes());
        Future result = rt.executeTask(ctx, launcher);
        result.waitOnComplete();
      }
    });

    Runtime.set_top_level_task_id(TOP_ID);
    Runtime.start(args);
  }
}
