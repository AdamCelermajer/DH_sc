#include "../ghost_ai_session.hpp"

#include <array>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>

using namespace dh2;
using namespace dh2::ghost_ai_session;

namespace {
void require(bool value, const char* message) {
    if (!value) throw std::runtime_error(message);
}
std::string read_file(const char* path) {
    std::ifstream stream(path, std::ios::binary);
    require(bool(stream), "original script fixture missing");
    return {std::istreambuf_iterator<char>(stream), {}};
}

struct Fixture {
    std::uintptr_t owner_id, ai_id, active_id, active_callee;
    std::uintptr_t owner_object_id, target_object_id, target_character_id;
    std::array<std::uintptr_t, 51> ai_virtuals{}, ais_virtuals{};
    character::AIEventOwner48 event_owner{};
    character::AIEventState64 event_state{};
    character_enemy_spotted::State enemy_state{};
    character_enemy_spotted::Services enemy_services{};
    character::AIEventServices24 event_services{};
    character_ai_relations::State relation_state{};
    character_ai_relations::Services relation_services{};
    character_ai_relations::FactionRow faction_rows[3]{};
    character_ai_relations::FactionTable faction_table{};
    data::AiFactionEntry enemy_entry[1]{{2, -1}};
    character::set_target::OwnerFacts owner_facts{};
    character::set_target::State target_state{};
    character::set_target::Services target_services{};
    character::aggro_search::Services search_services{};
    ScriptQueries script_queries{};
    character::ControllerCommandState32 controller_state{};
    character::CharacterControlServices16 control_services{};
    character::PathToState40 path_state{};
    character::PathToServices16 path_services{};
    character::aggro_search::GameObject owner_object{}, target_object{};
    character::aggro_search::Character owner_character{}, target_character{};
    character::aggro_search::ObjectEntry room_entry_sentinel{}, room_entry{};
    character::aggro_search::Room room_sentinel{}, room{};
    character::aggro_search::RoomRegistry rooms{};
    character::aggro_search::TargetInfo heap[4]{};
    character::aggro_search::TargetList list{};
    ActorSession session;
    Bindings bindings{};
    std::vector<std::string> trace;
    int event_machine_calls = 0, path_calls = 0, search_calls = 0;
    bool fail_search = false, fail_path = false, rebind_in_search = false;
    bool empty_search = false, mutate_target_during_search = false;
    bool mutate_relation_after_first_query = false, relation_target_changed = false;
    unsigned relation_resolve_calls = 0;
    std::uintptr_t search_replacement_target = 0, expected_event12_payload = 0;
    int event12_calls = 0;
    Status nested_bind_status = Status::complete;
    float requested_path[3]{};

    explicit Fixture(std::uintptr_t base)
        : owner_id(base + 0x10), ai_id(base + 0x20), active_id(base + 0x30),
          active_callee(base + 0x40), owner_object_id(base + 0x50),
          target_object_id(base + 0x60), target_character_id(base + 0x70) {
        ai_virtuals[0x34 / 4] = base + 0x80;
        ai_virtuals[0x48 / 4] = base + 0x88;
        ais_virtuals[0x34 / 4] = active_callee;
        event_owner = {owner_id, base + 0x90, base + 0xa0, base + 0xb0, 0, 0, 0, 0};
        event_state = {ai_id, &event_owner, ai_virtuals.data(), active_id,
                       ais_virtuals.data(), 0, 0, 0, 0, 0, 0};
        enemy_state = {ai_id, owner_id, 0, {active_id, active_callee}, 0};
        enemy_services = {this, nullptr, nullptr, awaiting, limbus, in_combat,
                          is_player, get_aggro, initial_aggro, add_aggro,
                          nullptr, nullptr, nullptr};
        event_services = {this, event_invoke,
            (1u << character::ai_event_state_event) |
            (1u << character::ai_event_virtual), 0};
        relation_state = {ai_id, owner_id, 0};
        faction_rows[1] = {enemy_entry, 1, 1};
        faction_table = {faction_rows, 3};
        relation_services = {this, resolve_handle, object_word_f4, faction_id,
            faction_count, relation_player, faction_table_capture, is_interactive,
            interaction_type};
        owner_facts = {owner_id, 68, 0, 0};
        target_state = {ai_id, &owner_facts, 0, 0, 0, 0, 0, 0, 0};
        target_services = {this, 100, set_target_invoke};
        search_services = {this, search_invoke};
        script_queries = {this, get_py_struct, get_prop, get_py_constant,
            has_target, get_target, get_state, has_path};
        controller_state = {base + 0x90, owner_id, 0, 0, 0, 0};
        control_services = {this, control_invoke};
        path_state = {owner_id, 0, 0, 0, 0, {0, 0, 0}, 0};
        path_services = {this, find_path};

        owner_object = {owner_object_id, {0, 0, 0}, {0, 0, 0}, {1, 0, 0},
                        1, 0, 0, 0};
        target_object = {target_object_id, {100, 0, 0}, {100, 0, 0}, {1, 0, 0},
                         1, 0, 0, 0};
        owner_character = {owner_id, &owner_object, 0, 1};
        target_character = {target_character_id, &target_object, 0, 0};
        room_entry_sentinel.next = &room_entry;
        room_entry = {&room_entry_sentinel, &target_object};
        room_sentinel.next = &room;
        room.objects = &room_entry_sentinel;
        room.next = &room_sentinel;
        rooms = {&room_sentinel};
        require(character::aggro_search::dh2_aggro_target_list_init(
            &list, heap, 4, &owner_character) == character::aggro_search::complete,
            "source target list initialization failed");
        bindings = make_bindings();
    }

