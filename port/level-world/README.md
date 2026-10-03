# Authored Crypt level and initial movement

This module loads original room geometry into a native level and extracts its walkable floor triangles. The Android checkpoint places the animated Prince in that level with touch movement. It builds from C++ source for ARM64 and x86_64; the APK does not run the original ARM32 engine.

The reference is `x07_crypt_backup.mlx`, an original authored eight-room layout using `crypt.bdae`. It is not a generated playthrough of `007_crypt_01.rule.xml`. The rule file and sixteen linked MGP/MVP room definitions are retained alongside the geometry and layout. Their 166 object records are preserved. The current renderer instantiates 84 animated decors and 11 directly named, automatically spawned monsters; 71 conditional/scripted/template or other factory records remain pending.

## Evidence and implementation

The current [Prince bank checkpoint validation](../android-native/reports/prince-bank-checkpoint-validation.json)
records native live two-slot playback with116 resources/158 occurrences and
passes emulator movement, combat, lifecycle and complete-bank inspection checks.
[Occurrence coordinator evidence](reports/actor-blended-occurrences-host-audit.json)
binds the registration bridge to genuine host DSOs,72 original PlayClip cases,
17 original selections and158 clip bounds. Full AIS/Lua/gameplay service ownership
and physical ARM64 verification remain unfinished.

The earlier [timing checkpoint validation](reports/character-timing-source-validation.json)
binds the saved 6d9782be APK to frozen actual compiler inputs and 189 assets.
Native [timers](reference/character-timers/NOTES.md),
[stance selection](reference/character-stance/NOTES.md) and gate clearing are
connected to the live Prince. Both packaged ARM64 libraries pass 6,392 timing,
stance and state cases each; both animation/math library pairs pass 5,786 typed
contribution cases. Current stationary/moving combat, movement and lifecycle
smokes pass. Live nonzero delay expiry and full AI/scripts/equipment producers
remain pending. The separate two-slot implementation in development is outside
that saved APK. Earlier character-combat evidence below is historical.

New source work outside the saved APK includes [compiled raw transforms](../engine-animation/reference/compiled-transforms/NOTES.md):
11,656 original/ARM64 samples pass with owned resources, exact cached-key
boundaries, ordered union and clip/template defaults. A separate
[animation-event router](reference/animation-event-routing/NOTES.md) passes
1,344 original-derived ARM64 and sanitized host cases with 2,422 ordered
callbacks. [AI animation consumers](reference/character-animation-ai/NOTES.md)
now compile into the main shared world library. Original/optimized ARM64,
ASan/UBSan and the actual NDK ARM64 world ELF all pass 2,197 consumers, 90
lookups, 4,530 ordered service requests and 17 atomic guards. The Android ELF
is 64-bit AArch64 with 16 KiB LOAD alignment; its CPU replay has explicit
controller/target/AIS services and is not a device or whole-library proof.
[Script selection](reference/character-script-selection/NOTES.md) passes 1,080
original/ARM64 and sanitized host cases. Source
[script lifecycle](reference/character-script-lifecycle/NOTES.md) passes1,480/
4,339 ordered requests and [combat queries](reference/character-combat-queries/NOTES.md)
pass4,401 plus448 real decoded-table bridge checks. Both are now compiled into
the main world library and pass their CMake source-linked sanitizer targets.
Full AIS update/unload, common-Lua bindings, inventory ownership and live observer
integration are pending. An independent [review](reference/animation-blend-composition-reentry/review.md)
also identifies a historical replay-fixture limitation: PlayClip queries
IsEnded after unconditional SetClip, rather than the loop flag used by the
earlier single-slot helper. Its old proof does not establish that producer.
The main source-built library's two-slot audit passes 3,280 real-asset scene
frames, 2,500 root samples, 2,442 zero-weight history checks and callback-tail
regressions under ASan/UBSan in EACH static and dynamic domain, plus 48 scheduler,
72 selection and 2,160 public control cases. The actual Prince factory selects
CDynamicAnimationSet at 0x62f61c; its [different default/channel rules](../engine-animation/reference/dynamic-compiled-transforms/NOTES.md)
now pass 117,030 original/ARM64 samples. Dynamic host registration/defaults are
still explicit caller fixtures. The actual [Prince registration](../engine-animation/reference/prince-registration/NOTES.md)
has158 ordered library occurrences/116 unique resources and template1111 as
default; repeated registrations are not deduped from the library. All116 exact
cache resources are staged in source APK assets, with42 additional files and
`data/prince-animation-bank.json` retaining both requests and resource identities.
The10 position-axis and7 angle tracks across10 resources now pass9,376 original/
ARM64 dynamic sampling checks plus host sanitizer replay. Type9 uses scalar
angle keys with authored axis defaults. A native registration adapter preserves
all engine occurrence indices. Full-bank production-library replay passes26,228
samples and17 original mappings after borrowed Players are destroyed, with
zero sanitizer findings. Live actor selection and full-bank renderer integration
remain pending before the next playback APK; full-bank original-pose parity is
not established by the lifetime test.

