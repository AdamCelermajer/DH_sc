# Pending and active AIS lifecycle

The new source module reconstructs the lifecycle wrappers and their projected
stores, gates, ordered calls and synchronous reloads. AIS allocation/deletion,
Lua, skill/spell lists, owner initialization and timer/property backends remain
explicit borrowed services. It does not replace AISPlayerIPhone with an
always-accept implementation or claim complete AIS ownership.

Original ELF SHA-256:
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
Manifests under this directory bind 38 captured functions. The vtable/string
capture independently binds the selected AISPlayerIPhone and CharAI vtables.
The previous `character-script-selection` proof owns exact name/row selection;
it selected AISPlayerIPhone for authored Player AI row44 `__player__`.

## Fields and API

`ScriptLifecycleState64` projects owner (`CharAI+4`), active AIS (`+0x1c`),
pending AIS (`+0x20`), borrowed external filename (`+0x30`), signed load step
(`+0x28`), AI_Tick/DoT_Tick timer IDs (`+0x10/+0x14`), delayed-loading byte
(`+0x24`) and scripted byte (`+0x2c`). Constructor `0x3ced50` initializes active,
pending, filename, delayed/scripted and stage to 0; timer IDs to -1 at
`0x3cede4/0x3cede8`. The owner is supplied by its separate binding caller.

`dh2_character_script_lifecycle(state,operation,argument,services)` exposes
the iPhone factory, LoadScriptProcess, StepInitScript, InitScriptProcess,
LoadNInitScriptProcess, OnInit, AI_ScriptInit, AI_ScriptCleanUp, OnInitPost and
OnInitFinal. The argument is the source boolean InitFinal on the two process
initializers. Requests are 32 bytes; responses carry a word and 64-bit identity;
the services object is 16 bytes. Callbacks are synchronous and may refresh live
fields. The source void wrappers use native completion status 1; LoadNInit
preserves the actual source 0/1 result. -1 is a native malformed-input guard,
not an original script acceptance predicate.

## Creation, promotion and initialization

SetScript<AISPlayerIPhone> (`0x3ccfe4`) tests pending `+0x20`, calls CharAI virtual
`+0x14` (OnTerminate), then **reloads pending**. If still nonnull, it invokes
that object's deleting destructor `+4` and stores pending=0 **after** return.
It allocates original ARM32 size `0xd8`, calls CharAIScript C2 with true, builds
the AISPlayer vector and zero fields, installs AISPlayerIPhone's vtable, then
stores the new object to pending at `0x3cd09c`. It does not publish active or
reset loading progress. The construct service returns an opaque native object
identity; `0xd8` identifies original allocation, not a requested 64-bit layout.
The deeper allocator, LuaScript constructor and vector ownership are not copied
as ARM32 memory into native objects.

LoadScriptProcess (`0x3cf1f0`) returns immediately for signed stage>6. With
delayed-loading=0 it executes seven stage iterations. With delayed!=0 it first
queries owner virtual `+0x28` (IsCharacter); true also runs seven iterations.
Otherwise StepQueryAvailableStep (`0x3cb854`) is called with `7-live_stage`; a
signed result<=0 returns, otherwise exactly that many iterations execute. The
query helper takes min(requested, global available-step count), subtracting it
from that global count; this budget backend is explicit here.

The actual jump table at `0x3cf24c..0x3cf264` is:

| Stage | Source body | Behavior |
|---:|---|---|
| 0 | StepCreateScript `0x3cf04c` | Existing exact selector, then pending factory |
| 1 | StepBindFunction `0x3cc278` | Pending CharAIScript::BindFunction `0x3d8f2c` |
| 2 | StepSetCharacter `0x3cc26c` | Pending SetCharacter `0x3d90f8` with live owner |
| 3 | StepLoadCommon `0x3cc218` | If scripted!=0, pending LuaScript.Load `_commons` |
| 4 | StepLoadScript `0x3cdf7c` | If filename!=0, pending LuaScript.Load(filename) |
| 5 | StepInitScript `0x3cb314` | CharAI OnInit; if live filename!=0, pending InitVCB |
| 6 | `0x3cf268` | Store active=pending; retain pending |

After each iteration the original reloads the live stage, increments it and
stores it. It decrements the captured remaining-iteration counter, so callbacks
can redirect later stages. It does not stop merely because live stage crossed
6 during that call. Assertions/log diagnostics for impossible stage values are
excluded from the gameplay projection. Entry negative stages are rejected by
the native contract; original debug assertions are not claimed as guards.

StepInitScript first invokes CharAI OnInit (`0x3d12b0`). That function queries
owner IsDead `+0x34`. If alive, it stops the old AI_Tick timer when ID!=-1,
looks up `CharacterDesign/AI_Tick` through design GetInt `0x4c4bdc`, and starts
timer `(duration,repeat=-1,event=0x33,user=null)`; the returned ID is stored only
after Start returns. It repeats for `CharacterDesign/DoT_Tick`, event `0x34`.
The owner for each Start is captured before its design lookup callback; timer
IDs and owner for the second stop are read live. If dead, timer setup is skipped.
Finally, any live pending AIS receives virtual `+8` (OnInit). Thus pending is
initialized while active can still be null. StepInitScript then reloads filename
and, only when nonnull, invokes pending virtual `+0xcc` (AISPlayer::InitVCB
`0x3dd884` in the selected class).

