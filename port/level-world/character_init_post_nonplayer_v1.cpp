#include "character_init_post_nonplayer_v1.hpp"
#include "character_gameplay_save_v1.hpp"

#include <cmath>
#include <cstring>
#include <limits>
#include <utility>

namespace dh2::character_init_post_nonplayer_v1 {
namespace {

using Component = factory::ctor::Component;

bool available(const Services& services) noexcept {
    return services.is_player && services.check_spawn_probability &&
        services.safe_get_properties_id && services.load_base_properties &&
        services.recalc_properties && services.get_model_name &&
        services.read_base_scale && services.apply_owner_scale &&
        services.game_object_init_post && services.meet_condition &&
        services.load_save_mask2;
}

bool valid_record(const factory::Record& record) noexcept {
    return record.character.identity != 0 &&
        record.character.identity == record.game_object.identity &&
        record.character.object == &record.aggro_object &&
        record.constructor.constructor_complete &&
        !record.init_post_complete() && !record.init_final_complete() &&
        record.net_state.constructed &&
        record.components.find(Component::game_object) != nullptr &&
        record.components.find(Component::properties) != nullptr &&
        record.components.find(Component::net_state_primary) ==
            &record.net_state.primary &&
        record.components.find(Component::net_state_secondary) ==
            &record.net_state.secondary;
}

template<class Call, class... Args>
bool invoke(Call call, void* context, Result& result, std::string& error,
            const char* label, Args&&... args) {
    ++result.provider_calls;
    try {
        if (call(context, std::forward<Args>(args)...) == 0) return true;
    } catch (...) { }
    if (error.empty()) error = label;
    return false;
}

void snapshot(factory::Record& record, Result& result, Stage stage) noexcept {
    result.stage = stage;
    result.character_identity = record.character.identity;
    result.game_object_owner = reinterpret_cast<std::uintptr_t>(
        record.components.find(Component::game_object));
    result.properties_owner = reinterpret_cast<std::uintptr_t>(
        record.components.find(Component::properties));
    result.property_id = record.property_id_13c8;
    result.prefix_complete = record.nonplayer_init_post_prefix_stage ==
        static_cast<std::uint8_t>(Stage::waiting_for_scene_runtime);
    result.full_init_post_complete = record.init_post_complete();
    result.save_mask2_loaded = result.prefix_complete;
}

std::int32_t signed_word(std::uint32_t value) noexcept {
    std::int32_t result{};
    std::memcpy(&result, &value, sizeof(result));
    return result;
}

void pull_ai_state(const character_ai_initialization::State& ai,
                   character::ScriptLifecycleState64& script) noexcept {
    script = {ai.owner_04, ai.active_ais_1c, ai.alternate_ais_20,
        ai.script_name_30, signed_word(static_cast<std::uint32_t>(ai.pointer_28)),
        signed_word(ai.word_10), signed_word(ai.word_14), ai.byte_24,
        ai.byte_2c, 0, 0, 0};
}

void push_ai_state(character_ai_initialization::State& ai,
                   const character::ScriptLifecycleState64& script) noexcept {
    ai.owner_04 = script.owner;
    ai.active_ais_1c = script.active;
    ai.alternate_ais_20 = script.pending;
    ai.script_name_30 = script.external_name;
    ai.pointer_28 = static_cast<std::uint32_t>(script.load_step);
    ai.word_10 = static_cast<std::uint32_t>(script.timer33);
    ai.word_14 = static_cast<std::uint32_t>(script.timer34);
    ai.byte_24 = static_cast<std::uint8_t>(script.delayed);
    ai.byte_2c = static_cast<std::uint8_t>(script.scripted);
}

struct AiDispatch {
    character_ai_initialization::State* ai{};
    character::ScriptLifecycleServices16 source{};
    std::uint32_t calls{};
    bool failed{};
};

struct AiDispatchFailure {};

void dispatch_script_service(void* raw,
        character::ScriptLifecycleState64* script,
        const character::ScriptLifecycleRequest32* request,
        character::ScriptLifecycleResponse16* response) {
    auto& dispatch = *static_cast<AiDispatch*>(raw);
    if (!dispatch.ai || !dispatch.source.invoke || !script || !request || !response) {
        dispatch.failed = true;
        throw AiDispatchFailure{};
    }
    push_ai_state(*dispatch.ai, *script);
    ++dispatch.calls;
    try {
        dispatch.source.invoke(dispatch.source.context, script, request, response);
    } catch (...) {
        push_ai_state(*dispatch.ai, *script);
        dispatch.failed = true;
        throw AiDispatchFailure{};
    }
    push_ai_state(*dispatch.ai, *script);
}

} // namespace

Status run_prefix(factory::Record& record, const Services& services,
                  Result* output, std::string& error) {
    error.clear();
    if (output) *output = {};
    if (!output || !valid_record(record)) return Status::invalid_argument;
    if (!available(services)) return Status::service_unavailable;
    Result& result = *output;
    snapshot(record, result, Stage::not_started);

    const auto game_object = record.components.find(Component::game_object);
    const auto properties = record.components.find(Component::properties);
    auto stage = static_cast<Stage>(record.nonplayer_init_post_prefix_stage);
    if (stage == Stage::waiting_for_scene_runtime) {
        snapshot(record, result, stage);
        return Status::awaiting_scene_runtime;
    }
    if (stage == Stage::spawn_rejected) {
        snapshot(record, result, stage);
        return Status::spawn_rejected;
    }
    if (!record.nonplayer_init_post_selected) {
        bool is_player = true;
        if (!invoke(services.is_player, services.context, result, error,
                    "Character IsPlayer provider failed", record, &is_player))
            return Status::provider_failed;
        if (is_player) return Status::wrong_character_kind;
        if (record.source_save_slot_kind != factory::SourceSaveSlotKind::constructor_null ||
            record.source_save_14e8) {
            error = "Non-player Character Save slot is not the constructor-null owner";
            return Status::invalid_argument;
        }
        record.source_save_slot_kind = factory::SourceSaveSlotKind::nonplayer_null;
        record.nonplayer_init_post_selected = true;
    }

    if (stage == Stage::not_started) {
        bool accepted = false;
        snapshot(record, result, Stage::check_spawn_probability);
        if (!invoke(services.check_spawn_probability, services.context, result, error,
                    "Character spawn-probability provider failed", record, &accepted))
            return Status::provider_failed;
        result.spawn_accepted = accepted;
        if (!accepted) {
            record.nonplayer_init_post_prefix_stage =
                static_cast<std::uint8_t>(Stage::spawn_rejected);
            snapshot(record, result, Stage::spawn_rejected);
            return Status::spawn_rejected;
        }
        record.nonplayer_init_post_prefix_stage =
            static_cast<std::uint8_t>(Stage::resolve_properties_id);
        stage = Stage::resolve_properties_id;
    }

    if (stage == Stage::resolve_properties_id) {
        std::int16_t property_id = -1;
        snapshot(record, result, stage);
        if (!invoke(services.safe_get_properties_id, services.context, result, error,
                    "SafeGetCharPropsId provider failed", record, &property_id))
            return Status::provider_failed;
        record.property_id_13c8 = property_id;
        record.nonplayer_init_post_prefix_stage =
            static_cast<std::uint8_t>(Stage::load_base_properties);
        stage = Stage::load_base_properties;
    }
    if (stage == Stage::load_base_properties) {
        snapshot(record, result, stage);
        if (!invoke(services.load_base_properties, services.context, result, error,
                    "LoadBaseProperties provider failed", properties,
                    record.property_id_13c8)) return Status::provider_failed;
        record.nonplayer_init_post_prefix_stage =
            static_cast<std::uint8_t>(Stage::recalc_properties);
        stage = Stage::recalc_properties;
    }
    if (stage == Stage::recalc_properties) {
        snapshot(record, result, stage);
        if (!invoke(services.recalc_properties, services.context, result, error,
                    "RecalcProperties provider failed", properties, true))
            return Status::provider_failed;
        // LoadBaseProperties/RecalcProperties are the prefix producers for
        // the canonical factory readiness barrier.  Do not accept a caller
        // that merely pre-marked an empty/unresolved Character Record.
        if (!record.source_properties_ready()) {
            error = "Character RecalcProperties provider did not publish canonical source properties";
            return Status::provider_failed;
        }
        record.nonplayer_init_post_prefix_stage =
            static_cast<std::uint8_t>(Stage::get_model_name);
        stage = Stage::get_model_name;
    }
    if (stage == Stage::get_model_name) {
        snapshot(record, result, stage);
        std::string model_name;
        if (!invoke(services.get_model_name, services.context, result, error,
                    "GetCharModelName provider failed", record, &model_name))
            return Status::provider_failed;
        record.source_model_name = std::move(model_name);
        record.nonplayer_init_post_prefix_stage =
            static_cast<std::uint8_t>(Stage::read_base_scale);
        stage = Stage::read_base_scale;
    }
    if (stage == Stage::read_base_scale) {
        snapshot(record, result, stage);
        std::int32_t base[3]{};
        if (!invoke(services.read_base_scale, services.context, result, error,
                    "base visual scale provider failed", properties, base))
            return Status::provider_failed;
        record.source_owner_scale = {
            static_cast<float>(base[0]) * 0.009f,
            static_cast<float>(base[1]) * 0.009f,
            static_cast<float>(base[2]) * 0.01f};
        record.nonplayer_init_post_prefix_stage =
            static_cast<std::uint8_t>(Stage::apply_owner_scale);
        stage = Stage::apply_owner_scale;
    }
    if (stage == Stage::apply_owner_scale) {
        snapshot(record, result, stage);
        if (!invoke(services.apply_owner_scale, services.context, result, error,
                    "GameObject visual scale provider failed", game_object,
                    record.source_owner_scale.data())) return Status::provider_failed;
        record.nonplayer_init_post_prefix_stage =
            static_cast<std::uint8_t>(Stage::game_object_init_post);
        stage = Stage::game_object_init_post;
    }
    if (stage == Stage::game_object_init_post) {
        snapshot(record, result, stage);
        if (!invoke(services.game_object_init_post, services.context, result, error,
                    "GameObject::InitPost provider failed", game_object))
            return Status::provider_failed;
        record.nonplayer_init_post_prefix_stage =
            static_cast<std::uint8_t>(Stage::meet_condition);
        stage = Stage::meet_condition;
    }
    if (stage == Stage::meet_condition) {
        bool condition = false;
        snapshot(record, result, stage);
        if (!invoke(services.meet_condition, services.context, result, error,
                    "GameObject::MeetCondition provider failed", game_object,
                    &condition)) return Status::provider_failed;
        result.condition_met = condition;
        if (!condition) {
            record.nonplayer_init_post_prefix_stage =
                static_cast<std::uint8_t>(Stage::spawn_rejected);
            snapshot(record, result, Stage::spawn_rejected);
            return Status::spawn_rejected;
        }
        record.nonplayer_init_post_prefix_stage =
            static_cast<std::uint8_t>(Stage::load_save_mask_2);
        stage = Stage::load_save_mask_2;
    }
    if (stage == Stage::load_save_mask_2) {
        if (record.source_save_mask2_attempted &&
            !record.source_save_mask2_complete) {
            error = "Character SG_Load(2) failed after its one source attempt";
            return Status::provider_failed;
        }
        factory::gameplay_save::Result save_result{};
        snapshot(record, result, stage);
        if (!invoke(services.load_save_mask2, services.context, result, error,
                    "Character SG_Load(2) provider failed", record,
                    &save_result, error)) return Status::provider_failed;
        if (save_result.captured_character != record.character.identity ||
            save_result.mask != 2 || save_result.captured_save ||
            save_result.provider_calls || save_result.load_calls) {
            error = "Non-player SG_Load(2) did not preserve the null-slot no-op";
            return Status::provider_failed;
        }
        record.nonplayer_init_post_prefix_stage =
            static_cast<std::uint8_t>(Stage::waiting_for_scene_runtime);
        snapshot(record, result, Stage::waiting_for_scene_runtime);
        result.save_mask2_loaded = true;
        return Status::awaiting_scene_runtime;
    }
    return Status::provider_failed;
}

AiBranchStatus run_ai_branch(factory::Record& record,
        character_ai_initialization::State& ai,
        const AiBranchServices& services, AiBranchResult* output,
        std::string& error) {
    error.clear();
    if (output) *output = {};
    if (!output || !valid_record(record) ||
        !record.nonplayer_init_post_selected ||
        record.nonplayer_init_post_prefix_stage !=
            static_cast<std::uint8_t>(Stage::waiting_for_scene_runtime) ||
        !record.source_properties_ready() || !record.source_save_mask2_complete ||
        record.source_save_slot_kind != factory::SourceSaveSlotKind::nonplayer_null ||
        record.source_save_14e8 ||
        record.components.find(Component::ai) != &ai || !ai.identity ||
        ai.owner_04 != record.character.identity ||
        record.nonplayer_ai_postload_complete ||
        !services.get_char_ai_id || !services.ai_tables) {
        if (record.nonplayer_ai_postload_complete && output) {
            output->character_identity = record.character.identity;
            output->ai_id = record.nonplayer_source_ai_id;
            output->completed = true;
            return AiBranchStatus::already_complete;
        }
        return AiBranchStatus::invalid_argument;
    }

    AiBranchResult& result = *output;
    result.character_identity = record.character.identity;
    result.ai_identity = ai.identity;
    std::int32_t ai_id = -1;
    try {
        if (services.get_char_ai_id(services.context, record, &ai_id) != 0) {
            error = "Character::GetCharAIId provider failed";
            return AiBranchStatus::provider_failed;
        }
    } catch (...) {
        error = "Character::GetCharAIId provider threw";
        return AiBranchStatus::provider_failed;
    }
    result.ai_id = ai_id;
    const auto* declaration = data::ai_props(*services.ai_tables, ai_id);
    // Character::InitPost tests AITable::delayed_load and writes the result to
    // Character+0x3ec. It does not copy that value into CharAI+0x24: the
    // source CharAI constructor initializes +0x24 independently to zero.
    // Keep the actual Character delay projection below and validate only the
    // selected table row plus the signed CharAI script-step range here.
    if (!declaration || declaration->delayed_load > 1 ||
        ai.pointer_28 > 0x7fffffffu) {
        error = "Character CharAI state does not match its selected AITable row";
        return AiBranchStatus::invalid_argument;
    }
    result.delayed_load = declaration->delayed_load;

    if (declaration->delayed_load) {
        // Source Character::InitPost writes byte +0x3ec and defers script
        // loading. This is a semantic projection in the canonical Record.
        record.source_delay_init_3ec = 1;
        record.nonplayer_source_ai_id = ai_id;
        record.nonplayer_ai_postload_complete = true;
        result.delayed_init_requested = true;
        result.completed = true;
        return AiBranchStatus::complete;
    }

    if (!services.script.invoke) {
        error = "CharAI LoadScriptProcess service unavailable";
        return AiBranchStatus::service_unavailable;
    }
    character::ScriptLifecycleState64 script{};
    pull_ai_state(ai, script);
    if (script.owner != record.character.identity || script.delayed != 0 ||
        script.load_step < 0) {
        error = "CharAI LoadScriptProcess state is not bound to this non-delayed Character";
        return AiBranchStatus::invalid_argument;
    }
    AiDispatch dispatch{&ai, services.script};
    const character::ScriptLifecycleServices16 forwarded{
        &dispatch, &dispatch_script_service};
    try {
        result.script_load_result = dh2_character_script_lifecycle(
            &script, character::script_load_process, 0, &forwarded);
    } catch (...) {
        push_ai_state(ai, script);
        result.lifecycle_calls = dispatch.calls;
        error = "CharAI::LoadScriptProcess dependency failed";
        return AiBranchStatus::provider_failed;
    }
    push_ai_state(ai, script);
    result.lifecycle_calls = dispatch.calls;
    result.script_load_called = true;
    if (result.script_load_result != 1 || dispatch.failed) {
        error = "CharAI::LoadScriptProcess was not completed";
        return AiBranchStatus::provider_failed;
    }
    record.nonplayer_source_ai_id = ai_id;
    record.nonplayer_ai_postload_complete = true;
    result.completed = true;
    return AiBranchStatus::complete;
}

EffectChainStatus run_effect_chain(factory::Record& record,
        const data::AiTables& tables, const EffectChainServices& services,
        EffectChainResult* output, std::string& error) {
    error.clear();
    if (output) *output = {};
    if (!output || !valid_record(record) ||
        !record.nonplayer_init_post_selected ||
        record.nonplayer_init_post_prefix_stage !=
            static_cast<std::uint8_t>(Stage::waiting_for_scene_runtime) ||
        !record.source_properties_ready() || !record.source_save_mask2_complete ||
        record.source_save_slot_kind != factory::SourceSaveSlotKind::nonplayer_null ||
        record.source_save_14e8 || !record.nonplayer_ai_postload_complete) {
        if (output && record.nonplayer_init_post_effects_stage == 4) {
            output->character_identity = record.character.identity;
            output->animator_identity = reinterpret_cast<std::uintptr_t>(
                record.components.find(Component::animator));
            output->ai_id = record.nonplayer_source_ai_id;
            output->fx_id = record.source_self_anim_fx_id;
            output->fx_handle = record.source_self_anim_fx_1484;
            output->completed_operations = 4;
            output->source_fx_base_loaded = record.source_anim_fx_base_ready;
            output->complete = true;
            return EffectChainStatus::already_complete;
        }
        return EffectChainStatus::invalid_argument;
    }

    EffectChainResult& result = *output;
    result.character_identity = record.character.identity;
    result.animator_identity = reinterpret_cast<std::uintptr_t>(
        record.components.find(Component::animator));
    result.ai_id = record.nonplayer_source_ai_id;
    result.fx_handle = record.source_self_anim_fx_1484;
    result.fx_id = record.source_self_anim_fx_id;
    result.completed_operations = record.nonplayer_init_post_effects_stage;
    result.source_fx_base_loaded = record.source_anim_fx_base_ready;

    if (record.nonplayer_init_post_effects_stage > 4 ||
        record.nonplayer_source_ai_id < 0 || !result.animator_identity ||
        record.components.find(Component::ai) == nullptr) {
        error = "non-player FX/animation/sound state is incomplete or uncertain";
        return EffectChainStatus::invalid_argument;
    }
    if (record.nonplayer_init_post_effects_attempted != 0xff) {
        error = "FX/animation/sound provider outcome is uncertain; refusing replay";
        return EffectChainStatus::provider_failed;
    }
    const auto* ai_row = data::ai_props(tables, record.nonplayer_source_ai_id);
    if (!ai_row || ai_row->delayed_load > 1 ||
        record.source_delay_init_3ec != ai_row->delayed_load) {
        error = "non-player FX/animation/sound row no longer matches its AI branch";
        return EffectChainStatus::invalid_argument;
    }
    if (record.nonplayer_init_post_effects_stage == 4) {
        result.complete = true;
        return EffectChainStatus::already_complete;
    }
    if (!record.source_anim_fx_base_ready) {
        if (!services.read_anim_fx_base) {
            error = "Character+0x1488 FX base provider is unavailable";
            return EffectChainStatus::service_unavailable;
        }
        std::int32_t base = 0;
        ++result.provider_calls;
        try {
            if (services.read_anim_fx_base(services.context, record, &base) != 0) {
                error = "Character+0x1488 FX base provider failed";
                return EffectChainStatus::provider_failed;
            }
        } catch (...) {
            error = "Character+0x1488 FX base provider threw";
            return EffectChainStatus::provider_failed;
        }
        record.source_anim_fx_base_1488 = base;
        record.source_anim_fx_base_ready = true;
        result.source_fx_base_loaded = true;
    }

    while (record.nonplayer_init_post_effects_stage < 4) {
        const auto operation = static_cast<EffectOperation>(
            record.nonplayer_init_post_effects_stage);
        const bool provider_available = [&] {
            switch (operation) {
            case EffectOperation::grab_anim_fx: return services.grab_anim_fx != nullptr;
            case EffectOperation::register_character_fx_table:
                return services.register_character_fx_table != nullptr;
            case EffectOperation::set_animation_set:
                return services.set_animation_set != nullptr;
            case EffectOperation::init_sounds: return services.init_sounds != nullptr;
            }
            return false;
        }();
        if (!provider_available) {
            error = operation == EffectOperation::grab_anim_fx
                ? "VisualFXManager::GrabAnimFX provider is unavailable"
                : operation == EffectOperation::register_character_fx_table
                    ? "Character::RegisterCharacterFXTable provider is unavailable"
                    : operation == EffectOperation::set_animation_set
                        ? "CharAnimator::SetAnimationSet provider is unavailable"
                        : "Character::_InitSounds provider is unavailable";
            result.completed_operations = record.nonplayer_init_post_effects_stage;
            result.fx_id = record.source_self_anim_fx_id;
            result.fx_handle = record.source_self_anim_fx_1484;
            return EffectChainStatus::service_unavailable;
        }
        const auto op_index = record.nonplayer_init_post_effects_stage;
        // Before any operation that can affect external owners, poison replay.
        record.nonplayer_init_post_effects_attempted = op_index;
        ++result.provider_calls;
        int status = 1;
        try {
            switch (operation) {
            case EffectOperation::grab_anim_fx: {
                const auto fx_bits = static_cast<std::uint32_t>(
                    record.source_anim_fx_base_1488) +
                    static_cast<std::uint32_t>(ai_row->self_fx);
                std::int32_t fx_id{};
                std::memcpy(&fx_id, &fx_bits, sizeof(fx_id));
                std::uintptr_t handle = 0;
                status = services.grab_anim_fx(services.context, fx_bits,
                    record.character.identity, &handle);
                if (status == 0) {
                    record.source_self_anim_fx_id = fx_id;
                    record.source_self_anim_fx_1484 = handle;
                }
                break;
            }
            case EffectOperation::register_character_fx_table:
                status = services.register_character_fx_table(services.context, record);
                break;
            case EffectOperation::set_animation_set:
                status = services.set_animation_set(services.context,
                    record.components.find(Component::animator));
                break;
            case EffectOperation::init_sounds:
                status = services.init_sounds(services.context, record);
                break;
            }
        } catch (...) {
            status = 1;
        }
        if (status != 0) {
            error = "source FX/animation/sound provider failed; stage will not replay";
            result.completed_operations = record.nonplayer_init_post_effects_stage;
            result.fx_id = record.source_self_anim_fx_id;
            result.fx_handle = record.source_self_anim_fx_1484;
            return EffectChainStatus::provider_failed;
        }
        ++record.nonplayer_init_post_effects_stage;
        record.nonplayer_init_post_effects_attempted = 0xff;
        result.completed_operations = record.nonplayer_init_post_effects_stage;
        result.fx_id = record.source_self_anim_fx_id;
        result.fx_handle = record.source_self_anim_fx_1484;
    }
    result.complete = true;
    return EffectChainStatus::complete;
}

} // namespace dh2::character_init_post_nonplayer_v1
