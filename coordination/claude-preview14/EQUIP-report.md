# Preview 14 report EQUIP (branch p14/equip)

Status: IN PROGRESS (continuation worker). Update this file as work lands.

## 0. Starting state (WIP snapshot 22508fcd reviewed by the continuation worker)
Reviewed `git diff cedb3a99 HEAD`: per-slot Auto-Equip, ALL, class gate, SortByValueAndClass and 202/203 policy flags were already
implemented and tested. Re-checked against IDA: `Character::EquipAllSlotsAuto` 0x3a4c14 (unequip 0..n-1, auto n-1..0, retry empty except 1/2),
`ItemInventory::_EquipSlotAuto` 0x3fe..., `ItemInstance::IsEquippableBy` 0x3fa330 (default class code keeps the player's class = unrestricted),
`ItemInventory::IsItemEquippable` 0x3fdcbc (Character+4896/+4900 = property cells 202/203 = dual-wield / one-hand-two-hander flags).
Build: `p14_build.ps1 -Name equip -Test`: 103/104 ctest pass. `session_skill_binding` fails with "Asset path escapes asset root"
(environment: the worktree `.local-inputs` junction; unrelated to EQUIP, fails the same way on the baseline tree layout).

## 1. Auto-Equip (done in WIP, verified by tests)
## 2. Class restriction / rules (done in WIP)
## 3. Drop and Transmute
## 4. B045 second-row hit regions
## 5. B042 leftovers

## Needs from schema v4
## Package files required
## Verifier script
## Open risks
