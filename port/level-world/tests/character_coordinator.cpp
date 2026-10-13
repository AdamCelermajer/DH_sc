#include "character_coordinator.hpp"
#include "character_ai_skill_machine_projection_v1.hpp"
#include "character_skill_state_dispatch_v1.hpp"
#include "character_skill_state_queries.hpp"
#include "player_char_ai_target_prefix_v1.hpp"
#include "../player_character_zonability_v1.hpp"
#include "../world_object_character_identity_v1.hpp"
#include "../character_physical_collision_gate_v1.hpp"

#include <cstdio>
#include <stdexcept>
#include <string>
#include <vector>

using namespace dh2::character;
namespace {
void check(bool value, const char* message) {
    if (!value) throw std::runtime_error(message);
}
namespace Frozen=dh2::character_skill_fsm_callbacks_v1;
namespace Dispatch=dh2::character_skill_state_dispatch_v1;
namespace TargetUpdate=dh2::character_ai_update_target;
namespace PlayerTarget=dh2::player_char_ai_target_prefix_v1;
namespace PlayerZonability=dh2::player_character_zonability_v1;
namespace BodyIdentity=dh2::world_object_character_identity_v1;
struct AIFrameFixture {
    Coordinator character{0x410000001ull,1};
    AIFrameOwner48 owner{};
    AIFrameState32 frame{};
    std::vector<std::uint32_t> calls;
    AIFrameFixture() {
        character.state.flags=0x100;
        owner={character.owner(),0x411,character.state.flags,0,0,0,0,0,0,0};
        CoordinatorBindings bindings{};
        bindings.context=this;bindings.facts=read_facts;
        bindings.services={this,state_service};bindings.ai_identity=0x412;
        bindings.ai_owner_projection=&owner;bindings.controller_identity=owner.controller;
        character.bind(bindings);
        frame={bindings.ai_identity,&owner,0,0,0,0};
    }
    static Facts read_facts(void*) { return {}; }
    static void state_service(void*,State*,const Request*) {}
    static int service(void* context,AIFrameState32*,
                       const AIFrameRequest16* request,std::uint32_t* value) {
        auto& self=*static_cast<AIFrameFixture*>(context);
        self.calls.push_back(request->service);
        *value=0; // Source Character::IsZonable result for this fixture.
        return 0;
    }
};
struct LogicFrameFixture {
    Coordinator character{0x420000001ull,1};
    AIFrameOwner48 owner{};
    AIFrameState32 frame{};
    std::vector<std::uint32_t> order;
    LogicFrameFixture() {
        character.state.current=3;
        character.state.flags=0x100;
        owner={character.owner(),0x421,character.state.flags,0,0,0,0,0,0,0};
        CoordinatorBindings bindings{};
        bindings.context=this;bindings.facts=read_facts;
        bindings.services={this,state_service};
        bindings.before_timer_event=before_timer;
        bindings.route_timer_event=route_timer;
        bindings.ai_identity=0x422;bindings.ai_owner_projection=&owner;
        bindings.controller_identity=owner.controller;
        character.bind(bindings);
        frame={bindings.ai_identity,&owner,0,0,0,0};
    }
    static Facts read_facts(void*) { return {}; }
    static void state_service(void* context,State*,const Request* request) {
        auto& self=*static_cast<LogicFrameFixture*>(context);
        if(request->service==idle_common_update)self.order.push_back(6);
    }
    static void before_timer(void* context,Coordinator&,std::int32_t,
                            Timer32&,std::uint32_t) {
        static_cast<LogicFrameFixture*>(context)->order.push_back(1);
    }
    static TimerRouting route_timer(void*,Coordinator&,std::int32_t,
                                    Timer32&,std::uint32_t) {
        return TimerRouting::delivered;
    }
    static int ai_service(void* context,AIFrameState32*,
                          const AIFrameRequest16* request,std::uint32_t* value) {
        auto& self=*static_cast<LogicFrameFixture*>(context);
        self.order.push_back(request->service+2);
        *value=0; // IsZonable=false; all other service results are ignored.
        return 0;
    }
};
// CSAnim (CharStateMachine state 14) fixture: Player-like facts plus an ordered
// log of every state service the kernel asks the host for.
struct AnimStateFixture {
    Coordinator character{0x430000001ull, 1};
    Facts facts{};
    struct Call { std::uint32_t service; std::int32_t argument; std::int32_t state; };
    std::vector<Call> calls;
    std::vector<std::int32_t> previous_states;
    AnimStateFixture() {
        facts.is_player=1;facts.idle=11;facts.walk=22;facts.run=33;
        facts.attack_static=44;facts.attack_moving=55;
        facts.walk_threshold=.45f;facts.run_threshold=.85f;facts.walk_speed=1.3f;
        facts.heading[0]=.5f;
        CoordinatorBindings bindings{};
        bindings.context=this;bindings.facts=read_facts;
        bindings.services={this,state_service};
        character.bind(bindings);
    }
    static Facts read_facts(void* context){return static_cast<AnimStateFixture*>(context)->facts;}
    static void state_service(void* context,State* state,const Request* request){
        auto& self=*static_cast<AnimStateFixture*>(context);
        check(state==&self.character.state,"CSAnim Coordinator state owner differs");
        self.calls.push_back({request->service,request->argument[0],state->current});
        if(request->service==set_animation)state->current_animation=request->argument[0];
        if(request->service==raise_event&&request->argument[0]==0x1d)
            self.previous_states.push_back(request->argument[1]);
    }
    unsigned count(std::uint32_t service) const {
        unsigned n=0;for(const auto& call:calls)if(call.service==service)++n;return n;
    }
};
struct Fixture {
    Coordinator character{0x100000001ull, 1};
    Facts facts{};
    SpawnFacts spawn{};
    std::vector<std::uint32_t> timer_order;
    unsigned facts_reads = 0;
    unsigned service_calls = 0;
    unsigned interaction_blurs = 0;
    bool change_idle_on_stop = false;
    bool nested_gate_event = false;
    bool throw_animation = false;
    bool throw_expiry = false;
    bool mutate_timer_event = false;
    bool test_growth_inside_expiry = false;
    std::uint32_t captured_gate = 0;
    std::uintptr_t timer_payload = 0;

