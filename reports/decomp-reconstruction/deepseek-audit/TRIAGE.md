# DeepSeek audit triage (owner DS)

Source: 8 top-level `findings-*.csv` + `_parts/findings-*.csv`, deduped by address+kind: 14 unique findings (16 rows). Each verified against
`recovered/native/hexrays-all` and the ARM listing (`tools/decomp_asm.py`). Binary wins. `check_tu.sh` OK on every touched source.

| Address | Function | Sev | DeepSeek claim | Verdict | Fix made |
|---|---|---|---|---|---|
| 0x370020 | PlayerManager::_PackInventory | high | slot loop condition differs | REAL. Original `v8 - 1 > 1` is unsigned: slot 0 and slots >= 3 fold their own item id, only slot 1 folds the id sum of slots 1+2. Recon used a signed compare, sending slot 0 down the sum path (hash differs, so replicated equipment array/counter wrong). | unsigned compare + comment fixed (game-character PlayerManager-025dba-1) |
| 0x42b150 | fs_read_func | high | no cursor advance, returns 0 | REAL (listing: `STRLE R3,[R5,#8]`, `MOV R0,R4`). | advances pos when in [0,size], returns copied count; unit test updated (core-platform) |
| 0x312848 | Point2D<float>::lineIntersection | med | NaN t/u classified inside | REAL (`__aeabi_fcmpge/le` failure = outside; parallel test is `fcmplt && fcmpgt`, so NaN is non-parallel). Explains armdiff r0 3 vs 5 mismatch. | negated forms (NaN -> outside / non-parallel) |
| 0x312848 | same | low | double literal 1.0 / 0.0001 vs float | REAL (float 1.0f, 0x38D1B717 constant). | `1.0f/denom`, float eps constant |
| 0x32ade8 | Application::_Draw | med | gui->drawAll before beginScene2D | REAL (driver +0x14 then gui +0x0c). | order swapped |
| 0x32ade8 | Application::_Draw | low | bounding-box debug path lacks material setup | REAL (createMaterialRenderer(1)/getMaterialInstance/setMaterial(tech or 255) before traverse). | sequence added, same as ObjectManager::Draw3D |
| 0x3fd5d0 | SortByEquipability::operator() | med | tie-break must use LeftHand/LeftHandRingFinger | FALSE. Snippet is not what the pseudocode shows; listing 0x3FD954 does `LDM R4,{R0,R1}` = m_equipSlot (+4). Recon correct. | none |
| 0x410ea4 | CameraOverview::CameraOverview | med | node never addChild'ed to scene root | REAL (`rootNode` vtable +0x5c with m_node). | addChild added |
| 0x410ea4 | same | low | setUpVector(0,1,0) missing | REAL (slot +0x114 before setTarget +0x104). | added |
| 0x419b4c | HUDControls::initCachedChars | med | "bg" searched from wrong context | REAL (Find("bg", joystick GetChar()), joystick = +0x4c). | now m_joystick.GetChar() |
| 0x7fdf24 | CEvent::CopyData | med | returns -1 always | REAL (returns size; guards: dst null or cap <= 0 or cap < size -> -1). Also recon's copy-guard form was wrong. | rewritten |
| 0x4556c4 | Script_ShowFlash::IsBlocking | low | unsigned vs signed compare | REAL (CMP/MOVGT). | signed compare |
| 0x348ea4 | ObjectManager::Remove | low | RemoveNoRoomObject/+0x90 unlink hoisted out of null guard | FALSE for the current file: both calls are already inside `if (object != nullptr)`. The `unk_2fc` null guard is a harmless host guard (original derefs; would fault). | none |
| 0x4761cc | AnimSetManager::GetSynchronizedAnimator | low | original null-guards engine set | FALSE: original does `v6[112]` with no null test. Recon matches. | none |
| 0x3b10b4 | Character::F_ApplyResult | low | extra `__android_log_print` DH2R4 | REAL but owned by R4 (their emulator debug line, file modified minutes ago). Not touched. Remove before final. | skipped (concurrent owner) |

Totals (15 distinct claims in table; two _Draw rows share one kind in the CSV merge): 11 real and fixed, 1 real but skipped (R4 debug line), 3 false. Fixes in 8 source files + 1 test.

## DeepSeek workspace

- `C:\Users\nacworkstation\Documents\deepseek-harness\default-workspace`: contains NO session transcripts, jsonl or logs (only `dh2apk/fetch-tools.log`
  unrelated). Nothing could be recovered beyond what its working files show: `dh2-deepseek-audit/` (AUDIT-BRIEF.md, showfunc.py, batch dumps
  `app-cf*.txt`, `cf-b*.txt`, `_scratch_netcore/`, indices) and the repo copies of its progress notes in `_parts/progress-*.md`.
  Unfinished findings: none beyond what the progress notes list (prepared but unread dumps `app-cf*.txt`, `cf-b*.txt`).
- Coverage reached (from `_parts/progress-*.md`): prio 0 and prio 2 rows essentially all audited per slice; prio 1 only the `const:`/`float:` lead rows,
  partially (anim-ui 32 of 303 read individually, app 46 of 240, char-ai 88-row target, items 48/102, world-misc ~55 hand-read of 204, world-obj 31 of 212
  hand-read, net-core prio 0-2 only). **Not reached:** all prio 3 and prio 4 rows in every slice (~3,500 rows incl. anim-ui 789, app 884, net-core 992+,
  world-obj 491, world-misc 307, char-ai 262, items rows beyond 96); anim-ui largest unverified: 0x418d28 HUDControls::OnEvent, RenderFX/MenuFX/
  StringManager/HUDMinimap families; world-obj 0x347fd0 ForceFullUpdate, 0x3f82d8 Level::Update; app: 194 prio-1 const/float rows; core-platform/game-items
  mostly empty (items: 0 findings from 96 functions). Its quality: 2 of 14 findings false and the quoted "original snippet" of 0x3fd5d0 was not in the
  pseudocode, so unverified leads from its prio-1 screening should not be trusted as clean either.
- Edits outside the audit folder: **none found**. `git status` shows only: ItemInventory TransferInventoryTo and CAnimationSet.addAnimationLibraryBindings
  (both annotated "W2", AddressSanitizer fixes), IColladaSkinTechnique/CBatchSceneNode (V1 rendering), host-runner/*, FIRST-BOOT/HOST-RUNNER.md,
  armdiff chain.py/combat.py (C); untracked DEEPSEEK-QUALITY-AUDIT.md (written Oct 15, earlier task), REFERENCE-DEBUG-INFO-SCOPE.md, run_session.sh, scripts.
  The `_ledger_patch*.json` / `_gv_patch*.py` / `_scratch_patch.py` files are dated Oct 15 01:34-03:06 (a previous ledger task, before this audit, which
  started Oct 16 15:40); no `ledger.csv` is modified in the working tree, so those patches are not pending changes (any applied then are committed history).
