# Original enemy defensive reactions

`character_defensive_state_v1` reconstructs the whole source
`CharStateMachine::SM_SetBlockingState` (0x3c5c60) and
`SM_SetDodgingState` (0x3c5b3c). The shared retained NPC injury runtime now
delivers SkillApply services 9 and 10 through this owner.

The source checks the SAME Character gate at +0x14fc before callbacks, calls
IsPlayer without branching on its result, writes 3000, selects the original
CharAnimTable row, and reads Blocking (+0xc) or Dodging (+0x20). Missing rows
and animation -1 return after the gate write. Stance masks are respectively
0x2000 and 0x4000; the chosen sequence is stored at the same FSM animation
override before event 50010 or direct transition to Injured state 11.
The parsed authored table fields are 2 and 7, as confirmed by its schema and
the retained source table probes. This does not choose a replacement animation.

The original ARM control flow was executed for 2,880 combinations, including
NaN gates, absent rows, animation -1, both stance bits, direct dispatch and
integer overflow. Table, design and event successors are explicit fixtures.
The current APK-linked native owner matched gate/override bytes and ordered
service calls for every case. See `defensive-state-original-v1.json` and
`../reports/android-native-owner-tests/defensive-state-v1/receipt.json`.

The actual targeted FIRE cast now passes this former provider-10 failure.
Its current remaining failure is the original NPC post-hit Lua HeadTo command
in provider 19. This is not yet successful live skill completion.
