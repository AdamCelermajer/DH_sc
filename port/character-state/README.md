# Owned character property state and original writes

Source C owns four 224-integer sheets: base, saved, equipment and final. Initialization
resets each to the checked serialized defaults. This is authored portable sheet
ownership; it does not reconstruct the complete original Character constructor.
The module uses [property records](../character-properties/README.md) and
[stat composition](../property-composition/README.md).

## Preserved native operations

`PROPS_Set` and `PROPS_Add` check type bit 32 before bit 8. Bit 32 writes/adds
to the saved sheet and recalculates final. Bit 8 writes/adds directly to final.
Other types retain their state. Add uses ordinary low-32-bit integer wrap in
these operations; it differs from the sentinel-aware composition helper.
`SetInt` and `AddInt` first shift the incoming integer left eight bits, retaining
low-32-bit wrap. `GetInt(false)` reads final with an arithmetic eight-bit shift.

The source wrapper stages the state, validates borrowed buff groups and commits
only after success. Counts/indexes/table errors and state/input aliases reject
without changing output. Buff groups retain composition's 32/64/1,024 bounds.

## Original comparison

**3,352** original ARM32/compiled ARM64/host comparisons pass: 896 actual-type
writes, 780 synthetic flag writes and 1,676 current-final integer reads. Actual
original Set/Add/Int/GetInt bodies, recomposition and property helpers execute.
All four sheets match exactly after every write; original reserved fields,
output guards and stack are preserved. All 224 cache fields are covered, with
additional type combinations and integer extremes.
[Comparison evidence](arm-differential-validation.json).

The routing comparison uses empty original buff containers. Buff-inclusive
write routing is unverified here; the separate composition check exercises
actual original tree/deque traversal. Cache reads, allocation/disposal and libc
copy/compare dependencies are explicit models. Source ARM64 executes in Unicorn,
without hardware or Android app execution.

Strict ASan/UBSan passes **12,000** valid/damaged-input iterations including
wrapped writes, invalid indexes/operations/counts, state/buff aliases and corrupt
table metadata. [Host build/safety](host-build-validation.json),
[16 KiB ARM64 build](android-build-validation.json).

## Remaining work

Original Lua argument/coercion/return behavior, entity ownership, derived base-stat
calculations, buff management, APK wiring and full source-built gameplay remain
unfinished. Original default/global sheet access is not exposed by this component.
The [rights statement](../../RIGHTS.md) applies to recovered metadata.

## Reproduce

```sh
python3 port/character-state/build.py --host --report /path/to/host.json
python3 port/character-state/build.py --ndk /path/to/ndk --report /path/to/android.json
python3 port/character-state/tests/differential.py --help
```

The differential test needs Unicorn/pyelftools, the pinned original library,
owner cache, arithmetic oracle and both compiled source modules.
