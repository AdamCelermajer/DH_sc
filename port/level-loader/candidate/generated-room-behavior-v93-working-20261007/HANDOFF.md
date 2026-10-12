Generated Room behavior composition V93

Source-only candidate; no build, test, syntax, runtime or device job performed. Apply the INC inside model_renderer after renderer_campaign_generated_rooms_v92.inc and before renderer_source_boundary_v61.inc. Parent V93 source-world-owner-hooks-v93.patch supplies the exact generated_room_native_v92 slot; this candidate adds no alternate public interface.

Global include closure:
- renderer_campaign_generated_rooms_v92.hpp (parent public existing V92 packet)
- source_campaign_runtime_v61.hpp, model_renderer.hpp
- source_campaign_object_update_actor_v104.hpp (ACTUAL existing public Character Room borrower)
- source_campaign_character_fsm_v101.hpp, renderer_character_campaign_v62.hpp
- renderer_native_gslevel_v27.hpp
- <canonical_character_candidate_v60.hpp>, <canonical_object_manager_v1.hpp>
- <canonical_level_context_v1.hpp>, <object_enable_condition_v2.hpp>
- <native_conditions_v69.hpp>, <character_design_services.hpp>
- <application_services_owner_v5.hpp>, <application_player_manager_bootstrap_v59.hpp>
- <loot_source_fields_v47.hpp>, <player_savegame_v1.hpp>
Existing owning TUs remain linked once: source_campaign_object_update_actor_v104.cpp, source_campaign_character_fsm_v101.cpp, object_enable_condition_v2.cpp, native condition runtime/table. No CMake source owner is introduced by this header-only composition fragment.

Call install_campaign_generated_room_behavior_v93(result,error) at the end of create_renderer_source_borrow_v61 AFTER Main installs its genuine generated_room_native_v92 primitive provider and before returning World. The installer weakly captures SourceWorld and composes that existing provider lazily after actual Level exists. Alternatively Main's existing provider can call prepare_campaign_generated_room_behavior_v93(actual,source,bag,error) directly; use one route, not both. The INC owns an independent adapter state; it stores weak World/Level/manager references and retains only the supplied independent primitive authority.

Implemented available source behavior: Character c4 via Main's exact Player/Faerie method; initial and occupant ZoneEntered/ZoneExited via actual Character FSM APIs; fresh Debug.Load/GetSwitch at every Room trace boundary; Character TestEnableCondition via existing original condition kernel over SAME8a/ec/f1/a8/ac, SAME PM GetLocalPlayer(0,true), actual Save14e8/ready14, nullable actual current Level.mode118, and ONE existing NativeConditionRuntimeV69. Enabled/Disabled use actual Character native event APIs. Native class identification uses actual as_character dispatch and ordered manager record, never type-name guesses. Classification intentionally does not require a Module GameObject projection before its c4.

Genuine Main inputs still required by existing V92 bag: non-Character selected IsZonable+c4; qualified ZoneEntered/Exited preserving selected c4/3c; non-Character TestEnableCondition preserving selected44/48; ObjectManager Add/DelRoomObjects on SAME list80 and SAME occupants list address. Null-handle assertion and true Room/Zone D1/D0 remain preserved incoming callbacks. No Room destroy is replaced with generic GameObjectD2. Missing primitive provider or any mandatory non-Character/list callback rejects the packet before publication. This candidate supplies no placeholder callable to disguise a missing body.

Concrete Main Character source mismatch reported separately: current FSM ZoneEntered/Exited uses generic2ed and direct85. Original38c710/38c69c dispatches actual c4 and selected SetUpdatable3c after source2f0/visibility mutations. Main must correct that public behavior owner; this adapter intentionally consumes it instead of creating a competing Character implementation. Original Room.Activate396d24/DeActivate396918 directly invokes ZoneEntered/ZoneExited, then fresh trace and SAME Add/DelRoomObjects; occupant activation is not Enabled/Disabled.
