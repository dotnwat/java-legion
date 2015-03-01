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
    long futureHandle = executeTask(handle, ctx.handle,
        launcher.nativeHandle);
    return new Future(futureHandle);
  }

  /**
   *
   */
  public IndexSpace createIndexSpace(Context ctx, long max_elems) {
    long isHandle = createIndexSpace(handle, ctx.handle, max_elems);
    return new IndexSpace(isHandle);
  }

  /**
   *
   */
  public IndexSpace createIndexSpace(Context ctx, Domain domain) {
    long isHandle = createIndexSpaceFromDomain(handle, ctx.handle,
        domain.nativeHandle);
    return new IndexSpace(isHandle);
  }

  /**
   *
   */
  public FieldSpace createFieldSpace(Context ctx) {
    long fsHandle = createFieldSpace(handle, ctx.handle);
    return new FieldSpace(fsHandle);
  }

  /**
   *
   */
  public FieldAllocator createFieldAllocator(Context ctx, FieldSpace fs) {
    long faHandle = createFieldAllocator(handle, ctx.handle,
        fs.nativeHandle);
    return new FieldAllocator(faHandle);
  }

  /**
   *
   */
  public LogicalRegion createLogicalRegion(Context ctx, IndexSpace is, FieldSpace fs) {
    long lrHandle = createLogicalRegion(handle, ctx.handle,
        is.nativeHandle, fs.nativeHandle);
    return new LogicalRegion(lrHandle);
  }

  /**
   *
   */
  public PhysicalRegion mapRegion(Context ctx, InlineLauncher launcher) {
    long prHandle = mapRegion(handle, ctx.handle, launcher.nativeHandle);
    return new PhysicalRegion(prHandle);
  }

  /**
   *
   */
  public IndexAllocator createIndexAllocator(Context ctx, IndexSpace is) {
    long iaHandle = createIndexAllocator(handle, ctx.handle, is.nativeHandle);
    return new IndexAllocator(iaHandle);
  }

  /**
   *
   */
  public IndexPartition createIndexPartition(Context ctx, IndexSpace is,
      Domain domain, DomainColoring coloring, boolean disjoint) {
    long ipHandle = createIndexPartition(handle, ctx.handle, is.nativeHandle,
        domain.nativeHandle, coloring.nativeHandle, disjoint);
    return new IndexPartition(ipHandle);
  }

  /**
   *
   */
  public LogicalPartition getLogicalPartition(Context ctx, LogicalRegion parent,
      IndexPartition partition) {
    long lpHandle = getLogicalPartition(handle, ctx.handle, parent.nativeHandle,
        partition.nativeHandle);
    return new LogicalPartition(lpHandle);
  }

  private native long executeTask(long rt, long ctx, long launcher);
  private native long createIndexSpace(long rt, long ctx, long max_elems);
  private native long createIndexSpaceFromDomain(long rt, long ctx, long domain);
  private native long createFieldSpace(long rt, long ctx);
  private native long createFieldAllocator(long rt, long ctx, long fs);
  private native long createLogicalRegion(long rt, long ctx, long is, long fs);
  private native long mapRegion(long rt, long ctx, long launcher);
  private native long createIndexAllocator(long rt, long ctx, long is);
  private native long createIndexPartition(long rt, long ctx, long is,
      long domain, long colorDomain, boolean disjoint);
  private native long getLogicalPartition(long rt, long ctx, long lr, long ip);
}
