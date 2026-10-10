# Bug-only handoff after V19 Preview 11

Work in `C:/Users/adamc/Desktop/workspace/DH_sc`. Read AGENTS.md, docs/BUGS-AND-IMPLEMENTATION.md and docs/RESOLVED-BUGS.md first. The user currently requests bug fixes only, not new feature development. Root owns releases; coordinate shared main/core/CMake edits with the integration lead. Keep systems reusable across maps/classes/weapons.

Preview11 is accepted at `.local-inputs/windows-source-clock-v19-preview-11`; EXE SHA256 FD850478D8ADC2711F47CD3E0EE5A3897EA6FCC9C8C9BFC40E144227CD270A2F. Its manifest, receipt and release notes are authoritative. Do not mutate it or user saves. The source checkpoint also contains unfinished feature helpers: a commit does not mean they are integrated or released. B035 private proposals are unfinished and must be reviewed before applying.

Latest user reports, all OPEN and not yet investigated in this pass:

- B037: Skills cannot activate while Space is held. Investigate original attack-button tap versus hold behavior, skill priority/queue/interruption, and what happens when attack is released before a skill. Do not assume automatic attacking or immediate interruption. Existing B003 proof covers attack recovery AFTER a skill, not skill admission DURING held attack.
- B004/B029: Target selection/ring disappears when Space is released or after skills. Distinguish current combat target, preferred target, sticky selection and visible ring; compare source-specific skill clearing rules and continuous original footage.
- B038: XP awards exist but the HUD XP bar does not fill. Compare live progression, level-relative thresholds, HUD binding and source artwork. Numeric XP tests do not prove rendered bar fill.
- B039: Audio stutters/chops. Reproduce with real production playback; inspect animation-event duplicate dispatch, device/buffer lifetime, clocks, frame stalls and cue transitions before choosing a fix.
- B040: Missing original ambience. Locate authentic map ambient audio and its source lifecycle; preserve looping/focus/transition behavior and never substitute invented samples.
- B041 / B005: Missing white/bluish sword swing/skill trace. Treat weapon trail and ground impact as distinct effects. Resolve exact original effect bindings, anchors, authored timing and blend mode, then verify actual animated GPU output. Decoded packets alone do not prove visible effects.

For each bug, use a fresh bounded implementation worker when implementation is authorized; investigate original video plus IDA/recovered logic, record uncertainty, reproduce, patch, immediately run focused tests, then integrate and verify the actual frozen executable visually/logically. Keep bugs OPEN until integrated checks pass. No new workers were launched in this documentation-only pass at the user's explicit request.

Original reference: https://www.youtube.com/watch?v=z_Zky7qQdYs ; incoming-hit example at 377 seconds. Existing source reports live under port/windows-foundation/features. Search those before repeating evidence collection. No UI automation on the user's live game: isolated CLI native test windows and copied/new saves only.

Update tracker owners/status/evidence after each result. Remove resolved rows only into resolved history with exact build evidence. Package the next verified bug-fix batch promptly, keeping incomplete issues explicit. Do not claim all of Act 1 or full menus are complete. Maintain bug-only priority until the user resumes feature work.
