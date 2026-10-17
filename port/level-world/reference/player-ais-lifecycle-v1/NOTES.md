# Borrowed Player AIS lifecycle composition

## Source and attribution

The original input is `libDungeonHunter2.so`, SHA256
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
`original-functions.json` pins the factory, constructor, lifecycle callers,
empty Player virtual leaves, and both Player vtables. This composition reuses
the maintained selection, lifecycle, constructor, default callback, VCB,
Session, preparation, update/use, vitals and Coordinator implementations.

Adam's `791e961b12233100b303038c961666834f4beb9d` V6 Player implementation
macro-includes the unchanged V3 implementation. Its initialization advances
the script owner, observes the actual active PlayerIPhone AIS, then runs the
source InitProcess services. That caller ordering informs this adaptation.
No private Adam VM, property or timer owner is imported, and this composition
claims no additional complete original function body.

The missing bounded inline constructor is recovered from
`CharAI::SetScript<AISPlayerIPhone>` at `0x3ccfe4`: construct CharAIScript with
skip binding true; zero b8/bc/c0; install Player dispatch; construct an empty
vector c4/c8/cc; zero d4/d0; install PlayerIPhone dispatch; publish pending AIS.
The zero-count vector allocation branch allocates nothing. Resource allocation
and actual Lua construction remain required caller operations.

## Actual native ownership audit

`model_renderer.cpp` owns a shared `NativeCharAIProjection` for the Player.
Its actual CharAI constructor appends once to the registration queue and its
association stores the actual Character owner. Graphics reload rebuilds the
registry around retained projections. The native Player skill runtime currently
prepares eagerly outside LoadScriptProcess, leaving the Player projection's
active/pending AIS and phase 28 unpopulated. The monster owner already runs the
real lifecycle kernel, retaining one VM and pausing its unfinished AI/DoT
timers explicitly after source initialization.

This module supplies the necessary Player caller over that existing ownership.
Native integration must move the real deferred Session creation or transfer
and real Owner.prepare/update/use creation into the two required backend
operations. The unchanged eager prepared Session cannot be presented as an
unbound constructor result. Constructing a second VM or independently publishing
phase 7 would violate this boundary.

## API and lifetime

`Runtime(Bindings)` borrows the actual CharAI state, caller-owned PlayerFields,
the existing Session's canonical AIS callback flags, actual AI declaration,
dispatch identities, Session/preparation/update/use slots, Save, Coordinator,
dead word, design constants and vitals Runtime/Storage. Its temporary lifecycle
projection is synchronized with the actual CharAI fields before and after
providers; it is not another persistent owner.

`Backend::construct_vm` must deliver or transfer the genuinely created unbound
Session with allocation size 0xd8 and skip_bind 1. `Backend::configure_skills`
must create and prepare the sole Owner with owner Character and AI identity
equal to the published active AIS, and publish the real update/use adapters.
Both operations are required; missing, nonzero or throwing providers fail.

`initialize(final, result, error)` reuses the existing source lifecycle kernel.
Fresh Player selection binds AIS functions, associates Character and binds its
functions, loads real commons, executes original OnInit timer ordering and the
proven empty inherited AISDefault initial callback, then performs actual
pending-to-active publication and phase advancement. Only then does it run
vitals, configure, update, Post and optional Final. Player VCB runs through the
actual preparation provider in the same Session.

`load(result, error)` dispatches exact `script_load_process`/LoadScriptProcess
0x3cf1f0. It delivers constructor/binding/commons/OnInit and publication, then
returns wrapper completion 1 without vitals, preparation, skill update or
Post/Final. Its original phase>6 guard returns completion without services.
`initialize_process(final, result, error)` dispatches exact
`script_init_process`/InitScriptProcess 0x3ce7c0. It runs vitals, configure,
update, Post and optional Final at the caller's explicit boundary. It has no
source active or replay guard; the native InitPost caller must deliver that
operation once. All three methods share the same busy/failed latch and control
alias checks. Calling compatibility `initialize` after `load` returns source 0
through the active guard and does not deliver the pending InitProcess.

