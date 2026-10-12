# Native level and fast-travel data

The original array readers are FastTravelList.read at 0x4ba8e4 and
LevelList.read at 0x4ba794. They execute actual row readers at 0x4fcff0 and
0x4fca84, scalar/string stream readers and array allocation/finalization.
The virtual byte stream and allocator are explicit caller services. This is
data decoding proof, not original Application ownership or level selection.

Original cache data contains 33 fast-travel destinations (bytes 0..1133), then
51 levels (ending at byte 5574). Names and schemas contain matching sections.
`original-loader-capture.json` records original engine/cache/input hashes.
`level-table-fixtures.bin` retains all 84 original cache records and 128
synthetic records with raw signed words, empty/embedded-NUL/high-byte strings
and noncanonical bool bytes. Source vtables/string pointers and bool padding
are normalized; serialized bool bytes themselves are preserved unchanged.

Native record decoders use borrowed string spans and atomic bounded failure.
`load_levels` owns decoded strings and rows and validates the two schemas and
section dimensions. This deliberately rejects malformed/truncated input;
original unsafe short-read behavior is not claimed equivalent.

Original versus optimized ARM64 passes 212 records, 2,749 scalar words and
327 strings. Central sanitizer replay additionally checks 11,214 truncated
prefixes, 13 malformed-input cases, every loaded cache row and section bounds.
Application construction, active row selection, difficulty input and full
level loading remain separate producers.

Source scalar projections retain 18 words for LevelDeclaration and seven for
FastTravelDestination, with pointer slots zero. They are not native object
layouts. Level range words are indices 12..14 (maximum by difficulty) and
15..17 (minimum). GOTHICUS_CRYPT_01 has maximum [10,47,76] and minimum [8,45,74].
This alone does not choose an active level or prove the current player session.
