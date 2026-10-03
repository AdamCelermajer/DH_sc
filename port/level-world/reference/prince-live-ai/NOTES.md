# Prince live AI state and gates

The supplied Android original establishes the state producers below. A new bounded native `character_ai_state` module reconstructs `CharAI::AI_CanAttack` decision and synchronous query order. It does not replace hostility, equipment ownership, distance queries, the controller, Lua, or the full AI update with fixed acceptance values.

Original ELF SHA256: `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`. The main [manifest](original-functions.json) binds 34 functions. Separate [attack-gate capture](attack-gate/original-functions.json) binds four predicates and [inventory capture](equipment-query/original-functions.json) binds `ItemInventory::CanMeleeAttack`. [bindings.json](bindings.json) resolves the controller global and inventory virtual slot directly from this ELF.

## Initial state is not a zero-filled session

`CharAI::CharAI()` C1 `0x3ced50` and C2 `0x3cebf0` write owner-independent AI state and register the AI in the global update registry. The relevant original offsets are relative to CharAI, embedded at Character+`0x3c8`:

| Field | Constructor value | Later producer |
|---|---:|---|
| owner `+4` | preserved, not written | `SetCharacter(0x3cb7c0)` stores the owner only |
| paused `+0x18` | 0 | source pause/event lifecycle |
| active/pending AIS `+0x1c/+0x20` | null/null | script lifecycle |
| candidate/target/last target `+0x3c/+0x40/+0x44` | null/null/null | `AI_SetTarget`, `AI_SyncLastTarget` |
| target seeking `+0x4a` | 1 | `AI_PauseTargetSeeking`, event `0x32`, state/target handlers |
| sticky target `+0x4b` | 1 | actual input/control producers below |
| attack index `+0x74` | **preserved** | attack-begin depth0 writes actual step index |
| continued/last/finisher `+0x78/+0x79/+0x7a` | **preserved** | attack commands and animation consumers |
| skill ID `+0xcc` | -1 | `AI_BeginSkill` |
| skill started/stop requested `+0xd0/+0xd1` | 0/0 | skill/spell commands and consumers |

Neither constructor resets `+0x74..+0x7a`. `SetCharacter` does not reset them either. A port zero-initialized projection is its allocation policy, not a recovered original attack-session reset. The first source producer must run before relying on those fields. Constructor container headers also initialize `+0x7c`; those are separate from the preceding untouched attack bytes.

`AI_DoMeleeAttack(0x3d01ac)` sets continued=1 when already attacking and last is false (`0x3d02a4`); its new attack path sets continued=0 (`0x3d03cc`). `AI_DoRangeAttack(0x3d076c)` also writes continued=0 (`0x3d0964`). `OnTargetDied(0x3d1f60)` clears continued at `0x3d1fc8`. These commands have real state, target, networking and equipment branches; this report does not replace them with a blanket field assignment.

The source consumer module already reconstructs the remaining animation-field producers. Attack-begin `0x3d4044` captures animator **stack depth**, owner+`0x4c8`, not a weapon or combo type. Depth0 writes index from live step. Depth1 first step writes last=1/finisher=0; later steps compute last from step count and set finisher using the source comparisons. Attack-end clears continued on its tested branches. Skill-begin at depth1 step0 sets started=1. `AI_BeginSpell(0x3d81c0)` and `AI_BeginSkill(0x3d86bc)` clear started/stop-requested before beginning their sequence. EndSpell `0x3d7f60` / EndSkill `0x3d8474` set stop-requested=1 when their source skill/type gates pass and started is still false; already-started paths call original animator `StopLoop(true)`.

## Target flags and object of interest

`AI_SetTarget(0x3d6890)` always stores the requested candidate at `+0x3c`. Its true second argument takes the short branch at `0x3d6a20`, writing target `+0x40` only. It does **not** write sticky/seeking flags. The false branch performs the original notifications, owner reset, last-target bookkeeping, target-dead and sight queries. Sticky must not be inferred from the bool argument.

`AI_PauseTargetSeeking(0x3d53ec)` clears seeking and schedules event `0x32` on owner+`0x3b4` with the supplied duration, repeat=0 and null payload. The direct `RaiseAIEvent` switch for `0x32` restores seeking=1 at `0x3cbc9c`. `AI_IsTargetSeeking(0x3d49d0)` also requires owner flags `+0x520` bit `0x1000` to be clear. Seeking is therefore not solely the raw byte.

