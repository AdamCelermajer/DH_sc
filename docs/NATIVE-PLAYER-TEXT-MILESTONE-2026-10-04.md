# Owned player data and original text layout — 2026-10-04

Current successor: [native text rendering and fresh-player source milestone](NATIVE-TEXT-RENDER-PLAYER-CREATION-MILESTONE-2026-10-04.md), main125 and actual Android library verification. This document preserves the earlier main120 batch.

This completed native source batch adds the original dynamic text-layout
coordinator, owned player save/inventory/profile data, and complete authored
HUD initialization callbacks to the main CMake libraries. The Android build
is native ARM64 and x86_64. It does not promote a new APK or claim that
inventory and skill menus are live.

The text subsystem owns fonts, images, styled records and glyphs. It recovers
the original plain/HTML reader, tag attributes, line wrapping/alignment,
inline images, baseline propagation, spacing and missing-glyph diagnostics.
Its 960 whole-layout comparisons execute the captured original instructions
in the test harness; production contains native C++ and no ARM interpreter.
Font metrics, font cloning, image resolution and preload remain required
services. Connecting these records to retained movie text and GPU display
is the next rendering task.

Owned player data includes profile section indexing, name/level/class
loading, saved skill rows and equipped-row maps, faery state, inventory
items, two equipment sets, potion ownership and ordered inventory loading.
The six integrated data audits cover 3,720 original-derived comparisons.
Item stats, powers, equip logic and notifications are controlled services
in that proof; fresh-character initial loot and skill grants still need
their genuine producers before these owners can drive playable menus.

The HUD initialization includes all five recovered callbacks for equipped
skills, skill details, active faery, option parameters and platform music
support. Real owned saved rows/skill tables/settings feed its query adapter;
localization formatting, skill-info script effects and platform/world
providers remain explicit test fixtures. The new base-movie adapter executes
all four authored HUD styles and the original manager updates with 30 exact
cache-path checks. The frozen Droid adapter retains its two actual missing
cache failures. The pure callbacks pass 962 original-derived comparisons
and 8,907 required-provider failure-prefix checks.

Validation: [120 integrated native host suites](../port/level-world/reports/native-owned-player-text-hud-initialization-main-linked-host-audit-v3.json)
pass with zero ASan/UBSan/leak findings. The
[actual Android library inspection](../port/android-native/reports/native-owned-player-text-hud-initialization-library-build-v1.json)
binds the current source hashes and freeze manifests to both ABI builds,
actual defined exports, compiler records and 16 KiB ELF load alignment.
No additional emulator run or APK packaging is claimed by these receipts.
Host clients now share the engine inside the UI library and use a separate
header contract; this fixes a duplicate GameSWF-global sanitizer failure
without suppressing the check. The earlier 117-suite/text-player receipts
are preserved as historical results.

The current emulator checkpoint remains
`dh2-native-player-status-d1cbb521.apk`, SHA-256
`d1cbb5215e4595034c70451110626452bfef1bd3d1c7e608381485c2cf6c625c`.
It shows the player in the Crypt with connected HP/MP/XP status bars. It
does not contain the newer whole-manager/settings, owned-player or original
text-layout source batches.

The authored HUD investigation found a device-variant composition mismatch:
the staged Droid movie's styles 0/1 contain a skill-button list, while styles
2/3 contain static buttons; the recovered native manager selects list paths
for styles greater than 1. The base `dqhud.swf` has the matching placements.
`MenuManager::Init` selects the Droid variant at width 854; ordinary widths,
including 480 and 1080, use the base variant for ordinary language settings.
Width 800 and some languages have other source-selected variants. The
canonical APK is version 1.0.2, and its original ELF and the staged cache
movie match the supplied inputs. The option getters do not invert HUDStyle.
The new base-movie connection is being implemented separately from the
frozen Droid adapter. This supersedes the earlier assumption that styles
2/3 only need a missing list initialization; no list is fabricated.

The next visible milestone connects genuine fresh-player creation,
authored HUD initialization, original input/frame scheduling and retained
text display into the player scene. Checkpoint packaging and emulator
verification follow that complete connected feature. The supplied cache
ZIP is available locally; the current checkpoint includes 770 assets,
not all 6,833 files in the full cache.
