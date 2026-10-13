// tGLRoomInfo: one room record of the lobby room list (CMatchingGLLiveLobbyObserver +0x18).
// OnMPListSessionSuccess allocates each record with CustomAlloc(0x28). Layout (original 32-bit
// offsets, kept as comments; the host build uses its own pointer widths):
//   +0x00 room id (int, read from the packet)
//   +0x04 name (char*, owned; released by the destructor 0x82138c)
//   +0x08 second string (char*, owned; released by the destructor)
//   +0x20 attribute blob (char*, owned; released by the destructor)
//   +0x24 blob length (int, set to 0 by the destructor)
// The fields from +0x0c to +0x14 are packet reads and are not named yet. The destructor
// (0x82138c) frees the owned strings and the blob; see the cpp.
#pragma once

#include <cstdint>

class tGLRoomInfo {
public:
    tGLRoomInfo() = default;
    // 0x82138c _ZN11tGLRoomInfoD1Ev. Frees +0x04, +0x08 and +0x20 with CustomFree and sets +0x24 to 0.
    ~tGLRoomInfo();
    tGLRoomInfo(const tGLRoomInfo&) = delete;
    tGLRoomInfo& operator=(const tGLRoomInfo&) = delete;

    // The name at +0x04, compared with strcmp by GetRoomIndexByName. Null when unset.
    const char* name() const { return name_; }

    // Test seam. Not in the original. Stores a copy that the record owns, as the original does.
    void SetNameForTest(const char* name);

private:
    std::int32_t roomId_ = 0;           // +0x00
    char* name_ = nullptr;              // +0x04 (owned; released by the destructor)
    char* second_ = nullptr;            // +0x08 (owned; released by the destructor)
    std::uint8_t pad_0c[0x14] = {};     // +0x0c..+0x1f: packet reads, not named yet
    char* attributes_ = nullptr;        // +0x20 (owned; released by the destructor)
    std::int32_t attributesLength_ = 0; // +0x24 (set to 0 by the destructor)
};
