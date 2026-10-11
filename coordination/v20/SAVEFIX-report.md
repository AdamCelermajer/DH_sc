# v20 SAVEFIX report

Worktree `DH_wt/v20savefix`, branch `v20/savefix` from `v20/integrate` (09a1bb68).
Frozen exe: `C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/v20/savefix/final/dh-foundation.exe`
SHA256 `1511B238350351AA1E22F734DA75FAF6544EADB0BB5F4765AE5BD134CEBEAE32` (build `build-v20savefix`, exit 0).

## 1. Investigation (source/log evidence; no new video evidence, internal infrastructure)
- `combat_session.cpp` ~1368: live ActorState vitals are a nonnegative projection of the source sheet; the -1 "no pool" sentinel is clamped to 0 for every actor (P16). The original sheet cell stays -1.
- `game_save.cpp normalize()` (run by `capture_game_save`) reconciled only the CURRENT cells (36 health, 41 resource) with the live value, never the MAX cells (38, 43). `validate_game_save` compares all four. A targetable/attacking actor whose max_resource sheet is -1 (live 0), such as `_prim_NPC`, therefore always failed the save. Visual-only actors (non-targetable, no attacks) were already exempt, which is why P16 saved its scripted troll etc.
- Original: the original actor has a single value per cell (no live/sheet split); the port's split exists only because of the nonnegative projection, so the save must write the projection back to the sheet exactly as it already does for current HP/MP. I did not re-derive `Character::Serialize` 0x3a65c4 from IDA in this pass [not investigated; the port keeps its own save format].
- All save paths (script SaveGame via `saveLevelWorld`, transition save, F9/live save at main.cpp ~5517, reload snapshot, menu return) call the same `capture_game_save`, so NPCs are handled identically.

## 2. Implementation
`game_save.cpp normalize()`: loop now covers 36, 38, 41, 43. Max cells (38/43) are reconciled only for the sentinel case (live 0, sheet < 0); any other max mismatch is left to the validator and is still rejected. The validator is unchanged. Generic; no name special-case.

## 3. Isolated tests
`tests/game_save_tests.cpp` new block: targetable attacking actor with resource sheets -1 and live 0 captures successfully and the sheet is reconciled to 0; forged live-0/sheet(-1) save is rejected by `validate_game_save`; non-sentinel max mismatch (live 5 vs sheet 0) is rejected. Existing `combat_session_tests` rejection (forged targetable animation-only) unchanged and passing.
Build exit 0. Full ctest: 161/162 pass; only `session_skill_binding` fails.

## 4. Integrated runtime (frozen exe, quiet_run.ps1 hidden, DH_AUDIO_SILENT=1, fresh private dir)
Dir `.local-inputs/v20/savefix/`; args are the FXVERIFY normal-opening set (compiled bank, `001_swamp`, `--campaign-triggers`, `--fresh-player`, no source-root-scopes/condition overrides), `--frames 4900`, file `opening/jobs`-derived `jobs-open.json`.
- `opening`: exit 0, 85 s, "Rendered frames=4900; clean shutdown". ScriptSaveGame ran twice: frame 3775 and 4431, each "Level world saved reason=script-quicksave ... actors=44" then "SaveGame wrote profile=.../character.save". Files written: `character.save` (2661 B), `character.world-001_swamp.gsave` (177 KB).
- `resume` (`jobs2.json`): copy of those saves, `--fresh-player` + `--save`, 400 frames: exit 0, clean shutdown. That path throws on a failed `load_character` (main.cpp 1181-1182), so the profile loaded; the rewritten character.save is byte-identical. The `.gsave` world restore only runs on level-transition arrival and was NOT exercised.
- `witch` (scriptsvc witchcave args): exit 0, 1200 frames; PlaySound dispatches as before; SaveGame @740 wrote the profile.

## 5. Visual verification
Opening f900 (cage with prince behind bars, SKIP button, moth at left) vs FXVERIFY pre-fix run f900: same composition; moth wing/position phase differs (PSNR 30.0 at f900, 28.4 f4500, 28.1 f4900). Cause not established (animation phase/timing; the pre-fix run aborted nothing before f900). f4900 not inspected visually; title/menu not exercised beyond this run.

## 6. Remaining gaps
- Level-world `.gsave` restore on resume not exercised; `Character::Serialize` not re-checked in IDA; moth phase difference unexplained.
