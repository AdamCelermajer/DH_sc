// Batch g1-game-app-ScriptManager-b3ae8b-0 (ScriptManager, part 1 of 2). Rebuilt from the Hex-Rays pseudocode
// (libDungeonHunter2.so) and checked against the assembly where the call targets were unclear.
// Member offsets are the original 32-bit ones and are listed in dh2/ScriptManager.hpp.
// ScriptManager::s_inst is defined here, as dh2/ScriptManager.hpp asks ("Defined by the ScriptManager batch").
#include "dh2/ScriptManager.hpp"

#include <cstddef>
#include <cstdint>
#include <string>

#include "core-platform/deps/application.hpp"  // ApplicationOpenStream, ApplicationCloseStream (file system slots +0x88, +0x78)
#include "core-platform/lc_api.hpp"            // LC_API_STRCASECMP (the original's LC_API_STRCASECMP_0 is its tail-call target)
#include "core-platform/memory.hpp"            // CustomFree, MemoryHintState
#include "dh2/DebugSwitches.hpp"
#include "dh2/IStreamBase.hpp"
#include "dh2/ScriptCmdImpl.hpp"
#include "dh2/StreamReader.hpp"
#include "dh2/global/runtime_support.hpp"      // PushProfilingContext, PopProfilingContext, operator new[](size, MemoryHintState)
#include "dh2/v2Controller.hpp"

// Static storage of the original (file-static in its ScriptManager translation unit).
ScriptManager ScriptManager::s_inst;
const char* ScriptManager::s_dbgRunningScriptName = nullptr;

namespace {

// Releases a script file's stream when the loader returns. The original releases it right after its StreamBuffer copy.
class ScriptStreamGuard {
public:
    explicit ScriptStreamGuard(IStreamBase* stream) : m_stream(stream) {}
    ~ScriptStreamGuard() { ApplicationCloseStream(m_stream); }
    ScriptStreamGuard(const ScriptStreamGuard&) = delete;
    ScriptStreamGuard& operator=(const ScriptStreamGuard&) = delete;

private:
    IStreamBase* m_stream;
};

// PARTIAL: PyDataConstants::getConstant("ScriptCmdID", "ExecScript") (the object is ApplicationGetPyDataConstants(),
// dword_99F758). The PyDataConstants header in the tree (pydata-accessors/w7_pydata12.hpp) redefines IStreamBase and
// StreamReader, so it cannot be included in the same translation unit as dh2/IStreamBase.hpp and dh2/StreamReader.hpp.
// Until the headers are unified this returns -1, which matches no command. The only effect is in skip mode: commands
// that are not ExecScript are then never checked for blocking.
int ExecScriptCmdIdNotRecovered() {
    return -1;
}

}  // namespace

// 0x455980 ScriptManager::ScriptManager()
// 0x4559bc ScriptManager::ScriptManager()
// C1 and C2 have identical bodies; one definition covers both. The original zeroes the vector headers (+0x0c .. +0x2c),
// which the vectors' default construction gives too. pad_31 is not written.
ScriptManager::ScriptManager()
    : m_skipScript(-1),   // +0x00
      m_activeCount(0),   // +0x04
      m_loadedCount(0),   // +0x08
      unk_30(false) {}    // +0x30

// 0x455ad8 ScriptManager::InitCommands()
// Calls the Init slot (+0x00) of every command of every script block, in order. The original re-reads the vectors after
// each call. The index form below does the same.
void ScriptManager::InitCommands() {
    for (int script = 0; script < static_cast<int>(m_cmds.size()); ++script) {
        for (int c = 0; c < m_cmds[script].m_count; ++c) {
            m_cmds[script].m_cmds[c]->Init();
        }
    }
}

// 0x455b8c ScriptManager::GetNameFromID(int) const
const char* ScriptManager::GetNameFromID(int id) const {
    if (id < 0) {
        return nullptr;
    }
    if (id >= static_cast<int>(m_scriptNames.size())) {
        return nullptr;
    }
    return m_scriptNames[id];
}

