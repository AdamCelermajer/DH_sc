// Batch g1-game-app-global-f216b6-12 (<global>, part 13 of 22): the gameswf natives of the message queues, online kick
// and login, equipment swap, zone travel, skill lists and details, ranking, world map, quest list, merchant list, HUD
// back, map icons, iPod, string lookup and multiplayer rooms, ELF 0x440e5c .. 0x445a10. Declarations:
// dh2/global/native_gameswf_menus.hpp.
//
// What is reconstructed and what is not:
//   - BubbleSortByTitle (header) is the title sort of NativeGetQuestIDsInRange. It is tested in
//     game-app/tests/g1-game-app-global-f216b6-12_test.cpp.
//   - Every native is PARTIAL. Each one either reads an argument or writes its result through gameswf, which is the
//     blocker for all twenty, or it needs a piece the tree does not declare yet: the Singleton accessor and the
//     s_SkipFuncName and s_StartFuncName pointers of the message queues, the OnlineStatusMsg, StatusMsg and TutorialMsg
//     classes, RenderFX::InvokeASCallback and RenderFX::GetRoot, and MultiMenuManager::ResetUpdateCursor. The bodies are
//     shells. The comment above each one gives the original's steps so that it can be filled in when those land.
//
// Shared facts used by the comments:
//   - Argument layout. fn_call +12 is the environment pointer (env, the object that fn_call +0x0c points to; see
//     dh2/global/native_app_hud_level.hpp). The argument array base is the word at env +0x00, that is **(fn_call +12).
//     fn_call +16 is the argument count and +20 the index of the last argument ("top"). "top-1" is the argument below
//     it, and the value at index i is at base + 12 * i. A value record is 12 bytes and its tag is the byte at +1:
//     1 boolean, 2 number, 5 object. Tags 3 and 4 are the string kinds (NativeStopMessage accepts either). The result
//     value is the fn_call's first pointer (*call).
//   - Application singleton: Singleton<Application>::s_inst. Application::GetCurrentLevel() is the current Level or
//     null. Application::LoadLevel takes (name, entryPointId, savegameSlot, resume, useSpawnPoint, difficulty,
//     isRemotelyTriggered, seed, syncedSeed).
//   - Level fields: m_isLoaded +0x144, m_loadStep +0x130 (38 = loaded), m_isInGameView +0x198.
//   - Character sub-objects: ItemInventory +0x37c, CharProperties +0x560, CharAI +0x3c8. PlayerInfo +0x660 is the
//     local player's character (named character_660 in the Hex-Rays output).
//   - NativeGetPlayerChar(int, bool) is declared in dh2/global/native_app_bindings.hpp and returns Character*.
//   - MenuManager: GetInstance() is static. m_multiMenuManager +0xf4, m_actionIconType +0x108.
//   - HUDControls (dh2/HUDControls.hpp): static GetInstance() and Touch().
//   - MenuMessageManager<T, N> (dh2/MenuMessageManager.hpp) keeps one std::deque per queue in m_queues[N], at +4 of
//     the object. Its Invoke(functionName, queue) and SkipEnqueuedMessage(queue, flag) take a queue index, and the
//     pseudocode passes none of them. The Hex-Rays deque globals (dword_99FBB8 and the others) are not tied to a
//     singleton instance address in the tree, and Singleton<T>::s_inst is not declared.
//   - OnlineSingleton<OnlineGameState>::GetInstance() is the online game state. Its state word +0x14 is the state
//     machine (stateMachine_). Its mode word +0x34 and its room list +0x50 are not declared in
//     net-session-online/online_game_state.hpp.
//   - CMatching::Get() takes no argument. The assembly of NativeLoginInfos (0x444ae0) calls it with none, so the
//     argument that Hex-Rays prints on some of its calls is a decompiler artifact.
//   - Global instance pointers without an accessor in the tree: the StringManager global at 0x99f760, the
//     PyDataConstants global at 0x99f758, the ObjectManager global at 0x99f764 (ObjectManager::GetObjectByName(name,
//     flags, active, type) is declared in dh2/ObjectManager.hpp), the PlayerManager global at 0x99f76c, the MenuManager
//     global at 0x99f780 (dh2/MenuManager.hpp declares a static GetInstance(), not checked against this global) and the
//     ZoomHandler global at 0x99f77c.
//   - The gameswf value API (argument accessors, value setters, as_object and as_array members, tu_string, drop_refs)
//     is not in the tree. dh2/global/native_app_bindings.hpp records the accessor signatures from the assembly, but
//     they are not declared. A native that uses them is PARTIAL, and its comment lists the calls it needs.
//   - Arrays::FastTravelList (stride 28), Arrays::LevelList (stride 72) and Arrays::v2Quests (stride 284) are in
//     pydata-accessors (array_strides.hpp, member_names.hpp).

