#include "ghost_ai_session.hpp"

#include <cstddef>
#include <new>
#include <utility>

namespace dh2::ghost_ai_session {
namespace {
using SearchRequest = character::aggro_search::Request;
using SearchResponse = character::aggro_search::Response;
using RelationKind = character_aggro_candidate_events::Relation;
using RelationQuery = character_ai_relations::Relation;

bool overlaps(const void* a, std::size_t an, const void* b, std::size_t bn) {
    const auto x = reinterpret_cast<std::uintptr_t>(a);
    const auto y = reinterpret_cast<std::uintptr_t>(b);
    if (x > UINTPTR_MAX - an || y > UINTPTR_MAX - bn) return true;
    return x < y + bn && y < x + an;
}

struct BusyScope {
    bool& busy;
    explicit BusyScope(bool& flag) : busy(flag) { busy = true; }
    ~BusyScope() { busy = false; }
};

struct HoldToken;
}  // namespace

struct ActorSession::Impl : std::enable_shared_from_this<ActorSession::Impl> {
    Bindings b{};
    character_enemy_spotted::Services enemy_source{};
    character::AIEventServices24 event_source{};
    character_ai_relations::Services relation_source{};
    character::set_target::Services set_target_source{};
    character::aggro_search::Services search_source{};
    ScriptQueries script_source{};
    character::CharacterControlServices16 control_source{};
    character::PathToServices16 path_source{};

    character_enemy_spotted::Services enemy_bound{};
    character::AIEventServices24 event_bound{};
    character_ai_relations::Services relation_bound{};
    character::set_target::Services set_target_bound{};
    character::aggro_search::Services search_bound{};
    character::CharacterControlServices16 control_bound{};
    character::PathToServices16 path_bound{};
    monster_external_script::Services script_bound{};
    monster_external_script::Session owned_script;
    monster_external_script::Session* script_vm = nullptr;

    std::uintptr_t ai = 0, owner = 0, active = 0, active_callee = 0;
    const character::ScriptLifecycleState64* lifecycle = nullptr;
    bool pending_phase = false;
    bool busy = false;
    std::uint32_t active_holds = 0;
    ScanResult* current_result = nullptr;
    character_aggro_candidate_events::State* current_candidates = nullptr;
    std::weak_ptr<Impl> self;
    std::string callback_error;
    std::int32_t last_ai_event_status = -1;
    std::int32_t last_enemy_gate_status = -1;

    explicit Impl(const Bindings& bindings) : b(bindings) {
        ai = b.ai_identity; owner = b.owner_identity;
        active = b.active_ais_identity; active_callee = b.active_ais_callee;
        enemy_source = *b.enemy_services;
        event_source = *b.event_services;
        relation_source = *b.relation_services;
        set_target_source = *b.set_target_services;
        search_source = *b.search_services;
        script_source = b.script_queries;
        control_source = *b.control_services;
        path_source = *b.path_services;

        enemy_bound = {this, enemy_source.debug_switch ? enemy_debug : nullptr,
            enemy_group, enemy_awaiting, enemy_limbus,
            enemy_combat, enemy_player, enemy_aggro, enemy_initial, enemy_add,
            retain_active, dispatch_active, release_active};
        event_bound = {this, invoke_ai_event, event_source.available, 0};
        relation_bound = {this, relation_resolve, relation_word_f4, relation_faction,
            relation_faction_count, relation_player, relation_table, relation_interactive,
            relation_type};
        set_target_bound = {this, set_target_source.ai_property_count, invoke_set_target};
        search_bound = {this, invoke_search};
        control_bound = {this, invoke_control};
        path_bound = {this, invoke_find_path};
        script_bound = {this, owner, script_struct, script_prop, script_constant,
            script_has_target, script_get_target, script_get_state, script_has_path,
            script_set_target, script_head_to, script_move_to};
        script_bound.get_py_oid = script_oid;
        script_bound.get_position = script_position;
        script_bound.get_host_player_level = script_host_level;
        script_bound.get_host_player_difficulty = script_host_difficulty;
        script_bound.get_current_level_range = script_level_range;
        script_bound.set_level = script_level_set;
        script_bound.stop = script_source.stop ? script_stop : nullptr;
        script_bound.attack = script_source.attack ? script_attack : nullptr;
    }

    monster_external_script::Session* active_script() noexcept { return script_vm; }
    const monster_external_script::Session* active_script() const noexcept { return script_vm; }

    bool live() const noexcept {
        if (!b.enemy_state || !b.event_state || !b.relation_state ||
            !b.set_target_state || !b.controller_state || !b.path_state ||
            !b.event_state->owner || !b.set_target_state->owner) return false;
        if (lifecycle && (lifecycle->owner != owner ||
            (pending_phase ? lifecycle->pending != active : lifecycle->active != active))) return false;
        const bool selected_live = pending_phase ?
            b.enemy_state->active.identity == lifecycle->active :
            b.enemy_state->active.identity == active && b.enemy_state->active.callee == active_callee;
        return b.enemy_state->ai_identity == ai &&
            b.enemy_state->owner_identity == owner &&
            selected_live &&
            b.event_state->ai == ai && b.event_state->owner->owner == owner &&
            b.relation_state->ai == ai && b.relation_state->owner == owner &&
            b.set_target_state->identity == ai &&
            b.set_target_state->owner->identity == owner &&
            b.controller_state->owner == owner && b.path_state->owner == owner;
    }