    Bindings make_bindings() {
        return {ai_id, owner_id, active_id, active_callee,
            &enemy_state, &enemy_services, &event_state, &event_services,
            &relation_state, &relation_services, &target_state, &target_services,
            &search_services, script_queries, &controller_state, &control_services,
            &path_state, &path_services};
    }

    static Fixture& from(void* raw) { return *static_cast<Fixture*>(raw); }
    static std::int32_t awaiting(void* raw, character_enemy_spotted::State*, std::uintptr_t,
                                 std::uint32_t* out) { from(raw).trace.emplace_back("Awaiting"); *out = 0; return 0; }
    static std::int32_t limbus(void* raw, character_enemy_spotted::State*, std::uintptr_t,
                               std::uint32_t* out) { from(raw).trace.emplace_back("Limbus"); *out = 0; return 0; }
    static std::int32_t in_combat(void* raw, character_enemy_spotted::State*, std::uintptr_t,
                                  std::uint32_t* out) { from(raw).trace.emplace_back("Combat"); *out = 1; return 0; }
    static std::int32_t is_player(void* raw, character_enemy_spotted::State*, std::uintptr_t,
                                  std::uint32_t* out) { from(raw).trace.emplace_back("IsPlayer"); *out = 0; return 0; }
    static std::int32_t get_aggro(void*, character_enemy_spotted::State*, std::uintptr_t,
        std::uintptr_t, std::uint32_t* out) { *out = 0x3f800000; return 0; }
    static std::int32_t initial_aggro(void*, character_enemy_spotted::State*,
                                      std::uint32_t* out) { *out = 0; return 0; }
    static std::int32_t add_aggro(void*, character_enemy_spotted::State*, std::uintptr_t,
        std::uintptr_t, std::uint32_t, std::uint32_t* out) { *out = 0; return 0; }

    static int event_invoke(void* raw, character::AIEventState64*,
        const character::AIEventRequest40* request, std::uint32_t* out) {
        auto& f = from(raw);
        if (request->event == 9 && request->service == character::ai_event_state_event &&
            request->subject == f.event_owner.state_machine &&
            request->payload == f.target_object_id) {
            ++f.event_machine_calls; *out = 0; f.trace.emplace_back("FSM:9"); return 0;
        }
        if (request->event == 0x0c && request->service == character::ai_event_virtual &&
            request->operation == 0x48 && request->subject == f.ai_id &&
            request->callee == f.ai_virtuals[0x48 / 4] &&
            request->payload == 0 && f.target_state.target == f.expected_event12_payload) {
            ++f.event12_calls; *out = 0; f.trace.emplace_back("FSM:12"); return 0;
        }
        if (request->event == 0x0c) std::fprintf(stderr,"event12 req service=%u op=%x subject=%llx callee=%llx payload=%llx current_target=%llx expected_target=%llx\n",
            request->service,request->operation,static_cast<unsigned long long>(request->subject),
            static_cast<unsigned long long>(request->callee),static_cast<unsigned long long>(request->payload),
            static_cast<unsigned long long>(f.target_state.target),
            static_cast<unsigned long long>(f.expected_event12_payload));
        return 1;
    }
    static std::int32_t resolve_handle(void* raw, character_ai_relations::State*,
        std::uintptr_t object, std::uintptr_t* out) {
        auto& f = from(raw); f.trace.emplace_back("resolve");
        if (object != f.target_object_id) return 1;
        if (f.mutate_relation_after_first_query && ++f.relation_resolve_calls == 2) {
            // Enemy was evaluated against a neutral row. The source performs
            // a fresh table/property traversal for Friend and Neutral, so an
            // intervening provider mutation can make both evaluate false.
            f.enemy_entry[0].value=-1;
            f.target_state.target = f.search_replacement_target;
            f.relation_target_changed = true;
        }
        *out = f.target_character_id; return 0;
    }
    static std::int32_t object_word_f4(void*, character_ai_relations::State*,
        std::uintptr_t, std::uint32_t* out) { *out = 0; return 0; }
    static std::int32_t faction_id(void* raw, character_ai_relations::State*,
        std::uintptr_t character_id, std::int32_t* out) {
        auto& f = from(raw);
        if (character_id == f.owner_id) *out = 1;
        else if (character_id == f.target_character_id) *out = 2;
        else return 1;
        return 0;
    }
    static std::int32_t faction_count(void*, character_ai_relations::State*, std::int32_t* out) {
        *out = 3; return 0;
    }
    static std::int32_t relation_player(void*, character_ai_relations::State*,
        std::uintptr_t, std::uint32_t* out) { *out = 0; return 0; }
    static std::int32_t faction_table_capture(void* raw, character_ai_relations::State*,
        const character_ai_relations::FactionTable** out) {
        *out = &from(raw).faction_table; return 0;
    }
    static std::int32_t is_interactive(void*, character_ai_relations::State*,
        std::uintptr_t, std::uintptr_t, std::uint32_t* out) { *out = 1; return 0; }
    static std::int32_t interaction_type(void*, character_ai_relations::State*,
        std::uintptr_t, std::uintptr_t, std::int32_t* out) { *out = 8; return 0; }

