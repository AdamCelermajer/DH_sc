# Live actor wiring review

Read-only review of `port/android-native/app/src/main/cpp/model_renderer.cpp` and its borrowed native services. Latest inspected renderer SHA256: `c705b2e3489f8982267cb8674c54754696b8824704e590795398b228b07cd43d`; JNI native_app.cpp SHA256: `cd34f8c62c86cb5d1b4f34d8939b1b0e1f7c044307beb471bb2cdc1da4aac033`. Parent edits occurred during the review; the four concrete findings below are fixed in the inspected working tree. This review made no renderer, CMake, playback or production-module changes, and performed no emulator/GL runtime test.

## Failure handling verified after fixes

**Failed initialization now disables the invalid world.** The load_world catch clears native readiness, clears the actor world while owner contexts remain alive, clears prince_body, and disables enabled/world_mode before resource cleanup and its error return. The next frame cannot enter the partially initialized native world branch.

**Live service errors now stop at the JNI boundary.** NativeBridge_draw catches std::exception and unknown C++ exceptions around model_renderer::draw, logs Native frame failed, invokes deactivate and returns before the submitted-frame log. Deactivate clears the actor world before clearing prince_body and disables readiness/rendering/world mode. The new actor-phase errors occur before GL submission, so a failed actor frame cannot be reported as submitted or continue using partial actor state.

Both previously reported P1 findings are closed by source inspection. Runtime exception injection and emulator/GL execution were not performed by this review.

## Findings fixed during review

**Melee heading target was lost after the first smooth turn.** The original wiring assigned the global desired heading into rotation.heading_angle on every attacking frame, then overwrote that same global with the current smoothed angle at frame end. At speed1/dt16, a target farther than approximately.201 radians would move only one increment and then become its own next-frame target. Parent now writes the desired target once in player_attack to runtime rotation.heading_angle/controller.heading.angle and removes the per-frame attack assignment. Global heading remains the displayed current angle. This preserves the verified coordinator's persistent +0x178 target independently of current +0x174.

**GL restore retained locomotion while resetting movement policy.** Reload resets walking=false, flags0x2380, movement type0 and creates a pinned body, but the earlier restore branch skipped Idle selection and retained a Walk/Run scheduler. Parent now starts the authored Idle sequence unconditionally after resetting these facts. Keeping the prior bound identities/clock while replaying the correct Idle sequence is consistent with the current restore policy. Full FSM/attack-state restoration remains outside this locomotion adapter.

The temporary mesh bounds were replaced during review by the pending source-backed character visual-scale and owner-bounds helpers. This review does not independently certify those helpers; their source/oracle audit is separately owned.

## Source conditions verified

Move::OnFocus `0x3c3bf8` writes flags0x23c1 and movement type0 before UpdateType, then unpins. The live branch selects the authored Walk/Run sequence before unpin, after the current frame's Step. This preserves the recovered ordering within the bounded locomotion transition.

Move::OnBlur `0x3c3aa4` invokes GameObject::Stop at`0x3c3b08`, then pin at`0x3c3b18`. Stop always performs its logical path/destination/heading resets. Its physical branch additionally requires a body and virtual slot0x64 (`0x393974`), resolved to Character::IsUpdatingPositionFromPhysics `0x3a2e44`, flags bit1. Move0x23c1 has that bit clear. The live blur correctly drops/resets the logical path before pinning without forcing a physical Stop under this policy. Runtime controller policy likewise obtains update_physics from the decoded bit rather than forcing it for Move.

Idle::OnFocus `0x3c3020` conditionally exits when Character+0x538 is set. Its ordinary branch writes0x2380 at`0x3c30a8/0x3c30ac`, then selects animation. The live prototype uses that ordinary idle policy; it does not reconstruct the complete +0x538/FSM gate.

The live locomotion phase order is scene_phase -> actor_world.update -> input/FSM-prototype transition and animator_phase -> update_actor. The verified coordinator then executes snapshots -> path -> rotation -> subobjects -> target cache. One source-derived millisecond dt reaches the genuine Step and rotation policy. There is no additional dt cap/fixed-step subdivision in this live actor path. The renderer's steady-clock producer/default multipliers and gap gate are a bounded application adapter, not a reconstruction of all Application::ComputeDt fields and pause/state dispatch.

## Borrowed ownership and rendering checks

RuntimeRequest borrows static prince_runtime/prince_body/prince_visual, the current Scene and Level-owned native floors, stable path/scratch vectors, explicit registry/motion policy and the resolved224 payload. Initialization allocates/resizes the storage before publishing the state pointers; advance does not resize it. Optional avoidance and target node are absent explicitly. The fallback service handles the events emitted by this policy, including an absent camera; unsupported events report UINT_MAX.

WorldObject contexts point to permanent prince_body_owner or heap-owned BodyOwner objects held by unique_ptr. The world is cleared before those owners are removed during reconstruction; reset/deactivate also clear the world before zeroing the borrowed Prince body. Scenery BodyOwner::native borrows decor_bodies elements. The current fixed Crypt population fits the reserve84; its address stability depends on that fixed capacity until world reconstruction. Generic expanding scene support would require reserving the actual collider count before publishing pointers, or using stable element storage.

Body physics rotation remains separate from actor visual rotation, consistent with the fixed collision-body source configuration. The visual root supplies owner * helper * authored-node world matrices to skinning. Skinned draws use projection alone, while unskinned nodes use projection * node.world; the actor transform is not applied twice. This is a source inspection, not visual pixel validation.

## Explicit remaining scope

Run/Walk hysteresis .45/.85 is now source verified: reference/live-animation-selection/NOTES.md records execution of the original DesignSettings::read at0x4ee0d0, all43 field writes, and first-row offsets+0x5c/+0x60 with words0x3ee66666/0x3f59999a. The original UpdateType probe covers336 cases with zero mismatches. This supersedes the earlier unverified-threshold note. Touch-to-destination input, its moving dead zone, and the producer of the input magnitude remain development controls; verifying the setting values does not establish full original input/FSM parity.

Player attack/death playback is still advanced and sampled later in draw, after the locomotion actor phase. Existing enemy/combat scheduling, contact gameplay consequences, full FSM eligibility, application pause/input order and blend services are therefore outside the verified locomotion scene -> Step -> actor sequence. Keep those scope boundaries in the milestone description. The existing module differentials and composite host fixture do not certify full original-frame gameplay parity.
