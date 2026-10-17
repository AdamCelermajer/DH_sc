# Character AI classification and cached getters

This unit reconstructs 17 complete bounded original source bodies. They were
previously recorded as dependencies of acquisition, relations, sight, range
and zoning. Their addresses are reused evidence; they are not 17 new addresses.

`GetCharAIId` reads cached `Character+ffc` before the signed global count. A
negative ID skips the count read and returns 8. IDs not less than the current
count also return 8. `GetCharAIFactionId` uses `+ff8` and fallback 10. Neither
getter clamps the signed count or tests whether fallback rows exist.

`GetCharAI` captures the global row-array base before calling `GetCharAIId`.
`GetCharType` reads the selected row's `+38` only after that call. Native table
capacity is a port memory bound; an original unsafe out-of-capacity access is
reported as invalid source facts, rather than treated as successful parity.

The type predicates match Monster=4, Follower=2, Faerie=3, Summoned=5,
Merchant=7, InvisibleMan=9 and Cleaner=8. `IsNPC` first asks for type6, then
independently calls Merchant, then Cleaner: the three type lookups are fresh,
with the original short-circuit order. Boss, MiniBoss and Sitting read bits
2, 1 and 3 of the selected row's `+14` flags.

`IsPlayer` has a significant type0 branch: it captures the `Character+44`
name, calls `strstr(name,"PlayerCharacter")`, and tests whether the returned
pointer equals the captured start pointer. A match later in the name is false.
Type1 is true; other nonzero types are false. `IsDead` returns the raw byte at
`+1449`, without canonicalizing it or interpreting HP/lifecycle/state-machine
fields as that byte.

## Verification and limits

The runner checks original ELF/symbol/body hashes, executes original ARM
instructions, and compares compiled native results and ordered table/count/
name-import boundaries. It covers fallback values, signed counts and IDs,
every type branch, flag bits, name-prefix cases, raw dead bytes, fresh NPC
lookups, captured-table/cached-ID/name mutations and dependency failures.
All executable caller instructions must be covered; literal pools are excluded.
The libc `strstr` import is an exact byte-string/address model; its implementation
is not counted as reconstructed source. Host guard tests cover missing
providers, unsafe backing, malformed aliases and identities above 32 bits.

Services borrow real owner/table/count/name storage through synchronous return.
They may expose fresh values at a source read but may not invalidate captured
storage, change the stable Character identity, or overwrite results/services.
Errors preserve completed effects. Full Character construction, ObjectBase
handles, current-AI frame owners and native gameplay integration are separate
work; this source unit alone does not establish autonomous enemies.

Run:

```powershell
python port/level-world/tests/run_character_ai_classification_host.py --compiler C:/Users/noamc/.local/mingw/mingw64/bin/g++.exe --original-elf <original-libDungeonHunter2.so>
```
