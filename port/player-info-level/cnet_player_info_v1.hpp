#pragma once
#include "netstruct_members_v1.hpp"

namespace dh2::cnet_player_info_v1 {
using Status = netstruct_members_v1::Status;
using Member = netstruct_members_v1::Member;
struct Record {
    netstruct_members_v1::NetStruct network;
    std::array<Member, 8> members;
    std::uint32_t field_280 = 0;
    std::uint64_t* serial = nullptr; // sole global NetStructMember serial
    bool constructed = false;
    bool busy = false;
    Record() = default;
    Record(const Record&) = delete;
    Record& operator=(const Record&) = delete;
    ~Record();
    Member* at(std::uint32_t source_offset);
    const Member* at(std::uint32_t source_offset) const;
};
// Before construct, seed each scalar header.value with its actual prior
// backing word. C1 and C2 have the same reached semantics; both are represented.
Status construct(Record&, std::uint64_t* serial);
Status construct_c2(Record&, std::uint64_t* serial);
Status reset(Record&);
Status copy_construct(Record&, const Record&);
Status assign(Record&, const Record&);
Status destroy(Record&);
} // namespace dh2::cnet_player_info_v1
