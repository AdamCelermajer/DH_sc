# Test 5 local Android check — 2026-10-02

This check used the published Test 5 APK (`e6b81ec649e25bb32c6ec7f3f477d5ef1c2a79b7af43b7ca7643518c5f5e1b8d`) and the separately verified cache ZIP (`3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679`). It did not run on the Galaxy Z Fold7. Raw emulator logs and the exported diagnostic ZIP are retained outside Git because they are test artifacts, not source.

## x86_64 Google APIs API 30 emulator

Official Android Emulator 37.2.12 with the API 30 Google APIs x86_64 revision-16 image booted in 24.5 seconds on this Windows host. The exact APK installed, and its launcher reached `Ready. Import the cache before the first launch.` The reusable launcher runner's `result.json` recorded `booted=true`, `installed=true` and `launcher_ready=true`.

The file picker displayed the 433 MB ZIP but did not return a selected URI in this automation run. This observation alone does not establish a defect in `CacheArchive.install`. To test engine startup separately, all 6,833 validated cache files were placed directly into the disposable emulator's app-specific files tree; the Prince model's SHA-256 matched the input archive. This manual placement does **not** validate the UI import path.

Game launch then failed **before** the translated engine loaded. The guest process is x86_64 while its proxy `libnativeinterface.so` is AArch64. Android reported:

```text
java.lang.UnsatisfiedLinkError: dlopen failed: .../proxy/libnativeinterface.so is for EM_AARCH64 (183) instead of EM_X86_64 (62)
```

The runtime report recorded zero proxy loads, zero `JNI_OnLoad` calls and zero GL calls. Android classified the guest process exit as a crash. This is an emulator architecture mismatch, not a result for the Test 5 model-path repair or gameplay. An exported diagnostic ZIP was CRC-checked and retained locally (SHA-256 `00453c7dcfedfba1c60562754a703ba4e0ec01bc1cb689dabc87525c648e5fbc`).

## ARM64 Google APIs API 30 emulator attempt

The official ARM64-v8a revision-16 system image installed in a separate AVD, but Emulator 37.2.12 on this x86_64 Windows host exited before Android boot:

```text
FATAL | Avd's CPU Architecture 'arm64' is not supported by the QEMU2 emulator on x86_64 host. System image must match the host architecture.
```

No ARM64 emulator process or guest game code ran. The next runtime gate is the existing Fold7 Test 5 procedure in [`LOCAL_AGENT_HANDOFF.md`](../LOCAL_AGENT_HANDOFF.md): install over Test 4 without clearing its cache or saves, repeat the same load/settings, and export diagnostics. The separate source-component test results against the newly available complete cache are in [`COMPLETE-CACHE.md`](COMPLETE-CACHE.md).
