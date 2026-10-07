# Native GameSWF HUD source reuse

The separately owned `swf_movie` facade loads the genuine `dqshared_droid.swf`
then `dqhud_droid.swf`, executes their native ActionScript/timelines, resolves
`_root.menu_HUD_0.HUDelements.HealthBars.player.bar_hp` (character 90),
`bar_mp` (147), and their player parent (157), and emits authored geometry.
`display_clip` brackets a retained subtree using the root stage and caller's
viewport. The next visible slice is this actual health/mana subtree connected
to the modern GLES2 sink; this audit does not establish its GPU pixels.

## Primary source and license

Official fixed revision: [tu-testbed SVN r1714](https://svn.code.sf.net/p/tu-testbed/code/!svn/bc/1714/trunk/tu-testbed/).
The [maintainer's GameSWF page](https://tulrich.com/geekstuff/gameswf.html)
describes the engine as public domain and explicitly notes incomplete support.
Source files retain their credits and public-domain notices. `core-manifest.json`
binds fetched bytes; `vendor-manifest.json` binds all 282 production files to
their original and modified hashes. No exact Gameloft fork revision is asserted.

## Source correspondences and actual differences

Original matrix reader `0x7965d4` uses align, flags, signed bit fields, 16.16
scale/skew and twip translations, corresponding to upstream `matrix::read`.
Captured whole routine hashes are in `original-functions.json`; no reader
instruction differential is claimed by the facade smoke.

Original export loader `0x75c5f8` handles bitmap exports before sound. Its
bitmap branch exports the object then calls **substitute_bitmap_character**
`0x759cfc` (this is its exact symbol, superseding earlier shorthand
`load_external_texture`). When a loader exists, that helper obtains bitmap info,
reads width/height, invokes loader(name,width,height), and applies a nonnull
returned texture. The isolated core adapter exposes this exact provider
boundary. Stock r1714 lacks the bitmap-export branch. `game-adapter-edits.json`
records that additive change; callback/state instruction parity remains a
separate future proof, not implied by source similarity.

The genuine filename resolver/backend lives in `scene-materials/swf_texture`:
SwfTextureLoader's `data/%s` producer precedes the actual filename rewrite.
Pass exact exporter names to that provider; never invent a basename lookup.

The original Glitch renderer, orientation-aware viewport, hit-test implementation,
and RenderFX contexts differ from upstream. r1714 stencil hit-test queries use
authored pixel coordinates; the modern sink's scaled query is an explicit
upstream adapter. No original stencil-query symbol was found in the inventory.
Source matrix/display-list read paths are reusable; this is not a drop-in claim.

Original `font::get_glyph` tries bitmap/FreeType providers before code lookup
and preserves successful provider advance (Font3 times20). Upstream resolves
code first and can overwrite provider advance with embedded metrics. This
fork difference is **not patched or claimed equivalent** here. The independent
font worker supplies the actual font/provider proof and adapter. Only eight
of the 52 authored font records contain one glyph each, so embedded outlines
alone cannot produce the menu alphabet.

## Native ownership and portability

`portability-edits.json` records modern compiler/ARM64 changes: qualify the
legacy global array, avoid math macro/overload collisions, preserve size_t hash
values, make unspecialized assertions dependent, adapt Linux socket fields,
separate build-version macros, and replace unavailable Android ftime with
equivalent gettimeofday milliseconds. Signed bit extension now uses unsigned
shifts; the short-string terminator uses object-representation byte access
when its final NUL aliases the length byte. These preserve existing bytes and
remove undefined behavior reached by the actual HUD.

Facade destruction is a **native ownership correction**, not original destructor
parity: pin the reachable AS graph (global, tracked heap, prototypes, members,
properties, arrays, closures, environment registers/locals), sever cycles only
after playback ends, then release roots/player. Upstream's tracked heap omits
DefineFunction allocations, and ordinary teardown leaked 1,348,296 bytes in
1,760 allocations. The corrected actual HUD audit passes ASan/UBSan and leak
detection. Vptr instrumentation is excluded because disabled JPEG abstract
interfaces have no linked RTTI implementation; JPEG is not a supported provider
in this build. Concurrent threads must serialize facade calls; synchronous
reentry is rejected with `SWF core busy`. Provider objects/textures must outlive
every movie and glyph that borrows them. Inputs are trusted authored SWFs;
hostile arbitrary-SWF hardening is not established.

## Services and rendering contract

`SwfServices.read` copies owned stream bytes. `texture` gets exact export name
and source dimensions; `image` gets borrowed alpha/RGB/RGBA bytes and pitch.
Required texture/image/native/stencil failures propagate from the reached
operation. `draw` preserves command order, signed16 vertices converted exactly
to float, row-major 2x3 matrices, RGBA multiply/add terms, fill UV matrix,
wrap/blend and mask boundaries. Coordinates are twips. Rects are
`xmin,xmax,ymin,ymax`. Bitmap glyph quads use caller RGBA and ignore current
cxform. Solid/bitmap color conversion is the genuine separate swf_texture
kernel; this facade passes raw inputs. Visibility culling submits conservatively.

The actual HUD requests `NativeGetStringFromSymbol("GAMEPLAYMENUS_FASTTRAVEL")`
and an authored random `GLOBAL_DEATH{1..4}_01` key. The audit's symbol echo is
explicitly a localization fixture. The production facade routes this through
`native_call`; the root's real localization provider is required for visible UI.
The audit preserves 27 core error diagnostics, including AS package lookups;
full classes/actions, source shared-context behavior and game-global binding
remain unproved. Successful draw emission does not erase these gaps.

## Integration and reproduction

Include `gameswf_sources.cmake`; build its 95 source files plus `swf_movie.cpp`
with the vendor root include path. Define JPEG/PNG/THREAD/FREETYPE link options
as zero (the external font provider is separately linked); link zlib and dl.
GCC currently requires `-fpermissive`; Clang ARM64 compiled all 95 sources at O2
without that flag. Use C++17 for the app/facade. No networking/video/audio host
provider is connected. No ARM32 library/runtime is shipped by this module.

Test target source: `tests/swf_movie.cpp`; its sole argument is a directory
containing genuine `dqshared_droid.swf` and `dqhud_droid.swf`. The isolated
sanitizer executable is `/home/adampalace/dh2-gameswf-asan/swf_movie_audit`.
`tools/swf_movie_host.py` binds source, inputs, executable/static core, stdout,
stderr, sanitizer options and the actual separate NDK compile report. Reports
scope this as a native upstream HUD facade audit, **not full original UI parity**.
