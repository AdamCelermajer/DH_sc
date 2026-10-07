# Playable Act 1 delivery tracker

## Current delivery gate - 7 October

Latest checkpoint: V120 startup source built, linked, packaged and installed on
the visible emulator. First-boot GSInit is now connected to the
original intro completion, process trophy creation, phased MenuManager resource
initialization and constructor-only PlayerManager preparation. The process Zoom
owner is retained before a World exists and reused by gameplay. Selected menu
Hide and the typed application input handler are included in this checkpoint.
Fresh launch mounts all 6833 bundled cache files, then stops with
`Source process startup: Original compiled Structs member metadata differs`.
This is earlier than the separately confirmed preview lifecycle blocker. Act 1
does not run. Evidence: `.local-inputs/act1-startup-v120-build.log` and
`.local-inputs/act1-startup-v120-live.log`; screenshot has the same stem as PNG.
The APK is an x86_64 test build, not the final ARM64 phone package.
The first failing data group is Arrays stage1, `ai_factions_pyarray.bin`.
Its companion schema lists the nested `{Id, Value}` record before the outer
`{factions}` record. The parser assigns that nested schema to the outer table,
and the generated outer-table descriptor incorrectly declares zero members.
Both schema association and the one-member descriptor must be repaired. This
diagnosis used the bundled schema and recovered getter; no runtime bypass was
added.

Source review identified a further definite startup dependency: loaded main-menu
and class-selection clips have valid7c set before Init24 calls their Hide
overrides. Those overrides require the genuine preview scene/avatar cleanup and
reconstruction lifecycle. That lifecycle is still missing; the existing demo
persona camera callback is not a replacement. The new startup batch retains an
explicit failure for these positive paths rather than clearing validity or
pretending cleanup succeeded.

The typed App event handler and cursor-slot facade are implemented, but the
Android input producer must still publish the typed payload to the actual App14
event dispatcher, and menu consumption must update its shared consumed flag.
Those final connections are not yet complete. Existing direct front/HUD input
must not be counted as proof of the new App-event path.

| Missing milestone coverage | What remains |
|---|---|
| Boot to Swamp scene | Fix the actual startup Structs metadata mismatch, then authentic menu preview lifecycle and remaining reached loading failures |
| Full Act 1 progression | Exercise chapter scripts, objectives, encounters and linked-area transitions through chapter completion |
| Combat and rewards in the campaign | Verify targeting continuity, skill alignment/kill cleanup, damage/status, loot and XP together in Swamp |
| Character interface | Verify portrait entry, original stats/items/skills artwork, interactions and return to gameplay |
| Audio | Verify authored combat, monster, chest and item playback; unavailable cues still need their real resources |
| Persistence and final device package | Verify save/reload with selected profile, then build and exercise ARM64 single APK on a physical device |

The visible emulator is booted with the requested 15 GiB guard. The integrated
native source now builds, links and packages successfully. The new x86_64 debug
APK is installed. This proves the emulator build, not the final ARM64 phone
APK or playable Act 1. The real APK output is
`port/android-native/app/build/intermediates/apk/debug/app-debug.apk`; the older
`build/outputs/apk/debug` copy is stale and was not installed.

| Delivery requirement | Current evidence | Acceptance |
|---|---|---|
| Visible emulator | New V120 installed game is visible with its actual startup error | V for launch only |
| Native build and bundled APK | Native libraries linked, Gradle assembleDebug succeeded, emulator APK installed with cache inside | V for x86_64 debug build |
| Main menu, profile selection, Play to Swamp | V118 menu/Play was exercised; V120 authentic first boot stops earlier at Structs metadata ingestion | Pending |
| Character/item resource startup | Actual decoded cache owner loan and original merchant/quantity suffix parser repaired; next startup consumer reached | V for these resource handoffs only; gameplay pending |
| Campaign audio resource lookup/initialization | Correct Listener URI and whole process initializer now pass; original preload ignores unavailable sample handles, retaining explicit per-cue failures | V for initialization; playback/full parity pending missing clips |
| Swamp rendering, movement, targeting and skills | Shared renderer/combat source joined | Pending live gameplay |
| Damage, mobs, chests, loot, XP, audio and character menus | Source implementations integrated; no claim of chapter completion | Pending live gameplay |
| Saves, reload and linked-area progression through Act 1 | Must be exercised on the new campaign path | Pending |

The loader's LevelEC repair passed the fresh startup attempt. Android campaign
routing now follows actual native scene activation and displays explicit load
errors instead of claiming Crypt gameplay. Local workers have delivered the
shared cache, merchant parser and target-directory ordering repairs. Root owns
the build, APK installation and emulator campaign. WiFi/online services and
performance experiments remain deferred. Latest build succeeded, APK installed
and original Play exercised in the earlier V118 checkpoint. The combined script/animator batch built and was
installed successfully. Latest V118 world-systems build also succeeded and is
installed as the previous checkpoint. Its Play reaches loading stage6 and actual inventory trophy
notification, then fails because the initial front startup has no completed
process GSInit11/Trophy singleton. Evidence:
`.local-inputs/act1-world-systems-v118-live.log`. The startup worker owns first
process initialization; the movie worker owns real Android intro playback and
completion; the trophy worker owns process services and achievement UI; the
audio worker owns the observed construction cancel and UI retry lifecycle.
Authored FX, AI/faery/Limbus, tutorial and inventory
source are built/installed, but their gameplay coverage remains pending.
No Swamp gameplay acceptance has been earned. Stage6 occurs before later
script initialization: the earlier changed error was not proof of that later
phase passing.

