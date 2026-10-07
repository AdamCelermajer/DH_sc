# Act 1 coordination handoff for a new session

## User direction

Organize approximately 20–30 GPT-6 Luna workers, each with high reasoning, to finish the native Dungeon Hunter 2 reconstruction. Use the IDA export and original assembly as the reverse-engineering reference. Stop using Ghidra exports for new reconstruction decisions. Preserve useful existing C++ implementations; changing the reference is not a request to discard the code.

The user requested basic organization only in the original session and will start the coordinating session separately. Development remains paused there. Two newly started Luna workers were interrupted before this handoff; do not count them as active or their work as completed. Inspect any partial changes before reassigning their files.

## Goal and actual state

First deliverable: launch → main menu → selected/new character → genuine Swamp → movement, chest, melee and skill kills → loot/XP → save and reload.

Final Act 1 deliverable: complete The Boglands, including the Witch Cave, objectives, dialogue, tutorials, companions, bosses and chapter exit, with working shared gameplay systems and one bundled ARM64 APK.

Last observed installed checkpoint: V120 x86_64 emulator APK. It mounted the bundled cache and stopped at `Source process startup: Original compiled Structs member metadata differs`. Act 1 has not been accepted as playable. A data correction and substantial V121/V122 preview/input/character source changes exist but have not been built and exercised together. Refresh their state; do not implement them again from an old gap list.

Existing source is extensive. Each worker must finish its assigned missing behavior and connect it through existing owners, rather than starting a parallel replacement system.

## Reference locations

- Workspace: `C:/Users/adamc/Desktop/workspace/DH_sc`
- IDA handoff: `C:/Users/adamc/Desktop/workspace/DH_sc/docs/IDA-EXPORT-HANDOFF-2026-10-07.txt`
- IDA evidence: `C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/ida-apk-export-2026-10-07`
- Engine function index: `libraries/libDungeonHunter2.so/functions.jsonl` beneath that evidence root.
- Exact function bodies: `libraries/libDungeonHunter2.so/pseudocode/<address shard>/<eight-digit address>.c`.
- Assembly and references: the same library's `assembly-functions.asm`, `xrefs.jsonl`, ELF symbols and relocations.
- IDA MCP: `http://127.0.0.1:8746/mcp`; static exports suffice for most tasks. Shared live database access must be coordinated.
- Current tracker: `C:/Users/adamc/Desktop/workspace/DH_sc/docs/ACT1-DELIVERY-TRACKER-2026-10-06.md`
- Earlier chapter audit: `C:/Users/adamc/Desktop/workspace/DH2-LEVEL1-GAP-LIST.md` — useful inventory, outdated completion statuses.
- This proposal's source chat: `01a1116e-69ac-79d1-b424-883d16e478dd`, **Coordinate Act 1 rebuild sessions**.

IDA output is inferred pseudocode, not compilable original studio source. Verify important signatures, pointer lifetimes, branch conditions and call order against original assembly/callers. Keep original serialized 32-bit data distinct from native 64-bit objects.

## Proposed 24 work lanes

All lanes below are **unassigned**. The list describes deliverables, not confirmed missing implementations. Refresh current source before dispatch. Split responsibilities by owned files and actual interfaces; lane names alone do not prevent conflicting edits.