    Fixture() {
        facts.is_player = 1;
        facts.idle = 11; facts.walk = 22; facts.run = 33;
        facts.attack_static = 44; facts.attack_moving = 55;
        facts.walk_threshold = .45f; facts.run_threshold = .85f;
        facts.walk_speed = 1.3f;
        facts.heading[0] = .5f;
        character.bind({this, read_facts, {this, service}, before, after});
    }
    static Facts read_facts(void* context) {
        auto& fixture = *static_cast<Fixture*>(context);
        ++fixture.facts_reads;
        return fixture.facts;
    }
    static void service(void* context, State* state, const Request* request) {
        auto& fixture = *static_cast<Fixture*>(context);
        check(state == &fixture.character.state, "borrowed Character owner differs");
        ++fixture.service_calls;
        if (request->service == restore_interaction_rotation) {
            ++fixture.interaction_blurs;
        } else if (request->service == set_animation) {
            if (fixture.throw_animation) throw std::runtime_error("injected backend failure");
            state->current_animation = request->argument[0];
        } else if (request->service == stop && fixture.change_idle_on_stop) {
            fixture.facts.idle = 99;
            fixture.facts.heading[0] = 0;
            fixture.character.refresh_facts();
        } else if (request->service == raise_event) {
            const auto event = std::uint32_t(request->argument[0]);
            if (event == 0x3f)
                check(fixture.character.event(event) == 1, "nested stop event failed");
            if (event == 0x1d && fixture.nested_gate_event) {
                const auto cause = fixture.character.event_cause();
                check(cause == 0xc351, "outer event cause is missing");
                check(fixture.character.event(0x2c) == 0, "nested gate event failed");
                check(fixture.character.event_cause() == cause,
                      "nested event did not restore outer cause");
            }
        }
    }
    static void before(void* context, Coordinator& owner, std::int32_t event,
                       Timer32& timer, std::uint32_t gate) {
        auto& fixture = *static_cast<Fixture*>(context);
        check(&owner == &fixture.character && event == 0x2a,
              "expiry owner or captured event differs");
        check(owner.timers().update_depth == 1, "timer storage is not borrowed");
        fixture.captured_gate = gate;
        fixture.timer_payload = reinterpret_cast<std::uintptr_t>(&timer);
        fixture.timer_order.push_back(1);
        if (fixture.mutate_timer_event) timer.event = 0x2c;
        if (fixture.test_growth_inside_expiry) {
            // One repeating timer occupies the sole slot. Source Start refuses
            // allocation during Update, preserving the live expiry pointer.
            check(owner.start_timer(10, 0, 0x2a, 0) == -3,
                  "expiry relocated borrowed timer storage");
            check(owner.timers().slots == &timer, "expiry timer pointer moved");
        }
        if (fixture.throw_expiry) throw std::runtime_error("injected AI expiry failure");
    }
    static void after(void* context, Coordinator& owner, std::int32_t event,
                      Timer32& timer, std::uint32_t gate) {
        auto& fixture = *static_cast<Fixture*>(context);
        check(event == 0x2a && gate == fixture.captured_gate,
              "captured expiry inputs were changed");
        check(reinterpret_cast<std::uintptr_t>(&timer) == fixture.timer_payload,
              "timer payload identity differs");
        check(owner.state.attack_gate == (gate & ~1u),
              "state gate was not cleared after expiry hook forwarding");
        fixture.timer_order.push_back(2);
        if (fixture.test_growth_inside_expiry) owner.stop_timer(timer.id);
    }
};

struct SkillFixture {
    Coordinator character{0x200000001ull,1};
    Facts facts{};
    std::vector<Frozen::Operation> skill_operations;
    std::vector<std::int32_t> previous_states;
    std::uint8_t heading=0,moving=0;
    std::uintptr_t physical=0;
    Frozen::Character skill_character{};
    Frozen::State callback_state{};
    Frozen::Globals globals{};
    Frozen::Services callback_services{};
    Dispatch::Projection projection{};

    explicit SkillFixture(bool bind_projection_during_coordinator_bind=true){
        facts.is_player=1;facts.idle=11;facts.walk=22;facts.run=33;
        facts.attack_static=44;facts.attack_moving=55;
        facts.walk_threshold=.45f;facts.run_threshold=.85f;facts.walk_speed=1.3f;
        facts.heading[0]=.5f;
        skill_character={0x201,0x202,0x203,0x204,0x205,
            &character.state.flags,&character.state.attack_gate,&heading,&moving,&physical};
        callback_state={&skill_character};globals={0x206};
        callback_services={this,skill_service};
        projection={&character.state,&callback_state,&globals,&callback_services};
        CoordinatorBindings bindings{};
        bindings.context=this;bindings.facts=read_facts;
        bindings.services={this,state_service};
        if(bind_projection_during_coordinator_bind)
            bindings.skill_projection=&projection;
        character.bind(bindings);
    }
    static Facts read_facts(void* context){return static_cast<SkillFixture*>(context)->facts;}
    static void state_service(void* context,State* state,const Request* request){
        auto& fixture=*static_cast<SkillFixture*>(context);
        check(state==&fixture.character.state,"skill Coordinator state owner differs");
        if(request->service==set_animation)state->current_animation=request->argument[0];
        if(request->service==raise_event&&request->argument[0]==0x1d)
            fixture.previous_states.push_back(request->argument[1]);
    }
    static std::int32_t skill_service(void* context,Frozen::State*,
        const Frozen::Request* request,Frozen::Response* response){
        auto& fixture=*static_cast<SkillFixture*>(context);
        fixture.skill_operations.push_back(request->operation);
        if(request->operation==Frozen::Operation::string_construct)
            response->identity=0x207;
        // This fixture is a non-monster for the CSSkill classification query;
        // the frozen source caller returns normally without changing flags.
        if(request->operation==Frozen::Operation::is_monster)response->word=0;
        return 0;
    }
};

struct SkillMachineBridgeFixture {
    Coordinator character{0x300000001ull,1};
    Facts facts{};
    std::vector<Frozen::Operation> operations;
    std::vector<std::uint32_t> callback_events;
    std::vector<std::int32_t> transition_previous;
    std::uint8_t ooi_intent=1;
    std::uintptr_t physical=0x303;

    SkillMachineBridgeFixture(){
        facts.is_player=1;facts.idle=11;facts.walk=22;facts.run=33;
        facts.attack_static=44;facts.attack_moving=55;
        facts.walk_threshold=.45f;facts.run_threshold=.85f;facts.walk_speed=1.3f;
        CoordinatorBindings bindings{};
        bindings.context=this;
        bindings.facts=read_facts;bindings.services={this,state_service};
        character.bind(bindings);
    }
    static Facts read_facts(void* context){
        return static_cast<SkillMachineBridgeFixture*>(context)->facts;
    }
    static void state_service(void* context,State* state,const Request* request){
        auto& fixture=*static_cast<SkillMachineBridgeFixture*>(context);
        check(state==&fixture.character.state,"bound skill callback changed Coordinator state owner");
        if(request->service==raise_event&&request->argument[0]==0x1d)
            fixture.transition_previous.push_back(request->argument[1]);
    }
    static std::int32_t skill_service(void* context,Frozen::State*,
        const Frozen::Request* request,Frozen::Response* response){
        auto& fixture=*static_cast<SkillMachineBridgeFixture*>(context);
        fixture.operations.push_back(request->operation);
        if(request->operation==Frozen::Operation::string_construct)
            response->identity=0x304;
        if(request->operation==Frozen::Operation::raise_event)
            fixture.callback_events.push_back(request->argument0);
        if(request->operation==Frozen::Operation::start_timer)
            check(request->argument0==10&&request->argument1==0&&
                  request->argument2==0x30,
                  "CSSkill Blur did not use source timer parameters on the same Coordinator");
        if(request->operation==Frozen::Operation::is_monster)response->word=0;
        return 0;
    }
};
}

