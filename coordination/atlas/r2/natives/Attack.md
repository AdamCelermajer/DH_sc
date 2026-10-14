# Attack

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **PARTIAL**.

Engine functions to implement next ranking: **#36**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `Attack` | bindFunction | `Character::_Attack` | `_ZN9Character14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:132352 |
| `Attack` | bindMethod | `Character::_Attack` | `_ZN9Character14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:132357 |

Address-identified callback: `Character::_Attack` at `0x003b9f44`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9f44.c`, unified lines 135518-135551.
Recovered declaration: `sfc::script::lua::Value *__fastcall Character::_Attack( Character *this, const sfc::script::lua::Arguments *a2, sfc::script::lua::ReturnValues *a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
- Lua argument 1: optional pointer tag2/userdata tag7; absent current target; unit/scale attack target identity. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9f44.c:22`.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Arg pointer/userdata targets supplied object; no args uses receiver current target only when nonnull; other types no-op. Cmd_Attack requires controller override or global unblocked/local unlocked. Offline or non-network-controlled path dispatches actor virtual+56. Online controlled path probes AI_DoMeleeAttack temporarily, restores target/last-target/ranged flag, conditionally sends CMsgControllerAction, then dispatches virtual. Wrapper pushes no combat result. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9f44.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9f44.c:16`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9f44.c:19`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9f44.c:22`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9f44.c:23`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9f44.c:29`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9f44.c:30`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9f44.c:32`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0040/00405b04.c:21`.
**Gates:**
- PROVEN: See explicit conditional branches in behavior; valid Binder receiver is assumed, with no wrapper-level actor state/cooldown gate unless stated. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9f44.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9f44.c:16`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9f44.c:19`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9f44.c:22`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9f44.c:23`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9f44.c:29`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9f44.c:30`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9f44.c:32`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0040/00405b04.c:21`.
**State, timers and events:**
- PROVEN: No Lua value; conditional actor attack/network message/temporary target probe, downstream combat owns timers. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9f44.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9f44.c:16`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9f44.c:19`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9f44.c:22`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9f44.c:23`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9f44.c:29`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9f44.c:30`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9f44.c:32`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0040/00405b04.c:21`.
**Return:**
- PROVEN: No Lua value; conditional actor attack/network message/temporary target probe, downstream combat owns timers. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9f44.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9f44.c:16`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9f44.c:19`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9f44.c:22`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9f44.c:23`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9f44.c:29`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9f44.c:30`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9f44.c:32`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0040/00405b04.c:21`.

## Lua wrapper/helper traces

No helper-to-native trace was proven. Missing trace does not prove direct-only use.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**PARTIAL:** Selected monster/player combat requests use source services; general0x3b9f44 callback absent in CharacterScriptObjects (:144-146). Controller gates and online probe restoration/message branch not proven by a normal hit event.
- Source evidence: `port/windows-foundation/actor_combat_runtime.cpp`:162 — `error = "Attack damage binding has no authored clip marker: " + marker.marker_name; return false;`.
- Source evidence: `port/windows-foundation/actor_movement.cpp`:127 — `state_.action=ActorAction::Attack;state_.animationName=config_.attackAnimation;return false;`.
- Source evidence: `port/windows-foundation/actor_movement.cpp`:173 — `state_.action=ActorAction::Attack;state_.animationName=config_.attackAnimation;motionScale_=1;`.
- Source evidence: `port/windows-foundation/actor_movement.hpp`:22 — `enum class ActorAction { Idle, Walk, Run, Attack };`.
- Source evidence: `port/windows-foundation/combat_session.cpp`:253 — `if(selection->state=="Attack")event.source_attack_moving=true;`.
- Source evidence: `port/windows-foundation/combat_session.cpp`:885 — `selection.state=live_original_state(id)==4?"Attack":"AttackStatic";`.
- Source evidence: `port/windows-foundation/combat_session.cpp`:928 — `if(transitionDelivering||restoreTeardown||checkingAnimationCheckpoint){error="Attack command cannot reenter actor transition/checkpoint/teardown";return false;}`.
- Source evidence: `port/windows-foundation/combat_session.cpp`:980 — `if(found==entries.end()){error="Attack command requires a bound live session actor";return false;}`.
- Source evidence: `port/windows-foundation/combat_session.cpp`:1331 — `if((sequence.state!="Attack"&&sequence.state!="AttackStatic")||`.
- Source evidence: `port/windows-foundation/combat_session.cpp`:1333 — `throw std::runtime_error("Source attack bank may replace only explicit Attack/AttackStatic roots");`.
- Source evidence: `port/windows-foundation/combat_session.cpp`:1367 — `!policy.sourceAttackBank->sequence("Attack",0)||!policy.sourceAttackBank->sequence("AttackStatic",0)))`.
- Source evidence: `port/windows-foundation/combat_session.cpp`:1795 — `error="Attack command requires a bound live session actor";return false;`.

