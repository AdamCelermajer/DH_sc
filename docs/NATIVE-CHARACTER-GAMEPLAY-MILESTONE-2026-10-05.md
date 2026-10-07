# Native character gameplay checkpoint, 2026-10-05

Checkpoint: `port/android-native/build/checkpoints/dh2-native-character-gameplay-8f580946.apk`.
SHA-256: `8f580946d664365b9b2b6b6713c0700cc22510d1abfe85224e493596999efeb1`.
Installed on visible emulator-5554; independent menu and loader chats' emulators were untouched.

## Completed in this milestone

- Retained live SkillV6 and Save sharing the world's actual player Gear, properties, life and script authority. Original initial-skill coordinator assigns/learns Headsplitter using the real first skill point. No invented skill/faery unlocks.
- World gameplay controls now display three source skill icons/levels, a faery slot, and a real potion stack. Original authored atlas pixels are decoded/cropped; locked and empty states are explicit.
- Usable modal character panel with stats, inventory/equipment, skills and faery pages. Native queries/action coordinators supply the original localized descriptions, stat values, prices/requirements, power presentation and Save state. Stat allocation and skill training controls honor actual points. Equipment and skill-slot mutations use the same gameplay owners.
- Potion action follows source controller/dead/fullness/stock guards, actual inventory removal, source use counter and frozen HP/MP regeneration.
- Original source NPC state constructor backing and stable World actor/handle registry are connected, including the actual shared Scene. Genuine constructor flags remain zero until the missing NPC initialization pipeline produces live interactive states.
- State-aware animation events prevent idle clip `do_skill` events from being interpreted as player skill use. Headsplitter activation now reaches source state 6/flags `6341`, sequence root 347, clip 1234 before the missing trophy-manager continuation.

The modal panel is newly written Android view transport over recovered native gameplay queries/actions. It does **not** establish the original animated SWF character-menu lifecycle/navigation, which remains a separate reconstruction. Item table icon strings are preserved, including genuinely blank starter item icons; the original resolver provides no proven fallback. Equipment labels resolve original EquipmentSlots constants.

## Verification

Both ARM64 and x86_64 Gradle native/Java builds passed. Latest installed APK SHA exactly matches the checkpoint. All 18 ELF64 libraries have at least 16 KiB PT_LOAD alignment. The APK's 771 asset entries match source inputs, including the uncompressed canonical cache ZIP with all 6,833 original files; no external cache is required.

Visible emulator smoke passed on the exact final APK:

- Armor 4 -> 2 -> 4 through actual garb unequip/equip widgets.
- Headsplitter level 1 initialized; skill assigned to slot 2 and restored to slot 1.
- Training disabled with zero unspent points; original faery names and locked Save states visible.
- Full HP/MP potion request refused with count 5 unchanged.
- Existing original enemy attack command caused HP 42,265 -> 32,218. Touching potion restored HP 42,265 and reduced count to 4.

`reports/gameplay-menu-checkpoint-final/gameplay-character-panel-smoke.json` and its log/XML/screenshots preserve these observations. The additional skill activation attempt reached the genuine animation/state prefix and returned `Required skill AI provider 8`; this is not accepted full skill combat.

Host connected-menu regression passed at O1/O2 under ASAN/UBSAN: three classes, 207 stat members, 341 item members, 27 skill members, three whole training transactions, 27 actions, 15 missing-provider guards and all 219 original scripts. NPC constructor/CF differential passed 683 cases; its supported initialization/transition host composition passed 23 checks per build. World SkillV6 application prefix passed 443 host checks and 6,144 original application comparisons; missing side-effect services remain explicit.

Build provenance receipt: `reports/native-character-gameplay-8f580946-checkpoint-validation.json`. Exact recorded compiler inputs/Java sources are preserved in `build/checkpoints/dh2-native-character-gameplay-8f580946-source.zip` (1,271 source inputs). Source freeze manifests from earlier milestones remain historical; current Gear's narrow potion/raw-swap extensions are separately documented successor candidates.

## Remaining and next work

Three parallel large tasks are assigned: source TrophyManager/achievement ownership (unconditional skill Begin and Hit/potion continuations), genuine NPC initialization into source AI/FSM states with body/controller providers, and original faery spell/HUD/active-selection ownership.

Full targeting/pursuit/monster AI, complete skill damage/FX/status/audio/kill tails, following faery rendering/campaign unlocks, original animated character-menu visuals, item icon absence/fallback investigation, drop/transmute, complete campaign/save-file IO, and physical modern ARM64 phone/tablet validation remain unfinished. Potion audio and use-count trophy continuation after 99 remain missing. The game is not yet fully playable; the overall goal stays active.