    static std::int32_t enemy_debug(void* raw, character_enemy_spotted::State* state,
                                    character_enemy_spotted::DebugPoint point) {
        auto& s = *static_cast<Impl*>(raw);
        if (!s.live() || state != s.b.enemy_state || !s.enemy_source.debug_switch) return 1;
        try { return s.enemy_source.debug_switch(s.enemy_source.context, state, point) || !s.live(); }
        catch (...) { return 1; }
    }
    static std::int32_t enemy_group(void* raw, character_enemy_spotted::State* state,
        std::uintptr_t group, std::uintptr_t owner_id, std::uintptr_t enemy) {
        auto& s = *static_cast<Impl*>(raw);
        if (!s.live() || state != s.b.enemy_state || !s.enemy_source.group_enemy_spotted) return 1;
        try { return s.enemy_source.group_enemy_spotted(s.enemy_source.context, state,
                    group, owner_id, enemy) || !s.live(); }
        catch (...) { return 1; }
    }
    static std::int32_t enemy_awaiting(void* raw, character_enemy_spotted::State* state,
        std::uintptr_t character_id, std::uint32_t* value) {
        auto& s = *static_cast<Impl*>(raw);
        if (!s.live() || state != s.b.enemy_state || !s.enemy_source.is_awaiting_to_spawn) return 1;
        try { const auto rc=s.enemy_source.is_awaiting_to_spawn(s.enemy_source.context,state,character_id,value);return rc||!s.live()?1:0; }
        catch (...) { return 1; }
    }
    static std::int32_t enemy_limbus(void* raw, character_enemy_spotted::State* state,
        std::uintptr_t character_id, std::uint32_t* value) {
        auto& s = *static_cast<Impl*>(raw);
        if (!s.live() || state != s.b.enemy_state || !s.enemy_source.is_in_limbus) return 1;
        try { const auto rc=s.enemy_source.is_in_limbus(s.enemy_source.context,state,character_id,value);return rc||!s.live()?1:0; }
        catch (...) { return 1; }
    }
    static std::int32_t enemy_combat(void* raw, character_enemy_spotted::State* state,
        std::uintptr_t ai_id, std::uint32_t* value) {
        auto& s = *static_cast<Impl*>(raw);
        if (!s.live() || state != s.b.enemy_state || !s.enemy_source.is_in_combat) return 1;
        try { const auto rc=s.enemy_source.is_in_combat(s.enemy_source.context,state,ai_id,value);return rc||!s.live()?1:0; }
        catch (...) { return 1; }
    }
    static std::int32_t enemy_player(void* raw, character_enemy_spotted::State* state,
        std::uintptr_t character_id, std::uint32_t* value) {
        auto& s = *static_cast<Impl*>(raw);
        if (!s.live() || state != s.b.enemy_state || !s.enemy_source.is_player) return 1;
        try { const auto rc=s.enemy_source.is_player(s.enemy_source.context,state,character_id,value);return rc||!s.live()?1:0; }
        catch (...) { return 1; }
    }
    static std::int32_t enemy_aggro(void* raw, character_enemy_spotted::State* state,
        std::uintptr_t ai_id, std::uintptr_t enemy_id, std::uint32_t* bits) {
        auto& s = *static_cast<Impl*>(raw);
        if (!s.live() || state != s.b.enemy_state || !s.enemy_source.get_aggro) return 1;
        try { const auto rc=s.enemy_source.get_aggro(s.enemy_source.context,state,ai_id,enemy_id,bits);return rc||!s.live()?1:0; }
        catch (...) { return 1; }
    }
    static std::int32_t enemy_initial(void* raw, character_enemy_spotted::State* state,
                                      std::uint32_t* bits) {
        auto& s = *static_cast<Impl*>(raw);
        if (!s.live() || state != s.b.enemy_state || !s.enemy_source.initial_aggro) return 1;
        try { const auto rc=s.enemy_source.initial_aggro(s.enemy_source.context,state,bits);return rc||!s.live()?1:0; }
        catch (...) { return 1; }
    }
    static std::int32_t enemy_add(void* raw, character_enemy_spotted::State* state,
        std::uintptr_t owner_id, std::uintptr_t enemy_id, std::uint32_t amount,
        std::uint32_t* delta) {
        auto& s = *static_cast<Impl*>(raw);
        if (!s.live() || state != s.b.enemy_state || !s.enemy_source.add_aggro) return 1;
        try { const auto rc=s.enemy_source.add_aggro(s.enemy_source.context,state,owner_id,enemy_id,amount,delta);return rc||!s.live()?1:0; }
        catch (...) { return 1; }
    }