    static std::int32_t search_invoke(void* raw,
        const character::aggro_search::Request* request,
        character::aggro_search::Response* out) {
        auto& f = from(raw); ++f.search_calls;
        if (f.rebind_in_search && f.search_calls == 1) {
            std::string error;
            f.nested_bind_status = f.session.bind(f.bindings,
                {nullptr, 0}, {nullptr, 0}, error);
        }
        if (f.fail_search) return 1;
        using namespace character::aggro_search;
        switch (static_cast<Operation>(request->operation)) {
            case ai_melee_radius:
                if (f.mutate_target_during_search)
                    f.target_state.target = f.search_replacement_target;
                out->number = 120.f; return 0;
            case resolve_character:
                if (f.empty_search) { out->word = 0; return 0; }
                if (request->subject != f.target_object_id) return 1;
                out->word = reinterpret_cast<std::uintptr_t>(&f.target_character); return 0;
            case is_zonable: out->word = 0; return 0;
            case character::aggro_search::is_interactive: out->word = 1; return 0;
            case character::aggro_search::interaction_radius: out->number = 0.f; return 0;
            default: return 1;
        }
    }

    static std::int32_t get_py_struct(void* raw, const char* category,
        const char* member, std::int32_t* out) {
        auto& f = from(raw);
        require(std::string(category) == "CharacterProperties" &&
                std::string(member) == "SkillTree", "source property lookup changed");
        f.trace.emplace_back("GetPyStruct"); *out = 28; return 0;
    }
    static std::int32_t get_prop(void* raw, std::uintptr_t owner, std::int32_t property,
                                 float* out) {
        auto& f = from(raw);
        if (owner != f.owner_id || property != 28) return 1;
        f.trace.emplace_back("GetProp:raw-fixed"); *out = -1.f; return 0;
    }
    static std::int32_t get_py_constant(void* raw, const char* category,
        const char* member, std::int32_t* out) {
        auto& f = from(raw);
        if (std::string(category) != "AIStates" || std::string(member) != "Idle") return 1;
        f.trace.emplace_back("Idle"); *out = 3; return 0;
    }
    static std::int32_t has_target(void* raw, std::uintptr_t owner,
                                   std::uint32_t* out) {
        auto& f = from(raw); if (owner != f.owner_id) return 1;
        f.trace.emplace_back("HasTarget"); *out = f.target_state.target != 0; return 0;
    }
    static std::int32_t get_target(void* raw, std::uintptr_t owner, std::uintptr_t* out) {
        auto& f = from(raw); if (owner != f.owner_id) return 1;
        *out = f.target_state.target; return 0;
    }
    static std::int32_t get_state(void* raw, std::uintptr_t owner, std::int32_t* out) {
        auto& f = from(raw); if (owner != f.owner_id) return 1;
        *out = 3; return 0;
    }
    static std::int32_t has_path(void* raw, std::uintptr_t owner, std::uint32_t* out) {
        auto& f = from(raw); if (owner != f.owner_id) return 1;
        *out = f.path_state.path_nonempty; return 0;
    }