The main world library also builds the selected script update/pause-expiry and
collision counter kernels, with actual shared-library sanitizer gold replays.
The main game-data library reads all1,322 authored ItemTable rows, with original
lookup and ranged-weapon query bridges verified. These remain source additions
outside the preserved timing APK; Lua/common bindings and inventory ownership
still need reconstruction and live integration.

[Character state coordination](reference/character-state/NOTES.md) now supplies
the live Prince's bounded Idle/Move/Attack/Dead policies. [Playback events and
Swap](reference/actor-playback-events/NOTES.md) preserve synchronous scene/replay
handoffs, finite closure, retained terminal pose and metadata-only swaps.
Stationary/moving attacks, authored damage events and source Idle completion
pass emulator checks alongside locomotion and lifecycle regressions. The
[current checkpoint validation](reports/character-combat-source-validation.json)
binds189 assets and both actual APKs to 3,910 state and 2,504 fade-metadata cases
each. Single-slot playback, supplied stance0, complete AI/timers/equipment and
live death testing remain boundaries; recovered weight metadata alone does
not implement the full two-slot typed pose blend.

[UpdatePath coordination](reference/navigation-controller/NOTES.md) now has a
source implementation combining the recovered path, heading, avoidance and
boundary helpers. [Physical Stop composition](reference/controller-physical/NOTES.md)
now applies recovered velocity/sleep/force resets and the body-transform kernel.
[UpdateSubObjects](reference/subobjects-update/NOTES.md) also has a source
coordinator, including physics/visual policy order and real Crypt floor/registry
callback checks. The live Prince source now invokes these coordinators through
`actor_runtime.cpp`, following scene playback, genuine physics Step and
animator completion/replay. Both APKs and actual emulator Walk/Run, release,
rotation and pause/resume checks pass; see
[prior locomotion validation](reports/live-actor-source-validation.json).

[Physical controls](reference/physical-controls/NOTES.md),
[SetXForm body writes](reference/body-transform/NOTES.md), and
[character body definitions](reference/character-body-config/NOTES.md) are now
reconstructed. The matching original [Box2D source](../physics-backend/README.md)
now supplies genuine shape creation/mass, proxy synchronization/broadphase,
contacts, collision solving and world stepping. NativeWorld installs the
recovered gameplay filter/contact listeners and creates character bodies from
the verified definitions. NativeBody binds physical controls and subobject
services to real source-built bodies. Visual/root-motion and rotation kernels
plus the native sampled-scene bridge are also reconstructed; see
[scope](reference/visual-motion/NOTES.md) and
[verified artifacts](reports/physics-backend-source-validation.json).

[Movement policy and speed](reference/move-state/NOTES.md),
[decor body definitions](reference/decor-body-config/NOTES.md), and
[absolute-time animation and replay](reference/visual-timeline/NOTES.md) are
also packaged from recovered source. The latest
[validation](reports/actor-timing-source-validation.json) adds 15,266 comparisons
per APK and sanitizer host replay. [Decor scene evaluation](reference/decor-scene/NOTES.md)
now recovers owner Euler/matrix production and actual marker bounds for all 82
Crypt collider placements. [Character scene evaluation](reference/character-scene/NOTES.md)
recovers joint-box bounds, modular provider union, visual scale, collision scale
and owner AABB padding. Full equipment/save producers and blend ownership remain
explicit caller boundaries.

