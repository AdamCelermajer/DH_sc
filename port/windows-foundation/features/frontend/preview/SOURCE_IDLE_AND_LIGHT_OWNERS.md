# Required original idle and class-light owners

## Idle completion

Original CSAnim::OnEvent3c1a14 receives event34(decimal,0x22). With the selected
actor's flag at Character+1344 set, it calls SM_SetIdleState3c1a00(false), which
enters state3 through the genuine CharStateMachine transition. The selected
MenuOnSelect state was installed by Update428498 via SM_SetAnimState3c1aa8 with
flag1. The ordinary idle is derived from actual equipment/stance and state facts;
it is not equivalent to selecting the preview MenuIdle alias.

Reusable existing native owner APIs are CharacterStateOwner/NativeFsm24 and
CharacterAnimationInstance (or same-scene animator). CharacterStateOwner::event
delivers the current OnEvent before registered-event handling and allows the
same current owner to change synchronously. Its source method delivery must
reach CSAnim14 OnEvent3c1a14. StateOwnerBehavior and StateOwnerExtensions currently
reconstruct states3/4/5/12/17 and original empty methods; they leave nonempty
CSAnim14 behavior as a required remaining-method provider. They are not a
complete frontend Character factory/FSM/ordinary-idle service.

Coordinated animation_review and character_state: neither exposes a complete
same class-character nativeFSM with the ordinary idle/stance owner. A new
CharacterStateOwner, direct state.current write, arbitrary idle clip, or fixture
callback would create a substitute owner. None was introduced.

Preview retains exact source endpose and exposes required_idle_owner(),
idle_transition_required() and status. Optional set_idle_transition_provider must
borrow the actual same class Character/nativeFSM/animator and deliver the source
event/idle transition. The endpoint transport tests are recording fixtures only.
No successful native idle provider has been claimed or installed.

## Class-select LightPoint

Original MenuCharacterSelect::Show428f38:

1. Retains the first real SceneManager getSceneNodeFromType('lght') receiver.
2. Spawns actual ObjectManager LightPoint named charselectLight, with both source
   boolean arguments true; verifies source object type19.
3. LightBase::setLightNode40b498 retains that same scene light node and borrows
   its actual CLight. It copies existing source light fields, not a new light.
4. LightPoint::AssignTweaker40b828(0,0) publishes that same CLight through
   LightSetManager::SetLight40d368, set0(PlayerLight), slot0. It also captures
   application tweaker fields from the same LightPoint at this prefix.
5. Appends that same LightPoint to the SceneManager automatic-light object list.
6. Sets attenuation and ambient/diffuse/specular on that same CLight. The source
   light already retained by the LightSet sees these subsequent mutations.

Exact original ARM Show constants:

| Setter component | Source bits | Conversion by SetAttenuation40b3c8 |
| --- | --- | --- |
| constant | 3ec00000 | retained |
| linear | 3f030004 | divide1000 |
| quadratic | 3f86002a | divide1000000 |

The linear/quadratic source floats are not rounded convenient fractions. Color
setters write float1 RGB directly and alpha1; they do not divide these calls by
255. Scene ambient is explicitly(0,0,0,1). The imported first light in the actual
CLASS_SELECTION asset is Omni01, source point-light type1, at its actual graph
world position. class_light_source.cpp reads those exact authored node fields and
setter inputs, exposes them as metadata, and deliberately does not allocate a
CLight/LightPoint, choose an actor light set, or upload uniforms.

VisualObject::ApplyLightSet471214 calls SceneManager::UpdateLightSet3549a0 with
the actual VisualObject lightset/filter/root. UpdateLightSet traverses actual
mesh/skinned/modular receivers and actual material cells. ApplySettings40c9a8
compacts enabled nonnull light slots with actual shader parameters into light0
etc.; disabled slots bind actual dummy-off cells. VisualObject::ApplyMaterial470cd4
is an original empty body. This does not authorize synthetic material tinting.

Original shader/uniform path must retain the exact selected material/preamble,
actual CLight pointer and program history. commitLightParameter5b187c uploads
light semantics only for nonnull CLight; null makes no write and retains prior
program values. L1 shader uses unattenuated ambient plus attenuated Lambert
diffuse multiplied by vertexRGB. These differ from COMMON lighting/unlit paths.
The recovered light inputs alone cannot prove the selected material was bound
to that light or reproduce program-uniform history.

An existing menu_class_select_light_owner_v1 outside this lane declares the
required retained owner/service interfaces. It was still an unfinished Show
draft when inspected; this lane neither edits it nor admits it as a completed
backend. Preview has decoded asset Scene geometry, not actual source
SceneManager/ObjectManager/LightPoint/material receivers. required_light_owner()
therefore exposes the exact missing same-owner route. Root must bind genuine
services and material uniform owners before original-lighting acceptance.

Verification: strict native C++17 compile and actual-original-asset tests pass
for Omni01 presence, source set/slot inputs, exact attenuation unit conversions,
source color/scene-ambient inputs, full showcase endpoints and previous camera/
class routing checks. No renderer tint, ground extension, ordinary idle
substitution or claim of complete native FSM/light-owner integration was added.
