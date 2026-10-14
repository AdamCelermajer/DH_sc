# w3-dot: damage-over-time tail (general), 2026-10-14

Branch `v21.8-w3-dot`, worktree `.claude/worktrees/w3-dot`, subst `W:`, build dir `C:/dh-w3-dot/build`. Base `windows-foundation` 3e5d6dca, merged with 28dfa273 (w3-status) before the final ctest. Not pushed, not merged into windows-foundation.

## 1. Original behaviour (IDA `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c`)

- `CF_CalcDotDamage` 0x3b09c4 (line 129334). The 4th argument selects the branch. Normal (a4 = 0): duration = prop 125 (Dot_Duration), tick = prop 123 + GetRandom(prop 124 - prop 123). Elemental (a4 = 1): duration = prop 181, tick = prop 179 + GetRandom(prop 180 - prop 179), element = prop 178 >> 8, and the tick is reduced by the defender's resistance (prop element + 74) when the element is not -1.
- Caller `CF__CalcDamage` 0x3b1fb8 (line 130159). The a8 argument is the skill flag. Line 130317 calls `CF_CalcDotDamage(..., a8)`, so a skill hit takes the elemental branch and a melee hit takes the normal branch. Line 130385 is the same rule in the type-2 branch.
- `Character::F_ApplyResult` 0x3b10b4, DoT tail (lines 129862-129883): when the target is not dead and duration (word +8, >> 8) > 0 and tick (word +12) > 0, call `CharProperties::PROPS_AddDot(target + 344, duration_ms, tick, element)`. The tail is not gated on the boss flag (no boss check in the excerpt).
- `PROPS_AddDot` 0x3e2720 (line 164407): buff `dot_<element>` (normal = `dot_normal`), capacity 1 per element (a new DoT replaces the old one), tick value written to property element + 127 (126 for normal).
- `CharProperties::HandleDots` 0x3df3f0 (line 161398) runs on the DoT_Tick timer (CharacterDesign constant 1000 ms). Its per-tick F_DotAttack was not re-read in this pass.
- Lua: PoisonArrows author note says "DOT does not seem to work" (w2-archer report); the class row still carries SnS_Dot 179-181 (atlas skills.md DisplayProps 173, 179, 180, 181). Pestilence and Figments carry SnS_Dot_* (`prince_necromancer_pestilence.luac`, `prince_illusionist_figments.luac`).
- Visual evidence of the original: no original gameplay footage for these skills is in the repo. All frames below are port frames, not compared to the original.

## 2. Engine change (what was wrong, what changed)

Before this session the DoT hook (w2-assassin) read the ATTACKER's melee cells (123-125) for every applied hit. Skill hits therefore never produced their own elemental DoT (Figments, PoisonArrows, Pestilence rows carry 178-181), and DoT ticks re-queued their own DoT through the same observer.

Changes (`port/windows-foundation/combat_session.cpp`, `features/status_effects/dot_tick_owner_v1.hpp`):

1. Source (skill) hits feed the DoT owner from that hit's own `CombatResult` words (`dot_duration`, `dot_amount`, `dot_element`, computed by `dh2_combat_damage` with the skill bit 0x08000000, resistance already applied). `Impl::activeSourceResult` is set around `apply_calculated_hit` in `apply_source_result`. Skill hits no longer read the attacker's melee cells.
2. `dot_from_result_words_v1`: F_ApplyResult tail gate (duration > 0 and tick > 0), ms and HP from x256 words.
3. Ticks never re-queue: `Impl::applyingDotTick` guard in the hit observer.
4. Ticks are applied with source outcome 0 and source mask 0 (not `nullopt`). Without this, the push observer (`b035_session_push_effect_v1.cpp`) rejected any tick on a target that survived it: "Applied source result lacks original outcome or result-mask bits". The game aborted at the first tick (found in real run, see section 4).
5. Ticks run after the frame-begin provider (`sourcePhysicalFrameBegin`), which opens the enemy-contact clock for this Session serial. Before, the tick failed with "Enemy contact projection has no current Session frame/dt projection".
6. Tick receipts go to `pendingSourceHits` instead of `events`. `ActorCombatRuntime::update` clears the event list (`actor_combat_runtime.cpp`, `damage.clear()`), so receipts pushed before it were lost. Consequence before the fix: no `Source death reward` (XP/loot) for any DoT kill; the earlier Assassin run had two DoT kills with no reward line. After the fix, DoT kills reach the reward consumer with the caster (player) as recipient.

Not changed: melee-hit DoT still reads the attacker's cells (equivalent to the native normal branch for melee, but it draws its own RNG roll instead of the result's roll). Elemental DoT FX/tint is not implemented. Tick failure is still fatal (`return false` from update), as before.

## 3. Tests

- `dot_tick_owner_v1_tests`: extended with the words gate (zero/negative duration or tick adds nothing), duration and tick x256 conversion, elemental and normal DoTs on one target kept separate, and replacement of the same element. Passes.
- Full CTest, pre-merge (base 3e5d6dca + change): 216/216 passed (`ctest-full.out`).
- Full CTest, after merging 28dfa273: see section 6.

## 4. Real runs (hidden window, real keys or test aids, frames viewed, log lines)

Harness: `coordination/v21.8/act1rogue-run-rogue2.ps1` (runner), staged3 package, seeds from `C:/dh218/seeds`. Figments uses the Illusionist test-aid case (no keys, `--skill-key-frame`).

