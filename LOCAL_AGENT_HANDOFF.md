# Local agent handoff — Dungeon Hunter 2

Checkpoint: 1 October 2026. This document describes the compatibility work and
the continuation boundary. Start with the Fold7 Test 5 device retest below.
For the independent native-source roadmap, see
[RECONSTRUCTION-HANDOFF.md](RECONSTRUCTION-HANDOFF.md), added concurrently and
preserved here. Its next bounded milestone is image/material decoding and a
minimal renderer.
The user has asked to continue the work, commit changes and publish test releases
to `Noamcelermajer/DH_sc`. No local PC terminal or desktop was accessible in the
previous session; its Android tools ran in a Linux workspace.

## First actions

1. Clone or pull the latest `main`. Preserve other work already on the branch.
2. Read this file, `compatibility/work/fold7-build/TEST5.md`, and
   `compatibility/work/fold7-build/ANDROID_TESTING.md`.
3. Download the existing Test 5 APK. Verify its hash before changing code.
4. Connect the Fold7 through adb and reproduce the same load using the existing
   cache and options. Test 5 has not yet been confirmed past that load.
5. Export the new diagnostic ZIP. Let its model-open evidence determine the
   next change. Do not infer a graphics-library cause merely from a loading
   screen or an earlier GL error.

If only source work is possible, prepare the workspace using the helper below.
It makes the missing `original/lib` inputs available without overwriting Git
sources. Start with the prebuilt, verified runtime instead of rebuilding the
translator before obtaining new evidence.

## Current state and scope

| Track | Current state | Main entry points |
| --- | --- | --- |
| Fold7 compatibility | Original ARM32 game hosted in an ARM64 ZettaBridge/Dynarmic wrapper, with targeted Java/C/assembly fixes; signed Test 5 published; loading/gameplay unverified | `compatibility/work/fold7-build/` |
| Native engine reconstruction | Independent C++ math, resource and mesh/animation components, with original-instruction comparison reports; not an integrated game | `port/engine-math/`, `port/engine-resources/`, `port/asset-payloads/` |
| Local Android testing | Emulator and adb installed; API 30 software AVD booted; APK install failed before launch with package-service `Broken pipe (32)` | `compatibility/evidence/emulator-test5/`, `ANDROID_TESTING.md` |

The C++ component test counts do not validate the wrapper APK. The wrapper's
native probes do not validate ART, graphics presentation or gameplay. Neither
track is a completed reconstruction of the studio's original source.

## Published artifacts and exact identities

Release: https://github.com/Noamcelermajer/DH_sc/releases/tag/v1.0.2-fold7-test5

| Item | Identity |
| --- | --- |
| Test 5 source/tag commit | `26964b63b690624936b90ec71f3328432a9e964e` |
| Independent mesh/animation checkpoint | `3fb8bc4af8c14071accdb6f0e7d57658d8f65347` |
| Emulator runner/report commit | `ded2115ee60048666e5fd146f80693613e70dadb` |
| Application | `local.dh2.fold7`, versionCode `5`, versionName `1.0-test5` |
| APK | `Dungeon-Hunter-2-Fold7-test5.apk`, 13,769,952 bytes |
| APK SHA-256 | `e6b81ec649e25bb32c6ec7f3f477d5ef1c2a79b7af43b7ca7643518c5f5e1b8d` |
| Release work ZIP SHA-256 | `6a4eb1c3aaf754fe47cbaea6fa41d8640f32a1596488530ebeb36d9934c5d3eb` |
| Git snapshot ZIP SHA-256 | `9ce519019b2e34d4b777455a10b57a50f4dd0a32c82aa9b1892abc9d9c522440` |
| Original input APK SHA-256 | `32c2d027b585a42547311cd95da6a3975fdb3174e663e513d42a7f49d1a4c200` |
| Original engine SHA-256 | `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80` |
| Test 5 engine SHA-256 | `45891aad9e7a5b1d84a5218f91926a04bd13a62ce104cb29beca7c70228c93c4` |

Use current `main` for this handoff and later test tooling. The release work ZIP
and immutable Test 5 snapshot predate this handoff; they are the release's build
checkpoint, not a replacement for pulling current source.

