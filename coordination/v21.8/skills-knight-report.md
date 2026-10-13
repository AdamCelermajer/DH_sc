# v21.8 skills-knight: Knight-tree (Berserker, Paladin) active skills

Branch `v21.8-skills-knight` (worktree `.claude/worktrees/skills-knight`, base `windows-foundation` @ aa2e8624).
Scope: the Knight base has no active skills left beyond BashDown/GroundSlam/Charge (handled by skills-a).
Group = the 7 Berserker/Paladin-owned actives: WhirlingDrift, Intimidation, Berserk (Berserker); DivineJudgement,
OffensiveAura, GuardingAura, PurifyingAura (Paladin).

STATUS: WIP. Code and unit tests are in place, but NO real hidden-window runs and NO full CTest have been done yet.
This commit is a work-in-progress checkpoint written when the owner's usage limit was nearly exhausted.

## Handoff (status at stop)

| Skill | Script | Status | Evidence so far |
|---|---|---|---|
| WhirlingDrift | prince_berseeker_whirling_drift | NOT STARTED (row admitted, unit-tested) | profile unit test only. Push chance, pushback and forward movement not in port (gap) |
| Intimidation (Warcry) | prince_berseeker_warcry | NOT STARTED (row admitted, rolls_combat=false) | unit test only. Scare/stun states do not exist in port, so the effect will be DID NOT WORK; cast/anim/FX/sound still to be run |
| Berserk | prince_berseeker_berserk | DID NOT WORK (by design, not admitted) | Use is 3 volleys (stun over 360, damage and knockback on top target) with 3 class sheets, plus OnPre self-HP cost. One class sheet per cast cannot express it. Expect a real-run rejection line `No complete generic same-session Use arm` |
| DivineJudgement | prince_paladin_divine_judgement | NOT STARTED (row admitted) | unit test only. Per-target OnPre spell FX is a gap |
| OffensiveAura | prince_paladin_offensive_aura | NOT STARTED (row admitted, buff arm, fixed 12000 ms) | unit test only. Self-only buff (inference: single-player, no other "Player" characters) |
| GuardingAura | prince_paladin_guarding_aura | NOT STARTED (row admitted, buff arm, fixed 12000 ms) | unit test only |
| PurifyingAura | prince_paladin_purifying_aura | NOT STARTED (row admitted, buff arm, fixed 12000 ms) | unit test only. Heal (skill_level*15 HP via SetBuffProp) is a gap, so expect PARTIAL |

Runtime-verified column in SKILL-VERIFICATION-MATRIX.md: NOT updated (no runs yet).

## What was changed (committed in this WIP)

- `features/generic_skills/runtime_skill_use_profile_v1.hpp`: two defaulted fields only. `RuntimeSkillBuffProfileV1::fixed_duration_ms` (0 = projected SnS_Duration, as before) and `RuntimeSkillUseProfileV1::rolls_combat` (default true). Existing rows unchanged.
- `features/generic_skills/runtime_skill_use_profile_knight_v1.hpp` (new): 6 rows (`kRuntimeSkillUseProfilesKnightV1`) plus `find_skill_use_profile_registered_v1` (base table first, then Knight rows). The header lists the Lua line-level transcription and the gaps.
- `features/generic_skills/runtime_skill_cast_coordinator_v1.cpp`: include of the Knight header; admission uses `find_skill_use_profile_registered_v1`; buff duration uses `fixed_duration_ms` when > 0; the Use hit loop breaks at once when `rolls_combat` is false.
- `features/generic_skills/runtime_skill_use_profile_knight_v1_tests.cpp` (new) and CMake target `runtime_skill_use_profile_knight_v1` (test passes: "six Knight-tree rows, exact admission, Lua-transcribed values").
- Base test `runtime_skill_use_profile_v1` still passes (table count 7 unchanged).

## Findings needed for the real runs

- Subclass characters: CharacterTable rows are `KnightPlayerBase_Berserker` and `KnightPlayerBase_Paladin`. Their SkillTree lists are `KnightBerserker` and `KnightPaladin` (16 entries each, in the decoded order). Progression (`runtime_skill_progression_v1.cpp`) requires `state.class_id` to name the CharacterTable row and `state.skills` to match the list order exactly. The existing Knight seed save (`C:/dh218/seeds/knight-charge/character.save`) has class `KnightPlayerBase` and the base list only, so the existing `make_rogue_skill_save.exe` cannot bind these skills (it reports "save has no skill row named ..."; confirmed for all 7).
- Name lists for the save tool are written to `C:/dh218/tools/skills-knight/{berserker,paladin,knight}-list.txt`.
- Next tool step: a small `make_subclass_save` (in `C:/dh218/tools/skills-knight/`) that loads the seed save via `save_store.hpp` / `character_state.hpp`, sets `class_id` to the subclass token, replaces `skills` with the list file (rank 0), binds the skill to slot 0 with rank 1, and saves. It needs linking against `libfoundation_data.a` in `C:/dh218/skills-knight-build` (not yet attempted; compile errors may need extra include dirs).
- Whether the actor's combat properties follow `class_id` (the run uses `--actor-row KnightPlayerBase`) is UNKNOWN. The real run must show whether the cast gets `phase=1` and a sane mana projection.

## Build / test / run recipes

- Worktree subst: `K:` -> the worktree (PowerShell `subst K: "C:\Users\adamc\Desktop\workspace\DH_sc\.claude\worktrees\skills-knight"`). It is removed at the end of this session.
- Junctions: the worktree `.local-inputs` holds ~931 directory junctions to the primary `.local-inputs` dirs. They are removed at the end (`cmd /c rmdir` per link). Recreate with PowerShell `New-Item -ItemType Junction` for each primary subdir.
- Build: `bash C:/dh218/skills-knight-build.sh` (env as in C:/dh217/build.sh; `TARGET=<name>` for one target). Build dir `C:/dh218/skills-knight-build`; `dh-foundation.exe` there includes the coordinator change.
- Unit test: `C:/dh218/skills-knight-build/runtime_skill_use_profile_knight_v1_tests.exe "<worktree>/.local-inputs/windows-shared-assets"` (needs the llvm-mingw bin on PATH for DLLs).
- Real run: `C:/dh218/tools/skills-a/run-skill.ps1` (`-Case`, `-Save`, `-Exe`, `-Keys "150:2"`, `-Captures`). Asset root is the staged package. Check for `Source skill key=2 ... phase=1 skill=<name>`, `Source player step FX ... dispatched=1`, `Audio step source ... status=1`, and for buffs `Timed buff applied ... class=Skill_Paladin_*`.
- CTest: run from bash (not Start-Process). Full run takes about 50 minutes. Expected count is baseline 190 plus the new target (191). NOT RUN yet.

## Known failures / open items

- Full CTest not run on this branch.
- Real-run evidence missing for all 7 skills.
- Berserk: DID NOT WORK by design (see table). Needs a multi-volley Use with per-volley classes and an HP self-cost, which the engine does not have.
- Gaps documented in the new header: push/knockback tails, Warcry scare/stun, Divine Judgement per-target FX, aura self PlayFX, Purifying heal, ally buffs, WhirlingDrift forward movement.

## Next steps (in order)

1. Build `make_subclass_save` against `libfoundation_data.a` and create saves for Berserker and Paladin skills.
2. Real hidden-window runs for WhirlingDrift, Intimidation, DivineJudgement, OffensiveAura, GuardingAura, PurifyingAura, and one Berserk run to capture the rejection line.
3. Update the runtime-verified column for these rows in SKILL-VERIFICATION-MATRIX.md, and this report's table with final statuses.
4. Full CTest in bash; record the count.
