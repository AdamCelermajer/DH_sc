# Character Kill and Ctrl_Kill

Original ELF SHA256 `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
The complete routines are captured in `original-functions.json` and
`original-functions.asm`: Kill `0x3a5b18`, 1776 bytes; Ctrl_Kill `0x3ad528`,
76 bytes. Capture includes the literal pools; disassembly after the terminal
branch is data, not additional executed instructions.

`character_kill.hpp/.cpp` implements both recoverable control-flow bodies.
The deeper services are mandatory synchronous borrowed backends, not completed
loot, XP, quest, player-manager or AI implementations. This is source-only;
no APK, scene death, or full-game execution claim follows from these fixtures.

## Original order and gates

1. Ctrl_Kill invokes receiver IsDead virtual+34 (`3ad540`). A true result
   returns. Otherwise it calls actual Kill (`3ad55c`) and tail-calls
   Character.RaiseEvent(2,attacker) (`3ad564..570`). Kill independently invokes
   IsDead again at `3a5b34`. Thus an inner dead reread skips Kill's work but
   does not omit the outer event2 tail. There is no controller-lock gate here.
2. Kill's false dead result writes Character byte1449=1 (`3a5b5c`) and
   PROPS_Set(36,0) (`3a5b68`). The existing native property kernel preserves
   source property type behavior. Kill then invokes live IsPlayer virtual+28.
3. Player and !force: PROPS_AddInt(25,1), IsLocalPlayer(receiver); for a local
   player, capture the current TrophyManager before querying cached property25
   at thresholds10/50/100. Exact trophy names are `quest_died_10_times`,
   `quest_died_50_times`, `quest_died_100_times`. Arrays.GetMemberIDByString
   `3a3f70` supplies the ID to UnlockTrophy `3813b8`; even a negative ID is
   delivered without invented filtering. Then COnline `7fd794` reads byte5;
   true calls GetLocalPlayer(0,true) and ignores its result. This branch
   returns without loot, killer, aggro, XP or quest processing.
4. Other cases obtain Application.GetCurrentLevel `31f594` and read Level+150
   (`3a5bec`). Zero invokes Character.DropLoot(attacker) `3a5ae4`. Only AFTER
   this gate/possible loot call does nonzero force return, including with a
   null attacker. Initial null Level is an unsafe original dereference.
5. Nonzero attacker writes Character.killer144c (`3a5c0c`). Initial credit is
   attacker==receiver. The loop rereads the signed current aggro count each
   iteration (`3d4a10`); GetAggroEntry `3d72d0` supplies Character and float,
   with the float unused by Kill. Null entries are skipped. A nonnull owner
   at entry+14d4 replaces the selected Character, but the original entry
   identity is retained for attacker credit.
6. RaiseEvent(4,receiver) on the selected participant precedes live IsPlayer.
   A player whose current tracked_target14a4 equals the receiver gets it
   cleared. AddInt(23,1) and AddInt(24,1) follow. Credit becomes true iff the
   original entry equals the attacker; owner redirection does not change that
   comparison. IsLocalPlayer follows. Its true result captures the current
   TrophyManager and rereads cached property23 for100/500/1000/2000; exact
   trophy names are `killed_100`, `killed_500`, `killed_1000`, `killed_2000`.
7. Credited kills reread current killer144c and invoke IsCharacter virtual+24
   (`3a5e08`). True reloads killer AGAIN and executes GetHandle `33dd2c` then
   Character cast `33ff54`. False passes null. Character.DistributeXP
   `3bf828` receives the resulting nullable Character and victim. A null
   attacker skips killer, aggro and XP entirely.
8. Current IsRemotelyUpdated virtual+54 (`3a5e28`) then byte14e4 gate quest
   processing. False/zero obtains CurrentLevel AGAIN. The returned identity
   is retained for all four events. Null Level enters source assert/debug
   handling (`3a6150`): mode2 deliberately faults; other modes can continue
   into an unsafe null queue receiver. Native reports this reached boundary,
   without pretending any null queue or assertion backend is valid.
9. PyDataConstants.getConstant `4c4bdc` uses exact group
   `v2QuestObjectiveType`. Keys and typed events in order:

   | Kind | Key | Subject |
   | --- | --- | --- |
   | 1, QE_KillEnemies | KillXEnemies | signed property_id13c8 |
   | 2, QE_ClearEnemies | ClearEnemies | reread property_id13c8 |
   | 3, QE_KillEnemyTemplate | KillEnemyTemplate | signed template_id13ca |
   | 4, QE_ClearEnemyTemplate | ClearEnemyTemplate | reread template_id13ca |

   Events1/2 always occur after these gates. Template_id is read after event2;
   -1 skips3/4. Each event's subject and OID+64 are captured BEFORE its constant
   query; mutations during query or queue affect only later events. All events
   carry the original attacker identity, network_id=-1, flags0/0 and that
   captured Level. The source uses EventManager.RaiseAsync `339090` on stack
   event storage (`3a5ed0`, `3a5f44`, `3a5fb8`, `3a6014`). The provider must
   synchronously copy/own the payload, not retain the borrowed event pointer.

## Death routing is a required backend

Kill directly changes only dead1449, killer144c, participant tracked_target14a4
and properties25/23/24/36. It does not directly set animation, remove a physical
body, or change a native StateInfo. Those effects must flow through the actual
event2 backend; they must not be fabricated in this module.

`death-routing-functions.{json,asm}` captures the deeper ownership boundary:

- Character.RaiseEvent `3a4d5c` forwards event2 to CharAI.RaiseAIEvent `3cbb34`.
  The existing genuine dispatcher calls AI virtual+24 before reloading the
  owner and forwarding to the native FSM. That CharAI virtual callee is
  OnDied `3d1000`, not an arbitrary Lua OnDied shortcut.
- OnDied reads GroupInfo+34; if present, GroupInfo.OnDied `3d2628` receives
  owner+4 and attacker. It then reloads active AIS+1c and calls its virtual+24
  if present, and tail-calls AI_SetDead `3d6cdc`. GroupInfo kind2 can Cmd_Kill
  multiple group members synchronously; its stable group ownership and reentry
  behavior are required, not an accepted empty callback.
- AI_SetDead order: AI_SetTarget(NULL,false) `3d6890`, AI_SyncLastTarget
  `3d49c4`, owner's SM_SetDeadState(false,NULL,true) `3c58c8`, TMR_Stop
  `3db2d8` for current AI timer+10 and then current+14, writes both=-1,
  AI_ClearAllAggro `3d5fa8`, AI_ClearAllAggroTowardMe(false) `3d6abc`,
  _SkillCleanUp `3d8ae0`, then tail _SpellCleanUp `3d8a98`.
  These bodies are captured for handoff; this Kill proof does not execute them.
- Existing source state12 focus/blur/animation/body kernels can be reused by
  a genuine SM_SetDeadState owner. Simply assigning current=12 would omit its
  transition predicates, prior blur, animation selection, timers, filtering,
  buff removal, and actual service order.
- DropLoot calls Character.GetCharDecl `3a2fcc`, then ItemGenerator.DropLoot
  `3ecba0(decl,receiver,attacker,-1,stack0)`. Full DistributeXP is also captured
  (1972 bytes) but remains a mandatory deeper ownership service.

## Correction to historical HitFor wording

The exact symbol at `36e478` is PlayerManager.GetLocalPlayer(int,bool), not
GetCoopGame. Application+40 is the PlayerManager; its returned Player record
has the Character pointer at+660 used by HitFor. `36effc` is the boolean
PlayerManager.IsLocalPlayer(Character*), not a host-player pointer getter.
The historical HitFor projection behavior remains correct; its frozen prose
used incorrect names. Likewise `3bf828` is DistributeXP, and `339090` is
typed async quest delivery, not generic OnCharKill/scene FX. No frozen proof
or prior source was changed to conceal these corrections.

## Native ABI and caller contract

KillActor56 projects a retained Character and its existing PropertyView.
Owner/aggro participant projections, Level projections and all sheets/groups
must remain alive through callbacks. Stable pointers are not a substitute for
world ownership. Mutable fields, classifications, aggro counts and global
trophy/constants identities are reread at their source positions. No arbitrary
loop cap, callback suppression, once-only guard or reentry lock is added.

KillServices16 returns0 only for genuine delivery. Queries fill KillResponse16.
current_level and aggro_entry return retained KillLevel16*/KillActor56*;
handle_character returns the actual nullable handle/cast Character identity.
The source virtuals remain dynamic services even though native query helpers
can provide classifications for genuine owned objects. KillQuest48 defines
semantic payload fields; original uninitialized stack padding is not promised.
Reserved native fields must be0. The original methods are void; native status
is a diagnostic contract:1 completed, -1 malformed atomically, -2 failed
required service/invalid provider projection, -3 unsafe null-Level producer.
Reached original mutations remain after -2/-3. Failed service delivery cannot
create a fake successful event2 tail.

## Proof and reproduction

`tools/build_character_kill_oracle.ps1` builds O2 ARM64 from character_kill.cpp
and the existing properties.cpp, with no fast math. Then run
`tests/character_kill_differential.py` using the direct configured Python.
It executes actual original Kill/Ctrl_Kill and source property/type/resolution
instructions. Deeper services are labelled fixtures, not skipped claims of
their implementation. Unknown imports fail. Native projections use64-bit
pointers above4GiB.

Final original/O2 corpus:2200 golden cases plus22 synchronous reentry cases,
24705 ordered requests,69 unsafe null-Level prefixes,8 atomic guards,
zero mismatches. It checks complete source orders, all16 sheets, actor fields,
typed quest payloads, trophies, signed/wrapping integer boundaries, force/null
attacker gates, changing counts and callback-mutated killer/OID/template/global
manager fields. Reentry executes actual nested Kill/Ctrl_Kill at dead/player/
Level/event/queue boundaries; outer and nested results/traces are independent.
One important case writes death in a nested call during outer Ctrl's IsDead;
the outer snapshot stays alive, the inner reread skips, and event2 still fires.

`tests/character_kill_host.py` creates the binary gold then builds an isolated
sanitizer DSO linked to actual central world/data/runtime DSOs. It verifies
dladdr/ldd library identities and dependency hashes before/after execution.
Final host:484688 checks,2200+22 original gold replays,836 service-failure
prefixes,8 atomic guards,zero ASan/UBSan findings. Genuine dependency cases
execute property/classification/handle kernels and the real AI event2 dispatcher;
the unresolved OnDied backend fails with its actual callee3d1000. Forced Kill
completes with explicit retained Level/no-loot projection. Nonforced self XP,
loot, and async quest ownership fail at their real required boundaries.
The quest prefix loads the actual cache-backed v2quests_pycst.bin through the
genuine constants DSO, gets KillXEnemies, constructs its source payload, then
fails required async delivery; no fabricated constant or queue success.

Parent integration: add character_kill.cpp to dh2_level_world and optionally
target `character_kill_audit` from tests/character_kill.cpp linked to
dh2_level_world, dh2_game_data, dh2_script_runtime and dl. The executable takes
two arguments: `.local-inputs/character-kill-discovery/host-fixtures.bin` and
`port/android-native/app/src/main/assets/data/v2quests_pycst.bin`. After central
rebuild, run `tests/character_kill_host.py --main-linked`; it independently
compiles its test executable and writes a separate main-linked report, preserving
this isolated audit's bound dependency/library hashes.
