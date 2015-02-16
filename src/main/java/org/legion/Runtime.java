package org.legion;

import java.io.IOException;
import java.util.HashMap;

/**
 *
 */
public class Runtime {

  static {
    loadLibrary();
  }

  public static synchronized void loadLibrary() {
    String tmpDir = System.getenv("LEGION_JNILIB_DIR");
    try {
      NativeLibraryLoader.getInstance().loadLibraryFromJar(tmpDir);
    } catch (IOException e) {
      throw new RuntimeException("Unable to load the Legion shared library: " + e);
    }
  }

  private static int top_level_task_id;

  private static HashMap<Integer, TaskFunction> tasks = 
    new HashMap<Integer, TaskFunction>();

  /**
   *
   */
  public static void set_top_level_task_id(int task_id) {
    top_level_task_id = task_id;
  }

  /**
   *
   */
  public synchronized static void register_task(int task_id, TaskFunction func) {
    if (func == null)
      throw new IllegalArgumentException();
    if (tasks.containsKey(task_id))
      throw new RuntimeException("duplicate task id");
    tasks.put(task_id, func);
  }

  /**
   *
   */
  private static void top_level_task_wrapper(final long taskp,
      final long[] regionp, final long ctxp, final long rtp) {

    Context ctx = new Context(ctxp);
    Runtime rt = new Runtime(rtp);
    Task t = new Task(taskp);

    TaskFunction func;
    synchronized(Runtime.class) {
      func = tasks.get(top_level_task_id);
    }

    if (func == null)
      throw new NullPointerException();

    func.task(t, ctx, rt);
  }

  /**
   *
   */
  private static void task_wrapper(final long taskp, final long[] regionp,
      final long ctxp, final long rtp, int task_id) {

    Context ctx = new Context(ctxp);
    Runtime rt = new Runtime(rtp);
    Task t = new Task(taskp);

    TaskFunction func;
    synchronized(Runtime.class) {
      func = tasks.get(task_id);
    }

    if (func == null)
      throw new NullPointerException();

    func.task(t, ctx, rt);
  }

  /**
   *
   */
  public static native void start(String[] args);

  /**
   *
   */
  Runtime(long handle) {
    this.handle = handle;
  }

  long handle;

  /**
   *
   */
  public Future executeTask(Context ctx, TaskLauncher launcher) {
    long futureHandle = hlr_execute_task(handle, ctx.handle,
        launcher.nativeHandle);
    return new Future(futureHandle);
  }

  private native long hlr_execute_task(long rt, long ctx, long launcher);
}
