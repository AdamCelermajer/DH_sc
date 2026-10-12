# Crypt live owned-state readiness

This is a read-only integration review of the frozen live renderer stage, not a new live-enemy implementation. No renderer, DACT, shared source, CMake, APK or emulator changes were made. `original-functions.json` binds the captured ARM32 function bytes to original ELF SHA `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`. `crypt-authoring.json` reads actual MGP XML directly from the authorized cache ZIP SHA `3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679`; it records each source member hash.

## Authoring and genuine descriptor defaults

All eleven placements request `_templateName="Monster"`. Five direct placements author `ai_state="Idle"`; six cemetery temporary cultists omit that attribute. The six are `_prim_tmp_cultist01`, `03`, `05`, `06`, `07`, `08`. Their resolved source descriptor value is the **empty string**, and original `Character::GetPreSetAIState` maps that value to **Idle3**. Do not turn their raw missing attribute into an authored `Idle` string, or assign PreSpawn17 as a convenience default.

| Field | Six temporary cultists | Five direct monsters | Shipping descriptor default |
|---|---|---|---|
| `ai_state` | absent | `Idle` | empty string → preset Idle3 |
| `ai_state_visible` | absent | absent | true |
| `auto_spawn` | absent | absent | true |
| `spawn_delay` | absent | absent | signed integer pair `[0,0]`, raw ms |
| `spawn_view_radius` | absent | absent | float32 `+0` |
| `char_group` | absent | authored empty | empty string |
| `char_group_role` | absent | `Normal` | empty string |

`char_group` text is resolved here; conversion to a live GroupInfo pointer is still a separate original AddToGroup/lookup producer. An empty string is not a proved substitute for executing that producer. Source PreSpawn permission uses the actual group pointer and the raw auto-spawn byte. It is not needed to invent initial state17: all eleven current source presets resolve to3.

The exact loader chain is `ObjectManager::LoadFromXML` `0x34b868`:

1. `0x34b95c`: actual `GetNewObject` factory, with source XML name/game type.
2. `0x34b9a4`: `PropertyMap::InitProperties` `0x513d78`, on the embedded receiver at object+4. A missing class descriptor map invokes virtual DeclareProperties at `0x513fc8`.
3. `0x34b9b4`: read `_templateName`; if present, `0x34b9ec` calls `SetTemplate` `0x513fec`.
4. `0x34ba28`: `LoadDefaultProperties` `0x5136ec`, which iterates the real descriptor map and calls each descriptor virtual+0x0c.
5. `0x34ba40`: `LoadOverridesFromXML` `0x513a00`. It iterates descriptors, calls XML Attribute, and **always** passes that result to `SetProperty` `0x51387c`; missing attributes are not ignored.
6. `SetProperty` resolves `(Property*,receiver)` with `GetProperty` `0x513858`. Nonnull text invokes virtual+4; null text takes `0x5138d8..0x5138f4` and invokes virtual+0x0c, resetting to the descriptor default.

`Character::DeclareProperties` `0x3a9fe4` adds `ai_state` at `0x3aa038` through `AddProperty<string>` `0x33ef7c`. That overload constructs an **empty** string default (`0x33efc4..0x33efdc`), then passes it to the default-bearing overload `0x33e404`; it does not snapshot some earlier actor's current ai_state. String reset `0x33e298` assigns the stored default to the receiver at descriptor-relative offset. `GetPreSetAIState` `0x3a5784` immediately returns3 for the resulting empty range. Its explicit nonempty special cases are Limbus0 and PreSpawn17; other text, including Idle, returns3.

The bool descriptor clone at `0x3a92b4` stores default1 at descriptor+0x20 (`0x3a930c`). DeclareProperties uses it for visible `0x3aa04c` and auto-spawn `0x3aa0a0`; reset is `0x33de80`. Delay descriptor defaults are two zero words at `0x3aa108/0x3aa10c`, reset `0x3a35f0`. Radius descriptor default is float bits0 at `0x3aa164`, reset `0x394f00`. The constructor also initializes visible/auto true, delay zero and ai_state empty, but the descriptor/reset evidence is what resolves omitted XML.

