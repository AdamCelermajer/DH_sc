# Source AI relationship queries

## Reuse audit and scope

Existing `port/game-data/aggro.cpp` reconstructs threat table mutation/query,
not `AI_IsEnemy`, `AI_IsFriend` or `AI_IsNeutral`. Existing `ai.cpp::dh2_ai_enemy`
implements the pure Character faction-row first-match negative-value lookup,
with supplied player-pair facts. The `ai-target` reference explicitly scopes
that implementation to the Character enemy branch and snapshot inputs. There
is no existing `ai-relations` reference directory or friend/neutral source body.

The new `character_ai_relations::query` reconstructs three bounded normal source
query orchestrations and reuses `dh2_ai_enemy` for the enemy row sign lookup.
It adds null-target fallback, object-handle resolution, word `+0xf4` routing,
generic interaction queries, repeated live faction getters/count reads, correct
table/row capture timing, and friend/neutral predicates. It introduces no group,
global-design or float predicate: these three original bodies use integer
faction comparisons. There is no native/CMake wiring in this change.

Handle resolution, player/interaction virtual providers and invalid-ID diagnostic
assertion/crash bodies remain explicit boundaries. Thus the implementation count
is **three bounded query orchestrations**, not three full native bodies or ten
implemented dependency functions. Existing enemy leaf code is reused unchanged.

## Object identity and source branches

All three functions accept null input and load current AI `+0x40`. With no
candidate after that fallback, enemy/friend return false and neutral returns true.
The candidate GameObject identity is then fixed for this query.

`0x33dd70` is **const ObjectBase::GetHandle**, followed by `0x33ff8c`, **const
ObjectHandle::GetObject(false)**, a tail call to the non-const implementation at
`0x33fdc0`. These are handle resolution services, not an RTTI cast. The returned
live object is fixed in a register for the remaining query; it can be null after
handle resolution. These bodies treat resolved object word `+0xf4 == 0` as the
Character branch. A nonzero word follows the generic/default branches.

Friend returns false and neutral true for null/non-Character resolved objects.
Enemy instead uses the **original candidate GameObject identity**, not the
resolved handle identity: it calls current object virtual `+0x88`
`IsInteractive(current owner)`. If truthy, it calls current object virtual
`+0x90` `GetInteractionType(fresh owner)` and returns whether the integer is 8.
It does not call the adjacent `+0x94` interaction-radius slot here.

| Normal Character source phase | Enemy | Friend | Neutral |
| --- | --- | --- | --- |
| Target first faction getter / negative-ID assertion | `0x3d57c8` | `0x3d5170` | `0x3d5aec` |
| Target second getter / fresh global count | `0x3d57d8..0x3d57e8` | `0x3d5180..0x3d5190` | `0x3d5afc..0x3d5b0c` |
| Fresh owner first getter / negative-ID assertion | `0x3d5814..0x3d5820` | `0x3d51bc..0x3d51c8` | `0x3d5b38..0x3d5b44` |
| Separately fresh owner second getter / count | `0x3d5824..0x3d5834` | `0x3d51cc..0x3d51dc` | `0x3d5b48..0x3d5b58` |
| Owner IsPlayer / conditional target IsPlayer | `0x3d5860..0x3d591c` | Not called | Not called |
| Capture owner before loading global table | `0x3d5880..0x3d5888` | `0x3d520c..0x3d5214` | `0x3d5b88..0x3d5b90` |
| Third getter on that captured owner / row address | `0x3d588c..0x3d5894` | `0x3d5218..0x3d5220` | `0x3d5b94..0x3d5b9c` |
| Third getter on fixed resolved target | `0x3d589c` | `0x3d5228` | `0x3d5ba4` |
| Read captured row's count and entries afterwards | `0x3d58a0..0x3d58ac` | `0x3d522c..0x3d5238` | `0x3d5ba8..0x3d5bb4` |
| First matching entry's signed integer predicate | value `< 0` | value `> 0` | value `== 0` |
| Empty/missing entry | false | false | true |

