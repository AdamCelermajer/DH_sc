# Native reconstruction goal

Deliver a playable Dungeon Hunter 2 HD rebuild from native source for modern ARM64 Android, with every required asset inside one APK. The native build must not execute the supplied ARM32 engine through a translation runtime. Exact historical studio source text is not the deliverable; verified reconstructed behavior is.

## Current checkpoints

- Full Prince bank live checkpoint: `dh2-native-prince-bank-4f5b7d11.apk` now
  runs native two-slot playback from116 resources/158 source occurrences, using
  template1111, canonical first-index mapping and retained zero-track1138.
  Both Android ABIs/repository and Studio builds pass. All233 APK assets and
  14 ELF64 libraries are verified;16KiB LOAD/ZIP alignment passes. Source-bound
  host audits verify17 selections/158bounds/72 original PlayClip cases and
  26,228 bank samples. API37 emulator movement, combat, lifecycle and bank
  freeze/resume/recreation pass. [Validation](reports/prince-bank-checkpoint-validation.json)
  binds frozen compiler inputs and exact artifacts. Full six-event AI/AIS/Lua
  ownership, equipment/NPC gameplay, quests/UI/audio/saves and physical ARM64
  verification remain required. The goal remains active.
- Earlier source work developed outside the timing APK: compiled raw transform sampling passes
  11,656 original/ARM64 cases plus ASan/UBSan ownership/default/cursor checks.
  Character animation-event routing passes 1,344 cases/2,422 callbacks on both
  ARM64 and sanitized host replay. Source delivery gates and callback ordering
  are reconstructed. AI consumer source now builds in the main world library
  and passes 2,197 original/ARM64 cases, 90 lookups and 4,530 ordered requests;
  the actual NDK ARM64 world ELF also replays that gold and 17 atomic guards.
  Script selection passes 1,080 cases; source script lifecycle passes1,480/
  4,339 ordered requests and combat queries pass4,401 plus448 real-table bridges.
  The latter two now compile into the main world library and pass sanitized
  source-linked replay. Selected AIS update/pause expiry now passes603 cases;
  collision counter production passes4,096 cases/6,839 ordered callbacks.
  The actual1,322-row item loader and original item lookup/range gold also pass.
  Full AIS update/unload, Lua/common bindings, inventory
  ownership and live observer integration remain unfinished. Independent review corrected the prior PlayClip fixture's
  loop/IsEnded assumption; historical helper checks do not establish that
  producer. See [raw scope](../engine-animation/reference/compiled-transforms/NOTES.md)
  and [routing scope](../level-world/reference/animation-event-routing/NOTES.md).
  The main native library now builds the two-slot coordinator; 3,280 real-asset
  scene frames in EACH static and dynamic domain, 48 scheduler, 72 selection
  and 2,160 public control cases pass under ASan/UBSan. The dynamic compiler
  at 0x62f61c now matches 117,030 original/ARM64 samples with distinct defaults/
  channel pruning. Both Android ABIs build, but the host default-registration
  fixture does not use the actual Prince template. Original registration now
  proves158 library occurrences/116 unique resources, with template1111 first;
  dictionary IDs map by first resource control identity rather than deduping
  the library. Source assets now contain all116 resources (42 added). Their10
  position-axis and7 angle tracks pass9,376 original/ARM64 samples; static11,656
  and dynamic117,030 sample regressions remain clean. A native registration
  adapter preserves occurrence indices. Full-bank production-library replay
  passes26,228 samples,17 original mappings and158 clip bounds after borrowed
  Players are destroyed; full-bank original-pose parity remains unverified.
  The bank/coordinator now execute in the live checkpoint above; complete
  source AIS/Lua service integration remains required.
- Character timing source integration: recovered timer storage/update/reentry,
  stance priority/clamp and state-event gate clearing now compile into the live
  actor. Timers execute after Step and before state/animator. Both actual APKs
  pass 6,392 timing/stance/state cases plus 5,786 typed contribution cases, including
  actual packaged quaternion math. Current combat/movement/lifecycle smokes pass.
  [Timing checkpoint validation](../level-world/reports/character-timing-source-validation.json)
  binds `dh2-native-character-timing-6d9782be.apk` to 189 assets and frozen actual
  compiler inputs. Live nonzero cooldown expiry, complete AIS/equipment/scripts,
  live two-slot composition and physical ARM64 remain unverified/unfinished.
  Two-slot compiled target sampling, playback and AI consumer additions are
  source work explicitly outside this saved APK.
