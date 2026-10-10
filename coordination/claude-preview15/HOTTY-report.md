# HOTTY report: B050 Hotty slot-4 cast ends with "Source Post cannot complete before its authored retained Use event"

Branch `p15/hotty` (from `p15/integrate`). Build dir `DH_wt/build-hotty`.

## Status

- FIXED in the real EXE: Hotty (key 4, slot 4) completes its state-7 cast, the `do_spell` Use applies its source hits, and the run exits 0. Celest key 4 still exits 0 with unchanged output.
- Fallback rule NOT used (Hotty is not rejected).
- Connected session test (`run_hotty_session_cast_v1_tests.ps1`) is NOT green on this branch. It now compiles and runs, but it stops at the B009 Faery-page release checks, before the Hotty/Celest cast section. See "Verification gaps".
- Visual evidence for the Hotty cast (footage/animation frames) was not captured in this session. Logic and timing evidence are from the shipped authored clip data and the EXE logs.

## Evidence: reproduction (unmodified build, branch EXE)

Scratch: `.local-inputs/claude-preview15/hotty/run1/` (args copied from the faerysound scratch; saves are copies). Quiet runner, `jobs-base.json`:

- `base-hotty-far`: exit 1. Log: `Source Faery key=4 frame=70 slot=4 sequence=351 ...`, `Faery cast sound uid=479 label=sfx_mage_staff_elemental_fire targets=0 ... status=dispatched`, then `Foundation error: Live combat: Source Post cannot complete before its authored retained Use event`. No `do_spell` event in the log.
- `base-hotty-near`: exit 1, same error (targets=3, label `sfx_spell_fire_medium`).
- `base-celest-far`: exit 0, `sequence=348`, `do_spell` at frame 95, `Faery cast sound` lines unchanged.

## Root cause

1. The Faery cast request in `main.cpp` (~line 3010) passed the selection as
   `request.selection = {selected.selection_state, 0, sequence->phases.front().sourcePath}`.
   That makes `group_path` the FIRST phase's leaf path, which the playback treats as an explicit scope.