// 0x455bb8 ScriptManager::StopScript(int, bool)
// Marks script id finished (state 2) and clears its module. The notify argument is not read by the original body.
void ScriptManager::StopScript(int id, bool notify) {
    static_cast<void>(notify);
    m_contexts[id].m_state = 2;    // ScriptContext +0x08
    m_contexts[id].m_module = -1;  // ScriptContext +0x00
}

// 0x455be0 ScriptManager::StopSkipping()
void ScriptManager::StopSkipping() {
    m_skipScript = -1;
}

// 0x455bec ScriptManager::IsScriptRunning(int) const
bool ScriptManager::IsScriptRunning(int id) const {
    return m_contexts[id].m_state != 2;
}

// 0x455c40 ScriptManager::GetScriptModule(int) const
int ScriptManager::GetScriptModule(int id) const {
    return m_contexts[id].m_module;
}

// 0x455c54 ScriptManager::IsCutSceneRunning() const
// The original takes the script count from m_cmds (its begin and end) and tests each script's state in turn.
bool ScriptManager::IsCutSceneRunning() const {
    const int count = static_cast<int>(m_cmds.size());
    for (int id = 0; id < count; ++id) {
        if (IsScriptRunning(id)) {
            return true;
        }
    }
    return false;
}

// 0x45632c ScriptManager::ScriptCmds::Free()
// Destroys each command of the block (its destructor is called directly, not through the vtable, as in the original),
// frees each command and then the pointer array, and zeroes the block.
void ScriptManager::ScriptCmds::Free() {
    if (m_cmds != nullptr) {
        for (int i = 0; i < m_count; ++i) {
            ScriptCmdImpl* cmd = m_cmds[i];
            if (cmd != nullptr) {
                cmd->~ScriptCmdImpl();
                CustomFree(cmd);
            }
        }
        CustomFree(m_cmds);
    }
    m_count = 0;
    m_cmds = nullptr;
}

// 0x4591f0 ScriptManager::GetIDFromName(char const*, bool) const
// searchAll false starts at m_loadedCount (the scripts added by the last append load), true starts at 0.
// Returns the first id whose name matches case-insensitively, or -1.
int ScriptManager::GetIDFromName(const char* name, bool searchAll) const {
    const int count = static_cast<int>(m_scriptNames.size());
    const int start = searchAll ? 0 : m_loadedCount;
    if (start >= count) {
        return -1;
    }
    for (int id = start; id < count; ++id) {
        if (LC_API_STRCASECMP(m_scriptNames[id], name) == 0) {
            return id;
        }
    }
    return -1;
}

// 0x45a1c8 ScriptManager::UnLoadAllScripts()
// Frees every script block and every name, empties the vectors (their storage is kept), clears the skip request and the
// loaded count.
void ScriptManager::UnLoadAllScripts() {
    for (int script = 0; script < static_cast<int>(m_cmds.size()); ++script) {
        m_cmds[script].Free();
    }
    for (int id = 0; id < static_cast<int>(m_scriptNames.size()); ++id) {
        CustomFree(m_scriptNames[id]);
        m_scriptNames[id] = nullptr;
    }
    m_scriptNames.clear();
    m_cmds.clear();
    m_contexts.clear();
    StopSkipping();
    m_loadedCount = 0;
}

// 0x45a2ac ScriptManager::Flush()
// Resets the skip request, the unk_30 flag, the active count and the loaded count, unloads everything, and clears the
// global v2Controller block flag.
void ScriptManager::Flush() {
    m_skipScript = -1;
    unk_30 = false;
    m_activeCount = 0;
    m_loadedCount = 0;
    UnLoadAllScripts();
    v2Controller::s_blocked = false;
}

// 0x45a880 ScriptManager::~ScriptManager()   (D2)
// 0x460204 ScriptManager::~ScriptManager()   (D1, same body; one definition covers both)
// The original unloads the scripts, frees the name storage, then destroys m_cmds and m_contexts. In C++ the member vectors
// do the last part after this body, in reverse declaration order, which is the original order.
ScriptManager::~ScriptManager() {
    UnLoadAllScripts();
}

