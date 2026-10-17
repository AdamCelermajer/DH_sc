# Original monster acquisition scheduling

## Reconstructed scope

`character_aggro_delay.cpp` reconstructs the timed, non-player/non-Faerie branch
of `CharAI::_UpdateAggro`, starting at original ELF address `0x3cf75c` and
ending at its wait or acquisition continuation. It is a source kernel, not a
replacement for the complete 2,052-byte function.

The caller supplies the original `IsMyTurn` and debug-switch decisions, a
fresh `Application::GetDt` provider, and the existing ordinary random stream.
The two mutable words represent `CharAI+0x8` and `CharAI+0xc`. They are explicit
raw words; no original object layout is overlaid on a 64-bit pointer.

The branch at `0x3cf74c` calls `Character::IsFaerie` (`0x3a3094`), not
`IsNPC`. Faeries bypass delay. The separate `IsNPC` (`0x3a310c`) acquisition
gate at `0x3cf46c` runs after the timing continuation. NPCs therefore do not
bypass this timing branch merely because they are NPCs.

## Source order

1. Query `IsMyTurn`. When false and the signed elapsed word is below 500,
   capture countdown before a fresh delta read, subtract modulo 32 bits,
   then capture elapsed after that store and read delta again before adding.
   Crossing 500 in this branch still returns without acquisition this frame.
2. Otherwise reset elapsed before querying
   `DisableAIDelayTimerOptim`. A true switch proceeds without changing
   countdown or consuming random values.
3. With the switch false, a positive signed countdown is captured before a
   fresh delta read. A still-positive result waits. Zero/negative countdown
   skips the delta read and expires immediately.
4. Expiry consumes one ordinary random draw using the existing source
   `dh2_random_next(state, 200, 0)` and stores the result plus 100. The
   synchronized random stream is unchanged. Continue at `0x3cf468`.

The original multiply/add wrap, remainder, signed comparisons, and counter
wrap are preserved. There is no frame-delta clamp or substitute random engine.
Provider failures retain already completed effects. Upfront overlapping
state/RNG/service/output projections and unaligned typed pointers are rejected
as port ownership guards. Provider exceptions become `service_failed` after
their prior effects; native state is not rolled back.

## Verification

`tests/run_character_aggro_delay_host.py` builds the C++ kernel and existing C
random implementation, then compares 20 fixtures to the original ARM branch.
The oracle executes the original branch and `GetDt` leaf. `IsMyTurn`, the
debug query, and debug-loader/string storage remain supplied dependencies.
Fixtures include unequal delta reads, callbacks mutating source words, 499/500,
negative words/deltas, wraparound, and exact countdown expiry. Ten separate
guard cases cover invalid aliases, absent taken/untaken services, and partial
effects after provider failure/exception. Root validation: 20 matched cases, ten guard
cases, zero mismatches.

The ARM oracle is a component comparison on the host. It is not a game
emulator, an Android runtime test, or evidence about Android page size. Its
global values are fixture values, not recovered original startup values.

## Remaining work

The queue/global producers inside `IsMyTurn`, source player/Faerie branches,
candidate acquisition, relationship classification, AI events, and native
actor ownership still need their corresponding source implementations and
integration. This slice is not yet wired to either Android game build.
