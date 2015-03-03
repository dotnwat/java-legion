import java.nio.ByteBuffer;
import org.legion.Runtime;
import org.legion.*;

public class DaxbyExample {

  static final int TOP_TASK_ID   = 1;
  static final int DAXBY_TASK_ID = 2;
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
      output_fa.allocateField(8, 3);

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

      double alpha = 0.5;
      IndexLauncher daxby_launcher = new IndexLauncher(DAXBY_TASK_ID,
          launch_domain, toByteArray(alpha), arg_map);
      RegionRequirement input_daxby_req = new RegionRequirement(input_lp, 0,
          PrivilegeMode.READ_ONLY, CoherenceProperty.EXCLUSIVE, input_lr);
      input_daxby_req.addField(1);
      input_daxby_req.addField(2);
      daxby_launcher.addRegionRequirement(input_daxby_req);

      RegionRequirement output_daxby_req = new RegionRequirement(output_lp, 0,
          PrivilegeMode.WRITE_DISCARD, CoherenceProperty.EXCLUSIVE, output_lr);
      output_daxby_req.addField(3);
      rt.executeIndexSpace(ctx, daxby_launcher);
    }
  }

  public static byte[] toByteArray(double value) {
        byte[] bytes = new byte[8];
            ByteBuffer.wrap(bytes).putDouble(value);
                return bytes;
  }

  public static double toDouble(byte[] bytes) {
        return ByteBuffer.wrap(bytes).getDouble();
  }

  static class InitFieldTask implements TaskFunction {
    public void task(Task task, Context ctx, Runtime rt) {
      System.out.println("hello from init field task");
    }
  }

  static class DaxbyTask implements TaskFunction {
    public void task(Task task, Context ctx, Runtime rt) {
      System.out.println("hello from daxby task");
    }
  }

  public static void main(String[] args) {

    Runtime.register_task(TOP_TASK_ID, new TopLevelTask());
    Runtime.register_task(DAXBY_TASK_ID, new DaxbyTask());
    Runtime.register_task(INIT_FIELD_TASK_ID, new InitFieldTask());

    Runtime.set_top_level_task_id(TOP_TASK_ID);
    Runtime.start(args);
  }
}
