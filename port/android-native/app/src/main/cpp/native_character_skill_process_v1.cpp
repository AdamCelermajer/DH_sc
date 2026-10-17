#include "native_character_skill_process_v1.hpp"
#include "../../../../../../port/level-world/character_skill_state_queries.hpp"

#include <cstdio>

namespace dh2::native::character_skill_process_v1 {
namespace {
int discard_returns(void*, const dh2_script_first_return_v1*, std::uint32_t,
                    char*, std::size_t) { return 0; }

} // namespace

void Adapter::services(process::SkillServices& output) noexcept {
    output = {this, &Adapter::prepare, &Adapter::update};
}

bool Adapter::valid(factory::Record& record,
                    monster_external_script::Session& session) const noexcept {
    return bindings_.runtime && bindings_.ai && bindings_.properties &&
        bindings_.skill_tables && bindings_.faery_tables && bindings_.script_path &&
        bindings_.faery_constants && bindings_.debug_globals &&
        bindings_.debug_services && bindings_.debug_globals->singleton &&
        bindings_.load_script && record.source_external_ais.get() == &session &&
        session.ready() && session.vm_identity() &&
        record.character.identity && record.coordinator &&
        record.coordinator->owner() == record.character.identity &&
        bindings_.ai->identity && bindings_.ai->owner_04 == record.character.identity &&
        record.components.find(factory::ctor::Component::ai) == bindings_.ai &&
        record.external_lifecycle.owner == record.character.identity &&
        record.external_lifecycle.active &&
        record.external_ais.identity == record.external_lifecycle.active &&
        bindings_.ai->active_ais_1c == record.external_lifecycle.active &&
        *bindings_.script_path == record.source_external_script_path;
}

int Adapter::load_script(void* raw, const char* name, std::size_t bytes,
                         bool* loaded) {
    auto* self = static_cast<Adapter*>(raw);
    if (!self || !self->active_session_ || !loaded || (!name && bytes) ||
        !self->bindings_.load_script) return 1;
    try {
        return self->bindings_.load_script(self->bindings_.script_loader_context,
            *self->active_session_, name, bytes, loaded);
    } catch (...) { return 1; }
}

int Adapter::call_script(void* raw, const char* function,
                         const dh2_script_value* arguments, std::uint32_t count) {
    auto* self = static_cast<Adapter*>(raw);
    if (!self || !self->active_session_ || !function || (count && !arguments)) return 1;
    try {
        self->error_.clear();
        const int status = self->active_session_->call_all(function, arguments,
            count, discard_returns, nullptr, self->error_);
        return status == 0 ? 0 : 1;
    } catch (...) { return 1; }
}

int Adapter::contains_alias(void* raw, ais_external_init_vcb::State* state,
                            const char* name, bool* present) {
    auto* self = static_cast<Adapter*>(raw);
    if (!self || !self->active_record_ || !self->active_session_ || !state ||
        state->ais != self->active_record_->external_ais.identity || !name || !present)
        return 1;
    return self->active_session_->contains_source_alias(name, *present) ? 0 : 1;
}

int Adapter::initialize_vcb(void* raw, std::uintptr_t active) {
    auto* self = static_cast<Adapter*>(raw);
    if (!self || !self->active_record_ || !self->active_session_ ||
        active != self->active_record_->external_ais.identity ||
        active != self->active_record_->external_lifecycle.active) return 1;
    auto& ais = self->active_record_->external_ais;
    ais_external_init_vcb::State state{ais.identity, ais.flags_b8};
    const ais_external_init_vcb::Services services{self, &Adapter::contains_alias};
    ais_external_init_vcb::Result result{};
    if (ais_external_init_vcb::initialize_external(&state, &services, &result) !=
        ais_external_init_vcb::Status::complete) return 1;
    ais.flags_b8 = state.flags_b8;
    return 0;
}

int Adapter::prepare(void* raw, factory::Record& record,
                     monster_external_script::Session& session,
                     process::set_skills::Result* output) {
    auto* self = static_cast<Adapter*>(raw);
    if (!self || !output || !self->valid(record, session)) return 1;
    self->active_record_ = &record;
    self->active_session_ = &session;
    self->result_ = {};
    self->error_.clear();
    const ghost_skills::Bindings bindings{
        self->bindings_.ai, self->bindings_.properties,
        self->bindings_.skill_tables, self->bindings_.faery_tables,
        self->bindings_.script_path, self->bindings_.assert_level,
        self->bindings_.faery_constants, self->bindings_.debug_globals,
        self->bindings_.debug_services, self, initialize_vcb,
        self, load_script, call_script};
    ghost_skills::Status status = ghost_skills::Status::service_failed;
    try {
        status = self->bindings_.runtime->prepare(bindings,
                                                  self->result_.preparation);
        *output = self->result_.preparation.source;
    } catch (...) {
        self->error_ = "Ghost SetSkillsAndSpells adapter threw";
        self->active_record_ = nullptr;
        self->active_session_ = nullptr;
        return 1;
    }
    self->active_record_ = nullptr;
    self->active_session_ = nullptr;
    return status == ghost_skills::Status::complete ? 0 : 1;
}

std::int32_t Adapter::update_operation(
        void* raw, character_ai_update_all_skills::State* state,
        const character_ai_update_all_skills::Request* request,
        character_ai_update_all_skills::Response* response) {
    auto* self = static_cast<Adapter*>(raw);
    if (!self || !self->active_record_ || !self->active_session_ || !state ||
        !request || !response || state->ai != self->bindings_.ai->identity ||
        state->owner != self->active_record_->character.identity) return 1;
    using Operation = character_ai_update_all_skills::Operation;
    if (request->operation == Operation::is_using_skill ||
        request->operation == Operation::is_casting) {
        const character_skill_state_queries::Machine machine{
            &self->active_record_->coordinator->state.current};
        character_skill_state_queries::Result result{};
        const auto query = request->operation == Operation::is_using_skill
            ? character_skill_state_queries::Query::using_skill
            : character_skill_state_queries::Query::casting;
        if (character_skill_state_queries::query(query, &machine, &result) !=
            character_skill_state_queries::Status::complete) return 1;
        response->word = result.value;
        return 0;
    }
    if (request->operation != Operation::on_skill_update ||
        (request->list != character_ai_update_all_skills::List::skill &&
         request->list != character_ai_update_all_skills::List::faery)) return 1;
    self->error_.clear();
    return ghost_skill_update_v1::update(*self->active_session_,
        *self->bindings_.runtime, state->owner, request->subject,
        self->result_.last_script_update, self->error_);
}

int Adapter::update(void* raw, factory::Record& record,
                    monster_external_script::Session& session,
                    process::update_skills::Result* output) {
    auto* self = static_cast<Adapter*>(raw);
    if (!self || !output || !self->valid(record, session) ||
        !self->bindings_.runtime->prepared() ||
        self->bindings_.runtime->skill_scripts().size() > (1u << 20) ||
        self->bindings_.runtime->faery_scripts().size() > (1u << 20)) return 1;
    self->active_record_ = &record;
    self->active_session_ = &session;
    self->error_.clear();
    const auto range = [](const std::vector<std::uintptr_t>& vector) {
        const auto* begin = vector.empty() ? nullptr : vector.data();
        return character_ai_update_all_skills::ScriptVector{
            begin, begin ? begin + vector.size() : nullptr};
    };
    character_ai_update_all_skills::State state{
        self->bindings_.ai->identity, record.character.identity,
        range(self->bindings_.runtime->skill_scripts()),
        range(self->bindings_.runtime->faery_scripts())};
    const character_ai_update_all_skills::Services services{
        self, &Adapter::update_operation};
    const auto status = character_ai_update_all_skills::update(
        &state, &services, &self->result_.update);
    *output = self->result_.update;
    self->active_record_ = nullptr;
    self->active_session_ = nullptr;
    return status == character_ai_update_all_skills::Status::complete ? 0 : 1;
}

} // namespace dh2::native::character_skill_process_v1
