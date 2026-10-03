# Native reconstruction publication ? 2026-10-04

This is a snapshot of the current native reconstruction work, forked from Noamcelermajer/DH_sc at db4ebe5. Development remains active in the original working directory.

## Source and assets

All current port modules are included: Android application/GLES2 renderer, resource and mesh readers, texture decoding, scene/materials, animation and skinning, game tables, level/world and character kernels, Box2D 2.0.1 physics, and the float32 Lua 5.1.4 runtime. Original-instruction fixtures, bounded comparison reports and sanitizer audits are preserved with the modules. These component proofs have the scope stated in their individual reports.

The Android project's 233 prototype assets and their provenance manifests are committed, so cloning this fork also retrieves the assets used by the current application. Open port/android-native in Android Studio with the SDK/NDK/CMake versions stated in its README and Gradle files. Toolchains, local SDK paths, build intermediates, signing material, private local inputs and duplicate diagnostic captures are excluded from Git. Saved APKs and frozen compiler-input snapshots are attached to the public release.

## Saved APKs

- Latest candidate: dh2-native-ai-timers-alias-b4493ed5.apk, 22,721,062 bytes, SHA-256 b4493ed558fb0956205d000acfee57aef4ba89d1325af288a85579a9dd0a8127. Native timer/AI-event/alias/attack-geometry integration compiles for both ABIs. Build and asset validation pass. Recorded movement, rotation/pause and animation-bank suites pass; the touch-driven combat route stopped against a collision boundary before reaching the attack assertions. This candidate has not replaced the earlier fully verified checkpoint. See [candidate validation](../port/android-native/reports/ai-timers-alias-checkpoint-build-validation.json).
- Last fully emulator-verified checkpoint: dh2-native-prince-bank-4f5b7d11.apk, 20,726,897 bytes, SHA-256 4f5b7d11891e575793f0b5e99056fe5d3cf3cf7a1a705f7ee0f5b632ba19bd82. Four recorded API 37 x86_64 emulator suites pass. It adds all 116 Prince animation resources and 158 original registrations with native two-slot blending. See [checkpoint status](../port/android-native/reports/prince-bank-source-status.md) and [validation](../port/android-native/reports/prince-bank-checkpoint-validation.json).

Current source can contain work newer than either saved APK. Frozen source snapshots and compiler captures identify the inputs of saved builds. Earlier status documents are historical where superseded by the candidate validation report and this publication note.

## Remaining work

This is a limited Crypt gameplay prototype. Complete AI/script/service ownership, inventory/equipment, quests/progression, original UI/audio/saves, all levels and required game assets, original rendering/whole-game parity, and physical modern ARM64 device testing remain unfinished. ARM64 compilation and 16 KiB alignment are verified component/package properties, not a physical-device gameplay result.

[Download checkpoints](https://github.com/AdamCelermajer/DH_sc/releases/tag/native-checkpoint-2026-10-04). Original and third-party provenance and rights are retained; see [RIGHTS.md](../RIGHTS.md).