#include "dh2/global/native_gameswf_menus.hpp"

// 0x440e5c NativeGetNextOnlineStatusMessage(gameswf::fn_call const&)
// PARTIAL. No argument is read. If the OnlineStatusMsg queue is not empty (the deque object is at global dword_99FB34,
// and its start and finish cur pointers are at 0x99FB34 and 0x99FB44), the result becomes the front element's text. The
// text is the element's string at Hex-Rays +16 and +20, copied into a local std::string with _M_assign. The element's
// word at +24 is read into a local and never used. An empty queue leaves the result unset. The original's first local
// (the literal "not yet supported with no hud code") is built and discarded, so it has no effect. Missing:
// as_value::set_string on the result, the OnlineStatusMsg element layout and class (no dh2/OnlineStatusMsg.hpp), and the
// queue's declared type (MenuMessageManager has no OnlineStatusMsg instance in the tree).
void NativeGetNextOnlineStatusMessage(const gameswf::fn_call& call) {
    (void)call;
}

// 0x441b60 NativeKickPlayerOut(gameswf::fn_call const&)
// PARTIAL. The player id is to_number of the top argument. Online branch: when the online state's mode word (+0x34) is 3
// (GLLive), CMatchingGLLive::GetMemberByMemberId(id) finds the member record. When the member's first word is not -1,
// the original calls CMatching::Get() and then the virtual at vtable +0x40 with the id. That is the kick. Other modes:
// the original walks CNetPlayerManager::GetPlayerIdList() from the first id to the end of the list. For each id it
// copies CNetPlayerManager::GetPlayer(id, false) into a local CNetPlayerInfo, calls GetPlayer(id, false) again, and
// compares the word at +0x1a0 of that second result with the id. On a match it calls the same virtual at +0x40 on
// CMatching::Get() with the id. The local copy is destroyed on both paths. The loop does not stop after a kick: it
// advances by one id (ADD R7,R7,#4) and repeats until the end of the list (CMP R7, end; BNE back to the loop head), with
// no break. So every listed player whose +0x1a0 word equals the argument is kicked. The native writes no result.
// Missing: to_number, OnlineGameState +0x34 (not declared), the name of the CMatching virtual at +0x40 (not in the vtable
// listing used here), and CNetPlayerInfo +0x1a0 (no dh2 header declares it; net-transport/tb/cnet_player_info.hpp documents
// that word as conn_index_). CMatching::Get() takes no argument (see the layout facts).
void NativeKickPlayerOut(const gameswf::fn_call& call) {
    (void)call;
}

// 0x442198 NativeSwapEquipment(gameswf::fn_call const&)
// PARTIAL. The player index is to_number of the top argument. For the Character from NativeGetPlayerChar(index, false),
// when it is non-null, the original runs ItemInventory::SwapEquipmentSet on the inventory at +0x37c, then
// CharProperties::UpdateGearsProperties at +0x560, Character::INV_CheckItemsRequirements, Character::INV_UpdateSkin and
// Character::ValidateHPMP. Then the HUD: RenderFX::InvokeASCallback "DisplayRightHud" with no arguments, and
// "FillActionIcon" with one number, MenuManager::GetInstance()->m_actionIconType (+0x108). Missing: to_number, the gameswf
// root and root movie accessors, RenderFX::InvokeASCallback and RenderFX::GetRoot (not declared in dh2/RenderFX.hpp), and
// drop_refs. NativeGetPlayerChar is declared, so the inventory part can be written once the gameswf accessors land.
void NativeSwapEquipment(const gameswf::fn_call& call) {
    (void)call;
}

