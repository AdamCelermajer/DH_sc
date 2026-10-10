#include "runtime_skills_menu_v1.hpp"
#include "../skill_ui/original_skill_art.hpp"
#include "../skill_ui/skill_ui.hpp"
#include "../character_menu/skill_page_text_projection_v1.hpp"

#include <algorithm>
#include <charconv>
#include <limits>
#include <stdexcept>

namespace dh::foundation::generic_skills {
namespace {
bool fail(std::string& error, const char* message) {
    error = message;
    return false;
}

CharacterState& require_state(const std::shared_ptr<CharacterState>& state) {
    if (!state) throw std::invalid_argument("Runtime Skills page requires CharacterState");
    return *state;
}

std::optional<int> skill_position_for_value_field(const std::string& path) {
    const auto marker = path.find("/buttons/skill");
    if (marker == std::string::npos || path.find("/cnt/value") == std::string::npos) return std::nullopt;
    const auto first = marker + std::char_traits<char>::length("/buttons/skill");
    auto last = first;
    while (last < path.size() && path[last] >= '0' && path[last] <= '9') ++last;
    if (last == first) return std::nullopt;
    int value = -1;
    const auto parsed = std::from_chars(path.data() + first, path.data() + last, value);
    if (parsed.ec != std::errc{} || parsed.ptr != path.data() + last) return std::nullopt;
    return value;
}

bool skill_detail_field(const std::string& path) {
    return path.find("/SKILL_NAME/") != std::string::npos ||
           path.find("/skill_description/") != std::string::npos ||
           path.find("/current_skill_description/") != std::string::npos ||
           path.find("/next_skill_description/") != std::string::npos;
}

bool skills_value_field(const std::string& path) {
    return path.find("cp_Skill_Points/") != std::string::npos ||
           path == "menu_SkillTreeSheetNew/btn_add/AddText/text" ||
           skill_position_for_value_field(path).has_value() || skill_detail_field(path);
}

bool source_class_frame_for_row(const dh2::data::CharacterTable& characters,
                                int source_class_row, unsigned& frame) {
    std::string direct_error;
    if (skill_ui::original_class_frame_for_row(source_class_row, frame, direct_error)) return true;

    // Specialization CharacterTable rows retain the same source ClassID as
    // their family base row. Reuse the native SWF family-frame mapping only
    // when that exact table relation identifies one unique supported base;
    // never infer a family from class-name spelling or a UI label.
    const auto class_id_field = std::find(characters.fields.begin(), characters.fields.end(), "ClassID");
    if (class_id_field == characters.fields.end() || source_class_row < 0 ||
        static_cast<std::size_t>(source_class_row) >= characters.rows.size()) return false;
    const auto column = static_cast<std::size_t>(class_id_field - characters.fields.begin());
    if (column >= characters.rows[static_cast<std::size_t>(source_class_row)].size()) return false;
    const auto class_id = characters.rows[static_cast<std::size_t>(source_class_row)][column];
    std::optional<unsigned> family_frame;
    for (std::size_t row = 0; row < characters.rows.size(); ++row) {
        if (column >= characters.rows[row].size() || characters.rows[row][column] != class_id) continue;
        unsigned candidate_frame = 0;
        std::string candidate_error;
        if (!skill_ui::original_class_frame_for_row(static_cast<int>(row), candidate_frame,
                                                     candidate_error)) continue;
        if (family_frame && *family_frame != candidate_frame) return false;
        family_frame = candidate_frame;
    }
    if (!family_frame) return false;
    frame = *family_frame;
    return true;
}
} // namespace

RuntimeSkillsMenuV1::RuntimeSkillsMenuV1(
    std::shared_ptr<CharacterState> same_state,
    std::shared_ptr<void> same_source_owner,
    const dh2::data::CharacterTable& characters,
    dh2::data::SkillTables::Borrow tables,
    ServicesV1 services,
    RuntimeSkillsRendererV1 renderer,
    RuntimeSkillsActiveSetV1 active_set,
    RuntimeSkillsReleaseV1 release,
    RuntimeSkillsSymbolTextV1 symbol_text,
    RuntimeSkillsTrainAvailableV1 train_available)
    : state_(std::move(same_state)), source_owner_(std::move(same_source_owner)),
      characters_(&characters), tables_(tables), skill_level_text_(services.skill_level_text),
      page_(require_state(state_), characters, std::move(tables), std::move(services)),
      renderer_(std::move(renderer)), release_(std::move(release)), active_set_(std::move(active_set)),
      symbol_text_(std::move(symbol_text)), train_available_(std::move(train_available)) {}

bool RuntimeSkillsMenuV1::ready(std::string& error) const {
    error.clear();
    if (!state_) return fail(error, "Runtime Skills page requires the existing shared CharacterState");
    if (!source_owner_) return fail(error, "Runtime Skills page requires the canonical shared source owner token");
    ViewV1 view;
    return page_.view(view, error);
}

bool RuntimeSkillsMenuV1::append(character_menu::Frame& frame, std::string& error) {
    if (!ready(error)) return false;
    ViewV1 view;
    if (!page_.view(view, error)) return false;
    auto next = frame;
    class_art_available_ = false;

    // The exact native class-frame resolver is reused from the source skill UI.
    // It intentionally returns no frame for class rows the original branch
    // does not support; the generic text/data projection remains available.
    unsigned class_frame = 0;
    std::string art_error;
    if (characters_ && source_class_frame_for_row(*characters_, view.character_row, class_frame)) {
        for (const auto& row : view.rows) {
            if (!skill_ui::append_original_skill_icon(class_frame, row.position, row.source_icon,
                                                       next.art, error)) return false;
        }
        class_art_available_ = true;
        if (active_set_) {
            std::optional<unsigned> set;
            if (!active_set_(*state_, set, error)) {
                if (error.empty()) error = "Original active skill equipment-set provider failed";
                return false;
            }
            if (set && *set >= 2)
                return fail(error, "Original active skill equipment set is outside the two saved source sets");
            if (set) {
                for (unsigned slot_id = 0; slot_id < 3; ++slot_id) {
                    const auto binding = std::find_if(view.slots.begin(), view.slots.end(),
                        [&](const SlotV1& slot) { return slot.equipment_set == *set && slot.slot == slot_id; });
                    if (binding == view.slots.end() && !view.slots_source_known) continue;
                    std::string icon = "blank";
                    if (binding != view.slots.end()) {
                        if (!binding->class_skill_position || *binding->class_skill_position < 0 ||
                            static_cast<std::size_t>(*binding->class_skill_position) >= view.rows.size())
                            return fail(error, "Saved skill slot row cannot be resolved in the active original SkillList");
                        icon = view.rows[static_cast<std::size_t>(*binding->class_skill_position)].source_icon;
                    }
                    if (!skill_ui::append_original_skill_slot_icon(class_frame, static_cast<int>(slot_id),
                                                                     icon, next.art, error)) return false;
                }
            }
        }
    }

    // Use the original authored text-field geometry/style. The optional
    // localization seam must be a borrow of the same MenuLocalization owner
    // already used by CharacterMenu; it is never a second text cache.
    const auto& authored = character_menu::original_menu_art(character_menu::Tab::skills);
    next.text.erase(std::remove_if(next.text.begin(), next.text.end(), [this](const auto& item) {
        const bool add_label = item.field.path == "menu_SkillTreeSheetNew/btn_add/AddText/text";
        // When no symbol provider is installed, the shared CharacterMenu
        // frame may already contain its correctly localized label. Keep that
        // value only until the source visibility gate below is known.
        return skills_value_field(item.field.path) && (!add_label || bool(symbol_text_));
    }), next.text.end());
    const auto find_authored_field = [&](const std::string& path) -> const character_menu::MenuTextField* {
        const auto at = std::find_if(authored.text_fields.begin(), authored.text_fields.end(),
            [&](const auto& field) { return field.path == path; });
        return at == authored.text_fields.end() ? nullptr : &*at;
    };
    const auto apply_source_style = [&](character_menu::MenuTextField& field) -> bool {
        character_menu::SkillPageSourceFieldStyleV1 style;
        if (!character_menu::skill_page_source_field_style_v1(field.path, style, error)) return false;
        field.character_id = style.field_character;
        field.font_id = style.font_character;
        for (std::size_t i = 0; i < style.rgba.size(); ++i)
            field.rgba[i] = static_cast<std::uint8_t>(std::clamp(style.rgba[i] * 255.0f, 0.0f, 255.0f) + 0.5f);
        return true;
    };

    character_menu::SkillPageTextInputV1 source_cells;
    bool source_cells_known = true;
    for (auto& cell : source_cells.cells) cell.skill_icon_frame = "blank";
    if (view.rows.size() > source_cells.cells.size())
        return fail(error, "Original Skills source class list exceeds the authored 16-cell layout");
    for (std::size_t position = 0; position < source_cells.cells.size(); ++position) {
        // sprite496 authors exactly sixteen cells, while some source class
        // SkillLists are shorter. The absent tail remains the source blank
        // state; it must not inherit Grey/Lock from the generic data model.
        if (position >= view.rows.size()) continue;
        const auto& row = view.rows[position];
        if (!row.saved_rank) { source_cells_known = false; break; }
        if (row.table_id < 0 || static_cast<std::size_t>(row.table_id) >= tables_.skills().size())
            return fail(error, "Original Skills cell has no same-table SkillTable record");
        if (*row.saved_rank > static_cast<std::uint32_t>(std::numeric_limits<int>::max()))
            return fail(error, "Original Skills rank is outside source display integer range");
        const auto& record = tables_.skills()[static_cast<std::size_t>(row.table_id)];
        source_cells.cells[position] = {
            static_cast<int>(*row.saved_rank),
            state_->stats.level >= static_cast<std::uint32_t>(std::max(0, row.required_level)),
            record.scalar.words[11] != 0,
            row.source_icon.empty() ? "blank" : row.source_icon};
    }
    if (source_cells_known) {
        std::array<character_menu::SkillPageCellPresentationV1, 16> states{};
        if (!character_menu::project_skill_page_cells_v1(source_cells, states, error)) return false;
        for (std::size_t i = 0; i < states.size(); ++i) {
            const auto lock_prefix = "menu_SkillTreeSheetNew/buttons/skill" + std::to_string(i) + "/Lock/";
            const auto grey_prefix = "menu_SkillTreeSheetNew/buttons/skill" + std::to_string(i) + "/Grey/";
            if (!states[i].lock_visible)
                next.art.batches.erase(std::remove_if(next.art.batches.begin(), next.art.batches.end(),
                    [&](const auto& batch) { return batch.role.compare(0, lock_prefix.size(), lock_prefix) == 0; }),
                    next.art.batches.end());
            if (!states[i].grey_visible)
                next.solids.erase(std::remove_if(next.solids.begin(), next.solids.end(),
                    [&](const auto& batch) {
                        return batch.geometry.role.compare(0, grey_prefix.size(), grey_prefix) == 0;
                    }), next.solids.end());
            else if (class_art_available_ && i < view.rows.size()) {
                // The authored Grey fill is a source solid ordered after the
                // default `btimg/1` bitmap. The source icon provider replaces
                // that default bitmap with a batch named `btimg/<SkillIcon>`;
                // main draws a solid only after an exact bitmap-role match.
                // Retarget the same source overlay so the 102/256 alpha tint
                // follows the replacement icon instead of becoming orphaned.
                const auto icon_role = "menu_SkillTreeSheetNew/buttons/skill" +
                    std::to_string(i) + "/btimg/" + view.rows[i].source_icon;
                const auto authored_default_role = "menu_SkillTreeSheetNew/buttons/skill" +
                    std::to_string(i) + "/btimg/1";
                for (auto& solid : next.solids) {
                    const auto grey_role = "menu_SkillTreeSheetNew/buttons/skill" +
                        std::to_string(i) + "/Grey/";
                    if (solid.geometry.role.compare(0, grey_role.size(), grey_role) == 0 &&
                        solid.after_bitmap_role == authored_default_role)
                        solid.after_bitmap_role = icon_role;
                }
            }
        }
        // The source Lock chain is drawn above its cell icon (reference-399).
        // The icon presenter appends source icon batches after the authored
        // Lock, so move the visible Lock batches last, keeping their order.
        std::stable_partition(next.art.batches.begin(), next.art.batches.end(), [&](const auto& batch) {
            for (std::size_t i = 0; i < states.size(); ++i) {
                if (!states[i].lock_visible) continue;
                const auto lock_prefix = "menu_SkillTreeSheetNew/buttons/skill" + std::to_string(i) + "/Lock/";
                if (batch.role.compare(0, lock_prefix.size(), lock_prefix) == 0) return false;
            }
            return true;
        });
    }

    // presetAllSkills initially exposes Add Skill when the same source point
    // query is positive. Once a row is selected, the authored onUp handler
    // replaces that display gate with NativeSkillsTrainSkill(true, position,
    // player 0). A missing source probe is closed by hiding the action art.
    bool show_train_button = !page_.selected_position() && view.skill_points_known &&
        view.skill_points && *view.skill_points > 0;
    if (page_.selected_position()) {
        const auto selected = *page_.selected_position();
        if (selected >= 0 && static_cast<std::size_t>(selected) < view.rows.size() &&
            train_available_) {
            const auto& row = view.rows[static_cast<std::size_t>(selected)];
            if (row.table_id < 0 || view.active_skill_list_id < 0)
                return fail(error, "Selected Skills action has no active source list or SkillTable row");
            if (!train_available_(*state_, view.active_skill_list_id, selected,
                                  row.table_id, show_train_button, error)) {
                if (error.empty()) error = "NativeSkillsTrainSkill availability probe failed";
                return false;
            }
        }
    }
    if (!show_train_button)
        next.art.batches.erase(std::remove_if(next.art.batches.begin(), next.art.batches.end(),
            [](const auto& batch) {
                return batch.role.compare(0, std::string_view("menu_SkillTreeSheetNew/btn_add/").size(),
                                          "menu_SkillTreeSheetNew/btn_add/") == 0;
            }), next.art.batches.end());
    if (!show_train_button)
        next.text.erase(std::remove_if(next.text.begin(), next.text.end(), [](const auto& item) {
            return item.field.path == "menu_SkillTreeSheetNew/btn_add/AddText/text";
        }), next.text.end());

    if (symbol_text_) {
        if (!view.skill_points_known || !view.skill_points)
            return fail(error, "Original Skills labels require the same CharacterState source skill-points result");
        if (!source_cells_known)
            return fail(error, "Original Skills labels require all same-CharacterState source ranks");
        auto text_input = source_cells;
        text_input.symbol_text = symbol_text_;
        if (*view.skill_points > static_cast<std::uint32_t>(std::numeric_limits<int>::max()))
            return fail(error, "Original Skills point value is outside source display integer range");
        text_input.skill_points_left = static_cast<int>(*view.skill_points);
        std::vector<character_menu::SkillPageTextBindingV1> text_bindings;
        if (!character_menu::project_skill_page_text_v1(text_input, text_bindings, error)) return false;
        for (const auto& binding : text_bindings) {
            if (!show_train_button &&
                binding.field_path == "menu_SkillTreeSheetNew/btn_add/AddText/text") continue;
            const auto* source_field = find_authored_field(binding.field_path);
            if (!source_field) return fail(error, "Original Skills text projection references an absent source field");
            auto field = *source_field;
            if (!apply_source_style(field)) return false;
            next.text.push_back({std::move(field), binding.text});
        }
    } else {
        for (const auto& field : authored.text_fields) {
            std::optional<std::string> value;
            if (field.path.find("cp_Skill_Points/") != std::string::npos) {
                if (view.skill_points) value = std::to_string(*view.skill_points);
            } else if (const auto position = skill_position_for_value_field(field.path)) {
                if (*position >= 0 && static_cast<std::size_t>(*position) < view.rows.size()) {
                    const auto& row = view.rows[static_cast<std::size_t>(*position)];
                    if (row.saved_rank) value = std::to_string(*row.saved_rank);
                }
            }
            if (value && !value->empty()) next.text.push_back({field, std::move(*value)});
        }
    }
    if (page_.selected_position()) {
        const auto selected = *page_.selected_position();
        if (selected >= 0 && static_cast<std::size_t>(selected) < view.rows.size()) {
            const auto& row = view.rows[static_cast<std::size_t>(selected)];
            for (const auto& field : authored.text_fields) {
                std::optional<std::string> value;
                if (field.path.find("/SKILL_NAME/") != std::string::npos) value = row.localized_name;
                else if (field.path.find("/skill_description/") != std::string::npos) value = row.description;
                if (value && !value->empty()) {
                    auto styled = field;
                    if (symbol_text_ && !apply_source_style(styled)) return false;
                    next.text.push_back({std::move(styled), std::move(*value)});
                }
            }
            // The source level descriptions are not derivable from the static
            // description or the training delta. Populate them only when the
            // canonical NativeGetSkillDetails provider returns a result for
            // this exact CharacterState/list/position/table/rank/level.
            if (skill_level_text_ && row.saved_rank && row.table_id >= 0 &&
                view.active_skill_list_id >= 0) {
                character_menu::SkillPageCurrentNextRequestV1 request;
                request.state = state_.get();
                request.skill_list_id = view.active_skill_list_id;
                request.class_skill_position = selected;
                request.skill_table_id = row.table_id;
                request.saved_rank = *row.saved_rank;
                request.character_level = state_->stats.level;
                character_menu::SkillPageCurrentNextTextV1 details;
                if (!skill_level_text_(*state_, request, details, error)) {
                    if (error.empty()) error = "NativeGetSkillDetails current/next provider failed";
                    return false;
                }
                std::vector<character_menu::SkillPageTextBindingV1> level_bindings;
                if (!character_menu::project_skill_page_current_next_v1(
                        request, details, symbol_text_, level_bindings, error)) return false;
                for (const auto& binding : level_bindings) {
                    const auto* source_field = find_authored_field(binding.field_path);
                    if (!source_field)
                        return fail(error, "Original Skills current/next projection references an absent source field");
                    auto field = *source_field;
                    if (!apply_source_style(field)) return false;
                    next.text.push_back({std::move(field), binding.text});
                }
            }
        }
    }

    try {
        if (renderer_ && !renderer_(view, page_.selected_position(), next, error)) {
            if (error.empty()) error = "Runtime Skills source renderer rejected the page";
            return false;
        }
    } catch (const std::exception& ex) {
        error = std::string("Runtime Skills source renderer threw: ") + ex.what();
        return false;
    } catch (...) {
        error = "Runtime Skills source renderer threw an unknown exception";
        return false;
    }
    frame = std::move(next);
    error.clear();
    return true;
}

bool RuntimeSkillsMenuV1::release(float x, float y, std::string& error) {
    error.clear();
    if (!ready(error)) return false;
    try {
        if (release_) {
            if (!release_(x, y, page_, error)) {
                if (error.empty()) error = "Runtime Skills source release provider failed";
                return false;
            }
        } else {
            ViewV1 view;
            if (!page_.view(view, error)) return false;
            unsigned class_frame = 0;
            if (!characters_ || !source_class_frame_for_row(*characters_, view.character_row, class_frame))
                return fail(error, "Original Skills release has no source class-family hit frame");
            const auto hit = skill_ui::original_skill_hit(class_frame, x, y);
            if (!hit) { error.clear(); return true; }
            switch (hit->kind) {
            case skill_ui::HitKind::select:
                return page_.select(hit->position, error);
            case skill_ui::HitKind::train: {
                if (!train_available_)
                    return fail(error, "Original skill training requires the NativeSkillsTrainSkill source admission probe");
                if (!page_.selected_position())
                    return fail(error, "Original skill training requires the source-selected skill row");
                ViewV1 selected_view;
                if (!page_.view(selected_view, error)) return false;
                const auto position = *page_.selected_position();
                if (position < 0 || static_cast<std::size_t>(position) >= selected_view.rows.size())
                    return fail(error, "Original selected skill is outside the active source SkillList");
                const auto& row = selected_view.rows[static_cast<std::size_t>(position)];
                bool available = false;
                if (!train_available_(*state_, selected_view.active_skill_list_id, position,
                                      row.table_id, available, error)) {
                    if (error.empty()) error = "NativeSkillsTrainSkill admission probe failed";
                    return false;
                }
                if (!available)
                    return fail(error, "NativeSkillsTrainSkill source admission rejected training");
                return page_.train(error);
            }
            case skill_ui::HitKind::assign: {
                if (!active_set_) return fail(error, "Original skill-slot click requires the source active-set provider");
                std::optional<unsigned> set;
                if (!active_set_(*state_, set, error)) {
                    if (error.empty()) error = "Original active skill equipment-set provider failed";
                    return false;
                }
                if (!set || *set >= 2) return fail(error, "Original active skill equipment set is unavailable");
                return page_.assign(*set, static_cast<unsigned>(hit->position), error);
            }
            }
        }
    } catch (const std::exception& ex) {
        error = std::string("Runtime Skills source release provider threw: ") + ex.what();
        return false;
    } catch (...) {
        error = "Runtime Skills source release provider threw an unknown exception";
        return false;
    }
    error.clear();
    return true;
}

bool RuntimeSkillsMenuV1::install_content(character_menu::Bindings& bindings,
                                           std::string& error) {
    error.clear();
    if (content_installed_) return fail(error, "Runtime Skills content callback is already installed");
    if (!state_ || bindings.character != state_.get())
        return fail(error, "CharacterMenu Bindings must borrow the same CharacterState owner");
    if (!source_owner_) return fail(error, "Runtime Skills page requires the canonical shared source owner token");
    std::shared_ptr<RuntimeSkillsMenuV1> self;
    try {
        self = shared_from_this();
    } catch (const std::bad_weak_ptr&) {
        return fail(error, "Runtime Skills menu must be retained by shared_ptr before binding");
    }
    auto previous = std::move(bindings.content);
    bindings.content = [self = std::move(self), previous = std::move(previous)](
        character_menu::Tab tab, character_menu::Frame& frame, std::string& message) mutable {
        if (previous && !previous(tab, frame, message)) return false;
        if (tab != character_menu::Tab::skills) {
            message.clear();
            return true;
        }
        return self->append(frame, message);
    };
    content_installed_ = true;
    error.clear();
    return true;
}

character_menu::SourcePageProviderV1 RuntimeSkillsMenuV1::source_page_provider() {
    character_menu::SourcePageProviderV1 provider;
    if (!source_owner_) return provider;
    std::shared_ptr<RuntimeSkillsMenuV1> self;
    try {
        self = shared_from_this();
    } catch (const std::bad_weak_ptr&) {
        return provider;
    }
    provider.owner = source_owner_;
    provider.ready = [self](std::string& error) { return self->ready(error); };
    provider.append = [self](character_menu::Frame& frame, std::string& error) {
        return self->append(frame, error);
    };
    provider.release = [self](float x, float y, std::string& error) {
        return self->release(x, y, error);
    };
    return provider;
}

} // namespace dh::foundation::generic_skills
