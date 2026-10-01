# Android 17 emulator results — 2026-10-02

The primary current-platform test used official Android 17/API 37 Google APIs x86_64 system images in separate Pixel 9 phone AVDs. No physical phone or Fold7 device was used. Both AVDs were given 4 GB RAM. The project-local SDK installation supplied Android Emulator 37.2.12, platform tools, build tools and NDK r29. See [the test plan](ANDROID-17-EMULATOR-PLAN.md) for the APK routes and source references.

| AVD | Official image | Page size | Advertised ABIs | Result |
| --- | --- | ---: | --- | --- |
| `DH2_API37_2_X86_64` | `system-images;android-37.2;google_apis_ps16k;x86_64`, revision 5 | 16,384 | `x86_64,arm64-v8a` | ARM32 guest rejected by package manager. Published ARM64 wrapper installed and showed its launcher, then its own runtime guard refused 16 KiB pages. Reconstructed asset readers passed a separate native x86_64 runtime check. |
| `DH2_API37_0_X86_64` | `system-images;android-37.0;google_apis;x86_64`, revision 6 | 4,096 | `x86_64,arm64-v8a` | ARM32 guest rejected by package manager. Published ARM64 wrapper installed and showed its launcher, but game startup failed while loading its AArch64 proxy into an x86_64 plugin process. |

## APK checks

The emulator-only signed Test 7 guest (SHA-256 `aa0628e277b7bc17aaa2273a257c3c624af52c525cdb9e606779cea42a85142a`) has `armeabi-v7a` native libraries and target SDK 24. `adb install` on each AVD returned `INSTALL_FAILED_NO_MATCHING_ABIS` with native-library extraction result `-113`. Thus its Android 9 menu behavior cannot be replayed directly on these Android 17 images.

The published Test 5 wrapper (SHA-256 `e6b81ec649e25bb32c6ec7f3f477d5ef1c2a79b7af43b7ca7643518c5f5e1b8d`) has `arm64-v8a` host libraries and target SDK 35. It installed on both AVDs with `adb install --abi arm64-v8a`, and `dumpsys package` reported `primaryCpuAbi=arm64-v8a`. On the 16 KiB image, Launch Game displayed: `This translation runtime currently requires 4096-byte memory pages. This phone reports 16384.` This is an explicit guard in the wrapper's Java source, before guest engine startup.

On the 4 KiB image, Launch Game started the plugin path, then Android's native loader reported `libnativeinterface.so is for EM_AARCH64 (183) instead of EM_X86_64 (62)` and threw `UnsatisfiedLinkError`. This happened before cache access or gameplay. Installing with `--abi arm64-v8a` did not make that plugin process accept a directly loaded AArch64 proxy. The error also occurred in the older Android 11 x86_64 diagnostic AVD. The cache was not imported on either Android 17 AVD because these startup failures precede game asset loading.

## Reconstructed source runtime check

An independent x86_64 executable built from the reconstructed PVRTC decoder and BRES/material readers ran on the 16 KiB API 37.2 AVD. Synthetic PVRTC1 2bpp/4bpp and material/effect/sampler checks passed. The owner-supplied 1024×1024 atlas texture and 16,856-byte candle BRES also passed, with on-device SHA-256 values matching the host inputs. The executable's four load segments are 16 KiB aligned; it exited with code zero. See [`port/android-smoke/runtime-validation.json`](../port/android-smoke/runtime-validation.json) and its [reproduction steps](../port/android-smoke/README.md). This validates isolated new source components on current Android, not the original engine or a game scene.

The verified Android 17 result is therefore **launcher startup and isolated reconstructed asset-reader execution**. A playable Android 17 game has not been demonstrated. The next native-startup path is an x86_64-capable proxy/translator or a genuinely ARM64-hosted Android environment; the 16 KiB page runtime also needs a separate compatibility audit. Renaming an ARM32 or AArch64 library does not change its machine code.
