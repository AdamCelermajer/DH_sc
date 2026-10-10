# Original frontend preview adapter

The adapter consumes original pydata through existing `class_preview_definitions`.
`ModelFile` is located in the serialized schema and indexes the original
`character_models_dictionary`; no class-specific base-model filename is invented.
The actual three records select `models/prince_modular.bdae`, with four body
modules and class-authored MenuIdle/MenuOnSelect/Template animation resources.
Starting fixed-loot armor modules remain in original order. INV_UpdateSkin's
empty head uses `MC_Head__naked`, and the two Rogue daggers remain two separate
attachments at the original right/left offset anchors. Geometry, skeleton,
materials and animation sampling use CharacterVisual/EquipmentAttachmentSet.

Source recovery reused:

- `port/game-data/class_preview_setup.cpp` and its original-data tests.
- `port/android-native/app/src/main/cpp/renderer_front_visual_v87.inc`:
  temporary actors, head fallback, starting equipment, original class camera.
- `port/android-native/app/src/main/cpp/renderer_front_exports_v87.inc`:
  CLASS_SELECTION named `lol_*` clip transitions and sample-before-dt cursor.
- `.local-inputs/menu-preview-model-v180/evidence.md`: MenuCharacterSelect Show
  ARM428f38, Update428498, OnEvent4281b8 and shared prince-model contract.
- `port/level-world/character_scene.cpp`: ARM3b4f3c visual-scale constants.

`ClassPreviewScene` loads CLASS_SELECTION, samples its actual animation, exposes
the three dummy-node world matrices and Camera01 world pose. Horizontal
74.08049774169922 degrees/aspect1.5 becomes vertical FOV, with planes50/50000.
Perspective aspect retains original base-constructor0x3faaaaab(4/3), independent
of physical viewport. CAMERA_ASPECT_PROOF.md records the original ownership chain.
The separate main preview camera uses original ARM42bf68 setters:
position(0,-900,150), target(0,0,225), Z-up, FOV bits3f3579c8,
aspect bits3fd578e9, planes10/2000. The actual swamp backdrop is decoded with
the existing OriginalScene decoder. Missing staged assets report an error.

Integration: compile `creation_preview.cpp`, `class_preview_scene.cpp`,
`port/game-data/class_preview_setup.cpp`, `port/game-data/loot_tables_v2.cpp`,
and link the existing foundation_data/recovered_content/content_xml libraries.
Call both load methods on the original asset catalog; texture-bind each body's
and attachment's original_materials with the existing host binder, plus backdrop
materials. Call scene.sample(index,dtMilliseconds), preview.select(index), and
preview.update(dtSeconds). Begin the frame with scene.camera() and use
`draw_creation_preview` or the existing render queue to submit source transforms.
Class actor placement uses the dummy world directly: original Show copies dummy
position, rotation and scale after LoadMeshVisual, overriding property scale.
The helper neither begins nor clears frames; the host submits authored UI next.
The main-menu saved-slot visual must come from its saved Character/Gear owner;
these fresh starting actors do not substitute for saved inventory.

Verification 2026-10-09: LLVM MinGW C++17 `-Wall -Wextra -Werror -static` compiled
and linked the adapter/tests against existing Windows libraries. Actual original
asset run against `port/android-native/app/src/main/assets` passed:

    KnightPlayerBase | models/prince_modular.bdae | prince_menu_idle_knight.bdae | weapons 1
    RoguePlayerBase | models/prince_modular.bdae | prince_menu_idle_rogue.bdae | weapons 2
    MagePlayerBase | models/prince_modular.bdae | prince_menu_idle_mage.bdae | weapons 1
    PASS original class models, appearance, weapons, select/idle, source camera

Tests additionally verify original rows263/325/290, four loaded body meshes,
head module, distinct authored class anchors, animated selection transition
gate, camera planes, swamp geometry, invalid index/time rejection, and exact
MenuOnSelect endpoint holds at the required native idle-owner boundary.
Optimized prince MenuOnSelect resources contain
old_anchor_weapon_* targets omitted from the base skeleton; the adapter opts
into the existing original missing-target policy rather than inventing nodes.

This is asset/pose/geometry verification. No screenshot or native GPU visual
parity claim is made here. The existing OriginalScene decoder bakes static scene
geometry; camera and dummy worlds follow the sampled source animation.

Original-reference correction supersedes the earlier initial-idle workaround.
Original Show428f38 starts the scene at lol_1_idle but retains previous class=-1.
First Update428498 always enters the changed-selection branch, assigning the
selected actor MenuOnSelect with flag1 and the others MenuIdle with flag0. The
adapter now starts previous=-1 and resets all three animations on that first
selection, including initial Knight. The source destination-specific scene
branches are destination0→lol_2_to_1, destination2→lol_2_to_3, and
destination1→lol_1_to_2 when previous0, otherwise lol_3_to_2. All nine pairs and
all three initial choices passed actual-asset tests under these exact branches.
The adapter never constructs nonexistent1to3/3to1 names. Input remains unavailable
during the source scene transition. Source Show also changes camera local
position to(0,0,-200) before animator construction; the adapter now does that.

`scene_materials.cpp` applies selected embedded COMMON rich pass state and its
actual unlit preamble to backdrop ranges, reusing source_material_pass. This
prevents the generic renderer light from overriding actual COMMON unlit shading.
This additional .cpp must be compiled by the host target. External L1 lighting
does not yet have its genuine light/uniform owner; see ORIGINAL_PARITY_AUDIT.md.
`class_light_source.cpp` is an additional target source: exposes original first
Omni01 receiver metadata and exact Show setter bits/conversion without creating
or binding a light. SOURCE_IDLE_AND_LIGHT_OWNERS.md records actual source APIs
and missing same-owner services; light_inputs()/required_light_owner() keep this
boundary explicit rather than representing the metadata as a renderer provider.

Post-showcase correction: removed the unverified automatic MenuIdle fallback.
The adapter samples and holds the full original OnSelect final pose/clock/socket
at5000ms(Knight/Rogue) or5366ms(Mage). idle_transition_required()/status expose
the missing CSAnim34→SM_SetIdleState source service; an optional provider can
deliver that genuine transition. Absent service does not select a stand-in idle.
Failure retains the pending boundary and reports provider error. Tests compare
every endpoint vertex against direct original CharacterVisual end sampling and
verify held clock/socket/geometry across subsequent updates, as well as recording
provider failure/delivery and clearing the boundary on source selection change.
