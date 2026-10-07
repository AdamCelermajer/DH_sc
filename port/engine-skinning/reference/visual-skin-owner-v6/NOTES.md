# Visual resource owner V6

This batch replaces V5's delegated modular/weapon construction with owned native
Scene, Mesh, Material and Skin resources. It leaves V2/V3/V4/V5 unchanged. No APK,
device, original GPU buffer-layout or whole-engine rendering parity is claimed.

## Evidence and executed scope

The original ELF SHA-256 is
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
The versioned `factories`, `construction`, `deeper` and `wrappers` captures bind
each function's address, size and instruction bytes to that ELF.

The executed original/O2 ARM64 selection proof has 766 cases and 1,404 ordered
service requests. It executes category lookup `0x647538`, ordered module lookup
`0x6474b8`, category replacement `0x648fd0`, and weapon replacement `0x473cd8`.
Queries cover all 172 real module URIs. Replacement cases cover same ID, missing
resource with retained ID, null factory results, successful factories, source
flags, both weapon cells, null names, three anchor modes and callbacks changing
the selected resource while the factory runs. Native callback identities exceed
32 bits. STL strings, imported string routines, resource allocation/refcounts,
GPU packing and scene attachment are explicit instruction-probe services.

The actual three-class original composition executes Character::Skin
`0x3a999c`, original inventory/equipment routines and VisualObject wrappers
`0x470e5c`, `0x474568`, `0x470e18`, `0x473cd8`. It produces six cases, 112
inventory operations and 1,168 visual requests, including original starter loot
165/174/213, both equipment sets, equip, unequip and swaps. The native sanitizer
replay uses the SAME authoritative V4 inventory, genuine V5 property/class/vitals
effects and real V6 resource construction. This is not a deep original factory
execution claim: native resource construction is independently checked against
actual serialized data and the existing native decoders.

Static source evidence additionally captures modular constructors `0x649120`
and `0x64928c`, controller factory `0x61ace8`, URI controller factory `0x61aa00`,
modular factory `0x60e6f0`, skin factory `0x60f924`, scene factories
`0x61bbd4`/`0x61babc`/`0x61b9e8`, node factory `0x61b2f4`, addChild `0x598864`,
packed buffer rebuilding `0x6483c8`, and Visibility notification `0x5890a8`.
Those captures establish construction inputs and boundaries; they were not all
executed end to end in this batch.

## Actual resources and source selection

The staged Prince BRES is 3,207,072 bytes, SHA-256
`7e998e60bdfcbc9237c9867bd8de146f6d59bc9f8c9c028cd4d2391f460f0b3e`.
Its `prince_modular-node` has original instance tag 13, four ordered categories
MC_Feet, MC_Hands, MC_Head, MC_Torso, each with 43 controller modules. The source
defaults are the category's `__naked` URIs at index 0. They are distinct from the
renderer preview's chosen default-warrior modules. `getModuleId` searches ALL
categories in order and returns the first category-local index. The wrapper's
category argument is ignored. It forms `#<name>-mesh-skin` literally.

`setCategoryModule` skips rebuilding on same ID. A nonnull old resource is
cleared before release; a null old resource leaves the original stale ID until
a successful replacement. A successful factory resource is retained, then any
intervening selected resource is released using its fresh value. Buffer mode is
`(flags ^ 1) & 1`. VisualObject SetModular still notifies Visibility after an
unchanged valid selection. Source notification marks SceneManager dirty.

Weapon construction uses
`data/3d/characters/prince/weapons/<name>.bdae`, BEFORE removal of the old
weapon. Slot 1 selects the first cell; all other source slot values select the
second. Anchor modes 0/1/2 are respectively `anchor_shield_left_offset`,
`anchor_weapon_right_offset`, `anchor_weapon_left_offset`. Null replacement
still searches and invokes addChild with null when the parent exists. Source
addChild retains the child and links its existing transform without resetting
it. Native rigid draw transforms are live anchor world multiplied by the
weapon-instance world matrix.

The 781 weapons are extracted read-only from the authorized cache ZIP SHA-256
`3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679`.
`weapon-manifest.json` binds every exact ZIP member and extracted byte hash.
All have zero controllers and genuine geometry instances: 1,301 total parts.
The host tests decode ALL 172 modules and ALL 781 weapons. They sample 25,200
module vertices and observe 729 changed vertices when an actual authored walk
clip advances the same live joint Scene. These checks establish backing and
pose connection; they do not compare the whole animated pose to original GPU
output.

