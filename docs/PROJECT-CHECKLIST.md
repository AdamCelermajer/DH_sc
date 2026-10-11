# Dungeon Hunter 2 — project completion checklist

Updated: 2026-10-11. Branch: `reconstruction/item-world-runtime-2026-10-07`.

Latest source round (not in R4):
- [x] Restore SWAMP character resource closure: four models, 20 missing animation clips and one atlas; all 133 animation-provenance inputs match packaged bytes/hashes (`8032f542`). Gameplay validation remains open.
- [x] Preserve manager-owned FX handles after pool return so repeated objective-marker removal/reinstallation follows the source lifecycle; focused regression passes (`aad90a92`).
- [x] Build `a1ac6b9f` for ARM64/x86_64 and verify intro → main menu → new Warrior → SWAMP world/HUD on API 37/16 KiB. Fixed repeated identical menu frame labels that aborted assertion-enabled builds. Android combat and audible cinematic output remain unverified.
- [x] Connect authored Faery selection to the canonical Save/skill and Level-placement providers, with source-selected BDAE/Idle rendering. Shared Windows target compiles; full Faery factory/AI lifecycle and live selection remain open.
- [x] Restore local-player starter-slot initialization for loaded Saves with empty slot maps; retain source CharacterList-based interaction selection and activated-enemy target eligibility. Shared Windows target compiles; fresh starter skill 7/level 1 is logged. Restored-empty grant and merchant checks remain open.
- [x] Reconstruct Irrlicht finite/infinite/orthographic projection and GLES depth conversion (`c8610338`); recovered perspective chain now selected by the shared renderer. Camera host checks and Windows build pass; nine aspect ratios match prior coefficients bit-for-bit.
- [x] Verify selected quest-owner libraries: 5,552 host checks, 384 actual quest instances, including authored MoveInZone/OpenGameObject delivery. Live campaign progression remains open.
- [x] Build Windows SDL gameplay-audio output; three Crypt frames pass with dummy audio. Audible output remains unverified.
- [ ] Verify source merchant inventory, buy/sell transactions and NPC death integration in gameplay; current implementations remain in progress.
- [x] Reach SWAMP startup on Windows: nine modules, four Monsters and one merchant load; 69 original assets restored. Source instance-material overrides fixed (`constructController` 0x61ace8); SwampKing regression passes. Three frames render player/world/HUD; combat and Android remain unverified.
- [x] Retain source `isGlobal` through the exact MGP census and canonical ObjectManager; focused 50-row SWAMP census passes. Source Monster movement facts and animation-speed timing compile; gameplay movement remains unverified.
- [x] Verify Windows frontend framing with the source-selected SWF and seven restored original textures: Start/Options/Info fit after the entrance animation (60/120 frames). Remaining ornamental overflow is authored outside the stage. Audible output remains unverified.
- [x] Fix canonical static-Character SCT ownership and verify a Windows Crypt kill through normal input: cultist07 HP 12160 → 3711 → 0, dead=1, exit 0 with rendered world/player/HUD. XP providers, loot tail and ordinary enemy aggro remain open.
- [ ] Connect TalkToNPC objective markers to canonical Character `+0x2fa`: source Register/Install sets it, Unregister/Remove clears it (`0x47dbb4`, `0x47d998`, `0x47dc44`, `0x47daec`). Generic quest-NPC interaction remains incomplete.
- [ ] Clear current Windows combat gate: the combined runtime builds and renders Crypt/HUD; first aggro fails at `monster.luac:68` (`HeadTo`, status 205). No attack/death/XP/loot verification for this batch. [Receipt](../port/desktop-native/reports/crypt-combat-snapshot-2026-10-11.json).

**Goal:** a complete, source-built native Android game, preserving original
gameplay/content and providing documented fan modding. **The game is unfinished.**

**Latest local APK:** `outputs/android-next-build/artifacts/Dungeon-Hunter-2-native-a1ac6b9ff24cf597-debug.apk` (249,421,537 bytes; SHA-256 `55F3393DC8D3551B7245A9D04B12457AE92129B5DB81915818145EC1D0AC1E00`). ARM64/x86_64 build passed; API 37 / 16 KiB intro → menu → new Warrior → SWAMP world/HUD passed. Android combat and audible cinematic output remain unverified. Local, unreleased. Earlier R4 evidence remains in its [receipt](../port/android-native/reports/native-combat-ui-r4-2026-10-11.json).

- [ ] Verify reported regressions: cinematic/loading borders and sound, selected-character preview, attack/ability icons, aggro and hit-triggered blank rendering. R3 includes saved-GEAR appearance, enemy HUD and combat callback fixes; preview still samples a static idle pose. Combat remains unverified. Allocator seed 4 is a reconstructed key range, not original menu cursor parity.

Current source: early localization initialization, animated saved-gear preview, Monster home-return/floor placement and template population queries are implemented; R4 compiles them. Later source restores Crypt's exact ObjectManager handle ledger and fixes Level-field invalidation during actor initialization. The Windows executable shares Android's runtime source manifest and reaches generated Crypt through canonical Save creation/assignment/start: seven rooms, 26 monsters and the authored HUD. Three frames exit successfully, with world/player/HUD visually confirmed in corrected RGBA readback. Combat remains unverified. These later changes are not in R4. [Desktop receipt](../port/desktop-native/reports/crypt-startup-2026-10-11.json).