Current script subsystem batch (built/installed, gameplay not accepted): Character C1
now creates its single animator before scripted animation-library registration;
later visual setup reuses that owner. Script execution is being joined for
animation/movement/visibility/stopping, world FX, character/container spawning,
scripted loot, safe-zone music and saved level/map states. Character cleanup
detaches surviving FX from the retiring target; generic GameObject FX anchors
use the original base IsDead result and object-keyed visual lookup. The audio
worker owns authored PlaySound/StopSound/PlayLevelMusic command integration.
Root built/installed this combined batch and exercised original Play, without
worker test runs. The inventory callback is now reached; process Trophy startup
is the next failure. Commands needing actual gameplay remain unaccepted.

| Script subsystem subtask | Source implementation | Integrated gameplay acceptance |
|---|---|---|
| Early actor animation libraries and one animator lifetime | V | Pending |
| Actor play/wait, movement/warp/wait, stop, idle, scripted state and visibility | V | Pending |
| FX play/stop, character/container spawn and scripted loot | V | Pending |
| Saved level/map states using selected Save and process difficulty | V | Pending |
| Script sound/music and safe-zone music commands | V | Pending audible campaign flow; missing clips remain |
| FX anchor lifetime after target cleanup and generic anchor transforms | V | Pending kill/skill and scenery-effect coverage |
| Inventory gold/power/fullness/destruction continuations | V | Pending fresh campaign startup and item flow |
| Authored FX sequences, random selection, redirects and parent callbacks | V | Pending skills/world-effects gameplay |
| Faery/AI/scripted Limbus commands | V | Pending chapter scripts |
| HUD and CharacterMenu tutorial queues, retrieval, callbacks and settings control | V | Pending opening chapter tutorial flow |

The V entries in this table mean source implementation only. The combined
checkpoint build is `.local-inputs/act1-script-system-v117-build.log`; no Act 1
gameplay V is implied by compiling or linking this batch.

Current priority: main menu to the complete Swamp chapter, including required
linked areas and gameplay systems. Performance optimization and FPS experiments
are deferred at the user's request. Keep the visible emulator and its existing
15 GiB guard for functional checks.

First Act 1 checkpoint is offline. WiFi detection, legacy online score
submission, CX protocol/login and other network services are deferred; they
must not block local menu/Play/Swamp delivery. Preserve their partial source in
the backlog and report their scope separately from playable chapter completion.

## Milestone validation cadence

Binding workflow for root, subagents and collaborating sessions:

- Implement complete systems between checkpoints. A helper, file or handoff
  completion does not trigger validation.
- No independent worker builds, emulator runs, helper test/syntax-check cycles,
  repeated peer audits or proof/hash packets.
- Root runs one comprehensive integrated campaign when a milestone below is
  assembled. Repair failures and rerun affected coverage within that campaign.
- Inspect source only as needed to implement behavior or resolve a concrete
  blocker; report system-level progress briefly.
- Keep source-complete and gameplay-accepted separate. Add V only after the
  integrated acceptance scope passes.

Next gate: milestone 1, main menu/selection/Play through authentic Swamp,
selected profile/equipment, rendering, camera, movement/HUD and lifecycle.
Comprehensive coverage remains required at that gate.

Current integration: character-menu language refresh now borrows the live
campaign manager's persistent scene. Publication, Character registration,
removal and partial teardown keep that scene synchronized. Projectile source
and target pointers now use native-width storage; common firing/update/expiry
methods are joined with the existing pool and physical/callback owners. These
are source changes awaiting the single integrated milestone pass, not gameplay
acceptance. No per-helper validation was run for this integration.

Next integration batch: terminal loading now borrows the same enrolled faery
provider as the character menu. Localization follows FakeRemove and lends the
actual ExitZone819 cache byte. Common and Laser projectile methods, collision,
expiry, native-width target pointers and physical/callback sources are linked
for the shared skill runtime. FX startup retains the existing animation Scene
after dynamic visual transfer. Dynamic batch/pose ownership is implemented;
material/shader and real light submission are the next renderer subsystem.
These source integrations still await milestone acceptance.

