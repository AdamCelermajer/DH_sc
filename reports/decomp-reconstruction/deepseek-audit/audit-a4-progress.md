# audit-a4 progress (worklist-a4.csv, 600 rows)

Status: COMPLETE. Worklist indexes 0-599 audited (600 of 600). Findings in audit-a4.csv: 11 (high 0, medium 3, low 8).

Checkpoints: 284 (earlier pass), 400, 450, 500, 550 (rows 0-599 finished; no unmatched rows).

## Method
- Original: `recovered/native/hexrays-all/chunk_*.c`, located by `// ===== <addr> <name> =====`; body read to the next header. Where the pseudocode
  is truncated or ambiguous, the ARM listing in `recovered/native/ida-asm/asm_chunk_*.asm` was used (it can differ from the pseudocode). Examples:
  row 393 (GetTargetListTop constant is single precision, 0x42652EE0), row 391 (pending interactor is passed in r1), row 492 (m_node passed as the
  search root in r3), row 556 (refusal result 50 passed in r1).
- Reconstruction: `port/decomp-reconstruction/<category>/src`, located by the `// 0x<addr>` comment (category first, then whole tree).
- Matching: all 600 addresses matched both sides (0 unmatched).
- Clean rows are not written to the CSV. Documented host-only deviations (null guards, abort in place of a crash, PARTIAL notes, buffer size
  changes) were not reported.
- Not compared, by design: debug-trace functions that only print (`DBG_*`, e.g. rows 271, 272, 280, 438, 439, 460); template bodies that live in
  headers (rows 273, 275 used the header body for 273 only).

## Findings (audit-a4.csv)
- medium 3:
  - 0x43f630 NativeCreateSaveSlot: original clamps PlayerSavegame +0x3C (m_unlockedDifficulty) when negative and sets unk_194 to 1;
    the reconstruction clamps m_saveDate (+0x38) and sets m_unlockedDifficulty = 1.
  - 0x7fdfe0 CEvent::Init: original stores COnline +0x28 (the counter) in this+0x10; the reconstruction stores OnlineNowMs() (the clock).
  - 0x4999b8 GameCenter::ResetPlayerList: original always calls SetPlayerIDStatus(userUid, "ResetPlayerList", skip) and walks a second list at
    this+0x74; the reconstruction does neither.
- low 8:
  - 0x84473c XmlNode::findFirstChild: list element used as the string operand in the listing; compared field not certain.
  - 0x7ad81c fscommand_callback_handler: holder reference release at character+0x2C not reproduced.
  - 0x4604c0 ScriptManager::SkipScript: DebugSwitches::load() before the probe not reproduced (side effect only).
  - 0x3bdca4 Character::RegenHP: original returns an int; the reconstruction is void.
  - 0x371294 PlayerInfo::~PlayerInfo (and 0x37168c): vtable +0x14 loop on 40-byte elements not reproduced.
  - 0x352d4c SceneManager 4-arg ctor writes +0x490 (m_secondaryRefCount); the original 4-arg ctor does not (only the VTT ctor 0x352c3c does).
  - 0x4718f0 VisualObject::_FindModularSkinnedMeshNode: null m_node returns in the reconstruction; the original asserts and still searches.
  - 0x800808 CMatching::GetMemberIdList: members_ declared at this+0x24 while the header comment gives element base 0x20 (id at 0x170);
    in-object access is self-consistent, so this matters only for raw-offset access and the claimed layout.

## Partial coverage (listing truncated in the dump, or the reconstruction is partial; not fully compared)
- 0x32c438 Application::_Update (row 61): first ~110 of 245 lines.
- 0x311914 PerfCounters::Draw (row 24): structure only.
- 0x32c1f4 GoToMainMenu (row 62): last ~6 lines.
- Character::Kill (row 211): first ~60%.
- Rows 49, 101, 112, 121, 130, 155, 157, 180, 183, 186, 187, 215, 216, 233, 249, 257, 275, 370 (resolved: no finding), 407 (Level::_LoadProcess, 870 lines:
  switch arms 0-5 compared), 412 (last 11 lines), 415 (middle truncated), 477 (TriggerPlate::Update, second half), 483 (TriggerZone::Update tail),
  488 (tail), 493 (CalcMeshBox, 229 lines, mostly unread), 494 (tail), 496 (dtor tail), 506 (SaveRMS, 191 lines, first ~70), 534 (tail),
  537 (Initialize tail), 541 (processRankingAround tail), 546 (tail), 547 (LoadConfig, 163 lines, mostly), 549 (tail), 550, 561 (dtor, 162 lines, first
  part), 567 (CalculateTotalLength, 395 lines, partial), 568 (Run, 208 lines, partial), 570 (createRoom tail), 571 (tail), 580 (tail), 585 (508 lines,
  partial), 586 (tail), 589 (tail).

## Unmatched rows
- none (0 of 600)
