# B106 — dedicated E travel and held-key session repair

Owner: b106_press_repair; reviewer: b106_review. Branch fix/b106, reviewed integration base 314b7204 merged as 578e50b8. This contains the original d11cb5c4 E-binding change plus the repair below; it includes reviewed B113 presentation, B111, B105 and B109, with no B086/B093/B108 source.

## Investigation before implementation

Visual: inspected original Part1 v1.0.3 sequence `.local-inputs/v20.1/fix113/evidence/p1-713-728-5.jpg`, 713–728 s at 2 fps. Separate Bogwitch Cavern/World map buttons appear ~715.5–717.5; loading follows; arrival ~724.5 presents The Boglands return control; departure ~727 hides them. The earlier report's missing-footage statement is superseded. Shot-23 shows the hero near Rene and a travel prompt, but does not establish object-of-interest state or actual key failure. E is a deliberate PC adaptation; neither the still nor mobile footage proves keyboard rules.

Logic directly examined: IDA TriggerZoneExitLevel::Update 0x0039c33c, pseudocode-all.c:115396 onward: current-level host byte324, local player/touching, local hosting, MeetCondition, zone field501 != -1 and display-edge byte2073 admit DisplayFastTravel; leave/failed admission hides it. NativeGoToZone 0x004422b8 (:230532 onward) admits host, normalizes entry -1 to0, QuickSave at level state38, SG_SaveAllPlayer, character slot/LevelList lookup, HUD hide, LoadLevel. B113's recovered SWF btn_LocTravel onRelease0x181c3 invokes travel separately from context attack/talk. No NPC OOI condition belongs to dedicated E travel.

Production caller: main.cpp's armed prompt lookup and dispatcher retain gameplay-pause, cinematic, alive/controller, hero-interaction, and pending-transition admission. E reads uiInput.actions.interact independently of OOI; Space remains the existing conditional alias when OOI is absent. The scripted --transition-key feeds E through the same SemanticInput as Win32 input, rather than the legacy private-confirm branch.

Failure reproduced by the unchanged independent review: `.local-inputs/v20.1/b106-review/held-E/run.log` confirms Cave70; arrival71 fabricates SWAMP71; leaves72 while scheduled E remains held70–74. SemanticInput was constructed inside the outer session loop with empty keys_. Its down&&!was rule therefore recreated the held key's press edge. The frontend-created Window/Win32 key state survives loading; pendingTransition only deduplicates within one session.

Expected behavior and minimal fix, defined before editing: one E press travels once, an immediately armed arrival return control cannot consume that same held press, and release/repress can return. Preserve held keyboard movement/attack levels, avoid fabricated skill/profile/Escape press edges, preserve modal suppression, and drop departed-session pointer captures/pending events. No zone cooldown, map-specific suppression, inferred original keyboard rule, or change to native admission was added.

Verification defined before coding: session-boundary regression for E/Space/skill/profile/Escape/movement; stale pointers/pending events; modal suppression; release/repress and existing focus/modal tests. Freeze the executable; run unarmed refusal, pause refusal/close/newpress, Space alias, single E, held E70–74, release/new E90, hidden Win32 held and 10ms pulses; inspect frame sequences and run full CTest. All tests use private windows and isolated saves.

## Implementation

SemanticInput now lives beside the persistent Window outside the outer session loop. New reusable `replace_session_surface` clears scene-specific pointer captures/pending events and transient button edges, preserves keyboard levels and suppression, and replaces scene callbacks. Existing `set_surface`, menu cancellation, focus and level cancellation semantics remain intact. This also preserves continuous held movement and attack without creating a new arrival press. Release then new press restores normal key edges.

The original d11cb5c4 dedicated-E binding remains required: E confirms an armed exit regardless of OOI, while Space keeps its previous OOI priority. --transition-key uses real semantic E admission. Integration must include this binding and the persistence repair exactly once.

## Isolated and source tests

- Expanded semantic_input_tests PASS: E, Space, skill1, profile, Escape, movement across session replacement; release/repress; pending travel discard; stale pointer release; new pointer acceptance; retained modal suppression. Existing edge, focus, multi-source and controller gate tests pass.
- Direct source_owner_contract_tests PASS: genuine Level198, leases, release-before-replace, expired epoch rejection. No recovered-source ownership contract was modified.
- Full CTest 169/170 PASS in 18.72s; sole documented worktree failure session_skill_binding (`Asset path escapes asset root`). Semantic input and source-backed level_transitions pass. Build exit0; git diff --check passes.

## Frozen production and visual verification

Executable SHA256 **52B560B3B7354DBFD1179CC2D3A2B7F1EE77F3B5BBC74C5197F4872EAACB9017**. Receipt/source snapshot: `.local-inputs/v20.1/b106-repair/frozen/`; exact combined B106 diff against reviewed integration is B106-combined-source.patch. Six copied independent-review fixtures use new save paths and this frozen executable; jobs.json and summary.json preserve args/logs. All exited0 without timeout.

| Fixture | Observed production result |
|---|---|
| unarmed | E70 produces no confirmation or transition. |
| pause-refusal | E70 refused during pause60; close80/new E90 confirms once, arrival91. |
| space-alias | Space70 confirms once, arrival71. |
| single-E | E70 confirms once, arrival71. |
| held-E | Consecutive E70–74 confirms Cave70, arrival71 arms The Boglands; no return through frame120. |
| release-repress | Held70–74 produces only Cave70; released75; new E90 confirms SWAMP90, arrival91. |

Viewed `held-E/frame65.png`, frame75.png and frame100.png: bronze Bogwitch Cavern button changes to The Boglands on cave arrival and stays in the cave without an automatic return. Viewed release-repress/frame100.png: back in Swamp after the genuine new press; active quest banner correctly suppresses the destination presentation. This uses reviewed B113 presentation and does not reopen or make a new B109 rendering acceptance claim.

Hidden Win32 receipt `win32-short`: quiet_click posts a 10ms WM_KEYDOWN/UP E pulse at5s; focus=True, exit0, one Cave confirm268/arrival269. This establishes that pulse succeeded in this run, not a guarantee for all subframe timing.

Hidden normal frontend receipt `win32-frontend`: uses accepted v20 assets read-only for both gameplay/menu identity, a copy of this repair's isolated saved profile, frontend Single Player/Continue, and the same persistent HWND. E down at5s held3500ms crosses Cave confirm53/arrival54 with The Boglands armed54 and no automatic return; release8.51s/new press10s for80ms produces SWAMP225/arrival226, exactly two genuine confirmations overall. Exit0 and focus=True; injection.txt records both key releases. Earlier attempted mixed asset roots failed the source identity guard before gameplay and were corrected before this successful receipt.

The direct-start `win32-held` receipt travels once, but cannot prove release/repress through its original HWND: existing direct-start initialization leaves windowOpened false and reopens the HWND during travel. Its second message targets the departed handle. This separate startup-window issue is disclosed to root and was not changed by B106. The normal frontend receipt proves physical-message persistence and genuine return through the production frontend path. Real user hardware remains untested.

## Remaining ownership and package requirements

Re-review by the same b106_review and root integration/package acceptance remain required; B106 is not closed in the shared tracker. No accepted package, user save, user's game window, root main/CMake, or other worktree was modified. Runtime art/assets remain the reviewed B113 dependencies; this repair adds no package asset. Exact mobile slide/press timing, World map menu and broader input timing remain outside this scope. Preserve the direct-start HWND caveat for root follow-up.