| Integrated system | Source status | Milestone acceptance |
|---|---|---|
| Character/skill/item menus and faery switching | Same Primary1/profile-icon/V95 owners, real language traversal and faery visual/animator rebinding connected | Pending whole flow |
| Active projectile skills | Shared ordinary/Laser pool, script bindings, updates, physical collision, protected skill callbacks and impact FX/audio connected | Pending gameplay loop |
| Dynamic compiled actors | Pose, movement transform and audio-listener basis use the retained compiled graph | Pending rendering/movement |
| Area transition coordinator | Real SaveAllPlayers snapshot, independent fresh profile, exact request, native retirement/start and logical-profile completion connected; native LoadLevel/2 and specified-render PopAll now joined | Pending integrated transition/load flow |
| Materials/light submission | Same native Scene light ownership and original shader attributes/uniforms in implementation | Pending integrated renderer |
| Authored audio event routing | Dedicated audio session finishing current native character/skill/container/item routes | Pending audible whole flow |
| Buffered gold/XP | Original styles/localization/colors/queue and source HUD draw connected; provider enrolled before warm-start; actual source camera used for projection | Pending rewards/animation gameplay loop |
| SWF FSCommand engine | Scoped actual-fc transport, same captured manager/roster, LoadLevel/2, stack actions, specified-render PopAll, SkipScript and EndLoading source routes connected | Native return-to-main-menu binding in progress; pending integrated authored flow |
| Return to main menu | Retained command/Front state, original Reset/entry and same native retirement joined; real menu request/update frame connected; command blocks competing area/save requests and preserves resumed entry completion; local trophy persistence connected; legacy score submission explicitly deferred | Original main-movie registration and local player removal remain in implementation; offline runtime acceptance pending |
| Shared combat and presentation | Native melee events and player/NPC Lua inputs use the same combat/damage/death/audio owner; V116 player-receiver Hit/Apply added; original Flash/StringManager, critical camera and merchant/cleaner/info callbacks enrolled after Stage26 LoadMenu3 | Low-health sound/settings-job tail and final reactions in implementation; integrated combat acceptance pending |
| Exit zones | Full original touching/unlock/display update and signed idle-sound gate use the actual current receiver and frame audio provider | Pending integrated portal/fast-travel gameplay |
| Failed startup lifecycle | Reached bootstrap/GS/Level prefix retained; existing retirement coordinator supports quiesce/cancel/release/unpublication; failed Save allocations now survive until cache release; unpublished Level/GS cleanup uses actual Lua/cache/event primitives | Native abort request/quiescence/final-alias binding and published-C1 early-failure release enrollment remain required |

No acceptance V has been added. These systems are being assembled for one
comprehensive integrated milestone build and runtime campaign.

Geometry/loading integration status (source work, awaiting milestone 1):

| Subsystem | Implemented source | Remaining before acceptance |
|---|---|---|
| Batch compiler ownership/release | SAME Level158 typed owner, source compiler algorithms and LevelD1 binding | Live resource destruction and full flow testing |
| Batch selection/mapping | Loading21..24 enrolled; actual Module/Character/scenery root, names, attached children and visibility loans | Integrated loading execution |
| Native batch resources | Native C1/reference/Scene ownership, typed compiler/resource/APK binding and static compile/GPU path connected; static visual assignment/retirement | Original/compiled submission deduplication, dynamic mesh/animator transfer and integrated release/rendering |
| Projectile loading | Stage29 sequence, sole process pool, canonical Projectile/Laser family, inherited properties and native Spawn/resource/cleanup enrollment connected | Active firing/movement/collision/impact skill loop and integrated acceptance |

The original menu root now owns and flushes its actual buffered text queue;
Primary1 loading uses a retained process/Level continuation that permits later
same-campaign menu calls after loading succeeds. Spawning/quest source updates
and rewards are connected to V107. Character queued start, AI state callbacks
and zoning are advancing under the same frame coordinator. All remain source
changes awaiting the comprehensive milestone build/runtime pass.

Native batching now lends the launch APK pointer, typed resource allocations
and implemented backend format/count capabilities to the V111 compiler. These
capabilities are a documented modern backend adaptation; authored Config still
sets source batch thresholds. Static compilation/GPU storage is being joined
to the same draw pipeline. Dynamic mesh/animator transfer remains required and
is rejected before destructive visual retirement until implemented.

These rows carry no gameplay V until the integrated milestone passes.

Current source batch: original Primary1 menu loading is connected to Level12
and the retained process menu/session directory. It uses the selected SWF URI,
allows genuinely absent HUD3, defers common PostLoad to the manager, and keeps
partial resources for actual unload. Failed field/render/camera cleanup resumes
after completed destruction rather than replaying it. Startup profile queries
borrow the actual selected inactive Character; ordinary actions still require
gameplay admission. LevelUnload/D1 now preserves the first nested failure.
Character core, the complete original text-buffering effect and remaining
loading stages still require integration. This batch is source-only, with no
new APK, tests or gameplay V.

User instruction: comprehensive validation belongs at BIG milestones, not at
each function or helper. Implement and integrate substantial systems before
testing. Stop individual helper syntax-check/freeze/approval/handoff cycles.
Source inspection needed to implement the original behavior remains part of
development. Retain useful existing fixtures for the milestone suite; avoid
new tests that merely mirror getters or small implementation details.

| Milestone | Deliverable before comprehensive validation | State |
|---|---|---|
| 1 | Main menu/selection/Play -> authentic Swamp scene, selected player/profile/equipment, camera and movement controls connected through native loading | In implementation; next integrated checkpoint |
| 2 | Complete combat loop: targeting/facing/markers/HP, attack and skills/effects, monster AI/damage/death, loot/XP/potions and original character menus | In implementation; validate the complete loop together |
| 3 | Required Act1 areas, quests/scripts/NPCs/chests/transitions, category-complete audio, save/continue and one bundled APK | Goal acceptance remains open; comprehensive chapter and device validation required |

