# Character saved class and saved property readers v1

Original ELF SHA256 is pinned by `original-functions.json`; the complete ARM
ranges and disassembly remain adjacent. This is a caller and reader borrower.
Native Player slot assignment, PlayerInfo association, complete InitPost and
mask2/mask4 section orchestration remain owning producers.

## Class caller

`SafeGetCharPropsId` 728B at `0x3b3d38` first returns the existing signed halfword
at Character `+13c8` when it is not -1. Otherwise it calls fresh IsPlayer. The
Player branch invokes Character `SG_Load(1)` using live `+14e8`, reads the fresh
Save class word `+34`, truncates/sign-extends it to 16 bits, and publishes the
cache. Only -1 takes the first-match CharacterTable `KnightPlayerBase` lookup.
The final signed class is written through the current gameplay Save pointer.
Null Save follows the original wrappers' no-load/get--1/no-write branch.

The LoadOwner must borrow that same Save. It retains its own canonical profile
slot; preview metadata is never copied. Save/profile replacements delivered by
callbacks remain live through the call, and following wrappers read the current
slot. Failure preserves the completed Save and cache prefix. No failure latch
or automatic retry was introduced.

The nonplayer template field takes priority over explicit property name. The
176B helper at `0x3b36ec` performs a new first-match name scan and writes the
existing template halfword cache. Ordered duplicate alternatives are retained.
The existing `template_random` adapter advances only ordinary RNG stream0.
Halfword alternatives are published without extra class or CharacterTable ID
validation. The existing factory's conflict and ambiguity policies are not
used because they are stricter than this original caller. Source strings are
projected through the existing Source descriptor; its editor/data-class fields
are not additional conditions in these original functions.

## PROP reader

`PlayerSavegame::__LoadProperties` 296B at `0x46932c` requires the existing
nonnull Character. A count other than 224 is a normal four-byte early return.
Every source word is consumed; the borrowed live type is normalized by the
actual `_GetType` rule (-1 becomes 0x10), then `type & 0x20` controls the write
to the same existing saved sheet. The final raw byte is stored in the sole
Save's source `+194` projection. This byte has a blank-constructor zero producer
at `0x465b9c`; it is not a readiness, healing or profile-loaded flag.

No resolve/recalc, inventory, buff, HP/MP, animation, VM or timer calls occur.
Bounded truncation retains every completed four-byte store and the previous
byte194 until its final byte read. The null-Character Debug assertion/reread
branch and unsafe pointer/assertion domains are explicit added failure
boundaries, not successful emulations. The caller supplies the actual
Character's PropertyView; this reader creates no property owner.

## Proof scope

Public gold uses synthetic table and stream fixtures. Whole original class
caller/template helper, SG pointer wrappers/getter/setter and inline RNG execute.
IsPlayer, SG_Load's external body, strcmp, integer division and stream virtual
are declared leaves. Whole nonnull PROP reader, nested `_GetType` and valid-id
`_GetProperty` execute. Original reads stopped at insufficient stream bytes are
compared with native retained prefixes.

The selected-host runner stages missing scoped/helper sources into the actual
`dh2_level_world` target, detects translation units exactly once, links the test
to that DSO and its real `dh2_game_data`, and guards actual Ninja dependencies
plus exact replay files. Final reports state whether central selection was
already present. The real private campaign mask1 composition runs through
its index, seven metadata readers and the same gameplay Save; only anonymized
class/level assertions are published. No private campaign/name payload is
written to public fixtures or reports.
