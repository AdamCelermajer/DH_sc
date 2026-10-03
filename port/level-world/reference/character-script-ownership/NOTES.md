# Selected player script and VM ownership

The real authored Player AI row44 selects `__player__`, sets CharAI.scripted=1,
and constructs AISPlayerIPhone with a privately owned Lua VM. The constructor
argument1 defers registration; it does **not** disable Lua or share a manager VM.
This is original-source discovery, outside the frozen b449 checkpoint. No
production runtime, renderer, or CMake file changed in this task.

All manifests bind original ELF SHA256
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
`ownership-probe.json` contains47 focused original-instruction cases.
`vm-core-probe.json` additionally executes the actual original Lua constructor,
four library cores, and close:756 heap allocations and756 frees. Its allocator,
libc and successful setjmp continuation are explicit desktop services. These
are ownership/source probes, not an original-versus-native whole-AI proof.

## Authored selection and pending ownership

The supplied cache SHA256 is
`3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679`.
The existing hash-verified AI assets expose row44 Player with Script
`__player__` (10 serialized bytes). Runtime StepCreateScript `0x3cf04c` reads
length/name at row+0x28/+0x2c in a68-byte resolved row. Our probe uses those
authored values and executes actual SetScriptByName `0x3ceeb0`, the iPhone
factory `0x3ccfe4`, CharAIScript constructor `0x3d8fb0`, LuaScript constructor
`0x37c674`, Instance constructor `0x31b268` and its VM creator `0x31b224`.
Only backend allocation and VM/library/registration services are intercepted
in that first probe; the second probe executes the Lua VM/library cores.

The factory allocates216 original ARM32 bytes, calls CharAIScript(true),
initializes the AISPlayer vector and fields, installs vtable0x966cd8, then
stores pending at CharAI+0x20. It leaves active+0x1c and progress+0x28 unchanged.
StepCreate stores scripted=1 **after** returning from the factory. Exact
216-byte ARM32 allocation is evidence, not a native64 object-layout proposal.

Factory replacement invokes AI OnTerminate, reloads pending, calls its deleting
destructor if still present, and clears pending after destruction. Active is
not cleared by the factory. Borrowed callbacks must therefore maintain valid
receiver lifetime across the separately recovered cleanup/unload sequence.

AISDefault factory `0x3cce14` and ordinary AISPlayer factory `0x3cd11c` also
call CharAIScript(true) and allocate owned VMs. The scripted flag belongs to
CharAI selection, not to that constructor bool. Thus an empty-row fallback can
have scripted=0 and still possess a VM; it skips common loading, rather than
skipping construction or the unconditional BindFunction/SetCharacter stages.
The actual default/player factories and deleting destructors execute in the
probe. No separate out-of-line AISDefault constructor was found; the factory
contains its derived initialization.

## Private VM creation and teardown

Instance default C1 `0x31b268` / C2 `0x31b2a8` sets ownership byte+8 to1,
calls newstate `0x31b224`, and stores Lua_State at Instance+4. That helper calls
lua_newstate `0x8579f8` with LuaAllocator `0x310500` and userdata0, then installs
panic `0x31a9e8` through lua_atpanic `0x84b11c` only for a nonnull state. No
library is opened there. LuaScript embeds this Instance at+4, so state is at
LuaScript+8 and ownership byte at+0xc. Arguments/Binder at script+0x10 points
to the embedded Instance via field+0x14. It has no independent VM ownership.

The state-pointer Instance constructors `0x31aa20` / `0x31a9f0` instead store
the supplied pointer and ownership0. D1 `0x31b180` / D2 `0x31b1e0` calls
lua_close only when owned. Repeated private constructor probes return distinct
states; borrowed destruction never closes its supplied state.

LuaScript constructors initialize argument storage, alias main/backup trees
at+0x34/+0x4c, tracking byte+0x64, path string+0x68 and per-script loaded-file
set+0x80 empty. CharAIScript adds borrowed Character+0x98, AI-state tree+0x9c
and current-state pointer+0xb4, all initially null/empty. Existing alias-module
proof covers the alias-tree mutations and lookup, separately.

