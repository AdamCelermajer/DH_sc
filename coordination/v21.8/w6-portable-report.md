# w6-portable: Linux / non-MSVC portability of port/windows-foundation

Branch `v21.8-w6-portable` (worktree `.claude/worktrees/w6-portable`, from `windows-foundation` 5b1437a3). Commits `a22b5ada` (fixes + runner script), `af436cf2` (LF rule for the .sh), then this report. Nothing pushed, nothing merged.

## Verdicts

| Area | Verdict | Evidence |
|---|---|---|
| Linux gcc 15 build, all targets incl. all tests (WSL Ubuntu, ext4) | **WORKED** after fixes | Before fixes: 8 test translation units failed to compile (missing standard headers; a Win32-only harness). After fixes: `ninja` 0 failures. |
| Linux gcc 15 full CTest | **WORKED**: 221/221 on the final tree (see "Final Linux run" for the exact log) | Run before the last CMake message-only edits: 220/220. The one test registered only after reconfigure (`runtime_session_projectile_render_v1_skill_models`) passes alone. |
| Case-sensitive file system (ext4) | **WORKED** | All asset reads in the 220 tests resolve on ext4; no case-mismatch failure remained. The w4 resolver (`path_case`) passes. |
| Use-after-move (`std::move` then read) | **PARTIAL** | clang-tidy `bugprone-use-after-move` on `main.cpp` and `features/frontend/frontend_presentation_v1.cpp`: 0 findings (Windows/mingw target). The ~1800 `std::move` sites were NOT audited exhaustively. The w4 `std::function` fix is present (`borrowSelectedProfileSnapshot`). |
| Argument-evaluation-order bugs (same call reads and writes the same error string) | **WORKED** (found and fixed 5 sites) | `original_attack_sequence.cpp` (test failure on GCC, fixed), `original_character.cpp` x4 (error text wrong on GCC). Remaining `require(f(err), "..."+err)` hits in `features/effects/*wgl_smoke*.cpp` and `features/combat_text/tests.cpp` are not built by any target on Linux, or are test-only; not changed. |
| clang 18 (Fedora container) | **DID NOT RUN (blocked)** | Before the container went down, all targets compiled with clang 18 (1213 steps, 0 failures, pre-fix sources). Container then became unreachable (see Environment). No clang CTest. |
| ASan + UBSan | **DID NOT COMPLETE** | gcc ASan/UBSan build was started in WSL and stopped at ~300/1213 to free the shared host; no ASan result exists. Command to run it is in "Reproduce". |
| Windows (llvm-mingw clang, own build dir, 223 tests) | **WORKED** (223/223 pass across the three result logs of the final tree; one earlier full run had `source_module_floors` fail once, passes alone) | See "Windows verification". |

## What was fixed (commit a22b5ada)

1. `original_attack_sequence.cpp`: the Type2 draw error message was built from `drawError` in the same call that fills it, so the message was read before the draw ran on GCC (unspecified argument evaluation order). Sequenced into two statements. This is a real product bug (wrong error text), and it failed `original_attack_sequence` on Linux.
2. `original_character.cpp`: four `require(load(..., error), "..."+error)` sites, same class. Sequenced.
3. Test sources that used `std::find` / `std::reverse` / `std::copy_n` / `std::find_if` / `memcpy` without the header (libstdc++ does not provide them transitively; MSVC does): `npc_root_sample`, `original_actor_navigation`, `original_attack_sequence`, `original_campaign_runtime`, `scene_probe`, `runtime_ranged_skill_monster_v1`, `runtime_timed_buff_v1` tests. Compile failures on Linux.
4. `CMakeLists.txt`: `private_save_transport_win32` is a Win32 harness (`<windows.h>`, `GetCurrentProcessId`, `GetTickCount64`). It is now built only on WIN32; other platforms print a STATUS line instead of failing to compile.
5. `tools/portability/run_linux_tests.sh` (new, see Reproduce) and `.gitattributes` (`tools/portability/*.sh text eol=lf`, so a Windows checkout does not break bash).

Windows-API use in game code: the uses in `game_save.cpp`, `save_store.cpp`, `platform_context_identity.cpp`, `main.cpp`, `frontend_presentation_v1.cpp` are behind `_WIN32` guards and compile on Linux (verified by the build). Five files include `<windows.h>` without a guard and are not in any CMake target on any platform, so they are not built anywhere: `features/frontend/interactive.cpp`, `features/device_provider/actual_device_provider_wgl_v1.cpp`, `features/effects/runtime_effects_*_wgl_smoke.cpp`, `features/equipment/source_equipment_renderer_smoke.cpp`. Dead code; left alone.

## Platform-only tests (Windows 223 vs Linux 221)

Windows-only, not built on Linux (each now prints a `message(STATUS ...)` at configure time):
- `private_save_transport_win32`: Win32 harness.
- `window_lifecycle`, `renderer_visual_quality`: Win32/WGL window.
- `winmm_pump_priority_v1`: WinMM thread priority API.
- `source_l1_specular_render`, `source_material_pass`: WGL render harness.

Linux-only smoke tests: `linux_platform_smoke`, `linux_sdl2_output_dummy`, `linux_sdl2_output_failure`, `linux_sdl2_source_session_control`.

223 - 6 + 4 = 221, matching the Linux count. Nothing is silently skipped.

## Asset layout used (ext4 mirror)

