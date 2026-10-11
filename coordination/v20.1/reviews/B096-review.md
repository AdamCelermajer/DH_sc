# B096 independent review — 2026-10-11

Reviewer: fresh `/root/b096_review`, GPT-6.1 Sol HIGH, read-only.
Reviewed clean `fix/b096` tip `5dc852ea`, merge base `dfc67d8d`, core `ab2512cb`.
Verdict: **READY WITH CAVEATS**, no blocking finding.

- Original P2 394–397 s sequence shows selection, Inner Strength rank1→2,
  points2→1 and Upgrade art. Old 507–510 s and SemanticInput hypotheses are
  explicitly superseded by the current investigation.
- Source GetCharSkill `0x3bc784` resolves class position through SkillList then
  global SkillTable. IsSkillAvailable `0x3bca50` uses signed Level>=required;
  CanIncrementSkill `0x3bc9ec` compares u16 rank with signed level-minus-required.
  Assembly MOVGE/MOVLE confirmed. Reviewer examined IncSkill `0x3bcc58`, native
  train `0x43d548`, SWF selection/release, initial-grant and legacy-menu callers.
- Both gates now resolve validated live unscaled property28→SkillList[position]
  →global table; saved ranks stay by class position. Point/level/difficulty/
  ownership gates, staging, recalculation and potion capacity remain intact.
- Regression oracle uses the separate existing progression resolver. 48 Knight
  comparisons cover rank0/rank1/Normal cap/locked cells; existing tests cover
  nonmutating probe, no-point/out-of-list, publication and save/load.
  Focused CTest and independent level2 invocation pass. Retained full suite is
  167/168, only permitted session_skill_binding failure.
- Inspected shot15, baseline/success/refusal/reload images and logs. Baseline has
  no Upgrade for Battle Hardened rank0/points1; fixed capture does; exactly one
  commit leads to rank1/points0, followed by refusal. Locked run refuses twice;
  reload retains state. Worker frozen hash independently confirmed:
  `7BBAD649882A1DE5293B09CFAED3A2CBAD41642D153F558DEA172CBFCE24AFC7`.
- Input latch cf6d0fc9 and B102 8b5bfcee have identical stable patch ID
  `c003da42d3e9d2d9222a1f72f7cee1dea0f6f810`; import once with coordinate support.

## Integration obligations

Repeat success/refusal/reload on merged frozen EXE before resolving B096.
Describe route precisely: --menu-actions dispatches actual frontend Continue,
--skills-page-frame opens Skills diagnostically, then real posted10ms selection/
training clicks. Natural level-up and physical frontend navigation not proved.
Battle Hardened level2 admission is source/data-backed, not video-observed.
Passive details/effects remain a gap; continuous-probe button refresh differs
from SWF post-spend refresh timing. Test-only point grant assigns both balances,
so unspecified balance is zeroed; account for it in fixtures.

Reviewer made no tracked edit, merge, push or game-window interaction. Root
recorded this summary from the returned independent review.
