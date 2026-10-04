# Native CharProperties buff ownership

Original ELF SHA256: `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
`original-functions.json` and `reference/original-functions.asm` capture sixteen
actual functions. This module owns the source buff map/instance lifetimes; it
does not substitute buff expiry for periodic DoT attacks.

## Source producers and ownership

`PROPS_AddDot` **0x3e2720** searches the two supplied source string dictionaries
independently for `AUTO_DOT_01_FIRE`, adds signed element -1..4 to the resulting
index (including missing index -1), and selects `dot_normal`, `dot_fire`,
`dot_water`, `dot_lightning`, `dot_earth`, `dot_air`. It calls AddBuff with
duration in raw integer milliseconds, capacity1, raw amount as unsigned strength,
and the resolved FX OID. It writes raw amount to property127+element and runs
actual RecalcProperty for that channel. Duration/amount/element source assertions
continue or deliberately trap according to a global assertion policy; the port
accepts the proved safe domain duration>0, amount>=0, element -1..4.

`PROPS_AddBuff` **0x3e232c** uses a signed-ID red-black declaration map at
CharProperties+0xe18. The ARM32 node has key+0x10 and BuffDecl begins+0x14:
ID0, FX handle4, string8, deque+0x20. Deque iterators are node+0x34/+0x44.
Default declaration constructor **0x3e1a08** initializes ID=-1, FX=null, empty
string and allocated empty deque. Each BuffInst is a property sheet plus unsigned
strength+0x384, signed timer ID+0x388, properties owner+0x38c and declaration
pointer+0x390 (source allocation0x394). Native instances are owned heap objects;
their actual addresses, never integer stand-ins, become timer user_ref.

Capacity<=0 becomes128. When instance count equals capacity, the actual source
scans all insertion-ordered instances. Lower unsigned strengths become candidates
and are immediately overwritten even when a later candidate wins. Equal strength
with timerID=-1 wins immediately; otherwise candidate/current TimeLeft services
compare **elapsed**, not remaining duration, retaining the candidate when its
elapsed>=current elapsed. Stronger instances are skipped. If none qualifies,
the source still assigns declaration name/acquires missing FX before capacity
rejection. If count differs from capacity, no replacement scan occurs.

Reused instance: StopTimer(old ID), store-1. New instance: append before timer
start. Nonzero duration starts repeat0/event**0x36**/user_ref=instance; source
StartTimer=-1 deletes that instance through DelBuff and returns null. Duration0
is a permanent buff without a timer. FX enable occurs only with nonnull FX and
instance count>1: cached FX object, virtual+0x1c(index=count-1,enable1,arg0).
Finally ResetSheet **0x3def84→0x3def34** copies all224 source defaults, then writes
wrapping `strength<<8` to property172 in the instance and current resolved sheet.

There is **one existing resolved sheet** (properties+0xa94), including DoT126..131.
No additional DoT sheet is allocated. Periodic event0x34 neither changes duration
nor removes these buffs. `RemoveDot`0x3de83c is the original empty function.

## Expiry, removal, teardown

`BuffExpired` **0x3e123c** calls Timer.GetReference first, captures instance timer
ID before Timer.GetID, checks the IDs through the source assertion policy, then
calls DelBuff(instance.declaration.ID,instance). Non-trapping policy continues
even on mismatch. TimerStore makes a repeat0 slot inactive **before** expiry,
and delivers the Timer object, not user_ref directly.

`DelBuff` **0x3e101c** searches signed ID. Missing ID is empty. A declaration with
exactly one instance is deleted **regardless of supplied instance identity**:
StopTimer, property-sheet destructor/free, FX release (even null), erase map,
RecalcProperties(true). For other counts, null/missing instance is empty; pointer
match performs StopTimer/destructor/free/deque erase then full recalc, retaining
the declaration. RecalcProperties(true) **0x3e0810** first loads source class into
base using base ClassID26, then RecalcProperty0..223 in that order.

`RemoveAllBuffs` **0x3e0af8** traverses ascending signed-ID map, insertion-ordered
instances, stops/frees each, releases FX per declaration (even null), clears map,
then full recalc. `CharProperties D1` **0x3e0c6c** differs: free sheets, clear each
deque, release **nonnull** FX, clear map; **no timer stop and no recalc**. It then
destroys resolved/gear/saved/base sheets, which remain borrowed in this bounded
buff adapter. Caller must end timer delivery before owner destruction; otherwise
original timer references would dangle as well.

## Native services and explicit limits

`character_buffs.hpp/.cpp` owns signed-key std::map, declaration strings and
deque-owned instances. PropertyView groups are refreshed in exact source map /
instance order, and remain owned by BuffOwner. Bindings borrow properties,
TimerStore, TimerServices and BuffServices for the entire owner lifetime.
No original 32-bit object overlay or runtime is used in native game code.

Actual native TimerStore start/stop/time-left executes. Required FX load,
release/object/enable and **full class-to-base plus all-property recalc** are
synchronous explicit services. Return1 only after genuine delivery; missing
providers return failure at the source prefix. A valid FX cache miss may return
null after real delivery. The current audit supplies explicit FX fixtures; it
does not prove physical FX application. A parent full-recalc provider can use
`dh2_class_recalc_base(Borrow.class_rows(),mutable base,view)`.

Malformed inputs/reentrant mutation leave output and owner unchanged (-1).
Read-only snapshots are allowed during services; synchronous mutation/destruction
is outside the supported caller ownership contract. Service failures preserve
the executed prefix (-2). Allocation exception detaches group pointers and
quarantines the owner from further mutation; destruction remains available.
Original TimeLeft(-1/inactive) leaves comparison locals unwritten: the port
rejects this undefined-storage selection domain, retaining earlier strength
mutations. Native timer storage/growth failure is explicit and never treated as
a successful timer ID. Snapshot pointers expire on corresponding source free,
name reassignment or owner destruction; groups can change on every mutation.

## Evidence and reproduction

`tests/character_buffs_differential.py` executes original AddBuff/AddDot/delete/
expiry/remove_all/D1 control bodies, genuine RB searches and deque count/advance,
all-property ResetSheet/RecalcProperty/full Recalc instructions. Source class
fixture is genuine negative ClassID=-1 (source skips class row). ARM32 allocation,
map/deque **mutation**, std::string, timer and FX imports are explicit fixtures.
ARM64 O2 executes native owned STL, actual native timers/property/class kernels.
Signed IDs, unsigned extreme strengths, wrapped shift, capacity128, multi-block
deque, zero duration, stronger/weaker/equal replacement, unequal elapsed,
missing dictionaries, six DoT channels, mismatched expiry IDs, one-instance
nonmatching deletion and destructor order are covered.

`reports/character-buffs-arm64-differential.json`: **528 comparisons**, **1231
ordered service requests**, zero mismatches. Original-derived gold:
`buff-fixtures.json`, SHA256
`9111b79af4d47c88df7e809557ed659215424d2af39850ad1a86ddf32635a1ed`.

`tests/character_buffs_host.py` converts that preserved gold to BUF1, builds
an isolated sanitizer DSO linked to genuine central world/game-data/runtime
libraries, and observes timer calls through executable interposers which
immediately forward `dlsym(RTLD_NEXT)` to real kernels. Original-gold replay,
six actual one-shot TimerStore→BuffExpired→full-recalc expiries, real authored
class table, nine atomic guards, stale selection rejection and missing-service /
reentrant ownership checks pass: **268980 checks**, ASan/UBSan zero findings.
`character-buffs-host-audit.json` binds actual module/executable/dependency hashes.

Commands from repository root (direct Python/cache dependencies, no uv):

```
port/level-world/tools/build_character_buffs_oracle.ps1
python port/level-world/tests/character_buffs_differential.py --library .local-inputs/character-buffs-discovery/oracle.so
python port/level-world/tests/character_buffs_host.py
```

Parent integration: add `character_buffs.cpp` to world DSO; add
`character_buffs_audit` from `tests/character_buffs.cpp`, link world/game-data/dl
and `-Wl,--export-dynamic` for observation interposers. Then host runner
`--main-linked` records the actual central module identity. Direct executable
arguments: derived `host-fixtures.bin`, Android assets/data directory.
No packaged-APK, live monster damage, full FX, or whole-frame parity is claimed.