- Irrlicht lineage confirmed: original `Core/Irrlicht` paths and `glitch` symbols,
  historical GUI correspondences and 56 original receiver-filter probes establish
  useful stock-source recovery guides. Scene graph, quaternion caching, Android
  programmable driver and animation differ. [Evidence](../level-world/reference/irrlicht-lineage/NOTES.md)
  preserves official historical source hashes and license. Exact base version is
  unidentified; wholesale stock-engine replacement is not supported.
- Character combat source integration: recovered Idle/Move/Attack/Dead state
  focus/blur, movement hysteresis, cached property speed, body pin/filter and
  animation services now drive the live Prince. Authored events dispatch before
  Step; finite closure and deferred selection follow Step. Stationary3→5 and
  moving4→5 attacks, native HP damage, preserved attack root motion, Swap
  timeline preservation, busy guard and source0x22→Idle pass bounded audits and
  emulator checks. Both actual APKs pass 3,910 state and 2,504 fade-metadata
  cases each. [Checkpoint validation](../level-world/reports/character-combat-source-validation.json)
  binds `dh2-native-character-combat-2241cfe9.apk`,189 bundled assets and current
  movement/lifecycle/combat evidence. Supplied stance0, single-slot playback,
  incomplete AI/timers and untested live death remain explicit limits.

- Live actor source integration: Prince locomotion now composes authored scene
  playback/root displacement, genuine physics Step, animator notification/replay,
  UpdatePath, rotation and UpdateSubObjects. The playback sanitizer audit checks
  10,584 phase states and 5,628 authored root samples; the runtime sanitizer audit
  checks real body, floor/registry and coordinator order. Both final APKs pass
  independent ARM64 checks; emulator Walk/Run, release, rotation and held-input
  pause/resume pass. [Current validation](../level-world/reports/live-actor-source-validation.json)
  binds checkpoint `dh2-native-actor-f6840449.apk` to source/assets and runtime
  evidence. These are component/composition audits, not a
  full original Character frame/FSM oracle.

- Genuine physics: source-built Box2D 2.0.1 provides shape creation/mass,
  broadphase, contacts, solving and world stepping, with original gameplay
  listeners and body pin/unpin/Stop services. Current live construction creates
  83 bodies: the Prince and 82 authored Crypt `_colbox_` decors. Marker matrix
  production uses actual scene bounds, scales and recovered Euler conventions;
  unmarked visual decors receive no invented collider. Character bounds use
  selected warrior joint boxes/factory matrices, source base visual scale and
  resolved Collision_Scale, then original AABB padding/absolute production.
  Complete source equipment/save and scene clone/cache producers remain inputs.

- Path and turn source: UpdatePath/IsAtDestination, destination/waypoint
  distinction, LookTowards/SetHeadingDirection, floor boundary steering,
  PF registration and physical Stop now supply the live Prince coordinator.
  Source avoidance exists, but the live avoidance policy, enemy pursuit and
  original camera remain integration boundaries. Modeled-import angle tests do
  not establish historical device libm parity.

The entries below retain evidence and limits from earlier saved checkpoints.
Their then-pending helper integration is superseded by the live source status
above; they do not validate the new APK or current gameplay frame as a whole.

The next source integrations are two-slot typed animation blending, actual
equipment stance producers and the complete AI expiry callback services. The
[Character state notes](../level-world/reference/character-state/NOTES.md) and
[event/Swap notes](../level-world/reference/actor-playback-events/NOTES.md)
record recovered behavior and exact component boundaries. Complete AI, original
camera/input, general level factories, UI, audio and saves remain required for
the full-game goal.

