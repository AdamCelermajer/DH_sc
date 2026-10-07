# NPC Walk/Attack source body and animation consumer connection

Add `renderer_npc_state_animation_v1.inc` after the existing NPC command include
and PlayerSkillsRuntime attack geometry declarations. It adds no automatic calls.
It compiles against the complete current renderer on arm64-v8a and x86_64.

Retain `RendererNpcStateAnimationV1` on the same MonsterScriptHandle. Inputs are
the SAME optional NativeBody, SAME Session timer services (never another store),
and an optional actual selected AIS PreAttack successor. Actual NPC External
PreAttack is accepted only when selected source key is 3dbef8, the original
`AISDefault::OnPreAttack(int)` literal bx-lr. CharAI3d0ed4's preceding
AI_CanAttack(null) is executed against the actual world/geometry/inventory.

Before source StateOwner dispatch/update, call `refresh_facts()`. This reads
actual IsPlayer, COUNT_IPHONE, resolved properties2/46/47/48, actual animation
table schema names, AI fallback8/AttackDelay, same target/path/destination and
heading. Nonplayer Walk does not reach joystick thresholds or Run selection;
those fields are deliberately not manufactured. Existing death/stance-mask
facts are left with their upstream owner; this adapter owns states3/4/5 only.
The original nonplayer stance result is0 for every signed COUNT_IPHONE.

In MonsterScriptHandle::state_body, route supported source states3/4/5 to
`m.state_animation->body(state,*request)` before the old empty-body boundary.
Keep source death/reaction services with their existing owners. All body calls
refresh facts after synchronous mutations, including reentrant callbacks.

In MonsterScriptHandle::animation_service, BEFORE the restricted Idle/Injury
step branch, call `m.state_animation->ai_helper(ai,*q,out)`. Return0 to the
CharAI dispatcher when helper result1; return-1 on helper result-1; otherwise
continue the existing source relay. The new helper executes exact StepBegin
3d4204/StepEnd3d3ff8 on the same AnimationAIState96 and same actual playback.
It does not replace the observer, manufacture22 or bypass selected AIS End.
Character+408 is embedded CharAI(+3c8)+40, current target; it is NOT LastTarget.
ClearNonSticky preserves the actual source sticky field and complete
SetTarget(null,false)→SyncLastTarget. Scope borrows the real current command
capability and restores it synchronously.

Additive command facade helpers: `object_stop` is raw GameObject.Stop (does not
add Character event3f), `character_heading` is the actual heading owner, and
`move_object` runs actual existing CmdMoveObject gates and PathTo service.

Frame integration remains the separately verified `npc_actor_frame_v1` after
Scene/FSM/Animator in original path→rotation→subobjects→target-cache order.
Supply actual target-node absolute position, retained base auxiliary2e0 (source
ctor38c2d0 stores0), registry/motion policy/workspace/body and real bounds
mutation service. Publish reached position back into the same actor/session.
This is an actor phase, not a claim of whole outer profiling/network/audio.

Constructor integration required once on the actual retained actor:
Character C1 3a9594 stores signed byte14a8=-1;3a967c stores word14a4=0.
Do not reset them per frame, or initialize unwritten AI74/78/79/7a fields.

Remaining production endpoint: named attack_mainhand/offhand still needs the
whole same-NPC OnAttack/native MeleeCalculate/Apply receiver, including actual
NPC ItemInventory ownership. The current constructor-only query projection
cannot be passed to APIs requiring FreshInventoryOwnedV4, and player Gear is
not a replacement. Unknown PreAttack source overrides, timer capacity growth,
nonbase visual/auxiliary and outer online/audio branches remain explicit.

Verification: both-ABI complete-renderer syntax PASS; existing isolated Android
attack/frame test PASS with real Crypt BRES+DWLD/PF and explicitly named table,
geometry and body fixtures. That executable test does NOT exercise this new
renderer include, actual authored attack damage, or rendered NPC locomotion.