[Original frame ordering](reference/frame-order/NOTES.md) now establishes scene
animation/root displacement, then physics Step, then eligible character
timers/AI/FSM/animation scheduling, followed by path, rotation and subobjects.
This instruction trace is not an end-to-end gameplay frame oracle.

The live source now uses [actor playback](reference/actor-playback/NOTES.md)
and `actor_runtime.cpp` to compose authored root motion, body/world services,
path, rotation and subobjects. It creates 83 genuine bodies: the Prince and 82
decors with authored `_colbox_` markers. Knight's base Scale_X/Y/Z 100/100/100
produces visual scale (.9,.9,1), and resolved Collision_Scale 85 scales the owner
bounds before the recovered character-body constructor. The selected four
warrior controllers use their authored joint boxes and immutable factory pose;
owner XYZ reaches the visual root without foot-Z subtraction.

[Playback](reports/actor-playback-host-audit.json) and
[runtime](reports/actor-runtime-host-audit.json) sanitizer audits verify their
compositions, separately from the original-instruction kernel audits. Complete
Character FSM, original input/type and equipment selection, blend ownership,
enemy pursuit, avoidance policy, camera and full combat/frame ordering remain
unfinished. The verified bundled Crypt checkpoint is
`dh2-native-actor-f6840449.apk`; [current validation](reports/live-actor-source-validation.json)
binds its complete APK hash, source/assets, component audits and emulator proof.
Physical ARM64 and original GPU parity remain unverified.

Recovered [heading rules](reference/navigation-heading/NOTES.md) now drive
player facing during movement and melee. Floor sewing, route finding, waypoints,
position/direction validation, obstacle membership, avoidance and concrete
obstacle/radius producers also have source implementations. Their current
Android probes establish helper integration, while the live source composition
now advances the Prince through root/body/controller services. Complete pursuit
and gameplay scheduling remain pending. Older checkpoint sections below
describe the scope at the time of each saved build.

[Captured original routines](original-functions.json) and [assembly](reference/original-functions.asm) identify `Module::LoadModule`, `PFWorld::LoadRoom`, floor loading and height/collision queries. The original `PFWorld::LoadRoom` prefix table at `0x956b34` contains `minimap`, `floor` and `_exit_`; [inspect_constants.py](tools/inspect_constants.py) reads those strings from the supplied engine. Room root geometry represents bounds, while `_floor_` children provide navigation geometry. Neither is submitted as visible scenery.

`prepare_world.py` checks the cache SHA-256 and ZIP CRCs, follows the authored room links, and compiles a deterministic descriptor. It accepts this layout's zero rotations and unit scales explicitly; support for rotated/generated rooms remains pending. `world.cpp` copies each selected template's descendant graph, replaces the room translation, recomputes checked hierarchical transforms and preserves material bindings. The resulting level has 146 nodes, 97 visible mesh instances, 14,251 scenery triangles and 314 nondegenerate navigation triangles. Four Prince equipment meshes add 586 rendered triangles.

The original entry-point position is `[-2227.77, 1220.93, 812.557]`. Projection onto the original floor produces Z `842.3644`. Eight native floor records supply the recovered selector, octree and collision routines used by gameplay height. The earlier checkpoint's supported-floor adapter used radius 36, tolerance 72 and speed 420 as development choices. Current live source instead obtains body bounds/radius from the selected source mesh/property inputs and displacement from authored playback, with the recovered floor/path/controller chain. The follow camera and touch-to-destination policy remain development choices; full original floor lifecycle, enemy pursuit and game scheduling remain unfinished.

The Android renderer uses the existing preview shader, original diffuse textures and vertex colors. Original lighting, specular techniques, wall visibility/render ordering, shadows, camera behavior, equipment scheduling and animation blending remain pending. Original GPU parity is not claimed. The character's animated foot positions are not replaced with an inverse-kinematics planting system.

