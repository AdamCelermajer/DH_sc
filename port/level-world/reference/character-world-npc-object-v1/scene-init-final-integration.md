# Retained Crypt NPC physical and scene integration

Include `character_world_npc_scene_bridge_v1.inc` outside the renderer namespace.
`CharacterWorldNpcSceneBridgeV1` is ready for the actual eleven Crypt actors.
It owns dispatch adapters, not a second actor, rotation, property map, life,
FSM, AI, script VM, scene, physical world or PF object. Its constructor signature
is in the include and all arguments are borrowed references.

Retain these source fields once per same registered actor: ObjectFields,
InitFinalFields, visual light40, target node180, and the single existing
`actor::RuntimeState` (including its RotationState and NavigationObject). Retain
the collision, object lifecycle, physical and scene bridge adapters at stable
addresses. The script object identity and position remain the existing ones.
The CPU CharacterAnimationInstance supplies its actual SceneBinding and Scene.
Do not create a second placement transform around the already placed root.

## Creation order

1. Publish the existing FSM/VM/AIS owner and actor in CharacterWorldRuntimeV1.
   Preserve existing genuine NPC Level/Idle initialization. Load CPO1 visible
   and static defaults into the same ObjectFields before collision can read80.
2. Borrow the player's actual NativeWorld and Crypt floor geometry/obstacle
   registry. Do not make an NPC-only World or use host fixture World bounds.
   Initialize the same RuntimeState.object with source navigation defaults.
3. Construct collision over the same AI event owner, script session, FSM,
   actual shared collision counter, actual frame/dt source and paused byte.
   Physical peer_owner must resolve each actual WorldObject context to its
   same registered GameObject identity, including player and noncharacters.
   enabled80 reads source ObjectFields; it is not mesh visibility.
4. Construct object lifecycle with that PFObject/geometry/registry. Its physical
   query reads this actor's actual physical owner (null before assignment).
   AABB reads the actual InitPost/body projection. Compose its virtual leaves
   using character_pf_services_v1: source Character obstacle=true, radius50,
   strength20. Its Visual/Stop/filter methods require real same-actor services.
5. Construct CharacterWorldNpcPhysicalV1. Its update_pf callback invokes that
   same lifecycle.update_pf(), propagating errors. Physical phase3 represents
   the source assignment prefix before the tail, phase4 complete continuation.
6. Construct the scene bridge with the same registered object, session,
   AIEventState64.active, actual AiTables, physical/lifecycle owners, borrowed
   RuntimeState.rotation, actual animation SceneBinding/Scene, light40/node180,
   common Random references and constructor-produced Network byte5.
7. Call bridge.check_spawn before physical initialization, then honor the actual
   threshold and disabled gates. Current Crypt input receipt has six default100
   and five authored100 thresholds. All are accepted. Cached270 begins-1 and
   becomes-2; the later source calls still perform genuine handle/IsPlayer.
8. Call physical.initialize_source using CharacterNpcBodyModel over the actor's genuine
   model snapshot, actual properties/AiTables and authored position/rotation/
   scale. Install PhysicalServices.debug_switch through
   character_npc_physical_debug_v1(sharedDebug,sharedFiles,key,result,error).
   It performs actual load/query with temporary source string lifetime.
   MP_NoCollisions is queried in PhysicalObject ctor before Body allocation;
   true forces group=-666. MP_NoPhysics is queried in SetPhysicalObject after
   the newly allocated body exists; true destroys that body and skips assignment
   and UpdatePF. Do not prefetch both switches. The initialize(request) overload
   remains an explicitly bounded input path. InitPhysical allocates real Body/shape/mass,
   pins, assigns and reaches UpdatePF. Constructor-null PF legitimately returns
   before obstacle queries. Required missing tails preserve the body prefix.
9. Require physical.phase()==4; bridge.receive_init_post(error) receives the
   genuine projection into the actual SceneBinding and the one borrowed
   RotationState. It copies converted XYZ to rotation[] and SAME Z to heading.
