// Batch g1-game-app-global-f216b6-1 (<global>, part 2 of 22).
// The subtitle and skip helpers, and the CMsg factories GetNewInstance<CMsgSpawnObject>(bool) and
// GetNewInstance<CMsgScriptCmd>(bool). The object factories GetNewInstance<T>() are in
// g1-game-app-global-f216b6-1_objects.cpp: those need the dh2 game headers, which clash with
// net-session-online's c_msg_fixed.hpp in one translation unit, so the two sets are built separately.
// The Hex-Rays listing drops the hint and GO_IDS arguments, so they were read from the asm where used.
// Declarations: dh2/global/message_factory.hpp and dh2/global/subtitles.hpp.
#include "dh2/global/message_factory.hpp"
#include "dh2/global/subtitles.hpp"

#include "core-platform/memory.hpp"
#include "core-platform/w7_global_00.hpp"
#include "net-session-online/c_msg_fixed.hpp"

namespace {

// The subtitle deque holds 32-slot nodes (a 0x80-byte node of 4-byte pointers, as ClearSubtitles notes).
constexpr int kSubtitleNodeSlots = 32;

// Layout request (SubtitleEntry, core-platform/w7_global_00.hpp): the original reads an int start time at +0x00
// and an int end time at +0x04. The header declares those words as void* (unknown_00, unknown_04), which is
// host-width, so the times cannot be read yet. Both read as zero until the entry layout is fixed.
int SubtitleStartTime(const SubtitleEntry* /*entry*/) {
    return 0;
}

int SubtitleEndTime(const SubtitleEntry* /*entry*/) {
    return 0;
}

// Text of an entry (+0x08 in the original), or null when the entry has none.
char* SubtitleText(const SubtitleEntry* entry) {
    return static_cast<char*>(entry->text_08);
}

// Frees an entry: its text buffer first (when present, and cleared), then the entry itself.
void FreeSubtitleEntry(SubtitleEntry* entry) {
    if (entry->text_08 != nullptr) {
        CustomFree(entry->text_08);
        entry->text_08 = nullptr;
    }
    CustomFree(entry);
}

}  // namespace

// 0x33931c GetSubtitleTimeInfo(char*, int&, int&)
// The original body is a single BX LR (asm checked). Nothing is read or written.
void GetSubtitleTimeInfo(char* /*text*/, int& /*startTime*/, int& /*endTime*/) {}

// 0x339320 GetSkipText()
// Asm: loads the address of g_skip through the GOT and returns it.
extern "C" int* GetSkipText() {
    return &g_skip;
}

// 0x33933c GetSubtitleTextToSet(int)
// PARTIAL, for two gaps. (1) The start and end times read through SubtitleStartTime and SubtitleEndTime are
// stubs that return zero: SubtitleEntry's +0x00 and +0x04 int fields are not declared (layout request above).
// With zero times, a queued subtitle is always due and a current subtitle always ends on the next call, so the
// timing is wrong until the stubs are replaced. (2) The node release at the end of a node calls CustomFree,
// where the original calls the SGI node allocator (__node_alloc::_M_deallocate, 0x80 bytes). The allocator that
// matches LoadSubtitles is not recovered, the same gap as ClearSubtitles in core-platform/src/w7_global_00.cpp.
// The control flow and the stores otherwise follow the original's inlined deque pop_front.
extern "C" char* GetSubtitleTextToSet(int currentTime) {
    SubtitleDeque& q = g_SubtitlesQueue;  // start.cur, start.first, start.last, start.node, finish.cur

    // v2 in the original: the front entry, read only when the queue is not empty.
    SubtitleEntry* front = nullptr;
    if (q.cur != q.finish) {
        front = *q.cur;
    }

    if (q.cur == q.finish || SubtitleStartTime(front) > currentTime) {
        // Queue empty, or its next subtitle is not due yet. The current subtitle ends or keeps showing.
        SubtitleEntry* current = g_CurrentSubtitle;
        if (current == nullptr) {
            return nullptr;
        }
        if (currentTime >= SubtitleEndTime(current)) {
            FreeSubtitleEntry(current);
            g_CurrentSubtitle = nullptr;
            return nullptr;
        }
        return SubtitleText(current);
    }

    // The next subtitle is due. Drop the current one, then pop the front entry.
    SubtitleEntry* current = g_CurrentSubtitle;
    if (current != nullptr) {
        FreeSubtitleEntry(current);
        g_CurrentSubtitle = nullptr;
    }

    SubtitleEntry** pos = q.cur;  // v1 in the original
    if (pos == q.last - 1) {
        // Last slot of the node: release the node and move the deque map to the next node.
        if (q.first != nullptr) {
            CustomFree(q.first);  // original: __node_alloc::_M_deallocate(first, 0x80), see PARTIAL note
        }
        ++q.node;
        q.cur = *q.node;
        q.first = q.cur;
        q.last = q.cur + kSubtitleNodeSlots;
    } else {
        ++q.cur;
    }
    g_CurrentSubtitle = front;
    return SubtitleText(front);
}

// 0x32a6e8 CMessage* GetNewInstance<CMsgSpawnObject>(bool)
// Asm: operator new(0x6c, hint 2), then CMsgSpawnObject::CMsgSpawnObject(bool), with the bool passed through.
template <>
CMessage* GetNewInstance<CMsgSpawnObject>(bool withSerial) {
    return new CMsgSpawnObject(withSerial);
}

// 0x32a710 CMessage* GetNewInstance<CMsgScriptCmd>(bool)
// PARTIAL: the original runs CMessage::CMessage("CMsgScriptCmd", bool) on the new object, so the serial gate is
// applied. The original then stores the vptr, the type name (through std::string::_M_assign), sendMode_ = 1
// (+0x2c), flag32_ = 1 (+0x32), reliable_ = 1 (+0x30) and includeLocalMember_ = 1 (+0x33). c_msg_fixed.hpp has no
// CMsgScriptCmd(bool) constructor, so the bool is dropped: the default constructor is followed by SetProperties(),
// which makes the same type-name, sendMode_, flag32_, reliable_ and includeLocalMember_ stores. Missing: the
// serial gate of the base constructor. Layout request: add an explicit CMsgScriptCmd(bool) constructor
// (CMessage("CMsgScriptCmd", withSerial)) to net-session-online/include/net-session-online/c_msg_fixed.hpp.
template <>
CMessage* GetNewInstance<CMsgScriptCmd>([[maybe_unused]] bool withSerial) {
    auto* msg = new CMsgScriptCmd();
    msg->SetProperties();
    return msg;
}
