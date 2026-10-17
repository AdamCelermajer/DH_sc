#include "../app/src/main/cpp/native_character_list.hpp"
#define main ghost_ai_session_original_host_main
#include "../../level-world/tests/ghost_ai_session.cpp"
#undef main

#include "../app/src/main/cpp/ghost_ai_owner.hpp"
#include "../../level-world/character_ai_master_update.hpp"
#include "../../level-world/character_ai_update_target.hpp"
#include "../../level-world/ais_state_callbacks.hpp"

#include <array>
#include <algorithm>
#include <cstdio>
#include <stdexcept>

namespace {
using namespace dh2;
using namespace dh2::native::ghost_ai;

struct OwnerFixture final : Fixture {
    Owner owner;
    character_aggro_acquisition_prefix::State acquisition{};
    character_aggro_acquisition_prefix::Services acquisition_services{};
    character_aggro_acquisition_prefix::AiPropsRow rows[69]{};
    character_aggro_acquisition_prefix::AiPropsTable table{};
    dh2_random_state random{};
    character::AIFrameServices24 frame_services{};
    character::AIUpdateState80 character_update_state{};
    character::AIUpdateServices24 character_update_services{};
    ais_external_update::State ais_update_state{};
    ais_external_update::Services ais_update_services{};
    ais_state_callbacks::State state_callbacks{};
    Identity identity{};
    unsigned on_update_boundary = 0;
    unsigned master_updates = 0;
    unsigned target_updates = 0;
    character_monster_retarget::Services retarget_services{this, retarget_query};
    character_enemy_retention::Services retention_services{this, retention_query,
        {this, retention_search}};
    character_enemy_retention::Owner retention_owner{};
    character_enemy_retention::Object retention_player{};
    data::AggroEntry retention_entry{};
    data::AggroTable retention_map{&retention_entry, 1, 1};
    character_enemy_retention::AiRow retention_rows[69]{};
    character_enemy_retention::AiTable retention_table{retention_rows, 69};
    target_search::Entry16 retention_end{}, retention_item{};
    target_search::Room16 retention_room_end{}, retention_room{};
    character_enemy_retention::Registry retention_registry{&retention_room_end};
    character_enemy_retention::Level retention_level{&retention_registry};
    character_enemy_retention::Application retention_app{&retention_level, 0x990000001ull};
    character_enemy_retention::PlayerInfo retention_info{};
    character_enemy_retention::Point retention_origin{{0, 0, 0}}, retention_point{{100, 0, 0}};
    bool bind_existing = true, bind_retention = true, empty_retention = false;
    bool existing_enemy = true, switch_highest = false, mutate_target_read_owner = false;
    std::uintptr_t highest_subject = 0, resolve_subject = 0;
    unsigned clear_calls = 0, sync_calls = 0;

    static std::int32_t different_stop(void*,std::uintptr_t) { return 0; }
    static std::int32_t different_attack(void*,std::uintptr_t,std::uintptr_t) { return 0; }
    static OwnerFixture& from(void* raw) { return *static_cast<OwnerFixture*>(raw); }

    explicit OwnerFixture(std::uintptr_t base) : Fixture(base) {
        rows[68] = {0x44fa0000u, 0x44bb8000u}; // source AIProps aggro/view radii
        table = {rows, 69};
        acquisition = {ai_id, owner_id, {0, 0}};
        acquisition_services = {this, prefix_query, capture_props};
        random = {0x12345678u, 0u, 0u, 0u};
        identity = {base + 0x500, owner_id, ai_id, active_id, active_callee};
        ais_update_state = {active_id, owner_id, 0, 0};
        ais_update_services = {this, ais_invoke};
        character_update_state.active = active_id;
        character_update_state.owner = owner_id;
        character_update_state.machine_present = 1;
        character_update_state.state = 3; // The test actor has a live Idle machine.
        character_update_state.zoned = 1; // Avoid unrelated zoning services in this fixture.
        character_update_services = {this, character_update_query,
            1u << character::ai_update_active, 0};
        state_callbacks = {active_id, nullptr};
        frame_services = {this, frame_query,
            (1u << character::ai_frame_is_zonable) |
            (1u << character::ai_frame_update_target) |
            (1u << character::ai_frame_update_master), 0};
        retention_owner.object.identity = owner_id;
        retention_owner.object.visible = 1;
        retention_owner.outgoing = &retention_map;
        retention_player.identity = target_object_id;
        retention_player.visible = 1;
        retention_entry = {target_object_id, 0x3f800000u, 0};
        retention_rows[68].words[16] = 0x44bb8000u;
        retention_info.character_660 = target_object_id;
        retention_end.next = &retention_item;
        retention_item = {&retention_end, &retention_player};
        retention_room_end.next = &retention_room;
        retention_room = {&retention_room_end, &retention_end};
    }

