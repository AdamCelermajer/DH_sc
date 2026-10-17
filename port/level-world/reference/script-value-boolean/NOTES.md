# Source Value boolean conversion

## Reconstructed body and original evidence

`sfc::script::lua::Value::getBool() const` is the complete 172-byte ARM body at `0x31bc80`. The original ELF hash and five symbol/range hashes are in `original-functions.json`. The runner executes all 43 original instructions and verifies the actual relocated `0x30df8c` import is `__aeabi_fcmpeq` before supplying its numeric result. Only that floating comparison and four explicitly named Lua dependencies are modeled in the original caller oracle.

The logical `Value` view is not a native or ARM memory overlay. Its fields correspond to original type word `+4`, binary32 bits `+8`, C-string pointer `+0x20`, and object identity `+0x6c`.

| Captured source tag | Original result/actions |
| --- | --- |
| 1, 3 | Compare binary32 number to positive zero; negate equality. Both zero signs are false; nonzero, infinity, subnormal, and every NaN word are true. |
| 2, 7 | Test object identity at `+0x6c` for nonzero. |
| 4 | `luaL_newstate`; fresh string pointer read; `lua_pushstring`; `lua_toboolean(-1)`; capture canonical boolean; `lua_close`; return captured boolean. |
| 0, all other words | Return false without Lua calls. |

The string branch creates a temporary state. It does **not** fetch a registry reference from an existing VM. Exact ELF symbol identification resolves `0x84c7e0`, `0x84c04c`, `0x84b320`, and `0x85797c` to newstate, pushstring, toboolean, and close respectively. Those callees are dependency evidence, not newly reconstructed bodies here.

## Services and ownership

`script_value_boolean::execute` captures the original Value identity/type and the port service binding/context once. `new_state` may change the live Value string/type; the string is reread after its normal return while the initial type controls the branch. The temporary Lua identity is captured. The boolean is captured before close, so a close provider changing the Value or returning arbitrary register data cannot change the result.

The single owning thread retains Value controls, callback context, and old/new string backing through synchronous return. Same-Value/output destruction or reentry is unsupported; independent resources can nest. Native controls must be aligned and disjoint. Preflight rejection leaves output unchanged. Native pointer-width extensions are tested separately and are not claimed as ARM32 identity cases.

Missing callbacks, nonzero port results, and exceptions stop at the entered phase without added close or rollback. Prior effects and created Lua resources remain provider-owned. A null newstate is rejected before the original would use a null Lua state. This is an explicit invalid-source boundary, not a recovered null guard. Source Lua void returns are discarded; providers must distinguish their own port failures.

## Reused real Lua adapter

`script_value_boolean_lua::TemporaryLua` owns real temporary states from the existing attributed float32 Lua 5.1.4 sources in `port/adam-script-runtime/lua`. It opens no libraries and executes the actual four named C API operations. Real `lua_pushstring(nullptr)` pushes nil and yields false; every nonnull C string, including empty, `"0"`, `"false"`, and strings beginning with NUL, yields true. The host gate also exercises a 256 KiB string, repeated allocation/close, fresh string mutation, invalid handles/index, and failures after actual effects.

The native adapter protects push with `lua_cpcall` to report allocation errors as port failures. Its internal registry slot preserves the actual pushed string/nil across the protected C frame, then restores it on the outer stack. This protected scaffolding and host error reporting are deliberately scoped native adaptation, not parity with the original unprotected Lua panic or allocator. Failed call resources remain in the owning registry. Adapter destruction closes them as outer teardown; source execution never adds per-call failure cleanup. Allocation/close are real operations, not successful no-ops. Ownership metadata is reserved before creating a state so a C++ allocation error cannot orphan a newly created resource.

The adapter is isolated from production Session/CMake. An eventual integration must reuse the existing Lua image and retain this adapter until all calls complete; it must not link a duplicate Lua implementation. No whole-VM equivalence or APK integration is claimed.

## Verification and accounting

The runner compiles both the caller and a separate real Lua adapter executable, checks exact source/ELF hashes, and compares the caller with original ARM execution. Controlled raw Lua boolean fixtures (including noncanonical words) prove caller normalization and call order; they do not claim real Lua would return those noncanonical words. Provider-binding mutation bits test native captured binding behavior; original fixed callee addresses stay unchanged. Original instruction coverage is collected per instruction, not inferred from blocks containing intercepted callees.

Accounting: **one complete source caller body**, **zero new dependency bodies**, one real reused Lua dependency adapter, no complete Lua callback/VM or native app wiring claim. The original skill-check callers can consume this provider once their actual Value/ReturnValues storage and Lua Call adapters are implemented.