## Why Test 5 exists

The owner's Test 3 session aborted after the cinematic, while the fairy loading
screen was visible. The real ARM32 `CFileSystem::open`/`CFile` path was independently
reproduced: a pathname ending in `/` reaches the basename bounds check and aborts.
Test 4 rejects directory-shaped paths at the engine's `fopen` import with EISDIR.
The subsequent Test 4 phone report confirms that guard executed.

Test 4 then failed differently: SIGSEGV/status 139, PC reported as zero, LR
`0x60b2e4` in `glitch::collada::COnDemandReader::read`. Immediately before that,
reopening `prince_modular.bdae` failed with a duplicated cache root. The original
engine recognizes a colon as absolute-path syntax, but not Android's leading
slash, and prefixes nonempty `WorkingDirectory` again during deferred reads.

Test 5 replaces exactly five ARM instructions / 20 bytes at `0x56dd9c`. It retains
the original colon and relative-path branches and recognizes leading `/`.
`patch_engine.py` verifies the whole original hash and expected instruction bytes
before patching. No function addresses or serialized layouts move. This is the
first compatibility build that modifies `libDungeonHunter2.so`; older statements
that it is byte-identical describe Tests 1–4.

Additional Test 5 diagnostics record model existence/size/first 32 bytes before
startup and successful/failed `prince_modular.bdae` opens. Native logging preserves
errno and writes to captured fd 2 as well as Android liblog.

Relevant facts from the Test 4 phone run:

- SM-F966B, Android 16; previous device reports establish 4096-byte pages.
- `fit16by9=false`, `preserveContext=false`, surface 2184x1968; context recreated
  after video playback. The Test 3 comparison used different options.
- 11,121 GL calls and an earlier `GL_INVALID_ENUM` do not establish the cause of
  the later null-read crash. The stack candidates are not an unwound backtrace;
  fault registers are not guaranteed to describe the exact faulting instruction.
- libc report offsets must be mapped through ELF PT_LOAD; they are not blindly
  interchangeable with virtual addresses. The engine text offsets used here match.

## Evidence map

Paths below are under `compatibility/work/fold7-build/` unless stated otherwise.

| Evidence / implementation | Files |
| --- | --- |
| Latest diagnosis and device procedure | `TEST5.md` |
| Earlier abort and bridge assessment | `TEST4.md`, `TEST3.md`, `BRIDGE-ASSESSMENT.md` |
| Original phone evidence | `phone-test3/`, `phone-test4/`; included in the verified snapshot |
| Absolute-path repair | `patch_engine.py`, `engine_path_fix.S`, `engine_path_fix.ld`, `engine-patch-report.json` |
| Private-linker repair, directory guard, model-open logging | `patch_storm.py`, `storm_import_fix.c`, `storm_bias_fix.S`, `storm_bias_fix.ld` |
| Guest Java paths/media/render observations | `patch_game.py`, `guest-java/local/dh2/compat/` |
| Launcher/import/export | `java/com/zettabridge/launcher/Dh2Activity.java`, `CacheArchive.java`, `Dh2Diagnostics.java` |
| Translator source changes | `zettabridge-dh2.patch`, plus readable copies in `compatibility/upstream-modified/` |
| 13 original/fixed rooted-path cases | `tests/run_path_open_tests.py`, `tests/dh2_file_open_probe.c`, `path-open-tests.json`, `path-open-*.log` |
| Two focused final model-logging cases | `path-open-tests-final-smoke.json`, corresponding logs |
| Five original/guarded file cases | `tests/run_file_open_tests.py`, `file-open-tests.json`, `file-open-*.log` |
| Library loading, hooks and 4096 helper inputs | `run_native_probe.py`, `native-probe-test5.log`, `native-load-test.log` |
| Deliberate abort/exit/signal tests | `tests/test_diagnostics.py`, `diagnostic-tests.json` |
| Package validation | `verify_package.py`, `validation.json`, `build-result.json`, `apk-badging.txt`, `runtime-manifest.json` |
| Emulator attempt | `ANDROID_TESTING.md`, `emulator-smoke-test5.json`, `tests/run_emulator_smoke.py`, `compatibility/evidence/emulator-test5/` |

