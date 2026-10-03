# Selected AIS update and Lua dependency evidence

`character_script_update.hpp/.cpp` reconstructs the complete selected
AISDefault::OnUpdate body and its AI_PauseUpdate stores. Timer Start and complete
controller Cmd_Stop remain borrowed synchronous services. The optional dispatch
mode reconstructs only CharAI::OnUpdate's active virtual prefix; its subsequent
owner animation/visibility body is explicitly outside this module.

Original ELF SHA-256:
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
Captures bind OnUpdate, its dependencies, collision producer, selected dispatch,
Lua manager/state calls, original Lua header/undump and callback consumers.

## Selected source body and exact order

AISPlayerIPhone's actual vtable `0x966cd0`, slot `+0x18` relative to its address
point `+8`, targets AISDefault::OnUpdate (`0x3dc798`), not a player-specific or
external Lua update method. The previous lifecycle vtable capture identifies
that pointer; this differential installs the actual original vtable and executes
CharAI's virtual instruction at `0x3d1074`.

At `0x3dc79c` the source reads unsigned AIS `+0xbc`. Values<=199 return without
effects. For values>199 it reads script owner `AIS+0x98`, clears `+0xbc=0` at
`0x3dc7b8`, and calls the owner's embedded CharAI (`owner+0x3c8`) with duration
1000. AI_PauseUpdate (`0x3cb748`) loads that CharAI's owner (`+4`), sets its
paused byte `+0x18=1`, and invokes owner's CharTimers (`owner+0x3b4`):
`Start(duration=1000, repeat=0, event=0x31, user=null)`.

After the synchronous timer call returns, OnUpdate **reloads script owner**
from AIS `+0x98`, reloads its controller `+0x378`, and tails Cmd_Stop
(`0x40559c`). It does not restore the counter or paused byte after callback
effects. There is no Lua, random selection, target seeking or animation
acceptance call inside this selected update body.

`ScriptUpdateState48` projects active identity, script owner, embedded AI owner,
controller, unsigned collision time and paused byte. The two owners are distinct
inputs because the original reads them at different points. Timer callbacks can
refresh the owner/controller projection before the Stop request. Services use
`ScriptUpdateRequest32 {service,duration_ms,repeat,event,subject,user_ref}` and
a 16-byte synchronous callback object. `dh2_character_script_update` dispatch0
executes the direct AIS body; dispatch1 checks active and executes the selected
virtual prefix. It returns native completion1 or malformed-input -1; these are
not fabricated script acceptance results.

The original controller Stop includes controller/debug enable gates then invokes
its policy object's virtual `+0x34`. That complete existing controller boundary
is borrowed here. The remainder of CharAI OnUpdate (`0x3d1078..0x3d11b8`) does
owner state/target/visual work after the selected AIS call. The original dispatch
fixture explicitly exits through its source epilogue at `0x3d10c0` after the
virtual prefix; no full CharAI update claim follows from this bounded probe.

## Pause expiry returns before forwarding

CharAI::RaiseAIEvent (`0x3cbb34`) dispatches event `0x31` through the jump slot
at `0x3cbc1c` to `0x3cbc8c`. At `0x3cbc90` it stores zero to the AI paused byte
`+0x18`; `0x3cbc94` returns directly. It invokes neither the active AIS nor the
FSM. The dedicated `dh2_character_script_pause_expired` entrypoint projects this
branch without passing event31 into the animation-event router. Original probes
execute the actual RaiseAIEvent dispatch for paused0/1/255 and observe no service
calls. Native validation rejects malformed state atomically before the store.

## Collision-time producer is not a generic clock

The only relevant nonconstructor `AIS+0xbc` increment found in selected/default
script functions is OnCollisionPersist (`0x3dbfa0`, captured). Its native body is
not substituted by this module. It first queries SM_IsMoving(false)
(`0x3c029c`): state4 or state0x13 accepts. It compares the collided object with
owner `+0x408`, checks its IsCharacter virtual `+0x24`, and has separate target/
player branches which can invoke source AI target/change helpers.

The accumulation tail (`0x3dc020..0x3dc058`) is reached for a character collision
or selected noncharacter types `+0xf4 == 2/0x15`, under earlier gates. It compares
AIS `+0xc0` with Application current frame word `+0x74`, skips repeated same-frame
work, and rejects owner AI paused byte `owner+0x3e0`. It stores the frame stamp,
captures the old counter, calls Application::GetDt (`0x31f66c`, integer
Application `+0x8c`), then adds it with 32-bit wraparound. The native update
therefore consumes collision-produced milliseconds; callers must not increment
it every frame regardless of contacts. Factory constructors initialize
`+0xbc/+0xc0` to 0. Complete collision/policy accumulation remains a source
integration task rather than an invented counter service implementation.

## Original VM/compiler signature

The original ELF contains the release marker at file offset `0x90f5b8` identifying
**Lua 5.1.4**, with the PUC-Rio author/copyright block. Symbols include
`luaU_header` (`0x85bc64`), `luaU_undump` (`0x85c320`), `lua_load`,
`luaD_protectedparser`, `lparser.c` and `lvm.c`. The original header routine was
actually executed, producing:

