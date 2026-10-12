# LuaScript private integers and Value string coercion

The source is the original `libDungeonHunter2.so`, SHA256
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
`original-functions.json` binds nine routines to their original bytes;
`reference/original-functions.asm` retains their instructions. The separate
`teardown` capture binds the destructor and recursive integer-map erase.
`value-contract.json` records the actual ELF literals and imported symbols.

## Source operations

- `_SetInt` at `0x37de5c` accepts at least two already-projected Values, regardless
  of their types. It calls the first Value's `getString`, calls the second
  Value's `getNumber`, converts with imported signed float-to-int, then invokes
  private `SetInt` at `0x37d990`. Additional Values are ignored by this callback;
  their earlier VM-to-Value projections still happen. It returns no Lua values.
- `_GetInt` at `0x37ec14` accepts at least one Value. It obtains the name via
  `getString`, calls private `GetInt` at `0x37da30`, then `pushInteger` at
  `0x37cb24`. The returned Lua number is the signed integer converted to float32.
  Additional Values are ignored after their projections. Zero arguments return
  no values.
- The map is **private to each LuaScript**, at original owner `+0x1c` (root
  `+0x20`, minimum `+0x24`, maximum `+0x28`, size `+0x2c`). Its keys are unsigned
  32-bit hashes, with signed 32-bit values. Name spelling is not retained for
  equality: genuine hash collisions share one value. `GetInt` inserts zero on a
  miss, even though the original method is declared const.
- `hashString` at `0x37c164` reads signed bytes through the first NUL. Starting
  with zero, each byte updates `h ^= signed_byte + 0x9e3779b9 + (h << 6) +
  (h >> 2)`, with unsigned wrap. `Alias16038` and `Alias16275` genuinely collide.
- In `LuaScript::~LuaScript` at `0x37c004`, loaded files and path precede alias
  backup/main clearing. Integer clearing then executes `0x37c0bc..0x37c0e8`,
  recursively erasing via `0x37bcc0`, before `Instance::~Instance` at `0x37c100`.
  The earlier ownership label “arguments at +0x1c” was incorrect: this is the
  integer map. The new `clear_contents` leaves the wrapper alive through Lua
  close/finalizers. Destroy the wrapper after VM close, because a finalizer may
  perform another genuine insertion through its borrowed callback receiver.

## Value conversion

`Value::getString` at `0x31c49c` has these exact branches:

| Source Value kind | String |
| --- | --- |
| 0 | Shared literal `nil` |
| 1 | `getBool` at `0x31bc80`, then shared `true` or `false` |
| 4 | Exact existing source `string.c_str()` pointer; no copy |
| 3, `floorf(n) == n` | Imported signed float-to-int, then `sprintf("%d", integer)` |
| 3, otherwise | Promote the source float to double and imported `sprintf("%f", double)` |
| 2 or 7 | Original identity word at Value `+0x6c`, formatted `0x%0*x` with width8 |
| Other | Null pointer |

Converted numbers/identities are copied into the original Value's string
storage. The new helper uses explicitly owned projection-workspace storage,
valid through the next conversion or workspace destruction. Original strings
return the identical borrowed pointer. Negative zero becomes `0`; integral
infinities use the imported signed conversion contract. Fractional numbers and
NaNs require the supplied formatting service, so the port does not assert that
its Android printf, locale, or NaN spelling equals the shipping dependency.

`Value::getNumber` at `0x31bbf0` reads bool/number payloads, converts source
32-bit identities unsigned-to-float, and parses strings by creating a fresh
empty Lua state, pushing the C string, calling `lua_tonumber`, and closing it.
Other Value kinds yield zero. The native helper uses the genuine float32 Lua
parser in a separately protected fresh state; allocation failure is an explicit
native error rather than a claim to reproduce the original allocator panic.

The source's AEABI imports are undefined in this ELF. The scalar proofs' same
explicit contract applies here: signed conversion truncates toward zero,
saturates to signed limits, and maps NaNs to zero. This is a dependency contract,
not recovered machine code for the missing platform helper.

Source `_setFromStack` projection precedes these callbacks. Tables become kind7
from their normal `_this` lookup, with observable metamethod effects. Lua
functions/threads/raw full userdata become nil. Consequently an actual Lua
function passed as a key becomes the `nil` key, whereas a synthetic unprojected
kind6 Value has a null `getString` result. The native direct callback rejects
that original null-name crash domain explicitly. Embedded NULs stop source
strings and hashing at their first NUL.

