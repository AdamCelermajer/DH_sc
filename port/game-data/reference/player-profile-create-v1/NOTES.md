# Player profile creation V1

The authority is the pinned original ELF SHA256
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
The replay executes NativeCreateSaveSlot (504B at `0x43f630`), the indexed
PlayerSavegame constructor and mask1 load dispatch, the offline SG_Save and
volatile tail, all seven metadata writers, and the complete 1032B
Savegame::saveAll stream assembly. AS conversion, underlying profile storage,
registration, catalogue/clock/online queries, stream virtuals, cache/file I/O,
and temporary destruction are declared leaves. The independently replayed
writers use the exact Save image captured at the live saveAll dispatch; its
original stack frame must not be reused after NativeCreate returns.

The selected host links the actual game-data library and the native Transport.
Nine accepted creations produce byte-identical persisted metadata and same-index
publication; a distinct gameplay Save then reads each file through SG_Load(1).
Eighteen original caller cases and eleven provider-failure/mutation cases pass.
The compiler closure contains 94 stable project inputs. All five checked
production translation units occur once.

PNAM/PLVL/PCLS/PDFL/LNAM/LEPT/LUSP are the only indexed-constructor metadata
writers. Save+38 is the date; +50 starts at Crypt41, +5c contains generated
seeds, and the embedded quest constructor sets +fc to1. The current difficulty
is the borrowed source global. Real clocks and a fresh authored difficulty
count remain caller providers. No gameplay inventory/property/VM initialization
is added. Full online save and volatile continuation remain mandatory providers.

`catalogue-functions` pins the source static catalogue/slot/clock bodies.
These additional bodies are static evidence, not part of the 662-word replay.
The native catalogue uses source substring filters, lexical sort, and atoi at
filename+4; there is no four-slot creation bound. Overflow and nonregular
matching entries are explicit native boundary failures.

Fixtures use synthetic names and a synthetic CharacterTable corpus. No private
campaign, player name, or raw inventory payload is published.
