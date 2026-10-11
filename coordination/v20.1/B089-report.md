# B089 report: destroyed vase keeps its red target ring and stays targetable

## Verdict
Real gap in ours (reproduced in the real EXE); fixed generally for every container (destructible and chest).

## Evidence
- Screenshot shot-9: vase debris on the floor, red ring still under it.
- IDA Container::IsInteractive 0x39f384 = `!flag(+129) && state(+916) == 2` (idle only); Character::UpdateObjectOfInterest 0x3abb9c re-checks the stored OOI every frame through vt+140 and zeroes it when false (the 500 ms timer only re-selects). Container::Interact 0x3a0b38 leaves state 2 at once (SetState 4, then 3 + "activate" clip; no visual -> DoOpen), and drops the physical object. DestructibleContainer::Interact 0x3a0da0 final hit calls Container::Interact. So in the original a hit-to-destroy object is never a candidate/OOI again: no ring, no Attack/Open context.
- Video: no destructible visible in Part 1 (earlier B075/B085 montages); not asserted from video (ring-under-vase after destroy is therefore not observed in the video either way).

## Root cause (two gaps)
1. main.cpp interactable provider returned -1 only for a destructible in state 4 (opened). During state 3 (activate clip, plus the whole clip after the final hit) the barrel stayed a type-8 candidate; chests in state 3 too.
2. ObjectOfInterestOwnerV1::update per-frame validity checked only `eligible`, not that a non-Character still has an action type, so even a -1 object stayed OOI up to 500 ms.

## Change
- main.cpp (B089 hunk): container candidate type is -1 unless state == Idle (source IsInteractive).
- features/combat/object_of_interest_owner_v1.cpp: stored non-Character OOI is dropped the same frame its type becomes -1.
- Test: object_of_interest_owner_v1_tests (3 B089 checks: selected while idle, dropped before refresh timer, stays out after refresh).

## Verification (real EXE, quiet, silent, isolated save; barrel _47, Space at frame 40, hit 56, loot 72)
- Before (fix/b085 build) frame 160: ring still under the debris (.local-inputs/claude-v20.1/fix089/b160/cap.png). Frame 100 same (b100).
- After (fix089 build) frame 160: no ring (a160/cap.png); a100 also captured.
- Build exit 0; ctest 165/166 (only session_skill_binding, allowed). OOI owner test compiled standalone: pass.

## Not verified / gaps
- Mouse/tap target selection of destructibles does not exist, so no click path to check; Space/context uses the OOI, now empty after destruction (press then falls to the generic held melee swing, same as source "no OOI" Cmd_Attack(null)).
- Multi-stage (stages>0) destructibles stay Idle between non-final hits, so they stay targets until the final hit (as source state 2); not present in Swamp, untested live.
- Debris staying on the floor after the break is the activate clip's last pose (not removed); whether the original removes the mesh later (e.g. despawn) not investigated here.
- Cached icon type can linger up to 500 ms after destruction (as in source, which only clears the pointer).
