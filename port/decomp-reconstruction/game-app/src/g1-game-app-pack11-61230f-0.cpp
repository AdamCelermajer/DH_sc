// Batch g1-game-app-pack11-61230f-0 (game-app, nine script commands, 19 ELF functions). Declarations: dh2/Script_<Name>.hpp,
// each derived from ScriptCmdImpl.
//
// What is reconstructed and what is not:
//   - The nine IsBlocking() bodies return false, as the Hex-Rays bodies do. They are reconstructed.
//   - Every Execute body, and Script_StartDialog::Init, are PARTIAL with an empty body. Each one needs pieces that the tree
//     does not declare yet. They are listed here once and repeated in the body comments:
//       (a) The argument record behind ScriptCmdImpl::m_args (+0x0c). It is the polymorphic object read from the script
//           stream, and no class for it exists in the tree. The fields the commands read are: +0x08 int (StopEffect FX id,
//           StartDialogID dialog list id, StartDialog actor index), +0x0c const char* (SpawnCharacter, SpawnContainer and
//           StopActor object name), +0x0c int (StartDialog style), +0x10 int (StartDialog text id). The command headers
//           record the same offsets.
//       (b) The singletons. Each message manager is a static object (size 44) whose deque is m_queues[0] at +0x04. The tree has
//           no Singleton<T> accessor for them: Singleton<MenuMessageManager<TutorialMsg,1>>::s_inst at 0x99fb5c (deque
//           0x99fb60), Singleton<MenuMessageManager<DialogMsg,1>>::s_inst at 0x99fb88 (deque 0x99fb8c),
//           Singleton<MenuMessageManager<CharMenuTutorialMsg,1>>::s_inst at 0x99fbb4 (deque 0x99fbb8),
//           Singleton<VisualFXManager>::s_inst at 0x9a1cd4, Singleton<Application>::s_inst at 0x99f72c (its PyDataConstants*
//           at +0x2c is the global at 0x99f758), and the ObjectManager pointer global at 0x99f764.
//       (c) Headers that do not exist yet: dh2/TutorialMsg.hpp and dh2/VisualFXManager.hpp.
//   - MenuMessageManager calls: the Hex-Rays calls show no queue argument. The assembly calls the clones of EnqueueMessage,
//     FlushEnqueuedMessages, Invoke and SkipEnqueuedMessage with the queue index folded in. Each manager has one queue, so
//     the index is 0. The clone of EnqueueMessage still takes one more argument: 1 at the StartDialog call site and 0 at
//     the StartDialogID one. Whether that argument is the queue or the flag, against the (message, queue, flag) declaration
//     in dh2/MenuMessageManager.hpp, is not settled.
//   - Shared prefix: most Execute bodies start with DebugSwitches::s_inst.load() and then GetSwitch("isTracingScriptCmd"),
//     with the result discarded. It is not called here, because each body is a shell until the argument record and the
//     singletons land.
//   - Hex-Rays gives some Execute bodies an int or pointer result. The slots are void, so that value is not part of the
//     contract.
//   - The skipping argument: SpawnCharacter, SpawnContainer, StopActor and StopEffect ignore it, and their work runs
//     whatever its value. StartDialog and StartDialogID do nothing when it is set. SkipAllTutorialMessages,
//     SkipCharMenuTutorialMessage and SkipTutorialMessage ignore it.

#include "dh2/Script_SkipAllTutorialMessages.hpp"
#include "dh2/Script_SkipCharMenuTutorialMessage.hpp"
#include "dh2/Script_SkipTutorialMessage.hpp"
#include "dh2/Script_SpawnCharacter.hpp"
#include "dh2/Script_SpawnContainer.hpp"
#include "dh2/Script_StartDialog.hpp"
#include "dh2/Script_StartDialogID.hpp"
#include "dh2/Script_StopActor.hpp"
#include "dh2/Script_StopEffect.hpp"

// 0x455948 Script_SkipAllTutorialMessages::IsBlocking() const
bool Script_SkipAllTutorialMessages::IsBlocking() const {
    return false;
}

// 0x46033c Script_SkipAllTutorialMessages::Execute(bool, int)
// PARTIAL, empty body. The original (1) advances the start iterator of the TutorialMsg queue (m_queues[0] of
// Singleton<MenuMessageManager<TutorialMsg,1>>, deque at 0x99fb60, finish cur at 0x99fb70) one 8-byte element at a time until
// it reaches the finish cur. Each 128-byte node it leaves behind goes back to the node allocator (_M_deallocate, 0x80 bytes).
// The queue ends empty, which is std::deque::clear() for this element size. (2) Then it calls
// MenuMessageManager<TutorialMsg,1>::SkipEnqueuedMessage. Missing: the TutorialMsg singleton accessor (s_inst at 0x99fb5c),
// and dh2/TutorialMsg.hpp, which does not exist yet (waits on dh2/TutorialMsg.hpp), so the deque cannot be instantiated.
void Script_SkipAllTutorialMessages::Execute(bool skipping, int module) {
    (void)skipping;
    (void)module;
}

