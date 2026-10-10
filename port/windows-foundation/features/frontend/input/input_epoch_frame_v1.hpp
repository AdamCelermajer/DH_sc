#pragma once

#include <cstdint>
#include <memory>
#include <string>

namespace dh::foundation::frontend::input {

// A queued frame is meaningful only while both its source epoch and exact
// shared owner are still current. The frame payload remains owned by the
// caller; this envelope does not copy or mutate any input/gameplay state.
template <class FrameT>
struct InputEpochFrameV1 {
    std::uint64_t epoch{};
    std::shared_ptr<void> owner_lease;
    FrameT frame{};
};

inline bool same_input_owner_v1(const std::shared_ptr<void>& a,
                                const std::shared_ptr<void>& b) noexcept {
    return a && b && a.get() == b.get() && !a.owner_before(b) && !b.owner_before(a);
}

template <class FrameT>
bool validate_input_epoch_frame_v1(const InputEpochFrameV1<FrameT>& queued,
                                   std::uint64_t current_epoch,
                                   const std::shared_ptr<void>& current_owner,
                                   std::string& error) {
    error.clear();
    if (!queued.epoch || queued.epoch != current_epoch ||
        !same_input_owner_v1(queued.owner_lease, current_owner)) {
        error = "Queued input frame belongs to a stale epoch or different owner";
        return false;
    }
    return true;
}

} // namespace dh::foundation::frontend::input