    struct HoldToken { std::shared_ptr<Impl> keep; };
    static std::int32_t retain_active(void* raw, character_enemy_spotted::State* state,
                                      const character_enemy_spotted::ActiveAIS* active_pair,
                                      void** hold) {
        auto& s = *static_cast<Impl*>(raw);
        if (!hold || !s.live() || state != s.b.enemy_state || !active_pair ||
            active_pair->identity != s.active || active_pair->callee != s.active_callee ||
            !s.active_script() || !s.active_script()->ready()) return 1;
        auto keep = s.self.lock();
        if (!keep) return 1;
        auto* token = new (std::nothrow) HoldToken{std::move(keep)};
        if (!token) return 1;
        ++s.active_holds;
        *hold = token;
        return 0;
    }
    static std::int32_t dispatch_active(void* raw, character_enemy_spotted::State* state,
        const character_enemy_spotted::ActiveAIS* active_pair, void* opaque,
        std::uintptr_t enemy_id) {
        auto& s = *static_cast<Impl*>(raw);
        auto* token = static_cast<HoldToken*>(opaque);
        if (!token || token->keep.get() != &s || !s.live() || state != s.b.enemy_state ||
            !active_pair || active_pair->identity != s.active ||
            active_pair->callee != s.active_callee || !enemy_id) return 1;
        if (s.current_result) ++s.current_result->script_dispatches;
        try {
            s.callback_error.clear();
            s.last_script_status = static_cast<std::int32_t>(s.active_script()->dispatch(
                monster_external_script::Event::enemy_spotted, enemy_id, s.callback_error));
            return s.last_script_status == static_cast<int>(monster_external_script::Status::complete) && s.live() ? 0 : 1;
        } catch (...) { return 1; }
    }
    static void release_active(void*, const character_enemy_spotted::ActiveAIS*, void* opaque) noexcept {
        auto* token = static_cast<HoldToken*>(opaque);
        if (!token) return;
        if (token->keep && token->keep->active_holds) --token->keep->active_holds;
        delete token;
    }

    static std::int32_t invoke_ai_event(void* raw, character::AIEventState64* state,
        const character::AIEventRequest40* request, std::uint32_t* value) {
        auto& s = *static_cast<Impl*>(raw);
        if (!state || state != s.b.event_state || !request || !value || !s.live()) return 1;
        if (request->service == character::ai_event_virtual && request->operation == 0x34) {
            if (request->event != 9 || request->subject != s.ai ||
                !state->ai_virtuals || request->callee != state->ai_virtuals[0x34 / 4] ||
                !request->payload) return 1;
            if (s.current_result) ++s.current_result->enemy_callbacks;
            try {
                s.scratch_enemy_result = {};
                s.last_enemy_gate_status = static_cast<std::int32_t>(character_enemy_spotted::on_enemy_spotted(
                    s.b.enemy_state, request->payload, &s.enemy_bound, &s.scratch_enemy_result));
                if (s.current_result) s.current_result->last_enemy_gate = s.scratch_enemy_result;
                *value = 0;
                return s.last_enemy_gate_status == static_cast<int>(character_enemy_spotted::Status::complete) && s.live() ? 0 : 1;
            } catch (...) { return 1; }
        }
        if (!s.event_source.invoke) return 1;
        try {
            const auto rc=s.event_source.invoke(s.event_source.context,state,request,value);
            return rc||!s.live()?1:0;
        } catch (...) { return 1; }
    }

    static std::int32_t classify_candidate(void* raw,
        character_aggro_candidate_events::State* state, RelationKind kind,
        std::uintptr_t owner_id, std::uintptr_t candidate, std::uint32_t* output) {
        auto& s = *static_cast<Impl*>(raw);
        if (!state || state != s.current_candidates || !output || !s.live() ||
            owner_id != s.owner || !candidate) return 1;
        RelationQuery query_kind;
        switch (kind) {
            case RelationKind::enemy: query_kind = RelationQuery::enemy; break;
            case RelationKind::friend_: query_kind = RelationQuery::friend_; break;
            case RelationKind::neutral: query_kind = RelationQuery::neutral; break;
            default: return 1;
        }
        if (!s.relation_source.resolve_object_handle || !s.relation_source.read_object_word_f4 ||
            !s.relation_source.get_faction_id || !s.relation_source.faction_count ||
            !s.relation_source.is_player || !s.relation_source.capture_faction_table ||
            !s.relation_source.is_interactive || !s.relation_source.interaction_type) return 1;
        character_ai_relations::Result result{};
        const auto status = character_ai_relations::query(query_kind, s.b.relation_state,
            candidate, &s.relation_bound, &result);
        if (status != character_ai_relations::Status::complete || !s.live()) return 1;
        *output = result.value;
        // The source empty/no-enemy convergence reloads CharAI+0x40 after the
        // last candidate relation callbacks, even if no event was raised.
        // Keep the adapter's cursor fresh when a borrowed relation provider
        // changes the target during an all-false classification pass.
        state->source_identity_40 = s.b.set_target_state->target;
        return 0;
    }
    static std::int32_t raise_candidate_event(void* raw,
        character_aggro_candidate_events::State* state, std::uintptr_t owner_id,
        std::uint32_t event_id, std::uintptr_t payload) {
        auto& s = *static_cast<Impl*>(raw);
        if (!state || state != s.current_candidates || !s.live() || owner_id != s.owner) return 1;
        character::AIEventPayload24 event_payload{payload, 0, 0, 0};
        character::AIEventResult16 event_result{};
        try {
            s.last_ai_event_status = dh2_character_ai_event(&event_result, s.b.event_state,
                event_id, &event_payload, &s.event_bound);
        } catch (...) { return 1; }
        if (s.last_ai_event_status != 0 || !s.live()) return 1;
        if (s.current_result) ++s.current_result->events_raised;
        // The original consumer loads CharAI+0x40 again after normal event
        // delivery. Keep the local cursor used by its final event-0x0c path
        // fresh after friend/neutral Character events.
        state->source_identity_40 = s.b.set_target_state->target;
        return 0;
    }