    static std::int32_t capture_props(void* raw,
        character_aggro_acquisition_prefix::State*,
        const character_aggro_acquisition_prefix::AiPropsTable** out) {
        *out = &from(raw).table;
        return 0;
    }

    static std::int32_t prefix_query(void* raw,
        character_aggro_acquisition_prefix::State* state,
        character_aggro_acquisition_prefix::Query query,
        std::uintptr_t subject, std::uint32_t* out) {
        auto& self = from(raw);
        using Q = character_aggro_acquisition_prefix::Query;
        if (!state || !out) return 1;
        switch (query) {
        case Q::is_player: *out = 0; break;
        case Q::is_faerie: *out = 0; break;
        case Q::is_npc: *out = 0; break;
        case Q::is_monster: *out = subject == self.owner_id; break;
        case Q::is_remotely_updated: *out = 0; break;
        case Q::target_408_present:
            if (subject != self.owner_id) return 1;
            *out = self.target_state.target != 0;
            if (self.mutate_target_read_owner) state->owner = self.owner_id + 0x1000;
            break;
        case Q::target_418_present: *out = 0; break;
        case Q::get_char_ai_id: *out = 68; break;
        case Q::state_awaiting_to_spawn: *out = 0; break;
        case Q::has_aggro: *out = 0; break;
        case Q::spawn_radius_143c: *out = 0; break;
        case Q::is_my_turn: *out = 1; break;
        case Q::disable_optimization: *out = 1; break;
        case Q::frame_delta: *out = 16; break;
        }
        return 0;
    }

    static std::int32_t frame_query(void* raw, character::AIFrameState32* frame,
        const character::AIFrameRequest16* request, std::uint32_t* value) {
        auto& self = from(raw);
        if (!frame || !request || !value || frame->ai != self.ai_id) return 1;
        *value = 0;
        switch (request->service) {
        case character::ai_frame_is_zonable:
            self.trace.emplace_back("CharAI::IsZonable");
            return 0;
        case character::ai_frame_update_target: {
            ++self.target_updates;
            character_ai_update_target::Services services{&self, target_query};
            character_ai_update_target::Result result{};
            const auto status = character_ai_update_target::update(
                &self.target_state, &services, &result);
            self.trace.emplace_back("CharAI::_UpdateTarget");
            return status == character_ai_update_target::Status::complete ? 0 : 1;
        }
        case character::ai_frame_update_master: {
            ++self.master_updates;
            character_ai_master_update::State state{
                self.ai_id, self.owner_id, 0, 0, 0, 0};
            character_ai_master_update::Services services{&self, master_query};
            character_ai_master_update::Result result{};
            const auto status = character_ai_master_update::update(
                &state, &services, &result);
            self.trace.emplace_back("CharAI::_UpdateMaster(null-master)");
            return status == character_ai_master_update::Status::complete ? 0 : 1;
        }
        default:
            // Owner intercepts AISExternal::OnUpdate with its source kernel.
            self.on_update_boundary = 1;
            self.trace.emplace_back("AISExternal::OnUpdate:unsupported");
            return 1;
        }
    }

    static std::int32_t ais_invoke(void* raw, ais_external_update::State* state,
        const ais_external_update::Request* request) {
        auto& self = from(raw);
        using Op = ais_external_update::Operation;
        if (!state || !request || state->ais != self.identity.active_ais ||
            request->ais != self.identity.active_ais) return 1;
        switch (request->operation) {
        case Op::pause_character_ai:
        case Op::stop_character_controller:
        case Op::call_ais_on_update:
            return 1; // The fixture has no collision-persist counter or VCB override.
        case Op::call_state_update:
        case Op::call_state_conditions: {
            const auto callback = request->operation == Op::call_state_update ?
                ais_state_callbacks::Callback::update :
                ais_state_callbacks::Callback::conditions;
            ais_state_callbacks::Result result{};
            const ais_state_callbacks::Services services{nullptr, nullptr};
            const auto status = ais_state_callbacks::invoke(
                &self.state_callbacks, callback, &services, &result);
            self.trace.emplace_back(request->operation == Op::call_state_update ?
                "AIS:StateUpdate(null)" : "AIS:StateConditions(null)");
            return status == ais_state_callbacks::Status::complete && !result.called ? 0 : 1;
        }
        }
        return 1;
    }

    static std::int32_t character_update_query(void* raw,
        character::AIUpdateState80* state,
        const character::AIUpdateRequest32* request, std::uint32_t* result) {
        auto& self = from(raw);
        if (!state || !request || !result || state->active != self.identity.active_ais ||
            request->service != character::ai_update_active) return 1;
        return 1; // Owner supplies this vtable edge from AISExternal::update.
    }

