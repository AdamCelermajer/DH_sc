# `AISDefault::OnAttack` source map

Binary: `libDungeonHunter2.so`, SHA-256 `36498eb8…2f5e80`; IDA 9.3 analysis
of the matching ELF (image base 0).

- `CharAI::OnAttack` `0x3d0e7c` first calls `AI_CanAttack(nullptr)`
  (`0x3d67f4`). Only on true, and with an active AIS, it dispatches virtual
  slot `+0xa8` with attack index, step index, and offhand flag.
- `AISDefault::OnAttack` `0x3dc12c` gets the owning Character from `AIS+0x98`
  and calls `CharAI::AI_GetTargetAsCharacter` `0x3d5450`. That resolves the
  current `CharAI+0x40` ObjectHandle; a missing handle yields null.
- With a target, order is `Character::F_MeleeAttack`
  (`0x3b3368`, offhand, alternate=false), then `Character::F_ApplyResult`
  (`0x3b10b4`, mode=false), with the same attacker/target identities.
- Without a target, it calls `Character+0x408` CharAI virtual slot `+0x98`:
  `CharAI::OnAttackDelayExpired` (`0x3d0c7c`), which dispatches active AIS
  slot `+0x8c`. `AISDefault::OnAttackDelayExpired` (`0x3dbee8`) is an empty
  leaf; an external AIS override is not assumed empty.

## Integration boundary

The current renderer routes authored melee animation events to
`apply_actor_attack`, which already applies the selected native melee/result
owners and same-owner combat-result callbacks. It does not execute the source
`AI_CanAttack` gate or resolve the source ObjectHandle, and its AIS projection
has no `+0xa8`/`+0x8c` virtual dispatcher. Calling `OnAttack` alongside the
existing animation hit would double-apply damage. Keep this source callback
unwired until the active-AIS virtual provider and handle-to-canonical-Character
resolver exist; when connected, replace the direct hit edge rather than adding
a second one.

