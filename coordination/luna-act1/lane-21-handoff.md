# Lane 21 handoff: ordinary enemy AI and spawners

Status: source owners are present; renderer integration remains pending. No source or gameplay verification is claimed by this handoff.

## Existing source delivered

- `source_campaign_ai_queue_v105.cpp` borrows the active candidate World, App dt and the same registered Character/CharAI records. `character_world_ai_queue_v1` is process-wide and its constructor/destructor registrations are already on `RetainedCharacterActorV1`; the query path uses live controller forced/locked/global-blocked, dead/remote, zonability and the actual source bytes. Root's stage 32 binding already calls `source_campaign_ai_inc_queue_v105`.
- `spawn_group_manager_v108.cpp` owns the process `SpawnGroupManager` projection. SpawnSpot `InitFinal`/destruction bindings enroll and erase the actual retained spots. `source_campaign_spawn_groups_v108.cpp` reads `Arrays.SpawnGroups` from the same process Arrays lease, consumes the source RNG, honors authored zone eligibility and calls the canonical Character spawn → `Character::InitSpawned` → `SpawnSpot::PlaceObject` path. The existing early gameplay packet connects its update callback. Root's `native_spawn_groups_v108.inc` only borrows `original_ui.process_arrays_borrow_v101`.
- `renderer_npc_commands_v1.inc` provides the same retained controller/target/path command facade. `renderer_npc_state_animation_v1.inc` supplies Walk/Attack/Idle body services and source AnimationAI operations against that actor. `renderer_npc_attack_frame_v1.inc` supplies the retained NPC attack command owner; `renderer_npc_melee_event_v1.inc` routes source named animation events and selected AIS OnAttack through the NPC inventory/world combat owners. Existing handoff notes in the two assigned renderer handoff files carry the lifetime and callback contracts.

## Original source anchors checked

- `CharAI::IncUpdateQueue` `0x3ce9b8`: countdown starts at `-1`, resets to `180`, rotates through constructor-registered AIs, and applies forced/global/locked/dead/remote/visible/zone gates. Its caller is `Level::Update` `0x3f84ec`.
- `CharAI::Update` `0x3cfbf4`: pause/controller/flags/zone gates precede `_UpdateTarget` `0x3cb908`, `_UpdateMaster` `0x3cc5a4`, `_UpdateAggro` `0x3cf3f0`, then `OnUpdate` `0x3d1050`.
- `SpawnGroupManager::InsSpawn` `0x3ea098`, `DelSpawn` `0x3e907c`, `update` `0x3e9238`; `SpawnSpot::InitFinal` `0x3ea7b8`, `PlaceObject` `0x3ea704`; `Character::InitSpawned` `0x3b379c`. The existing manager mirrors the observed group delay, weighted row selection, unique eligible spot sampling, `Spawn_%05u` naming, and canonical spawn/init/place order.
- IDA pseudocode was checked against `assembly-functions.asm` for the queue and spawn manager. Relevant per-character AI frame/target evidence is retained in `port/level-world/reference/character-ai-frame/NOTES.md` and `character-target-update/NOTES.md`. No Ghidra output was used.

## Root wiring required

`source_campaign_ai_inc_queue_v105` and `update_spawn_groups` already enter through existing campaign stage/gameplay owners. Root must activate the retained NPC adapters in the existing renderer owner: preserve one command owner on each actual `MonsterScriptHandle`; attach its script bindings before `construct_script`; retain the attack/inventory/state/melee/frame owners on that same handle after the shared World attack graph is available; refresh actual NPC facts before StateOwner/FSM dispatch; route source state bodies 3/4/5 and StepBegin/StepEnd through `RendererNpcStateAnimationV1`; route named attack events through `RendererNpcMeleeEventV1`; and run the actor frame after Scene/FSM/Animator in original order. These changes touch root-owned `model_renderer.cpp`/renderer includes and CMake/build wiring, outside lane 21's write boundary.

Known source boundary remains actual production melee/projectile/skill endpoint callbacks and full actor rendering/physics integration where supplied services are absent. Keep those as explicit failures; do not infer success from the current adapters or author substitute rewards/AI groups.

## Canonical same-Character `Cmd_Attack` provider plan