    static std::int32_t invoke_search(void* raw, const SearchRequest* request,
                                      SearchResponse* response) {
        auto& s = *static_cast<Impl*>(raw);
        if (!request || !response || !s.live() || !s.search_source.invoke) return 1;
        try {
            const auto rc = s.search_source.invoke(s.search_source.context, request, response);
            return rc || !s.live() ? 1 : 0;
        } catch (...) { return 1; }
    }
    static std::int32_t relation_resolve(void* raw, character_ai_relations::State* state,
        std::uintptr_t object, std::uintptr_t* result) {
        auto& s=*static_cast<Impl*>(raw); if(!s.live()||state!=s.b.relation_state||!s.relation_source.resolve_object_handle)return 1;
        try {const auto rc=s.relation_source.resolve_object_handle(s.relation_source.context,state,object,result);return rc||!s.live()?1:0;}catch(...){return 1;}
    }
    static std::int32_t relation_word_f4(void* raw, character_ai_relations::State* state,
        std::uintptr_t object, std::uint32_t* result) {
        auto& s=*static_cast<Impl*>(raw); if(!s.live()||state!=s.b.relation_state||!s.relation_source.read_object_word_f4)return 1;
        try {const auto rc=s.relation_source.read_object_word_f4(s.relation_source.context,state,object,result);return rc||!s.live()?1:0;}catch(...){return 1;}
    }
    static std::int32_t relation_faction(void* raw, character_ai_relations::State* state,
        std::uintptr_t character_id, std::int32_t* result) {
        auto& s=*static_cast<Impl*>(raw); if(!s.live()||state!=s.b.relation_state||!s.relation_source.get_faction_id)return 1;
        try {const auto rc=s.relation_source.get_faction_id(s.relation_source.context,state,character_id,result);return rc||!s.live()?1:0;}catch(...){return 1;}
    }
    static std::int32_t relation_faction_count(void* raw, character_ai_relations::State* state,
        std::int32_t* result) {
        auto& s=*static_cast<Impl*>(raw); if(!s.live()||state!=s.b.relation_state||!s.relation_source.faction_count)return 1;
        try {const auto rc=s.relation_source.faction_count(s.relation_source.context,state,result);return rc||!s.live()?1:0;}catch(...){return 1;}
    }
    static std::int32_t relation_player(void* raw, character_ai_relations::State* state,
        std::uintptr_t character_id, std::uint32_t* result) {
        auto& s=*static_cast<Impl*>(raw); if(!s.live()||state!=s.b.relation_state||!s.relation_source.is_player)return 1;
        try {const auto rc=s.relation_source.is_player(s.relation_source.context,state,character_id,result);return rc||!s.live()?1:0;}catch(...){return 1;}
    }
    static std::int32_t relation_table(void* raw, character_ai_relations::State* state,
        const character_ai_relations::FactionTable** result) {
        auto& s=*static_cast<Impl*>(raw); if(!s.live()||state!=s.b.relation_state||!s.relation_source.capture_faction_table)return 1;
        try {const auto rc=s.relation_source.capture_faction_table(s.relation_source.context,state,result);return rc||!s.live()?1:0;}catch(...){return 1;}
    }
    static std::int32_t relation_interactive(void* raw, character_ai_relations::State* state,
        std::uintptr_t object,std::uintptr_t owner_id,std::uint32_t* result) {
        auto& s=*static_cast<Impl*>(raw); if(!s.live()||state!=s.b.relation_state||!s.relation_source.is_interactive)return 1;
        try {const auto rc=s.relation_source.is_interactive(s.relation_source.context,state,object,owner_id,result);return rc||!s.live()?1:0;}catch(...){return 1;}
    }
    static std::int32_t relation_type(void* raw, character_ai_relations::State* state,
        std::uintptr_t object,std::uintptr_t owner_id,std::int32_t* result) {
        auto& s=*static_cast<Impl*>(raw); if(!s.live()||state!=s.b.relation_state||!s.relation_source.interaction_type)return 1;
        try {const auto rc=s.relation_source.interaction_type(s.relation_source.context,state,object,owner_id,result);return rc||!s.live()?1:0;}catch(...){return 1;}
    }

    static std::int32_t invoke_set_target(void* raw,
        const character::set_target::Request* request,
        character::set_target::Response* response) {
        auto& s=*static_cast<Impl*>(raw);
        if(!s.live()||!request||!response||!s.set_target_source.invoke)return 1;
        try {const auto rc=s.set_target_source.invoke(s.set_target_source.context,request,response);return rc||!s.live()?1:0;}catch(...){return 1;}
    }
    static std::int32_t invoke_find_path(void* raw,const character::PathToRequest32* request,
                                         std::uint32_t* result) {
        auto& s=*static_cast<Impl*>(raw);
        if(!s.live()||!request||!result||!s.path_source.find_path)return 1;
        try {const auto rc=s.path_source.find_path(s.path_source.context,request,result);return rc||!s.live()?1:0;}catch(...){return 1;}
    }
    static int invoke_control(void* raw,const character::CharacterControlRequest32* request,
                              character::CharacterControlResponse16* response) {
        auto& s=*static_cast<Impl*>(raw);
        if(!request||!response||!s.live())return 0;
        if(request->service == character::control_path_to) {
            if(request->subject!=s.owner||!s.b.path_state||s.b.path_state->owner!=s.owner)return 0;
            character::PathToResult16 path_result{};
            const int status=dh2_character_path_to(&path_result,s.b.path_state,request->position,&s.path_bound);
            if(status!=0||!s.live()) { if(s.current_result)s.current_result->last_path_status=status?status:1; return 0; }
            response->word=path_result.find_result;
            if(s.current_result) { ++s.current_result->path_requests; s.current_result->last_path_status=0; }
            return 1;
        }
        if(!s.control_source.invoke)return 0;
        try {const int rc=s.control_source.invoke(s.control_source.context,request,response);return rc==1&&s.live()?1:0;}catch(...){return 0;}
    }

