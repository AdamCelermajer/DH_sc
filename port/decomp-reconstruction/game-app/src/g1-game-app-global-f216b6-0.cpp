// Batch g1-game-app-global-f216b6-0 (<global>, part 1 of 22).
// Allocator forwarders, the hinted operator new overloads, the profiling markers, the config and save
// path helpers, and the CMessage factories GetNewInstance<T>(bool). Forwarders were checked against the
// listing (asm: B target). Declarations are in dh2/global/runtime_support.hpp and
// dh2/global/message_factory.hpp.
#include "dh2/global/message_factory.hpp"
#include "dh2/global/runtime_support.hpp"

#include "core-platform/memory.hpp"
#include "net-session-online/c_msg_drop_loot.hpp"
#include "net-session-online/c_msg_fixed.hpp"
#include "net-session-online/c_msg_quests_sync.hpp"

// 0x310440 CustomFree(void*)
// Not defined here: core-platform/src/memory.cpp has the single definition (std::free; asm: B free).

// 0x310448 LuaFree(void*)
// Asm: B CustomFree.
void LuaFree(void* p) {
    CustomFree(p);
}

// 0x3104fc LuaAlloc(unsigned int)
// Asm: B CustomAlloc(unsigned int).
void* LuaAlloc(unsigned int size) {
    return CustomAlloc(size);
}

// 0x31056c operator new[](unsigned int, MemoryHintState)
// Asm: B CustomAlloc(unsigned int). The hint is not used by the original's thunk.
void* operator new[](std::size_t size, MemoryHintState /*hint*/) {
    return CustomAlloc(static_cast<unsigned int>(size));
}

// 0x310570 operator new(unsigned int, MemoryHintState)
// Asm: B CustomAlloc(unsigned int). The hint is not used by the original's thunk.
void* operator new(std::size_t size, MemoryHintState /*hint*/) {
    return CustomAlloc(static_cast<unsigned int>(size));
}

// 0x3136b4 PushProfilingContext(char const*)
// Asm: BX LR only. Profiling markers are compiled out of the binary.
void PushProfilingContext(const char* /*name*/) {}

// 0x3136b8 PopProfilingContext(char const*)
// Asm: BX LR only.
void PopProfilingContext(const char* /*name*/) {}

// 0x31f4ec GetConfigFilePath(char*, char const*)
// Asm: BX LR only. The output buffer is not written in this build.
void GetConfigFilePath(char* /*out*/, const char* /*name*/) {}

// 0x31f4f0 GetSaveFilePath(char*, char const*)
// Asm: BX LR only. The output buffer is not written in this build.
void GetSaveFilePath(char* /*out*/, const char* /*name*/) {}

// Factories. Each original builds the object with operator new(size, hint 2), then runs the class
// constructor or the inline CMessage(name, bool) sequence. The host build uses the plain operator new,
// so the hint has no effect here.

// 0x328ad4 CMessage* GetNewInstance<CMsgControllerAction>(bool)
template <>
CMessage* GetNewInstance<CMsgControllerAction>(bool withSerial) {
    return new CMsgControllerAction(withSerial);
}

// 0x328afc CMessage* GetNewInstance<CMsgMenuReady>(bool)
// PARTIAL: the bool is not passed to CMessage(name, bool). The original runs that base constructor
// on the new object, then stores the vptr, the type name and sendMode_ = 1 (+0x2c). c_msg_fixed.hpp
// has no CMsgMenuReady(bool) constructor, so this uses the default constructor, then SetProperties(),
// which makes the same type-name and sendMode_ stores. Missing: the serial gate of the base constructor.
// Layout request: add explicit CMsgMenuReady(bool) : CMessage("CMsgMenuReady", withSerial) to c_msg_fixed.hpp.
// Also missing: the rest of the base constructor effects. The header (type id, serial, sender) and lifetime_ are
// not set, trackSent_ stays 0 instead of 100, and reliable_ and includeLocalMember_ stay false unless SetProperties() sets them.
template <>
CMessage* GetNewInstance<CMsgMenuReady>([[maybe_unused]] bool withSerial) {
    auto* msg = new CMsgMenuReady();
    msg->SetProperties();
    return msg;
}

// 0x328b6c CMessage* GetNewInstance<CMsgAttackResult>(bool)
// PARTIAL: same shape as CMsgMenuReady. The original stores sendMode_ = 1 (+0x2c) and includeLocalMember_
// = 0 (+0x33) inline, and allocates 0x84 bytes. c_msg_fixed.hpp's CMsgAttackResult has that size (80 + 52)
// and SetProperties() makes the same stores. Missing: the bool, and CMsgAttackResult(bool) is not declared
// by this class. c_msg_attack_result.hpp declares a second CMsgAttackResult with that constructor, and the
// two cannot be in one translation unit. The duplicate must be merged before this is complete.
// Also missing: the rest of the base constructor effects. The header (type id, serial, sender) and lifetime_ are
// not set, trackSent_ stays 0 instead of 100, and reliable_ and includeLocalMember_ stay false unless SetProperties() sets them.
template <>
CMessage* GetNewInstance<CMsgAttackResult>([[maybe_unused]] bool withSerial) {
    auto* msg = new CMsgAttackResult();
    msg->SetProperties();
    return msg;
}