Native path probes read synthetic fixture bytes through the real engine and
translator; they do not parse a real BDAE or draw it. JNI_OnLoad probing used an
inert VM for those entry points, not a functioning ART environment.

## Prepare a clean local work directory

Linux x86_64 is the recorded build host. WSL2 can host builds on Windows; connect
the phone with the host's working adb installation as appropriate. Do not assume
USB forwarding or acceleration is already configured.

```sh
git clone https://github.com/Noamcelermajer/DH_sc.git
cd DH_sc
python3 tools/prepare_local_agent.py ../DH2-local-work
```

The destination must be new and outside the clone. The helper verifies all 2,195
snapshot entries, overlays current tracked compatibility files, checks the runtime
bundle, and copies the three original ELF libraries from `decoded-original/lib`
to `original/lib`. The historical snapshot lacked that latter directory, although
the patch scripts require it. `RESTORE-REPORT.json` records the actual source
overlay and hashes. No downloads/builds are performed by this helper.

Your build root is now `../DH2-local-work/compatibility/work`. SDK/NDK/JDK, the
upstream checkout, a release APK and the complete game cache are separate inputs.
Use the latest Git checkout as the source of truth; copy authored edits back to
the matching `compatibility/work/...` paths before committing.

`unpack_compatibility.py` remains an exact historical restorer: it refuses files
that differ from its snapshot. Do not force it over newer Git edits. Older build
READMEs are preserved as historical evidence; this handoff resolves the layout
and revision ambiguities for continuation.

## Fastest next check: the real Fold7

Download the APK and checksum file from the Test 5 release (or use `gh release
download v1.0.2-fold7-test5 --repo Noamcelermajer/DH_sc` in a download directory).
Verify the APK hash above. Once the phone authorizes USB debugging:

```sh
adb devices -l
# Replace PHONE_SERIAL with the observed device serial in every command.
adb -s PHONE_SERIAL shell getprop ro.product.model
adb -s PHONE_SERIAL shell getprop ro.build.version.release
adb -s PHONE_SERIAL shell getprop ro.product.cpu.abilist
adb -s PHONE_SERIAL shell getconf PAGESIZE
adb -s PHONE_SERIAL install -r Dungeon-Hunter-2-Fold7-test5.apk
adb -s PHONE_SERIAL shell am start -n local.dh2.fold7/com.zettabridge.launcher.Dh2Activity
```

Install over Test 4; do not uninstall, clear app data or reimport the cache on
the existing phone. The package and development signing key are unchanged.
If an upgrade is rejected for a signer mismatch, stop and inspect signatures;
uninstalling is not the remedy while saves need preservation.

Keep the last Test 4 options: both display/context checkboxes off. Repeat the
same character/load action. Export `DH2-test5-diagnostics.zip` immediately after
the run, successful or failed. After a crash, reopen the launcher and export
before another run. Optional terminal capture while testing:

```sh
adb -s PHONE_SERIAL logcat -v threadtime > test5-device-logcat.txt
```

Stop that capture after the run. The app's exported guest report is essential:
a translated abort may terminate the host normally and produce no Android native
tombstone. Inspect `DH2Model opened:`, `DH2Model open failed:`, model preflight,
guest stderr, failed opens, PC/LR and offsets together.

| New result | Continue here |
| --- | --- |
| Model preflight missing / open still fails | Check exact filename, case-folding aliases, single root, file size and cache variant; trace the failed original read path |
| Model opens and failure advances | Symbolize the new engine PC/LR/call-site candidates; add a targeted original-code regression before changing behavior |
| Load succeeds | Check movement, combat, inventory, save/reload and another level; export evidence; then test graphics/language options one at a time |
| Failure involves context/shaders after model opens | Correlate Java surface milestones, EGL context lifetime and actual GL error-producing calls; do not disable graphics hooks wholesale |

Open items: Russian menu text despite English callback, inner-screen aspect/touch
alignment, graphics-context behavior, earlier GL enum error, later gameplay and
16 KB page support. The current runtime requires 4 KB pages; 16 KB ELF alignment
does not establish 16 KB runtime support. Keep the page-size guard.