## Reproduce

From the repository root, with the owner-supplied original cache:

```powershell
uv run python port/level-world/tools/prepare_world.py "C:\path\Dungeon-Hunter-2-HD-v1-0-2-cache.zip" --output port/android-native/app/src/main/assets/worlds
uv run python port/android-native/tools/bundle_samples.py --cache "C:\path\Dungeon-Hunter-2-HD-v1-0-2-cache.zip"
uv run python port/level-world/tools/prepare_actors.py "C:\path\Dungeon-Hunter-2-HD-v1-0-2-cache.zip"
```

The first command writes the descriptor, original inputs and provenance; the second supplies the character, locomotion clips and textures. Original assets stay ignored by Git. See [Android build instructions](../android-native/README.md).

The third follows the authored object records, resolves original character/model table links, bundles twelve model resources and three explicitly selected development idle clips, and compiles `DACT` v1. The checked native reader verifies each monster's model against the original table again. Case changes and the authored `env_crypt_alpha.tga` alias resolve to existing verified archive entries; assets are never replaced with white placeholders. The [object input provenance](reports/object-input-provenance.json) retains all records, excluded gates, source hashes and alias choices.

`objects.cpp` owns decoded vertices, indices, skin data, graph and animation. It removes collision helpers and mesh shadow helpers from visible batches. Resources are shared across instances; each instance retains its authored room translation, position, Euler-degree adapter and scale. The [native animation tables and scheduler](../game-data/README.md) drive monster selection, speed, completion, loops and redirect advancement. Each monster owns its clock and state; an owned rest graph allows every bank clip to sample independently before uploading that instance's vertices. Decor animation still uses a shared resource clock.

Run `prepare_animation_tables.py` after object preparation, followed by `prepare_actor_states.py`, to bundle the original tables and 26 reachable Idle/Walk/Attack/Died clips. The [state provenance](reports/actor-state-input-provenance.json) records their original entries and hashes. The [26-clip audit](reports/actor-clips-host-audit.json) checks 45,218 millisecond poses under ASan/UBSan, with 31 validated unbound tracks and zero unsupported tracks. Skeleton's ten clips each omit three model targets; slime walk omits one. Character percentage scales, the XYZ Euler adapter, RNG seed initialization and frame-time adapter remain development policies. Original object lifecycle, AI/combat, collision volumes, conditional activation, particle factories, blending, event/root-motion dispatch and shadow techniques remain pending. Three nontransform tracks in the cave entrance effect are counted as unsupported; it currently renders its static geometry.

`prepare_class_tables.py` adds the four original class files and provenance. Android reads all 260 classes and computes an owned cached base-class sheet for each of the eleven monsters; all 224 property values match original-instruction snapshots. Live health initialization, dungeon level selection, dynamic bonuses and attack damage remain pending. See [class formula reconstruction](../game-data/README.md).

The skeleton's optimized model omits targets used by its shared clip. The original animator skips null target bindings at `0x65d9c4`; the native actor loader explicitly permits unbound targets while validating their key data. Other callers remain strict by default. The skeleton binds 23 tracks and reports three unbound targets; slime and ghost clips bind 12 and 24 tracks. See the [object host audit](reports/objects-host-audit.json) for all 44,769 millisecond poses, 3,000 descriptor mutations, strict rejection and malformed-unbound-key rejection. Run `objects_audit path/to/android/assets` to reproduce it under ASan/UBSan.

For host safety checks, configure this directory with CMake and `-fsanitize=address,undefined` for C++ and linker flags. Build and run:

```text
world_audit path/to/crypt.bdae path/to/crypt01.dwld
locomotion_audit path/to/prince_modular.bdae path/to/prince_idle_shield.bdae path/to/prince_walk_1hand.bdae
inspect_world path/to/crypt.bdae path/to/crypt01.dwld
```

The inspector exports world-space floor triangles as JSON. Supplying three additional XYZ arguments checks a point against the current radius policy.

