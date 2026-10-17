#include "ghost_ai_owner.hpp"

#include <cstring>
#include <limits>
#include <new>

namespace dh2::native::ghost_ai {
namespace {
constexpr std::uint32_t kFullCircleWord = 0x40c90fdbu;

bool same_identity(const Identity& a, const Identity& b) noexcept {
    return a.actor == b.actor && a.character == b.character && a.ai == b.ai &&
           a.active_ais == b.active_ais &&
           a.active_ais_enemy_spotted == b.active_ais_enemy_spotted;
}

float float_from_word(std::uint32_t word) noexcept {
    float value;
    std::memcpy(&value, &word, sizeof(value));
    return value;
}

struct BusyScope {
    bool& flag;
    explicit BusyScope(bool& value) noexcept : flag(value) { flag = true; }
    ~BusyScope() { flag = false; }
};

bool same_script_projection(const ghost_ai_session::Bindings& a,
                            const ghost_ai_session::Bindings& b) noexcept {
    const auto& x=a.script_queries; const auto& y=b.script_queries;
    return a.ai_identity==b.ai_identity && a.owner_identity==b.owner_identity &&
        a.active_ais_identity==b.active_ais_identity && a.active_ais_callee==b.active_ais_callee &&
        a.enemy_state==b.enemy_state && a.enemy_services==b.enemy_services &&
        a.event_state==b.event_state && a.event_services==b.event_services &&
        a.relation_state==b.relation_state && a.relation_services==b.relation_services &&
        a.set_target_state==b.set_target_state && a.set_target_services==b.set_target_services &&
        a.search_services==b.search_services && a.controller_state==b.controller_state &&
        a.control_services==b.control_services && a.path_state==b.path_state && a.path_services==b.path_services &&
        x.context==y.context && x.get_py_struct==y.get_py_struct && x.get_prop==y.get_prop &&
        x.get_py_constant==y.get_py_constant && x.has_target==y.has_target && x.get_target==y.get_target &&
        x.get_state==y.get_state && x.has_path==y.has_path && x.get_py_oid==y.get_py_oid &&
        x.get_position==y.get_position && x.get_host_player_level==y.get_host_player_level &&
        x.get_host_player_difficulty==y.get_host_player_difficulty &&
        x.get_current_level_range==y.get_current_level_range && x.set_level==y.set_level &&
        x.stop==y.stop && x.attack==y.attack;
}
}  // namespace

Status Owner::bind(const Bindings& bindings, std::string& error) {
    return bind_impl(bindings,nullptr,error);
}

Status Owner::prepare_pending(const ghost_ai_session::Bindings& bindings,
    const character::ScriptLifecycleState64* lifecycle, monster_external_script::Services& services,
    std::shared_ptr<void>& callback_lifetime, std::string& error) {
    if (busy_ || bound_) { error="Ghost owner already in use"; return Status::busy; }
    const auto result=script_.prepare_pending(bindings,lifecycle,error);
    if (result!=ghost_ai_session::Status::complete)
        return result==ghost_ai_session::Status::allocation_failed ? Status::allocation_failed : Status::invalid_argument;
    if (!script_.staged_services(services,callback_lifetime)) {
        error="pending Ghost callback context became stale";
        return Status::source_failed;
    }
    pending_bindings_=bindings;
    pending_prepared_=true;
    error.clear();
    return Status::complete;
}

Status Owner::bind_staged(const Bindings& bindings, monster_external_script::Session& vm,
                         std::string& error) {
    if (busy_ || bound_) { error="Ghost owner already in use"; return Status::busy; }
    if (!pending_prepared_ || !same_script_projection(bindings.script,pending_bindings_)) {
        error="Ghost frame owner does not match its prepared pending script";
        return Status::invalid_argument;
    }
    return bind_impl(bindings,&vm,error);
}

Status Owner::bind_impl(const Bindings& bindings, monster_external_script::Session* vm, std::string& error) {
    if (busy_) return Status::busy;
    const auto& id = bindings.identity;
    if (!id.actor || !id.character || !id.ai || !id.active_ais ||
        !id.active_ais_enemy_spotted ||
        bindings.script.ai_identity != id.ai ||
        bindings.script.owner_identity != id.character ||
        bindings.script.active_ais_identity != id.active_ais ||
        bindings.script.active_ais_callee != id.active_ais_enemy_spotted ||
        !bindings.script.set_target_state || !bindings.script.path_state ||
        !bindings.frame_services || !bindings.frame_services->invoke ||
        bindings.frame_services->reserved ||
        (bindings.frame_services->available & ~((1u << character::ai_frame_service_count) - 1u)) ||
        !bindings.acquisition_state || !bindings.acquisition_services ||
        !bindings.acquisition_services->invoke ||
        !bindings.acquisition_services->capture_ai_props || !bindings.random ||
        (int(bindings.rooms != nullptr) + int(bindings.characters != nullptr) +
         int(bindings.objects != nullptr)) != 1 || !bindings.character_registry_context ||
        !bindings.resolve_character || !bindings.candidate_capacity ||
        bindings.candidate_capacity > 65536 ||
        reinterpret_cast<std::uintptr_t>(bindings.acquisition_state) %
            alignof(character_aggro_acquisition_prefix::State) ||
        !bindings.script.search_services || !bindings.script.search_services->invoke ||
        !bindings.script.relation_state || !bindings.script.enemy_state ||
        !bindings.script.event_state || !bindings.script.set_target_state ||
        !bindings.script.controller_state || !bindings.script.path_state) {
        error = "invalid live Ghost AI owner bindings";
        return Status::invalid_argument;
    }
    if (!bindings.ais_update_state ||
        bindings.ais_update_state->ais != id.active_ais ||
        !bindings.ais_update_services || !bindings.ais_update_services->invoke ||
        reinterpret_cast<std::uintptr_t>(bindings.ais_update_state) %
            alignof(ais_external_update::State)) {
        error = "invalid live Ghost AIS update projection";
        return Status::invalid_argument;
    }
    constexpr std::uint32_t update_active_bit = 1u << character::ai_update_active;
    if (!bindings.ai_update_state ||
        bindings.ai_update_state->active != id.active_ais ||
        bindings.ai_update_state->owner != id.character ||
        !bindings.ai_update_services || !bindings.ai_update_services->invoke ||
        bindings.ai_update_services->reserved ||
        (bindings.ai_update_services->available & ~((1u << character::ai_update_service_count) - 1u)) ||
        reinterpret_cast<std::uintptr_t>(bindings.ai_update_state) %
            alignof(character::AIUpdateState80) ||
        !(bindings.ai_update_services->available & update_active_bit)) {
        error = "invalid live CharAI::OnUpdate projection/services";
        return Status::invalid_argument;
    }

    std::vector<character::aggro_search::TargetInfo> candidate_heap;
    std::vector<character_enemy_retention::Target> retention_heap;
    try {
        candidate_heap.resize(bindings.candidate_capacity);
        if (bindings.retention_services) {
            if (!bindings.retention_services->invoke ||
                !bindings.retention_services->search.invoke || !bindings.resolve_retention_owner) {
                error = "invalid live Ghost retention bindings";
                return Status::invalid_argument;
            }
            retention_heap.resize(bindings.candidate_capacity);
        }
        if (bindings.retarget_services && !bindings.retarget_services->invoke) {
            error = "invalid live Ghost retarget bindings";
            return Status::invalid_argument;
        }
    } catch (const std::bad_alloc&) {
        error = "Ghost AI target-list allocation failed";
        return Status::allocation_failed;
    } catch (...) {
        error = "Ghost AI target-list allocation failed";
        return Status::allocation_failed;
    }

    const auto script_status = vm ? script_.adopt_staged(*vm,error) :
        script_.bind(bindings.script, bindings.commons, bindings.monster, error);
    if (script_status != ghost_ai_session::Status::complete)
        return script_status == ghost_ai_session::Status::allocation_failed ?
               Status::allocation_failed : Status::script_not_ready;

    identity_ = id;
    bindings_ = bindings;
    candidate_heap_ = std::move(candidate_heap);
    retention_heap_ = std::move(retention_heap);
    candidate_list_ = {};
    upstream_acquisition_services_ = *bindings.acquisition_services;
    acquisition_services_ = {this, dispatch_acquisition, capture_acquisition_props};
    ais_update_services_ = *bindings.ais_update_services;
    upstream_character_update_services_ = *bindings.ai_update_services;
    character_update_services_ = {this, dispatch_character_update,
        upstream_character_update_services_.available | update_active_bit, 0};
    frame_owner_ = {};
    frame_state_ = {id.ai, &frame_owner_, 0, 0, 0, 0};
    frame_services_ = {this, dispatch_frame,
        bindings.frame_services->available |
            (1u << character::ai_frame_update_aggro) |
            (1u << character::ai_frame_on_update), 0};
    bound_ = true;
    pending_prepared_=false;
    pending_bindings_={};
    error.clear();
    return Status::complete;
}

Status Owner::reset(std::string& error) {
    if (busy_) return Status::busy;
    const auto result = script_.reset(error);
    if (result != ghost_ai_session::Status::complete)
        return result == ghost_ai_session::Status::allocation_failed ?
               Status::allocation_failed : Status::source_failed;
    identity_ = {};
    bindings_ = {};
    frame_owner_ = {};
    frame_state_ = {};
    frame_services_ = {};
    character_update_services_ = {};
    upstream_character_update_services_ = {};
    ais_update_services_ = {};
    acquisition_services_ = {};
    upstream_acquisition_services_ = {};
    target_read_owner_ = 0;
    retention_heap_.clear();
    candidate_list_ = {};
    candidate_heap_.clear();
    bound_ = false;
    pending_prepared_=false;
    pending_bindings_={};
    error.clear();
    return Status::complete;
}

bool Owner::ready() const noexcept {
    return bound_ && script_.ready();
}

monster_external_script::Statistics Owner::script_statistics() const noexcept {
    return script_.script_statistics();
}

Status Owner::dispatch_combat_result(monster_external_script::Event event,
        std::uintptr_t attacker, std::uintptr_t defender, std::string& error) {
    if (!bound_ || !script_.ready()) {
        error = "active Monster AIS owner is not ready";
        return Status::script_not_ready;
    }
    if (busy_) { error = "Monster AIS owner busy"; return Status::busy; }
    if (identity_.character != attacker && identity_.character != defender) {
        error = "combat participants do not include this Monster Character";
        return Status::invalid_argument;
    }
    BusyScope busy(busy_);
    const auto status = script_.dispatch_combat_result(event, attacker, defender, error);
    if (status == ghost_ai_session::Status::complete) return Status::complete;
    if (status == ghost_ai_session::Status::busy) return Status::busy;
    if (status == ghost_ai_session::Status::invalid_argument) return Status::invalid_argument;
    if (status == ghost_ai_session::Status::stale_binding) return Status::stale_owner;
    if (status == ghost_ai_session::Status::not_ready) return Status::script_not_ready;
    return Status::source_failed;
}

Status Owner::tick(const FrameInput& input, FrameResult* output) {
    if (!output || busy_ || !bound_) return busy_ ? Status::busy : Status::invalid_argument;
    if (!ready()) return Status::script_not_ready;
    if (!same_identity(input.identity, identity_) ||
        input.owner.owner != identity_.character || input.owner.reserved0 ||
        input.owner.reserved1 || input.paused > 255 || input.global_blocked > 255)
        return Status::stale_owner;

    FrameResult result{};
    result.status = Status::complete;
    result.actor_identity = identity_.actor;
    result.character_identity = identity_.character;
    result.ai_identity = identity_.ai;
    result.active_ais_identity = identity_.active_ais;
    frame_owner_ = input.owner;
    frame_state_ = {identity_.ai, &frame_owner_, input.paused,
                    input.global_blocked, 0, 0};
    bindings_.ais_update_state->owner = input.owner.owner;
    bindings_.ai_update_state->owner = input.owner.owner;
    active_result_ = &result;
    BusyScope scope(busy_);
    try {
        result.source_frame_status = dh2_character_ai_frame(
            &result.frame, &frame_state_, &frame_services_);
    } catch (...) {
        result.source_frame_status = 3;
        result.source_aggro_status = -1;
    }
    result.frame_skip = result.frame.skip;
    capture_observation(result);
    if (result.source_frame_status == 0) result.status = Status::complete;
    else if (result.source_aggro_status == static_cast<std::int32_t>(Status::unsupported_branch))
        result.status = Status::unsupported_branch;
    else result.status = Status::source_failed;
    *output = result;
    active_result_ = nullptr;
    return result.status;
}

std::int32_t Owner::dispatch_frame(void* context, character::AIFrameState32* state,
                                   const character::AIFrameRequest16* request,
                                   std::uint32_t* value) {
    if (!context || !state || !request || !value) return 1;
    auto& self = *static_cast<Owner*>(context);
    if (request->service == character::ai_frame_update_aggro)
        return self.update_aggro(state);
    if (request->service == character::ai_frame_on_update)
        return self.update_character(state);

    const auto& upstream = *self.bindings_.frame_services;
    if (!upstream.invoke || !(upstream.available & (1u << request->service))) return 1;
    try {
        return upstream.invoke(upstream.context, state, request, value) ? 1 : 0;
    } catch (...) {
        return 1;
    }
}

std::int32_t Owner::update_ais(character::AIFrameState32* frame) {
    if (!active_result_ || !frame || !frame->owner ||
        !bindings_.ais_update_state ||
        bindings_.ais_update_state->ais != identity_.active_ais ||
        frame->ai != identity_.ai ||
        frame->owner->owner != identity_.character)
        return 1;
    auto& report = *active_result_;
    report.ais_update_status = static_cast<std::int32_t>(
        ais_external_update::update(bindings_.ais_update_state,
                                    &ais_update_services_,
                                    &report.ais_update));
    report.ais_update_calls = report.ais_update.service_calls;
    return report.ais_update_status ==
            static_cast<std::int32_t>(ais_external_update::Status::complete) ? 0 : 1;
}

std::int32_t Owner::dispatch_character_update(void* context,
        character::AIUpdateState80* state,
        const character::AIUpdateRequest32* request, std::uint32_t* value) {
    if (!context || !state || !request || !value) return 1;
    auto& self = *static_cast<Owner*>(context);
    if (request->service == character::ai_update_active) {
        if (!self.active_result_ || state != self.bindings_.ai_update_state ||
            request->subject != self.identity_.active_ais ||
            state->active != self.identity_.active_ais ||
            state->owner != self.identity_.character ||
            self.frame_state_.ai != self.identity_.ai) return 1;
        *value = 0;
        return self.update_ais(&self.frame_state_);
    }
    const auto& upstream = self.upstream_character_update_services_;
    if (!upstream.invoke || !(upstream.available & (1u << request->service))) return 1;
    try {
        return upstream.invoke(upstream.context, state, request, value) ? 1 : 0;
    } catch (...) {
        return 1;
    }
}

std::int32_t Owner::update_character(character::AIFrameState32* frame) {
    if (!active_result_ || !frame || !frame->owner ||
        frame->ai != identity_.ai || frame->owner->owner != identity_.character ||
        !bindings_.ai_update_state ||
        bindings_.ai_update_state->active != identity_.active_ais)
        return 1;
    auto& report = *active_result_;
    auto& state = *bindings_.ai_update_state;
    state.owner = frame->owner->owner;
    report.character_update_status = dh2_character_ai_update(
        &report.character_update, &state, &character_update_services_);
    return report.character_update_status == 0 ? 0 : 1;
}

std::int32_t Owner::update_aggro(character::AIFrameState32* frame) {
    if (!active_result_ || !frame || !frame->owner || !bindings_.acquisition_state)
        return 1;
    auto& report = *active_result_;
    report.source_service = character::ai_frame_update_aggro;
    auto& state = *bindings_.acquisition_state;
    state.ai = frame->ai;
    state.owner = frame->owner->owner;
    target_read_owner_ = 0;

    auto prefix_status = character_aggro_acquisition_prefix::prepare(
        &state, bindings_.random, &acquisition_services_, &report.acquisition);
    report.acquisition_decision = static_cast<std::uint32_t>(report.acquisition.decision);
    report.source_aggro_status = static_cast<std::int32_t>(prefix_status);
    if (prefix_status == character_aggro_acquisition_prefix::Status::unsupported_branch &&
        report.acquisition.decision ==
            character_aggro_acquisition_prefix::Decision::monster_target_branch) {
        return update_existing_target(&state);
    }
    if (prefix_status != character_aggro_acquisition_prefix::Status::complete) return 1;
    if (report.acquisition.decision !=
        character_aggro_acquisition_prefix::Decision::ready_normal_acquisition) return 0;

    character::aggro_search::Character* owner = nullptr;
    try {
        if (bindings_.resolve_character(bindings_.character_registry_context,
                report.acquisition.list_owner, &owner) != 0 || !owner ||
            owner->identity != report.acquisition.list_owner || !owner->object ||
            !owner->object->identity)
            return 1;
    } catch (...) {
        return 1;
    }
    if (dh2_aggro_target_list_init(&candidate_list_, candidate_heap_.data(),
            static_cast<std::uint32_t>(candidate_heap_.size()), owner) !=
        character::aggro_search::complete)
        return 1;

    report.source_search_started = 1;
    const float view_radius = float_from_word(report.acquisition.radius_word);
    const float cone = float_from_word(kFullCircleWord);
    const auto scan_status = bindings_.objects ?
        script_.search_objects_and_dispatch(&candidate_list_, bindings_.objects,
            view_radius, cone, &report.scan) : bindings_.characters ?
        script_.search_characters_and_dispatch(&candidate_list_, bindings_.characters,
            view_radius, cone, &report.scan) :
        script_.search_and_dispatch(&candidate_list_, bindings_.rooms,
            view_radius, cone, &report.scan);
    report.candidate_count = report.scan.candidates_before_dispatch;
    report.source_aggro_status = static_cast<std::int32_t>(scan_status);
    if (scan_status != ghost_ai_session::Status::complete) return 1;
    return 0;
}

std::int32_t Owner::dispatch_acquisition(void* context,
    character_aggro_acquisition_prefix::State* state,
    character_aggro_acquisition_prefix::Query query, std::uintptr_t subject,
    std::uint32_t* output) {
    if (!context || !state || !output) return 1;
    auto& self = *static_cast<Owner*>(context);
    // This direct field read's owner was captured by the original caller.
    // Keep it before the provider can replace State::owner.
    if (query == character_aggro_acquisition_prefix::Query::target_408_present)
        self.target_read_owner_ = subject;
    const auto& upstream = self.upstream_acquisition_services_;
    return upstream.invoke(upstream.context, state, query, subject, output);
}

std::int32_t Owner::capture_acquisition_props(void* context,
    character_aggro_acquisition_prefix::State* state,
    const character_aggro_acquisition_prefix::AiPropsTable** output) {
    if (!context || !state || !output) return 1;
    const auto& upstream = static_cast<Owner*>(context)->upstream_acquisition_services_;
    return upstream.capture_ai_props(upstream.context, state, output);
}

std::int32_t Owner::update_existing_target(character_aggro_acquisition_prefix::State* acquisition) {
    auto& report = *active_result_;
    report.retarget_entry_owner = target_read_owner_;
    if (!bindings_.retarget_services || !target_read_owner_) {
        report.source_aggro_status = static_cast<std::int32_t>(Status::unsupported_branch);
        return 1;
    }
    character_monster_retarget::State state{acquisition->ai, acquisition->owner};
    report.retarget_started = 1;
    const auto status = character_monster_retarget::update(&state, target_read_owner_,
        bindings_.retarget_services, &report.retarget);
    report.retarget_status = static_cast<std::int32_t>(status);
    acquisition->owner = state.owner;
    frame_owner_.owner = state.owner;
    if (status == character_monster_retarget::Status::complete) {
        report.source_aggro_status = 0;
        return 0;
    }
    if (status != character_monster_retarget::Status::unsupported_branch ||
        report.retarget.decision != character_monster_retarget::Decision::enemy_retention_search_boundary) {
        report.source_aggro_status = static_cast<std::int32_t>(Status::source_failed);
        return 1;
    }
    if (!bindings_.retention_services || !bindings_.resolve_retention_owner || retention_heap_.empty()) {
        report.source_aggro_status = static_cast<std::int32_t>(Status::unsupported_branch);
        return 1;
    }
    character_enemy_retention::Owner* owner = nullptr;
    if (bindings_.resolve_retention_owner(bindings_.character_registry_context, state.owner, &owner) ||
        !owner || owner->object.identity != state.owner) {
        report.source_aggro_status = static_cast<std::int32_t>(Status::source_failed);
        return 1;
    }
    character_enemy_retention::State retention{state.ai, owner};
    report.retention_started = 1;
    const auto retained = character_enemy_retention::update(&retention, report.retarget.current,
        retention_heap_.data(), static_cast<std::uint32_t>(retention_heap_.size()),
        bindings_.retention_services, &report.retention);
    report.retention_status = static_cast<std::int32_t>(retained);
    if (retention.owner) {
        acquisition->owner = retention.owner->object.identity;
        frame_owner_.owner = acquisition->owner;
    }
    report.source_aggro_status = retained == character_enemy_retention::Status::complete ? 0 :
        static_cast<std::int32_t>(Status::source_failed);
    return report.source_aggro_status ? 1 : 0;
}

void Owner::capture_observation(FrameResult& result) const noexcept {
    if (bindings_.script.set_target_state)
        result.target_identity = bindings_.script.set_target_state->target;
    if (bindings_.script.path_state) {
        result.path_nonempty = bindings_.script.path_state->path_nonempty;
        std::memcpy(result.path_target, bindings_.script.path_state->path_target,
                    sizeof(result.path_target));
    }
}

}  // namespace dh2::native::ghost_ai
