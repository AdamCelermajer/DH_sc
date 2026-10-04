# Native GameSWF texture and color connection

The new `swf_texture.hpp/.cpp` is frozen. It supplies complete bounded source string, packed-wrap, renderpass-conversion, enum-table and color projections. It does not own a texture manager, GL object, archive or SWF display list. The real renderer must retain resource/bitmap identity and execute required bind/upload/draw/mask operations.

The captured original ELF SHA256 is `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`. `original-functions.json` and `original-functions.asm` contain 25 complete relevant routines. The original instruction differential has 2,879 comparisons with no mismatches; the isolated actual host DSO replay has the same 2,879 plus 11 atomic malformed-contract checks and no ASan/UBSan findings. A separate original connection probe executes 32 alpha constructors / 256 pixels and authentic pixel-format conversion, and binds six exact canonical cache files.

## Filename and physical resource contract

`SwfTextureLoader416c14` generic requests are `data/` followed by the complete export name, as independently recovered by the UI worker. `CTextureManager.getTexture5ed210` passes that name to the real filesystem. `FileSystemBase.createAndOpenFile34e688` invokes virtual `+b8` filename rewriting before lower-level file lookup. `ApplyFilenameHacks34e96c` searches for the supplied current working directory anywhere in the input, preserves everything through the following byte, and rewrites a suffix containing the case-sensitive substring `.tga` to the literal `ata/3d/textures/` plus its basename. Only the suffix is then ASCII-lowercased. The seemingly missing `d` is an actual original literal: with the initial empty working directory, the preserved first input character supplies it.

`CFileSystem.WorkingDirectory99b138` is an initially empty BSS array. The actual getter56c214 executes in the oracle. A nonempty working directory remains an explicit owner snapshot, not an assumed permanent empty value. The port rejects empty/malformed suffix requests and embedded NULs atomically where the original can read outside a C string.

Thus `data/menus/MenusGraphics_droid.tga` resolves to `data/3d/textures/menusgraphics_droid.tga`. The exact canonical cache entry is `com.gameloft.android.GAND.GloftD2SS/files/data/3d/textures/MenusGraphics_droid.tga`. `connection-probe.json` binds its original bytes and SHA along with map_top, map_bottom, shared MenusGraphics, iphone_table_dh2 and splash_final_droid. The canonical cache SHA is `3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679`.

`swf_texture_archive_key` executes the original CZipReader.findFile578520 query preparation: optional ASCII case folding and optional last-slash stripping, preserving the unusual leading-only slash case. The oracle observes the prepared query at the binary-search service boundary. Original archive constructor577fa4 stores caller ignore-case / ignore-path bytes at `+c/+d`; the actual platform archive registration flags have not been established. The modern APK resource provider's unique casefold index is therefore an explicit backing service, not a claim that the original registered the supplied ZIP with those flags. Preserve the physical member's case and bytes; do not substitute a basename-only lookup.

## Wrap, filter and diffuse uniform

Original `ITexture.setWrap7d3bb4` updates packed texture bits X18..20, Y21..23, and its unusual Z24..26 condition, plus dirty flags10/20/40. `SwfTextureState8` explicitly projects original packed word+38, dirty halfword+40 and preserved halfword+42; it is not an ARM32 pointer overlay. Valid requests0..7 leave Z unchanged. The full32-bit requested value is compared before masking, which is covered by the corpus.

Original CTexture.updateParameters5afd40 maps valid wrap codes0..4 to `[GL_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_REPEAT]`. Valid filter0..5 maps to `[NEAREST, LINEAR, NEAREST_MIPMAP_NEAREST, LINEAR_MIPMAP_NEAREST, NEAREST_MIPMAP_LINEAR, LINEAR_MIPMAP_LINEAR]`. The oracle executes its genuine 2D GL parameter branches with driver extra capabilities absent; full anisotropy/mipmap/LOD ownership is outside this helper. Invalid table indices fail atomically. No extension fallback is inserted.

Bitmap fill_style.apply7d6b54 realizes its borrowed bitmap, binds the texture, requests wrap0 for style2 / wrap2 for style3 and uses true provider width/height for normalized UVs. Glyph draw_bitmap7d8df4 requests wrap1. These source requested values must pass through the original logical enum table before GL upload.

