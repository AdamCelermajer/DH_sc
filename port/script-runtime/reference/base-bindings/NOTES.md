# Source base callbacks and nested Include handoff

Discovery is frozen. This directory adds captures/probes only. It does not change
the runtime, ScriptOwner, CMake, renderer or an APK. Original ELF SHA-256 is
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
The focused manifest binds every captured routine to its original bytes;
`catalog/original-functions.json` additionally captures all 33 original base
callbacks, from the original registration evidence bound by `catalog.json`.

## Include and load ordering

`LuaScript::_Include` **0x37efe4** reads the projected Arguments vector (112-byte
source Values). Zero arguments or first Value type other than 4 returns silently,
with **zero ReturnValues**. The string path uses Value::getString **0x31c49c**,
then tail-calls LuaScript::Load **0x37b574**, which obtains Application's
LuaManager and invokes AddFile **0x37b23c** with this same script identity.
No AddFile result is converted to a Lua result or raised as an exception.
The binder projects all arguments before this wrapper; unused table arguments
can therefore still run their `_this` lookup metamethod.

AddFile performs this order:

1. Reject null/empty filename as source false; concatenate script path+0x68 and
   requested name. Search **the requested name** with `strstr(name,".lua")`.
   Absent substring appends `.luac`; present substring whose first five bytes are
   not `.luac` appends `c`. This is not an extension/end-of-string check:
   `child.lua.extra` becomes `child.lua.extrac`; uppercase `.LUA` gains `.luac`.
2. Query this script's loaded-file set+0x80. A hit returns source true without
   touching shared stream/cache or executing the VM.
3. Query the manager's filename-to-StreamBuffer cache. A hit resets the stream
   through virtual+0x20 to 64-bit offset zero before load.
4. A miss opens the genuine resource via virtual+0x88. Missing resource returns
   false. Otherwise construct StreamBuffer48 at **0x3172d8**, insert the stream
   into the manager cache, then release the opened resource via virtual+0x78.
   **Cache insertion and resource release precede VM execution.** The copied
   stream survives a compilation/runtime failure.
5. Execute this script's Instance::loadFile **0x31acf4**. On nonzero Error.status,
   destroy Error and return false, without inserting into the loaded-file set.
6. Only successful load/execution inserts the complete filename into this
   script's loaded-file set; then return true.

Instance::loadFile initializes an Error, calls genuine `lua_load` **0x84bbbc**
with reader **0x31b018**, reader buffer size **1024**, and chunk name
**`loadFile()`**. It invokes Error::setError **0x31a8ac**. Only parse success
continues to **`lua_pcall(L,0,0,0)` 0x84bc50**, followed by another setError.
The reader checks stream position versus length and reads into the 1024-byte
buffer through virtual+0x18. The stream constructor first copies resource bytes
into owned buffer storage; it does not retain a resource stream for parsing.

setError stores the status. Success copies an empty message. Failure reads
`lua_tolstring(L,-1,NULL)`, copies only through the first NUL, then calls
`lua_settop(L,-2)`, popping exactly one error object. This string-error domain is
verified; non-string Lua error objects that cannot become a string expose the
original null-string failure boundary, not a recovered safe fallback policy.

There is no manager in-progress insertion/cycle suppression. A file still
executing has not yet entered the per-script loaded set; a nested Include of
it finds the cached bytes and executes again. This consequence follows directly
from original order; the native probe exercises a bounded three-level recursive
file. It does not claim to execute original Lua core instructions in that case.
The outer chunk has already parsed before its protected execution, so a nested
cached-stream reset occurs after outer parsing. Nested failure is popped and
AddFile returns false; Include ignores false and its Lua caller resumes.
Mutations performed before a nested error are retained.

## Frozen additive integration proposal

Implement a **dedicated source Include binding**, rather than allowing generic
VM reentry. Suggested names (proposal, not implemented exports):