Root schedules the integrated build/test/runtime suite after the corresponding
system is assembled. RAM safeguards still apply; they do not justify repeated
helper-level validation. No gameplay V is earned by individual syntax checks.

Reaffirmed by the user on 2026-10-07: this cadence applies to root, every
subagent and the collaborating menu, loader and audio sessions. Do not launch
independent validation jobs or produce per-helper hashes, freeze/proof packets
or repeated check reports. Report coherent implemented systems and concrete
integration blockers briefly. Fix a known integration error directly without
turning that repair into another broad validation cycle.

Latest instruction applies to review work too: do not repeatedly re-audit
finished helpers or request another peer validation for each small change.
Use targeted inspection only to resolve a concrete implementation blocker.
Root coordinates one comprehensive validation campaign for the assembled
milestone, including necessary failure repairs and affected retesting.

The next validation gate is milestone 1 as a complete integrated flow. Root
owns the single build/runtime sweep, covering main menu/selection/Play,
authentic Swamp loading, selected profile/equipment, rendering, camera,
movement/HUD, lifecycle/context restoration and logs. Workers continue source
implementation until that gate is assembled; comprehensive coverage is
retained for the milestone, rather than repeated after small edits.

2026-10-07 source integration batch: actual GL admission now covers campaign
updates/drawing, HUD/menu/action input and player snapshots. Native frames drain
one retained retirement request before ordinary deliveries; original loading
cancellation and GS/Level release journals precede captured audio retirement,
old-world menu detach and context-generation-aware geometry/FX retirement.
The original ScriptManager InitCommands pass is connected at loading state13.
Typed existing noncharacter factory owners are published for derived gameplay
visitors, and real Module save/restore/visibility exports replace the empty
restore lender. Final menu detach, full source D1 composition and generated-room
primitive enrollment remain open. This batch is unbuilt and earns no gameplay V.

Next source batch: process UI/audio final detach is now connected; generated
RoomZone behavior/list80 and Stage10/17 class loading use actual retained owners.
Stage9 LightSet selection/file loading uses the existing native file pipeline.
Typed physical filters retain original saved masks/shapes/disabled cells, and
per-actor/root GPU retirement removes only matching records. Mandatory process
DebugHUD/confirmation constructors are exposed. Full Character/gameplay startup,
remaining source release domains and integrated Swamp acceptance remain open;
no build or test was run for this batch.

Original startup18/32 bodies are now source-enrolled: same Character list60
preset/FSM initialization, followed by one real object/AI-queue/physics/camera
warm-start sequence. The native early gameplay service producer and Character
core coordinator are still being completed; this is not a passed startup or
playable Swamp claim. Door and other derived updates borrow their actual frame
services without running an extra inherited update. Ordinary per-object release
uses its own GL/draw/physics/contact barrier; complete Level D1 additionally
requires closed world admission.

The early V66 native service composition is now called after the actual V62
class publication and script/preload preparation, before the first GS tick.
Its production object-update inputs still require the completed Character core;
regular gameplay save/group updates and several terminal loading/resource
providers remain open. All current source changes await the combined milestone.

Terminal loading36/37 is now source-connected to the real end-loading signal,
captured loading-menu pop, controller8 unblock and nullable follower/faery
placement. Process RES_PATH retains the original immutable logical prefix;
Stage15 uses that loan and the same audio/driver routing body. Source Level D1
now binds the actual process UI message/HUD flush services. Positive native
faery/online leaves, remaining startup/menu resources, Character core and the
full chapter flow still require implementation/integration and milestone
acceptance. No validation jobs or new APK were produced for this source batch.

Do not treat a completed helper, callback, source file or handoff as a validation
gate. Before milestone 1, spend work on implementation and integration, not
repeated status audits or acceptance packets. At the milestone, exercise the
whole flow comprehensively; fix discovered failures and rerun affected coverage
before the final integrated acceptance pass.

After the 2026-10-06 RAM incident, emulator/build/heavy-test launches are held.
Source work continues. Individual syntax checks are deferred until the large
integration milestone; WSL, heavy builds and the emulator remain off. Menu kernels, actual
weak receiver/update/deletion, shared controller unload and Android exports pass
both ABI syntax checks. Behavioral/link/Swamp acceptance and several platform
providers remain open. This is not a new APK or an accepted Swamp flow.

V means the stated scope passed in gameplay. Component tests alone do not finish
an Act 1 system. The chapter is not complete yet.

