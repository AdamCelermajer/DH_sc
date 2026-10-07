# Character family visual and animator V6

This capsule is a real Character receiver consumer, not another GameObject base, Crypt monster clone, or alternate property/HP/Scene authority. `RetainedCharacterFamilyVisualV6` borrows the existing `RetainedCharacterActorV1`; `source_visual()` remains the sole inherited+2d8 field. The generic visual owns the actual BRES bytes, graph, skins, node handles, root, and manager registration. CharAnimator uses that **same** graph and the **same** SceneBinding/Root storage. Source initial bounds are not recomputed from a selected idle pose.

## Production linkage

Add these five TUs to level-world:

* character_npc_animation_set_v6.cpp
* character_visual_asset_owner_v6.cpp
* character_same_scene_animator_v6.cpp
* character_live_body_projection_v6.cpp
* retained_character_family_visual_v6.cpp

Also link retained_character_visual_connection_v5.cpp if absent. Existing engine-animation, engine-skinning, engine-math, scene-materials, game-data and Box2D dependencies remain. `canonical_character_family_v4.cpp` is required when the loader constructs that canonical factory, but is not an implicit V6 visual dependency. No new RNG implementation or Application startup substitute is introduced.

Shared additions require a coherent rebuild of every consumer: GameObjectVisualFieldBorrowV5 gains an appended marker-aware callback; RetainedGameObjectVisualV1 gains one retained animator-removal callback. Actor and GameObjectSceneBinding add accessors only. PhysicalV1's model reference becomes an optional pointer in the same native pointer slot, with a model-free constructor and explicit live-projection entry. Existing legacy model constructor and initializer methods retain their behavior.

## Exact binding points

Create one `GameObjectSceneRootRegistryV1` and one `CharacterAnimationSetCacheV6` on the same retained World. Keep both through actor teardown. Create one `RetainedCharacterFamilyVisualV6(actor, services)` for each actual Character record. Services borrow actual asset reads, animation dictionary, debug/sound/FX preloads, source animation selection, shared manager and PF update; none may acknowledge an unimplemented positive source call.

At inherited **GameObject.LoadVisualObject394eb0**, call `family.load_visual(error)`. This executes real SetVisualObject names/force, retained Visual constructor, root registration and failed-load destruction rules over the same model290/xref2a8 strings. It is one helper inside whole GameObject.InitPost38be5c, not a replacement for that method.

At **CharAnimator.SetAnimationSet3c9f4c**, call `family.set_animation_set(actualAnimationTables,error)`. `animation_selection` must deliver source GetCharAnimTable/GetCharSkillList results, including real skill animation entries; never force an empty list for an unknown tree. The original non-player registration order, repeated occurrences, redirects, template default library, debug, nullable sound-manager branch and FX preloads are retained. Existing set IDs use the shared actual cache and do not register a second copy. Failed/in-progress cache prefixes are retained and rejected rather than replayed.

Before ANIM_Set/start, provide the actual same Character RaiseEvent observer and authored step-FX callback. Missing callbacks fail explicitly; the isolated test's event/preload hooks are declared fixtures. Use `family.animator()->start/scene_phase/animator_phase` and its `playback()` for the existing exact source callbacks. Pose changes reskin the existing visual. The same `binding()` and `visual()->scene()` must feed subsequent physical/actor frames.

At **InitPhysicalObject3b4088**, construct the model-free `CharacterWorldNpcPhysicalV1(actualNativeWorld, actualCollisionOwner, SAME object, SAME session, real services)` and invoke:

```cpp
physical.initialize_source_live_v6(request,
    [&](const auto& q, auto& result, auto& error) {
        return family.body_projection(q, result, error);
    });
```

The request must borrow the SAME ScriptSession property view and AI table. The physical owner supplies its genuine new-physical identity, queries original Debug MP_NoCollisions before allocation and MP_NoPhysics after allocation, creates real body/shape/mass/pin, and invokes the required same Character UpdatePFObject tail after assignment. No dummy CharacterNpcBodyModel or competing scene is needed.

The live projector reads source-published Character bounds, original visual mesh box/marker, raw scale and cached properties. It uses the existing original Character body-definition kernel. Nonzero AI types other than1 have the source literal IsPlayer=false branch; type0 archetype lookup and type1 player require separate providers. Actual Priest type2 creates source group−2/category8/mask0xd3b, rather than monster type4 group2/category0x10. Character marker models now pass the actual Visual+28 `already_scaled` byte into SetRelativeAABB; the earlier hardcoded false incorrectly reapplied Collision_Scale.

Close actual body/PF/collision registrations first while World/VM identities are live. Then `family.close(error)` executes source root release/removeAnimators/ForceRegister and drops the real visual. This detaches the actual CharAnimator before its receiver is destroyed. Unpublish canonical World/ObjectManager last. A failed close must remain visible; do not discard the receiver or catch it as successful teardown.

## Acceptance and remaining boundaries

Original ARM registration gold lists for Skeleton, Slime, Slime_RE and Ghost match the native ordered collector exactly. Actual table census:77 supported template families. Native test: actual row419 WanderingPriest/table47/Priest_good.bdae, Skeleton, Slime, Ghost and a second Priest sharing the same manager; same-scene/root/skin playback185 frames, source marker-bound checks, real native body allocation/pinning and correct Priest physical filter. It intentionally declares PF, Debug/FX preload and AI event delivery as fixtures. It proves those native ownership/asset paths, **not full live Swamp initialization**.

The new `initialize_source_live_v6` physical-owner overload is strict-compiled on both ABIs. This native test allocates the projected body definition directly; it does not execute that overload's full collision/Debug/PF services graph. That remaining integrated initialization must be tested with the real retained ScriptSession and registered World before acceptance.

Full inherited ObjectBase/GameObject InitPost still needs actual ConditionData8c/b0, difficulty/idle sound stores, spawn RNG, source TRS/clamp/heading/SetPosition and device predicates over the same Character storage. Character InitPost still requires actual script load/init, FX cache registration/selfFX, sounds, initial-floor position, Revive/HP-MP/group and full source PF/AI continuations at their reached calls. V6 does not pretend a successful Visual constructor completes these methods. Alias dictionary IDs sharing one CCDB path currently remain an explicit required cache binding, not duplicated resources. Player/Type0 archetype branches, renderer submission and live loader acceptance remain separate.

Reproduce: `.local-inputs/run_character_npc_animation_set_v6.py`, `.local-inputs/prepare_character_visual_v6.py`, `.local-inputs/run_character_same_scene_animator_v6.py`, `.local-inputs/compile_character_visual_v6.py`. No APK install, app lifecycle, input, HP or RNG mutation is performed by these isolated fixtures.