## Native API and ownership

`script_int_bindings.hpp/.cpp` add a map per source Script/session, a borrowed
40-byte binding receiver, source conversion helpers, and actual `SetInt`/`GetInt`
Lua bindings. No shared global/default integer map is created. The receiver,
map wrapper, and service context must survive callbacks including VM close.

Nonzero native64 identities require `dh2_script_int_identity` to map the real
source identity explicitly to its source32 word. Pointer truncation is never
used. Null identity maps zero without a service. Fractional formatting requires
`dh2_script_int_format_fraction` to provide genuine platform spelling; omission
fails explicitly before map mutation. Integral `%d` and width8 lowercase hex
projection have fixed ASCII behavior. Missing/invalid receivers, reserved words,
output capacity, and unsupported name kinds fail explicitly; no successful
source crash behavior is invented. Generic source-values bridge's existing
16-argument cap remains an explicit boundary; core runtime and scopes were not
changed for this module.

To integrate, add `script_int_bindings.cpp` to `dh2_script_runtime`. Bind with
`dh2_script_int_bind(vm, &session_bindings)` after creating the real per-script
map. This module does not allocate or publish an AIS identity, does not choose
owner lifecycle stages, and does not grant general VM reentry. The parent's
owner integration remains responsible for coherent VM/map/session identity and
source teardown order.

## Evidence

`reports/script-int-arm64-differential.json` is PASS for **3,163** cases,
**6,326** complete two-owner map snapshots, **367** ordered coercion services,
and zero mismatches against optimized standalone ARM64. Actual original callback,
private map search/store, hash, Value conversion, and recursive erase instructions
execute. The destructor's integer-only region executes eight clears, releasing
199 actual original nodes and proving empty headers before the VM-close boundary.
Original tree insertion/allocation/balance, string storage and imported helpers
are explicit services. The optimized native unsigned-key RB-tree instructions
execute and their parent/color/black-height/order/size invariants are checked.
Original STL balancing/allocator instructions are not claimed reconstructed.

`int-original-gold.json` SHA256
`cf5f965c2de02b20eef6725ac2a3b1bb508585e9965c837e1da67c418624998f`
retains all input Values, result projections, both maps, and ordered services.
The lossless binary projection `int-original-gold.bin` SHA256
`1b62ea9e1083e860490c800a75ce16aaff51126596d729910d23abbcf45583c8`
is replayed by the host test. Parser-service calls are deliberately not imitated
in the host: the genuine Lua parser executes instead.

`reports/script-int-host-audit.json` binds the isolated module DSO, actual frozen
float32 Lua DSO, executable, unchanged core source, test, corpus, and ARM report.
PASS: **3,163** gold cases, **6,326** map snapshots, **357** ordered identity/format
services, **35** actual VM checks, **15** atomic/unsupported guards, and zero
ASan/UBSan/leak errors. The ten additional original parser services execute as
real parsing in the host. VM checks include separate owners, hash-only collision,
missing insertion, arity, bool/nil/number/string keys, NaN/identity conversion
fixtures, table projection effects, ignored extra argument projection, embedded
NUL, explicit missing formatting/identity failures, and a real `newproxy` close
finalizer invoking SetInt/GetInt after the map was cleared but kept alive.

The combined host Lua chunk intentionally verifies the observed negative-zero
constant reuse: its `1/0` after a prior `-0` resolves to negative infinity and
thus the `-2147483648` key. Both positive/negative infinities are independently
covered in the instruction gold corpus. This host observation is not a claim
of whole original Lua VM instruction parity.

Reproduce host proof with Python `tests/run_script_int_host.py`. Its default
isolated build directory is `/home/adampalace/dh2-script-int-build`; it never
rebuilds the central runtime. The host executable accepts one positional
argument: `reference/int-bindings/int-original-gold.bin`. Original/native proof
is `tests/script_int_differential.py --library <optimized ARM64 oracle.so>`;
build the new cpp with NDK29 clang++ target `aarch64-linux-android24`, shared PIC,
`-O2 -std=c++17 -fno-fast-math -ffp-contract=off -Wall -Wextra -Werror`.

This is isolated source/instruction plus genuine host VM proof. No package,
device, shipping-libc formatting, full game binding inventory, or live manager
ownership claim is made. Existing scalar/root/scope source and historical
reports were preserved.
