# Reconstructed Android Java

This is a repaired reconstruction of the supplied game's Android Java layer. The original smali and unedited Java exports are in the [separate recovery handoff](https://drive.google.com/drive/folders/1njTxLAJHt08SinEskn7gC6IWylXemwDW) under `recovered/android/smali` and `recovered/android/java`; those directories are not in this Git checkout. The imported files and their source-archive hashes are listed in [`reports/recovery-source-import.json`](../../reports/recovery-source-import.json).

The reconstruction compiles with JDK 17 against Android SDK platform 35 and `org.apache.http.legacy.jar`. This proves Java compilation and DEX conversion; it does not prove that the game runs on a current phone. The native ARM32 engine, modern storage/permission flows, manifest integration, original assets, and device testing still require separate work.

The imported 288 Java files were also compiled on Windows with JDK 25 (`--release 17`) against Android SDK platform 37.0. D8 produced a DEX with all 46 original native declarations matching. The [API 37 validation record](../../reports/java-port-api37-validation.json) reports 364 generated class files, 15 legacy warnings and the DEX hash. This verifies source compatibility with the newer SDK stubs; it does not set the app's target SDK or run the game on Android 17.

## Build the Java layer

Install JDK 17, Python 3, Android SDK platform 35, and build tools 35.0.0. `java` and `javac` must resolve to that JDK. Set `ANDROID_HOME` to your SDK directory. Then run these commands from the repository root, using new output directories:

```sh
python3 tools/compile_android_java.py \
  --android-platform "$ANDROID_HOME/platforms/android-35" \
  --output /tmp/dh2-java-classes \
  --log /tmp/dh2-java-compile.log

python3 tools/build_android_dex.py \
  --android-platform "$ANDROID_HOME/platforms/android-35" \
  --build-tools "$ANDROID_HOME/build-tools/35.0.0" \
  --classes-dir /tmp/dh2-java-classes \
  --output /tmp/dh2-java-dex \
  --report /tmp/dh2-java-dex-report.json
```

The second command checks every native declaration's class, method name, parameter/return descriptor, and staticness against the original inventory. `classes.dex` is the Java layer only. These commands do not create a playable APK.

The compiler and DEX tools accept `--javac` and `--java` to select explicit executables. `build_android_dex.py` defaults to minimum API 23 for this build check; this is not a claim of runtime support on API 23.

## Repairs and evidence

The port was re-exported with JADX's `--no-inline-methods`, retaining original synthetic access bridges rather than weakening private access to authorization state. Repairs correct type inference, shadowed resource/class names, collection typing, checked exception declarations, missing loop exits, and missing return branches with the corresponding smali as evidence.

The cache download layer needed additional control-flow reconstruction: split-file tracking, download-list iteration, checksum decisions, resume lengths, and error cleanup are derived from original branches. The checksum result conventions are retained, including the unusual direct MD5 result used by `DownloadComponent`.

`WebSettingsCompat` is the single intentional modern API adaptation: it invokes the old AppCache setter by reflection when available. When the setter is absent, the feature itself is absent and the call has no effect. It preserves invocation failures rather than swallowing them.

Legacy Samsung/KDDI authorization and billing checks, endpoints, result codes, and success conditions are retained. They have not been tested against functioning legacy services.

See `reports/java-port-vendor-repairs.md` for the vendor audit and `reports/java-port-repairs.md` for the remaining repairs. Focused host tests under `tools/tests/` cover reconstructed Base64 and checksum code; test fixtures are separate from production sources.

## Remaining limits

JADX renames some obfuscated Java members. The native-declaration comparison verifies exported native methods; it does not verify native calls into every Java field/helper method or reflection string. The corresponding JNI lookup and reflection contracts need a separate audit.

Compilation, focused host tests, and DEX conversion are not a proof of semantic equivalence for every reconstructed method. No Android device, account flow, billing flow, renderer, audio pipeline, or gameplay session has been run from this reconstructed Java layer.
