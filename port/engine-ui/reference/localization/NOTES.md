# Native HUD localization

The new `localization.hpp/.cpp` owns the ordered common-text table and decoded
text-sheet strings. It implements the text-only path needed by the actual HUD
`NativeGetStringFromSymbol` calls. No fallback translations are invented:
`GAMEPLAYMENUS_FASTTRAVEL` returns the original English `World Map`; an absent
symbol returns the original wrapper literal `notfound`.

Original ELF SHA256:
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
Root's four complete wrapper/lookup/preload/filename captures remain unchanged.
`dependencies/original-functions.json` and its `reference/original-functions.asm`
add seventeen complete parser, owner-getter and table-reader bodies.

## Ordered input and loading

Actual `Arrays::StrID_Languages::read` at `4b53f8` and nested
`Structs::LangSheetList::read` at `4dc2ac` consume all 10,167 bytes of the supplied
common_text array. Executing `getSheetName507568` and `getSheetFilename507bbc`
produces all 333 ordered bindings. There are nine packs, in the names-file order:
ENGLISH, FRENCH, GERMAN, ITALIAN, JAPANESE, KOREAN, SC, SPANISH, SYMBOLS; each has
37 sheets. Neither the text-index dictionary nor alphabetic sorting supplies the
source order. The serialized sheet entry is filename then name; the ARM32 row is
20 bytes, with filename length/pointer at +4/+8 and name length/pointer +12/+16.

`getSheetFilename` formats `text/%s`. The resource provider explicitly resolves
that logical name under the source data directory. Current staged bytes are
`original-cache/data/text/<filename>`; this is an APK asset-provider mapping,
not a newly claimed original FileManager alias. Metadata paths are:

- `original-cache/data/pydata/common_text_pyarray.bin`
- `original-cache/data/pydata/common_text_pyarraynames.bin`
- `original-cache/data/pydata/common_text_pystructnames.bin`

Sheet data is little-endian uint16 count followed by uint16 byte-length strings.
The native owner checks EOF and bounds before publishing a sheet. Embedded NUL,
truncation, count above signed32767 and trailing input are bounded native caller
rejections; those malformed domains do not claim source stale-memory parity.
Source list/count assignment precedes the mandatory Debug query and file
close; a delivered Debug failure retains the published sheet and closes its
lease. A force reload clears the old sheet first. Every successful acquired
resource lease is closed, including parser/provider failure paths.

## Lookup and transformations

`StringManagerC1 507d10` initializes current language to -1. Index lookup maps
-1 to pack0; `getSymbolPack5075b0` returns8. `getStringFromSymbol508c74` performs
DebugSwitches.load/GetSwitch(`isTracingStringManager`) before searching. The
prefix before the first underscore is compared against each ordered sheet name
using **strncasecmp**, then the full symbol using **strcasecmp**. A successful
lookup repeats the Debug pair. Source comparison is prefix length, not exact
sheet-name equality. Missing underscore is outside the native caller contract.

Loaded empty entries return the original `#!SNL!#`; an index beyond a loaded
count returns `#!WTF!#` in the source nonasserting domain. Missing symbols return
`notfound` directly, without numeric defaults or player-name callbacks.

`preloadPackSheet50851c` invokes `parseColors507ea4` only for raw strings containing
caret or pipe, and replaces their bytes **only if parseColors returns true**.
Thus a string containing only an unknown caret directive retains its raw bytes
at preload. Numeric colors use genuine `FontTextColors` zero..nine constants,
mask RGB to24 bits and emit uppercase `<font color="#%06X">`. `^n` emits newline,
`^r` emits `</font>`, `|` emits byte0x11. The source parser preserves the listed
varargs/service directives for the subsequent general parser. Unknown directives
drop both bytes in its candidate output; candidate output is discarded if no
actual color/control transformation occurred.