// 0x4422b8 NativeGoToZone(gameswf::fn_call const&)
// PARTIAL. The zone name is to_string of the top argument. The entry point id is to_number of the argument below it,
// with -1 changed to 0. The original then runs this block when there is no current level or the current level is
// loaded (m_isLoaded +0x144). If there is a level and its m_loadStep (+0x130) is 38, it calls Level::QuickSave(false).
// It then calls Level::SG_SaveAllPlayer(false), and the save slot is Character::SG_GetSlot of the local player's
// character (PlayerInfo +0x660). With no level the slot is 0. When the name is not empty, the original searches
// Arrays::LevelList (stride 72) for the name, and the level file name is the member's word at +32. It then reads
// Character::SG_GetGameDifficulty of the local player's character, runs RenderFX "DisplayFastTravel" with four
// arguments (false, name, name, entry id), and calls Application::LoadLevel(level file, entry id, slot, true, true,
// difficulty, false, 0, 0). Finally it releases the name. A name that is not in the table is not checked by the
// original: it reads the element before the table (index -1, offset -72). A rebuild must treat that case as an error
// and must not read that memory. Missing: to_string, to_number, the setters for the callback arguments and drop_refs;
// RenderFX::InvokeASCallback and RenderFX::GetRoot (not declared in dh2/RenderFX.hpp).
void NativeGoToZone(const gameswf::fn_call& call) {
    (void)call;
}

// 0x4425a0 NativeSkillGetEquipedSkillsIDs(gameswf::fn_call const&)
// PARTIAL. Needs two or three arguments. The top argument must be an object (tag 5) and the one below it a number (the
// player index). With three arguments, the third from the top must be undefined or boolean (tag 0 or 1). Its to_bool is
// the flag for NativeGetPlayerChar(index, flag). The top object (cast to as_array) gets the three equipped skill ids,
// Character::SG_GetSkillInSlot(character, 0..2), each pushed as a number. Then the result is set to true. With no
// character the result is left as it was. Missing: the argument accessors (count, tag, is_number, to_number, to_bool),
// cast_to<as_array>, as_array::push, set_bool and drop_refs.
void NativeSkillGetEquipedSkillsIDs(const gameswf::fn_call& call) {
    (void)call;
}

// 0x442838 NativeSkipCharMenuTutorialMessage(gameswf::fn_call const&)
// PARTIAL. No argument is read. If the CharMenuTutorialMsg queue is not empty (the deque object is at global dword_99FBB8,
// and the empty test compares its start and finish cur pointers at 0x99FBB8 and 0x99FBC8), the original pops the front
// (std::deque::pop_front). When CharMenuTutorialMsg::s_SkipFuncName is set, it calls
// MenuMessageManager<CharMenuTutorialMsg, 1>::Invoke with that name. Missing: the instance of
// MenuMessageManager<CharMenuTutorialMsg, 1> (Singleton<T>::s_inst is not declared in the tree, so the deque at
// dword_99FBB8 cannot be tied to m_queues[0]), CharMenuTutorialMsg::s_SkipFuncName (not declared in
// dh2/CharMenuTutorialMsg.hpp), and the queue index that Invoke takes (the pseudocode passes only the name). This native
// uses no gameswf value, so it is the first candidate to finish once the singleton and the statics land.
void NativeSkipCharMenuTutorialMessage(const gameswf::fn_call& call) {
    (void)call;
}

// 0x442994 NativeSkipAchievementMessage(gameswf::fn_call const&)
// PARTIAL. No argument is read. If the AchievementMsg queue is not empty (the deque object is at global dword_99FBE4, the
// empty test compares dword_99FBF4 with it), the original pops the front. It calls
// MenuMessageManager<AchievementMsg, 1>::Invoke on AchievementMsg::s_SkipFuncName when that is set. Then, if the queue is
// still not empty, it calls the same Invoke on AchievementMsg::s_StartFuncName. Missing: the Singleton accessor for
// MenuMessageManager<AchievementMsg, 1> (see the native above), the two function pointers (not declared in
// dh2/AchievementMsg.hpp), and the queue index for Invoke. No gameswf value is used.
void NativeSkipAchievementMessage(const gameswf::fn_call& call) {
    (void)call;
}

