#include "native_ghost_skill_update_v1.hpp"

#include <cstdio>
#include <cstring>
#include <map>
#include <memory>
#include <stdexcept>

namespace dh2::native::ghost_skill_update_v1 {
namespace script_update = character_ai_skill_script_update;

struct Runtime {
    struct Returns {
        script_update::ValueVector projection{};
        std::vector<dh2_script_value> values;
        std::vector<std::string> texts;
        void sync() {
            const auto first = values.empty() ? 0u :
                reinterpret_cast<std::uintptr_t>(values.data());
            projection = {first, first ? first + values.size() * sizeof(dh2_script_value) : 0u};
        }
    };

    monster_external_script::Session& session;
    ghost_skills::Runtime& skills;
    std::uintptr_t character;
    std::uintptr_t script;
    std::map<std::uintptr_t, std::unique_ptr<Returns>> returns;
    std::string error;
    std::uint32_t lua_errors = 0;
    int last_lua_status = 0;

    Runtime(monster_external_script::Session& source_session,
            ghost_skills::Runtime& skill_owner, std::uintptr_t character_id,
            std::uintptr_t script_id)
        : session(source_session), skills(skill_owner), character(character_id),
          script(script_id) {}

    static int observe(void* raw, const dh2_script_first_return_v1* input,
                       std::uint32_t count, char* error, std::size_t capacity) {
        auto& output = *static_cast<Returns*>(raw);
        if (count && !input) return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
        try {
            output.values.clear();
            output.texts.clear();
            output.texts.resize(count);
            output.values.reserve(count);
            for (std::uint32_t i = 0; i < count; ++i) {
                dh2_script_value value{};
                value.type = input[i].type;
                value.number = input[i].number;
                value.boolean = input[i].boolean;
                value.identity = input[i].identity;
                if (input[i].type == DH2_SCRIPT_STRING) {
                    if (!input[i].text) throw std::runtime_error("Ghost skill return string is missing");
                    output.texts[i].assign(input[i].text, input[i].text_bytes);
                    value.text = output.texts[i].c_str();
                    value.text_bytes = output.texts[i].size();
                }
                output.values.push_back(value);
            }
            output.sync();
            return 0;
        } catch (...) {
            output.sync();
            if (error && capacity)
                std::snprintf(error, capacity, "%s", "Ghost skill return storage failed");
            return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
        }
    }