    static std::int32_t target_query(void*, character_ai_update_target::State*,
        const character_ai_update_target::Request* request,
        character_ai_update_target::Response* response) {
        using Op = character_ai_update_target::Operation;
        if (!request || !response) return 1;
        switch (request->operation) {
        case Op::is_awaiting_spawn:
        case Op::is_in_limbus: response->word = 0; return 0;
        case Op::is_interactive: response->word = 1; return 0;
        case Op::char_ai_id: response->word = 68; return 0;
        case Op::is_dead: response->word = 0; return 0;
        case Op::is_in_sight: response->word = 0; return 0;
        case Op::raise_event: response->word = 0; return 0;
        default: return 1; // Target is null in the first acquisition frame.
        }
    }

    static std::int32_t master_query(void*, character_ai_master_update::State*,
        const character_ai_master_update::Request*,
        character_ai_master_update::Response*) {
        return 1; // Null master must not invoke any master-dependent service.
    }

    static std::int32_t retarget_query(void* raw, character_monster_retarget::State*,
        const character_monster_retarget::Request* q, character_monster_retarget::Response* r) {
        auto& f = from(raw);
        using Op = character_monster_retarget::Operation;
        switch (q->operation) {
        case Op::highest_aggro:
            f.highest_subject = q->subject;
            r->identity = f.switch_highest ? f.target_object_id + 0x100 : f.target_state.target; break;
        case Op::resolve_target_408: f.resolve_subject = q->subject; r->identity = f.target_state.target; break;
        case Op::get_aggro: r->word = q->peer == f.target_state.target ? 0x3f800000u : 0x447a0000u; break;
        case Op::design_factor_8: r->word = 0x3f800000u; break;
        case Op::diagnostic_switch: break;
        case Op::set_target: f.target_state.target = q->peer; break;
        case Op::is_enemy: r->word = f.existing_enemy; break;
        case Op::clear_aggro: ++f.clear_calls; break;
        case Op::sync_last_target: ++f.sync_calls; break;
        }
        return 0;
    }
    static std::int32_t resolve_retention_owner(void* raw, std::uintptr_t identity,
        character_enemy_retention::Owner** out) {
        auto& f = from(raw); *out = identity == f.owner_id ? &f.retention_owner : nullptr;
        return 0;
    }
    static std::int32_t retention_query(void* raw, character_enemy_retention::State*,
        const character_enemy_retention::Request* q, character_enemy_retention::Response* r) {
        auto& f = from(raw); using Op = character_enemy_retention::Operation;
        switch (q->operation) {
        case Op::application: r->view = &f.retention_app; break;
        case Op::ai_table: r->view = &f.retention_table; break;
        case Op::char_ai_id: r->word = 68; break;
        case Op::get_local_player:
            require(q->word == 0 && q->extra == 1, "retention local player arguments changed");
            r->view = &f.retention_info; break;
        case Op::is_enemy: r->identity = 1; break;
        case Op::design_30: r->word = 0x3f800000u; break;
        case Op::add_aggro: return 1; // These cases already hold genuine membership.
        case Op::clear_aggro: ++f.clear_calls; break;
        case Op::set_target: f.target_state.target = q->peer; break;
        case Op::sync_last_target: ++f.sync_calls; break;
        }
        return 0;
    }
    static std::int32_t retention_search(void* raw, character_enemy_retention::List*,
        const character_enemy_retention::SearchRequest* q, character_enemy_retention::SearchResponse* r) {
        auto& f = from(raw); using Op = character_enemy_retention::SearchOperation;
        switch (q->operation) {
        case Op::is_character: r->identity = 1; break;
        case Op::look_vector: r->point = {{1, 0, 0}}; break;
        case Op::target_position: r->view = q->subject == f.target_object_id ? &f.retention_point : &f.retention_origin; break;
        case Op::diagnostic_switch: break;
        case Op::melee_radius: r->number = 0; break;
        case Op::resolve_character: r->view = q->subject == f.target_object_id ? &f.retention_player : nullptr; break;
        case Op::is_zonable: r->identity = 0; break;
        case Op::is_interactive: r->identity = !f.empty_retention; break;
        case Op::is_dead: r->identity = 0; break;
        case Op::is_enemy: r->identity = 1; break;
        case Op::is_player: r->identity = q->subject == f.target_object_id; break;
        case Op::angle: r->number = 0; break;
        case Op::interaction_radius: r->number = 0; break;
        }
        return 0;
    }

