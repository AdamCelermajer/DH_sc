# Source skill mana-cost projection

`runtime_skill_mana_v1.hpp` reproduces the data calculation behind
`CalcManaCost(CLASS_ID, rank)` from `skills-commons.luac`. It starts from the
original property defaults, writes the source fixed-point rank to property 172
(`SnS_Level`), applies the exact source `ClassTables` row with the same live
actor's resolved property sheet as the class formula buff input, and reads raw
property 173 (`SnS_ManaCost`). The class token is supplied from that skill
script's authored `CLASS_ID` declaration and resolved by the source class-name
table; no class ID or cost curve is invented.

Source scalar conversion is `ToFixed`: signed float-to-int conversion followed
by wrapping left shift 8. Saved source skill ranks are u16, so the helper rejects
larger values. Current `MP` is property 41 and `Max_MP` is property 43. This
helper returns the raw authored cost only; original `HasMana` and `UseMana`
also require their application/player/debug bypass inputs and exact same-owner
mana mutation. It does not equate `CharacterState.stats.resource` to native MP,
admit a cast, locate a target, invoke combat effects, or report a gameplay
action as successful.

Run `run_runtime_skill_mana_v1_tests.ps1` to load the original character,
property, and class tables and exercise the real
`Skill_Warrior_BashDown` row at ranks 0, 1, 2, 7, and 65535. The fixture checks
the same resolved-player buff input, the source raw `SnS_ManaCost`, and that
the borrowed live property sheet stays unchanged. Unknown class tokens and
out-of-range saved ranks fail closed.
