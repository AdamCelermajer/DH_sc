# StartSkillTimerCB

Contract status: **NOT-NEEDED**. Static IDA/Lua/port source investigation only. Frozen port status: **MISSING**.

## Binding and handler

**NOT A NATIVE:** source-identified Lua callback/table field; no Binder registration is expected.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
**PROVEN Lua signature/call shape:** `StartSkillTimerCB(timeMs,loop,callback,refData)`. This is a Lua-defined helper/table-field contract unless explicitly marked NOT-FOUND.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Lua helper captures current SKILL_ID with callback/refData, calls StartTimerCB(timeMs,loop,SkillTimerCB_,captured). Native StartTimer allocates timer; callback dispatch temporarily installs captured SKILL_ID, invokes callback(timerId,originalRefData), restores priorID after normal return. Nonfunction user callback is skipped but wrapper still registers function SkillTimerCB_. Error unwind restoration not guaranteed by Lua code. Evidence: `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:146`, `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:133`, `.local-inputs/assets-extra/android/data/scripts/ai/_commons.luac:26`.
**Gates:**
- PROVEN: Lua definition/field type admission as described; not an independent engine registration. Evidence: `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:146`, `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:133`, `.local-inputs/assets-extra/android/data/scripts/ai/_commons.luac:26`.
**State, timers and events:**
- PROVEN: Lua table/context mutation and native calls only as described. Evidence: `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:146`, `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:133`, `.local-inputs/assets-extra/android/data/scripts/ai/_commons.luac:26`.
**Return:**
- PROVEN: Lua return as described; no native C signature because this row is a Lua helper/table callback. Evidence: `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:146`, `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:133`, `.local-inputs/assets-extra/android/data/scripts/ai/_commons.luac:26`.

## Lua wrapper/helper traces

- PROVEN: Exact Binder public-name scan and unified original registration strings checked; Lua definition/table field found at supplied source; real engine calls are dependencies below, not a hidden native of this spelling. Evidence: `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:146`, `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:133`, `.local-inputs/assets-extra/android/data/scripts/ai/_commons.luac:26`.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**MISSING:** This Lua helper needs its authored script loaded plus its reusable native dependencies; a same-name C++ identifier or generic native registration is not its implementation. Runtime execution not verified.

**Concrete behavioral difference:** This Lua helper needs its authored script loaded plus its reusable native dependencies; a same-name C++ identifier or generic native registration is not its implementation. Runtime execution not verified.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: current SKILL_ID3; StartSkillTimerCB(100,false,callback,data) Timer wrapper stores skill3; returns native timer ID. Evidence: `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac`:146, `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac`:133, `.local-inputs/assets-extra/android/data/scripts/ai/_commons.luac`:26.
2. **PROVEN:** Given: timer dispatch when currentID9,captured3,callbackfunction Callback seesID3 and original data; normal completion restoresID9. Evidence: `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac`:146, `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac`:133, `.local-inputs/assets-extra/android/data/scripts/ai/_commons.luac`:26.

## Original Lua call sites

