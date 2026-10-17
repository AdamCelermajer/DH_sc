# Host cache integrity test fixtures

**Test-only files. Do not include this directory in an Android build or APK.**

`GameInstaller.java` supplies only paths and an unexpected-error assertion for a host JVM. The real GameInstaller initializes Android Environment at class loading time, which is unavailable on a plain JVM. The harness executes the actual recovered `DownloadComponent`, `Utils`, metadata class `f`, `CRC`, and `MD5` implementations. It does not substitute their checksum, marker, or deletion behavior.

The production compile helper only scans `port/android-java/java`, so this fixture is excluded. Keep the host test output ahead of production classes only when running the test. It deliberately shadows GameInstaller in the test classpath.

From the repository root, with Python 3, JDK 17, and Android API 35 installed, use the following POSIX-shell commands. Set `DH2_ANDROID_PLATFORM` to the installed platform directory. Both class output directories are temporary and outside the repository.

```sh
DH2_ANDROID_PLATFORM=/path/to/android-sdk/platforms/android-35
DH2_CACHE_TEST_WORK=$(mktemp -d)
python3 tools/compile_android_java.py \
  --android-platform "$DH2_ANDROID_PLATFORM" \
  --output "$DH2_CACHE_TEST_WORK/production"
mkdir -p "$DH2_CACHE_TEST_WORK/cache-tests"
javac --release 17 -sourcepath '' \
  -cp "$DH2_CACHE_TEST_WORK/production:$DH2_ANDROID_PLATFORM/android.jar" \
  -d "$DH2_CACHE_TEST_WORK/cache-tests" \
  port/android-java/java/com/gameloft/android/GAND/GloftD2SS/installer/utils/DownloadComponent.java \
  port/android-java/java/com/gameloft/android/GAND/GloftD2SS/installer/utils/CRC.java \
  port/android-java/java/com/gameloft/android/GAND/GloftD2SS/installer/utils/MD5.java \
  port/android-java/java/com/gameloft/android/GAND/GloftD2SS/installer/utils/f.java \
  port/android-java/java/com/gameloft/android/GAND/GloftD2SS/installer/Utils.java
javac --release 17 \
  -cp "$DH2_CACHE_TEST_WORK/cache-tests:$DH2_CACHE_TEST_WORK/production:$DH2_ANDROID_PLATFORM/android.jar" \
  -d "$DH2_CACHE_TEST_WORK/cache-tests" \
  tools/tests/cache-fixtures/GameInstaller.java \
  tools/tests/cache-fixtures/DownloadIntegrityRecoveryTest.java
java \
  -cp "$DH2_CACHE_TEST_WORK/cache-tests:$DH2_CACHE_TEST_WORK/production:$DH2_ANDROID_PLATFORM/android.jar" \
  DownloadIntegrityRecoveryTest
```

The harness uses a temporary cache directory, removes it afterward, and makes no network calls. It checks good/bad sizes, CRC behavior, the bytecode's unusual direct MD5 Boolean branch, partial-file resume, reset/deletion rules, positive split-file handling, real marker matching, and duplicate marker prevention.

The test preserves historical behavior. In particular, the original DownloadComponent uses the direct result of `MD5.isValidChecksum` in its download-required Boolean, while it negates the CRC result. This is recorded behavior, not a proposed integrity fix. Any intended modernization of that behavior should be a separate reviewed change.