    static std::int32_t set_target_invoke(void* raw, const character::set_target::Request* req,
                                          character::set_target::Response* out) {
        auto& f = from(raw); f.trace.emplace_back("AI_SetTarget-service");
        using namespace character::set_target;
        switch (static_cast<Operation>(req->operation)) {
            case debug_switches_load: return 0;
            case debug_switch_lookup: out->word = 0; return 0;
            case target_is_dead: out->word = 0; return 0;
            case ai_is_in_sight: out->word = 1; return 0;
            default: return 1;
        }
    }
    static int control_invoke(void* raw, const character::CharacterControlRequest32* req,
                              character::CharacterControlResponse16* out) {
        auto& f=from(raw);
        if(req->service==character::control_is_remotely_updated) { out->word=0; f.trace.emplace_back("remote?"); return 1; }
        if(req->service==character::control_target_position && req->subject==f.target_object_id) {
            out->position[0]=f.target_object.position[0];out->position[1]=f.target_object.position[1];out->position[2]=f.target_object.position[2];
            f.trace.emplace_back("target-position");return 1;
        }
        return 0;
    }
    static int find_path(void* raw,const character::PathToRequest32* req,std::uint32_t* out) {
        auto& f=from(raw);++f.path_calls;f.trace.emplace_back("FindPath");
        if(f.fail_path)return 1;
        for(unsigned i=0;i<3;++i){f.requested_path[i]=req->target[i];f.path_state.path_target[i]=req->target[i];}
        f.path_state.path_nonempty=1;*out=1;return 0;
    }
};

void initialize(Fixture& fixture, const std::string& common, const std::string& monster) {
    std::string error;
    const auto status=fixture.session.bind(fixture.bindings,{common.data(),common.size()},
        {monster.data(),monster.size()},error);
    require(status==Status::complete&&error.empty(),"actor-owned monster session bind failed");
}

void run_one(ActorSession& session, Fixture& fixture) {
    ScanResult result{};
    const auto status=session.search_and_dispatch(&fixture.list,&fixture.rooms,
        1500.f,6.2831855f,&result);
    if (status != Status::complete) std::fprintf(stderr,
        "scan status=%d search=%d candidates=%d events=%u enemy=%u script=%u path=%u target=%llx count=%u ai=%d gate=%d scriptstatus=%d gatequeries=%u/%u/%u/%u/%u aggro=%u\n",
        static_cast<int>(status),result.search_status,result.candidate_status,
        result.candidate_events.candidates_consumed,result.enemy_callbacks,
        result.script_dispatches,result.path_requests,
        static_cast<unsigned long long>(fixture.target_state.target),fixture.list.count,
        result.last_ai_event_status,result.last_enemy_gate_status,result.last_script_status,
        result.last_enemy_gate.awaiting_spawn_queries,result.last_enemy_gate.limbus_queries,
        result.last_enemy_gate.combat_queries,result.last_enemy_gate.player_queries,
        result.last_enemy_gate.aggro_queries,result.last_enemy_gate.active_retains);
    if (status != Status::complete) for(const auto& item:fixture.trace) std::fprintf(stderr,"trace:%s\n",item.c_str());
    require(status==Status::complete,"source search/event/script/controller composition failed");
    require(result.search_status==0&&result.candidate_status==0,"source subkernel failed");
    require(result.candidates_before_dispatch==1&&result.candidate_events.candidates_consumed==1,
            "source candidate wasn't consumed after event");
    require(result.candidate_events.enemy_events==1&&result.enemy_callbacks==1,
            "source enemy event did not reach OnEnemySpotted");
    require(result.script_dispatches==1&&result.last_script_status==0,
            "original Lua OnEnemySpotted didn't dispatch");
    require(fixture.target_state.target==fixture.target_object_id,
            "Lua SetTarget did not mutate the source AI target projection");
    require(fixture.path_calls==1&&fixture.path_state.path_nonempty==1&&
            fixture.requested_path[0]==100.f,"HeadTo did not reach source PathTo/FindPath");
    require(fixture.event_machine_calls==1,"RaiseAIEvent state-machine convergence missing");
    require(session.script_statistics().completed_callbacks==1,
            "per-actor Lua callback accounting missing");
}
void run_one(Fixture& fixture) { run_one(fixture.session, fixture); }

void run_flat_character_routes(const std::string& common, const std::string& monster) {
    using character::aggro_character_list::Entry;
    using character::aggro_character_list::CharacterList;
    using character::aggro_character_list::dh2_aggro_character_list_init;
    const auto scan = [](Fixture& f, CharacterList& chars, ScanResult& result) {
        return f.session.search_characters_and_dispatch(&f.list,&chars,1500.f,6.2831855f,&result);
    };
    { Fixture f(0xf10000000ull); initialize(f,common,monster);
      Entry end{}, own{}, enemy{}; end.next=&own; own={&enemy,&f.owner_character}; enemy={&end,&f.target_character};
      CharacterList chars{}; require(dh2_aggro_character_list_init(&chars,&end)==0,"flat list init failed");
      f.room_sentinel.next=&f.room_sentinel; // The independent historical room route is empty.
      ScanResult result{};
      require(scan(f,chars,result)==Status::complete && result.candidates_before_dispatch==1 &&
        result.script_dispatches==1 && result.path_requests==1 && f.target_state.target==f.target_object_id &&
        f.path_calls==1 && f.requested_path[0]==100.f && f.session.script_statistics().completed_callbacks==1,
        "flat ObjectManager Character route did not reach unchanged Lua/SetTarget/PathTo"); }
    { Fixture f(0xf20000000ull); initialize(f,common,monster);
      f.mutate_target_during_search=true;f.search_replacement_target=f.target_object_id;f.expected_event12_payload=f.target_object_id;
      Entry end{nullptr,nullptr};end.next=&end;CharacterList chars{&end,&end,&end};ScanResult result{};
      require(scan(f,chars,result)==Status::complete && !result.candidates_before_dispatch &&
        !result.script_dispatches && f.event12_calls==1 && !f.path_calls,
        "empty flat Character query invented an enemy or omitted source event12"); }
    { Fixture f(0xf30000000ull); initialize(f,common,monster);f.fail_search=true;
      Entry end{}, enemy{&end,&f.target_character};end.next=&enemy;
      CharacterList chars{&end,&enemy,&end};ScanResult result{};
      require(scan(f,chars,result)==Status::source_failed && result.search_status!=0 &&
        !result.script_dispatches && !f.path_calls,"flat query provider failure reached a Lua callback"); }
    { Fixture f(0xf40000000ull); initialize(f,common,monster);f.enemy_state.owner_identity+=1;
      Entry end{};end.next=&end;CharacterList chars{&end,&end,&end};ScanResult result{};result.search_status=1234;
      require(scan(f,chars,result)==Status::stale_binding && !f.search_calls && result.search_status==1234,
        "flat query used stale owner or wrote the output before validation"); }
    { Fixture f(0xf50000000ull); initialize(f,common,monster);
      Entry end{};end.next=&end;CharacterList chars{&end,&end,&end};const auto before=chars;
      require(f.session.search_characters_and_dispatch(&f.list,&chars,1500.f,6.2831855f,
        reinterpret_cast<ScanResult*>(&chars))==Status::invalid_argument && !f.search_calls &&
        std::memcmp(&before,&chars,sizeof(chars))==0,"flat query output corrupted borrowed iterator storage"); }
    { Fixture f(0xf60000000ull); initialize(f,common,monster);f.rebind_in_search=true;
      Entry end{}, enemy{&end,&f.target_character};end.next=&enemy;
      CharacterList chars{&end,&enemy,&end};ScanResult result{};
      require(scan(f,chars,result)==Status::complete && f.nested_bind_status==Status::busy &&
        result.script_dispatches==1 && result.path_requests==1,
        "flat query reentry replaced the retained VM or lost the outer callback"); }
}

void unexpected_publication_service(void*, character::ScriptLifecycleState64*,
    const character::ScriptLifecycleRequest32*, character::ScriptLifecycleResponse16*) {
    throw std::runtime_error("source stage 6 unexpectedly dispatched a provider");
}

void prepare_pending(Fixture& fixture, character::ScriptLifecycleState64& lifecycle,
                     monster_external_script::Session& vm,
                     const std::string& common, const std::string& monster,
                     monster_external_script::Services& services) {
    lifecycle = {fixture.owner_id, 0, fixture.active_id, 1, 6, -1, -1, 0, 1, 0, 0, 0};
    fixture.enemy_state.active = {0, 0};
    fixture.event_state.active = 0;
    std::string error;
    require(fixture.session.prepare_pending(fixture.bindings, &lifecycle, error)==Status::complete,
            "pending actor callback preparation fabricated an active AIS");
    std::shared_ptr<void> lifetime;
    require(fixture.session.staged_services(services, lifetime) && lifetime,
            "pending source callbacks were unavailable before publication");
    require(vm.create(services, error, 2*1024*1024, lifetime)==monster_external_script::Status::complete &&
            vm.bind_functions(error)==monster_external_script::Status::complete &&
            vm.load_common({common.data(),common.size()},error)==monster_external_script::Status::complete &&
            vm.load_external({monster.data(),monster.size()},error)==monster_external_script::Status::complete,
            "source pending VM could not load unchanged scripts");
}

void publish_pending(Fixture& fixture, character::ScriptLifecycleState64& lifecycle) {
    const character::ScriptLifecycleServices16 services{nullptr, unexpected_publication_service};
    require(dh2_character_script_lifecycle(&lifecycle, character::script_load_process, 0, &services)==1 &&
            lifecycle.active==fixture.active_id && lifecycle.pending==fixture.active_id && lifecycle.load_step==13,
            "actual source lifecycle stage 6 did not publish its pending AIS");
    // Non-delayed LoadScriptProcess performs seven iterations even at stage 6;
    // later stages have only excluded diagnostic effects. Refresh the consumer
    // projection only after the source producer wrote active.
    fixture.enemy_state.active={lifecycle.active,fixture.active_callee};
    fixture.event_state.active=lifecycle.active;
}
}  // namespace