`1b 4c 75 61 51 00 01 04 04 04 04 00`

This is Lua signature, version0x51, format0, little-endian1, int4, size_t4,
Instruction4, lua_Number4, and integral-number flag0. Thus this binary uses a
float32 number configuration; standard double-number Lua must not be silently
substituted. Float wrapper symbols such as `Value::setNumber(float)` also exist.
The header establishes binary configuration, not full equivalence of every
Gameloft VM or binding body with an unmodified release.

The [official release index](https://www.lua.org/ftp/) identifies the exact
Lua5.1.4 archive and published SHA-256
`b038e225eaf2a5b57c9bcc35cd13aa8c6c8288ef493d52970c9545074098af3a`.
No VM archive was downloaded or integrated in this task. The
[official 5.1 undump source](https://www.lua.org/source/5.1/lundump.c.html) explains
the header field order, but that browsable tree is explicitly5.1.5 and is not
claimed to be DH2's exact release source. A future source backend should start
from confirmed5.1.4, reproduce float32 arithmetic/configuration, and verify the
actual wrapper/binding/manager semantics rather than selecting a current VM.

## Exact `_commons` cache entry and dispatcher dependencies

Supplied cache ZIP SHA-256:
`3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679`.
The exact AI entry is:

`com.gameloft.android.GAND.GloftD2SS/files/data/scripts/ai/_commons.luac`

It is **13,535 bytes of plaintext Lua source despite the .luac suffix**, copied
byte-for-byte to `lua-inputs/ai-commons.luac`, SHA-256
`20d34968e9983e14c24223085ab40d55d47f9387dfdbbf3ff7e6b39aea91252c`.
Its CRLF bytes are preserved; no decompiler or invented source was used.
The distinct skills `_commons.luac` is 11,291 bytes, SHA-256
`f3abf69124728bab3d344b56e8f3cb67127fbea4a8d1a7ab1d7401f2a2132e9d`.
Both entries and function/line inventories are in `lua-inputs/discovery.json`.
The 219 `.luac` script entries have zero Lua binary-magic headers; 217 are entirely
ASCII text and two have other bytes. The latter classification is not claimed
as a successful syntax/VM audit of those two entries.

The AI common source creates local animation-event and timer-event maps. Its
OnInit/Post/Final/Terminate/EndOfAnim functions are empty. `OnUpdate(timestamp)`
is also empty, with comments that it must be redefined to be called. OnTimer
looks up a registered timer callback and removes it after a nonlooping call;
OnAnimEvent calls the registered animation callback with saved reference data.
Those source facts do not mean loading the common script is optional.

Row44's builtin selection sets scripted=1, so lifecycle StepLoadCommon loads
this file. LuaScript::Load (`0x37b574`) forwards to LuaManager::AddFile
(`0x37b23c`). That body manages concatenated filenames, loaded-file caches,
resource streams, ScriptData and execution. It can append `.luac`, which is
consistent with the actual source-text filenames. Common loading and binding
are real backend services, not merely name inventory.

The selected class also inherits AISDefault::OnAnimEvent (`0x3dca50`): it first
calls Lua `OnAnimEvent` with event name and owner `+0x4f4` (CharAnimator triggered
event lag, animator `+0x58`), then performs native event-name/foot-effect work.
OnScriptTimer (`0x3dcc80`) calls Lua `OnTimer` with the timer ID. These remain
explicit consumers requiring genuine Lua/binding services; this update module
does not silently omit or emulate their callbacks. CharAIScript::CallStateUpdate
and CallStateConditions (`0x3d8eb4/0x3d8ea0`) only call their selected state's
function names when state pointer `+0xb4` is nonnull; neither is called by the
selected AISDefault OnUpdate body.

## Verification and next boundary

Original versus optimized ARM64 passed **600 update cases, 540 ordered
service-entry snapshots, 3 pause-expiry dispatch cases and 18 atomic guards**
(11 update plus7 expiry), zero mismatches. Host ASan/UBSan passes the same gold
with no findings. Cases cover unsigned threshold198/199/200/201, high-bit
and UINT_MAX counters, active absent/present, paused0/1/255, and synchronous
controller/owner/counter/paused mutations. Identities above4GiB are preserved.
Gold `update-fixtures.bin` SHA-256:
`953364066f36ca19c4d13d3c72967c0288db2b0fe3fbd788e62b2903bb7be70b`.
Reports bind source/test, original, actual compiled binaries and copied inputs.
No APK or complete Lua/AIS/player loop claim is made.

The direct next integration can bind the verified update to actual selected AIS
identity, native timer Start and existing source controller Stop. Collision
Persist must supply the source counter/frame gate. Timer event31 calls the
dedicated pause-expiry entrypoint and returns. Common-Lua loading, bindings,
animation/timer consumers and full unload ownership remain named source tasks.
No existing lifecycle/AI module, CMake, renderer, Android or shared source changed.
