#include "runtime_skills_menu_v1.hpp"
#include "../skill_ui/original_skill_art.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>

using namespace dh::foundation;
using namespace dh::foundation::generic_skills;

namespace {
using Raw = std::vector<std::uint8_t>;
Raw read(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) throw std::runtime_error("missing original source asset: " + path);
    return {std::istreambuf_iterator<char>(file), {}};
}
void check(bool value, int line) {
    if (!value) throw std::runtime_error("runtime skills menu test failed line " + std::to_string(line));
}
#define CHECK(x) check((x), __LINE__)

int find_field(const dh2::data::CharacterTable& characters, const char* name) {
    const auto at = std::find(characters.fields.begin(), characters.fields.end(), name);
    return at == characters.fields.end() ? -1 : static_cast<int>(at - characters.fields.begin());
}
std::pair<float, float> centroid(const skill_ui::HitZone& zone) {
    const auto& a = zone.triangles.at(0);
    const auto& b = zone.triangles.at(1);
    const auto& c = zone.triangles.at(2);
    return {(a.x + b.x + c.x) / 3.0f, (a.y + b.y + c.y) / 3.0f};
}
} // namespace

int main(int argc, char** argv) {
    try {
        CHECK(argc == 2);
        const std::string root = argv[1];
        auto skill_data = read(root + "/skills_pyarray.bin");
        auto skill_names = read(root + "/skills_pyarraynames.bin");
        auto skill_schema = read(root + "/skills_pystructnames.bin");
        dh2::data::SkillTables skill_owner;
        std::string error;
        CHECK(skill_owner.load({skill_data.data(), skill_data.size()},
                               {skill_names.data(), skill_names.size()},
                               {skill_schema.data(), skill_schema.size()}, error));
        const auto tables = skill_owner.borrow();

        auto character_data = read(root + "/character_properties_pyarray.bin");
        auto character_names = read(root + "/character_properties_pyarraynames.bin");
        auto character_schema = read(root + "/character_properties_pystructnames.bin");
        dh2::data::CharacterTable characters;
        CHECK(dh2::data::load_characters({character_data.data(), character_data.size()},
                                         {character_names.data(), character_names.size()},
                                         {character_schema.data(), character_schema.size()},
                                         characters, error));
        const int skill_tree = find_field(characters, "SkillTree");
        CHECK(skill_tree == 28);
        const auto knight = std::find(characters.names.begin(), characters.names.end(), "KnightPlayerBase");
        CHECK(knight != characters.names.end());
        const auto class_row = static_cast<std::size_t>(knight - characters.names.begin());
        const int list_id = characters.rows[class_row][static_cast<std::size_t>(skill_tree)];
        CHECK(list_id >= 0 && static_cast<std::size_t>(list_id) < tables.lists().size());

        auto state = std::make_shared<CharacterState>();
        state->id = "runtime-skill-page-state";
        state->class_id = "KnightPlayerBase";
        state->source_points_known = true;
        state->source_skill_points = 4;
        state->source_skill_slots_known = true;
        const auto& class_skills = tables.lists()[static_cast<std::size_t>(list_id)];
        CHECK(!class_skills.empty() && class_skills.size() <= 16);
        for (std::size_t i = 0; i < class_skills.size(); ++i) {
            const auto id = class_skills[i];
            state->skills.push_back({tables.skill_names()[static_cast<std::size_t>(id)],
                                     static_cast<std::uint32_t>(i + 1)});
        }
        state->skill_slots.push_back({0, 0, 0});
        auto source_owner = std::static_pointer_cast<void>(state);
        const auto* expected_state = state.get();
        int release_calls = 0;
        int release_position = 0;
        ServicesV1 services;
        bool stale_skill_details = false;
        bool block_next_level_training = false;
        bool allow_selected_training = true;
        unsigned selected_training_probe_calls = 0;
        services.localized_text = [expected_state](const CharacterState& shared, int,
                                                   std::optional<std::string>& name,
                                                   std::optional<std::string>& description,
                                                   std::string&) {
            CHECK(&shared == expected_state);
            name = "localized test provider name";
            description = "[<font color=\"#9CFF9A\">Active</font>] localized test provider description";
            return true;
        };
        services.skill_level_text = [expected_state, &stale_skill_details,
                                     &block_next_level_training, &tables, &class_skills, list_id](
                const CharacterState& shared,
                const character_menu::SkillPageCurrentNextRequestV1& request,
                character_menu::SkillPageCurrentNextTextV1& result, std::string&) {
            CHECK(&shared == expected_state && request.state == expected_state);
            CHECK(request.skill_list_id == list_id);
            CHECK(request.class_skill_position >= 0 &&
                  static_cast<std::size_t>(request.class_skill_position) < class_skills.size());
            CHECK(class_skills[static_cast<std::size_t>(request.class_skill_position)] ==
                  request.skill_table_id);
            CHECK(request.saved_rank == shared.skills.at(
                      static_cast<std::size_t>(request.class_skill_position)).rank);
            CHECK(request.character_level == shared.stats.level);
            result.state = request.state;
            result.skill_list_id = request.skill_list_id;
            result.class_skill_position = request.class_skill_position;
            result.skill_table_id = request.skill_table_id + (stale_skill_details ? 1 : 0);
            result.saved_rank = request.saved_rank;
            result.character_level = request.character_level;
            const auto required_level = static_cast<std::uint32_t>(std::max(0,
                static_cast<int>(tables.skills()[static_cast<std::size_t>(request.skill_table_id)]
                                     .scalar.words[8])));
            if (request.character_level < required_level) {
                result.current_level = "Unlock at level " + std::to_string(required_level);
                result.next_level.clear();
            } else {
                result.current_level = "<font color=\"#FF0000\">native current result</font>";
                result.next_level = block_next_level_training
                    ? "Training is unavailable" : "native next result";
            }
            return true;
        };
        auto runtime = std::make_shared<RuntimeSkillsMenuV1>(
            state, source_owner, characters, tables, services, RuntimeSkillsRendererV1{},
            [expected_state](const CharacterState& shared, std::optional<unsigned>& active, std::string&) {
                CHECK(&shared == expected_state);
                active = 0;
                return true;
            },
            [&](float x, float y, PageV1& page, std::string& e) {
                CHECK(x == 117.0f && y == 88.0f);
                ++release_calls;
                return page.select(release_position, e);
            },
            [](std::string_view symbol, std::string& text, std::string& e) {
                static const std::pair<std::string_view, std::string_view> entries[]{
                    {"GAMEPLAYMENUS_SKILLS_TITLE", "Skills"},
                    {"MENU_CLASS_SKILL", "Class skill"},
                    {"MENU_SPECIALISATION", "Specialization"},
                    {"MENU_SKILL_BUTTONS", "Skill Mapping"},
                    {"GAMEPLAYMENUS_SKILL_POINTS", "Skill points left"},
                    {"GAMEPLAYMENUS_ADD_SKILL", "Upgrade Skill"},
                    {"GAMEPLAYMENUS_current_level", "Current level"},
                    {"GAMEPLAYMENUS_next_level", "Next level"},
                };
                const auto found = std::find_if(std::begin(entries), std::end(entries),
                    [&](const auto& entry) { return entry.first == symbol; });
                if (found == std::end(entries)) { e = "symbol is not authored by Skills onShow"; return false; }
                text = found->second;
                e.clear();
                return true;
            },
            [expected_state, &allow_selected_training, &selected_training_probe_calls,
             &tables](const CharacterState& shared, int list_id, int position, int table_id,
                      bool& available, std::string& e) {
                CHECK(&shared == expected_state);
                CHECK(list_id >= 0 && static_cast<std::size_t>(list_id) < tables.lists().size());
                CHECK(position >= 0 && static_cast<std::size_t>(position) < tables.lists()[list_id].size());
                CHECK(tables.lists()[list_id][position] == table_id);
                ++selected_training_probe_calls;
                available = allow_selected_training;
                e.clear();
                return true;
            });

        character_menu::Bindings bindings;
        bindings.character = state.get();
        unsigned previous_calls = 0;
        bindings.content = [&](character_menu::Tab tab, character_menu::Frame&, std::string&) {
            ++previous_calls;
            return tab == character_menu::Tab::skills || tab == character_menu::Tab::equipment;
        };
        CHECK(runtime->install_content(bindings, error));
        character_menu::Frame equipment_frame;
        CHECK(bindings.content(character_menu::Tab::equipment, equipment_frame, error));
        CHECK(previous_calls == 1 && equipment_frame.art.batches.empty());
        const auto& source_art = character_menu::original_menu_art(character_menu::Tab::skills);
        const auto add_label_field = std::find_if(source_art.text_fields.begin(), source_art.text_fields.end(),
            [](const auto& field) {
                return field.path == "menu_SkillTreeSheetNew/btn_add/AddText/text";
            });
        CHECK(add_label_field != source_art.text_fields.end());
        character_menu::Frame frame;
        frame.art.batches = source_art.batches;
        frame.solids = source_art.solids;
        // CharacterMenu's shared base frame has already localized this
        // source field before RuntimeSkillsMenu applies the train admission.
        frame.text.push_back({*add_label_field, "stale base Add Skill label"});
        CHECK(bindings.content(character_menu::Tab::skills, frame, error));
        CHECK(previous_calls == 2 && runtime->source_class_art_available());

        CHECK(!source_art.batches.empty());
        const auto retained_panel = std::find_if(source_art.batches.begin(), source_art.batches.end(), [](const auto& batch) {
            return batch.role.find("/buttons/btn_skill") == std::string::npos &&
                   batch.role.find("/buttons/skill") == std::string::npos;
        });
        CHECK(retained_panel != source_art.batches.end());
        CHECK(std::any_of(frame.art.batches.begin(), frame.art.batches.end(), [&](const auto& batch) {
            return batch.role == retained_panel->role;
        }));
        CHECK(std::any_of(frame.art.batches.begin(), frame.art.batches.end(), [](const auto& batch) {
            return batch.role.find("menu_SkillTreeSheetNew/buttons/btn_skill0/btimg") != std::string::npos;
        }));
        CHECK(std::any_of(frame.text.begin(), frame.text.end(), [](const auto& item) {
            return item.field.path.find("cp_Skill_Points/") != std::string::npos && item.value == "4";
        }));
        CHECK(std::any_of(frame.text.begin(), frame.text.end(), [](const auto& item) {
            return item.field.path.find("/buttons/skill0/cnt/value") != std::string::npos && item.value == "1";
        }));
        for (const auto* label : {"Skills", "Class skill", "Specialization", "Skill Mapping",
                                  "Skill points left", "Upgrade Skill"}) {
            CHECK(std::any_of(frame.text.begin(), frame.text.end(), [&](const auto& item) {
                return item.value == label;
            }));
        }
        CHECK(std::count_if(frame.text.begin(), frame.text.end(), [](const auto& item) {
            return item.field.path == "menu_SkillTreeSheetNew/btn_add/AddText/text";
        }) == 1);
        const std::string add_button_prefix = "menu_SkillTreeSheetNew/btn_add/";
        CHECK(std::any_of(frame.art.batches.begin(), frame.art.batches.end(), [&](const auto& batch) {
            return batch.role.compare(0, add_button_prefix.size(), add_button_prefix) == 0;
        })); // Initial presetAllSkills uses the known positive skill-point count.
        CHECK(std::count_if(frame.text.begin(), frame.text.end(), [](const auto& item) {
            return item.field.path.find("/buttons/") != std::string::npos &&
                   item.field.path.find("/cnt/value") != std::string::npos;
        }) == 32);
        const auto title = std::find_if(frame.text.begin(), frame.text.end(), [](const auto& item) {
            return item.field.path == "menu_SkillTreeSheetNew/TitleText/txt_title";
        });
        const std::array<std::uint8_t, 4> title_color{255, 238, 170, 255};
        CHECK(title != frame.text.end() && title->field.character_id == 322 && title->field.font_id == 287 &&
              title->field.rgba == title_color);
        ViewV1 projected_view;
        CHECK(runtime->page().view(projected_view, error));
        for (std::size_t i = 0; i < projected_view.rows.size(); ++i) {
            const auto lock_role = "menu_SkillTreeSheetNew/buttons/skill" + std::to_string(i) + "/Lock/";
            const bool lock_rendered = std::any_of(frame.art.batches.begin(), frame.art.batches.end(),
                [&](const auto& batch) { return batch.role.compare(0, lock_role.size(), lock_role) == 0; });
            const bool source_unlocked = state->stats.level >=
                static_cast<std::uint32_t>(std::max(0, projected_view.rows[i].required_level));
            CHECK(lock_rendered == !source_unlocked);
            // Draw order: the source Lock chain is drawn over the cell icon
            // (reference-399 shows the chain glyph on top of the greyed icon).
            // The generic icon presenter appends the source icon batch, so a
            // visible Lock must be ordered after every btimg batch of its cell.
            if (lock_rendered) {
                const auto btimg_prefix = "menu_SkillTreeSheetNew/buttons/skill" + std::to_string(i) + "/btimg/";
                std::size_t lock_index = frame.art.batches.size(), last_icon_index = 0;
                for (std::size_t b = 0; b < frame.art.batches.size(); ++b) {
                    const auto& role = frame.art.batches[b].role;
                    if (role.compare(0, lock_role.size(), lock_role) == 0 && lock_index == frame.art.batches.size())
                        lock_index = b;
                    if (role.compare(0, btimg_prefix.size(), btimg_prefix) == 0) last_icon_index = b;
                }
                CHECK(lock_index < frame.art.batches.size() && lock_index > last_icon_index);
            }
        }
        CHECK(std::none_of(frame.text.begin(), frame.text.end(), [](const auto& item) {
            return item.field.path.find("/SKILL_NAME/") != std::string::npos ||
                   item.field.path.find("/skill_description/") != std::string::npos;
        }));
        CHECK(std::any_of(frame.art.batches.begin(), frame.art.batches.end(), [](const auto& batch) {
            return batch.role.find("btn_activeskill01_drop_all/btimg") != std::string::npos;
        }));
        CHECK(std::none_of(frame.solids.begin(), frame.solids.end(), [](const auto& solid) {
            return solid.geometry.role.find("/buttons/skill") != std::string::npos &&
                   solid.geometry.role.find("/Grey/") != std::string::npos;
        }));

        const auto provider = runtime->source_page_provider();
        CHECK(provider.owner.get() == source_owner.get());
        CHECK(provider.ready && provider.append && provider.release);
        CHECK(provider.ready(error));
        character_menu::Frame provider_frame;
        provider_frame.art.batches = source_art.batches;
        provider_frame.solids = source_art.solids;
        CHECK(provider.append(provider_frame, error));
        CHECK(!provider_frame.art.batches.empty());
        CHECK(provider.release(117.0f, 88.0f, error));
        CHECK(release_calls == 1 && runtime->page().selected_position() == 0);
        character_menu::Frame selected_frame;
        selected_frame.art.batches = source_art.batches;
        selected_frame.solids = source_art.solids;
        selected_frame.text.push_back({*add_label_field, "stale base Add Skill label"});
        CHECK(provider.append(selected_frame, error));
        CHECK(std::any_of(selected_frame.text.begin(), selected_frame.text.end(), [](const auto& item) {
            return item.field.path.find("/SKILL_NAME/") != std::string::npos &&
                   item.value == "localized test provider name";
        }));
        CHECK(std::any_of(selected_frame.text.begin(), selected_frame.text.end(), [](const auto& item) {
            return item.field.path.find("/skill_description/") != std::string::npos &&
                   item.value == "[<font color=\"#9CFF9A\">Active</font>] localized test provider description" &&
                   item.field.character_id == 466 && item.field.font_id == 103 &&
                   item.field.rgba == std::array<std::uint8_t, 4>{255, 255, 204, 255};
        }));
        const auto current_description = std::find_if(selected_frame.text.begin(), selected_frame.text.end(),
            [](const auto& item) { return item.field.path ==
                "menu_SkillTreeSheetNew/current_skill_description/text"; });
        const auto next_description = std::find_if(selected_frame.text.begin(), selected_frame.text.end(),
            [](const auto& item) { return item.field.path ==
                "menu_SkillTreeSheetNew/next_skill_description/text"; });
        CHECK(current_description != selected_frame.text.end() &&
              current_description->value == "Current level\n\n<font color=\"#FF0000\">native current result</font>" &&
              current_description->field.character_id == 468 && current_description->field.font_id == 103 &&
              next_description != selected_frame.text.end() &&
              next_description->value == "Next level\n\nnative next result");
        CHECK(selected_training_probe_calls == 1);
        CHECK(std::any_of(selected_frame.art.batches.begin(), selected_frame.art.batches.end(),
            [&](const auto& batch) { return batch.role.compare(0, add_button_prefix.size(), add_button_prefix) == 0; }));
        CHECK(std::count_if(selected_frame.text.begin(), selected_frame.text.end(), [](const auto& item) {
            return item.field.path == "menu_SkillTreeSheetNew/btn_add/AddText/text" &&
                   item.value == "Upgrade Skill";
        }) == 1);
        const auto rank_before_probe = state->skills[0].rank;
        const auto points_before_probe = state->source_skill_points;
        allow_selected_training = false;
        character_menu::Frame rejected_train_frame;
        rejected_train_frame.art.batches = source_art.batches;
        rejected_train_frame.solids = source_art.solids;
        rejected_train_frame.text.push_back({*add_label_field, "stale base Add Skill label"});
        CHECK(provider.append(rejected_train_frame, error));
        CHECK(selected_training_probe_calls == 2);
        CHECK(std::none_of(rejected_train_frame.art.batches.begin(), rejected_train_frame.art.batches.end(),
            [&](const auto& batch) { return batch.role.compare(0, add_button_prefix.size(), add_button_prefix) == 0; }));
        CHECK(std::none_of(rejected_train_frame.text.begin(), rejected_train_frame.text.end(), [](const auto& item) {
            return item.field.path == "menu_SkillTreeSheetNew/btn_add/AddText/text";
        }));
        CHECK(state->skills[0].rank == rank_before_probe && state->source_skill_points == points_before_probe);
        allow_selected_training = true;
        auto stale_frame = selected_frame;
        stale_skill_details = true;
        CHECK(!provider.append(stale_frame, error) &&
              error.find("stale or belongs to another source row/state") != std::string::npos &&
              stale_frame.text.size() == selected_frame.text.size());
        stale_skill_details = false;

        // The source details handler keeps the selected source row, rank, and
        // same CharacterState level together. At a locked RequiredLevel, the
        // current field carries the localized unlock branch and Next stays
        // empty; at an eligible row, current and next retain distinct source
        // values. A false source CanIncrementSkill result only suppresses the
        // next-level body and does not alter rank/points.
        const auto locked_row = std::find_if(class_skills.begin(), class_skills.end(), [&](int id) {
            return id >= 0 && static_cast<std::size_t>(id) < tables.skills().size() &&
                   static_cast<std::uint32_t>(std::max(0, static_cast<int>(
                       tables.skills()[static_cast<std::size_t>(id)].scalar.words[8]))) > state->stats.level;
        });
        CHECK(locked_row != class_skills.end());
        release_position = static_cast<int>(locked_row - class_skills.begin());
        CHECK(provider.release(117.0f, 88.0f, error));
        CHECK(release_calls == 2 && runtime->page().selected_position() == release_position);
        allow_selected_training = false;
        character_menu::Frame locked_frame;
        locked_frame.art.batches = source_art.batches;
        locked_frame.solids = source_art.solids;
        CHECK(provider.append(locked_frame, error));
        const auto required_level = static_cast<std::uint32_t>(std::max(0, static_cast<int>(
            tables.skills()[static_cast<std::size_t>(*locked_row)].scalar.words[8])));
        const auto locked_current = std::find_if(locked_frame.text.begin(), locked_frame.text.end(),
            [](const auto& item) { return item.field.path ==
                "menu_SkillTreeSheetNew/current_skill_description/text"; });
        const auto locked_next = std::find_if(locked_frame.text.begin(), locked_frame.text.end(),
            [](const auto& item) { return item.field.path ==
                "menu_SkillTreeSheetNew/next_skill_description/text"; });
        CHECK(locked_current != locked_frame.text.end() && locked_next != locked_frame.text.end());
        CHECK(locked_current->value == "Current level\n\nUnlock at level " +
              std::to_string(required_level));
        CHECK(locked_next->value == "Next level\n\n");
        const auto locked_icon = "menu_SkillTreeSheetNew/buttons/skill" +
            std::to_string(release_position) + "/Lock/";
        CHECK(std::any_of(locked_frame.art.batches.begin(), locked_frame.art.batches.end(),
            [&](const auto& batch) { return batch.role.compare(0, locked_icon.size(), locked_icon) == 0; }));
        CHECK(std::none_of(locked_frame.art.batches.begin(), locked_frame.art.batches.end(),
            [&](const auto& batch) { return batch.role.compare(0, add_button_prefix.size(), add_button_prefix) == 0; }));
        CHECK(state->skills[static_cast<std::size_t>(release_position)].rank > 0 &&
              state->source_skill_points == points_before_probe);

        // The current row is still eligible, but an exact CanIncrementSkill
        // rejection leaves only the current-level details visible.
        release_position = 0;
        CHECK(provider.release(117.0f, 88.0f, error));
        block_next_level_training = true;
        character_menu::Frame prerequisite_frame;
        prerequisite_frame.art.batches = source_art.batches;
        prerequisite_frame.solids = source_art.solids;
        CHECK(provider.append(prerequisite_frame, error));
        const auto prerequisite_current = std::find_if(prerequisite_frame.text.begin(), prerequisite_frame.text.end(),
            [](const auto& item) { return item.field.path ==
                "menu_SkillTreeSheetNew/current_skill_description/text"; });
        const auto prerequisite_next = std::find_if(prerequisite_frame.text.begin(), prerequisite_frame.text.end(),
            [](const auto& item) { return item.field.path ==
                "menu_SkillTreeSheetNew/next_skill_description/text"; });
        CHECK(prerequisite_current != prerequisite_frame.text.end() && prerequisite_next != prerequisite_frame.text.end());
        CHECK(prerequisite_current->value ==
              "Current level\n\n<font color=\"#FF0000\">native current result</font>");
        CHECK(prerequisite_next->value == "Next level\n\nTraining is unavailable");
        CHECK(state->skills[0].rank == rank_before_probe && state->source_skill_points == points_before_probe);
        block_next_level_training = false;
        allow_selected_training = true;

        auto other = std::make_shared<CharacterState>(*state);
        character_menu::Bindings wrong;
        wrong.character = other.get();
        auto separate_runtime = std::make_shared<RuntimeSkillsMenuV1>(
            state, source_owner, characters, tables, ServicesV1{});
        CHECK(!separate_runtime->install_content(wrong, error));
        CHECK(error.find("same CharacterState") != std::string::npos);
        CHECK(!runtime->install_content(bindings, error));
        CHECK(error.find("already installed") != std::string::npos);

        // Full existing SourceComposition path: register the runtime's exact
        // owner token, install its authored-content dispatcher, select the
        // ready Skills page, append original art/data, and route a real source
        // skill-tree release without supplying rank or action policy.
        auto composed_state = std::make_shared<CharacterState>();
        composed_state->id = "source-composition-state";
        composed_state->class_id = "KnightPlayerBase";
        auto composed_owner = std::static_pointer_cast<void>(composed_state);
        auto composed_runtime = std::make_shared<RuntimeSkillsMenuV1>(
            composed_state, composed_owner, characters, tables, ServicesV1{});
        character_menu::SourceCompositionV1 composition(composed_owner);
        const auto composed_provider = composed_runtime->source_page_provider();
        CHECK(composed_provider.owner.get() == composed_owner.get());
        CHECK(composition.register_page(character_menu::Tab::skills, composed_provider, error));
        character_menu::Bindings composed_bindings;
        composed_bindings.character = composed_state.get();
        unsigned composed_root_calls = 0;
        composed_bindings.content = [&](character_menu::Tab, character_menu::Frame&, std::string&) {
            ++composed_root_calls;
            return true;
        };
        CHECK(composition.install_content(composed_bindings, error));
        character_menu::Presenter composed_presenter;
        composed_presenter.open();
        CHECK(composition.select(composed_presenter, character_menu::Tab::skills, error));
        character_menu::Frame composed_frame;
        CHECK(composed_bindings.content(composed_presenter.tab(), composed_frame, error));
        CHECK(composed_root_calls == 1 && !composed_frame.art.batches.empty());
        CHECK(composed_runtime->source_class_art_available());
        CHECK(std::none_of(composed_frame.text.begin(), composed_frame.text.end(), [](const auto& item) {
            return item.field.path.find("/buttons/skill") != std::string::npos;
        }));
        const auto composed_zones = skill_ui::original_skill_hit_zones(0);
        const auto source_select = std::find_if(composed_zones.begin(), composed_zones.end(), [](const auto& zone) {
            return zone.kind == skill_ui::HitKind::select;
        });
        CHECK(source_select != composed_zones.end());
        const auto source_click = centroid(*source_select);
        CHECK(composed_provider.release(source_click.first, source_click.second, error));
        CHECK(composed_runtime->page().selected_position() == source_select->position);
        CHECK(composed_state->skills.empty() && composed_state->skill_slots.empty() &&
              !composed_state->source_points_known && composed_state->source_skill_points == 0);
        composed_state->source_points_known = true;
        composed_state->source_skill_points = 0;
        character_menu::Frame no_points_frame;
        no_points_frame.art.batches = source_art.batches;
        no_points_frame.solids = source_art.solids;
        no_points_frame.text.push_back({*add_label_field, "stale base Add Skill label"});
        CHECK(composed_provider.append(no_points_frame, error));
        CHECK(std::none_of(no_points_frame.art.batches.begin(), no_points_frame.art.batches.end(),
            [](const auto& batch) { return batch.role.find("menu_SkillTreeSheetNew/btn_add/") == 0; }));
        CHECK(std::none_of(no_points_frame.text.begin(), no_points_frame.text.end(), [](const auto& item) {
            return item.field.path == "menu_SkillTreeSheetNew/btn_add/AddText/text";
        }));

        // Default release routing consumes the recovered source contours and
        // forwards select/assign/train to PageV1 on the same CharacterState.
        auto action_state = std::make_shared<CharacterState>(*state);
        auto action_owner = std::static_pointer_cast<void>(action_state);
        const auto* expected_action_state = action_state.get();
        bool allow_action_training = true;
        ServicesV1 action_services;
        action_services.can_assign = [expected_action_state](const CharacterState& shared, int, int,
                                                             bool& available, std::string&) {
            CHECK(&shared == expected_action_state);
            available = true;
            return true;
        };
        action_services.train = [expected_action_state](CharacterState& shared, int, int position,
                                                        int, std::string&) {
            CHECK(&shared == expected_action_state);
            ++shared.skills.at(static_cast<std::size_t>(position)).rank;
            --shared.source_skill_points;
            return true;
        };
        auto action_runtime = std::make_shared<RuntimeSkillsMenuV1>(
            action_state, action_owner, characters, tables, action_services, RuntimeSkillsRendererV1{},
            [expected_action_state](const CharacterState& shared, std::optional<unsigned>& active, std::string&) {
                CHECK(&shared == expected_action_state);
                active = 1;
                return true;
            },
            RuntimeSkillsReleaseV1{}, RuntimeSkillsSymbolTextV1{},
            [expected_action_state, list_id, &tables, &allow_action_training](const CharacterState& shared, int active_list,
                                                      int position, int table_id, bool& available,
                                                      std::string& e) {
                CHECK(&shared == expected_action_state);
                CHECK(active_list == list_id && position >= 0 &&
                      static_cast<std::size_t>(position) < tables.lists()[active_list].size());
                CHECK(tables.lists()[active_list][position] == table_id);
                available = allow_action_training;
                e.clear();
                return true;
            });
        const auto zones = skill_ui::original_skill_hit_zones(0);
        std::size_t action_position = class_skills.size();
        for (std::size_t i = 0; i < class_skills.size(); ++i) {
            const auto id = class_skills[i];
            if (action_state->skills[i].rank > 0 &&
                tables.skills()[static_cast<std::size_t>(id)].scalar.words[11] != 0) {
                action_position = i;
                break;
            }
        }
        CHECK(action_position < class_skills.size());
        auto find_zone = [&](skill_ui::HitKind kind, int position) -> const skill_ui::HitZone& {
            const auto at = std::find_if(zones.begin(), zones.end(), [&](const auto& zone) {
                return zone.kind == kind && zone.position == position;
            });
            CHECK(at != zones.end());
            return *at;
        };
        const auto action_provider = action_runtime->source_page_provider();
        CHECK(action_provider.ready && action_provider.append && action_provider.release);
        const auto select_point = centroid(find_zone(skill_ui::HitKind::select, static_cast<int>(action_position)));
        CHECK(action_provider.release(select_point.first, select_point.second, error));
        CHECK(action_runtime->page().selected_position() == static_cast<int>(action_position));
        character_menu::Frame fallback_label_frame;
        fallback_label_frame.art.batches = source_art.batches;
        fallback_label_frame.solids = source_art.solids;
        fallback_label_frame.text.push_back({*add_label_field, "Upgrade Skill"});
        CHECK(action_provider.append(fallback_label_frame, error));
        CHECK(std::any_of(fallback_label_frame.art.batches.begin(), fallback_label_frame.art.batches.end(),
            [](const auto& batch) { return batch.role.find("menu_SkillTreeSheetNew/btn_add/") == 0; }));
        CHECK(std::count_if(fallback_label_frame.text.begin(), fallback_label_frame.text.end(), [](const auto& item) {
            return item.field.path == "menu_SkillTreeSheetNew/btn_add/AddText/text" &&
                   item.value == "Upgrade Skill";
        }) == 1);
        const auto assign_point = centroid(find_zone(skill_ui::HitKind::assign, 0));
        CHECK(action_provider.release(assign_point.first, assign_point.second, error));
        CHECK(action_state->skill_slots.back().equipment_set == 1 &&
              action_state->skill_slots.back().slot == 0 &&
              action_state->skill_slots.back().saved_skill_row == action_position);
        const auto train_point = centroid(find_zone(skill_ui::HitKind::train, -1));
        const auto old_rank = action_state->skills[action_position].rank;
        const auto old_points = action_state->source_skill_points;
        CHECK(action_provider.release(train_point.first, train_point.second, error));
        CHECK(action_state->skills[action_position].rank == old_rank + 1 &&
              action_state->source_skill_points + 1 == old_points);
        allow_action_training = false;
        const auto rejected_rank = action_state->skills[action_position].rank;
        const auto rejected_points = action_state->source_skill_points;
        CHECK(!action_provider.release(train_point.first, train_point.second, error));
        CHECK(error.find("source admission rejected") != std::string::npos &&
              action_state->skills[action_position].rank == rejected_rank &&
              action_state->source_skill_points == rejected_points);

        // All three source specialization class rows retain their exact
        // family ClassID (Knight 77, Rogue 120, Mage 94). The family art is
        // selected from that actual table relation and each cell displays its
        // own specialization SkillTable icon, rather than the base list's
        // placeholders or the default `undefined` sprite.
        for (const char* specialized_name : {"KnightPlayerBase_Berserker", "KnightPlayerBase_Paladin",
                                             "RoguePlayerBase_Archer", "RoguePlayerBase_Assassin",
                                             "MagePlayerBase_Illusionist", "MagePlayerBase_Necromancer"}) {
            const auto specialized_class = std::find(characters.names.begin(), characters.names.end(), specialized_name);
            CHECK(specialized_class != characters.names.end());
            const auto row_index = static_cast<std::size_t>(specialized_class - characters.names.begin());
            const int specialized_list = characters.rows[row_index][static_cast<std::size_t>(skill_tree)];
            CHECK(specialized_list >= 0 && static_cast<std::size_t>(specialized_list) < tables.lists().size());
            auto specialized = std::make_shared<CharacterState>();
            specialized->class_id = specialized_name;
            specialized->stats.level = 12;
            for (const auto id : tables.lists()[static_cast<std::size_t>(specialized_list)])
                specialized->skills.push_back({tables.skill_names()[static_cast<std::size_t>(id)], 0});
            auto specialized_owner = std::static_pointer_cast<void>(specialized);
            auto specialized_runtime = std::make_shared<RuntimeSkillsMenuV1>(
                specialized, specialized_owner, characters, tables, ServicesV1{});
            character_menu::Bindings specialized_bindings;
            specialized_bindings.character = specialized.get();
            CHECK(specialized_runtime->install_content(specialized_bindings, error));
            character_menu::Frame specialized_frame;
            specialized_frame.art.batches = source_art.batches;
            specialized_frame.solids = source_art.solids;
            CHECK(specialized_bindings.content(character_menu::Tab::skills, specialized_frame, error));
            CHECK(specialized_runtime->source_class_art_available());
            ViewV1 specialized_view;
            CHECK(specialized_runtime->page().view(specialized_view, error));
            CHECK(specialized_view.character_row == static_cast<int>(row_index));
            CHECK(specialized_view.authored_skill_list_id == specialized_list &&
                  specialized_view.active_skill_list_id == specialized_list);
            for (const auto& row : specialized_view.rows) {
                const auto lock_role = "menu_SkillTreeSheetNew/buttons/skill" +
                    std::to_string(row.position) + "/Lock/";
                const bool lock_rendered = std::any_of(specialized_frame.art.batches.begin(),
                    specialized_frame.art.batches.end(), [&](const auto& batch) {
                        return batch.role.compare(0, lock_role.size(), lock_role) == 0;
                    });
                CHECK(lock_rendered == (specialized->stats.level <
                    static_cast<std::uint32_t>(std::max(0, row.required_level))));
                if (row.source_icon.empty() || row.source_icon == "blank") continue;
                const auto expected_role = "menu_SkillTreeSheetNew/buttons/btn_skill" +
                    std::to_string(row.position) + "/btimg/" + row.source_icon;
                CHECK(std::any_of(specialized_frame.art.batches.begin(), specialized_frame.art.batches.end(),
                    [&](const auto& batch) { return batch.role == expected_role; }));
            }
            const auto expected_grey_count = static_cast<std::size_t>(std::count_if(
                specialized_view.rows.begin(), specialized_view.rows.end(), [](const auto& row) {
                    return !row.source_icon.empty() && row.source_icon != "blank";
                }));
            const auto actual_grey_count = static_cast<std::size_t>(std::count_if(
                specialized_frame.solids.begin(), specialized_frame.solids.end(), [](const auto& solid) {
                    return solid.geometry.role.find("/buttons/skill") != std::string::npos &&
                           solid.geometry.role.find("/Grey/") != std::string::npos;
                }));
            CHECK(actual_grey_count == expected_grey_count);
        }

        // With a source-supported Knight SkillList, rank-zero unlocked cells
        // retain only the authored Grey overlay for rows with real SkillIcon
        // labels; the disabled/untrained art is not synthesized.
        auto grey_state = std::make_shared<CharacterState>();
        grey_state->class_id = "KnightPlayerBase";
        grey_state->stats.level = 99;
        for (const auto id : class_skills)
            grey_state->skills.push_back({tables.skill_names()[static_cast<std::size_t>(id)], 0});
        auto grey_owner = std::static_pointer_cast<void>(grey_state);
        auto grey_runtime = std::make_shared<RuntimeSkillsMenuV1>(
            grey_state, grey_owner, characters, tables, ServicesV1{});
        character_menu::Bindings grey_bindings;
        grey_bindings.character = grey_state.get();
        CHECK(grey_runtime->install_content(grey_bindings, error));
        character_menu::Frame grey_frame;
        grey_frame.art.batches = source_art.batches;
        grey_frame.solids = source_art.solids;
        CHECK(grey_bindings.content(character_menu::Tab::skills, grey_frame, error));
        ViewV1 grey_view;
        CHECK(grey_runtime->page().view(grey_view, error));
        const auto expected_untrained = static_cast<std::size_t>(std::count_if(
            grey_view.rows.begin(), grey_view.rows.end(), [](const auto& row) {
                return row.saved_rank == 0 && !row.source_icon.empty() && row.source_icon != "blank";
            }));
        const auto actual_untrained = static_cast<std::size_t>(std::count_if(
            grey_frame.solids.begin(), grey_frame.solids.end(), [](const auto& solid) {
                return solid.geometry.role.find("/buttons/skill") != std::string::npos &&
                       solid.geometry.role.find("/Grey/") != std::string::npos;
            }));
        CHECK(expected_untrained > 0 && actual_untrained == expected_untrained);
        for (const auto& solid : grey_frame.solids) {
            if (solid.geometry.role.find("/buttons/skill") == std::string::npos ||
                solid.geometry.role.find("/Grey/") == std::string::npos) continue;
            CHECK(std::abs(solid.rgba[3] - 102.0f / 256.0f) < 1.0e-6f);
            CHECK(std::any_of(grey_frame.art.batches.begin(), grey_frame.art.batches.end(),
                [&](const auto& bitmap) { return bitmap.role == solid.after_bitmap_role; }));
            CHECK(solid.after_bitmap_role.find("/btimg/") != std::string::npos);
            CHECK(solid.after_bitmap_role.find("/btimg/1") == std::string::npos);
        }

        // The release screenshot uses this exact base Rogue list/profile.
        // Verify rank-zero icons retain their source Grey overlay after the
        // Rogue class-frame provider replaces the default bitmap role.
        const auto rogue = std::find(characters.names.begin(), characters.names.end(), "RoguePlayerBase");
        CHECK(rogue != characters.names.end());
        const auto rogue_row = static_cast<std::size_t>(rogue - characters.names.begin());
        const auto rogue_list_id = characters.rows[rogue_row][static_cast<std::size_t>(skill_tree)];
        CHECK(rogue_list_id >= 0 && static_cast<std::size_t>(rogue_list_id) < tables.lists().size());
        auto rogue_state = std::make_shared<CharacterState>();
        rogue_state->class_id = "RoguePlayerBase";
        rogue_state->stats.level = 1;
        for (const auto id : tables.lists()[static_cast<std::size_t>(rogue_list_id)])
            rogue_state->skills.push_back({tables.skill_names()[static_cast<std::size_t>(id)], 0});
        auto rogue_owner = std::static_pointer_cast<void>(rogue_state);
        auto rogue_runtime = std::make_shared<RuntimeSkillsMenuV1>(
            rogue_state, rogue_owner, characters, tables, ServicesV1{});
        character_menu::Bindings rogue_bindings;
        rogue_bindings.character = rogue_state.get();
        CHECK(rogue_runtime->install_content(rogue_bindings, error));
        character_menu::Frame rogue_frame;
        rogue_frame.art.batches = source_art.batches;
        rogue_frame.solids = source_art.solids;
        CHECK(rogue_bindings.content(character_menu::Tab::skills, rogue_frame, error));
        CHECK(rogue_runtime->source_class_art_available());
        const auto rogue_grey_count = static_cast<std::size_t>(std::count_if(
            rogue_frame.solids.begin(), rogue_frame.solids.end(), [](const auto& solid) {
                return solid.geometry.role.find("/buttons/skill") != std::string::npos &&
                       solid.geometry.role.find("/Grey/") != std::string::npos;
            }));
        CHECK(rogue_grey_count > 0);
        for (const auto& solid : rogue_frame.solids) {
            if (solid.geometry.role.find("/buttons/skill") == std::string::npos ||
                solid.geometry.role.find("/Grey/") == std::string::npos) continue;
            CHECK(std::abs(solid.rgba[3] - 102.0f / 256.0f) < 1.0e-6f);
            CHECK(std::any_of(rogue_frame.art.batches.begin(), rogue_frame.art.batches.end(),
                [&](const auto& bitmap) { return bitmap.role == solid.after_bitmap_role; }));
        }

        std::cout << "{\"validation\":\"PASS\",\"authored_rows\":" << class_skills.size()
                  << ",\"authored_tree_icons\":" << class_skills.size()
                  << ",\"same_owner_callback\":true,\"source_composition_e2e\":true,\"native_vm_required\":false,\"localized_provider_fixture\":true,\"live_source_localization_claimed\":false}\n";
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << '\n';
        return 1;
    }
}
