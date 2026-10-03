# Character timers, attack gate and animation stance

Read-only discovery against original ARM ELF SHA256
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
The focused manifest identifies 30 captured routines by symbol, address, size
and code hash. `reference/original-functions.asm` contains their instructions.
No existing production source, build, APK or device state was changed here.

## Attack delay clears through the state-event prefix

Attack focus `0x3c404c` writes policy 0x2341 and sets Character+0x528 bit 0
when GetAttackDelay is nonzero. The getter `0x3a3438` selects the character's
AI-properties row (stride 0x44) and reads its unsigned delay at row+4. Attack
blur `0x3c3f74` first checks that getter; if nonzero it calls the getter again
at `0x3c4014`, then starts a timer at `0x3c4030` with
`(delay_ms, repeat=0, event=0x2a, user_ref=null)`, before pinning the physical
object at `0x3c3ff0`. This is a timer on leaving Attack, including a same-state
blur/focus transition. Do not start a countdown solely when Attack focuses.

CharTimers is embedded at Character+0x3b4. State-machine word+0x2c is the
same storage as Character+0x528 because the machine is at Character+0x4fc.
The original synchronous expiry chain is:

| Original address | Operation |
| --- | --- |
| `0x3db7f0` | CharTimers calls Character.RaiseEvent(timer.event, timer-slot pointer) |
| `0x3a4d5c..68` | Character forwards non-0x36 events to embedded CharAI+0x3c8 |
| `0x3cbfbc..d0` | Event 0x2a calls AI virtual+0x8c, then forwards the same event/payload to the machine |
| `0x3d0c7c` | CharAI.OnAttackDelayExpired delegates to current AIS virtual+0x8c, if present |
| `0x3dbee8` | AISDefault.OnAttackDelayExpired is an empty return |
| `0x3cbc60..74` | CharAI forwards to CharStateMachine.RaiseStateEvent at Character+0x4fc |
| `0x3c584c..58` | RaiseStateEvent clears machine+0x2c mask 1, then joins ordinary event handling |
| `0x3c5700..2c` | Current state's OnEvent is called after the gate clear |
| `0x3c5730..40` | Registered transition-map lookup follows current OnEvent |

Event 0x2b clears mask 2 at `0x3c583c`; event 0x2c clears mask 4 at
`0x3c582c`. Preserve unrelated bits. The AI controller gate at
`0x3cbd04..2c` can skip the AI virtual callback when global blocked or
controller+8 locked, but still forwards to the machine and clears the gate.
Controller+9 forced bypasses those two gates. Thus the attack gate must not
remain set just because the AI callback is suppressed.

Character's normal frame order calls CharTimers at `0x3ac02c`, CharAI at
`0x3ac034`, state machine at `0x3ac03c`, animator at `0x3ac048`, then GameObject
at `0x3ac054`. The parent `character-frame/NOTES.md` captures those caller
instructions. Timers must run before AI/FSM/animator, after the world step in
the surrounding frame sequence. Callback delivery is synchronous.

## Timer storage and update semantics

The original `_Timer` is 32 bytes: vptr+0, unsigned ID+4, signed repeat+8,
unsigned duration_ms+0xc, unsigned elapsed_ms+0x10, active byte+0x14,
paused byte+0x15, event int+0x18, user_ref pointer+0x1c. `GetRef` at
`0x3db290` reads user_ref. The expiry payload is the timer object pointer,
not user_ref; the recipient may query GetRef. A 64-bit source struct must
widen pointers and preserve these logical fields rather than ARM offsets.

CharTimers.Ctor `0x3dbb0c` clears owner/vector storage, then its reserve
helper `0x3dba84` reserves 20 slots. `_findTimerSlot` `0x3dbd70` scans for
the lowest inactive slot. If none exists it appends a timer whose ID is the
previous slot count. `TMR_Start` `0x3dbe24` initializes active=1, paused=0,
elapsed=0, duration/repeat/event/user_ref and returns the retained slot ID;
it returns -1 if slot lookup fails.

`Update` `0x3db640` returns immediately when
`ScriptManager::s_inst + 0x30` is nonzero. The actual GOT entry is 0x9964b8,
target 0x9a5fdc, resolved by relocation symbol, not a guessed pause flag.
Otherwise it obtains Application.GetDt milliseconds once at `0x3db6a4` and
snapshots the initial slot count. It reloads vector begin for each index.
Inactive or paused slots are skipped. Active unpaused slots add dt with
unsigned 32-bit wrap. Catch-up continues while active, duration !=0, and
unsigned elapsed >= duration.

When repeat is zero, Update clears active **before** raising the event and
leaves elapsed unchanged. When repeat is nonzero it subtracts duration;
positive repeats decrement, negative repeats remain unchanged. A positive
repeat value N therefore fires N+1 times overall; negative means indefinite
repetition. Duration zero never fires. Event -1 becomes event 0x29 at
`0x3db860`, still passing the timer-slot pointer.

Callbacks can mutate timers synchronously. The source checks paused only at
the start of processing the slot, so pausing from an expiry callback does not
stop the current catch-up loop. Stopping does. Restarting the current slot
resets elapsed/duration and changes that loop immediately. Activating a later
already-existing slot lets it receive this update's dt when its index arrives.
Appended slots beyond the initial count wait until the next update. During
catch-up the source retains the old slot pointer while callback delivery
recomputes payload from current vector begin. Vector growth/reallocation from
a nested callback is an explicit allocator/lifetime boundary, not established
by this bounded fixture. The original 20-slot reserve avoids growth in the
narrow attack-delay workload; do not silently promise unlimited safe reentry.

