# Native text rendering and fresh-player source milestone

Current central engine source passes **125 host suites**, with **zero ASan/UBSan/LSan findings**, and builds real Android **ARM64 and x86_64** UI/world/data libraries with **16 KiB LOAD alignment**. This is a source/library milestone. No new APK was promoted.

## Engine

- Complete original `display_glyph_records`: two cache producers and filter paths, images, underlines, missing-glyph boxes, embedded shapes, matrix scaling/rotation, UV clipping, Font3 and color overrides. **768 original whole-routine executions** and **six ownership/domain guards** pass.
- Owned matching FT2.3.7 face supplies layout metrics and raster identity. Registered textures emit the `SwfDraw` commands accepted by Android `SwfGpu`. **42 actual-font raster/owner cases**, **four plain/HTML layout-to-draw compositions**, **25 draw commands** and **nine source/ownership guards** pass. Host upload/draw sinks are fixtures; GPU hardware execution is not inferred.
- Frozen layout source is unchanged. The new layout clone projection resets Font3 and excludes inherited embedded glyph/kerning tables.
- Separate input/frame session is frozen with 121 retained-session checks and 10,800 original input/frame/drag comparisons. Destruction/recreation found an upstream use-after-free; a versioned lifetime overlay fixes it. This session is not central-enabled: startup observers, status ownership and Android input/viewport must migrate together.

Still required: retained original edit-text setter/display/preload integration, actual bitmap-font name/cache/filter producers, render-list caching and complete live HUD callbacks. Captured methods are not automatically implemented methods. The new production font/texture pipeline does not prove every existing APK text field uses it.

## Game

Real data/world libraries now contain immutable 339-row loot backing, stable owned starting inventory, original initial-equipment and skill-slot callers, and skill increment/grant ordering. Actual three-class starter tables are used; saved row indices remain distinct from skill dictionary IDs.

Fresh-player source and its independent original/optimized-ARM64 proof are frozen in `port/game-data/reference/player-creation-v2`. This closes owned starting loot and grant callers, not complete campaign character creation. The separate auto-equip caller graph is also frozen; authoritative same-inventory mutation and genuine split/merge/item/property/Skin effects are the next connection. Item Req/Stats, active skill-script lifecycle and campaign profile writing remain required.

## Evidence

- Main: `port/level-world/reports/native-text-render-fresh-player-main-linked-host-audit-v4.json`; SHA256 `d16e6674d9157d8ae1388c66e4e1489d641c0ed246418a8ed5b3b2de10d97a49`.
- Android: `port/android-native/reports/native-text-render-fresh-player-library-build-v1.json`; SHA256 `f406a5b42afdb69ef46f5c617c94dc6499f55dacdf65f65a40a8def5466965fb`.
- Text freeze: `port/engine-ui/reference/text-display-v2/freeze-manifest.json`; SHA256 `55d71a3340508c0edf4febe65a932dea2745a6b63c24b850d5d344fc69979fde`.
- Fresh-player freeze: `port/game-data/reference/player-creation-v2/freeze-manifest.json`; SHA256 `f85e80b19701722fffbd237810b863ad1658853ce23a2ef07242cae398b24b56`.
- Session freeze: `port/engine-ui/reference/swf-input-session-v1/freeze-manifest.json`; SHA256 `1c26539e5c9c5d66fce6ef634b4bfc00f1be6dc7a0df1389012b9d612807d52f`.

Earlier main110/117/120 receipts remain historical. The first three new main attempts did not produce acceptance receipts: executable placement, suite/library classification and an inherited runtime-report requirement were corrected for the new targets. Only final v4 is acceptance evidence. Display gold v1/v2 limitations are recorded in the frozen notes; v3 is current acceptance evidence.

## APK and emulator

Latest checkpoint remains `port/android-native/build/checkpoints/dh2-native-player-status-d1cbb521.apk`; SHA256 `d1cbb5215e4595034c70451110626452bfef1bd3d1c7e608381485c2cf6c625c`. No new APK was created or installed in this batch. Live read of visible `emulator-5554` showed the existing app resumed in landscape: Crypt, player and status HUD. Its development panel was open and the player dead when captured. This is prior gameplay state, not a preview of the new renderer.

The checkpoint contains 770 selected immutable assets. The supplied `C:\Users\adamc\Downloads\Dungeon-Hunter-2-HD-v1-0-2-cache.zip` is already used for actual tables/assets and contains 6,833 files; the full cache has not been bundled. The one-APK objective remains active. Physical ARM64 phone/tablet testing remains unperformed.

Next consequential checkpoint must connect retained original text, safe source input/viewport, real HUD callbacks and authoritative inventory into the cohesive player scene. A library-only package displaying the same prior scene must not be labeled new visual progress.