    static std::int32_t script_struct(void* raw,const char* category,const char* member,
                                       std::int32_t* output) {
        auto& s=*static_cast<Impl*>(raw); if(!s.live()||!s.script_source.get_py_struct)return 1;
        try {const auto rc=s.script_source.get_py_struct(s.script_source.context,category,member,output);return rc||!s.live()?1:0;}catch(...){return 1;}
    }
    static std::int32_t script_prop(void* raw,std::uintptr_t owner_id,std::int32_t property,float* output) {
        auto& s=*static_cast<Impl*>(raw); if(!s.live()||owner_id!=s.owner||!s.script_source.get_prop)return 1;
        try {const auto rc=s.script_source.get_prop(s.script_source.context,owner_id,property,output);return rc||!s.live()?1:0;}catch(...){return 1;}
    }
    static std::int32_t script_constant(void* raw,const char* category,const char* member,
                                         std::int32_t* output) {
        auto& s=*static_cast<Impl*>(raw); if(!s.live()||!s.script_source.get_py_constant)return 1;
        try {const auto rc=s.script_source.get_py_constant(s.script_source.context,category,member,output);return rc||!s.live()?1:0;}catch(...){return 1;}
    }
    static std::int32_t script_has_target(void* raw,std::uintptr_t owner_id,std::uint32_t* output) {
        auto& s=*static_cast<Impl*>(raw); if(!s.live()||owner_id!=s.owner||!s.script_source.has_target)return 1;
        try {const auto rc=s.script_source.has_target(s.script_source.context,owner_id,output);return rc||!s.live()?1:0;}catch(...){return 1;}
    }
    static std::int32_t script_oid(void* raw,const char* category,const char* member,std::int32_t* output) {
        auto& s=*static_cast<Impl*>(raw); if(!s.live()||!s.script_source.get_py_oid)return 1;
        try {return s.script_source.get_py_oid(s.script_source.context,category,member,output)||!s.live()?1:0;}
        catch(...){return 1;}
    }
    static std::int32_t script_position(void* raw,std::uintptr_t owner_id,float output[3]) {
        auto& s=*static_cast<Impl*>(raw); if(!s.live()||owner_id!=s.owner||!s.script_source.get_position)return 1;
        try {return s.script_source.get_position(s.script_source.context,owner_id,output)||!s.live()?1:0;}
        catch(...){return 1;}
    }
    static std::int32_t script_host_level(void* raw,std::int32_t* output) {
        auto& s=*static_cast<Impl*>(raw); if(!s.live()||!s.script_source.get_host_player_level)return 1;
        try {return s.script_source.get_host_player_level(s.script_source.context,output)||!s.live()?1:0;}
        catch(...){return 1;}
    }
    static std::int32_t script_host_difficulty(void* raw,std::int32_t* output) {
        auto& s=*static_cast<Impl*>(raw); if(!s.live()||!s.script_source.get_host_player_difficulty)return 1;
        try {return s.script_source.get_host_player_difficulty(s.script_source.context,output)||!s.live()?1:0;}
        catch(...){return 1;}
    }
    static std::int32_t script_level_range(void* raw,const float* argument,std::int32_t output[2],std::uint32_t* count) {
        auto& s=*static_cast<Impl*>(raw); if(!s.live()||!s.script_source.get_current_level_range)return 1;
        try {return s.script_source.get_current_level_range(s.script_source.context,argument,output,count)||!s.live()?1:0;}
        catch(...){return 1;}
    }
    static std::int32_t script_level_set(void* raw,std::uintptr_t owner_id,float value) {
        auto& s=*static_cast<Impl*>(raw); if(!s.live()||owner_id!=s.owner||!s.script_source.set_level)return 1;
        try {return s.script_source.set_level(s.script_source.context,owner_id,value)||!s.live()?1:0;}
        catch(...){return 1;}
    }
    static std::int32_t script_stop(void* raw,std::uintptr_t owner_id) {
        auto& s=*static_cast<Impl*>(raw); if(!s.live()||owner_id!=s.owner||!s.script_source.stop)return 1;
        try {return s.script_source.stop(s.script_source.context,owner_id)||!s.live()?1:0;}
        catch(...){return 1;}
    }
    static std::int32_t script_attack(void* raw,std::uintptr_t owner_id,std::uintptr_t target) {
        auto& s=*static_cast<Impl*>(raw); if(!s.live()||owner_id!=s.owner||!s.script_source.attack)return 1;
        try {return s.script_source.attack(s.script_source.context,owner_id,target)||!s.live()?1:0;}
        catch(...){return 1;}
    }
    static std::int32_t script_get_target(void* raw,std::uintptr_t owner_id,std::uintptr_t* output) {
        auto& s=*static_cast<Impl*>(raw); if(!s.live()||owner_id!=s.owner||!s.script_source.get_target)return 1;
        try {const auto rc=s.script_source.get_target(s.script_source.context,owner_id,output);return rc||!s.live()?1:0;}catch(...){return 1;}
    }
    static std::int32_t script_get_state(void* raw,std::uintptr_t owner_id,std::int32_t* output) {
        auto& s=*static_cast<Impl*>(raw); if(!s.live()||owner_id!=s.owner||!s.script_source.get_state)return 1;
        try {const auto rc=s.script_source.get_state(s.script_source.context,owner_id,output);return rc||!s.live()?1:0;}catch(...){return 1;}
    }
    static std::int32_t script_has_path(void* raw,std::uintptr_t owner_id,std::uint32_t* output) {
        auto& s=*static_cast<Impl*>(raw); if(!s.live()||owner_id!=s.owner||!s.script_source.has_path)return 1;
        try {const auto rc=s.script_source.has_path(s.script_source.context,owner_id,output);return rc||!s.live()?1:0;}catch(...){return 1;}
    }
    static std::int32_t script_set_target(void* raw,std::uintptr_t owner_id,std::uintptr_t target) {
        auto& s=*static_cast<Impl*>(raw); if(!s.live()||owner_id!=s.owner)return 1;
        if(s.current_result) ++s.current_result->set_target_calls;
        return dh2_character_ai_set_target(s.b.set_target_state,target,0,&s.set_target_bound)==character::set_target::complete&&s.live()?0:1;
    }
    static std::int32_t script_head_to(void* raw,std::uintptr_t owner_id,std::uintptr_t target) {
        auto& s=*static_cast<Impl*>(raw); if(!s.live()||owner_id!=s.owner)return 1;
        if(s.current_result) ++s.current_result->head_to_calls;
        const auto status=dh2_character_controller_character(s.b.controller_state,
            character::controller_move_object,target,&s.control_bound);
        return status==1&&s.live()?0:1;
    }
    static std::int32_t script_move_to(void* raw,std::uintptr_t owner_id,std::uintptr_t target) {
        auto& s=*static_cast<Impl*>(raw); if(!s.live()||owner_id!=s.owner)return 1;
        if(s.current_result) ++s.current_result->move_to_calls;
        const auto status=dh2_character_controller_character(s.b.controller_state,
            character::controller_move_object,target,&s.control_bound);
        return status==1&&s.live()?0:1;
    }

