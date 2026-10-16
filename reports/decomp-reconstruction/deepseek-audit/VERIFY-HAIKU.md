# Verification of the four Haiku audits (owner VF)

Method: every finding of audit-a1..a4.csv (54 rows) checked against the ARM listing (`tools/decomp_asm.py`), with the Hex-Rays
pseudocode as a second source. Where they disagree the listing wins. Verdicts: REAL (fixed), FALSE (reconstruction already matches
the binary, or the claim came from a Hex-Rays artefact), UNDEF (the original reads uninitialised or out-of-range memory, or asserts;
the reconstruction keeps a documented, safe deviation).

## Result: 11 REAL (all fixed), 6 UNDEF, 37 FALSE. Unmatched rows: 1 genuinely missing (SearchByName, 3 overloads), the rest present.

| address | function | claim | verdict | fix / reason |
|---|---|---|---|---|
| 0x89cd00 | LC_API_STRTRIM | no-op | REAL | strips leading spaces in place, returns the string (listing: flag loop); header now `char*`; test added in lc_api_test |
| 0x335b14 | Console::onEvent clamp | signed vs unsigned | FALSE | listing uses CMP/STRLS/STRHI = unsigned, as the port |
| 0x33179c | Console::DBG_TryStartingLevel | this passed as name | FALSE | function is static; R0 is the name (Hex-Rays prototype wrong) |
| 0x337a88 | DebugSwitches::GetSwitch family | missing load() | FALSE | ReadTraceSwitch goes through GetInstance(), which calls load() |
| 0x4210a8 | MenuBase::FS_PlayMusic | PlayMusic missing | FALSE | call is present and matches (PlayMusic(idx,1,0,2000)); stale PARTIAL comment removed (also FS_PlaySoundFX) |
| 0x424540 | MenuBase::FS_SetText2 | FormatHTML missing | FALSE | implemented and matches; stale comment fixed (also FS_SetText) |
| 0x4248dc | MenuBase::FS_IsMapLocLocked | retval not published | FALSE | _root.retval is published via SetMember; comment fixed |
| 0x81f04c | CMatchingGLLive::SetPlayerParam | observer forward no-op | FALSE | observer SetPlayerParam (0x822638) is reconstructed and called; null-lobby guard removed, comment fixed |
| 0x530ba8 | appInit return / window size | returns 0; size deviation | FALSE | return is a _DEBUG_OUT leftover nobody reads; size deviation is the documented Android one |
| 0x89bfb4 | ConfigFile::Decode | stride signed; length clamp | FALSE | server stride is LDRB + AND #0xFF (unsigned); clamp is the documented 512-byte guard |
| 0x34f8a4 | FileSystemWin32::formatFilePath | run handling | FALSE | listing is find('/') + replace(pos,1,"\\") one for one, as the port |
| 0x827970 | TransToIp | return value dropped | FALSE | r0 leftover of strcpy; the only caller ignores it |
| 0x330bf0 | Console::_setMenuAnimList | 12-byte stride | FALSE | pydata arrays use the host struct everywhere (same cast in AnimationSet) |
| 0x844464 | slim::XmlDocument::findLabel | comment threshold 3 vs 7 | REAL | comment path now needs rest > 6 (CMP R2,#6; BHI); shorter tails read as plain labels |
| 0x33cf84 | TouchScreenIPhone::touchCancelled | trace read dropped | REAL | all four handlers (began/moved/ended/cancelled, same listing) now do load() + GetSwitch("isTracingKeyInputs") after getID |
| 0x52f11c | appOnTouch | returns 0 | FALSE | return is the void callee's leftover r0; documented |
| 0x322924 | Application::Pause | static vs device timer | FALSE | device virtual forwards to os::Timer::getRealTime; documented substitution |
| 0x50e830 | GetScreenPosFromWorldPos | return | FALSE | sret register leftover, unused |
| 0x45eec0 | Script_StopActor::Execute (All) | +44 word | FALSE | [node+0x2C] is the entry's m_object, the same test as the port |
| 0x3cbb34 | CharAI::RaiseAIEvent | returns | FALSE | void virtuals leave r0 undefined; documented PARTIAL |
| 0x3a4398 | Character::SetRelativeAABB | double 0.01 | FALSE | listing multiplies floats by 0x3C23D70A (0.01f) |
| 0x376100 | PlayerManager::_CheckLocalDeaths | returns Level* | FALSE | the sole caller ignores the result |
| 0x3e58cc | Projectile::SetInfo (angle) | double math | FALSE | the angle overload is at 0x3e57cc; its listing uses __aeabi_fmul/fadd with 1000.0f |
| 0x39cdc4 | ProjectileTrap::SpecificUpdate | int 1 vs 1.0f | FALSE | the callee is the bool Spawn overload (...S4_b); argument 1 is the bool |
| 0x82b634 | XP_API_PARSE_DATA_NEW | missing field | UNDEF | original reads one byte before the text; documented deviation |
| 0x34e96c | FileSystemBase::ApplyFilenameHacks | basename of raw name | FALSE | listing passes R4 (the trimmed result) to getFileBasename, as the port |
| 0x33d054 | TouchScreenIPhone::touchBegan | trace read | REAL | see touchCancelled |
| 0x320da4 | Application::ComputeDt | signed convert | FALSE | listing uses __aeabi_ui2f (unsigned) |
| 0x328fe0 | Application::Resume | timer read twice | REAL | m_lastTickTime now takes a second getRealTime() after WakeUp/ResetTouch |
| 0x43e830 | NativeMoveCharacter | scale arg | FALSE | listing: angle = arg0 (90-a), scale = arg1 multiplies the rotated vector, player = arg2; the port matches |
| 0x43eac0 | NativeLockCharacter | result false not set | REAL | result set to false after the lock (character found); a NaN argument is rejected; helper now returns bool |
| 0x43ea0c | NativeUnlockCharacter | same | REAL | same fix |
| 0x45cba0 | Script_ChangeLevel::Execute | unknown level | UNDEF | original reads the row at index -1 (outside the table); port skips |
| 0x3d7da8 | CharAI::AI_SpellInfo | signed faerie id | UNDEF | -1 reads begin-4 in the original; port asserts and returns null |
| 0x3c184c, 0x3c1694 | CharStateMachine::_GetState, _GetEvent | header+20 after assert | UNDEF | non-null pointer into the map header in the original; port returns null on the assert path |
| 0x374cc0 | PlayerInfo::PlayerInfo | SetChanged x15 | UNDEF | each branch compares the member's own uninitialised word (+0x2a8, +0x308 ...) with zero, so it depends on allocator contents; only the change counter is affected; documented |
| 0x4247c0 | MenuBase::FS_GetCharProp | snprintf keeps 7 chars | FALSE | the original buffer is char s[8] with the stack-protector canary right above it; snprintf only avoids the overrun |
| 0x507c0c | StringManager::formatForMulti | newline flag | FALSE | string is r2, the bool is r3 and selects the "\n"; Hex-Rays mislabelled the arguments |
| 0x50bf28 | AssetManager::dropTexture | refcount | FALSE | the port's intrusive_ptr(T*) takes the reference without add_ref, so it is the original's single manual ++ |
| 0x3ef614 | Level::GetSpawnPoint | stale result | REAL | result-register semantics reproduced: last visited type-13 object (set at 0x3ef6bc), cleared by an unresolved or non-13 object (0x3ef680), unchanged by a null m_object |
| 0x84473c | XmlNode::findFirstChild | compares element | FALSE | XmlNode begins with its name string; same compare |
| 0x7ad81c | fscommand_callback_handler | holder release | FALSE | inlined weak_ptr::get_ptr (clears a dead weak reference); get_ptr() does that |
| 0x4604c0 | ScriptManager::SkipScript | load missing | FALSE | GetInstance() = load() + GetSwitch, as listed |
| 0x43f630 | NativeCreateSaveSlot | wrong field clamped | REAL | +0x3c (m_unlockedDifficulty) is clamped to 0 only when negative; +0x38 is not touched; the flag byte at +0x194 (unk_194) is set; the old m_unlockedDifficulty = 1 is removed |
| 0x3bdca4 | Character::RegenHP | returns int | FALSE | the Lua binding drops it (the listing tail-branches) |
| 0x371294 | PlayerInfo::~PlayerInfo | member loop | FALSE | the loop is the inlined destruction of the seven members at +0x548..+0x638; the C++ members do the same |
| 0x352d4c | SceneManager::SceneManager | +0x490 store | FALSE | the store is in the C1 (0x352c3c), which is what new uses |
| 0x4718f0 | VisualObject::_FindModularSkinnedMeshNode | null m_node | UNDEF | assert, then a null-root scene search; the port returns early (documented) |
| 0x7fdfe0 | CEvent::Init | clock vs counter | REAL | timestamp_ is COnline +0x28 (the receive counter); the same listing fact fixes CEvent::IsExpired (0x7fdf04 reads +0x28); c_event_test now drives the counter |
| 0x800808 | CMatching::GetMemberIdList | layout | FALSE | no behavioural difference, only a header offset comment |
| 0x4999b8 | GameCenter::ResetPlayerList | status not set | REAL | clears the status list (+0x74) and calls SetPlayerIDStatus(user, "ResetPlayerList", !gate); game_center_test extended |

Totals by auditor: A1 8 findings (1 real), A2 19 (2 real), A3 16 (5 real, 5 undefined), A4 11 (3 real, 1 undefined).

## Unmatched rows
- SearchByName `char*` overloads 0x359fac, 0x35a044, 0x35a0e4: declared in SceneManager.hpp, only a header comment in the .cpp (the ledger said
  reconstructed). Now defined in g1-game-world-SceneManager-9fc3ac-0.cpp from the listing (null checks, then the std::string overload). A real gap.
- 0x7ffc28 CMatching::MemberInfoNetStruct dtor: the implicit destructor does exactly the listing (ByteArray buffer free, history map clear); comment added.
- Array2d<Tile*> 0x487dd8, Rule::Impl 0x48bd60, CharStateMachine C1 0x3c1ac4, ItemInstance C2 0x3fc494, PFFloor C2 0x51d40c, PFRoom C1 0x521540,
  item C1 0x843cd8, GLXPlayerUserFriend C1 0x82cb60, addScoreEntry 0x832c2c, GLXPlayerPing D2 0x8401cc and the six ObjectiveTemplate instantiations
  (0x47df64, 0x47f194, 0x47f5a0, 0x47eb90, 0x47eec0, 0x47ed70): all have bodies (explicit instantiations, or shared C1/C2 and D1/D2 bodies) and an
  address comment; the auditors' search missed them. Nothing to add.
- Rows the progress notes list as only partly compared (long bodies in A1 rows 110-600, A2 large functions, A3 rows 363-600) were not re-audited here.

## Estimated real-bug rate of the auditors
11 of 54 findings were real (20%); among the 12 mediums 4 (33%), plus 1 medium that is undefined behaviour in the original; among the 42 lows 7 (17%).
The false calls are dominated by Hex-Rays artefacts (signed compares, dropped or misnamed arguments, double literals for float constants, void functions shown
as returning) and by stale PARTIAL comments in already finished code (4 of the 6 mediums reported as missing side effects were already implemented).
Roughly one finding in five needs a fix, and every real one was settled only by reading the ARM listing, so a finding should not be acted on from the
pseudocode alone.

## Verification
check_tu OK on every touched source. build/own-vf (Release, configured like tb-int): all libraries rebuilt; ctest core-platform 38/38, net-session-online 98/98,
game-app + game-character + game-anim 153/153, game-items-save + game-ui + game-world 139/139 pass. No test covers GetSpawnPoint, NativeCreateSaveSlot or the
Lock/Unlock result (they need a live Level or gameswf player).