```c
typedef struct dh2_script_include_scope dh2_script_include_scope;
typedef int (*dh2_script_include_provider)(void *context,
    dh2_script_include_scope *scope, const char *requested_name);
int dh2_script_vm_bind_source_include(dh2_script_vm *,
    dh2_script_include_provider, void *borrowed_context);
int dh2_script_include_load(dh2_script_include_scope *,
    const void *bytes, size_t size); /* source Lua status, message separately */
```

The binder owns the actual C closure. It projects **every** supplied argument
through source Value conversion before applying the original first-string guard;
there is no arbitrary 16-argument cap. It constructs a borrowed capability valid
only through this particular synchronous Include invocation and only for its
current VM. `include_load` uses its private Lua state directly for source
lua_load/pcall0 and one-error-pop, preserving the caller's C-frame stack. It uses
the actual chunk name `loadFile()`, retains the current allocator budget, and
does not set `busy=0`, enter generic `protected_operation`, or enable arbitrary
GetGlobal/Call/Bind operations. Nested Include creates another properly scoped
capability; an expired or mismatched capability is malformed caller input.

ScriptOwner needs an additive explicit operation taking receiver identity,
filename, genuine persistent cache/provider services and this capability. Hold
the exact session across provider callbacks and execution, even if selected
active/pending identity changes synchronously. Reuse the original path/suffix,
loaded-set/cache gate and post-success insertion. The existing owner's `load`
already covers that order for a non-busy VM; factoring only its VM execution
operation is sufficient. It currently calls generic vm_load and cannot be used
for nested Include. Its service pointer is borrowed only during `advance` and
restored afterward, so an Include installed for later OnTimer cannot safely
capture that ephemeral pointer/view. Provider/binding context must outlive the
session and VM finalizers. No duplicate AIS ownership object is needed.

Delivered missing files/parse/runtime errors remain source false and zero-result
Include; malformed native capability or unsupported provider/allocation failure
must remain explicit diagnostics. Do not map a generic busy rejection to source
success. Do not add automatic cycle prevention, cross-script shared globals,
rollback or alias resets. Generic VM busy guards stay unchanged. Coroutine/main
state receiver differences and Include during VM-close finalizers require their
own source domain audit before being accepted.

## Actual Crypt external files

`cache-include-inventory.json` binds the exact supplied ZIP
`3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679`,
73 AI `.luac` entries, textual literal Include inventory, and these source files:

| Script | Bytes | SHA-256 | Top-level dependencies |
|---|---:|---|---|
| monster | 7393 | `84f07caaeb2c04f2024cc3e27d41f33806b2c53d6e8861bb3d8b371118fd0e1d` | FromFixed, GetProp, GetPyStruct, alias registrations |
| follower | 1901 | `2d281c3bc784bbc12356e37cff9e7bdcd44a13e1d45da1fcb21069b4fef48d9d` | function definitions and AddToVFTable registrations |
| rene | 9336 | `631e7137c6ba49fca45e6cf0b37d1e45e4f42af4ca120106274e7e28d5f56fc7` | GetPyCst, GetPyOID, alias registrations |

These three files have no literal Include calls in supplied source. They still
must be loaded after `_commons` into their own selected external script session
before InitVCB. The original InitVCB predicates inspect alias-main membership
(LuaScript::IsInVFTable **0x37c2a0**), not global function existence. Merely loading
commons into a fresh VM leaves those external aliases absent. Textual inventory
does not establish computed Include names or executed branches. Cache lookup's
case sensitivity is a genuine resource service; mixed-case requests elsewhere
in supplied files cannot be assumed absent from a case-folding resource backend.

## Base callback owner groups

Stage1 registers exactly the 33 callbacks in `catalog.json`, plus the two
CharAIScript globals RegisterAIState and ChangeAIState in the ownership catalog.
It **does not register SetBool/GetBool/SetString/GetString**. Full original symbol
scan also finds no such LuaScript/LuaManager wrappers. Source Value getBool,
getString and getNumber are conversions, not global owner services. Do not add
modern convenience globals to claim this catalog complete.

