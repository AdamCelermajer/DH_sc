# Combined reconstruction status and continuation brief

Updated 2026-10-06. Working branch: `reconstruction/android17-irrlicht-rebuild-2026-10-03`.
Adam reference: [`AdamCelermajer/DH_sc`](https://github.com/AdamCelermajer/DH_sc), latest reviewed `791e961b12233100b303038c961666834f4beb9d`; checkpoint `c3ae7973` and earlier baseline `45c5348e` retained.

This report supersedes older documents' descriptions of the current default development build. Historical reports retain their original artifact identities and test scopes.

**Latest work:** Adam v69's frontend is selected in our native app: original main menu, name/class selection, real Single Player popup and Crypt entry for all three classes. Source profile creation, canonical slot assignment and saved-class reads reuse existing owners. Original HP/MP/XP bars borrow the live 224-word sheet. Fixed GameSWF loader/builtin-table lifetime bugs, startup focus blocking and Rogue's class-property calculation. All three class flows, occupied-slot restart, Back/Home-resume and existing player death/skill regressions pass on API37/16KiB. [Evidence](../reports/reconstruction-2026-10-06/menu-crypt/validation.json).

**Latest startup work:** registered menu players now retain genuine managed Save680 metadata with class/level/name setters. Character save/InitFinal and QuestSavegame callers are selected; LVLS/FTVL readers bind the same Save arrays and real WorldMap/LevelList owners. Selected transport passes 522 checks; all three classes pass menu/Crypt/restart/resume and visible movement/input on API37/16KiB. [Evidence](../reports/reconstruction-2026-10-06/player-startup/validation.json).

**Latest Quest work:** native startup binds both canonical gameplay Save logs to 384 genuine Quest objects using original definitions/constants and selected child factories. All three classes pass startup, same-owner Home/resume and source destructor cleanup on Back. Failed row47 cleanup and valid retry pass 3,700 selected host checks. Scalar/QEST/list/factory/condition/reward callers are selected; Compile, scripts/events/rewards and QEST persistence remain disconnected. [Evidence](../reports/reconstruction-2026-10-06/quest-startup/validation.json).

**New source checkpoint after that release:** selected Quest/QuestLog Compile wrappers and typed payload readers pass 8,147 ARM comparisons. Native Owner restores 384 quests/1,164 objectives through one borrowed whole-file cursor; 6,559 checks cover full/truncated loads and cleanup. Both ABIs compile; Warrior startup still passes. SG_Load2/live restoration, automatic scripts, enemy list/cache queries, event registration and NPC markers remain unbound. MoveInZone's source field `+20` needs one native-width scalar/object projection before its gameplay body is connected. [Evidence](../reports/reconstruction-2026-10-06/quest-payload/validation.json).

**Verified APK:** [download menu/class/Crypt quest-startup prerelease](https://github.com/Noamcelermajer/DH_sc/releases/download/native-quest-startup-2026-10-06/DH2-native-quest-startup-2026-10-06.apk), `e431b7f6…`, 154,420,798 bytes. Both ABIs, 1,282 actual compiler inputs per ABI, 1,333 captured files and 22 ELF64/16KiB libraries. [Matching source capture](https://github.com/Noamcelermajer/DH_sc/releases/download/native-quest-startup-2026-10-06/DH2-native-quest-startup-source-capture-2026-10-06.zip). Full NativeStartGame, Character660/InitPost/SG_Load2/4, equipment and campaign remain open. [Checklist](PROJECT-CHECKLIST.md).

## Latest Adam reconciliation

Reviewed his three-session checkpoint and player/UI, item-effects, skills/loot,
native-combat and loader milestones against actual code/build selections at
`791e961b`, rather than treating historical receipts as our test results.

| Adam system | Decision | Current result / reason |
|---|---|---|
| `fresh_inventory_owned_v4` | Already adapted | Selected; borrows our live properties/RNG. Native player inventory binding remains open. |
| `player_equipment_v3` | Superseded | V4 is the sole selected inventory/equipment graph; preserve V3 research without another owner. |
| Gear properties/power tables/gear effects V5 | Already present | Selected shared algorithms; actual visual/vitals/requirements integration remains open. |
| `item_presentation_v5` | Imported/adapted | Existing item identities/table authority; selected host and both Android builds pass. Actual localization/formatting providers remain open. |
| `loot_power_resources_v7` | Already present | Actual power/quantity resources already selected; no second table authority. |
| `loot_power_creation_v7` | Imported/adapted | Shares V4's stream0 descriptor. 9,931 pinned gold replays and 363 powered V4 items pass; native death/drop/pickup/AddLoot remain disconnected. |
| Player skills V3 / V6 composition | Selective reuse | Preparation/session/update/use/cooldown/save callers adapted to our retained VM/Coordinator. Whole private-session transplant is incompatible with that ownership graph. |
| Buff dependencies | Adapted and live | One buff group owner, existing property sheets and Coordinator; 40 whole-state ARM comparisons and actual Celest resistance pass. FX services and live timed-buff expiry remain open. |
| CSSkill state callbacks/dispatch | Adapted source | Selected library, 214 ARM dispatch comparisons and Android compilation pass. Native state6/input/animation/target activation remains disconnected. |
| Initial skill grants | Adapted source | Existing progression/slot callers share the actual save, inventory and buff-aware property view. Selected-host/ARM and Android gates pass; real profile/InitPost producers and native inventory binding remain required. |
| Equipment render owner | Adapted services | Reuses V4/V5 algorithms through our exact property view and real provider contracts. Requirements/pruning/Skin-before-vitals and retirement-prefix tests pass; native visual/text/HUD and whole-item lifetime binding remain open. |
| Character::_InitEquipment | Adapted/selected | Sole V4 inventory/property view; 18 original caller cases and 2,157 host checks across three classes pass, both Android ABIs compile. Full AddLoot, profile/locality/InitPost and native text/Skin remain required. |
| Ordinary Player Kill continuation | Adapted/selected | 58 ARM comparisons/zero mismatches; 43 host continuations, 27 failures, seven guards and three real-class event2 compositions pass. Shares Session/death owner/Coordinator. Native locality/trophy/online and general Kill/rewards remain open. |
| Actor scene retention | Port lifecycle adapter, live | Seven host boundaries/402 exact frames/nine guards. API37/16KiB dead pose retains playback/pose hashes through reload/rotation; terminal CPU teardown passes. Zero new original bodies; full animation/GPU parity remains open. |
| Weapon/equipment queries | Imported/adapted | Borrows sole V4 inventory/property view; 1,000 original weapon cases, both sets and combat projections pass. Native inventory producer remains required. |
| Localization/item text dependencies | Imported/selected | Five shared text TUs use existing game-data DSO; 1,322 actual items/936 power descriptions and 1,087 formatter/355 varargs replays pass. Menu language/file/text providers are now bound; native inventory item formatting and remaining HUD providers stay open. |
| Player AIS initialization/death dependencies | Adapted and live | Native load/InitProcess and OnDied cleanup share the same owners. Real timers33/34, regen, source timer retirement and all13 cleanup callbacks pass. UpdateAllSkills is delivered once by InitProcess; real progression callers remain required. Full Kill/event2, positive DoT, linked-aggro producers, full AIS frame and profile/grants remain open. |
| Native combat/NPC candidates | Audit/selective reuse | Useful target/event contracts. Upstream melee shortcuts, no-target skill receipts and unselected NPC candidates do not prove our full enemy/skill combat loop. |
| Profile filename/index and SG_Load | Adapted/selected | 96 index and 1,780 whole-load ARM cases; 23,017 failure prefixes, 128 filename cases. Borrows the sole Save and its canonical profile. Fresh seven-writer metadata creation is live; full gameplay sections and backup/recovery remain open. |
| Seven campaign metadata readers | Adapted and live | 128 seven-reader ARM cases; private real profile imports class263/level1/difficulty0 on API37/16KiB. One file lease survives reload/rotation; metadata Save remains separate from gameplay Save. |
| Player/Matching locality queries | Adapted and live fallback | 141 ARM comparisons/zero mismatches. Canonical Character660 remains null: local=true is a fallback query result, not registration. Host-level synchronization correctly skips with zero reads/writes. |
| Saved GEAR inventory | Adapted/selected | 19 original caller cases × three classes; 2,498 host checks, both Android ABI builds. Reuses V4 and presentation/power/text services. Retained-owner failure prefixes and retirement pass in selected host tests; native SG4 restoration remains open. |
| Separate menu v69 contribution | Imported/adapted, live | Actual menu/name/class/Single Player movies, authored class camera/body previews, font/text/texture/shader and audio routing selected. Three-class profile creation → canonical Assign → development Crypt and original vitals HUD pass. Preview weapons, complete lighting and remaining HUD input stay open. |
| Separate level-loader contribution | Deferred integration | Preserve factory/staging research; native factories, quest/campaign state and full Application.LoadLevel continuation remain unbound. |

The newer cloud branch `6dc314db` has useful generic loader interfaces and
coercion fixtures. Its Session integer-map patch overlaps our proven native
dictionary; adapt those fixtures through existing providers before reuse.
Its native factories remain unbound. Our `origin/main` refresh (`1219a43a`)
adds no gameplay source beyond the reviewed branch.

Next live milestone: connect recovered `_AddCharacter` → Character660 → gameplay Save → InitAll/InitPost
→ SG_Load2/4/GEAR → equipment/grants. The existing fallback slot must not be
copied into a newly registered record. Reuse the sole inventory, properties,
skill VM and timer owners. Connect skill activation, enemy frames and
animation-driven hits/rewards/loot pickup for one complete encounter.
Full NativeStartGame/Application.LoadLevel and campaign persistence remain open.

### Rough estimate

Planning ranges assume sustained focused development, parallel work on independent
services and reuse of verified source/cache, without major new factory/save discoveries: **2–4 weeks** for a useful integrated player/enemy/skill encounter;
**4–8 weeks** for one level with progression, death rewards, loot pickup and
retained state; **6–12+ months** for complete campaign/UI/audio/rendering,
durable saves, modding and ARM64 acceptance. The full-game range has low
confidence until factories, save compatibility and remaining engine behavior
are exercised. These are estimates, not completion percentages or deadlines.


## Improved project brief

Reconstruct Dungeon Hunter 2's game logic and engine behavior as maintainable, buildable source by combining Adam Celermajer's research with the existing recovery and implementation work. Produce a native 64-bit Android app for the current Android release, with 16 KiB memory-page compatibility, that preserves the original gameplay and content and supports documented fan changes to data, assets and scripts.

Maintain an evidence-backed ledger from original symbols, addresses, data layouts and scripts to reconstructed implementations. Distinguish recovered evidence, compilable implementation, isolated behavior tests and live gameplay integration. Exercise actual movement, combat, levels, progression, saves and Android lifecycle behavior; investigate and fix failures. Use upstream Irrlicht to help reconstruct the game's customized `glitch::` engine, preserving known custom behavior rather than assuming upstream is interchangeable.

Keep private documents and their contents outside Git. Publish coherent source milestones on the reconstruction branch. Do not update PR #1 or upload to Drive. Runtime validation for this project uses the modern Android 17/API 37 16 KiB development emulator; no Fold7 or 4 KiB test is requested.

### Completion gates

1. A clean checkout builds the app and its game/engine code for ARM64 without the original ARM32 game binary or a translation runtime.
2. Original content is loadable through a documented asset installation/export pipeline. Authored and generated levels, transitions and object factories work.
3. Player classes, equipment, skills, enemies, AI, quests, loot, progression and game UI work together through the reconstructed runtime.
4. Persistent saves survive process death and app updates; audio, effects, input and lifecycle behavior work.
5. Android compatibility is demonstrated by source builds, native-library/ZIP alignment checks and gameplay tests; physical ARM64 execution remains a separate required proof.
6. Original-to-source mappings and reproducible tests accompany reconstructed behavior; uncertainty and approximations are visible.
7. Fan changes have documented entry points, validation and a way to restore the baseline.

Exact lost studio source text is not required to meet these gates. Reconstructed code must reproduce its behavior. The goal is active; these gates have not all been met.

## Where the project actually stands

There is now a source-built native **Crypt development app** under [`port/android-native`](../port/android-native/README.md). It renders the Prince and an original authored eight-room Crypt layout, with 13 resolved monster records and 84 scenery objects. The two surprise Ghosts now appear through the original GhostAmbush01 trigger and timed script, reconstructed Limbus/Spawn/Idle states and real native body creation; their full combat/AI services remain pending. It uses reconstructed world, scene, animation, skinning, character, navigation, physics and combat code. The renderer is a development GLES2 implementation. Its camera, touch control and some gameplay service boundaries are development adapters.

There is also our separate source app under [`port/android-app`](../port/android-app/README.md), containing an authored encounter, Lua/stat/equipment/quest diagnostic integration, Infected Village and SWAMP previews, and an Irrlicht NativeActivity route. The Irrlicht SWAMP route now uses the Prince's four source warrior skins, mapped atlas, complete authored bank registration and two-slot playback through the shared Character coordinator. It reuses Adam's skinning and visual owner/helper/graph code. A level session owns a real player body, navigation registry and path storage. Source Move focus/blur dispatches unpin and Stop then Pin; root motion, one physics step and actor update feed the rendered pose. The exact current APK passes free movement, original navigation boundary sliding and same-process resume on API 37/16 KiB. Environment bodies, other module seams, enemy AI/combat and complete game flow remain pending. Two source additive bridge-overlay states are mapped. Source navigation floors, room bounds and exit markers are now excluded from scenery while their navigation data remains available; the white floor bands are resolved. A newer source-alpha preview removes the dark foliage/ground rectangles by copying the original shader's blue mask into diffuse alpha and enabling fractional-alpha blending on the 22 resolved Material__11611 draws. The newer selector-driven build decodes AL in both source profiles. Original runtime profile/effect group, compiled shader ordinal, blend/depth state and remaining rendering gaps are still being reconstructed.

These are **two development runtimes**, not a completed game. The source is substantial and real, but integration, full game flow and content coverage remain incomplete. The old ARM32 compatibility wrapper is retained as historical/reference work; it does not satisfy the native-rebuild completion gate.

## How much source has been rebuilt?

A single honest game-wide percentage is not available. There is no audited list assigning every original function to one implementation and proving it is linked, exercised and behaviorally equivalent in the finished game. Dependencies, duplicate symbols, generated exports and test scaffolding must not count as rebuilt game logic.

Use the following concrete measures instead:

| Measure | Established amount | Meaning and limit |
|---|---:|---|
| Original engine function ranges | 31,018 | Original symbol/assembly inventory; includes engine, libraries and support code. Not a count of reconstructed functions. |
| Adam original-address mapping reach | 1,455 unique addresses / 137 manifests | 2,210 repeated checkpoint records deduplicated by address. Mapping/evidence reach, not a completed implementation count. |
| Current Character/engine mapping extension | 551 verified unique ranges / 894 evidence records across 91 manifests; 376 additional addresses; 1,831 combined unique addresses | Bounded game/engine callers and dependencies. Duplicate addresses are deduplicated; adapter/provider boundaries earn no new original-body credit. Not a count of fully rebuilt functions. |
| Earlier selective-import mapping reach | 133 unique addresses / 9 manifests | Much of Adam's deeper mapping was initially omitted; evidence restoration is part of this continuation. |
| Original engine pseudocode exports | 31,794 successful exports | Generated decompiler text; not correct compilable C++ and not the same denominator as unique symbol ranges. |
| Engine `.text` bytes accounted for | 99.989968% | Assembly/disassembly accounting only. No source/game-completion percentage follows from it. |
| Original plaintext scripts recovered | 219 files / 900,493 bytes | Actual cache Lua source, preserved exactly. Native service ownership and complete live execution remain incomplete. |
| Repaired/decompiled Android Java | 288 source files | Source recovery/reconstruction of Android glue; distinguish it from independently reconstructed native gameplay. |
| Maintained reconstruction/port code at `92f3c416` | 703 source files / 81,984 lines / 4,347,837 bytes; 896 test/tool files | Git-index inventory excludes dependencies, recovered evidence, packaged scripts/assets and unfinished drafts. Includes imported code with local changes. Source size is not game completion. |
| Adam core modules imported | 577 files / 2,929,226 bytes | Seven modules imported at a pinned commit before dependency-path adaptations; includes headers/tools/tests, not 577 gameplay implementations. |
| Adam script runtime imported | 158 files / 1,064,065 bytes | Isolated import includes Lua dependency source; do not count all of it as reconstructed game code. |
| Current native app bundled assets | 696 | Selected Crypt/Prince/menu/HUD assets and source scripts; not the complete game cache. |
| Crypt object instantiation | 97 / 166 records | 84 decor plus 13 monster records, including two gated first-spawn actors; 69 conditional/script/template/factory records remain. This fraction applies only to this authored level's object records. |
| Prince animation bank | 116 resources / 158 occurrences | Live two-slot registration/playback in Adam's runtime; full original pose/GPU parity is not established. |
| New Irrlicht Prince slice | 4 controllers / 27 joint references / 487 vertices / 586 triangles; module0 has 2 floors / 99 triangles / 133 graph nodes | Four default warrior parts and complete bank/shared state feed source root motion and an owned player body. Only Idle/Move exercised live; environment bodies, other modules, original camera, AI/combat and effects remain incomplete. |

The reproducible [`combined-source-inventory.json`](../reports/combined-source-inventory.json) and [`tools/source_inventory.py`](../tools/source_inventory.py) count the maintained source tree separately from vendor code, archived pseudocode and tests. The inventory also separates repaired/decompiled Java. File/line quantities measure repository size; they are not a claim of game fidelity or completion. The [function audit](generated/combined-function-audit.md) and [1,455-address ledger](generated/combined-function-audit.json) preserve per-address symbols, ranges, map files and provenance. The [277-file evidence import ledger](generated/adam-evidence-import.json) records restored hashes and remaining unavailable references.

## What Adam contributed and what our work adds

The [other-branch audit](BRANCH-AUDIT-2026-10-05.md) found a separate engine
research overlay at `e6da25b`. We selectively imported 23 evidence/tool files,
verified 1,733 original ELF hash records and replayed the BRES corpus. This adds
reference reach; it does not increase completed source-body or gameplay counts.
Adam's latest `791e961b` changes are reconciled above; the old audit checkout
remains pinned at c3 for reproducibility.

| System | Our existing work | Adam's contribution now imported | Combined result / gap |
|---|---|---|---|
| Evidence and cache | Exact assembly, symbols, pseudocode, Java/smali, shaders/configuration; complete-cache verification | Further subsystem disassembly, original-instruction comparisons and runtime checkpoints | Broad evidence with two independent research trails; references/report corpora require reconciliation. |
| Engine resources | BRES fixups, math and payload views; full-cache differential checks | Shared identical math/resource foundation; deeper scene/animation resource consumers | Buildable source readers; source contracts differ in some payload APIs and must stay explicit. |
| Materials/textures | Texture/material parsers and Android/Irrlicht previews | Scene-material resolution, GLES rendering and actor resources | Complementary rendering paths; full original GPU pass/effect fidelity remains open. |
| Animation/skinning | Typed values, pose/timeline/mixing/layers/transitions/root delta; mutable Irrlicht meshes and shared Character coordinator | Live Prince bank, compiled transforms, two-slot playback, event routing, skin deformation and visual owner/helper composition | Complete authored bank registration/blended playback now feeds both renderers; bounded Irrlicht Idle/Move and Crypt gameplay pass. CPU dead-pose/playback retention through reload/rotation now passes; full animation-state coverage and GPU parity remain open. Complete AI/Lua ownership and full pose parity remain pending. |
| World/navigation | SWAMP and Infected Village data, movement/floor components; source-to-actor floor bridge and level session | Authored Crypt, native floor graph, path search, smoothing, avoidance, producers | SWAMP module-zero geometry/flags feed Adam's collision/graph view live; Android verifies free motion and validated boundary sliding. Generated rooms, remaining modules, complete environment collision and enemy pursuit remain incomplete. |
| Physics/actor | Actor registry, shared state/timer ownership, exact-name spawn factory, Limbus/Spawn/respawn/group kernels and owned SWAMP session | Box2D source, native body/filter/transform services and Prince actor/frame coordinator | Source Character services own Irrlicht Stop/pin/unpin; each logical tick has one world step. Two Ghosts create real bodies on source Spawn completion. Full template/factory/lifecycle and enemy controllers remain open. |
| Stats/equipment | Character properties/classes, item/power calculations, Lua object bindings | Live actor properties, vitals, animation/combat tables | Tested calculations exist. Complete inventory/equip/power/buff ownership is not live in the combined Crypt app. |
| Combat/AI | Damage/health/death and counted-kill kernels, authored encounter | Animation-event combat, player/monster hit application, target/range/faction/aggro research | A real native combat slice exists. Full AI FSM, pursuit, skills/status effects, rewards and original service order remain unfinished. |
| Quests/scripts/triggers | Quest table/compile/progress, original Lua, owned Crypt script session and source trigger producer | Lua aliases/timers/callback research and native Character event consumers | Original GhostAmbush01 contact/Wait/Spawn now runs natively with source frame ordering. Other script services and campaign progression remain incomplete. |
| Saves | Authored encounter persistence and original save inspection | In-process combat restoration during Activity/GL recreation | Neither proves complete original-compatible campaign saves across process death. |
| Android | API 37 ARM64/x86_64 source app and Irrlicht integration | API 37 native Crypt Gradle app with aligned libraries | Both build; physical ARM64 gameplay and complete release validation remain open. |
| Fan modding | Editable scripts/data and recovery tools | Structured asset/data readers and native runtime | Initial validated asset override loader added to the Crypt app; full script/mod APIs and packaging remain future work. |

## Actual reverse-engineering mapping

The following are implementation anchors, not claims that entire original classes have been recovered. Original symbols/addresses in `original-functions.json`, assembly exports and per-module notes supply evidence.

| Original behavior / evidence | Source implementation | Current integration |
|---|---|---|
| `glitch::` Irrlicht lineage and custom scene/driver interfaces | [`irrlicht-feasibility`](../port/irrlicht-feasibility/README.md), [`scene_mesh_adapter`](../port/irrlicht-android/game/scene_mesh_adapter.hpp) | Source Irrlicht diagnostic. Original custom driver is not a drop-in upstream replacement. |
| BRES resource headers, relocations and pointer tables | [`engine-resources`](../port/engine-resources/README.md) | Shared reader foundation; complete-cache instruction comparisons reported. |
| Mesh payloads, scene graph, materials and controllers | [`asset-payloads`](../port/asset-payloads/README.md), [`scene-materials`](../port/scene-materials/README.md), [`engine-skinning`](../port/engine-skinning/README.md) | Crypt scene and skinned Prince; separate older preview readers remain. |
| Animation transform/time evaluation, clip registration and occurrence playback | [`engine-animation`](../port/engine-animation/README.md), [`actor_blended_playback.hpp`](../port/level-world/actor_blended_playback.hpp) | Live Prince bank. Exact ordered registration and selected comparisons are distinct from whole-bank pose equivalence. |
| `PFFloor::_LoadNavMesh` at `0x520b40`; `Module::LoadModule` at `0x38a88c` | [`level-world`](../port/level-world/README.md), [`original-functions.json`](../port/level-world/original-functions.json) | Authored Crypt floor/room extraction and native navigation. |
| `PFFloor::GetCollisionAt` at `0x51b96c`; `PFWorld::GetFloorHeightAt` at `0x525508` | [`floors.hpp`](../port/level-world/floors.hpp), navigation/floor source under `level-world` | Actual floor service and separately identified startup probes; probes are not moving AI proof. |
| `Structs::CharacterProperties::read` at `0x4f2750`; `Arrays::CharacterTable::read` at `0x4b4340` | [`game-data`](../port/game-data/README.md), [`character-properties`](../port/character-properties/README.md) | Recovered portable table/state projections; original pointers/ABI are not overlaid on ARM64. |
| Character state, timer, stance, path commands and frame order | [`character_state.hpp`](../port/level-world/character_state.hpp), [`actor_runtime.hpp`](../port/level-world/actor_runtime.hpp) | Live Prince; explicit service requests identify unfinished camera/AI/effects ownership. |
| `Script_SpawnCharacter::Execute` at `0x45f400`; `CSSpawn::OnFocus` at `0x3c35ec`; Limbus/Spawn transitions and service dependencies | [`character_factory.cpp`](../port/level-world/character_factory.cpp), [`character_coordinator.hpp`](../port/level-world/character_coordinator.hpp), [82-range mapping extension](generated/character-runtime-function-map.json) | Two direct Crypt Ghosts run original GhostAmbush01  then  source Spawn  then  Idle with real bodies; separate Hallway templates, full AI/combat and respawn/group ownership remain pending. |
| `Character::SafeGetCharPropsTemplateId` at `0x3b36ec`; `SafeGetCharPropsId` at `0x3b3d38` | [`character_template_factory.cpp`](../port/level-world/character_template_factory.cpp), [`character_template_random.cpp`](../port/level-world/character_template_random.cpp) | Host-tested five-slot template/property/class projection and ordinary-stream draw for four ambushers. Startup seed/draw order, property defaults and native actor integration remain pending. |
| `ScriptManager::ExecuteScript` at `0x45c16c`; `Level::Update` at `0x3f82d8`; original Wait/Spawn command bytes | [`crypt_spawn_script_session.cpp`](../port/level-world/crypt_spawn_script_session.cpp), [`script_runtime.cpp`](../port/script-runtime/script_runtime.cpp) | Live owned GhostAmbush01 session; blocking check precedes Wait update, ScriptManager precedes physics/ObjectManager. Other command services and full class bodies remain pending. |
| Visual root-motion history, displacement cancellation and owner/helper/authored transforms | [`visual_motion.hpp`](../port/level-world/visual_motion.hpp), [`visual_motion.cpp`](../port/level-world/visual_motion.cpp) | Live Crypt source behavior; required by the Irrlicht actor integration so clip-local placement does not displace the rendered character incorrectly. |
| Melee result, hit/health/death application and authored animation events | [`game-data`](../port/game-data/README.md), [`character-damage`](../port/character-damage/README.md), [`character-death`](../port/character-death/README.md) | Crypt player/monster applications plus separate Lua diagnostics. Status/audio/loot/quest consumers are incomplete. |
| Quest array reader at `0x4b8acc`; quest record reader at `0x504a94` | [`quest-data`](../port/quest-data/README.md), [`quest-compile`](../port/quest-compile/README.md), [`quest-kill`](../port/quest-kill/README.md) | 64 real quests and supported counted-kill diagnostic integration. Full quest lifecycle/rewards/campaign flow remain open. |
| `GameObject::IsTouching`, trigger gates and `Zone::InitPost`/absolute bounds | [`crypt_spawn_trigger.hpp`](../port/level-world/crypt_spawn_trigger.hpp), [`trigger-contact`](../port/trigger-contact/README.md), [`zone-contact-runtime`](../port/zone-contact-runtime/README.md) | Original Crypt trigger now uses source absolute-AABB overlap in the app. Separate Zone::IsInside has its own physical-object/selector guards; it is not the trigger call chain. |
| `GroupInfo::CanRespawn` at `0x3d2a34`; `SM_IsInLimbus` at `0x3c01c0`; `AI_ClearAllAggro` at `0x3d5fa8` | [`character_group_respawn`](../port/level-world/character_group_respawn.hpp), [`character_aggro_cleanup`](../port/level-world/character_aggro_cleanup.hpp) | Host-tested all-member predicate and retained-peer cleanup/OnDeAggro ordering. Native group/map storage and real AI/Lua callbacks remain unbound. |
| `CSLimbus::OnFocus` at `0x3c2e58`; Character respawn decisions; post-Revive group branch of `CSLimbus::OnBlur` | [`character_limbus_respawn`](../port/level-world/character_limbus_respawn.hpp), [`character_respawn_outer`](../port/level-world/character_respawn_outer.hpp), [`character_group_limbus_blur`](../port/level-world/character_group_limbus_blur.hpp) | Fresh two-read timer producer, signed property delay arithmetic and any-other-Limbus group producer pass host checks. Group blur also passes seven original ARM branch cases. Native property/group/hosting/event ownership remains unbound. |
| `GameObject::SetVisible` at `0x38b0f0`, actual Character vtable address point and constructor bytes | [visibility audit](../port/level-world/reference/character-spawn-visibility/NOTES.md), [`character_state`](../port/level-world/character_state.hpp) | Corrected native Limbus/Spawn visibility service; hidden actors still update their timers. General condition/network-enabled producers and complete visual synchronization remain open. |
| Source Move body services, native world step, navigation and visual actor phase ordering | [`swamp_actor_session`](../port/irrlicht-android/game/swamp_actor_session.hpp), [`swamp_actor_floor_bridge`](../port/irrlicht-android/game/swamp_actor_floor_bridge.hpp) | Owned module0 player session passes Android movement/slide/resume; body is detached before world teardown. Development20ms clock/input and absent environment bodies are explicit boundaries. |
| Authored MGP/MLX Character identities and scripted spawn requests | [`actor-runtime`](../port/actor-runtime/README.md), [`level-runtime`](../port/level-runtime/README.md) | Registry/source trace. A registered or requested actor is not automatically a live rendered Character. |

## Findings that materially changed the approach

- The engine lineage is Irrlicht, with substantial custom `glitch::` interfaces and rendering changes. Upstream source is useful for reconstruction; replacing names alone cannot restore that fork.
- The complete local cache has 6,833 CRC-valid files, 2,904 BRES resources and the Prince model. An older partial-cache manifest is retained as historical evidence and must not be treated as the complete input. See [`COMPLETE-CACHE.md`](COMPLETE-CACHE.md).
- The 219 `.luac` files are plaintext Lua. One original sandworm syntax error has a separately labeled repair. Exact cache bytes remain distinct from authored overrides.
- Adam's Crypt is `x07_crypt_backup.mlx`, an authored eight-room layout. It is not proof of procedural `007_crypt_01.rule.xml` generation or a completed campaign level.
- Adam's runtime connects far more native gameplay than our Irrlicht preview. Reusing its renderer-neutral scene/animation/skinning/physics logic is the strongest route forward.
- His GLES2 draw loop cannot be pasted into Irrlicht's draw loop. One renderer must own the surface; shared runtime state must feed it.
- Skin matrices already incorporate scene transforms. Applying player placement twice would produce incorrect rendering and bounds.
- The first Irrlicht Prince adapter compiled and passed source geometry/movement tests, but actual screenshots exposed a clipped head in Idle and disappearance in Walk. A fixed Idle bounds anchor and the old sphere camera were insufficient. This regression is retained in [`irrlicht-prince-visual-regression.json`](../reports/reconstruction-2026-10-04/irrlicht-prince-visual-regression.json); source motion logs do not prove visible actor correctness.
- Reusing Adam's source visual binding corrected clip-root placement; reframing the development camera restored the full actor view. The black bridge overlay traced to source draws 13/24 using `Material__11598`: depth writes alone had been mapped, leaving blending disabled. The corrected Irrlicht path maps explicit ONE/ONE factors and ADD, preserving the geometry. This fixed the broad opaque-black occlusion; independent floor/material gaps remain.
- The older `SceneMesh` uses 16-bit combined indices and a 65,535-vertex cap. Full worlds and actors require split buffers or wider intermediate indexing.
- Infected Village Ambush references `_prim_tmp_infected17`, for which no static MGP/MVP record has been resolved. The trace records a lookup miss rather than inventing an actor.
- Full campaign save semantics, object factory resolution, generated levels and Lua/AI ownership remain major integration work even though isolated numeric tests are extensive.
- Source review found the old scheduler dispatched a completed Wait in the same update that reached its duration. Original ExecuteScript checks blocking first, updates once and returns; the corrected source preserves that frame delay. Scripts run before physics/ObjectManager, so contact is consumed next frame.
- GhostAmbush01 owns the direct SURPRISE01/02 pair. GhostAmbushHallway owns four different weighted-template actors. The source tables for both paths are available; their factories and integration are separate tasks.
- The importer and live runtime had incompatible scene/world/triangle types and an overlapping C symbol. Serialized scene records now use `scene_payload`, imported worlds use `SourceLevel`, imported triangles use `SurfaceTriangle` and `dh2_nav_get_triangle`; Adam's live types remain distinct. A single executable links both implementations and reads all nine SWAMP modules/626 imported triangles. This removes the production link blocker without reinterpreting incompatible layouts.
- The old Character vptr+0x40 mapping used the wrong table address point. Original bytes prove it is `SetVisible`, while `IsUpdatable` is +0x38. Limbus hides the object, and blur restores visibility from its enabled byte. The native Ghost owner now follows that service and continues updating timers while invisible.
- Respawn delay is read twice in Limbus focus; the second result is not checked again and the timer's return is ignored. Group respawn requires all members in Limbus, while Limbus blur's role3 branch asks whether any other member is in Limbus. Treating these as the same group predicate changes game behavior.
- The two direct Ghost variants resolve actual AIProps row68 (`Ugly_Dog`, Type4, Script=`monster`). Plain `monster` selects `AISExternal` and the recovered `data/scripts/ai/monster.luac`; `__monster__` selects a different built-in AIS. The next enemy integration must preserve the external Lua callback and native candidate-search path. This finding is evidence, not completed AI integration.

## Test evidence and its limits

The [source frame ownership checkpoint](SOURCE-FRAME-OWNERSHIP-CHECKPOINT-2026-10-05.md)
binds local APK `99dc04b8...` to 466 unchanged compiler inputs and 1,314
component/compiler source-hash checks. Original ARM replay passes 246 Stop
cases, 67,594 Character physics-policy cases and 1,024 base-policy cases, plus
18 bounded scheduler cases covering all 127 reached instructions. Focused bridge
and scheduler sanitizer gates, independent bridge/Owner review and six Session
regressions pass. The actual API37/16 KiB Crypt regression also passes, including
damage/VM retention, reload and rotation. The bridge is a port ownership adapter;
none of these source prerequisites establishes autonomous native pursuit or a
finished level. Shared scheduler, culling/zoning and path/body owners remain open.

The [native frame foundations checkpoint](NATIVE-FRAME-FOUNDATIONS-CHECKPOINT-2026-10-04.md)
binds APK `1b3a2551...` to 460 unchanged compiler inputs and the actual API37/16 KiB
Crypt regression. The classifier passes 514 original ARM comparisons/all 130
instructions; IsZonable passes 45. Complete CanUpdate passes 26 primary/348
independent original comparisons/all 77 instructions plus pointer/failure guards.
The six Session-dependent host gates were rerun against the extended VM implementation.
Actual Ghost selection uses the new classifier; zonability is diagnostic, while
CanUpdate/state callbacks/resolved-path loading still lack native frame/path owners.
DACT instance names and normalized port death are adapter inputs; original producers
remain open. The retained empty-skill/five-null-faery path does not prove nonempty skills.

The [native Ghost skill initialization checkpoint](NATIVE-GHOST-SKILL-INIT-CHECKPOINT-2026-10-04.md) binds APK `0dbb44c7...` to 452 unchanged actual compiler inputs and 495 component source-hash checks. It passes actual Crypt touch/spawn/original OnInit, all five bounded Ghost initialization phases, real private Debug I/O and damaged-health/same-VM/vector/catalogue retention through reload and rotation on API37/16 KiB. Root inspected the restored Prince/Ghost view. All 183 Skill/Faery records match original readers; their owned catalogue and source state predicates are now linked. Independent new OnSkillUpdate and Usable/Active source callers, plus a real-Lua-tested per-VM resolved-path helper, remain unlinked. The world setup adapter omits full Character scheduling gates; five null faery slots do not establish nonempty script support. Earlier reports retain their exact historical build identities.

The historical source-alpha preview APK `81286903...` (72,063,674 bytes) passes Android17/API37/16KiB rendering, movement, source navigation boundary sliding, body pin/unpin and same-process pause/resume. Built and installed SHA-256 match. Root inspected initial, boundary-held and resumed screenshots and compared the original starting view against `c7094b53...`: the prior opaque dark cards are visibly removed. The shader preview is explicitly labeled because the original AL/AT technique and blend/depth state are unresolved. See [current runtime evidence](../reports/reconstruction-2026-10-04/source-alpha-preview/runtime.json) and [source audit](../port/irrlicht-android/reference/swamp-render-material-audit/NOTES.md).

The acquisition-prefix, both sight-query overloads and bounded already-target threat-switch/cleanup branch are now implemented and independently tested: 40 + 35 + 33 original ARM comparisons passed, with 45 + 32 + 28 host cases. These add bounded source behavior, not three completed original AI classes or live Ghost pursuit. An actor-owned search/event/Lua/controller composition passed eight host case groups; live Android service binding remains the next integration gate.

The latest [acquisition/alpha checkpoint](SOURCE-AI-ACQUISITION-ALPHA-CHECKPOINT-2026-10-04.md) packages thirteen source AI/script units and the unchanged original monster scripts for both native ABIs. Crypt APK `1193fa7d...` (24,822,127 bytes, 243 assets) passes Android17/API37/16KiB original ambush/contact/timed-spawn, source visibility/body creation and reload/recreation tests. A 48-input production snapshot matches artifact validation and the installed APK hash. Live enemy pursuit/attacks remain pending; compilation and isolated composition tests are reported separately from gameplay integration.

The current [source AI frame checkpoint](SOURCE-AI-FRAME-CHECKPOINT-2026-10-04.md) adds nine AI/AIS units: existing-enemy retention, complete target/master/range caller orchestration, external AIS update, state-callback leaves, pause/timer requests and the genuine collision-counter producer. Root original-instruction comparisons pass with zero mismatches; host/guard/FSM/provider boundaries are reported separately. Actual NDK29 ARM64/x86_64 library links pass with ELF64/16KiB alignment and kernel hashes matching component reports. These AI additions are not inside the previously tested thirteen-unit Crypt APK. Real native owner/services/pursuit integration is being implemented.

The selector-driven Irrlicht APK `9bf79739...` (72,116,922 bytes) now decodes both GLES material selectors, deriving AL for the 22 module-zero alpha draws. A frozen 133-input production snapshot matches the build. Android17/API37/16KiB rendering, touch movement, navigation-boundary sliding and same-process pause/resume pass with matching built/installed hashes; root inspected the screenshots. See [runtime evidence](../reports/reconstruction-2026-10-04/source-selector/runtime.json). Original runtime profile/effect group, compiled shader ordinal, blend/depth state and unresolved texture references remain pending.

The newer [source AI and room-render checkpoint](SOURCE-AI-ROOM-CHECKPOINT-2026-10-04.md) adds nine tested AI/script source units compiled and exported for both Android ABIs, plus the scenery-role correction. Crypt APK `aecba18e...` passes the original trigger/spawn/reload/recreation regression on API37/16KiB; live Ghost pursuit remains pending. Irrlicht APK `c7094b53...` passes unchanged navigation/body/movement/resume checks and visual inspection with white navigation polygons removed. All-nine module classification preserves626 navigation triangles. These newer local candidates have separate artifact/source/runtime reports; prior release identities remain unchanged.

The latest [Crypt and Irrlicht checkpoint](CRYPT-SCRIPT-CHECKPOINT-2026-10-04.md) records Crypt APK `df725260...` (240 assets) and Irrlicht APK `2e3464ef...`, exact mapping/host checks and Android 17/API37/16KiB results. Actual touch/root-motion travel into the unchanged authored trigger ran both original timed Spawn requests. Native reload and rotation preserve the consumed trigger, Ghost Idle and restored source visibility without replay. The test used an explicitly labeled fan player-start override near that trigger; it does not claim a complete level playthrough. Irrlicht now passes a linked source player-body/session test: free +X/-Y motion, +Y input redirected along a validated +X boundary, Move-owned unpin and Stop then Pin, and HOME/resume with matching actor/world-step counters. Current reports bind 130 Irrlicht build-source hashes and 63 host-source hashes to the tested workspace. The [previous shared Character checkpoint](CHARACTER-RUNTIME-CHECKPOINT-2026-10-04.md) retains Crypt APK `2b7b064c...` and Irrlicht APK `b367d0fa...` identities and prior tests.

We independently rebuilt Adam's pinned source, rebuilt the imported branch app, and built it from a clean checkout. On Android 17/API 37 x86_64 with `PAGE_SIZE=16384`, Crypt loading, visible Prince rendering and joystick movement passed. An earlier attack tap returned the correct out-of-range message; that observation alone did not prove a successful hit.

Adam's repository includes substantially deeper movement/combat/lifecycle and original-instruction reports. Those remain attributed inherited evidence until the matching current build is retested. The evidence restore copied 277 missing public files (3,677,855 bytes) without replacing implementations and repaired 201 of 214 missing relative-link occurrences. The 13 remaining references require intentionally omitted compatibility payloads, old APK/source archives or an owner-local fixture. Source import alone does not revalidate the inherited results.

Current continuation adds a native mod-file boundary with host checks for valid overrides, absent-file fallback, unsafe paths, unreadable/non-file entries, size limits and unchanged output on rejection. The exact continuation APK also passed touch walking/running, both playback slots, actor freeze/resume, finite attack closure and Activity recreation. Three successful hits reduced a nearby skeleton's HP from 12,160 to 11,556. A real external world override shifted the native spawn by 100 game units; a malformed override rejected without crashing, and removal restored the packaged baseline. See [the build/test checkpoint](NATIVE-BUILD-CHECKPOINT-2026-10-04.md) for artifact identity, structured results and limits. Modern Android targeting is consistent with the official [Android 17 release documentation](https://developer.android.com/blog/posts/android-17-is-here).

The earlier Irrlicht APK `eeb3b1932b5584c1d9766bfc86e239ab3edfd9f4033407bfb4e2424491dfe9a0` passed source geometry, atlas/material diagnostics, both joystick axes, Walk/Idle selection and release stability on the same 16 KiB emulator. HOME/background followed by same-process Activity resume preserved `(1099.104,-203.522,255)` and the visible Idle actor. Manual inspection confirmed head-to-feet rendering in Idle, Walk and resume, and removal of the prior opaque-black overlay. Its [structured evidence](../reports/reconstruction-2026-10-04/irrlicht-prince-runtime.json) retains that artifact identity; the current `2e3464ef...` actor/body build has [separate runtime evidence](../reports/reconstruction-2026-10-04/crypt-script/irrlicht-swamp-runtime.json). Neither establishes process-death saves, full Character playback, campaign gameplay or complete scene fidelity.

![Source-skinned Prince walking through Irrlicht after transform, camera and blend fixes](images/irrlicht-prince-walk-2026-10-04.png)

## Remaining work, in implementation order

| Milestone | Deliverable | Pass condition |
|---|---|---|
| 1. Combined source/evidence ledger | Consolidated branch snapshot, mapping inventory, retained attribution and repaired reference links | Fresh checkout builds; readers can follow original evidence to implemented source and see test/integration limits. |
| 2. Complete Irrlicht actor rendering | Four-skin complete-bank/shared-state Idle/Move slice passes; extend original camera, materials and all states | Original actor behavior and appearance preserved across all states; development producers replaced where original behavior is recovered. |
| 3. Shared level-owned runtime | State/timer coordinator and source player body/navigation session now feed Irrlicht; extend environment bodies and module/object ownership | Same source actors, physics and animation operate through Irrlicht without duplicate update/draw ownership. |
| 4. Live enemies and factories | Two direct Ghosts now source-spawn; resolve weighted templates, stable actor ownership, original candidate search and external `monster.luac` dispatch | Enemy acquires/pursues/attacks player through reconstructed services; death/despawn/spawn paths work. |
| 5. Full gameplay loop | Classes/equipment/skills/status, triggers, quests, loot, XP/rewards and transitions | A real level is completable and advances campaign state with original data. |
| 6. Content and presentation | All authored/generated levels, UI, camera, audio, effects and full asset pipeline | Coverage matrix and repeatable playthroughs; known visual/audio gaps closed. |
| 7. Saves and modding | Persistent versioned campaign state, original-save handling where supported; validated fan data/script changes | Process-death/update restore, baseline recovery and documented compatibility contracts. |
| 8. Release verification | Clean ARM64 build, alignment proof, physical current-Android gameplay and regression suite | Installation and sustained gameplay pass; source/build/mod documentation complete. |

The goal remains active. No estimated completion percentage or promised completion date replaces these gates.