[Host audit](reports/host-audit.json): original room/material links, grounded stairs and boundary movement, nine synthetic checks for slopes, gaps, stacked floors, sliding and invalid inputs, and 2,000 mutated descriptors pass under ASan/UBSan. [Locomotion audit](reports/locomotion-audit.json): all 1,868 millisecond poses in the idle-shield and one-hand walk clips sample and deform four selected skins without nonfinite output; all 23/27 tracks resolve, with none skipped.

[Android world test](../android-native/reports/world-smoke.json) checks actual touch movement, height changes on stairs, the rubble boundary, live/frozen animation, portrait/landscape, position restoration after context recreation and cancellation of held movement on backgrounding. [Checkpoint validation](../android-native/reports/build-validation-world.json) binds those tests to the built APK and bundled asset hashes. Physical ARM64 testing, procedural layouts and full gameplay remain pending; the full reconstruction goal is active.

## Prince stationary combat assets

`tools/prepare_player_combat.py` follows KnightPlayerBase's original
animation table 48 / AttackStatic root 243 and bundles its nine original
clips. Three authored main-hand events occur at 300, 899 and 1,500 ms in
their respective source clips; the original sequence uses speed 1.3.
`player-combat-provenance.json` binds each bundled file to its cache entry
and hash. Existing Prince idle/walk preview policies remain separate.

The [player host audit](reports/player-combat-host-audit.json) compares all
896 initialized owner words with the original Knight reference, resolves
all nine clips without unsupported/unbound tracks, and samples 3,475
millisecond poses through four warrior skins under ASan/UBSan. Run
`player_combat_audit path/to/android/assets path/to/vitals-original-spawn.bin`
to reproduce it.

## Native navigation graph reconstruction

`navigation.cpp` reconstructs the original PFFloor triangle-to-graph builder:
edge midpoint nodes, CompPos lookup and red-black insertion, normalized edge
directions, directed travel weights, passage clearances, rejected-node records
and reverse-link validation order. It compiles into both Android ABIs.
See [source boundaries and original routines](reference/navigation/NOTES.md).

The [ARM64 differential](reports/navigation-arm64-differential.json) compares
1,880 triangle cases, including all 314 exported Crypt triangles under three
supplied floor-support scenarios, with zero mismatches. The
[packaged ARM64](reports/navigation-packaged-arm64-differential.json) and
[Studio ARM64](reports/navigation-studio-arm64-differential.json) binaries pass
the same corpus. All 6,666 floor query coordinates and their call order match.
`navigation_audit path/to/navigation-original.bin` replays the original-verified
logical snapshots under ASan/UBSan; [host results](reports/navigation-host-audit.json)
also pass. Inputs and reference binaries remain local, outside the APK.

The builder is not called by gameplay yet. Original floor collision/selector,
floor flags and identities, cross-floor sewing, graph search, smoothing,
obstacles and controller/animation integration remain pending. The fresh
[Android world regression](../android-native/reports/world-smoke-navigation.json)
checks ten existing rendering, movement and lifecycle cases;
[checkpoint validation](../android-native/reports/build-validation-navigation.json)
binds those checks and the differential reports to the built APK.

## Native floor collision and graph support

`collision.cpp` reconstructs triangle plane/same-side intersection, the
collision manager's ordered ray tests and nearest-hit shortcut, and PFFloor's
bounds check and vertical ray. The native FloorSet callback now lets the
native graph builder decide node support by executing native collision.
[Reconstruction boundaries](reference/collision/NOTES.md) distinguish the
supplied selector triangle lists from the pending original octree producer.

The [collision differential](reports/collision-arm64-differential.json)
compares 5,071 original-instruction cases with zero mismatches. The
[graph/collision differential](reports/navigation-collision-arm64-differential.json)
checks all 314 exported Crypt triangles and 1,239 actual floor support queries,
also with zero mismatches. Both packaged ARM64 builds pass those corpora and
the earlier 1,880-case graph regression. Host replay under ASan/UBSan passes
with `collision_audit path/to/collision-original.bin` and
`navigation_audit path/to/navigation-collision-original.bin`.

