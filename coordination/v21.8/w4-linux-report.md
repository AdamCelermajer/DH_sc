# w4-linux: case-sensitive file system + missing hero preview (Fedora report)

Branch `v21.8-w4-linux` (from windows-foundation, includes the first fix `befd0761`). Nothing pushed, nothing merged.
Commits: `517d32ce` (shared resolver + preview fix), `fd5bdfec` (all remaining opens routed + test), then this report.
Evidence in `coordination/v21.8/w4-linux-evidence/` (images, driver scripts). Test host: WSL Ubuntu, **ext4 (case-sensitive)**, package
copied to the ext4 home (not /mnt/c), WSLg X11 window, PulseAudio, Mesa llvmpipe. Driven with real X events (xdotool) through
boot movie -> main menu -> Start game -> empty slot -> name -> class -> Confirm -> Single Player -> Swamp.

## Verdict per item

1. Missing "preview/render after creating the character": **WORKED (fixed). It was NOT a case problem, not GL, not Mesa.**
   - Root cause: `run_frontend_v1` (features/frontend/frontend_presentation_v1.cpp) calls `runtime.attach(window, renderer, std::move(runtimeServices), error)` and
     afterwards still read `runtimeServices.borrow_selected_profile_snapshot`. A moved-from `std::function` is EMPTY under libstdc++ (Linux)
     but still callable under libc++/MSVC (small functor is copied), so on Linux `projectSavedProfileActor` returned early,
     `selectedProfileActor` stayed -1 and the saved-profile hero was never drawn. No error was logged.
   - Proven by temporary instrumentation (removed): `inuse=1 actor=-1 valid=0` every frame on ext4 with the unfixed logic.
   - Fix: copy the callable before the move (`borrowSelectedProfileSnapshot`) and use the copy.
   - Evidence: `preview-missing-before.png` (Linux, same profile, hero absent) vs `w4-win-240.png` (Windows reference, hero standing)
     vs `preview-fixed-linux.png` (Linux after the fix, identical to Windows); the log line `{"profile_actor":"PASS",...,"draw_packets":5}` appears only after the fix.
     `fed-menu.png`: the Fedora-40-container binary on ext4 shows the hero too; live creation (clicked through) shows it right after Confirm.
2. `Asset not found ... npc_m_dialog.bdae` / `Skybox skipped`: the first fix (AssetCatalog::resolve_authored) handles these; **WORKED** end to end:
   log `Level notice: Skybox loaded: data/3D/skybox/skybox_swamp.bdae triangles=12`, Act 1 loads, no Foundation error.
3. One shared resolver, every game-data open routed: **WORKED** (below).
4. Menu -> create -> Act 1 -> walk/attack, 1500+ frames without a Foundation error: **WORKED** (player side; see limits).
5. Windows ctest: **221/221 pass** (own build dir `C:\dh-w4build`, subst W:, includes the new `path_case` test).

## Shared resolver
New `port/windows-foundation/path_case.{hpp,cpp}`:
- `find_path_ignoring_case(root, relative)` and `existing_path_ignoring_case(path)`.
- Exact spelling wins; otherwise each component is matched against its directory listing ignoring ASCII case; two names differing only by case:
  lexicographically first (deterministic). Per-directory listing cache keyed by directory, mutex, refreshed when the directory mtime changes
  (files created later are found). `_WIN32`: pass-through (no behaviour change).
