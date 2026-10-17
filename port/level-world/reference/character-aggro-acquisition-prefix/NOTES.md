# Original aggro acquisition prefix

## Scope and reuse

The existing delay kernel reconstructs only the source timed branch. TargetList
search and candidate-event consumption reconstruct later normal acquisition.
No existing source module supplies the intervening entry/type/radius gates.
This new module adds **one bounded prefix orchestration**, reusing the unchanged
`character_aggro_delay::update` and original ordinary RNG reconstruction. The
11 original ranges and one vtable are evidence, not 11 new source bodies.
There is no native wiring, registry search, candidate consumer or whole AI claim.

The source function is `_UpdateAggro` at `0x3cf3f0`, 2,052 bytes. Covered branches
end at the normal `TargetList` constructor call `0x3cf624 -> 0x4a2730`, or their
earlier return/diversion. The local Monster existing-target branch starting
`0x3cf4b4` is explicitly unsupported before any of its target/aggro actions.

## Exact gates and order

| Source phase | Reconstructed behavior |
| --- | --- |
| `0x3cf3fc..0x3cf42c` | Fresh owner virtual `+0x28` IsPlayer; truthy returns immediately |
| `0x3cf44c..0x3cf464` | Separately fresh owner IsPlayer; truthy bypasses timing |
| `0x3cf74c..0x3cf758` | Fresh owner IsFaerie; truthy bypasses timing |
| `0x3cf75c..0x3cf8c0` | Existing timed kernel; waiting returns, ready continues |
| `0x3cf468..0x3cf474` | Fresh IsNPC; truthy returns after prior timing effects |
| `0x3cf478..0x3cf4a0` | Fresh IsMonster, then only if truthy fresh virtual `+0x54` IsRemotelyUpdated |
| `0x3cf4a4..0x3cf4b0` | Local Monster reads fresh owner target `+0x408`; nonzero diverts to uncovered retarget branch |
| `0x3cf59c..0x3cf5bc` | IsFaerie; if true read fresh owner target `+0x418`, nonzero returns |
| `0x3cf5c0..0x3cf5dc` | Capture global AIProps array before fixed owner's GetCharAIId, choose `0x44`-stride row, refresh owner |
| `0x3cf5e4..0x3cf5f0` | Read row radius `+0x3c` before AwaitingToSpawn on that fresh owner |
| `0x3cf8c4..0x3cf8e8` | Awaiting branch captures owner, reads float word `+0x143c`; strictly positive overrides radius and bypasses HasAggro |
| `0x3cf5f4..0x3cf608` | Otherwise HasAggro on captured AI; false reads captured row `+0x40` afterwards; then fresh owner for constructor |

`0x3a3064` is **IsMonster**, not IsPC. Character vtable address point
`0x965f38 + 0x54` resolves to `ObjectBase::IsRemotelyUpdated` at `0x33dd10`.
GetCharAIId (`0x3a2fec`) reads Character `+0xffc` and current global count;
negative/out-of-range raw IDs return fallback **8**. Typed providers must
preserve that getter, not supply an unclamped cached property.

For a local Monster without target `+0x408`, the owner used by that direct read
remains the IsFaerie argument. Other paths reload owner at `0x3cf59c`. If Faerie
is false, the conditional owner reload at `0x3cf5a8` fixes the later GetCharAIId
argument. If true, the owner captured for target `+0x418` remains that argument.
Capturing the table may change current owner/global table; both already captured
identities remain fixed at the subsequent getter.

Row `+0x3c` is a snapshot before Awaiting. Row `+0x40` stays live until after
HasAggro. A provider changing the captured row between those phases therefore
affects only the later reads. Captured old table storage survives replacement.
The positive spawn override uses its captured owner as constructor owner; other
paths read owner freshly after HasAggro. Both zero signs, negative values and
NaN fail the override comparison; positive infinity passes, as original fcmpgt.
No finite clamp is introduced. A later TargetList adapter may separately reject
nonfinite radius at its explicit port boundary.

The output ends before constructor execution and supplies raw radius word and
captured owner. The original normal constructor flags are `0x7fffffff`, object
filter 2 and closest sort 1. The later Search source cone is `0x40c90fdb`, outside
this prefix. Existing TargetList/search/event kernels handle those next phases.

## Contracts and errors

Views are renderer-neutral projections, not original memory overlays. AIProps
row array/capacity remain stable bounds; radius words remain live. One owning
thread retains all owners, state, services/context and retired table/row views
through synchronous return. The source AI identity is captured once. Providers
may update owner/fields synchronously, and independent nested outputs are allowed;
they must not overwrite this output/service table or destroy borrowed views.

Providers return zero for success, with separate source words. The two target
queries project null/non-null without truncating port 64-bit object identities.
Taken-branch missing providers, errors and exceptions fail explicitly, preserving
prior mutations and reused timer/RNG effects. There is no rollback or cleanup.
Malformed top-level alignment/range/aliases reject before effects. Borrowed table
bounds are limited to 4,096 rows. RNG is required only upon entry to the timed
kernel; no eager timing-service query is introduced on bypassed paths.

## Verification

Host cases cover gates/skipped providers, timing reuse, fresh owner changes,
captured table replacement, precise radius-read order, both zeros/NaN/infinities,
source getter fallback8, unsupported retarget and failure/alignment/alias bounds.
The ARM oracle executes original entry/gate/radius instructions, original timed
arithmetic and ordinary RNG, and real GetCharAIId/GetDt instructions.
Type/player/FSM/aggro/turn/debug services and the external `__aeabi_fcmpgt` PLT
boundary supply fixtures. That comparison library implementation is absent from
the ELF; IEEE comparison fixtures preserve original raw operands and branch
results, including NaN and both zeros. Handle/retarget/search
and native actor providers are not claimed. It compares raw words, owner,
timer/RNG state and ordered provider/direct-field traces at each boundary.

```powershell
python port/level-world/tests/run_character_aggro_acquisition_prefix_host.py `
  --compiler C:/Users/noamc/.local/mingw/mingw64/bin/g++.exe `
  --original-elf C:/Users/noamc/Documents/Codex/2026-10-02/contineu-from-where-they-left-off/work/test_strategy/libDungeonHunter2.so `
  --output port/level-world/build/character-aggro-acquisition-prefix-review/host.exe `
  --report port/level-world/build/character-aggro-acquisition-prefix-review/validation.json
```
