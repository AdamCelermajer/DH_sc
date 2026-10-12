# Alias contents clear before VM close

The additive `dh2_script_alias_clear_contents(map)` clears backup first, then
main. It preserves the recording flag and the wrapper's identity/lifetime;
it neither closes a VM nor destroys an alias owner.

Actual LuaScript D1 at `0x37c004` (D2 `0x37bec0`) clears loaded-set/path,
backup+0x4c, main+0x34 and Arguments, then enters Instance destruction.
Backup clear is `0x37c068..0x37c088`, main `0x37c098..0x37c0b8`, and Instance
destruction is called at `0x37c100`. Neither destructor stores tracking byte
0x64. The captured manifest/assembly binds both original bodies.

`probe_arm64.py` executes 16 actual original destructor-to-Instance cases
versus optimized ARM64 clear instructions: empty and nonempty maps, recording
0/1, 0/1/8/32 main keys and repeated clears. Both maps are empty and tracking
is unchanged at original Instance-close service entry. The native tree-node
delete groups match original backup-before-main clear order; all allocations
except the live wrapper are freed. Original container allocation and Instance
close remain explicit services here; complete Lua close is not emulated as a
successful original VM proof.

`script_function_alias_teardown.cpp` instead executes genuine native Lua close.
Eight actual `newproxy` finalizers invoke native PopVFTable then AddToVFTable
from `__gc` after clear. Old backups do not resurrect OnTimer, the new GC alias
survives until wrapper destruction, and the callback context remains alive.
A separate Add/Pop check proves clear preserves recording. ASan/UBSan report
zero findings. Full cache/path/Arguments destructor ownership is not supplied
by this helper.

Reports, all newly named:

- `script-function-alias-teardown-host-audit.json`: preserved 4,209-case host
  replay plus 33 genuine Lua checks, eight GC cases and 187 teardown checks.
- `script-function-alias-teardown-arm64-regression.json`: 4,371 preserved and
  extended alias cases pass on the updated O2 oracle.
- `script-function-alias-teardown-arm64-differential.json`: 16 original-bound
  clear/ordering/tracking cases pass on that same oracle.

Updated oracle SHA-256:
`df71965b493727689d4dc3936c45dcdf4079ee7946c2a32f72a733dbd611c2e6`.
Historical alias reports/gold and previous oracle were not overwritten. Build
the existing standalone builder with output `oracle-teardown.so`, replay the
existing differential tool with a NEW report path, then run `probe_arm64.py`.
`run_host_audit.py` builds only isolated current alias/test executables against
the frozen preceding sanitized VM DSO. These are source/host/instruction
proofs, not packaged APK or full manager teardown parity.