// 0x442ea4 NativeStopMessage(gameswf::fn_call const&)
// PARTIAL. Needs at least one argument, and the top argument must have tag 3 or 4. Its to_xstring is the message kind.
// The original constructs five function-local statics ("status", "tutorial", "dialog", "quest", "online"). The status and
// tutorial kinds are compared with the global buffers dword_9A5B90 and dword_9A5BAC instead, and dialog, quest and online
// with the statics. Then PlayerManager::GetLocalPlayer(0, false) is called and its result is not used.
//   status: when the StatusMsg deque is not empty, pop the front, call MenuMessageManager<StatusMsg, 4>::Invoke on
//     StatusMsg::s_SkipFuncName when set, and call it on StatusMsg::s_StartFuncName[0] when the queue is still not empty.
//   tutorial: when the TutorialMsg deque is not empty, advance its front by one 8-byte element. The deque uses 128-byte
//     nodes, so at the end of a node the original frees it and moves to the next one. Then it calls the skip function if
//     TutorialMsg::s_SkipFuncName[0] is set, and the start function if the queue is still not empty.
//   dialog: when GetOnline()+5 (COnline::isOnlineGame_) is set, PlayerManager::IsLocalPlayerHosting() must be non-zero,
//     or the native returns without skipping. Otherwise it creates a CMessage with CMessage::CreateMessage("CMsgScriptCmd",
//     true), sets its words 20 = 3, 21 = -1 and 22 = -1, and sends it through the CMessaging instance's SendMsg. Online or
//     offline, dialog then calls MenuMessageManager<DialogMsg, 1>::SkipEnqueuedMessage (queue and flag not in the
//     pseudocode).
//   quest: calls MenuMessageManager<DialogMsg, 1>::SkipEnqueuedMessage.
//   online: when the OnlineStatusMsg deque is not empty, pop the front, call OnlineStatusMsg::s_SkipFuncName when set, and
//     call the start function when the queue is still not empty. Other kinds do nothing.
// Missing: the argument accessors and to_xstring; the global buffers dword_9A5B90 and dword_9A5BAC (contents not in the
// tree); the StatusMsg, TutorialMsg and OnlineStatusMsg queues and their headers; the Singleton accessor and the queue
// and flag arguments of MenuMessageManager; COnline::isOnlineGame_ (no getter). IsLocalPlayerHosting is declared int in
// dh2/PlayerManager.hpp, but the original passes its result to CMessaging::Get as a pointer, so that return type needs
// checking. The header has CMessaging::GetInstance(), not CMessaging::Get, so the name also needs checking.
void NativeStopMessage(const gameswf::fn_call& call) {
    (void)call;
}

// 0x44344c NativeGetPlayerRanking(gameswf::fn_call const&)
// PARTIAL. The output object is the top argument when it has tag 5 (cast_to<as_object>). The player index is to_number of
// the argument below it. For each of the seven stats 0..6, the original sets a property on the object. The property name
// is PlayerStatManager::GetStatStr(stat), and the number is PlayerStatManager::GetStatValue(stat, player) on the
// PlayerStatManager singleton. The temporary name is released after each stat. The result is the object (set_as_object).
// Missing: the argument accessors, the as_object property setter (vtable slot +28, index 7), tu_string, the value setters,
// set_as_object and drop_refs.
void NativeGetPlayerRanking(const gameswf::fn_call& call) {
    (void)call;
}

// 0x4435e4 NativeGetWorldMapLocations(gameswf::fn_call const&)
// PARTIAL. The output array is the top argument when it has tag 5. The player index is to_number of the argument below it.
// The player's Character comes from NativeGetPlayerChar(index, false). When it exists and Arrays::FastTravelList has
// members, the original loops over the members (stride 28). For each member it creates a new as_object bound to the
// call's gameswf player, or to no player. The cached player is the word at env +0x68, and the block it is held in is at
// env +0x64 (env is the object that fn_call +0x0c points to, not the fn_call itself). Before each object, when env +0x68
// is not zero and the block's byte at +4 is zero, the original decrements the block's count word at +0 (freeing the block
// at zero) and clears env +0x64 and +0x68. The new object then gets no player. Otherwise it is bound to the env +0x68
// player (none when that word is zero). It sets:
// Name = StringManager::getString(member +24); Importance = member +20; LevelName = member +16 (a string);
// EntryPointId = member +8; Description = StringManager::getString(member +4); Visible = Character::SG_GetFastTravelIdUnlocked(
// character, index, -1). The object is then pushed onto the array. The result is the array. Missing: the argument
// accessors, the as_object constructor and property setter, tu_string, add_ref and push, and set_as_object.
void NativeGetWorldMapLocations(const gameswf::fn_call& call) {
    (void)call;
}

