# Source target assignment and Lua scalar bindings

Original ELF SHA256: `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
The adjacent manifest binds fourteen complete captured routines and their bytes.
New production files are `character_target_bindings.hpp/.cpp`; existing search,
relationship, target provider, Lua runtime and renderer files are unchanged.

## Actual setter

`CharAI::AI_SetTarget` at `0x3d6890` always writes candidate `AI+0x3c` first.
Nonzero mode only writes current target `AI+0x40` and returns. False mode:

1. If incoming differs from the initial current target, zero the **16-bit**
   `Character+0x14d0` through the initial live owner `AI+4`.
2. Call DebugSwitches.load and query `IsTracingCharAITarget`. If its raw return
   is nonzero and the reloaded target differs from captured incoming, perform
   a second load/query of the distinct lowercase `isTracingCharAITarget`.
   All three changed-nullness branches have that same second lookup. Strings,
   allocator and debug storage remain explicit services; they are not target
   acceptance predicates. This body has no gameplay target-notification call.
3. Write captured incoming to current target, including after nested setters.
   Null then returns without modifying last target, changed, alive or sight.
4. Nonnull calls `Character::GetCharAIId` (`0x3a2fec`) on the reloaded owner;
   the result is discarded. This call is nevertheless preserved.
5. Reload current target and last target. A difference zeros `AI+0x4c` and
   writes current to last `AI+0x44`; equality preserves the changed byte.
6. Call current target's virtual `+0x34` IsDead, then store low byte of raw
   result XOR 1 in `AI+0x48`. This is not Boolean normalization.
7. Reload target after IsDead and call `AI_IsInSight` (`0x3d4ed8`); store low
   byte of its raw result in `AI+0x49`.

`SyncLastTarget` at `0x3d49c4` copies current to last only. Character's
`_ClearTarget` at `0x3b5690` calls setter(null,false), then SyncLastTarget;
it also leaves the other three bytes untouched. No new target-update event
or blanket reset has been inserted. Existing separate target-update/event
delivery ownership is outside this module.

The port uses borrowed `TargetState48` and `TargetOwner16` views. Caller must
project original fields from the same genuine actor session, retain view and
backing lifetimes through synchronous calls, and apply mutations to those
live views. Identity values remain opaque and are never dereferenced by this
module. Debug, dead virtual and sight services deliver raw words. Nonzero
provider status reports a failed executed prefix, not an atomic rollback.
Malformed aligned/reserved entry checks reject before effects. A nonnull
unaligned owner is rejected before dereference. Mode-only paths may omit the
owner/services because the original never reads them.

## Native getter and sight backend

`GetCharAIId` reads signed cached property word 1 (`Character+0xffc`), accepts
nonnegative values below signed global AI count, otherwise returns 8. The
public helper supports the genuine bounded decoded-table domain (count
0..65536), with no property recomputation or fabricated row.

`AI_IsInSight` resolves a null argument from live current target; null remains
false. It calls owner then target `GetTargetPosition` (`0x3935dc`) and reads
coordinates after both calls. That method chooses cached point `+0x184` only
when node `+0x180` exists and byte `+0x80` is nonzero; otherwise game `+0x160`.
It obtains actual AI row's ViewRadius at runtime offset `+0x3c` via cached AI
ID. Exact predicate is `radius*radius > ((dx*dx+dy*dy)+dz*dz)` (strict).
Native sight arithmetic permits all IEEE values. Original corpus executes
these complete bodies and decodes all 76 actual AI rows with original stream
loader `0x506f3c`. Host uses existing native `load_ai` on the same six cache
blobs and checks all 76 radius words, plus genuine native IsDead provider.
Borrowed position and debug adapters in fixtures are not a world registry.

## Actual Lua wrappers and remaining object boundary

* `_HasTarget` `0x3b6f50`: ignores argument count/kinds, reads owner `+0x408`,
  pushes one Boolean for pointer nonnull through `0x37c7e4`.
* `_SetTarget` `0x3b8f38`: requires at least one argument. First source Value
  type 2 **or** 7 is accepted, including null identity; its `getUserData`
  (`0x31b5a0`) result is passed directly to false-mode setter. Other kinds
  silently do nothing. It neither resolves a handle nor performs Character
  RTTI in this wrapper. Additional arguments are ignored. Native wrapper
  keeps those exact guards and returns zero values.
* `_ClearTarget` ignores arguments and returns zero values.
* `_GetTarget` `0x3b6c7c` passes raw target identity to
  `ReturnValues::pushUserData` `0x37c9f8`. Its projection entry is instruction
  tested. The complete UserData Value/VM/metatable/lifetime producer remains
  unreconstructed and **GetTarget is not installed**. The native identity
  getter is a native snapshot only, never a replacement Lua lightuserdata.

The task lead address `0x386f28` is precisely
`sfc::script::lua::Arguments::pushUserData`, not a GameObject constructor.
Both captured UserData append methods construct a Value through `0x37c978`
and append through `0x3195c0`. Full object representation and subsequent
methods (`target:GetState`, position, etc.) need that separate backend.
Input `_this` table projection is performed by existing genuine source-value
runtime and its borrowed caller identity service. This module introduces no
successful cast or nearest-enemy fallback.

Set/Clear installation uses existing scoped discarded-call capability. The
ephemeral scope exists only during that callback and restores after nested
calls. The host reentry test's debug-provider Lua callback is an adversarial
provider fixture, not a claim that original DebugSwitches dispatches Lua.
No generic same-VM permission or retained scope is granted.

## Evidence and reproduction

Optimized ARM64 versus original: **3,955 cases / 21,946 ordered services**,
zero mismatches. CTB1 gold binds 47 input words per case, actual 76 radius
words, source result words, nine live state words and eleven-word callback
snapshots. Fixtures cover mode 0/1/255, all target/last equality and null
branches, case-sensitive debug branches, first Value types 0..8 and arities
0/1/3, all 64 live mutation masks, five nested setter modes, raw dead bytes,
cached-point selection, negative/out-of-range AI IDs, strict radius edges,
NaN/Inf/signed zero and randomized coordinate words. Arithmetic sight result
is a Boolean; IEEE coordinates are copied as exact words.

Sanitized host: same **3,955 / 21,946**, **502,443 checks**, eight actual Lua
checks and six malformed/provider guard cases; ASan/UBSan/LSan zero findings.
The isolated target DSO links the actual stable central world, game-data and
Lua runtime DSOs; before/after dependency hashes and dladdr/ldd are recorded.
This evidence is not a central compiler provenance, packaged APK, full
GameObject Lua representation or whole world registry claim.

```
& port/level-world/tools/build_character_target_bindings_oracle.ps1
python port/level-world/tests/character_target_bindings_differential.py
python port/level-world/tests/character_target_bindings_host.py
```

For parent integration: add new cpp to world sources, and a host target from
`tests/character_target_bindings.cpp` linked to actual world/game-data/runtime
and dl. Executable arguments are `target-original-gold.bin` and assets/data
directory. After that target exists, without rebuild:

```
python port/level-world/tests/character_target_bindings_host.py --main-linked /home/adampalace/dh2-world-build/character_target_bindings_audit
```

That writes a **separate** main-linked report. Existing standalone proof is
retained; compiler provenance for central modules remains caller-owned.
