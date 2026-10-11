# B075 report: vases untargetable / break when walked over

## Verdict
- Walk-over breaking is NOT original. "Untargetable" (no ring) was a real gap in ours and is fixed. Walk-over breaking could not be reproduced (code audit only); likely a Space press edge on an unmarked OOI.

## Evidence
- IDA (libDungeonHunter2.so pseudocode-all.c): DestructibleContainer::Interact 0x3a0da0 (hit counter, stage clip; at zero DestroyGameObject event + Container::Interact), GetInteractionType 0x3a0d60 = 8 (same as an enemy). Physical body is PODecor; PODecor::onCollisionBegins 0x470074 is an EMPTY function, so contact never breaks a destructible (only POItem/POZone/POCharacter react).
- Only caller reaching Interact on a non-Character target: melee hit callback (~line 159054: target +1032, no Character -> vt+152(target)); +1032 is set by Character::UseOOI -> AI_SetTarget(OOI) (CONTEXT-report 1.4). So original = targeted + attack only. OOI chosen in UpdateObjectOfInterest 0x3abb9c (CONTEXT-report 1.1).
- Marker: FX[type] under the target (CONTEXT-report 1.3); type 8 = attack circle, as for enemies.
- Video (Part 1, 4 s contact montages t=200-800 s): no destructible urn/barrel visible or broken, so no direct video observation of vases; not asserted from video. (Tutorial captions 3:44-3:56 describe red circles under enemies only.)
- Ours before: containers were OOI candidates (`Action icon type=8`) but main.cpp drew the ring only for combat actors, so a destructible had no ring. Interact is reached only by the Space press edge on the OOI (and debug --interact-at); no movement/contact path exists. Scripted walk repro failed (start positions off valid floor / intro lock), so walk-over was audited in code only.

## Change
- main.cpp: attack-ring mesh drawn under a destructible OOI (type 8, non-actor, unbroken); hunk marked B075 before the "P14 DROPS" draw block.
- Chest (type 0) circle art (`target_circle_00_chest`) not drawn (art not located; no placeholder).

## Verification
- Quiet A/B, player 120 units from barrel `_prim_DestructibleContainer_05_..._47`, no key: `.local-inputs/claude-preview161/fix075/ab/old.png` (Preview 16: no ring) vs `new.png` (red ring at the barrel). ctest not run (draw-only hunk).

## Gaps
- Space still breaks in one press (PC adaptation, SPACEBTN-report); original walks to the target and swings (timing unverified).
- Mouse/tap targeting of destructibles not implemented.
- Chest ring art missing. Barrel barely visible in the capture (banner overlay).

## Package files required
None.