| ID | Done | Feature / acceptance | Current state | Owner |
|---|---|---|---|---|
| M01 | V | Original main menu → character selection → Play reaches development Crypt | Tested installed APK c6691313; exact hash/same PID/foreground, no native errors. Front-end flow only | Root |
| M02 | — | Play constructs the real selected profile/GS/Swamp loading flow | Native activation/frame router now checks true loading completion, selected PM/Save/Gear, loaded camera, actual HUD caches and gameplay coordinator before changing loading presentation to source scene. Remaining required stage/player/gameplay bodies are being connected; installed APK remains Crypt | Root + loader integration agent |
| M03 | — | Continue restores actual last saved character, gear, map and position | Positive same-profile save/load publication and restoration remain open | Root + menus agent |
| L01 | — | Original Swamp row/file resolution and generic module loading | Fixed filename and generated-stream LoadFileData140/parser/release system integrated into root; actual file/stream leaves and campaign binding in progress. Milestone runtime acceptance pending | Loader session + loader integration agent |
| L02 | — | Sole GS/C1/current Level, authentic loading stages and completion | Actual four-slot movie/camera lifecycle, live render array, source front draw order and automatic GS installation assembled; source registry/resource retirement allows reload without stale identities. HUD timing/reload, remaining stage bindings and functional Swamp completion open | Loader integration agent + root |
| L03 | — | Full terrain/scenery/materials/light/fog/collision rendering | Campaign GPU path reads actual Module submissions plus selected Gear skin views and ordinary canonical character visual/skin streams. Shared texture/material submission, current transforms/visibility and context teardown connected. Room activation/culling source is written; four App light/fog tweaker owners are enrolled. Room callback integration, final runtime publication and integrated Swamp acceptance remain open | Root + loader session |
| L04 | — | All declared actors/chests initialize through their actual lifecycle | Combined object-family factories and retained zone resources are source-connected. The actual Item factory now prepares before XML and precaches the same pool at stage29. Remaining native family callbacks, startup and gameplay acceptance stay open | Loader session + loot agent + audio session |
| L05 | — | Required chapter areas, portals and level transitions | Source-driven map/quest progression and save boundary acceptance pending | Root + loader session |
| C01 | — | Target acquisition, animated marker, enemy HP and invalid/dead cleanup | Shared source fixes exist; broader actual combat acceptance stays open | Root |
| C02 | — | Attack tap/hold/release/cancel and post-skill reacquisition | Actual APK58f6668: quick tap1 command,500ms hold26, no native frame errors; release/cancel/multi-touch and post-skill enemy continuity still open | Root |
| C02a | V | Quick attack tap and held press reach actual authored command handler | APK58f6668 exact foreground/same PID: tap1, hold26; scope excludes nearby target/release/cancel | Root |
| C03 | — | Act 1 skill effects, facing, meshes, trails and impact animation | Proven stale mesh transform publication fixed generally; new build needs post-fix gameplay checks | Root |
| C04 | — | Surviving and lethal attacks/skills without black screen | Nonlethal Bash live HP 25600→20994 passed; lethal Hit8 death continuation remains open | Root + loot agent |
| C05 | — | Monster spawn/AI/pathfinding/attacks/damage/death/despawn | Actual source actor/physics/event owners must be connected in Swamp | Root + loader session + loot agent |
| G01 | — | Monster/chest drops, visible loot animation and pickup | Item145, pickup/UseOOI and physical publisher implementations staged; genuine stage29 binding needed | Loot agent |
| G02 | — | XP, level-up animation, stat/skill points and rewards | Same Save/PM and full death/reward sequence pending | Root + loot agent + menus agent |
| G03 | — | Potion pickup/count/use and healing | Actual inventory and HUD actions exist in parts; Swamp gameplay acceptance pending | Root + loot agent |
| G04 | — | Item equipment and rendered weapon/armor changes | Actual campaign GPU path now reads the selected Character's restored Gear skin views; shared visual GPU adapter supports equipment/pose/material changes. Generic profile/menu/pickup publication and gameplay verification remain open | Root + menus agent |
| G05 | — | Faery unlock/selection/model/AI/skills/HUD | Positive source visual and chapter unlock/runtime flow incomplete | Menus agent + root |
| U01 | V | Portrait opens original stats artwork on the same live player | Tested installed APK c6691313 normal menu flow; other tabs/actions are separate | Root |
| U02 | — | Original inventory/skills/faery tabs, details, mapping, upgrades and Back | V95 original-movie campaign panel adapter source-linked; actual existing settings/text owners exported for generic player and profile queries. Selected runtime/writer/remaining services and gameplay acceptance pending | Menus agent |
| U03 | — | HUD health/mana/skills/faery/potions/XP and pause/profile navigation | Existing authored HUD; complete Swamp state/lifecycle verification pending | Root |
| Q01 | — | Act 1 objectives, triggers, NPC interaction/dialogue and completion | Distinct Level194 event manager source implementation ready; config/scripts/effects/quest integration remains open | Loader session + root |
| Q02 | — | Required scripted sequences and chapter progression | Native script/config command lifecycle and actual content acceptance pending | Loader session + root |
| A01 | — | Actual SoundManager construction/precache/output/focus lifecycle | Process manager, Stage0 hook, canonical precache hooks, campaign/category bridge and actual event-time scopes integrated in source; completed settings/listener/RNG publication and audible milestone checks pending | Audio session |
| A02 | — | Audible attacks, skills, monsters, chests, loot, potions, menus and ambience | Source routes/decoders exist; positive gameplay transport and audible category checks remain open | Audio session + root |
| S01 | — | Real campaign save, checkpoints and menu write jobs | Single App file/save queue connected to selected-profile restore and actual native frame; complete save/relaunch gameplay acceptance pending | Menus agent + root |
| S02 | — | Save/relaunch restores chapter, quests, inventory, skills and player state | Requires complete live save/load and chapter flow | Root |
| B01 | — | One APK embeds the complete supplied cache and runs the chapter | Embedded cache and ARM64/x86_64 packaging verified; complete chapter gameplay not accepted | Root |

