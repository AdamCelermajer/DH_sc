// Batch g1-game-ui-MenuMessageManager-4d4518-1 (part 2 of 2): MenuMessageManager<T, N> (dh2/MenuMessageManager.hpp), the
// Invoke clones (callback by name on the HUD root), SkipEnqueuedMessage and EnqueueMessage for DialogMsg. Part 1
// (g1-game-ui-MenuMessageManager-4d4518-0.cpp) holds the destructors, FlushEnqueuedMessages and GetNextEnqueuedMessage.
//
// Facts shared by these bodies (asm checked at 0x442734, Hex-Rays for the rest):
//   - The clones drop their `this` and their queue argument. The DialogMsg bodies use m_queues[0], the only queue of
//     MenuMessageManager<DialogMsg, 1>. The clone with a constant flag (0x480b2c) is the flag-true path of the
//     EnqueueMessage body.
//   - Invoke takes the callback name as its first argument (the Hex-Rays "MenuManager *" is that name, not an object).
//     MenuManager::GetInstance() takes no argument.
//   - The cache of a message type is a DebugCachedCharacter (0x30 bytes) named T::s_NodeCachedChar. Its weak proxy is the
//     smart_ptr_proxy at +0x28 and the cached pointer is at +0x2c (the asm reads [R3, #0x28] and [R3, #0x2c]). The name
//     is T::s_NodeName, one 4-byte pointer. The start and skip names are T::s_StartFuncName and T::s_SkipFuncName.
//   - ELF addresses: DialogMsg s_NodeCachedChar 0x9a5448, s_NodeName 0x99982c, s_StartFuncName 0x999830 (4 bytes),
//     s_SkipFuncName 0x9a5478. CharMenuTutorialMsg s_NodeCachedChar 0x9a547c, s_NodeName 0x9a54ac. AchievementMsg
//     s_NodeCachedChar 0x9a54b8, s_NodeName 0x999834, s_StartFuncName 0x999838. TutorialMsg s_NodeCachedChar 0x9a5418,
//     s_NodeName 0x999820. StatusMsg s_NodeCachedChar 0x9a53b0, s_NodeName 0x999810. OnlineStatusMsg s_NodeCachedChar
//     0x9a53e4, s_NodeName 0x999818.
//
// Status of the Invoke clones: PARTIAL. The callback needs the cache object and the name of each message type, and no
// header declares them: dh2/DialogMsg.hpp, dh2/CharMenuTutorialMsg.hpp and dh2/AchievementMsg.hpp declare neither
// s_NodeCachedChar nor s_NodeName (layout requests). dh2/TutorialMsg.hpp, dh2/StatusMsg.hpp and dh2/OnlineStatusMsg.hpp
// do not exist yet. So Invoke below is one generic template that finds the HUD root and returns when there is none. The
// rest of the callback is written out in the comment inside it, and it is not run yet. Every Invoke clone is partial.
//
// Not written here: SkipEnqueuedMessage for TutorialMsg (0x46026c) and the Invoke clones for TutorialMsg, StatusMsg and
// OnlineStatusMsg, which wait on their headers.

#include "dh2/MenuMessageManager.hpp"

#include <deque>

#include "dh2/AchievementMsg.hpp"
#include "dh2/CharMenuTutorialMsg.hpp"
#include "dh2/DialogMsg.hpp"
#include "dh2/MenuManager.hpp"