Lane22 has now supplied the same-Character script command path; `script_controller_attack` at `source_campaign_character_fsm_v101.cpp` remains the explicit missing branch. Keep the attack owner as a retained member of that record's `CampaignFsmV101`, created lazily from the current record and reused for each call. It must borrow only `r_.services.world_targets`, `r_.actor->object->binding`, `r_.actor->machine`, `r_.actor->controller->command_state(...)`, `r_.actor->animation_ai`, and the record's source OOI field. Do not create a second target, FSM, AI record, World, or `MonsterScriptHandle`; this NPC path has no Crypt `MonsterScriptHandle` dependency. Use `NpcAttackCommandOwnerV1` as written: it wraps the proven generic `CharacterWorldPlayerAttackOwnerV1` command coordinator, while all projected supplemental fields are read/written on this same actor.

The canonical owner composition is implementable in `CampaignFsmV101` as follows (names illustrative; root owns this source integration):

```cpp
bool ensure_attack_owner() {
  ControllerCommandState32* c{};
  if (!controller(c) || !geometry() || !r_.actor->machine ||
      !r_.services.world_targets || !r_.actor->object) return false;
  const auto* ooi = &r_.actor->source_ooi14a4; // actual Character +14a4 field
  const std::uint8_t* network_a = nullptr; // replace only when exact controller +a backing is exposed
  attack_owner_ = std::make_unique<NpcAttackCommandOwnerV1>(
    NpcAttackCommandBorrowV1{r_.services.world_targets.get(),
      &r_.actor->object->binding, r_.actor->machine.get(), c,
      &r_.actor->animation_ai, ooi, network_a},
    NpcAttackCommandServicesV1{this, attack_online, attack_backend,
      geometry_->queries(), attack_melee_radius});
  return true;
}

// In script_command_invoke, for script_controller_attack:
const auto* scope = t.r_.actor->session
  ? t.r_.actor->session->current_skill_callback_scope() : nullptr;
return t.attack_command(q->target, scope) ? 0 : -1;
```

The real implementation must check actor identity coherence on every call (same target owner, controller owner, FSM Character, and AI owner); refresh the live controller projection and copy the same FSM's `controller_locked` into it as `script_commands_refresh` already does. `NpcAttackCommandOwnerV1` temporarily borrows the exact callback scope through `TargetBindings48.scope` and restores it with RAII. `scope == nullptr` is allowed only when the source call truly has no active session scope; do not substitute a renderer-global scope. The caller's requested target is the existing `ScriptCommandRequest40::target` resolved by the existing script-command owner; null remains null so original acquisition runs.

The callback backend map is:

- `attack_network_mode`: `r_.services.online_byte5`, same candidate World/App. The owner already fails explicitly when online is true but the exact original v2Controller byte `+0xa` cannot be borrowed. `ControllerCommandState32.reserved` is not evidence of that byte; do not alias it or initialize the missing byte to zero. The current canonical controller projection has no established `+0xa` backing, so online NPC packet scheduling remains a concrete integration blocker until the source controller owner exposes that exact byte.
- `attack_set_attack_state`: already owned by `NpcAttackCommandOwnerV1`; its special case calls this same `r_.actor->machine->event(0xc354, target)`. Keep this source FSM route; do not transition state directly or replace it with `prince_event`.
- `attack_frontal_angle`: actual design lookup `CharacterDesign/Attack_FrontalAngle`. `attack_diagnostic`: same World Debug/file lookup and the exact source string keys already used by `renderer_player_attack_v1.inc`.
- `attack_owner_dead` / `attack_target_dead` / `attack_owner_player`: same registered World target/list predicates; owner-dead must remain the actual owner virtual predicate, not an assumed live value. `attack_is_attacking`: same native FSM integer state equals5. The remaining melee predicates (`IsEnemy`, `InventoryCanMeleeAttack`, `IsInMeleeRange`, `CharacterCanRangeAttack`, `IsInRange`) come from `geometry_->queries()` over the same registered world, actual `inventory37c`, AI tables, and geometry. `melee_radius` is `geometry_->melee_radius`.
- `attack_list_create/reset_sort/search/pop/destroy`: the generic attack coordinator already uses the same registered Character's source TargetList/search services and `target_frontal_sort_v41`; preserve actual ordering, empty-search target retention, and list lifetime. Do not add a chosen nearest-target fallback.
- `attack_set_target` / `attack_sync_last_target`: same `TargetBindings48.services` (`dh2_character_ai_set_target` and source BackupTarget) already embedded on the actor's `ScriptCharacterObject`. The generic owner dispatches them in original order.
- `attack_range_redirect`: retain/use `CharacterWorldRangedAttackV41` over this same world, geometry, target binding, `State`, attack fields, and heading-active producer; pass the original requested target and speculative mode. Its required inventory, look-at, state-event and same-AI melee-fallback callbacks remain explicit. Until a complete actual ranged owner is retained and this callback calls it, ranged-capable NPCs must fail at this reached continuation; they must not be reported as ordinary melee attacks.
- `attack_network_send`: implement the original `CMsgControllerAction` source operation with actual owner network ID at Character `+0x108`, action byte `0` at message `+0x50`, requested target's low16 network ID at `+0x52` (zero for null), and actual Client enqueue. Reuse only proven same-World network manager/message factory/client/send providers where exposed (the UseOOI operation's action value is `5`, so do not reuse that operation or packet). This callback is reached only through the original online/enable/Character gates and changed speculative target/continued decision.