BufferedRenderer.flush7d6894 always binds its current texture first. It then rereads the current texture because that binding can reenter and replace it. Only a nonnull texture and diffuse parameter ID other thanffff produce a vector request. `(packed_flags >> 4) & 63 == 2` produces `(1,1,1,0)`; all other formats produce zero. The 260-case probe includes texture-changing callbacks and checks exact bind -> diffuse -> material-bind ordering. `swf_texture_diffuse` is the projection after that genuine reread; it does not replace the bind service.

## Authored render state and blending

The authored GameSWF pass state must first pass through original renderpass constructor5d7a10. Raw high bits are source fields, not driver equation bits. `swf_render_state` reproduces the complete 76-byte-source to 32-byte runtime projection, including every directly read word. 500 random source states and all four actual cached passes compare exactly.

The original runtime blend5af3f8 was executed on all four converted cached passes. All enable GL_BLEND, use FUNC_ADD and runtime flagsword1=`00010007` (blend enabled, cull/depth disabled):

| Technique | Fragment shader | Source factor | Destination factor |
|---|---|---|---|
| default | GameSWFFS | SRC_ALPHA | ONE_MINUS_SRC_ALPHA |
| multiply | GameSWFFS_blend | DST_COLOR | ONE_MINUS_SRC_ALPHA |
| screen | GameSWFFS_blend | ONE | ONE_MINUS_SRC_COLOR |
| overlay | GameSWFFS_blend | DST_COLOR | ONE |

`material-state-probe.json` and the differential bind the exact source resource and requests. `swf_render_blend` returns validated table values and the source blend-enable bit; it does not mutate cached GL state or replace stencil/depth/color-mask services.

## Exact Color0 producers and glyph pixel layout

Solid fill_style_color7d4624 invokes cxform.transform794f8c: separate float32 byte*multiplier then additive term, clamp to0..255, actual unsigned float-to-integer conversion8be2a0, and byte truncation. There is no round-to-nearest or +0.5. `swf_solid_color` preserves these operations, including IEEE clamp behavior.

Bitmap fill_style.set_bitmap7d4430 first executes cxform.clamp795130: multipliers0..1 and additive terms-255..255. Its Color0 bytes are truncated float32 multiplier*255. It retains the clamped cxform and sets its source additive flag when any additive term is greater than1. `swf_bitmap_color` returns all40 bytes, not just an approximation of the byte colors. The original immediate mesh route (draw_mesh_primitive7d94f8 -> fill.apply7d6b54 -> process_mask_intersection7d860c -> buffer queue) does not consume those retained additive terms/flag. Do not inject generic bitmap additive shader terms. Cached render-cache playback is a distinct ownership path. Glyph draw_bitmap copies its supplied byteRGBA directly into all four vertices.

The helper8be2a0 is `__aeabi_f2uiz/__fixunssfsi`, proved by its captured instructions. The source source-format tablePFDTable8e36c0 has40-byte rows. Format2 has RGB masks0 and alpha maskff (alpha8). Format12 has masksRff00/Gff0000/Bff000000/Aff, hence little-endian bytes[A,R,G,B]; format14 is[R,G,B,A].

Original create_bitmap_info_alpha7d551c -> bitmap_info_ogl constructor7d5404 allocates logical image format12 and writes each alpha sample as `[A,255,255,255]`. The connection probe executes all256 alpha samples and genuine original pixel_format.convert5f95ac from12 to14, producing `[255,255,255,A]` exactly. This supplies an honest modern RGBA upload conversion. GL_ALPHA with the original Format2 diffuse-white vector has equivalent logical shader texels, but does not establish the original texture-upload factory's chosen GL format. GPU shader/blend acceptance must be checked separately.

## Reproduction and integration

Build `tools/build_swf_texture_oracle.ps1`, then run direct Python `tests/swf_texture_differential.py`, `tests/swf_texture_connection_probe.py` and `tests/swf_texture_host.py` with the pinned Unicorn/ELF dependency path. The host runner builds a private shared DSO and executes actual imports, leaving central DSOs untouched.

Production scene DSO integration is `swf_texture.cpp`, using existing `-fno-fast-math -ffp-contract=off`. Host target `swf_texture_audit` compiles `tests/swf_texture.cpp` and links the actual scene DSO; its argument is `reference/swf-render-connection/swf-texture-fixtures.bin`. Its stdout includes validation=PASS, comparisons2879, atomic_guards11, mismatches0. Parent owns CMake, Android resource staging, GL sink, actual shader compilation, retained texture/bitmap lifetimes, masking and visible acceptance.