- Native object/obstacle registry: recovered InitObject/InitObstacle, motion and obstacle defaults, capability setters, ordered floor memberships and parent relocation attached to position validation. All three ARM64 binaries match 1,093 original requests, including deque block growth/erase; sanitizer corpus and nine atomic caller/storage rejections pass. The live 64-pair probe matches 64 accepted/registered objects and 56 relocations; ten emulator cases pass. Force/avoidance, GameObject producers and original character controller remain pending. See [checkpoint validation](reports/build-validation-navigation-objects.json).
- Native floor sewing: original room/world overlap gates and call order, forced boundary-node creation, directed links, validation references and neighbour-floor membership match 259 original-instruction cases / 869 sewing calls in both packaged ARM64 builds. The authored Crypt's live linked graph has 335 nodes, 838 edges, 998 validation references and 14 directed neighbour-floor relations. Host asset loading builds the same complete logical state; sanitizer replay and ten fresh emulator cases pass. Route search, endpoint selection, smoothing, obstacles and original movement/AI pursuit remain pending. See [checkpoint validation](reports/build-validation-navigation-link.json).
- Native floor records: actual Crypt BRES streams create eight owned floors, with original default flags, clone transforms, selector baking, octrees, bounds and separately raised retained triangles. Gameplay height uses recovered selector/collision code. The live graph has 321 nodes and 778 edges; both packaged ARM64 builds match all 314 authored triangles, 208 clone transforms and 1,239 graph support queries. Ten fresh emulator cases and sanitizer asset/pose regressions pass. Full floor metadata/lifecycle, sewing, route search and the original movement controller remain pending. See [checkpoint validation](reports/build-validation-floor-records.json).
- Resource/BRES parsing, mesh decoding, quaternion math and animation access/search: integrated existing reconstruction modules.
- Texture decoding: native implementation, original-instruction comparisons and Android texture regression checks.
- Static scenes: original candle and swamp-menu meshes/material links, checked hierarchical transforms and GLES2 rendering.
- Node playback: two original candle scale tracks animate in the native renderer; deterministic fixed-frame inspection and original interpolation comparisons pass.
- Character playback: Prince warrior preview, native CPU skinning, all 29 position/quaternion/scale tracks and two animation segments. All 173 controller tables pass the host reader/deformation audit; four selected meshes render in Android.
- Level integration: original authored eight-room Crypt layout, visible room geometry, original floor triangles, animated Prince, touch movement, stairs and boundary checks. Separate original idle-shield/walk clips resolve 23/27 tracks. Rotation restores position; backgrounding cancels held movement. This initial movement controller and follow camera are new policies, with original PF/game scheduling still pending.
- Object integration: original character/model tables resolve 448 records, 224 fields and 116 models. The authored Crypt instantiates 84 animated decors and 11 directly spawned idle monsters using twelve resources. Shared optimized skeleton clips permit three validated unbound targets. The original three-matrix skin palette fixes nonidentity bind shapes. Conditional/template factories and 71 remaining records are still pending.
- Animation states: native readers consume all original 785 sequences, three camera sets, 80 character sets and 1,447 clip paths. Each monster has its own completion scheduler and clock; 26 original clips cover idle, walk, three-stage attacks and death for skeleton/slime/ghost. Random re-selection, finite/infinite loops and redirect unwind are reconstructed. The random helper matches 10,000 original-instruction ARM64 cases, and the isolated completion decision matches 3,000. Debug commands select states; full AI, blending and root motion remain pending.
- Animation events: immutable native readers recover five original monster attack tracks, including `attack_mainhand`. Search, named-time lookup, callback timing/order, repeated-boundary suppression and single-wrap dispatch match 30,775 original-instruction comparisons / 109,462 callbacks. Every monster dispatches independently. State-5 melee events now execute hits for supplied development targets. Other state-machine branches, complete original timeline scheduling, FX/audio and root motion remain pending.
- Class properties: complete native reader for 260 class lists / 1,659 formulas, wrapping fixed-point arithmetic, ordered groups and explicit buff snapshots. The cached evaluator matches original instructions across 3,568 complete 224-word sheets. Normal uncached base evaluation and HP/MP initialization are implemented below; general application to other owner sheets and dungeon level selection remain pending.
- Property resolution: recovered all original defaults/types and native base/saved/gear/ordered-buff resolution plus raw/fixed-point mutations. Original instructions match 119,168 resolutions and 7,168 mutation cases; ASan/UBSan checks all 448 source character sheets against original-instruction references. Android owns four sheets per monster and verifies eleven resolved checksums. Gear/buff producers and general owner-sheet class contexts remain pending.
- Normal base/vitals: uncached linear class reads resolve current owner stats, and normal class recalculation resolves all 224 properties. HP/MP fill and raw regeneration preserve original defaults, wrapping and signed caps. Original comparisons cover 2,080 uncached class cases, 10,000 regeneration cases and 448 full character recalculations/spawn-vitals sequences each. All eleven native Crypt monsters initialize to HP 12,160 and MP 5,632 through the original two-pass call sequence. Full actor lifecycle, equipment/buff producers, dungeon level selection, timed regeneration and combat remain pending.
- Damage/event decisions: native damage, DoT, weapon-category bonus and combat RNG match 23,792 original calls; state-5 main/off-hand and ranged event decisions match 6,000. Android preserves the original outer/inner step arguments and resolves unarmed Crypt capability from the original projectile property. Fresh event evidence includes all three outer sequences observed in the actor regression. Automatic target selection, inventory capability fallback and full combat/AI remain pending.
- Hit/result calculation: the full native ten-word result path includes miss/dodge, block/critical, all five status chances, damage selection and exact RNG order. It matches 9,792 original results, and the native melee mask/category wrapper matches 2,000 original calls. All result cases pass ASan/UBSan. Android retains eleven copied initialization probes and also executes live melee results for supplied targets. Alternate-mode buff removal and complete equipment services remain pending.
- Health writes/requests: native HitFor health subtraction, original property mutation, dead-target/session/debug gates, kill/lifecycle requests and low-health hysteresis match 10,926 original-instruction cases and all four owner sheets. ASan/UBSan repeats these cases; packaged Android probes verify 22 nonlethal/lethal copied-state outputs. Actor/session/debug facts are supplied; tutorial/audio/party/achievement services are omitted. The application adapter below executes core kill writes for its limited monster path.
- Live monster result application: native offline, single-player, nonplayer application preserves combo, ordered health/leech writes, core IsDead/HP/lifecycle writes, result suppression on death and ordered aggro/status requests. All owner sheets, result/state/output words and observed service order match 6,914 original cases, including 18 persistent melee hits. Android's authored events execute those hits against explicit development targets and select three original death clips. Rotation and background/resume retain HP and event state without replaying hits. Full AI/death FSM, actual aggro/status services, rewards and FX/audio remain pending.
- Prince touch combat: the KnightPlayerBase fallback initializes all four owner sheets through the verified normal class/vitals helpers. Original player-attacker application matches 2,732 cases with an empty critical-skill manager and external achievement/services scope. Nine original stationary combo clips bind and skin through 3,475 host poses. The Android touch test walks down the left stairs, crosses their connected bottom and approaches a right-stair skeleton. All 112 attacks match original results, health/combo/death and RNG state through the automatic original death animation. Frozen/busy/out-of-range inputs, rotation and pause/resume checks pass. A single Attack tap plays the whole three-combo sequence against the nearest living target within the current native range policy. Original targeting/line-of-sight/approach/input timing, skills, equipment, blending/root motion and full player lifecycle remain pending.

