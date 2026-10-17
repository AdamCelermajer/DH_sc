#include "source_level_owner_v1.hpp"

#include <cmath>

namespace dh2::source_level_owner_v1 {
namespace {
struct LevelObjectSaveContext {
    level_savegame_owner_v1::Owner* owner{};
    const level_savegame_owner_v1::Object* objects{};
    std::size_t count{};
    std::uint32_t online{};
    const level_savegame_owner_v1::SerializeServices* serializers{};
    const level_savegame_owner_v1::PublishServices* publisher{};
    level_savegame_owner_v1::SaveResult* output{};
    std::uintptr_t expected_identity{};
};

struct QuickSaveContext {
    level_savegame_owner_v1::Owner* owner{};
    const level_quick_save_v1::Services* downstream{};
    std::uintptr_t savegame{};
};

std::int32_t quick_copy_position(void* raw, std::uintptr_t character,
        std::uint32_t word_168, std::uint32_t word_160, std::uint32_t word_164,
        std::string& error) {
    auto* context = static_cast<QuickSaveContext*>(raw);
    if (!context || !context->downstream ||
        !context->downstream->copy_checkpoint_position) {
        error = "Character checkpoint-position provider unavailable";
        return 1;
    }
    return context->downstream->copy_checkpoint_position(
        context->downstream->context, character, word_168, word_160, word_164, error);
}

std::int32_t quick_write_gate(void* raw, std::uintptr_t savegame,
        std::uint8_t value, std::string& error) {
    auto* context = static_cast<QuickSaveContext*>(raw);
    if (!context || !context->owner || savegame != context->savegame ||
        !context->owner->set_gate_39(value)) {
        error = "QuickSave gate write crossed its bound LevelSavegame owner";
        return 1;
    }
    return 0;
}

std::int32_t quick_save_level(void* raw, std::uintptr_t savegame,
        std::string& error) {
    auto* context = static_cast<QuickSaveContext*>(raw);
    if (!context || !context->downstream ||
        !context->downstream->save_level_savegame ||
        savegame != context->savegame) {
        error = "QuickSave::Save provider unavailable or crossed its LevelSavegame owner";
        return 1;
    }
    return context->downstream->save_level_savegame(
        context->downstream->context, savegame, error);
}

std::int32_t save_level_objects_all(void* raw, std::uintptr_t identity,
                                   std::string& error) {
    auto* context = static_cast<LevelObjectSaveContext*>(raw);
    if (!context || !context->owner || !context->output ||
        identity != context->expected_identity) {
        error = "LevelSavegame::Save crossed its bound INFO/OBJS owner";
        return 1;
    }
    const auto status = context->owner->save_all(
        context->objects, context->count, context->online,
        context->serializers, context->publisher, context->output, error);
    return status == level_savegame_owner_v1::SaveStatus::saved ? 0 : 1;
}
}

bool Owner::begin_load() noexcept {
    if (state_.phase == Phase::loading || state_.phase == Phase::active ||
        state_.phase == Phase::tearing_down) return false;
    if (state_.phase == Phase::empty) ++state_.generation;
    state_.phase = Phase::constructed;
    state_.renderer_projection = nullptr;
    floor_world_ = nullptr;
    floor_height_query_ = nullptr;
    floor_query_generation_ = 0;
    state_.floor_query_generation = 0;
    state_.floor_query_bound = false;
    state_.source_level_caches_cleared = false;
    state_.object_manager_flushed = false;
    return true;
}

bool Owner::request_level(std::int32_t row, std::int32_t entry_point,
                          std::int32_t difficulty) noexcept {
    if (state_.phase != Phase::constructed || row < 0 || entry_point < 0)
        return false;
    state_.level_row = row;
    state_.entry_point = entry_point;
    state_.difficulty = difficulty;
    // Recovered Level constructors write -1 to Level+0x148. A future source
    // mutation provider may replace it; no renderer default is involved.
    state_.pending_script_id = -1;
    state_.pending_script_id_known = true;
    state_.source_gslevel = reinterpret_cast<std::uintptr_t>(this);
    state_.source_level = reinterpret_cast<std::uintptr_t>(&native_level_identity_);
    state_.phase = Phase::load_requested;
    return true;
}

bool Owner::begin_source_load() noexcept {
    if (state_.phase != Phase::load_requested) return false;
    state_.phase = Phase::loading;
    return true;
}

bool Owner::publish_projection(const void* projection) noexcept {
    if (!projection || state_.phase != Phase::loading) return false;
    state_.renderer_projection = projection;
    state_.phase = Phase::active;
    return true;
}

bool Owner::suspend_projection() noexcept {
    if (state_.phase != Phase::active || !state_.renderer_projection) return false;
    state_.renderer_projection = nullptr;
    state_.phase = Phase::suspended;
    retained_logical_level_ = true;
    floor_world_ = nullptr;
    floor_height_query_ = nullptr;
    floor_query_generation_ = 0;
    state_.floor_query_generation = 0;
    state_.floor_query_bound = false;
    return true;
}

bool Owner::begin_teardown() noexcept {
    if (state_.phase == Phase::empty) return true;
    if (state_.phase == Phase::tearing_down) return true;
    state_.phase = Phase::tearing_down;
    state_.renderer_projection = nullptr;
    floor_world_ = nullptr;
    floor_height_query_ = nullptr;
    floor_query_generation_ = 0;
    state_.floor_query_generation = 0;
    state_.floor_query_bound = false;
    state_.source_level_caches_cleared = false;
    state_.object_manager_flushed = false;
    return true;
}

bool Owner::mark_source_level_caches_cleared() noexcept {
    if (state_.phase != Phase::tearing_down || state_.source_level_caches_cleared ||
        state_.object_manager_flushed) return false;
    if (character_oid_cache_.level_identity() &&
        character_oid_cache_.clear_level(state_.source_level,
            character_oid_cache_.generation()) !=
                character_oid_cache_v1::Status::complete)
        return false;
    state_.source_level_caches_cleared = true;
    return true;
}

bool Owner::flush_object_manager_after_source_level_clear() noexcept {
    if (state_.phase != Phase::tearing_down ||
        !state_.source_level_caches_cleared || state_.object_manager_flushed)
        return false;
    object_manager_.reset_after_native_flush();
    ++object_manager_flush_count_;
    state_.object_manager_flush_count = object_manager_flush_count_;
    state_.object_manager_flushed = true;
    return true;
}

bool Owner::finish_teardown() noexcept {
    if (state_.phase != Phase::tearing_down) return false;
    // Never destroy the lifecycle identity while a canonical Application
    // ObjectManager still has rows/lists. Its source flush must follow cache
    // retirement in the explicit order above.
    if (!object_manager_.empty() && !state_.object_manager_flushed) return false;
    const auto generation = state_.generation;
    state_ = {};
    retained_logical_level_ = false;
    state_.generation = generation;
    state_.object_manager_flush_count = object_manager_flush_count_;
    state_.object_manager_identity = reinterpret_cast<std::uintptr_t>(&object_manager_);
    native_level_identity_ = {};
    level_savegame_identity_ = {};
    level_savegame_.destroy();
    floor_world_ = nullptr;
    floor_height_query_ = nullptr;
    floor_query_generation_ = 0;
    return true;
}

bool Owner::bind_source_chain(std::uintptr_t gslevel, std::uintptr_t level,
                              std::uintptr_t level_savegame,
                              std::int32_t source_level_state) noexcept {
    if (!gslevel || !level ||
        (state_.phase != Phase::loading && state_.phase != Phase::active)) return false;
    state_.source_gslevel = gslevel;
    state_.source_level = level;
    state_.source_level_savegame = level_savegame;
    state_.canonical_player_savegame = 0;
    state_.source_level_state = source_level_state;
    // A borrowed foreign Level pointer needs an explicit field reader; the
    // synthetic constructor sentinel must not be carried across identities.
    state_.pending_script_id = 0;
    state_.pending_script_id_known = false;
    return true;
}

bool Owner::bind_native_level(const NativeLevelBinding& binding) noexcept {
    if (state_.phase != Phase::active || !state_.renderer_projection ||
        !binding.level_fields || !binding.quest_owner ||
        !binding.player_savegame || !binding.player_character ||
        binding.level_row < 0 || binding.entry_point < 0 ||
        binding.source_save_slot < 0 || !binding.character_table_size ||
        binding.level_state != 38 || state_.level_row != binding.level_row ||
        state_.entry_point != binding.entry_point ||
        state_.difficulty != binding.difficulty) return false;
    std::string filename;
    std::string error;
    if (!level_savegame_owner_v1::format_filename(
            static_cast<std::uint32_t>(binding.source_save_slot),
            binding.level_list_hub, binding.level_row, binding.difficulty,
            filename, error) ||
        !level_savegame_.construct(filename, binding.level_row, error))
        return false;
    native_level_identity_.generation = state_.generation;
    level_savegame_identity_.player_savegame = binding.player_savegame;
    level_savegame_identity_.quest_owner = binding.quest_owner;
    level_savegame_identity_.player_character = binding.player_character;
    state_.source_gslevel = reinterpret_cast<std::uintptr_t>(this);
    state_.source_level = reinterpret_cast<std::uintptr_t>(&native_level_identity_);
    state_.source_level_savegame = reinterpret_cast<std::uintptr_t>(&level_savegame_identity_);
    state_.canonical_player_savegame = binding.player_savegame;
    state_.source_level_state = binding.level_state;
    if (!retained_logical_level_) {
        state_.transition_flag_144 = 0; // Level C1 writes zero at 0x3f3260.
        state_.application_flag_145 = 0; // Level C1 writes zero at 0x3f3264.
        state_.loading_callback_guard_198 = 0; // Level C1 writes zero at 0x3f3284.
        state_.unload_force_f0 = 0;
        // Level C1 starts these null, then Level::_LoadCamera installs the
        // CameraLevel and CameraOverview owners. Until those owners are bound,
        // the live unload values are unknown and queued teardown must fail closed.
        state_.cleanup_index_74_known = false;
        state_.cleanup_index_75_known = false;
        state_.has_cleanup_index_74 = false;
        state_.has_cleanup_index_75 = false;
        state_.cleanup_index_74_identity = 0;
        state_.cleanup_index_75_identity = 0;
    }
    retained_logical_level_ = false;
    state_.level_fields = binding.level_fields;
    state_.quest_owner = binding.quest_owner;
    state_.player_character = binding.player_character;
    state_.level_row = binding.level_row;
    state_.entry_point = binding.entry_point;
    state_.difficulty = binding.difficulty;
    state_.source_save_slot = binding.source_save_slot;
    if (character_oid_cache_.begin_level(state_.source_level,
            binding.character_table_size) !=
                character_oid_cache_v1::Status::complete)
        return false;
    if (binding.pending_script_id_known) {
        state_.pending_script_id = binding.pending_script_id;
        state_.pending_script_id_known = true;
    }
    return true;
}

bool Owner::publish_level_update_complete(std::uint32_t generation) noexcept {
    if (state_.phase != Phase::active || !state_.renderer_projection ||
        !state_.source_level || generation != state_.generation ||
        state_.source_level_state != 38)
        return false;
    // Level::Update writes Level+0x144=1 only after the Level has reached the
    // loaded state (IDA 0x3f87c8). Keep this transition gate attached to the
    // same retained source-Level identity and generation.
    state_.transition_flag_144 = 1;
    return true;
}

bool Owner::set_application_flag_145(std::uintptr_t level,
                                     std::uint8_t value) noexcept {
    if (value > 1 || state_.phase != Phase::active ||
        !state_.renderer_projection || level != state_.source_level)
        return false;
    state_.application_flag_145 = value;
    return true;
}

bool Owner::set_unload_force_f0(std::uintptr_t level,
                                std::uint8_t value) noexcept {
    if (value > 1 || state_.phase != Phase::active ||
        !state_.renderer_projection || level != state_.source_level)
        return false;
    state_.unload_force_f0 = value;
    return true;
}

bool Owner::set_source_level_state(std::uintptr_t level,
                                   std::int32_t value) noexcept {
    if (state_.phase != Phase::active || !level ||
        state_.source_level != level)
        return false;
    state_.source_level_state = value;
    return true;
}

bool Owner::begin_checkpoint_load(std::uintptr_t level) noexcept {
    if (state_.phase != Phase::active || !state_.renderer_projection ||
        state_.source_level_state != 38 || !level ||
        level != state_.source_level || state_.checkpoint_load_in_progress)
        return false;
    state_.checkpoint_load_in_progress = 1;
    return true;
}

bool Owner::finish_checkpoint_load(std::uintptr_t level) noexcept {
    if (state_.phase != Phase::active || !state_.renderer_projection ||
        !level || level != state_.source_level ||
        !state_.checkpoint_load_in_progress)
        return false;
    state_.checkpoint_load_in_progress = 0;
    return true;
}

bool Owner::reset_is_loaded(std::uintptr_t level) noexcept {
    if (state_.phase != Phase::active || !state_.renderer_projection ||
        level != state_.source_level)
        return false;
    // Application::LoadLevel calls ResetIsLoaded before the queued GSLevel
    // handoff. Source ResetIsLoaded clears both Level+0x144 and +0x130.
    state_.transition_flag_144 = 0;
    state_.source_level_state = 0;
    return true;
}

bool Owner::bind_level_cleanup_owners(std::uintptr_t level,
                                     std::uintptr_t camera_level,
                                     std::uintptr_t camera_overview) noexcept {
    if (state_.phase != Phase::active || !state_.renderer_projection ||
        level != state_.source_level || !camera_level || !camera_overview ||
        camera_level == camera_overview ||
        state_.cleanup_index_74_known || state_.cleanup_index_75_known)
        return false;
    state_.cleanup_index_74_identity = camera_level;
    state_.cleanup_index_75_identity = camera_overview;
    state_.cleanup_index_74_known = true;
    state_.cleanup_index_75_known = true;
    state_.has_cleanup_index_74 = true;
    state_.has_cleanup_index_75 = true;
    return true;
}

bool Owner::clear_level_cleanup_owner(std::uintptr_t level, std::uint32_t index,
                                     std::uintptr_t owner_identity) noexcept {
    if (state_.phase != Phase::active || !state_.renderer_projection ||
        level != state_.source_level || !owner_identity)
        return false;
    if (index == 74 && state_.cleanup_index_74_known &&
        state_.has_cleanup_index_74 &&
        state_.cleanup_index_74_identity == owner_identity) {
        state_.has_cleanup_index_74 = false;
        state_.cleanup_index_74_identity = 0;
        return true;
    }
    if (index == 75 && state_.cleanup_index_75_known &&
        state_.has_cleanup_index_75 &&
        state_.cleanup_index_75_identity == owner_identity) {
        state_.has_cleanup_index_75 = false;
        state_.cleanup_index_75_identity = 0;
        return true;
    }
    return false;
}

bool Owner::sg_block_level(std::uintptr_t level, std::uint32_t blocked) noexcept {
    if (blocked > 1 || state_.phase != Phase::active ||
        !state_.renderer_projection || !state_.source_level ||
        level != state_.source_level || !level_savegame_.constructed())
        return false;
    return level_savegame_.set_gate_39(static_cast<std::uint8_t>(blocked));
}

bool Owner::bind_floor_query(const navigation::CollisionWorld* world,
                             FloorHeightQuery query) noexcept {
    if (state_.phase != Phase::active || !state_.renderer_projection ||
        !state_.source_gslevel || !state_.source_level ||
        !state_.level_fields || !state_.quest_owner ||
        !state_.canonical_player_savegame || !state_.player_character ||
        !world || !query) return false;
    floor_world_ = world;
    floor_height_query_ = query;
    floor_query_generation_ = state_.generation;
    state_.floor_query_generation = state_.generation;
    state_.floor_query_bound = true;
    return true;
}

int Owner::set_initial_position_height(std::uint32_t generation,
                                       float xyz[3]) const noexcept {
    if (!xyz || !std::isfinite(xyz[0]) || !std::isfinite(xyz[1]) ||
        !std::isfinite(xyz[2]) || state_.phase != Phase::active ||
        !state_.renderer_projection || generation != state_.generation ||
        !floor_world_ || !floor_height_query_ ||
        floor_query_generation_ != generation ||
        state_.floor_query_generation != generation ||
        !state_.floor_query_bound) return -1;
    navigation::HeightHit hit{};
    hit.height = xyz[2];
    const int result = floor_height_query_(&hit, floor_world_, xyz, 0);
    if (result == 0) return 0;
    if (result != 1 || !std::isfinite(hit.height)) return -1;
    xyz[2] = hit.height;
    return 1;
}

int Owner::query_floor(std::uint32_t generation, const float xyz[3],
                       std::uint32_t include_special,
                       navigation::HeightHit* hit,
                       const navigation::CollisionWorld** world) const noexcept {
    if (world) *world = nullptr;
    if (!xyz || !hit || !world || include_special > 1 ||
        !std::isfinite(xyz[0]) || !std::isfinite(xyz[1]) ||
        !std::isfinite(xyz[2]) || state_.phase != Phase::active ||
        !state_.renderer_projection || generation != state_.generation ||
        !floor_world_ || !floor_height_query_ ||
        floor_query_generation_ != generation ||
        state_.floor_query_generation != generation ||
        !state_.floor_query_bound) return -1;
    *hit = {};
    hit->height = xyz[2];
    *world = floor_world_;
    const int result = floor_height_query_(hit, floor_world_, xyz,
                                           include_special);
    if (result == 0) return 0;
    if (result != 1 || hit->floor >= floor_world_->floor_count ||
        hit->room >= floor_world_->room_count || !std::isfinite(hit->height))
        return -1;
    return 1;
}

Snapshot Owner::snapshot() const noexcept {
    Snapshot result=state_;
    result.object_manager_identity=reinterpret_cast<std::uintptr_t>(&object_manager_);
    result.object_manager_flush_count=object_manager_flush_count_;
    return result;
}

level_quick_save_v1::Status Owner::quick_save(
        const level_quick_save_v1::State* input, std::uint32_t force,
        const level_quick_save_v1::Services* services,
        level_quick_save_v1::Result* output, std::string& error) {
    if (!input || !output || state_.phase != Phase::active ||
        !state_.renderer_projection || !state_.source_gslevel ||
        !state_.source_level || !state_.source_level_savegame ||
        !state_.canonical_player_savegame ||
        input->level != state_.source_level ||
        input->level_savegame != state_.source_level_savegame ||
        input->level_state != state_.source_level_state) {
        error = "QuickSave blocked: active source GSLevel/Level/LevelSavegame chain unavailable";
        if (output) *output = {};
        if (output) output->status = level_quick_save_v1::Status::skipped;
        return level_quick_save_v1::Status::skipped;
    }
    if (!level_savegame_.constructed() || !services) {
        error = "QuickSave blocked: bound LevelSavegame services unavailable";
        if (output) { *output = {}; output->status = level_quick_save_v1::Status::skipped; }
        return level_quick_save_v1::Status::skipped;
    }
    auto state = *input;
    state.level_savegame_gate_39 = level_savegame_.gate_39();
    QuickSaveContext context{&level_savegame_, services,
                             state_.source_level_savegame};
    const level_quick_save_v1::Services bound_services{
        &context, quick_copy_position, quick_write_gate, quick_save_level};
    return level_quick_save_v1::run(&state, force, &bound_services, output, error);
}

level_savegame_save_v1::Status Owner::save_level(
        const level_savegame_save_v1::State* input,
        const level_savegame_save_v1::Services* services,
        level_savegame_save_v1::Result* output, std::string& error) const {
    if (!input || !output || state_.phase != Phase::active ||
        !state_.renderer_projection || !state_.source_gslevel ||
        !state_.source_level || !state_.source_level_savegame ||
        input->savegame != state_.source_level_savegame) {
        error = "LevelSavegame::Save blocked: active source owner chain unavailable";
        if (output) *output = {};
        if (output) output->status = level_savegame_save_v1::Status::skipped;
        return level_savegame_save_v1::Status::skipped;
    }
    auto state = *input;
    state.gate_39 = level_savegame_.gate_39();
    return level_savegame_save_v1::run(&state, services, output, error);
}

level_savegame_save_v1::Status Owner::save_level_objects(
        const level_savegame_save_v1::State* input,
        const level_savegame_owner_v1::Object* objects, std::size_t count,
        std::uint32_t online,
        const level_savegame_owner_v1::SerializeServices* serializers,
        const level_savegame_owner_v1::PublishServices* publisher,
        level_savegame_save_v1::Result* output,
        level_savegame_owner_v1::SaveResult* object_output,
        std::string& error) {
    if (!input || !output || !object_output ||
        input->savegame != state_.source_level_savegame ||
        !level_savegame_.constructed()) {
        error = "LevelSavegame INFO/OBJS dispatch requires the bound active Save owner";
        if (output) *output = {};
        if (object_output) *object_output = {};
        if (output) output->status = level_savegame_save_v1::Status::skipped;
        return level_savegame_save_v1::Status::skipped;
    }
    LevelObjectSaveContext context{&level_savegame_, objects, count, online,
                                   serializers, publisher, object_output,
                                   state_.source_level_savegame};
    const level_savegame_save_v1::Services services{
        &context, &save_level_objects_all};
    return save_level(input, &services, output, error);
}

} // namespace dh2::source_level_owner_v1
