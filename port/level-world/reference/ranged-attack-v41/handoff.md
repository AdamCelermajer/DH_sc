# Ranged command V41: source-complete command stage

The new command kernel implements whole CharAI::AI_DoRangeAttack 0x3d076c (1044 bytes). It uses the same AttackState64 and registered Character/TargetState48/State owners. No actor, target, property, inventory or FSM authority is cloned. The world adapter owns only temporary search storage and ordered result pointers.

## Acceptance

* Original ARM execution: 256 contrasting cases, including reentrant heading mutation and selected-list-entry mutation.
* Native O1 and O2 ASAN/UBSAN: 256 cases / 604 exact ordered callbacks each, 11 mandatory provider failures each.
* Three production translation units: strict ARM64 and x86_64 syntax compilation passed.
* Same-world adapter is compile-validated; its runtime registration/inventory composition has not received a new positive graph or live app test. Do not describe that composition as live acceptance.

Source endpoint fixtures supply capability, search, LookAt, debug, melee fallback and SetAttackState. Original list constructor, frontal comparator clone, and SM_IsAttacking execute in the oracle. The complete projectile code was captured for continuation, but is not implemented by this packet.

## Integration

Add `character_ranged_attack_v41.cpp`, `target_frontal_sort_v41.cpp`, and `character_world_ranged_attack_v41.cpp` to level-world only after coherent integration. Retain one CharacterWorldRangedAttackV41 beside the existing attack owner. Borrow SAME AttackState64, ScriptCharacterObject.target, State, GameObject heading-active logical storage, actual TargetServices, and a lease pinning those owners. Pass existing world, attack geometry, debug and file owners.

Backend inventory returns the actual equipment authority. Frontal angle comes from CharacterDesign/Attack_FrontalAngle. SetAttackState invokes source SM_SetAttackState(0,false), preserving its actual animation providers. Melee fallback invokes the same AI's genuine melee command. The direct LookAt fallback uses registered World control services and deliberately does not introduce controller Cmd gates. `attack(requested,speculative)` returns a required failure with a diagnostic when a reached endpoint is unavailable.

## Shared bug and proposed root correction

Existing CharacterWorldPlayerAttackOwnerV1 attack_list_reset_sort assigns list_.sort=0. Original clone 0x3d015c stores comparator from GOT 0x996a6c, whose value is _sortFrontal 0x38d5b4. Native target heap enum maps frontal to 2 and no-sort to 0. Replace the reached clone implementation with `target_frontal_sort_v41(list_)`, or assign sort2 after the original emptying loop. Constructor default sort is not this clone. This packet does not modify the existing shared owner.

## Exact behavior retained

Ranged search uses actual maximum range, not melee radius0. Broad search uses source frontal comparator and 2*pi cone; narrow search uses actual half-angle arithmetic. New commands clear continued78; continuing commands preserve it. Last79 blocks only after IsAttacking. Explicit requested targets on new commands receive LookAt but are not installed with SetTarget. After SetTarget, the source reloads the first result before LookAt. Required-provider failures retain already performed source mutations and dispose only native scratch list storage.

## Remaining projectile stage

Original normal ranged animation event calls ProjectileManager::Spawn(id,owner,NULL,check=NULL,hit=CharAI::_HandleProjectile,context=NULL,crit=false). Its check/hit callback order must not be reversed. Manager validates the actual 72-byte projectile table row, creates/reuses the proper ordinary/laser family, enables it, calls SetInfo, then writes byte85. The source includes owner/target handles, same collision/PF/visual objects, motion, expiry, pooling and callback lifetime. These are required, not replaced with instant damage.

Captured complete constructor/SetInfo/update/collision/expire/manager Spawn and skill-specific projectile hit/check bodies are in source.asm/source.json. _HandleProjectile first calls actual owner CharAI OnProjectileHit, then real F_MeleeAttack/F_ApplyResult for Character victims (offhand false, actual crit byte); non-Character interaction8 calls virtual Activate. That combat path must borrow current retained combat/loot/kill providers, never nearest-target or fabricated success. No RNG surrogate, invented projectile table, hit receipt, live skill acceptance or all-skills claim is supplied.