    native::ghost_ai::Bindings make_owner_bindings(const std::string& commons, const std::string& monster) {
        native::ghost_ai::Bindings bindings{};
        bindings.identity = identity;
        bindings.script = make_bindings();
        bindings.commons = {commons.data(), commons.size()};
        bindings.monster = {monster.data(), monster.size()};
        bindings.frame_services = &frame_services;
        bindings.ai_update_state = &character_update_state;
        bindings.ai_update_services = &character_update_services;
        bindings.ais_update_state = &ais_update_state;
        bindings.ais_update_services = &ais_update_services;
        bindings.acquisition_state = &acquisition;
        bindings.acquisition_services = &acquisition_services;
        bindings.retarget_services = bind_existing ? &retarget_services : nullptr;
        bindings.retention_services = bind_retention ? &retention_services : nullptr;
        bindings.resolve_retention_owner = bind_retention ? resolve_retention_owner : nullptr;
        bindings.random = &random;
        bindings.rooms = &rooms;
        bindings.character_registry_context = this;
        bindings.resolve_character = resolve_character;
        bindings.candidate_capacity = 4;
        return bindings;
    }

    bool bind(const std::string& commons, const std::string& monster) {
        const auto bindings=make_owner_bindings(commons,monster);
        std::string error;
        return owner.bind(bindings, error) == native::ghost_ai::Status::complete && error.empty();
    }

