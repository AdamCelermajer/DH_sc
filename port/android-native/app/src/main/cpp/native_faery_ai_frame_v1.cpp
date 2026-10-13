#include "native_faery_ai_frame_v1.hpp"

#include <cstring>
#include <exception>

namespace dh2::native::faery_ai_frame_v1 {
namespace {

constexpr std::int32_t kStateAwaitingToSpawn = 17;  // CharStateMachine::SM_IsAwaitingToSpawn
constexpr std::int32_t kStateInLimbus = 0;          // CharStateMachine::SM_IsInLimbus

std::uint32_t bits(float value) noexcept {
    std::uint32_t word;
    std::memcpy(&word, &value, sizeof(word));
    return word;
}

float number(std::uint32_t word) noexcept {
    float value;
    std::memcpy(&value, &word, sizeof(value));
    return value;
}

void set_point(character_ai_sight::Point& point, const float* xyz) noexcept {
    for (unsigned i = 0; i < 3; ++i) point.words[i] = bits(xyz[i]);
}

struct BusyScope {
    bool& flag;
    explicit BusyScope(bool& value) noexcept : flag(value) { flag = true; }
    ~BusyScope() { flag = false; }
};

}  // namespace

std::int32_t Owner::fail(const char* reason) {
    if (error_.empty()) error_ = reason ? reason : "Faery CharAI frame failed";
    return 1;
}

bool Owner::bind(std::uintptr_t character, std::uintptr_t ai_identity,
                 std::uintptr_t controller, character_ai_initialization::State* ai) {
    if (!character || !ai_identity || !controller || !ai || busy_) return false;
    character_ = character;
    ai_identity_ = ai_identity;
    controller_ = controller;
    ai_ = ai;
    frame_owner_ = {};
    frame_owner_.owner = character;
    frame_owner_.controller = controller;
    frame_state_ = {};
    frame_services_ = {this, &Owner::dispatch_frame,
                       (1u << character::ai_frame_service_count) - 1u, 0};
    return true;
}

Status Owner::tick(character::Coordinator& coordinator, Host& host,
                   const FrameInput& input, Result* result) {
    if (!result) return Status::invalid_argument;
    *result = {};
    if (!ai_) {
        result->status = Status::not_bound;
        return result->status;
    }
    if (busy_) {
        result->status = Status::busy;
        return result->status;
    }
    BusyScope scope(busy_);
    host_ = &host;
    result_ = result;
    error_.clear();
    input_ = input;
    machine_state_ = coordinator.state.current;

    // The canonical owner projection is refreshed from this Character's own
    // Coordinator immediately before the frame (flags and controller lock), as
    // Coordinator::update_logic_frame does; zoning bytes are the GameObject's.
    frame_owner_.flags520 = coordinator.state.flags;
    frame_owner_.forced = input.forced;
    frame_owner_.locked = coordinator.state.controller_locked;
    frame_owner_.zoned = input.zoned;
    frame_owner_.in_zone = input.in_zone;
    frame_state_ = {ai_identity_, &frame_owner_, ai_->paused_18,
                    input.global_blocked, 0, 0};

    int status = -1;
    try {
        status = coordinator.update_ai_frame(&frame_state_, &frame_services_,
                                             &result->frame);
    } catch (const std::exception& exception) {
        status = 3;
        if (error_.empty()) error_ = exception.what();
    } catch (...) {
        status = 3;
        if (error_.empty()) error_ = "Faery CharAI frame threw";
    }
    result->coordinator_status = status;
    ++statistics_.frames;
    if (status == 0 && result->frame.skip != character::ai_frame_not_skipped)
        ++statistics_.skipped;
    host_ = nullptr;
    result_ = nullptr;
    if (status == 0) {
        result->status = Status::complete;
        return result->status;
    }
    result->status = Status::source_failed;
    result->error = error_.empty()
        ? "Faery CharAI::Update frame rejected its Coordinator/owner projection"
        : error_;
    return result->status;
}

Status Owner::update_master_only(Host& host, std::int32_t machine_state, Result* result) {
    if (!result) return Status::invalid_argument;
    *result = {};
    if (!ai_) {
        result->status = Status::not_bound;
        return result->status;
    }
    if (busy_) {
        result->status = Status::busy;
        return result->status;
    }
    BusyScope scope(busy_);
    host_ = &host;
    result_ = result;
    error_.clear();
    machine_state_ = machine_state;
    std::int32_t status = 1;
    try {
        status = update_master();
    } catch (const std::exception& exception) {
        if (error_.empty()) error_ = exception.what();
    } catch (...) {
        if (error_.empty()) error_ = "CharAI::_UpdateMaster threw";
    }
    result->update_master_status = status;
    host_ = nullptr;
    result_ = nullptr;
    if (status == 0) {
        result->status = Status::complete;
        return result->status;
    }
    result->status = Status::source_failed;
    result->error = error_.empty() ? "CharAI::_UpdateMaster rejected its providers" : error_;
    return result->status;
}

std::int32_t Owner::dispatch_frame(void* context, character::AIFrameState32*,
        const character::AIFrameRequest16* request, std::uint32_t* value) {
    if (!context || !request || !value) return 1;
    auto& self = *static_cast<Owner*>(context);
    if (!self.host_ || !self.ai_ || !self.result_) return 1;
    *value = 0;
    switch (request->service) {
    case character::ai_frame_is_zonable:
        // Character::IsZonable 0x3a36e4 is zero for IsPlayer || IsFaerie.
        return 0;
    case character::ai_frame_update_target:
        return self.update_target();
    case character::ai_frame_update_master:
        self.result_->update_master_status = self.update_master();
        return self.result_->update_master_status;
    case character::ai_frame_update_aggro:
        self.result_->update_aggro_status = self.update_aggro();
        return self.result_->update_aggro_status;
    case character::ai_frame_on_update:
        self.result_->on_update_status = self.on_update();
        return self.result_->on_update_status;
    default:
        return 1;
    }
}

// ---------------------------------------------------------------- _UpdateTarget
std::int32_t Owner::update_target() {
    // CharAI::_UpdateTarget 0x3cb908: nothing happens while the Character is
    // awaiting its spawn or in Limbus, or while CharAI+0x40 is null. A Faery's
    // target is only ever the null set by follower.luac ClearTarget; a non-null
    // target needs the Target providers that no Faery path has.
    std::int32_t state = machine_state_;
    if (!host_->character_state(state)) return fail("Faery _UpdateTarget has no state-machine provider");
    if (state == kStateAwaitingToSpawn || state == kStateInLimbus) return 0;
    if (!ai_->target_40) return 0;
    return fail("Faery _UpdateTarget reached a non-null CharAI target");
}

// ---------------------------------------------------------------- _UpdateMaster
std::int32_t Owner::update_master() {
    character_ai_master_update::State state{
        ai_identity_, character_, ai_->master_50, ai_->byte_54, ai_->byte_55, 0};
    const character_ai_master_update::Services services{this, &Owner::master_service};
    character_ai_master_update::Result report{};
    const auto status = character_ai_master_update::update(&state, &services, &report);
    // The caches live in CharAI+0x54/+0x55; the kernel keeps its snapshots in
    // step with every source write, so publish the final values.
    ai_->byte_54 = state.alive_54;
    ai_->byte_55 = state.sight_55;
    if (status != character_ai_master_update::Status::complete)
        return fail("Faery CharAI::_UpdateMaster source orchestration failed");
    return 0;
}

std::int32_t Owner::master_service(void* context,
        character_ai_master_update::State* state,
        const character_ai_master_update::Request* request,
        character_ai_master_update::Response* response) {
    using Operation = character_ai_master_update::Operation;
    if (!context || !state || !request || !response) return 1;
    auto& self = *static_cast<Owner*>(context);
    if (!self.host_ || !self.ai_) return 1;
    // Source stores the cached bytes before any later virtual runs.
    self.ai_->byte_54 = state->alive_54;
    self.ai_->byte_55 = state->sight_55;
    response->word = 0;
    switch (request->operation) {
    case Operation::char_ai_id: {
        std::uint32_t id = 0;
        if (!self.host_->char_ai_id(id)) return self.fail("Faery GetCharAIId is unavailable");
        response->word = id;
        return 0;
    }
    case Operation::master_is_dead: {
        bool dead = false;
        if (!self.host_->is_dead(request->subject, dead))
            return self.fail("Faery master IsDead provider is unavailable");
        response->word = dead ? 1u : 0u;
        return 0;
    }
    case Operation::sight: {
        std::uint32_t value = 0;
        if (self.sight_value(request->peer, value))
            return 1;
        response->word = value;
        return 0;
    }
    case Operation::my_turn:
        // CharAI::IsMyTurn 0x3cc484: IsFollower || IsFaerie bypasses the
        // 180 ms update queue.
        response->word = 1;
        return 0;
    case Operation::can_range_attack: {
        bool ranged = false;
        if (!self.host_->can_range_attack(ranged))
            return self.fail("Faery CanRangeAttack provider is unavailable");
        response->word = ranged ? 1u : 0u;
        return 0;
    }
    case Operation::close_range:
    case Operation::range:
        return self.fail("Faery master range query reached a ranged-attacker branch");
    case Operation::melee_range: {
        std::uint32_t value = 0;
        if (self.melee_value(request->peer, value))
            return 1;
        response->word = value;
        return 0;
    }
    case Operation::raise_event: {
        const std::uint32_t event = request->event;
        if (event >= 18 && event <= 25) {
            ++self.statistics_.master_events[event - 18];
            self.statistics_.last_event = event;
        }
        const bool ok = self.host_->raise_event(event, request->peer);
        // Character::RaiseEvent callbacks (Lua SetMaster) may replace the
        // master pointer; the source rereads CharAI+0x50 afterwards.
        state->master = self.ai_->master_50;
        if (!ok) return self.fail("Faery master event dispatch failed");
        return 0;
    }
    }
    return self.fail("Faery master operation is unsupported");
}

std::int32_t Owner::sight_value(std::uintptr_t master, std::uint32_t& value) {
    // CharAI::AI_IsInSight(const GameObject*) 0x3d4ed8 -> distance^2 form.
    struct Context {
        Owner* self;
    } context{this};
    const character_ai_sight::Services services{
        &context,
        [](void* raw, character_ai_sight::State*, std::uintptr_t object,
           const character_ai_sight::Point** output) -> std::int32_t {
            auto& self = *static_cast<Context*>(raw)->self;
            float position[3]{};
            if (!output || !self.host_->target_position(object, position))
                return self.fail("Faery sight GetTargetPosition is unavailable");
            auto& point = self.points_[object == self.character_ ? 0 : 1];
            set_point(point, position);
            *output = &point;
            return 0;
        },
        [](void* raw, character_ai_sight::State*, std::uintptr_t,
           const character_ai_sight::ViewRadius** output) -> std::int32_t {
            auto& self = *static_cast<Context*>(raw)->self;
            float view = 0.f, no_aggro = 0.f, interact = 0.f;
            if (!output || !self.host_->ai_row(view, no_aggro, interact))
                return self.fail("Faery AITable row is unavailable");
            self.view_.word_3c = bits(view);
            *output = &self.view_;
            return 0;
        }};
    character_ai_sight::State state{ai_identity_, character_, ai_->target_40};
    character_ai_sight::Result report{};
    if (character_ai_sight::evaluate_object(&state, master, &services, &report) !=
        character_ai_sight::Status::complete)
        return fail("Faery CharAI::AI_IsInSight source provider failed");
    value = report.value;
    return 0;
}

std::int32_t Owner::melee_value(std::uintptr_t master, std::uint32_t& value) {
    // CharAI::AI_IsInMeleeRange 0x3d6188. The master's GetInteractionType is 8
    // only for an enemy non-Player; a friend (the Player) falls through to
    // AI_IsInInteractionRange 0x3d4f98 with the Faery's interact radius.
    struct Context {
        Owner* self;
    } context{this};
    namespace melee = character_ai_melee_range;
    const melee::Services services{
        &context,
        [](void* raw, melee::State*, const melee::Request* request, melee::Response* response) -> std::int32_t {
            auto& self = *static_cast<Context*>(raw)->self;
            *response = {};
            switch (request->operation) {
            case melee::Operation::resolve_object:
                if (!self.host_->resolve_object(request->subject)) {
                    response->view = nullptr;  // unresolved handle: source null object
                    return 0;
                }
                self.resolved_ = {request->subject, 0};
                response->view = &self.resolved_;
                return 0;
            case melee::Operation::interaction_type: {
                std::uint32_t type = 0;
                if (!self.host_->interaction_type(request->subject, request->other, type))
                    return self.fail("Faery master GetInteractionType is unavailable");
                response->word = type;
                return 0;
            }
            case melee::Operation::target_position: {
                float position[3]{};
                if (!self.host_->target_position(request->subject, position))
                    return self.fail("Faery melee GetTargetPosition is unavailable");
                auto& point = self.points_[request->subject == self.character_ ? 0 : 1];
                set_point(point, position);
                response->view = &point;
                return 0;
            }
            case melee::Operation::melee_radius: {
                float radius = 0.f;
                const bool owner_ai = request->kind == melee::Subject::original_ai;
                if (!self.host_->melee_radius(owner_ai, request->subject, radius))
                    return self.fail("Faery AI_GetMeleeRadius is unavailable");
                response->word = bits(radius);
                return 0;
            }
            case melee::Operation::diagnostic_switch: {
                std::uint32_t flag = 0;
                if (!self.host_->debug_switch(request->key, flag))
                    return self.fail("Faery melee debug switch is unavailable");
                response->word = flag;
                return 0;
            }
            case melee::Operation::interaction_range: {
                std::uint32_t in_range = 0;
                if (self.interaction_range_value(request->other, in_range)) return 1;
                response->word = in_range;
                return 0;
            }
            }
            return self.fail("Faery melee-range operation is unsupported");
        }};
    melee::State state{ai_identity_, character_, ai_->target_40};
    melee::Result report{};
    if (melee::evaluate_object(&state, master, &services, &report) != melee::Status::complete)
        return fail("Faery CharAI::AI_IsInMeleeRange source provider failed");
    value = report.value;
    return 0;
}

std::int32_t Owner::interaction_range_value(std::uintptr_t master, std::uint32_t& value) {
    struct Context {
        Owner* self;
    } context{this};
    namespace spot_range = character_ai_interaction_range;
    const spot_range::Services services{
        &context,
        [](void* raw, spot_range::State*, const spot_range::Request* request, spot_range::Response* response) -> std::int32_t {
            auto& self = *static_cast<Context*>(raw)->self;
            *response = {};
            switch (request->operation) {
            case spot_range::Operation::target_position: {
                float position[3]{};
                if (!self.host_->target_position(request->subject, position))
                    return self.fail("Faery interaction GetTargetPosition is unavailable");
                auto& point = self.points_[2];
                set_point(point, position);
                response->view = &point;
                return 0;
            }
            case spot_range::Operation::interaction_spot: {
                float spot[3]{};
                bool node = false;
                if (!self.host_->interaction_spot(request->subject, spot, node))
                    return self.fail("Faery master GetInteractionSpot is unavailable");
                self.spot_facts_ = {request->subject, node ? std::uintptr_t{1} : 0};
                set_point(response->point, spot);
                response->view = &self.spot_facts_;
                return 0;
            }
            case spot_range::Operation::melee_radius: {
                float radius = 0.f;
                if (!self.host_->melee_radius(true, 0, radius))
                    return self.fail("Faery AI_GetMeleeRadius is unavailable");
                response->word = bits(radius);
                return 0;
            }
            case spot_range::Operation::interaction_radius: {
                float radius = 0.f;
                if (!self.host_->melee_radius(false, request->subject, radius))
                    return self.fail("Faery master GetInteractionRadius is unavailable");
                response->word = bits(radius);
                return 0;
            }
            case spot_range::Operation::char_ai: {
                float view = 0.f, no_aggro = 0.f, interact = 0.f;
                if (!self.host_->ai_row(view, no_aggro, interact))
                    return self.fail("Faery AITable row is unavailable");
                self.ai_row_ = {};
                self.ai_row_.words[6] = bits(interact);  // AIProps+0x18
                response->view = &self.ai_row_;
                return 0;
            }
            case spot_range::Operation::interaction_type: {
                std::uint32_t type = 0;
                if (!self.host_->interaction_type(request->subject, request->other, type))
                    return self.fail("Faery master GetInteractionType is unavailable");
                response->word = type;
                return 0;
            }
            }
            return self.fail("Faery interaction-range operation is unsupported");
        }};
    spot_range::State state{ai_identity_, character_, ai_->target_40};
    spot_range::Result report{};
    if (spot_range::evaluate_object(&state, master, &services, &report) != spot_range::Status::complete)
        return fail("Faery CharAI::AI_IsInInteractionRange source provider failed");
    value = report.value;
    return 0;
}

// ---------------------------------------------------------------- _UpdateAggro
std::int32_t Owner::update_aggro() {
    character_aggro_acquisition_prefix::State state{ai_identity_, character_, {0, 0}};
    const character_aggro_acquisition_prefix::Services services{
        this, &Owner::prefix_service, &Owner::prefix_props};
    character_aggro_acquisition_prefix::Result report{};
    // A Faerie never enters the timed (non-player, non-Faerie) branch, so no
    // random stream is consulted.
    const auto status = character_aggro_acquisition_prefix::prepare(
        &state, nullptr, &services, &report);
    if (status != character_aggro_acquisition_prefix::Status::complete)
        return fail("Faery CharAI::_UpdateAggro entry prefix failed");
    using Decision = character_aggro_acquisition_prefix::Decision;
    if (report.decision == Decision::skip_faerie_target) return 0;  // has a master
    if (report.decision != Decision::ready_normal_acquisition) return 0;
    ++statistics_.scans;
    if (!host_->aggro_scan(number(report.radius_word)))
        return fail("Faery _UpdateAggro TargetList acquisition failed");
    return 0;
}

std::int32_t Owner::prefix_service(void* context,
        character_aggro_acquisition_prefix::State*,
        character_aggro_acquisition_prefix::Query query, std::uintptr_t,
        std::uint32_t* output) {
    using Query = character_aggro_acquisition_prefix::Query;
    if (!context || !output) return 1;
    auto& self = *static_cast<Owner*>(context);
    if (!self.host_ || !self.ai_) return 1;
    *output = 0;
    switch (query) {
    case Query::is_player:
    case Query::is_npc:
    case Query::is_monster:
    case Query::is_remotely_updated:
    case Query::spawn_radius_143c:
    case Query::disable_optimization:
    case Query::frame_delta:
        return 0;
    case Query::is_faerie:
    case Query::is_my_turn:
        *output = 1;
        return 0;
    case Query::target_408_present:
        *output = self.ai_->target_40 ? 1u : 0u;
        return 0;
    case Query::target_418_present:
        *output = self.ai_->master_50 ? 1u : 0u;
        return 0;
    case Query::get_char_ai_id:
        return self.host_->char_ai_id(*output) ? 0 : self.fail("Faery GetCharAIId is unavailable");
    case Query::state_awaiting_to_spawn: {
        std::int32_t state = self.machine_state_;
        if (!self.host_->character_state(state))
            return self.fail("Faery state-machine provider is unavailable");
        *output = state == kStateAwaitingToSpawn ? 1u : 0u;
        return 0;
    }
    case Query::has_aggro:
        // CharAI::AI_HasAggro 0x3d49f0 tests the outgoing relation tree count.
        *output = self.ai_->tree_7c.count != 0 ? 1u : 0u;
        return 0;
    }
    return 1;
}

std::int32_t Owner::prefix_props(void* context,
        character_aggro_acquisition_prefix::State*,
        const character_aggro_acquisition_prefix::AiPropsTable** output) {
    if (!context || !output) return 1;
    auto& self = *static_cast<Owner*>(context);
    if (!self.host_) return 1;
    std::uint32_t id = 0;
    float view = 0.f, no_aggro = 0.f, interact = 0.f;
    if (!self.host_->char_ai_id(id) || !self.host_->ai_row(view, no_aggro, interact) ||
        id >= 4096)
        return self.fail("Faery AITable captured row is unavailable");
    // Only the Faery's own row is consulted; keep its slot index.
    self.prefix_rows_.assign(static_cast<std::size_t>(id) + 1, {});
    self.prefix_rows_[id] = {bits(view), bits(no_aggro)};
    self.prefix_table_ = {self.prefix_rows_.data(),
                          static_cast<std::uint32_t>(self.prefix_rows_.size())};
    *output = &self.prefix_table_;
    return 0;
}

// ---------------------------------------------------------------- CharAI::OnUpdate
std::int32_t Owner::on_update() {
    character::AIUpdateState80 state{};
    state.active = ai_->active_ais_1c;
    state.owner = character_;
    state.target408 = ai_->target_40;
    state.target418 = ai_->master_50;
    state.visual = input_.visual;
    state.visual_node = 0;
    state.state = machine_state_;
    state.machine_present = 1;
    state.zoned = input_.zoned;
    state.flag85 = 0;
    const character::AIUpdateServices24 services{
        this, &Owner::update_service,
        (1u << character::ai_update_service_count) - 1u, 0};
    character::AIUpdateResult16 report{};
    if (dh2_character_ai_update(&report, &state, &services) != 0)
        return fail("Faery CharAI::OnUpdate source orchestration failed");
    return 0;
}

std::int32_t Owner::update_service(void* context, character::AIUpdateState80*,
        const character::AIUpdateRequest32* request, std::uint32_t* value) {
    if (!context || !request || !value) return 1;
    auto& self = *static_cast<Owner*>(context);
    if (!self.host_ || !self.ai_) return 1;
    *value = 0;
    switch (request->service) {
    case character::ai_update_active: {
        // AISExternal::OnUpdate 0x3dce64 on the Faery's one active AIS.
        std::uint32_t flags = 0;
        if (!self.host_->ais_flags(flags))
            return self.fail("Faery AIS flags are unavailable");
        ais_external_update::State state{self.ai_->active_ais_1c, self.character_, flags, 0};
        const ais_external_update::Services services{&self, &Owner::ais_service};
        ais_external_update::Result report{};
        if (ais_external_update::update(&state, &services, &report) !=
            ais_external_update::Status::complete)
            return self.fail("Faery AISExternal::OnUpdate source orchestration failed");
        return 0;
    }
    case character::ai_update_disable_zoning:
        return self.host_->disable_zoning() ? 0 : self.fail("Faery DisableZoning failed");
    case character::ai_update_sync_visibility:
        return self.host_->sync_visibility() ? 0 : self.fail("Faery SyncVisibility failed");
    case character::ai_update_is_zonable:
        return 0;  // Character::IsZonable is zero for a Faerie.
    default:
        // EnableZoning/IsDead/SetPosition sit behind IsZonable, which is false.
        return self.fail("Faery CharAI::OnUpdate reached a zoning branch behind IsZonable");
    }
}

std::int32_t Owner::ais_service(void* context, ais_external_update::State*,
        const ais_external_update::Request* request) {
    using Operation = ais_external_update::Operation;
    if (!context || !request) return 1;
    auto& self = *static_cast<Owner*>(context);
    if (!self.host_) return 1;
    switch (request->operation) {
    case Operation::pause_character_ai:
    case Operation::stop_character_controller:
        // AISDefault::OnUpdate only reaches these when AIS+0xbc exceeds 199;
        // that counter is advanced by collision-persist callbacks no Faery has.
        return self.fail("Faery AISDefault::OnUpdate pause/stop branch is unreachable");
    case Operation::call_ais_on_update:
        return self.host_->ais_script_on_update() ? 0 : self.fail("Faery AIS OnUpdate script callback failed");
    case Operation::call_state_update:
    case Operation::call_state_conditions:
        // CharAIScript::CallStateUpdate/Conditions test AIS+0xb4 themselves;
        // follower.luac registers no AIS state table.
        return 0;
    }
    return 1;
}

}  // namespace dh2::native::faery_ai_frame_v1
