# Source LuaScript function aliases

This module recovers the per-LuaScript VFTable name resolver and its three Lua
producers. It does not allocate/publish selected AIS instances or reconstruct
the complete LuaManager. Existing runtime, game bindings and CMake were not
edited. The new `script_function_alias.h/.cpp` is ready for owner integration.

Original ELF SHA-256:
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
The focused manifest identifies 15 original functions by byte hash and address.

| Source function | Address | Relevant behavior |
| --- | --- | --- |
| LuaScript constructor | 0x37c584 | Calls Instance constructor on this+4; main and backup trees empty, tracking byte64=0. Nonzero constructor bool suppresses BindFunction; zero calls it. |
| glitch::core::hashString | 0x37c164 | Signed-byte, modulo32 hash loop over the first-NUL string. |
| LuaScript::_GetFuncName | 0x37c314 | Unsigned hash-only tree lookup; hit returns mapped string pointer, miss returns original caller pointer. |
| LuaScript::IsInVFTable | 0x37c2a0 | Same hash-only lookup, returns membership rather than testing string contents. |
| LuaScript::_PushVFTable | 0x37be00 | Clears backup tree, then enables recording. |
| LuaScript::_AddToVFTable | 0x37ec70 | Two exact projected string Values required; otherwise silent zero-result return. Records first old value, then copies replacement. |
| LuaScript::_PopVFTable | 0x37dc44 | Ascending-key backup traversal, restore nonempty previous value or erase empty, clear backup, disable recording. |
| LuaScript::Call | 0x37c390 | Resolve name, then call this+4 Instance with resolved pointer; no recursive mapping. |
| Instance::pCall(name) | 0x31abe8 | Fresh lua_getfield on globals(-10002) for each call. No cached closure. |
| LuaScript::BindFunction | 0x37b5a0 | Registers AddToVFTable at37b660, PushVFTable at37b67c, PopVFTable at37b698. |
| LuaScript::Load | 0x37b574 | Passes explicit LuaScript identity to LuaManager::AddFile. |
| LuaManager::AddFile | 0x37b23c | Uses explicit script's Instance+4 and loaded-file/path state. No direct alias-tree write in this body; executed Lua can call the producers. |

## Hash, copies and identity

Starting h=0, each **signed** nonzero byte c executes
`h ^= uint32(c + 0x9e3779b9 + (h << 6) + (h >> 2))`.
High bytes are sign-extended as in original `ldrsb` at37c1dc; the shift-right
of h is logical. Embedded NUL ends the input; an empty name has hash0.
There is no case folding, full-name comparison or chained resolution.

The original-instruction corpus finds and executes an actual collision:
`Alias16038` and `Alias16275` both hash to3315024447. Adding either name changes
the same entry observed through the other. This behavior is preserved.

Main tree is LuaScript+34, root+38/count+44. Backup is+4c, root+50/count+5c.
Only the hash key is retained; the requested original name is not copied into
the map. Replacement strings are owned copies, truncated at their first NUL.
An unmapped lookup returns the **exact requested pointer**. A mapped lookup
returns that entry's owned buffer, including a mapped empty string. Contains
therefore distinguishes absent from present-empty. Unrelated insertions retain
the mapped pointer; modifying/erasing its entry invalidates the borrowed result.

Native std::map/std::string implement these logical ownership and ordering
semantics. ARM32 allocator addresses, inline string capacity, red-black node
layout and allocation-failure behavior are explicit boundaries, not parity
claims. Native allocation is outside the VM memory budget.

## One backup, not a stack

Push clears any previous backup even when already recording. On the first
Add of a hash while recording, source creates the backup entry and main entry
if needed, then copies the old main value into backup before assigning the new
replacement. Repeated Add of the same hash keeps the first prior value.

Pop visits unsigned keys in ascending order. Nonempty backup values are copied
back into main. Empty backup values cause main erasure, even if an empty main
entry existed before Push. It then clears backup and turns recording off.
A second Push loses the earlier backup; there is no nested restoration stack.
Push/Pop ignore arguments and return no Lua values. Add ignores extra values,
but source argument projection still runs for them before the callback body,
including normal table._this metamethod effects.

## Native API and call integration

