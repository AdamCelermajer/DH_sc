# Original dynamic text layout v1

This native subsystem owns fonts, glyph images, styled records and parser
attributes. It reconstructs the complete original `format_text(bool)`,
`html_reader::parse/parse_tag`, `append_text`, `append_image`, alignment and
bounding-box coordinators. It preserves the existing frozen plain-text helper
and vendor/core translation units. It does not yet replace Android text draws.

Whole original instruction replay and optimized native sanitizer replay compare
960 cases: 768 combinations of HTML/plain, four alignments, multiline flags,
font3 and 24 actual input patterns, plus 192 missing-glyph cases with initial
global diagnostic counts 0/9/10/11. All final record/glyph fields, cursor/bounds,
line/word state, styled font identities, diagnostic counter and ordered font
services match. ASan/UBSan/leak findings are zero. Original arrays, strings,
hash storage, font metrics/glyphs, image lookup and imported C-locale table are
explicit test services. Their engine/platform producers are separate work.

The original uses 1024 in its root scale calculation, separately rounded
float32 operations, relative font-size deltas multiplied by 20, and wrapping
at equality with width-minus-right-margin-minus-80. Word movement can span
styled records. CRLF, backspace, invisible word separators, nonbreaking spaces,
Unicode narrowing and the source CJK advance adjustment are retained. Tags
support `p`, `font`, `b`, `i`, `u`, `img`; unrecognized tags do not acquire new
browser HTML semantics. Closing tags pop one attribute regardless of their
name. The original first-ten missing-glyph diagnostic counter is caller-owned
and shared, with required delivery and prefix retention.

The recovered valid domain includes bounded, well-formed UTF-8 and safe source
tag/array/conversion inputs. Unsafe backward scans, truncated UTF-8, original
512-byte tag buffer overflow and unsafe float-to-integer conversion fail
explicitly. Overflowing imported atoi inputs and arbitrary synchronous
mutation of text storage/record containers are outside this proof domain.

The typed retained core adapter, original textformat setters, bound-variable
text setter composition, source preload body and full glyph/image/underline
display body remain prerequisites. Font cloning, unit/height/kerning queries,
image lookup and root scale/preload/logging require actual owners. No default
font, bitmap, offline root or successful missing-provider no-op is supplied.

`text-layout-v1-host-audit-v2.json` binds the final sources and v2 gold. The older
768-case receipt and corpus remain historical and are not rebound to the final
diagnostic extension. Android evidence is compilation of final ELF64 objects
for ARM64 and x86_64, not a linked APK, device run or 16-KiB LOAD proof.

Keep this kernel frozen. Implement retained rendering/AS connection in separate
versioned source; extend or migrate the kernel only with a new explicit proof.
