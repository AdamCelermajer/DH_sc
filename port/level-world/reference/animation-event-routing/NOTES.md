# Native character animation-event routing

`character_animation_events.cpp` reconstructs the Character/CharAI dispatcher
for events 0x22 through 0x27. It is compiled into the source world module, but
has not yet been wired to the live Android Prince. The saved 6d9782be APK does
not contain this work. Actual state-specific AI consumers remain unfinished.

The [original instruction corpus](native-reference/probe.json) executes
Character.RaiseEvent at 0x3a4d5c, CharAI.RaiseAIEvent at 0x3cbb34, the source
state getter and the simple sequence/end handlers. Its manifest captures 14
functions from original ELF SHA-256
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
The preliminary probe in this directory is preserved; `native-reference`
is the final script-bound capture and binary host/ARM64 replay input.

| Event | Original handling before FSM |
| --- | --- |
| 0x22, 0x23 | Simple end handler returns 1; AI virtual +0x98 runs before forwarding. Its return is ignored. These branches precede generic gates. |
| 0x24, 0x25 | If permitted by the generic gate, query current state through the sequence begin/end handler; forward to FSM. |
| 0x26 | If permitted, query current state; state 4 calls Move begin, 5 Attack begin, 6/7 Skill/Spell begin. Consumer return controls forwarding. |
| 0x27 | If permitted, query current state; state 5 calls Attack end, 6/7 Skill/Spell end. Consumer return controls forwarding. |

For the generic branches, controller forced bypasses global blocked/controller
locked. Otherwise either block skips the AI consumer and still forwards to
the machine. Event and payload are captured before synchronous callbacks.
The native API obtains the live state through its getter service rather than
passing a stale pre-callback state snapshot. It preserves pointer identities
above 4 GiB. Unsupported events and malformed services return -1 without
dispatch; successful routing returns 1 even if the consumer suppresses FSM
delivery. Original void return values are not compared.

Both [optimized ARM64](../../reports/character-animation-events-arm64-differential.json)
and [direct-source ASan/UBSan](../../reports/character-animation-events-host-audit.json)
replays pass 1,344 cases, 2,422 ordered callbacks and eight native rejection
checks with zero mismatches/findings. Facts span states -1/3/4/5/6/7/12,
all gate combinations, null/non-null payload and consumer returns 0/1.
The source, original manifest/gold, binary and test scripts are hash-bound.

Attack/Skill/Move handlers, AI virtual +0x98 and final FSM delivery are explicit
service fixtures. Their actual gameplay mutations, nested state changes,
equipment/audio/scripting producers and the rest of CharAI are not proved here.
This module provides their recovered delivery order; it must not be used to
invent an always-accept AI consumer for the live game. The separate compiled
sampler and two-slot coordinator audits cover different behavior.