// 0x328c5c CMessage* GetNewInstance<CMsgInitialSetup>(bool)
template <>
CMessage* GetNewInstance<CMsgInitialSetup>(bool withSerial) {
    return new CMsgInitialSetup(withSerial);
}

// 0x328d08 CMessage* GetNewInstance<CMsgDropLoot>(bool)
template <>
CMessage* GetNewInstance<CMsgDropLoot>(bool withSerial) {
    return new CMsgDropLoot(withSerial);
}

// 0x328d30 CMessage* GetNewInstance<CMsgAskResendAttributes>(bool)
// PARTIAL: same shape as CMsgMenuReady (inline base-constructor sequence, then sendMode_ = 0, flag32_ = 1).
// SetProperties() makes those stores. Missing: the bool. Layout request: add the CMsgAskResendAttributes(bool)
// constructor to c_msg_fixed.hpp. The original allocates 0x54 bytes (a 4-byte payload at +0x50), while the
// header's payload is 1 byte; check the payload size against the binary.
// Also missing: the rest of the base constructor effects. The header (type id, serial, sender) and lifetime_ are
// not set, trackSent_ stays 0 instead of 100, and reliable_ and includeLocalMember_ stay false unless SetProperties() sets them.
template <>
CMessage* GetNewInstance<CMsgAskResendAttributes>([[maybe_unused]] bool withSerial) {
    auto* msg = new CMsgAskResendAttributes();
    msg->SetProperties();
    return msg;
}

// 0x328da8 CMessage* GetNewInstance<CMsgIsHost>(bool)
// PARTIAL: same shape as CMsgMenuReady (sendMode_ = 0, flag32_ = 1). Missing: the bool, see CMsgMenuReady.
// Layout request: add an explicit CMsgIsHost(bool) constructor to c_msg_fixed.hpp.
// Also missing: the rest of the base constructor effects. The header (type id, serial, sender) and lifetime_ are
// not set, trackSent_ stays 0 instead of 100, and reliable_ and includeLocalMember_ stay false unless SetProperties() sets them.
template <>
CMessage* GetNewInstance<CMsgIsHost>([[maybe_unused]] bool withSerial) {
    auto* msg = new CMsgIsHost();
    msg->SetProperties();
    return msg;
}

// 0x329270 CMessage* GetNewInstance<CMsgQuestsSync>(bool)
template <>
CMessage* GetNewInstance<CMsgQuestsSync>(bool withSerial) {
    return new CMsgQuestsSync(withSerial);
}

// 0x329298 CMessage* GetNewInstance<CMsgGlobalDeath>(bool)
// PARTIAL: same shape as CMsgMenuReady (reliable_ = 1 at +0x30, sendMode_ = 1, flag32_ = 1). SetProperties()
// makes those stores. Missing: the bool. Layout request: add an explicit CMsgGlobalDeath(bool) constructor to
// c_msg_fixed.hpp.
// Also missing: the rest of the base constructor effects. The header (type id, serial, sender) and lifetime_ are
// not set, trackSent_ stays 0 instead of 100, and reliable_ and includeLocalMember_ stay false unless SetProperties() sets them.
template <>
CMessage* GetNewInstance<CMsgGlobalDeath>([[maybe_unused]] bool withSerial) {
    auto* msg = new CMsgGlobalDeath();
    msg->SetProperties();
    return msg;
}

// 0x3296ac CMessage* GetNewInstance<CMsgRaisedEvent>(bool)
// PARTIAL: the original calls CMsgRaisedEvent(bool) (0x329630, in c_msg_raised_event.hpp). That header and
// c_msg_fixed.hpp each declare a CMsgRaisedEvent, so they cannot be in one translation unit. This uses the
// c_msg_fixed.hpp class (allocation size 0x64 matches its 80 + 20 bytes), default constructor, then
// SetProperties() (0x327a0c). The constructor body at 0x329630 is not mirrored, and the bool is dropped.
// Also missing: the rest of the base constructor effects. The header (type id, serial, sender) and lifetime_ are
// not set, trackSent_ stays 0 instead of 100, and reliable_ and includeLocalMember_ stay false unless SetProperties() sets them.
template <>
CMessage* GetNewInstance<CMsgRaisedEvent>([[maybe_unused]] bool withSerial) {
    auto* msg = new CMsgRaisedEvent();
    msg->SetProperties();
    return msg;
}

// 0x32a140 CMessage* GetNewInstance<CMsgStartGame>(bool)
// PARTIAL: same shape as CMsgMenuReady (flag32_ = 1, includeLocalMember_ = 0 at +0x33, sendMode_ = 1).
// SetProperties() makes those stores. Missing: the bool. Layout request: add an explicit CMsgStartGame(bool)
// constructor to c_msg_fixed.hpp.
// Also missing: the rest of the base constructor effects. The header (type id, serial, sender) and lifetime_ are
// not set, trackSent_ stays 0 instead of 100, and reliable_ and includeLocalMember_ stay false unless SetProperties() sets them.
template <>
CMessage* GetNewInstance<CMsgStartGame>([[maybe_unused]] bool withSerial) {
    auto* msg = new CMsgStartGame();
    msg->SetProperties();
    return msg;
}
