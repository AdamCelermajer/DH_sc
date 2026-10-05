# Native gameplay runtime checkpoint, 2026-10-05

Checkpoint: `port/android-native/build/checkpoints/dh2-native-gameplay-runtime-3db7cac4.apk`.
SHA-256: `3db7cac414584e97d6968a778a5374a21e1203046003d61057c4731bf2cebcf2`.
Installed on visible emulator-5554; all four live checks passed on the same APK and process 3306.

## Integrated systems

- Source animation events now reach the retained player AIS and its actual Lua `OnAnimEvent` method with live animator lag. Left/right steps use the registered World actor, current scene foot bones, actual cached CharacterFX row and current PFFloor. Scene lookup preserves the original root-container forest and child order. Crypt's authored footprint/empty-floor FX IDs are -1 and follow the original early return; positive visual effects still require their actual backend.
- The enemy status HUD is connected to the same registered World/character/AI target authority. It runs the original manager enemy body and original droid SWF display-list widgets for name, level, visibility and HP timeline. It does not manufacture a target from the development nearest-enemy attack index. Positive targeted gameplay remains unverified until the genuine attack target producer is integrated.
- Recurring gameplay effects use the existing V6 session's timer store. Timer scanning continues independently of a previous diagnostic readiness failure without resetting errors or replacing owner state. The accepted live scene regenerates mana and permits casting again after the actual cooldown expires.
- Faery press/release uses one physical lifecycle: Begin on primary press, End on release/cancel/focus loss/pause. The source cast controller and state own execution. Faeries remain genuinely locked in the current Save, so unlocked live casting and companion rendering are not verified.
- Stats, equipment, skills and faeries panels operate on the same native Save/Gear/player. The visible gameplay HUD includes three skill slots, faery status and potion stock beside the player. The panel is a native functional UI; the original animated character-menu lifecycle remains incomplete.

## Reload crash resolved

Equipment changes rebuild the level's visual/floor storage while retaining player Script/Skill state. The new footstep consumer exposed a stale raw floor borrow after this reload. The retained initialization branch now rebinds the floor borrow to the replacement level before animation can run. The movement test runs after actual equipment unequip/equip transactions and verifies both left/right events, source Move/Idle transitions, and survival of the same process.

## Live verification

Receipts and screenshots are in `port/android-native/reports/native-gameplay-runtime-fixed/`:

- `gameplay-character-panel-smoke.json`: stats/equipment/skills/faeries actions; armor 4 -> 2 -> 4; skill slot reassignment/restoration; zero-point training disabled; original locked faeries; full-health potion refusal at stock 5; actual enemy development-command damage 42,265 -> 32,218 and potion restoration to 42,265 at stock 4.
- `player-animation-event-smoke.json`: two physical joystick swipes after reload; state 3 -> 4 -> 3; eight actual left/right footstep deliveries with Lua status 0.
- `skill-world-activation-smoke.json`: eleven original Crypt NPC state initializations, 69 trophy rows, no-target Headsplitter clip 1234 and state 3 -> 6 -> 3. Zero targets means enemy skill damage is unproven.
- `gameplay-recurring-effects-smoke.json`: mana 4,672 -> 4,991 -> 5,732 -> 6,473 -> 6,976; repeated no-target cast accepted; 120-second soak in the same process with no fatal signal, native frame failure or GL error.

Both ARM64 and x86_64 builds passed. The APK has 18 ELF64 libraries with at least 16 KiB segment alignment and 771 assets, including the uncompressed canonical cache ZIP containing 6,833 files. It needs no separate cache download or ARM32 reference runner.

Provenance: `port/android-native/reports/native-gameplay-runtime-3db7cac4-checkpoint-validation.json`.
Source/compiler snapshot: `port/android-native/build/checkpoints/dh2-native-gameplay-runtime-3db7cac4-source.zip`, with 1,304 recorded inputs and the four live receipts. Included host evidence covers the source animation event path, enemy SWF/manager fixtures and same-owner timer expiry; its fixture/frozen-dependency limits are retained in the reports.

## Remaining substantial work

Three agent tasks continue: genuine player attack initialization/target search; full NPC physical/collision/room/AI lifecycle; and positive damage-over-time/injury/status reactions. These candidates are outside this checkpoint until integrated and tested. Complete enemy damage/health targeting, skill FX/audio, unlocked faery casting/follow/rendering, original animated menus, full campaign worlds and persistence, and physical ARM64 testing remain incomplete. The reconstruction goal remains active.
