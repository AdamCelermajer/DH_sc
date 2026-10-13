# Seven native questions: original-source answers

These are source contracts and proposed known-answer cases. They do not certify runtime implementation, gameplay or visuals. Confidence applies per claim; source bodies and their callers take precedence over shorthand in earlier handoffs.

## Q1. Player and defender combat-result delivery

**PROVEN:** Character::F_ApplyResult 0x3b10b4 dispatches first to attacker CharAI at Character+968, then defender CharAI at the same offset, both virtual slot +180 with the same (attacker, defender, result). Result mask 0x20000000 suppresses both. Online authority can return before this tail. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b10b4.c:403`; `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b10b4.c:404`; `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b10b4.c:401`.

**PROVEN:** CharAI::OnCombatResults 0x3d0da4 forwards to its selected AIS at +28, virtual slot +180. Exported CharAI and AISDefault vtables map those slots to 0x3d0da4 and 0x3dc9a8 respectively. This is not intrinsically a monster-only event. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d0da4.c:5`; `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/vtables-and-rtti.json:5911`.

**PROVEN:** AISDefault::InitVCB 0x3dc7d8 records 0x800 for OnTargetHit and 0x1000 for OnTargetMissed. OnCombatResults 0x3dc9a8 sends missed when outcome byte+24 has either low bit set; otherwise hit, subject to callback registration. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003dc7d8.c:7`; `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003dc9a8.c:8`.

**PROVEN:** StealAIEvent is defined in skills/_commons.luac. It installs an AIEventProxy through AddToVFTable and invokes stored listeners while temporarily changing SKILL_ID. It is not a missing native registration named StealAIEvent. Evidence: `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:294`; `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:315`.

**PROVEN:** The frozen port has character_combat_results_pair_v1, which calls both borrowed AIS contexts. Static existence of this low-level pair does not establish that the Windows player-skill production path supplies and invokes both contexts. Evidence: `port/level-world/character_combat_results_ai_v1.cpp:30`.

Known answers (source-derived; runtime tests still required):

- **PROVEN:** Both selected AIS contexts register OnTargetHit; outcome low bits are zero; suppression mask absent. → Two ordered OnTargetHit deliveries, attacker context first, defender context second, each receiving the same actor pair.
- **PROVEN:** Result flags contain 0x20000000. → Neither CharAI receives this F_ApplyResult tail callback.
- **PROVEN:** Outcome low bit 1 is set; selected AIS registers both callbacks. → OnTargetMissed is delivered instead of OnTargetHit.

Native delivery is answered. Production player passive owner/VM integration and actual gameplay execution remain separate work.

## Q2. SnS target-chance units and comparisons

**PROVEN:** The original _F_CalculateResult 0x3b2638 requests Random(100), then shifts the random integer left by 8 before the chance helper. Random uses seed modulo the positive bound, so the comparison domain is integers 0..99 multiplied by 256. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b2638.c:184`; `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0049/00493780.c:6`; `port/level-world/reference/character-dot-attack/original-functions.asm:199`.

**PROVEN:** Mode 2 stun and push use resolved SnS chance properties 184 and 183. If the raw chance is positive, the effective threshold is chance + 5*attacker_level_raw - defender_resistance_raw - 5*defender_level_raw; strict threshold > random_raw succeeds. Stun resistance is 138; push resistance is 136. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b0fa0.c:21`; `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b0d78.c:21`; `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b0fa0.c:35`.

**PROVEN:** Values such as 80+5L and 185+4L are chance percentage points represented in fixed256, not an 8-bit probability divided by 256. A base above 100 can still fail after defender resistance and level adjustment. A successful roll is also separate from state admission, including the boss rejection in SM_SetStunState. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b2638.c:209`; `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b0fa0.c:27`; `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003c/003c5ffc.c:8`.

**PROVEN:** SkillCombatRoll(object) is a Lua wrapper passing SKILL_ID to SkillCombatRoll__(SKILL_ID, object), whose registered handler is Character::_SkillCombatRoll 0x3b9fbc; it runs F_SkillAttack and then F_ApplyResult. Evidence: `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:178`; `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9fbc.c:70`.

Known answers (source-derived; runtime tests still required):

- **PROVEN:** Mode2 stun; equal levels, zero resistance, chance85*256; supplied random84*256. → Chance succeeds; random85*256 fails because comparison is strict.
- **PROVEN:** Mode2 push; equal levels, chance185*256, resistance100*256. → Effective threshold is85*256; random85*256 fails despite the base exceeding100.
- **PROVEN:** Mode2 stun; chance0, supplied random0. → No positive base bonus is applied and 0>0 is false.

This answers the native roll units. It does not assign certainty to a specific visible proc without the actor properties, skill flags and state-admission gates.

## Q3. Class formula types 5 and 6

