# Game owner progress log (Track B)

Owner of the six game categories (game-anim, game-app, game-character, game-items-save, game-ui, game-world): 1,468 partial rows at the
start. Phase 1 fixes the cross-cutting blockers, phase 2 redoes the partial rows class by class. Ledger parts: `<cat>/ledger.d/owner-<cat>.csv`.
Resume rule: read this file, then continue with the first phase-2 class not listed below.
Scratch helpers (not in the tree): the owner kept throwaway scripts in the session scratchpad; nothing there is needed to resume.

## Phase 1: cross-cutting fixes

| # | fix | state |
|---|---|---|
| 1 | windows.h leak from gameswf `base/tu_loadlib.h` | done. HOST-PORT in `vendor-gameswf/upstream/gameswf-r1714/base/tu_loadlib.h` (forward-declares `struct HINSTANCE__` instead of including `<windows.h>`; `tu_loadlib.cpp` still includes it). No game TU defines `GetClassName`, `GetObject`, `GetMessage`, `ERROR`, `IN`, `near`, `far` any more (all 473 game .cpp preprocessed). The per-file `#undef GetClassName` in `game-app/src/g1-game-app-global-f216b6-13.cpp` is removed. Noted in the gameswf README. |
| 2 | pydata `Arrays::X` defined once | done. New `pydata-accessors/arrays.hpp` holds the 71 `Arrays::X` classes (union of members, size, finalize, read, m_memberNames, finalizeNames, readNames, skipNames); `member_names.hpp`, `w7_arrays_00.hpp`, `w7_arrays_02.hpp`, `w7_arrays27.hpp` include it. All four can share a TU. Helper units `game-app/src/g1-game-app-global-f216b6-12_names.cpp` and `game-character/src/g1-game-character-Character-ee9946-0_modeldict.cpp` removed; their callers use `Arrays::LevelList::m_memberNames` and `Arrays::ModelDict::size` directly (NativeGoToZone loop checked against 0x4422b8). |
| 3 | dh2/Level.hpp vs net-transport IStreamBase | already resolved by the unify wave (`dh2/IStreamBase.hpp` is the only definition; `net-transport/deps/i_stream_base.hpp` and `pydata-accessors/deps/stream_reader.hpp` only include it). Level.hpp + Character.hpp + Container.hpp + PlayerManager.hpp + all pydata headers + net-transport stream_buffer.hpp compile in one TU. The source comments saying otherwise are stale. |
| 4 | missing headers | done: `dh2/StatusMsg.hpp`, `TutorialMsg.hpp`, `OnlineStatusMsg.hpp`, `ObjectListItem.hpp`, `VarArgs.hpp`, `tMsgRaisedEventData.hpp`, `EvKeyboard.hpp`, `EvMouseMove.hpp`, `EvMouseButton.hpp`, `EvMouseWheel.hpp`, `EvGamepad.hpp`, `EvGamepadAxis.hpp`. Layouts from the push_back/copy code, the raisers (`Application::onEvent` 0x32a9c8, `_CheckGamepad` 0x321164, `v2GamepadController::Update`) and the consumers (`MenuManager::onEvent`, `v2EmuController::_onEvent`). `ObjectManager.hpp` now includes `ObjectListItem.hpp`. |
| 5 | static definitions | done for the message queues: `s_NodeName`, `s_NodeCachedChar`, `s_StartFuncName`, `s_SkipFuncName` declared in all six message headers and defined in `game-app/src/statics_<Msg>.cpp` with the .data initial values read from the binary; `singleton_MenuMessageManager_{Status,OnlineStatus,Tutorial}Msg.cpp` added. |
| 6 | online gate | done. `NetGateOpen()` (game-app/src/net_gate.cpp, declared in dh2/global/net_singletons.hpp) is the four-term test read from the asm of `PlayerManager::GetNumPlayers` 0x36d7a8: `GetOnline()+5` && `OnlineSingleton<OnlineGameState>::GetInstance()->flag24_` (+0x24) && `CMatching::Get()` vtable +0x64 (`UnknownGate`, IsInRoom) && `CNetPlayerManager::IsInitialized()`. `GetOnlineGameFlag()` (game-app/src/online_flag.cpp) reads COnline +0x05. The stub `OnlineGameFlag` functions in Character-3, CSIdle, pack2, LevelSavegame-0/-1, PlayerSavegame-0/-1, GameObject-2, ObjectManager-2, ScriptManager-1 and PlayerManager-1 now read the real flags. Test fixtures that stubbed `GetOnline()` to null return a real COnlineImpl. |
| 7 | layout requests | applied as classes are redone. Done so far: `glitch::IDevice` (+0x08 resFileManager, +0x0c animationStreamingManager, +0x10 videoDriver, +0x1c sceneManager, +0x20 timer, +0x34 fileSystem, glitch-core-io/os/device.hpp); `dh2/IFileStream.hpp` (IStreamBase + skip) and `FileSystemWin32::_FileHandle : IFileStream` with `doesSavefileExists`; `dh2/global/savefile_fs.hpp` (SavefileFileSystem, SavefileFileSystemPtr, SavefileExists); `Buffer::sized/block_vector_size/block`; `CMsgQuestsSync::SetBlock`; `QE_GatherLoot` constructors; `DebugCachedCharacter::RefreshCacheIfStale`. Character +0x39c gold was already satisfied (ItemInventory::m_gold, a Character base). Others open: CharProperties::BuffExpired/PROPS_DelBuff return int, ZoomHandler::ResetFingerMap, ScriptManager +0x30 byte, ISceneNode getPosition slot comment (scene owner). |