Deleting iPhone destructor `0x3de294` calls AISPlayer D2 `0x3de100`, then
CharAIScript D2 `0x3d92f0`, LuaScript D2 `0x37bec0`, Instance D1, lua_close, and
finally frees the original AIS allocation. The derived Character vector,
AI-state tree, loaded-file set, path string, alias trees and argument collection
are released before Instance close. Empty-state teardown executes in our
probe; nonempty container implementations are captured/call boundaries, not a
new native STL-ownership proof here.

An original VM-allocation failure stores null while leaving owned=1. Original
Instance destruction then passes null to lua_close without a guard. The
failure probe captures that invocation through a close service; it does not
claim executing lua_close(null) is safe. Native allocation failure must remain
an explicit protected caller failure, with no published AIS.

## Load-stage registration and publication

Actual LoadScriptProcess `0x3cf1f0` executes the already recovered stages:

|Stage|Original producer|Selected player effect|
|---|---|---|
|0|StepCreateScript `0x3cf04c`|Private pending iPhone VM, scripted1|
|1|StepBindFunction `0x3cc278` -> CharAIScript.BindFunction `0x3d8f2c`|Libraries,33 LuaScript globals,2 AI globals|
|2|StepSetCharacter `0x3cc26c` -> SetCharacter `0x3d90f8`|Borrow Character, actual virtual+0xc registration, set source path|
|3|StepLoadCommon `0x3cc218`|When scripted!=0, pending.Load(`_commons`)|
|4|StepLoadScript `0x3cdf7c`|No external load when filename+0x30 is null|
|5|StepInitScript `0x3cb314` -> AI OnInit `0x3d12b0`|Timers33/34 for alive owner, then pending OnInit; no InitVCB for null filename|
|6|Store `0x3cf268`|active=pending; pending is retained; live progress increments|

The selected inherited AISDefault.OnInit `0x3dbe78` is an actual empty body.
That does not erase the prior VM, binding, common load, or owner timer work.
Our complete stage probes execute actual selection/factory/registration caller
bodies, original IsDead getter `0x3a2ed4` (Character byte+0x1449), OnInit and
publication. Design integer/timer start and Manager.AddFile are explicit
services; active remains null during common and pending OnInit.

LuaScript.BindFunction `0x37b5a0` opens **Base -> Math -> Table -> String**
through Instance methods `0x31b010`, `0x31b000`, `0x31aff8`, `0x31b008`.
Those tail-call original library cores directly. Actual VM probes show a fresh
stack0, then tops2,3,4,5 (return counts2,1,1,1). Library results remain on the
caller stack. The registration services in this probe leave them untouched;
captured Instance.registerFunction `0x31af08` pushes upvalues, forms its closure
and sets the global, without an enclosing stack reset. An eager wrapper that
uses lua_call(...,0 results) is not this constructor/binding stack policy.

The ordered33 LuaScript registrations, exact callback addresses and borrowed
script userdata are in `ownership-probe.json` case step1-bind. They begin
Include/Trace/SetInt/GetInt, contain AddToVFTable/PushVFTable/PopVFTable, and end
PlayMusic/PlaySound/StopSound. Source global **OnTargetDied** maps to symbol
LuaScript::_IsPlayerCharacter; preserve the captured name instead of renaming
it from the C++ symbol. CharAIScript then adds RegisterAIState (`0x3da144`) and
ChangeAIState (`0x3d9710`) with script userdata.

SetCharacter stores Character+0x98 before invoking the live Character virtual
+0xc. The selected Character vtable resolves that method to createBindings
`0x3b56bc`, which first calls GameObject.createBindings `0x38d7ec`. Executing
both callers produces135 global function registrations and130 method
registrations, in source order, with borrowed Character userdata for globals.
LOCK and UNLOCK are method-only. Full identities and order are in case
step2-real-character-and-gameobject-registration. These are real registration
producers with Binder.bindFunction `0x31a4d4` and bindMethod `0x319af4` as
explicit services; they do not prove every registered gameplay callback body.
After registration, the exact16-byte path is `data/scripts/ai/`.

