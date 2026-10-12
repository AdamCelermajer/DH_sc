# GameObject spatial Lua getters

These captures and native kernels use original ELF SHA-256
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
The adjacent manifests bind the original function bytes and disassembly.

| Original function | Address | Recovered behavior |
| --- | --- | --- |
| GameObject::_GetPosition | `0x38e700` | Push owner fields `+0x160`, `+0x164`, `+0x168` as three floats; ignore arguments. |
| GameObject::_GetDistanceFrom | `0x393444` | Require at least one argument of exact source type 4 or 7; otherwise return zero values. |
| GameObject::_GetDistanceBetween | `0x391438` | Require two exact source type-4 strings; otherwise return zero values. |
| Arguments::operator[] | `0x37baf8` | Actual bounded argument accessor executes in the original corpus. |
| Value::getString | `0x31c49c` | Actual string branch executes after the wrapper's exact type guard. |
| Value::getUserData | `0x31b5a0` | Reads `+0x6c` for types 2 and 7; From admits only type 7 before calling this helper. |

`GetPosition` exposes raw game coordinates, without physical-unit conversion or
floor/mesh-foot adjustment. `GetDistanceFrom` computes target minus owner;
`GetDistanceBetween` computes first resolved object minus second resolved object.
Both return `(dx*dx + dy*dy) + dz*dz`, in float32, with no square root. Accepted
lookups that miss return one float `-1`; rejected argument types/arity return no
values. A raw lightuserdata argument is rejected by From even though the deeper
Value helper would accept type 2.

The string path calls ObjectManager::GetObjectByName at `0x34aca0` with
`name, -1, false, null`, then the GameObject carrier conversion at `0x33fee4`.
Between resolves and converts its first argument at `0x391534/0x39153c`, then
resolves and converts the second at `0x3915b4/0x3915bc`, even when the first is
missing. Only after both calls does it test pointers and read coordinates at
`0x3915d8` onward. Synchronous lookup can therefore change either object's
coordinates before arithmetic. From likewise reads live owner coordinates after
its target resolution. The table path calls Value::getUserData and uses that
GameObject pointer directly; it does not execute the string carrier cast.

The native `SpatialBindings40` receiver borrows owner xyz, context, a named-object
resolver and a native64 userdata resolver. Named resolution combines the manager
lookup and carrier conversion into one explicit service. Null delivered position
is a genuine lookup miss. Nonzero service status is a native provider failure,
reported through protected Lua error handling. Source services and returned xyz
arrays must survive the entire invocation, including the second lookup; their
coordinates may mutate synchronously. Native opaque identity is `uintptr_t` and
the corpus checks `0xfedcba9800001000`, without ARM32 pointer truncation.

The source-values VM adapter projects every argument before invoking the wrapper,
including ignored arguments. A table's `_this` lookup may execute `__index` and
produce type 7; raw function/thread/full-userdata projection follows the existing
runtime contract. Its existing limit of 16 arguments is an explicit unsupported
native bridge boundary; it is not claimed as original behavior. No general VM
reentry, SetPosition, object-manager ownership or complete world lookup is added.
`dh2_character_spatial_bind` is a convenience installer for the three genuine
callbacks; a full source registration catalog remains the owner's responsibility.
Borrowed receiver/services must also remain alive through VM finalizers.

`reports/character-spatial-bindings-arm64-differential.json` is PASS for 2,746
original versus NDK 29 optimized ARM64 cases: 850 Position, 914 From and 982
Between. It compares 3,861 logical lookup/cast services and 624 cases with actual
resolver mutations, return arity, coordinate state and output words. The original
wrappers, Arguments accessor and Value helpers execute. Original ReturnValues
pushNumber, manager/cast services and imported AEABI arithmetic remain declared
boundaries. Native arithmetic executes in the ARM64 instructions. Copied Position
words, including signaling NaN payloads, compare exactly. Arithmetic NaNs compare
by NaN class; every finite/infinite/signed-zero word compares exactly without
tolerance.

The immutable source gold is `spatial-original-gold.bin`, SHA-256
`b4d945f4f15ea36cf668b286f183d6b1acdddc2cbe1638d9bf30cebd269f4f83`.
The accompanying JSON is SHA-256
`e49335434e5365359c308d341ba43e3c069ca078ffa9b35c263b38f825045e61`.
The binary stores arguments, initial/replacement coordinates, source returns,
final coordinates and ordered service traces, rather than recomputing host gold.

`reports/character-spatial-bindings-host-audit.json` is PASS for the same 2,746
fixtures and 3,861 services, plus 25 genuine float32 Lua VM checks and 16 native
malformed/capacity/unsupported guards. It verifies missing first objects still
resolve second objects, mutation-before-read, native64 table identity, source
lightuserdata rejection, embedded NUL strings, ignored-argument projection side
effects and protected provider errors. ASan, UBSan and leak detection report zero
findings. The isolated spatial DSO and test executable link the actual central
runtime DSO; `dladdr`, `ldd`, before/after dependency hashes and all source hashes
are recorded. The central runtime was not rebuilt.

Reproduce the host replay with:

```text
python port/level-world/tests/character_spatial_bindings_host.py
```

The host executable takes only the gold binary path. It is suitable for a later
main-linked CMake audit; this task changes no shared CMake, owner, renderer, APK or
device state. Full-manager lookup, live NPC ownership and packaged instruction
parity remain separate integration proofs.
