#pragma once

// Campaign loading tip (Preview 15). Original NativeGetLoadingTipStrID (IDA 0043cb24): a row of the
// help-page table is picked with the game LCG, and the row's string id is looked up in common_text.
// Table: engine-ui LoadingHintTableV1 (help_pages_pyarray/pyarraynames/pystructnames). Text: the
// original MenuLocalization (common_text pack 0). Shared code: the asset catalog only.

#include <cstdint>
#include <string>

namespace dh::foundation {
class AssetCatalog;
}

namespace dh::foundation::startup {

struct LoadingTip {
    std::uint32_t string_id = 0;
    std::string text;
};

// Advances seed (the game Random seed) and returns the tip for the new seed.
bool choose_loading_tip_v1(const AssetCatalog& assets, std::uint32_t& seed, LoadingTip& tip, std::string& error);

} // namespace dh::foundation::startup