2. The Hotty state-7 root (`Knight_Hotty_DD`, sequence 351, which is also the Rogue's `CharAnimTable.Spells[4]`) has two phases (dump via temporary diagnostic, now removed):
   - phase-0 `player-source-skills/source-skill-sequence-351/phase-0`, anim 1100, clip `prince_spell_pre_hotty.bdae`, no markers;
   - phase-1 `.../phase-1`, anim 1097, clip `prince_spell_hotty.bdae`, marker `do_spell` at t=434 ms.
3. `retained_sequence_playback.cpp` `complete_hierarchy`: with `scope_path_.size()==leafPath.size()` it returns "stop", so phase-0 completion finished the whole sequence. The finished callback (`complete_v1`) then ran with no Use and raised the error. The log shows completion at frame 71, one frame after start.
4. Celest's sequence 348 is a single phase whose leaf path equals the scope, so its behaviour was unaffected.
5. The connected test had hidden this: `hotty_session_cast_v1_tests.cpp` set `group_path` to the do_spell leaf (phase-1), so it never ran the pre-to-Use chain.

Source for the "empty group = whole root" rule: `original_attack_sequence.hpp` line 12 ("Empty executes the entire selected root sequence; nonempty executes that subtree only").

## Changes

- `port/windows-foundation/main.cpp` (~3010): Faery cast selection uses an empty group path (whole selected state-7 root). Single-line hunk with a comment.
- `port/windows-foundation/features/generic_skills/runtime_skill_cast_coordinator_v1.cpp` (Hotty `apply_use_v1`): the do_spell success summary is set like the Celest path, keeping retained FX diagnostics after it. Previously the summary was dropped whenever an FX diagnostic existed (cosmetic log text).
- `port/windows-foundation/features/faery_menu/tests/hotty_session_cast_v1_tests.cpp`: Hotty and Celest connected-test selections use the whole root (`group_path = {}`), matching production.
- `port/windows-foundation/features/faery_menu/run_hotty_session_cast_v1_tests.ps1` (stale harness repair, needed to run the connected test at all):
  - adds `faery_cast_sound_v1.cpp` and `effects/runtime_source_fx_asset_v1.cpp` (symbols the cast coordinator now links; the list predated them);
  - resolves `.local-inputs` through the worktree junction (`AssetCatalog` canonicalizes paths, so junction paths fail with "Asset path escapes asset root"). In the main tree `.local-inputs` is a real directory, so nothing changes there.
- Untracked (gitignored `*_build/`), NOT committed: copied `port/windows-foundation/features/actor_frame/source_character_owner_factory_native_build/native.a` from `DH_sc` (read-only source). The connected runner links it.

## Evidence: after the fix (quiet batch, 220 frames, `final-args/`, EXE `bin/final.exe`)

| job | exit | key lines |
|---|---|---|
| final-hotty-far | 0 | `slot=4 sequence=351`, `Faery cast sound ... label=sfx_mage_staff_elemental_fire targets=0 ... dispatched`, lifecycle frame 93 `phase=2` (do_spell applied), frame 127 `phase=3` completed, `hits=0` |
| final-hotty-near | 0 | `label=sfx_spell_fire_medium targets=3`, frame 93 `phase=2 hits=3`, frame 127 `phase=3 hits=3` completed |
| final-celest-far | 0 | `slot=0 sequence=348`, `label=sfx_spell_lightning_medium` + `StaticBallKilled` failed (missing asset, logged), frame 95 do_spell, frame 133 completed |
| final-celest-near | 0 | `label=sfx_spell_lightning_medium targets=3`, frame 95 `hits=6`, frame 133 completed |

No `Foundation error` lines in any final log. Hotty do_spell fires at frame 93, which is the authored phase-1 marker after the pre clip.

## Tests

- `p14_build.ps1 -Name hotty -Test`: build exit 0. ctest 113 of 114 passed. Only `session_skill_binding` failed, the known worktree junction failure.
- Connected runner `run_hotty_session_cast_v1_tests.ps1` (with the harness fixes above): compiles and links, then FAILS before the cast section:
  - `same-Session Faery release must use the preloaded Celest bank and source continuation order` (line ~340). Diagnostic: `slot_at(80,80)==0`, but after `session_provider.release(80,80)` `current_faery` stays 4 and `menu_order` stays empty, with no error text.
  - With those checks bypassed in a scratch copy only, the next check `Faery provider must reject after its bound Session is destroyed...` also fails.
  - This is the B009 Faery-page release path, which the cast change does not touch. My reading is that the release now takes a CharacterState-only path (`character_state_page_v1.cpp` release -> `select_character_state_faery_v1`) without the ChangeFaery/UpdateAllSkills continuations. I did not confirm this against an older commit.
  - So the connected Hotty/Celest cast assertions were not executed. The bypass was temporary and is reverted; the committed test has no bypass.
- No focused unit test was added for the playback rule. The existing `tests/retained_sequence_playback_tests.cpp` already runs a multi-phase whole-root selection (Knight, 3 phases), but nothing guards the caller's choice of selection. The EXE batch is the real guard for that.

## Verification gaps (plain)

- Not verified: Hotty mana and cooldown numbers. The logs show `MP=30.25` both before and after; the 20 MP debit is asserted only by the connected test, which does not run here. Cooldown is not checked.
- Not verified: the Hotty visual and audio timing against original footage. No Hotty reference frames were reviewed in this session.
- Hotty FX not dispatched: `Player_Pre FX diagnostic: dispatcher unavailable` and `target FX diagnostic: Hotty target effect dispatch was not bound`. `main.cpp` wires `celest_effect_dispatch` but not Hotty's `effect_dispatch`. The source effect (hits, MP, cooldown order) applies without it, but there is no Hotty Player_Pre or target visual. Left as an open item; it is a presentation wiring task.

## Package files required

None new. Runs use the preview-13 assets and the preview-14 rc1 audio tables already used by the B050 jobs.

## Verifier script

Quiet runner, one job per args file in `.local-inputs/claude-preview15/hotty/run1/final-args/`, EXE `bin/final.exe` (copy of this branch's build):
- `v2-rogue-hotty-far`, `v2-rogue-hotty-near`, `v2-rogue-celest-far`, `v2-rogue-celest-near` (`--skill-key-frame 70:4`, 220 frames).
- Expect: all exit 0; no `Foundation error`; Hotty lifecycle reaches `phase=3`; Hotty `Faery cast sound` label `sfx_mage_staff_elemental_fire` (far) and `sfx_spell_fire_medium` (near, targets=3); Celest unchanged (`sequence=348`, `do_spell` frame 95, completed frame 133).
- Saves: reset the Hotty saves from `saves/*-src.save` before each run (the run overwrites `--save`).

## Open risks

1. Connected session test blocked by the B009 Faery-page release regression (above). The Hotty cast checks are updated to production selection but not executed here.
2. Hotty presentation FX unbound (above).
3. Hotty mana/cooldown not numerically verified in the EXE.
4. The fix relies on the whole-root selection being a single root container of type 1 for the Faery sequences. Verified for 351 (ordered, two phases) and 348 (single phase). Other Faery spell roots (Spells[1..3]) were not checked; they remain unused by the current FaeryList.
5. Mage path (connected test) was never run with the production selection; the test now uses it, pending the page-release fix.

## Commit

Branch `p15/hotty`, commit contains the main.cpp fix, coordinator Use-text, connected test selection, runner repair, and this report.