    static std::int32_t resolve_character(void* raw, std::uintptr_t id,
        character::aggro_search::Character** out) {
        auto& self = from(raw);
        if (!out) return 1;
        if (id == self.owner_id || id == self.owner_object_id) *out = &self.owner_character;
        else if (id == self.target_object_id) *out = &self.target_character;
        else *out = nullptr;
        return 0;
    }
};

std::uintptr_t create_vm_then_pending_callbacks(OwnerFixture& f,
    character::ScriptLifecycleState64& lifecycle,monster_external_script::Session& vm,
    monster_external_script::Services& services,std::shared_ptr<void>& lifetime,
    const std::string& commons,const std::string& monster,std::string& error) {
    // Source SetScript calls AISExternal/LuaScript construction at 0x3ccb68
    // BEFORE publishing Character+0x3e4 at 0x3ccb6c. No active or pending
    // identity is fabricated to make callback preparation succeed earlier.
    lifecycle={f.owner_id,0,0,1,6,-1,-1,0,1,0,0,0};
    f.enemy_state.active={0,0};f.event_state.active=0;
    monster_external_script::Services constructor{};constructor.owner=f.owner_id;
    require(vm.create(constructor,error)==monster_external_script::Status::complete &&
        vm.stage()==monster_external_script::Stage::created && vm.vm_identity()!=0 &&
        !lifecycle.pending && !lifecycle.active && vm.statistics().source_functions_bound==0,
        "constructor VM did not precede pending publication");
    const auto identity=vm.vm_identity();
    require(f.owner.prepare_pending(f.make_bindings(),&lifecycle,services,lifetime,error)==
        native::ghost_ai::Status::invalid_argument && !lifetime && !f.owner.ready() &&
        vm.vm_identity()==identity,"callback owner accepted unpublished pending AIS");
    lifecycle.pending=f.active_id;
    require(f.owner.prepare_pending(f.make_bindings(),&lifecycle,services,lifetime,error)==
        native::ghost_ai::Status::complete && lifetime && !f.owner.ready() &&
        vm.install_created_services(services,error,lifetime)==monster_external_script::Status::complete &&
        vm.vm_identity()==identity && vm.bind_ais_functions(error)==monster_external_script::Status::complete &&
        vm.bind_character_functions(error)==monster_external_script::Status::complete &&
        vm.load_common({commons.data(),commons.size()},error)==monster_external_script::Status::complete &&
        vm.load_external({monster.data(),monster.size()},error)==monster_external_script::Status::complete &&
        vm.vm_identity()==identity && !lifecycle.active && lifecycle.pending==f.active_id,
        "prepared callbacks did not join the original constructor VM in source order");
    return identity;
}

void run_owner_pipeline(const std::string& commons, const std::string& monster) {
    OwnerFixture f(0x700000000ull);
    require(f.bind(commons, monster), "native source owner bind failed");
    const FrameInput input{f.identity,
        {f.owner_id, f.owner_id + 0x80, 0x100, 0, 0, 0, 0, 0, 0, 0}, 0, 0};
    FrameResult result{};
    const auto status = f.owner.tick(input, &result);
    require(status == native::ghost_ai::Status::complete,
            "null-state AISExternal/AISDefault update did not complete");
    require(!f.on_update_boundary && f.target_updates == 1 && f.master_updates == 1,
            "source frame ordering did not reach target/master/AIS services");
    require(result.ais_update_status == static_cast<std::int32_t>(ais_external_update::Status::complete) &&
            result.ais_update_calls == 2 &&
            result.ais_update.phase == ais_external_update::Phase::complete,
            "AISExternal update did not reach both null-state callbacks");
    require(result.character_update_status == 0 && result.character_update.phase == 9 &&
            result.character_update.last_service == character::ai_update_active,
            "CharAI::OnUpdate wrapper did not complete after the active AIS virtual");
    require(result.frame.last_service == character::ai_frame_on_update &&
            result.frame.phase == 6,
            "source frame did not complete after the AIS OnUpdate service");
    require(result.source_search_started == 1 && result.candidate_count == 1 &&
            result.scan.enemy_callbacks == 1 && result.scan.script_dispatches == 1 &&
            result.scan.path_requests == 1,
            "source aggro/search/script/controller path did not complete");
    require(f.target_state.target == f.target_object_id && f.path_state.path_nonempty &&
            f.path_calls == 1 && f.requested_path[0] == 100.f,
            "Lua target and HeadTo effects did not reach PathTo/FindPath");
    const auto zonable = std::find(f.trace.begin(), f.trace.end(), "CharAI::IsZonable");
    const auto target_update = std::find(f.trace.begin(), f.trace.end(), "CharAI::_UpdateTarget");
    const auto master_update = std::find(f.trace.begin(), f.trace.end(), "CharAI::_UpdateMaster(null-master)");
    require(zonable != f.trace.end() && target_update != f.trace.end() &&
            master_update != f.trace.end() && zonable < target_update && target_update < master_update,
            "CharAI source dispatcher service order changed");
    std::printf("{\"ghost_ai_owner_host_cases\":9,\"existing_target_cases\":6,\"pending_vm_shared\":true,\"constructor_before_pending_cases\":3,\"flat_character_owner_cases\":4,\"flat_published_vm_shared\":true,\"manager_cursor_owner_cases\":2,\"manager_cursor_live_links\":true,\"combat_vm_dispatch\":true,\"status\":\"PASS\","
        "\"frame_status\":%d,\"last_service\":%u,\"candidates\":%u,"
        "\"events\":%u,\"script_callbacks\":%u,\"set_target_calls\":%u,"
        "\"head_to_calls\":%u,\"path_count\":%u,\"target_id\":%llu,"
        "\"ais_update_calls\":%u,\"mismatches\":0}\n",
        result.source_frame_status, result.frame.last_service,
        result.candidate_count, result.scan.events_raised,
        result.scan.script_dispatches, result.scan.set_target_calls,
        result.scan.head_to_calls, result.scan.path_requests,
        static_cast<unsigned long long>(result.target_identity), result.ais_update_calls);
}

void run_combat_result_owner(const std::string& commons, const std::string& monster) {
    const std::string callbacks = monster + R"lua(
function NativeCombatResult(...)
    assert(select('#', ...) == 2, 'combat callback argument count changed')
    local attacker, defender = ...
    assert(type(attacker) == 'table' and type(attacker._this) == 'userdata',
           'combat attacker is not Character userdata')
    assert(type(defender) == 'table' and type(defender._this) == 'userdata',
           'combat defender is not Character userdata')
    assert(attacker._this == GetTarget()._this,
           'source attacker was not the first callback argument')
    assert(attacker._this ~= defender._this, 'combat Character identities were aliased')
end
AddToVFTable('OnTargetHit', 'NativeCombatResult')
AddToVFTable('OnTargetMissed', 'NativeCombatResult')
)lua";
    OwnerFixture f(0x790000000ull);
    require(f.bind(commons, callbacks), "combat callback source AIS owner failed to bind");
    f.target_state.target = f.target_object_id;
    const auto before = f.owner.script_statistics().completed_callbacks;
    std::string error;
    require(f.owner.dispatch_combat_result(monster_external_script::Event::target_hit,
                f.target_object_id, f.owner_id, error) == dh2::native::ghost_ai::Status::complete && error.empty() &&
            f.owner.ready() && f.owner.script_statistics().completed_callbacks == before + 1,
            "OnTargetHit did not execute on the active Monster AIS VM");
    require(f.owner.dispatch_combat_result(monster_external_script::Event::target_missed,
                f.target_object_id, f.owner_id, error) == dh2::native::ghost_ai::Status::complete && error.empty() &&
            f.owner.ready() && f.owner.script_statistics().completed_callbacks == before + 2,
            "OnTargetMissed did not execute on the same active Monster AIS VM");
    require(f.owner.dispatch_combat_result(monster_external_script::Event::target_hit,
                f.target_object_id, f.target_object_id + 0x100, error) == dh2::native::ghost_ai::Status::invalid_argument &&
            f.owner.script_statistics().completed_callbacks == before + 2,
            "combat dispatch accepted an AIS owner absent from the source participants");
}