`attack_controllable_dispatch` is intentionally satisfied inside `CharacterWorldPlayerAttackOwnerV1` for the same controller Character: it recursively invokes the same `AI_DoMeleeAttack` coordinator with mode=false, matching `v2Controller::Cmd_Attack -> controllable virtual +0x38 -> Character::Ctrl_Attack -> CharAI::AI_DoMeleeAttack(false)`. Thus offline ordinary melee still proceeds. The same FSM's `event(0xc354)` then drives authored Attack state/animation; named animation OnAttack must reach the existing `SourceCampaignCombatV115::melee` provider with this actor's actual `inventory37c`, selected AIS virtual, World combat owner, and synchronized random stream. This does not guarantee a hit: missing selected-AIS OnAttack, inventory, range, combat context, or downstream callback remains a required failure.

IDA/assembly anchors for this composition: `v2Controller::Cmd_Attack` `0x405b04`; `Character::Ctrl_Attack` `0x3ad87c` (thunk `0x3ad874`); `CharAI::AI_DoMeleeAttack` `0x3d01ac`; `SM_SetAttackState` `0x3c6488` to FSM event `0xc354`; AI SetTarget `0x3d6890`; SyncLastTarget `0x3d49c4`; melee geometry `0x3d6188`; AI range redirect `0x3d076c`; network manager singleton `0x80b1bc`. The exact controller ordering, speculative snapshots/restoration, gates, packet fields, and melee branches are recorded in `port/level-world/reference/prince-live-attack/NOTES.md` and `NATIVE.md`, checked against original `assembly-functions.asm`. Existing executable coordinator/provider fixtures establish caller choreography only; no live NPC command or gameplay verification is claimed here.

Root wiring sequence: add one retained `NpcAttackCommandOwnerV1` member and its ensure/callback methods to `CampaignFsmV101`; replace only the explicit `script_controller_attack` failure; then wire actual ranged and network continuations plus the exact controller byte `+0xa` when their same-owner providers exist. The lane does not modify C++ while root's build is in progress. No builds/tests were run for this handoff update.

## Canonical offline NPC attack implementation update

`source_campaign_character_fsm_v101.cpp` now retains one `NpcAttackCommandOwnerV1` per `CampaignFsmV101` and routes `script_controller_attack` through it with the original `q->target` and the active same-session callback scope. New private entry/helpers are `ensure_npc_attack()`, `command_attack(uintptr_t)`, `attack_online`, `attack_backend`, and `attack_melee_radius`; there is no public API/header change. The adapter borrows the record's `world_targets`, `ScriptCharacterObject::binding`, machine, live controller command state, `animation_ai`, and `source_ooi14a4`. Before each invocation it refreshes only owner/controller/current-target/look-target/flags/seeking/sticky from the actual actor and 412/413 fields, preserving attack sequence fields owned by the source Animator.

Offline melee services now use same-World online byte+5, actual `CharacterDesign/Attack_FrontalAngle`, same-World DebugSwitches with original AI trace keys, existing target-list/SetTarget/BackupTarget services, actual `inventory37c` geometry, and same FSM event `0xc354`. Network byte+a stays null; `NpcAttackCommandOwnerV1` only rejects it when online mode is actually true. Actual ranged redirect and packet-send remain explicit reached failures (`AI_DoRangeAttack` owner; `CMsgControllerAction` sender), so ranged-capable/online NPC behavior is not claimed complete. No build/test was run as root owns build verification.

## Offline command wiring delivered

Exclusive source implementation is in `source_campaign_character_fsm_v101.cpp`: one retained `NpcAttackCommandOwnerV1`, live same-record actor/controller/target/AI refresh, identity coherence checks, same-session callback scope, original requested target, real online/design/debug/melee-radius callbacks, and only reached failures for unavailable NPC range/network continuations. The existing lane22 script commands and timer services remain connected. No public API changed. Root owns compilation and tests; neither was run here. This source wiring is still not runtime/gameplay verification.