// 0x443b18 NativeGetQuestIDsInRange(gameswf::fn_call const&)
// PARTIAL. Arguments from the top: the output array (tag 5), the player index (number, index-1), the first quest index
// (number, index-2, clamped to 0), the maximum count (number, index-3) and the state name (tu_string, index-4). The
// character comes from NativeGetPlayerChar(index, false). The quest filter is GetQuestFunctor(state name), declared in
// dh2/global/native_stats_inventory_merchant.hpp. The original's count is Character::SG_GetNumQuests(filter, -1), capped at
// the maximum. The first quest is Character::SG_GetQuestByID(filter, start, -1). Each next quest is
// Character::SG_GetNextQuest(filter, quest id + 1, -1), and the chain stops at the count or when no quest is left. The
// collected quests are sorted by title (BubbleSortByTitle in the header, with Quest::GetTitle). Each one becomes an object
// with QuestTitle (GetTitle), QuestID (the quest id word at +8) and IsCurrent (Character::SG_GetCurrentQuest(-1) equals the
// id). The object is pushed onto the array. The result is the number of objects pushed, and 0 when there is no character.
// Missing: the argument accessors (to_number, to_tu_string), and the as_array, as_object, tu_string and value setters.
void NativeGetQuestIDsInRange(const gameswf::fn_call& call) {
    (void)call;
}

// 0x444168 NativeMerchantGetItemsListForSlot(gameswf::fn_call const&)
// PARTIAL. Arguments from the top: the equipment slot (number), the output array (tag 5), the player index (number), the
// merchant name (to_string) and a number that the object lookup takes. The character comes from NativeGetPlayerChar(index,
// false). The merchant is looked up with ObjectManager::GetObjectByName (dh2/ObjectManager.hpp) on the ObjectManager global
// (0x99f764) and turned into a Character. The pseudocode passes the number as the flags argument and null as the type, and
// does not show the active argument, so that call needs the assembly. When both checks Character::IsMerchant pass: if the
// slot equals PyDataConstants::getConstant("EquipmentSlots", "Count"), the list is ItemInventory::GetValuableItems on the
// merchant's inventory (+0x37c). Otherwise it is ItemInventory::GetItemListForSlot(inventory, slot). The list is then
// sorted (an introsort plus an insertion sort on ItemInventory::Item, 12-byte elements). The comparator is not identified
// in the tree. The insertion step calls II_Item_sortbybuyback. Each item becomes an object with: ItemName =
// "<font color='#%000000X'>%s</font>" (ItemInstance::GetColor() and the item's name at +28); ItemIndex = the element's
// third word (+8); ItemEquippable = valuable mode, or ItemInstance::IsEquippableBy(character); ItemEquipped = not valuable
// mode and the item equals ItemInventory::GetEquippedItem(inventory, slot); ItemBuyback = ItemInstance +0x69 (unk_69, which
// Track A calls buyback, unconfirmed); ItemQuantity = the quantity word (+0x50, int16) as decimal in valuable mode,
// otherwise a single space. The objects are pushed onto the array, and the result is the array. Missing: the argument
// accessors and setters, the sort comparator, the active argument of GetObjectByName, and confirmation of the literal in
// .rodata. Hex-Rays prints the format as %000000X (zero flags, no width, the same as %X).
void NativeMerchantGetItemsListForSlot(const gameswf::fn_call& call) {
    (void)call;
}