Create one `dh2_script_aliases` per source LuaScript identity, independently of
the Lua_State allocation policy recovered by the ownership worker. Destroy it
after its VM bindings cease use. `dh2_script_alias_bind(vm,aliases)` installs
the three exact names using the already verified source-values binder; it does
not install timers, fake game callbacks or create a script manager.

`dh2_script_alias_call_discard_source(vm,aliases,requested,args,count)` resolves
once and invokes the existing source discarded-return path. Subsequent calls
observe both current aliases and current global function definitions. A Lua
body can synchronously change aliases through the bound producers; this does
not re-enter the public VM call API. Same-VM nested C ABI calls remain unsupported
under the existing runtime contract. Existing argument-count/input validation,
protected error handling and output projection are inherited port policies.
The resolver itself has no invented name/string-size cap.

`dh2_script_alias_add_values` exposes the producer guard on already projected
Values. Native null caller pointers are rejected; original invalid raw pointers
are outside the recovered domain. The binder's borrowed map must remain alive.
Binding allocation errors can follow earlier installation; no transactional
rollback is claimed. No existing timer adapter was modified. An alias-aware
OnTimer caller should resolve literal OnTimer before the source VM call while
preserving the separately verified signed32-to-float timer argument conversion.

## Commons and broader scripts

The exact AI commons input is13535 bytes, SHA-256
`20d34968e9983e14c24223085ab40d55d47f9387dfdbbf3ff7e6b39aea91252c`.
Its source contains none of the three producer names, and actual execution in
a fresh native VM with genuine alias bindings leaves OnTimer unmapped. The
original constructor probe independently confirms the initial maps are empty.
This establishes the commons-only fresh-script case, not every manager load or
selected instance's later state.

`inventory_cache.py` reads the supplied hash-identified cache without modifying
it. Of219 scripts,31 contain VFTable producer names. For example
`ai/generic_boss_core.luac` maps OnInitFinal to myBoss_OnInit and OnUpdate to
myBoss_OnUpdate. The inventory is textual evidence, not executed branch proof
or an exhaustive analysis of dynamically constructed names.

## Verification and reproduction

`probe_original.py` executes **4209 original ARM32 cases**: constructor defaults,
263 actual signed-byte hash cases,1882 name lookups plus membership checks,
producer type/count guards, repeated overwrites, colliding keys, empty prior
erasure, Push replacement, Pop restoration and randomized mutation histories.
Original hash/lookup/producer instruction bodies execute unchanged; temporary
string construction, native tree allocation/erase/clear, strlen and lazy guard
are explicit services. The original Pop iterator and copy order execute; the
provided balanced tree does not claim original allocator/rebalancing parity.
Three additional actual Call→resolver→Instance cases confirm fresh globals
lookup, caller pointer identity on miss and one-pass mapping.

`alias-reference.bin` SHA-256:
`154c320b65b43180ece8dbdaa4115305c58f21ab6aa6b5f3522992b2ad1a892e`.
`tests/script_function_alias.cpp` replays all4209 cases against native64 code
and runs33 genuine Lua DSO integration checks. These include copied strings,
stable unrelated-entry identity, a real hash collision, no recursive alias,
global function replacement, zero-result wrong types, embedded NUL, ignored
table-argument metamethod side effects, aliases modified inside a called Lua
function and exact commons execution with timer globals still absent.

`run_host_audit.py` builds only the new module/test atO1 with ASan/UBSan and links
the frozen, source-bound `dh2_script_runtime` DSO. Report:
`../../reports/script-function-alias-host-audit.json`, **PASS**, zero mismatches
and sanitizer diagnostics. It binds executable/library hashes, final source,
original/gold/capture identities and the runtime's preceding audited source.
No CMake, Android/APK/device or original full-VM proof is claimed.

From the repository root, use the configured Python with the existing
Unicorn/ELF dependencies to run `probe_original.py`, then `inventory_cache.py`.
Run `run_host_audit.py` with WSL access; its default frozen VM DSO is
`/home/adampalace/dh2-lua514-build/libdh2_script_runtime.so`. The resulting native
test lives at `/home/adampalace/dh2-script-alias-audit/script_function_alias_audit`
and also under `.local-inputs/script-function-alias/` with its linked VM copy.