10. Call bridge.initialize(actualActorName, actualLightSetString). This executes
    whole Character3b4978/GameObject38cd48 current-Crypt InitFinal in order:
    once1395 prefix, spawn/disabled gates, visibility, real PF InitObject,
    debug name, actual visual Sync/light, actual scene target_node lookup,
    whole UpdatePF, source fresh type/player queries, MonsterLight and genuine
    same VM OnInitFinal. All eleven actual models have no target_node; real null
    lookup is valid. Do not substitute actor position as a node. Keep original
    target cached184 constructor0; LookAt may use original position160 fallback
    only through the original source branch.

On any failure stop the bootstrap and retain/report the reached prefix. Source
once1395 is written before callbacks; a second call's once guard is not evidence
that a failed first call completed. Network-enabled, rejected spawn deletion,
follower/player tails have explicit required providers when reached; this bridge
does not claim those unsupported families.

## Rotation and visual updates

Source GameObject::InitPost38be5c converts authored degrees using3c8efa35;
38bf28 stores Z to178 and38bf2c stores the identical value to174. The bridge
borrows RuntimeState.rotation rather than owning a new RotationState. No source
constructor producer for turn_positive17c has been established here; do not
claim an invented zero producer. Actual SetHeading/UpdateRotation must supply
future runtime changes before consumers require that field.

InitFinal Sync uses the borrowed Euler rotation and actual projection scale.
Changed rotation/scale reaches mandatory SceneServices.refresh_bounds for source
CalcMeshBox/UpdateOwnerAABB/PF continuation. An empty backend fails when reached.
All eleven initial transforms are already received, so those changed branches
are genuinely skipped in the verified initial domain. Runtime movement, changing
scale and AI controller physics need their actual producers before use.

`character_world_npc_bounds_v1` supplies the pure required CalcMeshBox and
Character owner-bounds arithmetic. Pass the actual BRES and CPU cached
model-space pose, actual visual root matrix, position, resolved Collision_Scale
and previous flat byte. It selects the real marker-first branch or actual skin
joint boxes, then source collision scaling/padding/absolute AABB. It does not
create a model/body projection to fake that calculation. The caller must store
the result into the same retained owner bounds and dispatch lifecycle.update_pf
in source order. A placed SceneBinding scene contains the external root in its
world matrices; passing it directly as model-space pose would apply root twice.
Actual model-space cached-pose production remains a caller requirement.

## Globals and destruction

WorldNpcSpawnGlobalsV1 stores references to the common engine Random seeds and
counters (s_seed99f89c, s_syncedSeed99f8a0, debugCounters99f8a4), not a separate
spawn RNG. Preserve the existing shared owner's lifetime across floor reloads.
The recovered Network singleton chain is GetThis7fd794/instance7fd744,
derived825000/base7fd838; byte5 constructor0 only proves the offline mode field.

Before reload, stop World stepping, then release every actual NPC body while
collision, same VM/AI/FSM, peer lookup and World still exist. DestroyBody can
deliver genuine End callbacks; never replace those with no-op delivery. After
physical release, explicitly call the same lifecycle.update_pf() to remove its
obstacle. The source empty floor-map keys remain. Then remove actor registration
and destroy dispatch adapters before their borrowed owners; destroy the shared
World last. This release+PF removal is verified but is not a claim that the whole
original GameObject destructor/ObjectManager/room dispatcher has been recovered.

## Evidence and scope

Latest direct O1/O2 ASAN+UBSAN host passed1435 checks each including actual
shared Debug queries and pure bounds comparisons over all eleven Crypt
actors/monster VMs, three BRES families, actual Crypt floor graph, native bodies,
filters, collision callbacks, source spawn cache and complete current-Crypt
InitFinal with actual OnInitFinal dispatch. Twelve native body lifecycles include
the additional missing-tail prefix test. Whole original GenerateSpawnProbability
passed1400 calls. Strict ARM64 direct modules and include TU compile passed.
The host explicitly records collision-clock/moving-state fixtures and frozen
dependency attribution; it proves neither full NPC AI nor whole Character
InitPost outer orchestration. Renderer/CMake integration is the root's work.
