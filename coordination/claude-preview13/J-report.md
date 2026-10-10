# Preview 13 Group J: B013 injury rate and attack continuity

Status: **B013 remains OPEN.** No production source changed. The group G "100% Injury" finding is an artifact of a probe input (a level-20 lizard override that the live EXE does not use). With live-like inputs, the port's Injury admission and interruption match the original, and the lizard Type1/Type2 hits never produce Injury against the authored Knight. The reported symptom is therefore not reproduced by the lizard path. Two real divergences are recorded below, not patched.

Implementer: Preview 13 worker J. HEAD `f6b7f134`. Scratch: `.local-inputs/claude-preview13/j/`.

## 1. Evidence

### 1a. Logic: the original rule (IDA, `pseudocode-all.c`)

- **Outcome calc** `Character::_F_CalculateResult` (`003b2638`, ~lines 130516-130580). Mask bit `0x80` requests Injury. The Injury roll is `Random::GetRandom(100) << 8`, with `Random::GetRandom` returning `[0, N)` (line 42409; N is the `&dword_64` constant = 100). When mask `0x20` or `0x40` (critical) is also set, the critical roll is reused for Injury (`v14` is not `-1`, so no new roll). The port's `combat_result.cpp` `dh2_combat_result` (`critical_roll` reuse, `status(...)`) mirrors this.
- **Formula** `Character::CF__CalcHurt` (`003b0e8c`, lines 129545-129580), mode 0: `P135(att)` must be `> 0`, else result is `P135 > roll`. Otherwise
  `hurt = P135(att) + 5*P19(att) - P134(def) - 5*P19(def)`, and Injury iff `hurt > roll<<8`.
  Port `combat_result.cpp` `status()` computes the same expression with the same property ids (135/134/19). Identical.
- **Admission** `CharStateMachine::SM_SetInjureState` (`003c5d84`, line 142766): if the shared Character timer `+0x14fc` is positive, return; else set it to 3000 ms and raise event 50010. The port's `ActorCombatRuntime::start_injure_reaction` (`actor_combat_runtime.cpp:241`) does the same: gate `>0` is a no-op, else set 3000.
- **Effect on an active swing**: `CSAttack::OnInit` (`003c8284`, registrations near line 144584) maps event 50010 to state 11 with handler `Character::CSM_Injured` (`003ad23c`), and that handler returns `1` unconditionally (line 127001-127005). So in the original, every admitted Injury leaves the attack state and enters Injured. The port's `start_injure_reaction` restarts the cursor on the Injured clip (interrupting the attack, as the B013 test shows: `Injure outcomes=0x18 ... active-attack=0`). **Conclusion: the swing is cut at every admitted Injury in both; the admission gate is the same 3000 ms.**
- **Idle conversion** (IDA lines 129886-129896, `F_ApplyResult`): if `victim->vtbl+40()` (inferred "is player": same slot gates the GOD check at line 129768 and the player-only dodge statistics) and `SM_IsIdle(victim+319)`, then when no bit in `0x16` (dodge/block/injure) is set, the code sets `0x10` (Injury) if damage > 0, else `0x2` (dodge). **The port has no equivalent** (see 3.2).

### 1b. Inputs: what the live port feeds

- Enemy level: `main.cpp` sets `level_raw` only for the player (`main.cpp:957`). Enemy profiles keep `propertyOptions.level_raw = nullopt`, so a lizard uses its CharacterTable base level. `actor-profiles-v2.xml` for `Swamp_LizadMan_Type1` gives `Level=256, LevelMin=256, LevelMax=768, LevelOffset=0` (1.0 to 3.0). The port's monster-level policy (`features/enemy_ai/runtime_monster_level_policy_v1.cpp`, source `monster.luac`) is **not wired** into the live path (only its own tests use it).
- The earlier B013 histogram used `enemy.propertyOptions={20*256}` (level 20.0), which the live EXE never uses. That override produced P19=5120 and the 98% Injury rate that G reported.

### 1c. Measured (B013 probe, `resolve_result` with mask `0x22aab5` = the lizard's non-alternate melee mask, seeds 1..5000, Knight starting gear without shield; defender P134=20736 (81.0), P19 = Knight level)

| enemy (live, no override) | Knight level | attacker P135 / P19 | positive dmg | Injury (bit 0x10) seeds |
|---|---|---|---|---|
| Swamp_LizadMan_Type1 | 1 | 14080 / 256 | 3682 | 0 |
| Swamp_LizadMan_Type1 | 5, 10, 20 | 14080 / 256 | 3093, 2500, 2500 | 0 |
| Swamp_LizadMan_Type2 | 1 | 16640 / 256 | 3682 | 0 |
| Swamp_LizadMan_Type3 | 1 | 22272 / 256 | 3775 | **241** (0x10 and 0x14 only) |
| Swamp_MovieMoth | 1 | 14080 / 256 | 3682 | 0 |