## Rebuild the wrapper after evidence justifies a change

From the prepared `compatibility/work` directory, install these inputs in the
locations the scripts use:

| Input | Required path/version |
| --- | --- |
| NDK | `toolchains/android-ndk-r29` (29.0.14206865; preserve archive symlinks) |
| Java | one matching `toolchains/jdk-17*` directory; tested Temurin 17.0.20.1+1 |
| SDK | `android-sdk/platforms/android-35/android.jar`, `android-sdk/build-tools/35.0.0/` |
| Python | 3.12+, `pyelftools==0.33`, `capstone==5.0.9` |
| apktool | `tools/apktool.jar`, v2.12.1; `python download-tools.py` verifies pinned downloads |
| HiddenApiBypass | `downloads/hiddenapibypass-6.1.aar`; extract `classes.jar` as `downloads/hiddenapibypass.jar` |
| Upstream | `research/ZettaBridge`, public base `7c647a4f1ea150eab7978ab0da28fdf49f3a79de` |

`fold7-build/download-hashes.json` records archive names, sizes and hashes, not a
complete URL manifest. `tool-manifest.json` contains the apktool/signer URLs.
Use official SDK/NDK distributions and the recorded versions; do not guess that
a newer toolchain produces identical embedded machine code.

```sh
python -m pip install pyelftools==0.33 capstone==5.0.9
python download-tools.py
mkdir -p research
git clone https://github.com/ZailoxTT/ZettaBridge.git research/ZettaBridge
git -C research/ZettaBridge checkout 7c647a4f1ea150eab7978ab0da28fdf49f3a79de
git -C research/ZettaBridge apply ../../fold7-build/zettabridge-dh2.patch
git -C research/ZettaBridge submodule update --init --recursive
python -m zipfile -e fold7-build/runtime-bundle.zip research/ZettaBridge/build/launcher
```

Read that checkout's instructions before editing it. The cumulative patch is the
transportable source change. `3094fd150ad7fe5891fd3741fd2f43892424277f` is the
previous session's local integration commit, not a guaranteed public upstream
object. Do not try to fetch it from upstream or reapply the cumulative patch
over the readable `upstream-modified` copies.

With Java 17 selected in your environment and all inputs placed above:

```sh
python fold7-build/patch_game.py
java -jar tools/apktool.jar b fold7-build/game-tree -o fold7-build/game-unsigned.apk
python fold7-build/build_apk.py
python fold7-build/verify_package.py
```

Run dependent steps sequentially and stop on the first error. The APK goes to
`compatibility/deliverables/` in the prepared layout. Verification requires the
tested runtime bundle and recorded native-load log; that log check is historical
evidence, not a fresh native test. ZIP timestamps/tool differences may change the
outer APK hash on rebuild; check native hashes, package contents, signer and
alignment rather than silently replacing the pinned release.

For a new Test 6, update versionCode/versionName, APK filename references, launcher
REVISION/preparation marker, labels and diagnostic-export filename consistently
in `build_apk.py`, `verify_package.py`, `Dh2Activity.java` and related reports.
Retain the same package and development signing identity for an upgrade. The
public test key is a fixture, not a production or Gameloft signing key.

## Optional native/runtime regression reproduction

A runtime rebuild needs CMake/Ninja, Boost 1.83 headers at
`toolchains/boost/usr/include`, the pinned Dynarmic submodule
`86458a0bd369d63ba4c2ef812cacbb6c9080c065`, and both patches under
`research/ZettaBridge/third_party/patches`. Inspect and apply those to the submodule
once as upstream documents. `python fold7-build/build_runtime.py` builds the
ARM64 core/CLI, guest libraries and launcher bundle. Keep the verified bundle
until the rebuilt one has passed the native tests.

The original reference probes also require `toolchains/qemu8/usr/bin/qemu-aarch64-static`
(8.2.2) and the matching Android ARM64 bionic loader/libraries under
`host64-runtime/`. Their file hashes are in
`compatibility/REFERENCE_RUNTIME_MANIFEST.json`. These reference-host files and
toolchain executables are external inputs, not included or installed by the
workspace-preparation helper. Host Linux glibc cannot substitute for Android
bionic. Real phone testing can proceed without this QEMU reference environment.

