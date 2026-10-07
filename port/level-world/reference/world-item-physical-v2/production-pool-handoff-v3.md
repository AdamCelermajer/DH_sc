# Same-world Item pool graph V3

This is a callable native receiver composition, not a live lethal-skill acceptance claim. Root owns renderer insertion and app builds.

## Verified owners

* Original cached ItemDrops asset: 225424 bytes, SHA256 `c4d783a23a158b4e52687d8ed718c584ee5ad90a325e0111c8da95ae86937610`.
* `world-item-visual-cache-v2`: Android PASS30 visuals/429 checks, all29 audiovisual xrefs plus fallback. Real BRES subtree/mesh/material/animation bytes; declared Scene/PF/condition transports.
* `world-item-pool-graph-v3`: Android PASS145 receivers/1916 checks. Same canonical Manager/PropertyMap and actual Item factory, genuine Item Spawn InitPost leaf, qualified later GameObject InitPost, real scene registration/release, native sensor body, source enable/deferred visibility/filter lifecycle. PF/device/condition/Debug endpoints are declared fixtures. Drop/award/XP/quest are outside that fixture.
* `canonical-retained-actor-adoption-v3`: Android PASS unchanged existing Character identity/Handle/property/life/name/room and one source publication. It does not claim full Character InitPost/VM lifecycle.
* `point3d-normalize-v2-original`: original ARM versus ARM64 PASS1008 comparisons, zero mismatches including zero/NaN/overflow. Source Point3D normalization divides each component unconditionally; glitch vector3d's guarded reciprocal normalization is different.
* `player-save-quest-sync-v3`: Android PASS source offline/local/hosting/phase38/receive/unpack ordering. Positive quest/messaging leaves are declared fixtures. Same Save owns readiness+14; no profile+8 proxy.

All receipts under `reports/android-native-owner-tests` pin their exact executable sources and linked APK. They include new TUs in the isolated executable and therefore do not establish that those new TUs were present in the APK.

## Source corrections

1. SceneManager3596f8 appends `-node` to the actual audiovisual xref. `getNode61c290` searches visualScene0 depth-first by serialized ID. `ResetPositionFromFile35cc0c` resets selected first-child TRS. The current complete-scene transport explicitly requires one visualScene for this named subtree domain; actual ItemDrops has exactly one.
2. Shared animation clips can refer to other item subtrees. Original applyAnimationValues65d9c4..65da08 skips NULL target bindings. The existing MissingTargets::ignore path validates bytes/keys but omits unbound transforms.
3. Item.InitAgain calls GetColor then discards its result. Actual AnimController virtual+1c is PlayClip(index0,false,extra0,group0), not a material tint setter.
4. Item virtual SetRelativeAABB3ebfa0 ignores the supplied mesh box. Nonflat multiplies existing XY extents by1.5 and updates absolute bounds; flat leaves relative bounds unchanged. It does not call PF. Default±100 and two original Apply calls yield±225 XY for nonflat Item initialization.
5. Item Enabled/Disabled inherits GameObject38ba38/38ba04. It invokes source visibility/updating in order, toggles PF object_flags bit8, restores/zeros the actual PhysicalObject saved sensor filters and Refilters, then writes disabled373.
6. RootSceneNode35c26c defers positive visibility via byte209. onAnimate35d310 applies base visibility, calls SceneManager.notifyVisibilityChanged5890a8, then clears209. The latter is a leaf writing manager byte289. Required notification failures retain the reached prefix and diagnostic.
7. Drop sound uses actual destination/cache+1a8 after pool SetDestination, boolfalse/int1/floats−1,−1. The base's1a8 alias is the existing runtime.controller.destination, not its separate target_position field.
8. ObjectBase.TestEnableCondition directly reads Character14e8 Save pointer and Save quest-sync byte14. Both Save C1 variants write0. Original offline SG_TryQuestSync4679e8 sets1 after actual GetOnline byte5 query. Actual call sites are Character.InitPost, Level._LoadProcess and Save.SG_Update; calling only sync does not implement the latter's quest Update tail.

## Production composition

Retain one CanonicalObjectManager/PropertyMap on the current World. Adopt existing NPC receivers through `RetainedCharacterActorV1::canonical_adoption_v2` and `CanonicalExistingActorPublicationV3`, before target-runtime registrations copy handle_key. Do not replay C1/defaults/VM/FSM. A real borrowed Player canonical facet is also required; do not allocate a second player/base.

`CanonicalItemFactoryV2` must receive that same manager/map and the existing immutable loot/audiovisual leases. Its source Item services route to one `WorldItemGraphV3` per actual receiver. Build graph services with the real World physical/PF/Debug/device/condition, same GameDesign and existing FontPalette, actual Vox and ItemPresentation destruction owner. `world_item_scene_services_v3` replaces registration/release callbacks with the same real GameObjectSceneRootRegistryV1; lookup uses `WorldItemVisualV2::constructing_root_v3` during constructor registration.

`WorldItemGraphV3::route` reports handled=false for missing outer operations; caller must deliver actual GameObject frame/Stop/IsAtDestination, tooltip/local-player/interaction providers. `WorldItemVisualV2::update_v3` takes real manager notify callback: call `notify_visibility_changed_v3` on that same registry. Item sensor collision routing uses the canonical GetHandle/GetObject/AsChar, actual peer owner/visibility and Character OOI14a4. No nearest target, guessed pickup radius or direct Gear insertion is permitted.

`WorldLootItemRuntimeV1` then precaches the source29×5 pool, and its real drop callback composes CharacterLootDropV8→actual LootCreationV8 on existing RNG/text/power owners→DropAndAward into this same pool. Scatter uses `character_loot_scatter_connected_v9`, sharing the generation RNG and source Vec3f_K. Automatic interaction occurs only at the original PickUpType branch. Canonical deletion must release the graph's visual through original source services before removing retained dispatch; failed candidates retain their source failure prefix for discard.

CtrlKill remains ordered CharacterKill (dead/HP0→loot→contributors OnKilled→XP→async quests) then OnDied. Target changes belong to original CharAI._UpdateTarget; do not add another death publisher.

## Translation units

Existing compiled RetainedVisual requires: `authored_scene_subtree_v2.cpp`, `visual_mesh_box_fallback_v2.cpp`, `visual_aabb_dispatch_scope_v3.cpp` (added level-world CMake).

New production graph closure not yet live-bound: `world_item_visual_v2.cpp`, `base_index_animation_controller_v2.cpp`, `item_color_lookup_v2.cpp`, `world_item_drop_sound_v2.cpp`, `world_item_graph_v3.cpp`, `world_item_scene_services_v3.cpp`, `object_enable_condition_v2.cpp`, `point3d_normalize_v2.cpp`, existing `world_loot_item_runtime_v1.cpp`, `world_item_object_owner_v1.cpp`, `world_item_physical_v2.cpp`, `item_body_config_v2.cpp`, `canonical_spawn_owner_v1.cpp`, `canonical_item_factory_v2.cpp`, `game_object_set_position_v2.cpp`. Save source endpoint adds game-data `player_save_quest_sync_v3.cpp`.

Save header and RetainedVisual fields changed; rebuild all callers before APK acceptance. Current manager/visual/actor edits are stable. Existing APIs retain their previous contracts; new semantic successors are explicit.