Type3 at Knight level 1: threshold = 22272 + 1280 - 20736 - 1280 = 1536, so Injury for rolls 0..5 (about 6%). The 98% figure is the level-20 override artifact: threshold = 38400 + 25600 - 20736 - 1280 = 41984, above every roll.

The live `Damage frame` log lines (G, candidate EXE) show lizard hits of 7-9 with no reaction, which agrees with 0 Injury for Type1 at these values.

### 1d. Visual (reference video, v1.0.3, z_Zky7qQdYs)

- Contact sheet `sheets1/s016.jpg` (6:00-6:20, 4 s apart): the Warrior fights Trungus with the sword. Damage numbers appear over the enemy. At 6:16 (376 s) the level-up tutorial appears. Not an incoming-hit clip for the hero.
- Dense extraction 360-376 s at 2 fps (`.local-inputs/claude-preview13/j/ref/sheet_360_376_fps2.png`, 32 frames): the hero's swing trails are visible while numbers appear over Trungus. No hurt pose on the hero is visible. Inventory and equipment screens open from about 366 to 374 s (not combat). **Inconclusive** for the question "does an incoming hit cut the swing": no hit on the hero is isolated in this window. Frames at 377 s specifically were not separately extracted (the sheet grid does not contain 377 s).
- G's candidate-EXE captures (frames 128-136, 276-292) show the swing pose continuing through a red `8` on the player. Single-frame interval only.

### 1e. Idle conversion, visual check

Not verified. No frame of an idle hero being hit was isolated in the footage I checked.

## 2. Expected behaviour

- Clean hit (`0x0`) on an attacking hero: no pose change, target change or clock reset. **Port: already so** (`start_injure_reaction` returns early without `0x10`; nothing else in `handle_applied_hit` touches the attack).
- Injury admitted (`0x10`, 3000 ms gate): the Injured clip replaces the swing (IDA CSAttack 50010 -> CSM_Injured). **Port: matches.**
- Injury with a lizard at live values: Type1/Type2 never; Type3 about 6% of non-miss hits, one admission per 3 s at most.
- Idle hero clean hit: original converts it to Injury (IDA 129886-129896). **Port: no** (3.2).

## 3. Divergences and decisions

