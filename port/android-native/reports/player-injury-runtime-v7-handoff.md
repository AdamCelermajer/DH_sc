# Player injury V7 integration candidate

`port/level-world/player_injury_runtime_v7.hpp/.cpp` is the narrow renderer adapter. It borrows the existing CharacterStateOwner, CharacterPlayerSkillsV6, PropertyView backing arrays, AnimationTables, TargetState48, Character sneaking byte415, and one retained PlayerInjuryFieldsV7 per real actor. It creates no FSM, Save, Gear, timer, RNG or target authority.

Renderer integration:

1. Retain PlayerInjuryFieldsV7 once: constructor14fc=-1.f and1448=1. Use its actual1448 pointer for HitPlayerTailBorrowV7 too.
2. Construct PlayerInjuryRuntimeV7 after the same V6/FSM/target owners are stable. Supply the existing outer FSM dispatcher and actual WorldDebug, target query, Gear GetAnimStance, animator SM_SetAnim, and controller CmdLookAt callbacks. `valid()` checks actor identity and all six property backing pointers.
3. In the outer FSM dispatcher, route `body(machine,request)` first for state11. Return provider success only for result1; propagate negative failures; leave result0 to the existing handler. State11 focus3c33e8, blur3c4ba4 and event3c0044 are implemented. Update3c0040 is an original empty body.
4. In actual World application services, route `application(request,response)`. It handles only source SkillApplyInjure11 and CancelSneaking for this actual actor. Convert result1 to provider return0, propagate errors and retain other providers. Injury calls `injure(request.attacker,request.flags!=0)`: actual event50010 or direct transition11 as requested by the original flags.
5. Wire `is_idle(identity,out)` to DotPlayerReactionServicesV7. It uses the recovered SM_IsIdle(false), including source states13/18, rather than selecting just state3.
6. Call `tick(actual Application::GetDt())` once in the actual Character update. It subtracts unsigned dt converted to float only while14fc>0; it preserves negative overshoot.
7. Positive source timer34 can call character_world_self_dot_v7 with the SAME WorldSkillExecutionV6, shared CF, real player identity and native DoT services. Application must use the V7 player continuation, not invoke the frozen nonplayer-only ApplyV6 wrapper.

Source proof: SM_SetInjureState3c5d84 checks14fc, sets3000 before table resolution, selects character animation scalar field14, obtains AnimStancedAnim from SL__LIST_IPHONE, and applies actual Gear stance when mask1000 is present. CSInjured focus sets flags2b41 before SM_SetAnim(-1) and CancelSneaking. Blur calls CmdLookAt(Character+408), including null. Character+408 is original inline CharAI+3c8 plus current target+40: C1 creation3aa1f4/3aa210 and AI_SetTarget3d6890/AI_GetTargetAsCharacter3d5454. It is distinct from raw HUD target14a4.

Verification:

- Actual original ARM versus optimized ARM64 injury/gate/body differential:282 cases,504 ordered callbacks,0 mismatches. Receipt player-injure-state-v7-original.json.
- New same-FSM composition test player_injury_same_fsm_v7.cpp passes ASan/UBSan: event50010 enters actual registered state11, source focus writes flags/animation, explicit supplied animation-end22 returns to Idle3 and blur forwards the actual target. Missing animator preserves reached state/gate/flags and reports failure. Other-state and animator/controller providers are named fixtures; this does not claim live renderer casting or animation.
- Actual cache/World/Property/HitFor/Debug/Trophy/Aggro composed positive self-DoT host receipt refreshed:458 checks, ASan/UBSan/LeakSanitizer pass. Native HP Add(-256), actual self identities, shared CF and no direct-damage RNG consumption proven. This earlier fixture deliberately stops at missing injury backend; the new same-FSM fixture separately verifies that backend composition.
- Current adapter passes strict ARM64 syntax compilation with -Wall -Wextra -Werror.

Remaining required services: existing V6 native_cancel_sneaking/native_delete_buff refuse a failed diagnostic owner. The adapter preserves this explicit failure; a diagnostic-independent successor is required if prior timer failure must not suppress the reached cancellation. Positive DoT also requires actual scrolling combat text, source character sound table/audio and player lookup tail providers. Lethal Hit requires actual CmdKill. GOD-related Hit receiver branch remains explicit unsupported. No fake providers or synthetic animation-end events were added. Full DoT completion and live renderer hit success are not yet claimed.

Parsed AnimationTables omit the original row-ID header. Original raw row+0x3c therefore maps to Injured field14; Idle is field9.
