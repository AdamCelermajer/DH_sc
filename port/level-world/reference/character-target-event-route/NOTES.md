# Target notification routing and the Revived endpoint

The NEW proof in this directory checks the **existing complete** production dispatcher `character_ai_events.cpp`, export `dh2_character_ai_event`. No second runtime router is needed. An initially drafted subset was removed before integration; none of its production files remain, and the final reports bind the actual existing full dispatcher.

## Original gates and forwarding

The original Character `RaiseEvent 0x3a4d5c` forwards these events to its embedded AI `+0x3c8`, then complete `CharAI::RaiseAIEvent 0x3cbb34` captures event and payload. The generic gate reads current owner's controller forced byte `+9`; any nonzero bypasses global blocked and controller locked byte `+8`. Without forced, either nonzero gate suppresses the CharAI handler and forwards to the current owner's FSM. Gate bytes are raw source bytes, including `255`, not a manufactured successful predicate.

When permitted, Character event **9** calls CharAI virtual `+0x34` (`OnEnemySpotted(capturedCharacter)`), ignores its return, **reloads owner**, then forwards the captured event/payload to its FSM. This happens even if the handler internally returns early. A synchronous handler may replace owner; source forwards to the replacement owner's FSM.

Permitted target events **`0xa..0x11`** call CharAI virtual `+0x40..+0x5c`, respectively, and return **without FSM forwarding**. Their virtual calls are no-argument notifications: the original event payload is retained by the outer dispatcher but is not an actual argument to those methods. If the generic gate blocks, all these events instead go to FSM without invoking the CharAI notification. Do not implement a uniform handler-then-FSM route for all nine IDs.

The existing dispatcher matches these distinctions, so no shared source change was made. Its service `ai_event_virtual` must compose to the separately frozen `character_target_events` prefix implementation for `0xa..0x11`, or the genuine EnemySpotted prefix for event9. Only the selected active AIS virtual endpoint after each prefix may reach external script dispatch. `ai_event_state_event` is the real current-owner FSM provider. Deeper FSM/Character ownership is not supplied by this proof.

## TargetRevived is genuinely inherited empty

Actual source vtable address points and slot `+0x44`:

| Selected class | Address point | Raw slot word | Actual body |
|---|---|---|---|
| AISDefault | `0x966978` | `b0be3d00` | `AISDefault::OnTargetRevived 0x3dbeb0` |
| AISExternal | `0x966a50` | `b0be3d00` | same inherited Default body |
| AISPlayerIPhone | `0x966cd8` | `b0be3d00` | same inherited Default body |

`0x3dbeb0` is exactly four bytes **`1eff2fe1`**, ARM `bx lr`, SHA256 `379bec29dccd0a93c94826144d7ef6e42fab64ef195a3b8313a16926f66f388f`. The selected Default/External/IPhone target-Revived virtual therefore has no memory, state or VM effect. There is no missing `OnTargetRevived` Lua wrapper to invent. The separate CharAI `OnTargetRevived 0x3d1eb4` still executes its full debug prefix and active virtual call; it is not skipped upstream. Captured `AISMonster::OnTargetRevived 0x3dd490` is also `bx lr`, but its selected vtable was not needed for the three-class proof above.

The source probe executes the actual single-instruction body with eight receiver words including null, unaligned and unmapped values, observes unchanged sentinel memory and retained `r0`. This is **original-only endpoint evidence**; void return register contents are not a new semantic native API. Selected receiver/vtable lifetime and the source CharAI prefix must still be supplied faithfully.

## Evidence and reproduction

`original-functions.json` captures nine complete functions, including both routers, the empty endpoints, complete EnemySpotted handler and group/aggro helpers. Original ELF SHA256 is `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.

- `tests/character_target_endpoints_original.py` executes complete original Character and CharAI routers for **1,944 cases** across events9..17, all0/1/255 gates, ignored handler return0/1, null/high payload and post-handler owner replacement. It verifies the actual selected vtable bytes and eight empty Revived executions. `endpoints-original-probe.json` explicitly reports native comparisons0.
- `tests/character_target_event_route_differential.py` independently runs O2 ARM64 **existing `dh2_character_ai_event`** against those source observations: **1,944 cases / 2,096 service calls / zero mismatches**. It verifies exact virtual operation/callee, no-argument target notification payload0, EnemySpotted's captured payload, current-owner FSM identity and native identities/payloads above4GiB.
- Sanitized host replay: **1,944 cases / 10,328 word checks / nine malformed guards / two provider failures / zero ASan/UBSan findings**. One native-only recursive event test and one native-only captured-payload mutation test are reported separately and do not inflate original comparison counts.
- Gold `target-route-fixtures.bin`, magic `CTR1`, SHA256 `4ad8f6a1c7987970c5c223baeda6e8dfda9bec40aaa0add5603ec65f71585935`. Logical handler entries retain dispatcher-context event/payload for trace comparison; the proof separately checks that target virtual calls have no payload argument.

Final existing production hashes bound by these new reports: header `e78a1542dd1316047414419bcea7d8ad9fee32a410959d2f80db4af47ef357c3`; source `fa459fb66755400887d4e042d1038013001ec2ebd3746962a7d2f29830baa5b0`. The isolated O2 audit DSO SHA256 is `89966e7b24b1354ec8395303193b226e91752a70ea162ec96347280a2d5dddc2`. Reports are `reports/character-target-event-route-arm64-differential.json` and `reports/character-target-event-route-host-audit.json`.

The build tool `tools/build_character_target_event_route_oracle.ps1` compiles only the existing `character_ai_events.cpp`. New test target `character_target_event_route_audit` needs only `tests/character_target_event_route.cpp`, linked to the existing world DSO; argument is the CTR1 corpus. Main-linked invocation:

```powershell
& C:\Users\adamc\AppData\Roaming\uv\python\cpython-3.12.13-windows-x86_64-none\python.exe port/level-world/tests/character_target_event_route_host.py --main-linked /home/adampalace/dh2-world-build/character_target_event_route_audit
```

Standalone runner omits `--main-linked`; it records actual compiler/source/corpus/executable hashes. Main-linked mode saves a separate report and pins executable/dependencies before and after replay, while leaving central compiler provenance to the central build owner. No CMake, runtime, session, renderer or APK edits were made here.

The complete `OnEnemySpotted 0x3d14b4` body is captured but its deep group, state, combat/IsPlayer, aggro and active-AIS services need separate prefix reconstruction. In particular, it compares GetAggro **equal to0** before adding threat, and tests the AddAggro return **greater than0** before a second `isTracingThreatChange` debug prefix. It reads initial threat from `Arrays::DesignSettingsTable::members` member `+0x30`; that live table producer must be established before treating an input fixture as authored threat.
