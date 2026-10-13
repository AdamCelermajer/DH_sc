#include "native_ghost_skill_use_v1.hpp"

#include "../../../../../../port/level-world/script_value_boolean_lua.hpp"

#include <cstring>
#include <deque>
#include <map>
#include <memory>
#include <stdexcept>
#include <vector>

namespace dh2::native::ghost_skill_use_v1 {
namespace check_source = character_ai_skill_script_check;
namespace boolean = script_value_boolean;

namespace {
struct Runtime {
    struct Returns {
        check_source::ValueVector vector{};
        std::vector<boolean::Value> values;
        std::deque<std::string> strings;
        void sync() {
            const auto first = values.empty() ? 0 : reinterpret_cast<std::uintptr_t>(values.data());
            vector = {first, first ? first + values.size() * sizeof(boolean::Value) : 0};
        }
        void erase() {
            values.clear();
            strings.clear();
            sync();
        }
    };

    monster_external_script::Session& session;
    ghost_skills::Runtime& skills;
    std::uintptr_t character;
    script_value_boolean_lua::TemporaryLua bool_lua;
    std::map<std::uintptr_t, std::unique_ptr<Returns>> retained;
    Result* output = nullptr;
    std::string error;

    Runtime(monster_external_script::Session& s, ghost_skills::Runtime& k, std::uintptr_t c)
        : session(s), skills(k), character(c) {}

    static int observe(void* raw, const dh2_script_first_return_v1* values,
                       std::uint32_t count, char*, std::size_t) noexcept {
        auto& r = *static_cast<Returns*>(raw);
        try {
            if (count && !values) return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
            r.erase();
            r.values.reserve(count);
            for (std::uint32_t i = 0; i < count; ++i) {
                const auto& input = values[i];
                boolean::Value value{input.type, 0, input.identity, nullptr};
                std::memcpy(&value.number_word, &input.number, 4);
                if (input.type == DH2_SCRIPT_STRING) {
                    if (!input.text) return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
                    r.strings.emplace_back(input.text, input.text_bytes);
                    value.string = r.strings.back().c_str();
                }
                r.values.push_back(value);
            }
            r.sync();
            return 0;
        } catch (...) {
            return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
        }
    }

    std::uintptr_t slot_script(std::uint32_t slot) const {
        const auto& scripts = skills.skill_scripts();
        if (slot >= scripts.size() || !scripts[slot])
            throw std::invalid_argument("missing source skill slot");
        if (!skills.arguments_for(scripts[slot]))
            throw std::invalid_argument("source skill instance has no retained Arguments");
        return scripts[slot];
    }

    int call(std::uintptr_t skill, const char* function, bool set, Returns& values,
             std::uint32_t& source_error) {
        std::vector<dh2_script_value> args;
        if (set) {
            const auto* original = skills.arguments_for(skill);
            if (!original) throw std::invalid_argument("source Arguments identity changed");
            dh2_script_value name{};
            name.type = DH2_SCRIPT_STRING;
            name.text = original->script_name.c_str();
            name.text_bytes = original->script_name.size();
            dh2_script_value index{};
            index.type = DH2_SCRIPT_NUMBER;
            index.number = original->skill_index;
            args.push_back(name);
            args.push_back(index);
        }
        ++output->call_count;
        const int status = session.call_all(function, args.empty() ? nullptr : args.data(),
                                            std::uint32_t(args.size()), observe, &values, error);
        output->last_lua_status = status;
        if (status < 0) return -1;
        source_error = std::uint32_t(status);
        return 0;
    }

    std::uintptr_t construct() {
        auto r = std::make_unique<Returns>();
        const auto id = reinterpret_cast<std::uintptr_t>(r.get());
        retained.emplace(id, std::move(r));
        return id;
    }
    Returns& returns(std::uintptr_t id) {
        auto it = retained.find(id);
        if (it == retained.end()) throw std::invalid_argument("unknown ReturnValues resource");
        return *it->second;
    }
    bool truth(const boolean::Value* value) {
        auto services = bool_lua.services();
        boolean::Result result{};
        if (boolean::execute(value, &services, &result) != boolean::Status::complete)
            throw std::runtime_error("source Value::getBool failed");
        return result.value != 0;
    }