**Latest published APK:** [Native UI, camera and movement — Android API 37](https://github.com/Noamcelermajer/DH_sc/releases/tag/native-ui-movement-api37-2026-10-08) ([direct APK download](https://github.com/Noamcelermajer/DH_sc/releases/download/native-ui-movement-api37-2026-10-08/Dungeon-Hunter-2-native-ui-movement-api37-debug.apk)). This debug checkpoint is still an unfinished reconstruction. New-save inventory starts empty; skill-save and skill-confirmation Back callbacks remain incomplete. Live item drop is unverified in this release; the unpublished branch has a Prince-only V4 drop/pickup path, also not live-tested.

Unreleased follow-up: API 37 class-0 Equip/Unequip and Back/HUD smoke passes; details are in `.checks/quick-ui-equip-inplace-20261010/menu-ui-runtime-smoke.json`. Camera pinch-to-pan resumes the remaining finger per IDA ZoomHandler; focused Java regression passes, live SWAMP check pending. The Character factory now gates InitPost on explicit source-property readiness and InitFinal on InitPost; host regression and Android arm64 library build pass. Production providers/gameplay wiring and Faery visuals remain missing.

Prior live evidence applies to the previous APK from [commit `d4142762`](https://github.com/Noamcelermajer/DH_sc/commit/d4142762): API37/16KiB emulator tests passed all three class create/reopen/Back/Home-resume flows. A bounded Crypt smoke verified six player hit events with enemy AI disabled. A separate live exchange verified touch movement, seven enemy hits, and a 57-damage ordinary player attack. Neither run tested loot or progression. [Live exchange](../port/android-native/reports/live-crypt-ai-player-combat-62557f03.json), [menu](../port/android-native/reports/menu-ui-runtime-smoke-62557f03.json), [combat](../port/android-native/reports/character-combat-smoke-62557f03.json).

The earlier [quest-startup build and source capture](https://github.com/Noamcelermajer/DH_sc/releases/tag/native-quest-startup-2026-10-06) remain available; its death/pose and skill/buff regression receipts apply to that earlier APK.

## How to read this checklist

- `[x]` means the specific stated task is verified. Its scope matters: a tested
  source function does not establish a finished gameplay system.
- `[ ]` means required work remains, including work implemented locally but
  still awaiting integration or its final test.
- Source components and live gameplay have separate sections. All known
  completion requirements are listed; new defects discovered during playtests
  should be added here.
- Checkbox totals and source size are not a game-completion percentage.

## Overview by system

These counts refer to the scoped tasks below. Checked source components can
still require live gameplay integration. All final completion gates remain open.

| System | Verified tasks | Remaining tasks |
|---|---:|---:|
| Inputs, Adam's work and research | 12 | 3 |
| Native Android build and setup | 8 | 4 |
| Rendering, resources and animation | 13 | 6 |
| World, physics, navigation and factories | 37 | 9 |
| Character properties, equipment and state | 36 | 8 |
| Lua, skills and enemy AI | 54 | 15 |
| Combat, death, loot and progression | 25 | 12 |
| Quests, campaign, UI, audio and saves | 47 | 11 |
| Fan modding and source delivery | 3 | 6 |
| Final completion gates | 0 | 9 |
| **Total scoped tasks** | **235** | **83** |

Prior verified source gate: [Loot composition and world-pickup host report](../reports/reconstruction-2026-10-07/loot-world-gold-host.json): 173,967 selected-library checks pass across presentation, V7 loot, fixed/random/nested AddLoot and Type 13 `Gold_01`. Row 124 matches source item/value/RNG; gold pickup credits V4 wallet gold and retires the same staged item. Debug, text and `AddPower` callbacks are controlled fixtures; full `AddLoot` and Android gameplay are not claimed. Its `:app:assembleDebug` succeeded for ARM64/x86_64; the APK contains both native libraries and `crypt01.spwn`.

Current branch projects the supported Player melee kill edge through dead/HP-zero and active-Player-killer guards into V4 DropLoot staging → original ItemAudioVisual/BDAE draw + Box2D sensor → deferred MoveOn pickup. It does not yet run complete Character::Kill. IDA confirms AudioVisualID is ItemRecord word 21 (`ItemObject::InitAgain` reads `ItemInstance::GetItem()+84`); the earlier word-24 lookup was a blocker and is corrected. Failed one-shot loot continuations retain the killer identity across world-update retries; partial V4 suffix items are removed by item identity before another roll. ARM64/x86_64 Android build and package checks pass. Live loot/pickup remains unverified.

2026-10-09 IDA-backed continuation: Monster target-out-of-range event 14 now creates per-actor `PathTo` state and advances outside drawing; GatherLoot pickup checks the same canonical V4 inventory’s registration list. Level `GSLevel`/`EventManager` delivery and objective registration remain unconnected. Skill and spell HUD actions now route through the retained runtime and source controller gate; nonempty live use remains unverified. IDA confirms animation-selection event `0x26` is not the frame-step callback; the separate callback runs after a validated step is stored. The authored CameraTests rig is loaded and selected in source; live SWAMP camera routing and zoom/movement interaction remain unverified.

Player menu teardown now follows IDA's `GoToMainMenu -> RemoveAllPlayers -> Character::~Character` boundary: retire borrowed skill/cast projections without Blur, release timer storage, and clear the reusable Character state. Source-only change; no new build or runtime validation.

Movement/camera reconciliation: IDA confirms the fixed 45° transform matches the authored HUD joystick; camera-relative gamepad math is separate. Crypt remains on the tested source projection. SWAMP CamAnimSet Default resolves through the source row and supplies the selected idle asset; all four referenced BDAEs are now restored in the Android original-cache package. Template/shake/crit composition and live SWAMP camera verification remain open. The 20:9 surface mapping is retained. Menu start carries the selected class/profile into the canonical Character and source `LevelList`; fresh saves use SWAMP row 41, while Crypt row 23 is a debug override. Live click-to-world remains unverified. 2026-10-11: joystick now uses the source touch-down origin and circular clamp (11 Java checks); original menu mode 0 uses full-surface independent-axis scaling, now shared by Android render/input paths (26 assertions, ARM64/x86_64 objects and fullscreen guard pass). APK rebuild pending.

2026-10-11: `NativeSaveGame` now saves through the canonical profile transport, then applies the three original trophy conditions (17 host checks). Player aggro/music callbacks borrow the existing AIS counters and audio queue; ARM64 objects pass. Named camera selection now follows LevelConfig through the authored scene node; camera/minimap host checks pass. NPC Kill composition and null-killer ordering compile in the selected ARM64 library; live Kill providers remain incomplete. The factory/Faery bridge passes its host lifecycle check but is not connected to the authored Faery owner. Combat ObjectiveList dispatch supports kill/count/template routes; canonical CompileQuests once/force guard passes an isolated host check. Complete population/event bindings remain pending. Consolidated APK builds and menu-to-SWAMP entry passes; these gameplay side effects remain unverified.

Latest native loop work: original 69-row TrophyTable and saved AutoTransmute settings now bind to the shared runtime. AutoTransmute transfers through canonical V4 (including stack merges) without firing the normal GatherLoot tail; normal pickup now applies property 223 and local pickup/full-inventory trophy checks. API 37 APK builds for ARM64/x86_64 and launches; item pickup, transmute, nonempty skill casting, faerie casting, and complete quest progression are not yet live-verified.

Historical [quest compilation and payload gate](../reports/reconstruction-2026-10-06/quest-payload/validation.json): selected SaveLoad masks 2/4 pass 9,659 host checks with nonempty SKIL, FAES, QEST and typed PROP on one Save/PropertyState; a FAES count mismatch still reaches QEST. FAES passes 74 original-ARM differential cases. In the current smoke, mask 1 attaches a metadata-only profile before `SG_Load(4)` dispatches its eight section requests and registers callbacks; all eight mask-4 payloads are absent. Full InitPost, existing-profile payload restoration (including GEAR), and quest-world callbacks remain open.

2026-10-10 continuation: IDA confirms `Character::SG_Load` forwards mask 4 and `_Load` skips eight section callbacks when Save is null; focused host checks pass. The shared CharAI queue, SpawnGroupManager/`InitSpawned(Char)` and paired ObjectManager-map/CharacterList registration kernels also pass focused host checks. API37 menu → Crypt → occupied-slot reload → Home/resume → terminal Back now passes with retained CharAI/CSSkill owners cleaned up. Crypt still lacks live SpawnSpot records and has only a zero-quantity TEST spawn group; equipment/loot/skill gameplay and progression remain incomplete.

2026-10-10 earlier snapshot: fixed CSSkill projection rebind; connected Crypt Player map/list registration; corrected inventory quantities; routed automatic pickup through V4; preserved authored FaeryList ID; added same-Save QEST serialization and PlayerManager class-count fix. That APK failed before World ready; it is superseded by the local build recorded above.

2026-10-10 follow-up: IDA confirmed item interaction radii remain in source position units; removing the port-only `>10` cap fixes BattleAxe's 220×220 no-colbox bound (radius 165; focused host check: 15). The API 37 Crypt run queued ambient UID 486 and logged `DH2VxnMusic` mixer active. Combat and loot were not tested.

Evidence and Adam comparison: [combined status](COMBINED-RECONSTRUCTION-STATUS.md).
Earlier frame foundation scope: [source frame ownership checkpoint](SOURCE-FRAME-OWNERSHIP-CHECKPOINT-2026-10-05.md).
Historical reports retain their original APK identities and test scopes.

## 1. Inputs, Adam's work and research tracking

- [x] Locate the local APK and complete cache; verify the complete cache inventory.
- [x] Recover original symbols, assembly, decompiler exports and Android glue.
- [x] Correct six ARM EABI float-helper prototypes in Ghidra 11.0.3; add the [1,424-function cutoff-free caller overlay](../recovered/native/decompiled/eabi-float-helper-overlay-2026-10-06/README.md) while preserving the archival export. The remaining 126 callers are unresolved.
- [x] Recover 219 original plaintext Lua scripts and preserve baseline bytes.
- [x] Pin Adam's baseline `45c5348e` and updated main `c3ae7973`; preserve import attribution.
- [x] Reconcile latest Adam `791e961b` code and milestone documents with our selected libraries; preserve compatible reuse and identify duplicate-owner/deferred work.
- [x] Import Adam's seven core modules and script runtime, preserving attribution.
- [x] Reconcile incompatible scene, world, triangle and exported-symbol interfaces
  so the imported modules can link with the existing reconstruction.
- [x] Publish the comparison of our work with Adam's work in the combined status.
- [x] Publish original-address ledgers: Adam's 1,455 addresses and the current
  551-range extension (376 additional unique addresses); 1,831 combined unique
  addresses. These are evidence reach.
- [x] Separate recovered evidence, maintained source, tests, dependencies and assets
  in the reproducible source inventory.
- [x] Audit the other public branches; selectively import 23 useful engine
  evidence/tool files. Verify 1,733 original range hashes and replay the BRES
  corpus. Untested source candidates remain separate; see the [branch audit](BRANCH-AUDIT-2026-10-05.md).
- [ ] Map every remaining required game/engine behavior to an implementation and
  record unresolved layouts, indirect calls and ownership.
- [ ] Resolve remaining unavailable evidence/reference links and conflicting interpretations.
- [ ] Audit the final implementation ledger against the complete game, rather
  than treating decompiler output as compilable reconstructed source.

## 2. Native Android build and development setup

- [x] Install the Android SDK/NDK/JDK and testing dependencies.
- [x] Run the modern Android 17/API37 development emulator with 16 KiB pages.
- [x] Build the Crypt app's game/engine code for native ARM64 and x86_64.
- [x] Verify ELF64 libraries, 16 KiB ELF/ZIP alignment and APK signatures.
- [x] Run the source-built development app without the original ARM32 game library.
- [x] Publish a downloadable APK and its exact raw compiler-input source archive.
- [x] Freeze the camera APK with 468 compiler inputs per ABI, 508 source/build archive entries,
  18 ELF64 libraries and Android API37/16 KiB evidence. Capture is post-build;
  Git publication matches tested bytes except documented line-ending conversion.
- [x] Keep milestones on the separate reconstruction branch and private documents outside Git.
- [ ] Make the final complete-game build reproducible from a clean checkout with
  documented asset installation and dependency setup.
- [ ] Consolidate the two development runtimes into the final game architecture,
  with one owner for each update, physics step and rendered surface.
- [ ] Validate the finished game on a physical ARM64 device running current Android.
- [ ] Produce final release packaging, installation instructions and reproducible artifacts.

## 3. Engine resources, rendering and animation

- [x] Reconstruct and differentially test bounded engine math functions.
- [x] Reconstruct BRES memory/subfile access and whole-buffer relocation behavior.
- [x] Decode the cache's checked mesh, animation, image, effect and material records.
- [x] Decode all 234 PVRTC BTEX images and compare RGBA output with the reference decoder.
- [x] Implement source animation sampling, transforms, skinning and animation-bank consumers.
- [x] Register the Prince's complete authored bank in the native development runtime.
- [x] Render the Prince, Crypt scenery and textured Ghosts in the native Crypt app.
- [x] Feed four Prince warrior skins and shared Idle/Move playback through the Irrlicht preview.
- [x] Fix observed actor placement/camera clipping and the broad black bridge overlay regression.
- [x] Implement bounded source material/technique/blend/depth mappings and record remaining gaps.
- [x] Close the full frustum graph: 80 intersection/77 composition ARM cases; nine actual Android snapshots match. Original camera transform/scene producers remain open.
- [x] Correct VoxN framing/tag parsing; verify 17 audio files. Decoding/playback remain open.
- [x] Wire 18 source-catalogued Player PCM WAV cues through the shared Android effect owner; build/runtime verification pending. VoxN music remains unsupported.
- [x] Reuse Adam's GFNT/viewport code; 8,532/5,200 fixture comparisons pass with sanitizers. Native gameplay UI binding remains open.
- [x] Match MenuFlash2DCamera's full-surface bounds for front UI and backdrop; 20:9 input mapping fixture passes on Android 17.
- [ ] Package and verify the intro crop and gameplay overlay anchors in the full 20:9 APK.
- [ ] Complete the original custom Irrlicht/`glitch::` rendering behavior and ownership.
- [ ] Complete all material techniques, lighting, effects, transparency and shader-state selection.
- [ ] Complete all animation states, mixing/layers/transitions/events and visual-state synchronization.
- [ ] Finish remaining resource formats, external/split loading, dynamic controllers and unsupported payloads.
- [ ] Restore original camera behavior, full actor equipment visuals and all game presentation states.
- [ ] Verify visual fidelity across every supported level and actor type.

## 4. World, physics, navigation and object factories

- [x] Load the original authored eight-room Crypt layout into the native app.
- [x] Parse both current Crypt MGP SpawnPoints into SPWN; fresh Android start selects ID 0 and floor-snaps, while resume preserves saved pose. Host checks and both ABI builds pass; active-object ordering remains open.
- [x] Import SWAMP row 41's original `.mlx`, nine MGPs and nine MVPs at runtime to rebuild DWLD/SPWN, five direct-monster DACT rows and ten static Decor instances sharing one BDAE with source-selected node roots. DACT matches byte-for-byte; both focused host audits pass; API 37 ARM64/x86_64 APK builds and signature verifies. Conditional/animated objects, other source rows, full Character lifecycle/AI and live rendering remain open.
- [x] Package all 35 active Crypt rule MGP/MVP references from the original cache with source path, size and SHA-256 provenance; latest APK byte-checks all 78 staged Crypt module files.
- [x] Add the eight-module Crypt backup-MLX root-bounds registry to the Android world-load path. Host probe: 146 scene nodes/127 geometry instances; API 37 ARM64/x86_64 build passes. This does not create Module/RoomZone owners, source memberships or procedural layouts.
- [x] Validate generated Crypt root bounds from actual generated MLX and `crypt.bdae` against generated module identity/order and DWLD. Seed `0xC0FFEE`: 7 modules, 132 scene nodes/116 geometry; host test and Android ARM64/x86_64 native builds pass. This bounds registry is now consumed by the direct-Character RoomZone/manager adapter.
- [x] Parse all 21 Crypt MGX definitions, retain 35 source-float exits/cells and derive 160 directed candidates; host fixture passes 395 checks. MGX-only placement passes 22 checks; MGP/MVP do not determine the footprint.
- [x] Audit 22 active Crypt rule entries: 21 exact triples resolve to listed MGX plus referenced MGP/MVP/MVX; MGX links and MVX geometry roots parse and agree.
- [x] Verify the selected-library DACT/world actor regression resolves the five source-authored SWAMP Monsters and model dictionary entries.
- [ ] Verify actor animation, AI and combat in gameplay. The generated Crypt loads 18 direct Monsters and confirms player movement; ambush Monsters remain gated. Completed Died animations now run retained AIS OnEndOfAnim then same-Coordinator event 0x22; timer event 0x2e enters the bounded CSDespawn state 2. Live lethal gameplay remains unverified.
- [x] Parse source `floortypes` with IDA-confirmed duplicate/key/quote behavior, apply native type masks, and make default floor snapping skip void/wall in source order; unknown tags such as `sand` add no mask.
- [x] Resolve 97 of its 166 object records: 84 scenery objects and 13 monsters.
- [x] Reconstruct bounded navigation graph/search/path/smoothing/avoidance and floor producers.
- [x] Connect a real owned player body, root motion, physics and navigation in development runtimes.
- [x] Test movement, validated boundary sliding and same-process resume in the Irrlicht SWAMP slice.
- [x] Reconstruct bounded Character factory, group, respawn, spawn and ownership kernels.
- [x] Connect original GhostAmbush01 touch contact, timed Wait/Spawn commands and two real Ghost bodies.
- [x] Implement source Limbus/Spawn/Idle visibility behavior and continue hidden actor timers.
- [x] Own a flat native Character list with 14 stable nodes; test enrollment/removal/cursor behavior.
- [x] Compare the complete bounded TargetList query with original ARM execution and fix callback order.
- [x] Reconstruct Character::IsZonable against original ARM behavior and compose
  real Ghost classification for native diagnostics; room enrollment remains open.
- [x] Reconstruct the bounded GameObject::Stop caller; verify 246 original ARM
  cases, virtual physics-policy selection and ordered partial effects. Native
  native frame invocation remains open.
- [ ] Finish the full ObjectManager factory, exact-name map, group membership and teardown.
- [x] Add a tested rollback-safe registration transaction across the existing
  ObjectManager map and CharacterList projections. It accepts borrowed canonical
  actors only; live factory/menu integration remains open.
- [x] Reconstruct ObjectManager's per-object dispatch slice: 30 ARM cases/101 guards. Full traversal, deletion and providers remain open.
- [ ] Resolve Crypt's remaining 69 conditional/script/template/factory records.
- [x] Compose source Stop with real Box2D; 14 cases include callback reentry and retained partial effects. Native frame binding remains open.
- [x] Reconstruct zoning/visibility setters and synchronization; 1,399 ARM cases pass. Native room/scene services remain open.
- [ ] Resolve weighted-template actors and the separate GhostAmbushHallway spawn path.
- [ ] Finish environment bodies, collision ownership, all module seams and other object types.
- [ ] Finish all trigger/script commands with real native services.
- [x] Snapshot all 15 supported `gDistributions` rows and add the Path length/direction helper. Focused host tests: 57 distribution checks, 33 Path checks; all 2,412 row bytes and 15 terminators match IDA.
- [x] Implement the host RootRule and recursive `Rule::Impl::Step/OneStep` executor; 18 focused checks pass, including default child `ListElem`, per-candidate Path exits and rollback/retry order. Android rule callbacks remain unwired.
- [x] Model source Array2d occupancy, growth, placement order and rollback; 26 focused host checks pass.
- [x] Add host ObjectManager/RoomZone kernels and bind them in Android for generated Crypt direct Characters using borrowed actor fields; focused host tests and ARM64/x86_64 build pass. DACT actors use source ObjectManager handles from ordered LevelConfig/Module roots and MGP-before-MVP loads; first-name-wins removes duplicates. Seed `0xC0FFEE` audit passes (16 retained Monsters, 36 retained AnimatedDecor, 39 duplicate visual candidates). Generated AnimatedDecor renders through the existing static BDAE root path; authored animation/update lifecycle remains open. Six-plane transitions gate direct actor visibility/updates. Original camera production and other factories remain open.
- [x] Serialize source-shaped `LevelConfig` plus flat preorder Module GameObjects and DWLD v1; preserve Crypt rule-root overrides over IDA-derived defaults and ignore undeclared properties like native PropertyMap (23 layout checks, 60 parser checks, ARM64/x86_64 NDK compile).
- [x] Compile generated Crypt SpawnPoints from source-ordered module MGPs to SPWN v1; fixture IDs 0 and 2 preserve module/order/transforms. Conditions, scripts and duplicate IDs fail closed.
- [x] Check custom BRES renderer roots for Crypt: 20/21 source MVX roots exist; missing `entrance_s` is unreachable from the current RootRule. A generated one-module `cemetery_entrance` DWLD loads and yields 73 floor triangles with authored spawn.
- [x] Connect row-23 Crypt rules/catalogue and resolved seed to Android generation; pass generated DWLD/SPWN and supported direct-Monster DACT records into `load_world`. API 37/target 37 APK builds for ARM64/x86_64; signature/alignment pass. Live API 37 run loaded 7 rooms and 18 Monsters, then accepted player movement.
- [x] Preserve the authored Crypt `templateName=Faery` Character descriptor and bundle the five DefaultFairy fallback models, four AnimTable 23 clips and six referenced textures from the pinned cache. ObjectManager/Character/AI activation and final placement remain open.
- [x] Port `Level::PlaceFaeryAndFollowers` as a source-ordered kernel over borrowed Character/Player services. Android providers, an instantiated Faery, and live selection/placement remain open.
- [ ] Rebuild the original camera producer and static module visibility; add remaining factories/conditions/scripts and complete the object update lifecycle. Port-side RoomZone frustum transitions already gate direct Crypt Characters.
- [ ] Connect authored and generated levels to the actual level stack and loading lifecycle.
- [ ] Complete exits, level transitions, hubs, fast travel and return-to-level behavior.

## 5. Character properties, equipment and state

The bounded Adam item-system decisions and remaining native integration are in
the [branch-audit reconciliation](BRANCH-AUDIT-2026-10-05.md#adam-reconciliation-at-c3ae797).

- [x] Reconstruct original Character property readers and typed property operations.
- [x] Reconstruct bounded base/gear/class stat composition and class recalculation.
- [x] Decode original class, item, power, health/mana, AI and level tables used by the source components.
- [x] Reconstruct bounded HP/MP setters, regeneration, mana use and damage-health kernels.
- [x] Select source saved-option query predicates: 640 original ARM executions match; full settings/profile lifecycle remains open.
- [x] Implement live native SetLevel/class/design/property/Debug providers for the two Ghosts.
- [x] Reconstruct shared Character state/timer ownership and bounded movement/attack/spawn consumers.
- [x] Reconstruct Character::_InitHpMp and test the live property/real-file adapter on host.
- [x] Reconstruct UsingSkill/Casting predicates against original ARM behavior.
- [x] Reconstruct 17 complete bounded Character AI/faction/type/flag/name/death
  bodies; pass 514 ARM comparisons and use actual native cached IDs/types for
  Ghost AIS selection. Original name/raw-death producers remain open.
- [x] Reconstruct the Character physics-position override and live flag adapter;
  verify 67,594 Character flag cases and 1,024 base-object cases against ARM.
  General Character policy/frame binding remains open.
- [x] Select V4 as the single source-style item/equipment graph; its live API
  borrows caller-owned properties/RNG. All 84 source sessions/1,720 steps and
  60 earlier regressions pass through that path: 676 draws, 3,584 aliases,
  zero mismatches. Native player binding remains open.
- [x] Host-verify V5 gear effects on the V4 owner/property graph and V7 power
  resources: 6 source sessions/112 steps across three classes, plus 121 power
  lists and 39 quantity lists. Text/debug and visual Skin providers remain
  bounded fixtures; this is not live loot or player gameplay. See the
  [selected-library host report](../reports/branch-audit-2026-10-05/adam-integrated-host.json).
- [x] Select borrowing equipment requirements/recalculation services over the same V4 inventory and buff-aware properties; 7,451 selected-host checks and both Android ABIs pass. Review fixed retirement before item deletion. Native Skin/text/HUD and final teardown remain open.
- [x] Parse the Prince modular BRES catalog into the selected engine-skinning library: 4 categories, 172 modules, 10 exact starter mappings and exact placeholder/naked fallback; focused target host audit passes. This resolves assets only.
- [x] Pass the selected Player `ClassID` (263/290/325) to class-specific world draw selection, distinct from the character-table row index. API37 ARM64/x86_64 compilation passes; no visual playtest.
- [x] Resolve restored V4 torso/feet/hands/head slots 0/3/4/8 into the existing Prince draw batches and pose graph; fresh entry keeps class defaults. Combined Android ARM64/x86_64 compile passes; no visual test.
- [ ] Add mutation-time equipment redraw against the same draw/pose owner; then check equip/unequip. No second scene or pose owner.
- [x] Route saved PROP into the same gameplay Save and PlayerCombat PropertyRules/PropertyState: 753 selected-host checks, 26 saved fields, no generic fallback, and reached-prefix retention on truncation. Live InitPost remains unbound.
- [x] Select `CharAI::UpdateSkills` (`0x3d8a04`) against the existing Save map, live FSM gates, difficulty and borrowed vector/script callbacks; five focused host cases pass. This source kernel is selected but has no live Android InitPost call site.
- [ ] Bind Character InitPost before InitScriptProcess: preserve SG_Load/GEAR/property order, map-0 slot-0→skill-row-0 setup, conditional IncSkill and pre-init CharAI callbacks through the same Save/V4/PropertyState. First connect a real profile/level sequence and provide source Skin lifetime plus Player AddLoot; current Android Player setup is the development path.
- [ ] Complete Character construction, all property sheet/buff/gear ownership and lifecycle phases.
- [x] Select Adam's V5 item presentation on existing item identities/table authority; 4,031 presentation and 1,119 power-instance gold replays pass. Native text/localization remains open.
- [x] Select borrowing weapon queries over sole V4 inventory/properties; 1,000 original cases, both equipment sets and preserved combat fields pass. Native inventory binding remains open.
- [ ] Connect full inventory/equipment mutation, requirements, random powers and visual updates.
- [ ] Complete player classes, skill progression, buffs/debuffs, auras and status effects in gameplay.
- [ ] Replace remaining development inputs/facts with the original game-owned producers.
- [ ] Validate all character types and states together in live encounters.

- [x] Select borrowed Character::_InitEquipment over the sole V4 inventory and buff-aware properties: 18 original caller cases, 2,151 host checks, three class tables, 13 failure prefixes and eight guards pass; both Android ABIs compile. Genuine profile/locality/InitPost, full AddLoot and native text/Skin remain unbound.

- [x] Select original Player/Matching locality query bodies: 141 ARM comparisons and selected-host gates pass; native unregistered fallback/null queries pass. Full PlayerInfo/NetStruct registration remains open.
- [x] Select original fixed Win32 input ownership and channel/stick updates: 325 ARM comparisons, 12 failure prefixes; both Android ABIs compile. Native registration/input delivery remains open.
- [x] Select CNetPlayerInfo lifecycle and shared scalar/string/byte-array members: 72 lifecycle and 138 member ARM comparisons, six native failure/reentry checks; both Android ABIs compile. Offline map registration and live gameplay remain open.
- [x] Select full 33-field PlayerInfo lifecycle, Reset/copy/assignment/destruction/setters and native factory backing: 150 ARM comparisons and 14 ownership checks pass through selected libraries. Both Android ABIs compile; three classes retain menu/Crypt behavior on API37/16KiB. Controller/map and source-backed Character660 registration remain open.
- [x] Bind reconstructed `NativePlayerCharacterOwnerV1` to registered PlayerInfo `Character660` and the same Save/properties through menu → Crypt → HUD → Home/resume; 12 host checks, both Android ABIs, and API37/Android17 x86_64/16-KiB smoke pass. [Smoke report](../port/android-native/reports/native-character-owner-api37-2026-10-06.json). Source `_AddCharacter` parity and inventory attachment remain open.
- [x] Select offline membership/AddPlayer/renumber/controller callers over full stable records and the sole input owner: 440 ARM comparisons, 72 failure/reentry cases and 68 integration checks pass. Native controller0 registers before authored Assign; live Warrior create/reopen/Crypt/Back/Home passes on API37/16KiB. Joining/network and source-backed Character660 remain open.
- [x] Select original GEAR reader over the same V4/property/presentation graph: 19 original cases across three classes, six cached items/one power, 2,495 checks and both Android ABI builds pass. Six original power-table assets are bundled/hash-gated. Mask 4 now runs on the gameplay Save, but existing GEAR payload loading and the full InitPost path remain open.
- [ ] Retain powered equipment split remainders through callback failure and retire presentation before item destruction.

- [x] Select PlayerInfo activity/SetState: 3,716 ARM comparisons; offline activity and state member remain distinct.
- [x] Select friendly-ID/player selectors: 308 ARM comparisons and 344 failure prefixes over the canonical registry.
- [x] Select whole `_AddCharacter`: 277 ARM comparisons and 88 native failure checks; genuine spawn/InitAll providers remain unbound.
- [x] Connect bounded normal managed metadata to registered menu players: 245 ARM comparisons, 46 composition checks and three live class flows. Class/level/name use the same Record and Save680.
- [x] Select bounded Character::InitPost caller block through `0x3b51bc`: 4 original ARM cases/37 calls match host order, callsites, owners and arguments; failure-prefix, reentry and Android ARM64/x86_64 builds pass. Providers remain unbound.
- [x] Select Character save/InitAll wrappers: 46 ARM comparisons and 115 selected-host checks; exact embedded Quest owner fields/order and same-Save LoadOwner are enforced. Android now calls mask 2 then mask 4 once on the same retained Character/Save/loader. API 37 smoke verifies identity, eight mask-4 requests after profile attachment and no replay on Home/resume; the generated profile has no mask-4 payloads. Full InitPost and existing-profile section restoration remain open.
- [x] Guard mask-4 save dispatch against the source null-Save path: profile load requests exactly eight sections; an absent profile is accepted only for slot -1, requests none and makes one offline status call. Focused host checks pass; durable save payloads remain open.
- [x] Select Character::InitFinal: 260 ARM comparisons and 803 host checks; real lighting/AI/skills/save providers remain required.

## 6. Lua, skills and enemy AI — current implementation focus

- [x] Reuse source-built float32 Lua and preserve native pointer identities without numeric narrowing.
- [x] Reconstruct staged AIS construction, association, binding order and pending/active publication.
- [x] Run both Ghosts' unchanged original monster OnInit on their real native properties.
- [x] Retain the same published VM, damaged health and source timer slots across reload/recreation.
- [x] Reconstruct bounded AIS OnInit/Post/Final caller functions and same-VM dispatch.
- [x] Reconstruct acquisition, candidate events, target/master/range/pause/state callback kernels.
- [x] Test owned Character-list → source query → Lua callback → target/path composition on host.
- [x] Reconstruct UpdateAllSkills, faery selection and CharAISkillScript construction kernels.
- [x] Reconstruct bounded `OnSkillUpdate`, `OnSkillCheck_Usable` and
  `OnSkillCheck_Active` callers; compare host behavior with original ARM execution.
  Native nonempty checks now pass; full activation remains open.
- [x] Host-test the per-VM resolved-path execution cache with 60 real Lua checks.
  Full `LuaManager::AddFile`, shared byte caching and Android wiring remain open.
- [x] Reconstruct source `Value::getBool` and its isolated real Lua adapter:
  276 original ARM comparisons, 269 host cases and 42 real Lua cases pass.
  Native full-return skill calls now pass; complete Value lifecycle remains open.
- [x] Decode all 183 original Skill/Faery list/row records; compare with original ARM readers.
- [x] Preserve authored `FaeryList` separately from its source fallback row; 12 host cases and five original ARM comparisons pass with zero mismatches. The selector is not yet wired to a live generated Faery Character.
- [x] Adapt Adam's player skill ownership to our source callers: each class has 16 skill/5 faery slots and 13 instances; 29 script names overall. Host/ARM gates pass; full Player AIS/FSM lifecycle remains open.
- [x] Select saved skill-slot access on the actual current save, preserving source map0/skill-set0 semantics; selected initial-grant regressions and Android builds pass.
- [x] Select original skill progression/slot initialization callers; prerequisite 777 ARM predicate cases and selected initial-grant 150 ARM comparisons pass. No invented free skill grants.
- [x] Compose initial grants with the same save, inventory and live buff-aware properties; 113 normal/15 failure cases and five guards pass in the actual selected libraries. Profile/InitPost/native binding remains open.
- [x] Compose Player AIS construction and separate load/InitProcess phases through the same Session/Coordinator; five lifecycle completions/four failure prefixes/seven guards/two split cases pass in selected host libraries. Phase7 alone is not readiness.
- [x] Drive native Knight initialization through those source phases; one vitals pass, 13 unchanged updates, canonical AIS flags/VM/preparation/properties and timer identities survive reload/rotation on API37/16KiB. Profile/grants/full frame remain open.
- [x] Select real RegenTick and Player33/34 dispatch through sole Coordinator traversal; 11 original traces, 33 compositions, 93 failure prefixes/five guards and existing Coordinator regressions pass. No extra FSM event/store/frame update.
- [x] Run unpaused Player AI3000ms/DoT1000ms timers live; retain counts/elapsed/repeat identities and verify MP0→741 through source regeneration. Positive DoT attack/application remains unbound.
- [x] Adapt Player callback membership and skill/spell cooldowns through the
  selected Lua core and borrowed timer fields; host/real-Lua gates and 545
  cooldown ARM comparisons pass. Additive VM protocols pass 145 host checks.
  Full native skill activation remains open.
- [x] Select bounded CharAI Begin/End/Use skill-command kernel in
  `dh2_level_world`: 16 ARM comparisons, 13 source cases, four failure prefixes,
  two guards and shared-Player-VM passive checks pass. Player activation remains open.
- [x] Add the borrowed CharStateMachine fields needed by that kernel to the
  Android-selected build; IDA/REA field mapping and ARM64/x86_64 compilation pass.
  Host regression is authored but not run; production binding and HUD activation remain open.
- [x] Add retained-VM `List::faery` OnPreSkill/OnSkill/OnPostSkill callback access and route the state-7 `do_spell` marker by the saved faery slot; both Android ABIs compile and API37 install/launch pass. Casting itself is not verified.
- [x] Wire Android state-7 entry/exit, CSCast Focus/Blur and NativeHUDSpell through the retained Coordinator, animation and Player VM owners. Latest source-only correction makes SM_SetCastState read only CharAnimTable.Spells[faeryId], as IDA does. Current source has not been rebuilt; nonempty live casting remains unverified.
- [x] Run bounded authored Ghost `LoadNInitScriptProcess(true)` through HP/MP,
  SetSkillsAndSpells, UpdateAllSkills, Post and Final in source order: 0 ordinary
  skill entries and 5 null-script faeries on the same retained VM.
- [x] Verify Ghost VM, health, vectors/catalogue backing and timer slots survive
  reload/recreation without replaying initialization or healing.
- [x] Fix terminal world discard retaining Ghost owners; verify zero surviving
  references and fresh initialization when re-entering Crypt in the same process.
- [x] Reconstruct the complete CanUpdate predicate; verify all 77 instructions,
  26 primary/348 independent ARM comparisons and sanitized pointer/failure guards.
  Compile it for both ABIs; native frame invocation remains open.
- [x] Extend the retained VM with zero-argument update and independent
  current-state callbacks; verify source wrappers and Lua mutation/error behavior.
- [x] Connect the exact resolved-path cache helper to the retained Session VM
  and compile it into Android; verify real load/hit/retry/replacement behavior.
- [x] Add the created-only retained-VM callback bridge; test constructor before
  pending publication, same-VM adoption, reset/destructor reentry guards and
  callback lifetime with independent review and ASan/UBSan/LSan. This port
  ownership adapter earns zero new original-body credit; native wiring remains open.
- [x] Reconstruct the bounded Character::Update lazy-script/concurrent-AI map
  slice; verify 18 ARM cases, all 127 reached instructions, 15 guards and
  partial effects. Preserve full-width AIS identity. Native shared-map binding remains open.
- [x] Retain one shared CharAI turn queue across actor registration, apply the
  source Monster turn gate and advance its 180 ms timer once after Character
  updates. Focused host audit and Android ABI compilation pass; full
  Character::Update gates and Ghost frame ownership remain open.
- [ ] Complete `Character::Update` scheduler/eligibility gates, the shared
  `CharAI::IncUpdateQueue`/`Application::GetDt` eligibility gates and frame
  ownership; native world setup still lacks the complete Character/Ghost frame. See
  the [checkpoint](SOURCE-FRAME-OWNERSHIP-CHECKPOINT-2026-10-05.md).
- [x] Trace original Ghost pursuit in IDA: Level/Character frame order,
  unchanged `monster` callbacks, PathTo/FindPath, the `+0x1450` home point,
  and event-40 body attachment. POMonster-specific constructor/pin policy and
  the ordinary movement/body owner remain open.
- [x] Map source event 17 to zero-argument `OnTargetInMeleeRange` and replay
  the unchanged `monster` Lua `Stop(); Attack(GetTarget())` branch in the
  retained session. Android frame dispatch and controller providers remain open.
- [ ] Wire controller `Stop`/`Attack` with live movement bytes and source
  attack-state selection (`AttackStatic` when available), then route named
  animation markers through `OnAttack`/`F_MeleeAttack` exactly once.
- [x] Reconstruct ObjectBase culling/remote predicates (2,256 ARM cases) and compose CanUpdate (126 nested ARM cases, 63 guards).
- [ ] Bind CanUpdate to actual scene/culling, visibility, player/online and
  respawn owners, and invoke it in the native Character frame.
- [x] IDA-confirm RoomZone event leaves: `ZoneEntered/ZoneExited` call
  `VisualObject::SyncVisibility` (`0x4713d0`) via `+0x2d8`, then
  `ObjectBase::setUpdating` (`0x33dcf0`) via vtable `+0x3c`; adapter names and
  fields corrected; 22 original ARM cases pass with zero mismatches.
- [ ] Connect authentic `Module::InitPost` root bounds → native `RoomZone` creation →
  `InitObjectList`/`AddInitialObject`, then source zonability, InZone and object
  ownership. Start from the packaged 8-module `x07_crypt_backup.mlx` matching
  `crypt01.dwld`; room-7 DACT rows 78/79 already own the native Ghost VM path.
  Host `crypt_room_zone_owner_v1` now models ordered bounds/enrollment and passes
  its focused test; it is not registered with ObjectManager and does not cover
  PlayerManager no-room or activation/streaming. Keep source InZone unset until
  native ownership is connected.
- [ ] Bind current-state constructor/registry/transition ownership and actual
  OnUpdate/state callbacks to the native AIS frame.
- [ ] Supply original Character name/raw-death producers for all actor types.
- [x] Load, declare and allocate the Knight's 13 nonempty original skill/faery instances
  in one retained native VM; use current properties/timers/catalogue. Reload/rotation pass.
- [x] Dispatch the original framework Bashdown cooldown through the same native timer
  and slot field18; expiry/rearm pass. This shell fixture does not execute full skill use.
- [x] Adapt the sole saved-skill owner and read callbacks; replay 64 original sessions/
  1,536 snapshots. Native fresh SkillTree rows have level0; starter grants/profile load remain open.
- [x] Select same-VM full ReturnValues and original check/use/outer-dispatch adapters;
  host and original ARM gates pass. Full native CSSkill/input activation remains open.
- [x] Run all13 original Knight/faery updates and both Bashdown/passive check returns on
  Android 17/16 KiB with one shared temporary property sheet; full activation remains open.
- [x] Adapt Adam's CurrentSpell caller to the sole native save/faery catalogue: 436 ARM comparisons, same-VM Lua tests and source five-row initialization; no invented selection/grant.
- [x] Connect original HasMana/UseMana to native properties/options/Debug: 34 host cases, 26 ARM comparisons and live MP debit/rejection/reload. Full networking remains open.
- [x] Adapt Adam's GetInt/SetInt to the retained native AIS dictionary: 440 ARM comparisons and live signed writes/insertion/reload/rotation; no second property store.
- [x] Adapt Adam's equipped-faery element query to the sole save/catalogue: 112 caller plus 180 nested ARM comparisons, selected-library tests and native Celest execution to the next provider.
- [x] Connect source equipped-faery ID/level wrappers to that same save/VM: 49 host checks, 130 ARM comparisons and complete native faery updates; no invented unlock/grant.
- [x] Adapt one source buff owner to current properties/Coordinator and run actual Celest resistance live across reload/rotation. 40 whole-state ARM and 40 same-VM host updates pass; FX and live timed expiry remain open.
- [x] Select CSSkill Focus/Blur and state6 event projection over borrowed Coordinator fields; 214 new ARM comparisons, 60 host cases and both Android ABIs pass. Native activation remains open.
- [x] Compose CSSkill state 6 into the same Character Coordinator: C355 entry, event28, source Focus/Blur ordering, elapsed reset, and close event22. Android binds this projection and HUD command path to the retained Character; live nonempty activation remains open.
- [x] Resolve `NativeHUDSkill` through the Save HUD-slot map, selected SkillList/Save row, and same-index retained Player script; route Begin/End through the same runtime and controller gate. A validated playback frame-step hook now handles deferred EndSkill loop stopping.
- [x] Map CSSkill fields: Character+0x554 is embedded CharStateMachine+0x58 `moving`; Character+0x412 is separate OOI interaction intent. Add OOI storage to the canonical Character owner with fresh-session/retirement clearing. State-6 callbacks are bound to the existing Coordinator; live nonempty casting remains open.
- [ ] Complete nonempty skills for all classes through full Player AIS construction,
  saved skill levels, `LuaManager::AddFile` and lifecycle ownership.
- [ ] Verify nonempty HUD skills through state 6, authored `do_skill` event 40, EndSkill loop stop and Blur; finish target search/LookAt/ClearTarget and exact `SkillCombatRoll` arguments. No starter grant or damaging cast is verified.
- [x] Register `NativeHUDSpell` on the separate current-faerie cast path through state 7, CSCast Focus/Blur and retained faerie callbacks. Live casting with a selected faery remains open; do not route this action through the skill-slot API.
- [ ] Complete all 265 original Character bindings and every actually used game/engine service.
- [ ] Connect native Ghost acquisition and pursuit through real frame/path/body services.
- [ ] Bind positive Player DoT attack/application and Ghost AI/DoT providers; Ghost timers remain paused.
- [x] Reconstruct same-Session skill/faery cleanup: three actual classes, 23 ARM comparisons, ordinary-error branches and required-failure prefixes; no extra VM, timer stop or instance destruction.
- [x] Select source OnDied→AI_SetDead composition: 18 ARM routes/473 pinned words, 58 failure prefixes and three real classes through the existing world/VM libraries.
- [x] Bind dead-focus Debug, CancelSneaking, buff removal and constructor-null FX paths to canonical fields/owners; 18 host cases and 3,910 selected-state comparisons pass. Nonnull FX remains required.
- [x] Verify native direct state12/null, target synchronization, timer33/34 retirement, all13 cleanup callbacks, buff removal, authored body removal and same-VM dead-state reload through a controlled fatal-hit fixture.
- [x] Remove invented per-frame UpdateAllSkills replay; original initialization/progression callers own this update. Live InitProcess/reload receipts retain exactly one call.
- [ ] Bind genuine linked-aggro/group/OnAggro producers and reached inventory-stance/nonnull-FX services for death; finish outer Character Kill/event2 routing.
- [x] Retain Player CPU playback and actual scene pose through development reload and Activity recreation without event replay: seven host boundaries, 402 exact continuation frames and nine guards pass. The API37/16KiB frozen dead-pose fixture retains exact playback/pose hashes and event count.
- [ ] Complete remaining Monster Dead lifecycle: model CSDespawn state exits plus safe object/script cleanup after Despawn_Delay; finish nonnull FX-manager services before enabling ambush targeting and testing death/loot/credit.
- [ ] Complete other AIS factories, enemy types, bosses and player AI/input behavior.
- [ ] Playtest all enemy/skill combinations and preserve original decisions and timing.

## 7. Combat, death, loot and progression

- [x] Reconstruct bounded random streams and combat calculation/script components.
- [x] Add one process RNG owner with original GSInit/Level unload writes and a borrowed service for V4 inventory/V7 loot. Selected-library checks, both Android ABIs, and API 37 menu/Crypt smoke pass; live inventory/loot and NPC consumers remain open.
- [x] Reconstruct bounded health damage, nonplayer death and kill/clear quest-counter components.
- [x] Reconstruct HandleDots/F_DotAttack and the bounded offline nonplayer F_ApplyResult caller.
- [x] Exercise supported native health changes and prove damaged Ghost health survives recreation.
- [x] Record earlier development encounter hits and diagnostic combat/quest evidence with their build identities.
- [x] Run one live current-build Crypt exchange with automatic enemy melee and normal nearest-target player attack on API37/16KiB; 7 enemy damage events and one 57-point player hit. [Receipt](../port/android-native/reports/live-crypt-ai-player-combat-62557f03.json). Equipment-derived damage and the full AI loop remain open.
- [x] Select Adam's V7 powered-loot creation on V4's caller-owned RNG: 9,931 presentation/power/loot gold replays, 363 actual powered items and 369 shared draws pass. Android compilation passes; native drop/pickup/AddLoot remains open.
- [x] Compose one original-derived fixed AddLoot entry through the same V4 inventory, V7 power append and V5 value/name path: 147,822 selected-library checks, zero mismatches; original and host each produce one power, value 770 and RNG seed 1302343 after two draws. Text/debug are fixtures; native/full AddLoot remains open. [Report](../reports/branch-audit-2026-10-05/item-loot-backbone-v7-selected-host.json).
- [x] Host-test a bounded `ItemInventory::AddLoot` adapter over the existing V4/V7 owners: powered row-5 fixture and shared RNG match; rejects mismatched properties/arguments and missing providers, retaining the pending-item prefix on provider failure (106 checks). The direct fixture requests count 1; `_InitEquipment` caller arguments, auto-equip, general callsite integration and live loot remain open.
- [x] Reconstruct loot-entry percentage classification, class-weighted probability and weighted selection; 320 original ARM32 vs Android ARM64 comparisons, 204 raw-entry checks and exact RNG states pass ([differential](../reports/reconstruction-2026-10-06/loot-entry-selection-v1-arm32-arm64.json), [host](../reports/reconstruction-2026-10-06/loot-entry-selection-v1-host.json)). Recursive AddLootItems and live drops remain open.
- [x] Add V4-owned world-drop staging outside player inventory and pickup handoff. Original LootTable row 124 (`Gold_01`, Type 13) matches the selected host item/value/RNG; pickup credits its value to wallet gold and retires the staged item ([receipt](../reports/reconstruction-2026-10-07/loot-world-gold-host.json)). Ordinary item pickup preserves ItemInstance identity.
- [x] Retain V5/V7 loot-power and presentation owners in Android PlayerCombat, borrow the existing process RNG/V4 inventory, and retain each actor's raw `Loot` ID.
- [x] Verify generated Crypt row 23, connected HUD, occupied-slot reopen and Home/resume on API 37 / 16 KiB (one Knight case; APK SHA-256 `0e325613…`; device saves restored). Live enemy drop/pickup, mask-4 payload restore and full source `Character::InitPost` remain open.
- [x] Compile the new Android death-to-item path for ARM64 and x86_64, including original assets and ItemRecord word-21 visual lookup. `:app:assembleDebug`, APK signature and 16 KiB ZIP alignment pass; APK is attached to the [item-world release](https://github.com/Noamcelermajer/DH_sc/releases/tag/native-item-world-2026-10-07).
- [x] Retry failed one-shot death-loot continuations from the world update; retire partial V4 staging by retained item identity before rerolling. Compile both Android ABIs; no live loot claim.
- [x] Feed player melee and actor stance/ranged facts from the canonical V4 inventory. IDA `F_MeleeAttack` confirms slots 1/2 and item word 37; `HasRangedWeapon` uses word 22 types 4/5. API37 ARM64/x86_64 build/package passes. Inventory UI/equip mutations and gameplay remain open.
- [ ] Live-test the guarded kill → BDAE model/sensor → deferred MoveOn pickup path. The build is available; gameplay and pickup are not verified.
- [x] Align the three current direct melee directions with positive-amount `AI_AddAggro` before `HitFor` through the existing reciprocal aggro tables; `combat_application_order_audit` passes nonlethal, lethal, no-hit and rejected cases.
- [x] Share per-Character outgoing/incoming aggro storage between combat and retained CharAI; relation-owner and seven-case cleanup host checks pass. Nonempty peer callbacks/lifetime remain gated.
- [x] Route generated Crypt `SpawnCharacter` through the exact registered ObjectManager Character and source FSM transition; three manager-backed host cases pass. Missing-actor creation and deferred scripted MGP activation remain open.
- [x] Select `Character::InitSpawned` at `0x3b379c` as a source-ordered activation kernel; focused host checks pass. Runtime SpawnGroupManager/SpawnSpot creation and hookup remain open.
- [x] Add a tested SpawnGroupManager policy kernel for timer/weighted selection,
  spot gating and the `Create Character -> InitSpawned(Char) -> PlaceObject`
  sequence. Crypt hookup remains blocked by absent SpawnSpot rows and a zero-quantity TEST group.
- [x] Carry active Player killer identity through loot retries and require the source dead/HP-zero Kill prefix before V4 DropLootTable staging; the Android API 37 ARM64/x86_64 build passes. The event4 Android bridge is built; live behavior remains unverified. XP, quests and outer event2 remain open.
- [x] Select the Player event4/credit kernel over the existing dispatcher and PropertyView: focused host audit passes callback→target clear→properties 23/24 order, FSM fallback, single-Player scope and retry suppression.
- [x] Bind the kernel to the supported Android Player melee death edge exactly once after the initial DropLoot attempt, including deferred/failed loot staging; reuse the retained CharAI/AIS/VM and canonical target/property owners. Focused host audit and API37 ARM64/x86_64 APK build pass; live gameplay remains unverified.
- [x] Add same-VM `Event::died` → `OnDied(killer)` forwarding: nonnull killer is userdata table, null killer is Lua nil. Focused session test passes 39 cases; Android providers and event-2 routing remain open. `Stop()` and explicit `Attack(target)` adapters are optional; no-argument `Attack()` fails closed because its hidden `ReturnValues+0x408` target is not projected.
- [x] Select the `_GiveXP` callee kernel over the canonical Save/PropertyView: focused audit verifies source XP arithmetic/gates, normal-constant lookup order and missing-key zero, fresh difficulty/Level reads, conditional `a3` lookup, and post-LevelUp XP/MaxXP reread/clamp.
- [x] Select the post-loot `Character::Kill` quest-event tail at `0x3a5b18`; host checks pass 12 cases for source order, gates, mutable payloads and failure prefixes. Live Level/EventManager dispatch remains open.
- [x] Select host `Character::DistributeXP`: focused audit covers two roster passes, level scaling, kill-centered radius/self bypass, cooperative share and fixed-point grants. Android binding remains open.
- [ ] Connect `DistributeXP` and `_GiveXP` to the same live Player registry, PropertyView and Save; the current Android roster has one Player. Do not enable threshold-crossing awards until LevelUp/SG_Save is complete.
- [ ] Implement the LevelUp transaction and normal offline `SG_Save`: class/property recalc, HP/MP, existing-profile `saveAll` writers and durable persistence, HUD/scripts/VFX/trophies and overage XP. `_SaveVolatileQuestsLog` is online-only; current Android saving only writes explicitly created profiles.
- [ ] Complete quest progression: the generated Crypt Kill tail is wired to the current quest owner; a combined host test proves progress on a real authored KillX objective. ClearEnemies now has Compile/Register/Unregister support with 5,297 owner checks (cache has no authored Clear rows, so that selector is projected in-memory). IDA-backed module ID is `N + source module index`; direct CharacterTable row is retained. Runtime objective registration, template events and non-DACT/gate coverage remain. `RaiseAsync` enters synchronous `Raise` in this binary.
- [ ] Complete outer event2/`OnDied`/`AI_SetDead` on the same actor with real group, active-AIS, timer, FSM and aggro owners; general enemy Character/CharAI/AIS runtime remains open.
- [ ] Complete all CalculateResult/ApplyResult dependencies, effects, notifications and actor ownership.
- [ ] Connect melee/ranged/spell combat, skills, criticals, resistances and status effects in the final runtime.
- [ ] Complete player damage/death, attacker/killer credit, resurrection and respawn.
- [ ] Complete source loot coverage: currently gated to one active Warrior/Rogue/Mage on Normal with level+336 zero and `InfiniteLootDrops`; automatic-pickup routing now uses the original constant and shared V4 interaction but still needs a live kill/drop test. Add `DBG_DropAllLoots`, all class/multiplayer counts and difficulty variants, then AutoTransmute/full-inventory UI and rewards/progression.
- [ ] Complete XP, leveling, rewards, gold and difficulty scaling through actual game owners.
- [ ] Validate boss encounters and any original cooperative/network behavior retained by the project.
- [ ] Complete a real original level through its exit using the integrated combat loop.

- [x] Select ordinary Player Kill continuation, exact Ctrl_Kill and event2 composition: 58 ARM comparisons with zero mismatches, 43 host continuations, 27 failure prefixes, seven guards and three same-Session classes pass; both Android ABIs compile. Native locality/trophy/online providers and general Kill/rewards remain open.

## 8. Quests, campaign, UI, audio and saves

- [x] Decode all 64 original quest records and their conditions/objectives/rewards/script slots.
- [x] Reconstruct bounded counted-kill/clear compilation and progress components with diagnostic integration.
- [x] Decode 33 FastTravel and 51 Level catalogue rows; connect bounded source range callbacks.
- [x] Select Adam's five-TU shared localization/item-text closure on existing data DSO; 1,322 real items/936 powers and 99,622 composition checks pass. Native language/file/Application/HUD providers remain open.
- [x] Select original campaign filename/index and borrowed SG_Load orchestration: 96 index/1,780 load/128 filename ARM cases and 23,017 failure prefixes pass through actual game-data selection; no second Save/profile authority.
- [x] Complete seven metadata readers on the sole Save: 128 seven-reader ARM cases and 7,043 truncation/reload prefixes pass; use real CharacterTable names and source current-difficulty global.
- [x] Run read-only real campaign metadata import on API37/16KiB; verify corruption and live-slot rejection, retained profile/file lease, and distinct metadata/gameplay Saves across reload/rotation. Gameplay load, writes and backups remain open.
- [ ] Complete quest conditions, automatic event dispatch, objective types, rewards and persistence.
- [ ] Complete campaign progression, story/dialogue, unlocks and difficulty transitions.
- [x] Reuse Adam v69's original menu, name/class selection and real Single Player screen; the previous API37/16 KiB build tested Warrior/Rogue/Mage profiles into the development Crypt.
- [x] Render original HP/MP/XP timelines from the retained live player property sheet for all three classes, after the single world update/render.
- [x] Verify menu/world Back, occupied-slot cold restart and Home/resume through actual UI input; preserve and restore existing emulator saves.
- [ ] Re-run API 37 menu taps with source-faithful full-driver mode-0 bounds; the earlier centered 3:2 smoke result tested an interim viewport policy, not `MenuFlash2DCamera` behavior.
- [x] Render live Player equipment through Adam's V6 visual owner over the existing V4 inventory; connect equip/unequip SWF callbacks through the source equipment service. Host gate: 7,383 checks; API 37 emulator confirmed a live Unequip → Equip cycle.
- [x] Fix `NativeInvDropItem` arity handling: the SWF encodes `CallFunction` argc=2, while IDA and REA confirm the native reads arg0 as the inventory index. The Android 17 APK rebuilt and installed; live drop confirmation remains open.
- [x] Resolve SWAMP CamAnimSet Default through the source animation tables and feed its idle path into the camera-rig loader; host audit resolves row 0 and advances 0→8 ms. The four selected BDAEs are now packaged from the supplied original cache; Template, shake and crit composition are still not connected to CameraLevel playback. Camera math/input gates pass 46/28 assertions; API 37 joystick drag moved the Player ~18.8 world units and release stopped. Live camera-route and zoom checks remain open.
- [x] Preserve authored `NativeStartGame` arguments with the assigned `PlayerInfo` slot across deferred SWF delivery, then resolve through the existing metadata `Save` and 51-row `LevelList` plan. Offline selection matches IDA; numeric and LUSP `SG_Save` effects use the same temporary Save/index. Loaders cover generated Crypt and static SWAMP; row 23 still uses a debug-only override.
- [x] Map NativeStartGame's current-Level state-38 early return to the retained active-world owner before argument conversion; normal menu starts and EGL restoration remain allowed.
- [ ] Finish `Application`/`GSLevel` startup. Online host/client remains deferred. IDA shows `useSpawnPoint` is stored on `Level` but no reader was found; keep it separate from entrypoint-based placement until its consumer is identified.
- [x] Apply NativeStartGame numeric and selected-LUSP `SG_Save` effects to the same metadata Save/index; host audit passes 773 checks, API 37 ARM64/x86_64 build and 16 KiB menu/start/movement/reopen smoke pass. No reachable enemy in this smoke; combat remains unverified.
- [x] Add the bounded GEAR payload writer to the selected `dh2_level_world` library; source field order/encodings and 95-byte fixture pass 293 host checks, including all 95 output truncation prefixes. The original ARM writer is mapped; this test does not execute it.
- [x] Select raw profile-section assembly over the existing index: host audit verifies lexical tag order, last-duplicate retention, GEAR replacement/round-trip, transactional failure, and the metadata serializer’s seven-tag guard. This utility does not dispatch `Savegame::saveAll` callbacks.
- [ ] Replace the direct renderer-loader shortcut with `Application::LoadLevel`/`GSLevel`/`Level`, source parser/factory owners, and their remaining providers.
- [ ] Complete original NativeStartGame/Application.LoadLevel, difficulty/location/quest handoff and full gameplay startup.
- [ ] Finish original `MenuManager`/`HUDControls` and in-game UI callback paths. Inventory actions use the shared V4/V6 owners and talent training/equip use the retained Player skill owner; QEST now has same-Save serializer coverage and ARM64/x86_64 native links, but live skill-save behavior awaits a rebuild after the Player class-count initialization fix. Faery selection saves and refreshes skills but cannot yet place/rebind an in-world companion. Fresh-save item population, class-spec selection, live item-drop confirmation, merchant and combat-skill callbacks remain open. Potion read/use is not gameplay-verified with current edits.
- [ ] Complete `MenuManager`/`HUDControls` input ownership, pinch zoom, orientation/window and lifecycle behavior. `NativeTouchToMove` is an exact source no-op.
- [ ] Connect music, sound, voice, visual effects and their original timing/lifetimes.
- [ ] Complete campaign/profile save serialization, load ownership and version handling. Selected host-tested writers cover CFEE/FAES/FTVL, SKIL, PROP and LVLS; QEST, mask-4 GEAR registration and existing-profile Transport/saveAll persistence remain open.
- [ ] Support original-save import where its data format is established and compatible.
- [ ] Verify saves after process death, cold launch, app update and interrupted writes.
- [ ] Validate every authored/generated level and complete the full campaign playthrough.

- [x] Select `_InitLevelStates`: 670 ARM comparisons and 116 native checks; retain one set of six Save arrays.
- [x] Decode all 13 original WorldMap locations/3 lockers; compare all 16 readers and 192 real LevelList/WorldMap defaults; retain actual tables in Android.
- [x] Select LVLS and both state setters: 404 ARM comparisons/172 guards; bind real table owners through the native transport.
- [x] Select FTVL: 249 ARM comparisons/478 guards; same six canonical bitset words, retained early-exit/failure stores; bind native transport reader.
- [x] Select QuestSavegame constructors/InitQuests/destructor: 220 ARM comparisons/395 guards; borrow canonical act arrays and correct blank Save defaults.
- [x] Share Character identity across Save and both embedded Quest logs; 10 original SetPlayer branch projections pass.
- [x] Bind immutable actual Quest rows/lists/stubs/names: 64 original row comparisons and 49,684 native checks pass.
- [x] Select Quest scalar construction/assignment/ReInit/load/destruction: 5,220 ARM comparisons and failure prefixes pass; gameplay leaves remain mandatory.
- [x] Select QEST/LoadQuests/UnpackQuests/UnpackQuest over canonical log controls: 534 ARM comparisons/75 native checks; native stream/payload binding remains open.
- [x] Select ConditionList lifecycle/assignment/evaluation callers: 326 ARM comparisons/55 native checks; real condition evaluation providers remain required.
- [x] Select ObjectiveList lifecycle/assignment/owner/stream loops: 776 ARM comparisons/35 guards; actual payload leaves remain required.
- [x] Select RewardList lifecycle/assignment/owner callers: 3,019 ARM comparisons/16 guards; retain source text/array failure effects.
- [x] Select all seven Condition factories/destructors: 412 ARM comparisons/27 guards and actual 80-condition composition pass.
- [x] Select all 13 Objective factories/destructors: 2,830 ARM comparisons/all 727 ordinary words and 3,209 checks pass.
- [x] Select all five Reward factories/destructors: 850 ARM comparisons/15 guards and actual 222-reward composition pass.
- [x] Select generic quest-state and current-Level condition evaluation: 1,473 ARM comparisons/225 native checks; genuine Character lookup/Compile remains required.
- [x] Select five Reward Compile leaves and Gold/XP Give callers: 640 ARM comparisons/14 guards; Gold shares the canonical inventory, real GiveXP remains required.
- [x] Compose Quest Instance and actual lists on one Record: 192 real row/difficulty instances and 4,401 host checks pass.
- [x] Bind native factory/constant ownership to both gameplay Save logs: 3,700 checks; failed row47 retains its prefix, destroys 47 published plus one unpublished Quest, and retries 384 instances.
- [x] Verify native 384-Quest startup, same-owner Home/resume and source destructor cleanup on Back for all three classes on API37/16KiB; preserve existing emulator saves.
- [x] Select Objective base/SavedQty payload and returned bool/int readers: 1,719 ARM comparisons/all 94 words, 846 checks/194 actual object shapes; preserve publication after return.
- [x] Select three direct destination word readers: 2,514 ARM comparisons/all 114 words, 33 checks; retain partial state writes and original assertion boundaries.
- [x] Select Quest/QuestLog Compile and list invalidation/compilation wrappers: 3,914 ARM comparisons/all 236 words, 21 checks; preserve already-marked recursive lookup and mandatory objective gameplay leaves.
- [x] Implement one retained whole-campaign absolute cursor: 30 native checks and 6,561 selected QEST/Objective composition checks; no second Save or cursor.
- [x] Bind native Owner QEST/Quest/action/list payload routes: 6,559 checks restore 384 quests/1,164 objectives, replay both logs, retain truncated prefixes and run genuine cleanup. SG_Load2 handoff/assertion policy remain open.
- [x] Route SaveLoad masks 2 and 4 through one Save/Quest owner and whole-profile cursor: 9,659 selected-host checks restore nonempty SKIL/FAES/PROP plus 384 Quests/1,164 payloads on the same Save; a FAES count mismatch remains nonfatal and QEST continues. Offline Online is an explicit test value.
- [x] Differential-test FAES against original `__LoadFaeries` at `0x4691d0`: 74 cases, including all 69 truncated prefixes; 1,011 fully consumed fields match, with the unsafe source short-read difference recorded.
- [x] Compile the transport bridge for both Android ABIs and smoke-test menu/class selection, Crypt/input/restart/Back/Home-resume for all three classes on API37/16KiB. This does not execute live `Character::InitPost` or restore a real campaign.
- [x] Wire fresh Android player setup to the source Character Save association and `SG_Load(2)` then `SG_Load(4)` exactly once each. API 37/16 KiB smoke verifies the same Character/Save/LoadOwner/embedded Quest owners, eight mask-4 section requests after profile attachment and no mask replay on Home/resume. Test data contains metadata only; full InitPost interstitial and real campaign section restore remain open.

## 9. Fan modding and source delivery

- [x] Implement validated external asset/world overrides with packaged fallback.
- [x] Test a real world override, malformed rejection and baseline restoration in the native app.
- [x] Publish reconstruction source, evidence mappings, tests and build-specific source archives.
- [ ] Document supported data, Lua, asset, level and gameplay modification entry points.
- [ ] Provide validation tools for fan changes and meaningful diagnostics for unsupported content.
- [ ] Complete mod lifecycle/reload/version behavior and restore-to-baseline workflows.
- [ ] Build and playtest example mods that change a skill, enemy, asset and level.
- [ ] Finalize attribution, dependency licenses and asset/source distribution documentation.
- [ ] Write final developer setup, engine/game architecture, build and modding guides.

## 10. Final completion gates

- [ ] A clean checkout produces the complete native ARM64 game and documented assets.
- [ ] Original authored/generated levels, factories and transitions work together.
- [ ] Player, equipment, skills, enemy AI, combat, quests, loot and progression work together.
- [ ] Campaign UI, audio/effects, input and persistent saves are complete.
- [ ] Sustained gameplay and process-death/update recovery pass on current Android.
- [ ] Physical ARM64 gameplay and 16 KiB compatibility are demonstrated.
- [ ] Remaining behavioral/visual regressions are resolved or explicitly accepted against the goal.
- [ ] Public source, mappings, reproducible tests and fan-mod documentation are complete.
- [ ] **The full project goal is finished.**

## Immediate work order

1. Finish live Player/Character initialization through the menu launch, then exercise a real enemy kill, automatic pickup and inventory/save transfer using the shared V4 owner.
2. Connect valid Crypt SpawnSpot/SpawnGroup data to the tested spawn policy, then extend `RoomZone::Update` through remaining factories and conditions. The current cache has no Crypt SpawnSpots and only a zero-quantity TEST group.
3. Finish `Character::Update` gates and Ghost frame ownership on the shared VM/path owner; then connect source-backed movement, combat death, loot and event 2.
4. Build the generator from the 21 MGX definitions, exit graph, `gDistributions`, occupancy/backtracking and source-compatible MLX serialization; connect authored/generated levels to the real loading lifecycle.
5. Complete campaign, durable saves, equipment/skills/UI and fan-modding; validate sustained gameplay on current Android and physical ARM64 before publishing major checkpoints.

Update the relevant checkboxes only after their stated verification passes. Keep
the detailed artifact-specific proof in checkpoint documents and reports.