Current implementation assignments:

| Worker | System batch | Remaining integration |
|---|---|---|
| Root | Campaign rendering, selected-player inventory portrait, source condition service integration | Join the workers' systems into the complete menu-to-Swamp flow; own the milestone validation |
| combat_animation | Offline PlayerManager update and selected-player construction/profile/Gear | Complete actual actor startup, HUD/control publication and actor-owned commands |
| level_runtime_integration | Zone/checkpoint/fast-travel source batch; now monster death, XP and loot | Loader attaches zone class/physics binders; worker connects same campaign death/reward/item owners |
| status_progression | Actor script commands complete; now scheduler start/update and cutscene integration | Enroll actual script start sites, source execution phase and PM/process cutscene fields |
| Menu session | Original character panels and native script scheduling/execution | Connect authored blocking/update commands to actual campaign effects |
| Loader session | Non-character lifecycle, static scenery, collision, save/return stages | Compose actual Swamp object services and hand them to the shared runtime |
| Audio session | Campaign audio/category/listener/settings transport | Actual completed runtime publishers and audible milestone checks |

These rows describe source work, not accepted gameplay. Root schedules one
comprehensive validation batch after the corresponding integrated milestone.

## Integrated source batch, 2026-10-07

Root has attached the selected-player startup fragments and actor-script
implementation. Stage26 now enrolls the actual dialogue/UI and actor command
providers before LoadMenu3. A shared typed RenderFX bridge delivers IsSpecTime
to the retained character movie and cutscene callbacks to the retained HUD
root; it preserves the original missing-target/method result and environment.
The zone batch now includes offline checkpoint writes through the existing
profile/Level save jobs, quest movement events and original-art fast-travel
localization/callbacks. Loader class/physical enrollment remains in progress.

The grouped game_objects reader and its shared process snapshot are imported
and source-linked, including door/trigger/container views from the same
streams and dictionary. Door CString/sound/visual fields now have their
actual typed layout. Level camera offsets now remain floats through fog
calculation. These are integrated source changes, not an accepted APK or
Swamp gameplay result. No compiler, build, test or device job accompanied
this batch; comprehensive validation stays at the assembled milestone.

The production renderer now includes the shared non-character composition and
chains it into the existing Character factory once, with the same condition
arena. Zone physics prefixes are retained before construction/assignment in
the existing class release journal. Door/trigger startup now uses the actual
idle-animation signature and a captured nullable SoundManager through its
reached sound loads. The original cutscene PopMenu and IGM query now borrow
the same retained MultiMenu/lifecycle fields. Loader enrollment of the complete
per-class input producer and scheduler enrichment remain in progress; this
source connection is not a claim that Swamp startup has passed in gameplay.

The cutscene runtime enrichment is now enrolled at Stage26 as part of that
same script/UI batch. It uses actual menu-stack PopMenu, the original IGM
field, animation-scaling global and PlayerInfo cutscene state. Level gameplay
StartScript/ExecuteAllScripts now forward to the retained source ScriptManager;
authored start-site/scaling-consumer closure continues. The legacy Crypt trophy
owner is excluded from campaign reward integration: its actual process startup
phase and save/UI transport are assigned to the menu session.

Loading stages33/34 are now attached as complete original algorithms with typed
borrows of the selected retained Character, active controller and existing
Level/camera/config. Location setters update the actual Save cells. Checkpoint
metadata and serialization share the actor's original1468/1474 storage, and
the stage tail calls the existing offline player/Level checkpoint protocol.
The shared scheduler is enrolled before loading starts spawn-point scripts;
Stage26 replaces only its UI/actor services and preserves running contexts.
The remaining scene/light/fog/material primitive enrollment, full character
OBJS serializer binding and Swamp gameplay acceptance are still open.

The source SceneManager update now owns its original float-time accumulator,
Timer sentinel, hierarchy-dirty prefix and root animator traversal; Stage34
calls it directly for0,false without invoking the draw-epoch wrapper. Its
optimized descendant branch requires real typed node callbacks and remains
unaccepted. Source C1 hierarchy/render dirty bytes are corrected to1. The item
owner now has separate once-only factory preparation and stage29 precaching,
and the canonical animation-step/swoosh/audio implementation is source-linked.
Full native item composition, remaining fog/light/material services and the
combined Swamp milestone validation are still pending.

The native filename composition now binds both the root document and later
Module MGP/MVP occurrences to the same CFS/Size/Read/parser authorities. Each
occurrence uses the actual Level140 slot and retained canonical object walk;
close/capture precedes candidate unload. The production build attaches the
V65 closure to the existing dh2_level_world and TinyXML owners once.