- Player health/death: native nonplayer-to-player application matches 2,969 original cases, including 73 persistent melee attempts. Original idle hurt/dodge result mutations, low-health hysteresis and saved block/evade/knockdown counters are preserved. The original Knight Died clip passes all 1,400 host poses. Android executes 24 attacks from each of skeleton/slime/ghost through low-health warning state and automatic Prince death. HP/MP are displayed from native properties; defeated movement/attacks are blocked and renderer recreation preserves health/death. That isolated checkpoint used explicit development targets. Native standing melee now acquires the Prince automatically; complete AI, actual status/warning-audio/stat/achievement services, full player FSM and revive/game-over remain pending.

- Aggression table module: native Set/Add/Clear and Get/Highest/Count/Has/IsAggroed preserve reciprocal incoming/outgoing relations and original guards and threat selection. The standalone oracle and both packaged ARM64 builds pass 6,114 original-instruction comparisons; ASan/UBSan replays the records and atomic capacity/input checks. Finite values and Set bits are exact; arithmetic NaNs are compared by unordered class without sign/payload parity. That checkpoint compiled the module into the APK without renderer integration; the native enemy melee checkpoint below now maintains live threat relations. Callback backends, acquisition, target/range/visibility/decay rules, pursuit and complete AI/FSM remain pending. Fresh world checks pass after recovery from a system_server WindowManager/display watchdog.