The graph experiment supplies room grouping, ordered transformed triangle
lists, bounds and zero floor flags. Octree/collision coupling, floor producers,
sewing, route search and moving controller are still needed before gameplay
uses this backend. [Fresh Android checks](../android-native/reports/world-smoke-collision.json)
verify ten existing rendering/movement/lifecycle cases;
[checkpoint validation](../android-native/reports/build-validation-collision.json)
binds the APK, source, assets, packaged audits and prior combat checkpoint.

## Native octree checkpoint

`octree.cpp` reconstructs the original selector's recursive tree construction,
bounds/corner arithmetic, stable child partitioning and parent-first box
query order. [Source boundaries](reference/octree/NOTES.md) identify the
remaining mesh/transform services and the modern bounded-output policy.
The [ARM64 differential](reports/octree-arm64-differential.json),
[packaged differential](reports/octree-packaged-arm64-differential.json) and
[Studio differential](reports/octree-studio-arm64-differential.json) each match
74 original builds, 875 nodes and 2,294 queries, with zero mismatches.
`octree_audit path/to/octree-original.bin` replays the corpus under ASan/UBSan
and verifies bounded buffers and capacity rejection;
[host results](reports/octree-host-audit.json) record those checks.

The octree checkpoint precedes the selector coupling described below.
Mesh extraction/baking, original floor identities/flags, cross-floor
sewing, search, smoothing, obstacles and movement integration remain pending.
[Android regression](../android-native/reports/world-smoke-octree.json) and
[checkpoint validation](../android-native/reports/build-validation-octree.json)
bind the fresh APK to ten existing world cases and unchanged gameplay/assets.

## Transformed selector and collision integration

`selector.cpp` reconstructs original query setup, node/extra matrix composition,
matrix inverse and box/output transforms. It connects octree selection to ray
and floor collision, and supplies graph support through
`dh2_selector_floor_query`. [Boundaries](reference/selector/NOTES.md) distinguish
the remaining mesh/floor producers from the now-source-built selector chain.
The [ARM64 differential](reports/selector-arm64-differential.json),
[packaged differential](reports/selector-packaged-arm64-differential.json) and
[Studio differential](reports/selector-studio-arm64-differential.json) each
match 1,007 matrix inverses, 3,072 selections, 3,072 coupled rays and 3,072
coupled floors, with zero mismatches. The
[graph/selector comparison](reports/navigation-selector-arm64-differential.json)
matches all 314 Crypt triangles and 1,239 actual support queries using the
complete native/original chains. Both packaged ARM64 builds pass it too.

`selector_audit path/to/selector-original.bin` and
`navigation_audit path/to/navigation-selector-original.bin` replay the verified
corpora under ASan/UBSan. The earlier octree, collision and graph corpora still
pass. [Checkpoint validation](../android-native/reports/build-validation-selector.json)
binds those results to both builds and ten fresh Android world cases. This
historical checkpoint did not use the backend in gameplay. The floor-records
checkpoint below now uses selector/collision for height; sewing and path search
remain necessary for pursuit.

`floor_source.cpp` now reconstructs the original selector mesh extraction,
seven position scalar formats, reversed corner order and unconditional
constructor baking, plus floor-tag flags and node/floor bounds arithmetic.
[Scope](reference/floor-source/NOTES.md) distinguishes these numeric routines
from the remaining full floor loader and scene clone/metadata services.
Standalone and both packaged ARM64 builds match 328 constructions, 850
triangles, 212 tag cases and 512 bounds cases. The packaged tests execute the
actual shared vertex decoder. `floor_source_audit` replays the FMS2 corpus
under ASan/UBSan and checks 249 storage/index/topology rejections.
[Checkpoint validation](../android-native/reports/build-validation-floor-source.json)
also binds selector/graph regressions and ten fresh Android world cases.

`floors.cpp` now creates actual owned Crypt floor records, with parentless
clone transforms, original default flags, baked reversed triangles, expanded
bounds, octrees and selectors. Graph construction uses object flags and real
support queries before separately raising retained triangles by +1 Z.
[Original scope](reference/floor-records/NOTES.md) describes the scene wrapper
and mesh-child transform distinction, supplied lifecycle services and pending
metadata parser. Both packaged ARM64 builds match 208 clone transforms, all
314 authored and raised triangles, and 1,239 graph support queries. The host
loader builds the same 321-node/778-edge final graph state under ASan/UBSan.
[Checkpoint validation](../android-native/reports/build-validation-floor-records.json)
binds native gameplay height and ten fresh emulator cases to the APKs.