// 0x442734 MenuMessageManager<CharMenuTutorialMsg, 1>::Invoke(char const*, int) const  (clone, PARTIAL)
// 0x45a0fc MenuMessageManager<CharMenuTutorialMsg, 1>::Invoke(char const*, int) const  (clone, PARTIAL)
// 0x442890 MenuMessageManager<AchievementMsg, 1>::Invoke(char const*, int) const        (clone, PARTIAL)
// 0x442a14 MenuMessageManager<TutorialMsg, 1>::Invoke(char const*, int) const           (clone, PARTIAL, waits on TutorialMsg.hpp)
// 0x45a030 MenuMessageManager<TutorialMsg, 1>::Invoke(char const*, int) const           (clone, PARTIAL, waits on TutorialMsg.hpp)
// 0x442b18 MenuMessageManager<DialogMsg, 1>::Invoke(char const*, int) const             (clone, PARTIAL)
// 0x459f64 MenuMessageManager<DialogMsg, 1>::Invoke(char const*, int) const             (clone, PARTIAL)
// 0x49b858 MenuMessageManager<DialogMsg, 1>::Invoke(char const*, int) const             (clone, PARTIAL)
// 0x442c9c MenuMessageManager<StatusMsg, 4>::Invoke(char const*, int) const             (clone, PARTIAL, waits on StatusMsg.hpp)
// 0x442da0 MenuMessageManager<OnlineStatusMsg, 1>::Invoke(char const*, int) const       (clone, PARTIAL, waits on OnlineStatusMsg.hpp)
//
// Original: GetInstance, then GetHUDRoot; returns when the root is null. Otherwise, when the cache pointer (+0x2c of
// T::s_NodeCachedChar) is set and its proxy is alive, the cached character is used. A set pointer with a dead proxy
// drops the proxy (freed at zero) and clears the cache, and an empty cache is refreshed by
// DebugCachedCharacter::RefreshCache(T::s_NodeName, hud, nullptr). Then GetChar, and
// RenderFX::InvokeASCallback(hud, char, functionName, &arg, 1) with one NUMBER argument holding 0.0, and drop_refs.
// The clones do not read the queue argument.
template <typename T, int N>
void MenuMessageManager<T, N>::Invoke(const char* functionName, int queue) const {
    (void)functionName;
    (void)queue;
    MenuFX* hud = MenuManager::GetInstance()->GetHUDRoot();
    if (hud == nullptr) {
        return;
    }
    // PARTIAL: the cache step and the callback are not run. They read T::s_NodeCachedChar and T::s_NodeName, which
    // dh2/<Class>.hpp does not declare for any message type yet (layout requests). Once they are declared, the missing
    // part is the proxy check on the cache, RefreshCache(T::s_NodeName, hud, nullptr) when the cache is empty, GetChar
    // and the InvokeASCallback call with the 0.0 argument.
}

// Explicit instantiations for the message types whose header exists. Each line names the clones it covers. The
// TutorialMsg, StatusMsg and OnlineStatusMsg instances wait on their headers and are not instantiated here.
template void MenuMessageManager<AchievementMsg, 1>::Invoke(const char*, int) const;       // 0x442890
template void MenuMessageManager<CharMenuTutorialMsg, 1>::Invoke(const char*, int) const;  // 0x442734, 0x45a0fc
template void MenuMessageManager<DialogMsg, 1>::Invoke(const char*, int) const;            // 0x442b18, 0x459f64, 0x49b858

// 0x442c1c MenuMessageManager<DialogMsg, 1>::SkipEnqueuedMessage(int, bool)  (clone, PARTIAL)
// The clone reads neither the queue argument nor the flag, so both are dropped here: queue 0 is the only queue, and the
// flag's effect on the other callers is not recovered. When queue 0 is not empty, the original pops its front. It then
// invokes DialogMsg::s_SkipFuncName when that is set, and when the queue is still not empty it invokes
// DialogMsg::s_StartFuncName[0]. Those calls go through Invoke, which is PARTIAL, so the callbacks are not run yet.
template <>
void MenuMessageManager<DialogMsg, 1>::SkipEnqueuedMessage(int queue, bool flag) {
    (void)queue;
    (void)flag;
    std::deque<DialogMsg>& messages = m_queues[0];
    if (messages.empty()) {
        return;
    }
    messages.pop_front();
    if (DialogMsg::s_SkipFuncName != nullptr) {
        Invoke(DialogMsg::s_SkipFuncName, 0);
    }
    if (!messages.empty()) {
        Invoke(DialogMsg::s_StartFuncName[0], 0);
    }
}

// 0x460dec MenuMessageManager<DialogMsg, 1>::EnqueueMessage(DialogMsg const&, int, bool)  (clone, PARTIAL)
// 0x480b2c MenuMessageManager<DialogMsg, 1>::EnqueueMessage(DialogMsg const&, int, bool)  (clone with flag true, PARTIAL)
// Appends the message to queue 0. When the flag is set and the queue then holds one message, it invokes the start
// function DialogMsg::s_StartFuncName[0]. The clone at 0x480b2c has the flag fixed to true and inlines the same Invoke
// body, so it runs the start step with no flag test. The start step goes through Invoke, which is PARTIAL.
template <>
void MenuMessageManager<DialogMsg, 1>::EnqueueMessage(const DialogMsg& message, int queue, bool flag) {
    (void)queue;
    std::deque<DialogMsg>& messages = m_queues[0];
    messages.push_back(message);
    if (flag && messages.size() == 1) {
        Invoke(DialogMsg::s_StartFuncName[0], 0);
    }
}

// 0x460474 MenuMessageManager<DialogMsg, 1>::FlushEnqueuedMessages(int)  (clone, reconstructed)
// The body is the generic FlushEnqueuedMessages template of g1-game-ui-MenuMessageManager-4d4518-0.cpp, which has an
// explicit instantiation for DialogMsg, 1 there. The clone pops the front until the queue is empty, the same loop.
// Not defined again here, so the symbol is defined once.
