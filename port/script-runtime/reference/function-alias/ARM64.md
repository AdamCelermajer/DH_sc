# Optimized ARM64 alias proof

`../../reports/script-function-alias-arm64-differential.json` is PASS: 4,371
original-instruction versus optimized ARM64 cases, zero mismatches. The previous
4,209-case corpus and host proof are preserved; 162 new cases exercise string
lengths 0, 1, 15, 16, 22, 23, 24, 127, 128, 129 and 1024, backup restoration,
empty-value erasure and ordered restoration of 20 keys.

The test executes actual original hash, lookup, membership, guarded producers,
Push and Pop bodies from the captured ARM32 ELF. It re-executes the original
against the preserved expected bytes before comparing the new ARM64 result.
It reuses only definitions from `probe_original.py`, avoiding its original
corpus/report writers. No host hash or alias algorithm substitutes for either
instruction kernel.

The native ELF executes NDK libc++ map insertion, balancing, lookup, erasure
and destruction instructions. The fixture checks both trees after each
non-hash case, including parent links, root color, red-child rules, equal black
height, unsigned in-order keys, minimum pointer and size. There are 4,108
complete main/backup/recording-state comparisons, 8,216 tree integrity checks,
317 ordered string-copy service comparisons and 84 long-string assignments.
All native object and fixture allocation pointers exceed 4 GiB.

Native `dh2_script_alias_bind` registers the real three private callbacks in
source order: AddToVFTable, PushVFTable, PopVFTable. The replay invokes those
actual callback pointers for all producer operations, totaling 377 calls,
and verifies zero returned Lua values. Three additional actual original
LuaScript::Call -> _GetFuncName -> Instance::pCall cases agree with the compiled
alias-aware wrapper: OnTimer resolves once to Renamed, unmapped Unmapped
retains caller pointer identity, and Renamed resolves once to Other.

## Explicit boundaries

Original container allocation/tree shape and string construction/copy are
services. ARM64 operator new/delete and the imported libc++ string assignment
primitives are services using the observed NDK short/long string object ABI;
the test compares exact copied bytes and ordered calls. It does not prove
allocator failure behavior, numerical allocation addresses or the original
container rebalancing algorithm. Native tree algorithms do execute.

Native VM binding registration and resolved-call execution are observed
service boundaries here. Complete Lua argument projection and actual VM
execution belong to the preserved host proof, including 33 genuine Lua DSO
integration checks. This standalone ARM64 proof does not establish packaged
APK execution, full VM instruction parity or deferred selected-script VM
initialization. The separate character-script-ownership discovery records
that the original privately owned Instance allocates a VM before deferred
binding; eager native runtime setup is not claimed to match that constructor
ordering by this alias proof.

## Reproduction and bindings

From the repository root, run
`port/script-runtime/reference/function-alias/build_arm64_oracle.ps1`, then
`port/script-runtime/tests/script_function_alias_differential.py` with the
configured Python/Unicorn dependencies. The build is NDK 29.0.14206865,
aarch64-linux-android24, C++17, O2, no fast math and FP contraction disabled.
The new report records compiler binary/version, full arguments, the key
libc++ map/tree/string header hashes, input source hashes, imported test
dependency hashes, build manifest, original capture and both gold identities.

Original ELF SHA-256:
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
Optimized oracle SHA-256:
`1c97444357cc3de3a62aeb4e4fd3ac5a86597e6785a114eacf5619d4719e9226`.
Preserved gold SHA-256:
`154c320b65b43180ece8dbdaa4115305c58f21ab6aa6b5f3522992b2ad1a892e`.
Extension gold SHA-256:
`2fab4c5eaa3589d47e36fec3fae0977470e7d6cee658f754bb454a9325d65ad0`.
Preserved host report SHA-256:
`6ceea1f2623f170ee1b1fd8e46872c1e47dd7cead61ac7ce8d6de3716f863771`.

The frozen alias header/source hashes remain respectively
`9d138d3aa92e96fd0d0dabddea721c7fc8470b7e64d5bf116c98865e7c84d868`
and `d6bc06c9ac09969b914f4d8025d2efea281eaf2bd91a9bb52dccc141512e3271`.
No existing runtime, game binding or CMake source was edited for this proof.
