# Audit A3 progress

Worklist: `worklist-a3.csv` (600 rows, worklist order). Findings: `audit-a3.csv`.

## Final checkpoint at row 600

- Rows done: 600 of 600 (all worklist rows audited, one pass; rows 358-362 finished at the start of this pass).
- Findings: 16 in audit-a3.csv (13 low, 3 medium). No high findings.
  - Medium: 0x374cc0 (PlayerInfo ctor, stale-temp SetChanged not reproduced), 0x4247c0 (MenuBase), 0x3ef614 (Level::GetSpawnPoint, stale non-matching spawn returned in the original; null in the port).
  - Low: 0x34e96c, 0x33d054, 0x320da4, 0x328fe0, 0x43e830, 0x43eac0, 0x43ea0c, 0x45cba0, 0x3d7da8, 0x3c184c, 0x3c1694, 0x507c0c, 0x50bf28.
- Unmatched: 1. 0x7ffc28 (CMatching::MemberInfoNetStruct destructor) has no reconstruction comment or explicit definition; the port relies on the implicit destructor.
- Depth: rows 1-362 were compared line by line in the earlier pass. Rows 363-600 were compared against the original pseudocode. Where the pseudocode was truncated, the checks covered the visible logic and the call/ref/assert structure; long bodies (for example mpProcessIncomingMessages, the NetStruct array loads, ProcessAcknowledgedPacket paths) were only spot-checked.
- Still open for asm verification: 0x507c0c formatForMulti newline flag, 0x43e830 NativeMoveCharacter scale argument.
- Checked against asm and not reported: 0x39d430 PlayClip loop argument (r2 = 1 is set and not shown by the decompile), 0x471a0c scene-query root (r3 = m_node from function entry), 0x81ef7c: the original builds and destroys a temporary CMatchingGLLiveLobbyObserver that the port omits; not verified whether its ctor/dtor has side effects.

## Checkpoint at row 500

- Rows done: 500 of 600 (rows 1-500 in worklist order). Rows 501-600 remain.
- Findings unchanged since row 400: 16 in total (13 low, 3 medium). Rows 401-500 added no findings after the GetSpawnPoint entry at row 397.
- Rows 401-500 were all compared against the original (worklist rows 401-500, addresses in worklist-a3.csv). Their findings, if any, are the entries in audit-a3.csv with the row-400 list above.
- Unmatched addresses so far: none.

## Checkpoint at row 400

- Rows done: 400 of 600 (rows 1-400 in worklist order). Rows 401-600 remain.
- Findings so far: 16 (13 low, 3 medium). Full list in `audit-a3.csv`.
  - Medium:
    - 0x374cc0 PlayerInfo ctor: stale-temp SetChanged calls not reproduced (the port documents this).
    - 0x4247c0 MenuBase (g1-game-ui-MenuBase-8948cc-4.cpp): see CSV row.
    - 0x3ef614 Level::GetSpawnPoint: when no spawn matches the entry point, the original returns the stale last-visited goId-13 object (not null). Callers fall back to the local player on null, so the position differs.
  - Low: 0x34e96c, 0x33d054, 0x320da4, 0x328fe0, 0x43e830, 0x43eac0, 0x43ea0c, 0x45cba0, 0x3d7da8, 0x3c184c, 0x3c1694, 0x507c0c (formatForMulti newline flag, asm check pending), 0x50bf28 (dropTexture: extra reference not released on the removal path, depends on removeTexture ownership).
- Rows 363-400 checked in this pass: 0x7d7094, 0x8180f4, 0x8192d0, 0x40e9a8, 0x40fa68, 0x40fd18, 0x40ff70, 0x3a0110, 0x3a0a98, 0x3a0da0, 0x3e74c8, 0x3e8088, 0x386190, 0x386818, 0x3860bc, 0x38b228, 0x38c790, 0x38b91c, 0x38b48c, 0x38c710, 0x38b6d4, 0x38fcf0, 0x390974, 0x39040c, 0x3906fc, 0x39088c, 0x39193c, 0x39212c, 0x391438, 0x392714, 0x392620, 0x3938f8, 0x394bf8, 0x3e4570, 0x3ef614, 0x3f059c, 0x3f0690, 0x3ef7ac. Only 0x3ef614 gave a new finding.
- Unmatched addresses so far: none.
- Still open for asm verification: 0x507c0c formatForMulti newline flag source, 0x43e830 NativeMoveCharacter scale argument.

## Checkpoint at row 302 (superseded)

- Rows 201-302 were checked. Findings in that range: 0x374cc0 (medium), 0x4247c0 (medium), 0x507c0c (low), and the earlier list below.

## Checkpoint at row 200

- Rows done: 200 of 600 (rows 1-200 in worklist order, row 201 and 202 also checked).
- Findings so far: 11, all low severity (no high or medium findings yet).
  - 0x34e96c FileSystemBase::ApplyFilenameHacks: `.tga` branch takes the basename of the raw name, reconstruction takes it of the trimmed result. Low.
  - 0x33d054 TouchScreenIPhone::touchBegan: original's discarded DebugSwitches load and GetSwitch on a literal key is omitted; GetSwitch inserts missing keys. Low, debug-switch state only.
  - 0x320da4 Application::ComputeDt: elapsed time signed in original, unsigned in reconstruction. Differs only if the timer goes backwards. Low.
  - 0x328fe0 Application::Resume: original reads the device clock twice (m_lastTickTime is the second read). Low, timestamp only.
  - 0x43e830 NativeMoveCharacter: decompile discards the scale argument; reconstruction multiplies the target by it. Not confirmed from asm. Low.
  - 0x43eac0 / 0x43ea0c NativeLockCharacter / NativeUnlockCharacter: original sets the script return value to false; reconstruction does not write call.result. Low.
  - 0x45cba0 Script_ChangeLevel::Execute: for an unknown level name the original still calls LoadLevel with a row at index -1 (undefined read); reconstruction skips. Low.
  - 0x3d7da8 CharAI::AI_SpellInfo: signed vs unsigned bounds on the faerie id; -1 reads begin-4 in the original, reconstruction asserts and returns null. Low.
  - 0x3c184c CharStateMachine::_GetState and 0x3c1694 _GetEvent: after the assert, absent key returns header+20 in the original, nullptr in the reconstruction. Low.
- Decompiler artefacts checked against the asm and not reported: literal strings shown as "" (pc-relative literal, seen at 0x33d054), 0.0039062 in _FromFixed (asm is 2^-8), GetRandom calls go to a clone that takes only the range.
- One finding was withdrawn after checking: the OneStep "[%d]" sprintf writes a stack buffer (char s[32]), not a global.