The V67 non-Character catalog and actual chest/scenery/trigger/light constructor
dependencies are imported and attached to that same owner. PropertyMap retains
the original declarations and reads SoundEmitter defaults from the actual
GameDesign constants. Actual renderer/physics/script/interaction service
composition and chaining into the single Character factory remain open; source
attachment alone does not establish working mobs/chests or chapter scripts.

The public character-panel connection now selects the actual campaign PM
PlayerInfo/Character and composes V95 on its existing Save/Gear/skills/writer.
Equipment actions mutate that same Gear, whose visual views are consumed by
the source renderer. NativeStartGame's validated selected-slot receipt now
publishes onto the existing offline PlayerInfo664 before player loading.
Stage14/AddCharacter, full writer completion and actor menu continuations
remain under the combat worker's implementation.

No build, emulator or helper-validation jobs ran for this batch. No new APK or
gameplay V is claimed. Milestone 1 remains in implementation.

### Shared scene and object pipeline

The common V68 object graph now binds actual scoped class bases to their native
visual/scene/position/resource lifecycle. Existing containers can lend their
own visual graph for rendering without another constructor. Source GPU drawing
now includes these actual static/skinned object submissions alongside modules
and characters, using the same material/texture/pose path.

Scene animation consumes the already published native Timer and a distinct
draw-delivery epoch once per scene. The source SceneManager registration gate
uses real dirty/cadence/counter fields and concrete native clear/rebuild/cached
primitive-list transport. Cached draws read current source pose and visibility;
neither rendering nor context restoration adds another animation/physics tick.

The same App settings/difficulty manager now survives front movie reload and
feeds campaign saves. Player selection supplies the validated saved class and
slot; pre-gameplay services remain separate from true38 gameplay admission.
AnimatedDecor V73's original initialization body and typed native physical-body
borrows are integrated on the existing constructor authority.

The original condition-list/table/evaluation subsystem and the distinct Level194
event storage/typed lookup are source-linked. Actual quest compile queries,
Stage6/31 event execution, non-Character platform composition, HUD provider
publication and full player combat/control connections remain under integration.
The first Swamp gameplay milestone is not yet accepted. No test/build/runtime
jobs or new APK were produced for this source batch.

### Campaign condition lifecycle integration

Modules and Characters now receive the campaign's actual shared condition
table and compiled-list arena before authored XML initialization. Campaign
Items receive a scoped binding to the same arena. This replaces owner-only
condition services that could handle empty defaults but could not initialize
named conditions. The arena borrows campaign state weakly rather than retaining
its containing World through a callback cycle.

Character and Item removal now clear both embedded compiled conditions before
their storage is dropped. Failed cleanup retains the remaining receiver and
condition owner. The existing Module release path uses that same arena too.
The development Crypt keeps its explicit constructor-empty path outside the
native campaign.

The new player-manager startup/update helpers and campaign event runtime are
attached to the production CMake targets. Quest compile/reward/marker effects,
the complete selected-player startup and the authored Swamp script scheduler
remain in implementation. This batch is source integration; no build, test,
emulator run, new APK or gameplay V is claimed.

### Shared object startup and frame resources

The main workspace now contains the complete shared Zone startup body and the
Door/TriggerObject startup algorithms, including Checkpoint and QuestMove zone
startup forwarding. These use the existing class records and catalog authority;
physical resources, collision nodes, table rows, sounds and external scripts
remain actual typed service calls. Collision/story interactions and remaining
renderer service composition are still open.

The campaign frame environment now lends its own current collision floors,
navigation graph/obstacle registry and published App dt to generic object updates.
Reusable native controller scratch is independent of actor/path state and leased
per delivery, so nested updates cannot reallocate an active caller's workspace.
The motion policy comes from the existing recovered default constructor. This
does not tick physics/animation or borrow Crypt resources. Object-specific
subobject, target and auxiliary methods still belong to their actual receivers.

These source batches remain unbuilt and untested until milestone 1 is assembled.

### Fresh campaign effects backend and quest compilation

The real loaded Character visual now publishes the campaign's native effects
instance backend before target-indicator allocation. It borrows the existing
App-owned libraries and original cache rather than the Crypt runtime, with
actual anchor position/root rotation/scale/life flags, floor queries and camera
view. Library precaching remains at its original stage.

Effects scene sampling runs once per native scene epoch; effects-manager update
is connected at the existing gameplay update site. Particle and mesh drawing
now use the campaign's GPU/cache/material transport. Quest marker allocation,
source visual ownership and animator looping use that same instance backend.
Generic object base borrows expose existing graph records for authored waypoint
and anchor access without a copied actor or a raw void-pointer cast.

Whole Quest compilation is connected on the existing saved quest records,
including shared objectives, prerequisites, rewards, registration and NPC marker
protocol. Remaining object/character-cache producers, positive effect families,
dialogue UI and complete selected-player startup still need integration. Effects
teardown must follow the actual marker/character cleanup order; it is not forced
at cancellation request. No build, device run or gameplay V is claimed.