// 0x444990 NativeBackToHud(gameswf::fn_call const&)
// PARTIAL. No argument is read. When Application::GetCurrentLevel() exists, its m_isInGameView (+0x198) is set to true.
// Unless the global byte at 0x9a600c is set, the HUD gets RenderFX "DisplayRightHud" with no arguments and
// "FillActionIcon" with one number, MenuManager::GetInstance()->m_actionIconType (+0x108). The original then calls
// ZoomHandler::ResetFingerMap on the ZoomHandler global (0x99f77c), MultiMenuManager::ResetUpdateCursor on
// MenuManager::m_multiMenuManager (+0xf4), and HUDControls::Touch on HUDControls::GetInstance(). When the local player's
// character exists (PlayerInfo +0x660), it calls ItemInventory::TrophyCheckArmorSet on that character's inventory
// (+0x37c). Missing: the HUD callbacks (RenderFX::InvokeASCallback and RenderFX::GetRoot, not declared in
// dh2/RenderFX.hpp), MultiMenuManager::ResetUpdateCursor (not declared in dh2/MenuManager.hpp), and the name of the flag
// at 0x9a600c. The HUDControls part is declared and can be written with the rest.
void NativeBackToHud(const gameswf::fn_call& call) {
    (void)call;
}

// 0x444ae0 NativeLoginInfos(gameswf::fn_call const&)
// PARTIAL. The output object is the top argument when it has tag 5 (cast_to<as_object>). The assembly calls CMatching::Get()
// with no argument, twice. The first result gives the username (C string at +0x6be1, that is +27617), and the second gives the
// password (C string at +0x6bf2, that is +27634). The native sets the object's Username and Password properties (vtable slot
// +28, index 7), then sets the result to the global save_remember (a boolean). Missing: the argument accessors, tu_string,
// the value setters, drop_refs, set_bool, the login fields at +0x6be1 and +0x6bf2 (not declared in
// net-session-online/c_matching_gl_live.hpp), and the global save_remember (not declared in the tree).
void NativeLoginInfos(const gameswf::fn_call& call) {
    (void)call;
}

// 0x444c68 NativeGetMapIcons(gameswf::fn_call const&)
// PARTIAL. The output object is the top argument when it has tag 5 (cast_to<as_object>). The native sets its "length"
// property to the number 6 (the two words 0 and 1075314688 are the double 6.0) and its "Graphic" property to the string
// "Objective", then sets the result to the object. Missing: the as_object property setter (vtable slot +28), tu_string,
// the value setters, set_as_object and drop_refs.
void NativeGetMapIcons(const gameswf::fn_call& call) {
    (void)call;
}

// 0x444dec NativeGetIPodPlaylistName(gameswf::fn_call const&)
// PARTIAL. The top argument is a number, the playlist index (to_number). The name is VoxSoundManager::GetIPodPlaylistName
// on the VoxSoundManager::s_instance global, and the result is set to that name as a tu_string. Missing: to_number,
// set_tu_string, and the declaration of VoxSoundManager::GetIPodPlaylistName (not in
// vox-audio/include/vox-audio/vox/sound_manager.hpp). Hex-Rays prints the first parameter as as_value**. The tree's
// natives use const fn_call& like the others, and the fields read here are the fn_call's own.
void NativeGetIPodPlaylistName(const gameswf::fn_call& call) {
    (void)call;
}

// 0x444ebc NativeGetStringFromSymbol(gameswf::fn_call const&)
// PARTIAL. The symbol is to_string of the top argument. When it is "MENU_ERROR_NO_USERNAME" or "MENU_ERROR_NO_PASSWORD",
// the global Back_key_Glive is set to 1 (not declared in the tree). The key is then read again as a tu_string and passed to
// StringManager::getStringFromSymbol. When that finds a text, StringManager::parse formats it into a std::string, which goes
// through ParsePlayerName (declared in core-platform/include/core-platform/w7_global_rem.hpp) and is set as the result with
// set_tu_string. When it finds nothing, the result is "notfound". Missing: to_string, to_tu_string, set_tu_string and
// drop_refs. The StringManager global (0x99f760) has no accessor in the tree.
void NativeGetStringFromSymbol(const gameswf::fn_call& call) {
    (void)call;
}

