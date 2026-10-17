# Infected actor AI and state-request audit

This host-only slice translates one cache-backed request boundary: a decoded
Infected Village `SpawnCharacter` command that resolves to exactly one static
`Character` record emits a **Spawn state request**. It does not run the
script/trigger, choose one of the template's six models, update AI, move an
actor, start combat, or play an animation.

## Verified source path

The exact Infected Village source reader finds five `SpawnCharacter` commands
in the `Ambush` script. Four names resolve to one static `Character` record;
`_prim_tmp_infected17` has no matching static actor and remains unresolved.
Each matched record uses template `InfectedVillage_CommonType1`, authors
`ai_state="Limbus"`, and `auto_spawn="0"`.

The C reader in `port/pydata-constants/constants.c` reads the cache's
`AIStates` group and returns `Limbus=0` and `Spawn=1`. The native
`Script_SpawnCharacter::Execute` routine calls
`CharStateMachine::SM_SetSpawnState(false, false)` after a successful object
lookup. The native setter requests state ID 1 for that argument case. The
translator therefore preserves the four explicit source requests, across all
six model alternatives: 4 actor requests and 24 actor/model pairs. It does not
claim a trigger fired or an actor spawned.

The six model alternatives come from the checked source manifest and all six
have passing skin/animation skeleton readers. No model choice is selected by
this slice. For the initial `Limbus` state, the infected animation table has
`Limbus=-1`; native `CSLimbus::OnUpdate` and `OnEvent` are single `bx lr`
handlers. There is no Limbus animation request to emit.

## Why AI decisions and spawn animation are withheld

The checked-in PyData readers decode integer constants, array/struct names,
and known script commands. They do not decode typed `AIProps`,
`CharTemplate`, or `CharAnimTable` records. Their original native record
readers are recovered, but no checked-in host record decoder returns the
AI/template values or typed animation slots. The AI properties' field names
(`Type`, `Script`, `ViewRadius`, `LeashDistance`, and others) do not establish
the values for this template.

The native `CSLimbus` initialization registers event `0x2f` to the character
spawn callback; its focus handler clears aggro and may schedule a respawn timer
based on runtime properties. Those facts do not recover autonomous transition
conditions. The native Idle state registers events that can select Move and
Attack, but the corresponding AI predicates and event production have not been
recovered for these template records. The table names `AIStates.Move` and
`CharAnim.Walk`, but this slice does not treat their similar names as proof of a
runtime state-to-clip request. Consequently this translator never infers Idle,
Move, Attack, Stunned, or their transition conditions.

The cache has `Infected` animation table candidates: Idle 321 and Walk 328
pass skeleton binding for all six variants. Attack 315 is partial (its Faery
cutscene clip binds on 0/6 variants and its Prince cutscene clip has one passing
variant); Stunned 326 binds on 0/6. The cache's Spawn mapping is present, but
its runtime typed slot is not proven by the checked-in readers and was not part
of the prior clip compatibility matrix. No animation request is emitted for
any of these candidates.

Native `CSSpawn::OnFocus` reads a `CharAnimTable` value at record offset
`0x80`. Without a typed reader tying that offset to a verified cache member,
treating the named `Spawn` entry as that slot would be an unsupported
assumption.

## Reproduce

From the repository root with Python 3 and GCC installed:

```powershell
python port/actor-ai-runtime/tests/run_host.py `
  --cache ..\cache\files `
  --report port/actor-ai-runtime/validation.json

python -m unittest discover -s port/actor-ai-runtime/tests -v
```

The runner compiles the exact checked-in PyData constants reader into this
directory's ignored `build/` folder, uses it to query `AIStates`, and imports
the exact level/script cache audit and PyData script reader. It verifies the
native state-request call chain in the checked-in decompilation, verifies the
Limbus no-op handlers in assembly, and cross-checks every model against the
existing six-variant skeleton report. It writes hashes and all per-request
results to `validation.json`; cache payloads are read in place, never copied.

No emulator, Android build, device, or Fold 7 is used.
