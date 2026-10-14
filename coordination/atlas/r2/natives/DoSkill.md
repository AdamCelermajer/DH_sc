# DoSkill

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **PARTIAL**.

Engine functions to implement next ranking: **#16**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `DoSkill` | bindFunction | `Character::_DoSkill` | `_ZN9Character14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:132361 |
| `DoSkill` | bindMethod | `Character::_DoSkill` | `_ZN9Character14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:132366 |

Address-identified callback: `Character::_DoSkill` at `0x003b8bd8`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c`, unified lines 134605-134644.
Recovered declaration: `int __fastcall Character::_DoSkill( int this, const sfc::script::lua::Arguments *a2, sfc::script::lua::ReturnValues *a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
- Lua argument 1: number tag3 -> unsigned index; unit/scale receiver skill-list index. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c:19`.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Requires numeric tag3 skill index, unsigned < GetCharSkillList count. AI_UseSkill calls AI_BeginSkill; if begin success immediately calls AI_EndSkill. Begin rechecks active toggle-type1 (calls OnPreSkill directly), otherwise Usable: while using skill requires flag0x8000; casting or missing loaded script process rejects; script index must exist/nonnull and OnSkillCheck_Usable pass. Admitted begin stores active index, clears loop/end bytes, selects Skill FSM, counts player skill stat216 and trophy>199, then checks using-skill result. End only affects active type2 loop: stop loop if byte208 true, otherwise marks byte209. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c:17`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c:19`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c:22`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c:23`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c:24`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c:28`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c:33`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c:34`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c:38`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d8868.c:5`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d86bc.c:14`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d8358.c:7`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d8474.c:7`.
**Gates:**
- PROVEN: See explicit conditional branches in behavior; valid Binder receiver is assumed, with no wrapper-level actor state/cooldown gate unless stated. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c:17`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c:19`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c:22`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c:23`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c:24`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c:28`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c:33`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c:34`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c:38`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d8868.c:5`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d86bc.c:14`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d8358.c:7`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d8474.c:7`.
**State, timers and events:**
- PROVEN: No Lua value; admitted skill state/script/stat effects as described; script may start timers downstream. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c:17`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c:19`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c:22`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c:23`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c:24`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c:28`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c:33`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c:34`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c:38`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d8868.c:5`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d86bc.c:14`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d8358.c:7`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d8474.c:7`.
**Return:**
- PROVEN: No Lua value; admitted skill state/script/stat effects as described; script may start timers downstream. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c:17`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c:19`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c:22`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c:23`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c:24`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c:28`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c:33`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c:34`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c:38`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d8868.c:5`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d86bc.c:14`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d8358.c:7`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d8474.c:7`.

## Lua wrapper/helper traces

No helper-to-native trace was proven. Missing trace does not prove direct-only use.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**PARTIAL:** Enemy and selected player skill owners trigger custom operation services; CharacterSkillNativeBindingsV5 list (:12) omits0x3b8bd8 and general CharacterScriptObjects dispatch (:144) lacks it. Original OnSkillCheck and FSM admission cannot be assumed from gameplay request alone.
- Source evidence: `port/windows-foundation/features/bosses/runtime_boss_encounter_plan_v1.cpp`:19 — `{"intimidate", "idle", "swampking_OnEndOfAnim", "intimidateAnim_flag == 1; otherwise DoSkill(2)", 0},`.
- Source evidence: `port/windows-foundation/features/bosses/runtime_swampking_core_ai_v1.cpp`:104 — `const std::regex call("DoSkill\\(\\s*([0-9]+)\\s*\\)");`.
- Source evidence: `port/windows-foundation/features/bosses/runtime_swampking_core_ai_v1.cpp`:232 — `if (!std::regex_search(end, m, std::regex("StartTimerCB\\(\\s*([0-9]+)\\s*,\\s*false\\s*,\\s*function\\(\\)\\s*DoSkill\\(\\s*([0-9]+)\\s*\\)")))`.
- Source evidence: `port/windows-foundation/features/bosses/runtime_swampking_core_ai_v1.cpp`:233 — `return fail(error, "OnEndOfAnim intimidate DoSkill timer absent");`.
- Source evidence: `port/windows-foundation/features/bosses/runtime_swampking_core_ai_v1.cpp`:272 — `return fail(error, "DoSkill branch layout differs from the dive-boss shape");`.
- Source evidence: `port/windows-foundation/features/bosses/runtime_swampking_core_ai_v1.cpp`:389 — `if (highest < 0 || static_cast<std::size_t>(highest) >= list.size()) { reason = "DoSkill index outside the SkillTree list"; return true; }`.
- Source evidence: `port/windows-foundation/features/bosses/runtime_swampking_core_ai_v1.cpp`:871 — `log("DoSkill(" + std::to_string(slot) + ") rejected actor=" + std::to_string(id) + " state=" + std::to_string(state));`.
- Source evidence: `port/windows-foundation/features/bosses/runtime_swampking_core_ai_v1.cpp`:881 — `log("DoSkill(" + std::to_string(slot) + ") rejected: Anim root " + std::to_string(skill.animation_sequence_id) + " not in the actor bank");`.
- Source evidence: `port/windows-foundation/features/bosses/runtime_swampking_core_ai_v1.cpp`:961 — `log("DoSkill(" + std::to_string(slot) + ") rejected actor=" + std::to_string(id) + ": " + problem);`.
- Source evidence: `port/windows-foundation/features/bosses/runtime_swampking_core_ai_v1.cpp`:965 — `log("DoSkill(" + std::to_string(slot) + ") actor=" + std::to_string(id) + " skill=" + skill.skill_name + " sequence=" +`.
- Source evidence: `port/windows-foundation/features/enemy_ai/external_monster_natives.cpp`:11 — `if(!b||!b->character||!b->receiver_lease)return fail("Required retained Character DoSkill receiver");`.
- Source evidence: `port/windows-foundation/features/enemy_ai/external_monster_scoped.cpp`:14 — `return dh2_script_vm_bind_source_scoped_values(vm,"DoSkill",scoped,b);`.

