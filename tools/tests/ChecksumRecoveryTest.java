/**
 * Host-only regression checks for the recovered installer CRC helper.
 * From the repository root, with a JDK 17 or newer:
 *   mkdir -p build/checksum-tests
 *   javac --release 17 -d build/checksum-tests \
 *     port/android-java/java/com/gameloft/android/GAND/GloftD2SS/installer/utils/CRC.java \
 *     tools/tests/ChecksumRecoveryTest.java
 *   java -cp build/checksum-tests ChecksumRecoveryTest
 *
 * Tests historical behavior, including the non-positive-index deletion rule
 * and the existing zero return value for I/O failure. It does not strengthen or
 * relax the original acceptance rules.
 */
import com.gameloft.android.GAND.GloftD2SS.installer.utils.CRC;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Comparator;
import java.util.Random;
import java.util.zip.CRC32;

public class ChecksumRecoveryTest {
  private static int checks;

  private static long reference(byte[] bytes, int offset, int count) {
    CRC32 checksum = new CRC32();
    checksum.update(bytes, offset, count);
    return checksum.getValue();
  }

  private static void equal(long expected, long actual, String context) {
    checks++;
    if (expected != actual) throw new AssertionError(context + ": expected=" + expected + " actual=" + actual);
  }

  private static void condition(boolean actual, String context) {
    checks++;
    if (!actual) throw new AssertionError(context);
  }

  private static void checkData(Path file, byte[] bytes) throws Exception {
    Files.write(file, bytes);
    long full = reference(bytes, 0, bytes.length);
    equal(full, CRC.calcChecksum(file.toString()), "whole-file length=" + bytes.length);
    equal(full, CRC.calcChecksum(file.toString(), 0), "index0 length=" + bytes.length);
    // Historical negative indices check the initial 2048 bytes without skip.
    equal(reference(bytes, 0, Math.min(2048, bytes.length)), CRC.calcChecksum(file.toString(), -1), "index-1 length=" + bytes.length);
    for (int index = 1; index <= (bytes.length + 2047) / 2048 + 2; index++) {
      int offset = Math.min(bytes.length, (index - 1) * 2048);
      int count = Math.min(2048, bytes.length - offset);
      equal(reference(bytes, offset, count), CRC.calcChecksum(file.toString(), index), "chunk" + index + " length=" + bytes.length);
    }
  }

  public static void main(String[] args) throws Exception {
    Path directory = Files.createTempDirectory("dh2-checksum-test-");
    try {
      Path file = directory.resolve("data.bin");
      byte[] known = "123456789".getBytes(StandardCharsets.US_ASCII);
      Files.write(file, known);
      equal(0xCBF43926L, CRC.calcChecksum(file.toString()), "CRC32 known vector");
      Random random = new Random(2063);
      for (int length : new int[]{0, 1, 127, 128, 129, 2047, 2048, 2049, 4095, 4096, 4097}) {
        byte[] bytes = new byte[length];
        random.nextBytes(bytes);
        checkData(file, bytes);
      }
      for (int i = 0; i < 128; i++) {
        byte[] bytes = new byte[random.nextInt(16385)];
        random.nextBytes(bytes);
        checkData(file, bytes);
      }

      long knownChecksum = reference(known, 0, known.length);
      Files.write(file, known);
      condition(CRC.isValidChecksum(file.toString(), knownChecksum), "whole-file valid return");
      condition(Files.exists(file), "whole-file valid preserves file");
      condition(!CRC.isValidChecksum(file.toString(), knownChecksum ^ 1L), "whole-file mismatch return");
      condition(!Files.exists(file), "whole-file mismatch deletes file");

      Files.write(file, known);
      condition(!CRC.isValidChecksum(file.toString(), knownChecksum ^ 1L, 1), "positive chunk mismatch return");
      condition(Files.exists(file), "positive chunk mismatch preserves file");
      condition(CRC.isValidChecksum(file.toString(), knownChecksum, 1), "positive chunk valid return");
      condition(Files.exists(file), "positive chunk valid preserves file");
      condition(!CRC.isValidChecksum(file.toString(), knownChecksum ^ 1L, 0), "index0 mismatch return");
      condition(!Files.exists(file), "index0 mismatch deletes file");

      Files.write(file, known);
      condition(!CRC.isValidChecksum(file.toString(), knownChecksum ^ 1L, -1), "negative index mismatch return");
      condition(!Files.exists(file), "negative index mismatch deletes file");
      equal(0L, CRC.calcChecksum(file.toString()), "missing file checksum zero");
      equal(0L, CRC.calcChecksum(file.toString(), 1), "missing chunk checksum zero");
      condition(CRC.isValidChecksum(file.toString(), 0L), "historical missing-file/zero-checksum equality");
      condition(!CRC.isValidChecksum(file.toString(), 1L), "missing-file/nonzero-checksum mismatch");
      System.out.println("Recovered CRC passes " + checks + " checks: known vector, 139 boundary/random files, full-file and 2048-byte chunks, negative/zero indices, and deletion/missing-file semantics.");
    } finally {
      try (var paths = Files.walk(directory)) {
        for (Path path : paths.sorted(Comparator.reverseOrder()).toList()) Files.deleteIfExists(path);
      }
    }
  }
}
