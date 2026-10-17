#pragma once

#include <cstddef>
#include <cstdint>
#include <climits>
#include <string>
#include <utility>

namespace dh2::class_specialization_selection_v1 {

enum class Status : std::uint8_t { complete, invalid_argument, failed };
enum class Stage : std::uint8_t {
    not_started, load_properties, set_player_class, reload_skills, save, complete
};

struct Services {
    void* context = nullptr;
    bool (*load_properties_for_class)(void*, std::int32_t, std::string&) = nullptr;
    bool (*set_player_class)(void*, std::int32_t, std::string&) = nullptr;
    bool (*reload_skills)(void*, std::string&) = nullptr;
    bool (*save)(void*, std::string&) = nullptr;
};

struct Result {
    Stage stage = Stage::not_started;
    std::int32_t previous_class = -1, selected_class = -1, selector = -1;
    std::uint32_t calls = 0;
};

// NativeSelectClassSpec chooses currentClass + 1 + selector. The authored UI
// only supplies 0 or 1. Invalid selections are rejected before any owner is
// called. Valid source order is LoadPropertiesForClassSelect, SG_SetPlayerClass,
// AI_ReloadSkills, SG_Save; a later owner failure preserves the source prefix.
inline Status select(std::int32_t current_class, std::int32_t selector,
                     std::size_t class_count, const Services& services,
                     Result* output, std::string& error) {
    if (!output) return Status::invalid_argument;
    *output = {};
    error.clear();
    if (current_class < 0 || selector < 0 || selector > 1 ||
        class_count > static_cast<std::size_t>(INT32_MAX) ||
        !services.load_properties_for_class || !services.set_player_class ||
        !services.reload_skills || !services.save) {
        error = "NativeSelectClassSpec requires selector 0/1 and complete live owners";
        return Status::invalid_argument;
    }
    const auto target = std::int64_t(current_class) + 1 + selector;
    if (target < 0 || target >= std::int64_t(class_count)) {
        error = "NativeSelectClassSpec target is outside the CharacterTable";
        return Status::invalid_argument;
    }
    output->previous_class = current_class;
    output->selector = selector;
    output->selected_class = static_cast<std::int32_t>(target);
    const auto invoke = [&](Stage stage, auto callback, auto&&... args) {
        output->stage = stage;
        ++output->calls;
        try {
            if (callback(services.context, std::forward<decltype(args)>(args)...,
                         error)) return true;
        } catch (...) {
            if (error.empty()) error = "NativeSelectClassSpec owner threw";
        }
        if (error.empty()) error = "NativeSelectClassSpec owner failed";
        return false;
    };
    if (!invoke(Stage::load_properties, services.load_properties_for_class,
                output->selected_class) ||
        !invoke(Stage::set_player_class, services.set_player_class,
                output->selected_class) ||
        !invoke(Stage::reload_skills, services.reload_skills) ||
        !invoke(Stage::save, services.save)) return Status::failed;
    output->stage = Stage::complete;
    error.clear();
    return Status::complete;
}

} // namespace dh2::class_specialization_selection_v1
