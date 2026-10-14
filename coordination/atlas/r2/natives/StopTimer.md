# StopTimer

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **PORTED**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `StopTimer` | bindFunction | `Character::_StopTimer` | `_ZN9Character14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:132798 |
| `StopTimer` | bindMethod | `Character::_StopTimer` | `_ZN9Character14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:132803 |

Address-identified callback: `Character::_StopTimer` at `0x003b7064`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7064.c`, unified lines 133428-133450.
Recovered declaration: `int __fastcall Character::_StopTimer( int this, const sfc::script::lua::Arguments *a2, sfc::script::lua::ReturnValues *a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
- Lua argument 1: Lua number (tag 3), read with getUInteger; unit/scale zero-based timer slot ID in the bound Character's CharTimers collection. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7064.c:12`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7064.c:14`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7064.c:16`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7064.c:17`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7064.c:18`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:148`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:149`.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: With a numeric first argument, calls TMR_Stop on the bound Character timer manager. TMR_Stop clears that slot’s active byte only when the ID is below the allocated slot count; an out-of-range ID leaves timer state unchanged. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7064.c:11`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7064.c:18`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:148`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:149`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:150`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:151`.
**Gates:**
- PROVEN: No TMR_Stop call occurs when argument 1 is missing or its Lua type tag is not 3. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7064.c:11`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7064.c:12`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7064.c:14`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7064.c:18`.
**Return:**
- PROVEN: No Lua values are returned: _StopTimer does not push a ReturnValues item, and Binder::__functionCallback / __smethodCallback invoke the handler through a void callback pointer before returning only ReturnValues::_doReturn's count. The handler's int C result and TMR_Stop's manager-pointer result are ignored by those adapters. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7064.c:18`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7064.c:21`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:151`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:11849`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:11850`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:11789`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:11790`.

## Lua wrapper/helper traces

Proven source chain: `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:408` `StopTimerCB(timer_attack_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:412` `StopTimerCB(timer_teleport_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:416` `StopTimerCB(timer_rangeattack_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:420` `StopTimerCB(timed_spotTargeting)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:424` `StopTimerCB(timed_specialAttack)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:37` `StopTimerCB(timer_attack_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:41` `StopTimerCB(timer_spot_targeting)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:45` `StopTimerCB(timer_sp_attack_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:49` `StopTimerCB(timer_teleport_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:53` `StopTimerCB(timer_rangeattack_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:234` `StopTimerCB(timer_attack_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:238` `StopTimerCB(timer_spot_targeting)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:242` `StopTimerCB(timer_sp_attack_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:246` `StopTimerCB(timer_teleport_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:250` `StopTimerCB(timer_rangeattack_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/energyspawner.luac:69` `StopTimerCB(t_timer)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/energyspawner.luac:96` `StopTimerCB(t_timer)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:31` `StopTimerCB(timer_attack_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:35` `StopTimerCB(timer_spot_targeting)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:39` `StopTimerCB(timer_sp_attack_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:363` `StopTimerCB(timer_attack_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:367` `StopTimerCB(timer_spot_targeting)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:371` `StopTimerCB(timer_sp_attack_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:30` `StopTimerCB(timer_attack_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:34` `StopTimerCB(timer_spot_targeting)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:38` `StopTimerCB(timer_sp_attack_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:413` `StopTimerCB(timer_attack_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee.luac:153` `StopTimerCB(g_tick_timer)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee.luac:157` `StopTimerCB(g_timer_playerPos)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee.luac:161` `StopTimerCB(g_debuff_timer)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee.luac:165` `StopTimerCB(g_timer_addPlayer)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee.luac:170` `StopTimerCB(g_warmup_timer)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee.luac:174` `StopTimerCB(g_destcheck_timer)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee.luac:191` `StopTimerCB(g_wave_timer)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee.luac:192` `StopTimerCB(g_wave_to_timer)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee.luac:193` `StopTimerCB(g_tick_timer)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee.luac:194` `StopTimerCB(g_debuff_timer)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee.luac:431` `StopTimerCB(g_tick_timer)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee.luac:440` `StopTimerCB(g_wave_to_timer)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee.luac:444` `StopTimerCB(g_wave_timer)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee.luac:573` `StopTimerCB(g_destcheck_timer)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee.luac:593` `StopTimerCB(g_destcheck_timer)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee.luac:610` `StopTimerCB(g_warmup_timer)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee.luac:631` `StopTimerCB(g_warmup_timer)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/madruk_buffer.luac:79` `StopTimerCB(g_heal_timer)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/madruk_buffer.luac:88` `StopTimerCB(g_heal_timer)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:66` `StopTimerCB(timer_summonHealer_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:70` `StopTimerCB(timer_summonBuffer_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:74` `StopTimerCB(timer_attack_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:78` `StopTimerCB(timer_helper_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:420` `StopTimerCB(timer_attack_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:421` `StopTimerCB(timer_helper_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:422` `StopTimerCB(timer_summonBuffer_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:423` `StopTimerCB(timer_summonHealer_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/madruk_healer.luac:90` `StopTimerCB(g_heal_timer)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/madruk_healer.luac:99` `StopTimerCB(g_heal_timer)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:301` `StopTimerCB(timer_attack_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:310` `StopTimerCB(timer_attack_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/mothcocoon_core.luac:185` `StopTimerCB(m_timer_spawn)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/mothcocoon_core.luac:194` `StopTimerCB(m_timer_spawn)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/resurrectinggod.luac:54` `StopTimerCB(m_timerID)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/resurrectinggod.luac:128` `StopTimerCB(m_timerID)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/resurrectinggod.luac:139` `StopTimerCB(m_timerID)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:243` `StopTimerCB(timer_attack_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:247` `StopTimerCB(timer_spot_targeting)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:251` `StopTimerCB(timer_sp_attack_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:14` `StopTimerCB(timer_attack_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:18` `StopTimerCB(timer_outOfRange_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:22` `StopTimerCB(timer_dive_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:26` `StopTimerCB(timer_intimidate_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:423` `StopTimerCB(timer_attack_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:427` `StopTimerCB(timer_outOfRange_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:431` `StopTimerCB(timer_dive_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:435` `StopTimerCB(timer_intimidate_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:257` `StopTimerCB(timer_attack_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:261` `StopTimerCB(timer_outOfRange_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:265` `StopTimerCB(timer_idle_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:269` `StopTimerCB(timer_intimidate_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:273` `StopTimerCB(timer_weak_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:380` `StopTimerCB(timer_attack_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:384` `StopTimerCB(timer_outOfRange_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:388` `StopTimerCB(timer_idle_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:392` `StopTimerCB(timer_intimidate_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:396` `StopTimerCB(timer_weak_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/soundtest/referee_sdd.luac:239` `StopTimerCB(g_tick_timer)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/soundtest/referee_sdd.luac:248` `StopTimerCB(g_wave_to_timer)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/soundtest/referee_sdd.luac:252` `StopTimerCB(g_wave_timer)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/soundtest/referee_sdd.luac:261` `StopTimerCB(g_wave_timer)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/soundtest/referee_sdd.luac:262` `StopTimerCB(g_wave_to_timer)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/soundtest/referee_sdd.luac:263` `StopTimerCB(g_tick_timer)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:141` `StopTimerCB(timer_attack_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:145` `StopTimerCB(timer_diving_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:149` `StopTimerCB(timer_divepattern_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:153` `StopTimerCB(emerge_timer)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:157` `StopTimerCB(dive_timer)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:161` `StopTimerCB(skill2_timer)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:386` `StopTimerCB(timer_diving_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:395` `StopTimerCB(timer_divepattern_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:553` `StopTimerCB(timer_attack_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:556` `StopTimerCB(timer_diving_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:559` `StopTimerCB(timer_divepattern_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:656` `StopTimerCB(timer_attack_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:657` `StopTimerCB(timer_diving_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:694` `StopTimerCB(timer_attack_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:698` `StopTimerCB(timer_diving_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:702` `StopTimerCB(timer_divepattern_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:706` `StopTimerCB(emerge_timer)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:710` `StopTimerCB(dive_timer)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:714` `StopTimerCB(skill2_timer)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:30` `StopTimerCB(timer_attack_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:34` `StopTimerCB(timer_spot_targeting)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:38` `StopTimerCB(timer_sp_attack_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:342` `StopTimerCB(timer_attack_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:129` `StopTimerCB(timer_teleport_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:162` `StopTimerCB(timer_teleport_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:220` `StopTimerCB(timer_teleport_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:229` `StopTimerCB(timer_teleport_flag)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/balrog_flames_skill.luac:53` `StopTimerCB(timer)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/edward_rise.luac:58` `StopTimerCB(firstRiseTimer)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/edward_rise.luac:69` `StopTimerCB(secondRiseTimer)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/edward_rise.luac:80` `StopTimerCB(thirdRiseTimer)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/edward_rise.luac:162` `StopTimerCB(timerToStop)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/flaminite_sp_attack.luac:45` `StopTimerCB(timer)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/lighthouseemissary_ranged_00.luac:50` `StopTimerCB(timer)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/npc_dummy.luac:71` `StopTimerCB(timer_id_steal)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_precision.luac:21` `StopTimerCB(timer_check)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_precision.luac:40` `StopTimerCB(timer)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_precision.luac:62` `StopTimerCB(timer)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_sidestep.luac:104` `StopTimerCB(timeq)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_sidestep.luac:110` `StopTimerCB(timeq)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_killing_frenzy.luac:64` `StopTimerCB(timer_resetBuff)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_killing_frenzy.luac:78` `StopTimerCB(timer_resetBuff)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_cyclone.luac:159` `StopTimerCB(timer_tick)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_fireskullrain.luac:82` `StopTimerCB(timer_id)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_fireskullrain.luac:99` `StopTimerCB(timer_id)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_pestilence.luac:122` `StopTimerCB(timer)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_rise.luac:58` `StopTimerCB(firstRiseTimer)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_rise.luac:69` `StopTimerCB(secondRiseTimer)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_rise.luac:80` `StopTimerCB(thirdRiseTimer)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_rise.luac:159` `StopTimerCB(timerToStop)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_wrath.luac:51` `StopTimerCB(timer_wrath)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_energy_balance.luac:83` `StopTimerCB(timer_buff)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_guarding_aura.luac:103` `StopTimerCB(buff_timer)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_offensive_aura.luac:96` `StopTimerCB(buff_timer)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_shield.luac:102` `StopTimerCB(timeq)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_shield.luac:108` `StopTimerCB(timeq)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_shield.luac:116` `StopTimerCB(timeq)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_shield.luac:122` `StopTimerCB(timeq)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_leadership.luac:57` `StopTimerCB(timer_buff)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:126` `StopTimerCB(timer_damage_dealer)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:167` `StopTimerCB(timer_fx_launcher)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:209` `StopTimerCB(timer_id)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:257` `StopTimerCB(timer_id)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_insidesweep.luac:179` `StopTimerCB(timer_id)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_insidesweep.luac:199` `StopTimerCB(timer_id)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_outsidesweep.luac:169` `StopTimerCB(timer_id)`; chain `StopTimerCB` -> `StopTimer`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_outsidesweep.luac:189` `StopTimerCB(timer_id)`; chain `StopTimerCB` -> `StopTimer`.
- AI _commons StopTimerCB(timerId) first removes TimerEvents[timerId], then calls StopTimer. Its OnTimer callback removes a one-shot record after callback delivery. Evidence: `.local-inputs/assets-extra/android/data/scripts/ai/_commons.luac:38`, `.local-inputs/assets-extra/android/data/scripts/ai/_commons.luac:39`, `.local-inputs/assets-extra/android/data/scripts/ai/_commons.luac:40`, `.local-inputs/assets-extra/android/data/scripts/ai/_commons.luac:95`, `.local-inputs/assets-extra/android/data/scripts/ai/_commons.luac:99`, `.local-inputs/assets-extra/android/data/scripts/ai/_commons.luac:103`.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**PORTED:** The inspected direct path matches: numeric IDs call the bound timer owner, in-range IDs deactivate that slot, out-of-range IDs leave slots unchanged, and the source-value binding returns zero Lua values. This is source comparison only, not integrated/runtime parity verification.
- Source evidence: `port/script-runtime/script_game_bindings.c`:46.
- Source evidence: `port/script-runtime/script_game_bindings.c`:48.
- Source evidence: `port/script-runtime/script_game_bindings.c`:49.
- Source evidence: `port/script-runtime/script_game_bindings.c`:50.
- Source evidence: `port/level-world/character_timers.cpp`:50.
- Source evidence: `port/level-world/character_timers.cpp`:51.
- Source evidence: `port/level-world/character_timers.cpp`:52.
- Source evidence: `port/level-world/character_timers.cpp`:53.
- Source evidence: `port/level-world/character_script_timers.cpp`:16.
- Source evidence: `port/level-world/character_script_timers.cpp`:17.
- Source evidence: `port/level-world/character_script_timers.cpp`:18.
- Source evidence: `port/level-world/character_script_timers.cpp`:27.

**Port comparison:** The inspected direct path matches: numeric IDs call the bound timer owner, in-range IDs deactivate that slot, out-of-range IDs leave slots unchanged, and the source-value binding returns zero Lua values. This is source comparison only, not integrated/runtime parity verification.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: StopTimer(2), with allocated active timer slot 2. Clears the active byte of timer slot 2. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c`:148, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c`:149, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c`:150.
2. **PROVEN:** Given: StopTimer(9), with timer-manager count at most 9. The ID bounds test fails and no timer slot changes. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c`:148, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c`:149, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c`:150.
3. **PROVEN:** Given: StopTimer("2"). The type gate prevents TMR_Stop from being called; timer state is unchanged. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7064.c`:12, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7064.c`:14, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7064.c`:18.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **2** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/ai/_commons.luac:40` | autoloaded_lua_common_helpers | `StopTimer(timerId)` | `[{"index":1,"expression":"timerId","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/resurrectinggod.luac:90` | outside scoped roots | `StopTimer(m_timerID)` | `[{"index":1,"expression":"m_timerID","literal_tokens":[]}]` |

## Related natives

`Character::_StopTimer`, `StopTimer`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