// 0x455958 Script_SkipCharMenuTutorialMessage::IsBlocking() const
bool Script_SkipCharMenuTutorialMessage::IsBlocking() const {
    return false;
}

// 0x4603d0 Script_SkipCharMenuTutorialMessage::Execute(bool, int)
// PARTIAL, empty body. When the CharMenuTutorialMsg queue is not empty (start cur at 0x99fbb8 against finish cur at 0x99fbc8),
// the original pops its front. Then, when CharMenuTutorialMsg::s_SkipFuncName (0x9a54b4) is set, it calls
// MenuMessageManager<CharMenuTutorialMsg,1>::Invoke with that name, queue 0. Missing: the singleton accessor for the queue
// (s_inst at 0x99fbb4, deque at 0x99fbb8), and CharMenuTutorialMsg::s_SkipFuncName, which dh2/CharMenuTutorialMsg.hpp does not
// declare (layout request). The Hex-Rays result (int*) is the address of the singleton, not a value the void slot uses.
void Script_SkipCharMenuTutorialMessage::Execute(bool skipping, int module) {
    (void)skipping;
    (void)module;
}

// 0x455940 Script_SkipTutorialMessage::IsBlocking() const
bool Script_SkipTutorialMessage::IsBlocking() const {
    return false;
}

// 0x460338 Script_SkipTutorialMessage::Execute(bool, int)
// PARTIAL, empty body. The original is a 4-byte tail branch (assembly 0x460338, B to the clone of
// MenuMessageManager<TutorialMsg,1>::SkipEnqueuedMessage(int, bool), .clone.30). A tail branch passes r0, r1 and r2 unchanged,
// so the clone receives the command pointer in r0 and the skipping flag in r1 as its (int, bool) arguments. That is not a
// queue index and a flag, so the thunk's intent is unresolved. The forward also needs the TutorialMsg singleton accessor
// (s_inst at 0x99fb5c), which the tree does not declare. Waits on dh2/TutorialMsg.hpp.
void Script_SkipTutorialMessage::Execute(bool skipping, int module) {
    (void)skipping;
    (void)module;
}

// 0x455784 Script_SpawnCharacter::IsBlocking() const
bool Script_SpawnCharacter::IsBlocking() const {
    return false;
}

// 0x45f400 Script_SpawnCharacter::Execute(bool, int)
// PARTIAL, empty body. The original (1) runs the DebugSwitches prefix (see the file comment); (2) reads the object name from
// the argument record (const char* at +0x0c); (3) calls ObjectManager::GetObjectByName(name, module, false, null) on the
// ObjectManager pointer at global 0x99f764, which the tree does not declare. The Hex-Rays list shows the module as the flags
// argument and no type argument, so the type argument of this call needs checking against the assembly. (4) It converts the
// handle to Character* and, when that is non-null, calls CharStateMachine::SM_SetSpawnState(false, false) on the character's
// CharStateMachine base (+0x4fc). The skipping flag is not checked. Missing: the argument record class and the ObjectManager
// accessor (layout requests).
void Script_SpawnCharacter::Execute(bool skipping, int module) {
    (void)skipping;
    (void)module;
}

// 0x4558dc Script_SpawnContainer::IsBlocking() const
bool Script_SpawnContainer::IsBlocking() const {
    return false;
}

// 0x45d008 Script_SpawnContainer::Execute(bool, int)
// PARTIAL, empty body. The original (1) runs the DebugSwitches prefix; (2) reads the object name from the argument record
// (const char* at +0x0c); (3) calls ObjectManager::GetObjectByName(name, module, false, null) on the ObjectManager pointer at
// global 0x99f764 (the Hex-Rays list has the name, the module, a null and a final null, one more argument than the
// SpawnCharacter list); (4) converts the handle to Container* and calls Container::Spawn() when it is non-null. The skipping flag
// is not checked. The Hex-Rays int* result is not used by the void slot. Missing: the argument record class and the
// ObjectManager accessor.
void Script_SpawnContainer::Execute(bool skipping, int module) {
    (void)skipping;
    (void)module;
}

// 0x45568c Script_StartDialog::IsBlocking() const
bool Script_StartDialog::IsBlocking() const {
    return false;
}

// 0x459034 Script_StartDialog::Init()
// PARTIAL, empty body. The original sets the base flag unk_04 (+0x04) to (the argument record's int at +0x0c, the
// DialogStyles id) == PyDataConstants::getConstant("DialogStyles", "EnterLocationDialog"). The constant is read through the
// PyDataConstants pointer at Singleton<Application>::s_inst +0x2c (global 0x99f758). PyDataConstants::getConstant is declared
// in pydata-accessors/include/pydata-accessors/w7_pydata12.hpp. Missing: the argument record (+0x0c int) and the Application
// accessor. The original returns the flag as bool. The void slot does not return it, and unk_04 is the only result.
void Script_StartDialog::Init() {
}

