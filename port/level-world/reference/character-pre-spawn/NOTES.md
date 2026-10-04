# Original PreSpawn17 behavior and startup ordering

The original library is SHA256
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
`methods/original-functions.json` and its complete assembly capture the four
PreSpawn methods and the animation-index, stance, Spawn predicate and
SetSpawnState dependencies. The native API executes the complete four method
bodies with explicit deeper services; it does not substitute a successful
Spawn, animation or physical backend.

| Method | Original address | Size | Native operation |
| --- | --- | --- | --- |
| Focus | `0x3c688c` | 404 | `state_method_focus=0` |
| Blur | `0x3c67d4` | 184 | `state_method_blur=1` |
| Update | `0x3c0070` | 4, `bx lr` | `state_method_update=2` |
| OnEvent | `0x3c2810` | 312 | `state_method_event=3` |

These operation numbers differ from StateOwnerOperation. Translate them
explicitly when delivering the corresponding source method address from the
owned StateInfo. Keep the original outer StateOwner transition/event machinery:
this body does not select a current StateInfo or deliver its Character event1d.

## Recovered effects and reload points

Focus first writes Character flags `0x1300`. It captures the current 160-byte
CharAnim row table before querying `GetCharAnimTableId(0x3a3228)` and tests word25
(`+0x64`) against `-1`. It then reloads the table pointer **before** a second live
index query. The direct branch reads word25 again and calls ANIM_Set. The
fallback reads word32 (`+0x80`), captures that base before the constants query,
queries `AnimStancedAnim/SL__LIST_IPHONE`, and adds actual GetAnimStance only if
mask bit1 is set. Addition preserves wrapping signed-word bits. It calls
ANIM_Set followed by ANIM_SetSpeed with float32 positive zero on this fallback.

Both branches then call SetPhysicalObject(NULL,false). After that call the source
reloads the raw Character byte `+0x13e4`; zero calls Character virtual+0x40 with
false. DisableCollisions is last. The virtual enable/disable service must retain
its genuine Character meaning rather than be inferred from physics presence.

Blur calls Character virtual+0x40(true), Revive(NULL,false), then
EnableCollisions, in this order. Update is the original empty method, handled by
the previously proved empty-method helper.

Event `0x28` compares the borrowed payload string against the original literal
at `0x8c4b70`, exactly `is_interactive`. Only an exact match ORs flags with
`0x2000` and calls InitPhysicalObject. Event9 starts a by-reference next state at1
and invokes actual CSM_Spawn(`event9`, payload, `state17`, next). A false return
ends this body. An accepted next1 calls SM_SetSpawnState(true,false), then reloads
Character word `+0x400`; kind3 clears word `+0x24` of the live pointed object at
Character `+0x3fc`. These fields are explicit projections, not owned AIS state.
Caller must refresh them after the SetSpawnState service.

An accepted next other than1 follows the original debug assertion policy:
policy1 delivers the diagnostic at source line114; policy2 deliberately stores
through NULL in the original. Native returns `-3` for that invalid source
boundary. Other policy values end the body. All other events have no effects.

Required source calls are ANIM_Set `0x3cacb0`, ANIM_SetSpeed `0x3c93fc`,
SetPhysicalObject `0x394bf8`, DisableCollisions `0x3949b0`, EnableCollisions
`0x394a3c`, Revive `0x3a59ac`, InitPhysicalObject `0x3b4088`, CSM_Spawn
`0x3ad2e4`, and SM_SetSpawnState `0x3c2734`. The latter has genuine spawn-count
and selection logic and remains a required backend; a generic transition to1
does not implement it. The genuine GetCharAnimTableId fallback and GetAnimStance
producer may be reused from their existing recovered modules.

## Native API and lifetimes

`character_pre_spawn.hpp/.cpp` exports
`dh2_character_pre_spawn_body(PreSpawnState48*, operation, event, payload,
PreSpawnServices16*)`. It requires current17 on entry, genuine decoded table
rows/count, nonzero Character identity, zero reserved fields, and the borrowed
service callback. `PreSpawnRequest32` carries full 64-bit Character/payload
identities. The source Character and State binding are captured for the whole
body, matching source held registers. Table pointers and the stay/kind fields
reload only at the recovered points. Preserve any earlier table backing across
synchronous callbacks even when a service changes the live table pointer.

The callback returns0 for delivered services. Nonzero stops at the delivered
prefix with `-2`; effects are not rolled back. Native return1 completes the body,
`-1` rejects malformed inputs before mutation, and `-3` records invalid source
row/null/assertion boundaries. Service callbacks may synchronously reenter the
owner. The borrowed contexts, State and captured table backings must remain
alive through that reentry. This API owns no Character, animation, body or AIS.

