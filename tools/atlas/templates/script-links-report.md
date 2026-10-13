# Lua symbolic data links

This report adds conservative source links from symbolic Lua arguments to exact
PyData rows. It is a static data cross-reference; it does not execute scripts or
claim a runtime branch ran.

## Consumer and namespace evidence

The Android IDA export `ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c`
maps the receiving APIs to native handlers. `Character::_PlayAnim` (`0x003ba388`,
source line 135689) reads argument 0 as an unsigned index and bounds-checks it
against `Arrays::AnimDict::size`; `Character::_RegisterAnim` (`0x003b70d8`, line
133451) passes the same argument to `CharAnimator::ANIM_AddAnimDictToSet`.
`GameObject::_PlayFX` (`0x003923cc`, line 108581) bounds-checks argument 0 against
`Arrays::AnimatedEffectTable::size`. `Character::_SpawnSkillProjectile`
(`0x003b99d0`, line 135299) bounds-checks its projectile argument against
`Arrays::ProjectileTable::size`. The Lua helper
`assets-extra/android/data/scripts/skills/_commons.luac:182` prepends `SKILL_ID`
in `SpawnSkillProjectile(...)`, so the call-site's first argument is the
projectile argument consumed by that native handler. `LuaScript::_PlaySound`
(`0x0037e578`, line 94257) and `_PlayMusic` (`0x0037e420`, line 94208) pass the
first string argument to `Arrays::GetMemberIDByString<Arrays::Sounds>`;
`GameObject::_PlaySound3D` (`0x0039212c`, line 108463) performs the same member
lookup for its first argument.

The Android registration manifest
`port/android-native/app/src/main/cpp/source_process_array_registration_v101.hpp`
maps those public namespaces to source arrays. The generated table names used by
this resolver are `animations_dictionary#group0`, `effect_sets`,
`projectiles#group0`, and `character_classes#group0`. `ApplyPropClass`,
`CreateBuff`, and `ApplyBuff` are linked only through their authored first
argument when it is an exact `GetPyOID("ClassTable", name)` value and the
corresponding class row is unique. This does **not** infer that a
`GetProp(GetPyStruct("CharacterProperties", ...))` result is a class ID.

The class handlers are `Character::_ApplyPropClass` (`0x003babdc`, which
bounds-checks argument 1 against `Arrays::ClassTable::size` before applying the
class properties) and `Character::_CreateBuff` (`0x003b86a8`, which likewise
bounds-checks its class argument). The first argument of `ApplyBuff` reaches the
same `_ApplyPropClass` implementation. These index checks prove the consumed
table namespace; they do not prove a callback or gameplay action ran.

## Resolution rule

The resolver follows one whole-line simple assignment at a time: a local or
global identifier can bind to a literal, another identifier, or an exact literal
`GetPyOID("namespace", "name")` call. Only a binding at Lua block depth zero
is admitted, regardless of formatting indentation. Every write is counted,
including writes inside functions, branches, and loops; a second write,
multiple-assignment LHS, or unsupported scoped binding remains unknown. It
rejects unbound names, computed expressions, escaped literals, mismatched
namespaces, missing table rows, and duplicate row names. Explicitly imported
global bindings are followed only when an exact source import is already in the
script import projection; imported locals are not visible, including a local
declaration without an initializer followed by a same-name assignment.
Every successful edge stores the binding source path and line, raw source text,
native consumer address, namespace, and target table row id. Existing resolved
reference rows are never replaced.

`RegisterSkill` and `RegisterSkillEx` are classified in the companion Lua
semantics resolver, not as resource-row links: the Android `_commons.luac`
helper maps the seven positional `RegisterSkill` callback arguments to
`update/check/pre/skill/post/delayed/info`, and maps `RegisterSkillEx` tables to
named callback fields. Sound and music edges stop at the proven `Sounds`
member-name lookup; they do not claim an audio file exists or that playback is
audible.

## Known source examples

`assets-extra/android/data/scripts/ai/witch.luac` binds `TELEPORT_FX` at line 2
to `GetPyOID("AnimatedEffectTable", "spell_dh2_prince_mage_stone_escape_magefx")`;
`PlayFX(TELEPORT_FX, ...)` at lines 56 and 84 passes that value to the native
effect namespace. `PlaySound("sfx_teleport", …)` and Witch/Swamp King music
strings reach the native `Sounds` member lookup. Later-boss scripts provide more
exact examples: `darkqueen1_core.luac` reuses the teleport effect,
`flaminite_core.luac` and `roottroll_core.luac` bind animation dictionary rows,
and Sandworm scripts bind multiple named animation rows.

Warrior `Charge` (`skills/prince_warrior_charge.luac:34-35`), `Bashdown`
(`skills/prince_warrior_bashdown.luac:22`), and `Ground Slam`
(`skills/prince_warrior_ground_slam.luac:26`) directly bind `ClassTable` rows for
class formulas; these and their `GetPyStruct("CharacterProperties", …)` property
reads are outside this API-argument resolver. They are not misclassified as
animation/projectile references. The native index and string lookups above are
API admission evidence; these static links do not imply that Witch teleportation,
sound playback, or a skill activation occurred in gameplay.

## Verification boundary and remaining gaps

Focused checks cover exact effect and animation links, direct API argument
position, class and projectile rows, namespace mismatch, duplicate or missing
rows, tuple assignments, branch/function writes, imported global/local scope,
escaped literals, iOS isolation, and preservation of already-resolved rows. The
read-only `global_lua_checks.run(ctx)` additionally checks corpus-wide
determinism and positive coverage. This helper does not change the atlas
generator or run gameplay. Conditional assignment values, concatenation, table
lookups, function-return values, implicit loader globals, duplicate table rows,
and platform-specific script differences remain unresolved.
Visual gameplay evidence is not applicable to this static source-link helper;
the native data consumer and the preserved Android script source are its direct
evidence. The supplied IDA export is Android-only; no iOS binding equivalence is
asserted.
