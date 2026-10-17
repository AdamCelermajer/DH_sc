#pragma once
#include <cstdint>
#include <string>

namespace dh2::data {
// Original SG_GetFilename@463c84 and its three literal getters. This selects
// a campaign/checkpoint filename; file backup and profile I/O are separate.
// The slot formats as unsigned32 with minimum width3, without truncation.
const char* player_profile_prefix_v1() noexcept;
const char* player_profile_extension_v1() noexcept;
const char* player_checkpoint_extension_v1() noexcept;
std::string player_profile_filename_v1(std::uint32_t slot, bool checkpoint,
                                      bool multiplayer);
}
