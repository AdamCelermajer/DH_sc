// Batch g1-game-ui-MenuMessageManager-4d4518-0: MenuMessageManager<T, N> (dh2/MenuMessageManager.hpp), the FIFO queues the
// front-end menus show one message at a time. This batch is the destructors, FlushEnqueuedMessages and
// GetNextEnqueuedMessage. The clones in the binary act on the singleton's queue directly; the member forms below act on
// `this`, which is the same object.
//
// Layout: the vptr at +0x00, then N queues at +0x04, 40 bytes each at 32-bit. Queue q is m_queues[q]. The original
// destructor releases the queues from the last to the first, which is the order C++ uses for the array member, so the
// destructor body is empty.
//
// The bodies that do not depend on the message type are template definitions, written once here. Each instance is
// explicitly instantiated where its message header is complete. An instance needs the message type to be complete,
// because the deque destructor destroys the elements. The instances whose header does not exist yet are listed after
// the explicit instantiations. They are not instantiated, so they have no symbol in this file yet.
//
// Not written here: EnqueueMessage<StatusMsg, 4> (0x3ecff8) and Invoke<DialogMsg, 1> (0x421bf8). Their bodies read
// message statics that no header declares. See the notes at those addresses.

#include "dh2/MenuMessageManager.hpp"

#include <deque>

#include "dh2/AchievementMsg.hpp"
#include "dh2/CharMenuTutorialMsg.hpp"
#include "dh2/DialogMsg.hpp"

// 0x3297a4 / 0x3297f0  MenuMessageManager<StatusMsg, 4>::~MenuMessageManager()        (D1 / D0)
// 0x329930 / 0x32997c  MenuMessageManager<AchievementMsg, 1>::~MenuMessageManager()    (D1 / D0)
// 0x329a54 / 0x329ae4  MenuMessageManager<TutorialMsg, 1>::~MenuMessageManager()       (D1 / D0)
// 0x329c4c / 0x329c98  MenuMessageManager<OnlineStatusMsg, 1>::~MenuMessageManager()   (D1 / D0)
// 0x329e78 / 0x329ec4  MenuMessageManager<CharMenuTutorialMsg, 1>::~MenuMessageManager() (D1 / D0)
// 0x329fe8 / 0x32a034  MenuMessageManager<DialogMsg, 1>::~MenuMessageManager()         (D1 / D0)
// One body for every instance. The D1 (complete-object) destructor has no statements: the member array of deques is
// destroyed from the last queue to the first, as the original loop does, and the vptr is set by the compiler. The D0
// (deleting) variant of the original calls CustomFree (operator delete) after D1; the compiler emits that variant for
// each explicit instantiation below. The TutorialMsg destructor inlines a walk over the element nodes that calls
// nothing for these trivially destructible 8-byte messages, so it has the same effect.
template <typename T, int N>
MenuMessageManager<T, N>::~MenuMessageManager() {}

// 0x383ea8 / 0x3f9064  MenuMessageManager<DialogMsg, 1>::FlushEnqueuedMessages(int)  (clones)
// 0x3f1af4  MenuMessageManager<OnlineStatusMsg, 1>::FlushEnqueuedMessages(int)  (clone)
// 0x3f2274  MenuMessageManager<TutorialMsg, 1>::FlushEnqueuedMessages(int)  (clone)
// 0x3f9018  MenuMessageManager<StatusMsg, 4>::FlushEnqueuedMessages(int)  (clone)
// Removes every message from queue q. The original pops one message at a time until the start and finish iterators
// meet. The TutorialMsg clone inlines that pop_front loop, including the release of emptied nodes. The end state is the
// same: the queue is empty. The StatusMsg clone returns the result register of its last pop_front, which the declared
// void return does not carry.
template <typename T, int N>
void MenuMessageManager<T, N>::FlushEnqueuedMessages(int queue) {
    std::deque<T>& messages = m_queues[queue];
    while (!messages.empty()) {
        messages.pop_front();
    }
}

// 0x43f890  MenuMessageManager<DialogMsg, 1>::GetNextEnqueuedMessage(DialogMsg&, int)  (clone)
// Copies the front message of queue q into message and returns true. Returns false, and leaves message alone, when the
// queue is empty. The message stays queued. The asm compares the finish and start cursors (empty when equal), then
// calls DialogMsg::operator=(message, *start) and returns 1, or returns 0.
template <typename T, int N>
bool MenuMessageManager<T, N>::GetNextEnqueuedMessage(T& message, int queue) {
    const std::deque<T>& messages = m_queues[queue];
    if (messages.empty()) {
        return false;
    }
    message = messages.front();
    return true;
}

