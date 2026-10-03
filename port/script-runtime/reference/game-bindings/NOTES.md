# Original character Lua timer bindings

All original captures identify ELF SHA-256
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
`extract_registration.py` resolves actual PIC literals/GOT callback pointers,
not inferred names. `registration-names.json` records:

| Global name | Registration call | Original callback |
| --- | --- | --- |
| `StartTimer` | Character.createBindings `0x3b6220` | Character._StartTimer `0x3b7590` |
| `StopTimer` | Character.createBindings `0x3b62bc` | Character._StopTimer `0x3b7064` |
| `Trace` | LuaScript.BindFunction `0x37b60c` | LuaScript._Trace `0x37ee80` |

Character.createBindings also registers method aliases through the separate
Binder.bindMethod path. The new module supplies the global functions consumed
by the actual AI commons; generic object methods and all other game bindings
remain outside this bounded reconstruction.

## Actual callback and coercion rules

The registered source callback signature is C++
`void(const sfc::script::lua::Arguments&, ReturnValues&, void* character)`.
Binder.bindFunction `0x31a4d4` stores callback/context as closure upvalues.
Binder.__functionCallback `0x31a250` constructs all Arguments from the Lua stack,
then invokes the callback, pushes all ReturnValues, and returns their count.
The new C ABI uses native borrowed identities and explicit timer services.

StartTimer requires at least one argument and checks the first engine Value
type is exactly Lua NUMBER3. A numeric string, boolean, nil, or object does not
start a timer and returns **zero** Lua values. It does not use luaL_checknumber
or raise an argument error. Extra arguments are unused by the callback body,
but the original Arguments conversion still processes them.

The duration is Value.getUInteger `0x38d798` through Value.getNumber `0x31bbf0`
and actual `__aeabi_f2uiz` `0x8be2a0`. The new bitwise kernel preserves:

- positive finite fractions truncate toward zero;
- negative values, either signed zero, values below1, and NaNs produce0;
- positive values at/above2^32 and positive infinity produce UINT32_MAX.

The optional loop argument defaults false. Value.getBool `0x31bc80` checks
numeric/boolean payload against zero; NaN is true. String Values are true even
for empty strings or the text `"false"`/`"0"`. Light/native-object pointers are
true exactly when nonnull. Other engine Value types are false. This differs
from treating Lua numeric0 as true.

Before the callback, Value._setFromStack `0x31c9c8` maps actual Lua arguments:

| Lua input | Engine Value projection |
| --- | --- |
| nil | nil0 |
| boolean | boolean1 with float0/1 payload |
| light userdata | type2 pointer |
| number | type3 float32 |
| string | type4 copied through strlen, truncating at first NUL |
| table | type7 pointer obtained by normal `table._this` lookup and lua_touserdata |
| function / raw full userdata / thread | nil0 |

The table lookup is a normal lua_getfield, so `__index` runs synchronously.
Plain tables have a null `_this` pointer and therefore false loop coercion.
Source-value binding is separately named `dh2_script_vm_bind_source_values`;
generic VM.bind retains its earlier scalar contract. Results remain scalar
under that binding ABI; portable full-userdata/object/table return handles are
not supplied by this module.

The actual start call is:

`CharTimers::TMR_Start(character+0x3b4, duration_ms, repeat, event=0x35, user_ref=0)`

Repeat is0 for one-shot and -1 for looping. Duration is already milliseconds;
there is no seconds conversion. On source service return -1, StartTimer returns
zero values. Every other original returned int32 produces one float32 Lua
number through ReturnValues.pushInteger `0x37cb24`; the callback adds no success
boolean or error result. StopTimer has the same exact NUMBER3 first-argument
gate, unsigned conversion, and owner. It invokes TMR_Stop `0x3db2d8`, which
clears the active byte only for an in-range slot. It always returns zero values.

Trace `0x37ee80` is exactly `bx lr`. It performs no string coercion, logging,
message formatting, or return pushes in the callback body. Argument projection
can still invoke a table `_this` metamethod before the empty body is reached.
No fake logging service is installed.

Native installation requires real owner identity and timer service pointers
before installing the names. Missing services leave names absent. Native
guard/protected-error results are port diagnostics, not invented source Lua
return values. The parent must map native storage/validation failures explicitly
to the original timer allocation/error boundary, rather than feeding native
negative guard codes to the source callback as accepted timer IDs.

## Timer ownership and actual selected dispatch

The timer belongs to Character, independently of its currently active AIS.
TMR_Start stores event35 and null user_ref. Expiry carries the Timer object,
not user_ref. RaiseAIEvent `0x3cbb34`, event35 jump `0x3cbc2c`, enters
`0x3cbcbc`: obtain Timer.GetID through its virtual0, or UINT32_MAX for null
payload, then invoke CharAI virtual+0x90 and return.

