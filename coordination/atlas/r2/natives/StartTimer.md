# StartTimer

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **PARTIAL**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `StartTimer` | bindFunction | `Character::_StartTimer` | `_ZN9Character14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:132771 |
| `StartTimer` | bindMethod | `Character::_StartTimer` | `_ZN9Character14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:132776 |

Address-identified callback: `Character::_StartTimer` at `0x003b7590`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7590.c`, unified lines 133666-133705.
Recovered declaration: `void __fastcall Character::_StartTimer( Character *this, const sfc::script::lua::Arguments *a2, sfc::script::lua::ReturnValues *a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
- Lua argument 1: Lua number (tag 3), read with getUInteger; unit/scale milliseconds; AI _commons names it timeMs. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7590.c:20`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7590.c:26`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7590.c:27`, `.local-inputs/assets-extra/android/data/scripts/ai/_commons.luac:26`.
- Lua argument 2: optional value converted with getBool; unit/scale repeat mode: true -> -1; false/absent -> 0. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7590.c:22`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7590.c:24`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7590.c:28`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7590.c:34`.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: With a numeric first argument, starts a timer owned by this Character's CharTimers member (byte offset 948). It passes the duration, repeat=-1 for true or repeat=0 otherwise, event code 53 and null user reference. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7590.c:17`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7590.c:20`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7590.c:28`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7590.c:34`.
**Gates:**
- PROVEN: A missing first argument or a first argument whose Lua type tag is not 3 skips the timer call. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7590.c:17`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7590.c:19`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7590.c:20`.
**State, timers and events:**
- PROVEN: The native schedules event 53 with no callback pointer. Lua callback dispatch is provided by the AI _commons TimerEvents table, not by the native timer call. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7590.c:28`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7590.c:34`, `.local-inputs/assets-extra/android/data/scripts/ai/_commons.luac:30`, `.local-inputs/assets-extra/android/data/scripts/ai/_commons.luac:32`, `.local-inputs/assets-extra/android/data/scripts/ai/_commons.luac:95`, `.local-inputs/assets-extra/android/data/scripts/ai/_commons.luac:99`, `.local-inputs/assets-extra/android/data/scripts/ai/_commons.luac:103`.
**Return:**
- PROVEN: Pushes the timer ID as one Lua integer when TMR_Start returns anything other than -1; pushes no Lua return value on -1. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7590.c:36`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7590.c:37`.

## Lua wrapper/helper traces

Proven source chain: `SetSkillCooldown` -> `StartSkillTimerCB` -> `StartTimerCB` -> `StartTimer`.
Proven source chain: `SetSkillCooldown` -> `StartSkillTimerCB` -> `StartTimerCB` -> `StartTimer`.
Proven source chain: `StartSkillTimerCB` -> `StartTimerCB` -> `StartTimer`.
Proven source chain: `StartSkillTimerCB` -> `StartTimerCB` -> `StartTimer`.
Proven source chain: `StartTimerCB` -> `StartTimer`.
Proven source chain: `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:379` `StartTimerCB(ATTACK_DELAY, true, isAttackReady)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:380` `StartTimerCB(MOVE_DELAY, true, DoTeleport)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:381` `StartTimerCB(RANGE_ATTACK_DELAY, true, isRangeAttackReady)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:384` `StartTimerCB(SEARCH_DELAY, true, searchForEfficientTarget)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:388` `StartTimerCB(SP_ATTACK_DELAY, true, Do_SpecialAttack)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/dummy.luac:36` `StartTimerCB(ATTACK_DELAY, true, StealTargets)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:213` `StartTimerCB(ATTACK_DELAY, true, isAttackReady)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:214` `StartTimerCB(MOVE_DELAY, true, DoTeleport)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:215` `StartTimerCB(RANGE_ATTACK_DELAY, true, isRangeAttackReady)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/energyspawner.luac:84` `StartTimerCB(m_timer_attack, false, energyspawner_DoDamage)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:336` `StartTimerCB(ATTACK_DELAY, true, isAttackReady)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:339` `StartTimerCB(SEARCH_DELAY, true, searchForEfficientTarget)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:344` `StartTimerCB(SP_ATTACK_DELAY, true, Do_SpecialAttack)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:489` `StartTimerCB( 500, false, performingSpecialAttack )`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:366` `StartTimerCB(ATTACK_DELAY, true, isAttackReady)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:389` `StartTimerCB(SEARCH_DELAY, true, searchForEfficientTarget)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:393` `StartTimerCB(SP_ATTACK_DELAY, true, Do_SpecialAttack)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee.luac:211` `StartTimerCB(1500, true, referee_playersearch)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee.luac:223` `StartTimerCB(500, true, UpdatePlayersWithPosition)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee.luac:434` `StartTimerCB(delay, true, cb)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee.luac:447` `StartTimerCB(delay, false, cb)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee.luac:448` `StartTimerCB(delay, false, cb)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee.luac:616` `StartTimerCB(WARMUP_TICK_TIME, true, StartFirstWave)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee.luac:692` `StartTimerCB(WARMUP_TICK_TIME_START, true, Warmup)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee.luac:708` `StartTimerCB(1000, false, playAlarmAnimation)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee.luac:709` `StartTimerCB(3000, false, goToNextDestination)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee.luac:710` `StartTimerCB(WARMUP_TICK_TIME, true, StartNextWave)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee.luac:718` `StartTimerCB(1000, false, playAlarmAnimation)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee.luac:719` `StartTimerCB(3000, false, goToNextDestination)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee.luac:720` `StartTimerCB(WARMUP_TICK_TIME, true, StartNextWave)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee_vip_escort_big.luac:39` `StartTimerCB(DEBUFF_DELAY, true, ApplyPlayerDebuff)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee_vip_escort_big.luac:143` `StartTimerCB(1000, true, UpdatePlayerPosition)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/vip_core.luac:199` `StartTimerCB(DEBUFF_DELAY, true, ApplyPlayerDebuff)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/madruk_buffer.luac:71` `StartTimerCB(TIMER_DELAY, false, Do_Buff)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:218` `StartTimerCB(RESPAWN_DELAY, false, SummonHelpers, "Healer")`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:226` `StartTimerCB(RESPAWN_DELAY, false, SummonHelpers, "Buffer")`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:389` `StartTimerCB(ATTACK_DELAY, true, isAttackReady)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:390` `StartTimerCB(HELPER_CHECK_DELAY, true, isHelpersAlive)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/madruk_healer.luac:82` `StartTimerCB(TIMER_DELAY, false, Do_Healing)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:289` `StartTimerCB(ATTACK_DELAY, true, Attack)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/mothcocoon_core.luac:178` `StartTimerCB(m_timer_time_spawn, true, SummonMothMinion)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/rene.luac:200` `StartTimerCB(200, false, CaughtUp)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/resurrectinggod.luac:96` `StartTimerCB(g_timer, true, ResurrectingGodTimer)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:216` `StartTimerCB(ATTACK_DELAY, true, isAttackReady)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:219` `StartTimerCB(SEARCH_DELAY, true, searchForEfficientTarget)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:224` `StartTimerCB(SP_ATTACK_DELAY, true, Do_SpecialAttack)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:368` `StartTimerCB( 500, false, performingSpecialMeleeAttack )`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:373` `StartTimerCB( 500, false, performingSpecialAttack )`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:378` `StartTimerCB( 500, false, performingSpecialAttack )`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:337` `StartTimerCB(ATTACK_DELAY, true, isAttackReady)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:354` `StartTimerCB(OUTOFRANGE_DELAY, false, SetSwampkingInDiveState)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:465` `StartTimerCB(DIVE_DELAY, false, SetSwampkingInIdleState)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:477` `StartTimerCB(500, false, function() DoSkill(2); end)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:367` `StartTimerCB(OUTOFRANGE_DELAY, false, SetSwampkingInDiveState)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:377` `StartTimerCB(DIVE_DELAY, false, SetSwampkingInDiveState)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:457` `StartTimerCB(DIVE_DELAY, false, SetSwampkingInIdleState)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:469` `StartTimerCB(500, false, function() DoSkill(2); end)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:484` `StartTimerCB(WEAK_DELAY, false, SetSwampkingInIdleState)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:294` `StartTimerCB(OUTOFRANGE_DELAY, false, SetSwampkingInDiveState)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:420` `StartTimerCB(DIVE_DELAY, false, SetSwampkingInIdleState)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:432` `StartTimerCB(500, false, function() DoSkill(2); end)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:447` `StartTimerCB(WEAK_DELAY, false, SetSwampkingInIdleState)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/soundtest/referee_sdd.luac:242` `StartTimerCB(delay, true, cb)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/soundtest/referee_sdd.luac:255` `StartTimerCB(delay, false, cb)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/soundtest/referee_sdd.luac:256` `StartTimerCB(delay, false, cb)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:373` `StartTimerCB(500, false, GoToDivePattern)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:388` `StartTimerCB(INDIVEPATTERN_DELAY, true, SetSwampkingInDiveState)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:397` `StartTimerCB(GOTODIVE_DELAY, true, GoToDivePattern)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:668` `StartTimerCB(ATTACK_DELAY, true, isAttackReady)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:669` `StartTimerCB(GOTODIVE_DELAY, true, GoToDivePattern)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:740` `StartTimerCB(500, false, MoveToPosition)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:741` `StartTimerCB(DIVE_DELAY, false, SetSwampkingInEmergeState)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:742` `StartTimerCB(3000, false, TargetAgain)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:749` `StartTimerCB(500, false, DiveAttackDecision)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:761` `StartTimerCB(500, false, function() DoSkill(2); end)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:295` `StartTimerCB(ATTACK_DELAY, true, isAttackReady)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:318` `StartTimerCB(SEARCH_DELAY, true, searchForEfficientTarget)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:322` `StartTimerCB(SP_ATTACK_DELAY, true, Do_SpecialAttack)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:151` `StartTimerCB(MOVE_DELAY, true, DoTeleport)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:149` `StartTimerCB(timeMs, loop, SkillTimerCB_, tmpData)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:207` `StartSkillTimerCB(delay, false, SkillCooldownExpiredCB_, SKILL_NAME)`; chain `StartSkillTimerCB` -> `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:229` `StartTimerCB(delay, false, SpellCooldownExpiredCB_)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/balrog_flames_skill.luac:93` `StartSkillTimerCB(DAMAGE_TICK, true, doDamage)`; chain `StartSkillTimerCB` -> `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/edward_rise.luac:166` `StartSkillTimerCB(bufftime, false, killed, spawnSlot)`; chain `StartSkillTimerCB` -> `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/edward_rise.luac:295` `StartSkillTimerCB(bufftime, false, killed, count)`; chain `StartSkillTimerCB` -> `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/flaminite_sp_attack.luac:88` `StartSkillTimerCB(1000, true, checktarget)`; chain `StartSkillTimerCB` -> `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/lighthouseemissary_ranged_00.luac:88` `StartSkillTimerCB(DAMAGE_TICK, true, doDamage)`; chain `StartSkillTimerCB` -> `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_drainlife.luac:98` `StartSkillTimerCB(2000, 0, drainlife)`; chain `StartSkillTimerCB` -> `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/npc_dummy.luac:76` `StartSkillTimerCB(2000, false, killstatue)`; chain `StartSkillTimerCB` -> `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/npc_dummy.luac:90` `StartSkillTimerCB(TNT_DELAY, false, DoExplosion)`; chain `StartSkillTimerCB` -> `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/npc_dummy.luac:91` `StartSkillTimerCB(350, true, StealTargets)`; chain `StartSkillTimerCB` -> `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_exploding_arrows.luac:102` `SetSkillCooldown(cooldown)`; chain `SetSkillCooldown` -> `StartSkillTimerCB` -> `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_exploding_arrows.luac:130` `StartSkillTimerCB(500, 0, DoExplosion, target)`; chain `StartSkillTimerCB` -> `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_pin_shot.luac:98` `SetSkillCooldown(cooldown)`; chain `SetSkillCooldown` -> `StartSkillTimerCB` -> `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_pin_shot.luac:120` `StartSkillTimerCB(SWITCH_TARGET_DELAY, false, PinShot, target)`; chain `StartSkillTimerCB` -> `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_pin_shot.luac:128` `StartSkillTimerCB((SWITCH_TARGET_DELAY + SWITCH_TARGET_DELAY), false, PinShot, target)`; chain `StartSkillTimerCB` -> `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_precision.luac:32` `StartSkillTimerCB(1000, true, checkbuff)`; chain `StartSkillTimerCB` -> `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_precision.luac:67` `StartSkillTimerCB(1000, true, checkbuff)`; chain `StartSkillTimerCB` -> `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_sidestep.luac:126` `StartSkillTimerCB(500, true, check, attacker)`; chain `StartSkillTimerCB` -> `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassination.luac:74` `SetSkillCooldown(cooldown)`; chain `SetSkillCooldown` -> `StartSkillTimerCB` -> `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_blindness.luac:99` `StartSkillTimerCB(BUFF_TIME, false, BuffExpired)`; chain `StartSkillTimerCB` -> `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_poison_sting.luac:57` `StartSkillTimerCB(BUFF_TIME, 0, BuffExpired)`; chain `StartSkillTimerCB` -> `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_killing_frenzy.luac:52` `StartSkillTimerCB(BUFF_DELAY, false, resetBuff)`; chain `StartSkillTimerCB` -> `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_killing_frenzy.luac:66` `StartSkillTimerCB(BUFF_DELAY, false, resetBuff)`; chain `StartSkillTimerCB` -> `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_killing_frenzy.luac:80` `StartSkillTimerCB(BUFF_MAX_DELAY, false, resetBuff)`; chain `StartSkillTimerCB` -> `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_survival.luac:39` `StartTimerCB(BUFF_TIMER, false, StopBuffTimer)`; chain `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_warcry.luac:138` `StartSkillTimerCB(bufftime, false, BuffExpired)`; chain `StartSkillTimerCB` -> `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_disciples.luac:53` `StartSkillTimerCB(bufftime, false, BuffExpired, counter, defender)`; chain `StartSkillTimerCB` -> `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_figments.luac:74` `SetSkillCooldown(cooldown)`; chain `SetSkillCooldown` -> `StartSkillTimerCB` -> `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_terror.luac:124` `StartSkillTimerCB(BUFF_DURATION, false, delete_slot, 1)`; chain `StartSkillTimerCB` -> `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_terror.luac:131` `StartSkillTimerCB(BUFF_DURATION, false, delete_slot, 2)`; chain `StartSkillTimerCB` -> `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_terror.luac:138` `StartSkillTimerCB(BUFF_DURATION, false, delete_slot, 3)`; chain `StartSkillTimerCB` -> `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_timecontrol.luac:74` `SetSkillCooldown(cooldown)`; chain `SetSkillCooldown` -> `StartSkillTimerCB` -> `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_cyclone.luac:140` `StartSkillTimerCB(TICK_DELAY, true, HandleTick)`; chain `StartSkillTimerCB` -> `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_stone_escape.luac:163` `StartSkillTimerCB(TNT_DELAY, false, DoExplosion, statue)`; chain `StartSkillTimerCB` -> `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_deathclaw.luac:66` `SetSkillCooldown(cooldown)`; chain `SetSkillCooldown` -> `StartSkillTimerCB` -> `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_fireskullrain.luac:144` `SetSkillCooldown(cooldown)`; chain `SetSkillCooldown` -> `StartSkillTimerCB` -> `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_fireskullrain.luac:150` `StartSkillTimerCB(TICK_DELAY, true, DamageTick)`; chain `StartSkillTimerCB` -> `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_pestilence.luac:125` `StartSkillTimerCB(500, true, UpdateAttackedTargets)`; chain `StartSkillTimerCB` -> `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_pestilence.luac:191` `StartSkillTimerCB((timeofdot + 1000), false, RemoveTarget, target )`; chain `StartSkillTimerCB` -> `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_rise.luac:163` `StartSkillTimerCB(bufftime, false, killed, spawnSlot)`; chain `StartSkillTimerCB` -> `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_rise.luac:299` `StartSkillTimerCB(bufftime, false, killed, count)`; chain `StartSkillTimerCB` -> `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_wrath.luac:55` `StartSkillTimerCB(TIMER_DELAY, true, canApplyBuff)`; chain `StartSkillTimerCB` -> `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_conversion.luac:127` `StartSkillTimerCB(2000, 0, BuffExpired, defender)`; chain `StartSkillTimerCB` -> `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_divine_judgement.luac:76` `SetSkillCooldown(cooldown)`; chain `SetSkillCooldown` -> `StartSkillTimerCB` -> `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_energy_balance.luac:90` `StartSkillTimerCB(BUFF_DELAY, true, updateSelfBuff)`; chain `StartSkillTimerCB` -> `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_guarding_aura.luac:144` `StartSkillTimerCB(BUFF_TIME, 0, BuffExpired)`; chain `StartSkillTimerCB` -> `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_offensive_aura.luac:139` `StartSkillTimerCB(BUFF_TIME, 0, BuffExpired)`; chain `StartSkillTimerCB` -> `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_shield.luac:138` `StartSkillTimerCB(500, true, check, attacker)`; chain `StartSkillTimerCB` -> `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_shield.luac:141` `StartSkillTimerCB(500, true, check2, attacker)`; chain `StartSkillTimerCB` -> `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_quickness.luac:96` `StartSkillTimerCB(bufftime, 0, BuffExpired)`; chain `StartSkillTimerCB` -> `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_charge.luac:89` `SetSkillCooldown(cooldown)`; chain `SetSkillCooldown` -> `StartSkillTimerCB` -> `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_ground_slam.luac:71` `SetSkillCooldown(cooldown)`; chain `SetSkillCooldown` -> `StartSkillTimerCB` -> `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_leadership.luac:65` `StartSkillTimerCB(BUFF_DELAY, true, startGroupBuff)`; chain `StartSkillTimerCB` -> `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:234` `StartSkillTimerCB(LAUNCHER_DELAY, true, Do_PlayFX)`; chain `StartSkillTimerCB` -> `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:235` `StartSkillTimerCB(DAMAGE_DELAY, true, Do_Damages)`; chain `StartSkillTimerCB` -> `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:250` `StartSkillTimerCB(timer_time, true, DoDamage)`; chain `StartSkillTimerCB` -> `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_insidesweep.luac:191` `StartSkillTimerCB(timer_time, true, DoDamage)`; chain `StartSkillTimerCB` -> `StartTimerCB` -> `StartTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_outsidesweep.luac:181` `StartSkillTimerCB(timer_time, true, DoDamage)`; chain `StartSkillTimerCB` -> `StartTimerCB` -> `StartTimer`.
- AI _commons StartTimerCB(timeMs, loop, callback, refData) calls StartTimer and records the Lua callback/refData by timer ID. OnTimer(timerId) invokes callback(timerId, refData), then clears the callback record only for non-looping timers. StopTimerCB clears the record before it calls StopTimer. Evidence: `.local-inputs/assets-extra/android/data/scripts/ai/_commons.luac:26`, `.local-inputs/assets-extra/android/data/scripts/ai/_commons.luac:30`, `.local-inputs/assets-extra/android/data/scripts/ai/_commons.luac:32`, `.local-inputs/assets-extra/android/data/scripts/ai/_commons.luac:38`, `.local-inputs/assets-extra/android/data/scripts/ai/_commons.luac:39`, `.local-inputs/assets-extra/android/data/scripts/ai/_commons.luac:40`, `.local-inputs/assets-extra/android/data/scripts/ai/_commons.luac:95`, `.local-inputs/assets-extra/android/data/scripts/ai/_commons.luac:99`, `.local-inputs/assets-extra/android/data/scripts/ai/_commons.luac:103`.
- The separate skills _commons layer StartSkillTimerCB captures SKILL_ID in its callback envelope, then calls AI StartTimerCB. SkillTimerCB_ temporarily installs the captured SKILL_ID during the callback and restores the prior value afterward. Evidence: `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:136`, `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:138`, `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:139`, `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:140`, `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:141`, `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:146`, `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:147`, `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:149`.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**PARTIAL:** The current source-value bridge matches the successful duration/repeat/event/ID path. When the timer start service reports failure, character_script_timers.cpp converts that failure into a Lua callback error; the original handler simply pushes no values when TMR_Start returns -1.
- Source evidence: `port/script-runtime/script_game_bindings.c`:34.
- Source evidence: `port/script-runtime/script_game_bindings.c`:36.
- Source evidence: `port/script-runtime/script_game_bindings.c`:37.
- Source evidence: `port/script-runtime/script_game_bindings.c`:38.
- Source evidence: `port/script-runtime/script_game_bindings.c`:39.
- Source evidence: `port/level-world/character_script_timers.cpp`:10.
- Source evidence: `port/level-world/character_script_timers.cpp`:11.
- Source evidence: `port/level-world/character_script_timers.cpp`:27.
- Source evidence: `port/level-world/character_script_timers.cpp`:31.

**Concrete behavioral difference:** The current source-value bridge matches the successful duration/repeat/event/ID path. When the timer start service reports failure, character_script_timers.cpp converts that failure into a Lua callback error; the original handler simply pushes no values when TMR_Start returns -1.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: StartTimer(200), manager returns ID 4. Calls TMR_Start(duration=200 ms, repeat=0, event=53, reference=null) and pushes integer 4. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7590.c`:20, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7590.c`:32, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7590.c`:34, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7590.c`:36, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7590.c`:37.
2. **PROVEN:** Given: StartTimer(200, true), manager returns ID 4. Calls TMR_Start(duration=200 ms, repeat=-1, event=53, reference=null) and pushes integer 4. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7590.c`:22, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7590.c`:26, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7590.c`:27, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7590.c`:28, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7590.c`:36, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7590.c`:37.
3. **PROVEN:** Given: StartTimer(200), manager returns -1. Pushes zero Lua return values. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7590.c`:36, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7590.c`:37.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **9** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/ai/_commons.luac:28` | autoloaded_lua_common_helpers | `StartTimer(timeMs, loop)` | `[{"index":1,"expression":"timeMs","literal_tokens":[]},{"index":2,"expression":"loop","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/_commons.luac:30` | autoloaded_lua_common_helpers | `StartTimer(timeMs, loop)` | `[{"index":1,"expression":"timeMs","literal_tokens":[]},{"index":2,"expression":"loop","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/coward_core.luac:21` | outside scoped roots | `StartTimer(200)` | `[{"index":1,"expression":"200","literal_tokens":["200"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/coward_core.luac:41` | outside scoped roots | `StartTimer(15000)` | `[{"index":1,"expression":"15000","literal_tokens":["15000"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/coward_core.luac:42` | outside scoped roots | `StartTimer(200)` | `[{"index":1,"expression":"200","literal_tokens":["200"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/miniboss_state.luac:48` | outside scoped roots | `StartTimer(5000)` | `[{"index":1,"expression":"5000","literal_tokens":["5000"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/miniboss_state.luac:51` | outside scoped roots | `StartTimer(5000)` | `[{"index":1,"expression":"5000","literal_tokens":["5000"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/miniboss_state.luac:64` | outside scoped roots | `StartTimer(GetRand(3000, 7000))` | `[{"index":1,"expression":"GetRand(3000, 7000)","literal_tokens":["3000","7000"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/miniboss_state.luac:68` | outside scoped roots | `StartTimer(GetRand(3000, 7000))` | `[{"index":1,"expression":"GetRand(3000, 7000)","literal_tokens":["3000","7000"]}]` |

## Related natives

`Character::_StartTimer`, `StartTimer`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