// Explicit instantiations for the message types whose header is complete. Each line names the original addresses it
// provides. The explicit instantiation emits the destructor pair (D1 and D0) for each type.
template MenuMessageManager<AchievementMsg, 1>::~MenuMessageManager();         // 0x329930, 0x32997c
template MenuMessageManager<CharMenuTutorialMsg, 1>::~MenuMessageManager();    // 0x329e78, 0x329ec4
template MenuMessageManager<DialogMsg, 1>::~MenuMessageManager();              // 0x329fe8, 0x32a034
template void MenuMessageManager<DialogMsg, 1>::FlushEnqueuedMessages(int);    // 0x383ea8, 0x3f9064
template bool MenuMessageManager<DialogMsg, 1>::GetNextEnqueuedMessage(DialogMsg&, int);  // 0x43f890

// Waiting on message headers. Add the matching explicit instantiations when each header exists:
//   dh2/StatusMsg.hpp (not yet in the tree): ~MenuMessageManager<StatusMsg, 4> 0x3297a4 / 0x3297f0;
//       FlushEnqueuedMessages(int) 0x3f9018
//   dh2/TutorialMsg.hpp (not yet in the tree): ~MenuMessageManager<TutorialMsg, 1> 0x329a54 / 0x329ae4;
//       FlushEnqueuedMessages(int) 0x3f2274
//   dh2/OnlineStatusMsg.hpp (not yet in the tree): ~MenuMessageManager<OnlineStatusMsg, 1> 0x329c4c / 0x329c98;
//       FlushEnqueuedMessages(int) 0x3f1af4

// 0x3ecff8  MenuMessageManager<StatusMsg, 4>::EnqueueMessage(StatusMsg const&, int, bool)  (clone)
// NOT WRITTEN (partial). The clone pushes the message onto queue 0 (the queue index is folded to 0; the flag is not read
// in this clone). If the queue then holds exactly one message and MenuManager::GetInstance()->GetHUDRoot() is non-null,
// it runs the StatusMsg start call on the HUD root and returns nothing when there is no HUD root. The cache step, as the
// Hex-Rays text shows it: if the cached pointer StatusMsg::s_NodeCachedChar +0x2c is set and its weak proxy (+0x28; the
// alive byte is at proxy +4) is alive, the cached character is used as it is. Otherwise, when the pointer is set, the
// proxy is released (count drop, free at zero) and both fields are cleared, and then
// DebugCachedCharacter::RefreshCache(StatusMsg::s_NodeName[0], hud, nullptr) runs. Then
// RenderFX::InvokeASCallback(hud, GetChar(), StatusMsg::s_StartFuncName[0], args, 1) and as_value::drop_refs.
// Missing: dh2/StatusMsg.hpp (the push and the element type need a complete StatusMsg), and the StatusMsg statics
// StatusMsg::s_NodeCachedChar (0x9a53b0, 48 bytes), s_NodeName (0x999810) and s_StartFuncName (0x999814), which no
// header declares. The flag's effect in the non-clone callers is not recovered.

// 0x421bf8  MenuMessageManager<DialogMsg, 1>::Invoke(char const*, int) const  (clone)
// NOT WRITTEN (partial). The clone takes the callback name as its first argument. MenuManager::GetInstance() takes no
// argument, so the Hex-Rays "MenuManager *" for that argument is the name. It finds the HUD root and returns when there
// is none. The cache step: if the cached pointer DialogMsg::s_NodeCachedChar +0x2c is set and its weak proxy (+0x28; the
// alive byte is at proxy +4) is alive, the cached character is used as it is. Otherwise, when the pointer is set, the
// proxy is released and the pointer cleared, and then DebugCachedCharacter::RefreshCache(DialogMsg::s_NodeName[0], hud,
// nullptr) runs. Then it calls
// RenderFX::InvokeASCallback(hud, GetChar(), name, args, 1) with a one-element as_value array, which the Hex-Rays text
// fills with zero words and a byte 2 (the as_value layout is not checked), and releases it with as_value::drop_refs.
// Missing: DialogMsg::s_NodeCachedChar (0x9a5448, DebugCachedCharacter, 48 bytes) and DialogMsg::s_NodeName (0x99982c).
// dh2/DialogMsg.hpp declares neither, so the body is not written.
