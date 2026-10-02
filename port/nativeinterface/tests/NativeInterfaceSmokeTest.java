package com.samsung.zirconia;

import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.security.MessageDigest;
import java.util.Arrays;

/* Desktop-JVM JNI marshaling tests; all identities and paths are synthetic. */
public final class NativeInterfaceSmokeTest {
    private static void require(boolean condition) {
        if (!condition) throw new AssertionError();
    }

    public static void main(String[] args) throws Exception {
        System.load(Path.of(args[0]).toAbsolutePath().toString());
        String first = "synthetic-test-identity", second = "synthetic-test-product";
        String fragment = NativeInterface.doPassphraseTest(first, second);
        require(fragment != null && fragment.length() > 0 && fragment.length() <= 1023);
        String unicode = NativeInterface.doPassphraseTest("caf\u00e9", "\u4e16\u754c");
        require(unicode != null && !unicode.isEmpty());
        byte[] digest = MessageDigest.getInstance("SHA-1").digest(
            (fragment + first + second).getBytes(StandardCharsets.UTF_8));
        Path directory = Files.createTempDirectory("dh2-jni-smoke-");
        Path file = directory.resolve("synthetic-license");
        try {
            require(!NativeInterface.checkLicenseFile(file.toString(), first, second));
            require(!NativeInterface.storeLicenseKey(file.toString(), new byte[19], "metadata"));
            require(NativeInterface.storeLicenseKey(file.toString(), digest, "metadata"));
            byte[] stored = Files.readAllBytes(file);
            require(stored.length == 40 && Arrays.equals(Arrays.copyOf(stored, 20), digest));
            byte[] metadata = MessageDigest.getInstance("SHA-1").digest("me".getBytes(StandardCharsets.UTF_8));
            require(Arrays.equals(Arrays.copyOfRange(stored, 20, 40), metadata));
            require(NativeInterface.checkLicenseFile(file.toString(), first, second));
            require(!NativeInterface.checkLicenseFile(file.toString(), "wrong-identity", second));
            stored[0] ^= 1;
            Files.write(file, stored);
            require(!NativeInterface.checkLicenseFile(file.toString(), first, second));
            require(NativeInterface.checkLicenseFile2("missing-path", "any-value"));
            require(NativeInterface.checkLicenseFile2(null, null));
            require(!NativeInterface.checkLicenseFile(null, first, second));
            require(!NativeInterface.storeLicenseKey(file.toString(), null, "metadata"));
            require(NativeInterface.doPassphraseTest(null, second) == null);
        } finally {
            Files.deleteIfExists(file);
            Files.delete(directory);
        }
        System.out.println("PASS: all 4 JNI methods on host JVM; storage, validation, Unicode marshaling, invalid and null inputs");
    }
}

/* Same native declarations as recovered NativeInterface.smali; load is explicit
 * in the test to select the host build, never the original ARM library. */
final class NativeInterface {
    public static native boolean checkLicenseFile(String path, String first, String second);
    public static native boolean checkLicenseFile2(String first, String second);
    public static native String doPassphraseTest(String first, String second);
    public static native boolean storeLicenseKey(String path, byte[] key, String metadata);
}