**PROVEN:** Type5 dispatches to _ScaleWithPercentage 0x3e2c78: destination S[d] becomes arithmetic_shift_right_8(low32(S[a]*S[d])). Type6 dispatches to _ScaleWithPercentageForBuff 0x3e2c10: scale S[a] multiplies S[d], or owner resolved[d] when the caller enables buff mode. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e2c78.c:8`; `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e2c10.c:9`; `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e2e20.c:62`.

**PROVEN:** Type6 then falls through to _AddValue 0x3e2bdc, adding raw Param1 (the scale property index). This unusual addition is actual original behavior: assembly calls 0x3e2c10 at0x3e2ef4 and 0x3e2bdc at0x3e2f04. Do not remove it as an assumed bug. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e2e20.c:64`; `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e2bdc.c:6`; `port/game-data/reference/classes/reference/original-functions.asm:183`.

**PROVEN:** For valid property indices the frozen dh2_class_formula cases5/6 match these formulas, including low32 multiplication, arithmetic right shift and the raw Param1 addition. Its buff argument denotes the owner sheet for the type6 destination operand; reversing multiplication order is equivalent. Evidence: `port/game-data/class_tables.cpp:23`; `port/game-data/class_tables.cpp:24`.

**PROVEN:** A resulting buff-sheet Max_HP delta is not the final actor Max_HP. RecalcProperty type&4 accumulates the base, saved, gear and set buff-sheet values. A smaller buff delta alone does not prove Toughness shrinks final HP. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003dfe60.c:91`; `port/game-data/properties.cpp:16`.

Known answers (source-derived; runtime tests still required):

- **PROVEN:** Type5, destination raw25600, scale raw384. → Raw result38400.
- **PROVEN:** Type6, scale property190 contains384; owner destination raw25600; buff mode enabled. → Raw buff destination38590:38400+190.
- **PROVEN:** Type6, same scale but owner destination raw51200. → Raw buff destination76990:76800+190.

Kernel formula comparison is answered; passives must still apply and combine the correct sheets through their production owner. No runtime HP claim is made.

## Q4. Buff capacity, reuse and per-property combination

**PROVEN:** PROPS_AddBuff 0x3e232c maps capacity<=0 to128. Replacement/reuse scans run only when current count equals requested capacity. Lower unsigned strength entries qualify and their strength is immediately rewritten; equal-strength permanent entries qualify; timed ties compare elapsed time through TMR_TimeLeft. This is not a general FIFO eviction policy. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e232c.c:85`; `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e232c.c:112`; `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003db344.c:14`; `port/level-world/reference/character-buffs/NOTES.md:33`.

**PROVEN:** At full capacity, if no entry qualifies, allocation is refused and nullptr returned. A qualifying entry is reused after stopping its prior timer; nonzero duration starts a one-shot event54 timer, zero duration is permanent. ResetSheet initializes defaults before SnS_Level receives strength<<8. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e232c.c:178`; `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e232c.c:200`; `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e232c.c:238`.

**PROVEN:** Combination depends on each property type from AAA_TYPES_DONT_DELETE, not a universal buff-sum rule. Type&4 accumulates; type&2 takes base then saved then gear, then first ascending declaration with a set value (last set instance in that group); type&1 favors the highest descending declaration group, then gear/saved/base. Type&32 uses base+saved; type&16 uses base. Defaults identify unset values. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003dfe60.c:181`; `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003dfe60.c:300`; `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003deed8.c:7`; `port/game-data/properties.cpp:15`.

**PROVEN:** The native buff kernel and property resolver are recovered, but Windows RuntimeTimedBuffOwnerV1 currently replaces every matching class instance on application and exposes no arbitrary native capacity selection. Its source is a partial production adaptation of the original buff-map contract. Evidence: `port/windows-foundation/features/generic_skills/runtime_timed_buff_v1.cpp:162`.

Known answers (source-derived; runtime tests still required):

- **PROVEN:** Capacity1; existing strength5; request strength4. → No eligible reusable instance; AddBuff returns null instead of evicting the stronger instance.
- **PROVEN:** Capacity1; one permanent strength5; request strength5 duration1000. → Same instance is reused, old timer is stopped, defaults reset, then a new event54 timer starts.
- **PROVEN:** Type&4 property default0, base100*256, two set buff values10*256 and20*256. → Resolved value130*256; for a nonaccumulating type this expectation is not applicable.

Exact observed gameplay stacking, visuals and production arbitrary-capacity integration remain unverified. The handoff FIFO shorthand is superseded by the original scan rules.

## Q5. RemoveBuff receiver and foreign handles

