// CApplyValueEx<glitch::core::vector3d<float>, CSceneNodePositionComponentMixin<CSceneNodePositionYEx<float>, 1, float>>,
// declared for Track B batch e1-glitch-scene-collada-gui-glitch_collada_animation_track_CApplyValueEx_gli-0 (the
// position Y component float entry, batch index 76 in reports/decomp-reconstruction/trackb-batches/e1.json; the
// batch id is shared with the other entries of this batch family, so this entry has its own ledger part and file).
//
// No header in this category declares this instantiation (the applicator and its mixin are only named in
// comments and forward declarations), so the static member is declared as a free function that takes the static
// member's arguments (no this pointer), as the sister PositionMixinVec3f and ScaleYVec3f headers do.
#pragma once

namespace glitch::collada::animation_track {

class CApplicatorInfo;

// 0x6293a4 CApplyValueEx<vector3d<float>, CSceneNodePositionComponentMixin<CSceneNodePositionYEx<float>, 1, float>>::applyBlendedValueEx(void*, float*, int, void*, CApplicatorInfo*)
// Static member: no this argument. Blends count vec3 values with the weights, then calls vtable slot +0xA4 of
// target with a pointer to the blended vector; see the source file for what is and is not reconstructed.
int applyBlendedValueExPositionYVec3f(void* values, float* weights, int count, void* target, CApplicatorInfo* info);

}  // namespace glitch::collada::animation_track