### Static scenery, hidden floors and live generic object updates

The complete static/hidden visual batch is integrated into the main workspace.
Retained scenery now keeps authored hidden geometry in the same scene, with
separate visibility and attachment state. The static parent path performs the
existing source bake algorithm; dynamic/animated parents retain their animation
path. Detached floor meshes do not render or become visible again.

The retained-visual PFRoom overload loads floors from that same scene/resource
graph. Floor clones pin the immutable read bytes independently of Visual or
World ownership. Existing material, skinning and sole scene-clock behavior are
retained.

Generic navigation/subobject updates now query source boundary Debug only when
reached, read auxiliary state after its actual update, and read target XYZ at
the actual target phase. Constructor-base admission allows scripts to borrow
existing waypoint storage before visual initialization. TriggerObject custom
condition destruction clears its compiled slot only after the real list is
destroyed, then performs the inherited cleanup.

Character template table loading is attached to the existing game-data target
for all maps. Full non-character factory/service composition and selected-player
startup still remain open. No compiler, build, test, emulator or APK jobs ran;
milestone 1 still awaits the assembled flow.

### Integrated constructor, environment and lifecycle source batch

The whole non-character startup producer is now enrolled on the actual campaign
World. Its release journal retains admitted constructor/resource prefixes; class
factories share the existing Arrays, conditions, scene graph and native physical
peer services. Object-manager Remove/Flush and no-room handling use the actual
object records and mutable room/flag cells. Late Stage34 forwarding reads the
live native provider rather than a stale service copy.

The environment uses one process names snapshot and the scene's existing
LightSet name facet. Original Level material/light refresh leaves and global
fog behavior are connected. The native Item worker can borrow the same typed
physical services at its reached operation. Save/restore and group/spawn source
units are linked; original UI text startup binding is included, awaiting the
menu worker's genuine persistent GSInit enrollment.

This is an integrated source batch, not a tested APK. Remaining milestone-1
work includes genuine process startup, PlayerLightTweaker/generated room-zone
providers and selected-player/combat/item service completion. No compiler,
build, test, emulator, hashes or helper proof packets ran for this batch. The
single comprehensive acceptance sweep remains at the assembled milestone.

### Object families, shared contacts and non-character persistence

The optional RoomZone/TriggerTrap, ColBox and Floor source families are attached
as complete catalog/native source units. Provided family services now reach the
single production factory instead of being discarded. Character collision
callbacks borrow the same typed peer-owner/visibility services already used by
zones and the item system, covering cross-family contacts on the existing world.

The V89 non-character serializer and its genuine ObjectBase visibility-write
observation are connected to the existing V86 save directory before map XML.
No separate save state or actor registry is created. Native Door/Container/
Trigger restore leaves and Module/LevelConfig projections remain in the loader
worker's whole save/room packet. The combat FSM source unit is enrolled; its
worker is completing the actual ObjectManager update/frame dispatch. Pickup
quest events use the captured current Level's existing EventManager, as shown
by the original call site; no duplicate GS event queue is introduced.

Source integration only. No new gameplay V, compiled APK or emulator result is
claimed. Menu startup, map startup, combat frames and item producers are still
being assembled for the single comprehensive milestone acceptance run.

### Room runtime, light/fog startup and Item145 ordering

RoomZone now has source implementations for occupant-list add/remove, initial
XY membership, activation/deactivation, camera-frustum culling, visited-module
writes and its one-time object-list initialization. The manager retains actual
room24 and visible-countf8 storage; its UpdateRooms calls those actual rooms.
The camera receipt reuses the existing original projection/frustum math without
an extra scene update. Loader integration is connecting the real room callbacks
and generated Module/RoomZone ownership.

The four original player-light tweaker owners are enrolled on the existing
Application, including original XML loading, actual field storage and Stage34
fog writes. The Item145 producer is now ordered before map XML/catalog calls,
while its same pool is precached at actual loading stage29. Native item
presentation and final class/frame providers are still being connected.
QuickSave source linkage and journal lifecycle/frame exports are available to
the combat and Continue workers. Source-only: no build, test or emulator run.

### Process menu directory and captured loading-menu lifecycle

The native menu directory can now be constructed on the real Application before
any World, with original empty movie/stack slots. Prefix, post-movie, resource
and callback bindings use that same process owner. Actual movie publication
registers each loaded primary into its existing stack/roster; gameplay attaches
to the same manager rather than rebuilding it. Startup service creation no
longer implicitly loads the HUD. The original GSInit/MenuManager.Init driver
still owns native constructor order, movie loads and phase advancement.

Captured loading-menu operations retain the actual menu receiver and callback
context separately from stack projections. Native deletion retires that receiver
logically while outstanding capture storage remains safe. Continue can Push,
IsVisible and Pop the captured receiver without another name lookup. The full
native startup worker has these process/singleton endpoints; the release worker
has the capture endpoint and its QuickSave/camera/release source units linked.

This batch is source-only. Full startup-driver/frame integration and renderer
retirement still remain, and no new gameplay V or APK acceptance is claimed.
No compiler, tests, build, emulator or per-helper proof jobs ran.
