# java-legion - Java bindings to Legion

# Examples

* [Hello World](#hello-world)

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