    character_enemy_spotted::Result scratch_enemy_result{};
    std::int32_t last_script_status = static_cast<int>(monster_external_script::Status::not_ready);

    bool all_callbacks_present() const noexcept {
        return script_source.get_py_struct && script_source.get_prop &&
            script_source.get_py_constant && script_source.has_target && script_source.get_target &&
            script_source.get_state && script_source.has_path && search_source.invoke &&
            event_source.invoke && relation_source.resolve_object_handle &&
            relation_source.read_object_word_f4 && relation_source.get_faction_id &&
            relation_source.faction_count && relation_source.is_player &&
            relation_source.capture_faction_table && relation_source.is_interactive &&
            relation_source.interaction_type && set_target_source.invoke &&
            control_source.invoke && path_source.find_path;
    }
};

ActorSession::ActorSession() = default;
ActorSession::~ActorSession() = default;

Status ActorSession::bind(const Bindings& bindings,
    monster_external_script::Source commons, monster_external_script::Source monster,
    std::string& error) {
    if (impl_ && (impl_->busy || impl_->active_holds)) { error = "actor session busy"; return Status::busy; }
    if (!bindings.ai_identity || !bindings.owner_identity || !bindings.active_ais_identity ||
        !bindings.active_ais_callee || !bindings.enemy_state || !bindings.enemy_services ||
        !bindings.event_state || !bindings.event_services || !bindings.relation_state ||
        !bindings.relation_services || !bindings.set_target_state || !bindings.set_target_services ||
        !bindings.search_services || !bindings.controller_state || !bindings.control_services ||
        !bindings.path_state || !bindings.path_services ||
        !bindings.event_services->invoke || !bindings.control_services->invoke ||
        !bindings.path_services->find_path) {
        error = "actor session binding is incomplete"; return Status::invalid_argument;
    }
    std::shared_ptr<Impl> candidate;
    try { candidate.reset(new (std::nothrow) Impl(bindings)); }
    catch (...) { error = "actor session allocation failed"; return Status::allocation_failed; }
    if (!candidate) { error = "actor session allocation failed"; return Status::allocation_failed; }
    candidate->self = candidate;
    if (!candidate->live() || !candidate->all_callbacks_present()) {
        error = "actor session identity or service projection is invalid";
        return Status::invalid_argument;
    }
    const auto script_status = candidate->owned_script.initialize(commons, monster,
        candidate->script_bound, error);
    if (script_status != monster_external_script::Status::complete) return Status::script_failed;
    candidate->script_vm = &candidate->owned_script;
    if (!candidate->live()) { error = "actor owner changed while binding script"; return Status::stale_binding; }
    impl_ = std::move(candidate);
    error.clear();
    return Status::complete;
}

Status ActorSession::prepare_staged(const Bindings& bindings, std::string& error) {
    return prepare_callbacks(bindings, nullptr, error);
}

Status ActorSession::prepare_pending(const Bindings& bindings,
        const character::ScriptLifecycleState64* lifecycle, std::string& error) {
    if (impl_ && (impl_->busy || impl_->active_holds)) { error = "actor session busy"; return Status::busy; }
    if (!lifecycle || reinterpret_cast<std::uintptr_t>(lifecycle) % alignof(character::ScriptLifecycleState64)) {
        error = "pending AIS lifecycle projection is invalid";
        return Status::invalid_argument;
    }
    return prepare_callbacks(bindings, lifecycle, error);
}

Status ActorSession::prepare_callbacks(const Bindings& bindings,
        const character::ScriptLifecycleState64* lifecycle, std::string& error) {
    if (impl_ && (impl_->busy || impl_->active_holds)) { error = "actor session busy"; return Status::busy; }
    if (!bindings.ai_identity || !bindings.owner_identity || !bindings.active_ais_identity ||
        !bindings.active_ais_callee || !bindings.enemy_state || !bindings.enemy_services ||
        !bindings.event_state || !bindings.event_services || !bindings.relation_state ||
        !bindings.relation_services || !bindings.set_target_state || !bindings.set_target_services ||
        !bindings.search_services || !bindings.controller_state || !bindings.control_services ||
        !bindings.path_state || !bindings.path_services ||
        !bindings.event_services->invoke || !bindings.control_services->invoke ||
        !bindings.path_services->find_path) {
        error = "actor session binding is incomplete";
        return Status::invalid_argument;
    }
    std::shared_ptr<Impl> candidate;
    try { candidate.reset(new (std::nothrow) Impl(bindings)); }
    catch (...) { error = "actor session allocation failed"; return Status::allocation_failed; }
    if (!candidate) { error = "actor session allocation failed"; return Status::allocation_failed; }
    candidate->self = candidate;
    candidate->lifecycle = lifecycle;
    candidate->pending_phase = lifecycle != nullptr;
    if (!candidate->live() || !candidate->all_callbacks_present()) {
        error = "actor owner identity or callback projection is invalid";
        return Status::invalid_argument;
    }
    impl_ = std::move(candidate);
    error.clear();
    return Status::complete;
}

bool ActorSession::staged_services(monster_external_script::Services& output,
                                   std::shared_ptr<void>& lifetime) const noexcept {
    if (!impl_ || impl_->script_vm || impl_->busy || !impl_->live()) return false;
    output = impl_->script_bound;
    lifetime = impl_;
    return true;
}

Status ActorSession::adopt_staged(monster_external_script::Session& session, std::string& error) {
    if (!impl_) { error = "actor session has no staged callback context"; return Status::not_ready; }
    if (impl_->busy || impl_->active_holds) { error = "actor session busy"; return Status::busy; }
    if (impl_->script_vm) { error = "actor session already owns a script binding"; return Status::busy; }
    if (!impl_->live()) { error = "actor owner changed while preparing AIS VM"; return Status::stale_binding; }
    if (impl_->pending_phase && (impl_->lifecycle->active != impl_->active ||
        impl_->b.enemy_state->active.identity != impl_->active ||
        impl_->b.enemy_state->active.callee != impl_->active_callee)) {
        error = "source pending AIS has not been published to active";
        return Status::not_ready;
    }
    if (!session.ready() || !session.uses_services(impl_->script_bound)) {
        error = "pending AIS VM does not use the prepared actor callbacks";
        return Status::script_failed;
    }
    impl_->script_vm = &session;
    impl_->pending_phase = false;
    error.clear();
    return Status::complete;
}

Status ActorSession::reset(std::string& error) {
    if (!impl_) { error.clear(); return Status::complete; }
    if (impl_->busy || impl_->active_holds) { error = "actor session busy"; return Status::busy; }
    const auto status = impl_->script_vm == &impl_->owned_script ?
        impl_->owned_script.reset(error) : monster_external_script::Status::complete;
    impl_.reset();
    return status == monster_external_script::Status::complete ? Status::complete : Status::busy;
}

bool ActorSession::ready() const noexcept { return impl_ && impl_->active_script() &&
    impl_->active_script()->ready() && impl_->live(); }
monster_external_script::Statistics ActorSession::script_statistics() const noexcept {
    return impl_ && impl_->active_script() ? impl_->active_script()->statistics() : monster_external_script::Statistics{};
}

Status ActorSession::search_and_dispatch(character::aggro_search::TargetList* list,
    const character::aggro_search::RoomRegistry* rooms, float view_radius, float cone,
    ScanResult* result) {
    return search_and_dispatch_impl(list, rooms, nullptr, nullptr, view_radius, cone, result);
}

Status ActorSession::search_characters_and_dispatch(character::aggro_search::TargetList* list,
    character::aggro_character_list::CharacterList* characters, float view_radius,
    float cone, ScanResult* result) {
    return search_and_dispatch_impl(list, nullptr, characters, nullptr, view_radius, cone, result);
}

Status ActorSession::search_objects_and_dispatch(character::aggro_search::TargetList* list,
    const character::aggro_character_list::ObjectListMethods* objects, float view_radius,
    float cone, ScanResult* result) {
    return search_and_dispatch_impl(list, nullptr, nullptr, objects, view_radius, cone, result);
}

Status ActorSession::dispatch_combat_result(monster_external_script::Event event,
        std::uintptr_t attacker, std::uintptr_t defender, std::string& error) {
    auto current = impl_;
    if (!current || !current->active_script() || !current->active_script()->ready()) {
        error = "active monster AIS VM is not ready";
        return Status::not_ready;
    }
    if (current->busy || current->active_holds) {
        error = "actor session busy";
        return Status::busy;
    }
    if (!current->live()) {
        error = "monster AIS owner binding is stale";
        return Status::stale_binding;
    }
    if (!attacker || !defender ||
        (current->owner != attacker && current->owner != defender)) {
        error = "combat callback does not include this AIS Character";
        return Status::invalid_argument;
    }
    BusyScope busy(current->busy);
    const auto status = current->active_script()->dispatch_combat_result(
        event, attacker, defender, error);
    if (status == monster_external_script::Status::complete)
        return current->live() ? Status::complete : Status::stale_binding;
    if (status == monster_external_script::Status::busy) return Status::busy;
    if (status == monster_external_script::Status::not_ready) return Status::not_ready;
    if (status == monster_external_script::Status::invalid_argument) return Status::invalid_argument;
    return Status::script_failed;
}

Status ActorSession::search_and_dispatch_impl(character::aggro_search::TargetList* list,
    const character::aggro_search::RoomRegistry* rooms,
    character::aggro_character_list::CharacterList* characters,
    const character::aggro_character_list::ObjectListMethods* objects, float view_radius,
    float cone, ScanResult* result) {
    auto current = impl_;
    if (!current || !current->active_script() || !current->active_script()->ready()) return Status::not_ready;
    if (!list || !result || (int(rooms != nullptr) + int(characters != nullptr) +
                            int(objects != nullptr)) != 1)
        return Status::invalid_argument;
    if (current->busy) return Status::busy;
    if (!current->live()) return Status::stale_binding;
    if (!list->owner || list->owner->identity != current->owner || !list->owner->object)
        return Status::invalid_argument;

    // `result` is committed after source callbacks have run. Reject aliases of
    // every directly known borrowed projection first, so that this final write
    // cannot corrupt state that a callback/provider still owns. Dynamic list
    // nodes and backing service data have caller-owned disjointness contracts;
    // their complete extents are not represented by these bounded adapters.
    const auto aliases = [result](const void* input, std::size_t size) {
        return overlaps(result, sizeof(*result), input, size);
    };
    constexpr std::size_t vtable_bytes = 51U * sizeof(std::uintptr_t);
    if ((current->lifecycle && aliases(current->lifecycle, sizeof(*current->lifecycle))) ||
        aliases(current->b.enemy_state, sizeof(*current->b.enemy_state)) ||
        aliases(current->b.event_state, sizeof(*current->b.event_state)) ||
        aliases(current->b.event_state->owner, sizeof(*current->b.event_state->owner)) ||
        aliases(current->b.event_state->ai_virtuals, vtable_bytes) ||
        aliases(current->b.event_state->ais_virtuals, vtable_bytes) ||
        aliases(current->b.relation_state, sizeof(*current->b.relation_state)) ||
        aliases(current->b.set_target_state, sizeof(*current->b.set_target_state)) ||
        aliases(current->b.set_target_state->owner, sizeof(*current->b.set_target_state->owner)) ||
        aliases(current->b.controller_state, sizeof(*current->b.controller_state)) ||
        aliases(current->b.path_state, sizeof(*current->b.path_state)) ||
        aliases(list, sizeof(*list)) ||
        aliases(list->owner, sizeof(*list->owner)) ||
        aliases(list->owner->object, sizeof(*list->owner->object)) ||
        (rooms && aliases(rooms, sizeof(*rooms))) ||
        (characters && aliases(characters, sizeof(*characters))) ||
        (objects && aliases(objects, sizeof(*objects))) ||
        aliases(list->heap,
            static_cast<std::size_t>(list->capacity) * sizeof(character::aggro_search::TargetInfo)))
        return Status::invalid_argument;
    BusyScope busy(current->busy);
    ScanResult completed{};
    completed.search_status = -1;
    completed.candidate_status = -1;
    completed.last_ai_event_status = -1;
    completed.last_enemy_gate_status = -1;
    completed.last_script_status = static_cast<int>(monster_external_script::Status::not_ready);
    completed.last_path_status = -1;
    current->last_ai_event_status = -1;
    current->last_enemy_gate_status = -1;
    current->last_script_status = static_cast<int>(monster_external_script::Status::not_ready);
    current->current_result = &completed;
    character_aggro_candidate_events::State candidates{
        current->ai, current->owner, current->b.set_target_state->target};
    current->current_candidates = &candidates;
    const int search_status = objects ?
        character::aggro_character_list::dh2_aggro_target_search_object_list(
            list, objects, view_radius, cone, &current->search_bound) : characters ?
        character::aggro_character_list::dh2_aggro_target_search_character_list(
            list, characters, view_radius, cone, &current->search_bound) :
        character::aggro_search::dh2_aggro_target_search(list, rooms,
            view_radius, cone, &current->search_bound);
    completed.search_status = search_status;
    if (search_status != character::aggro_search::complete) {
        current->current_result = nullptr;
        current->current_candidates = nullptr;
        completed.last_ai_event_status = current->last_ai_event_status;
        completed.last_enemy_gate_status = current->last_enemy_gate_status;
        completed.last_script_status = current->last_script_status;
        *result = completed;
        return current->live() ? Status::source_failed : Status::stale_binding;
    }
    // _UpdateAggro reloads CharAI+0x40 only after the source search returns.
    // A borrowed search/provider may therefore replace it before the empty
    // list's final event 0x0c; the initial snapshot is not authoritative.
    candidates.source_identity_40 = current->b.set_target_state->target;
    completed.candidates_before_dispatch = list->count;
    const character_aggro_candidate_events::Services services{
        current.get(), Impl::classify_candidate, Impl::raise_candidate_event};
    const auto candidate_status = character_aggro_candidate_events::consume(
        &candidates, list, &services, &completed.candidate_events);
    completed.candidate_status = static_cast<std::int32_t>(candidate_status);
    completed.last_ai_event_status = current->last_ai_event_status;
    completed.last_enemy_gate_status = current->last_enemy_gate_status;
    completed.last_script_status = current->last_script_status;
    current->current_result = nullptr;
    current->current_candidates = nullptr;
    if (!current->live()) { *result = completed; return Status::stale_binding; }
    *result = completed;
    if (candidate_status != character_aggro_candidate_events::Status::complete)
        return Status::source_failed;
    return Status::complete;
}

}  // namespace dh2::ghost_ai_session
