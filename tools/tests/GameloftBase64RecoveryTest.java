/**
 * Host-only regression test for the recovered Gameloft billing Base64 codec.
 * From the repository root, with a JDK 17 or newer:
 *   mkdir -p build/gameloft-base64-tests
 *   javac --release 17 -d build/gameloft-base64-tests \
 *     port/android-java/java/com/gameloft/android/GAND/GloftD2SS/billing/common/a.java \
 *     port/android-java/java/com/gameloft/android/GAND/GloftD2SS/billing/common/Base64.java \
 *     tools/tests/GameloftBase64RecoveryTest.java
 *   java -cp build/gameloft-base64-tests GameloftBase64RecoveryTest
 */
import com.gameloft.android.GAND.GloftD2SS.billing.common.Base64;
import com.gameloft.android.GAND.GloftD2SS.billing.common.a;
import java.nio.charset.StandardCharsets;
import java.util.Arrays;
import java.util.Random;

public class GameloftBase64RecoveryTest {
  private static void equal(byte[] expected, byte[] actual, String context) {
    if (!Arrays.equals(expected, actual)) throw new AssertionError(context);
  }

  public static void main(String[] args) throws a {
    String[] plain = {"", "f", "fo", "foo", "foob", "fooba", "foobar"};
    String[] encoded = {"", "Zg==", "Zm8=", "Zm9v", "Zm9vYg==", "Zm9vYmE=", "Zm9vYmFy"};
    for (int i = 0; i < plain.length; i++) {
      byte[] bytes = plain[i].getBytes(StandardCharsets.US_ASCII);
      if (!encoded[i].equals(Base64.encode(bytes))) throw new AssertionError("RFC vector encode " + i);
      equal(bytes, Base64.decode(encoded[i]), "RFC vector decode " + i);
    }
    byte[] alphabetVector = {(byte) 0xfb, (byte) 0xff, (byte) 0xff};
    if (!"+///".equals(Base64.encode(alphabetVector))) throw new AssertionError("standard alphabet");
    if (!"-___".equals(Base64.encodeWebSafe(alphabetVector, true))) throw new AssertionError("web-safe alphabet");

    Random random = new Random(2063);
    int cases = 0;
    for (int length = 0; length < 1024; length++) {
      byte[] input = new byte[length];
      random.nextBytes(input);
      String standard = Base64.encode(input);
      if (!java.util.Base64.getEncoder().encodeToString(input).equals(standard)) {
        throw new AssertionError("standard encode length=" + length);
      }
      equal(input, Base64.decode(standard), "standard decode length=" + length);
      byte[] rangedEncoded = new byte[standard.length() + 19];
      random.nextBytes(rangedEncoded);
      System.arraycopy(standard.getBytes(StandardCharsets.US_ASCII), 0, rangedEncoded, 7, standard.length());
      equal(input, Base64.decode(rangedEncoded, 7, standard.length()), "standard ranged decode length=" + length);
      cases++;
      for (boolean padding : new boolean[]{true, false}) {
        java.util.Base64.Encoder reference = java.util.Base64.getUrlEncoder();
        if (!padding) reference = reference.withoutPadding();
        String webSafe = Base64.encodeWebSafe(input, padding);
        if (!reference.encodeToString(input).equals(webSafe)) {
          throw new AssertionError("web-safe encode length=" + length + " padding=" + padding);
        }
        equal(input, Base64.decodeWebSafe(webSafe), "web-safe decode length=" + length + " padding=" + padding);
        cases++;
      }
    }
    // This historical codec accepts omitted padding. These cases exercise its
    // own explicit invalid-character/padding checks without imposing stricter
    // behavior than the original bytecode.
    for (String malformed : new String[]{"A", "=AAA", "AAAA=", "ZQ==A", "A===", "AA$="}) {
      boolean rejected = false;
      try {
        Base64.decode(malformed);
      } catch (a expected) {
        rejected = true;
      }
      if (!rejected) throw new AssertionError("Accepted malformed input: " + malformed);
    }
    System.out.println("Recovered Gameloft Base64 passes RFC vectors, standard/web-safe alphabets, " + cases + " randomized encode/decode cases, byte-range decode, and 6 malformed-input cases.");
  }
}
