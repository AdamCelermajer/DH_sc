# Character.Update startup prefixes

The two new entries restore bounded original startup gates around the controller
boundary. They do not implement the complete Character frame, the shared queue,
or the nonempty bot target/controller body.

* `dh2_character_update_startup`: Character.Update entry through the source
  Application game-stats+5c increment, stopping before controller v8.
* `dh2_character_update_after_controller`: call only after genuine controller
  v8 delivery; restore isABot diagnostics and its proved early-return branches,
  stopping before optional TimerUtil/CharTimers.

The source ordering matters: **isABot is after controller Update**, at
`0x3abfd4..3ac010`; it is not a pre-controller startup gate.

Original ELF SHA256:
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
`original-functions.json` captures11 complete bodies, including Character.Update
`0x3abe98` (3776 bytes), CanUpdate, the exact helper names, current-Level and
real-time getters, potion setter and queue unload/control forwarding helpers.
Capturing a helper does not imply its entire native ownership is implemented.

## Gates and reloads

Source release profiler calls3136b4/3136b8 are genuinely empty. Debug calls are
not: each performs load337888 → string constructor3140ec → query337a88 →
destructor318254. The module retains the literal's name buffer through those
typed synchronous service phases. The query result is captured before destruction.
These services are mandatory and may not supply a hardcoded false result.
Native scalar name ownership is bounded and is not a complete legacy std::string
heap/allocator ABI reproduction. A caller can bind the retained DebugSwitches/
file owner for load/query; the old incomplete owner's existing-file rejection
remains failure rather than publishing invented values.

`KillPlayerOne` true calls GetPlayer(0,false)36e744, compares Player+660 to this
Character, then reads signed resolved HP property36 at1088. HP>0 supplies raw
HP-200 to genuine SetProperty36 (3e07a0), with no caller-side zero clamp. HP<=0
supplies0, calls current IsDead virtual34, then, only when false, current
controller Cmd_Kill(NULL,false)40570c. **This function is Cmd_Kill, not Stop.**
The property setter owns its actual recomputation/cache rules; the scalar HP
projection must be refreshed by the genuine provider when needed.

`Give50Potions` true calls current IsPlayer virtual28; true then calls
ItemInventory.SetPotionQty(50)3ffc40. Neither debug branch bypasses CanUpdate.

The complete frozen native CanUpdate helper executes next. A genuine false result
sets stage `update_ineligible`. It still retains all already delivered debug,
property and node200 effects. Native malformed/provider failure is separate from
the source boolean; no failed predicate is treated as false eligibility.

When eligible, read actual current FSM state through SM_GetState3c01ac:

1. First result0 goes directly toward controller.
2. Otherwise read again; result12 also skips deferred startup.
3. Otherwise read again; result2 skips deferred startup.
4. Otherwise load interaction1480; nonzero skips deferred startup.
5. Otherwise load active AIS3e4; nonnull skips deferred startup.

The repeated reads are preserved and provider-visible. A callback may change
the next result or the live interaction/active fields. No snapshot of state3
replaces this choreography. `State0` here is a real current state, while source
SM_GetState's null-current result is unsigned-1 and follows the nonzero branch.

Remaining actors query Application.GetCurrentLevel31f594 and its word3c.
Word29 changes queue count threshold from8 to24. The comparison is unsigned
count>threshold. If queue first equals its sentinel, eviction is skipped even
when the count word is large; the projection uses first0 for the sentinel.

An eligible queue below its eviction threshold calls the genuine synchronous
LoadNInitScriptProcess(true)3cf3a4. Source result0 still proceeds toward the
controller; it is distinct from provider failure. Result1 triggers IsMonster
3a3064 → IsMiniBoss3a3144 → IsBoss3a3158, with the actual short-circuit branches.
Ordinary monster, offline receiver byte5==0 and reloaded delayed-load byte3ec
nonzero reads Timer.getRealTime60b0cc, then reaches the unowned registration
boundary. A load callback can publish active AIS and change live fields before
these later loads; these effects survive a subsequent boundary failure.

After the source gates finish, increment the actual borrowed Application
game-stats+5c word with 32-bit wrapping arithmetic and return controller-ready.
The helper does not invoke controller Update. A successful startup call followed
by a failed controller must not continue with the post-controller helper.

## Exact remaining queue boundaries

When count>threshold and first is not the sentinel, original captures its first
RB node, reads that node's Character+14/current controller+378, then calls
Cmd_Kill(NULL,true)40570c. It **reloads node+14 after that callback** and calls
Character.UnLoadScriptProcess(iterator,true)3a7b24. That helper erases the actual
node via336004/delete708f00, decrements queue count+10, THEN tailcalls
AI_UnLoadScript3cc9dc. Node identity/lifetime, callback reentry and active/script
unload ownership need a real queue owner; an arbitrary accepted removal callback
would hide them. This first module returns unavailable before eviction starts.

