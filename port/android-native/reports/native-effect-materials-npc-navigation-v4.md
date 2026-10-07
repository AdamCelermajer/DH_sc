# Effect materials and NPC navigation engine milestone

2026-10-05. Root's installed development APK is `0a8a3d26cf2b9c014cdab333cd3aad4c5e97df6fab86921dd56afcbbb3d32e59`. Both ARM64 and x86_64 builds pass. All original assets remain packaged inside one APK.

## Delivered integration

- Actual source shader-name collection shares one retained GL program across matching authored passes. Real GL reflection feeds typed, current material parameters. Full source material equality/order/hash and transparent heap sort replace the former distinct-material boundary. The actual material Matrix4f68, retained texture identity, current color and sampler bias remain authoritative.
- Retained monster script sessions now bind HeadTo/MoveTo/Stop/heading transport before script construction. Each uses its own existing controller, target, runtime and bounded PF route. Navigation adopts the candidate floor, initializes after actual NPC bounds and the registry reset, and suspends/rebinds before old floors retire. Static84 and source PF constructor/extent semantics are preserved.
- Immediate native animation stop and NPC defensive dodge/block source bodies are present. These do not claim complete NPC combat or AI.
- Live nonlethal FIRE now reaches its original post-hit monster command, decreases HP, submits real particle geometry (20 vertices/30 indices first cast; 16/24 on the latest captured repeat), closes clip1234 with event22 and returns to idle without a native frame error.

## Verification and limits

Actual APK-linked GLES material tests pass292 checks across three contexts, including shared program identity, real uniform reflection, isolated material values, original FIRE atlas draws and cleanup. Those tests use declared geometry/current-color fixtures. The separate live receipt proves real scene geometry submission; it is not full original visual-fidelity acceptance.

Actual-cache APK-linked NPC navigation tests pass routing, command gates, Stop, failure prefixes, floor destruction/reload and capability-preserving PF reinitialization. Immediate animation-stop tests pass. Original ARM defensive fixtures previously pass2880 cases. Dedicated texture tests previously pass57 actual GLES checks.

**Lethal targeted FIRE still fails.** Repetition against a nearly dead skeleton reaches required Hit service8 (`hit_controller_kill`). Its whole original continuation needs the same current Level receiver and genuine loot/XP/quest/death services. The source failure still produces a black screen. The old accepted HUD checkpoint remains retained; this new APK is an engine milestone with a known gameplay failure, not a fully fixed-skill or stable-game checkpoint. Physical-device acceptance remains outstanding.

Receipts: `skill-render-live-v1/receipt.json`, `skill-target-repeat-v1/lethal-failure-receipt.json`, and `port/level-world/reports/android-native-owner-tests/{effect-material-gles-v4-linked,npc-script-commands-v1,character-animation-stop-v1}/receipt.json`.

## Current ownership and next work

- Root: integrate and complete lethal Hit8/Ctrl_Kill with same actor/Level/loot/XP/quest/death owners; coordinate source contracts and retain truthful failure prefixes.
- `hud_gameplay`: full NPC attack/controller/state/animation/actor movement integration, beyond the now-tested navigation transport. Focused actor-generic NPC command/frame composition passes; live combat handoff remains unfinished.
- `world_target_health`: full LevelConfig/Module source load, including authored SWAMP subscenes, static visual registration, PF rooms/exits and generated RoomZones. Nine selected subtrees and sixteen actual floor records pass focused native tests; whole nine-module acceptance remains unfinished.
- `combat_animation`: original authored Character Sheet/Inventory/Skills menus on the sole CharacterPanelSession/movie/stack; genuine profile queries/actions, source geometry/input and runtime menu flow. Strict compilation passes; genuine lifecycle/reload/host-flow acceptance remains unfinished.
- Loader chat `Check running sessions safely`: same retained Level/current-Application publication contract plus loader integration on5590. Root supplied exact KillLevel16 and LevelConfig field-borrow signatures/hashes. Root stays on5554.
- Main menu chat `Inspect app launch and menus`: separate character-selection/start/save contract; not merged or relabelled as accepted by this milestone.

No Git commit, push or external release was performed for this milestone. The overall reconstruction goal remains active.
