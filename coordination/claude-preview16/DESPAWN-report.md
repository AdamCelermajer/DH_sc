# DESPAWN report (Preview 16, branch p16/despawn, from p16/integrate 2d703432)

Status: IN PROGRESS (skeleton). Goals: (1) automatic despawn after death through the lifecycle; (2) end-to-end barrel
moth summon from OnOpen in the real EXE; (3) glue in main.cpp / spawn lifecycle.

## 1. Investigation
### 1a. Death -> despawn state machine (IDA + registration plan)
- Source: `port/level-world/reference/character-monster-state-ownership/registration-plan.json` (OnInit SM_RegisterEvent
  records, original addresses) and `pseudocode-all.c`.
- `CSDead::OnEvent` 0x3c4c3c: event 34 (death anim end): `SetPhysicalObject(nullptr)`; if `!(vtbl+40)(character)`,
  `TMR_Start(Despawn_Delay, event 46)`, flags328=64.
- `CSDead::OnFocus` 0x3c4d50: flags328=577; if the anim is not pending and `!(vtbl+40)`, the same timer.
- Registered transitions (registration-plan): Dead(12): event 46 -> Despawn(2); event 50009 -> Revived(16).
- Despawn(2) OnInit 0x3c7dfc: event 64 -> Limbus(0), event 34 -> Limbus(0).
- `CSDespawn::OnFocus` 0x3c32fc: flags328=512, `SM_SetAnim(-1)`, CancelSneaking.
- `CSDespawn::OnBlur` 0x3c3794: `*(char*)(+1328)=1`; if `!Character::CanRespawn` (line: CanRespawn 0x3a5248 = property 11 > 0
  and group permits): clear local player target; if `+5348 || IsSummoned` (CharType 5) then `ObjectBase::Delete`.
  Otherwise the actor goes to Limbus (hidden) with the respawn timer (CSLimbus::OnFocus, +1328 and RespawnDelay -> event 47).
- Limbus(0) OnEvent 47 -> Spawn(1) (respawn).
- Open point (carried from PROFILES report): the event-46 owner is now resolved (registration plan row 12/46 -> 2).
