/**
 * Host-only behavioral regression test for the reconstructed KDDI Base64 codec.
 * No Android runtime or dependency stubs are used.
 *
 * From the repository root, using a JDK 17 or newer:
 *   mkdir -p build/vendor-base64-tests
 *   javac --release 17 -d build/vendor-base64-tests \
 *     port/android-java/java/com/kddi/market/alml/util/b.java \
 *     port/android-java/java/com/kddi/market/alml/util/Base64.java \
 *     tools/tests/VendorBase64RecoveryTest.java
 *   java -cp build/vendor-base64-tests VendorBase64RecoveryTest
 */
import java.util.Arrays;
import java.util.Random;
import java.nio.charset.StandardCharsets;
import com.kddi.market.alml.util.Base64;

public class VendorBase64RecoveryTest {
  public static void main(String[] args) {
    Random random = new Random(2063);
    int cases = 0;
    for (int length = 0; length < 1024; length++) {
      byte[] input = new byte[length];
      random.nextBytes(input);
      for (int flags = 0; flags < 16; flags++) {
        java.util.Base64.Encoder reference = (flags & 8) == 0 ? java.util.Base64.getEncoder() : java.util.Base64.getUrlEncoder();
        if ((flags & 1) != 0) reference = reference.withoutPadding();
        byte[] encoded = Base64.encode(input, flags);
        byte[] referenceBytes = reference.encode(input);
        if ((flags & 2) == 0 && referenceBytes.length > 0) {
          String plain = new String(referenceBytes, StandardCharsets.US_ASCII);
          StringBuilder wrapped = new StringBuilder();
          for (int offset = 0; offset < plain.length(); offset += 76) {
            wrapped.append(plain, offset, Math.min(plain.length(), offset + 76));
            wrapped.append((flags & 4) == 0 ? "\n" : "\r\n");
          }
          referenceBytes = wrapped.toString().getBytes(StandardCharsets.US_ASCII);
        }
        if (!Arrays.equals(referenceBytes, encoded)) throw new AssertionError("encode length=" + length + " flags=" + flags);
        byte[] decoded;
        try { decoded = Base64.decode(encoded, flags); } catch (IllegalArgumentException e) { throw new AssertionError("decode length=" + length + " flags=" + flags + " encoded=" + new String(encoded, StandardCharsets.US_ASCII),e); }
        if (!Arrays.equals(input, decoded)) throw new AssertionError("decode length=" + length + " flags=" + flags);
        // Also verify the public byte-range overloads ignore surrounding data.
        byte[] rangedInput = new byte[input.length + 19];
        random.nextBytes(rangedInput);
        System.arraycopy(input, 0, rangedInput, 7, input.length);
        if (!Arrays.equals(encoded, Base64.encode(rangedInput, 7, input.length, flags))) {
          throw new AssertionError("ranged encode length=" + length + " flags=" + flags);
        }
        byte[] rangedEncoded = new byte[encoded.length + 19];
        random.nextBytes(rangedEncoded);
        System.arraycopy(encoded, 0, rangedEncoded, 7, encoded.length);
        if (!Arrays.equals(input, Base64.decode(rangedEncoded, 7, encoded.length, flags))) {
          throw new AssertionError("ranged decode length=" + length + " flags=" + flags);
        }
        cases++;
      }
    }
    for (String malformed : new String[]{"A", "A=", "AA=", "AAA==", "=AAA", "AAAA=", "ZQ==A"}) {
      boolean rejected = false;
      try {
        Base64.decode(malformed.getBytes(StandardCharsets.US_ASCII), 2);
      } catch (IllegalArgumentException expected) {
        rejected = true;
      }
      if (!rejected) throw new AssertionError("Accepted malformed input: " + malformed);
    }
    System.out.println("Recovered KDDI Base64 agrees with java.util.Base64 for " + cases + " randomized vectors (16 flag combinations, lengths 0..1023), byte-range overloads, and 7 malformed-input cases.");
  }
}