int main(int argc,char** argv) {
 try {
    require(argc==3,"expected original _commons and monster script paths");
    const auto common=read_file(argv[1]), monster=read_file(argv[2]);
    Fixture first(0x100000000ull), second(0x200000000ull);
    initialize(first,common,monster); initialize(second,common,monster);
    run_one(first);
    require(first.session.script_statistics().completed_callbacks==1&&
            second.session.script_statistics().completed_callbacks==0,
            "actor Lua state/accounting leaked across Ghosts");
    run_one(second);
    require(second.target_state.target==second.target_object_id&&
            first.target_state.target==first.target_object_id,
            "two actor targets crossed identities");
    require(first.target_character_id!=first.target_object_id&&
            first.target_state.target==first.target_object_id,
            "candidate Character metadata replaced source TargetInfo+0 object identity");

    // Stale borrowed owner rejects before search, then a full actor rebind gets
    // new service contexts and a new script VM without disturbing the old one.
    const auto calls_before=first.search_calls;
    first.enemy_state.owner_identity+=1;
    ScanResult untouched{}; untouched.search_status=0x1234;
    require(first.session.search_and_dispatch(&first.list,&first.rooms,1500.f,
        6.2831855f,&untouched)==Status::stale_binding,
        "stale owner wasn't rejected");
    require(first.search_calls==calls_before&&untouched.search_status==0x1234,
            "stale owner rejection had effects");
    Fixture rebound(0x300000000ull);
    std::string error;
    require(first.session.bind(rebound.bindings,{common.data(),common.size()},
        {monster.data(),monster.size()},error)==Status::complete,
        "new owner rebinding failed");
    run_one(first.session,rebound);

    // Rebind attempted synchronously inside a borrowed search callback is
    // rejected while the actor/session is in use.
    Fixture busy(0x400000000ull); initialize(busy,common,monster);
    busy.rebind_in_search=true;
    run_one(busy);
    require(busy.nested_bind_status==Status::busy,"reentrant actor rebind wasn't rejected");

    // Path service failure occurs after source AI_SetTarget. The earlier source
    // target mutation remains, while the event candidate is left unpopped.
    Fixture failed(0x500000000ull); initialize(failed,common,monster);
    failed.fail_path=true;
    ScanResult failure{};
    require(failed.session.search_and_dispatch(&failed.list,&failed.rooms,1500.f,
        6.2831855f,&failure)==Status::source_failed,
        "PathTo failure wasn't propagated through source dispatch");
    require(failed.target_state.target==failed.target_object_id&&failed.path_calls==1&&
            failed.list.count==1&&failure.script_dispatches==1,
            "source effects/queue cursor changed on later controller failure");
    require(!failed.session.ready(),"faulted Lua callback didn't mark the session unusable");

    // Search-provider failure stops before Character/RaiseAIEvent and script.
    Fixture search_failure(0x600000000ull); initialize(search_failure,common,monster);
    search_failure.fail_search=true;
    ScanResult search_result{};
    require(search_failure.session.search_and_dispatch(&search_failure.list,
        &search_failure.rooms,1500.f,6.2831855f,&search_result)==Status::source_failed&&
        search_result.search_status==character::aggro_search::source_service_failed&&
        search_result.script_dispatches==0&&search_failure.target_state.target==0,
        "search failure leaked into actor event/script effects");

    // The normal _UpdateAggro search completes before its final +0x40 reload.
    // An empty search refreshes +0x40 after its borrowed search provider. Event
    // 0x0c then dispatches the source no-argument OnTargetOutOfSight vfunc at
    // +0x48, so the vfunc's borrowed actor state sees the fresh current target
    // while the dispatcher correctly supplies a null payload.
    Fixture empty(0x700000000ull); initialize(empty,common,monster);
    empty.empty_search=true;
    empty.mutate_target_during_search=true;
    empty.search_replacement_target=empty.target_object_id+0x100;
    empty.expected_event12_payload=empty.search_replacement_target;
    empty.room_entry_sentinel.next=&empty.room_entry_sentinel;
    empty.room.objects=&empty.room_entry_sentinel;
    ScanResult empty_result{};
    const auto empty_status=empty.session.search_and_dispatch(&empty.list,&empty.rooms,
        1500.f,6.2831855f,&empty_result);
    require(empty_status==Status::complete&&empty.event12_calls==1&&
        empty_result.candidate_events.source_event_12==1,
        "empty source search did not emit event 0x0c");

    // A candidate can classify false as Enemy, Friend, and Neutral without
    // raising any event. If a relation service mutates the live target during
    // that sequence, the final no-argument out-of-sight callback must still
    // observe the source's post-classification +0x40 value.
    Fixture all_false(0x900000000ull); initialize(all_false,common,monster);
    all_false.enemy_entry[0].value=0;
    all_false.mutate_relation_after_first_query=true;
    all_false.search_replacement_target=all_false.target_object_id+0x200;
    all_false.expected_event12_payload=all_false.search_replacement_target;
    ScanResult all_false_result{};
    const auto all_false_status=all_false.session.search_and_dispatch(&all_false.list,
        &all_false.rooms,1500.f,6.2831855f,&all_false_result);
    require(all_false_status==Status::complete&&all_false.relation_target_changed&&
        all_false_result.candidate_events.enemy_events==0&&
        all_false_result.candidate_events.friend_events==0&&
        all_false_result.candidate_events.neutral_events==0&&
        all_false_result.candidate_events.source_event_12==1&&all_false.event12_calls==1,
        "all-false relation pass lost the fresh post-provider target");

    // The output object cannot overwrite controller or other borrowed state at
    // the final commit point. Rejection occurs before the search provider runs.
    Fixture alias(0x800000000ull); initialize(alias,common,monster);
    std::array<unsigned char,sizeof(alias.controller_state)> controller_before{};
    std::memcpy(controller_before.data(),&alias.controller_state,sizeof(alias.controller_state));
    auto* alias_result=reinterpret_cast<ScanResult*>(&alias.controller_state);
    require(alias.session.search_and_dispatch(&alias.list,&alias.rooms,1500.f,
        6.2831855f,alias_result)==Status::invalid_argument&&alias.search_calls==0&&
        std::memcmp(controller_before.data(),&alias.controller_state,sizeof(alias.controller_state))==0,
        "overlapping output changed borrowed controller state");

    // AISExternal owns the staged VM. ActorSession prepares its stable service
    // callbacks, the pending VM copies those callbacks and retains the context,
    // then the wrapper adopts that exact VM instead of creating a second one.
    {
        monster_external_script::Session pending_vm;
        Fixture staged(0xa00000000ull);
        std::string staged_error;
        require(staged.session.prepare_staged(staged.bindings,staged_error)==Status::complete,
                "actor callback context preparation failed");
        monster_external_script::Services staged_services{};
        std::shared_ptr<void> callback_lifetime;
        require(staged.session.staged_services(staged_services,callback_lifetime) && callback_lifetime,
                "staged actor callbacks did not retain their implementation");
        require(pending_vm.create(staged_services,staged_error,2*1024*1024,callback_lifetime)==
                    monster_external_script::Status::complete &&
                pending_vm.bind_functions(staged_error)==monster_external_script::Status::complete &&
                pending_vm.load_common({common.data(),common.size()},staged_error)==
                    monster_external_script::Status::complete &&
                pending_vm.load_external({monster.data(),monster.size()},staged_error)==
                    monster_external_script::Status::complete,
                "pending AIS VM source lifecycle failed");
        require(staged.session.adopt_staged(pending_vm,staged_error)==Status::complete &&
                    staged.session.ready() && pending_vm.uses_services(staged_services),
                "actor wrapper failed to adopt the exact staged VM");
        run_one(staged.session,staged);
        require(staged.session.script_statistics().completed_callbacks==1 &&
                    pending_vm.statistics().completed_callbacks==1,
                "Owner callback VM was duplicated instead of shared");
        require(staged.session.reset(staged_error)==Status::complete && pending_vm.ready() &&
                    pending_vm.dispatch(monster_external_script::Event::enemy_spotted,
                        staged.target_object_id,staged_error)==monster_external_script::Status::complete,
                "borrowed VM lost its callback context when the wrapper detached");
        require(pending_vm.reset(staged_error)==monster_external_script::Status::complete,
                "staged source VM teardown failed");
    }

    // Initialize callbacks against the source pending field, then execute the
    // actual publication stage. The actor cannot acquire or adopt prematurely.
    {
        monster_external_script::Session pending_vm;
        Fixture pending(0xb00000000ull);
        character::ScriptLifecycleState64 lifecycle{};
        monster_external_script::Services services{};
        prepare_pending(pending,lifecycle,pending_vm,common,monster,services);
        float property=123.f;
        require(services.get_prop(services.context,pending.owner_id,28,&property)==0 && property==-1.f &&
                    lifecycle.active==0 && pending.enemy_state.active.identity==0,
                "pending script getter depended on fabricated active ownership");
        ScanResult result{}; result.search_status=0x1234;
        require(!pending.session.ready() && pending.session.search_and_dispatch(&pending.list,&pending.rooms,
                    1500.f,6.2831855f,&result)==Status::not_ready && pending.search_calls==0 &&
                    result.search_status==0x1234,
                "pending VM entered acquisition before source publication");
        std::string error;
        require(pending.session.adopt_staged(pending_vm,error)==Status::not_ready,
                "ready pending VM was adopted before source publication");
        publish_pending(pending,lifecycle);
        pending.enemy_state.active.callee+=1;
        require(pending.session.adopt_staged(pending_vm,error)==Status::not_ready,
                "wrong published AIS callee was accepted");
        pending.enemy_state.active.callee=pending.active_callee;
        require(pending.session.adopt_staged(pending_vm,error)==Status::complete &&
                    pending.session.ready() && pending_vm.uses_services(services),
                "source-published pending VM could not become the active actor VM");
        run_one(pending);
        require(pending_vm.statistics().completed_callbacks==1,
                "pending-to-active transition duplicated its VM");
        lifecycle.active+=1;
        const auto before=pending.search_calls;
        require(!pending.session.ready() && pending.session.search_and_dispatch(&pending.list,&pending.rooms,
                    1500.f,6.2831855f,&result)==Status::stale_binding && pending.search_calls==before,
                "replaced source active AIS continued to acquire targets");
        require(pending_vm.reset(error)==monster_external_script::Status::complete,
                "pending-to-active VM teardown failed");
    }
    {
        monster_external_script::Session pending_vm;
        Fixture replaced(0xc00000000ull);
        character::ScriptLifecycleState64 lifecycle{};
        monster_external_script::Services services{};
        prepare_pending(replaced,lifecycle,pending_vm,common,monster,services);
        lifecycle.pending+=1;
        float property=123.f;
        const auto trace_before=replaced.trace.size();
        std::string error;
        require(services.get_prop(services.context,replaced.owner_id,28,&property)!=0 && property==123.f &&
                    replaced.trace.size()==trace_before &&
                    replaced.session.adopt_staged(pending_vm,error)==Status::stale_binding,
                "replaced pending AIS reached the old callback provider");
        require(pending_vm.reset(error)==monster_external_script::Status::complete,
                "replaced pending VM teardown failed");
    }
    {
        monster_external_script::Session pending_vm;
        Fixture stale(0xd00000000ull);
        character::ScriptLifecycleState64 lifecycle{};
        monster_external_script::Services services{};
        prepare_pending(stale,lifecycle,pending_vm,common,monster,services);
        lifecycle.owner+=1;
        float property=123.f;
        std::string error;
        require(services.get_prop(services.context,stale.owner_id,28,&property)!=0 && property==123.f &&
                    stale.session.adopt_staged(pending_vm,error)==Status::stale_binding,
                "changed lifecycle owner retained the pending callback context");
        require(pending_vm.reset(error)==monster_external_script::Status::complete,
                "stale pending VM teardown failed");
    }
    {
        monster_external_script::Session pending_vm;
        Fixture guarded(0xe00000000ull);
        character::ScriptLifecycleState64 lifecycle{};
        monster_external_script::Services services{};
        prepare_pending(guarded,lifecycle,pending_vm,common,monster,services);
        publish_pending(guarded,lifecycle);
        std::string error;
        require(guarded.session.adopt_staged(pending_vm,error)==Status::complete,
                "lifecycle alias fixture adoption failed");
        const auto before=lifecycle;
        require(guarded.session.search_and_dispatch(&guarded.list,&guarded.rooms,1500.f,6.2831855f,
                    reinterpret_cast<ScanResult*>(&lifecycle))==Status::invalid_argument &&
                    guarded.search_calls==0 && std::memcmp(&before,&lifecycle,sizeof(before))==0,
                "scan output overwrote borrowed lifecycle ownership");
        require(guarded.session.prepare_pending(guarded.bindings,nullptr,error)==Status::invalid_argument &&
                    guarded.session.ready(),
                "invalid pending rebind discarded an existing active VM");
        require(pending_vm.reset(error)==monster_external_script::Status::complete,
                "lifecycle alias VM teardown failed");
    }

    run_flat_character_routes(common,monster);
    std::puts("{\"ghost_ai_session_cases\":19,\"flat_character_route_cases\":6,\"source_flat_character_to_path\":true,\"mismatches\":0,\"native_wired\":false,\"source_search_to_path\":true,\"per_actor_vm_and_target_identity\":true,\"stale_owner_rebind\":true,\"reentrant_rebind_guard\":true,\"partial_failure_effects\":true,\"fresh_empty_search_event_12\":true,\"fresh_all_false_relation_event_12\":true,\"output_alias_guard\":true,\"staged_vm_adopted_without_duplicate\":true,\"source_pending_publication\":true,\"pending_replacement_guard\":true,\"pending_owner_guard\":true,\"lifecycle_output_alias_guard\":true,\"unbuilt_updateaggro_prefix\":true}");
    return 0;
 } catch(const std::exception& error) { std::fprintf(stderr,"ghost_ai_session: %s\n",error.what());return 1; }
}