`SetTemplate` `0x513fec` stores a nonempty requested template name, obtains the instance map, and clones class-default descriptors if its map is empty before calling `LoadTemplate`. The shipping `LoadTemplate` `0x51419c` has only assertion-policy behavior: policy2 attempts a null store; policy1 emits a diagnostic (source line9); other policies return. It never reads the receiver/name, looks up a template file, or changes actor fields. Thus the Monster name cannot silently introduce a hidden PreSpawn default in this captured shipping implementation. `SetTemplateParameter` `0x513830` is a separate descriptor-default mutation API; no direct ARM branch caller was found in the ELF scan. This review does not claim a complete external API/startup audit of arbitrary descriptor mutation.

`.local-inputs/world/crypt01/crypt01.dwld` begins `DWLD` v1 and is the port's compiled descriptor. The original XML-loading authority for this review is the ZIP MGP resources, not that binary. Current `objects::Record`/DACT v1 preserves kind/room/name/character/model/position/rotation/scale/placement and drops these conditional/spawn/group attributes. `prepare_actors.py` uses `obj.get('auto_spawn','1')` to choose records; this is a converter filter, not evidence of the original default.

## Static evidence versus executed probes

The full loader order, descriptor constructors, template assertion stub and raw authored inventory above are static instruction/XML evidence. **The entire ObjectManager factory/XML/STL-map pipeline was not executed.** No optimized native counterpart, packaged instruction parity or new emulator test is claimed.

`default-probes.json` records thirteen actual original-instruction probes:

- Three LoadTemplate calls with assertion policies0,1,3 preserve a 512-byte receiver. Policy1 logger is an explicit no-effect diagnostic service; policy2 deliberate invalid memory store was not executed.
- Six absent cultist ai_state projections execute original `SetProperty(NULL)` → original string descriptor reset → original `GetPreSetAIState`, returning3. Property-map lookup and empty STL assignment are declared service fixtures, not fabricated template lookup.
- Two actual bool resets produce1, one actual Point2D<int> reset produces0/0, one actual float reset produces bits0.

The script `capture_probe.py` is reproducible with the repository's existing Unicorn/Python dependencies. It snapshots source bytes and re-reads XML from the authorized ZIP; it makes no cache/source mutations outside this new reference directory.

## Existing live backings and missing producers

| Required owned-state input/service | Existing production backing | Remaining work before claiming genuine enemy behavior |
|---|---|---|
| Character identity, PropertyState, CombatActorState, private Lua VM | `ObjectActor`, stable `ScriptCharacterObject`, `MonsterScriptHandle`, shared property/combat state; all11 genuine Init sessions | retain one actual StateOwner/native FSM per character, with the existing identity; session state_machine/commands are currently not supplied |
| `SpawnOwnerExtensions72.machine` | complete frozen `CharacterStateOwner` module and behavior/frame/spawn adapters | allocate it per monster; use its StateInfo current metadata and actual commands, never assign current directly |
| `.remaining`, `.outer_methods` | frozen Idle/Move/Attack/Dead behavior; empty and PreSpawn extensions; full outer ordering | persistent Facts/Services/predicate projections and actual Character RaiseEvent must exist; other nonempty families remain mandatory providers |
| `.spawn` (`SpawnBody48`) | stable character identity and decoded CharacterAnimation table available | persistent source 40-word row projection/count, live VisualObject identity+0x2d8 and fade word+0x1440 are not ObjectActor fields; cannot reinterpret the decoder's37 schema fields as40 runtime words |
| `.spawn_services` debug | genuine DebugSwitches owner, filesystem and queries in WorldScriptContext | use actual debug service; a no-op trace/fake GetSwitch is unnecessary |
| `.spawn_services` animation index/stance/mask | genuine GetCharAnimTableId/property2 fallback17, constants/table lookup, GetAnimStance kernels | bind live non-player/source equipment facts; stage source-selected sequences and their default resource with authentic registration identity |
| `.spawn_services` ANIM_Set/scene events | genuine BlendedPlayback and source event/router modules exist for Prince | monsters still use AnimationScheduler/start/complete and raw Player sampling; no per-instance scene/bank/BlendedPlayback, genuine24/26/27/23/28 choreography or synchronous Character→AI→FSM observer |
| `.spawn_services` target NULL + sync | `ScriptCharacterObject::target` and genuine native target kernels exist | route the existing target state; prototype `combat_target/target_sight/state` is independent and must not become a second authoritative AI state |
| `.spawn_services` CancelSneaking | genuine recovered kernel exists | supply true NPC IsPlayer predicate and resolved198; positive skill path requires real skill/AIS providers. Prince default logging fallback is not delivery |
| `.spawn_services` fade | exact source empty `StartFadeIn470ce4` helper exists | deliver the actual empty helper; do not invent a visual fade clock |
| `.spawn_services` InitPhysical | genuine character bounds/config, NativeWorld, native body and world backend exist | no NPC body is currently created. Recover/project each actual NPC mesh/controller/equipment bbox, source visual scale + resolved16 bounds, type/group/disable/owner policy; maintain body ownership/sync. DACT scale already includes character scale; do not apply it twice or substitute radius36 |
| `.selection` | actual NativeSpawn24/SetSpawnState source kernel exists | bind same native FSM, raw descriptor delay and actual shared RNG; source StateOwner transition or genuine TimerStart, not manual assignment |
| `.permission` | genuine current17 group false predicate/raw byte kernel exists | source group-pointer producer still required when this branch is reachable; raw default auto byte1 is now proved |
| `.timers`, `.timer_services` | session owns genuine TimerStore/storage; source Init timers33/34 are present; effect modules33/34/35/36 exist | supply genuine Character/AI timer expiry routing and dependencies. Renderer currently does not advance these timers; do not route every expiry straight to state_owner.event |
| Native movement/path/physical runtime | complete runtime/controller/nav/subobject kernels exist for Prince | ObjectActor has no per-monster runtime/controller/body/backing. Prototype positions and legacy sampled animation are not genuine UpdatePath/root services |