Sticky has real input producers: `Character::Ctrl_Click(0x3addc8)` writes owner+`0x413`=1 at `0x3ae084`; `HUDControls::Update(0x41a780)` on its held attack/no-object-of-interest branch passes null target to `Cmd_Attack` and stores the then-null r1 byte at owner+`0x413` (`0x41a9d8`), clearing sticky. This branch comes from actual HUD state, not animation event26. `_ClearNonStickyTarget(0x3d8d70)` retains a sticky target; otherwise it calls SetTarget(null,false), then SyncLastTarget. The live input adapter needs its own explicitly justified input producer until the original HUD path is connected.

Character+`0x14a8` is a signed **object-of-interest result byte**. Character C1 `0x3aa1b4` writes -1 at `0x3aa408` (C2 likewise). `UpdateObjectOfInterest(0x3abb9c)` uses a signed 16-bit refresh countdown at `+0x14aa`, subtracts actual Application.GetDt, resets the countdown to500 when due, clears the result to -1, and queries candidates. When it selects a candidate it stores candidate pointer `+0x14a4`, calls candidate virtual+`0x90` with the owner, and stores the low byte at `+0x14a8` (`0x3abda0`). It is not an equipment byte or a constant melee mode. Melee/attack-end paths test signed value8, but the nearby candidate/interaction producer must justify that value. Query collection/filtering remains outside this bounded module.

## Controller and global gates

Owner+`0x378` points to the controller. Controller+8 is locked, +9 is forced. Actual HUD-controller constructor `0x408930` initializes both to0 (and +`0xa` to0). Character construction also clears embedded/default controller gates.

The source router reads global `_ZN12v2Controller9s_blockedE`, `v2Controller::s_blocked`, through GOT `0x9980e8`; the byte is at ELF `0x9a318b`. It is BSS and loads as0. This is a shared controller global, not a new per-actor flag.

Verified producers:

- Script_LockCharacter.Execute `0x45dda0`: its global-name branch stores s_blocked=1 at `0x45de50`; its resolved-character branch stores controller.locked=1 at `0x45dea8`.
- Script_UnlockCharacter.Execute `0x45dc80`: global branch clears s_blocked at `0x45dd08`; resolved-character branch clears controller.locked at `0x45dd78`.
- ScriptManager.Flush `0x45a2ac` clears s_blocked at `0x45a2dc` after flushing its source work.
- Script_LookActor `0x45ec50`, MoveActor `0x45f0a8`, KillActor `0x45ea04` write forced=1 before their command and clear forced=0 afterwards. This synchronous interval bypasses global/locked gates; it is not a permanent player exemption.
- Native/Menu lock/unlock paths and Dead/Stunned/Reviving/KnockedBack state focus/blur methods also write locked. Their full frontend/FSM choreography remains separately owned.

For events24..27, forced bypasses global and locked. If not forced, global blocked or locked skips AI consumers while still forwarding the event to FSM. Events22/23 take their direct source branch before those gates; they invoke end notification and FSM regardless of generic lock/block. The existing six-event router proof covers this exact ordering. AI outer Update and the global AI queue also use these gates; a router-only binding does not reproduce their update scheduling.

Controller LookAt is not an unconditional empty service: parent recovered Character point override `0x3addbc` as a branch to GameObject.LookAt `0x393cec`. Its four-byte size does not mean `bx lr`. Object LookAt first obtains the object target point. Parent owns the recovered controller-command/path/rotation implementation.

## Actual pre-attack predicate and selected AIS

An earlier reference incorrectly calls `0x3d67f4` AI_GetTarget(false). The exact symbol and body are **`CharAI::AI_CanAttack(GameObject*) const`**. Existing shared note requiring parent repair: `reference/character-animation-ai/NOTES.md`, lines99–100. No existing reference was edited here.

The actual flow is:

1. Use the explicit target if nonnull, else snapshot AI target `+0x40`. Null returns0.
2. `AI_IsEnemy(0x3d574c)` must return nonzero.
3. Call `ItemInventory::CanMeleeAttack(0x3ffd38)` on embedded owner inventory `+0x37c`. If nonzero and `AI_IsInMeleeRange(0x3d6188)` is nonzero, return1.
4. Otherwise query owner virtual+`0x124`, Character.CanRangeAttack. False returns0; true tailcalls `AI_IsInRange(0x3d6604)` and retains its result word.

