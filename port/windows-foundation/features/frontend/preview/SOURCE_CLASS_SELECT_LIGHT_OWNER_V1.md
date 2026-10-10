# Class-select authored scene light route

`MenuCharacterSelect::Show` at `0x428f38` finds the first authored scene node
of type `'lght'`. If that lookup succeeds, it calls
`ObjectManager::Spawn("LightPoint", "charselectLight", true, true)`, verifies
ObjectBase type 19, binds that same scene-light node, calls
`LightPoint::AssignTweaker(0,0)`, appends the spawned object to the same
SceneManager automatic-light list, then calls `SetAttenuation(0.375,0.51172,
1.0469)`, `SetAmbientColor(1,1,1)`, `SetDiffuseColor(1,1,1)`, and
`SetSpecularColor(1,1,1)` in that order. A missing authored node skips the
whole branch. The detailed source setter semantics are recorded in
[`SOURCE_LIGHT_POINT_V1.md`](SOURCE_LIGHT_POINT_V1.md).

`MenuClassSelectLightOwnerV1` is the source-ordered lifecycle coordinator for
this branch. It validates each boundary against the same integration owner,
retains the original scene-node and spawned-object leases through the reached
prefix, and makes failures non-replayable. Its clear/flush acknowledgements
require SceneManager clear before ObjectManager flush; the owner performs no
substitute destruction.

`tests/menu_class_select_light_native_v1_contract.cpp` exercises the full
successful sequence and the early skip/failure branches. It binds the same
`NativeSceneLightNodeV113` and `NativeLightV113` into the actual
`NativeLightSetV113` slot (0,0), checks the source attenuation unit conversion
and RGB/alpha fields on that native CLight, and verifies ordering and retained
identity. Run it with
`tests/run_menu_class_select_light_native_v1_contract.ps1`.

This contract test validates the source coordinator and the real native
scene-light/CLight/light-set owners. It is not a production ObjectManager or
Application binding: the test's spawned LightPoint identity, app-tweaker
lease, and automatic-light vector are explicit boundary fixtures. The live
menu still needs providers that call the active ObjectManager Spawn/Flush,
`LightPoint::AssignTweaker` including the actual Application tweaker fields,
SceneManager automatic-light append, and the source LightBase setter entry
points. No CLight shader binding or rendered-light readiness is claimed.