Registration after getRealTime is inline Character.Update code, not a separate
accepted service call. It searches the shared RB tree using **signed** time-key
comparisons. Existing key overwrites mapped Character+14; absent key constructs
an actual node through3aa8b4 and RB insertion paths. It neither appends arbitrary
FIFO entries nor deduplicates by Character identity. Those operations are not
owned here. The module returns unavailable after the real-time query, before
registration. Valid ordinary delayed Crypt actors can reach this boundary after
their genuine Init/publication; they are not controller-ready in this module.

Upstream queue projection count/first is borrowed. It is not a native decoded
container and supplies no erase/insertion authority. Unknown providers and these
explicit ownership continuations fail rather than becoming successful no-ops.

## After-controller prefix

Source isABot debug false goes directly toward TimerUtil. True queries actual
online byte5; offline also goes there. Online then calls IsPlayer; false goes
there. True calls GetPlayerForCharacter(owner,false)36eea8, reads bot byte66c,
and if nonzero calls that getter again, then reads signed player number678.
Positive number calls GetPlayer(number-1,false)36e744 and reads returned660.
Null Character returns toward timers. Nonnull Character reaches the nonempty
bot stats/level/item/target/distance/controller continuation; this module returns
unavailable there. It supplies no guessed HeadTo/Attack or empty bot controller.

## Native contract

`UpdateStartupOwner64` borrows the same CanUpdate owner and its services, current
active/controller projections, current application counter, shared queue-header
view, signed resolvedHP and raw delayed-load byte. `UpdateStartupPlayer24` borrows
actual player identity/Character660/player-number678/bot66c. All captured
projections and contexts are pinned across calls and synchronous callbacks.

Typed services receive source operation, raw argument words, owner/controller
subject and scoped name, and return word/identity separately from delivery.
GetPlayer arguments are index/bool; SetProperty arguments are36/raw computed
word; SetPotions argument50; Cmd_Kill subject=currentcontroller and argument0
denotes boolfalse/targetNULL; LoadNInit argument1 denotes final=true.
GetCurrentLevel returns its actual word3c; online returns the actual raw byte5.
GetPlayer/GetPlayerForCharacter return borrowed `UpdateStartupPlayer24*`, not
just supplied booleans. SM_GetState and IsPlayer/Dead/type/deferred services must
bind genuine kernels or caller-owned source services. The test fixtures explicitly
do not claim those deeper bodies as reconstructed by this module.

Status0 means this bounded prefix completed; stage0=ineligible,1=controller-ready,
2=timers-ready. Status1 malformed,2 reached required provider/continuation
unavailable,3 delivery failed. Stage changes only on complete. The caller stops
the enclosing actor update after failure; no rollback of debug/property/script
publication or timer/scene effects, no silent retry to skip a now-active gate.

Body worker's new `CharacterDeferredScript(session).load_and_init(true,services)`
can deliver LoadNInit: negative means delivery failure; returned0/1 supplies the
source result. Its required refresh_vitals/configure_skills/update_skills and
actual selected Post/Final remain genuine services. Refresh this owner's live
active AIS after the helper. Merely supplying result1 or publishing a prototype
script early is not this production binding. The independent helper proof stays
separate from this prefix's provider-fixture corpus.

## Evidence and reproduction

Original prefixes versus independently compiled Android ARM64 O2:
**1,404 comparisons /14,451 ordered prefix services /2,616 ordered actual
CanUpdate services**, zero mismatches. The cases include all supported debug and
current0/12/2 gates, live repeated-getter mutation, active publication during load,
HP signed extremes, potion/player branches, count thresholds8/24 and post-controller
online/bot/player-number branches. **Nine cases stop at declared unavailable
queue/bot continuations.** These compare the delivered source prefix before
that boundary, not a fabricated source failure or whole-frame return.

The original harness enters Character.Update normally for startup. It enters
the original post-controller code with the captured caller/register inputs for
that separate phase. Both deliberately stop before controller/timers, or at the
unowned source branch. No Character epilogue/full-frame completion is inferred.
CanUpdate still executes its actual complete instructions on each side.
Original debug string, property/inventory/current-manager and LoadNInit bodies
are declared provider fixtures in this corpus; native scoped name buffers are
checked at each typed construct/query/destroy operation.

Host replay under ASan/UBSan/LeakSanitizer passes the same1,404 cases,7,848 live
string reads,20 malformed/unavailable/delivery checks, zero findings. No central
world DSO, renderer, CMake, runtime, Android project or device was changed.
Reports `character-update-startup-{arm64-differential,host-audit}.json` bind
source/corpus/original/O2 and sanitized executable bytes.

Repository-root reproduction with pinned Python/oracle PYTHONPATH:

```
powershell -File port/level-world/tools/build_character_update_startup_oracle.ps1
python port/level-world/tests/character_update_startup_differential.py
python port/level-world/tests/character_update_startup_host.py
```

Suggested central host target `character_update_startup_audit` compiles
`tests/character_update_startup.cpp`, links the world DSO and takes one argument:
`reference/character-update-startup/startup-prefix-fixtures.bin` relative to
level-world. Controller/timers/AI/FSM/Animator/GameObject/Character-tail and
upstream eligibility producers remain separate mandatory stages.
