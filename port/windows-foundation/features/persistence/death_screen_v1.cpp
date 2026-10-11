#include "death_screen_v1.hpp"

namespace dh::foundation::persistence::death_flow {
namespace {
constexpr std::array<DeathScreenArtPlacementV1, 5> kArt{{
    {DeathScreenArtRoleV1::continue_hit_area, 33,
     {1.f, 0.f, 0.f, 1.f, -10281.f, -46.f},
     {0, 9887, 0, 6540}},
    {DeathScreenArtRoleV1::inactive_block_hit_area, 33,
     {1.f, 0.f, 0.f, 1.f, -281.f, -46.f},
     {0, 9887, 0, 6540}},
    {DeathScreenArtRoleV1::black_fade_overlay, 685,
     {1.0415802001953125f, 0.f, 0.f, 1.0617523193359375f, -242.f, -204.f},
     {0, 9619, 0, 6478}},
    {DeathScreenArtRoleV1::original_panel_art, 739,
     {1.f, 0.f, 0.f, 1.f, 4813.f, 3219.f},
     {-4890, 4890, -3280, 3280}},
    {DeathScreenArtRoleV1::localized_death_quote, 741,
     {1.f, 0.f, 0.f, 1.f, 640.f, 4422.f},
     {0, 8199, 0, 491}},
}};
constexpr DeathScreenTimelineV1 kTimeline{};
} // namespace

const std::array<DeathScreenArtPlacementV1, 5>&
original_death_screen_art_v1() {
    return kArt;
}

const DeathScreenTimelineV1& original_death_screen_timeline_v1() {
    return kTimeline;
}

} // namespace dh::foundation::persistence::death_flow