**PROVEN:** Character::_RemoveBuff 0x3b842c calls PROPS_DelBuff on the bound receiver at Character+1376. The optional pointer selects an instance inside that receiver's class declaration; it never changes the receiver to the pointer's owner. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b842c.c:40`; `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b56bc.c:614`.

**PROVEN:** The first argument must be numeric and less than ClassTable size. With one argument deletion uses nullptr; with a second pointer-tag argument the pointer is supplied to PROPS_DelBuff 0x3e101c. Invalid first types/IDs or unsupported second types do not enter deletion. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b842c.c:29`; `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b842c.c:42`.

**PROVEN:** PROPS_DelBuff removes a sole instance regardless of the supplied pointer; for multiple instances it requires a nonnull exact pointer match. Thus receiverA:RemoveBuff(class, receiverB_handle) can delete A's sole instance, but does not delete B's buff. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e101c.c:72`; `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e101c.c:108`.

Known answers (source-derived; runtime tests still required):

- **PROVEN:** A has one instance of classC; B has its own; call A:RemoveBuff(C, B_handle). → A's declaration is deleted; B remains untouched.
- **PROVEN:** A has two instances; call A:RemoveBuff(C, B_handle), where handle is not in A. → No matching instance is deleted.
- **PROVEN:** A has one instance; call A:RemoveBuff(C) with valid numeric class. → A's sole instance/declaration is removed and properties recalculated.

The native receiver rule is answered. A general production per-instance handle path still needs to preserve this behavior.

## Q6. CharAnimTable stun lookup and admission

**PROVEN:** SM_SetStunState 0x3c5ffc rejects bosses first. It obtains GetCharAnimTableId 0x3a3228 (invalid authored ID falls back to17), then validates the returned row against CharAnimTable size. It reads the Stunned field at byte140 of the160-byte native row, not an arbitrary hardcoded animation. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003c/003c5ffc.c:20`; `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003a/003a3228.c:7`; `port/level-world/reference/character-native-effects/NOTES.md:24`.

**PROVEN:** Only when pending bit2 is absent does it start timer43 for the supplied unsigned integer milliseconds; reapplying while pending does not restart that timer. The timer response is ignored by this source caller, then pending bit2 is set. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003c/003c5ffc.c:15`; `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003c/003c5ffc.c:17`.

**PROVEN:** It queries AnimStancedAnim.SL__LIST_IPHONE mask0x200. If present, GetAnimStance is added to the base Stunned sequence; otherwise modifier0. The selected sequence is stored at machine+40. Force enters state9 through _SetState; otherwise event50012 is raised. Requested mode adds flag0x800 after that transition/event. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003c/003c5ffc.c:25`; `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003c/003c5ffc.c:30`; `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003c/003c5ffc.c:34`.

Known answers (source-derived; runtime tests still required):

- **PROVEN:** Boss actor, force=true, duration3000. → No stun timer, override or state transition is performed.
- **PROVEN:** Nonboss valid row, Stunned sequence50, stance7, stanced mask0x200, pending bit clear. → Override57; timer43 starts once with3000ms; normal mode raises50012.
- **PROVEN:** Same actor receives another stun while pending bit2 is already set. → Existing stun timer is retained; animation override/event selection still executes.

Native lookup is answered. Whether the Windows production state machine actually plays and admits each actor's authored sequence requires separate integration and visual verification.

## Q7. ApplyBuff binding and pointer branch

**PROVEN:** Character::createBindings 0x3b56bc explicitly registers ApplyBuff both as global bound function and object method to Character::_ApplyPropClass 0x3babdc. The atlas alias is correct; this is not a guessed similarly named routine. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b56bc.c:641`.

**PROVEN:** A numeric class first argument and nonnull pointer second argument select PROPS_ApplyClassToSheet on the bound receiver. That routine invokes _LoadClass(sheet,class,true), then _ApplyPropClass recalculates receiver properties. A boolean second argument selects PROPS_ApplyClass instead; omitted second argument uses the nonbuff false branch. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003babdc.c:71`; `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003df314.c:7`; `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003babdc.c:72`.

**PROVEN:** The source wrapper allows class index equal to size past its `>` gate, but _LoadClass subsequently admits only indices strictly less than size. The supplied pointer is a native sheet; the wrapper does not prove ownership or lifetime beyond its null check. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003babdc.c:33`; `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e2e20.c:9`.

Known answers (source-derived; runtime tests still required):

- **PROVEN:** ApplyBuff(validClass, liveBuffSheetPointer). → Class rows execute on that sheet in buff mode, then receiver properties recalculate.
- **PROVEN:** ApplyBuff(validClass, nullPointer). → Pointer branch performs no class application or recalculation.
- **PROVEN:** ApplyPropClass(validClass, true). → Boolean branch applies the class through PROPS_ApplyClass, not the pointer-sheet buff branch.

Registration and behavior branch are answered. A pointer association alone does not guarantee that every Windows skill owner applies an equivalent buff.
