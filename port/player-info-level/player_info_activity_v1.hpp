#pragma once
#include "player_info_record_v1.hpp"

namespace dh2::player_info_activity_v1 {
using Status = netstruct_members_v1::Status;
// Source offsets describe the existing owned members. The activity word and
// SetState's member are distinct; changing +240 does not change +1f0.
constexpr std::uint32_t activity_member = 0x1d0;
constexpr std::uint32_t state_member = 0x220;
struct Services {
    void* context = nullptr;
    // Borrow GetOnline()->byte+5 from the existing owner on each query.
    // Zero means delivered; exceptions/nonzero are explicit native failures.
    std::int32_t (*online)(void*, std::uint8_t*) = nullptr;
};
Status cnet_is_active(const cnet_player_info_v1::Record&, bool* active);
Status player_is_active(player_info_record_v1::Record&, const Services&, bool* active);
// The original char argument arrives as a promoted ARM word. No narrowing is
// done in this body: UInt8 is serialization metadata, not a stored-value mask.
// A native caller supplies a defined prior temporary word; source comparisons
// supply the captured [SP+20] residue. Both marks use the owner's sole serial.
Status set_state(cnet_player_info_v1::Record&, std::int32_t promoted_state,
                 std::int32_t temporary_residue);
Status set_state(player_info_record_v1::Record&, std::int32_t promoted_state,
                 std::int32_t temporary_residue);
} // namespace dh2::player_info_activity_v1
