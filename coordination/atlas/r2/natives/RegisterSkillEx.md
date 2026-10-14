# RegisterSkillEx

Contract status: **NOT-NEEDED**. Static IDA/Lua/port source investigation only. Frozen port status: **MISSING**.

## Binding and handler

**NOT A NATIVE:** source-identified Lua callback/table field; no Binder registration is expected.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
**PROVEN Lua signature/call shape:** `RegisterSkillEx(callbacks)`. This is a Lua-defined helper/table-field contract unless explicitly marked NOT-FOUND.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Lua table registry helper. Requires table,current WORKING_NAME,no duplicate,truthy update/check/pre/skill. Stores ID=WORKING_ID and nine callbacks with default fallbacks; required callbacks need truthiness, not function-type test. No return value. Evidence: `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:52`.
**Gates:**
- PROVEN: Lua definition/field type admission as described; not an independent engine registration. Evidence: `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:52`.
**State, timers and events:**
- PROVEN: Lua table/context mutation and native calls only as described. Evidence: `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:52`.
**Return:**
- PROVEN: Lua return as described; no native C signature because this row is a Lua helper/table callback. Evidence: `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:52`.

## Lua wrapper/helper traces

- PROVEN: Exact Binder public-name scan and unified original registration strings checked; Lua definition/table field found at supplied source; real engine calls are dependencies below, not a hidden native of this spelling. Evidence: `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:52`.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**MISSING:** This Lua helper needs its authored script loaded plus its reusable native dependencies; a same-name C++ identifier or generic native registration is not its implementation. Runtime execution not verified.

**Concrete behavioral difference:** This Lua helper needs its authored script loaded plus its reusable native dependencies; a same-name C++ identifier or generic native registration is not its implementation. Runtime execution not verified.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: callbacks nottable; RegisterSkillEx(5) No registry write. Evidence: `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac`:52.
2. **PROVEN:** Given: valid required callbacks,unique declaredskillID3 Creates entryID3; omitted cleanup/post/delayed/check/info use DEFAULT_NAME entry callbacks. Evidence: `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac`:52.

## Original Lua call sites

