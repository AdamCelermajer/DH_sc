#include "loading_tip_v1.hpp"

#include "../../../engine-ui/loading_menu_v1.hpp"
#include "../character_menu/menu_text.hpp"
#include "asset_catalog.hpp"
#include "content_paths.hpp"

#include <stdexcept>

namespace dh::foundation::startup {

bool choose_loading_tip_v1(const AssetCatalog& assets, std::uint32_t& seed, LoadingTip& tip, std::string& error) {
    try {
        const auto records = read_content(assets, "data/help_pages_pyarray.bin");
        const auto names = read_content(assets, "data/help_pages_pyarraynames.bin");
        const auto fields = read_content(assets, "data/help_pages_pystructnames.bin");
        dh2::ui::LoadingHintTableV1 table;
        if (!table.load({records.data(), records.size()}, {names.data(), names.size()}, {fields.data(), fields.size()}, error))
            return false;
        dh2::ui::LoadingHintRandomV1 random{seed, 0};
        std::uint32_t stringId = 0;
        if (!table.next(random, stringId, error)) return false;
        seed = random.seed;

        character_menu::MenuLocalization localization;
        if (!localization.load(assets, "data", 0, error)) return false;
        std::string text;
        if (!localization.string_id(static_cast<std::int32_t>(stringId), text, error)) return false;
        tip.string_id = stringId;
        tip.text = std::move(text);
        error.clear();
        return true;
    } catch (const std::exception& x) {
        error = x.what();
        return false;
    }
}

} // namespace dh::foundation::startup