The split permits actual Character InitPost profile/equipment/property/slot
producers between source load and final InitProcess. In original InitPost,
LoadScriptProcess is called at 0x3b5010; host skill-slot initialization branches
from 0x3b51b8 to 0x3b54d4 and resumes at 0x3b51bc, after profile/equipment and
property recalc/cache. Revive/_InitHpMp and final InitScriptProcess at 0x3b54c8
occur later. These out-of-line block addresses do not denote temporal order.
This API adds none of those missing profile/grant bodies.

The original phase 7 publication precedes InitProcess. A required configure or
update failure therefore leaves phase 7 and active/pending AIS intact. Status
failed is latched and does not replay or roll back source effects. Phase 7 alone
does not certify full initialization. An already active source guard returns
source_return 0 without replaying preparation, callbacks, timers or HP/MP.

All borrowed backing must remain alive through dispatch and VM close. Retire
this Runtime before replacing borrowed declarations or their native table
backing on graphics reload. Close the existing VM before preparation instances
or callback receivers retire. Providers must not reenter, rebind or destroy the
borrowed owners. Runtime owns no VM, property sheets, timers, Save or AIS.

## Executed evidence

The host gate loads actual cache AI declarations, design ticks and the three
Player base classes. It links the actual selected level-world and script-runtime
DSOs with one script runtime and exercises real Character constructor and
association, same Session AIS/Character aliases, real commons, actual Debug
backed HP/MP, real preparation/VCB and update/use owners. Explicit skill/faery
update overlays isolate lifecycle ordering; unchanged gameplay callback delivery
is a separate native gate.

The gate completed five lifecycles: three real Player classes, one dead
owner with Final false, and a separate load/InitProcess with Final false.
Four failure cases cover constructor, configure after publication, a required
update callback failure caught by Lua pcall, and split-phase configure failure
with the shared failure latch across all methods. Seven guard cases cover
provider reentry, invalid/aliased output, and the declaration's own Script
string as error output before it can mutate borrowed authored input. Split
tests check publication without healing/preparation, a real saved-row writer
between calls, and repeated load/compatibility calls without duplicate VM,
healing or preparation. No profile or grant caller credit is inferred. It checks
actual AI/DoT repeat -1 timers, source duration/order/reference, retained active
guard and same VM. All eleven original constructor scalar words and the empty
registry agree with the actual ARM factory capture.

`original-capture.json` executes original ARM factory, CharAIScript, zero-count
vector/allocator words and pending publication (87 distinct pinned words).
A second capture executes original LoadNInit, LoadScriptProcess, OnInit and
InitProcess with the original Player vtable's empty Init/Post/Final leaves
(266 distinct pinned words). Source phase stores and pending-to-active
publication are not intercepted. Selection, allocation, Lua construction,
binding/files, vitals/configure/update and design/timer allocation are explicit
service fixtures. Both original runs execute no imported functions.
The same pinned original source also executes a separate LoadScriptProcess
followed by InitScriptProcess, plus repeated load and active LoadNInit guards.
It observes exactly one constructor, one vitals and one configure service.

The report records all compiled inputs before/after, actual compiler commands,
DSO imports and binary hashes. The final split-phase gate compiles this module
exactly once in the actual selected level-world DSO, with one script runtime
translation unit. Host and original proof do not imply an Android build or
live gameplay delivery.

## Remaining mandatory providers

The real source OnInit creates unpaused repeating Coordinator timers 0x33 and
0x34. This module does not consume or pause them. Native AI_Tick and DoT_Tick
expiration services remain unfinished and must stay explicitly gated until
their actual source handlers are bound. Pending AIS replacement requires its
source termination and destruction owner and currently fails when reached.
Full native initialization also requires actual skill callback providers and
the existing runtime lifetime cleanup. These are integration boundaries, not
readiness flags.
