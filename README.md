# java-legion - Java bindings to Legion

This is your basic *hello world* example. It prints out the iconic message in the top-level task.

```java
import org.legion.Runtime;
import org.legion.TaskFunction;
import org.legion.Context;
import org.legion.Runtime;

public class LegionHelloWorld {

  static final int TOP_ID = 1;

  public static void main(String[] args) {

    Runtime.register_task(TOP_ID, new TaskFunction() {
      public void task(Context ctx, Runtime rt) {
        System.out.println("Hello world, I am Legion.");
      }
    });

    Runtime.set_top_level_task_id(TOP_ID);
    Runtime.start(args);
  }
}
```

Everything is packaged up nicely. Just run it from the command line:

```bash
java -cp legion-linux64.jar:. LegionHelloWorld
Hello world, I am Legion.
```

Launching sub-tasks is also easy. Here we register two tasks, `TOP_ID` and
`HELLO_ID`. The top-level task launches `MyTask` which prints out a message:

```java
import org.legion.Runtime;
import org.legion.TaskFunction;
import org.legion.Context;
import org.legion.Runtime;
import org.legion.Future;
import org.legion.TaskLauncher;

public class LegionHelloWorld {

  static final int TOP_ID   = 1;
  static final int HELLO_ID = 2;

  static class MyTask implements TaskFunction {
    public void task(Context ctx, Runtime rt) {
      System.out.println("Hello world, from sub-task");
    }
  }

  public static void main(String[] args) {

    Runtime.register_task(HELLO_ID, new MyTask());

    Runtime.register_task(TOP_ID, new TaskFunction() {
      public void task(Context ctx, Runtime rt) {
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

It is run the same way, and gives the expected results:

```bash
java -cp legionjni-linux64.jar:. -Xcheck:jni LegionHelloWorld
Hello world, from sub-task
```