// 0x460fe8 Script_StartDialog::Execute(bool, int)
// PARTIAL, empty body. When skipping is false the original (1) runs the DebugSwitches prefix (see the file comment); (2) when
// the style (argument record +0x0c) differs from the EnterLocationDialog constant (see Init), or when the DialogMsg queue is
// empty (finish cur at 0x99fb9c equals start cur at 0x99fb8c), builds a DialogMsg with the id constructor
// DialogMsg(0, record +0x10, record +0x0c, record +0x08) and enqueues it through EnqueueMessage (assembly: the clone is
// called with R1 = 1), then destroys it. (3) Otherwise it calls FlushEnqueuedMessages (queue 0). Missing: the argument
// record; the DialogMsg queue singleton (s_inst at 0x99fb88, deque at 0x99fb8c, no accessor); PyDataConstants access (see
// Init); and the mapping of the clone's second register argument onto dh2/MenuMessageManager.hpp (EnqueueMessage is declared
// with (message, queue, flag)).
void Script_StartDialog::Execute(bool skipping, int module) {
    (void)skipping;
    (void)module;
}

// 0x455694 Script_StartDialogID::IsBlocking() const
bool Script_StartDialogID::IsBlocking() const {
    return false;
}

// 0x461114 Script_StartDialogID::Execute(bool, int)
// PARTIAL, empty body. When skipping is false the original (1) runs the DebugSwitches prefix (see the file comment); (2)
// reads the dialog list id (int at argument record +0x08) and calls GetDialogMsgListById(id, list) (declared in
// dh2/global/app_helpers.hpp); (3) enqueues each DialogMsg of the list (76-byte elements, begin to end) through EnqueueMessage
// (assembly: the clone is called with R1 = 0), as in StartDialog; (4) when the queue then holds exactly one message (finish cur
// minus start cur, through std::priv::_Deque_iterator_base<DialogMsg>::_M_subtract on 0x99fb8c and 0x99fb9c, equals 1), it
// calls Invoke(DialogMsg::s_StartFuncName[0], queue 0) (the name is "StartDialog"; the statics are at 0x999830); (5) it
// destroys the vector. Missing: the argument record; the DialogMsg queue singleton (s_inst at 0x99fb88); the EnqueueMessage
// argument mapping (see StartDialog); and DialogMsg::s_StartFuncName, which dh2/DialogMsg.hpp does not declare (layout
// request).
void Script_StartDialogID::Execute(bool skipping, int module) {
    (void)skipping;
    (void)module;
}

// 0x4557e0 Script_StopActor::IsBlocking() const
bool Script_StopActor::IsBlocking() const {
    return false;
}

// 0x45eec0 Script_StopActor::Execute(bool, int)
// PARTIAL, empty body. The original (1) runs the DebugSwitches prefix (see the file comment); (2) reads the actor name (const
// char* at argument record +0x0c). When strcasecmp(name, "All") is not zero, (3a) it calls ObjectManager::GetObjectByName
// (name, module, false, null) on the ObjectManager pointer at global 0x99f764, calls GetObject(false), converts to Character*,
// and, when that is non-null, sets the controller flag at v2Controller::unk_009 (+0x09) to 1 around v2Controller::Cmd_Stop()
// (controller is v2Controllable::m_controller, Character +0x378). When the name is "All", (3b) it walks every entry of the
// ObjectManager object map (std::map<int, ObjectListItem> at ObjectManager +0x0c, in key order), and for each entry whose flag
// word at node +0x2c is set it gets the object's handle (ObjectBase::GetHandle) and applies the same stop to its Character.
// Missing: the argument record; the ObjectManager accessor; the map node layout for the walk (dh2/ObjectManager.hpp only
// forward-declares ObjectListItem, so the walk is not written).
void Script_StopActor::Execute(bool skipping, int module) {
    (void)skipping;
    (void)module;
}

// 0x4556b4 Script_StopEffect::IsBlocking() const
bool Script_StopEffect::IsBlocking() const {
    return false;
}

// 0x459890 Script_StopEffect::Execute(bool, int)
// PARTIAL, empty body. The original calls VisualFXManager::DropAnimatedFXSetByID(int) (0x494660) on
// &Singleton<VisualFXManager>::s_inst (0x9a1cd4), with the FX set id read from the argument record (int at +0x08). The
// assembly at 0x459890 is a tail branch to that function. Missing: dh2/VisualFXManager.hpp does not exist yet (waits on
// dh2/VisualFXManager.hpp), the singleton accessor, and the argument record. The skipping flag is not checked.
void Script_StopEffect::Execute(bool skipping, int module) {
    (void)skipping;
    (void)module;
}
