#include "menu_metadata_v1.hpp"
#include "../../../engine-ui/menu_save_slot_projection_v1.hpp"
#include "../../../engine-ui/save_slot_date_v1.hpp"
#include <algorithm>
#include <cctype>

namespace dh::foundation::menu_metadata {
namespace {
std::string lower_file_name(std::string_view path) {
    const auto slash = path.find_last_of("/\\");
    if (slash != std::string_view::npos) path.remove_prefix(slash + 1);
    std::string out(path);
    for (auto& ch : out) ch = char(std::tolower(static_cast<unsigned char>(ch)));
    return out;
}
struct SlotServices {
    character_menu::MenuLocalization* text{};
};
bool constant_cb(void* context, const char* group, const char* key, std::int32_t& value, std::string& error) {
    return static_cast<SlotServices*>(context)->text->constant(group, key, value, error);
}
bool string_cb(void* context, std::uint32_t id, std::string& value, std::string& error) {
    if (id > 0x7fffffffu) { error = "Menu save-slot string id is outside the localization range"; return false; }
    return static_cast<SlotServices*>(context)->text->string_id(std::int32_t(id), value, error);
}
bool date_cb(void*, std::uint32_t raw, std::tm& out, std::string& error) {
    return dh2::ui::menu_save_slot_local_date_v1(raw, out, error);
}
bool symbol(character_menu::MenuLocalization& text, const char* name, std::string& value, std::string& error) {
    return text.symbol(name, nullptr, value, error);
}
} // namespace

bool load_level_tables(const AssetCatalog& assets, dh2::data::LevelTables& out, std::string& error) {
    try {
        const auto records = assets.read("data/levels_pyarray.bin");
        const auto names = assets.read("data/levels_pyarraynames.bin");
        const auto schema = assets.read("data/levels_pystructnames.bin");
        return dh2::data::load_levels({records.data(), records.size()}, {names.data(), names.size()},
                                      {schema.data(), schema.size()}, out, error);
    } catch (const std::exception& failure) {
        error = std::string("LevelList assets unavailable: ") + failure.what();
        return false;
    }
}

std::int32_t find_level_row(const dh2::data::LevelTables& levels, std::string_view level_uri) {
    const auto wanted = lower_file_name(level_uri);
    if (wanted.empty()) return -1;
    for (std::size_t i = 0; i < levels.levels.size(); ++i)
        if (lower_file_name(levels.levels[i].file) == wanted) return std::int32_t(i);
    return -1;
}

void initialize_fresh_menu_metadata(CharacterState& state, std::uint32_t now) {
    state.current_difficulty = 0;
    state.unlocked_difficulty = 0;
    state.menu_metadata.known = true;
    state.menu_metadata.save_time = now;
    state.menu_metadata.level_row = {fresh_profile_level_row, fresh_profile_level_row, fresh_profile_level_row};
    state.menu_metadata.current_act = {1, 1, 1};
}

void stamp_menu_metadata(CharacterState& state, std::uint32_t now, std::int32_t level_row, std::int32_t act) {
    auto& meta = state.menu_metadata;
    const auto difficulty = std::size_t(std::clamp(state.current_difficulty, 0, 2));
    meta.known = true;
    meta.save_time = now;
    if (level_row >= 0) meta.level_row[difficulty] = level_row;
    if (act > 0) meta.current_act[difficulty] = act;
}

void stamp_menu_metadata_for_level(CharacterState& state, std::uint32_t now,
                                   const dh2::data::LevelTables* levels, std::string_view level_uri) {
    stamp_menu_metadata(state, now, levels ? find_level_row(*levels, level_uri) : -1);
}

void set_visited_module(CharacterState& state, const std::string& level_uri,
                        std::uint32_t module_id, bool visited) {
    auto& list = state.visited_modules;
    auto at = list.begin();
    while (at != list.end() && (at->level_uri < level_uri || (at->level_uri == level_uri && at->module_id < module_id)))
        ++at;
    if (at != list.end() && at->level_uri == level_uri && at->module_id == module_id) {
        at->visited = visited ? 1 : 0;
        return;
    }
    list.insert(at, CharacterVisitedModule{level_uri, module_id, std::uint8_t(visited ? 1 : 0)});
}

bool project_slot_presentation(const CharacterState& state, std::int32_t slot,
    std::int32_t character_row, const dh2::data::CharacterTable& characters,
    const dh2::data::LevelTables& levels, character_menu::MenuLocalization& text,
    std::uint32_t language, frontend::creation::SavedProfilePresentation& out, std::string& error) {
    // Hud_Level receiver: GLOBAL_LEVEL + " " + PlayerLVL (all saves).
    std::string level_label;
    if (!symbol(text, "GLOBAL_LEVEL", level_label, error)) return false;
    out.level_text = level_label + " " + std::to_string(state.stats.level);
    const auto& meta = state.menu_metadata;
    if (!meta.known) { error.clear(); return true; }

    SlotServices services{&text};
    dh2::ui::MenuSaveSlotPresentationServicesV1 callbacks;
    callbacks.context = &services;
    callbacks.constant = constant_cb;
    callbacks.string_id = string_cb;
    callbacks.local_date = date_cb;
    dh2::data::MenuProfileMetadataV1 profile;
    profile.slot = slot;
    profile.level = std::int32_t(state.stats.level);
    profile.character_row = character_row;
    profile.selected_difficulty = state.current_difficulty;
    profile.unlocked_difficulty = state.unlocked_difficulty;
    profile.name = state.name;
    profile.location.save_date = meta.save_time;
    profile.location.levels = meta.level_row;
    profile.location.current_acts = meta.current_act;
    profile.location.volatile_acts = meta.current_act;
    dh2::ui::SwfFrontSaveSlotDetailsV1 details;
    // difficulty override -1: the profile's own CurrentDifficulty (source default).
    if (!dh2::ui::project_menu_save_slot_v1(profile, characters, levels, -1, false, language,
                                            callbacks, details, error))
        return false;
    // The source maps an unset (-1) level to row 0; never show that as a place.
    const auto difficulty = std::size_t(state.current_difficulty);
    out.localized_location = meta.level_row[difficulty] < 0 ? std::string() : details.player_location;
    out.formatted_save_date = meta.save_time == 0 ? std::string() : details.last_save;
    out.current_act = unsigned(std::max(details.current_act, 0));
    out.current_act_known = true;
    out.difficulty = unsigned(details.difficulty);
    out.difficulty_known = true;

    if (!text.parsed_symbol("MENU_ACT", std::int32_t(out.current_act), out.act_text, error)) return false;
    if (!symbol(text, "MENU_LAST_SAVE", out.last_save_label, error)) return false;
    std::string difficulty_title;
    if (!symbol(text, "MENU_DIFFICULTY", difficulty_title, error)) return false;
    out.difficulty_title = difficulty_title + ":";
    static constexpr const char* names[] = {"GAMEPLAYMENUS_DIFFICULTY_NORMAL",
        "GAMEPLAYMENUS_DIFFICULTY_HARD", "GAMEPLAYMENUS_DIFFICULTY_VERYHARD"};
    if (!symbol(text, names[difficulty], out.difficulty_text, error)) return false;
    error.clear();
    return true;
}

} // namespace dh::foundation::menu_metadata