To integrate, route StateInfo17 Focus/Blur/Event from the existing behavior
adapter's required remaining-method service into this body, supplying actual
Character/playback/physics/Spawn services. Route its Update through the empty
method dispatcher. Do not replace initial state selection or later transition
with direct `state.current` assignment. Spawn1 and other nonempty methods remain
explicit required backends, including any event9 continuation selecting them.

## Source startup ordering

`producers/` captures Level::_LoadProcess `0x3f6990`, _LoadCharStates `0x3eff98`,
Character::InitPost `0x3b4d60`, and CharAI::InitScriptProcess `0x3ce7c0`.
`object-init/` captures ObjectManager::InitPost `0x34552c`.
`initialization-order-probe.json` binds actual ARM32 dispatch/retry/branch probes
to these captures and the original ELF. This is not a complete Level loader
execution.

The source Level stage at `Level+0x130` selects jump-table entry
`0x3f6a38+stage*4`. Stage10 entry `0x3f6a60` reaches `0x3f70cc` and calls
ObjectManager::InitPost at `0x3f7118`. A zero result retries at `0x3f7114`;
nonzero advances the stage. The actual probe returns0,0,1 and observes all three
calls. Stage18 entry `0x3f6a80` reaches `0x3f76c8`, calls _LoadCharStates at
`0x3f7700`, then increments the stage. Thus normal completed InitPost stage10
precedes initial Level state selection stage18.

ObjectManager invokes object virtual+0x1c at `0x3457f8`. Original Character
vtable `_ZTV9Character=0x965f30`, address point `0x965f38`, resolves this slot to
Character::InitPost `0x3b4d60`. Its tail calls Revive(NULL,true), reloads
properties, then tests Character byte `+0x3ec` (embedded CharAI+0x24). When zero,
the branch at `0x3b5440` reaches `0x3b54c4`, passes embedded CharAI
`Character+0x3c8` and boolfalse into InitScriptProcess at `0x3b54c8`. Nonzero skips
that call. Both continue to AddToGroup `0x3d37d0`.

InitScriptProcess reloads Character properties, sets skills/spells, updates all
skills, then invokes CharAI virtual+0x0c (OnInit). Booltrue additionally invokes
virtual+0x10 (OnInitPost); boolfalse skips it. The actual ARM probe proves both
ordered branches with explicit deeper return-only services. OnInit can use the
genuine existing lifecycle to publish the active AIS. Therefore do not assume
that initial FSM Focus/event1d precedes script initialization or that active
AIS is absent. Actual OnStateChanged `0x3d0bec` rereads active AIS+0x1c and only
skips delivery when it is genuinely null.

_LoadCharStates reads each Character's actual preset via `0x3a5784`: values0 and17
call SM_SetPreSpawnState `0x3c1a64`; all others call SM_SetIdleState(false)
`0x3c1a00`. It uses the embedded machine at Character+0x4fc. The earlier
initialization-alias proof established that machine+0x3c **is** Character+0x538;
these are the same idle-suppression byte, not separate fields.

## Evidence and scope

`reports/character-pre-spawn-arm64-differential.json` records 538 actual original
versus optimized standalone ARM64 cases and2698 ordered service requests, zero
mismatches. Fixture SHA256 is
`39248842633be43dcf4101528593d34982f02d33ae73e3d2884013bd28c73921`.
Coverage includes direct/fallback rows, signed stance and wrapping additions,
table pointer mutation during live index queries, base mutation after capture,
stay/kind reloads, strings, Spawn acceptance/next values, and diagnostic policy.
The full original method instructions execute; deeper services are controlled
fixtures, not full original physics/Spawn/Revive executions.

`reports/character-pre-spawn-host-audit.json` binds the same538 gold cases and2698
requests to the isolated optimized ASan/UBSan binary. It adds22 malformed,
failed-prefix and invalid-source checks plus one synchronous ANIM_Set callback
which reenters event28 before outer Focus finishes. No sanitizer diagnostics.
This host-only nested fixture supplements the original corpus; it is not an
additional original reentry parity claim. No central DSO, APK/device, full FSM,
complete Level loader, or full AI claim is made.

Reproduce the host replay with `tests/character_pre_spawn_host.py --output
<new-report-path>`. For a parent CMake target, compile
`tests/character_pre_spawn.cpp`, link the world library containing
`character_pre_spawn.cpp` and `character_state_empty.cpp`, and pass
`reference/character-pre-spawn/pre-spawn-fixtures.bin` as its only argument.