CanMeleeAttack calls inventory virtual+8 and returns `u8(result XOR1)`. The actual ItemInventory vtable `0x967760+8` resolves that slot to CanRangeAttack `0x400014`, which tailcalls HasRangedWeapon. The existing real inventory/item-table query can supply this callback via the exact complement of its source0/1 result. This is **inventory capability**, distinct from Character.CanRangeAttack's projectile-property shortcut.

OnPreAttack `0x3d0ed4` calls CanAttack(null); only a true predicate plus nonnull active AIS `+0x1c` invokes AIS virtual+`0xa4` with the captured attack index. OnEndOfAnim `0x3d0ce8` invokes active AIS virtual+`0x98` when present. Selected Player AI row44 `__player__` creates AISPlayerIPhone, whose original vtable is `0x966cd0`. Its inherited Default Init/Post/Final/Terminate and end/pre-attack methods are empty; end is `0x3dbeec`, pre-attack `0x3dbef8`. Its OnUpdate `0x3dc798` is nonempty. These empty selected callbacks do not remove the wrappers' genuine predicate, target, script and timer boundaries.

## Native kernel and evidence

`dh2_character_ai_can_attack(state, explicit_target, services, result)` reconstructs the decision flow above. State holds live owner/target identities. Query service IDs correspond to enemy, inventory melee, melee range, Character ranged capability and ranged distance. Calls are synchronous and permit state refresh. The selected target stays in a source local through the call; owner is reloaded for later queries. The final ranged result is retained exactly rather than boolean-normalized. Malformed pointer/alignment/output-alias contracts reject before callbacks or output changes. Owner/target allocation and the actual service implementations remain caller responsibilities.

[state-probes.json](state-probes.json):215 original-only cases, zero mismatches. These execute complete C1/C2 CharAI constructors with caller-provided registry capacity, complete HUD controller constructor, SetCharacter, forced SetTarget, PauseSeeking (timer-service observer), the two selected empty AIS bodies,65 CanAttack decision cases, and16 explicitly labeled two-instruction Character result-byte slices. No complete Character construction or native comparison is claimed by this probe.

[query-fixtures.bin](query-fixtures.bin), SHA256 `ff54cde7ae0e81b58773fa5e07ce0b735b93ccdb5359734e082bd5f444b26e94`:1,024 original/optimized ARM64 cases,2,006 ordered queries, zero mismatches. Fixtures cover explicit/current/absent targets, every predicate branch, noncanonical return words, synchronous owner/target changes, and native identities above4GiB. Host ASan/UBSan replay passes the same1,024 cases plus five atomic guards, with zero findings. Original hostility/range/inventory calls are named service fixtures in this corpus, not executions of those unreconstructed backends.

Reports: `reports/character-ai-state-arm64-differential.json` and `reports/character-ai-state-host-audit.json`. Build oracle with `tools/build_character_ai_state_oracle.ps1`; run `tests/character_ai_state_differential.py` with engine/library/report/reference-output arguments. `tests/character_ai_state_host.py` builds an isolated sanitizer executable and binds compiler inputs before/after; CMake-compatible host source is `tests/character_ai_state.cpp`, argument `<query-fixtures.bin>`. Original-only state discovery is reproducible with `tests/prince_live_ai_discovery.py` using the bound local ELF.

The parent integrated production/host targets separately. `tests/character_ai_state_host.py --main-linked` replays the actual central `character_ai_state_audit` without a rebuild, saving **separate** `reports/character-ai-state-main-linked-host-audit.json`. This passes1,024 cases/2,006 queries/five guards/zero sanitizer findings and binds executable, actual `libdh2_level_world.so`, all resolved DSO dependencies, current source and gold bytes before/after. It records observed current artifacts rather than inferring compiler provenance. Android project-library instruction evidence is pending the parent's loader with a modeled Android TLS canary; isolated ARM64 and central host evidence do not establish that result.

This agent changed no existing source, renderer, CMake, APK, ADB, old corpus, or old report. Full live Prince AI additionally needs genuine target/hostility/radius services, controller command gates, selected AI state publication, source Application clock/frame and update order, and the Lua/timer backend being recovered in parallel. This bounded evidence is not full AI, combat, frame or game parity.
