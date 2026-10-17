/* HOST TEST ONLY. Requires the GameInstaller fixture in this directory.
 * See README.md for standalone instructions and the exact fixture boundary.
 * The actual recovered DownloadComponent, Utils, f, CRC and MD5 classes run.
 */
import com.gameloft.android.GAND.GloftD2SS.installer.GameInstaller;
import com.gameloft.android.GAND.GloftD2SS.installer.Utils;
import com.gameloft.android.GAND.GloftD2SS.installer.utils.DownloadComponent;
import com.gameloft.android.GAND.GloftD2SS.installer.utils.CRC;
import com.gameloft.android.GAND.GloftD2SS.installer.utils.MD5;
import com.gameloft.android.GAND.GloftD2SS.installer.utils.f;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Comparator;
import java.util.Vector;

public class DownloadIntegrityRecoveryTest {
  private static int checks;
  private static Method verify;
  private static void set(Object target, String name, Object value) throws Exception {
    Field field = target.getClass().getDeclaredField(name);
    field.setAccessible(true);
    field.set(target, value);
  }
  private static Object get(Object target, String name) throws Exception {
    Field field = target.getClass().getDeclaredField(name);
    field.setAccessible(true);
    return field.get(target);
  }
  private static void check(boolean result, String context) {
    checks++;
    if (!result) throw new AssertionError(context);
  }
  private static DownloadComponent component(int files, boolean force) throws Exception {
    DownloadComponent result = new DownloadComponent("unused", "data");
    Vector<Object> entries = new Vector<>();
    for (int i = 0; i < files; i++) entries.add(new Object());
    set(result, "A", entries);
    set(result, "x", force);
    set(result, "j", 42L);
    return result;
  }
  private static f metadata(String name, long length, long crc, String md5) throws Exception {
    f result = new f();
    result.a("");
    result.b(name);
    result.a(length);
    result.b(crc);
    set(result, "k", md5);
    return result;
  }
  private static boolean verify(DownloadComponent component, f file, boolean checksum) throws Exception {
    return (boolean) verify.invoke(component, file, checksum, true, "unrelated-name");
  }
  public static void main(String[] args) throws Exception {
    Path directory = Files.createTempDirectory("dh2-integrity-test-");
    try {
      GameInstaller.DATA_PATH = directory.toString();
      GameInstaller.marketPath = directory.toString();
      GameInstaller.sd_folder = directory.toString();
      verify = DownloadComponent.class.getDeclaredMethod("a", f.class, boolean.class, boolean.class, String.class);
      verify.setAccessible(true);
      Path file = directory.resolve("cache.bin");
      byte[] bytes = new byte[1024];
      new java.util.Random(2063).nextBytes(bytes);
      Files.write(file, bytes);
      long crc = CRC.calcChecksum(file.toString());
      f valid = metadata("cache.bin", bytes.length, crc, "");
      DownloadComponent state = component(1, false);
      check(!verify(state, valid, true), "valid size/CRC does not request download");
      check(Files.exists(file), "valid size/CRC preserves file");
      check((long) get(state, "j") == 42L, "valid size/CRC preserves progress state");

      state = component(1, false);
      check(verify(state, metadata("cache.bin", 2048, crc, ""), true), "bad size requests download");
      check(Files.exists(file), "single file with force=false preserves partial file");
      check((long) get(state, "j") == bytes.length, "single file resumes at existing length");
      state = component(2, false);
      check(verify(state, metadata("cache.bin", 2048, crc, ""), true), "multi-file bad size requests download");
      check(!Files.exists(file), "multi-file reset deletes non-split file");
      check((long) get(state, "j") == 0L, "multi-file reset progress zero");

      Files.write(file, bytes);
      state = component(1, true);
      check(verify(state, metadata("cache.bin", 2048, crc, ""), true), "forced reset requests download");
      check(!Files.exists(file), "forced reset deletes partial non-split file");
      check(!(boolean) get(state, "x") && (long) get(state, "j") == 0L, "forced reset clears x and progress");

      Files.write(file, bytes);
      state = component(1, false);
      check(!verify(state, metadata("cache.bin", bytes.length, crc ^ 1L, ""), false), "original checksum=false skips checksum branch");
      check(Files.exists(file), "checksum=false preserves good-sized file");
      check(verify(state, metadata("cache.bin", bytes.length, crc ^ 1L, ""), true), "CRC mismatch requests download");
      check(!Files.exists(file), "CRC helper deletes checksum-mismatched whole file");
      check((long) get(state, "j") == 0L, "CRC deletion causes zero existing-length progress");

      Files.write(file, bytes);
      String digest = MD5.getMD5Checksum(file.toString());
      state = component(2, false);
      // Original DEX passes MD5.isValidChecksum's result directly into v2.
      // Preserve this unusual historical branch instead of silently inverting it.
      check(verify(state, metadata("cache.bin", bytes.length, crc, digest), true), "historical direct MD5=true branch requests download");
      check(!Files.exists(file), "historical direct MD5=true branch resets/deletes non-split file");
      Files.write(file, bytes);
      state = component(2, false);
      check(!verify(state, metadata("cache.bin", bytes.length, crc, "wrong-digest"), true), "historical direct MD5=false branch does not request download");
      check(Files.exists(file), "historical direct MD5=false branch preserves file");

      f split = metadata("cache.bin.split_1", bytes.length, crc ^ 1L, "");
      Path markers = directory.resolve("d_o_w_n_l_o_a_d_e_d.txt");
      check(!Utils.hasBeenDownloaded(split, false), "missing marker file returns false");
      Files.writeString(markers, "other-file\n");
      check(!Utils.hasBeenDownloaded(split, false), "nonmatching marker returns false");
      Utils.markAsSaved(split);
      check(Utils.hasBeenDownloaded(split, false), "real marker matching returns true");
      long markerLength = Files.size(markers);
      Utils.markAsSaved(split);
      check(Files.size(markers) == markerLength, "matching marker prevents duplicate markAsSaved append");
      state = component(2, false);
      check(!verify(state, split, true), "original positive split marker/goodSize branch accepted");
      check(Files.exists(file), "accepted positive split preserves base file");
      Files.writeString(markers, "other-file\n");
      check(verify(state, split, true), "missing positive split marker requests download");
      check(Files.exists(file), "positive split reset preserves base file");
      check((long) get(state, "j") == 0L, "positive split reset progress zero");
      System.out.println("Actual recovered DownloadComponent and Utils pass " + checks + " focused checks: size, CRC, original MD5 branch, resume/reset/deletion, positive split files and marker matching.");
    } finally {
      try (var paths = Files.walk(directory)) {
        for (Path path : paths.sorted(Comparator.reverseOrder()).toList()) Files.deleteIfExists(path);
      }
    }
  }
}