// 0x45ad40 ScriptManager::LoadScriptFileNames(char const*, bool)
// PARTIAL. The file starts with a raw 32-bit count of names. Each name is a 32-bit length followed by that many raw bytes,
// and the names are appended to m_scriptNames. With append set, the file is read only when every loaded script is
// registered (m_loadedCount >= script count). The original first copies the whole file into a net-transport StreamBuffer
// and then releases the file (slot +0x78). That StreamBuffer declares a second IStreamBase and cannot be included with
// dh2/IStreamBase.hpp, so the bytes are read from the open stream here and the file is released when the loader returns.
// The bytes read are the same; only the release comes later.
void ScriptManager::LoadScriptFileNames(const char* file, bool append) {
    if (append && m_loadedCount < static_cast<int>(m_cmds.size())) {
        return;
    }
    IStreamBase* stream = ApplicationOpenStream(file);
    if (stream == nullptr) {
        return;
    }
    ScriptStreamGuard guard(stream);

    int nameCount = 0;
    stream->read(&nameCount, 4);
    const int oldCount = static_cast<int>(m_scriptNames.size());
    const int newCount = oldCount + nameCount;
    m_scriptNames.resize(static_cast<std::size_t>(newCount), nullptr);
    for (int i = oldCount; i < newCount; ++i) {
        unsigned int length = 0;
        StreamReader::readAs<unsigned int>(stream, &length);
        char* name = new (MHS_UNRECOVERED) char[length + 1];
        StreamReader::readStringEx(stream, name, length);
        name[length] = '\0';
        m_scriptNames[i] = name;
    }
}

// 0x45b264 ScriptManager::LoadScriptFile(char const*, bool)
// PARTIAL. The file starts with a 32-bit script count, then one 32-bit command count per script. The original then creates
// each command through the factory table s_scriptCmdImplMap[typeId], reads its argument record through the argument
// object's vtable slot +0x0c, fills the ScriptCmdImpl fields (+0x08 type id, +0x0c arguments), runs its Init and sets the
// block flag from its +0x04 flag. None of that is reconstructed: the factory table (ELF _ZL18s_scriptCmdImplMap, 320 bytes in
// .data.rel.ro) and the argument-record class are not in the tree. So the load stops at the first script that has commands.
// Scripts with no commands load in full (count, resize and StopScript as in the original). The stream is read directly,
// as in LoadScriptFileNames.
void ScriptManager::LoadScriptFile(const char* file, bool append) {
    if (append && m_loadedCount < static_cast<int>(m_cmds.size())) {
        return;
    }
    IStreamBase* stream = ApplicationOpenStream(file);
    if (stream == nullptr) {
        return;
    }
    ScriptStreamGuard guard(stream);

    int scriptCount = 0;
    StreamReader::readAs<int>(stream, &scriptCount);
    const int firstNew = static_cast<int>(m_cmds.size());
    const int endNew = firstNew + scriptCount;
    if (append) {
        m_loadedCount += scriptCount;
    }
    m_cmds.resize(static_cast<std::size_t>(endNew), ScriptCmds{});
    m_contexts.resize(static_cast<std::size_t>(endNew), ScriptContext{});

    for (int script = firstNew; script < endNew; ++script) {
        int commandCount = 0;
        StreamReader::readAs<int>(stream, &commandCount);
        if (commandCount > 0) {
            // PARTIAL stop: the command factories and argument records are not reconstructed. The scripts from here on
            // are finished (state 2) with no commands, so ExecuteScript never runs them.
            for (int rest = script; rest < endNew; ++rest) {
                StopScript(rest, false);
            }
            return;
        }
        m_cmds[script].m_count = commandCount;                            // 0
        m_cmds[script].m_cmds = new (MHS_UNRECOVERED) ScriptCmdImpl*[0];  // operator new[](4 * count, 0) of the original
        StopScript(script, false);
    }
}

