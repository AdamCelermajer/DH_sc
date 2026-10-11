# B074 hero attacks from too far - report (INCONCLUSIVE, no code fix)

## IDA (libDungeonHunter2.so pseudocode-all.c)
- CharAI::AI_GetMeleeRadius 0x3d4c34 = weapon Param5 (0 if empty hand/ranged) + AITable[aiId].MeleeRadius (+32).
- CharAI::AI_IsInMeleeRange 0x3d6188: (rSelf + rTarget)^2 > 3D squared distance of GetTargetPosition points (strict).
- CharAI::AI_DoMeleeAttack 0x3d01ac: player search = TargetList::Search 0x4a2f34 with maxRange 0.0 (sqrt(d^2) - target InteractionRadius(vt+148 = AI_GetMeleeRadius) - own melee radius <= 0, cone Attack_FrontalAngle/2); attack state only if AI_IsInMeleeRange. Hit-time gate CharAI::OnAttack 0x3d0e7c -> AI_CanAttack. No player auto-approach found in this path.
- Agrees with docs/ida-fidelity-audit-2026-10-11/weapon-range.md.

## Ours (measured, quiet run, build fix074, Rogue + Longsword01, Limbus-gated lizards)
Temporary diagnostic (not committed) logged positions at hit markers. AI MeleeRadius is 200 for Player, Moth, LizardMan; weapon Param5 = 0, so the reach sum is 400. Hits landed at 3D distance 175.6 (lizard) and 399.4 (moth). Our formula (combat_system.cpp attack_target_in_range, playable_actor_world melee_reach) equals the IDA one, with data decoded from the original pydata.

## Video (z_Zky7qQdYs, 272-285 s, Bog Moth fight, 640x360, frame sequences 5-10 fps)
Hero swings at moths with hero-centre to target-ring distance about 1.1-1.2 hero heights. Ours: ~0.4 px/unit at 1200 px width, hero ~325 units tall, so 400 = 1.2 heights. Consistent.

## Conclusion / unknowns
- Reach number matches IDA and the video within measurement error; no range divergence found.
- Not implemented: GetTargetPosition (node position, 0x3935dc) is not used for the melee distance (ours uses actor origin); effect is mostly a z offset, small.
- Possible visual cause, UNVERIFIED: our sword swoosh arc looks larger than the video's (video arc radius ~0.8 hero heights, ours ~1.15 by eye). Needs a same-frame swoosh comparison (B041 area).
- No test or code change. Package files required: none.
