# Movement state policy and speed producers

Evidence is captured from original ELF SHA256 `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`. The checked-in manifest and assembly retain Character policy/property getters, CSMove focus/blur/update/type selection, and CharAnimator selection/speed/step routines. Discovery copies and GOT identification are under `.local-inputs/move-state-discovery`.

CSMove::OnFocus `0x3c3bf8` first performs diagnostic logging. Its actual state prefix at `0x3c3c60` writes Character+0x520=`0x23c1` and Character+0x53c=0. It then calls UpdateType, then unpins the physical object if present. OnBlur logs, executes complete GameObject::Stop, then pins if present. Consequently activation selects/configures animation before mass restoration and wake; deactivation stops before converting the body to static mass.

Original policy getters decode Character+0x520 as follows:

| Getter | Bit | Move focus result |
|---|---:|---:|
| Position from visual | 0 | 1 |
| Position from physics | 1 | 0 |
| Rotation from visual | 2 | 0 |
| Rotation from physics | 3 | 0 |
| Visual with game rotation | inverted 4 | 1 |
| Updating path | 7 | 1 |
| Validating floor position | inverted 6 | 0 |

Character::GetRotationSpeed returns -1 when flag0x20 is present; otherwise it uses the rotation property multiplier. Move focus does not set0x20. Camera validation separately calls virtual IsPlayer and is not a flag decoder here.

The native resolved 224-word sheet omits the original Structs::CharacterProperties vtable. Original resolved object begins at Character+0xff4, but payload starts+0xff8. CharProperties begins at Character+0x560. PROPS_GetWalkSpeed reads CharProperties+0xb50 = Character+0x10b0; its native payload index is `(0x10b0-0xff8)/4=46`, named Speed_Modifier_Walk. Rotation reads+0xb54, index47 Speed_Modifier_Rotation. Existing properties/combat instruction fixtures explicitly write four header bytes before their 224-word payload, confirming the offset.

Both getters execute float conversion of signed raw int32, multiply by float1/256, multiply by float.01, add float1, then return that value only if it compares greater than zero; otherwise return positive zero. Each intermediate operation rounds to float. These are percentage modifiers of animation/rotation speed, not root displacement in game units per second. Raw0 gives multiplier1; raw-25600 gives0; raw25600 gives2.

CharAnimator::ANIM_SetSpeed `0x3c93fc` stores the requested multiplier at animator+0x40. If a visual exists, it multiplies that value by animator+0x34 and invokes visual displacement controller vslot0x28. CharAnimator::_SetAnimStep `0x3ca79c` reads the selected 56-byte runtime step's Speed at+0x30 and writes animator+0x34 at`0x3ca924`. Initial PlayClip setup subsequently applies the same product. ANIM_Set normally resets animator+0x40 to1 before selecting; when animator+0x49 defers selection, it instead stores the pending animation ID at+0x50. Move UpdateType invokes ANIM_Set first, then ANIM_SetSpeed(PROPS_GetWalkSpeed), so eventual clip-time speed is property multiplier times authored step Speed.

The original authored PlayerKnight/Mage/Rogue character animation rows48/49/50 select base Walk280 Human_Walk_00_1H (clip1126) and Run271 Human_Run_00_1H (clip1114). Both have Loop=-1, MoveGO=1 and Speed=float1.3. Thus an unmodified resolved walk property gives actual clip-time multiplier1.3. These are base IDs; original debug/stance policy can add GetAnimStance to the base animation ID. Do not hard-code these clips for every equipment stance.

UpdateType calls virtual IsPlayer at slot0x28. For players it computes squared3D length of Character+0x1b8 heading. DesignSettingsTable::members GOT global is`0x9a6498`; first runtime row+0x5c supplies the walk threshold and+0x60 the run threshold. Existing movement type+0x53c=2 stays run until squared heading is strictly below squared walk threshold. A non-run state enters run only when squared heading strictly exceeds squared run threshold. Initial type0 otherwise selects walk1; unchanged walk1 returns. These are hysteresis comparisons, and threshold values must come from actual DesignSettings data. Nonplayers select walk only when type0. Base Walk/Run IDs come from CharAnimTable row stride0xa0 at+0x94/+0x70, respectively. GetCharAnimTableId reads Character+0x1000 (resolved payload index2 AnimTable), validates against the table count and falls back to17.

OnUpdate first checks Character+0x1b5. If zero it raises event0x3f and returns. Otherwise its source branches determine whether to call UpdateType. After that it obtains the walk multiplier again, compares absolute change with float0.0001, and updates cached Character+0x52c and ANIM_SetSpeed only when change is not below that threshold. The preserved squared threshold and exact float rounding matter around boundaries.

The new `move_state.hpp/.cpp` is bounded to pure policy, speed/rotation getters and the exact focus flag/type prefix. UpdateType does not itself write+0x520, but animation selection raises Character events0x24 and0x26 through CharAI/FSM; any policy changes from other event handlers or transitions remain caller-owned. The helper does not claim to implement full focus, animation selection, state transitions, unpin, loop scheduling or property-sheet production.

Original-vs-ARM64 comparison checks4,352 cases including all low-byte policy combinations and4,096 seeded signed-property/flag cases:30,464 actual policy getter calls, actual property getters, actual ANIM_SetSpeed's visual controller argument and exact OnFocus prefix instructions. Eleven malformed/overlapping caller contracts reject atomically. The original-derived68-byte record corpus SHA256 is `f19ae53552f898d017e816b808f1c20a86598b8a900bc503068153e3a7b37a8c`. ASan/UBSan host replay matches all4,352 records exactly. Position/path/rotation producer bits can therefore feed the existing SubobjectsRequest while clip-time factor feeds the separately reconstructed authored animation timing/root-motion pipeline.
