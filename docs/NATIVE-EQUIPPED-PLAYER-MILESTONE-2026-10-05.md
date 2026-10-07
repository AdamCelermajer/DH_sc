# Equipped player: native inventory, properties and live rendering

The accepted checkpoint is `port/android-native/build/checkpoints/dh2-native-equipped-player-04fb89d1.apk`, SHA256 `04fb89d1ac985050655240643ba46d6693419605aa84b2d64fd29ffcd247fb22`, 542,057,334 bytes. It was installed and checked on the visible `Medium_Phone_API_37.0` emulator, serial `emulator-5554`. This supersedes the earlier source-player UI checkpoint for this chat's equipment work.

## What changed

The live player now owns one source-derived inventory and property state through `PlayerEquipmentRenderOwnerV1`. Starting equipment is loaded, localized and equipped through the recovered original initialization and item/effect operations. The same property state and weapon category facts feed the player's native combat calculation.

The retained visual skin owner supplies actual armor and weapon geometry, cached material textures and animated vertex positions to Android rendering. The equipped sword follows its real scene anchor. Equipping, removing and swapping gear rebuilds the affected GPU graph; each frame follows the same animated scene. Inventory, item quantities, selected equipment set, property backing and RNG state survive GL context recreation.

Live equipment operations currently use debug-only shell commands. They are connected to the native player owner and are not a finished user-facing character menu. The demonstration player is the current development session's starter Knight; all three starting classes are covered by native owner checks, not by a new Android character selector in this APK.

## Verification

- Central native host run: **155 suites PASS**, zero address, undefined-behavior or leak sanitizer findings. Frozen original/O2 equipment comparisons include 2,000 requirement/stance query cases; retained native owner checks cover all three classes and source mutation/failure ordering.
- ARM64 and x86_64 Android native builds and Gradle assembly passed. The APK contains 18 ELF64 libraries with at least 16 KiB load alignment.
- Seven live equipment cases passed: starter items; sword removal; sword restoration; alternate set; retained state after Home/resume; restoration of original set; real touch movement while geared. Captured images were visually inspected and show the sword appearing/disappearing.
- Five live HUD cases passed: default scene; touch movement; authored enemy damage updating the same player's HUD; retained damaged state after resume; development drawer open/close. The attack scenario explicitly selects an enemy through debug controls; it does not prove full autonomous AI.
- All **6,833 original cache files** are inside the one APK. There is no separate cache installation or bundled ARM32 engine.

Evidence is recorded in `port/android-native/reports/native-equipped-player-04fb89d1-checkpoint-validation.json`. The paired build capture contains hashes of actual compiler/dependency inputs, assets and libraries. The companion `dh2-native-equipped-player-04fb89d1-source.zip` is a snapshot of captured build inputs, not a claim that it is a complete portable project backup.

Initial failed live attempts are preserved under `native-equipped-player-04fb89d1-live-v1/equipment` and `.../hud`. Their checks sampled startup before landscape surface recreation finished. The accepted `equipment-attempt2` and `hud-attempt2` require a visible textured world and colored HUD; equipment broadcasts use foreground delivery. No native owner or application change was needed to pass those corrected readiness checks.

## Ownership and remaining work

This chat owns the in-game character stats, inventory/items/equipment and skills menu, plus targeting, item/skill animation and effects. The separate **Inspect app launch and menus** chat owns the main menu and character selection.

The next user-facing milestone is a whole functional character menu on the same live player state. Original query schemas and action ordering are being recovered and composed with the retained skill/save/inventory owners. Active-skill targeting, application and effects remain incomplete; their parallel work is not claimed as live in this checkpoint.

Equipment-dependent collision bounds, full original GPU/Technique behavior, automatic enemy AI/pathing, complete levels/campaign, loot pickup/merchant flows, audio, campaign saves and physical ARM64 device verification remain unfinished. The reconstruction goal stays active.
