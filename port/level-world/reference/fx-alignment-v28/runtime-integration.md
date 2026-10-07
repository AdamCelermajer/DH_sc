# General FX owner transform correction V28

Production edits are stable for the root's coherent build. Add only
`character_fx_anchor_rotation_v28.cpp` to the level-world target; existing
source_fx_node_matrix_v4/character_mesh_fx_owner_v4 TUs remain in that target.
The new header is included by the existing mesh owner header. No class layout
changes or target-marker method changes are made by this correction.

## Confirmed defects corrected

1. FX owner TRS bypassed original VisualObject.SetRotation472874. It used Euler
   (X,Y,Z), while source and actor visuals use (Y,-X,-Z). The corrected outer TRS
   and legacy non-composite path call the existing source dh2_visual_rotation.
   Authored child quaternion tracks remain untouched: this conversion belongs
   only at the VisualObject owner boundary, not every bone/node.
2. Positive anchored rotation always borrowed raw actor16c. Original SyncIrrData
   492b3c..bb8 instead reads the actual visual CScene ROOT absolute matrix, executes
   getRotationDegrees432bbc and multiplies each float by source3c8efa35. New
   typed helper does this over the same native owner root; renderer binds actual
   initialized prince/NPC SceneBinding roots and checks registered scene identity.
   Genuine absent NPC visual retains original raw16c branch. No animation bone,
   helper child, cached target position or alternative visual scene is substituted.
3. Existing play_set warm branch reset material alpha and retained stale outer
   scale. Original _GetAnimFX494bdc calls SetScaling472708(1.0). It now resets actual
   runtime visual_scale to (1,1,1), leaves source material animation untouched, and
   allows scaleWithAnchor to apply afresh during source sync. Root's new marker
   warm branch uses the same corrected source operation independently.

## Preserved source behavior

GetTargetPosition3935dc is correctly named and remains unchanged: node180 nonNULL
and visible80 select cached184; otherwise raw160. Initial suspicion that3935dc was
raw GetPosition was disproved by its complete36-byte body; no such positional fix
was applied. Current live registry projections retain ctor-null target_node, so
FX queries use the actual position160 backing. The source floor-normal query and
discard are preserved: original Sync does not compute a slope rotation from it.

Time/event lag, authored meshes/particles, root-motion helper order, source UV and
material fade, target selection and damage have not been replaced. Root owns the
actual target-ring initializer/new marker API and GPU bindings. This packet does
not claim a live visual acceptance before root's multi-heading camera test.

## Verification

`visual-outer-original.json`: 192 bit-exact65-byte matrices from whole original
VisualObject.SetRotation472874 + original ISceneNode.getRelativeTransformation
598908 versus current compiled ARM64 outer transform (24 headings x4 tilts x2
scales). Additional192 bit-exact anchor radian outputs compare original
CMatrix4.getRotationDegrees432bbc and its float radians boundary. Original external
libm/soft-float dependencies use the same oracle model on both sides; historical
Android libm and full rendering are not claimed by this test.

Three current production TUs strict bothABI PASS6; complete current renderer
syntax bothABI PASS2. No emulator/app operations performed for FX validation.
Previously supported V6 actual resource domain remains38 of45 skill resources;
seven declared families remain required. These fixes change transforms and pool
reuse, not the family inventory. Prior actual-resource simulation/GPU receipts
remain prior receipts, not post-fix live evidence. Root is validating the real
Bash/normal swing/target-ring combination after the coherent rebuild.
