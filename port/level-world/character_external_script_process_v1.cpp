#include "character_external_script_process_v1.hpp"

#include "ais_external_init_callbacks.hpp"
#include "ais_external_init_vcb.hpp"
#include "monster_external_script_session.hpp"

#include <cstring>
#include <stdexcept>

namespace dh2::character_external_script_process_v1 {
namespace {
using Component = factory::ctor::Component;
using Lifecycle = character::ScriptLifecycleState64;
using Request = character::ScriptLifecycleRequest32;
using Response = character::ScriptLifecycleResponse16;
using Service = character::ScriptLifecycleService;

std::int32_t signed_word(std::uint32_t value) noexcept {
    std::int32_t result{};
    std::memcpy(&result, &value, sizeof(result));
    return result;
}

Lifecycle ai_state(const character_ai_initialization::State& ai) noexcept {
    return {ai.owner_04, ai.active_ais_1c, ai.alternate_ais_20,
        ai.script_name_30, signed_word(static_cast<std::uint32_t>(ai.pointer_28)),
        signed_word(ai.word_10), signed_word(ai.word_14), ai.byte_24,
        ai.byte_2c, 0, 0, 0};
}

bool same_state(const Lifecycle& a, const Lifecycle& b) noexcept {
    return a.owner == b.owner && a.active == b.active && a.pending == b.pending &&
        a.external_name == b.external_name && a.load_step == b.load_step &&
        a.timer33 == b.timer33 && a.timer34 == b.timer34 &&
        a.delayed == b.delayed && a.scripted == b.scripted;
}

void store_state(factory::Record& record, character_ai_initialization::State& ai,
                 const Lifecycle& state) {
    if (state.owner != record.character.identity || ai.owner_04 != state.owner)
        throw std::runtime_error("external CharAI owner changed during source dispatch");
    ai.active_ais_1c = state.active;
    ai.alternate_ais_20 = state.pending;
    ai.script_name_30 = state.external_name;
    ai.pointer_28 = static_cast<std::uint32_t>(state.load_step);
    ai.word_10 = static_cast<std::uint32_t>(state.timer33);
    ai.word_14 = static_cast<std::uint32_t>(state.timer34);
    ai.byte_24 = static_cast<std::uint8_t>(state.delayed);
    ai.byte_2c = static_cast<std::uint8_t>(state.scripted);
    record.external_lifecycle = state;
}

bool valid_record(factory::Record& record,
                  character_ai_initialization::State& ai,
                  std::int32_t selected_ai_id,
                  const data::AiProps& row,
                  std::string& error) {
    if (!record.character.identity || record.constructor.identity != record.character.identity ||
        record.game_object.identity != record.character.identity || !record.object_registered ||
        !record.character_listed || !record.constructor.constructor_complete ||
        record.components.find(Component::ai) != &ai || !ai.identity ||
        ai.owner_04 != record.character.identity || !row.delayed_load || row.delayed_load > 1 ||
        row.script.empty() ||
        (row.script.size() >= 2 && row.script[0] == '_' && row.script[1] == '_') ||
        selected_ai_id < 0 ||
        !record.nonplayer_ai_postload_complete ||
        record.nonplayer_source_ai_id != selected_ai_id ||
        record.source_delay_init_3ec != row.delayed_load || ai.byte_24 != row.delayed_load) {
        error = "external script process requires the selected canonical Character, delayed AITable row, CharAI and source AI selection receipt";
        return false;
    }
    if (!record.source_external_ais &&
        (ai.pointer_28 != 0 || ai.active_ais_1c || ai.alternate_ais_20 ||
         record.external_ais.identity)) {
        error = "external AIS Session is absent after the source create step should have published it";
        return false;
    }
    const Lifecycle projected = ai_state(ai);
    auto& retained = record.external_lifecycle;
    if (!retained.owner && retained.load_step == 0 && !retained.active && !retained.pending) {
        retained = projected;
    } else if (!same_state(retained, projected)) {
        error = "Record external lifecycle diverged from its canonical CharAI projection";
        return false;
    }
    return true;
}

struct Dispatch {
    factory::Record* record{};
    character_ai_initialization::State* ai{};
    const data::AiProps* row{};
    const Services* services{};
    Lifecycle* state{};
    Result* result{};
    std::string* error{};
    bool failed{};
};

struct Failure {};

[[noreturn]] void fail(Dispatch& dispatch, const char* message) {
    dispatch.failed = true;
    if (dispatch.error && dispatch.error->empty()) *dispatch.error = message;
    throw Failure{};
}

struct ExternalCallbacks {
    monster_external_script::Session* session{};
    std::string* error{};
};

int call_external(void* raw, ais_external_init_callbacks::State* state,
                  const ais_external_init_callbacks::Request* request) {
    auto& callbacks = *static_cast<ExternalCallbacks*>(raw);
    if (!callbacks.session || !callbacks.error || !state || !request ||
        state->ais != request->ais) return 1;
    using Event = monster_external_script::Event;
    Event event{};
    switch (request->callback) {
    case ais_external_init_callbacks::Callback::init: event = Event::init; break;
    case ais_external_init_callbacks::Callback::post: event = Event::init_post; break;
    case ais_external_init_callbacks::Callback::final: event = Event::init_final; break;
    default: return 1;
    }
    return callbacks.session->dispatch(event, 0, *callbacks.error) ==
        monster_external_script::Status::complete ? 0 : 1;
}

int contains_external(void* raw, ais_external_init_vcb::State* state,
                      const char* alias, bool* member) {
    auto& callbacks = *static_cast<ExternalCallbacks*>(raw);
    if (!callbacks.session || !state || !alias || !member) return 1;
    bool found = false;
    if (!callbacks.session->contains_source_alias(alias, found)) return 1;
    *member = found;
    return 0;
}

void invoke(void* raw, Lifecycle* state, const Request* request,
            Response* response) {
    auto& dispatch = *static_cast<Dispatch*>(raw);
    const bool create_step = request &&
        request->service == character::script_create_step;
    if (!dispatch.record || !dispatch.ai || !dispatch.row || !dispatch.services ||
        !dispatch.result || !dispatch.error || !state || state != dispatch.state ||
        !request || !response ||
        (!dispatch.record->source_external_ais && !create_step))
        fail(dispatch, "external CharAI lifecycle lost its canonical Record/Session");

    auto* const session_before = dispatch.record->source_external_ais.get();
    if (create_step && (state->load_step != 0 || state->active || state->pending))
        fail(dispatch, "AISExternal CreateStep did not start from the selected row's empty CharAI state");

    ++dispatch.result->lifecycle_calls;
    dispatch.result->last_lifecycle_service = request->service;
    try {
        switch (request->service) {
        case character::script_owner_is_character:
            if (request->subject != dispatch.record->character.identity)
                fail(dispatch, "external CharAI Character-owner query changed identity");
            response->word = 1;
            break;
        case character::script_pending_init_vcb: {
            auto& ais = dispatch.record->external_ais;
            auto* session = dispatch.record->source_external_ais.get();
            if (request->subject != state->pending || !state->pending ||
                ais.identity != state->pending || !session || !session->ready())
                fail(dispatch, "external pending InitVCB identity is not canonical");
            ais_external_init_vcb::State projection{ais.identity, ais.flags_b8};
            const ais_external_init_vcb::Services vcb_services{
                session, contains_external};
            ais_external_init_vcb::Result result{};
            if (ais_external_init_vcb::initialize_external(&projection,
                    &vcb_services, &result) != ais_external_init_vcb::Status::complete)
                fail(dispatch, "source AISExternal::InitVCB failed");
            ais.flags_b8 = projection.flags_b8;
            break;
        }
        case character::script_ais_init:
        case character::script_ais_init_post:
        case character::script_ais_init_final: {
            auto& ais = dispatch.record->external_ais;
            auto* session = dispatch.record->source_external_ais.get();
            if (request->subject != state->active && request->subject != state->pending)
                fail(dispatch, "external AIS lifecycle callback identity is not canonical");
            if (ais.identity != request->subject || !session || !session->ready())
                fail(dispatch, "external AIS lifecycle callback has no matching loaded Session");
            const auto callback = request->service == character::script_ais_init
                ? ais_external_init_callbacks::Callback::init
                : request->service == character::script_ais_init_post
                    ? ais_external_init_callbacks::Callback::post
                    : ais_external_init_callbacks::Callback::final;
            ais_external_init_callbacks::State projection{ais.identity};
            const ais_external_init_callbacks::Services callback_services{
                session, call_external};
            ais_external_init_callbacks::Result callback_result{};
            if (ais_external_init_callbacks::invoke(&projection, callback,
                    &callback_services, &callback_result) !=
                    ais_external_init_callbacks::Status::complete)
                fail(dispatch, "source AISExternal initialization callback failed");
            break;
        }
        case character::script_ai_init_post:
        case character::script_ai_init_final: {
            if (request->subject != state->owner)
                fail(dispatch, "CharAI lifecycle wrapper changed Character identity");
            const auto operation = request->service == character::script_ai_init_post
                ? character::script_on_init_post : character::script_on_init_final;
            const character::ScriptLifecycleServices16 nested{&dispatch, &invoke};
            if (dh2_character_script_lifecycle(state,
                    static_cast<std::uint32_t>(operation), 0, &nested) != 1)
                fail(dispatch, "source CharAI OnInitPost/OnInitFinal wrapper failed");
            break;
        }
        case character::script_ai_init: {
            const character::ScriptLifecycleServices16 nested{&dispatch, &invoke};
            if (dh2_character_script_lifecycle(state,
                    character::script_on_init, 0, &nested) != 1)
                fail(dispatch, "source CharAI::OnInit timer/AIS sequence failed");
            break;
        }
        case character::script_configure_skills: {
            const auto& skills = dispatch.services->skills;
            auto* session = dispatch.record->source_external_ais.get();
            if (!skills.prepare || !session || !session->ready() || !state->active ||
                session->vm_identity() != dispatch.result->vm)
                fail(dispatch, "CharAI::SetSkillsAndSpells providers are unavailable");
            if (skills.prepare(skills.context, *dispatch.record, *session,
                    &dispatch.result->set_skills) != 0)
                fail(dispatch, "source CharAI::SetSkillsAndSpells provider failed");
            dispatch.result->set_skills_called = true;
            break;
        }
        case character::script_update_skills: {
            const auto& skills = dispatch.services->skills;
            auto* session = dispatch.record->source_external_ais.get();
            if (!skills.update || !session || !session->ready() || !state->active ||
                session->vm_identity() != dispatch.result->vm)
                fail(dispatch, "CharAI::UpdateAllSkills providers are unavailable");
            if (skills.update(skills.context, *dispatch.record, *session,
                    &dispatch.result->update_skills) != 0)
                fail(dispatch, "source CharAI::UpdateAllSkills provider failed");
            dispatch.result->update_skills_called = true;
            break;
        }
        default: {
            if (!dispatch.services->invoke)
                fail(dispatch, "external Character/CharAI lifecycle provider is unavailable");
            store_state(*dispatch.record, *dispatch.ai, *state);
            const character::ScriptLifecycleServices16 nested{&dispatch, &invoke};
            if (dispatch.services->invoke(dispatch.services->context,
                    *dispatch.record, dispatch.record->source_external_ais.get(),
                    state, request, response, &nested) != 0)
                fail(dispatch, "external Character/CharAI source lifecycle provider failed");
            if (create_step) {
                auto* const created = dispatch.record->source_external_ais.get();
                if (!created || (session_before && created != session_before) ||
                    !created->vm_identity() ||
                    created->stage() != monster_external_script::Stage::created ||
                    state->active || !state->pending || !state->scripted ||
                    state->external_name != reinterpret_cast<std::uintptr_t>(
                        dispatch.row->script.c_str()) ||
                    dispatch.record->external_ais.identity != state->pending ||
                    !dispatch.record->external_ais.dispatch_table) {
                    fail(dispatch, "AISExternal CreateStep did not publish one created-stage VM and the selected external script on this Record");
                }
            } else if (session_before &&
                       dispatch.record->source_external_ais.get() != session_before) {
                fail(dispatch, "source lifecycle replaced the Record's retained AIS Session");
            }
            if (dispatch.record->source_external_ais) {
                const auto created_vm = dispatch.record->source_external_ais->vm_identity();
                if (!created_vm || (dispatch.result->vm && dispatch.result->vm != created_vm))
                    fail(dispatch, "source lifecycle replaced or failed to initialize its retained AIS VM");
                dispatch.result->vm = created_vm;
            }
            store_state(*dispatch.record, *dispatch.ai, *state);
            break;
        }
        }
        // Source may mutate CharAI through its external service; publish the
        // kernel's live state after every operation, including skill calls.
        store_state(*dispatch.record, *dispatch.ai, *state);
    } catch (...) {
        try { store_state(*dispatch.record, *dispatch.ai, *state); } catch (...) {}
        dispatch.failed = true;
        if (dispatch.error->empty())
            *dispatch.error = "external Character/CharAI lifecycle callback failed";
        throw;
    }
}

Status run(factory::Record& record, character_ai_initialization::State& ai,
           std::int32_t ai_id, const data::AiProps& row,
           const Services& services, character::ScriptLifecycleOperation operation,
           bool init_final, Result* output, std::string& error) {
    error.clear();
    if (output) *output = {};
    if (!output || !valid_record(record, ai, ai_id, row, error)) {
        if (error.empty()) error = "external script process requires a valid output";
        return Status::invalid_record;
    }
    if (!services.invoke) {
        error = "external Character/CharAI lifecycle service is unavailable";
        return Status::service_unavailable;
    }

    Lifecycle state = record.external_lifecycle;
    Result& result = *output;
    result.character = record.character.identity;
    result.char_ai = reinterpret_cast<std::uintptr_t>(record.components.find(Component::ai));
    result.vm = record.source_external_ais
        ? record.source_external_ais->vm_identity() : 0;
    Dispatch dispatch{&record, &ai, &row, &services, &state, &result, &error, false};
    const character::ScriptLifecycleServices16 forwarded{&dispatch, &invoke};
    try {
        result.source_return = dh2_character_script_lifecycle(&state,
            static_cast<std::uint32_t>(operation), init_final ? 1u : 0u,
            &forwarded);
    } catch (...) {
        try { store_state(record, ai, state); } catch (...) {}
        if (error.empty()) error = "external source lifecycle kernel/provider failed";
        return dispatch.failed ? Status::source_failed : Status::identity_mismatch;
    }
    try { store_state(record, ai, state); }
    catch (...) {
        error = "external lifecycle result no longer matches its canonical Character";
        return Status::identity_mismatch;
    }
    const auto* retained_session = record.source_external_ais.get();
    if ((retained_session && (!retained_session->vm_identity() ||
         retained_session->vm_identity() != result.vm)) ||
        (!retained_session && (state.active || state.load_step > 0)) ||
        (state.pending && record.external_ais.identity != state.pending) ||
        (state.active && record.external_ais.identity != state.active) ||
        ai.active_ais_1c != state.active || ai.alternate_ais_20 != state.pending ||
        state.owner != record.character.identity) {
        error = "external AIS/VM identity changed during source lifecycle";
        return Status::identity_mismatch;
    }
    result.active_external_ais = state.active != 0;
    if (dispatch.failed || result.source_return < 0) {
        if (error.empty()) error = "external source lifecycle did not complete";
        return Status::source_failed;
    }
    if (operation == character::script_load_and_init && result.source_return == 0) {
        error.clear();
        return Status::not_initialized;
    }
    if ((operation == character::script_load_and_init ||
         operation == character::script_init_process) && result.source_return == 1) {
        if (!result.set_skills_called || !result.update_skills_called || !state.active) {
            error = "external InitScriptProcess omitted skill preparation/update or active AIS publication";
            return Status::identity_mismatch;
        }
        result.init_process_complete = true;
    }
    error.clear();
    return Status::complete;
}
} // namespace

Status load_process(factory::Record& record,
        character_ai_initialization::State& ai, const data::AiProps& row,
        const Services& services, Result* output, std::string& error) {
    return run(record, ai, record.nonplayer_source_ai_id, row, services,
        character::script_load_process, false, output, error);
}

Status load_n_init(factory::Record& record,
        character_ai_initialization::State& ai, const data::AiProps& row,
        const Services& services, Result* output, std::string& error) {
    return run(record, ai, record.nonplayer_source_ai_id, row, services,
        character::script_load_and_init, true, output, error);
}

Status init_process(factory::Record& record,
        character_ai_initialization::State& ai, const data::AiProps& row,
        bool init_final, const Services& services, Result* output,
        std::string& error) {
    return run(record, ai, record.nonplayer_source_ai_id, row, services,
        character::script_init_process, init_final, output, error);
}

} // namespace dh2::character_external_script_process_v1
