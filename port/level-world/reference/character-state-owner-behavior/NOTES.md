# StateOwner composition with the four recovered behavior families

Original ELF SHA256:
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
This module is additive. Frozen StateOwner, state-body kernels, original gold,
old reports, renderer and CMake were not edited.

## API and exact ownership boundary

`character_state_owner_behavior.hpp/.cpp` provides
`dh2_character_state_owner_behavior_bind(out,context)` and its callback
`dh2_character_state_owner_behavior_invoke`. StateOwnerBehaviorContext40 borrows
stable Facts, body Services, Predicate8, and required remaining StateOwnerServices.
Keep every referenced object alive across the complete outer/nested call. Bind
rejects missing providers, malformed facts/reserved fields and output aliases
that would overwrite those borrowed inputs. It owns no allocation/session.

Idle3, Move4, Attack5, Dead12 requests preserve the source method keys and call
only the source `focus_body`, `blur_body`, `event_body` exports. The adapter never
calls the legacy whole transition or whole state-event coordinator. The owner's
outgoing blur→nullable selection/reset→incoming focus→Character event1d ordering
therefore runs once. Bounded method requests must match the owned StateInfo
metadata, actual owner and live current state; forged requests fail.

Seven CSM predicates compose the owner's proved pure helper. The adapter reads
live flags520, mask528 and stopped-attacking442 from owned State; Predicate8
supplies genuine interaction544 and can-interrupt441. No animation/target inference.
Source facts/services can change synchronously through body callbacks. Providers
must refresh borrowed Facts before returning from producer changes such as Stop.

Character.RaiseEvent, event30 pin, profiling, Spawn and the other16 behavior
families go to the required remaining provider with the original request intact.
It must deliver a genuine implementation or return failure. In particular Spawn
is not accepted by a constant or guessed predicate. Unsupported focus can fail
after current selection; owner preserves that already delivered source prefix.
The body Services API is the existing synchronous void callback contract: the
caller must supply supported genuine body services, not accepted empty effects.

When remove-body/animation callbacks change body presence, caller must also
update StateOwnerMachine40.physical. The same requirement applies to live owner
identity and other source projections. State.current selection remains owned
by StateOwner APIs, including nested callbacks. Pure predicate snapshots do not
replace live State mask/flag reads.

## Original and optimized ARM64 proof

NEW `tests/character_state_owner_behavior_differential.py` reuses the frozen
original ARM32 state-dispatch test/services and changes only native transition/
event entry to a NEW oracle composition fixture. The fixture owns stable flat
StateInfo/event records copied from the actual source registration data. It
binds this genuine adapter and executes native owner/body/predicate kernels.
Profiling and Character.RaiseEvent are explicit remaining fixture deliveries.
Original state instructions, predicates, focus/blur/event bodies and ordered
service projections execute. libc float arithmetic/imports retain the previously
proved source test contract. No full game service or allocator parity claim.

The emitted3910-case gold is **byte-identical** to the frozen original
`reference/character-state/state-reference.bin`, SHA256
`9e11a899a29f9682959c00e988ad4fc04ee4a3f610de9044e933d47997d27a57`.
Of3910 cases,937 transitions and975 events execute NEW owner+adapter; the other
1998 getter/update cases are regressions using existing kernels. This count is
not3910 new behavior-composition cases.

Returning the actual native callback from Bind uses a defined-symbol
AArch64 GLOB_DAT relocation. The NEW loader resolves the actual ELF symbol;
it does not replace that callback with a host model. Unused libc++ RTTI/exception
imports remain outside this fixture execution; vector allocation is separately
covered by actual owned-class host tests and prior owner proof.

`reports/character-state-owner-behavior-arm64-differential.json` binds all
source/oracle/test hashes, original ELF and actual O2 ARM64 ELF. It passes all
3910 with zero mismatches. Old reports remain historical and unchanged.

## Actual owned-class sanitizer replay and reentry

`tests/character_state_owner_behavior.cpp` constructs actual CharacterStateOwner
instances and binds stable contexts. It reads frozen original gold, compares
937 transitions+975 events and7894 ordered body requests, and records1860
explicit remaining-provider fixture deliveries. It additionally tests15 native
ownership/malformed/unsupported boundaries, including output alias rejection,
forged method/owner identity, live predicate mask reads and failed Spawn/other
family delivery.

Two actual nested host compositions execute:
1. Move→Attack outer event1d synchronously routes completion22. The nested owner
   transition blurs Attack, starts delay2a/pins, focuses Idle and notifies, with
   no duplicated legacy outer transition. Final Idle animation and11 body
   requests match the recovered source choreography.
2. Dead focus emits2a/2c/2b; the fixture explicitly forwards them through the
   owner event entry, clearing live source masks synchronously.

Those additional recursive checks are host composition evidence; no new claim
that their entire Character→AI service backend was executed in original ARM32.
The genuine source prefix/dispatch semantics were already instruction-proved.
Caller-driven recursion and missing backends remain explicit.

`reports/character-state-owner-behavior-host-audit.json`: O2 host ASan/UBSan,
leak detection, zero diagnostics. No central DSO/APK/device/live/full-AI claim.
Reproduce: `python tests/character_state_owner_behavior_host.py --output NEW.json`
from level-world or use its repository-relative path from repository root.
For parent's main-linked audit, compile `tests/character_state_owner_behavior.cpp`
against world and pass the full frozen `state-reference.bin` path as its only arg.
