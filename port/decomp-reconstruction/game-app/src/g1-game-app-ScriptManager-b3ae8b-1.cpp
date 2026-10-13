// Batch g1-game-app-ScriptManager-b3ae8b-1 (ScriptManager, part 2 of 2): ScriptManager::StartScript(int, int, bool).
// Rebuilt from the Hex-Rays pseudocode of libDungeonHunter2.so and checked against the assembly at 0x4605c0 for the call
// targets, the argument order and the branch structure. Member offsets are the original 32-bit ones (see
// dh2/ScriptManager.hpp).
#include "dh2/ScriptManager.hpp"

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>

#include "dh2/DebugSwitches.hpp"
#include "dh2/PlayerManager.hpp"
#include "dh2/global/IsMultiplayerGame.hpp"  // dword_99F76C, the PlayerManager global
#include "dh2/global/net_singletons.hpp"     // GetOnline()
#include "net-session-online/c_message.hpp"
#include "net-session-online/c_messaging.hpp"

// Test seam for the stubbed online flag below. A test sets it to run the online branches. Remove it together with the
// stub when the COnline getter exists.
bool StartScriptOnlineFlagForTest = false;

namespace {

// PARTIAL stub for *(GetOnline() + 0x05), the COnline::isOnlineGame_ byte. The field is private in
// net-session-online/include/net-session-online/c_online.hpp and has no getter, so the byte cannot be read yet. The
// original calls GetOnline() for this read, and that call is kept. Layout request: a public getter for COnline +0x05.
bool IsOnlineGameStub() {
    static_cast<void>(GetOnline());
    return StartScriptOnlineFlagForTest;
}

// The CMsgScriptCmd send of the original: CMessaging::Get() (0x80b1bc, a branch to CMessaging::GetInstance), then
// CMessage::CreateMessage("CMsgScriptCmd", true), three stores into the message (+0x50 = 0, +0x54 = script id, +0x58 =
// module), then CMessaging::SendMsg. CMsgScriptCmd keeps its 12-byte payload at +0x50 and returns it from GetDataPtr(),
// so the three words are copied there, each at its own offset.
void SendStartScriptCmd(std::int32_t scriptId, std::int32_t module) {
    CMessaging* messaging = CMessaging::GetInstance();
    CMessage* message = CMessage::CreateMessage("CMsgScriptCmd", true);
    const std::int32_t payloadWords[3] = {0, scriptId, module};
    std::memcpy(message->GetDataPtr(), payloadWords, sizeof payloadWords);
    messaging->SendMsg(message);
}

}  // namespace

// 0x4605c0 ScriptManager::StartScript(int, int, bool)
// PARTIAL. The online-game flag is stubbed to false (IsOnlineGameStub), so while the stub is in place the online branches
// are not taken: the CMsgScriptCmd send for a local start, and the already-running check for a remote start. Both are
// written as in the original and take effect once the flag can be read.
void ScriptManager::StartScript(int id, int module, bool fromRemote) {
    if (id < 0 || static_cast<std::size_t>(id) >= m_contexts.size()) {
        return;
    }

    // A start is allowed when some player is alive, or when the online game is past its first death step
    // (PlayerManager +0x714, m_globalDeathState > 1).
    PlayerManager* players = dword_99F76C;
    if (players->AllPlayersDead() != 0 && !(IsOnlineGameStub() && players->m_globalDeathState > 1)) {
        return;
    }

    // The original calls DebugSwitches::load() on DebugSwitches::s_inst, then GetSwitch("SkipAllScripts"). GetInstance()
    // runs the same load and returns the singleton. s_inst itself is private, so it is reached through GetInstance().
    const bool skipAllScripts = DebugSwitches::GetInstance()->GetSwitch(std::string("SkipAllScripts"));
    if (skipAllScripts && m_skipScript == -1) {
        SkipScript(id, false);
    }

    // Online game: a local start sends the command to the other players unless the command block already has its +0x04
    // flag (ScriptCmds::unk_04). A remote start goes ahead only when the script is not already running.
    bool startNow = true;
    if (IsOnlineGameStub()) {
        if (!fromRemote) {
            if (!m_cmds[static_cast<std::size_t>(id)].unk_04) {
                SendStartScriptCmd(id, module);
            }
        } else {
            startNow = !IsScriptRunning(id);
        }
    }
    if (!startNow) {
        return;
    }

    // Same sequence as the skip switch: load, then GetSwitch("isTracingScriptCmd") with the result discarded.
    DebugSwitches::GetInstance()->GetSwitch(std::string("isTracingScriptCmd"));

    ScriptContext& context = m_contexts[static_cast<std::size_t>(id)];
    context.m_module = module;  // +0x00
    context.m_nextCmd = 0;      // +0x04
    context.m_state = 0;        // +0x08, started
    ++m_activeCount;            // ScriptManager +0x04
}
