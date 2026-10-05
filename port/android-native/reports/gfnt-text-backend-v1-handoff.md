# Genuine GFNT backend

New `gfnt_text_backend_v1.hpp/.cpp` binds real original FontResolve/resource services, owning cache-by-name/bold/italic with actual resource-name alias reuse, immutable GFNT backing, source constructor height, raster, cached wrapper geometry and advance. No alternative asset or bitmap identity is invented. `original_ui_gfnt_backend_v1.inc` belongs inside shared OriginalUiSession::Impl; include `gfnt_text_backend_v1.hpp` and `hud_freetype_font_v2.hpp`, initialize platform backends via `initialize_gfnt_backend_v1()`, use `source_font_read_gfnt_v1` as its font reader. Destroy this backend before final Impl reset to break retained resource lifetime cycles.

## Exact active source configuration

MenuManagerC1 431c38 constructs RenderFX InitializationParameters atSP+4. Its stores431d70/78 set FreeType512×512 at offsets+c/+10. Stores431d50/54 leave bitmap width/height offsets+14/+18 at0. CreateContext7a9768/6c reads those separate bitmap dimensions; bitmap_glyph_provider ctor7a96b0..96d8 keeps cache NULL when either<=0. The successor retains genuine constructor bitmap dimensions0/0. This is not missing configuration and must not be changed to512 from the FreeType fields.

Actual SCT_Font_3.fnt resolves from Symbol. It is parsed through original GFNT owner and actual8,451 glyph slots. In the live source zero-cache configuration, cached wrapper7c5a98→7c5bf8 returns NULL after the raw glyph raster and leaves output glyph geometry/advance unpublished. The font then reaches its real FT/embedded continuation. Original FT get_face_entity7d113c invokes FT_New_Memory_Face7d1344, checks null7d134c..50, cleans up/logs7d14e0..1518 and returns no face. The safe reader validates actual GFNT and uses actual FT2.3.7 format rejection as a delivered no-face; resolver/resource/malformed GFNT failures remain errors. It does not use an unrelated font or fabricate an embedded glyph.

If a later original RenderFX context genuinely supplies positive bitmap dimensions, the backend requires `publish` to deliver a real source bitmap-cache/image owner registered with TextRenderOwnerV2. Missing publication fails explicitly. That enabled cache/atlas/RGBA core integration remains unbound; the current zero-cache MenuManager branch is complete. No claim of all fonts, positive live GFNT atlas rendering, or original streaming-font support is made.

## Proof and reproduction

- `gfnt-text-backend-v1-host.json`: ASAN/UBSAN, all8,451 actual slots, source zero-cache miss, same-resource alias lifetime, enabled-cache missing provider rejection. Resolver is explicitly a host fixture.
- `gfnt-ft-no-face-v1-host.json`: actual valid SCT GFNT, actual retained FreeType2.3.7 rejects face.
- `gfnt-text-geometry-v1-original.json`:242 positive original raster fixtures replay original7c5af0..7c5be0 wrapper float instructions versus optimized ARM64,0 mismatches.
- Existing frozen GFNT raster proof remains8,532 original/native comparisons.
- `swf-grid-snap-v1-original.json`:24,119 original/native cases,0 mismatches, including negative asymmetry, NaN/Inf and wrapping arithmetic. Kernel is `swf_grid_snap_v1.cpp`.

Scripts `.local-inputs/run_gfnt_text_backend_v1.py`, `run_gfnt_ft_miss_v1.py`, `run_gfnt_geometry_v1_original.py`, and `run_grid_snap_v1_original.py` reproduce checks. Durable source evidence is `port/engine-ui/reference/gfnt-text-backend-v1/source.asm`. No shared renderer/CMake or APK changes were made in this task.

## Projection audit

GameSWFUtils GetInvPixelScaleX416538 and Y416578 use actual authored movie extent/20 divided by their respective root viewport width/height, not root scalar pixel_scale+34. Current combat bridge formula agrees. ScanForAnims4151d8 does not reset clip position. Actual cached local origins normaldamage(-224.45,-125.55), crit(-222.45,-106.55), remaining SCT styles(-224.45,-105.55) are intentionally preserved; Draw414350..4143a4 adds queueXY to truncated original translations and restores414414..420. Live positive glyph quads near top edge are not evidence for recentering or clamping.
