# AIS animation event owner

CharacterAnimationEventOwnerV1 composes CharAI::OnAnimEvent relay3d0cc4 and
AISDefault::OnAnimEvent3dca50. Selected source AIS virtual94 must be the genuine
inherited implementation3dca50; null selected AIS returns without work. Caller
removes the source ev_ prefix before delivery.

Source always invokes actual LuaScript.Call("OnAnimEvent", event, Character+4f4
integer lag), using the same ScriptOwnerV2 identity, fresh alias/global lookup,
source return projection and discard. Lag is borrowed live. Positive ordinary
Lua diagnostics are exposed by source_lua_error(); original continuation
proceeds. Required native provider failures stop and report error.

Only step_left/right enter the footstep branch. GetLeftFootPosition3a57f4 and
GetRightFootPosition3a5874 first copy genuine GetTargetPosition. Actual absent
visual or missing named node retains that source position. Existing visuals
search Bip01_L_Foot/Bip01_R_Foot under their actual scene root and copy the SAME
animated node's absolute XYZ (native matrix slots12..14).

CRootSceneNode ctor65b734 calls CSceneNode ctor with NULL serialized SNode at
65b77c/65b784. constructVisualScene61b8bc iterates every serialized root
(stride50), attaching each under this SAME container via virtual5c at61b930.
Native scene loader omits the wrapper and preserves that ordered child forest
as parent=-1 entries. visual_root UINT_MAX explicitly represents the source
container, searching its whole child forest in root-first child-order DFS.
Concrete root indices search only their subtrees. No animation ancestor or
arbitrary index0 is selected.

GetFXFootprint3a3300 selects CharacterEffects from cached1014. Cache receiver
is +ff4 but scalar array starts +ff8 after its header; thus this is SAME
resolved[7], Effects. Invalid row IDs use actual row0. After footprint Play,
source reloads this property before floor-FX gating. Existing native property
cache/recalc owns its writer; the event helper adds no parallel effects field.

Character+1d8 is cached PFFloor*, not PFObject*. Actual PFFloor ctor51d1b4 stores
its type string buffer at+3c (51d200), then writes its first byte0 (51d21c).
Supported floors::append rejects floortypes overrides, so accepted retained
records keep that empty constructor string. AnimationFloorBorrowV1 borrows the
same cached floor index and World. UINT_MAX delivers source null; valid accepted
record delivers its source constructor empty type; invalid backing fails.

Footprint Play occurs first. Source reads cached floor/type, gates actual
CharacterEffects.trigger_floor_fx, scans ordered FootstepEffects for first
strcmp match and plays its effect at the same foot position. Invalid set IDs
reproduce whole PlayAnimFXSet invalid-index exit before manager reads. Valid
IDs require actual CharacterMeshFxOwnerV1 and preserve its frozen mesh-domain
limitations. This source routine has no footstep sound branch.

Actual bundled Default row: footprint=-1, floor trigger1. Actual empty-floor
step row: effect=-1. Thus supported Crypt completes without fabricated FX.
Water275 reaches required FX and fails when its real backend is unavailable.

Original oracle137 cases execute full source relay/branch/foot getters with
named Lua/foot/FX and node-query fixtures; no full renderer/Lua parity claim.
Host O1/O2 ASAN/UBSAN checks execute actual owned Lua and bundled effects with
same world/scene and frozen dependency hashes. Both NDK ABIs compile strictly.
Renderer and frozen FX/trophy/execution sources were not changed.
