# Source design-table script callbacks

New `script_design_bindings.h/.c` implements the actual Lua wrappers GetPyCst
(0x37f354), GetPyStruct (0x37f4a8) and GetPyOID (0x37f5fc). All require at least
two exact string Values; failed guards return zero values. Extra arguments are
ignored by the wrapper after source Value projection. Numeric names are not
coerced. GetPyStruct and GetPyOID both dispatch PyDataArrays::GetOID (0x4bd640);
GetPyCst dispatches PyDataConstants::getConstant (0x4c4bdc). Results use source
signed32 pushInteger, projected to float32, including -1 or zero on lookup miss.

Services24 supplies a persistent context and design lookup callback. It returns
the actual source signed value; delivery failure is separate from an authored
miss. Manager lookup, registered getter ownership, design reload lifetime and
other Application tables remain explicit native service boundaries. The module
does not invent success for absent providers. Generic source-value VM binding
still has a 16-argument bridge boundary; direct wrappers accept full uint32
arity. No live game namespace or complete manager is established by this module.

`differential.json` binds the original ELF, focused original functions and
optimized NDK ARM64 binary: 1,200 cases, 60 ordered lookup calls, zero
mismatches. Original wrapper instructions execute; manager lookup and return
allocation are explicit services. Cases cover counts through33, all first/second
types, zero-result guards, shared Struct/OID dispatch and signed extreme values.

`registry-probe.json` executes the actual PyDataArrays C1 registration call
sites, with called construction/allocation helpers explicitly skipped. It
captures all142 registrations for138 effective names. Four names repeat;
registerClassByName assigns map[name], so later assignments determine the
effective getter. This is registration producer evidence, not map construction.
The actual CharacterProperties registration supplies GetMemberIDByString at
0x4af110, separate from the Arrays CharacterTable getter.

The actual member getter executes 228 queries against the224 property strings
from the bundled cache field file: every field, missing/empty/case mismatch and
first-NUL behavior. This fixture borrows the post-loader static std::string
array; it does not execute its original file/allocation producer.

`../../reports/central-script-bridge-host-audit.json` binds the main native Lua
library and this module's actual VM audit: the same1,200-case gold replay,
457 actual VM checks, nine native boundary checks, all224 real Character fields,
extra argument table metamethod projection and protected provider failure.
The test borrows genuine decoded CharacterTable field names. Other Application
design tables and their factories/reload services remain outside this proof.
The same central stage also passes scoped calls and owned nested Include.

Original ELF SHA256:
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
This work is newer than the tested 0df1 APK. No Android/physical gameplay claim
is made for it. Original manager map traversal and constant-file loading are
captured dependencies to integrate next.
