# Verified historical source import

The older [Drive recovery handoff](https://drive.google.com/drive/folders/1njTxLAJHt08SinEskn7gC6IWylXemwDW) was retrieved on 2026-10-02. Its `Dungeon-Hunter-2-Source-Recovery.zip` passed SHA-256 `b3ff974e2b74f50387465d5665f60d56ac79c29a449c6299745461998045c4d8` and a full CRC check of 3,388 members (79,687,588 uncompressed bytes). `assembly.tar.gz` and `symbols.tar.gz` matched SHA-256 `e7bdc73d7db5c0b8320815f92c7d27c9c86c17710649645c027a4df4081e5601` and `f2dd9d64a9f19a66cfd05e194d30c7482c399ef07d7b488d737ac929652481ca`. The bundle archives remain outside Git.

The archived `tools/unpack_native.py` restored 3,625 assembly and 71 symbol files in a private working tree. Archived `tools/verify_recovery.py` passed: all 31,018 engine, 1,498 Storm and 10 JNI original named starts were attempted; 31,794, 3,332 and 31 pseudocode functions respectively were emitted, with one engine and one Storm export failure. It also checked 2,164 recovered text/shader hashes and 288 repaired Java source hashes. This is export accounting, not semantic correctness or a build of the game.

## Files made browsable in Git

The [per-file import manifest](../reports/recovery-source-import.json) records 333 selected files (about 1.65 MB) and both their original ZIP-member SHA-256 and current checkout SHA-256. The selection includes:

- 288 repaired/decompiled Java source files and their [README](../port/android-java/README.md).
- The 14-file reconstructed [JNI source component](../port/nativeinterface/README.md), including its build and focused test sources.
- Recovery/build scripts, Ghidra scripts, the historical JNI/Java validation reports and the two native bundle checksums.

The first selected import did not include raw code exports. A later archival-code import makes 44 [Ghidra pseudocode/index/summary files](../recovered/native/decompiled/README.md) and 721 [raw Java/smali files](../recovered/android/README.md) browseable in Git. They were copied byte-for-byte from the same verified ZIP, with separate per-file SHA-256 manifests and verification scripts. Another import makes the exact 3,625 [native assembly files and 71 symbol records](../recovered/native/README.md) browsable from the two verified bundles, with a separate manifest and verifier. The pseudocode and generated assembly are not a compilable or validated engine; raw Java has known decompiler damage. A subsequent [text and debug import](RECOVERED-TEXT-AND-DEBUG.md) adds all 2,164 recovered shader/configuration resources and 48 native debug exports from this ZIP. The compressed bundles, supplied APK and game cache remain separate. See [the reproduction guide](REPRODUCING.md) for the external handoff. The complete later cache ZIP is a separate local input. None of these imports implies an open-source license for the game; [RIGHTS.md](../RIGHTS.md) records provenance.

## Verify

From the Git checkout:

```sh
python tools/verify_recovery_import.py
python tools/verify_recovery_import.py --archive /path/to/Dungeon-Hunter-2-Source-Recovery.zip
python tools/verify_native_decomp_import.py --archive /path/to/Dungeon-Hunter-2-Source-Recovery.zip
python tools/verify_android_raw_import.py --archive /path/to/Dungeon-Hunter-2-Source-Recovery.zip
python tools/verify_native_evidence_import.py
python tools/verify_native_evidence_import.py --bundle-dir /path/to/bundles
python tools/verify_remaining_recovery_import.py --archive /path/to/Dungeon-Hunter-2-Source-Recovery.zip
```

The first command hashes all imported files without external inputs. The second also checks the exact source ZIP hash, every ZIP member CRC, and each corresponding original member. Paths in the import manifest are validated before reading.

For a current Android SDK source check, the imported Java was compiled against platform 37.0 and converted by Build Tools 35.0.0 D8. [The report](../reports/java-port-api37-validation.json) records 288 source files, 364 class files, 15 compiler warnings, a 572,124-byte DEX and 46 of 46 matching native declarations. CMake/Ninja host JNI semantic tests passed after portable binary-mode file handling; NDK r29 cross-built the JNI library for AArch64 with 16 KiB load alignment. None of these isolated checks proves a playable modern Android game.
