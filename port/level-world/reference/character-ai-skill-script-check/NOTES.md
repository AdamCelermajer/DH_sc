# Source skill-check callers

There is no unsuffixed C++ `CharAISkillScript::OnSkillCheck` symbol. This unit
rebuilds two complete normal callers:

| Function | Original address / size | Returned result |
| --- | --- | --- |
| `OnSkillCheck_Usable()` | `0x3da9dc` /288B | First returned Value's source getBool result |
| `OnSkillCheck_Active()` | `0x3db16c` /284B | Second returned Value's source getBool result |

Original ELF/symbol/range hashes, both call-name literal bytes and unchanged
`skills/_commons.luac` are pinned in`original-functions.json`.

## Caller control and freshness

Both capture the skill identity and construct local ReturnValues before reading
skill+4 Character and Character+`0x3e4` Lua script. A null script converges on
normal destruction and false. Otherwise they call `SetSkill` with the captured
skill's embedded Arguments+`0x0c`. The existing constructor provides name and
skill index; this caller does not construct or change those arguments.

Nonzero ReturnValues Error word+8 returns false after normal destruction.
Zero error reads the live vector pointer+`0x24` and clears its whole nonempty
begin/end range. Then the caller rereads skill+4 and the fresh Character script
and calls `OnSkillCheck` with zero explicit Lua arguments and the same output
ReturnValues. There is no second null-script guard. That late-null input is
an explicit invalid port boundary, never a successful skipped check.

After this second Call, both read the fresh Error word. Error returns false
normally, without reading the result vector. With zero error, the caller reads
the live vector and computes the count. Usable requires at least one result,
then calls actual `ReturnValues::operator[](0)` (`0x3da43c`/88B). Active requires
more than one result, then directly selects the second record (`begin+0x70`
in ARM32). Both call actual `Value::getBool()` (`0x31bc80`/172B), capture its
boolean in `r6`, destroy ReturnValues, and return that captured value. Destructor
mutation/raw return does not replace it. Too few results returns false normally.

SetSkill and both Lua Call returns are discarded; a normal Lua error is carried
by ReturnValues.error while the port service succeeds. Treating a normal Lua
error as a provider failure would incorrectly skip source cleanup.

## Exact source value coercion

The original getBool body runs in the ARM proof, but its native implementation
remains a mandatory typed provider and receives no new body credit here.

| Source Value type | Source getBool behavior |
| --- | --- |
| 0 or unsupported tags (including5/6/8) | false |
| 1 boolean or3 number | Binary32 word+8 compared to+0; equal false, otherwise true. Both signed zeros false; NaN true |
| 2 light userdata or7 projected object | Identity word+`0x6c` nonzero |
| 4 string | Create temporary Lua state; freshly read C-string pointer+`0x20`; push string; Lua bool(-1); close temporary state |

String helpers are proven by actual ELF symbols: `luaL_newstate` at`0x84c7e0`,
`lua_pushstring` at`0x84c04c`, `lua_toboolean` at`0x84b320`, and `lua_close`
at`0x85797c`. This is not a registry reference/current-state restoration path.
A retained nonnull C string, including empty or`"0"`, is true; null pointer
pushes nil and yields false. Provider allocation/copy/close effects remain
mandatory; a native provider cannot reduce the string branch to an effect-free
boolean computation and claim to have rebuilt these dependencies.

The numeric import at`0x30df8c` is independently resolved by executing its
relocated PLT stub and checking`__aeabi_fcmpeq` before comparison modeling.
Compiler trap`0x30e310` similarly resolves to`__stack_chk_fail`.

## Shared projection and native gaps

The new header reuses the frozen OnSkillUpdate `State`, `Character`,
`ReturnValues` and `ValueVector` logical views. Native contiguous Value stride
is explicit adapter-owned metadata; no original112-byte Value layout is cast
onto native memory. The caller reads begin/end and derives the native count
using that actual stride. Positive stride<=4096 and count<=1e6 are port bounds,
not recovered game rules. Usable's index service returns a real borrowed Value
handle from the current resource; Active selects the second native record.

Required providers: real ReturnValues construction/destruction, real full-range
erase, exact two Lua Call overloads with owned source result conversion,
`operator[](0)` and exact getBool coercion/dependencies. The current Session's
discard-result VM path does not own typed returned Values. These gaps were
coordinated with the other agent; none is supplied as a successful production
placeholder. The unchanged common Lua registry selects OnSkillCheck callbacks
through SetSkill; that script body/registration/VM adapter is not implemented
or executed by this standalone caller gate.

One owning thread retains controls, replaced owners/scripts, ReturnValues and
retired Value storage until synchronous return, including errors. Resource
identity is fixed; error/vector may change at real provider boundaries.
Bindings/context/stride and captured skill identity stay fixed. Independent
resources may nest; same-state/resource reentry or destruction is outside the
contract. Guards reject known aliases/malformed bounds before reached reads.
Provider failure preserves prefix effects without rollback or added destructor.
Normal false/error branches perform the original destructor.

## Verification and scope

The C++ fixture owns and mutates real vector storage, but its resource/Lua/bool
services are named dependency fixtures, not original dependency implementations.
It checks212 behavior cases,27 returned-error/throw/missing-provider cases,
16 reached-source bounds and14 preflight guards.

The runner compares206 cases against real original ARM callers. Normal caller
instructions execute unmodified, as do the actual indexed getter's normal
bounds path and getBool's full type dispatch. Only the five resource/Lua/erase
callees and getBool's four named string Lua callees are controlled fixtures;
numeric equality is modeled after exact import identity verification. Cases
cover both result indices, normal script errors, insufficient arity, owner and
script replacement, signed zero/NaN/infinity/subnormal, null/nonzero identities,
unknown types, empty/nonempty/null strings, fresh string pointer after newstate,
and captured boolean surviving destructor mutation.

Compiler canary corruption traps, literal pools and indexed getter assertion
path are excluded from execution coverage. Count: **two complete normal caller
bodies; zero new dependency or Lua callback bodies**. This is not APK/native
wiring or complete skills gameplay.