LoadNInitScriptProcess (`0x3cf3a4`) returns 0 if active is already nonnull. If
null it executes LoadScriptProcess, reloads active, and returns 0 if still null.
Otherwise InitScriptProcess executes and source returns 1. InitScriptProcess
(`0x3ce7c0`) calls owner `_InitHpMp` (`0x3b3a70`), SetSkillsAndSpells
(`0x3ce044`), UpdateAllSkills (`0x3d8894`), CharAI OnInitPost, and optionally
OnInitFinal. Those two CharAI wrappers (`0x3d0b80/0x3d0ba4`) check active and
forward its `+0xc/+0x10` virtual. They are live synchronous services that can be
bound recursively to this module's corresponding operations.

AI_ScriptInit (`0x3cfde4`) is a distinct caller: it performs the same timer setup
then checks active once, calls its OnInit, **reloads active** for OnInitPost,
then reloads it again for OnInitFinal. It does not repeat the null guard between
these latter calls; caller callbacks must maintain valid receiver lifetime.
ObjectManager::DoCharAIInit (`0x34064c`) traverses owner objects, checks their
IsCharacter virtual `+0x24`, then calls AI_ScriptCleanUp followed by
AI_ScriptInit. It is separate from initial pending publication.

## Selected class and ownership boundaries

`vtables-and-strings.json` verifies AISPlayerIPhone vtable `0x966cd0`:
Init `0x3dbe78`, InitPost `0x3dbe7c`, InitFinal `0x3dbe80`, Terminate
`0x3dbe84`, end-animation `0x3dbeec` and pre-attack `0x3dbef8` are inherited
AISDefault empty routines. OnUpdate is **not empty** (`AISDefault::OnUpdate`,
`0x3dc798`); numerous target/combat handlers inherit AISPlayer or iPhone-specific
bodies. Empty init/end methods do not imply an empty script implementation.

For row44 StepCreateScript sets scripted=1. Therefore the real `_commons` load
and BindFunction/SetCharacter services are required even for the builtin player.
With filename=0, external Load and InitVCB are skipped. Lua/common loading and
binding are not silently replaced with no-ops by this kernel. InitVCB, when
reachable, calls default VCB initialization then reads a Lua boolean and ORs
flag `0x400` into AISPlayer `+0xb8`; its backend remains explicit.

AI_ScriptCleanUp (`0x3cfd7c`) stops both stored timer IDs unconditionally, writes
both=-1, then if active existed calls skill cleanup, spell cleanup and the
**reloaded** active AIS Terminate virtual. It leaves active and pending intact.
CharAI OnTerminate (`0x3d11bc`) wraps this cleanup plus aggro/master/target
relationship teardown; the factory's OnTerminate is one explicit service rather
than a partial replacement of that ownership policy.

AIUnLoadScriptProcess (`0x3cc9dc`) has different semantics: it requires pending
nonnull and (force || delayed). It cleans skills/spells, destroys the two lists'
objects, then reloads pending and destroys it if nonnull. It clears filename,
pending, active, stage and scripted; delayed is retained. The list destruction
and full unload ownership body are captured but remain **outside the new module**.
Do not use ScriptCleanUp as an unload or treat factory replacement as an automatic
active swap. Replacing a previously loaded script needs the original caller's
unload/reset choreography and ownership backend.

CharAI::Update (`0x3cfbf4`) forwards to OnUpdate after pause/controller/owner
gates and target/master/aggro work. OnUpdate (`0x3d1050`) invokes the active AIS
`+0x18` first when present, then performs owner animation/visibility work. These
captured bodies do not promote pending; full update remains outside this module.

## Verification and integration

Optimized ARM64 versus original instructions: **1,480 cases, 4,339 ordered
service-entry snapshots, 13 atomic guards, zero mismatches**. Host ASan/UBSan
replays the same gold with the same counts and no findings. Sixty-four fixtures
run actual nested original OnInit/Post/Final wrappers through the live outer
frame; native callbacks invoke the corresponding source operations synchronously.
Cases cover alive/dead owner gates, byte values0/1/255, old/absent timers,
delayed budgets0/1/2/7, incomplete/published/already-active states, pending
replacement/nulling in Terminate, owner capture across design callbacks,
active mutation between init phases, stage mutation and identities above4GiB.

Gold `lifecycle-fixtures.bin` SHA-256:
`8ff18ce9715ab8307970fb9d724224209d106d66d20d60a995ec8558f8cf9202`.
Reports bind original, source/test, compiled library/binary, gold and captures.
This proof is standalone and does not bind an APK or establish live full AIS.

The narrow next integration is: bind the existing exact selector to the opaque
factory, execute the staged load, bind the Init/Post/Final callbacks to these
source wrappers, connect Tick design lookups and original TimerStore operations,
and preserve active/pending identity and lifetime. `_commons`/Lua and ownership
services must stay visibly unresolved until their actual source backend exists.
No renderer, CMake, Android, shared module or historical proof was changed.
