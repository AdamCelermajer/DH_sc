# Crypt character template and property projection

This source slice resolves the authored Crypt ambusher template route without
choosing a random variant. It is renderer-independent and is not yet wired into
the native world/actor factory.

## What the source data says

The four records `_prim_tmp_ambusher01` through `_prim_tmp_ambusher04` in the
original cached `data/3d/modules/crypt/mgp/crypt_straight_ns_01.mgp` all have:

- `gametype="Character"`;
- runtime key `char_template_pydata="Charater_Templates"` and
  `char_template="GothicusCrypt_Ghosts"`;
- editor metadata `_templateName="MonsterCommonType1"`;
- no `charpropsname` field.

`_templateName` identifies the editor primitive; it is not a fallback property
key. The runtime factory route is the `char_template_pydata` / `char_template`
pair. The template row has five ordered `CharInfo` slots:
`[35, 35, 35, 35, 37]`. Repeated IDs are retained because they are authored
selection weights. CharacterTable row 35 is `Crypt_Ghost` and row 37 is
`Crypt_Ghost_RE`. Both have `ClassID=18` (`BaseMonster`), AI row 68,
animation-table row 24 and model-file row 44. Their 224 serialized property
values differ only at `RespawnTime` (field 11): raw 0 versus raw 5120.

The resolver keeps the editor primitive string separate from the runtime key.
With no caller-supplied selected slot it returns `selection_required` and the
complete duplicate-preserving list. It never rolls its own random value.
`Character::SafeGetCharPropsId` selects an index with the game RNG, but the
original RNG seed and the per-actor call order needed to reproduce each of the
four choices are not established here. Callers must provide a source-selected
slot before using this result as an actor's property row.

After selecting a row, the kernel returns its CharacterTable ID/name and its
class ID/name. It does not calculate or apply any of the 224 properties. The
existing `port/game-data` property and class-table pipeline owns default/type
resolution and class formulas; a class ID of `-1` remains the no-class-formula
case. This is link resolution, not a reconstructed class/default evaluator.

## Code and validation

The fail-closed resolver is in [`character_template_factory.hpp`](../../character_template_factory.hpp)
and [`character_template_factory.cpp`](../../character_template_factory.cpp).
It rejects unsupported pydata classes, missing or duplicate template names,
conflicting explicit-property/template routes, invalid slot/property/class
indices, and duplicate explicit property names. Exact string keys are used;
the editor template name is not consulted as a fallback.

The focused C++ test is
[`character_template_factory.cpp`](../../tests/character_template_factory.cpp).
The runner first cross-checks the compiled fixture against the supplied cache
and all four original MGP records, then compiles the actual C++ resolver with
warnings as errors and executes the host assertions:

```powershell
python port/level-world/tests/run_character_template_factory_host.py --cache ../cache/files
```

The passing host fixture covers the five exact slots, both ghost property/class
projections, all four MGP runtime keys, explicit property routing, and atomic
failure cases. It does not exercise the engine's RNG, game loop, renderer or a
live native actor factory. The JSON report is written under
`port/level-world/build/character-template-factory/validation.json`.

## Recovered original-function boundaries

The pinned library and complete function byte-range bindings are in
[`original-functions.json`](original-functions.json). In the native path,
`Arrays::Charater_Templates::read`, `Structs::CharTemplate::read` and
`Structs::CharInfoName::read` load the authored slots;
`Character::SafeGetCharPropsTemplateId` resolves the template name;
`Character::SafeGetCharPropsId` chooses and resolves one slot. The C++ resolver
implements only the deterministic name/slot/property/class projection after
the source choice has been supplied. The actual `Random::GetRandom` draw and
`CharProperties` default/class recalculation are dependencies or boundaries,
not functions reproduced by this module.
