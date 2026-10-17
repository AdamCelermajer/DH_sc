# Character::_SpellCombatRoll

The selected ARM `libDungeonHunter2.so` (SHA-256 `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`) identifies `_ZN9Character16_SpellCombatRollERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv` at `0x3b92a4` (size `0x128`). REA/IDA evidence shows this caller order:

1. `SG_GetCurrentFaerieId(-1)` at `0x3b92c0`, before reading `Arguments[0]`.
2. Accept Value type `2` or `7`; call `getUserData`, then `ObjectBase::GetHandle` and `ObjectHandle::operator Character*` at `0x3b9330`.
3. For a Character target, call `GetCharFaery(selected_slot)` at `0x3b9344`, read row word `+8`, call `F_SpellAttack(..., element, 0)` at `0x3b9364`, then `F_ApplyResult(..., false)` at `0x3b9378`, and push the post-application result amount at `0x3b9384`.
4. If conversion returns null, the first virtual value query at `0x3b939c` is compared with integer `8` at `0x3b93a0`. Only that branch calls vtable slot `+0x98` at `0x3b93b8` and pushes boolean false at `0x3b93c4`.

`F_SpellAttack` at `0x3b3004` first loads `DebugSwitches`, constructs/queries `isTracingChar_Attack`, destroys its string, then forwards `_F_CalculateResult` mask `0x1005554A`, category `-1`, selected Faery row word `+8`, and direct amount `0`. The portable caller in `character_spell_combat_roll_v1.cpp` owns the exact branch/order and requires typed providers for the original callees; it does not synthesize a target, combat result, or application. Production app binding supplies those providers through the existing Player skill Runtime and the shared Character application path.

The five retained `faerie_{celest,hotty,rocky,wetty,windy}.luac` scripts call `SpellCombatRoll(target)` from `OnSkill` after `GetTargetListTop`/`PopTargetList`. Character targets use the combat branch; selected object targets use the source unresolved-userdata branch (some scripts call it twice for destructible objects).

The focused host test checks source order, exact spell calculation inputs, both result branches, and stopping at a reached provider failure. It validates the caller contract only; it does not claim the app-side providers or live spell behavior by itself.