    static std::int32_t invoke(void* raw, script_update::State* state,
                               const script_update::Request* request,
                               script_update::ReturnValues* value) {
        auto& self = *static_cast<Runtime*>(raw);
        if (!state || !request || !value || state->identity != self.script ||
            !state->owner || state->owner->identity != self.character) return -1;
        if (request->operation == script_update::Operation::construct_values) {
            if (value->resource || value->values) return -1;
            auto resource = std::make_unique<Returns>();
            const auto id = reinterpret_cast<std::uintptr_t>(resource.get());
            if (!id) return -1;
            value->resource = id;
            value->error = 0;
            value->values = &resource->projection;
            self.returns.emplace(id, std::move(resource));
            return 0;
        }
        if (!value->resource) return -1;
        auto found = self.returns.find(request->resource);
        if (found == self.returns.end() || value->resource != request->resource ||
            value->values != &found->second->projection) return -1;
        auto& result = *found->second;
        if (request->operation == script_update::Operation::erase_values) {
            if (request->first != result.projection.begin ||
                request->last != result.projection.end) return -1;
            result.values.clear();
            result.texts.clear();
            result.sync();
            return 0;
        }
        if (request->operation == script_update::Operation::destroy_values) {
            self.returns.erase(found);
            return 0;
        }
        const char* function = request->function;
        if (!function || (request->operation == script_update::Operation::call_set_skill &&
                           std::strcmp(function, "SetSkill")) ||
            (request->operation == script_update::Operation::call_on_skill_update &&
                           std::strcmp(function, "OnSkillUpdate")) ||
            (request->operation == script_update::Operation::call_on_skill_cleanup &&
                           std::strcmp(function, "OnSkillCleanUp")) ||
            request->skill != self.script ||
            request->script != self.session.vm_identity()) return -1;
        dh2_script_value arguments[2]{};
        std::uint32_t count = 0;
        if (request->operation == script_update::Operation::call_set_skill) {
            const auto* stored = self.skills.arguments_for(request->skill);
            if (!stored || request->arguments != request->skill + 0x0cu ||
                stored->script_name.empty()) return -1;
            arguments[0].type = DH2_SCRIPT_STRING;
            arguments[0].text = stored->script_name.c_str();
            arguments[0].text_bytes = stored->script_name.size();
            arguments[1].type = DH2_SCRIPT_NUMBER;
            arguments[1].number = stored->skill_index;
            count = 2;
        } else if (request->operation != script_update::Operation::call_on_skill_update &&
                   request->operation != script_update::Operation::call_on_skill_cleanup) {
            return -1;
        }
        self.last_lua_status = self.session.call_all(function,
            count ? arguments : nullptr, count, observe, &result, self.error);
        if (self.last_lua_status < 0) return -1;
        value->error = static_cast<std::uint32_t>(self.last_lua_status);
        if (self.last_lua_status) {
            ++self.lua_errors;
            result.values.clear();
            result.texts.clear();
            result.sync();
        }
        return 0;
    }
};

static int run(monster_external_script::Session& session, ghost_skills::Runtime& skills,
               std::uintptr_t character, std::uintptr_t script,
               Result& output, std::string& error, bool cleanup_call) {
    output = {};
    if (!character || !script || !session.ready() ||
        !skills.arguments_for(script)) {
        error = "Ghost skill update has no retained Character, script, VM or Arguments";
        return -1;
    }
    try {
        Runtime runtime(session, skills, character, script);
        script_update::Character owner{character, session.vm_identity()};
        script_update::State state{script, &owner};
        const script_update::Services services{&runtime, Runtime::invoke};
        const auto status = cleanup_call
            ? script_update::cleanup(&state, &services, &output.source)
            : script_update::update(&state, &services, &output.source);
        output.lua_errors = runtime.lua_errors;
        output.last_lua_status = runtime.last_lua_status;
        error = runtime.error;
        if (status != script_update::Status::complete) {
            if (error.empty()) error = "source Ghost CharAISkillScript update failed";
            return -1;
        }
        return 0;
    } catch (const std::exception& exception) {
        error = exception.what();
        return -1;
    } catch (...) {
        error = "Ghost skill update provider threw";
        return -1;
    }
}

int update(monster_external_script::Session& session, ghost_skills::Runtime& skills,
           std::uintptr_t character, std::uintptr_t script,
           Result& output, std::string& error) {
    return run(session, skills, character, script, output, error, false);
}

int cleanup(monster_external_script::Session& session, ghost_skills::Runtime& skills,
            std::uintptr_t character, std::uintptr_t script,
            Result& output, std::string& error) {
    return run(session, skills, character, script, output, error, true);
}

int cleanup_list(monster_external_script::Session& session,
                 ghost_skills::Runtime& skills, std::uintptr_t character,
                 character_ai_set_skills_and_spells::List list,
                 CleanupResult& output, std::string& error) {
    output = {};
    const auto* scripts = list == character_ai_set_skills_and_spells::List::skill
        ? &skills.skill_scripts()
        : list == character_ai_set_skills_and_spells::List::faery
            ? &skills.faery_scripts() : nullptr;
    if (!scripts || !character) {
        error = "Ghost skill cleanup has no retained list or Character";
        return -1;
    }
    try {
        for (const auto script : *scripts) {
            ++output.slots_examined;
            if (!script) {
                ++output.null_slots;
                continue;
            }
            if (!session.ready()) {
                error = "Ghost skill cleanup has a nonnull script but no retained VM";
                return -1;
            }
            Result result{};
            if (cleanup(session, skills, character, script, result, error)) return -1;
            output.lua_errors += result.lua_errors;
            output.callbacks += result.source.cleaned;
        }
        error.clear();
        return 0;
    } catch (const std::exception& exception) {
        error = exception.what();
        return -1;
    } catch (...) {
        error = "Ghost skill cleanup list provider threw";
        return -1;
    }
}

} // namespace dh2::native::ghost_skill_update_v1