void run_flat_owner(const std::string& commons, const std::string& monster) {
    using character::aggro_character_list::Entry;
    using character::aggro_character_list::CharacterList;
    { OwnerFixture f(0x780000000ull);Entry end{},enemy{&end,&f.target_character};end.next=&enemy;
      CharacterList chars{&end,&enemy,&end};auto bindings=f.make_owner_bindings(commons,monster);
      bindings.rooms=nullptr;bindings.characters=&chars;f.room_sentinel.next=&f.room_sentinel;
      std::string error;require(f.owner.bind(bindings,error)==native::ghost_ai::Status::complete,
        "flat native owner binding failed");
      const FrameInput input{f.identity,{f.owner_id,f.owner_id+0x80,0x100,0,0,0,0,0,0,0},0,0};FrameResult r{};
      require(f.owner.tick(input,&r)==native::ghost_ai::Status::complete && r.source_search_started &&
        r.scan.script_dispatches==1 && r.scan.path_requests==1 && r.target_identity==f.target_object_id &&
        f.owner.script_statistics().completed_callbacks==1,
        "native frame did not use flat Character producer for original Lua/path dispatch"); }
    { OwnerFixture f(0x790000000ull);Entry end{};end.next=&end;CharacterList chars{&end,&end,&end};
      auto bindings=f.make_owner_bindings(commons,monster);bindings.characters=&chars;std::string error;
      require(f.owner.bind(bindings,error)==native::ghost_ai::Status::invalid_argument && !f.owner.ready(),
        "native owner accepted ambiguous room and flat Character producers"); }
    { OwnerFixture f(0x7a0000000ull);
      f.mutate_target_during_search=true;f.search_replacement_target=f.target_object_id;f.expected_event12_payload=f.target_object_id;
      Entry end{};end.next=&end;CharacterList chars{&end,&end,&end};
      auto bindings=f.make_owner_bindings(commons,monster);bindings.rooms=nullptr;bindings.characters=&chars;
      std::string error;require(f.owner.bind(bindings,error)==native::ghost_ai::Status::complete,
        "empty flat native owner binding failed");
      const FrameInput input{f.identity,{f.owner_id,f.owner_id+0x80,0x100,0,0,0,0,0,0,0},0,0};FrameResult r{};
      require(f.owner.tick(input,&r)==native::ghost_ai::Status::complete && r.source_search_started &&
        !r.candidate_count && !r.scan.script_dispatches && f.event12_calls==1 && !f.path_calls,
        "empty native flat producer fell back to historical room membership"); }
    { monster_external_script::Session vm;OwnerFixture f(0x7b0000000ull);
      character::ScriptLifecycleState64 lifecycle{};monster_external_script::Services services{};
      std::shared_ptr<void> lifetime;std::string error;
      const auto identity=create_vm_then_pending_callbacks(f,lifecycle,vm,services,lifetime,commons,monster,error);
      publish_pending(f,lifecycle);Entry end{},enemy{&end,&f.target_character};end.next=&enemy;CharacterList chars{&end,&enemy,&end};
      auto bindings=f.make_owner_bindings(commons,monster);bindings.rooms=nullptr;bindings.characters=&chars;
      require(f.owner.bind_staged(bindings,vm,error)==native::ghost_ai::Status::complete && vm.uses_services(services),
        "flat native owner did not adopt the exact published VM");
      const FrameInput input{f.identity,{f.owner_id,f.owner_id+0x80,0x100,0,0,0,0,0,0,0},0,0};FrameResult r{};
      require(f.owner.tick(input,&r)==native::ghost_ai::Status::complete && r.scan.script_dispatches==1 &&
        vm.statistics().completed_callbacks==1 && f.owner.script_statistics().completed_callbacks==1 &&
        vm.vm_identity()==identity,"flat native acquisition created a duplicate VM or missed source callbacks");
      require(f.owner.reset(error)==native::ghost_ai::Status::complete &&
        vm.reset(error)==monster_external_script::Status::complete,"flat borrowed callback teardown failed"); }
}

