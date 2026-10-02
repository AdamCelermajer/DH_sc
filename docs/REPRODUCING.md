# Reproducing the recovery

This Git checkout contains selected browsable reconstructed Java/JNI source and the recovery scripts. The full raw Java/smali, native pseudocode, DWARF, assembly/symbols and original text/shader exports remain in the [separate recovery handoff](https://drive.google.com/drive/folders/1njTxLAJHt08SinEskn7gC6IWylXemwDW). In this checkout, `recovered/native/bundles/` contains only a checksum manifest. A fresh clone cannot run `unpack_native.py` or `verify_recovery.py` until the external files are restored. Verify the imported source without those files using:

```sh
python tools/verify_recovery_import.py
```

To verify the **full historical recovery**, download `Dungeon-Hunter-2-Source-Recovery.zip`, `assembly.tar.gz` and `symbols.tar.gz` from that folder into a private work directory. Verify their SHA-256 hashes against `ARTIFACT-CHECKSUMS.json`. Extract the source ZIP there, place both `.tar.gz` files beside `dh2-reconstruction/recovered/native/bundles/manifest.json`, then run the commands below from the extracted `dh2-reconstruction` root. Do not overlay the archive onto the newer Git checkout.

Use the exact input hashes in the reports. Keep APK/cache and toolchain files outside the repository. Python 3.10+ and the dependency versions in `requirements.txt` are required for fresh recovery. `c++filt` is required for demangled ELF names.

```sh
python -m pip install -r requirements.txt
python tools/unpack_native.py
python tools/verify_recovery.py
python -m unittest discover -s tools -p 'test_*.py'
```

## Native assembly/inventory and DWARF

```sh
python tools/recover_all.py \
  --apk /absolute/path/Dungeon-Hunter-2-HD-v1-0-2.apk \
  --work-dir /absolute/path/private-work \
  --stages native dwarf
```

This extracts the three `armeabi-v7a` libraries, preserves symbols/aliases/relocations/unwind indexes/vtables/strings, exports executable bytes and verifies their round trip. DWARF exports preserve full records and produce explicitly incomplete declaration sketches. They do not build an engine.

To recreate the separate compressed evidence bundles:

```sh
python tools/package_native.py
```

## Java/smali/manifest

The original export used APKTool 2.12.1 and JADX 1.5.6. Use the full JADX all-in-one JAR, not the small CLI-only Maven artifact. Java 17+ is required. To optionally reproduce the original Java compilation diagnostics, also supply `javac` and an Android SDK platform `android.jar`.

```sh
python tools/recover_all.py \
  --apk /absolute/path/game.apk --work-dir /absolute/path/private-work \
  --stages java \
  --apktool-jar /absolute/path/apktool.jar \
  --jadx-jar /absolute/path/jadx-all.jar \
  --javac /absolute/path/jdk/bin/javac \
  --android-jar /absolute/path/android-sdk/platforms/android-35/android.jar
```

Fresh raw decompiler output is expected to have the recorded compilation problems. It does not overwrite the separately repaired `port/android-java` tree. Follow that directory's own build instructions and evidence ledger.

## Native pseudocode

The committed export uses Ghidra 11.0.3 with Java 17. Analyzer output is version-dependent. The scripts disable speculative parameter-ID analysis, seed ARM/Thumb modes from ELF symbols, repair Thumb symbol ranges, and export every executable function in the Ghidra database with a success/error index. No pseudocode output is compiled automatically.

```sh
python tools/recover_all.py \
  --apk /absolute/path/game.apk --work-dir /absolute/path/private-work \
  --stages ghidra --ghidra-home /absolute/path/ghidra_11.0.3_PUBLIC \
  --workers 6
```

Ghidra needs a full JDK; set its `JAVA_HOME_OVERRIDE` in `support/launch.properties` when automatic detection fails. Per-function timeouts and warnings are evidence, not successful reconstruction. Generic analyzers may produce different heuristic-function counts between fresh imports and subsequent repairs. Recompute coverage with:

```sh
python tools/verify_recovery.py
```

Official tool sources: [Ghidra](https://github.com/NationalSecurityAgency/ghidra), [Ghidra headless API](https://ghidra.re/ghidra_docs/api/ghidra/app/util/headless/AnalyzeHeadless.html), [JADX](https://github.com/skylot/jadx), [APKTool](https://github.com/iBotPeaches/Apktool). Tool binaries and their licenses are separate from recovered game materials.

## Cache prefix and text/shader sources

The part directory must contain one numerically ordered `.zip.partNNNNN` set, beginning with part 1 and without gaps.

```sh
python tools/recover_cache.py \
  --parts /absolute/path/cache-parts --work-dir /absolute/path/private-cache \
  --manifest recovered/assets/cache-manifest.json \
  --report reports/cache-recovery.md \
  --source-data recovered/assets/source-data --extract
```

Exit code 2 means a partial archive was recovered; exit code 0 means complete ZIP processing. The uploaded ten-part prefix is expected to produce exit code 2. Every exported complete file has verified size and CRC; incomplete entries are not published as working assets. Original XML bytes, including duplicate attributes, remain unchanged. Art/audio binaries are retained only in the specified private work directory.

## Reconstructed native component

Follow [`port/nativeinterface/README.md`](../port/nativeinterface/README.md) to run host semantic/JNI tests, original-ARM differential tests and the ARM64 build. These tests apply to that component only. Building it does not build Dungeon Hunter 2.
