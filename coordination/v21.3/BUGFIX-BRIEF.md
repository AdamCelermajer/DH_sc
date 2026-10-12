# New user bug batch — 2026-10-12

Root owns integration, shared main/core/build files and releases. Fresh implementation sessions per stable bug ID; independent domain reviewers. Follow root AGENTS.md: investigate original visuals and IDA/recovered logic including callers/admission before implementation, record evidence and focused observable tests, then implement smallest reusable repair and verify actual frozen production path. Reuse valid prior evidence, distinguish unknowns and PC adaptations. A helper/audit/compile does not resolve an implementation bug.

## Workspace and isolation

Repository `C:/Users/adamc/Desktop/workspace/DH_sc`, current root has reviewed B117 chest and B118 Continue fixes. Final B118 frozen exe `.local-inputs/b118/root/dh-foundation.exe`, SHA256 `3D61A74EC3451011DADB70219CF97D83F11FE80AF5B783CF2C5897566433F306`. Source integration `11f82581`, root now includes its docs. Accepted v21/v21.1 and pending Continue-only v21.2 candidate must remain untouched. B118 passes 22 focused tests and independent actual Continue/save/quit/reopen. Separate B116 F9 targetable-role guard remains open.

Disk space is limited. Do not create full checkouts, copy whole packages/assets, configure full private builds, or delete old files. Work in own bounded `.local-inputs/v21.3/Bxxx/` scratch with read-only root sources and private copies of only changed source files. Produce a concrete patch/API proposal for root, who owns shared integration. Write only your own `coordination/v21.3/Bxxx-report.md`. Do not change root behavior/tracker/build files yourself. No two writers may edit the same file. You remain implementation owner through integrated runtime proof; do not stop at an audit or helper pass.

Use absolute compiler `C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe`; CMake/Ninja under `%LOCALAPPDATA%/Android/Sdk/cmake/3.22.1/bin`. Root reuses `DH_wt/v201int` and `build-v201int` incrementally. For small private tests use actual owned feature sources. Keep toolchain runtime bin on PATH. No extra agents; root dispatches all assignments concurrently.

Hidden, silent, isolated own windows/saves only: `port/windows-foundation/tools/quiet_run.ps1`, `quiet_click.ps1`; own cwd and save/capture paths, absolute asset roots from accepted package or `.local-inputs/v20.1/root/assets-integration`. Never control the user's game/window or edit/delete their saves. Do not recursively search all DH_wt. Current save snapshots and copied supplied screenshots are under `.local-inputs/v21.3/` when root finishes preparation; earlier read-only `.local-inputs/b118/user-seed` is available already. Make further seed copies per run; do not overwrite snapshots. Screenshot/version and session-order uncertainty must be explicit. Supplied images show current defects; original comparison still requires original footage/handoff for that feature.

## Assignments

| ID | User report / scope | Implementation owner | Reviewer |
|---|---|---|---|
| B119 | No selling-item sound; recover exact original successful/refused transaction cues and wire real sale path | b119_sell_sound | batch_audio_review |
| B120 | No buy sound; original transaction cue and normal purchase path | b120_buy_sound | batch_audio_review |
| B121 | No stat point allocation sound, including original increment/confirm/cancel semantics | b121_point_sound | batch_audio_review |
| B122 | Deep investigation and repair of broader missing sounds; map original callers/cue IDs/assets/admission and runtime output, not only sample existence. Coordinate B119–121 proposals; reusable audio routing fixes and follow-up ownership for gaps | b122_audio_research | batch_audio_review |
| B123 | Repeated similar stock items in merchant: identify actual instance/template/affix IDs, investigate original stock generation before assuming duplicates are incorrect | b123_merchant_stock | batch_menu_review |
| B124 | List hard to navigate/down-scroll; verify authored controls and requested PC wheel/keyboard/drag adaptation | b124_list_navigation | batch_menu_review |
| B125 | Rusted Ring shows no stats: inspect actual original item row/affixes/detail rules, prove whether missing or valid; never invent stats | b125_ring_stats | batch_menu_review |
| B126 | Enemies again refuse to die/disappear; exact green lizard and folded moth screenshots. Reopen actual lethal/state/lifecycle production path with current session evidence; no HP-only assumptions | b126_mob_death | batch_gameplay_review |
| B127 | Brown “bogmonp” still untargetable; identify actual actor from screenshot/assets/placement, do not substitute previously tested captain/TRUNGUS | b127_brown_target | batch_gameplay_review |
| B128 | Loot cannot be collected after fight; investigate source interaction admission, controls/target/range/drop state | b128_loot_pickup | batch_gameplay_review |
| B129 | Equipment tab becomes unresponsive; other menu tabs still respond. Reproduce after combat and trace actual native UI/input visit/gates | b129_equipment_input | batch_menu_review |
| B130 | Potion stopped working while stock remains6; source gates/cooldown/owner/control state and HP/actual consumption | b130_potion | batch_gameplay_review |
| B131 | After death, clicking/checkpoint restore puts hero where died and stays unresponsive; trace death End, UI, true saved checkpoint state, world/source state/control restoration | b131_death_checkpoint | batch_gameplay_review |

## Supplied visual references (current-game evidence)

1. Merchant repeated Worn Mallet/2H Cleaver/Splitter rows and lower list; item costs240, gold202. `codex-clipboard-802efbb4-fc30-4b30-b28c-b370f3c7242f.png`.
2. Rusted Ring selected, value55, blank statistics pane. `codex-clipboard-44630f0f-6423-44c2-8edb-1edfd7f44156.png`.
3. Upright green lizard near hero/priest on wooden pier, no target display; potion stock6. `codex-clipboard-4bf1ec08-5bae-4062-bee0-b1c29e2a6d92.png`.
4. Large brown armored actor beside hero/priest, blue overhead interaction symbol, no target display. `codex-clipboard-ed93043d-f462-47f5-b59c-b7ad27dff8fb.png`.
5. Open chest/drop “Conscript Armor” near rock/water; hero beside it. `codex-clipboard-a79156a6-13a1-4a00-83ea-73b443973ae2.png`.
6. Stats: level1, HP138/165, MP27/27, XP94/100, STR12/DEX5/END8/ENE3, pointsleft0. `codex-clipboard-024db625-4ed6-4c04-8980-7c7003bc010a.png`.
7. Folded moth beside hero/priest in water, potion6. `codex-clipboard-3197ae0e-8f69-4965-9420-a50b00428fa0.png`.
8. Hero lying/falling with HP bar empty, green lizards nearby, potion6. `codex-clipboard-b60fe525-427e-4b1b-bd08-a8eb0f0a294b.png`.

Do not conclude death/targetability/cooldown/animation continuity from stills alone. Input/state traces and sequences are required. Gameplay defects may share a stuck-action/controller cause; cross-coordinate findings promptly while retaining separate named bug ownership and verification.