Pause/Resume `0x3db298/0x3db2b8` set/clear paused for an in-range unsigned ID;
Stop `0x3db2d8` clears active; StopAll `0x3db2f8` clears all active bytes.
`TMR_TimeLeft` `0x3db344` actually returns raw elapsed and duration through
its two output pointers for an active in-range slot, including paused slots.
It does not compute remaining time despite its name.

## GetAnimStance: bare candidate 5 clamps to default 0

`Character::GetAnimStance` `0x3a53e0` calls Character virtual+0x28 IsPlayer.
Nonplayers select candidate 0. Players use **Character+0x37c ItemInventory**
as the receiver for each following query, in this exact priority:

| Query address | Predicate | Candidate |
| --- | --- | --- |
| `0x4000c8` | HasStaff | 3 |
| `0x400080` | HasBow | 4 |
| `0x40019c` → `0x400158` | IsDualWielding / HasOffHandWeapon | 2 |
| `0x4001a0` | HasTwoHander(false), explicitly r1=0 | 1 |
| `0x3ffe8c` | HasMainHandWeapon is false | 5 |
| otherwise | Main-hand weapon exists | 0 |

The actual constant query at `0x3a5424` passes string pointers 0x8c3298
`AnimStances` and 0x8c32a8 `COUNT_IPHONE`. At `0x3a5428..30` it performs a
**signed** candidate < count comparison; it returns the candidate if true,
otherwise 0. The authored constants cache count is 5, as already captured in
`live-animation-selection/NOTES.md` (same ELF and cache binding). Consequently
an unarmed player's candidate 5 returns **0**, not 5. The current authored
bank of offsets 0..4 needs no bare base+5 extension for this source getter.
If an explicit future constants service supplies count >=6, bare 5 becomes
reachable; that would be different source input, not today's Android binding.

The inventory getters read the current equip-set selector at Inventory+0x2e
through GetCurrentEquipSet `0x3fc6a8`, then equip-set pointer vector+0x14
with 12-byte entries. Staff/bow query main-hand item properties+0x94 for
category 5/4. HasMainHandWeapon only checks main-hand pointer presence.
HasOffHandWeapon queries offhand item properties+0x58 and excludes type 6;
a shield does not select dual stance. HasTwoHander(false) examines main-hand
properties+0x58/+0x68 and owner Character resolved property at +0x1324,
including the original special permission path. The complete inventory and
property producers remain supplied service facts for the proposed pure getter.
Do not replace these with inferred visual equipment names.

## Original execution evidence and next bounded implementation

`.local-inputs/character-timer-discovery/probe.py` executes the captured
original instructions in Unicorn and asserts logical state/call order. The
published `probe-report.json` passes 1,856 update cases / 1,488 expiries,
six timer-control observations, four synchronous reentry fixtures, 16 full
timer→Character→AI→FSM→Attack OnEvent chains (including null/default AIS and
blocked/locked/forced combinations), 30 gate-mask projections, and 576 stance
priority/clamp cases. Zero mismatches. Fixtures supply clock/debug logging,
inventory predicates/constants, and an empty registered-transition map; source
timer arithmetic, capacity-present slot append, AI forwarding, Attack OnEvent
and gate clearing execute. General allocation and full AI are not emulated.

Original-observation corpus `probe-gold.json` SHA256:
`fdc2415e2b41acac78a14ea4fd33118dbe78f46765f1b45d7ec3e8a0e0bc3f5f`.
It contains the 1,856 timer, four reentry, 16 expiry-chain, 30 gate-projection
and 576 stance observations; control smoke observations are report-only.
Manifest SHA256: `bdb56fb9d288bcacf46ba27e22df04252cf76fcdabddc41ca902f8369353e9a9`.
This is original discovery evidence, not native ARM64/packaged parity.

The next narrow module can expose TimerState with uint32 duration/elapsed/ID,
int32 repeat/event, active/paused bytes, uintptr_t user_ref, and synchronous
expiry callback `(context, event, borrowed_timer_pointer)`. Start/Stop/Pause/
Resume/Update should preserve lowest-free reuse and the initial-count rule.
Pass explicit integer dt_ms and ScriptManager+0x30 as source input facts.
Use owned storage reserved for the original 20 slots and document the growth
boundary before claiming nested allocating callbacks. The actor StartTimer
service can consume the existing Attack blur request directly.

Add the event 0x2a/2b/2c bit-clear prefix to the bounded state-event module
**before** current OnEvent/transition handling. The expiry adapter must call
the optional source AI expired service before forwarding to that prefix;
controller gating skips only the AI service. This closes pending_start_timer
and attack_gate without introducing a full AI FSM.

Expose a separate pure stance query with explicit IsPlayer, HasStaff, HasBow,
IsDualWielding, HasTwoHander(false), HasMainHandWeapon and signed stance_count.
Bind count5 from the authored constant cache and equipment predicates from
the resolved current equip set. Execute the supplied corpus against native
ARM64 and the eventual packaged libraries before extending checkpoint claims.