The renderer evidence is in `port/android-native/app/src/main/cpp/model_renderer.cpp`: ObjectActor70..84; MonsterScriptHandle165..170; reset367; prototype update_enemy524..545; Prince-only body/runtime/blended setup814..947; frame949..1005; prototype scheduler/bank collection1073..1121; source script creation1170..1240; late raw enemy sampling1432..1471. `objects.hpp`, `objects.cpp` and `tools/prepare_actors.py` explain the current descriptor boundary. Line numbers are review anchors, not promises that future edits retain them.

## Smallest source-faithful next change

First preserve a genuine initialization projection next to the current placement record: raw authored/missing distinction, source descriptor defaults, requested template, ai_state text, visible/auto/delay/radius/group/role. It should mirror the original InitProperties→SetTemplate→defaults→overrides order above. All eleven resolve preset3; no initial Spawn/PreSpawn is justified merely because the new bodies are available.

Retain the actual StateOwner/native FSM and its persistent behavior Facts/Services on the existing heap MonsterScriptHandle, before the session member. Pass that same native FSM to CharacterScriptSession at its already existing creation point; retain context lifetimes through VM close and GL recreation. Initialize via the recovered LevelLoadCharStates/native preset command after the actual source InitPost/script initialization order, including genuine outer event1d. Active AIS can already be published at that point; it must not be suppressed under an assumed “before script Init” phase. Original Level load stage10 finishes InitPost before stage18 LoadCharStates; Character InitPost can initialize the script before AddToGroup.

An owned Idle3 focus/GetState startup milestone is smaller than claiming full Spawn or full AI movement. Idle Focus is the already proved flags2380 plus source ANIM_Set path, but it still needs real animation/event backing. It must not falsely claim the absent NPC physical/frame producers are completed. Once the real playback/FSM frame is enabled for an actor, stop executing its parallel prototype scheduler/controller; derive diagnostics from the owned current state.

Per-instance CPU Scene and immutable registered Player/bank backing must survive each borrowed BlendedPlayback. Current group scene is shared and sampled late, and GL reset destroys group bank/resources while preserving Script handles. Either keep genuine CPU backing alive across GL reset or explicitly detach and authentically recompile/reselect before it is destroyed. Preserve source registration occurrence identity/default resource; the current sorted unique Idle/Walk/Attack/Died clip set is not original Monster registration proof and does not even stage PreSpawn/Spawn by policy.

For real frame composition use all characters' Scene phase before **one** shared physics Step, then per-character timers→AI→FSM→Animator→GameObject path/subobjects. Current Prince code embodies the bounded order; enemy raw samples occur after Step and cannot simply be wrapped as another scene phase there. Do not add a per-enemy Step or a second elapsed increment. Preserve the shared embedded Character/FSM field projections rather than creating parallel current/idle flags.

Spawn1/PreSpawn17 then become genuine reachable families only through actual commands/events and the authored/default producer. Their mandatory animation, CancelSneaking, scene and InitPhysical services must be supplied before accepting those transitions. The current source-empty Fade and empty methods can be delivered without inventing effects; unbound nonempty methods must continue to fail explicitly.