`navigation.cpp` now reconstructs complete PFFloor::_Link sewing.
`floors::post_load` follows original room/world overlap gates and floor order,
creates forced boundary nodes, updates directed links and validation lists,
and closes temporary boundary ranges. The current native Crypt graph has
335 nodes, 838 edges, 998 validation references and 14 directed neighbour-floor
relations. Both packaged ARM64 builds match 259 cases and 869 sewing calls;
the independent sanitizer asset loader has the same complete logical state.
[Sewing scope](reference/navigation-link/NOTES.md) describes thresholds,
allocation services and archived invalid records. Route search, endpoint
selection and original movement remain pending. See
[checkpoint validation](../android-native/reports/build-validation-navigation-link.json).

`navigation_search.cpp` now reconstructs the original specialized graph search,
including its heap ties, repeated predicates, budget behavior, early goal queue
purge and output-list insertion order. All three ARM64 binaries match 1,084
original-instruction cases, 96,279 node records and 101,090 predicate calls.
The sanitizer replay also searches the actual asset-loader graph in all 256
authored Crypt cases. `floors::search_nodes` owns the modern outgoing index and
scratch adapter. Android startup verifies all 64 ordered floor-pair routes
against original gold: 1,598 edges and checksum `5454073390c7d916`.
[Search scope](reference/navigation-search/NOTES.md) distinguishes this graph
search from pending original endpoint selection, radius/obstacle predicates,
world FindPath, smoothing and the movement controller. Player movement and
enemy pursuit do not yet use these routes. See the
[graph-search checkpoint](../android-native/reports/build-validation-navigation-search.json).

`navigation_world.cpp` now reconstructs position-to-position PFWorld::_SearchGraph:
first-hit room/floor traversal, triangle-midpoint lookup and nearest attachment,
direct routes, radius/capability predicates, failed-pair cache and output tail
trimming. It matches 525 original cases, and the actual asset-loader world
passes the same sessions under ASan/UBSan. Android probes all 64 ordered floor
pairs from world coordinates: 2,086 segments, checksum `3a3ab2a724cc383b`.
[World-search scope](reference/navigation-world/NOTES.md) documents the supplied
scene/storage facts and explains why this wrapper adds no temporary graph nodes.
`navigation_path.cpp` now adds original FindPath, incremental portal smoothing,
waypoint vectors, source-plane passage checks, MovePath, DropPath and per-edge
squared path lengths. The 661 intersection cases, 4,509 path operations and
366 complete FindPath requests match original instructions in all three ARM64
binaries. Both sanitizer corpora pass; FindPath uses the actual asset-loader
world. Android probes 64 FindPath routes with checksum `9356b419cae2bc57`.
[Path scope](reference/navigation-path/NOTES.md) records the ownership adapters
and original source-plane semantics. ValidatePosition/ValidateDirection, cache
lifecycle, actor producers and the moving controller/pursuit remain next;
characters do not yet follow these routes.

Android's Attack button currently chooses the nearest living monster within
the strict original 3D sum of melee radii, holds the Prince in place and
plays the complete original stationary three-combo sequence. This input,
target selection policy is newly implemented; original target acquisition,
line of sight, approach, input/FSM timing, moving attacks, equipment,
blending and root motion remain pending. Authored events execute native
melee/result application and queue the original monster death clip.

`tools/prepare_player_defender.py` additionally bundles KnightPlayerBase's
original Died root 259 / clip 1023 (`prince_dying_01.bdae`). Its separate
`player-defender-provenance.json` retains the cache entry and hash. The
[death pose audit](reports/player-death-host-audit.json) checks all 1,400
millisecond poses, 27 bound transform tracks, four warrior skins and all
896 normal initialization words under ASan/UBSan. Reproduce it with
`player_combat_audit path/to/android/assets path/to/vitals-original-spawn.bin --death`.
The [attack pose regression](reports/player-defender-attack-pose-regression.json)
also retains all 3,475 previous attack poses.

