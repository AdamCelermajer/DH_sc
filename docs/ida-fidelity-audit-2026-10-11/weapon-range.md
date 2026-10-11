# Weapon / attack range: end-to-end IDA vs Windows reconstruction audit

## Progress
- last_address_done: 0x3dc698 (AISDefault::OnTargetOutOfRange)
- groups_done: data sources, melee radius, target search + sorters, AI_DoMelee/RangeAttack, IsIn{Melee,Close,}Range, InteractionRange, _UpdateTarget events, AISDefault range handlers, hit-time OnAttack gate
- next: port enemy_ai range wiring; port ranged/projectile path; skills range; real data numbers; Attack_FrontalAngle value

## Working notes (IDA side, verified)
- AIProps layout (Structs::AIProps::read 0x506f3c): +24 InteractRadius f, +28 LeashDistance f, +32 MeleeRadius f, +60 ViewRadius f, +64 ViewRadiusNoAggro f; stride 68.
- Character::GetCharAIId 0x3a3020-ish: props word1 (Character dword 1023) with fallback 8 when out of range.
- ItemInventory::CanMeleeAttack(int&) 0x3fff30: returns 0 when ranged weapon equipped (vfunc+8); else reach = main-hand Item dword 39 (byte +156, Param5), 0 if empty hand.
- CharAI::AI_GetMeleeRadius 0x3d4c34: (float)itemReach + AITable[aiId].MeleeRadius.
- CharAI::AI_IsInMeleeRange 0x3d6188: non-character/interaction!=8 -> AI_IsInInteractionRange; else 3D squared distance between GetTargetPosition() points; in range iff (selfMelee + targetMelee)^2 > d^2 (strict).
- Character::CanRangeAttack(int&,int&,int&) 0x3a4cd0: if props RangeProjectileID (word32) != -1 -> min=word30>>8, max=word31>>8 (24.8 fixed), proj=word32; else ItemInventory::CanRangeAttack -> Item dwords 38/39/40 (Param4 min, Param5 max, Param6 projectile).
- Character vtable +148 = Character::GetInteractionRadius 0x3a374c = AI_GetMeleeRadius; +184 GetObstacleRadius = 50.0f constant; GameObject::GetInteractionRadius = 0.5*min(AABB x extent, y extent).
- ObjectSearcher::TargetList::Search 0x4a2f34: surface = sqrt(3D d^2) - target.vfunc148 (InteractionRadius) - searcher AI_GetMeleeRadius; accept surface <= maxRange; angle = |Point3D::angle(delta, lookAt)|, accept if maxAngle >= 3.1416 or angle <= maxAngle. Origin = searcher GetTargetPosition, heading = GetLookAtVec.
- CharAI::AI_DoMeleeAttack 0x3d01ac: if CanRangeAttack -> AI_DoRangeAttack. Target search maxRange 0.0; cone = (Attack_FrontalAngle >> 1) * 0.017453 rad when char byte 437 set, else full 6.2832 (sorted by SetSortType). Explicit target arg wins. Attack state entered only if AI_IsInMeleeRange(target) (or no target -> attack with null target).
- SetSortType(int) [clone] 0x3d015c ALWAYS installs _sortFrontal (asm 0x3d0194..0x3d019c). TargetList ctor(a5=1) installs _sortClosest. Sorters: characters before objects; _sortClosest by TargetInfo+4 (surface distance) ascending, _sortFrontal by TargetInfo+8 (|angle|) ascending.
- So AI_DoMeleeAttack: heading_active (GameObject byte 0x1b5, set by GameObject::SetHeadingDirection = steering input) -> cone Attack_FrontalAngle/2, closest-first; NOT steering -> full circle, smallest-angle-first (frontal sort). Both with maxRange 0 => only targets already within (selfMelee + targetInteractionRadius).
- AI_DoRangeAttack 0x3d076c: same structure, maxRange = (float)RangeMax (CanRangeAttack a3), LookAt target, SM_SetAttackState(nullptr) unconditionally (no in-range gate, min range unused).
- CharAI::OnAttack 0x3d0e7c (anim event attack_mainhand/offhand via _OnAnimEvent 0x3d4434 case 5): gate AI_CanAttack(current target) = IsEnemy && ((melee weapon && AI_IsInMeleeRange) || (CanRangeAttack && AI_IsInRange)); then AISDefault::OnAttack 0x3dc12c -> F_MeleeAttack + F_ApplyResult on current target. AISPlayer::OnAttack is a thunk to AISDefault.
- attack_ranged / attack_mainhand / do_skill in state 5 with a ranged character -> ProjectileManager::Spawn(projectile id from CanRangeAttack a4).
- AI_IsInRange 0x3d6604: min^2 <= d^2 <= max^2 (ints from CanRangeAttack). AI_IsInCloseRange 0x3d63d8: (character, interaction 8) ranged only: min^2 > d^2. AI_IsInInteractionRange 0x3d4f98: sqrt(d to InteractionSpot) - selfMelee - targetInteractionRadius; for interaction type 8: <= 0; else <= AIProps.InteractRadius (or 80.0 when target+744 set, radii 0).
- CharAI::_UpdateTarget 0x3cb908: events 17 InMelee / 14 OutOfRange for melee; ranged: 16 InClose (d<min), 15 InRanged, 14 OutOfRange.
- AISDefault::OnTargetInMeleeRange/InRangedRange: Cmd_Stop + Cmd_Attack(target). OnTargetInCloseRange (enemy, seeking): MoveTo(self + (self-target)) i.e. retreat by the full separation vector. OutOfRange: MoveTo(target InteractionSpot).
