# Player Kill continuation V1

## Source boundary

Pinned original ELF SHA256: `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.

`Character::Kill` is the complete 1,776-byte function at `0x3a5b18`, SHA256 `5d0c8880d8d9fa1d5e73d3be9b69320b019f3dfcee5babbd79b6700de76abf6b`. Its own fresh IsDead virtual `+0x34` precedes byte `Character+0x1449=1` at `0x3a5b5c` and `PROPS_Set(36,0)` at `0x3a5b68`. Runtime enters at `0x3a5b6c`, after that reached prefix. It requires the canonical dead flag equal to one and live cached HP36 equal to zero. It does not reset dead, call full Kill again, change HP, or infer force from damage.

`combat_application.cpp` already supplies that dead/HP prefix. Calling the original full Kill or Ctrl_Kill after it would take the dead entry skip. Runtime consumes one reached episode even when a required provider fails, preserving completed effects. A subsequent revival needs a new episode borrower.

## Ordinary Player order

1. Fresh Character IsPlayer virtual `+0x28` (`0x3a5b78`). A false result or nonzero force reaches a required external general-continuation provider; this module does not implement or claim that branch's loot, XP, aggro, or quests.
2. `PROPS_AddInt(25,1)` (`0x3a5b9c`), raw delta 256 in the same borrowed PropertyView.
3. Fresh Application singleton and its actual PlayerManager `+0x40` (`0x3a5ba0`), then `IsLocalPlayer(character)` (`0x3a5bac`). Offline alone does not establish locality. Original IsLocalPlayer calls GetPlayerByCharacter, then PlayerInfo's inherited CNetPlayerInfo virtual `+0x50`; that leaf requires the actual CMatching Get/IsServer/member IDs even in the offline branch. Native's host-level projection does not yet supply those owners, so native full Kill binding remains open.
4. If local, capture the current TrophyManager singleton (`0x3a602c..0x3a6040`) before GetInt. Read `PROPS_GetInt(25,false)` freshly for exact equality to 10, then 50, then 100 (`0x3a6044`, `0x3a605c`, `0x3a6074`). The false path reads cached `+0xa94` and signed-shifts by eight; it does not recalculate base/saved/gear/buffs. Each later read may see callback mutations. Fresh name lookup selects `quest_died_10_times`, `quest_died_50_times`, or `quest_died_100_times`, then UnlockTrophy uses the earlier captured manager. Negative lookup ID is passed through for the real UnlockTrophy skip.
5. Both local and nonlocal paths read `GetOnline()->byte_5` (`0x3a5bb8`). Zero returns normally. Nonzero reloads Application and PlayerManager (`0x3a5bc8`) and calls `GetLocalPlayer(0,true)` (`0x3a5bd8`); its return is discarded, including null.

All reached source services are mandatory. Their successful completion flags describe completed calls; a callback which mutates and then fails can leave effects beyond those flags. Nonzero return or exception stops at that prefix. No rollback, automatic retry, timer store, VM, property store, or matching/trophy authority is introduced.

## Separate Ctrl_Kill and event 2

`Ctrl_Kill` is the complete 76-byte function at `0x3ad528`, SHA256 `1b242e0090fb2cfa8f7b50c59bd2285f341ec0368ca7bc7e84294cf8d0516c53`. CtrlCaller reads outer IsDead before invoking the required genuine full Kill provider (`0x3ad55c`), then RaiseEvent(2,killer) (`0x3ad570`). Inner Kill has its own fresh IsDead. If that inner query skips, the outer event tail still runs. Failed full Kill or OnDied delivery retains effects and prevents later event forwarding; failed CtrlCaller attempts cannot be retried.

Character RaiseEvent is 28 bytes at `0x3a4d5c`, SHA256 `2422a21cba16aea71f10939f0f32fe6326e27beb11fe6160b058426b0e2156e3`. Event2 enters its CharAI at `+0x3c8`. The complete 1,764-byte RaiseAIEvent at `0x3cbb34`, SHA256 `f8acba22b51432beded75bba8e834aac58382fd66b137c1f310a16496789b966`, executes fresh CharAI virtual `+0x24` OnDied(killer) (`0x3cbea4`) before forwarding machine Event(2,killer) (`0x3cbc74`). It runs before ordinary blocked/locked/paused gates. The existing selected `character_ai_events` dispatcher supplies this route. The OnDied/AI_SetDead runtime's direct transition to state12 uses null payload; the outer FSM event retains the original killer.

## Evidence

Original replay executes the full original Kill body for the ordinary Player path, full Ctrl_Kill, Character RaiseEvent, AI event2 routing, and original Set/AddInt/Add/GetInt/RecalcProperty/property accessor kernels. No interior body is skipped. The original Application/TrophyManager memory reads execute. PlayerManager/locality, trophy lookup/unlock, online, OnDied and FSM leaves are declared caller fixtures. General Kill, Matching construction, trophy persistence and native locality are not dynamically proved by this replay.

The host gate covers exact thresholds, fresh cached reads against stale base/saved, captured trophy identity while the singleton changes, fresh Application manager, negative ID, discarded null GetLocalPlayer, required-service failures/exceptions, effect-then-failure, entry and alias guards, and once-only failure retention. The separate three-class composition prepares all unchanged authored player scripts, then runs CtrlCaller through the selected generic AI dispatcher, same borrowed player_ai_death Runtime, same Session cleanup and same Coordinator. The original death/state/animation/scene external services remain explicitly declared fixture providers. All 13 cleanups execute without another VM or initial update, timer33/34 IDs retire, and unrelated timer35 stays active. This is host composition evidence; native full Kill/locality is not bound.

The runner links actual dh2_level_world and its single script runtime. It stages the new TU only when central selection is absent and detects the actual selected object exactly once. Actual Ninja object dependencies, configured project CMake inputs, and exact test/reference inputs are snapshotted before a clean selected-target rebuild and checked afterward. Unselected adjacent headers/tests do not enter the guard.
