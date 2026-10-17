# Chaos test bug list (owner M, 2026-10-16)

Tooling: tools/trackb/boot/chaos.py (seeded action stream), chaos_emu.sh (private AVD dh2_m, emulator-5558), chaos_build_apk.py
(APK from build/own-m-arm64). Build: DH2_BOOT_DEBUG=ON, DH2_ANDROID_FORTIFY=ON. Sessions are in build/own-m/sessions/s<seed>/ (untracked).
Run: `py -3 tools/trackb/boot/chaos.py --serial emulator-5558 --seeds 2 3 --mode game --acts 80`.

| id | symptom | repro | top frames | root cause | status |
| --- | --- | --- | --- | --- | --- |
| M1 | SIGSEGV addr 0 on the UI thread, main-thread tap while the GL thread tore down the Application (many ~CMaterialRenderer just before) | seed 11 (act 121, tap burst after in-game menu / main-menu spam) | appOnTouch (w7_global_02.cpp) <- nativeOnTouch | Application::m_touchScreen is deleted+zeroed in the teardown (g1-game-app-Application-b291be-2.cpp:382); the binary's appOnTouch (0x52f11c) derefs it unchecked (the same race exists in the original, unlikely there) | fixed: null guard (deviation, commented) |
| M2 | ANR "Input dispatching timed out" while a level loads | seed 4 (twice, during loading after kill+relaunch with tap spam) | main thread blocked during the 40-90 s load on ndk_translation | environment (emulated arm64, load inside the input path), not a logic fault | noted, no action |
| M3 | "Could not open file .../files/qata/3d/textures/X.tga" after every missing texture | any new game | FileSystemBase::ApplyFilenameHacks | by design: the binary rewrites ".tga" names to "ata/3d/textures/<base>" after the working-directory prefix (listing 0x34ea40). Missing textures themselves (tex_prince*.tga, tex_faerie_001, fx_spark, char_faerie, env_swamp_alpha, mc_torso_default_warrior.bdae) are absent from the cache zip | not a bug of the code; data gap, for V1 if they matter |
| M4 | DH2DBG "E" updateData lines | all | CTexture::updateData trace | debug trace logged at ERROR level, not a fault | ignore |

Sessions with no finding: seeds 1-3, 5-7 (kill/relaunch, HOME and back, rotate, trim-memory, monkey bursts, stick/attack/skill/potion spam, menu spam).
