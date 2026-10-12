# Original glyph display and owned text rendering, version 2

This is the complete `display_glyph_records` coordinator at original ARM address `0x78faa0`, followed by an owning native font/texture/draw connection. It is not a replacement for the complete retained GameSWF edit-text class yet.

The original ELF SHA256 is `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`. Captures preserve the original display, font metrics, constructor/copy, setter and reset-format routines. The latter edit-field methods and cache/filter producers remain separately required; capturing their instructions does not establish their native implementation.

## Completed production pipeline

`text_display_v2` consumes the immutable layout-v1 projection and rereads live record/glyph counts and glyph advances around callbacks. It implements skipped unresolved fonts, inline images, missing-glyph boxes, underlines, embedded-shape dispatch, bitmap and FreeType atlas selection, both filter argument paths, normalized UV clipping, provider scale, Font3 factors, transformed positions, finite translation guards and override alpha/RGB. Font, face and bitmap identities remain owned native objects. It copies only style metadata per record; it does not duplicate glyph vectors on every frame.

Source fast positioning requires both diagonal matrix terms to equal one and both off-diagonal terms to equal zero. The general branch preserves its linear matrix terms through concatenated scale. Filter expansion is `(float(argument) + float(argument)) * provider_scale`, in that operation order. It is not division by provider scale. Bitmap-only caching and a non-FreeType atlas without its bitmap cache dereference absent source owners; the native coordinator reports required-owner errors for those source-invalid combinations.

`hud_freetype_font_v2` is a versioned copy of the frozen matching FT2.3.7 raster owner with a public metrics method. The pixel, padding, bearing, bounds and advance path is unchanged; the same owned FT_Face now supplies `units_per_EM` and signed ascender-minus-descender. Both source versions share the existing `HudBitmapInfo32` declaration. Original v1 source bytes remain untouched.

`TextRenderOwnerV2` retains the genuine caller font resolver and texture platform. Bitmap-font metrics are resolved first: a delivered face gives 1024 units and `height * 20`. Otherwise the real FT face gives its units and ascender-minus-descender; a delivered FT miss gives original defaults 1 and 0. Missing services or invalid font bytes fail explicitly. The layout-v1 clone projection copies name/style/descent/leading, resets Font3, and does not inherit embedded glyph or kerning tables. Private source encoding/ascent/table-hint fields are outside this projection.

Glyphs retain one real face identity and registered uploaded texture. Repeated requests while a glyph is retained reuse that image. A face's glyph-cache entry has a weak image reference, while exported images pin their face and platform, avoiding face/image cycles. Expired images can be uploaded again from the retained CPU raster. Arbitrary void pointers cannot be rendered as textures. The owner forwards the original source renderer's bitmap, inline-image and floating line commands as `SwfDraw` values accepted by the Android `SwfGpu` adapter. Already transformed text colors have identity draw cxform.

Root scale, real original font resolution/kerning, image lookup, preload, atlas regions/filter effects, world color transform and embedded-shape execution remain caller services. In particular, actual bitmap-font name/cache ownership is not fabricated from a generic font fallback.

## Evidence and boundaries

Current corpus: `whole-gold-v3.bin`, 768 whole original executions with ordered render/cache services. Matrix, image, underline, shape/missing glyph, both atlas producers, both filter paths, source Font3 and override color paths execute original ARM instructions. String/layout/font/GPU services are explicit fixtures at those boundaries. Null-font records contain inline-image glyphs; source-invalid null cache/embedded-font dereferences are not hidden behind service fixtures.

Historical `whole-gold.bin` contains duplicate virtual-service observations caused by two Unicorn code hooks and is invalid as an acceptance corpus. `whole-gold-v2.bin` corrected that hook issue, but coupled the filter/cache choices and included invalid original pointer-domain combinations. Both historical files are preserved for diagnosis; only v3 is current acceptance evidence.

The native font-owner test loads actual `Fontin SmallCaps.ttf` and `wqy-zenhei.ttf`, compares metrics against the matching FT2.3.7 face and 42 raster requests against the frozen producer, verifies retained image/face reuse, and executes four whole plain/HTML layout-to-draw compositions. Texture upload/draw are controlled host sinks, not GPU hardware or emulator execution. Source bitmap/embedded responses in provider-order guards are explicitly fixtures.

Central integration adds the three production CPP files to `dh2_engine_ui` and two executable audits linked to that actual DSO. The main engine receipt also includes the separate frozen fresh-player loot/grant batch. Android ARM64/x86_64 builds establish actual DSO exports and 16 KiB LOAD alignment; they do not establish an APK or visual checkpoint.

Still required for full retained HUD text: versioned original edit-text formatting/setter/display/preload integration, original bitmap atlas/filter providers, and the full live HUD manager's native callbacks. The current visible APK remains the previous player/status checkpoint until a cohesive complete app feature is connected and tested.