void run_manager_cursor_owner(const std::string& commons, const std::string& monster) {
    namespace manager = character::aggro::object_manager_list;
    struct Ring {
        native::character_list::Owner owned;
        const auto& methods() const {return owned.methods();}
        const auto& source() const {return owned.source();}
        auto owned_nodes() const {return owned.owned_nodes();}
        auto remove_after_remove(character::aggro_search::Character* p,std::size_t* n) {return owned.remove_after_remove(p,n);}
        void append(character::aggro_search::Character* p) {
            bool added=false;
            require(owned.enroll_after_add(p,false,&added)==manager::Status::ok && added,
                "native owned Character node enrollment failed");
        }
    };
    { OwnerFixture f(0x7c0000000ull);Ring ring;
      character::aggro_search::GameObject skipped_object=f.target_object;skipped_object.identity+=0x100;
      character::aggro_search::Character skipped=f.target_character;skipped.identity+=0x100;skipped.object=&skipped_object;
      ring.append(&f.target_character);ring.append(&skipped);
      const auto& list=ring.methods();
      struct Removal {OwnerFixture* fixture;Ring* ring;character::aggro_search::Character* skipped;unsigned calls=0;};
      Removal removal{&f,&ring,&skipped};f.search_services.context=&removal;
      f.search_services.invoke=[](void* raw,const character::aggro_search::Request* req,character::aggro_search::Response* out) {
          auto& s=*static_cast<Removal*>(raw);const auto rc=Fixture::search_invoke(s.fixture,req,out);
          if(!rc && req->operation==character::aggro_search::is_interactive && !s.calls) {
              std::size_t removed=0;
              if(s.ring->remove_after_remove(s.skipped,&removed)!=manager::Status::ok || removed!=1) return 1;
              ++s.calls;
          }
          return rc;
      };
      auto bindings=f.make_owner_bindings(commons,monster);bindings.rooms=nullptr;bindings.objects=&list;
      std::string error;require(f.owner.bind(bindings,error)==native::ghost_ai::Status::complete,
          "manager cursor native owner bind failed");
      const FrameInput input{f.identity,{f.owner_id,f.owner_id+0x80,0x100,0,0,0,0,0,0,0},0,0};FrameResult r{};
      require(f.owner.tick(input,&r)==native::ghost_ai::Status::complete && removal.calls==1 &&
          ring.source().character_count==1 && ring.owned_nodes()==1 && r.candidate_count==1 && r.scan.script_dispatches==1 &&
          r.scan.path_requests==1 && r.target_identity==f.target_object_id,
          "live manager link removal was snapshotted or did not reach original Lua/path");
      require(f.owner.reset(error)==native::ghost_ai::Status::complete,"manager owner detach failed"); }
    { monster_external_script::Session vm;OwnerFixture f(0x7d0000000ull);Ring ring;ring.append(&f.target_character);
      const auto& list=ring.methods();
      character::ScriptLifecycleState64 lifecycle{};monster_external_script::Services services{};
      std::shared_ptr<void> lifetime;std::string error;
      const auto identity=create_vm_then_pending_callbacks(f,lifecycle,vm,services,lifetime,commons,monster,error);
      publish_pending(f,lifecycle);auto bindings=f.make_owner_bindings(commons,monster);bindings.rooms=nullptr;bindings.objects=&list;
      require(f.owner.bind_staged(bindings,vm,error)==native::ghost_ai::Status::complete && vm.uses_services(services),
          "manager cursor native owner duplicated the published VM");
      const FrameInput input{f.identity,{f.owner_id,f.owner_id+0x80,0x100,0,0,0,0,0,0,0},0,0};FrameResult r{};
      require(f.owner.tick(input,&r)==native::ghost_ai::Status::complete && r.scan.script_dispatches==1 &&
          r.scan.path_requests==1 && vm.statistics().completed_callbacks==1 && vm.vm_identity()==identity,
          "published VM did not receive manager cursor source acquisition callback");
      require(f.owner.reset(error)==native::ghost_ai::Status::complete && vm.reset(error)==monster_external_script::Status::complete,
          "manager borrowed callback teardown failed"); }
}

void run_existing_targets(const std::string& commons, const std::string& monster) {
    const auto run = [&](OwnerFixture& f, FrameResult& r) {
        require(f.bind(commons, monster), "existing-target owner bind failed");
        f.target_state.target = f.target_object_id;
        f.target_state.alive_snapshot = 1;
        f.target_state.sight_snapshot = 0;
        const FrameInput input{f.identity,
            {f.owner_id, f.owner_id + 0x80, 0x100, 0, 0, 0, 0, 0, 0, 0}, 0, 0};
        return f.owner.tick(input, &r);
    };
    { OwnerFixture f(0x710000000ull); FrameResult r{};
      require(run(f, r) == native::ghost_ai::Status::complete && r.retarget_started && r.retention_started &&
        r.retention.decision == character_enemy_retention::Decision::retained_known_player &&
        r.target_identity == f.target_object_id && !r.source_search_started && !r.scan.script_dispatches,
        "existing enemy retention did not preserve target without replaying acquisition"); }
    { OwnerFixture f(0x720000000ull); FrameResult r{}; f.empty_retention = true;
      require(run(f, r) == native::ghost_ai::Status::complete && r.retention_started &&
        r.retention.decision == character_enemy_retention::Decision::cleared_empty_search &&
        !r.target_identity && f.clear_calls == 1 && f.sync_calls == 1,
        "empty retention did not clear and sync the source target"); }
    { OwnerFixture f(0x730000000ull); FrameResult r{}; f.switch_highest = true;
      require(run(f, r) == native::ghost_ai::Status::complete && !r.retention_started &&
        r.retarget.decision == character_monster_retarget::Decision::switched_to_highest &&
        r.target_identity == f.target_object_id + 0x100,
        "highest-threat retarget did not use the source branch"); }
    { OwnerFixture f(0x740000000ull); FrameResult r{}; f.existing_enemy = false;
      require(run(f, r) == native::ghost_ai::Status::complete && !r.retention_started &&
        r.retarget.decision == character_monster_retarget::Decision::cleared_non_enemy &&
        !r.target_identity && f.clear_calls == 1 && f.sync_calls == 1,
        "nonenemy retarget did not clear original AI target"); }
    { OwnerFixture f(0x750000000ull); FrameResult r{}; f.bind_retention = false;
      require(run(f, r) == native::ghost_ai::Status::unsupported_branch && r.retarget_started &&
        !r.retention_started && !r.source_search_started && r.target_identity == f.target_object_id,
        "missing retention provider was replaced by ordinary acquisition"); }
    { OwnerFixture f(0x760000000ull); FrameResult r{}; f.mutate_target_read_owner = true; f.switch_highest = true;
      // The later native identity gate correctly rejects a replaced owner;
      // the original aggro branch must still preserve its captured entry.
      require(run(f, r) == native::ghost_ai::Status::source_failed && r.retarget_started &&
        r.retarget_status == 0 && r.retarget_entry_owner == f.owner_id &&
        f.highest_subject == f.owner_id && f.resolve_subject == f.owner_id + 0x1000 &&
        r.target_identity == f.target_object_id + 0x100,
        "target-read owner capture or fresh retarget owner loads changed"); }
}