## Renderer and lifetime contract

1. Load complete Prince BRES bytes into `VisualSkinResourcesV6`. Its immutable
   Borrow retains source bytes, decoded factory Scene and all module data.
2. Construct `VisualSkinOwnerV6(borrow, animation_instance.scene(), files)` AFTER
   that animation instance has a stable final address. The SAME Scene object
   must remain alive and unmoved. Its graph IDs/SIDs/parents must remain intact;
   animation may mutate its transforms. Initialization selects source defaults
   in category order and builds the retained selected-part graph once.
3. Pass `owner.identity()` to the V5 effect projection and
   `owner.gear_services(genuine_debug_services)` to `update_skin`. Debug Load
   and GetSwitch remain required real services. No fixture acceptance is built
   into the owner. Asset read distinguishes actual absence from provider failure.
   The host basename/casefold file provider is explicit test plumbing, not a
   recreation of the original global resource manager.
4. After animation updates that Scene, request `draw_parts`. Each part retains
   all geometry/material backing. Skinned positions are world-space CPU output
   from the existing native palette/point kernels, with an identity `world`.
   Rigid weapon positions remain local with the composed world matrix. Primitive
   indices, material symbol/index mapping and ALL source attribute streams are
   preserved. **Normals in geometry are original attribute data; this API does
   not claim they have been skinned.** A caller needing animated normals must
   use the corresponding category/module Skin and genuine normal transform.
5. A draw snapshot remains valid after module/weapon replacement and after owner
   or resource-loader destruction. Reusing snapshot positions after a later
   pose intentionally renders that snapshot. Request a fresh snapshot per frame.

The owner never appends weapon nodes to the animation target graph. It retains
their genuine decoded scene independently and attaches draw resources to live
source anchors, preserving stable animation bindings. Future unsupported
controller weapons, external controller/material references, extra modular
arrays and incompatible graph changes fail explicitly. Source failure prefixes
are retained; provider failure after some source effects does not roll back
earlier effects.

Native `update_buffers` constructs the actual ordered retained part graph. It
does not reproduce the original packed merged GPU vertex/index buffers,
Technique/effect factories or texture uploads. Rendering those parts connects
to the existing renderer's real material/texture services; it must not be
presented as a completed original GPU ABI backend.

## Central integration mapping

Add exactly these production TUs to `dh2_engine_skinning`:

```
visual_skin_selection_v6.cpp
visual_skin_owner_v6.cpp
```

Existing `dh2_engine_skinning -> dh2_engine_animation -> dh2_scene_materials`
dependencies supply mesh/resources/scene/math symbols. The V5 header describes
the borrowed callback ABI; these two TUs introduce no calls requiring a new
Data/World/UI dependency, avoiding a circular Data/World link.

Host targets:

```
visual_skin_selection_v6_audit : tests/visual_skin_selection_v6.cpp
visual_skin_owner_v6_audit     : tests/visual_skin_owner_v6.cpp
visual_skin_inventory_v6_audit : tests/visual_skin_inventory_v6.cpp
```

The first two link `dh2_engine_skinning`; the resource audit also links
`dh2_engine_animation` (already public). The inventory audit links
`dh2_engine_skinning`, `dh2_game_data`, `dh2_level_world`, `dh2_engine_ui`,
`dh2_script_runtime`. Central targets already contain the six V5 TUs, so DO NOT
add them again to the inventory executable. The standalone tool deliberately
compiles V5 into that executable because its historical private dependency
snapshot predates V5 central integration.

Exact fixture arguments and isolated compile commands are in
`reports/visual-skin-owner-v6-host-audit-v1.json` under `commands`. Run from repo
root. The isolated rebuild command is:

```
python port/engine-skinning/tools/audit_visual_skin_owner_v6_host.py
```

That receipt binds the exact copied seven host DSOs as historical binary
dependencies, without attributing their objects to moving current source.
ASan/UBSan/LSan report zero findings for all three audits. The original/O2
selection report, original inventory gold, static captures and production
hashes are bound by `freeze-manifest.json`. Central/Android builds and visible
equipment integration remain the parent's next stage.
