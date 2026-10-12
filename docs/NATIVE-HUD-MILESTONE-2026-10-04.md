# Native HUD source milestone — 2026-10-04

This completed source batch integrates the whole original InfoHUDManager
coordinator, owned HUD settings, and retained typed ActionScript connections
into the main native CMake libraries. It does not promote a new APK.

Later source progress and a correction to the HUD-style composition assumption
are recorded in [the owned-player/text milestone](NATIVE-PLAYER-TEXT-MILESTONE-2026-10-04.md).

The manager includes all five initialization/update methods, 29 weak UI
caches, status/potion text, skill/spell cooldown and usability dispatch,
portrait/level/death updates, enemy visibility/name/HP, and online ally
dispatch. Genuine world, player, script and camera producers are still
required where the current prototype does not supply them. Retained original
HUD styles 0/1 execute in the host composition; dynamic styles 2/3 reject
their missing authored initialization instead of substituting static buttons.

Settings own the real cache design table, private option/tutorial state,
raw `dh2_settings.savegame` file reads, and source language scene traversal.
This is distinct from the unfinished campaign profile section loader.
Typed callbacks retain real AS objects/tags, source setter/getter timing and
graph/provider lifetimes. Required HTML text formatting remains unavailable.

Validation: [110 combined native host suites](../port/level-world/reports/native-whole-hud-settings-as-main-linked-host-audit-v2.json)
pass with zero ASan/UBSan/leak findings. The original-instruction manager
comparison batch covers 2,106 cases and 27,287 ordered services. Actual
[Android CMake libraries](../port/android-native/reports/native-whole-hud-settings-as-library-build-v1.json)
compile for ARM64 and x86_64 with verified ELF64 exports and 16 KiB load
alignment. This batch has no device-execution or physical ARM64 claim.

The visible emulator remains on `dh2-native-player-status-d1cbb521.apk`,
SHA-256 `d1cbb5215e4595034c70451110626452bfef1bd3d1c7e608381485c2cf6c625c`.
It already shows the player in the Crypt with connected HP/MP/XP status bars;
it does not contain this newer full-manager/settings source integration.

Next connected milestone: genuine owned PlayerSavegame/ItemInventory and
skill state, complete authored HUD/menu setup, and original cursor/root/sprite
frame ordering. Checkpoint packaging and visible gameplay validation follow
that complete live feature. The supplied full cache ZIP is available at
`C:\Users\adamc\Downloads\Dungeon-Hunter-2-HD-v1-0-2-cache.zip` (433,189,197 bytes).