## Build and tests

`build/tb-all` is shared with the other agents (concurrent ninja runs there fail with "failed recompaction"). The owner builds in `build/tb-go`
(same CMake settings; `cmake -S port/decomp-reconstruction -B build/tb-go -G Ninja -DCMAKE_BUILD_TYPE=Release`). Unit tests that call into the
Application (m_device, m_menuManager, GetOnline) need scaffolding: a zeroed MenuManager with a zeroed MultiMenuManager (null HUD root), a real
COnlineImpl from `GetOnline()`, and `RES_PATH` pointing at a directory that does not exist when a test must not write savefiles.

## Phase 2: classes done (one line per class)
game-items-save | QE_* FillNetworkMsg (7) | reconstructed 7 | new dh2/tMsgRaisedEventData.hpp
game-items-save | LevelSavegame | reconstructed 10 | savefile FS + online flag now real; __SaveObjects written (ObjectListItem)
game-items-save | PlayerSavegame | reconstructed 16 | SG_TryQuestSync sends the packed quest block (CMsgQuestsSync::SetBlock)
game-items-save | Savegame | reconstructed 5 | OpenSavefile/getFilesMatching/deleteSavefile/backup through the device file system; Buffer block accessors
game-items-save | ItemInventory | reconstructed 5 (TransferInventoryTo, SetPotionQty, _UnEquipItemAuto, _EquipItemToSlot, AddLoot) | QE_GatherLoot raised; AddLoot difficulty variant and per-character copies written
game-items-save | ItemInstance | reconstructed 3 (IsEquippableBy, _UpdateReqs, AddPower) | attribute gates read Character::m_props m_w20/m_w150..m_w157; AddPower builds VarArgs
game-items-save | PlayerStatManager | reconstructed 4 | InvokeASCallback on the HUD root
game-items-save | TrophyManager | reconstructed 4 | achievements.savegame through the device FS (bitset<128>), NotifyTrophy, HUD popup
game-ui | MenuMessageManager<T,N> | reconstructed 25 | one Invoke template body; StatusMsg/TutorialMsg/OnlineStatusMsg instantiated
game-app | DebugCachedCharacter | reconstructed 4 | weak_ptr refresh, RenderFX::Find, parent-chain walk
game-items-save | Quest, QuestSavegame, SavegameManager | reconstructed 12 | Quest::m_name (+0x14) named; QuestSavegame CompileQuests/UpdateQuests/ReInit; SavegameManager save/load of settings through the device FS, localisation walk over ObjectManager
game-items-save | SimpleTypeProperty<T> clones, PropertyMap, Property | reconstructed 13 | stale-register int results returned as 0; PropertyMap -> TiXmlElement (declared in tixml_dh.hpp)
game-items-save | Condition, Objective*, Reward, QuestMoveInZone, ObjectiveList, Objective_EventReceiver | reconstructed ~30 | debug traces read Arrays::X::m_memberNames; QE_MoveInZone raised; CheckMustSendThroughNetwork sends CMsgRaisedEvent::Create(event)
game-items-save | ItemObject | reconstructed 3 (_GetNextPlayerQuestRRId, Interact, InitAgain) | Interact: status lines, tutorial script, CMsgControllerAction (typed payload in c_msg_fixed.hpp: action_ +0x50, targetId_ +0x52, senderId_ +0x54), Play3D, loot_orb_fx; InitAgain: Play3D + POItem (POItem inherits the PhysicalObject constructor)
game-items-save | TOTAL | 103 rows reconstructed, 0 partial left in the category |
game-app | gameswf natives (global-f216b6-5..14) | reconstructed ~85 | natives written against the gameswf API (call.arg(i), call.result->set_*); online cluster blocked on net-session-online (GetRoomList, friend list, GameCenter singleton)
game-app | Script_* commands (pack4..pack12) | reconstructed 38 | records read from the Structs::<Name> of script_cmd.hpp; ExecScript/PlayEffect records completed in pydata (Pos3D moved into script_cmd.hpp); GSEndGame::s_inst defined
game-app | ScriptManager | reconstructed 5 + ~ScriptCmdImpl 2 | LoadScriptFile with the command factory tables (s_scriptCmdImplMap order read from the binary, game-app/src/script_cmd_map.cpp), StreamBuffer copy, ExecuteScript with ScriptCmdID/ExecScript, SkipScript/StartScript send CMsgScriptCmd
game-app | Application (part 2) | reconstructed 9 | IsOnlineGame, IsInternetAccessEnabled, IsWifiEnabled (nativeIsWifiAlive), SendGLHiScore (CXPlayerManager), IsUsingUncompiledData (empty string), ctor words +0xcc/+0xd0 are the empty strings
vox-audio | VoxSoundManager | reconstructed 11 | PlayMenu, RestartMusic, ResumeMusic, IPodControl, ShowPlayingMusicTitle, GetIPodPlaylistName/Count, SetIPodPlaylist, BackToMainMenu, PlayBeat, StopBeat
game-character | PlayerManager, PlayerInfo | reconstructed 9 | NetGateOpen gates real; _ManageCharacters (online handshake, 4912 bytes) still partial
game-character | CharStateMachine states (CSReviving, CSDespawn, CSLimbus, CSIdle) | reconstructed 8 | m_reviveTarget (+0x5c) named; CSDespawn clears the local player's object of interest
game-character | AIS* scripts (AISDefault, AISExternal, AISMonster, AISFaery, AISPlayer, AISPlayerIPhone) | reconstructed 12 | constructors take the skip-bindings bool; OnAnimEvent footstep FX; AISPlayer OnAggro/OnDeAggro combat music
game-character | CharAI (SetScript<T>, StepLoadScript/StepCreateScript, SetSkillsAndSpells, Update, OnUpdate, _UpdateAggro, OnTargetInSight, _OnAnimEvent, AI_Begin/EndSpell, AI_BeginSkill, AI_SpellInfo/SkillInfo, RaiseAIEvent, _UpdateRegen) | reconstructed 20 | CharAIScript is a LuaScript; DeclareSkill loops fill m_skillScripts/m_spellScripts; monster target branch of _UpdateAggro; isTracingCharAI_AnimEvent probes
game-character | CharAISkillScript | reconstructed 13 | Lua hooks through LuaScript::Call on the owner's AI script (SetSkill then the hook), projectile callbacks
game-character | CharProperties, CharTimers, SpawnGroupManager | reconstructed 9 | PROPS_Get clones (useTemp false), AddDot/DebuffSlow by name tables, FX clip advance; trace probes recovered from the string-constructor clones (isTracingSpawnGroupManager)
game-character | Character | reconstructed 78 (only Draw left partial) | INV_UpdateSkin, ReloadSkills, DeclareProperties, AddCharOIDToCache, UpdateObjectOfInterest, Update, CreateNPC/CreatePlayer; fields retyped (m_spawnDelay, NetStructCharacter)
game-character | PlayerManager, PlayerInfo | reconstructed 57 | only _ManageCharacters (online handshake) still partial
game-character | CharStateMachine, CharAI, AIS*, CS* | reconstructed 45 | pseudocode re-checked, stale-register returns dropped
game-world | ObjectManager | reconstructed 23 | InitPost function-local statics (it/mit), Create counters, AssignObjectNetworkId; SerializeGameObjectNetData, LoadGameObjectNetData, ForceFullUpdate, Draw3D, Flush still partial
game-world | Level | reconstructed 24 | savegame, checkpoint, fog helpers, SG_* loops over the local players, GetMPGameScript (const char*); camera/batching/_LoadProcess/Update/ctor/dtor still partial
game-world | GameObject | reconstructed 15 | _Summon online spawn (CMsgSpawnObject), GetCameraAnchorPosition, UpdateSubObjects; Draw (bounding box debug draw) still partial
game-world | Trigger, TriggerObject, TriggerPlate, TriggerZone, TriggerZoneExitLevel | reconstructed 21 | data rows via pydata::FindMemberName, QE_TriggerOn/QE_TriggerPlate raised on the level, PODecor body, Play3D; TriggerZone::Draw (debug box) partial
game-world | TriggerTrap, TimerTrap, ProjectileTrap | reconstructed 10 | GetDataId through Arrays::X::m_memberNames, m_data set from the row name, sound load and Play3D
game-world | Container, DestructibleContainer, OpenableContainer, SlotContainer | reconstructed 24 | __Callback getLoop, Deserialize gate (Level +0xf3/+0xf4), InitPost m_dae/LoadSound, PODecor body on dword_99F770 = Application::m_physicalWorld, DoOpen -> DropLootTable, Interact Play3D, QE_DestroyGameObject/QE_OpenGameObject raised, SlotContainer material through SceneManager::GetMaterial(node, "GL_", true) (now returns the material)
game-world | Level::UpdateCameraZoom | reconstructed 1 | online gate via GetOnlineGameFlag (old COnline blocker is stale); Level-7c7f5d-3.cpp compiles; CanMoveTowards still partial (angle test needs camera node position getter, vtable +0xa0 unnamed)