| Captured callback family | Genuine state/service boundary |
|---|---|
| Include | per-script path/loaded set/private VM; manager resource-stream cache above |
| Trace | shipping **0x37ee80** `bx lr`; no logging service |
| SetInt/GetInt | this LuaScript hash-keyed integer map+0x1c |
| AddToVFTable/PushVFTable/PopVFTable | this LuaScript alias main/backup maps+0x34/+0x4c, recording+0x64; existing proven alias module |
| ToFixed/FromFixed/MulFixed/DivFixed, BitNot/And/Or/XOr | source Value coercion, ARM arithmetic, ReturnValues projection; no owner backend |
| Rand/RandF | **Random::GetRandom clone 0x37bc2c** shared RNG producer; avoid replacing with host rand |
| GetPyCst | PyDataConstants::getConstant **0x4c4bdc** via Application design data |
| GetPyStruct/GetPyOID | PyDataArrays::GetOID **0x4bd640** via Application arrays, not a guessed Python VM |
| CallPyScript | ScriptManager GetIDFromName/IsScriptRunning/StartScript; accepts first string, no result; start only when ID exists and not running |
| GetNumPlayers/GetHostPlayer/GetHostPlayerLevel | Application PlayerManager live count/hosting player and Character level |
| GetHostPlayerDifficulty/GetCurrentLevelRange/GetGameScript/SetGameType | Application current Level and its authored table/game-script/type fields |
| GetGameObjectsByType | actual object registry/handles/PropertyMap class names, userdata binding |
| OnTargetDied | source registration name points to `_IsPlayerCharacter` **0x37ddc8**; compare current local Player to projected pointer, not an invented death service |
| PlayMusic/PlaySound/StopSound | authored sound name/ID lookup and genuine VoxSoundManager operations |

Exact callees/addresses are authoritative in catalog.json. The grouping is a
recovery map, **not** a proof that the bodies have all been implemented.

SetInt **0x37de5c** silently noops with fewer than two arguments; otherwise it
coerces first Value via getString, second via getNumber then signed float-to-int,
and calls LuaScript::SetInt **0x37d990**. GetInt **0x37ec14** returns no values with
zero arguments; otherwise getString→LuaScript::GetInt **0x37da30**→pushInteger.
Both use the actual signed-byte string hash and an unsigned hash-only tree.
**GetInt miss inserts zero through SetInt**, so the read mutates this owner map.
There is no second name comparison. These bodies are captured but not included
in the 188-case Include corpus. Their broader Value coercion must be audited
before implementation: numeric getString uses source formatting, pointers use
source 32-bit identity formatting, nil/bool use source literals. String
getNumber creates a temporary Lua state, pushes the string, invokes lua_tonumber,
then closes it; arbitrary host parsing is not a proven replacement.

## Evidence and reproducibility

`probe_original.py` → `include-original-probe.json`: **188 original instruction
cases** (80 Include guards/ignored booleans, 48 cached AddFile filename/gates/
insertion cases, 60 Instance parse/execution/error-stack cases), zero mismatches.
STL strings/sets/maps, cached stream reset and Lua load/pcall are explicit service
boundaries; this is not original full VM execution or full cache construction.

`run_host_probe.py` → `include-host-probe.json`: a **NEW feasibility harness**
linked to the existing genuine float32 Lua DSO, ASan/UBSan/LeakSanitizer clean.
Two private VMs share immutable cache bytes but separate globals/loaded sets;
18 actual nested Include callbacks, 33 stack checks, four handled errors, four
argument table projections, 11 resource requests and six cached resets. Returned
tables' metamethods are not projected because source load pcall requests zero
results; argument table metamethods still execute. A bounded recursive Include
is executed three times before loaded insertion. Missing file and parse/runtime
errors permit caller continuation; failed file remains cached but not loaded.
The report binds executable, actual loaded DSO and its historical source proof.
Current wrapper hashes are not substituted for that historical DSO.

Run original probes with the supplied Python environment; run
`run_host_probe.py` with WSL access. It compiles this new harness only and never
rebuilds the Lua DSO. No production Include binding, owner extension, ARM64
instruction differential, APK execution or complete external AI acceptance is
claimed by this handoff.