**Concrete behavioral difference:** Enemy and selected player skill owners trigger custom operation services; CharacterSkillNativeBindingsV5 list (:12) omits0x3b8bd8 and general CharacterScriptObjects dispatch (:144) lacks it. Original OnSkillCheck and FSM admission cannot be assumed from gameplay request alone.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: skill count3; DoSkill(3) No AI begin/end attempt. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c`:17, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c`:19, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c`:22, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c`:23, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c`:24, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c`:28, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c`:33, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c`:34, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c`:38, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d8868.c`:5, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d86bc.c`:14, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d8358.c`:7, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d8474.c`:7.
2. **PROVEN:** Given: valid skill, receiver casting; DoSkill(0) Usable fails, no skill state/end transition. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c`:17, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c`:19, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c`:22, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c`:23, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c`:24, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c`:28, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c`:33, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c`:34, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c`:38, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d8868.c`:5, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d86bc.c`:14, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d8358.c`:7, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d8474.c`:7.
3. **PROVEN:** Given: valid type2 currently using, loop byte208false; source Begin succeeds through DoSkill UseSkill invokes End and sets end-pending byte209true. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c`:17, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c`:19, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c`:22, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c`:23, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c`:24, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c`:28, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c`:33, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c`:34, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8bd8.c`:38, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d8868.c`:5, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d86bc.c`:14, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d8358.c`:7, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d8474.c`:7.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **116** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:56` | outside scoped roots | `DoSkill(2)` | `[{"index":1,"expression":"2","literal_tokens":["2"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:58` | outside scoped roots | `DoSkill(1)` | `[{"index":1,"expression":"1","literal_tokens":["1"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:63` | outside scoped roots | `DoSkill(1)` | `[{"index":1,"expression":"1","literal_tokens":["1"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:67` | outside scoped roots | `DoSkill(1)` | `[{"index":1,"expression":"1","literal_tokens":["1"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:70` | outside scoped roots | `DoSkill(4)` | `[{"index":1,"expression":"4","literal_tokens":["4"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:72` | outside scoped roots | `DoSkill(3)` | `[{"index":1,"expression":"3","literal_tokens":["3"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:89` | outside scoped roots | `DoSkill(2)` | `[{"index":1,"expression":"2","literal_tokens":["2"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:91` | outside scoped roots | `DoSkill(0)` | `[{"index":1,"expression":"0","literal_tokens":["0"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:96` | outside scoped roots | `DoSkill(0)` | `[{"index":1,"expression":"0","literal_tokens":["0"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:100` | outside scoped roots | `DoSkill(0)` | `[{"index":1,"expression":"0","literal_tokens":["0"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:103` | outside scoped roots | `DoSkill(2)` | `[{"index":1,"expression":"2","literal_tokens":["2"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:105` | outside scoped roots | `DoSkill(3)` | `[{"index":1,"expression":"3","literal_tokens":["3"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/dummy.luac:19` | outside scoped roots | `DoSkill(0)` | `[{"index":1,"expression":"0","literal_tokens":["0"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:83` | outside scoped roots | `DoSkill(1)` | `[{"index":1,"expression":"1","literal_tokens":["1"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:85` | outside scoped roots | `DoSkill(3)` | `[{"index":1,"expression":"3","literal_tokens":["3"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:103` | outside scoped roots | `DoSkill(0)` | `[{"index":1,"expression":"0","literal_tokens":["0"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:105` | outside scoped roots | `DoSkill(3)` | `[{"index":1,"expression":"3","literal_tokens":["3"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:115` | outside scoped roots | `DoSkill(4)` | `[{"index":1,"expression":"4","literal_tokens":["4"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:120` | outside scoped roots | `DoSkill(2)` | `[{"index":1,"expression":"2","literal_tokens":["2"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/energyspawner.luac:71` | outside scoped roots | `DoSkill(0)` | `[{"index":1,"expression":"0","literal_tokens":["0"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:51` | outside scoped roots | `DoSkill(2)` | `[{"index":1,"expression":"2","literal_tokens":["2"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:70` | outside scoped roots | `DoSkill(0)` | `[{"index":1,"expression":"0","literal_tokens":["0"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:75` | outside scoped roots | `DoSkill(1)` | `[{"index":1,"expression":"1","literal_tokens":["1"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:94` | outside scoped roots | `DoSkill(0)` | `[{"index":1,"expression":"0","literal_tokens":["0"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:99` | outside scoped roots | `DoSkill(0)` | `[{"index":1,"expression":"0","literal_tokens":["0"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:142` | outside scoped roots | `DoSkill(2)` | `[{"index":1,"expression":"2","literal_tokens":["2"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:145` | outside scoped roots | `DoSkill(0)` | `[{"index":1,"expression":"0","literal_tokens":["0"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:151` | outside scoped roots | `DoSkill(1)` | `[{"index":1,"expression":"1","literal_tokens":["1"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:156` | outside scoped roots | `DoSkill(1)` | `[{"index":1,"expression":"1","literal_tokens":["1"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:160` | outside scoped roots | `DoSkill(4)` | `[{"index":1,"expression":"4","literal_tokens":["4"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:163` | outside scoped roots | `DoSkill(3)` | `[{"index":1,"expression":"3","literal_tokens":["3"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:181` | outside scoped roots | `DoSkill(2)` | `[{"index":1,"expression":"2","literal_tokens":["2"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:183` | outside scoped roots | `DoSkill(0)` | `[{"index":1,"expression":"0","literal_tokens":["0"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:188` | outside scoped roots | `DoSkill(0)` | `[{"index":1,"expression":"0","literal_tokens":["0"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:192` | outside scoped roots | `DoSkill(0)` | `[{"index":1,"expression":"0","literal_tokens":["0"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:195` | outside scoped roots | `DoSkill(2)` | `[{"index":1,"expression":"2","literal_tokens":["2"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:197` | outside scoped roots | `DoSkill(3)` | `[{"index":1,"expression":"3","literal_tokens":["3"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/largeplant_core.luac:103` | outside scoped roots | `DoSkill(0)` | `[{"index":1,"expression":"0","literal_tokens":["0"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/largeplant_core.luac:124` | outside scoped roots | `DoSkill(0)` | `[{"index":1,"expression":"0","literal_tokens":["0"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/largeplant_core.luac:150` | outside scoped roots | `DoSkill(0)` | `[{"index":1,"expression":"0","literal_tokens":["0"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/madruk_buffer.luac:45` | outside scoped roots | `DoSkill(0)` | `[{"index":1,"expression":"0","literal_tokens":["0"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:180` | outside scoped roots | `DoSkill(2)` | `[{"index":1,"expression":"2","literal_tokens":["2"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:182` | outside scoped roots | `DoSkill(3)` | `[{"index":1,"expression":"3","literal_tokens":["3"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:191` | outside scoped roots | `DoSkill(0)` | `[{"index":1,"expression":"0","literal_tokens":["0"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:193` | outside scoped roots | `DoSkill(1)` | `[{"index":1,"expression":"1","literal_tokens":["1"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:364` | outside scoped roots | `DoSkill(0)` | `[{"index":1,"expression":"0","literal_tokens":["0"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/madruk_healer.luac:52` | outside scoped roots | `DoSkill(0)` | `[{"index":1,"expression":"0","literal_tokens":["0"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:179` | outside scoped roots | `DoSkill(1)` | `[{"index":1,"expression":"1","literal_tokens":["1"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:190` | outside scoped roots | `DoSkill(2)` | `[{"index":1,"expression":"2","literal_tokens":["2"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:198` | outside scoped roots | `DoSkill(0)` | `[{"index":1,"expression":"0","literal_tokens":["0"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/miniboss_state.luac:67` | outside scoped roots | `DoSkill(0)` | `[{"index":1,"expression":"0","literal_tokens":["0"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/monster.luac:130` | act1_level_boss_script_closure | `DoSkill(0)` | `[{"index":1,"expression":"0","literal_tokens":["0"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/monster.luac:162` | act1_level_boss_script_closure | `DoSkill(0)` | `[{"index":1,"expression":"0","literal_tokens":["0"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/monster.luac:187` | act1_level_boss_script_closure | `DoSkill(0)` | `[{"index":1,"expression":"0","literal_tokens":["0"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/resurrectinggod.luac:58` | outside scoped roots | `DoSkill(0)` | `[{"index":1,"expression":"0","literal_tokens":["0"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:52` | outside scoped roots | `DoSkill(2)` | `[{"index":1,"expression":"2","literal_tokens":["2"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:57` | outside scoped roots | `DoSkill(3)` | `[{"index":1,"expression":"3","literal_tokens":["3"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:100` | outside scoped roots | `DoSkill(0)` | `[{"index":1,"expression":"0","literal_tokens":["0"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:105` | outside scoped roots | `DoSkill(0)` | `[{"index":1,"expression":"0","literal_tokens":["0"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:109` | outside scoped roots | `DoSkill(0)` | `[{"index":1,"expression":"0","literal_tokens":["0"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:127` | outside scoped roots | `DoSkill(2)` | `[{"index":1,"expression":"2","literal_tokens":["2"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:137` | outside scoped roots | `DoSkill(1)` | `[{"index":1,"expression":"1","literal_tokens":["1"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:143` | outside scoped roots | `DoSkill(1)` | `[{"index":1,"expression":"1","literal_tokens":["1"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:146` | outside scoped roots | `DoSkill(0)` | `[{"index":1,"expression":"0","literal_tokens":["0"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:160` | outside scoped roots | `DoSkill(0)` | `[{"index":1,"expression":"0","literal_tokens":["0"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:162` | outside scoped roots | `DoSkill(1)` | `[{"index":1,"expression":"1","literal_tokens":["1"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:168` | outside scoped roots | `DoSkill(0)` | `[{"index":1,"expression":"0","literal_tokens":["0"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:170` | outside scoped roots | `DoSkill(1)` | `[{"index":1,"expression":"1","literal_tokens":["1"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:241` | outside scoped roots | `DoSkill(3)` | `[{"index":1,"expression":"3","literal_tokens":["3"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:379` | outside scoped roots | `DoSkill(1)` | `[{"index":1,"expression":"1","literal_tokens":["1"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:477` | outside scoped roots | `DoSkill(2)` | `[{"index":1,"expression":"2","literal_tokens":["2"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:161` | outside scoped roots | `DoSkill(1)` | `[{"index":1,"expression":"1","literal_tokens":["1"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:167` | outside scoped roots | `DoSkill(1)` | `[{"index":1,"expression":"1","literal_tokens":["1"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:170` | outside scoped roots | `DoSkill(0)` | `[{"index":1,"expression":"0","literal_tokens":["0"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:185` | outside scoped roots | `DoSkill(0)` | `[{"index":1,"expression":"0","literal_tokens":["0"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:187` | outside scoped roots | `DoSkill(1)` | `[{"index":1,"expression":"1","literal_tokens":["1"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:193` | outside scoped roots | `DoSkill(0)` | `[{"index":1,"expression":"0","literal_tokens":["0"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:195` | outside scoped roots | `DoSkill(1)` | `[{"index":1,"expression":"1","literal_tokens":["1"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:252` | outside scoped roots | `DoSkill(3)` | `[{"index":1,"expression":"3","literal_tokens":["3"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:394` | outside scoped roots | `DoSkill(1)` | `[{"index":1,"expression":"1","literal_tokens":["1"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:469` | outside scoped roots | `DoSkill(2)` | `[{"index":1,"expression":"2","literal_tokens":["2"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:71` | outside scoped roots | `DoSkill(1)` | `[{"index":1,"expression":"1","literal_tokens":["1"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:77` | outside scoped roots | `DoSkill(1)` | `[{"index":1,"expression":"1","literal_tokens":["1"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:80` | outside scoped roots | `DoSkill(0)` | `[{"index":1,"expression":"0","literal_tokens":["0"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:95` | outside scoped roots | `DoSkill(0)` | `[{"index":1,"expression":"0","literal_tokens":["0"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:97` | outside scoped roots | `DoSkill(1)` | `[{"index":1,"expression":"1","literal_tokens":["1"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:103` | outside scoped roots | `DoSkill(0)` | `[{"index":1,"expression":"0","literal_tokens":["0"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:105` | outside scoped roots | `DoSkill(1)` | `[{"index":1,"expression":"1","literal_tokens":["1"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:162` | outside scoped roots | `DoSkill(3)` | `[{"index":1,"expression":"3","literal_tokens":["3"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:316` | outside scoped roots | `DoSkill(1)` | `[{"index":1,"expression":"1","literal_tokens":["1"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:432` | outside scoped roots | `DoSkill(2)` | `[{"index":1,"expression":"2","literal_tokens":["2"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:260` | act1_level_boss_script_closure | `DoSkill(2)` | `[{"index":1,"expression":"2","literal_tokens":["2"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:270` | act1_level_boss_script_closure | `DoSkill(1)` | `[{"index":1,"expression":"1","literal_tokens":["1"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:276` | act1_level_boss_script_closure | `DoSkill(1)` | `[{"index":1,"expression":"1","literal_tokens":["1"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:279` | act1_level_boss_script_closure | `DoSkill(0)` | `[{"index":1,"expression":"0","literal_tokens":["0"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:293` | act1_level_boss_script_closure | `DoSkill(0)` | `[{"index":1,"expression":"0","literal_tokens":["0"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:295` | act1_level_boss_script_closure | `DoSkill(1)` | `[{"index":1,"expression":"1","literal_tokens":["1"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:301` | act1_level_boss_script_closure | `DoSkill(0)` | `[{"index":1,"expression":"0","literal_tokens":["0"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:303` | act1_level_boss_script_closure | `DoSkill(1)` | `[{"index":1,"expression":"1","literal_tokens":["1"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:430` | act1_level_boss_script_closure | `DoSkill(2)` | `[{"index":1,"expression":"2","literal_tokens":["2"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:470` | act1_level_boss_script_closure | `DoSkill(3)` | `[{"index":1,"expression":"3","literal_tokens":["3"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:618` | act1_level_boss_script_closure | `DoSkill(1)` | `[{"index":1,"expression":"1","literal_tokens":["1"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:761` | act1_level_boss_script_closure | `DoSkill(2)` | `[{"index":1,"expression":"2","literal_tokens":["2"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:142` | outside scoped roots | `DoSkill(2)` | `[{"index":1,"expression":"2","literal_tokens":["2"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:145` | outside scoped roots | `DoSkill(0)` | `[{"index":1,"expression":"0","literal_tokens":["0"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:151` | outside scoped roots | `DoSkill(1)` | `[{"index":1,"expression":"1","literal_tokens":["1"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:156` | outside scoped roots | `DoSkill(1)` | `[{"index":1,"expression":"1","literal_tokens":["1"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:160` | outside scoped roots | `DoSkill(4)` | `[{"index":1,"expression":"4","literal_tokens":["4"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:163` | outside scoped roots | `DoSkill(3)` | `[{"index":1,"expression":"3","literal_tokens":["3"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:181` | outside scoped roots | `DoSkill(2)` | `[{"index":1,"expression":"2","literal_tokens":["2"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:183` | outside scoped roots | `DoSkill(0)` | `[{"index":1,"expression":"0","literal_tokens":["0"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:188` | outside scoped roots | `DoSkill(0)` | `[{"index":1,"expression":"0","literal_tokens":["0"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:192` | outside scoped roots | `DoSkill(0)` | `[{"index":1,"expression":"0","literal_tokens":["0"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:195` | outside scoped roots | `DoSkill(2)` | `[{"index":1,"expression":"2","literal_tokens":["2"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:197` | outside scoped roots | `DoSkill(3)` | `[{"index":1,"expression":"3","literal_tokens":["3"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:192` | act1_level_boss_script_closure | `DoSkill(0)` | `[{"index":1,"expression":"0","literal_tokens":["0"]}]` |

## Related natives

`Character::_DoSkill`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