These are development checkpoints. The APK bundles the current preview, Prince, room and object resources, original character/model tables, generated descriptors and provenance. It supports walking and touch-controlled Prince melee in the authored level, automatic standing enemy melee, and isolated monster combat through development commands. It is not a complete playable game and does not contain the complete cache. ARM64 compilation/instruction validation and x86_64 emulator execution are verified; physical ARM64 Android execution remains pending.

## Work remaining

1. Recover original player creation/save/class/equipment producers, target selection and combat input timing; expand critical-triggered skills and original player FSM/revive/game-over services. Implement aggro/status services, full kill/death FSM, rewards and AI callbacks. Expand other animation-event branches, complete timeline/root-motion dispatch, queued state transitions, full actor lifecycle and conditional/template factories. Expand general owner-sheet class application, gear/buff producers, dungeon level assignment, save restoration and timed regeneration. Expand equipment selection, scheduling/blending, normals and skinning performance. Recover original timing, RNG initialization/sharing and rigid-vertex behavior; decor clocks are still shared.
2. Original shaders, material techniques, alpha/color conventions, lighting, camera and render ordering. Compare output against an original-device/reference capture where available.
3. Procedural level generation, rotated rooms, scene transitions and original player input/control. Native floor/selector collision, graph construction/sewing, route search, waypoints, floor registration and the path/turn/subobject coordinator now supply the live Prince source. Complete floor/cache lifecycle, live avoidance policy and enemy pursuit remain pending. Validate the new composition's actual traversal, collision and frame timing beyond the earlier checkpoint stairs/boundaries.
4. Combat, enemies, inventory/loot, quests and progression, using original symbols/data and tests for reconstructed behavior.
5. Audio, game UI, save/load and offline startup/resource services.
6. Full asset packaging inside one APK, reproducible build/signing and modern Android lifecycle/storage/input integration.
7. Complete game-flow tests, crash/rendering/resolution checks on the emulator and available physical ARM64 devices, including 16 KiB page devices when available.

Preserve the independently maintained `compatibility/` track and historical reports. New checkpoints should carry their own source, evidence, limits and reproducible tests. Do not claim that a parser/interpolation comparison proves full-game or GPU equivalence.


## Native enemy melee checkpoint

Original comparisons verify 23,304 AI range/faction/target-event cases in each
ARM64 build, plus 6,114 aggression regression cases and sanitizer audits.
The automatic fight checks 24 enemy attacks through Prince death. An isolated
112-attack Prince regression checks threat amounts, rounded deltas, reciprocal
relations and GL recreation retention. Ten world cases are replayed.

Automatic targeting/standing attacks are enabled
in the Crypt scene. Original AI/faction data, 3D melee/sight rules and target
transition decisions are reconstructed; live threat relations are maintained.
The isolated combat/animation harnesses use `--ez enemy_ai false` explicitly.
The automatic enemy test supplies no development target/state command. Original
spatial query filters/order/timer, pursuit/pathfinding, full FSM/attack delay,
callback ordering and status/audio services remain pending. No physical ARM64
phone has been tested and the full native game goal remains active. See
`../game-data/reference/ai-target/NOTES.md` for the precise current contract.

## Native graph search checkpoint

- Original specialized findNode/markNode and SGI heap ordering reconstructed:
  1,084 requests, 96,279 node records and 101,090 predicate calls per ARM64
  binary, with zero mismatches. Sanitizers additionally exercise all 256
  authored Crypt requests through the actual native floor-loader graph.
- Native Android startup probes all 64 ordered floor pairs successfully and
  matches the original result/statistics checksum `5454073390c7d916`; the
  returned 1,598 edge segments are development probes, not actor movement.
- Next: original PFWorld endpoint selection and temporary graph nodes/edges,
  radius/obstacle validity, full FindPath, smoothing and pursuit/controller.
  Full metadata/lifecycle, generated levels, UI/audio/saves, full cache bundle
  and physical ARM64 verification remain pending. Goal remains active.

## Native world-coordinate search checkpoint