**Concrete behavioral difference:** Selected monster/player combat requests use source services; general0x3b9f44 callback absent in CharacterScriptObjects (:144-146). Controller gates and online probe restoration/message branch not proven by a normal hit event.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: Attack() with no current target No controller attack. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9f44.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9f44.c`:16, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9f44.c`:19, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9f44.c`:22, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9f44.c`:23, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9f44.c`:29, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9f44.c`:30, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9f44.c`:32, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0040/00405b04.c`:21.
2. **PROVEN:** Given: Attack(T), controller locked/override0 No virtual attack or network message. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9f44.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9f44.c`:16, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9f44.c`:19, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9f44.c`:22, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9f44.c`:23, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9f44.c`:29, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9f44.c`:30, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9f44.c`:32, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0040/00405b04.c`:21.
3. **PROVEN:** Given: Attack(T), offline unlocked Dispatches actor virtual attack(T); no Lua result. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9f44.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9f44.c`:16, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9f44.c`:19, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9f44.c`:22, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9f44.c`:23, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9f44.c`:29, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9f44.c`:30, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9f44.c`:32, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0040/00405b04.c`:21.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **30** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/ai/attacking_follower.luac:133` | outside scoped roots | `Attack(g_target)` | `[{"index":1,"expression":"g_target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/attacking_follower.luac:160` | outside scoped roots | `Attack(g_target)` | `[{"index":1,"expression":"g_target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:513` | outside scoped roots | `Attack(GetTarget())` | `[{"index":1,"expression":"GetTarget()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:529` | outside scoped roots | `Attack(GetTarget())` | `[{"index":1,"expression":"GetTarget()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:336` | outside scoped roots | `Attack(GetTarget())` | `[{"index":1,"expression":"GetTarget()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:352` | outside scoped roots | `Attack(GetTarget())` | `[{"index":1,"expression":"GetTarget()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:450` | outside scoped roots | `Attack(GetTarget())` | `[{"index":1,"expression":"GetTarget()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:462` | outside scoped roots | `Attack(GetTarget())` | `[{"index":1,"expression":"GetTarget()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:478` | outside scoped roots | `Attack(GetTarget())` | `[{"index":1,"expression":"GetTarget()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:498` | outside scoped roots | `Attack(GetTarget())` | `[{"index":1,"expression":"GetTarget()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:512` | outside scoped roots | `Attack(GetTarget())` | `[{"index":1,"expression":"GetTarget()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:530` | outside scoped roots | `Attack(GetTarget())` | `[{"index":1,"expression":"GetTarget()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/largeplant_core.luac:105` | outside scoped roots | `Attack(GetTarget())` | `[{"index":1,"expression":"GetTarget()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/largeplant_core.luac:126` | outside scoped roots | `Attack(GetTarget())` | `[{"index":1,"expression":"GetTarget()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/largeplant_core.luac:152` | outside scoped roots | `Attack(GetTarget())` | `[{"index":1,"expression":"GetTarget()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/largeplant_core.luac:155` | outside scoped roots | `Attack(GetTarget())` | `[{"index":1,"expression":"GetTarget()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/miniboss_state.luac:53` | outside scoped roots | `Attack(GetTarget())` | `[{"index":1,"expression":"GetTarget()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/miniboss_state.luac:70` | outside scoped roots | `Attack(GetTarget())` | `[{"index":1,"expression":"GetTarget()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/monster.luac:134` | act1_level_boss_script_closure | `Attack(GetTarget())` | `[{"index":1,"expression":"GetTarget()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/monster.luac:164` | act1_level_boss_script_closure | `Attack(GetTarget())` | `[{"index":1,"expression":"GetTarget()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/monster.luac:189` | act1_level_boss_script_closure | `Attack(GetTarget())` | `[{"index":1,"expression":"GetTarget()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/monster.luac:192` | act1_level_boss_script_closure | `Attack(GetTarget())` | `[{"index":1,"expression":"GetTarget()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/rene.luac:242` | act1_level_boss_script_closure | `Attack()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/rene.luac:249` | act1_level_boss_script_closure | `Attack()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:326` | outside scoped roots | `Attack(GetTarget())` | `[{"index":1,"expression":"GetTarget()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:340` | outside scoped roots | `Attack(GetTarget())` | `[{"index":1,"expression":"GetTarget()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:356` | outside scoped roots | `Attack(GetTarget())` | `[{"index":1,"expression":"GetTarget()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:427` | outside scoped roots | `Attack(GetTarget())` | `[{"index":1,"expression":"GetTarget()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:441` | outside scoped roots | `Attack(GetTarget())` | `[{"index":1,"expression":"GetTarget()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:459` | outside scoped roots | `Attack(GetTarget())` | `[{"index":1,"expression":"GetTarget()","literal_tokens":[]}]` |

## Related natives

`Character::_Attack`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
