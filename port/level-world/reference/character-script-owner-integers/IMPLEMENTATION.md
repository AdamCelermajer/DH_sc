# Native Session integer integration

This is the subsequent approved implementation. `PROPOSAL.md`,
`integration-cases.json` and `inspection.json` preserve the earlier read-only
stage and its original source hashes; their not-implemented status remains
historical. No historical owner/int/runtime tests or reports were rewritten.

## Stable API

`ScriptNativeIntegerConfiguration32` contains the borrowed persistent service
context, genuine source32 identity mapper, imported fractional-number formatter,
and zero reserved word. New overload:

```cpp
ScriptOwner(character_identity, VM_memory_limit, native_integer_configuration);
bool integer_bindings(session_identity, const dh2_script_int_bindings*& out) const noexcept;
```

The original two-argument constructor preserves the provider-only registration
path. It intentionally owns no native integer map, so its historical missing
SetInt fixture remains valid as a boundary, not source namespace truth. The new
constructor selects native integer mode even with empty provider configuration.
Malformed reserved configuration throws `std::invalid_argument` before Session
allocation/publication. `ScriptSessionView` stays56 bytes and constructor field
projection stays72 bytes. The new accessor returns false/null for legacy or
unknown Session; a returned pointer expires with that Session.

Native mode constructs a map and stable40-byte receiver for every private
Session, independently of Character identity, active/pending status, factory
kind or scripted flag. Each Session copies the provider configuration; its
underlying borrowed context must survive that owner's VMs and finalizers.
Constructor allocation failure releases already-created wrappers and VM.
This extra opaque wrapper allocation is a native boundary; the original empty
embedded map did not allocate an RB node merely to initialize its header.

## Exact registration and override policy

The owner keeps all170 global service deliveries and the captured method null
gate. Stage1 index2 installs only genuine `dh2_script_int_set_callback`, then
delivers that SetInt descriptor. Index3 installs only genuine GetInt, then
delivers its descriptor. No bind-both convenience call is used.

The chosen ordering is **install before that same provider delivery**. A provider
sees the installed prefix, may override the exact closure at this delivery, and
may reject with a native diagnostic. Rejecting retains the already installed
prefix; this is explicit native failure behavior, not an invented rollback or
new original source exception claim. Rejecting index1 sees no SetInt; rejecting
index2 sees SetInt but no GetInt; rejecting index3 sees both. The first index2
delivery can genuinely execute SetInt while GetInt still remains nil.

Rebinding does not recreate/reset the Session map or receiver. The callbacks
borrow the stable receiver directly, rather than retain ephemeral request/view
objects or look a closing/erased Session up again. The existing provider-only
constructor path retains its old behavior exactly.

## Lifetime and scope

Source destructor evidence is original `0x37c004`, including the captured integer
clear `0x37c0bc..0x37c0e8` and `Instance` close at `0x37c100`. Session teardown now
clears loaded set/path, alias contents, integer contents, closes the VM, then
destroys alias and integer wrappers. Receiver/providers remain alive throughout
actual Lua finalizers; finalizers can insert/get genuine integers after the clear.

Missing source32 identity mapping or fractional `%f` formatting remains an
explicit protected binding error before map mutation. Ordinary string keys work
with empty services. No pointer truncation, shared map, guessed printf spelling,
or unsupported global callback is installed.

During ordinary VM execution, existing generic busy guards stay intact and the
dedicated source scoped call/Include capabilities remain unchanged. The frozen
runtime's `vm_destroy` does not set `busy` before `lua_close`; generic VM reentry
from a close finalizer is outside its provider contract and is **not** claimed
guarded. The close tests use only allowed direct native map/alias callbacks and
Lua's own calls. No core/runtime/int source was changed to alter that boundary.

The new test fixture explicitly resets its owner before destroying its external
provider/context members, because automatic derived/base destruction ordering
would otherwise expire those members before the base-owned VMs close. Actual
caller integration must preserve the same lifetime guarantee.

## Proof and reproduction

`reports/character-script-owner-integers-host-audit.json` binds the new isolated
owner DSO, executable and sources, **actual frozen central** world/runtime DSOs,
original constructor/destructor captures, integer instruction proof, and exact
source commons bytes. `dladdr` confirms the actual linked source lifecycle,
native integer kernels and Lua runtime execute.

The new native-mode test passes **6,979 checks**: simultaneous private owners,
same-key isolation, hash collision, missing-key insertion, explicit identity and
fraction provider calls, exact index2/3 visibility, repeated binding retaining
the map, four provider-failure prefixes, provider pass-through override, missing
providers, source pending replacement and fresh map, two real close finalizers,
all supported private Session kinds, external root/Init and ignored top-level
error prefixes, real nested Include, dedicated source-scoped Lua calls and three
generic busy guards, unknown/legacy accessors, malformed configuration, and binary
dispatch identities. Source map and alias wrappers remain alive through both
finalizers, and their contents are empty on entry before genuine reinsertion.

The same new owner DSO also executes the **unchanged** historical provider-only
test: **6,283 checks** pass. Its recorded hash equals the read-only inspection's
historical hash. Address/undefined/leak sanitizers report zero findings in both
executions. Existing kernel instruction corpus still binds the frozen integer
source:3,163 original/ARM64 cases,6,326 map snapshots and199 original erased nodes.

Run `tests/character_script_owner_integers_host.py`. It compiles only the new
isolated owner and NEW/unchanged legacy tests, linking existing central world and
runtime DSOs; it does not invoke CMake or rebuild those dependencies. New test
CLI takes the exact commons source path as its sole positional argument. Parent
central integration should add `character_script_owner_integers.cpp` as a NEW
host audit linked to `dh2_level_world`, `dh2_script_runtime` and `${CMAKE_DL_LIBS}`;
the owner source already belongs to the world production library. The new
audit source is `tests/character_script_owner_integers.cpp`; do not compile the
owner production cpp into that centrally linked test executable.

This is genuine host owner/VM/kernel composition with source-bound constituent
evidence, not a full owner original-instruction differential, complete game
namespace, package/device result, or live actor/manager ownership claim.