    static std::int32_t check_invoke(void* raw, check_source::State* state,
                                     const check_source::Request* q,
                                     check_source::ReturnValues* projection,
                                     check_source::Response* response) {
        auto& s = *static_cast<Runtime*>(raw);
        if (!s.output || !q || !state || !projection || !response || state->identity != q->skill)
            return -1;
        try {
            using Op = check_source::Operation;
            if (q->operation == Op::construct_values) {
                projection->resource = s.construct();
                projection->error = 0;
                projection->values = &s.returns(projection->resource).vector;
                return 0;
            }
            auto& r = s.returns(q->resource);
            if (projection->resource != q->resource || projection->values != &r.vector) return -1;
            switch (q->operation) {
            case Op::call_set_skill:
                if (q->script != s.session.vm_identity() || q->arguments != q->skill + 0xc) return -1;
                return s.call(q->skill, q->function, true, r, projection->error);
            case Op::erase_values:
                if (q->first != r.vector.begin || q->last != r.vector.end) return -1;
                r.erase();
                return 0;
            case Op::call_skill_check:
                if (q->script != s.session.vm_identity()) return -1;
                return s.call(q->skill, q->function, false, r, projection->error);
            case Op::operator_index:
                if (q->index || r.values.empty()) return -1;
                response->value = reinterpret_cast<std::uintptr_t>(r.values.data());
                return 0;
            case Op::get_bool: {
                const auto at = q->value, first = r.vector.begin, last = r.vector.end;
                if (!first || at < first || at >= last || (at - first) % sizeof(boolean::Value))
                    return -1;
                response->boolean = s.truth(reinterpret_cast<const boolean::Value*>(at));
                return 0;
            }
            case Op::destroy_values:
                s.retained.erase(q->resource);
                return 0;
            default:
                return -1;
            }
        } catch (const std::exception& e) {
            s.error = e.what();
            return -1;
        } catch (...) {
            s.error = "skill-check provider exception";
            return -1;
        }
    }

    int invoke(std::uintptr_t skill, Callback callback) {
        // The original callers construct before the owner's script gate.
        const auto id = construct();
        auto& r = returns(id);
        std::uint32_t source_error = 0;
        if (!session.ready()) {
            output->lua_error = true;
        } else {
            if (call(skill, "SetSkill", true, r, source_error)) return -1;
            if (source_error) {
                output->lua_error = true;
            } else {
                if (r.vector.begin != r.vector.end) r.erase();
                if (!session.ready()) {
                    error = "missing fresh source LuaScript";
                    return -1;
                }
                const char* name = callback == Callback::pre ? "OnPreSkill"
                    : callback == Callback::use ? "OnSkill" : "OnPostSkill";
                if (call(skill, name, false, r, source_error)) return -1;
                if (callback == Callback::post) {
                    // OnPostSkill's returns and errors are discarded.
                } else if (source_error) {
                    output->lua_error = true;
                } else if (r.values.empty()) {
                    output->value = 1;
                } else {
                    output->value = truth(r.values.data()) ? 1u : 0u;
                }
            }
        }
        retained.erase(id);
        return 0;
    }
};
} // namespace

int check(monster_external_script::Session& session, ghost_skills::Runtime& skills,
          std::uintptr_t character, std::uint32_t slot, Check kind, Result& out,
          std::string& error) {
    out = {};
    if (!character || !session.ready()) {
        error = "monster skill check requires a ready same-Character AIS VM";
        return -1;
    }
    try {
        Runtime runtime(session, skills, character);
        const auto skill = runtime.slot_script(slot);
        runtime.output = &out;
        check_source::Character owner{character, session.vm_identity()};
        check_source::State state{skill, &owner};
        const check_source::Services services{&runtime, sizeof(boolean::Value),
                                              Runtime::check_invoke};
        check_source::Result source{};
        const auto status = check_source::check(
            &state, kind == Check::usable ? check_source::Check::usable : check_source::Check::active,
            &services, &source);
        out.value = source.boolean;
        error = runtime.error;
        if (status != check_source::Status::complete) {
            if (error.empty()) error = "source monster skill check failed";
            return -1;
        }
        return 0;
    } catch (const std::exception& exception) {
        error = exception.what();
        return -1;
    } catch (...) {
        error = "monster skill check exception";
        return -1;
    }
}

int invoke(monster_external_script::Session& session, ghost_skills::Runtime& skills,
           std::uintptr_t character, std::uint32_t slot, Callback callback, Result& out,
           std::string& error) {
    out = {};
    if (!character || !session.ready()) {
        error = "monster skill callback requires a ready same-Character AIS VM";
        return -1;
    }
    try {
        Runtime runtime(session, skills, character);
        const auto skill = runtime.slot_script(slot);
        runtime.output = &out;
        const int status = runtime.invoke(skill, callback);
        error = runtime.error;
        return status;
    } catch (const std::exception& exception) {
        error = exception.what();
        return -1;
    } catch (...) {
        error = "monster skill callback exception";
        return -1;
    }
}

} // namespace dh2::native::ghost_skill_use_v1