Real-run bugs found and fixed on the way (all in section 2), archer case series `w3dot-archer-poison-N`: `-2` aborted at the first tick (error text lost); `-3` showed the push-observer error "Applied source result lacks original outcome or result-mask bits" (fixed by the zero outcome/mask, item 4); `-4`/`-5` aborted at the second tick (error text lost to stdout buffering, fixed by flushing the diagnostic); `-6` showed "Enemy contact projection has no current Session frame/dt projection" (fixed by the ordering, item 5); `-7` ran to the end with two ticks but no reward line for the tick kill (fixed by the receipt path, item 6). Run `-8` is the final archer run on the code of section 2.

| Item | Verdict | Evidence |
|---|---|---|
| Figments (Illusionist, `Skill_Illusionist_Figments`) | **PARTIAL (WORKED for the DoT path only in a test-aid case)** | `w3dot/cases/fig4` (3 lizards via `--spawn-test`, cast `--skill-key-frame 150:2`). Cast: `Source skill key=2 ... frame=150 skill=Figments`. Stun and buff (pre-existing, unchanged): `Enemy arms frame=198 ... stunned=1 buffClass=Skill_Illusionist_Figments_buff durationMs=4250`. The first target missed (`outcomes=0x1` at frame 198), the second landed (`outcomes=0x0`). `DoT applied source ... target=...769 element=-1 durationMs=4250 tickHp=26.1133`. Ticks: `Damage frame=261 marker=dot_tick removed=26.1133`, frame 323 same, frame 386 `removed=10.2734 dead=1`. HP curve 62.5 -> 36.4 -> 10.3 -> dead. Frames `f300.png` (after tick 1: target bar reduced, no DoT FX visible) and `f400.png` (after the kill: HUD on the other lizard, victim pose not conclusive). Reward line for the victim has `state=3 xpRecipients=0 error=Live ActorState vital could not be applied to its source property` (open, see section 5). Second and third casts refused: `HasMana returned false` (MP 61 after cast). |
| PoisonArrows (Archer) | **WORKED (DoT path); original DoT behaviour not established** | `w3dot-archer-poison-8`. `Source skill key=2 ... frame=3426 skill=PoisonArrows`. Arrow hit `Damage frame=3463 marker=do_skill removed=10.3359`. `DoT applied source ... element=-1 durationMs=3500 tickHp=10.168`. Ticks: `Damage frame=3525 marker=dot_tick removed=10.168` (lizard survives), `Damage frame=3587 marker=dot_tick removed=9.46484 dead=1`, `Source death reward frame=3587 victim=... xpRecipients=1 xp=1` (credited to the player). Frames `f3525.png` (lizard standing, bar partly red, `Miss` text from another attack), `f3585.png` (bar nearly empty, collapsing; death cue at 3587). No DoT visual distinguishable from the arrow impact. Uncertainty: the Lua author note says PoisonArrows' DOT does not work in the original, while the native skill roll with the class SnS_Dot rows produces it; not resolved (no original footage). |
| PoisonSting (Assassin) regression | **WORKED (no regression)** | `w3dot-poison-2`. `Source skill key=2 ... frame=3336 skill=PoisonSting`. Melee-applied DoT via the attacker-cell path: `DoT applied ... durationMs=10500 tickHp=71.3477` and `70.8477`. Tick kill: `DoT tick target=4141... applied=1 targetDied=1` then `Source death reward frame=3592 ... xpRecipients=1` (this reward line was missing in the earlier `w2a-poison-merged` run; a second tick kill at 3945 gets its reward too). Frames captured (`f3380/f3420/f3500`) but not viewed in this run. |
| Putrid (Necromancer) | **DID NOT WORK (code read; NOT RUN)** | `features/generic_skills/runtime_necromancer_passive_owner_v1.cpp` applies a timed buff "Putrid Touch" to the attacker (duration TempProp2). It queues no DoT and writes no Dot_* cells, so no tick is produced. A real run would need a defender-side proc on the Necromancer (w2-necromancer report: no applied hit on the player in the earlier run). Not run in this wave. Verdict rests on the code read. |

Sounds: no sound resolution was checked in this wave (the DoT tail plays none; the cast sounds are unchanged from their own reports). Nobody has listened to any sound.

## 5. Open items and gaps

- Elemental DoT FX/tint: not implemented, not located in IDA in this pass.
- Stacking/refresh: capacity 1 per element is unit-tested only (replace on re-application). No real run re-applied a DoT before the target died.
- Boss rules: no boss check found in the F_ApplyResult DoT tail excerpt; no boss target was run.
- Monster-side DoT tick FX and the F_DotAttack per-tick path inside HandleDots were not re-read (the tick is applied as a plain applied hit with no roll, as in the w2-assassin owner).
- Death attribution: killer is the caster for source and melee DoTs (player id 18446744073709551615 in the logs); XP to the player verified on three tick kills. Loot was not checked.
- Open, not caused by this change (not investigated): (a) the reward for the `--spawn-test` lizard in fig4 reports `state=3 xpRecipients=0` with a source-property error; (b) restoring the old `figments31` case save (world `.gsave`) on the current build fails with "Saved actor ... binding incompatible: player/targetable role changed"; the same save restored fine with the older Illusionist build, so the cause is not isolated. A fresh world (no `.gsave`) was used for fig2-fig4.
- Tick failure stays fatal (returns false to the caller; the harness reports `libc++abi: terminating`). This is the pre-existing update contract.

## 6. Final state

- Commits on `v21.8-w3-dot`: `b36cefc6` (DoT change), merge `5ef23cfb` (windows-foundation 28dfa273).
- Full CTest after merge (base 28dfa273 merged, rebuilt): 216/216 passed (`ctest-merged2.out`, 526 s).
- Real runs above were made on the pre-merge build (code identical to section 2). The merge changed the push consumer (`features/combat/b035_session_push_effect_v1.cpp`), which DoT ticks pass through (item 4). No real run was repeated on the merged build; this is an open check for the lead's integration run.
