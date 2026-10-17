# Android Java reconstruction audit

The supplied APK's Android layer was recovered independently with APKTool 2.12.1 and JADX 1.5.6. Every one of the 357 original DEX classes has a smali file and a matching unedited Java file. The original DEX contains 2,432 defined methods; 46 are native declarations. JADX also synthesized seven resource classes.

The repaired source is `port/android-java/java`. Its 288 Java files include original classes represented as nested Java types and one authored `WebSettingsCompat` helper. The repair starts from the same APK, not earlier compatibility-patched APKs.

## Verified build

| Check | Result |
| --- | --- |
| Full source compilation | Pass: JDK 17.0.20.1, `javac --release 17`, Android SDK platform 35 and its Apache HTTP legacy JAR. |
| Compiler output | 365 class files from a new output directory; zero errors; 15 legacy removal warnings and deprecation/unchecked notes. |
| DEX conversion | Pass: Android build tools 35.0.0 D8, release mode, minimum API 23. |
| Native declaration comparison | 46 expected and 46 rebuilt; identical declaring class, method name, parameter/return descriptors and staticness; zero missing or extra contracts. |
| Runtime/gameplay | Not tested. |

The exact DEX size/hash and native comparison are recorded in `java-port-dex.json`. The full compiler diagnostic is `java-port-javac.log`. Source file hashes are recorded in `java-port-source-manifest.json`. Portable build commands are in `port/android-java/README.md`.

## Main-layer repairs

| Source/method | Reconstruction and bytecode evidence |
| --- | --- |
| Synthetic access bridges | JADX `--no-inline-methods` retains the original `access$...` bridge methods and call sites. Private Bluetooth/device/billing state was not widened to make decompiler-inlined callers compile. |
| Resource references | Fully qualify `com.samsung.zirconia.R` so obfuscated fields named `R` cannot shadow the original resource class. Numeric values and resource identities are retained. |
| `GLMediaPlayer.isSoundLoadedBig` | Restore integer return `-1` on missing/unusable player states and `0` otherwise; smali's `v1` branch and `v0` return confirm this. JADX produced an invalid boolean/int ternary. |
| `GameGLSurfaceView.ConfigChooser`/`ContextFactory` | Make nested types static; their original constructors have no enclosing-instance argument and their DEX definitions contain static state. |
| `InAppBilling.save`/`saveLastItem` | Restore the boolean-string array declaration before its first use; preference keys, encrypted values, timestamps and status codes are retained. |
| `VZBilling.parserXML`/`bw.run` | Remove JADX-invented checked throws clauses for XML operations already caught inside the parser; no catch behavior or billing conditions changed. |
| `ResLoader.getInputStream` | Restore `trimName` outside the asset-open try block and the catch branch's null return, matching the original exception range and return register. |
| `SUtils.getOverriddenSetting` | Restore the read loop until EOF or the first matching key, then parse after `=`, close both readers, and return the line/null. JADX lost the branch around its early return. |
| `SUtils.readTextFile` | Restore the initial null value used when stream reading/decoding fails; a successfully decoded string survives a close failure as in smali. |
| `ZipFile.extract`/`unZip` | Restore the EOF exits (`read < 0` and `read == -1`, respectively) before original flush/close operations. |
| `IGPActivity.c`/`getSerialNo` | Restore null returns on caught failures; no device-identifier selection or preference logic was changed. |
| Gameloft `billing/common/Base64` | Remove a duplicated unreachable break and restore checked exception propagation in decode wrappers. The decoding algorithm remains the recovered implementation and passed focused vectors. |
| `SamsungIABActivity.onItemInformationListReceived` | Restore the first matching-item exit and no-item EOF exit. The purchase request and failure callback stay behind their original item-match decisions. |
| `SamsungIABActivity.onPurchasedItemInformationListReceived` | Restore the inner list's exhaustion exit; managed-item checks, reflection callback values and failure result are retained. |
| `GameInstaller.c(int)` | Restore the common exit after layout dispatch and the special re-dispatch to layout 23 in case 21. JADX turned the common path into an unconditional infinite loop. |
| `GameInstaller.y()` | Restore separate exception ranges for external-file-directory probing and `/proc/mounts` reading; open the mount reader before use, retain path filtering and final fallback path. |
| `GameInstaller` list/error references | Recover `ArrayList<DownloadComponent>` and `Vector<Pair>` element typing. Fully qualify shadowed error-code classes `p/q/r/s/u/v`; retain their numeric codes. |
| `HttpClient`, counted input streams `a/b` | Restore checked I/O exception declarations around existing URL/open/read/skip calls. Exceptions propagate to original caller catches instead of being suppressed. |
| `LicenseRetriever.open` | Restore `MalformedURLException` declaration for the existing URL construction; the original `retrieveLicense` catch already handles it. License request, verification and return-code behavior are retained. |
| `WebSettingsCompat` | Intentional SDK adaptation: call removed `setAppCacheEnabled` by reflection when available; no-op only when the method is absent. Invocation failures are rethrown. |

## Cache and vendor evidence

The download/cache methods needed repairs beyond compiler diagnostics. `DownloadComponent.a(f, boolean, boolean, String)` was reconstructed from the full smali control flow: missing-size checks, CRC inversion, direct MD5 result, split-file marker calls, the partial-resume-length branch, reset/deletion scope and the final return expression. The unusual original MD5 convention was preserved rather than redesigned. `DownloadComponent.a(String, int)` restores a single iteration increment and the maximum-size accumulator.

`Utils.hasBeenDownloaded` restores the immediate true return on the first marker match. `CRC.calcChecksum` restores sibling checked exception handlers and the original 128-byte read buffer. `Section.h` restores the stream-retention/reset branches and timeout/general error behavior; `Section.run` factors the duplicated original initialization through `h` and retains the extraction and cleanup branches. An independent agent compared these methods to the original smali and found no differing state/error branch. See `installer-integrity-audit.md`.

Vendor-specific repairs, including original synthetic authorization accessors, Binder exception contracts and payment parser loops, are documented in `java-port-vendor-repairs.md`.

Focused tests cover identified decompiler damage:

- KDDI Base64: 16,384 flag/length cases, range overloads and seven malformed inputs.
- Gameloft Base64: RFC/explicit-alphabet vectors, 3,072 random padded/unpadded cases, ranges and six malformed inputs.
- CRC: 1,296 known/random/chunk/deletion/missing-file checks.
- Actual `DownloadComponent` and `Utils`: 30 size/checksum/split-marker/resume/reset/deletion branch checks, using a clearly separated test-only path/error-log fixture; checksums and marker logic are the production code.

All these tests passed. Their sources and standalone commands are under `tools/tests/`. The test-only `GameInstaller` fixture is not in the production source tree and is not compiled by either production build tool.

## What this does not establish

The Android Java layer has a successful compile and DEX conversion, and all declared native method signatures match. Other JNI method/field lookups and reflection strings have not been verified. JADX renamed some obfuscated members, so matching only native declarations does not establish every cross-language contract.

The Android runtime, permissions/storage changes, obsolete Samsung/KDDI account and billing services, full native engine, graphics/audio behavior and gameplay have not been exercised. Java compilation and selected host tests do not prove semantic equivalence for every recovered method, and this work is not a complete modern Android game port.
