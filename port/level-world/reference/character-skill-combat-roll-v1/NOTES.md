# Character::_SkillCombatRoll

Source: `libDungeonHunter2.so`, SHA-256
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
Live IDA pseudocode for `_SkillCombatRoll` at `0x3b9fbc` establishes that the
source caller owns the skill-row and target resolution, selects the hit flags
and equipment hand, invokes `F_SkillAttack`, then calls the same
`F_ApplyResult` owner used by melee and spell hits. This module is the
portable caller choreography; calculation and application stay injected into
the shared owners.

- The Character skill-list bounds and accepted argument forms gate the call.
- Target userdata converts through its ObjectHandle. A non-Character target
  runs the source `Value::getValue` check; tag 8 then consumes the value and
  returns boolean false.
- `GetCharSkill(index)` supplies skill type at `+116`, flags at `+28`, and
  damage category at `+20`; only skill types 2 and 7 continue.
- If the skill flags contain `0x00800000`, query main hand first, calculate
  and apply it, then query offhand and calculate/apply with flag
  `0x04000000` added. Each leg's amount is returned only after application.
- Otherwise, one `F_SkillAttack(flags, damageCategory)` and one
  `F_ApplyResult(..., flags & 0x00800000)` run. That final boolean is false
  because the non-dual branch already tested the bit clear.

The caller kernel is selected by the real Player
`Fn::character_skill_combat_roll` native callback in
`player_skill_gameplay_native`; its adapters reuse the Player's existing
`F_ApplyResult` owner. `MonsterScriptSession` now registers the same native
name on the retained AIS VM and forwards its arguments/ReturnValues through an
optional app provider; a production Ghost provider must use the same Ghost
`F_SkillAttack` and `F_ApplyResult` owners before NPC integration is complete.

This recovers caller choreography and ReturnValues; it does not replace
`F_SkillAttack`, `F_ApplyResult`, VM lifecycle, projectile creation,
target-list selection, or script-authored area iteration. The source callback
`_ProjectileHit` at `0x3dae7c` dispatches `SetSkill`/`OnDelayedSkill`; projectile
hit behavior is then owned by the retained skill script.
