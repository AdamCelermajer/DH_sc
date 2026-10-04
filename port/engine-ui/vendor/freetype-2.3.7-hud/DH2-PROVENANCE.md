# Source-built font backend

FreeType 2.3.7 is the version written by the original game's `FT_Init_FreeType` (`0x70d708`); executed original version probes are in `port/level-world/reference/font-text/freetype-version`.

Official archive: `https://download.savannah.gnu.org/releases/freetype/freetype-old/freetype-2.3.7.tar.bz2`

Archive SHA256: `4ecf879eb69fc323669981f02aebff1e3045de415303e86ee67f2080cb3ee888`.

The FreeType License in `docs/FTL.TXT` is the selected distribution license. Original notices and separately licensed bundled code are retained. Portions of this software are copyright (C) 1996-2008 The FreeType Project (www.freetype.org). All rights reserved.

Native portability changes preserve the reached operations:

* `src/autofit/afloader.c` does not form an unused rightmost-edge pointer for an empty/null edge array. It also avoids copying null arrays for zero-length outlines.
* `include/freetype/internal/ftmemory.h` skips zero-byte array copy/move operations. Sanitizers reject null pointers passed to libc even when the requested count is zero; these operations have no payload effect.
* `include/freetype/internal/ftstream.h` extracts signed byte fields using bounded multiplication, and unsigned shifts before signed32 interpretation. This preserves decoded bits while avoiding negative signed left shifts in newer C dialects.
* This separate HUD snapshot additionally changes bounded signed sbit strike metrics in `src/sfnt/ttsbit.c` and bounded sbit glyph metrics in `src/truetype/ttgload.c` from left-shift6 to multiplication64. The actual original-cache `wqy-zenhei.ttf` reached a negative descender, which is undefined under signed left shift. The original frozen Fontin snapshot remains unchanged. No embedded glyphs are omitted.

The untouched downloaded archive and a complete original file-hash inventory are retained in discovery. Native file hashes and these deviations are recorded separately. No rasterizer, font data, glyph hinting policy or source arithmetic was replaced. Matching release version alone does not establish original build-option or complete raster-image parity; the accompanying original-instruction differential specifically verifies the engine's no-cache glyph layout and padding contract with explicit FreeType service inputs.
