# Original font/text renderer inventory

This is a discovery handoff, not a complete menu/text renderer. Original ELF SHA256 `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`; canonical cache ZIP SHA256 `3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679`. Actual captures are in this directory, `glyph-backend/` and `font-resolution/`. UI hierarchy/layout/anchors/hit-testing are independently owned by the UI worker.

## Authored resources

`.local-inputs/font-text-discovery/font-assets.json` binds all seven cached TTFs and the single cached GFNT. They are uncadis.ttf, nanumgothic.ttf, wqy-zenhei.ttf, japanese.ttf, arkham_reg.ttf, arkham_bold.ttf, Fontin SmallCaps.ttf and menus/sct_font_3.fnt. No replacement font was downloaded or selected. The last is 120568 bytes, SHA256 `45a426d731ec69996de61a2c47290afa0a59f6a64079440e298339cdfa15656d`.

The thirty actual SWFs contain 52 DefineFont2/3 declarations. `.local-inputs/font-text-discovery/swf-fonts.json` preserves exact name bytes, SWF hashes, offsets/IDs/flags/counts. Names are Arial, Fontin SmallCaps, Justus and Verdana. The cache lacks literal Justus.ttf and Verdana.ttf. That is a genuine resource-resolution boundary; their absence does not authorize substitution. Embedded glyphs/provider fallback must be followed as the original font::get_glyph implements. Adobe's primary SWFFontDescription documentation confirms the separate DefineFont2/3 character-to-glyph mapping domain: https://experienceleague.adobe.com/en/tools/aem-api-documentation/cloud-service/javadoc/com/adobe/fontengine/font/SWFFontDescription.html . The original source and actual bytes, rather than an assumed modern Flash renderer, drive this discovery.

## Source selection and dispatch

`get_fontfile` `0x42b38c` performs genuine Debug.load/GetSwitch("isTracingMenuFS") first. Its static branches compare `_sans`, `_serif`, `_typewriter`, `Symbol`, then `Arial`. `_sans` and `_serif` choose `#/system/fonts/DroidSans.ttf`; `_typewriter` chooses source system font strings with bold/italic combinations. `Symbol` creates `%sdata/menus/SCT_Font_3.fnt` then resolves it through the actual file manager. Arial queries original language (`0x46d514`): 4→japanese.ttf,5→nanumgothic.ttf,6→wqy-zenhei.ttf; other language values use `%sdata/%s.ttf`. Generic other names also take `%sdata/%s.ttf`, actual file-manager path transformation then file-open existence check. These are static instruction facts; Application base path, language and manager rewrite need genuine providers. Exact PC-relative literals are saved in `font-resolution-literals.json`.

`bitmap_glyph_provider::get_font_entity` `0x7c6048` caches by case-insensitive name with style suffixes, invokes that game resolver, and checks .fnt/.ttf extension branches. `default_bitmap_glyph_provider::get_font_entity_impl` `0x7a9634` constructs `default_bitmap_font_entity` `0x7c68c4`. The constructor opens actual bytes and reads big-endian GFNT header/offsets; it may retain the file or preload the remaining payload depending on the provider's source flag.

`font::get_glyph` `0x7d01bc` initializes output and tries the configured bitmap glyph provider, then the FreeType-backed glyph provider (`0x7d1614`/get_face_entity `0x7d113c`), then the original embedded character-code map/advance data. Font provider availability and embedded code indices must not be collapsed into one invented system-font path. Captured get_glyph preserves this ordering. Static `font::read` `0x7cfa30` and font data readers remain required for actual SWF embedded text.

`RenderFX::SetText(character,...)` `0x7a92e0` requires nonnull target and vtable is(0x20), then delivers `edit_text_character::set_text_value` `0x790ab0`. Formatting is `0x78efb8`; static glyph rendering is `display_glyph_records` `0x78faa0`, dynamic display `0x7928cc`. Ultimately bitmap glyph drawing reaches `render_handler_glitch::draw_bitmap` `0x7d8df4`. GPU uniforms/blend/premultiply/filter selection belongs to the source render-handler connection; it is not inferred from raw font pixels. Parent is recovering actual GameSWF shaders separately.

## Executed original GFNT slice

GFNT header has 8451 codepoint slots starting at32, fixed16×19 cell size,218 nonempty records; offset table starts at40 and payload33848. Missing/empty glyphs return false without changing caller output. Nonempty record begins four metric bytes then run encoding: token low7+1 pixels, high bit repeats one following RGBA word; otherwise each pixel has its own following RGBA word. Original instructions store exact native pixel bytes, preserving channels without texture-policy conversion.

`.local-inputs/font-text-discovery/probe.py` executes original `get_char_image` `0x7c4698` for121 selected present/missing/boundary cases with genuine cached payload bytes and caller-preallocated storage. Output dimensions/pitch and every RGBA byte equal an independent decoding of those records. `.local-inputs/font-text-discovery/glyph-probe.json` binds the original/font/script/parser hashes and per-glyph output hashes. Metrics pointer is null in this proof; float scale/advance conversion, streaming file reads, cache allocation/upload and GPU output are explicitly unproved. This is a viable next small native resource slice, particularly source Symbol/SCT text; it is not a generic replacement for menu fonts.

## Minimal next integration

1. Recover/prove a bounded immutable GFNT reader and source get_char_image metrics/RLE kernel with original/O2 parity; retain source codepoint-miss behavior and real owned bytes.
2. Connect its resulting bitmap/metrics through the genuine render-handler bitmap request/shader pipeline and a real authored Symbol text consumer. Do not render arbitrary menu text using this font.
3. For initial menus/HUD, decode the actual selected SWF Font2/3/style/code/advance records and follow get_glyph's configured provider order. Fontin SmallCaps has actual cached TTF backing; other authored names require original manager/embedded behavior. Character placement/layout and event-driven SetText remain the separate UI-worker slice.

Full gameswf parsing/ActionScript, FreeType version/source identity, TTF raster parity, kerning/layout, all icon bitmap resources and full authored menu rendering remain open. This handoff made no renderer/CMake/APK change and no device claim.