void run_pending_owner(const std::string& commons, const std::string& monster) {
    monster_external_script::Session vm;
    OwnerFixture f(0x770000000ull);
    character::ScriptLifecycleState64 lifecycle{};
    monster_external_script::Services services{};
    std::shared_ptr<void> lifetime;
    std::string error;
    const auto identity=create_vm_then_pending_callbacks(f,lifecycle,vm,services,lifetime,commons,monster,error);
    auto bindings=f.make_owner_bindings(commons,monster);
    require(f.owner.bind_staged(bindings,vm,error)==native::ghost_ai::Status::script_not_ready &&
                !f.owner.ready() && lifecycle.active==0,
            "native frame owner adopted before source pending publication");
    publish_pending(f,lifecycle);
    auto mismatched=bindings;
    character::PathToState40 other_path=f.path_state;
    mismatched.script.path_state=&other_path;
    require(f.owner.bind_staged(mismatched,vm,error)==native::ghost_ai::Status::invalid_argument && !f.owner.ready(),
            "native frame owner accepted different pending callback backing");
    auto mismatched_stop=bindings;
    mismatched_stop.script.script_queries.stop=OwnerFixture::different_stop;
    require(f.owner.bind_staged(mismatched_stop,vm,error)==native::ghost_ai::Status::invalid_argument && !f.owner.ready(),
            "native frame owner accepted a different Stop service table");
    auto mismatched_attack=bindings;
    mismatched_attack.script.script_queries.attack=OwnerFixture::different_attack;
    require(f.owner.bind_staged(mismatched_attack,vm,error)==native::ghost_ai::Status::invalid_argument && !f.owner.ready(),
            "native frame owner accepted a different Attack service table");
    require(f.owner.bind_staged(bindings,vm,error)==native::ghost_ai::Status::complete &&
                f.owner.ready() && vm.uses_services(services),
            "native frame owner duplicated or rejected the published pending VM");
    const FrameInput input{f.identity,
        {f.owner_id,f.owner_id+0x80,0x100,0,0,0,0,0,0,0},0,0};
    FrameResult result{};
    require(f.owner.tick(input,&result)==native::ghost_ai::Status::complete &&
                result.scan.script_dispatches==1 && result.scan.path_requests==1 &&
                vm.statistics().completed_callbacks==1 && f.owner.script_statistics().completed_callbacks==1 &&
                vm.vm_identity()==identity,
            "native frame callbacks did not use the one published AIS VM");
    lifetime.reset();
    require(f.owner.reset(error)==native::ghost_ai::Status::complete && vm.ready() && vm.vm_identity()==identity,
            "frame owner reset destroyed the AIS-owned VM");
    float property=123.f;
    require(services.get_prop(services.context,f.owner_id,28,&property)==0 && property==-1.f,
            "retained AIS VM lost its callback context after owner detach");
    require(vm.reset(error)==monster_external_script::Status::complete,
            "native frame pending VM teardown failed");
}
} // namespace

int main(int argc, char** argv) {
    try {
        require(argc == 3, "expected original _commons and monster script paths");
        const auto commons = read_file(argv[1]);
        const auto monster = read_file(argv[2]);
        run_manager_cursor_owner(commons, monster);
        run_flat_owner(commons, monster);
        run_pending_owner(commons, monster);
        run_existing_targets(commons, monster);
        run_owner_pipeline(commons, monster);
        run_combat_result_owner(commons, monster);
        return 0;
    } catch (const std::exception& error) {
        std::fprintf(stderr, "%s\n", error.what());
        return 1;
    }
}