StepLoadCommon does not examine LuaScript.Load's bool return. The probes with
Manager.AddFile responses0 and1 both continue through pending init and publish.
The native lifecycle's explicit protected-service failure policy is a bounded
port contract, not an original failure gate.

## Manager and common resource identity

LuaScript.Load `0x37b574` obtains the application LuaManager+0x3c and passes
the **particular script object** to Manager.AddFile `0x37b23c`; that method does
not substitute a manager-owned VM. Manager C1/C2 `0x379eb0` / `0x379e68` only
initialize their filename->StreamBuffer map. Actual empty manager construction
and destruction call no VM allocator or close.

AddFile builds script path+name and checks/appends the captured `.luac` suffix.
It first tests the per-script loaded-filename set at+0x80; an already loaded
filename returns true without reexecuting. Otherwise it finds/reuses the
manager's StreamBuffer cache or obtains a new resource stream, resets stream
position with virtual+0x20, and invokes Instance.loadFile `0x31acf4` on
**script+4**. That routine uses lua_load then, on successful load, lua_pcall;
successful execution inserts the filename into this script's set. Failed load
does not insert it. Therefore shared cached bytes do not imply shared globals
or a shared loaded-file set. Cache/VM execution branches are disassembly facts;
our whole-stage probe models AddFile's result explicitly.

Manager FlushBufferedFiles `0x379fe8` calls each StreamBuffer's deleting
destructor virtual+4, then erases its map. Manager D1 `0x37a0ac` calls that flush.
LuaScript destruction erases its own loaded-file set and closes only its own
Instance, without flushing the manager cache. Manager/cache resources and
script VM/alias state have separate ownership.

The exact player common input is cache entry
`com.gameloft.android.GAND.GloftD2SS/files/data/scripts/ai/_commons.luac`,13535
bytes, SHA256
`20d34968e9983e14c24223085ab40d55d47f9387dfdbbf3ff7e6b39aea91252c`.
The probe verifies the archive and bytes independently. It contains Lua source
despite its suffix. Existing game-binding/alias proofs execute these exact
bytes in a genuine native VM and establish the commons-only alias-empty case;
that is separate evidence from this original ownership probe.

## Additive native API proposal; no runtime edit yet

Preserve existing `dh2_script_vm_create` and its historical proofs. Add:

```c
dh2_script_vm* dh2_script_vm_create_empty(size_t memory_limit);
int dh2_script_vm_open_source_libraries(dh2_script_vm* vm);
```

create_empty owns a fresh state/allocator, installs the native protected-error
boundary, leaves all four libraries absent and stack0, and returns null on
allocation failure. memory_limit/error protection are native caller contracts,
not the original heap allocator or original null-state destruction behavior.

open_source_libraries invokes Base/Math/Table/String in that order and
preserves their five results on successful return. It must not use the current
initialize helper that calls library closures with0 requested results, or the
current protected_operation helper's unconditional success-stack restoration.
There is no source once-only flag: repeat invocation must perform the ordered
opens again and append the five results. A protected lua_pcall around a native
C wrapper returning5 results can preserve this observable stack policy while
reporting allocation/error failure. Failure may follow earlier global/table
mutations; no transaction or automatic binding rollback should be claimed.

A caller-owned native AIS session should then own exactly one VM and one alias
map per source LuaScript identity, while borrowing Character and registration
services. It must deliver the35 Stage1 registrations and265 Stage2 requests
with the captured names/identities/order, or fail explicitly for unsupported
services. Existing timer/alias globals cover part of this catalog; absent
globals/methods are not successful no-ops. Include/file caching needs a distinct
manager resource cache and per-session loaded set. Existing selectors and
ScriptLifecycle remain responsible for pending, init and publication; do not
publish during VM creation/common loading or manufacture an active script.

The next native proof should compare empty/global absence, exact ordered opens
and stack policy, distinct sessions/alias maps, source Stage1/2 service order,
commons execution on both private VMs, timer35 selected-instance routing,
failure prefixes, pending publication and teardown through the actual world
DSO and script runtime. Full135 gameplay globals/130 methods and manager I/O
must remain explicit services until separately recovered. This discovery alone
does not authorize a whole-AIS or full-gameplay parity claim.