| ID | Lane | Concrete deliverable |
|---|---|---|
| 01 | Startup data and tables | Finish the existing compiled-member/Arrays correction and any reached startup data failures. |
| 02 | Application startup and main-menu flow | Connect real initialization, intro completion, menu Show/Hide and Play routing. |
| 03 | Selected character and saved equipment | Finish canonical preview Character/Save/Gear construction, restore and destruction. |
| 04 | Preview rendering and camera | Draw that actual character/equipment using the retained camera; correct preview transforms and layout. |
| 05 | Level activation and transitions | Finish generic Swamp/Witch Cave lifecycle, resources, active-world handoff and area transitions. |
| 06 | Gameplay HUD and character entry | Original portrait alignment/click, HP/MP/skills/faery/potions and input placement. |
| 07 | Targeting and focus | Shared acquisition, red-ring animation, facing and target retention/cleanup. |
| 08 | Skill animation and effect placement | Shared anchors, headings, camera-relative appearance and cast/hit/end timing. |
| 09 | Enemy damage, death and skill-kill safety | Complete damage/death cleanup without black screens or stale target/effect references. |
| 10 | Player damage, statuses and recovery | Player hit/death states, status effects, critical-health pulse and potion consumption. |
| 11 | Loot drops and pickup | Real authored drop generation, world item rendering, pickup animation and inventory transfer. |
| 12 | XP and level progression | Original reward calculation, XP display, level-up animation, stats and skill-point awards. |
| 13 | Chests, destructibles and interactions | Target/open/break/state changes, authored contents and linked conditions/events. |
| 14 | Character and inventory menus | Original artwork/layout, stats, equipment, item actions and actual character preview. |
| 15 | Skill and faery menus | Original tabs, descriptions, upgrades, mapping and faery selection connected to gameplay. |
| 16 | Save, reload and respawn | Selected profile, character/inventory/quest/world state, checkpoints and death recovery. |
| 17 | Level scripts | Finish existing shared command execution, nested calls, waits and world-service bindings. |
| 18 | Quests, conditions and events | Original quest progression/rewards and condition/event dispatch for Act 1. |
| 19 | Dialogue and tutorials | Original text/name substitution, choices, tutorial sequencing and gameplay gating. |
| 20 | Cutscenes and cinematic cameras | Authored camera/actor sequencing, skip and clean return to gameplay. |
| 21 | Ordinary enemy AI and spawners | Shared movement/aggro/attacks and the Act 1 enemy/spawn families. |
| 22 | Companions, faery and bosses | Rene/Celeste behavior, Bogwitch and Mud Lord authored encounters. |
| 23 | Combat and world audio | Actual swing/hit/skill/monster/chest/item requests, sound resources and playback. |
| 24 | Chapter presentation and completion | Music/ambience/UI audio, journal/map/merchant connections and Act 1 exit conditions. |

Lanes 01–06 unblock the first playable flow. Lanes 07–16 complete its core gameplay. Lanes 17–24 advance chapter progression in parallel where their owned paths are independent. Dependent lanes can inspect IDA/data and prepare narrowly scoped source changes while an interface is unfinished.

## Coordination rules

1. One coordinator assigns work and tracks dependencies. One integrator owns the combined application, CMake/build wiring, APK and emulator. These roles may be the same session.
2. Inspect active chats, subagents, Git state and uncommitted changes before dispatch. Existing work must survive. Do not reset checkouts to an older published checkpoint.
3. Use GPT-6 Luna with high reasoning for workers. Verify that each worker actually has these settings. Report the actual concurrency limit; do not claim 24 active workers when only three are running.
4. A work lane is not necessarily a simultaneously active session. Schedule within available capacity. If the user starts multiple independent chats, give each its own assignment and ownership boundary.
5. Separate worktrees must include or explicitly receive the needed current uncommitted source baseline. A checkout from remote main may lack the latest integration work. Keep cache/IDA data in the shared evidence location rather than copying them into every checkout.
6. Before implementation, record each worker's exact writable paths, reused interfaces and dependency owner. Only one worker edits a file at a time. Shared-file changes go through the integrator.
7. Keep meaningful systems together. Avoid 24 workers producing unrelated helper functions that nobody connects.
8. Workers report concise interface deliveries, coherent completion and concrete blockers. Do not poll continuously or require repetitive acknowledgements.
9. No worker builds, emulators, WSL jobs, per-function test runs, hash inventories or large proof packets. Comprehensive validation happens at a large integrated milestone, owned by the integrator. Source inspection for implementation is expected.
10. No invented rewards, forced loading readiness, fake World/Level instances or success-only stub callbacks. Report unsupported branches honestly.
11. Performance optimization and WiFi/online-service work remain deferred unless required by the first playable flow.

## Single task ledger

Use one shared absolute location for the ledger and handoffs; directories inside separate worktrees are not automatically shared. The suggested shared directory is `C:/Users/adamc/Desktop/workspace/DH_sc/coordination/luna-act1`.

Columns: `lane | worker ID | model/reasoning | owned paths | deliverable | dependency | status | integration state | remaining acceptance`.

Statuses: `queued`, `working`, `source delivered`, `integrated`, `milestone verified`, `blocked`.

Mark **V** only for the explicitly verified milestone scope. Source delivered is not proof of playable Act 1.

## Opening prompt for the new coordinator

> Coordinate the DH2 native Act 1 rebuild using this handoff. Organize 24 substantial implementation lanes with GPT-6 Luna at high reasoning, using IDA pseudocode and original assembly exclusively for reverse engineering. First refresh existing source, chats and ownership, especially unbuilt V121/V122 startup/menu/preview work. Preserve it and finish missing connections. Assign independent writable paths and actual interfaces, track the real worker count, and keep one APK/emulator integrator. The first milestone is main menu → selected character → genuine Swamp → chest and lethal combat → loot/XP → save/reload. Comprehensive testing happens at integrated milestones. Maintain one concise task ledger and advance coherent deliverables without restarting completed systems.
