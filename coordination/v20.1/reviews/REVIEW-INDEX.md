# Fresh review decisions

These are review results, not release acceptance. Root owns combined frozen
verification, asset completeness and unresolved follow-up.

| Bug | Reviewer | Decision / reviewed source |
|---|---|---|
| B102 | /root/b102_review | READY WITH CAVEATS e40538b8; merged, root equip branches pass; models pending package |
| B096 | /root/b096_review | READY WITH CAVEATS 5dc852ea; merged, root success/refusal/reload pass |
| B091/B094 | /root/boss_cinematics_review | repaired scripted current-state gate READY WITH CAVEATS 3a4aec63; merged |
| B101 | /root/b101_review | core ee9bafe4 ready; residual fire appearance OPEN, owner resumed |
| B105 | /root/b105_review | normal Celeste path READY WITH CAVEATS 51e9a43c; merged and root glow check passes |
| B109 | /root/b109_review | repaired specular/alpha transport READY WITH CAVEATS 1a3e199f; merged, root exit/cave checks pass; user accepts rendering |
| B095 | /root/b095_review | closed-input repair READY WITH CAVEATS 4a5f0012; merged, root queued close no mutation passes |
| B100 | /root/b100_review | marker/Space dependency READY WITH CAVEATS a66b7f9c; full shop gate carried by B110 |
| B112 | /root/b112_review | circles READY WITH CAVEATS 3c802731; docs integrated separately, package checks pending |
| B113 | /root/b113_review | upper destination presentation READY WITH CAVEATS 9623f483; merged; private frozen B106 input dependency still under repair |
| B111 | /root/b111_review | authored sheets/navigation READY 8903fff5; merged; fresh reviewer clicks reproduce values, old save-state captures qualified |
| B110 | /root/b110_review | repaired equipped SELL/capped BUY admission READY 919a0a3d; merged, combined runtime pending |
| B086 | /root/b086_review | repaired real WM_CLOSE persistence READY WITH CAVEATS 90565f73; merged; combined B108/B093 checks pending |
| B106 | /root/b106_review | repaired held-key state READY WITH CAVEATS092b3501; merged with binding once |
| B093 | /root/b093_review | counter preservation ready; full scope NEEDS WORK map phantom markers/current-primary fallback; owner resumed |
| B108 | /root/b108_review | initial producer non_zonable/retry repair READY WITH CAVEATS ab1ee317; merged, combined death/close pending |
| B103 | /root/b103_review | stat-only clicks ready; mixed No skill rollback NEEDS WORK, /root/b103_mixed_repair implements shared visit |
| B079 | /root/container_review | chest focus missing source look-at; /root/b079_facing_repair owns nonaligned proof/repair |
| B085/B089 | /root/container_review | READY with disclosed audio/multistage/timing limits; initial integrated frozen clip/drop/target tests support |

Private reviewer evidence remains in `.local-inputs/claude-v20.1/` or
`.local-inputs/v20.1/`. Frozen hashes and qualified test routes are recorded in
the individual reports and INTEGRATION-REPORT.md. Independent tests passing do
not promote unknown original behavior or broader actor/class branches to verified.