Scoped exact original call ledger (supplement excludes outside-scope calls): **33** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:207` | outside scoped roots | `StartSkillTimerCB(delay, false, SkillCooldownExpiredCB_, SKILL_NAME)` | `[{"index":1,"expression":"delay","literal_tokens":[]},{"index":2,"expression":"false","literal_tokens":[]},{"index":3,"expression":"SkillCooldownExpiredCB_","literal_tokens":[]},{"index":4,"expression":"SKILL_NAME","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_exploding_arrows.luac:130` | outside scoped roots | `StartSkillTimerCB(500, 0, DoExplosion, target)` | `[{"index":1,"expression":"500","literal_tokens":["500"]},{"index":2,"expression":"0","literal_tokens":["0"]},{"index":3,"expression":"DoExplosion","literal_tokens":[]},{"index":4,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_pin_shot.luac:120` | outside scoped roots | `StartSkillTimerCB(SWITCH_TARGET_DELAY, false, PinShot, target)` | `[{"index":1,"expression":"SWITCH_TARGET_DELAY","literal_tokens":[]},{"index":2,"expression":"false","literal_tokens":[]},{"index":3,"expression":"PinShot","literal_tokens":[]},{"index":4,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_pin_shot.luac:128` | outside scoped roots | `StartSkillTimerCB((SWITCH_TARGET_DELAY + SWITCH_TARGET_DELAY), false, PinShot, target)` | `[{"index":1,"expression":"(SWITCH_TARGET_DELAY + SWITCH_TARGET_DELAY)","literal_tokens":[]},{"index":2,"expression":"false","literal_tokens":[]},{"index":3,"expression":"PinShot","literal_tokens":[]},{"index":4,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_precision.luac:32` | outside scoped roots | `StartSkillTimerCB(1000, true, checkbuff)` | `[{"index":1,"expression":"1000","literal_tokens":["1000"]},{"index":2,"expression":"true","literal_tokens":[]},{"index":3,"expression":"checkbuff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_precision.luac:67` | outside scoped roots | `StartSkillTimerCB(1000, true, checkbuff)` | `[{"index":1,"expression":"1000","literal_tokens":["1000"]},{"index":2,"expression":"true","literal_tokens":[]},{"index":3,"expression":"checkbuff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_sidestep.luac:126` | outside scoped roots | `StartSkillTimerCB(500, true, check, attacker)` | `[{"index":1,"expression":"500","literal_tokens":["500"]},{"index":2,"expression":"true","literal_tokens":[]},{"index":3,"expression":"check","literal_tokens":[]},{"index":4,"expression":"attacker","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_blindness.luac:99` | outside scoped roots | `StartSkillTimerCB(BUFF_TIME, false, BuffExpired)` | `[{"index":1,"expression":"BUFF_TIME","literal_tokens":[]},{"index":2,"expression":"false","literal_tokens":[]},{"index":3,"expression":"BuffExpired","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_poison_sting.luac:57` | outside scoped roots | `StartSkillTimerCB(BUFF_TIME, 0, BuffExpired)` | `[{"index":1,"expression":"BUFF_TIME","literal_tokens":[]},{"index":2,"expression":"0","literal_tokens":["0"]},{"index":3,"expression":"BuffExpired","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_killing_frenzy.luac:52` | outside scoped roots | `StartSkillTimerCB(BUFF_DELAY, false, resetBuff)` | `[{"index":1,"expression":"BUFF_DELAY","literal_tokens":[]},{"index":2,"expression":"false","literal_tokens":[]},{"index":3,"expression":"resetBuff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_killing_frenzy.luac:66` | outside scoped roots | `StartSkillTimerCB(BUFF_DELAY, false, resetBuff)` | `[{"index":1,"expression":"BUFF_DELAY","literal_tokens":[]},{"index":2,"expression":"false","literal_tokens":[]},{"index":3,"expression":"resetBuff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_killing_frenzy.luac:80` | outside scoped roots | `StartSkillTimerCB(BUFF_MAX_DELAY, false, resetBuff)` | `[{"index":1,"expression":"BUFF_MAX_DELAY","literal_tokens":[]},{"index":2,"expression":"false","literal_tokens":[]},{"index":3,"expression":"resetBuff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_warcry.luac:138` | outside scoped roots | `StartSkillTimerCB(bufftime, false, BuffExpired)` | `[{"index":1,"expression":"bufftime","literal_tokens":[]},{"index":2,"expression":"false","literal_tokens":[]},{"index":3,"expression":"BuffExpired","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_disciples.luac:53` | outside scoped roots | `StartSkillTimerCB(bufftime, false, BuffExpired, counter, defender)` | `[{"index":1,"expression":"bufftime","literal_tokens":[]},{"index":2,"expression":"false","literal_tokens":[]},{"index":3,"expression":"BuffExpired","literal_tokens":[]},{"index":4,"expression":"counter","literal_tokens":[]},{"index":5,"expression":"defender","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_terror.luac:124` | outside scoped roots | `StartSkillTimerCB(BUFF_DURATION, false, delete_slot, 1)` | `[{"index":1,"expression":"BUFF_DURATION","literal_tokens":[]},{"index":2,"expression":"false","literal_tokens":[]},{"index":3,"expression":"delete_slot","literal_tokens":[]},{"index":4,"expression":"1","literal_tokens":["1"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_terror.luac:131` | outside scoped roots | `StartSkillTimerCB(BUFF_DURATION, false, delete_slot, 2)` | `[{"index":1,"expression":"BUFF_DURATION","literal_tokens":[]},{"index":2,"expression":"false","literal_tokens":[]},{"index":3,"expression":"delete_slot","literal_tokens":[]},{"index":4,"expression":"2","literal_tokens":["2"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_terror.luac:138` | outside scoped roots | `StartSkillTimerCB(BUFF_DURATION, false, delete_slot, 3)` | `[{"index":1,"expression":"BUFF_DURATION","literal_tokens":[]},{"index":2,"expression":"false","literal_tokens":[]},{"index":3,"expression":"delete_slot","literal_tokens":[]},{"index":4,"expression":"3","literal_tokens":["3"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_cyclone.luac:140` | outside scoped roots | `StartSkillTimerCB(TICK_DELAY, true, HandleTick)` | `[{"index":1,"expression":"TICK_DELAY","literal_tokens":[]},{"index":2,"expression":"true","literal_tokens":[]},{"index":3,"expression":"HandleTick","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_stone_escape.luac:163` | outside scoped roots | `StartSkillTimerCB(TNT_DELAY, false, DoExplosion, statue)` | `[{"index":1,"expression":"TNT_DELAY","literal_tokens":[]},{"index":2,"expression":"false","literal_tokens":[]},{"index":3,"expression":"DoExplosion","literal_tokens":[]},{"index":4,"expression":"statue","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_fireskullrain.luac:150` | outside scoped roots | `StartSkillTimerCB(TICK_DELAY, true, DamageTick)` | `[{"index":1,"expression":"TICK_DELAY","literal_tokens":[]},{"index":2,"expression":"true","literal_tokens":[]},{"index":3,"expression":"DamageTick","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_pestilence.luac:125` | outside scoped roots | `StartSkillTimerCB(500, true, UpdateAttackedTargets)` | `[{"index":1,"expression":"500","literal_tokens":["500"]},{"index":2,"expression":"true","literal_tokens":[]},{"index":3,"expression":"UpdateAttackedTargets","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_pestilence.luac:191` | outside scoped roots | `StartSkillTimerCB((timeofdot + 1000), false, RemoveTarget, target )` | `[{"index":1,"expression":"(timeofdot + 1000)","literal_tokens":["1000"]},{"index":2,"expression":"false","literal_tokens":[]},{"index":3,"expression":"RemoveTarget","literal_tokens":[]},{"index":4,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_rise.luac:163` | outside scoped roots | `StartSkillTimerCB(bufftime, false, killed, spawnSlot)` | `[{"index":1,"expression":"bufftime","literal_tokens":[]},{"index":2,"expression":"false","literal_tokens":[]},{"index":3,"expression":"killed","literal_tokens":[]},{"index":4,"expression":"spawnSlot","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_rise.luac:299` | outside scoped roots | `StartSkillTimerCB(bufftime, false, killed, count)` | `[{"index":1,"expression":"bufftime","literal_tokens":[]},{"index":2,"expression":"false","literal_tokens":[]},{"index":3,"expression":"killed","literal_tokens":[]},{"index":4,"expression":"count","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_wrath.luac:55` | outside scoped roots | `StartSkillTimerCB(TIMER_DELAY, true, canApplyBuff)` | `[{"index":1,"expression":"TIMER_DELAY","literal_tokens":[]},{"index":2,"expression":"true","literal_tokens":[]},{"index":3,"expression":"canApplyBuff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_conversion.luac:127` | outside scoped roots | `StartSkillTimerCB(2000, 0, BuffExpired, defender)` | `[{"index":1,"expression":"2000","literal_tokens":["2000"]},{"index":2,"expression":"0","literal_tokens":["0"]},{"index":3,"expression":"BuffExpired","literal_tokens":[]},{"index":4,"expression":"defender","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_energy_balance.luac:90` | outside scoped roots | `StartSkillTimerCB(BUFF_DELAY, true, updateSelfBuff)` | `[{"index":1,"expression":"BUFF_DELAY","literal_tokens":[]},{"index":2,"expression":"true","literal_tokens":[]},{"index":3,"expression":"updateSelfBuff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_guarding_aura.luac:144` | outside scoped roots | `StartSkillTimerCB(BUFF_TIME, 0, BuffExpired)` | `[{"index":1,"expression":"BUFF_TIME","literal_tokens":[]},{"index":2,"expression":"0","literal_tokens":["0"]},{"index":3,"expression":"BuffExpired","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_offensive_aura.luac:139` | outside scoped roots | `StartSkillTimerCB(BUFF_TIME, 0, BuffExpired)` | `[{"index":1,"expression":"BUFF_TIME","literal_tokens":[]},{"index":2,"expression":"0","literal_tokens":["0"]},{"index":3,"expression":"BuffExpired","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_shield.luac:138` | outside scoped roots | `StartSkillTimerCB(500, true, check, attacker)` | `[{"index":1,"expression":"500","literal_tokens":["500"]},{"index":2,"expression":"true","literal_tokens":[]},{"index":3,"expression":"check","literal_tokens":[]},{"index":4,"expression":"attacker","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_shield.luac:141` | outside scoped roots | `StartSkillTimerCB(500, true, check2, attacker)` | `[{"index":1,"expression":"500","literal_tokens":["500"]},{"index":2,"expression":"true","literal_tokens":[]},{"index":3,"expression":"check2","literal_tokens":[]},{"index":4,"expression":"attacker","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_quickness.luac:96` | outside scoped roots | `StartSkillTimerCB(bufftime, 0, BuffExpired)` | `[{"index":1,"expression":"bufftime","literal_tokens":[]},{"index":2,"expression":"0","literal_tokens":["0"]},{"index":3,"expression":"BuffExpired","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_leadership.luac:65` | outside scoped roots | `StartSkillTimerCB(BUFF_DELAY, true, startGroupBuff)` | `[{"index":1,"expression":"BUFF_DELAY","literal_tokens":[]},{"index":2,"expression":"true","literal_tokens":[]},{"index":3,"expression":"startGroupBuff","literal_tokens":[]}]` |

## Related natives

No exact related name established.

## Unresolved reasons

- **NOT-NEEDED:** Exact original Lua definition/table field proves this ranked parser candidate is not an independent native; preserved in sealed inventory.

## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