`NativeGetStringFromSymbol444ebc` then calls `parse508ef4`. It always fetches three
StrID defaults for nonempty input, including the source misspelling
`GLOBAL_DECIMAL_SEPERATOR`, then `GLOBAL_THOUSANDS_SEPERATOR` and
`GLOBAL_THOUSANDS_GROUP_AT`. Each ID lookup fetches StringConfig PackIDShift,
PackIDMask, StrIDShift, StrIDMask and uses the original signed ARM ASR/masks. These
are real inputs even when the resulting number-formatting strings are unused.
Current constants bytes are `original-cache/data/pydata/common_text_pycst.bin`;
color constants are `data/fonts_pycst.bin`. The host connection uses the genuine
native PyDataConstants loader/getConstant, without supplied constant successes.

The bounded plain parser implements source byte/control/literal behavior used by
HUD translations. Active `$dfghikmpstv` varargs/service directives are explicit
failures until their real argument/service producers are supplied. General
numeric/date/string varargs formatting is not silently accepted. The final
`gameswf::format_utf_text752a18` replaces spaces preceding `!.:;?` with UTF8 NBSP,
for every pack. It also inserts an ASCII space after !/? only when `addSpace50750c`
is true: packs4,5,6 (Japanese, Korean, SC). This is source behavior, including
double spaces if an authored space already follows punctuation.

## Player and global effects

`ParsePlayerName433d00` queries `GetLocalPlayer(0,true)`, then reads Character
pointer at player+660. If null, it copies its input unchanged. If nonnull, it
queries the player again, rereads Character+660, and executes
`Character::SG_GetPlayerName3bb7e8` (profile pointer+14e8, name pointer profile+2c).
It calls `parse` with format `^$`, source input, `$player`, and the current name;
numeric defaults are fetched again. The native callbacks preserve these two
player queries and the name call order. A first-null branch is an explicit
no-character provider state; no player name is manufactured.

Original-instruction probes confirm the source replacement algorithm replaces
only the first complete `$player`. A trailing `$play` is discarded, and a failed
partial match does not reset the source needle cursor. This is preserved rather
than replaced with a generic replace-all operation.

The wrapper also writes its original menu-related global byte1 only for exact
`strcmp` equality with `MENU_ERROR_NO_USERNAME` or `MENU_ERROR_NO_PASSWORD`.
`LocalizationResult::sets_menu_string_flag` is a write1 request; false means no
write, not clearing an existing global. The native module does not own the full
original Application global. Result publication is atomic on native malformed
or provider errors; published sheet/cache prefixes remain, as described above.

## Proof and integration boundary

The optimized ARM64 differential executes original common_text readers/getters
and original color/plain/player parsing instructions, including **every one of
the4,178 real cached strings reaching the color-parser branch**. Its gold also
contains edge cases for escaping, first-only and partial player matches. The
owned connection oracle executes the **entire original NativeGetStringFromSymbol
body**, original lookup, parsers, and player-name getter. AS argument text/
tu_string conversion and AS result publication are explicit services, alongside
borrowed player records, genuine input constants and original Debug observers.

The sanitizer connection runs the source-built localization DSO against all333
real files/41,546 strings, the real native constants loader and genuine
DebugSwitches missing-file owner in a coherent private dependency snapshot.
It compares260 full wrapper results and4,760 ordered callbacks, then verifies
actual lazy World Map loading and malformed/provider failure contracts. This
does not claim the original Application language selection, live player manager,
GameSWF argument conversion, arbitrary varargs, or whole-menu GPU parity.

Providers and the Localization receiver must survive each synchronous call;
providers must not throw or destroy/reenter that receiver. Same-owner load,
pack mutation, preload and native-string reentry reject explicitly. Metadata
and strings are owned; supplied buffers may be released after load. Names and
filenames returned by reference are invalidated by successful metadata reload.
Boolean methods report native delivery success; unchanged pack or delivered
missing file need not equal the original changed/loaded boolean return.