1. **Monster level not fed (real divergence, not fixed).** The original's `monster_OnInit` (per the port's policy file) sets level = clamp(host player level, LevelMin, LevelMax) + LevelOffset. Swamp lizard range is 1.0 to 3.0. The live port keeps the base level 1.0 for all monsters. Effect on Injury: the attacker term `5*P19(att)` is too small by `5*(min(L,3)-1)` points, so for Knight level L >= 2 the port underestimates the Injury chance by up to 10 percentage points (Type3 base about 6%). Damage is also affected. **Not patched**: wiring needs the enemy-AI owner to provide the host player level and the current-level range (the policy's inputs), which is outside a minimal B013 edit.
2. **Idle conversion missing (real divergence, not fixed).** IDA lines 129886-129896 turn a positive-damage clean hit on an idle player into Injury, and a zero-damage one into dodge. The "is player" check is inferred from vtable slot +40. Patch site would be the outcome handling in `CombatSystem::consume_marker` (`combat_system.cpp:121-165`, shared with B035). Not implemented: (a) the reference footage does not show an idle hero being hit, so the visible result is unverified; (b) the change would make an idle Knight enter the Injured state on nearly every lizard hit (3622/5000 Type1 seeds are clean hits), and `combat_session.cpp:763` treats `hurt` as busy, so it could alter combo/target paths the B035 worker is editing. Needs a decision by the owner and a reference clip.
3. **G's 100% Injury claim (corrected).** Caused by the probe's level-20 override for the lizard. Not a live-path divergence. The B013 histogram comment now says so.
4. **The reported symptom is not explained by lizard hits.** Type1/Type2 cannot Injure this Knight at live values. Clean hits cannot reset the attack in the port (3.1 above). Remaining candidates: Type3-class enemies (about 6% of hits, which the original also interrupts), other monsters not probed, a different Knight P134 (gear, class or debuff), push outcomes (B035), or the step-cursor return to sequence 470 at frames 181/319 seen by G (not explained). Needs the user's save/enemy/clip, or an outcome trace in the normal EXE.

## 4. Changes

- `port/windows-foundation/features/combat/b013_attack_continuity_v1_tests.cpp` only (probe): `#include <cstdlib>`, `<optional>`; globals `liveEnemyLevel`, `knightLevel`; enemy profile id from env `B013_ENEMY` (default unchanged); `histogram-live [knightLevel]` mode with no enemy level override; a comment marking `20*256` as probe-only; histogram output prints the enemy id. Default run unchanged.
- No production source, no `combat_session.cpp`, no `combat_system.cpp`, no `main.cpp`, no CMake. No tracker edits.
- Scratch: `.local-inputs/claude-preview13/j/` (build, runner logs, reference sheet).

## 5. Tests

- `powershell -File port/windows-foundation/features/combat/run_b013_attack_continuity_v1_tests.ps1 -AssetRoot .local-inputs/windows-source-clock-v19-preview-12/assets -BuildRoot .local-inputs/claude-preview13/j/b013-build`: **exit 0, PASS** (log `j/runner-final.log`). Knight and Rogue: ordinary/miss preserve the attack; Injure and lethal interrupt; Push gap exposed; duplicate does not reroll. Dodge and block: no seed in 1..49999 for these loadouts.
- Probe (`b013_attack_continuity_v1_tests.exe <assets> <mode> [level]`, env `B013_ENEMY`):
  - `histogram` (old, lizard level 20): 98% positive-damage seeds carry 0x10. Artifact.
  - `histogram-live 1`: Type1 0 Injury / 5000.
  - `histogram-live 5|10|20` (Type1): 0 Injury.
  - `B013_ENEMY=Swamp_LizadMan_Type2 ... histogram-live 1`: 0 Injury.
  - `B013_ENEMY=Swamp_LizadMan_Type3 ... histogram-live 1`: 241 Injury / 5000.
  - `B013_ENEMY=Swamp_MovieMoth ... histogram-live 1`: 0 Injury.
- Not run: the other `run_*` runners. No production code changed, and `combat_session_combo_tests.cpp` depends only on `combat_session.cpp` (unchanged).
- Failure and edge cases: the probe checks the enemy-level override, not the live EXE. The 3000 ms gate and the single-admission behaviour were not separately tested here; they are covered in the B013 runner's "Injure" case.

## 6. Uncertainties / not verified

- Knight P134 and gear in the user's actual save (the probe uses starting gear, no shield). A lower P134 would make Type1/Type2 Injury possible; the root should check with the save.
- Whether the original's monster level is really clamp(player level, 1, 3) for Swamp lizards in Act 1 (from the port's policy file and the actor profile, not from a direct IDA trace of `monster.luac`, which is not in the IDA export).
- The idle "is player" identity (vtable +40) is inferred.
- The reference video window 360-376 s did not isolate an incoming hit on the hero.
- The candidate EXE was not run for this pass (no new outcome trace). The Injury rate in the live EXE is inferred from the probe, which calls the same `resolve_result` path as the normal `consume_marker` (`resolve` with the same mask/category).
- Dodge/block: not reached.

## 7. Package files required

None. No runtime asset is needed by this group (no runtime content change). The probe uses the Preview 12 assets already in the package (`assets/original-cache/data/pydata/*`, `assets/actor-profiles-v2.xml`, `assets/original-melee-bindings.xml`).

## 8. Verifier script

No EXE change to verify. Reproduce the probe:

```
b013_attack_continuity_v1_tests.exe <assets-root> histogram-live 1
set B013_ENEMY=Swamp_LizadMan_Type3
b013_attack_continuity_v1_tests.exe <assets-root> histogram-live 1
```

Expected: the first run prints `Swamp_LizadMan_Type1 knight=KnightPlayerBase samples=5000 positive-damage=3682`, with no `outcomes=0x10` line. The second run prints an `outcomes=0x10 count=241` line (or close to it; the RNG is deterministic per seed, so the count should be exact).

## Summary

- B013 remains OPEN. No production change.
- Original rule confirmed (IDA): Injury = `P135(att)+5*P19(att)-P134(def)-5*P19(def) > roll*256`, 3000 ms gate, admitted Injury always leaves the swing. The port matches all of it.
- G's 100% Injury figure came from a level-20 probe override, not from the live EXE. Live: Type1/Type2 never injure the authored Knight; Type3 about 6%.
- Real divergences, not patched: monster level not fed (up to +10 points Injury for Knight level >= 2); idle clean-hit conversion missing (IDA 129886-129896).
- The reported "ordinary hits reset the attack" is not reproduced by lizard hits. Needs the user's save/enemy or an outcome trace.