Enemy's player check is after the first four faction-getter calls, not an eager
precondition. If owner IsPlayer is false, target IsPlayer is not called. If both
are true, enemy returns false without capturing the table or final getters.
Friend/neutral have no player-pair override; their faction values decide.

The source getter `GetCharAIFactionId` (`0x3a3180`) reads Character `+0xff8`,
checks its current global count, and returns fallback 10 for a negative or
out-of-range raw property. Services must preserve that getter semantics on every
call; the outer kernel does not replace repeated calls with cached raw properties.
The additional source assertions use separately fresh IDs/count reads.

Original faction table rows and entries each have 12-byte stride: row count is
`+4`, entry pointer `+8`, entry matching ID `+4`, signed value `+8`. Port views
reuse the reader's 8-byte `AiFactionEntry` pair, with explicit memory capacities;
they are not binary overlays. A table's backing row-array identity is captured
before the third owner getter. Its selected row's count/entries are read only
after the third target getter. Replacing the current global table does not change
the already captured table; modifying that captured row before its later read
does affect the result.

## Lifetime and errors

The caller retains State, fixed candidate/handle-resolved object, each exposed
owner, context and all captured table/row/entry storage until synchronous return,
including retired tables after replacement. Owner and target/table producers may
change through providers; only source-prescribed later reads are fresh. Borrowed
table row-array identity and capacities are stable port bounds, while row count
and entries stay live until consumed. Providers must not overwrite the result or
service table or destroy captured views. No multithreaded replacement is claimed.
Independent nested outputs are allowed; no original reentry ban is invented.

Top-level null/alignment/identity/overlap and unknown relation reject before
effects. Missing providers are required only on taken branches. Provider failures
and exceptions return explicit failure while retaining prior provider mutations.
There is no rollback, source cleanup or target mutation added by the kernel.

Original assertion/crash behavior for invalid getter IDs/counts is excluded.
Such facts fail explicitly rather than indexing invalid memory or inventing a
new faction clamp. Table/row/entry capacities are bounded to 4,096, consistent
with the existing reader/leaf contract; malformed captured bounds also fail.
Counts describe attempted service calls; result value is meaningful on complete.

## Validation

The host fixture checks 62 cases: all signed faction predicates, first-match
duplicates, empty/missing rows, target fallback, expired/non-Character handles,
generic interaction type, player-pair gates and skipped providers, repeated live
owner/target reads, owner captured before table replacement, captured row read
after the target getter, actual getter fallback10, and failure/alias/alignment
bounds. It compiles the unchanged existing `ai.cpp` leaf with the new source.

The original ARM oracle runs all three original normal query bodies with real
`GetCharAIFactionId` instructions and original faction-array scan/sign predicates.
It compares values and ordered service traces across 26 scenarios per query.
Object-handle and player/interaction virtual services are observers; original
handle-map lifecycle and invalid-faction diagnostics are not executed. Range
symbols/sizes/hashes and interaction vtable bytes are pinned to the verified ELF.
No original APK, Android, current Ghost behavior or full native AI is claimed.

```powershell
python port/level-world/tests/run_character_ai_relations_host.py `
  --compiler C:/Users/noamc/.local/mingw/mingw64/bin/g++.exe `
  --original-elf C:/Users/noamc/Documents/Codex/2026-10-02/contineu-from-where-they-left-off/work/test_strategy/libDungeonHunter2.so `
  --output port/level-world/build/character-ai-relations-review/host.exe `
  --report port/level-world/build/character-ai-relations-review/validation.json
```

The future candidate-consumer adapter maps its Relation enum to this query and
uses the current owner's embedded AI projection. Its native providers must own
the original handle/faction/property/virtual storage and retain returned objects
and captured tables. Production linkage must supply the existing game-data
`dh2_ai_enemy` once; it must not add a second copy of that code or fake candidate
selection. CMake/native integration is left to the parent after proof freeze.
