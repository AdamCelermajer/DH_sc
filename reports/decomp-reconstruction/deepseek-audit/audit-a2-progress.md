# Audit A2 progress (read-only bug hunt, worklist-a2.csv)

Status: complete. All 600 rows (worklist index 0-599) were visited. Depth varied: small and medium
functions were compared line by line; large functions (200+ lines) were compared on their key branches,
constants and call targets only. Results are in audit-a2.csv.

Findings: 19 rows, all severity low, none high or medium.
Kinds: other 11, branch 2, math 2, field 1, side-effect 1, call 1, constant 1.
- 0x530ba8 appInit: return value (orig returns _DEBUG_OUT result); documented Android window-size deviation.
- 0x89bfb4 ConfigFile::Decode: server stride read signed in original, unsigned in recon; server length clamp (documented).
- 0x34f8a4 FileSystemWin32::formatFilePath: original decompile is garbled; recon maps each slash 1:1; run handling unsure.
- 0x827970 TransToIp: return value (char*) dropped.
- 0x330bf0 _setMenuAnimList: 12-byte record stride vs host ColladaFile sizeof (depends on members layout).
- 0x844464 XmlDocument::findLabel: comment detection threshold 7 vs 3 bytes (malformed input only).
- 0x33cf84 TouchScreenIPhone::touchCancelled: DebugSwitches load/GetSwitch side effect dropped.
- 0x52f11c appOnTouch: return value replaced by 0 (original returns callee result or _DEBUG_OUT result).
- 0x322924 Application::Pause: getRealTime via device timer object vs static call; unsure they are the same clock.
- 0x50e830 GetScreenPosFromWorldPos: return value is the sret register in original, X in recon.
- 0x45eec0 Script_StopActor::Execute: 'All' path tests a +44 word the recon does not check; unsure what it is.
- 0x3cbb34 CharAI::RaiseAIEvent: several cases return the virtual's result in the original; recon returns 0.
- 0x3a4398 Character::SetRelativeAABB: float 0.01f vs double 0.01 literal (1 ulp class).
- 0x376100 PlayerManager::_CheckLocalDeaths: original returns the Level pointer when not at load step 38; sole caller ignores it.
- 0x3e58cc Projectile::SetInfo (angle overload): double intermediate math in original vs float in recon (1 ulp class).
- 0x39cdc4 ProjectileTrap::SpecificUpdate: last Spawn argument is int 1 in the original, recon passes 1.0f (1 degree); unsure.
- 0x82b634 XP_API_PARSE_DATA_NEW: documented deviation for a missing field (original reads one byte before the text).

Checked and matching in detail (examples): ConfigFile key/PID tables, Console debug-key handler (W/S/A/D/X paths),
Console::_createList geometry, Detach, SetModule, Pause, InitWin32, Array2d, ProcessThree vertex layout,
LoadScriptProcess, SetScriptByName class dispatch, PlayAnimByName/PlayEffect/PlaySound offsets,
EquipItemAuto gates (+0x1320/+0x1324 = sheet +0x32c/+0x330), TransmuteItem, EffectiveProb (LootEntry offsets),
CheckpointSave, TrophyCheckArmorSet switch, PlayerStatManager::Invoke, _LoadVolatileQuestsLog (high-word mask
is correct: callee receives loadMode in r1), IsQuestInStateGeneric comparators, MaskList order (1,2,4,8,16,32),
downloadFile header strings, SetColorTransform, GameObject::SetRelativeAABB, LoadCamera SetData constants.

Coverage limits:
- Large functions not fully read: Console::onEventNotOpened (other keys), LoadSubtitles (tail), RecalcProperty (442
  lines; its header says partial but _GetType is implemented), SaveAsModuleXML, CharAI::RaiseAIEvent (cases after 63),
  NativeStartGame, OnlineGameState::Init (970 lines), processMessages, ParseChunkedContent, LCXPlayer routines.
- Declaration-only or instantiation-only recon matches (body not compared): ObjectiveTemplate_InteractWith DBG/handleEvent/
  Compile (indices 267-270), KillCharacter DBG (271), Objective_EventReceiver HasEnemyOfTypeLoaded (275), template
  Invoke instantiations (329-332), SearchByName (466, recon is a comment only), MenuMessageManager Invoke (329-332).
- Row 39 (ConvertUnicodeToUTF8_Old) delegates to ConvertUnicodeToUTF8Impl, which was not compared.
- Row 48 (AnimatorSet ctor C2 VTT form 0x367790) is marked PARTIAL in the recon; the C1 form at 0x3676b8 was compared.

Unmatched: no address failed to locate in the tree after a grep pass. Strictly, index 466 (SearchByName, 0x35a0e4) has
only a comment in the recon, so its body is missing from the port; treat it as the one unmatched row.
