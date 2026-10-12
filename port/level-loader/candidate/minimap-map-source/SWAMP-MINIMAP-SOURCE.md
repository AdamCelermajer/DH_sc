# SWAMP minimap source boundary

Source-data inspection only, with no native parser/runtime execution, compiler/tests/builds, hashes or freezes. Shared Root remains untouched. Original cache scope is exactly the nine Module and ten Decor declarations retained in reports/swamp-fixed-declarations.json. The literal source read uses only their two original BRES assets and the retained SNode schema (id/name, visibility, attached static/controller mesh count, child array), selected by authentic xrefobject + "-node". Mesh own-name is the retained native C1 empty-name domain; the captured first SearchByType name lookup uses parent fallback. No other maps/resources were sampled.

| Authored receiver | Source asset | Mesh children, including hidden | Floor-name children | Minimap-name parents |
|---|---|---:|---:|---:|
| obj_4of4_brdwalk_sw_00_0 | swamp.bdae | 52 | 2 | 0 |
| obj_3of4_brdwalk_sw_00_1 | swamp.bdae | 52 | 1 | 0 |
| obj_1of4_brdwalk_nse_00_2 | swamp.bdae | 44 | 1 | 0 |
| corner_ruin_ws_00_3 | swamp.bdae | 59 | 1 | 0 |
| merchantcamp_ruins_swe_00_4 | swamp.bdae | 59 | 2 | 0 |
| corner_brdwalk_se_00_5 | swamp.bdae | 49 | 1 | 0 |
| deadend_brdwalk_w_00_7 | swamp.bdae | 42 | 2 | 0 |
| bossroom_ruins_ns__8 | swamp.bdae | 9 | 4 | 0 |
| obj_2of4_brdwalk_sw_00_9 | swamp.bdae | 55 | 2 | 0 |
| _prop_corpse_01_001 (two rows) | prop_swamp_corpses.bdae | 1 each | 0 | 0 |
| _prop_corpse_02_001 (three rows) | prop_swamp_corpses.bdae | 1 each | 0 | 0 |
| _prop_small_bloodstain_001 (two rows) | prop_swamp_corpses.bdae | 1 each | 0 | 0 |
| _prop_small_bloodstain_02 | prop_swamp_corpses.bdae | 1 | 0 | 0 |
| _prop_corpses_small_pile_001 | prop_swamp_corpses.bdae | 1 | 0 | 0 |
| _prop_corpses_pile_001 | prop_swamp_corpses.bdae | 2 | 0 | 0 |

Exact cache resources under com.gameloft.android.GAND.GloftD2SS/files/: data/3d/modules/swamp/swamp.bdae, 5,585,024 source bytes / 976 total authored asset SNodes; data/3d/props/swamp/prop_swamp_corpses.bdae, 162,552 source bytes / 41 total authored asset SNodes. Selected totals are 421 Module mesh children / 16 floor-name children, and 11 mesh children in the ten Decor rows / zero floor-name children. No selected minimap match was found. This establishes authored branch coverage for these declarations, not a successful native SearchByType or SWAMP load/runtime.

## Corrected reached source order

Captured reference/loader-functions/00523c14.asm stores the last minimap match during first SearchByType (523d2c/523d98), then iterates the same search snapshot. Each floor reaches PFRoom::_LoadFloor521eb8 through523dfc BEFORE the captured minimap test at523e00. A selected minimap is passed to actual SceneManager.AddNodeToMap3524a0 at523e24. Without a selected minimap,523eb4..523ed4 lends the last floor record's newly copied mesh. _LoadFloor's own first clone AddNodeToMap is retained in both cases. When no floor was reached,523edc..5240c8 pops/destructs the empty room and returns nativeNULL; the mere presence of a minimap does not invoke a transfer.

The single CPP correction moves the explicit unsupported minimap boundary after completed real floor load/record/clone and first clone map add. It preserves the no-minimap last-clone second add. No positive minimap success is fabricated. The matching HPP is included for coherent adoption but unchanged. Private selectedG's active CPP receives this source correction; Root may apply minimap-source-order.patch.

## Why current storage cannot lend the positive minimap child

SceneManagerMapOwnerV2::add requires an actual child lifetime, native identity, parentec, flags11c, cached_position, set_position, previous-parent detach, OptimizeStatic, parent visibility and source scene phase. It performs cached-position -> setPosition -> detach -> OptimizeStatic -> new parent/40 -> actual child publication -> visibility. A read-only target-position alias supplies neither these mutable SAME mesh fields nor ownership.

Generic RetainedGameObjectVisualV1 currently owns scene_ (its Scene vectors), MeshFieldsV76, membership/visibility arrays and node handles inline; release destroys/clears those. Its independent resource_owner_v76 correctly pins only actual BRES bytes, not mutable child C1 fields/graph membership. ModuleStaticSceneV2 similarly embeds mesh fields and selected graph in the root allocation. Neither exposes an independently reference-counted actual authored IMeshSceneNode successor that survives old-parent/visual/root destruction after transfer. Borrowing a whole Visual/base/World, using the enclosing root as a fake mesh owner, or copying Scene/mesh state would violate the native child identity/lifetime contract.

A positive generic minimap branch therefore still requires genuine authored mesh receiver/storage ownership established at the existing factory C1 and transferred, with SAME local TRS/cache/flags/visibility/parent fields and original resource pin. Its map callbacks must own only that receiver/resource and weak map/scene scope, then lend SceneMapNodeBorrowV2 to existing map_->add; no replacement SceneManager/World/cache or graph copy. This architectural owner is absent today and is left explicit. It is not reached by the nineteen scoped SWAMP Module/Decor declarations above. Fixture's borrow_node_position_v80 remains separate, read-only, immediate actor-target use; no overlapping visual/getter edits were made.
