# Owned source actor registry

This component copies imported `gametype="Character"` MGP records into a
bounded, app-owned registry. Each record preserves its source identity, exact
name, template fields, authored AI/autospawn fields, module-local transform,
and imported world position. Registry replacement is transactional: invalid,
over-capacity, or ambiguous input leaves the previous registry unchanged.

`request_spawn(name)` models the smallest useful script boundary. It looks up
an existing registered Character by exact, case-sensitive source name, reports
an unresolved name as a miss, and records one idempotent
`registered -> spawn_requested` port lifecycle transition. Miss count and the
last missing name are retained for diagnostics. This does not create actors,
make them visible, or execute the native Character state machine.
The registry is capped at 512 Characters; every copied string field has a
declared fixed bound in `actor_registry.hpp`.

These `ActorInstance` values are not the original `ObjectBase`/`Character`
ABI. The component does not provide a renderer hookup, AI, collision, combat,
template-to-model resolution, or a complete spawn state machine. A later app
session can own this registry and connect it to asset selection and rendering.

## Infected Village Ambush fixture

The host runner imports the actual Infected Village MLX and both referenced
MGPs through `world-data`, then builds the registry from that `Level`.
`infected01.mgp` has four statically registered Ambush requests; all four use
`char_template="InfectedVillage_CommonType1"`,
`char_template_pydata="Charater_Templates"` (spelled this way in source),
`_templateName="MonsterCommonType1"`, `ai_state="Limbus"`, and
`auto_spawn="0"`:

| Ambush order | MGP record | Actor name | Local position | World position |
| ---: | ---: | --- | --- | --- |
| 1 | 6 | `_prim_tmp_infected05` | `(5619.87,-1716.89,1325.43)` | `(2171.37,1283.11,1325.43)` |
| 2 | 8 | `_prim_tmp_infected07` | `(5602.35,-2008.12,1325.43)` | `(2153.85,991.88,1325.43)` |
| 3 | — | `_prim_tmp_infected17` | no static MGP/MVP record | lookup miss |
| 4 | 7 | `_prim_tmp_infected06` | `(6320.09,-2016.95,1325.43)` | `(2871.59,983.05,1325.43)` |
| 5 | 19 | `_prim_tmp_infected16` | `(6010.92,-2348.02,1325.43)` | `(2562.42,651.98,1325.43)` |

World positions add module 0's MLX placement `(-3448.5,3000,0)`. Authored Z
rotations are respectively `78.9326`, `96.4741`, `-101.756`, and `174.741`
degrees. The order and missing fifth name are checked against
[`INFECTED-VILLAGE-GAMEPLAY.md`](../level-runtime/INFECTED-VILLAGE-GAMEPLAY.md).
No replacement actor is synthesized for the unresolved name.

The native behavior motivating this boundary is documented in the recovered
library pseudocode:

- `ObjectManager::LoadFromXML`, ELF `0x34b868`, asks the type factory to create
  source objects, initializes template/default/override properties, then adds
  module origin to position.
- `ObjectManager::GetNewObject`, ELF `0x34b520`, dispatches through the native
  gametype factory map; `ObjectManager::Add`, ELF `0x34b270`, registers an
  object handle and name.
- `Script_SpawnCharacter::Execute`, ELF `0x45f400`, performs a name lookup and
  requests state on a found Character; it does not allocate one.
- `Character::GetCharModelId/Name`, ELF `0x3a31e8` / `0x3a54d4`, read the
  character's ModelDict selection. This registry does not recover the template
  resolver or claim a model choice.

Ghidra pseudo-C address annotations are `0x10000` higher than the ELF addresses.
The references are evidence for the boundary, not code executed by this port.

## Build and run

From the repository root, with a C++17 compiler and the separately supplied
cache:

```powershell
python port/actor-runtime/tests/run_host.py --cache ../cache/files
```

The script compiles the new component with `-Wall -Wextra -Werror`, imports
the cache-backed level and MGP records, runs the C++ fixtures, and writes
`validation.json`. It verifies all 26 imported Character records, source-owned
copies, module translation for each actor, the four Ambush records and exact
template/AI/autospawn data, the five request results, idempotence, duplicate
names, non-finite transforms, the actor capacity limit, and successful and
failed transactional replacement. The cache is read-only and is not copied
into this repository.
