#include "source_quest_log_services_v1.hpp"
#include <cstring>

namespace dh::foundation {
namespace {
bool fail(std::string& error, const char* message) {
    error = message;
    return false;
}

bool debug_switch(const dh2::character::DebugLevelBinding16& binding,
                  const char* name, bool& out, std::string& error) {
    if (!binding.owner || !binding.files || !binding.files->open_read ||
        !binding.files->close_read)
        return fail(error, "Quest Log requires the live DebugSwitches and file owner");
    const int loaded = dh2_character_debug_load(binding.owner, binding.files);
    if (loaded != 1)
        return fail(error, "Source DebugSwitches::load failed for Quest Log predicate");
    std::uint32_t value{};
    if (dh2_character_debug_get(&value, binding.owner, name, binding.files) != 1)
        return fail(error, "Source DebugSwitches::GetSwitch failed for Quest Log predicate");
    out = value != 0;
    error.clear();
    return true;
}

bool is_debug_quest(const dh2::data::QuestPersistenceStateV51& quest,
                    const dh2::ui::HudTextEnvironmentV1& environment,
                    bool& out, std::string& error) {
    if (!quest.definition)
        return fail(error, "Quest Log source row has no native Quest definition");
    std::int32_t debug_priority{};
    if (!source_quest_log_constant_v1(environment, "v2QuestPriority", "Debug",
                                      debug_priority, error))
        return false;
    out = quest.definition->priority == debug_priority;
    error.clear();
    return true;
}
} // namespace

bool source_quest_log_constant_v1(const dh2::ui::HudTextEnvironmentV1& environment,
                                  const char* group, const char* key,
                                  std::int32_t& value, std::string& error) {
    if (!group || !key || !environment.localization.constant)
        return fail(error, "Quest Log requires the existing source PyData constant owner");
    std::uint32_t raw{};
    if (!environment.localization.constant(environment.localization.context,
                                          group, key, raw, error)) {
        if (error.empty()) error = "Source PyData constant lookup failed";
        return false;
    }
    std::memcpy(&value, &raw, sizeof value);
    error.clear();
    return true;
}

bool bind_source_quest_log_services_v1(
    const dh2::character::DebugLevelBinding16& debug,
    dh2::ui::HudTextV1& text,
    const dh2::ui::HudTextEnvironmentV1& environment,
    QuestLogFunctorV108& functor,
    QuestLogTextV108& localized,
    std::function<bool(const char*,const char*,std::int32_t&,std::string&)>& constant,
    std::string& error) {
    if (!debug.owner || !debug.files || !environment.localization.constant)
        return fail(error, "Quest Log requires actual DebugSwitches and PyData constant owners");

    functor = [debug, &environment](
        const dh2::data::QuestPersistenceStateV51& quest,
        QuestLogCategoryV108 category, bool& include, std::string& callback_error) {
        include = false;
        bool display_all{};
        if (!debug_switch(debug, "DisplayAllQuestInLog", display_all, callback_error))
            return false;
        bool is_debug{};
        if (display_all) {
            if (!is_debug_quest(quest, environment, is_debug, callback_error)) return false;
            include = !is_debug;
            callback_error.clear();
            return true;
        }
        bool display_debug_only{};
        if (!debug_switch(debug, "DisplayAllDebugOnlyQuestInLog", display_debug_only,
                          callback_error))
            return false;
        if (display_debug_only) {
            if (!is_debug_quest(quest, environment, is_debug, callback_error)) return false;
            include = is_debug;
            callback_error.clear();
            return true;
        }

        if (!is_debug_quest(quest, environment, is_debug, callback_error)) return false;
        if (is_debug) {
            callback_error.clear();
            return true;
        }
        if (category == QuestLogCategoryV108::active)
            include = quest.state >= 6 && quest.state <= 12;
        else
            include = quest.state > 12;
        callback_error.clear();
        return true;
    };

    localized = [&text, &environment](std::int32_t id, std::string& value,
                                      std::string& callback_error) {
        std::string staged;
        bool is_null{};
        if (!text.integer_string(id, environment.localization, staged, is_null,
                                 callback_error))
            return false;
        if (is_null || staged == "#!WTF!#" || staged == "#!SNL!#")
            return fail(callback_error, "Original Quest StringID has no localized value");
        value = std::move(staged);
        callback_error.clear();
        return true;
    };

    constant = [&environment](const char* group, const char* key,
                              std::int32_t& value, std::string& callback_error) {
        return source_quest_log_constant_v1(environment, group, key, value,
                                            callback_error);
    };

    error.clear();
    return true;
}

} // namespace dh::foundation