`build_runtime.py` populates the ARM32 `research/ZettaBridge/sysroot` from the
verified bundle if absent. Then:

```sh
python fold7-build/run_native_probe.py
python fold7-build/tests/run_file_open_tests.py
# Prepare the Test 4 baseline described below before this command:
python fold7-build/tests/run_path_open_tests.py
python fold7-build/tests/test_diagnostics.py
```

`run_native_probe.py` creates the current `probe-libs` directory. The 13-case path
runner additionally expects `fold7-build/probe-libs-test4/`. Restore its three
original/patched guest libraries from `assets/dh2/game.apk` inside the published
Test 4 APK, then apply the checkout's `tools/fix_guest_lib.py` as the probe setup
does. Baseline engine SHA-256 is the original hash above; baseline Storm SHA-256
is `5a9412b520237995e2a6513694170400eeffff93a0491be8afdf421d14f793b5`.
Do not accidentally use the Test 5 engine for the failing baseline.

## Emulator findings and next local attempt

The previous workspace was Linux x86_64 with no `/dev/kvm`, hardware GPU or USB
device exposure. Platform Tools 37.0.1, Emulator 37.2.12, and Google APIs API 30
x86_64 image revision 16 were installed. The image reported ARM64 native-bridge
support. Software CPU + SwiftShader booted in 224.1 seconds in the recorded run,
but installation failed with `Broken pipe (32)` and subsequent logcat capture
timed out. No game code was observed executing. The initial boot logcat is from
an earlier attempt and must not be mislabelled as a game crash log.

On a capable local host, verify acceleration with `emulator -accel-check`. Use a
system image whose supported ABIs include ARM64 for this APK. A successful
launcher boot alone still does not prove that nested ARM32-to-ARM64 translation
works through an x86 native bridge. Prefer the actual Fold7 for its driver and OS.
The reusable smoke runner is documented in `ANDROID_TESTING.md`; it does not
import cache or test gameplay. A non-streamed adb install is an untried fallback
for investigating the installer failure, not a proven game fix.

## Cache and reconstruction boundaries

The phone already has an imported cache. Preserve it for the Test 4/Test 5
comparison. Its expected app-specific root is:

```
/storage/emulated/0/Android/data/local.dh2.fold7/files/plugins/com.gameloft.android.GAND.GloftD2SS/
```

The saved complete ZIP records are 433,189,197 bytes each, but both download
endpoints failed with HTTP 502/connection refused in this session. No complete
ZIP was recovered into this compatibility workspace. The ten 30 MiB split parts
used by the independent reconstruction total 314,572,800 bytes and are only a
prefix: they end inside `data/sounds/m_world_map.wav` and omit the central directory.
Its 2,901 recovered BRES files are a usable corpus, not proof of a complete game
cache. Do not import that partial ZIP as a substitute for the phone's cache.

The independent reconstruction modules and reports are already on `main`. Start
at `port/asset-payloads/README.md` and `FORMATS.md` if continuing that track. Next
work there includes nine unsupported type-1 geometries, scene/controller and
material/image decoding, skin/skeleton/animation application, GPU ownership and
rendering. Those interfaces deliberately do not reproduce the original ARM32
class ABI. Do not link them into the wrapper as drop-in engine replacements.

Some old root-README paths (`recovered/`, general `tools/unpack_native.py`, and
full decompiler/assembly bundles) refer to the separate recovery package linked
there, not directories present in this Git checkout. The port modules carry their
own assembly evidence and comparison reports. Obtain the separate recovery
package if its wider export is needed; do not claim it is already checked out.

## Finish each continuation with an auditable checkpoint

Keep input/library hashes and original instructions. Preserve earlier APKs and
diagnostics. State the precise observed failure and change, run the matching
regression, and record device/ABI/page size, options and cache identity for device
results. Commit source, new evidence and updated status; publish a new prerelease
with checksums if the APK changes. A compile, emulator boot or component-level
test is not a gameplay pass. Preserve unrelated native reconstruction work when
updating `main`; never force-push this shared branch for the handoff.
