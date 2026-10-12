# tinyxml-and-c-libs notes

## Status (this session)

Ledger: 1793 rows (1734 upstream, 58 skipped, 1 partial), out of 1846 queued
functions. Remaining 53 are unledgered on purpose:

- game code (not library): II_Item_sortby* (3), VZPurchase_* (10), NVThread* (5),
  SSEncDec_* (14), Java_..._nativeInit (1)
- stb_vorbis parser (start_page, start_packet, start_decoder, start_page_no_capturepattern)
  and undecompiled/ambiguous FreeType helpers (ft_gzip_file_fill_output: Ghidra failed;
  Compute_Funcs, Compute_Point_Displacement, Dual_Project, Project*, Normalize, SkipCode,
  Sort, update_box, hash_bucket/hash_free/hash_insert, null_method, self_destruct,
  pass_startup: names could belong to more than one library)
- OutputDebugStringA (Win32 import wrapper)

Ledger generation: map_upstream.py (in this folder) reads queue.csv and writes
`upstream` rows by name pattern. Rows are name-matched, and a spot check of about
10 bodies per library found no obvious local modifications. They are not
byte-diffed against upstream.

## Libraries identified

| library | version evidence | functions queued | status |
| --- | --- | --- | --- |
| TinyXML | std::string-based API with TiXmlAttributeSet sentinel list and TIXML_ERROR codes, consistent with TinyXML 2.5.x. No version string in strings.csv; exact patch level not confirmed. | 219 (TiXml*) | 216 upstream/skipped, TiXmlPrinterDH::VisitEnter partial |
| zlib | strings: " inflate 1.2.3 Copyright 1995-2005 Mark Adler", " deflate 1.2.3 ..." | 71 | upstream |
| libpng | strings: "libpng version 1.2.32 - September 18, 2008" | 311 | upstream |
| libjpeg (IJG) | jpeg_* imports, CImageLoaderJPG; no version string found; 6b-era API | 155+105 | upstream (name pattern) |
| FreeType | FT_* exports; source paths from glitch freetype build; vendored copy is 2.3.7 | 668+178 | upstream (name pattern) |
| GLU tessellator (SGI libtess) | __gl_* / glu* symbol names, plus static helpers in the 0x7aed38-0x7e15c4 block | 97 | upstream |
| libpng, zlib, libjpeg, FreeType | queued under owner `lib-image-zip-font` | about 1300 | not started |
| misc (VZPurchase, NVThread, SSEncDec, stb_vorbis, Java glue) | game code | about 50 | not started |

No upstream TinyXML source is vendored in the repository, so "upstream" in
the ledger means behaviour matches TinyXML 2.5.x as read from the pseudocode, not
a byte-level diff against the release.

## Recommended vendoring

- TinyXML: vendor the upstream 2.5.x sources and keep only TiXmlPrinterDH
  (`include/tinyxml-and-c-libs/tixml_dh.hpp`, `src/tixml_dh.cpp`) as a game overlay.
- zlib, libpng, libjpeg, FreeType, GLU: vendor the upstream releases. Their
  functions should be ledgered `upstream` and not rebuilt. Versions still need to
  be confirmed from strings.

## Layouts inferred

The original TiXmlPrinter is 32-bit ARM-era: vptr +0x00, depth +0x04, buffer
(std::string) +0x0c, indent string at +0x34..+0x38. The header uses std::string
members instead, so these offsets are documentation only and are not used for
pointer arithmetic.

TiXmlAttribute: value at +0x40 (returned by Value()), name at +0x24..+0x28,
next/prev sentinel links at +0x44/+0x48. TiXmlAttributeSet is a circular list
whose sentinel is the set itself (+0x44 = last, +0x48 = first).

## Open questions

- TiXmlPrinterDH::VisitEnter: the pseudocode ends in a non-returning call, so
  attribute printing and the return value are not confirmed.
- TiXmlAttribute::SetIntValue and SetDoubleValue, TiXmlAttributeSet::Find(std::string),
  and TiXmlElement::Attribute(std::string): the pseudocode lost the return or the
  string assignment, so these are not ledgered.
- The rest of the TiXml family (Parse*, StreamIn*, Print, TiXmlDocument, TiXmlHandle,
  TiXmlNode accessors, EncodeString, ConvertUTF32ToUTF8, GetEntity, GetChar,
  ReadName, ReadText, StreamTo, StreamWhiteSpace, TiXmlHandle::Child*, ...) is
  still queued.

## Tests

tests/tixml_dh_test.cpp checks TiXmlPrinterDH::VisitEnter indentation and element name output.
