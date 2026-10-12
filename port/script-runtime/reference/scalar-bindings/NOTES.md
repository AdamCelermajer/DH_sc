# Source fixed-point and bit callbacks

Frozen new module: `script_scalar_bindings.h/.c`. No shared runtime/owner or
renderer changes are required by this module. Parent subsequently added it to
the main runtime target; the proofs here bind the independent genuine VM-linked
host DSO and the optimized standalone ARM64 oracle, not an APK.

Original ELF SHA-256:
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
`original-functions.json` and `reference/original-functions.asm` capture eight
callbacks, Value.getNumber, Arguments indexing and two ordered return helpers.

| Lua name | ARM address | Source guard and arithmetic |
|---|---|---|
| ToFixed | 37ebc4 | At least1 argument; getNumber→signed32 conversion→wrapping LSL8→integer return |
| FromFixed | 37ee84 | At least1; TWO returns: converted signed32 ASR8 first; reload/getNumber/convert again, signed32-to-float times1/256 second |
| MulFixed | 37e1a4 | At least2; getNumber for both, convert to signed32, low32 MUL then ASR8 |
| DivFixed | 37e068 | At least2; converted numerator divided by converted denominator ASR8; truncation toward zero |
| BitNot | 37f814 | Exactly1, source type3 number; convert then NOT |
| BitAnd | 37e9ec | At least2; inspect ALL types before any conversion, require type3, reduce every argument |
| BitOr | 37e814 | Same all-number guard; reduce every argument |
| BitXOr | 37f750 | Exactly2, both type3; convert then XOR |

Guard failures return zero Lua values. MulFixed/DivFixed's first-type3 branch
does an unused Arguments[1] lookup; it does not reject nonnumber arguments.
FromFixed's first return controls ordinary Lua single-result assignment in the
actual monster top-level expression. Replacing the callback with only the
fractional return changes the authored skill-tree ID.

Value.getNumber31bbf0 uses float payload for boolean/number, unsigned source32
identity conversion for kinds2/7, zero for other kinds, and a fresh empty Lua VM
for strings: pushstring→tonumber→close. Native uses genuine float32 Lua and the
first-NUL text boundary. Its protected temporary allocation failure is explicit;
this does not claim original unprotected panic behavior.

All five arithmetic helpers are SHN_UNDEF in the supplied ELF; see
`undefined-arithmetic-imports.json`. The instruction oracle declares, rather
than reconstructs absent Android helper bytes: signed float conversion truncates
to zero, saturates out of range/Infinity and maps NaN0; integer-to-float uses
nearest-even; integer division truncates0, MIN/-1 wraps. Division by zero is a
required borrowed service when encountered. The corpus intentionally returns
0x13579bdf to prove that service result is propagated, without asserting this
as the original platform's division-zero policy. Source identity conversion is
also a borrowed mapping; native64 pointers are never truncated to source32.

`dh2_script_scalar_bind(vm,services_or_NULL)` installs all eight real callbacks.
Services32 contains context, identity mapper, division-zero handler, reserved0.
NULL permits normal arithmetic and strings; missing identity/zero-divisor
services raise a protected error when required. Borrowed services/context must
outlive bound callbacks and must not throw/reenter the VM. Source-native direct
callbacks accept full uint32 arity. The existing VM bridge explicitly rejects
more than16 arguments; native BitAnd/Or kernels are tested through65, while the
VM test proves16 accepted/17 rejected. This remains an explicit bridge boundary.

Final evidence:

- Optimized NDK29 ARM64:13,978 original/native cases,862 ordered imported
  services, zero mismatches. Exact words include NaN/Infinity conversion,
  signed overflow, finite/random edges and all argument/type/count guards.
- Genuine VM-linked host DSO: same13,978 gold cases,836 identity/division
  service comparisons;40 actual Lua checks and13 malformed/reload guards.
  The26 string primitive calls in the ARM oracle are replaced by real fresh
  Lua parsing on host. ASan/UBSan/leaks0.
- Actual Lua checks include decimal/hex/invalid/negative/first-NUL strings,
  two-result arity, table __index argument projection, ignored extra arguments,
 64-bit identity mapping, absent division-zero rejection and ordered reload.

Gold binary SHA-256:
`1db4e59bb1aa052fb538bd25b4bd9f5d0ff4a8ce82fd702b39f411c1979c8898`.
Header SHA-256:
`378f06afa5311644a89a5d2d057e8e490dcf23e3ff2a555a5fc73b06a3833a3b`.
C SHA-256:
`de37788729dabcd53afa6ca9bf5f918229979b8624ae20e11dd1499b267ead61`.

Reproduce with the configured Python/PYTHONPATH:
`reference/scalar-bindings/run_arm64_audit.py`, then
`reference/scalar-bindings/run_host_audit.py`. Reports bind compiler, source,
original captures, gold and actual binaries. No full external AI/script
acceptance, packaged instructions, physical ARM64 or gameplay parity claim.