int main() {
    try {
        {
            Coordinator character{0x400000001ull, 1};
            check(character.source_machine_flags_44_528() == 0,
                  "CharStateMachine+0x44 constructor word is not zero");
            character.set_source_machine_flags_44_528(0x00000824u);
            check(character.source_machine_flags_44_528() == 0x00000824u &&
                  character.owner() == 0x400000001ull,
                  "Character+0x528 / CharStateMachine+0x44 word lost its canonical Coordinator owner");
        }
        {
            Fixture fixture;
            fixture.character.state.current=12;
            check(fixture.character.set_idle_state(true)==1&&
                  fixture.character.state.current==3&&
                  fixture.character.state.idle_suppressed==1,
                  "SM_SetIdleState(true) did not store suppression before the same FSM transition");
            check(fixture.character.set_idle_state(false)==1&&
                  fixture.character.state.current==3&&
                  fixture.character.state.idle_suppressed==0,
                  "SM_SetIdleState(false) did not clear suppression on the canonical FSM");
        }
        {
            Fixture fixture;
            const auto before = fixture.character.state;
            const auto timers_before = fixture.character.timers();
            check(fixture.character.register_source_state(
                      fixture.character.owner() + 1, 0, 0x600) == false &&
                  fixture.character.register_source_state(
                      fixture.character.owner(), 20, 0x600) == false &&
                  fixture.character.register_source_state(
                      fixture.character.owner(), 0, 0) == false,
                  "Source state registration accepted a foreign identity, invalid ID, or null handler");
            check(fixture.character.register_source_state(
                      fixture.character.owner(), 0, 0x600) &&
                  fixture.character.register_source_state(
                      fixture.character.owner(), 0, 0x600) &&
                  !fixture.character.register_source_state(
                      fixture.character.owner(), 0, 0x601) &&
                  !fixture.character.source_state_handlers_complete() &&
                  fixture.character.source_state_handler_mask() == 1 &&
                  fixture.character.source_state_registered(
                      fixture.character.owner(), 0) &&
                  !fixture.character.source_state_registered(
                      fixture.character.owner() + 1, 0) &&
                  !fixture.character.source_state_registered(
                      fixture.character.owner(), 1),
                  "Source state registration did not retain a stable handler token");
            for (std::uint32_t state = 1; state < 20; ++state)
                check(fixture.character.register_source_state(
                          fixture.character.owner(), state, 0x600),
                      "Canonical Coordinator rejected a valid source state handler");
            check(fixture.character.source_state_handlers_complete() &&
                  fixture.character.unregister_source_state(
                      fixture.character.owner(), 0, 0x600) &&
                  !fixture.character.source_state_handlers_complete() &&
                  fixture.character.register_source_state(
                      fixture.character.owner(), 0, 0x600) &&
                  fixture.character.state.current == before.current &&
                  fixture.character.state.elapsed_ms == before.elapsed_ms &&
                  fixture.character.timers().slots == timers_before.slots &&
                  fixture.character.timers().count == timers_before.count &&
                  fixture.character.timers().capacity == timers_before.capacity,
                  "Source state table changed live state/timers or failed exact rollback");
        }
        {
            Fixture fixture;
            fixture.character.state.current=3;
            fixture.character.set_source_interaction_row_48(3);
            check(fixture.character.event(50003u,0x410)==1&&
                  fixture.character.state.current==13&&
                  fixture.character.event(34u,0x410)==1&&
                  fixture.character.state.current==3&&
                  fixture.character.source_interaction_row_48()==3&&
                  fixture.interaction_blurs==1,
                  "Character::Interact 50003/CSInteract 34 did not close on the same Coordinator");
        }
        {
            Fixture fixture;
            fixture.character.state.current=13;
            fixture.character.set_source_interaction_row_48(3);
            check(fixture.character.event(34u,0)==1&&
                  fixture.character.state.current==3&&
                  fixture.character.source_interaction_row_48()==3&&
                  fixture.interaction_blurs==1,
                  "CSInteract event34 did not return the retained row3 Character to Idle");
            fixture.character.state.current=13;
            fixture.character.set_source_interaction_row_48(4);
            const auto before=fixture.character.state;
            check(fixture.character.event(34u,0)==-1&&
                  fixture.character.state.current==before.current&&
                  fixture.character.state.flags==before.flags,
                  "Unsupported LiftableObject event34 branch mutated the Character Coordinator");
        }
        {
            BodyIdentity::Projection projection{};
            int first_body=0,second_body=0;
            std::uintptr_t identity=0xdead;
            check(BodyIdentity::resolve(&projection,&first_body,&identity)==
                      BodyIdentity::Status::unbound&&identity==0xdead,
                  "Unbound physical Character identity resolved");
            check(BodyIdentity::bind(&projection,&first_body,0x440)==
                      BodyIdentity::Status::complete&&
                  BodyIdentity::bind(&projection,&first_body,0x441)==
                      BodyIdentity::Status::already_bound,
                  "Body identity projection replaced its live owner");
            check(BodyIdentity::resolve(&projection,&second_body,&identity)==
                      BodyIdentity::Status::body_mismatch&&identity==0xdead&&
                  BodyIdentity::retire(&projection,&second_body)==
                      BodyIdentity::Status::body_mismatch&&
                  projection.game_object_identity==0x440,
                  "Stale body resolved or retired another body's identity");
            check(BodyIdentity::resolve(&projection,&first_body,&identity)==
                      BodyIdentity::Status::complete&&identity==0x440&&
                  BodyIdentity::retire(&projection,&first_body)==
                      BodyIdentity::Status::complete&&
                  BodyIdentity::resolve(&projection,&first_body,&identity)==
                      BodyIdentity::Status::unbound&&identity==0x440,
                  "Body identity did not retire with its physical owner");
            check(BodyIdentity::bind(&projection,&second_body,0x441)==
                      BodyIdentity::Status::complete&&
                  BodyIdentity::resolve(&projection,&second_body,&identity)==
                      BodyIdentity::Status::complete&&identity==0x441,
                  "Replacement physical body did not acquire its own identity");
        }
        {
            int player_body=0,peer_body=0,unbound_body=0;
            BodyIdentity::Projection player{},peer{},unbound{};
            BodyIdentity::PersistEvent event{0xdead,0xbeef};
            check(BodyIdentity::bind(&player,&player_body,0x440)==
                      BodyIdentity::Status::complete&&
                  BodyIdentity::bind(&peer,&peer_body,0x441)==
                      BodyIdentity::Status::complete&&
                  BodyIdentity::project_player_persist(&player,&player_body,0x440,
                      &peer,&peer_body,true,&event)==BodyIdentity::Status::complete&&
                  event.character_event==0x39&&event.peer_game_object_identity==0x441,
                  "Player persist did not preserve source instigator event and peer identity");
            check(BodyIdentity::project_player_persist(&player,&player_body,0x440,
                      &peer,&peer_body,false,&event)==BodyIdentity::Status::complete&&
                  event.character_event==0x3a&&event.peer_game_object_identity==0x441,
                  "Non-instigator persist did not map to source event 0x3a");
            const auto unchanged=event;
            check(BodyIdentity::project_player_persist(&player,&player_body,0x442,
                      &peer,&peer_body,true,&event)==
                      BodyIdentity::Status::owner_identity_mismatch&&
                  event.character_event==unchanged.character_event&&
                  event.peer_game_object_identity==unchanged.peer_game_object_identity&&
                  BodyIdentity::project_player_persist(&player,&player_body,0x440,
                      &unbound,&unbound_body,true,&event)==
                      BodyIdentity::Status::non_character_peer&&
                  event.character_event==unchanged.character_event&&
                  event.peer_game_object_identity==unchanged.peer_game_object_identity,
                  "Persist projection accepted a foreign Player or non-Character peer");
        }
        {
            namespace Gate=dh2::character_physical_collision_gate_v1;
            check(Gate::evaluate(0,0)==Gate::Decision::reject_limbus&&
                  Gate::evaluate(0,3)==Gate::Decision::reject_limbus,
                  "POCharacter Limbus gate did not reject every category");
            check(Gate::evaluate(10,0)==Gate::Decision::reject_knockback_category&&
                  Gate::evaluate(10,1)==Gate::Decision::delegate_generic&&
                  Gate::evaluate(10,2)==Gate::Decision::delegate_generic&&
                  Gate::evaluate(10,3)==Gate::Decision::delegate_generic,
                  "POCharacter KnockedBack gate did not test other category low bits");
            check(Gate::evaluate(-1,0)==Gate::Decision::delegate_generic&&
                  Gate::evaluate(3,0)==Gate::Decision::delegate_generic,
                  "POCharacter custom gate altered non-Limbus/non-KnockedBack states");
        }
        {
            Fixture player;
            const PlayerZonability::Owner owner{player.character.owner()};
            PlayerZonability::Result result{};
            check(PlayerZonability::evaluate(&owner,&result)==
                      PlayerZonability::Status::complete&&
                  result.decision==dh2::character_zonability::Decision::player&&
                  result.service_calls==1&&result.is_player_word==1&&
                  result.is_faerie_word==0&&result.zonable==0&&
                  result.captured_character==player.character.owner(),
                  "Player zonability did not take the source IsPlayer early return");
        }
        {
            Fixture player;
            player.character.state.current=3;
            dh2::character_ai_initialization::State ai{};
            ai.identity=0x430;ai.owner_04=player.character.owner();
            ai.byte_54=1;ai.byte_55=1;
            set_target::OwnerFacts target_owner{
                player.character.owner(),8,0,0};
            const PlayerTarget::Bindings bindings{
                &player.character,&ai,&target_owner};
            PlayerTarget::Result result{};
            PlayerTarget::MasterResult master_result{};
            check(PlayerTarget::update_master_null(&bindings,&master_result)==
                      PlayerTarget::Status::complete&&master_result.calls==0&&
                  master_result.events==0&&ai.byte_54==1&&ai.byte_55==1,
                  "Player null-master update did not follow the source early return");
            ai.master_50=0x432;
            check(PlayerTarget::update_master_null(&bindings,&master_result)==
                      PlayerTarget::Status::source_failed&&master_result.calls==1&&
                  ai.byte_54==1&&ai.byte_55==1,
                  "Player master adapter fabricated an unavailable nonnull-master query");
            ai.master_50=0;
            check(PlayerTarget::update(&bindings,&result)==
                      PlayerTarget::Status::complete&&
                  result.decision==TargetUpdate::Decision::no_target&&
                  result.service_calls==2&&ai.target_40==0&&ai.last_target_44==0,
                  "Player CharAI target prefix did not reuse FSM state for its null-target path");
            player.character.state.current=17;
            check(PlayerTarget::update(&bindings,&result)==
                      PlayerTarget::Status::complete&&
                  result.decision==TargetUpdate::Decision::awaiting_spawn&&
                  result.service_calls==1,
                  "Player CharAI target prefix did not stop at source awaiting-spawn gate");
            player.character.state.current=0;
            check(PlayerTarget::update(&bindings,&result)==
                      PlayerTarget::Status::complete&&
                  result.decision==TargetUpdate::Decision::in_limbus&&
                  result.service_calls==2,
                  "Player CharAI target prefix did not stop at source limbus gate");
            player.character.state.current=3;ai.target_40=0x431;
            check(PlayerTarget::update(&bindings,&result)==
                      PlayerTarget::Status::source_failed&&
                  result.decision==TargetUpdate::Decision::incomplete&&
                  result.service_calls==3&&ai.target_40==0x431,
                  "Player CharAI target prefix fabricated a nonnull interaction result");
        }
        {
            LogicFrameFixture logic;
            check(logic.character.start_timer(1,0,0x2a,0)==0,
                  "logic-frame timer setup failed");
            AIFrameServices24 all_services{&logic,LogicFrameFixture::ai_service,31,0};
            LogicFrameResult result{};
            check(logic.character.update_logic_frame(1,0,&logic.frame,
                      &all_services,&result)==0&&
                  result.phase==LogicFramePhase::complete&&
                  result.timer_status==1&&result.ai_status==0&&
                  result.ai.service_calls==5&&result.state_status==1&&
                  logic.order==std::vector<std::uint32_t>({
                      1,2,3,4,5,6,6}),
                  "Character logic frame did not run timers, CharAI, then state through one owner");

            logic.order.clear();
            check(logic.character.start_timer(1,0,0x2a,0)==0,
                  "logic-frame failure timer setup failed");
            AIFrameServices24 target_missing{&logic,LogicFrameFixture::ai_service,1,0};
            check(logic.character.update_logic_frame(1,0,&logic.frame,
                      &target_missing,&result)==-3&&
                  result.phase==LogicFramePhase::char_ai&&
                  result.ai_status==2&&result.ai.last_service==ai_frame_update_target&&
                  logic.order==std::vector<std::uint32_t>({1,2}),
                  "logic-frame failure did not preserve completed prefixes and stop before Character state");

            logic.order.clear();logic.frame.paused=1;
            check(logic.character.update_logic_frame(1,0,&logic.frame,
                      &all_services,&result)==0&&
                  result.phase==LogicFramePhase::complete&&
                  result.ai.skip==ai_frame_paused&&result.ai.service_calls==0&&
                  result.state_status==1&&logic.order==std::vector<std::uint32_t>({6}),
                  "CharAI pause gate incorrectly skipped the following Character state phase");
        }
        {
            AIFrameFixture frame;
            AIFrameServices24 missing_target{&frame,AIFrameFixture::service,1,0};
            AIFrameResult16 result{};
            frame.frame.paused=1;
            check(frame.character.update_ai_frame(&frame.frame,&missing_target,&result)==0&&
                  result.skip==ai_frame_paused&&result.service_calls==0&&frame.calls.empty(),
                  "Coordinator AI frame did not apply the source pause gate first");
            frame.frame.paused=0;
            check(frame.character.update_ai_frame(&frame.frame,&missing_target,&result)==2&&
                  result.last_service==ai_frame_update_target&&result.service_calls==1&&
                  frame.calls==std::vector<std::uint32_t>({ai_frame_is_zonable})&&
                  frame.owner.updated88==1,
                  "Unavailable target owner did not stop the frame before AIS virtual dispatch");
            auto substituted_owner=frame.owner;
            substituted_owner.forced=1; // Must not forge CharAI's controller bypass.
            frame.frame.owner=&substituted_owner;
            const auto calls=frame.calls.size();
            check(frame.character.update_ai_frame(&frame.frame,&missing_target,&result)==-1&&
                  frame.calls.size()==calls,
                  "AI frame accepted a copied owner with a forged forced bit");
            frame.frame.owner=&frame.owner;
            frame.owner.controller^=1;
            check(frame.character.update_ai_frame(&frame.frame,&missing_target,&result)==-1&&
                  frame.calls.size()==calls,
                  "AI frame accepted the wrong canonical controller identity");
            frame.owner.controller^=1;
            frame.frame.ai^=1;
            check(frame.character.update_ai_frame(&frame.frame,&missing_target,&result)==-1&&
                  frame.calls.size()==calls,
                  "AI frame accepted a CharAI not associated with this Character");
            frame.frame.ai^=1;
            frame.owner.flags520^=1;
            check(frame.character.update_ai_frame(&frame.frame,&missing_target,&result)==-1&&
                  frame.calls.size()==calls,
                  "AI frame accepted a projection from another Character state");
        }
        Coordinator unbound(1);
        check(unbound.event(0xc351) == -1 && unbound.state.current == -1,
              "unbound event mutated state");
        bool rejected = false;
        try { unbound.bind({}); } catch (const std::invalid_argument&) { rejected = true; }
        check(rejected, "missing source services accepted");

        Coordinator machine_owner(0x100000002ull, 1);
        dh2::character_ai_skill_machine_projection_v1::Projection skill_machine(
            machine_owner, machine_owner.owner(), 0x100000003ull);
        auto* command_character = skill_machine.character();
        auto* command_machine = skill_machine.machine();
        check(command_character->identity == machine_owner.owner() &&
              command_character->skill_machine_4fc == command_machine &&
              command_machine->owner_04 == machine_owner.owner() &&
              command_machine->identity != 0 &&
              command_machine->state_query->current_state_id ==
                  &machine_owner.state.current &&
              skill_machine.state_query() == command_machine->state_query &&
              *command_machine->animation_28 == -1 &&
              *command_machine->skill_index_54 == 0 &&
              *command_machine->moving_58 == 0,
              "skill machine did not borrow Coordinator state with source constructor fields");
        dh2::character_skill_state_queries::Result skill_state{};
        machine_owner.state.current = 6;
        check(dh2::character_skill_state_queries::is_using_skill(
                  command_machine->state_query, &skill_state) ==
                  dh2::character_skill_state_queries::Status::complete &&
              skill_state.value == 1,
              "skill machine query did not read the Coordinator's live state ID");
        *command_machine->animation_28 = 0x1234;
        *command_machine->skill_index_54 = 7;
        *command_machine->moving_58 = 1;
        check(*command_machine->animation_28 == 0x1234 &&
              *command_machine->skill_index_54 == 7 &&
              *command_machine->moving_58 == 1 &&
              machine_owner.state.current == 6,
              "skill machine fields were not retained separately from the sole FSM state");
        rejected = false;
        try {
            dh2::character_ai_skill_machine_projection_v1::Projection wrong_owner(
                machine_owner, 0x100000004ull, 0x100000003ull);
        } catch (const std::invalid_argument&) { rejected = true; }
        check(rejected, "skill machine projection accepted a different Character owner");

        SkillMachineBridgeFixture skill_bridge;
        dh2::character_ai_skill_machine_projection_v1::Projection bridged_machine(
            skill_bridge.character,skill_bridge.character.owner(),0x300000002ull);
        std::string skill_bridge_error;
        Frozen::Services missing_skill_services{};
        check(!bridged_machine.bind_skill_state_callbacks(0x300000003ull,
                  0x300000004ull,&skill_bridge.ooi_intent,&skill_bridge.physical,
                  missing_skill_services,skill_bridge_error)&&
              !skill_bridge_error.empty()&&
              !bridged_machine.skill_state_callbacks_bound(),
              "CSSkill projection accepted missing operation services");
        const Frozen::Services skill_callbacks{&skill_bridge,
                                               SkillMachineBridgeFixture::skill_service};
        check(bridged_machine.bind_skill_state_callbacks(0x300000003ull,
                  0x300000004ull,&skill_bridge.ooi_intent,&skill_bridge.physical,
                  skill_callbacks,skill_bridge_error)&&
              bridged_machine.skill_state_callbacks_bound(),
              "CSSkill projection did not bind to the retained machine/Coordinator");
        // Rebinding the base Coordinator services replaces CoordinatorBindings
        // and can clear the optional borrowed CSSkill projection. Repeating the
        // same graph bind must restore that link, rather than treating the
        // Projection object's local fsm_bound_ bit as proof it is still attached.
        CoordinatorBindings skill_bridge_base{};
        skill_bridge_base.context=&skill_bridge;
        skill_bridge_base.facts=SkillMachineBridgeFixture::read_facts;
        skill_bridge_base.services={&skill_bridge,
                                    SkillMachineBridgeFixture::state_service};
        skill_bridge.character.bind(skill_bridge_base);
        check(bridged_machine.bind_skill_state_callbacks(0x300000003ull,
                  0x300000004ull,&skill_bridge.ooi_intent,&skill_bridge.physical,
                  skill_callbacks,skill_bridge_error),
              "same-graph CSSkill bind did not restore Coordinator projection after base rebind");
        check(skill_bridge.character.transition(3)==1,
              "CSSkill bridge could not enter source Idle");
        *bridged_machine.machine()->moving_58=1;
        check(skill_bridge.character.event(0xc355)==1&&
              skill_bridge.character.state.current==6&&
              skill_bridge.character.state.flags==0x6341&&
              skill_bridge.character.state.attack_gate==0x100&&
              skill_bridge.callback_events==std::vector<std::uint32_t>({0x1e})&&
              skill_bridge.operations==std::vector<Frozen::Operation>({
                  Frozen::Operation::debug_load,Frozen::Operation::string_construct,
                  Frozen::Operation::debug_query,Frozen::Operation::string_destroy,
                  Frozen::Operation::raise_event,Frozen::Operation::set_animation,
                  Frozen::Operation::set_speed,Frozen::Operation::cancel_sneaking,
                  Frozen::Operation::unpin,Frozen::Operation::is_monster}),
              "C355 did not route CSSkill Focus in source operation order");
        check(!bridged_machine.unbind_skill_state_callbacks(skill_bridge_error),
              "CSSkill projection detached while state 6 was still active");
        check(skill_bridge.character.event(0x22)==1&&
              skill_bridge.character.state.current==3,
              "CSSkill close event did not return to Idle");
        check(skill_bridge.callback_events==std::vector<std::uint32_t>({0x1e,0x1f})&&
              skill_bridge.operations==std::vector<Frozen::Operation>({
                  Frozen::Operation::debug_load,Frozen::Operation::string_construct,
                  Frozen::Operation::debug_query,Frozen::Operation::string_destroy,
                  Frozen::Operation::raise_event,Frozen::Operation::set_animation,
                  Frozen::Operation::set_speed,Frozen::Operation::cancel_sneaking,
                  Frozen::Operation::unpin,Frozen::Operation::is_monster,
                  Frozen::Operation::debug_load,Frozen::Operation::string_construct,
                  Frozen::Operation::debug_query,Frozen::Operation::string_destroy,
                  Frozen::Operation::sync_last_target,Frozen::Operation::stop,
                  Frozen::Operation::raise_event,Frozen::Operation::start_timer,
                  Frozen::Operation::is_monster}),
              "CSSkill Blur callback events or operations differ");
        check(skill_bridge.transition_previous==std::vector<std::int32_t>({-1,3,6}),
              "CSSkill Focus/Blur transition previous-state sequence differs");
        check(bridged_machine.unbind_skill_state_callbacks(skill_bridge_error)&&
              !bridged_machine.skill_state_callbacks_bound(),
              "CSSkill projection could not be unbound after leaving state 6");
        {
            // A direct _SetState (F_ApplyResult special) leaves a running CSSkill for a
            // combat reaction state through CSSkill::OnBlur; the timer gate bits of events
            // 0x2a-0x2c are cleared by RaiseStateEvent whichever state is current.
            SkillMachineBridgeFixture combat_bridge;
            CoordinatorBindings combat_base{};
            combat_base.context=&combat_bridge;
            combat_base.facts=SkillMachineBridgeFixture::read_facts;
            combat_base.services={&combat_bridge,SkillMachineBridgeFixture::state_service};
            combat_base.combat_facts=[](void*){
                CombatFacts combat{};combat.controller_present=1;combat.last_target_available=1;
                return combat;};
            combat_bridge.character.bind(combat_base);
            dh2::character_ai_skill_machine_projection_v1::Projection combat_machine(
                combat_bridge.character,combat_bridge.character.owner(),0x300000012ull);
            std::string combat_error;
            const Frozen::Services combat_callbacks{&combat_bridge,
                                                    SkillMachineBridgeFixture::skill_service};
            check(combat_machine.bind_skill_state_callbacks(0x300000013ull,0x300000014ull,
                      &combat_bridge.ooi_intent,&combat_bridge.physical,combat_callbacks,
                      combat_error),"combat CSSkill projection did not bind");
            check(combat_bridge.character.transition(3)==1&&
                  combat_bridge.character.event(0xc355)==1&&
                  combat_bridge.character.state.current==6,"combat bridge did not enter CSSkill");
            combat_bridge.character.state.attack_gate|=0x6u;
            check(combat_bridge.character.event(0x2b)==0&&
                  combat_bridge.character.state.current==6&&
                  (combat_bridge.character.state.attack_gate&0x6u)==0x4u,
                  "event 0x2b did not clear the stun gate bit while CSSkill was current");
            combat_bridge.callback_events.clear();
            check(combat_bridge.character.transition(10,50011,0x77)==1&&
                  combat_bridge.character.state.current==10&&
                  combat_bridge.character.state.flags==9025&&
                  combat_bridge.character.state.controller_locked==1&&
                  combat_bridge.callback_events==std::vector<std::uint32_t>({0x1f}),
                  "direct _SetState did not leave CSSkill through OnBlur into CSKnockedBack");
            check(combat_bridge.character.event(34)==1&&combat_bridge.character.state.current==3&&
                  combat_bridge.character.state.controller_locked==0,
                  "CSKnockedBack did not return to Idle on the sequence end");
            check(combat_machine.unbind_skill_state_callbacks(combat_error),
                  "combat CSSkill projection could not unbind after leaving state 6");
        }

        Fixture movement;
        check(movement.character.transition(3) == 1 &&
              movement.character.state.current_animation == 11, "Idle focus failed");
        movement.character.state.attack_gate = 7;
        movement.character.state.heading_active = 1;
        movement.nested_gate_event = true;
        check(movement.character.event(0xc351) == 1 &&
              movement.character.state.current == 4 &&
              movement.character.state.current_animation == 22 &&
              movement.character.state.move_type == 1 &&
              movement.character.state.cached_speed == 1.3f &&
              movement.character.state.attack_gate == 3 &&
              movement.character.event_cause() == 0, "source Move/reentry failed");
        movement.nested_gate_event = false;
        movement.change_idle_on_stop = true;
        movement.character.state.heading_active = 0;
        check(movement.character.update_state(16) == 1 &&
              movement.character.state.current == 3 &&
              movement.character.state.current_animation == 99,
              "Stop producer refresh was not consumed by nested Idle focus");

        Fixture timers;
        timers.character.state.current = 5;
        timers.character.state.attack_gate = 7;
        timers.mutate_timer_event = true;
        check(timers.character.start_timer(10, 0, 0x2a, 0x12345678) == 0,
              "source timer allocation failed");
        check(timers.character.update_timers(10, 1) == 1 &&
              timers.character.timers().slots[0].elapsed_ms == 0 &&
              timers.timer_order.empty(), "script blocking changed timer clock");
        check(timers.character.pause_timer(0, 1) == 1 &&
              timers.character.update_timers(10, 0) == 1 && timers.timer_order.empty(),
              "paused timer expired");
        check(timers.character.pause_timer(0, 0) == 1 &&
              timers.character.update_timers(10, 0) == 1 &&
              timers.timer_order == std::vector<std::uint32_t>({1, 2}) &&
              timers.character.state.attack_gate == 6 &&
              timers.character.timers().update_depth == 0,
              "expiry routing did not use captured source event");

        Fixture growth;
        growth.character.state.current = 5;
        growth.character.state.attack_gate = 7;
        growth.test_growth_inside_expiry = true;
        check(growth.character.start_timer(10, -1, 0x2a, 0) == 0 &&
              growth.character.update_timers(10, 0) == 1,
              "borrowed timer growth gate failed");
        check(growth.character.start_timer(20, 0, 0x2a, 0) == 0 &&
              growth.character.start_timer(20, 0, 0x2a, 0) == 1 &&
              growth.character.timers().count == 2 &&
              growth.character.timers().capacity >= 2,
              "free-slot reuse or safe timer growth failed");

        Fixture failure;
        failure.throw_animation = true;
        rejected = false;
        try { failure.character.transition(3, 0xc351); }
        catch (const std::runtime_error&) { rejected = true; }
        check(rejected && failure.character.event_cause() == 0,
              "service failure did not restore event scope");
        const auto reads = failure.facts_reads;
        failure.character.refresh_facts();
        check(failure.facts_reads == reads, "failed dispatch retained dangling facts");
        failure.throw_animation = false;
        failure.character.state.current = 5;
        failure.throw_expiry = true;
        check(failure.character.start_timer(10, 0, 0x2a, 0) == 0,
              "failure timer could not start");
        rejected = false;
        try { failure.character.update_timers(10, 0); }
        catch (const std::runtime_error&) { rejected = true; }
        check(rejected && failure.character.timers().update_depth == 0,
              "expiry exception leaked timer borrow");
        failure.character.reset_timers(2);
        check(failure.character.owner() == 2 && !failure.character.timers().count,
              "timer reset failed after recovered borrow");

        Fixture spawning;
        spawning.facts.is_player=0;
        spawning.spawn.spawn_animation=88;
        spawning.spawn.visual_present=1;
        spawning.spawn.raw_fade_in_argument=3000;
        spawning.spawn.can_respawn=1;
        spawning.character.bind({&spawning,Fixture::read_facts,{&spawning,Fixture::service},
            nullptr,nullptr,[](void* raw){return static_cast<Fixture*>(raw)->spawn;}});
        check(spawning.character.spawn_transition(0)==1&&spawning.character.state.flags==0,
              "source Limbus focus failed");
        check(spawning.character.update_state(999999)==0&&spawning.character.state.current==0,
              "Limbus invented timed transition");
        check(spawning.character.start_timer(10,0,0x2f,0)==0&&
              spawning.character.update_timers(10,0)==1&&spawning.character.state.current==1&&
              spawning.character.state.current_animation==88,
              "captured respawn timer did not reach source Spawn");
        check(spawning.character.spawn_event(0x28,"is_interactive_extra")==0&&
              spawning.character.spawn_event(0x28,"is_interactive")==1&&
              spawning.character.state.body_present==1&&
              (spawning.character.state.flags&0x2000u),"named Spawn event differs");
        check(spawning.character.event(0x22)==1&&spawning.character.state.current==3&&
              spawning.character.state.current_animation==11,
              "finite Spawn completion did not enter source Idle");

        SkillFixture skills;
        check(skills.character.transition(3)==1,"skill fixture Idle setup failed");
        skills.character.state.elapsed_ms=123;
        check(skills.character.event(0xc355)==1&&skills.character.state.current==6&&
              skills.character.state.elapsed_ms==0&&
              !skills.skill_operations.empty()&&
              skills.skill_operations.front()==Frozen::Operation::debug_load,
              "C355 did not enter CSSkill through the same Coordinator");
        check(skills.character.update_state(17)==1&&
              skills.character.state.current==6&&skills.character.state.elapsed_ms==17,
              "CSSkill update did not preserve Coordinator elapsed ownership");
        check(skills.character.event(0x28,reinterpret_cast<std::uintptr_t>("is_stoppable"))==0&&
              (skills.character.state.flags&0x8000u),
              "CSSkill OnEvent28 source string flag was not applied");
        const auto operation_count=skills.skill_operations.size();
        check(skills.character.event(0xc351)==1&&skills.character.state.current==4&&
              skills.character.state.elapsed_ms==0&&
              skills.skill_operations.size()>operation_count&&
              skills.skill_operations[operation_count]==Frozen::Operation::debug_load&&
              skills.character.state.current_animation==22&&
              skills.previous_states==std::vector<std::int32_t>({-1,3,6}),
              "CSSkill blur/registered exit did not transition through Coordinator");

        SkillFixture projection_lifecycle(false);
        State foreign_machine{};
        auto wrong_machine_projection=projection_lifecycle.projection;
        wrong_machine_projection.machine=&foreign_machine;
        check(projection_lifecycle.character.bound()&&
              !projection_lifecycle.character.bind_skill_projection(
                  &wrong_machine_projection),
              "CSSkill projection for another FSM was accepted");
        auto replacement_projection=projection_lifecycle.projection;
        check(projection_lifecycle.character.bind_skill_projection(
                  &projection_lifecycle.projection)&&
              projection_lifecycle.character.bind_skill_projection(
                  &projection_lifecycle.projection)&&
              !projection_lifecycle.character.bind_skill_projection(
                  &replacement_projection)&&
              !projection_lifecycle.character.unbind_skill_projection(
                  &replacement_projection),
              "CSSkill projection binding replaced or mismatched its borrowed owner");
        check(projection_lifecycle.character.transition(3)==1&&
              projection_lifecycle.character.event(0xc355)==1&&
              projection_lifecycle.character.state.current==6&&
              !projection_lifecycle.character.unbind_skill_projection(
                  &projection_lifecycle.projection),
              "CSSkill projection detached while state 6 was active");
        check(projection_lifecycle.character.event(0x28,
                  reinterpret_cast<std::uintptr_t>("is_stoppable"))==0&&
              (projection_lifecycle.character.state.flags&0x8000u),
              "CSSkill projection fixture did not enable the source stoppable gate");
        check(projection_lifecycle.character.event(0xc351)==1&&
              projection_lifecycle.character.state.current==4,
              "CSSkill source Move event did not leave state 6");
        check(projection_lifecycle.character.unbind_skill_projection(
                  &projection_lifecycle.projection),
              "CSSkill projection could not be unbound after leaving state 6");
        check(projection_lifecycle.character.event(0xc355)==-1&&
              projection_lifecycle.character.state.current==4,
              "unbound CSSkill event reached the former borrowed projection");

        skills.character.state.elapsed_ms=71;
        const auto reenter_skill=skills.character.event(0xc355);
        const auto close_skill=skills.character.event(0x22);
        check(reenter_skill==1&&skills.character.state.current==3&&
              close_skill==1&&skills.character.state.current==3&&
              skills.character.state.elapsed_ms==0&&
              skills.character.state.current_animation==11&&
              skills.previous_states==std::vector<std::int32_t>({-1,3,6,4,6}),
              "CSSkill playback close event0x22 did not return to Idle");

        Fixture no_skill_projection;
        no_skill_projection.character.state.current=3;
        check(no_skill_projection.character.event(0xc355)==-1&&
              no_skill_projection.character.state.current==3,
              "C355 without CSSkill callback ownership mutated state");

        {
            // CSAnim (state 14): SM_SetAnimState 0x3c1aa8, OnFocus 0x3c324c,
            // OnUpdate 0x3c1a3c, OnEvent 0x3c1a14, OnInit 0x3c8b28 registrations.
            AnimStateFixture anim;
            anim.character.state.current=3;anim.character.state.body_present=0;
            // Script_PlayActorAnim form (wait=false, idle-on-update=true).
            check(anim.character.set_anim_state(21,false,true)==1&&
                  anim.character.state.current==14&&anim.character.state.flags==0x2240&&
                  anim.character.state.animation_override==-1&&
                  anim.character.state.current_animation==21&&
                  anim.character.source_anim_idle_541()&&!anim.character.source_anim_wait_540(),
                  "SM_SetAnimState did not run CSAnim::OnFocus with the pending AnimTable row");
            check(anim.calls.size()>=2&&
                  anim.calls[anim.calls.size()-2].service==set_animation&&
                  anim.calls[anim.calls.size()-2].argument==21&&
                  anim.calls.back().service==raise_event&&anim.previous_states.back()==3,
                  "CSAnim::OnFocus service order differs from SM_SetAnim then RaiseEvent(29)");
            // Unscripted + idle-on-update: OnUpdate calls SM_SetIdleState(false).
            anim.character.state.elapsed_ms=40;
            check(anim.character.update_state(16)==1&&anim.character.state.current==3&&
                  anim.character.state.idle_suppressed==0&&anim.character.state.elapsed_ms==0&&
                  anim.character.state.current_animation==11,
                  "CSAnim::OnUpdate did not return an unscripted Character to Idle");
            // Scripted: OnUpdate is a no-op, and event 34 is ignored without the +0x44 byte.
            anim.character.set_source_scripted_1480(true);
            check(anim.character.set_anim_state(22,false,true)==1&&
                  anim.character.update_state(16)==1&&anim.character.state.current==14&&
                  anim.character.state.elapsed_ms==16&&
                  anim.character.event(34)==0&&anim.character.state.current==14,
                  "scripted CSAnim left the animation without its event");
            // 14 -> 14 re-runs OnFocus without resetting the state timer.
            check(anim.character.set_anim_state(23,false,true)==1&&
                  anim.character.state.current==14&&anim.character.state.elapsed_ms==16&&
                  anim.character.state.current_animation==23,
                  "SM_SetAnimState from CSAnim reset the state timer or skipped OnFocus");
            // Events CSAnim does not register are source no-ops (50008 dead, 50003 interact).
            check(anim.character.event(50008)==0&&anim.character.event(50003)==0&&
                  anim.character.raise_dead_state_event_50008(0x77)==0&&
                  anim.character.state.current==14,
                  "CSAnim accepted an event it does not register");
            // Lua PlayAnim form (wait=true, idle-on-update=false): flag word 0x2340, event 34 -> Idle.
            check(anim.character.set_anim_state(15,true,false)==1&&
                  anim.character.state.flags==0x2340&&anim.character.state.current==14,
                  "CSAnim::OnFocus flag word for the PlayAnim form differs");
            anim.character.set_source_scripted_1480(false);
            check(anim.character.update_state(16)==1&&anim.character.state.current==14,
                  "CSAnim idled without the +0x45 byte");
            check(anim.character.event(34)==1&&anim.character.state.current==3&&
                  anim.character.state.idle_suppressed==0,
                  "CSAnim::OnEvent(34) did not call SM_SetIdleState(false)");
            // OnInit registrations: 50001 -> CSMove.
            check(anim.character.set_anim_state(15,false,false)==1&&
                  anim.character.event(34)==0&&anim.character.state.current==14&&
                  anim.character.event(0xc351)==1&&anim.character.state.current==4&&
                  anim.character.state.flags==0x23c1&&anim.character.state.current_animation==22,
                  "CSAnim event 50001 did not enter CSMove");
            // 50005 -> CSSkill needs the skill callback projection; without it the event is refused.
            check(anim.character.set_anim_state(15,false,false)==1&&
                  anim.character.event(0xc355)==-1&&anim.character.state.current==14,
                  "CSAnim event 50005 without CSSkill ownership mutated state");
            // A Character awaiting spawn has no CSAnim edge in this kernel.
            AnimStateFixture limbus;limbus.character.state.current=17;
            check(limbus.character.set_anim_state(21,false,true)==-1&&
                  limbus.character.state.current==17,
                  "SM_SetAnimState from an unattached source state mutated the FSM");
            // Script_PlayActorAnim on a cutscene actor that is still in CSSpawn / CSLimbus:
            // the outgoing state's source OnBlur runs before CSAnim::OnFocus.
            Fixture awaiting;
            awaiting.facts.is_player=0;awaiting.spawn.spawn_animation=88;
            awaiting.spawn.visual_present=1;awaiting.spawn.raw_fade_in_argument=3000;
            awaiting.spawn.can_respawn=1;
            awaiting.character.bind({&awaiting,Fixture::read_facts,{&awaiting,Fixture::service},
                nullptr,nullptr,[](void* raw){return static_cast<Fixture*>(raw)->spawn;}});
            check(awaiting.character.spawn_transition(0)==1&&awaiting.character.spawn_transition(1)==1&&
                  awaiting.character.state.current==1&&awaiting.character.state.body_present==0,
                  "spawn fixture did not reach CSSpawn");
            check(awaiting.character.set_anim_state(21,false,true)==1&&
                  awaiting.character.state.current==14&&awaiting.character.state.flags==0x2240&&
                  awaiting.character.state.body_present==1&&
                  awaiting.character.state.current_animation==21&&
                  awaiting.character.event(0x22)==0&&awaiting.character.state.current==14,
                  "CSSpawn -> CSAnim did not run CSSpawn::OnBlur then CSAnim::OnFocus");
            // Script_PutCharacterInLimbus on that actor while CSAnim plays (SM_SetLimbusState
            // -> _SetState(0)): CSAnim::OnBlur is empty, CSLimbus::OnFocus clears the flags.
            check(awaiting.character.transition(0,-1,0)==1&&awaiting.character.state.current==0&&
                  awaiting.character.state.flags==0,
                  "CSAnim -> CSLimbus did not run CSLimbus::OnFocus");
            // CSSkill (6) <-> CSAnim (14) through the same skill callback projection.
            SkillFixture skill;
            check(skill.character.transition(3)==1&&skill.character.event(0xc355)==1&&
                  skill.character.state.current==6,"CSSkill setup for CSAnim failed");
            const auto skill_ops_before=skill.skill_operations.size();
            check(skill.character.set_anim_state(16,false,false)==1&&
                  skill.character.state.current==14&&skill.character.state.flags==0x2340&&
                  skill.character.state.current_animation==16&&
                  skill.skill_operations.size()>skill_ops_before,
                  "SM_SetAnimState from CSSkill did not run CSSkill::OnBlur then CSAnim::OnFocus");
            const auto skill_ops_mid=skill.skill_operations.size();
            check(skill.character.event(0xc355)==1&&skill.character.state.current==6&&
                  skill.skill_operations.size()>skill_ops_mid&&
                  skill.previous_states==std::vector<std::int32_t>({-1,3,6,14}),
                  "CSAnim event 50005 did not re-enter CSSkill through its callback projection");
            // Death: a forced SM_SetDeadState transition leaves CSAnim straight to CSDead.
            AnimStateFixture dying;dying.character.state.current=3;
            check(dying.character.set_anim_state(21,false,true)==1&&
                  dying.character.transition(12,50008,0x99)==1&&
                  dying.character.state.current==12,
                  "forced CSDead transition from CSAnim failed");
        }

        std::printf("{\"source_state_move_idle\":true,\"synchronous_reentry\":true,"
                    "\"facts_refresh\":true,\"captured_expiry_event\":true,"
                    "\"expiry_hook_before_state\":true,\"script_blocking_and_pause\":true,"
                    "\"spawn_timer_and_named_event\":true,"
                    "\"safe_growth\":true,\"exception_borrow_cleanup\":true,"
                    "\"csskill_c355_focus_blur_event_and_transition\":true,"
                    "\"csskill_machine_projection_bind_and_callback_order\":true,"
                    "\"csskill_projection_lifecycle\":true,"
                    "\"csskill_borrows_coordinator_machine_state\":true,"
                    "\"char_ai_frame_uses_same_coordinator_and_fails_closed\":true,"
                    "\"character_logic_frame_orders_timer_ai_state\":true,"
                    "\"player_char_ai_target_prefix_fails_closed\":true,"
                    "\"player_char_ai_master_null_branch\":true,"
                    "\"player_is_zonable_source_false\":true,"
                    "\"world_object_character_identity_lifecycle\":true,"
                    "\"player_persist_identity_event_projection\":true,"
                    "\"character_physical_collision_state_gate\":true,"
                    "\"csanim_state14_focus_update_event_and_transitions\":true}\n");
        return 0;
    } catch (const std::exception& error) {
        std::fprintf(stderr, "character coordinator: %s\n", error.what());
        return 1;
    }
}
