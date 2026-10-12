// tMemberInfo: one member entry. Layout (original 32-bit offsets, kept as comments):
//   +0x00 member id (int, set to -1 by the destructor)
//   +0x0c..+0x23 string-like member (STL internals; buffer at +0x20, end at +0x0c), not modelled
//   +0x24 int (set to 0 by the destructor). Field names are not recovered.
// Partial: the destructor frees the string buffer in the original (a block above 0x80 bytes goes
// to CustomFree, smaller ones to the node allocator). That string type is not in the port, so
// only the field resets are reproduced here.
#pragma once

#include <cstdint>

class tMemberInfo {
public:
    // 0x821dbc _ZN11tMemberInfoD1Ev. Partial, see above.
    ~tMemberInfo();

    // Test seams. Not in the original.
    std::int32_t MemberIdForTest() const { return memberId_; }
    std::int32_t Value24ForTest() const { return value24_; }
    void SetForTest(std::int32_t memberId, std::int32_t value24) {
        memberId_ = memberId;
        value24_ = value24;
    }

private:
    std::int32_t memberId_ = 0;  // +0x00
    std::int32_t value24_ = 0;   // +0x24
};
