# B117: chest interaction crashes while moving

Implementation: fresh `/root/b117_chest_crash`; independent reviewer: fresh `/root/b117_chest_review`; integration and release: root. Accepted v21 and its user saves are preserved.

## Investigation before implementation

- Visual evidence reused from B079: original P1 at209.3–212.5s, sampled15fps, shows approach, crouch/sword interaction, opening lid and tumbling drops, then upright movement. It cannot establish the precise input edge. Reviewer independently inspected the original sequence; footage1.0.3 versus recovered1.0.2 remains qualified.
- Recovered logic: AI_InteractWith0x3cff34 checks death/target/type/range and calls SM_SetInteractState0x3c64ac; CSInteract.OnFocus0x3c5548 selects animation and conditionally looks at the container. Authored prince interaction lasts1200ms with interact marker467ms. Container interaction occurs at the marker, not the initial press. See `coordination/v20.1/B079-report.md` and `port/level-world/reference/character-state-methods/original-functions.asm:1511`.
- Current consumer: main accepts Idle/Move container use, plays the retained interaction clip, blocks controls and skips locomotion selection. `CombatSession::play_actor_clip` sets stateManaged=true/stateSequence=false; physical locomotion selection sets stateManaged=false. `update` nevertheless applied neutral-input physical locomotion bookkeeping while the retained clip owned the action. The preserved Move action then disagreed with neutral input and triggered the strict admission error.
- Expected behavior: retain the explicitly admitted animation owner's action until completion; preserve marker/open/drop timing, controller refusal, duplicate prevention and the normal return to authored Idle/Move. No chest, map or class special case, forced idle, or blanket guard removal.
- Focused verification defined before coding: moving→interaction neutral frame advances; completion returns Idle; ordinary unadmitted Move→neutral still fails. Frozen actual full physical chest runs cover moving/open, idle/open, approach arrival, busy/spent presses, pose/lid/drop sequence and movement resumption. Original sound audibility and exact item label landing are inherited B079 uncertainties.

## Baseline reproduction

Root independently reproduced the user's exact failure on frozen accepted v21 executable SHA256 `0D2D0F039667D64719CDE1246FA226E54F9992E6703000220CDD474F4784B538`.

Root evidence: `.local-inputs/b117/root/baseline-physical/`, jobs and summary under its parent. Fresh isolated B079 side fixture, `--enemy-ai`, movement250:261:0,-1, normal Space260. Physical frame binds28actors, context facing and prince interaction start at260, then exact `Physical locomotion must be admitted through the current authored locomotion selection` error; exit1. Worker separately reproduced the same branch.

Earlier root baseline runs lacked `--enemy-ai`. That also omitted physical transition binding, so their successful chest opening is excluded from strict admission verification. This explains why inherited idle-oriented fixtures failed to cover this regression.

## Candidate implementation

Worker commit `dedcd8a5b15214ab9ad33e77b68f19719face6b4`; root integration `7f301b72`. Physical action bookkeeping preserves action only for an admitted managed retained leaf (`stateManaged&&!stateSequence`). The original mismatch error and update remain active without that owner. Public `owns_pose`, callbacks, B083 host-import ordering, animation, container and save code are unchanged.

Focused regression added to `combat_session_actor_transition_tests`: authored moving selection→retained clip→neutral update preserves action/ownership; completion returns Idle; authored Move reclaims ownership; unadmitted neutral update still rejects. Its fake transition callback and Idle asset establish the ownership invariant, not normal chest visual fidelity.

## Integrated verification and accepted v21.1

Build passed. Final frozen executable SHA256 `12E7BBB69449FFACE821E7466171BD81B68F3C878750CE55BA18B18FFA3B1E5F`; production source/test commit `ed90674f0b923657d956a2c540abcc87b5debda1`. Test alias correction8fb6787f/ed90674f changed only the regression fixture, not the production executable.

Root normal full physical tests (`.local-inputs/b117/root/integrated-{moving-open,idle-open,approach-open}/`) all exit0. Moving and idle start260, marker acceptance289, lid/open303, end332; approach240 arrives265, accepts294, opens308, ends337. One opening/drop delivery each; busy270 refused; movement370–390 and release succeed. Root inspected sequential260/270/290/303/320/332 images for pose, lid, drops and upright recovery. Same moving baseline crashed at260; repaired path completes.

Independent reviewer READY: own copied final frozen EXE, isolated windows/saves, full physical moving/idle/approach, busy and fresh spent350 press, postend movement/release, sequential visuals and corrected transition regression all pass. Spent350 naturally selects ordinary source attack after the chest ceases to be OOI; no second interaction/open/loot. See `B117-review.md`. No fabricated stale OOI invocation is claimed.

All175 CTest checks pass across the full run and corrected reruns. Initial full run168/175: six tests exited0xc0000135 because root omitted the LLVM runtime DLL directory from PATH; the new fixture also used an unregistered alias. Corrected alias and toolchain environment; all seven affected tests pass7/7 in3.75s. Full initial log and corrected logs remain `.local-inputs/b117/root/ctest-final.log`, `ctest-transition-rerun.log`, `ctest-corrected-final.log`. This is not relabelled a clean single full-suite run.

Accepted package: `.local-inputs/windows-source-clock-v21.1`,4420 frozen manifest files. Independent audit hashes every entry;4400 game-content entries equal accepted v21. Final acceptance refreshes receipt/notes/README/manifest; EXE/content unchanged. Existing profile, Swamp world, selective checkpoint and tutorial progress were copied separately after acceptance, with matching original/copy hashes in `.local-inputs/b117/root/save-continuation-copy.json`. Original v21 files and saves were never overwritten. B101, B114/B115 and B116 caveats remain unchanged; B117 is resolved.