- AssetCatalog uses it (the first fix's private copy was removed). Routed through it: texture_loader, original_scene, level_manifest, hud_glyphs,
  source_root_scopes (DebugFiles fallback), features/audio/feature_audio (AudioFilesystem::read = audio-assets reads), quest_table_v1, boot_runner_v1,
  features/frontend/art/original_art, frontend_presentation_v1 (DebugFiles fallback), level_inventory (read_file, present_in, data/scene listing),
  and in main.cpp `parse()` the startup-arg package roots `--assets --audio-assets --audio-table --menu-assets --menu-ui-assets`.
- Deliberately NOT routed (files we create, exact names kept): game_save.cpp, save_store.cpp, campaign_host tutorial settings, checkpoint snapshots,
  captures, `character.save`/`gameplay.save`, startup.args itself. `content_paths.cpp` already has its own case-insensitive folded lookup (all platforms) and was left alone.
- Test `tests/path_case_tests.cpp` (`ctest -R path_case`): authored spellings `data/3D/...`, exact wins, missing stays missing, late file found,
  `..` still rejected, AssetCatalog::read of `data/3D/characters/npcs/animations/npc_m_dialog.bdae`. Passes on ext4 (WSL) and on Windows.

## Audit method for "every other place"
`strace -f -e trace=file` of full runs (boot -> menu -> create -> Swamp, 1700 frames) + `casemiss.py`: every ENOENT under assets/audio-assets/ui-assets is
checked for a differently-cased existing file. Final binary: 8 case misses (`data/3D`, `AnimatedDecors`, `GameObjects`, `Modules`, `Swamp`,
`CS_wallbreak_death_cutscene.bdae`, `CS_moth_*_intro.bdae`), all subsequently opened through the resolver; 5 truly absent
(`DebugSwitches.savegame`, `env_swamp_alpha.tga` x4 candidate locations: absent on Windows as well, optional probes).
Coverage limit: only what the Act 1 Swamp start exercises; other levels / skills / audio events were not run on ext4 (the resolver is generic, unrun paths are unproven).
The ~550 failed `data/3D` stats per run are the exact-spelling probes before the case match (cheap).

## Real runs on ext4 (frames viewed)
- Full production path (boot intro movie with PulseAudio: `Boot outcome=0 movie="skipped: user" movie_clock="audio" soundtrack_seconds=8.8`, menu, create,
  Single Player, Act 1) with `--frames 1700 --capture-at 1500:...`: the game exited by itself, last log lines `Source FX final packetFrames=1700 ... presentationFailed=0`,
  zero `Foundation error` lines, `f1500.ppm` written; frame 1500 viewed (`w4-z3-f1500.png`: HUD, Knight, faery, priest, cages).
- Walk: WASD held, the Knight moves through the swamp ruins (`sheet-m.jpg`, frames at different places, a moth visible).
- Attack: Space held, frames at +0.15/+0.4/+0.65 s show the swing and the blue slash arc (`sheet-a.jpg`); skill key 2 pressed.
  No enemy was killed (none in reach of my scripted walk): enemy damage/death on Linux is NOT demonstrated, only player attack animation + VFX.
- Tutorial popups block input exactly as on Windows; SKIP click / Return dismiss them.

## Other Linux findings (not fixed)
- SDL tries ALSA first and fails in WSL (`Boot audio unavailable (SDL audio output open: ALSA ...)`); with `SDL_AUDIODRIVER=pulseaudio` audio works (movie clock = audio;
  nobody listened). Real Fedora PipeWire/Pulse should be fine; the game continues without audio when the device is missing.
- Gameplay window is 1201x720 while the menu is 1280x720 (not compared with Windows).
- The bug class (use after `std::move` of a struct holding `std::function`s) may exist elsewhere; none surfaced on the Act 1 path; I did not audit every `std::move(services)` (~30 candidates, mostly constructors moving into members).

## Fedora 40 binary
`C:\dh-fedora\out-w4\dh-foundation-fedora` (22,046,480 bytes, sha256 `b0131e1a6e639af92ae17822aa7a26c5c33dfdfa0e17ddc8ab257d43b10e46bf`), built from this worktree
(commit `fd5bdfec`) with `C:\dh-fedora\build.sh` in `fedora:40` (not zipped/packaged). **Max GLIBC symbol: GLIBC_2.38** (also 2.34, 2.35).
Sanity: that binary on ext4 shows the hero preview (`fed-menu.png`).

## Reproduce
`w4-linux-evidence/{dhrun.sh,w4drive.sh,casemiss.py}`: per-run directory with symlinks to the ext4 package, xdotool driver (`clickv` retries a click until the screen changes;
`xdotool windowfocus` is required, without focus the frontend ignores clicks), strace + case-miss analysis.
