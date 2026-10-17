#include "character_faery_script_process_v1.hpp"

#include <cstring>
#include <stdexcept>

namespace dh2::character_faery_script_process_v1 {
namespace {

using Component = factory::ctor::Component;
using Operation = character::ScriptLifecycleService;

std::int32_t signed_word(std::uint32_t value) noexcept {
    std::int32_t result{};
    std::memcpy(&result, &value, sizeof(result));
    return result;
}

bool valid_record(factory::Record& record) noexcept {
    const auto identity = record.character.identity;
    const auto* ai = record.canonical_ai();
    return identity && record.constructor.identity == identity &&
        record.game_object.identity == identity && record.object_registered &&
        record.character_listed && record.constructor.constructor_complete &&
        ai && ai->owner_04 == identity &&
        record.components.find(Component::controller) &&
        record.components.find(Component::properties);
}

character::ScriptLifecycleState64 load_state(factory::Record& record) {
    const auto* ai = record.canonical_ai();
    if (!ai) throw std::runtime_error("Faery CharAI Record is no longer canonical");
    return {ai->owner_04, ai->active_ais_1c, ai->alternate_ais_20,
        ai->script_name_30,
        signed_word(static_cast<std::uint32_t>(ai->pointer_28)),
        signed_word(ai->word_10), signed_word(ai->word_14), ai->byte_24,
        ai->byte_2c, 0, 0, 0};
}

void store_state(factory::Record& record,
                 const character::ScriptLifecycleState64& state) {
    auto* ai = record.canonical_ai();
    if (!ai || ai->owner_04 != state.owner)
        throw std::runtime_error("Faery CharAI owner changed during source dispatch");
    ai->active_ais_1c = state.active;
    ai->alternate_ais_20 = state.pending;
    ai->script_name_30 = state.external_name;
    ai->pointer_28 = static_cast<std::uint32_t>(state.load_step);
    ai->word_10 = static_cast<std::uint32_t>(state.timer33);
    ai->word_14 = static_cast<std::uint32_t>(state.timer34);
    ai->byte_24 = static_cast<std::uint8_t>(state.delayed);
    ai->byte_2c = static_cast<std::uint8_t>(state.scripted);
}

struct Dispatch {
    factory::Record* record{};
    const Services* services{};
    character::ScriptLifecycleState64* state{};
    Result* result{};
    std::string* error{};
    bool failed{};
};

void fail(Dispatch& dispatch, const char* message) {
    dispatch.failed = true;
    if (dispatch.error && dispatch.error->empty()) *dispatch.error = message;
    throw std::runtime_error(message);
}

void invoke(void* raw, character::ScriptLifecycleState64* state,
            const character::ScriptLifecycleRequest32* request,
            character::ScriptLifecycleResponse16* response) {
    auto& dispatch = *static_cast<Dispatch*>(raw);
    if (!dispatch.record || !dispatch.services || !dispatch.result ||
        !dispatch.error || !state || state != dispatch.state || !request ||
        !response || !dispatch.record->canonical_ai())
        fail(dispatch, "Faery script lifecycle dispatch lost its canonical owner");

    ++dispatch.result->lifecycle_calls;
    dispatch.result->last_lifecycle_service = request->service;
    try {
        switch (request->service) {
        case character::script_create_step: {
            const auto status = character_faery_record_session_v1::construct(
                *dispatch.record, dispatch.services->faery_script,
                *dispatch.error);
            if (status != character_faery_record_session_v1::Status::complete)
                fail(dispatch, "SetScript<AISFaery> source provider failed");
            const auto& session = dispatch.record->faery_script.session();
            if (!session.ais || session.char_ai_owner !=
                    reinterpret_cast<session::Identity>(
                        dispatch.record->components.find(Component::ai)))
                fail(dispatch, "SetScript<AISFaery> returned another CharAI owner");
            state->pending = session.ais;
            state->external_name = 0;
            state->scripted = 1;
            response->identity = session.ais;
            break;
        }
        case character::script_bind_functions:
            if (request->subject != state->pending ||
                character_faery_record_session_v1::bind_functions(
                    *dispatch.record, *dispatch.error) !=
                    character_faery_record_session_v1::Status::complete)
                fail(dispatch, "Faery CharAIScript::BindFunction failed");
            break;
        case character::script_set_character:
            if (request->subject != state->pending ||
                request->payload != state->owner ||
                character_faery_record_session_v1::bind_character(
                    *dispatch.record, *dispatch.error) !=
                    character_faery_record_session_v1::Status::complete)
                fail(dispatch, "Faery CharAIScript::SetCharacter failed");
            break;
        case character::script_load_common:
            if (request->subject != state->pending ||
                character_faery_record_session_v1::load_common(
                    *dispatch.record, *dispatch.error) !=
                    character_faery_record_session_v1::Status::complete)
                fail(dispatch, "Faery LuaScript::_commons load failed");
            break;
        case character::script_ais_init:
            if (request->subject != state->active &&
                request->subject != state->pending)
                fail(dispatch, "Faery AIS OnInit identity differs from pending owner");
            if (character_faery_record_session_v1::initialize(
                    *dispatch.record, *dispatch.error) !=
                    character_faery_record_session_v1::Status::complete)
                fail(dispatch, "Faery AIS OnInit failed");
            break;
        case character::script_ais_init_post:
            if (request->subject != state->active ||
                character_faery_record_session_v1::init_post(
                    *dispatch.record, *dispatch.error) !=
                    character_faery_record_session_v1::Status::complete)
                fail(dispatch, "Faery AIS OnInitPost failed");
            break;
        case character::script_ais_init_final:
            if (request->subject != state->active ||
                character_faery_record_session_v1::init_final(
                    *dispatch.record, *dispatch.error) !=
                    character_faery_record_session_v1::Status::complete)
                fail(dispatch, "Faery AIS OnInitFinal failed");
            break;
        case character::script_owner_is_character:
            if (request->subject != state->owner)
                fail(dispatch, "Faery CharAI owner query changed Character identity");
            response->word = 1;
            break;
        default: {
            if (!dispatch.services->source.invoke)
                fail(dispatch, "Faery CharAI source lifecycle service is unavailable");
            store_state(*dispatch.record, *state);
            const character::ScriptLifecycleServices16 nested{
                &dispatch, &invoke};
            if (dispatch.services->source.invoke(
                    dispatch.services->source.context, state, request,
                    response, &nested) != 0)
                fail(dispatch, "Faery source lifecycle provider rejected operation");
            store_state(*dispatch.record, *state);
            break;
        }
        }
    } catch (...) {
        // The lifecycle kernel has source-prefix semantics: publish any
        // CharAI writes made before a provider failed, then stop this attempt.
        try { store_state(*dispatch.record, *state); } catch (...) {}
        dispatch.failed = true;
        if (dispatch.error->empty()) *dispatch.error =
            "Faery source lifecycle callback failed";
        throw;
    }
}

Status run(factory::Record& record, const Services& services,
           std::uint32_t operation, bool init_final, Result* output,
           std::string& error) {
    error.clear();
    if (output) *output = {};
    if (!output || !valid_record(record)) {
        error = "Faery script process requires a registered canonical Character Record";
        return Status::invalid_record;
    }
    if (!services.faery_script.context || !services.faery_script.construct ||
        !services.faery_script.dispatch || !services.source.invoke) {
        error = "Faery script process requires CharAIScript and source lifecycle providers";
        return Status::service_unavailable;
    }

    auto state = load_state(record);
    Result& result = *output;
    result.character = record.character.identity;
    result.char_ai = reinterpret_cast<std::uintptr_t>(
        record.components.find(Component::ai));
    Dispatch dispatch{&record, &services, &state, &result, &error, false};
    const character::ScriptLifecycleServices16 forwarded{&dispatch, &invoke};
    try {
        result.source_return = dh2_character_script_lifecycle(&state,
            operation, init_final ? 1u : 0u, &forwarded);
    } catch (...) {
        try { store_state(record, state); } catch (...) {}
        if (error.empty()) error = "Faery CharAI lifecycle kernel/provider failed";
        result.session_constructed = record.faery_script.session().constructed;
        result.faery_ais = record.faery_script.session().ais;
        return dispatch.failed ? Status::source_failed : Status::identity_mismatch;
    }
    try { store_state(record, state); }
    catch (...) {
        error = "Faery CharAI lifecycle result no longer matches its canonical Record";
        return Status::identity_mismatch;
    }
    result.session_constructed = record.faery_script.session().constructed;
    result.faery_ais = record.faery_script.session().ais;
    if (dispatch.failed || result.source_return != 1) {
        if (error.empty()) error = "Faery CharAI lifecycle kernel did not complete";
        return Status::source_failed;
    }
    if (operation == character::script_load_process) {
        result.load_process_complete = state.active == result.faery_ais &&
            state.load_step >= 7;
        if (!result.session_constructed ||
            record.faery_script.stage() != session::Stage::initialized) {
            error = "Faery LoadScriptProcess did not initialize its pending AISFaery";
            return Status::identity_mismatch;
        }
    } else {
        result.init_process_complete = record.faery_script.session().initialized &&
            record.faery_script.session().init_post_called &&
            (!init_final || record.faery_script.session().init_final_called);
        if (!result.init_process_complete) {
            error = "Faery InitScriptProcess omitted an AIS lifecycle callback";
            return Status::identity_mismatch;
        }
    }
    error.clear();
    return Status::complete;
}

} // namespace

Status load_process(factory::Record& record, const Services& services,
                    Result* result, std::string& error) {
    return run(record, services, character::script_load_process, false,
               result, error);
}

Status init_process(factory::Record& record, bool init_final,
                    const Services& services, Result* result,
                    std::string& error) {
    const auto* ai = record.canonical_ai();
    const auto ais = record.faery_script.session().ais;
    if (record.faery_script.stage() != session::Stage::initialized ||
        !ai || !ais || ai->active_ais_1c != ais ||
        ai->owner_04 != record.character.identity) {
        error = "Faery InitScriptProcess requires the active initialized AISFaery on this canonical CharAI";
        if (result) *result = {};
        return Status::invalid_record;
    }
    return run(record, services, character::script_init_process, init_final,
               result, error);
}

} // namespace dh2::character_faery_script_process_v1