Android queues the Died clip after verified core player kill, cancels player
attacks, blocks movement and holds its final pose. Renderer recreation keeps
the logical health/death state and scheduler cursor. This is an original
clip adapter; full player FSM, revive/game-over services, root motion and
blending remain pending. Enemy-to-player target selection is still supplied
through development commands rather than original automatic AI.


`navigation_motion.cpp` reconstructs floor/room/world height and unnormalized
normals, cached floor/room priority, original position acceptance/restore rules,
capability/strict height limits and floor-boundary direction sliding. All three
ARM64 binaries match 2,353 original comparisons, including 2,860 ordered floor
queries and 171 slide attempts (149 accepted and 22 rejected). The ASan/UBSan
replay loads the real authored assets and checks all object/normal updates.
Android probes 64 position/direction floor pairs with checksum
`d60ad48039cc85c6`; existing path/search probes and ten emulator cases still pass.
[Motion scope](reference/navigation-motion/NOTES.md) records supplied policy,
libm/parent-service fixtures and exact return values. That checkpoint observed
the parent service; the object module below now supplies its registry backend.
Force/avoidance, actor producers and character controller remain pending.
Player movement still uses the supported-floor adapter.

## Object initialization and obstacle registry

`navigation_objects.hpp/.cpp` recovers PFObject motion/obstacle defaults,
InitObject, InitObstacle, capability flags and `_ChangeObstacleParentList`.
The attached position API executes the registry relocation before committing
accepted position/floor state. Bounded 64-bit-key storage preserves empty map
keys, duplicate members and ordered first-match removal. See
`reference/navigation-objects/NOTES.md` for exact scope and the 1,093-request
original-instruction comparison. Host `navigation_objects_audit` replays actual
Crypt assets under sanitizers, including atomic capacity failures. Dynamic forces,
actor producers, cache lifecycle and original controller integration are pending.

## Obstacle force, avoidance and physical filtering

`navigation_avoidance.hpp/.cpp` reconstructs ordered per-floor obstacle forces,
the original AvoidObstacles steering/turn limits and concrete PhysicalObject
group/category/mask collision filtering. The APIs preserve original duplicate
contributions, strict endpoint/influence checks, map-key side effects and
unguarded coincident-position normalization. Bounded caller storage adds atomic
malformed/capacity rejection. See
[scope](reference/navigation-avoidance/NOTES.md) for original addresses and fixtures.

Each of three ARM64 libraries matches 2,008 original requests, with zero mismatches.
Host `navigation_avoidance_audit` uses actual Crypt assets under ASan/UBSan, checks
eight atomic rejections and replays all previous navigation corpora. Android
matches eight floor probes, 16 contributions and eight limited adjustments,
checksum `09d1d79d2612c945`. This does not move actors. GameObject/physical-field
producers, cache lifecycle, speed/root motion and the original moving controller
and pursuit remain next.

## Concrete obstacle producers

`navigation_producers.hpp/.cpp` reconstructs GameObject::UpdatePFObject and
concrete GameObject/Character/Container/LiftableObject/TriggerTrap obstacle
traits. It preserves the null-user gate and registration-before-radius order.
Character obstacle radius/strength are 50/20; the three scenery classes use
150/10. Physical-body radius converts to game units with a factor of 100;
without a body the update uses half the larger existing XY bound extent.
The old force-field names `obstacle_weight`/`obstacle_extent` now have traced
producer meanings: obstacle radius/strength.

See [scope](reference/navigation-producers/NOTES.md). All three ARM64 libraries
match 1,238 original comparisons; `navigation_producers_audit` replays them
through actual Crypt assets under ASan/UBSan, with ten atomic rejection checks.
Android's eight-floor probe matches checksum `30c1f3ac1bf81145`. Original body
construction, filters/bounds/capability initialization and moving controller
remain pending. Actor movement does not use the producer API yet.
