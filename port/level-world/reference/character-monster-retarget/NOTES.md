# Local Monster existing-target branch

## Scope

This new module implements **one bounded branch caller orchestration** after the
normal acquisition prefix diverts local Monster with a nonzero owner `+0x408`.
It covers the highest-threat switch and non-enemy clear paths, with an explicit
unsupported result for the distinct enemy-retention search. Nine original ranges
are byte evidence, not nine newly implemented functions. Genuine owning aggro,
handle, relation, design/debug and target-setter services remain external.
No native/CMake wiring, distance approximation or whole AI claim is made.

## Corrections from exact symbols

- `0x3d4a18` is **AI_GetHighestAggro**, not AI_GetTarget. It selects a peer from
  the outgoing aggro map; this caller does not substitute nearest distance.
- `0x33dd2c` is **nonconst ObjectBase::GetHandle**, 68 bytes, with ABI struct
  return into the temporary. It is not an ObjectHandle constructor.
- `0x33ff54` is **ObjectHandle::operator Character***, 56 bytes. It calls
  GetObject(false), then live object virtual `+0x24` IsCharacter, and returns
  null when resolution/type fails. It is not a plain generic GetObject result.
- `0x3d49c4` is **AI_SyncLastTarget**, 12 bytes: load AI `+0x40`, store `+0x44`.
  It does not interrupt an attack.

## Original order and identities

Entry `0x3cf4b4..0x3cf4b8` calls HighestAggro using the owner already captured
for the preceding target-field gate. The API accepts that explicit entry owner;
an adapter must retain the subject of its `target_408_present` read, rather than
invent a reread after a provider that changes owner.

The caller then refreshes owner, obtains field `+0x408`'s handle and resolves it
to Character (`0x3cf4bc..0x3cf4e4`). Both returned peer identities remain fixed
and live for the rest of the branch. Null or equal highest/current enters the
relation path at `0x3cf990`; otherwise:

1. Fresh owner embedded AI_GetAggro(current), `0x3cf4f4..0x3cf500`.
2. Separately fresh owner embedded AI_GetAggro(highest), `0x3cf508..0x3cf514`.
3. Current global design pointer's float field `+8`, `0x3cf518..0x3cf52c`.
4. Binary32 current threat times factor; compare highest **strictly greater**
   than that threshold, `0x3cf530..0x3cf544`. False returns without mutations.
5. Diagnostic DebugSwitch load/lookup/string lifetime, `0x3cf548..0x3cf580`.
   The lookup return is ignored, but its call and ownership effects remain.
6. Fresh owner embedded AI_SetTarget(highest, force=0), `0x3cf584..0x3cf594`.

The bounded adapter represents diagnostic storage/string helpers by one genuine
synchronous service boundary. No boolean switch is invented around SetTarget.
Negative/zero/NaN/infinite threat/factor values retain IEEE multiply/compare;
no clamping or positive-threat predicate is added by this caller.

For the null/equal path, fresh owner embedded AI_IsEnemy(resolved current) runs
at `0x3cf990..0x3cf9a0`, even if current is null. Its source null fallback must
be preserved by the existing relation module/provider. If false, the caller
uses the **captured original AI** for each action, not a fresh owner's AI:

1. AI_ClearAggro(current), `0x3cfb10..0x3cfb18`.
2. AI_SetTarget(null, force=0), `0x3cfb1c..0x3cfb28`.
3. AI_SyncLastTarget, `0x3cfb2c..0x3cfb30`.

Sync reads the then-current stored `+0x40`; mutations/reentry inside SetTarget
can therefore change what gets copied. Failure stops at its provider boundary
and preserves already completed actions; there is no rollback/extra cleanup.

## Remaining search boundary

Enemy true takes `0x3cf9a8` onward: TargetList(owner, flags=1, object filter=2,
closest sort=1), no-aggro view radius, room/registry source enumeration, current
player property-set ID (`+0x660`), owner property tree and membership scanning.
It can add aggro using global design `+0x30` or clear target when its list is
empty. The ordinary maxflags/filter2 and melee flags1/filter0 accepted domains
do **not** implement this domain. This kernel returns `unsupported_branch`
before constructing that list. It never silently uses an ordinary scan.

## Lifetime and proof

One owning thread retains State, captured AI, entry/fresh owners, both returned
peers, context and services through synchronous return, including retired
objects after replacement. Independent nested outputs are allowed. Providers
must not overwrite this output/service table or destroy borrowed identities.
Top alignment/range/aliases reject before effects. Missing/error/throwing
providers fail explicitly at the taken phase, retaining prior mutations.

Host tests cover strict boundary/IEEE edge values, ignored diagnostic return,
captured/fresh owner ordering, original-AI clear ownership, Sync after callback
mutation, unsupported enemy search, provider failure and atomic input guards.
Original ARM comparisons execute the caller branch and the actual SyncLastTarget
leaf. HighestAggro, handle/Character conversion, GetAggro, relation, diagnostic,
clear and setter calls are observed services. External fmul/fcmpgt imports are
explicitly modeled as IEEE binary32; finite words compare exactly and NaN outputs
by class, with NaN payload/sign propagation excluded. No original storage service
body or enemy-retention search execution is claimed.

```powershell
python port/level-world/tests/run_character_monster_retarget_host.py `
  --compiler C:/Users/noamc/.local/mingw/mingw64/bin/g++.exe `
  --original-elf C:/Users/noamc/Documents/Codex/2026-10-02/contineu-from-where-they-left-off/work/test_strategy/libDungeonHunter2.so `
  --output port/level-world/build/character-monster-retarget-review/host.exe `
  --report port/level-world/build/character-monster-retarget-review/validation.json
```