Scoped exact original call ledger (supplement excludes outside-scope calls): **10** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:42` | outside scoped roots | `RegisterSkillEx({\tupdate=callback_update,\r\n\t\t\t\t\t\tcheck=callback_check,\r\n\t\t\t\t\t\tpre=callback_pre,\r\n\t\t\t\t\t\tskill=callback,\r\n\t\t\t\t\t\tpost=callback_post,\r\n\t\t\t\t\t\tdelayed=callback_delayed,\r\n\t\t\t\t\t\tinfo=callback_info,\r\n\t\t\t\t\t\t})` | `[{"index":1,"expression":"{\tupdate=callback_update,\r\n\t\t\t\t\t\tcheck=callback_check,\r\n\t\t\t\t\t\tpre=callback_pre,\r\n\t\t\t\t\t\tskill=callback,\r\n\t\t\t\t\t\tpost=callback_post,\r\n\t\t\t\t\t\tdelayed=callback_delayed,\r\n\t\t\t\t\t\tinfo=callback_info,\r\n\t\t\t\t\t\t}","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:118` | outside scoped roots | `RegisterSkillEx({\tupdate\t\t\t= function() Trace("Invalid OnSkillUpdate()");\t\t\t\t\t\t\t\tend,\r\n\t\t\t\t\tcleanup\t\t\t= function()\t\t\t\t\t\tend,\r\n\t\t\t\t\tcheck\t\t\t= function() Trace("Invalid OnSkillCheck()");\t\treturn false, false;\tend,\r\n\t\t\t\t\tpre\t\t\t\t= function() Trace("Invalid OnPreSkill()");\t\t\treturn false;\t\t\tend,\r\n\t\t\t\t\tskill\t\t\t= function() Trace("Invalid OnSkill()");\t\t\treturn false;\t\t\tend,\r\n\t\t\t\t\tpost\t\t\t= function() \t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\tend,\r\n\t\t\t\t\tdelayed\t\t\t= function() \t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\tend,\r\n\t\t\t\t\tdelayed_check\t= function()\t\t\t\t\t\t\t\t\t\t--[[return true]]\t\tend,\r\n\t\t\t\t\tinfo\t\t\t= function() \t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\tend,\r\n\t\t\t\t\t})` | `[{"index":1,"expression":"{\tupdate\t\t\t= function() Trace(\"Invalid OnSkillUpdate()\");\t\t\t\t\t\t\t\tend,\r\n\t\t\t\t\tcleanup\t\t\t= function()\t\t\t\t\t\tend,\r\n\t\t\t\t\tcheck\t\t\t= function() Trace(\"Invalid OnSkillCheck()\");\t\treturn false, false;\tend,\r\n\t\t\t\t\tpre\t\t\t\t= function() Trace(\"Invalid OnPreSkill()\");\t\t\treturn false;\t\t\tend,\r\n\t\t\t\t\tskill\t\t\t= function() Trace(\"Invalid OnSkill()\");\t\t\treturn false;\t\t\tend,\r\n\t\t\t\t\tpost\t\t\t= function() \t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\tend,\r\n\t\t\t\t\tdelayed\t\t\t= function() \t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\tend,\r\n\t\t\t\t\tdelayed_check\t= function()\t\t\t\t\t\t\t\t\t\t--[[return true]]\t\tend,\r\n\t\t\t\t\tinfo\t\t\t= function() \t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\tend,\r\n\t\t\t\t\t}","literal_tokens":["\"Invalid OnSkillUpdate()\"","\"Invalid OnSkillCheck()\"","\"Invalid OnPreSkill()\"","\"Invalid OnSkill()\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_exploding_arrows.luac:139` | outside scoped roots | `RegisterSkillEx({\tupdate=OnSkillUpdate_,\r\n\t\t\t\t\t\tcheck=OnSkillCheck_,\r\n\t\t\t\t\t\tpre=OnPreSkill_,\r\n\t\t\t\t\t\tskill=OnSkill_,\r\n\t\t\t\t\t\tpost=OnPostSkill_,\r\n\t\t\t\t\t\tdelayed=OnDelayedSkill_,\r\n\t\t\t\t\t\tinfo=OnSkillInfo_,\r\n\t\t\t\t\t\t})` | `[{"index":1,"expression":"{\tupdate=OnSkillUpdate_,\r\n\t\t\t\t\t\tcheck=OnSkillCheck_,\r\n\t\t\t\t\t\tpre=OnPreSkill_,\r\n\t\t\t\t\t\tskill=OnSkill_,\r\n\t\t\t\t\t\tpost=OnPostSkill_,\r\n\t\t\t\t\t\tdelayed=OnDelayedSkill_,\r\n\t\t\t\t\t\tinfo=OnSkillInfo_,\r\n\t\t\t\t\t\t}","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_grapling_hook.luac:112` | outside scoped roots | `RegisterSkillEx({\tupdate=OnSkillUpdate_,\r\n\t\t\t\t\t\tcheck=OnSkillCheck_,\r\n\t\t\t\t\t\tpre=OnPreSkill_,\r\n\t\t\t\t\t\tskill=OnSkill_,\r\n\t\t\t\t\t\tpost=OnPostSkill_,\r\n\t\t\t\t\t\tdelayed=OnDelayedSkill_,\r\n\t\t\t\t\t\tinfo=OnSkillInfo_,\r\n\t\t\t\t\t\t})` | `[{"index":1,"expression":"{\tupdate=OnSkillUpdate_,\r\n\t\t\t\t\t\tcheck=OnSkillCheck_,\r\n\t\t\t\t\t\tpre=OnPreSkill_,\r\n\t\t\t\t\t\tskill=OnSkill_,\r\n\t\t\t\t\t\tpost=OnPostSkill_,\r\n\t\t\t\t\t\tdelayed=OnDelayedSkill_,\r\n\t\t\t\t\t\tinfo=OnSkillInfo_,\r\n\t\t\t\t\t\t}","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_chain_lightning.luac:259` | outside scoped roots | `RegisterSkillEx({\tupdate=OnSkillUpdate_,\r\n\t\t\t\t\tcheck=OnSkillCheck_,\r\n\t\t\t\t\tpre=OnPreSkill_,\r\n\t\t\t\t\tskill=OnSkill_,\r\n\t\t\t\t\tpost=OnPostSkill_,\r\n\t\t\t\t\tdelayed=OnDelayedSkill_,\r\n\t\t\t\t\tdelayed_check=OnDelayedSkillCheck_,\r\n\t\t\t\t\tinfo=OnSkillInfo_,\r\n\t\t\t\t\t})` | `[{"index":1,"expression":"{\tupdate=OnSkillUpdate_,\r\n\t\t\t\t\tcheck=OnSkillCheck_,\r\n\t\t\t\t\tpre=OnPreSkill_,\r\n\t\t\t\t\tskill=OnSkill_,\r\n\t\t\t\t\tpost=OnPostSkill_,\r\n\t\t\t\t\tdelayed=OnDelayedSkill_,\r\n\t\t\t\t\tdelayed_check=OnDelayedSkillCheck_,\r\n\t\t\t\t\tinfo=OnSkillInfo_,\r\n\t\t\t\t\t}","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_cyclone.luac:261` | outside scoped roots | `RegisterSkillEx({\tupdate=OnSkillUpdate_,\r\n\t\t\t\t\t\tcheck=OnSkillCheck_,\r\n\t\t\t\t\t\tpre=OnPreSkill_,\r\n\t\t\t\t\t\tskill=OnSkill_,\r\n\t\t\t\t\t\tpost=OnPostSkill_,\r\n\t\t\t\t\t\tdelayed=OnDelayedSkill_,\r\n\t\t\t\t\t\tinfo=OnSkillInfo_,\r\n\t\t\t\t\t\tdelayed_check=OnDelayedSkillCheck_,\r\n\t\t\t\t\t\t})` | `[{"index":1,"expression":"{\tupdate=OnSkillUpdate_,\r\n\t\t\t\t\t\tcheck=OnSkillCheck_,\r\n\t\t\t\t\t\tpre=OnPreSkill_,\r\n\t\t\t\t\t\tskill=OnSkill_,\r\n\t\t\t\t\t\tpost=OnPostSkill_,\r\n\t\t\t\t\t\tdelayed=OnDelayedSkill_,\r\n\t\t\t\t\t\tinfo=OnSkillInfo_,\r\n\t\t\t\t\t\tdelayed_check=OnDelayedSkillCheck_,\r\n\t\t\t\t\t\t}","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_stone_escape.luac:186` | outside scoped roots | `RegisterSkillEx({\tupdate=OnSkillUpdate_,\n\t\t\t\t\tcheck=OnSkillCheck_,\n\t\t\t\t\tpre=OnPreSkill_,\n\t\t\t\t\tskill=OnSkill_,\n\t\t\t\t\tdelayed=OnDelayedSkill_,\n\t\t\t\t\tdelayed_check=OnDelayedSkillCheck_,\n\t\t\t\t\tinfo=OnSkillInfo_,\n\t\t\t\t\t})` | `[{"index":1,"expression":"{\tupdate=OnSkillUpdate_,\n\t\t\t\t\tcheck=OnSkillCheck_,\n\t\t\t\t\tpre=OnPreSkill_,\n\t\t\t\t\tskill=OnSkill_,\n\t\t\t\t\tdelayed=OnDelayedSkill_,\n\t\t\t\t\tdelayed_check=OnDelayedSkillCheck_,\n\t\t\t\t\tinfo=OnSkillInfo_,\n\t\t\t\t\t}","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_rise.luac:352` | outside scoped roots | `RegisterSkillEx({\tupdate=OnSkillUpdate_,\r\n\t\t\t\t\tcleanup=OnSkillCleanUp_,\r\n\t\t\t\t\tcheck=OnSkillCheck_,\r\n\t\t\t\t\tpre=OnPreSkill_,\r\n\t\t\t\t\tskill=OnSkill_,\r\n\t\t\t\t\tpost=OnPostSkill_,\r\n\t\t\t\t\tdelayed=OnDelayedSkill_,\r\n\t\t\t\t\tdelayed_check=OnDelayedSkillCheck_,\r\n\t\t\t\t\tinfo=OnSkillInfo_,\r\n\t\t\t\t\t})` | `[{"index":1,"expression":"{\tupdate=OnSkillUpdate_,\r\n\t\t\t\t\tcleanup=OnSkillCleanUp_,\r\n\t\t\t\t\tcheck=OnSkillCheck_,\r\n\t\t\t\t\tpre=OnPreSkill_,\r\n\t\t\t\t\tskill=OnSkill_,\r\n\t\t\t\t\tpost=OnPostSkill_,\r\n\t\t\t\t\tdelayed=OnDelayedSkill_,\r\n\t\t\t\t\tdelayed_check=OnDelayedSkillCheck_,\r\n\t\t\t\t\tinfo=OnSkillInfo_,\r\n\t\t\t\t\t}","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/swampking_atk_spit.luac:115` | outside scoped roots | `RegisterSkillEx({\tupdate=OnSkillUpdate_,\r\n\t\t\t\t\tcheck=OnSkillCheck_,\r\n\t\t\t\t\tpre=OnPreSkill_,\r\n\t\t\t\t\tskill=OnSkill_,\r\n\t\t\t\t\tdelayed=OnDelayedSkill_,\r\n\t\t\t\t\tinfo=OnSkillInfo_,\r\n\t\t\t\t\t})` | `[{"index":1,"expression":"{\tupdate=OnSkillUpdate_,\r\n\t\t\t\t\tcheck=OnSkillCheck_,\r\n\t\t\t\t\tpre=OnPreSkill_,\r\n\t\t\t\t\tskill=OnSkill_,\r\n\t\t\t\t\tdelayed=OnDelayedSkill_,\r\n\t\t\t\t\tinfo=OnSkillInfo_,\r\n\t\t\t\t\t}","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/witch_ranged_00.luac:77` | outside scoped roots | `RegisterSkillEx({\tupdate=OnSkillUpdate_,\r\n\t\t\t\t\tcheck=OnSkillCheck_,\r\n\t\t\t\t\tpre=OnPreSkill_,\r\n\t\t\t\t\tskill=OnSkill_,\r\n\t\t\t\t\tpost=OnPostSkill_,\r\n\t\t\t\t\tdelayed=OnDelayedSkill_,\r\n\t\t\t\t\tdelayed_check=OnDelayedSkillCheck_,\r\n\t\t\t\t\tinfo=OnSkillInfo_,\r\n\t\t\t\t\t})` | `[{"index":1,"expression":"{\tupdate=OnSkillUpdate_,\r\n\t\t\t\t\tcheck=OnSkillCheck_,\r\n\t\t\t\t\tpre=OnPreSkill_,\r\n\t\t\t\t\tskill=OnSkill_,\r\n\t\t\t\t\tpost=OnPostSkill_,\r\n\t\t\t\t\tdelayed=OnDelayedSkill_,\r\n\t\t\t\t\tdelayed_check=OnDelayedSkillCheck_,\r\n\t\t\t\t\tinfo=OnSkillInfo_,\r\n\t\t\t\t\t}","literal_tokens":[]}]` |

## Related natives

No exact related name established.

## Unresolved reasons

- **NOT-NEEDED:** Exact original Lua definition/table field proves this ranked parser candidate is not an independent native; preserved in sealed inventory.

## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