// 0x44506c NativeGetStringFromID(gameswf::fn_call const&)
// PARTIAL. The id is to_number of the top argument. StringManager::getString(id) gives the text. When it is found, it is
// formatted with StringManager::parse, passed through ParsePlayerName, and set as the result with set_tu_string. When it is
// not found, the result is "notfound". The steps match NativeGetStringFromSymbol, without the Back_key_Glive flag. Missing:
// to_number, set_tu_string and drop_refs, and the StringManager global (0x99f760) has no accessor in the tree.
void NativeGetStringFromID(const gameswf::fn_call& call) {
    (void)call;
}

// 0x4451ac NativeGetMultiplayerGameRooms(gameswf::fn_call const&)
// PARTIAL. Runs only with exactly one argument, and only when the online state word (+0x14) is not 300. The state is set
// to 102, the same write as NativeSendRoomSearchRequest. With a top argument of tag 5 (cast_to<as_array>), the room list
// comes from the CMatching virtual at vtable +0x90 (slot 36, name not identified) into a std::vector<tRoomInfo> of 968-byte
// records. When the online mode word (+0x34) is not 2, the list is also stored at the online state's +0x50. For each room
// the original creates an object with:
//   name: when the mode is 3 or 4, sprintf of the room's +28 text as the format. Otherwise CRoomAttributes attribute 3 in
//     32 bytes. Then StringManager::formatForMulti.
//   level: CRoomAttributes::GetAttributeInt(room+40, 3).
//   act: the "StrID"/"MENU_ACT" format parsed with attribute 5, then a space and the title of the v2Quest for attribute 6
//     (index 50 when that attribute is negative).
//   friendCount: 0.
//   playerCount: the room's word at +0x20.
//   maxPlayercount: 4.
// The objects are pushed onto the array, and the result is set to true. Missing: the argument accessors and the
// value setters, the gameswf player object, the online state's +0x34 and +0x50 (not declared in online_game_state.hpp),
// the CMatching virtual at +0x90, tRoomInfo (not reviewed here), and the call used for the name. Hex-Rays passes a
// 12-byte argument record, which does not match StringManager::formatForMulti(text, multiline) in dh2/StringManager.hpp.
void NativeGetMultiplayerGameRooms(const gameswf::fn_call& call) {
    (void)call;
}

// 0x445a10 NativeGetSkillDetails(gameswf::fn_call const&)
// PARTIAL. Arguments from the top: the skill index (number), the output object (tag 5), the player index (number) and, with
// four arguments, a flag (to_bool) for NativeGetPlayerChar(index, flag). The character's CharSkill record comes from
// Character::GetCharSkill(index). When the character's level is below the skill's required level (+32), the current text
// is GAMEPLAYMENUS_SKILL_UNLOCK_AT_LEVEL with the required level as its argument, the next text is empty, and SkillUnlocked
// is false. Otherwise: the current text is GAMEPLAYMENUS_NEEDS_SKILL_POINTS when the skill level is 0, and otherwise the
// description word (+48, with the faery offset when flag +24 is set). The next text is GAMEPLAYMENUS_MAX_SKILL_LEVEL when the
// level has reached the difficulty's maximum (CharacterDesign MaxSkillLevelBNormal, CHard or DVeryHard by
// Character::SG_GetGameDifficultyUnlocked). It is GAMEPLAYMENUS_SKILL_MAXIMUM_LEVEL_TRAINING when
// Character::CanIncrementSkill is false. Otherwise it is the training text (+68, with the faery offset). The two argument
// lists come from CharAI::AI_SkillInfo and CharProperties::_GetProperty over the skill's property ids (count +12, ids +16),
// for the current and the next level. SkillUnlocked is true on this path. The object gets nine properties: SkillName
// (+64), SkillDescription (+52), SkillCurrLevel and SkillNextLevel (StringManager::parseEx of the two texts with their
// lists), SkillAssignable (byte +44), SkillIcon (string +60), SkillLevel (Character::SG_GetSkillLevel), SkillAssignedToSlot
// (Character::SG_GetSkillSlot) and SkillUnlocked. Missing: the argument accessors, to_bool, the setters and tu_string,
// drop_refs. Character::GetCharSkill returns const void* in dh2/Character.hpp, so the skill record's fields at +12 .. +68
// cannot be read. The Variant layout used by StringManager::parseEx is not declared (VarArgs is forward-declared only).
void NativeGetSkillDetails(const gameswf::fn_call& call) {
    (void)call;
}
