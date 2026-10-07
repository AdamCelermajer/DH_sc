# Source held attack controls V46

Production TU: engine-ui `hud_attack_control_v46.cpp` plus header.
Staged renderer supplement: `renderer_hud_attack_actor_v46.inc`, included after
the actual PlayerSkillsRuntime/attack owner. Root exports its two functions in
model_renderer.hpp, including `hud_attack_control_v46.hpp`.
No source owner, HP, target or controller is cloned. The include defines private
`source_hud_*` helpers. Public root wrappers should be named
`model_renderer::player_hud_attack_actor_v46` and
`model_renderer::player_hud_attack_command_v46`, delegating to those helpers.

## Immediately usable modern authored UI adapter

Whole initCachedChars/cache8 is not currently produced. Do not set that byte.
Root may call the exact `hud_attack_dispatch_v46` branch under the existing
actual `prepare_player_frame`/gameplayActivated/live-active/dead and original
shape-hit pointer preparation. This is source held-dispatch within the modern
UI adapter, not acceptance of whole HUDControls.Update. Obtain actual actor
via `player_hud_attack_actor_v46(0,false,...)`, then invoke dispatch once in that
prepared frame. The command refreshes the actual existing controller gates.
No fake cache8/Level198 or unavailable positive callback is involved.

Menu push, deactivation and loss of gameplay preparation must cancel only the
actual retained attack hold through source7 before clearing pointer ownership.
Do not replay constructor defaults or keep a hidden hold across a menu/world
replacement. No reacquisition after finger release is permitted.

## Sole HUD state and pointer transport

Embed ONE `ui::HudAttackHeldFieldsV46 attack_controls` in retained
OriginalUiSession::Impl. Its C1 values are original41af4c/78..80: held9=0,
pending7c/80=-1, consumed84=0. Do not reconstruct it each frame/cast.

In `hud_pointer` actual shape-hit down branch, for Control::attack call
`hud_attack_event_v46(attack_controls,4,consumed,error)` immediately and retain
that pointer through the existing map. Do not fire the old release command.
On its drag retain ownership; source attack event5 leaves held unchanged.
On its release call source6 regardless of whether release still shape-hits;
outside/cancel maps source7. For global cancel, clear every actual held attack
pointer via source7. Releasing another pointer (skill/faery/joystick) must not
clear held9. Invoke joystick release only if a real joystick pointer was
cancelled; the old unconditional joystick release can inject unrelated Stop.
Normal press→update→release now issues Cmd while held, matching original.

## Frame ordering and genuine providers

Call `hud_attack_update_v46(attack_controls,joystick.active,services,error)`
once per normal actual HUD update, before its joystick update. Do not call it
again from rendering or nested callbacks. The kernel performs the exact held
prefix of Update41a780 and stops before joystick/world-touch continuation:

1. Read actual source application/input byte30, store normalized controller
   global block, then clear HUD consumed84.
2. Query genuine Application.GetCurrentLevel. NULL allows input. A nonNULL
   Level must borrow SAME byte198; zero clears held9 and joystickA and returns.
   C1 initializes198=0. Do not substitute phase38, fake198=1 or the development
   renderer level for an unpublished Application/GS CurrentLevel.
3. Read actual movie658 (`SwfMovie::player_identity`, actual GameSWF root
   receiver). NULL returns. Cache8 is the original initCachedChars completion
   store41a22c. If zero, execute actual cache initialization, then reset pending
   coordinates. Do not manufacture full cache readiness from a UI screenshot.
4. Get SAME PlayerManager.GetLocalPlayer(0,false), use its character660. NULL
   Character returns. There is no character_count override.
5. While held9 is nonzero, fresh OOI14a4 chooses Cmd_UseOOI(NULL). Otherwise
   clear SAME existing413 then execute Cmd_Attack(NULL). Do not pre-skip state6
   or force a target: real controller/AI/FSM gates execute inside Cmd.

Bind `local_player` to source_hud_attack_actor_v46 and `attack` to
source_hud_attack_command_v46. Actual click413 is the already-retained
`t.world_touch->pending.click_target413`; OOI is the same canonical metadata
field. Controller is the actual attack_controller.controllable. No new cell.
Bind global-block read/store to its genuine existing authority, never a second
temporary control flag. `use_ooi` still requires whole original4057fc→
Character3ad690→UseOOI3ad614/ForceUseOOI3ad5ac: offline/network predicates and
same current-target/Idle/Moving/byte412 providers remain mandatory. Replacing
this callback with Cmd_Attack would be incorrect. Default OOI0 does not reach it.

Bash/GroundSlam authored Post ClearTarget is retained. This held-control branch
can reacquire afterward only while the actual attack pointer remains held. A
released pointer cannot cause unconditional auto-retarget. Charge retains its
own TargetInMeleeRange condition unchanged.

## Evidence and limits

Actual original ARM OnEvent attack-receiver and held-dispatch branches replay
320 contrasts; native O1/O2 ASan/UBSan pass335 checks each, including cache,
NULL/nonNULL Level, source198, PM NULL, release/cancel and required-prefix cases.
Both ABI production/test strict compilation passes. Outer services and command
delivery are explicit observers in fixtures; no live controls, multi-touch,
controller execution, positive OOI interaction or post-cast visual acceptance
is claimed before root integration and runtime verification.
