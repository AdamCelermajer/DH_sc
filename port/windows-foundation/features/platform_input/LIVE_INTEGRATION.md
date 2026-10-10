# Live command integration

`live_dispatch.hpp/.cpp` is an optional source-command bridge. It is independent
of Win32/Android transport and acquires all owners freshly through `BorrowLive`
for each frame containing commands. No actor/controller/HUD/skill owner is
constructed by this module, and no borrow is retained across actor replacement.

Root must provide the existing leased player owner, its stable actor ID, native
Character identity and native controller identity. Requested endpoints require:

* Attack: actual `HudAttackHeldFieldsV46`, its same joystick-active byte, and
  complete actual `HudAttackServicesV46`. The source whole held-update prefix
  stores global input admission, observes level/cache/player owners, and chooses
  `Cmd_UseOOI(NULL)` or `Cmd_Attack(NULL)`. The local-player borrow is validated
  against the same native Character/controller before dispatch.
* Interaction: existing `ControllerUseOoiV47`, whose identity must match the
  borrowed controller. Original forced/locked/global flags and network prefix
  remain inside that receiver. The platform preserves the original NULL request.
* Skill slots: a same-controller callback taking the hotbar slot and both button
  edges. Press must reach original Ctrl_BeginSkill/Cmd_BeginSkill; release reaches
  Ctrl_EndSkill. `skill_ai_begin_v3` and `skill_ai_end_v3` are distinct from
  `skill_ai_use_v3` (native DoSkill) and authored `skill_ai_event_v3` (do_skill).
  Use `SessionSkillAnimation::hotbar_command` if an actual borrowed owner is
  available; never construct a private SkillAIContextV3 or SkillStateV4.
* Spell/potion: actual same-controller commands; no invented costs/effects.
* Target selection/world click: callbacks to the existing target owner, with
  actual projection/hit resolution for a screen point. No generated actor target
  or assumption that a screen point directly maps to a world position.

Call after UI transitions. Opening a menu invalidates the previously collected
gameplay frame: cancel via `set_menu_open(true)`, take its new cancellation frame,
then dispatch release edges (not the pre-open press/held frame). Focus loss,
level-disabled, reload and restore must similarly cancel semantic controls while
old source owners remain valid. After replacement reacquire owners; never replay
old queued clicks into a new world. This prevents lost EndSkill/release events.

`LiveDispatchResult::residual` removes commands already sent through retained
owners, preventing double attack/selection/interaction in CombatSession.update.
Do not also send `Frame::actions.attack` after HUD has dispatched it. If the
existing HUD attack service chooses to queue attack in the same CombatSession,
it must explicitly publish that one queued request through the host's sole
controller path. Held attack plus an interaction key in one frame delegates OOI
once. Successful source void wrappers may be blocked; dispatched flags do not
claim gameplay admission or completion.

Current host gap observed: CombatSession exposes actor/player/attack state, but
no public borrowed ControllerUseOoiV47 or SkillAIContextV3/SkillStateV4 owner.
Root main initially wires only actual portrait/menu hit shapes. Remaining
authored attack/skill/joystick receivers and real command borrowers must be wired
before declaring this dispatcher live in the game. Missing requested owners
return a specific error instead of substituting a diagnostic/private receiver.

Build integration: add `live_dispatch.cpp` plus production source kernels
`port/engine-ui/hud_attack_control_v46.cpp` and
`port/level-world/character_use_ooi_v47.cpp`. The latter also needs original
`character_state.cpp` and `character_target_bindings.cpp`, with the real script
runtime implementations used by target-bind callbacks. Do not link
`live_dispatch_test_support.cpp` into a production target: its rejecting Lua
endpoints exist only for standalone offline HUD/controller fixture tests.

Strict standalone `live_dispatch_tests` pass against original HUD held-update
and ControllerUseOOI kernels. They verify same-player identity, OOI choice,
actual controller block, disabled-level clearing, fresh owner borrows, skill
begin/end edges, residual command consumption, and missing-owner rejection.
They do not prove live interaction/skill gameplay or online packet delivery.

## Existing retained HUD publication

Actual owner discovery found `OriginalUiSession::Impl` in
`port/android-native/app/src/main/cpp/original_ui_session.cpp` owns one
`attack_controls_v46` and one `joystick`. Its public interface supplies
`authored_action_cache_v62(AuthoredGameplayHudV1*&, lease, error)`,
`bind_hud_world_v62(transport, HudWorldOperationV62, HudControlsServicesV62,
error, actual_world)`, `hud_controls_native_event_v68`, and
`hud_controls_update_v62`. Construction and actual LoadMenu(3) publication are
`construct_hud_controls_v62` and `loadmenu3_controls_store_v62`. Retirement is
`can_retire_hud_campaign_v104` / `retire_hud_campaign_v104`; world/transport
aliases must match that actual owner. None exposes a public raw held-field borrow.

`SourceOwnerBorrow` describes borrowing these actual owners without importing
Android asset/window code or constructing another HUD. A host may wrap existing
whole event/update methods in `native_pointer` / `controls_update`. Such a whole
owner transport route replaces the lower-level `live_dispatch` route; both must
never issue the same source control update. The `AuthoredGameplayHudV1` pointer
is a geometry/action-cache borrower, not ownership of HUDControls state.

`authored_geometry.cpp` queries its real SWF shape hits. It maps faery to spell
and skills to the three authored controls, including style-dependent control
paths handled by that existing class. Root must bind its saved style and current
HUD before these queries. Original attack/joystick have no authored onRelease
command handler: send those through actual native input. Other controls can use
the original authored `release` callback route when original touch semantics are
desired; that native DoSkill route is distinct from PC Ctrl_Begin/EndSkill.

`source_level_input` uses actual Application CurrentLevel and its same leased
`MenuLevelBorrowV58::byte198`. Do not substitute a loaded mesh or active player
boolean. `InputOwnerEpoch` is a publication receipt only. Its `before_replace`
cancels transport, delivers old release edges before replacing world actors,
advances publication only on successful delivery, and retains a failed release
for retry. Fresh SourceOwnerBorrow validation rejects old actor/epoch receipts.
This contract is tested with actual source field projections but is not yet
verified against a live retained UI restore cycle. Frontend/menu integration
must call it alongside canonical lifecycle ownership and existing quiescence.
