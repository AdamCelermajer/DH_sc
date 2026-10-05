# Retained NPC poses and enemy HUD

Checkpoint: `port/android-native/build/checkpoints/dh2-native-npc-pose-enemy-hud-b28d9b5c.apk`.
SHA256: `b28d9b5cc8fd93b029763cc235d5e7a05d545692df7689a0bd7d72aabc675df6`.

The enemy HUD now anchors to the selected monster's currently submitted mesh and camera, instead of the original authored fixed screen location. Its entire authored name/level/bar stays inside the current viewport. The original SWF movie receives valid HP frames; zero HP, dead, empty and offscreen targets hide immediately. Reacquiring the same live identity shows it again. The original frozen HUD manager remains unchanged; this presentation extension is versioned V2.

NPC Idle/Injure rendering uses each retained CPU scene, source GameObject transform and animation bank. Injury dispatch shares actual World, properties, life, controller, script session and FSM; finite animation completion passes through the original AI/external script event before returning to Idle. Android GL recreation no longer replays consumed developer actor commands.

Validation:

- Both ARM64 and x86_64 Android builds succeed. All18 packaged ELF64 libraries retain16KiB load alignment. All771 assets remain bundled, including the unmodified original cache archive; no ARM32 runtime is packaged.
- Current main-linked ASAN/UBSAN NPC host:2329 checks, all eleven actual Crypt sessions and animation-to-Idle completions. Heading, state-notification and profiling providers are explicit host fixtures.
- Source placement extraction:503 actual original ARM/current packaged ARM64 comparisons,82 authored placements, zero mismatches. The existing bounded factory/services scope is preserved.
- Actual HUD movie ASAN/UBSAN tests cover moved/repeated anchors, full bounds at viewport edges and resize, positive/low HP, death/zero/offscreen hide and reacquisition.
- Current visible emulator: eight genuine attacks reduce the selected skeleton from25600→21247→19370→14848→10882→8783→4814→390→0. The actual HUD follows these properties and changes to invisible with dead=1. Enemy mesh and player positions are distinct.
- All eleven source injuries completed live on earlier d5c5e279 build. The final build's repeated eleven-NPC probe was interrupted when the selected cultist07 had already been killed by actual attacks; its failure remains recorded and is not counted as a final-build PASS.

Receipts: `port/android-native/reports/native-npc-pose-enemy-hud-b28d9b5c-checkpoint-validation.json` and `port/android-native/reports/npc-pose-enemy-hud-v2/`. Source capture includes actual compiler/Ninja dependencies. Its packaged/studio labels point to the same repository project; a separate Studio build is not claimed.

Remaining: full original NPC physical/AI/pathing connection; full original melee and skill hit-FX/application; particles, scrolling combat text, audio, campaign/save integration and real ARM64 phone testing. Development Attack/Walk/Dead rendering remains until that NPC pipeline is complete. This milestone does not complete the overall goal.