The worktree's `.local-inputs` is made of junctions to the primary checkout's 939 subdirectories (as the brief asks). Top-level files of `.local-inputs` (4040 files, 11.5 GB) are not junctioned. No test on Linux needed them; the Windows run has them only through the primary checkout. The tests need these trees (copied to ext4): `actors`, `assets-extra`, `character-skill-session-v2`, `design-settings`, `player-loot-v7`, `runtime-session-projectile-v1`, `windows-encounter-source`, `windows-main-frontend-v1`, `windows-population-assets`, `windows-shared-assets`, `windows-melee-bindings`, `windows-equipment-assets`, `windows-camera-assets`, `windows-source-floor-fixture`, `windows-source-clock-v19-preview-{9,12,14-rc1,15-rc3}`, `windows-source-clock-v20`, `windows-source-clock-v21.6`, `windows-source-clock-v21.8-staged`, `windows-source-clock-v21.8-staged3`, `v20/asset-stage`, `claude-preview16/levels-root` (13k files), `claude-preview16/map/assets-darkwood`, `claude-preview15/startup/converted-media-v2`.

Notes on copying: `rsync -rL` from `/mnt/c` over 9p was very slow and some background copies were silently killed (SIGINT). Trees were then copied via Windows `tar` (fast, one file) and extracted in WSL. Always verify counts after a copy; a partial copy produced misleading "asset not found" failures.

## Final Linux run

- Tree: `af436cf2` + the CMake message-only edits (committed with this report).
- Before the message-only edits: `ctest` 220/220 pass (log `~/w6/ctest-final-gcc.log` in WSL).
- After the reconfigure, which registers `runtime_session_projectile_render_v1_skill_models`: `ctest -j6` **221/221 pass** (log `~/w6/ctest-final2.log` in WSL, `100% tests passed, 0 tests failed out of 221`).

## Windows verification

Build: `W:` = subst of the worktree (own letter), build dir `C:/dh-w6/win/build`, llvm-mingw clang 20-era toolchain from `.local-inputs/windows-toolchain` (same as `C:/dh217/build.sh`).
- First full run (pre-fix tree): 222/223; `source_module_floors` failed once (3 s), passed alone (2.9 s), i.e. a timing/ordering flake in that run, not reproduced.
- Final tree (after the original_attack_sequence / original_character fixes): every one of the 223 tests reported `Passed`, no `***` failure line in the three logs (`ctest2.log` partial, `ctest3.log` partial, `ctest4.log` for the stragglers). The run was interrupted twice by ctest stalls (no child process, host shared with other agents); the stragglers were re-run with `-R` and passed.
- Reconfigure after the message-only CMake edits: `Total Tests: 223`.

The stall is real and worth a note for the lead: the Windows ctest run became very slow (several minutes per short test) while the host was loaded, and ctest sometimes sat with no child process. Re-running the unreported tests with `-R` completed.

## Environment incidents (not fixed by me)

1. WSL distro `Ubuntu`: its ext4 root went `emergency_ro` (`errors=remount-ro`) during the run; writes failed. Later the distribution failed to start (`Wsl/Service/CreateInstance/E_FAIL`, error code 6). It came back about 30 minutes later on a different VHD device (`/dev/sde`), read-write, with my mirror intact. Cause not known to me (VM restarted about 30 minutes before the RO event as well).
2. Docker Desktop engine: `500 Internal Server Error` / "Docker Desktop is unable to start" throughout (same WSL backend). Started the app once per the brief's fallback; still not running at the time of writing. So the Fedora clang container is unavailable.
3. WSL has no sudo without a password, so `clang++` could not be installed in Ubuntu.

Action for the lead: check the Docker Desktop WSL integration; if clang is wanted in WSL, `sudo apt install clang` (needs the password). Then run the sanitizer and clang passes with the script below.

## Reproduce

`tools/portability/run_linux_tests.sh` (WSL Ubuntu):
```
tools/portability/run_linux_tests.sh                       # gcc, Release, full build, full CTest
tools/portability/run_linux_tests.sh --compiler clang      # needs clang++ installed
tools/portability/run_linux_tests.sh --sanitize            # ASan + UBSan (gcc or clang)
tools/portability/run_linux_tests.sh --no-copy --ctest-args "-R path_case"
```
What it does: rsyncs the worktree to an ext4 work dir (refuses `/mnt/*`), copies the asset trees listed above (`ASSET_DIRS`, `ASSET_SUBPATHS`), configures with Ninja, builds with `-k 0`, runs `ctest` with `--output-on-failure`, and writes logs under `<work>/logs`. Its asset copy uses `rsync -rL` (slow over 9p): prefer running it from inside WSL on ext4 data.

Status of the script: **written and `bash -n` checked, not executed end-to-end.** Every step it performs was run by hand in WSL with the same commands. Its `ASSET_DIRS` list is the one used for the final run except that it also includes `windows-source-clock-v21.8-staged`, which the manual run added later.

## Remaining failing tests

- Linux (gcc 15): none in the final tree (221/221 expected; see Final Linux run).
- Windows: none.
- Not run at all: Linux clang, ASan/UBSan (both gcc and clang), a whole-project `bugprone-use-after-move` pass.

## Next steps (for the lead)

1. Repair WSL/Docker (above), then `tools/portability/run_linux_tests.sh --sanitize` and `--compiler clang` (the ASan run is the one that can find use-after-free/UB in the ~1800 move sites).
2. clang-tidy `bugprone-use-after-move` over all `port/windows-foundation/*.cpp` (the compile DB exists at `C:/dh-w6/cc/build/compile_commands.json` on this host; single-file run took about 10 min).
3. Keep `source_module_floors` under watch (one flaky failure on Windows).
