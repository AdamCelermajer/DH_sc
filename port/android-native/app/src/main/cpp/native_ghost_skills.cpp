#include "native_ghost_skills.hpp"

#include <algorithm>
#include <cstring>
#include <limits>
#include <memory>

namespace dh2::native::ghost_skills {
namespace Source = dh2::character_ai_set_skills_and_spells;
namespace Selector = dh2::character_faery_selection;
namespace {

constexpr std::int32_t kSkillTreeProperty = 28;
constexpr std::int32_t kFaeryListProperty = 29;
constexpr std::uint32_t kMaximumRows = 1u << 20;

std::int32_t read_property(data::PropertyView* view, std::int32_t id,
                           std::int32_t* value) {
    if (!view || !value || dh2_property_resolve(view, id, value)) return 1;
    return 0;
}

std::size_t fallback_row(std::int32_t source_id, std::size_t count,
                         std::size_t fallback) {
    if (source_id >= 0 && static_cast<std::uint32_t>(source_id) < count)
        return static_cast<std::size_t>(source_id);
    return fallback;
}
} // namespace

struct Runtime::ScratchArguments {
    struct Entry {
        enum class Kind : std::uint8_t { text, integer, number } kind;
        std::string text;
        std::uint32_t integer = 0;
        float number = 0.0f;
    };
    std::vector<Entry> entries;
};

struct Runtime::ScriptInstance {
    character_ai_skill_script_constructor::State constructor{};
    SkillArguments arguments{};
    std::uintptr_t embedded_arguments = 0;
};

Runtime::Runtime() = default;

Runtime::~Runtime() {
    for (auto* argument : arguments_) delete argument;
    // Source CharAI::AIUnLoadScriptProcess destroys each nonnull pointer through
    // CharAISkillScript D0 and resets the vector end. These host-owned records
    // project that ownership: clearing unique_ptrs releases their embedded
    // Arguments strings (D1) before the containing AIS VM owner is destroyed.
    script_instances_.clear();
    skill_scripts_.clear();
    faery_scripts_.clear();
}

bool Runtime::build_selector_tables(const Bindings& bindings) {
    if (!bindings.faeries || bindings.faeries->faery_lists.empty() ||
        bindings.faeries->faeries.empty() ||
        bindings.faeries->faery_lists.size() > kMaximumRows ||
        bindings.faeries->faeries.size() > kMaximumRows)
        return false;

    selector_lists_.clear();
    selector_faeries_.clear();
    faery_script_names_.clear();
    selector_lists_.reserve(bindings.faeries->faery_lists.size());
    selector_faeries_.reserve(bindings.faeries->faeries.size());
    faery_script_names_.reserve(bindings.faeries->faeries.size());

    for (const auto& list : bindings.faeries->faery_lists) {
        if (list.members.size() > static_cast<std::size_t>(INT32_MAX)) return false;
        selector_lists_.push_back(Selector::FaeryListRow{
            0u, static_cast<std::int32_t>(list.members.size()), list.members.data()});
    }
    for (const auto& source : bindings.faeries->faeries) {
        Selector::FaeryRow row{};
        // The original native row has a pointer at +0x18. The 32-bit proof
        // shape keeps that word zero here; the typed host-width companion
        // carries its value, and the Ghost rows below all take the null gate.
        row.words[5] = source.spell_script_length == 0 ? 0u : 1u;
        row.words[6] = 0u;
        row.words[8] = static_cast<std::uint32_t>(source.type);
        selector_faeries_.push_back(row);
        faery_script_names_.push_back(source.spell_script.empty()
            ? 0u : reinterpret_cast<std::uintptr_t>(source.spell_script.c_str()));
    }
    selector_tables_ = {selector_lists_.data(),
                        static_cast<std::uint32_t>(selector_lists_.size()),
                        selector_faeries_.data(),
                        static_cast<std::uint32_t>(selector_faeries_.size())};
    selector_globals_ = {&selector_tables_, bindings.assert_level};
    selector_services_ = {this, get_faery_count, report_faery_assertion};
    full_width_names_ = {selector_faeries_.data(), faery_script_names_.data(),
                         faery_script_names_.size()};
    return true;
}

void Runtime::sync_vectors(Source::State& state) {
    const auto sync = [](std::vector<std::uintptr_t>& owned, Source::ScriptVector& source) {
        auto* begin = owned.empty() && owned.capacity() == 0 ? nullptr : owned.data();
        source.begin = begin;
        source.end = begin ? begin + owned.size() : nullptr;
        source.capacity = begin ? begin + owned.capacity() : nullptr;
    };
    sync(skill_scripts_, state.skills);
    sync(faery_scripts_, state.faeries);
}

std::int32_t Runtime::get_faery_count(void* raw, Selector::Character*,
                                      const Selector::Request* request,
                                      Selector::Response* response) {
    auto* self = static_cast<Runtime*>(raw);
    if (!self || !self->active_bindings_ || !request || !response ||
        request->operation != Selector::Operation::faery_types_count ||
        !request->category || !request->key ||
        std::strcmp(request->category, "FaeryTypes") ||
        std::strcmp(request->key, "COUNT"))
        return 1;
    const auto* constants = self->active_bindings_->faery_constants;
    if (!constants) return 1;
    dh2_pycst_result found{};
    if (dh2_pycst_get(constants, request->category,
                      static_cast<std::uint32_t>(std::strlen(request->category)),
                      request->key, static_cast<std::uint32_t>(std::strlen(request->key)),
                      &found) || !found.found)
        return 1;
    response->word = found.value;
    return 0;
}

std::int32_t Runtime::report_faery_assertion(void*, Selector::Character*,
                                            const Selector::Request*) {
    // The authored Ghost list/table fixtures satisfy all selector invariants
    // at assert level 0. Reaching a nonfatal source diagnostic requires a real
    // game logger binding; fail visibly instead of reporting fake success.
    return 1;
}

std::int32_t Runtime::construct_invoke(
    void* raw, character_ai_skill_script_constructor::State* state,
    const character_ai_skill_script_constructor::Request* request) {
    auto* self = static_cast<Runtime*>(raw);
    if (!self || !state || !request || !self->active_bindings_) return 1;
    const auto found = std::find_if(self->script_instances_.begin(),
        self->script_instances_.end(), [&](const auto& record) {
            return reinterpret_cast<std::uintptr_t>(record.get()) == state->identity;
        });
    if (found == self->script_instances_.end() ||
        state->identity != reinterpret_cast<std::uintptr_t>(found->get())) return 1;
    auto& instance = **found;
    switch (request->operation) {
    case character_ai_skill_script_constructor::Operation::arguments_construct:
        if (request->arguments_identity != state->identity + 0x0cu ||
            instance.embedded_arguments) return 1;
        instance.embedded_arguments = request->arguments_identity;
        return 0;
    case character_ai_skill_script_constructor::Operation::arguments_push_string:
        if (request->arguments_identity != instance.embedded_arguments ||
            !request->text) return 1;
        instance.arguments.script_name.assign(request->text);
        return 0;
    case character_ai_skill_script_constructor::Operation::arguments_push_integer:
        if (request->arguments_identity != instance.embedded_arguments) return 1;
        instance.arguments.skill_index = static_cast<float>(request->integer);
        return 0;
    case character_ai_skill_script_constructor::Operation::report_assertion:
        return 1;
    }
    return 1;
}

std::int32_t Runtime::construct_assertion(
    void* raw, character_ai_skill_script_constructor::State* state,
    const character_ai_skill_script_constructor::Request* request) {
    if (!raw || !state || !request) return 1;
    // Nonfatal assertions need the real source logger. Ghost skill rows in the
    // shipped cache satisfy the constructor's Character/name assertions.
    return 1;
}

std::int32_t Runtime::invoke(void* raw, Source::State* state,
                            const Source::Request* request,
                            Source::Response* response) {
    if (!raw || !state || !request || !response) return 1;
    return static_cast<Runtime*>(raw)->perform(*state, *request, *response);
}

std::int32_t Runtime::perform(Source::State& state, const Source::Request& request,
                              Source::Response& response) {
    if (!active_bindings_ || !active_result_ || !active_bindings_->ai ||
        state.ai != active_bindings_->ai->identity ||
        state.owner != active_bindings_->ai->owner_04)
        return 1;
    auto& bindings = *const_cast<Bindings*>(active_bindings_);
    const auto select_list = [&](Source::List list, std::int32_t property_id,
                                 std::size_t source_count, std::size_t fallback,
                                 const std::vector<data::IntegerListRow>* rows,
                                 std::size_t& selected) -> bool {
        std::int32_t id = 0;
        if (read_property(bindings.properties, property_id, &id)) return false;
        if (list == Source::List::skill) active_result_->skill_list_property = id;
        else active_result_->faery_list_property = id;
        selected = fallback_row(id, source_count, fallback);
        return rows && selected < rows->size();
    };

    switch (request.operation) {
    case Source::Operation::debug_load: {
        if (!bindings.debug_globals || !bindings.debug_services ||
            !bindings.debug_globals->singleton)
            return 1;
        ++active_result_->debug_loads;
        return bindings.debug_globals->singleton->load(*bindings.debug_globals,
                                                       *bindings.debug_services) ==
                       debug_switches::Status::complete ? 0 : 1;
    }
    case Source::Operation::debug_get_switch: {
        if (!bindings.debug_globals || !bindings.debug_services ||
            !bindings.debug_globals->singleton || !request.text ||
            request.text_size > (1u << 20))
            return 1;
        ++active_result_->debug_queries;
        const std::string key(request.text, request.text_size);
        std::uint8_t ignored = 0;
        return bindings.debug_globals->singleton->get_switch(
                   key, *bindings.debug_globals, *bindings.debug_services, ignored) ==
                       debug_switches::Status::complete ? 0 : 1;
    }
    case Source::Operation::capture_script_path:
        if (!bindings.ais_script_path || path_snapshot_live_) return 1;
        saved_ais_path_ = *bindings.ais_script_path;
        path_snapshot_live_ = true;
        response.identity = reinterpret_cast<std::uintptr_t>(&saved_ais_path_);
        response.path = saved_ais_path_.data();
        response.path_size = saved_ais_path_.size();
        return 0;
    case Source::Operation::set_script_path:
        if (!bindings.ais_script_path) return 1;
        if (request.saved_path) {
            if (!path_snapshot_live_ ||
                request.saved_path != saved_ais_path_.data() ||
                request.saved_path_size != saved_ais_path_.size())
                return 1;
            bindings.ais_script_path->assign(request.saved_path, request.saved_path_size);
            return 0;
        }
        if (!request.text || request.text_size > (1u << 20)) return 1;
        bindings.ais_script_path->assign(request.text, request.text_size);
        return 0;
    case Source::Operation::release_script_path:
        if (!path_snapshot_live_ ||
            request.script_name != reinterpret_cast<std::uintptr_t>(&saved_ais_path_))
            return 1;
        path_snapshot_live_ = false;
        saved_ais_path_.clear();
        return 0;
    case Source::Operation::get_skill_list: {
        std::size_t selected = 0;
        if (!bindings.skills || bindings.skills->skill_lists.empty() ||
            !select_list(Source::List::skill, kSkillTreeProperty,
                         bindings.skills->skill_lists.size(), 3,
                         &bindings.skills->skill_lists, selected))
            return 1;
        const auto& members = bindings.skills->skill_lists[selected].members;
        if (members.size() > kMaximumRows) return 1;
        response.count = static_cast<std::uint32_t>(members.size());
        return 0;
    }
    case Source::Operation::get_skill: {
        std::size_t selected = 0;
        if (!bindings.skills || bindings.skills->skill_lists.empty() ||
            !select_list(Source::List::skill, kSkillTreeProperty,
                         bindings.skills->skill_lists.size(), 3,
                         &bindings.skills->skill_lists, selected))
            return 1;
        const auto& members = bindings.skills->skill_lists[selected].members;
        if (request.slot >= members.size()) return 1;
        const auto row_index = members[request.slot];
        if (row_index < 0 || static_cast<std::size_t>(row_index) >= bindings.skills->skills.size())
            return 1;
        const auto& row = bindings.skills->skills[static_cast<std::size_t>(row_index)];
        response.skill.script_gate = row.script_length == 0 ? 0u : 1u;
        response.skill.script_name = row.script.empty() ? 0u :
            reinterpret_cast<std::uintptr_t>(row.script.c_str());
        return 0;
    }
    case Source::Operation::get_faery_list: {
        std::size_t selected = 0;
        if (!bindings.faeries || bindings.faeries->faery_lists.empty() ||
            !select_list(Source::List::faery, kFaeryListProperty,
                         bindings.faeries->faery_lists.size(), 0,
                         &bindings.faeries->faery_lists, selected))
            return 1;
        const auto& members = bindings.faeries->faery_lists[selected].members;
        if (members.size() > kMaximumRows) return 1;
        response.count = static_cast<std::uint32_t>(members.size());
        return 0;
    }
    case Source::Operation::get_faery_list_id: {
        std::int32_t id = 0;
        if (read_property(bindings.properties, kFaeryListProperty, &id)) return 1;
        active_result_->faery_list_property = id;
        std::memcpy(&response.word, &id, sizeof(id));
        return 0;
    }
    case Source::Operation::reserve: {
        auto& target = request.list == Source::List::skill ? skill_scripts_ : faery_scripts_;
        if (request.list == Source::List::none || request.integer > kMaximumRows || !target.empty())
            return 1;
        try { target.reserve(request.integer); }
        catch (...) { return 1; }
        sync_vectors(state);
        return 0;
    }
    case Source::Operation::append_skill_script: {
        auto& target = request.list == Source::List::skill ? skill_scripts_ : faery_scripts_;
        if (request.list == Source::List::none || target.size() >= target.capacity())
            return 1;
        try { target.push_back(request.script_name); }
        catch (...) { return 1; }
        sync_vectors(state);
        return 0;
    }
    case Source::Operation::arguments_construct: {
        try {
            auto scratch = std::make_unique<ScratchArguments>();
            auto* identity = scratch.get();
            arguments_.push_back(identity);
            response.identity = reinterpret_cast<std::uintptr_t>(identity);
            (void)scratch.release();
            ++active_result_->arguments_created;
            return response.identity ? 0 : 1;
        } catch (...) { return 1; }
    }
    case Source::Operation::arguments_push_string:
    case Source::Operation::arguments_push_integer: {
        const auto embedded = std::find_if(script_instances_.begin(), script_instances_.end(),
            [&](const auto& instance) {
                return instance->embedded_arguments == request.arguments;
            });
        if (embedded != script_instances_.end()) {
            if (request.operation == Source::Operation::arguments_push_string) {
                if ((!request.text && request.text_size)) return 1;
                (*embedded)->arguments.script_name.assign(request.text ? request.text : "",
                                                           request.text_size);
            } else {
                (*embedded)->arguments.skill_index = static_cast<float>(request.integer);
            }
            return 0;
        }
        auto found = std::find_if(arguments_.begin(), arguments_.end(), [&](auto* p) {
            return reinterpret_cast<std::uintptr_t>(p) == request.arguments;
        });
        if (found == arguments_.end()) return 1;
        ScratchArguments::Entry entry{};
        if (request.operation == Source::Operation::arguments_push_string) {
            if ((!request.text && request.text_size) || request.text_size > (1u << 20)) return 1;
            entry.kind = ScratchArguments::Entry::Kind::text;
            if (request.text_size) entry.text.assign(request.text, request.text_size);
        } else {
            entry.kind = ScratchArguments::Entry::Kind::integer;
            entry.integer = request.integer;
        }
        try { (*found)->entries.push_back(std::move(entry)); }
        catch (...) { return 1; }
        return 0;
    }
    case Source::Operation::arguments_set_string:
    case Source::Operation::arguments_set_number: {
        auto found = std::find_if(arguments_.begin(), arguments_.end(), [&](auto* p) {
            return reinterpret_cast<std::uintptr_t>(p) == request.arguments;
        });
        if (found == arguments_.end() || (*found)->entries.size() != 2) return 1;
        ScratchArguments::Entry entry{};
        if (request.operation == Source::Operation::arguments_set_string) {
            const std::string* name = nullptr;
            for (const auto& row : bindings.skills->skills)
                if (reinterpret_cast<std::uintptr_t>(row.script.c_str()) == request.script_name) {
                    name = &row.script; break;
                }
            if (!name) for (const auto& row : bindings.faeries->faeries)
                if (reinterpret_cast<std::uintptr_t>(row.spell_script.c_str()) == request.script_name) {
                    name = &row.spell_script; break;
                }
            if (!name) return 1;
            entry.kind = ScratchArguments::Entry::Kind::text;
            entry.text = *name;
            (*found)->entries[0] = std::move(entry);
        } else {
            entry.kind = ScratchArguments::Entry::Kind::number;
            std::memcpy(&entry.number, &request.number_bits, sizeof(entry.number));
            (*found)->entries[1] = std::move(entry);
        }
        return 0;
    }
    case Source::Operation::load_script: {
        if (!bindings.script_context || !bindings.load_script) return 1;
        const std::string* name = nullptr;
        std::string supplied;
        if (request.text) {
            supplied.assign(request.text, request.text_size);
        } else {
            for (const auto& row : bindings.skills->skills)
                if (reinterpret_cast<std::uintptr_t>(row.script.c_str()) == request.script_name) {
                    name = &row.script; break;
                }
            if (!name) for (const auto& row : bindings.faeries->faeries)
                if (reinterpret_cast<std::uintptr_t>(row.spell_script.c_str()) == request.script_name) {
                    name = &row.spell_script; break;
                }
            if (!name) return 1;
            supplied = *name;
        }
        if (supplied.empty() || supplied.size() > (1u << 20)) return 1;
        bool loaded = false;
        if (bindings.load_script(bindings.script_context, supplied.data(), supplied.size(), &loaded)) return 1;
        response.loaded = loaded ? 1u : 0u;
        return 0;
    }
    case Source::Operation::call_script: {
        if (!bindings.script_context || !bindings.call_script || !request.text ||
            request.text_size > 128) return 1;
        dh2_script_value values[2]{};
        std::uint32_t count = 0;
        if (request.arguments) {
            auto found = std::find_if(arguments_.begin(), arguments_.end(), [&](auto* p) {
                return reinterpret_cast<std::uintptr_t>(p) == request.arguments;
            });
            if (found == arguments_.end() || (*found)->entries.size() != 2) return 1;
            const auto& entries = (*found)->entries;
            if (entries[0].kind != ScratchArguments::Entry::Kind::text ||
                entries[1].kind != ScratchArguments::Entry::Kind::number) return 1;
            values[0].type = DH2_SCRIPT_STRING;
            values[0].text = entries[0].text.c_str();
            values[0].text_bytes = entries[0].text.size();
            values[1].type = DH2_SCRIPT_NUMBER;
            values[1].number = entries[1].number;
            count = 2;
        }
        const std::string function(request.text, request.text_size);
        return bindings.call_script(bindings.script_context, function.c_str(),
                                    count ? values : nullptr, count);
    }
    case Source::Operation::allocate_skill_script: {
        if (request.allocation_bytes != 0x1cu || !request.script_name ||
            !active_bindings_->ai || !state.owner) return 1;
        try {
            auto instance = std::make_unique<ScriptInstance>();
            const auto identity = reinterpret_cast<std::uintptr_t>(instance.get());
            instance->constructor.identity = identity;
            script_instances_.push_back(std::move(instance));
            response.identity = identity;
            return 0;
        } catch (...) { return 1; }
    }
    case Source::Operation::release_skill_script_allocation: {
        const auto found = std::find_if(script_instances_.begin(), script_instances_.end(),
            [&](const auto& instance) {
                return reinterpret_cast<std::uintptr_t>(instance.get()) == request.script_name;
            });
        if (found == script_instances_.end()) return 1;
        script_instances_.erase(found);
        return 0;
    }
    case Source::Operation::arguments_destroy: {
        auto found = std::find_if(arguments_.begin(), arguments_.end(), [&](auto* p) {
            return reinterpret_cast<std::uintptr_t>(p) == request.arguments;
        });
        if (found == arguments_.end()) return 1;
        delete *found;
        arguments_.erase(found);
        ++active_result_->arguments_destroyed;
        return 0;
    }
    case Source::Operation::init_vcb: {
        if (!bindings.init_vcb || !active_bindings_->ai) return 1;
        const auto active = active_bindings_->ai->active_ais_1c;
        if (!active) return 1;
        ++active_result_->init_vcb_calls;
        return bindings.init_vcb(bindings.init_vcb_context, active);
    }
    }
    return 1;
}

Status Runtime::prepare(const Bindings& bindings, Result& result) {
    result = {};
    result.source_status = Source::Status::invalid_argument;
    if (active_bindings_ || !arguments_.empty() || path_snapshot_live_ ||
        !bindings.ai || !bindings.ai->identity ||
        !bindings.ai->owner_04 || !bindings.properties || !bindings.skills ||
        !bindings.faeries || !bindings.ais_script_path || !bindings.faery_constants ||
        !bindings.debug_globals || !bindings.debug_services ||
        !bindings.debug_globals->singleton || !bindings.init_vcb ||
        (bindings.skills->skill_lists.size() && bindings.skills->skill_lists.size() > kMaximumRows))
        return Status::invalid_argument;
    if (prepared_ && (prepared_ai_identity_ != bindings.ai->identity ||
                      prepared_character_identity_ != bindings.ai->owner_04))
        return Status::invalid_argument;
    if (!bindings.script_context || !bindings.load_script || !bindings.call_script ||
        !build_selector_tables(bindings)) return Status::invalid_source_tables;
    const auto faery_size_before = faery_scripts_.size();
    if ((faery_size_before != 0 && faery_size_before != 5) ||
        std::any_of(faery_scripts_.begin(), faery_scripts_.end(),
                    [](std::uintptr_t identity) { return identity != 0; }))
        return Status::invalid_source_tables;
    if (skill_scripts_.size() > kMaximumRows ||
        std::any_of(skill_scripts_.begin(),skill_scripts_.end(),[&](std::uintptr_t identity) {
            return identity && !arguments_for(identity);
        })) return Status::invalid_source_tables;
    active_bindings_ = &bindings;
    active_result_ = &result;
    struct Reset {
        Runtime& self;
        ~Reset() {
            self.path_snapshot_live_ = false;
            self.saved_ais_path_.clear();
            self.active_result_ = nullptr;
            self.active_bindings_ = nullptr;
        }
    } reset{*this};

    Source::State state{};
    state.ai = bindings.ai->identity;
    state.owner = bindings.ai->owner_04;
    state.assert_level = bindings.assert_level;
    state.skills.identity = reinterpret_cast<std::uintptr_t>(&skill_scripts_);
    state.faeries.identity = reinterpret_cast<std::uintptr_t>(&faery_scripts_);
    state.faery_binding.character.identity = state.owner;
    state.faery_binding.globals = &selector_globals_;
    state.faery_binding.services = &selector_services_;
    state.faery_binding.full_width_script_names = &full_width_names_;
    sync_vectors(state);

    const character_ai_skill_script_constructor::Globals constructor_globals{
        bindings.assert_level};
    const character_ai_skill_script_constructor::Services constructor_services{
        this, construct_invoke, construct_assertion};
    Source::Services services{this, invoke, &constructor_services,
                              &constructor_globals};
    result.source_status = Source::prepare(&state, &services, &result.source);
    if (result.source_status != Source::Status::complete) return Status::source_failed;
    if (faery_scripts_.size() != 5 ||
        std::any_of(faery_scripts_.begin(), faery_scripts_.end(),
                    [](std::uintptr_t identity) { return identity != 0; }) ||
        result.source.skill_slots != skill_scripts_.size() ||
        result.source.faery_slots != (faery_size_before == 0 ? 5u : 0u) ||
        result.source.null_appends != (faery_size_before == 0 ? 5u : 0u) ||
        result.arguments_created != result.arguments_destroyed)
        return Status::invalid_source_tables;
    prepared_ = true;
    prepared_ai_identity_ = bindings.ai->identity;
    prepared_character_identity_ = bindings.ai->owner_04;
    return Status::complete;
}

const Runtime::SkillArguments* Runtime::arguments_for(std::uintptr_t script) const noexcept {
    if (!script) return nullptr;
    for (const auto& instance : script_instances_)
        if (reinterpret_cast<std::uintptr_t>(instance.get()) == script &&
            instance->constructor.identity == script && instance->embedded_arguments == script + 0x0cu)
            return &instance->arguments;
    return nullptr;
}

Status Runtime::cleanup_death_list(Source::List list,
                                   DeathCleanupResult& output) const noexcept {
    if (!prepared_ || (list != Source::List::skill && list != Source::List::faery))
        return Status::invalid_argument;
    DeathCleanupResult result{};
    result.list = list;
    const auto& scripts = list == Source::List::skill ? skill_scripts_ : faery_scripts_;
    if (scripts.size() > kMaximumRows) return Status::invalid_source_tables;
    for (const auto script : scripts) {
        ++result.slots_examined;
        if (script) {
            output = result;
            return Status::unsupported_script_dependency;
        }
        ++result.null_slots;
    }
    result.completed = 1;
    output = result;
    return Status::complete;
}

} // namespace dh2::native::ghost_skills
