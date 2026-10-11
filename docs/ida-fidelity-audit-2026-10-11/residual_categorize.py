import json,re,sys,collections
f=json.load(open(sys.argv[1]))
NET=r'^(GLX\w*|CMatching\w*|CNet\w*|CMessag\w*|CMsg\w*|CPacketManager|CTransport\w*|CConnection\w*|Connection\w*|ConnectionLobby|MultiplayerCallback|MultiplayerStateMachine|MultiMenuManager|OnlineGameState|COnline\w*|CXPlayerManager|CRoomSearchFilter|CLobbyParameterAndQuery|CBluetooth\w*|CAndroidSocket|LCAndroidSocket|LCXPlayer\w*|CTcp|CUdp|CNetworkEmulation|NetStruct\w*|NetBitStream\w*|DataPacket\w*|DefaultDataPacket|tMatchingPeer|tFriendInfo|tRoomInfo|tGLRoomInfo|tMemberInfo|CNetworkId|CNetMutex|CNetLog|CReadWriteLock|CMutex|Thread|updateJob_thread|ALicenseCheck|OnlineSingle|_LiveFeed|QE_\w+|MenuMessageM\w*|v2NetController|v2PS3MoveController|TouchScreenPS3|AccelerometerPS3|CharMenuTutorialMsg)$'
CATS=[
('G0 COMBAT FORMULAS: CF__Calc*/CF_SetCombatants free fns (damage, crit, block, miss/dodge, stun, slow, fear, push, hurt, dot)',r'^<free>CF_'),
('G1 INPUT: v2Controller/v2Controllable/v2Gamepad/Mixed/Hud/Emu controllers, Gamepad, InputManager',r'^(v2(Controller|Controllable|MixedController|GamepadController|HudController|EmuController)|Gamepad|GamepadTweaker|InputManager\w*|InputDevice|TouchScreenBase|TouchScreenIPhone|TouchScreenWin32|AccelerometerBase|AccelerometerIPhone|AccelerometerWin32|Keyboard|Mouse|Ev\w+|IEventReceiver|EventManager)::?$'),
('G2 HUD / zoom / minimap managers',r'^(HUDControls|HUDMinimap|InfoHUDManager|ZoomHandler|MenuMinimap|MenuDebugHUD|SWFAnimToolTip|PlayerLightTweaker)::$'),
('G3 gameswf Native* callbacks (non-matching F regex)',r'^<free>Native'),
('G4 quest objective templates / event receivers',r'^HIDDEN-BY-RETTYPE:quests|^<factory:GetObjectiveImpl|^<free>(GetRewardImplInstance|Script_CharacterState_Callback|GetQuestFunctor|IsNonDebugQuest\w*|IsDebugQuest|GetDialogMsgListById|ParsePlayerName|IsMultiplayerGame)'),
('G5 AI/state/object factories (CharAI::SetScript, GetNewState, GetNewInstance)',r'^HIDDEN-BY-RETTYPE:ai|^<factory:GetNew(State|Instance)'),
('G6 PropertyMap::AddProperty<T> (property registry)',r'^HIDDEN-BY-RETTYPE:stats'),
('G7 gameplay free fns (GetLevelScaledXP, TrophyCheckArmorSet*, playerClassAs*, IsPlayableClassID)',r'^<free>(GetLevelScaledXP|TrophyCheckArmorSet\w*|playerClassAs\w*|IsPlayableClassID|UpdateSavedValues|AssignSaveSlotToPlayer|TempHackScriptIDAreStrings)$'),
('G8 TrophyManager / TrophyData / AchievementMsg',r'^(TrophyManager|TrophyData|AchievementMsg|GameCenter\w*)::$'),
('G9 scripting leaf structs: Structs::<ScriptCmd> (97 cmds)',r'^Structs::(FlushMessages|DoTutorial|LockTutorial|Skip\w+|Enqueue\w+|BlockSaveGame|SaveGame|ShowTrophies|AwardEndGameTrophies|AwardTrophy|EndGame|ChangeLevel|RestartLevel|SetWorldMapLocationState|SetLevelState|SpawnContainer|\w+TriggerPlate|\w+ProjectileTrap|CloseDoor|OpenDoor|SetStatic|ReEquipHands|UnEquip\w+|AutoEquip|DropLoot|SetActor\w+|PlayActorAnim|KillActor|HideActor|ShowActor|LookActor|MoveActor|StopActor|AI\w+|MarkCharacterAsScripted|PutCharacter\w+|SpawnCharacter|IncFaeryLevel|SetFaeryState|Wait|UnlockCharacter|LockCharacter|HideFlash|ShowFlash|StopEffect|PlayEffect|PlayAnim\w+|LeaveSafeZone|EnterSafeZone|PlayLevelMusic|StopSound|PlaySound|WaitDialog|StartDialog\w*|NilCmd|SetCamera\w*|WaitCamera|PlayCamera|CONSOLE|ExitCutSceneMode|EnterCutSceneMode|ExecScript|HideHUD|ShowHUD|ScriptCmd)$|GetNewScriptCmdDataInstance'),
('D1 data schema (Structs::/Arrays:: read/finalize/GetMemberID: items, skills, loot, quests, conditions, rewards...)',r'^(Structs|Arrays)::'),
('N1 network/multiplayer/online/social',NET),
('E1 glitch engine',r'^glitch::?$'),
('E2 std/STL/template instantiations',r'^std::?$|^<free>(std|operator)'),
('E3 gameswf / RenderFX / GameSWFUtils',r'^(gameswf|RenderFX|GameSWFUtils|FSCommandParam\w*|FlashAnimManager|FlashAnimContext|SWFAnim\w*|RenderFX::Event)::?$|^<free>(fscommand|FSCommand)'),
('E4 vox audio / SoundEmitter / VoxSoundManager',r'^(vox|VoxSoundManager|SoundEmitter|CharSounds)::?$|^<free>(Vox|nativ)'),
('E5 boost',r'^boost::?$'),
('E6 Box2D',r'^b2\w*::?$|^<free>b2'),
('E7 Lua core (+LuaScript/LuaManager glue)',r'^<free>(lua\w*|f_\w+|str_\w+)|^(LuaScript|LuaManager)::'),
('E8 freetype / png / jpeg / zlib / vorbis / TinyXML',r'^TiXml\w*::?$|^<free>(FT_|ft_|tt_|TT_|cff|cid|pfr|pcf|PCF|psh|ps_|sfnt|t1_|T1_|t42|T42|png|jpeg|jinit|jcopy|jzero|jdiv|jround|inflate|deflate|huft|zlib|adler|crc|h2v|vorbis|stb|bdf|BDF|FNT|fnt|afm|af_|gray|decode|encode|dct|Compute|PVRTC|XP)'),
('E9 C runtime / GL / libc stubs / sub_',r'^<free>(gl[A-Z]|__|str|mem|isw|is[a-z]|tow|to[a-z]|f(open|close|read|write|seek|tell|get|put|flush|printf|puts|eof|setpos|getpos)|sin|cos|tan|asin|acos|atan|exp|log|pow|sqrt|floor|ceil|fmod|ldexp|frexp|modf|abs|time|clock|usleep|nanosleep|mmap|munmap|socket|ioctl|fcntl|lseek|unlink|rename|opendir|readdir|closedir|chdir|getenv|setlocale|qsort|malloc|calloc|realloc|free|exit|abort|printf|sprintf|snprintf|vsnprintf|vsprintf|vprintf|sscanf|puts|putc|putchar|getc|ungetc|atoi|atol|wcs|wmem|swprintf|sigaction|setjmp|longjmp|sysconf|uname|inet|gethost|bind|listen|accept|recv|send|connect|getsockopt|setsockopt|getsockname|getpeername|localtime|gmtime|mktime|strftime|gettimeofday|noop|dummy|asin|sys|pthread)|^<sub>|^<global-ctor>|^<free>__'),
('X1 misc: PyDataArrays (data-table loader), VisualFXManager, SimpleTypeProperty, sfc, other single-fn lib/free families',r'^(PyDataArrays|VisualFXManager|SimpleTypeProperty|sfc|Array2d|CDataStats|Point3D|Point2D|VectorSet|AbstractTableModel|CEvent|CEventQueueBase|channel|II_Item\w*)::?$|^<free>(II_Item|operator<)'),
('P1 platform/core glue (Application, StringManager, Console, Debug, File, Config, Menu/GS states, timers, scene nodes, strings)',r'^(Application|StringManager|Console|DebugSwitches|DebugCachedCharacter|DataReloaderManager|IDataReloader|FileSystem\w*|IFileStream|IStreamBase|ISerializable|StreamReader|StreamBuffer|Buffer|ConfigFile|Menu\w*|GS\w*|OnScreenKeyboard|Device|ADevice|_JNIEnv|AssetManager|PropertyInstance|Property|TimerUtil|StateBase|NonCopyable|PerfCounters|slim|batch|sys|Pos3D|Point|Rct|Vector3DFList|SceneFile|ColladaFile|ColladaFactory|IrrFactory|SceneManager|MeshSceneNode|RootSceneNode|UnderlaySceneNode|Billboard|BufferedRenderer|LightBase|LightPoint|LightSetManager|VisualObject|MaterialData|bitmap_info_ogl|render_handler_glitch|\w*SceneNode|Anim\w*|AnimSetManager|AnimationSet|Animator\w*|AnimStep|FootstepEffect\w*|CharEffect\w*|CharEffectTable|AnimatedEffectTable|CamAnimSet\w*|SimpleAnim|StrID_Languages|LangSheet|FontPalette|PFInnerTest\w*|PFG\w+|TestUD2?|Dummy|CEventQueue|CharSoundsTable|Sounds\w*|Sound\w*|Listener\w*|AdpcmState|VarArgs|TestChar\w+|PolyStat|lstr)::?$|^HIDDEN-BY-RETTYPE:level'),
]
rows=collections.OrderedDict((c,[0,0,[]]) for c,_ in CATS); rows['E10 single-function third-party/engine free fns (zlib/ft/jpeg/vorbis/mpc/lua/glu/etc.)']=[0,0,[]]; rows['UNCLASSIFIED']=[0,0,[]]
for k,(c,b,l) in f.items():
    kk=k
    hit='UNCLASSIFIED'
    for name,rx in CATS:
        if re.search(rx,kk) or re.search(rx,kk.rstrip(':')) or re.search(rx,kk+'::' if not kk.endswith('::') and not kk.startswith('<') and '::' not in kk else kk):
            hit=name;break
    if hit=='UNCLASSIFIED' and k.startswith('<free>') and c<=3: hit='E10 single-function third-party/engine free fns (zlib/ft/jpeg/vorbis/mpc/lua/glu/etc.)'
    rows[hit][0]+=c;rows[hit][1]+=b;rows[hit][2].append((k,c,b))
tc=tb=0
for n,(c,b,l) in rows.items():
    print(f'{c:6d} {b:9d}  {n}'); tc+=c;tb+=b
print(tc,tb)
print('UNCLASSIFIED top:')
for k,c,b in sorted(rows['UNCLASSIFIED'][2],key=lambda x:-x[2])[:int(sys.argv[2])]: print(' ',k,c,b)
json.dump({n:v for n,v in rows.items()},open(sys.argv[1]+'.cat','w'))