// 0x45c16c ScriptManager::ExecuteScript(int)
// PARTIAL (see ExecScriptCmdIdNotRecovered). Runs the commands of script id from its next command. Returns true when a
// blocking command is still waiting: its Update runs and the script stays in state 1. Returns false when the script has no
// commands left or is already finished (state 2). With a skip request active, only commands whose id is ExecScript are
// checked for blocking.
bool ScriptManager::ExecuteScript(int id) {
    s_dbgRunningScriptName = m_scriptNames[id];
    if (m_contexts[id].m_state == 2) {
        return false;
    }
    // State 1: the current command already ran, so the loop resumes at its blocking check.
    bool resumeAtCheck = (m_contexts[id].m_state == 1);
    for (;;) {
        ScriptCmdImpl* cmd = m_cmds[id].m_cmds[m_contexts[id].m_nextCmd];
        if (!resumeAtCheck) {
            cmd->Execute(m_skipScript != -1, m_contexts[id].m_module);
            m_contexts[id].m_state = 1;
        }
        resumeAtCheck = false;

        const int execScriptId = ExecScriptCmdIdNotRecovered();
        if (m_skipScript == -1 || execScriptId == cmd->m_cmdId) {
            if (cmd->IsBlocking()) {
                cmd->Update();
                return true;
            }
        }

        const int next = m_contexts[id].m_nextCmd + 1;
        m_contexts[id].m_state = 0;
        m_contexts[id].m_nextCmd = next;
        if (next >= m_cmds[id].m_count) {
            m_contexts[id].m_state = 2;
            --m_activeCount;
            // The original calls DebugSwitches::load(&s_inst), then GetSwitch("isTracingScriptCmd"). GetInstance() performs the
            // same load and returns the instance. The result is not used.
            DebugSwitches::GetInstance()->GetSwitch(std::string("isTracingScriptCmd"));
            return false;
        }
    }
}

// 0x45c37c ScriptManager::ExecuteAllScripts()
// Runs every script once and returns true when any script is still waiting on a blocking command. When no script is
// active the skip request is cleared. When nothing is still waiting, the active count is reset and the skip request is
// cleared again (the two StopSkipping paths of the original).
bool ScriptManager::ExecuteAllScripts() {
    PushProfilingContext("ScriptManager::ExecuteAllScripts");
    bool anyRunning = false;
    const int count = static_cast<int>(m_cmds.size());
    for (int id = 0; id < count; ++id) {
        anyRunning |= ExecuteScript(id);
    }
    if (m_activeCount == 0) {
        StopSkipping();
    }
    if (!anyRunning) {
        m_activeCount = 0;
        StopSkipping();
    }
    PopProfilingContext("ScriptManager::ExecuteAllScripts");
    return anyRunning;
}

// 0x4604c0 ScriptManager::SkipScript(int, bool)
// PARTIAL. Two steps are not reconstructed:
//  - MenuMessageManager<DialogMsg,1>::FlushEnqueuedMessages(): the asm calls clone 0x460474 with no arguments. That clone
//    takes no queue and pops the DialogMsg manager's deque at singleton +0x04 until it is empty. The header's member
//    takes a queue index (clone 0x3f9018 is the StatusMsg form), so the queue value of this clone is not in the tree and
//    the flush is skipped. Not guessed as queue 0.
//  - The online send. When GetOnline()->isOnlineGame_ (COnline +0x05, private; c_online.hpp has SetIsOnlineGame only, no
//    getter) is set and fromRemote is false, the original builds a "CMsgScriptCmd" with CMessage::CreateMessage, sets its
//    fields (+0x50 flag 1, +0x54 script id, +0x58 -1) and sends it with CMessaging::Get()->SendMsg. The CMsgScriptCmd type
//    is declared now (net-session-online c_msg_fixed.hpp); the send waits on the flag getter, the same blocker as StartScript.
// The tracing switch and the skip request are reconstructed.
void ScriptManager::SkipScript(int id, bool fromRemote) {
    static_cast<void>(fromRemote);
    DebugSwitches::GetInstance()->GetSwitch(std::string("isTracingScriptCmd"));
    if (m_skipScript == -1) {
        m_skipScript = id;
    }
}
