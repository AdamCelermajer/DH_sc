# Item visual and audio continuation

Original ELF SHA256: `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.

`ItemObject::InitAgain` 3ec0f0 calls `ItemInstance::GetColor` 3fa710, then discards its returned word. It reads VisualObject+38 and dispatches controller virtual+1c with `(index=0, loop=false, extra=0, group=0)`. `_ZTV14AnimController` 969058, address point +8, slot+1c is `PlayClip(unsigned,bool,int,unsigned)` 474b50. This is not a material tint setter. The old compatibility operation name `visual_item_material` is misleading and must be routed to `WorldItemVisualV2::init_again_source`.

GetColor counts the actual ItemInstance power vector. Counts0..4 resolve GameDesign `ItemPowerColor` keys `zero`, `one`, `two`, `three`, `four`; other counts choose palette row2. It then reads fonts palette row stride12 textcolor+8. `item_color_lookup_v2` borrows the actual design and existing font palette callback; it does not manufacture colors.

`WorldItemVisualV2` retains the same canonical Item lease and composes the existing qualified GameObject initializer, actual resource visual registry, asset setter, BRES scene, bounds, light/node access, timeline and source release. Its initializer preserves all remaining required world callbacks. Item Spawn InitPost remains its genuine bx-lr; the pool's later InitOnce invokes qualified GameObject initialization.

`world_item_drop_sound_v2` routes the exact cached position+1a8 to existing whole VoxPlay3DOwnerV2, source bool false, integer1 and both floats−1. A caller must borrow the real Vox manager and actual cached position. Constructor-only Level phase0 legitimately takes Vox's original phase gate; this is not evidence that the initialized-level sound branch is bound.

Actual cache itemdrops.bdae is225424 bytes SHA256 `c4d783a23a158b4e52687d8ed718c584ee5ad90a325e0111c8da95ae86937610`. It was extracted uniquely from original cache zip entry `com.gameloft.android.GAND.GloftD2SS/files/data/3d/gameobjects/itemdrops.bdae`. New resource test traverses every actual audiovisual table xref and fallback; SceneManager/PF/condition transports are declared fixtures. Android execution is pending root runner.

New translation units: `world_item_visual_v2.cpp`, `base_index_animation_controller_v2.cpp`, `item_color_lookup_v2.cpp`, `world_item_drop_sound_v2.cpp`. Strict ARM64 compilation passes. They are not yet in root CMake. Source inspection utility: `.local-inputs/audit_item_controller_v2.py`. Required production services remain enable callbacks/ConditionData, PF membership/lifecycle, actual SceneManager registry, world pose/update, tooltip/interaction and initialized-level Vox endpoints. Full lethal CtrlKill/loot/XP/quest completion is not claimed.
