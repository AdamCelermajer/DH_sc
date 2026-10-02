# Kill-objective progress

This source projection owns a kill objective and a mutable progress event. It
covers the four already-dispatched original ObjectiveTemplate_KillCharacter
handlers at ELF 0x47f100 (kill property), 0x47f228 (clear property), 0x47f2bc
(kill template) and 0x47f350 (clear template), each 148 bytes. It also projects
the completed byte in Objective::SetIsCompleted (0x47ba10, 76 bytes).

## Recovered behavior

- A nonmatching property/template ID leaves objective and event unchanged.
- A matching local event increments the signed count with ARM32 wrapping, sets
  its outbound byte to 1 and writes the new count to the event quantity.
- A matching synchronized event updates the count only if the incoming signed
  quantity is larger. It preserves both flags and the incoming quantity.
- After a count update, reaching the signed required count requests the completion
  virtual callback. A previously zero completed byte becomes 1. The callback can
  repeat for already-completed objectives; stale synchronized counts skip it.
- Original handleEvent always returns zero; source returns checked status and
  explicit match/change/completion outcomes for its owned caller.

State and event flags are native bytes represented by checked 32-bit fields.
Counters/required/match IDs are signed 32-bit fields. Bad pointers, aliases or
flags greater than 255 preserve objective, event and result bytes. Source event
kind dispatch is supplied by the caller; the C kernel receives dispatched input.

## Evidence

`differential-validation.json` records 14,396 original ARM32/host/source ARM64
comparisons with zero mismatches: 4,116 local updates, 3,538 synchronized increases,
4,708 stale synchronized events and 2,034 nonmatching IDs. The original executes
all four handlers and SetIsCompleted. Counts, completed/flag bytes, event quantity,
4,876 completion requests, source output guards and original reserved fields match.
Native record persistence ID -1 suppresses the external save action; the virtual
completion callback is an observed fixture. The fixture places required count
at objective +0x2c, current at +0x20, record pointer at +0x24 and completed at +0x14.

`host-build-validation.json` records 12,000 address/undefined sanitizer iterations
with recovery disabled. `android-build-validation.json` checks ARM64 16 KiB ELF
load alignment. The runtime additionally compiles `lua-bridge.c`.

## Owned Lua controls

`DH2CreateKillObjective(context)` requires `kind` (0..3), signed integers
`match_id`, `current`, `required` and boolean `completed`. Kinds use the death
projection order: KillXEnemies, ClearEnemies, KillEnemyTemplate, ClearEnemyTemplate.
The constructor is authored; it does not load/compile an original quest record.

`objective:GetProgress()` returns these fields. `ConsumeKillEvent(event)` requires
kind and match_id; optional boolean synchronized/outbound default false and signed
quantity defaults -1, corresponding to locally emitted death requests. It returns
matched, changed, completion_requested, newly_completed, progress and the updated
event. The input Lua table is never modified. Nonmatching kinds skip the kernel;
nonmatching IDs use its native no-op. Raw field reads avoid metamethods. State
commits after allocating the complete return value, preserving state on errors.

Lua numbers are float32: only exactly representable signed integer inputs are
accepted, and snapshots use that number profile. Full signed boundaries are
checked by the C/ARM64 differential test. Boolean Lua controls are narrower than
the kernel's native byte values. Completion requests do not grant rewards.

Actual quest data loading/compile, live world counts, automatic event dispatch,
markers, completion observers, persistence/rewards and complete source gameplay
remain pending. See current Android integration evidence when available; earlier
death reports retain their historical source/build identities.

## Reproduce

```sh
python port/quest-kill/build.py --host --report port/quest-kill/host-build-validation.json
python port/quest-kill/build.py --ndk /path/to/windows-ndk --report port/quest-kill/android-build-validation.json
python port/quest-kill/tests/differential.py \
  --original /private/libDungeonHunter2.so --oracle port/skin-payloads/build/oracle.so \
  --cache /private/cache/files --host port/quest-kill/build/quest-host.so \
  --arm64 port/quest-kill/build/quest-arm64.so --report port/quest-kill/differential-validation.json
```

Host execution uses Linux/WSL; Android build uses the Windows NDK. Differential
execution requires Unicorn and pyelftools. Addresses use the ELF zero load base.