- PFWorld::_SearchGraph, world/room first-hit floor traversal, midpoint endpoint
  selection, PFObject radius/capability predicates, direct edges, failed-pair
  cache and tail trimming match 525 original cases per ARM64 binary.
- The sanitizer asset-loader audit replays the same inputs, outputs, direct
  coordinates and cache contents. Live Android startup verifies 64 successful
  world-coordinate floor-pair routes and checksum `3a3ab2a724cc383b`.
- Next: complete original FindPath, smoothing, cache invalidation/lifecycle,
  waypoint vector and moving controller/pursuit. This original wrapper adds
  no temporary nodes to the shared graph. Obstacles, generated-room lifecycle,
  UI/audio/saves, full assets and physical ARM64 checks remain unfinished.


## Native FindPath and waypoint checkpoint

- Original FindPath with debug timing disabled, first-portal smoothing,
  CalcWaypointVec, IsPastWaypoint, MovePath, DropPath and GetPathLengthSQ are
  reconstructed in source. All three ARM64 binaries match 366 complete
  FindPath requests, 661 intersections and 4,509 lifecycle operations.
- Sanitizer audits pass through the real Crypt asset loader. Android probes
  all 64 ordered floor pairs, including route replacement, and matches state
  `9356b419cae2bc57`. This probe does not move characters.
- Next: original ValidatePosition/ValidateDirection and obstacle response,
  PFObject flags/radius/init producers, failed-cache invalidation, character
  movement controller and moving enemy pursuit. Waypoint stepping supplies a
  target; it does not implement positional motion. Full game flow, rendering,
  UI/audio/saves, asset bundling and physical-device checks remain unfinished.


## Native floor motion checkpoint

- Original floor/room/world heights and normals, ValidatePosition and
  floor-boundary ValidateDirection are reconstructed. 2,353 original cases
  per ARM64 binary include cached priorities, strict limits, special-inclusive
  height queries, capability failures and accepted/rejected edge slides.
- Actual-asset sanitizer replay and all prior path/search regressions pass.
  Android matches 64 motion probes and checksum `d60ad48039cc85c6`, while
  retaining the existing graph/world/FindPath probes and ten emulator cases.
- Next: original obstacle-parent backend, PFObject InitObject/InitObstacle
  producers, obstacle force/avoidance, cache lifecycle and character movement
  controller/root motion/pursuit. Characters still use supported-floor movement.
  Full rendering/game flow/UI/audio/saves, asset bundle and physical ARM64
  checks remain pending.

## Object initialization and obstacle-parent source

Recovered InitObject/InitObstacle and the observable ordered per-floor registry,
including its actual attachment to position validation. The 64-pair startup probe
is distinct from actor movement. Next recover force/AvoidObstacles and GameObject
capability/radius/obstacle producers, then integrate the character controller and
moving pursuit. Full authored/procedural game flow, UI/audio/saves, full asset
bundle, original GPU behavior and physical modern ARM64 validation remain pending.

## Native obstacle force and avoidance

Recovered original ordered force accumulation, steering, multi-obstacle turn
limits and concrete PhysicalObject collision filtering. All three ARM64 binaries
match 2,008 requests; sanitizers replay the complete corpus and previous navigation
regressions. Android verifies the eight-floor probe and ten emulator cases.
Actor movement does not use the new avoidance yet. Next reconstruct GameObject
capability/radius/obstacle and physical producers, cache lifecycle and the moving
controller/root motion/pursuit. Full authored/procedural game flow, UI/audio/saves,
complete asset bundle, GPU parity and physical ARM64 checks remain pending.

## Concrete obstacle producers and UpdatePFObject

Recovered the five concrete obstacle radius/strength groups, physical-to-game
radius conversion and GameObject's registration-before-radius update. Three
ARM64 libraries match 1,238 original comparisons. Actual-asset sanitizer replay,
complete navigation regressions, eight-floor Android producer probe and ten
emulator cases pass. These methods currently run in the startup probe.
Next: original body/filter/bounds and capability/initialization producers,
cache lifecycle and moving controller/root motion/pursuit, then integration into
actor movement. Full authored/procedural game flow, UI/audio/saves, asset bundle,
GPU fidelity and physical ARM64 checks remain unfinished; the goal is active.
