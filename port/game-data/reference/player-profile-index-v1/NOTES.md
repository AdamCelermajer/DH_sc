# Campaign profile index and saved metadata

Adapted from Adam commit `791e961b12233100b303038c961666834f4beb9d` using the pinned original ELF as authority.

- `Savegame::_cacheFile` at `0x315ad0` indexes count, payload size, four-byte C-string tag, payload. Duplicate tags publish the last offset/size. Immutable retained snapshots own the bytes; a borrowed snapshot blocks replacement.
- `SG_Load` at `0x465430` calls the complete `_Load` at `0x464f4c` before `_LoadVolatileQuestsLog` at `0x468574`. Typed services preserve every reached external boundary. The runtime borrows the sole Save and the caller's canonical profile slot. Explicit subsequent loads remain valid; there is no automatic retry or rollback.
- Mask `1` dispatches PNAM, PLVL, PCLS, PDFL, LNAM, LEPT, LUSP. It does not initialize levels, skills, faeries or quests (mask `2` does). Other masks require actual section/init/online/quest providers. Later tags reread the profile slot; CFEE captures the profile after FAES and retains its writer even with a null reader.
- PCLS resolves `Arrays::CharacterTable` names, not class-formula names. PDFL directly stores `PlayerSavegame::m_difficultyLevel` at original `0x9a6060`, then reads this Save's unlocked-difficulty word at `+0x3c`. A failed later read retains that global store.
- LNAM writes unsigned `+0x38`, then three ordered triples to `+0x50`, `+0x5c`, and both quest-log slots `+0xfc` / `+0x15c`. LEPT writes three signed words at `+0x40`; LUSP writes three raw bytes at `+0x4c`. Their backing fields live in the existing Save owner. Future quest-log services must borrow these canonical slots.
- The blank source ctor leaves the level ID and spawn bytes untouched. Port completion flags distinguish safe zero storage from source-produced values. Truncated native spans stop before an incomplete read and retain completed stores. Malformed index seeks are safely rejected before publishing an unsafe span; original corruption/backup paths are outside that claim.

The original replay executes 96 complete index cases (including original STL), 1,780 complete SG_Load cases, and all seven metadata bodies across 128 synthetic cases. It records full function hashes, executed instruction addresses, exact generator inputs, and binary gold. The selected host links the actual game-data DSO, checks every source load delivery failure prefix, completed metadata prefixes, profile replacement/capture behavior, lifetime/alias guards, and the source filename corpus.

The private cached campaign is read only in the host fixture. Its name and raw bytes are not published. Anonymized results are class ID 263, level 1, selected/unlocked difficulty 0/0. Class identity is from the 448-row CharacterTable. Filename slot 0 is an explicit fixture selection, not source slot discovery.

File construction/I/O and callback registration/persistence remain required provider boundaries. This metadata Save is separate from Character's gameplay Save in the original ownership layout. The evidence does not establish PlayerInfo Character association, locality registration, new-game creation, full mask4 saved sections, gameplay initialization, VM or vitals effects.