CharAI.OnScriptTimer `0x3d0ca0` checks active AIS+0x1c and invokes its virtual
+0x90 only when present. AISPlayerIPhone's actual vtable at `0x966cd0+8+0x90`
resolves to inherited **AISDefault.OnScriptTimer `0x3dcc80`**, an80-byte body.
It pushes the timer ID through Arguments.pushInteger `0x3cdd78` then calls
LuaScript.Call `0x37c41c` with the literal `OnTimer`. IDs therefore pass through
signed32 integer-to-float conversion, including null-payload UINT32_MAX→-1.
The distinct AISDefault.OnTimer virtual+0x80 `0x3dbedc` is empty and is not the
script timer path.

`dh2_script_game_on_timer(vm,id)` implements the selected AISDefault body after
the caller has resolved the active AIS/associated VM. It does not reconstruct
the outer CharAI dispatcher, ownership promotion, or LuaScript function-name
alias map. LuaScript._GetFuncName `0x37c314` maps the requested function name
through its per-script alias tree and otherwise uses the original name;
Instance.pCall `0x31abe8` then looks it up in Lua globals. A plain `_commons`
VM has no alias tree. The caller must supply source alias/ownership facts before
claiming a complete LuaManager integration.

## Discarded return values have observable projection effects

LuaScript.Call `0x37c41c` creates temporary ReturnValues, calls `0x37c390`, then
destroys the container. Instance.pCall_ `0x31aa78` calls lua_pcall with
LUA_MULTRET, computes the actual return count, and invokes
ReturnValues._addFromStack `0x31b4ac` for indices `-count .. -1` in that order.
Each uses the same Value._setFromStack projection. Consequently, returned
tables invoke normal `_this` lookups even though their converted pointers are
later discarded. Requesting zero results would skip these effects.

The new `dh2_script_vm_call_discard_source` requests all results and projects
them in order with no fixed wrapper return-count limit. Tables retain real
`__index` execution; strings use the source first-NUL length operation. Copying
and destroying the original temporary native std::string/vector have no Lua
callbacks; those allocation details are not substituted as a source allocator
parity claim. VM memory/standard stack limits apply. Source Lua errors are
recorded by Error.setError `0x31a8ac`; the port additionally protects projection
errors and reports them through its owned C ABI. Original panic behavior,
error-string/container allocation policy, and LuaScript's global call statistic
are not reconstructed by this entrypoint.

`probe_return_projection.py` executes actual original pCall_, _addFromStack,
and _setFromStack instruction loops using explicit Lua primitive/vector/string
services. Seven cases cover0/1/2/17/48/256 returns and mixed types. They prove
all return indices, left-to-right `_this` lookup order, source type mapping,
first-NUL string copy, and final stack restoration. This is not an original
whole-VM execution claim.

## Native proof and integration handoff

`script-game-bindings-arm64-differential.json` binds5,493 actual original
callback observations against the optimized standalone ARM64 library, with
3,194 exact ordered timer requests and zero mismatches. GetBool/GetUInteger
and the actual unsigned-conversion instructions execute. Timer methods,
ReturnValues integer pushes, and original string-getBool's Lua primitives are
explicit service boundaries. Binary gold is `callback-reference.bin`.

`script-game-bindings-host-audit.json` binds the same5,493-case ASan/UBSan replay
and a real-VM/genuine-source-CharacterTimers integration audit. The latter passes
123 checks,17 real timer starts,5 stops,6 actual `_commons` timer callbacks,
source one-shot replacement cleanup, table `_this` metamethod effects, signed
ID conversion,256 discarded returned tables in exact order, returned-value
projection error recovery, and a real timer start during a return metamethod.
The timer service is the actual existing `character_timers.cpp` kernel, not a
timer stub. Host original-corpus replay keeps its controlled service boundary
distinct from this real timer integration proof. The438-check standalone VM
regression and both Android ABI build reports are refreshed from final source.

The standalone `port/script-runtime/CMakeLists.txt` target
`dh2_script_runtime` already includes `script_runtime.c`,
`script_game_bindings.c`, and the source Lua core/libraries. Do not add
`script_game_bindings.c` twice when integrating the target. It depends on libc
and libm, not level-world; the parent supplies borrowed TimerStore services.
New host audit targets are `script_game_bindings_audit` and
`script_game_bindings_replay`.

Parent service wiring is:

1. Own a stable VM, GameBindings40 object, and Character TimerStore.
2. Start service calls `dh2_character_timer_start(store,duration,repeat,0x35,0,services)`;
   Stop service calls `dh2_character_timer_stop(store,id)`.
3. Timer expiry resolves source event35→active selected AIS→its VM, then calls
   `dh2_script_game_on_timer(vm,timer.id)`.
4. Destroy VM before releasing its borrowed bindings/context/owner storage.

Same-VM nested C ABI reentry remains unsupported and explicit. Timer expiry
from the outer native timer update can execute Lua and synchronously mutate the
same TimerStore. Tested slots remain stable during expiry; native allocation/
growth during update still follows the timer module's explicit boundary.
No shared CMake, renderer, APK, Android app, or device was changed by this task.
