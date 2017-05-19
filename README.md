# java-legion - Java bindings to Legion

# Building

```bash
git submodule update --init --recursive
cmake .
make
```

# Examples

* [Hello World](#hello-world)
* [DAXBY](#daxby)

## Hello World

This is your basic *hello world* example. It prints out the iconic message in a sub-task launched from the top-level task.

```java
import org.legion.Runtime;
import org.legion.TaskFunction;
import org.legion.Context;
import org.legion.Future;
import org.legion.TaskLauncher;
import org.legion.Task;

public class LegionHelloWorld {

  static final int TOP_ID   = 1;
  static final int HELLO_ID = 2;

  static class MyTask implements TaskFunction {
    public void task(Task task, Context ctx, Runtime rt) {
      System.out.println("Hello world, I am Legion.");
    }
  }

  public static void main(String[] args) {

    Runtime.register_task(HELLO_ID, new MyTask());

    Runtime.register_task(TOP_ID, new TaskFunction() {
      public void task(Task task, Context ctx, Runtime rt) {
        TaskLauncher launcher = new TaskLauncher(HELLO_ID);
        Future result = rt.executeTask(ctx, launcher);
        result.waitOnComplete();
      }
    });

    Runtime.set_top_level_task_id(TOP_ID);
    Runtime.start(args);
  }
}
```

Everything is packaged up nicely. Just run it from the command line:

```bash
java -cp legionjni-linux64.jar:. -Xcheck:jni LegionHelloWorld
Hello world, I am Legion.
```

## DAXBY

http://legion.stanford.edu/tutorial/physical_regions.html

```java
import org.legion.Runtime;
import org.legion.*;

public class DaxbyExample {

  static final int TOP_TASK_ID   = 1;
  static final int INIT_FIELD_TASK_ID = 3;

  static class TopLevelTask implements TaskFunction {
    public void task(Task task, Context ctx, Runtime rt) {
      int num_elements = 1024;
      int num_subregions = 4;

      Rect elem_rect = new Rect(new Point(0), new Point(num_elements-1));
      IndexSpace is = rt.createIndexSpace(ctx, new Domain(elem_rect));
      FieldSpace input_fs = rt.createFieldSpace(ctx);

      FieldAllocator input_fa = rt.createFieldAllocator(ctx, input_fs);
      input_fa.allocateField(8, 1);
      input_fa.allocateField(8, 2);

      FieldSpace output_fs = rt.createFieldSpace(ctx);
      FieldAllocator output_fa = rt.createFieldAllocator(ctx, output_fs);

      LogicalRegion input_lr = rt.createLogicalRegion(ctx, is, input_fs);
      LogicalRegion output_lr = rt.createLogicalRegion(ctx, is, output_fs);

      Rect color_bounds = new Rect(new Point(0), new Point(num_subregions-1));
      Domain color_domain = new Domain(color_bounds);

      int lower_bound = num_elements / num_subregions;
      int upper_bound = lower_bound + 1;
      int number_small = num_subregions - (num_elements % num_subregions);
      DomainColoring coloring = new DomainColoring();
      int index = 0;
      for (int color = 0; color < num_subregions; color++) {
        int num_elmts = color < number_small ? lower_bound : upper_bound;
        Rect subrect = new Rect(new Point(index), new Point(index+num_elmts-1));
        coloring.setColor(color, new Domain(subrect));
        index += num_elmts;
      }
      IndexPartition ip = rt.createIndexPartition(ctx, is, color_domain, coloring, true);

      LogicalPartition input_lp = rt.getLogicalPartition(ctx, input_lr, ip);
      LogicalPartition output_lp = rt.getLogicalPartition(ctx, output_lr, ip);

      ArgumentMap arg_map = new ArgumentMap();
      Domain launch_domain = color_domain;

      IndexLauncher init_x_launcher = new IndexLauncher(INIT_FIELD_TASK_ID,
          launch_domain, null, arg_map);
      RegionRequirement x_req = new RegionRequirement(input_lp, 0,
          PrivilegeMode.WRITE_DISCARD, CoherenceProperty.EXCLUSIVE, input_lr);
      x_req.addField(1);
      init_x_launcher.addRegionRequirement(x_req); // makes copy of req
      rt.executeIndexSpace(ctx, init_x_launcher);

      IndexLauncher init_y_launcher = new IndexLauncher(INIT_FIELD_TASK_ID,
          launch_domain, null, arg_map);
      RegionRequirement y_req = new RegionRequirement(input_lp, 0,
          PrivilegeMode.WRITE_DISCARD, CoherenceProperty.EXCLUSIVE, input_lr);
      y_req.addField(2);
      init_y_launcher.addRegionRequirement(x_req); // makes copy of req
      rt.executeIndexSpace(ctx, init_y_launcher);
    }
  }

  static class InitFieldTask implements TaskFunction {
    public void task(Task task, Context ctx, Runtime rt) {
      System.out.println("hello from init field task");
    }
  }

  public static void main(String[] args) {

    Runtime.register_task(TOP_TASK_ID, new TopLevelTask());
    Runtime.register_task(INIT_FIELD_TASK_ID, new InitFieldTask());

    Runtime.set_top_level_task_id(TOP_TASK_ID);
    Runtime.start(args);
  }
}
```
