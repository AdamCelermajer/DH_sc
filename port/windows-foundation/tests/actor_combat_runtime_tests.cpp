#include "../actor_combat_runtime.hpp"
#include <cmath>
#include <iostream>
#include <limits>
#include <stdexcept>

using namespace dh::foundation;
namespace {
void check(bool value, const std::string& message) { if (!value) throw std::runtime_error(message); }
struct TestWorld final : CombatWorld {
    std::map<ActorId, ActorState> actors;
    float damage = 3;
    bool available = true;
    mutable unsigned eligibility_queries=0;
    mutable unsigned damage_queries=0;
    std::optional<std::uint32_t> outcomes=std::uint32_t(0x10);
    std::optional<std::uint32_t> source_mask=std::uint32_t(0x22aab5);
    ActorState* find_actor(ActorId id) override {
        auto it = actors.find(id); return it == actors.end() ? nullptr : &it->second;
    }
    bool eligible_target(const ActorState&, const ActorState&) const override { ++eligibility_queries;return true; }
    float target_radius(const ActorState&) const override { return 0; }
    bool resolve_damage(const std::string&, const ActorState&, const ActorState&,
                        const std::string&, float& amount, std::string&) const override {
        ++damage_queries;amount = damage; return available;
    }
    bool resolve_damage_with_outcomes(const std::string& source,const ActorState& a,const ActorState& b,
                        const std::string& marker,float& amount,
                        std::optional<std::uint32_t>& result,
                        std::optional<std::uint32_t>& mask,std::string& error) const override {
        result=outcomes;mask=source_mask;return resolve_damage(source,a,b,marker,amount,error);
    }
};
struct Visual {
    std::map<std::string, AnimationMarkers> clips;
    std::string selected;
    double elapsed = 0;
    int updates = 0;
    void add(const std::string& name, std::int32_t start, std::int32_t end, std::int32_t hit) {
        std::string error;
        const char* event[] = {"attack_mainhand"};
        dh2::animation::EventGroup group[] = {{1, event}};
        dh2::animation::EventView view{4, 1, reinterpret_cast<const std::uint8_t*>(&hit), group};
        check(clips[name].load(view, start, end, error), error);
    }
    CombatVisualBinding binding() {
        return {
            [this](const std::string& name, bool loop, std::string& error) {
                if (loop || !clips.count(name)) { error = "Unknown mock clip"; return false; }
                selected = name; elapsed = 0; return true;
            },
            [this](const std::string& name, std::int32_t& start, std::int32_t& end, std::string&) {
                auto it = clips.find(name); if (it == clips.end()) return false;
                start = it->second.start_ms(); end = it->second.end_ms(); return true;
            },
            [this](const std::string& name, std::string&) -> const AnimationMarkers* {
                auto it = clips.find(name); return it == clips.end() ? nullptr : &it->second;
            },
            [this](double seconds, std::string&) { elapsed += seconds; ++updates; return true; }
        };
    }
};
ActorState actor(ActorId id) {
    ActorState a; a.id = id; a.definition_id = "test-definition";
    a.health = a.max_health = 10; a.attack_ids = {"test-attack"}; return a;
}
AttackDefinition attack() {
    AttackDefinition d; d.id = "test-attack"; d.animation_clip_id = "attack";
    d.maximum_range = 2; d.damage_markers = {{"attack_mainhand", "test-formula"}}; return d;
}
void nullable_melee_swing() {
    TestWorld world;world.actors[1]=actor(1);world.actors[2]=actor(2);
    world.actors[2].transform.position={1,0,0};
    Visual visual;visual.add("attack",0,200,50);
    CombatSystem combat(world);ActorCombatRuntime runtime(combat);std::string error;
    std::int32_t state=3;unsigned attack_focus=0,completion=0;
    CombatPoseBindings poses;
    poses.transition_state=[&](std::int32_t& out,std::string&){out=state;return true;};
    poses.transition=[&](const CombatRuntimeTransition& t,std::string&){
        if(t.stage==CombatRuntimeTransitionStage::after_change){
            state=t.to_state;
            if(t.cause==CombatRuntimeTransitionCause::attack)++attack_focus;
            if(t.cause==CombatRuntimeTransitionCause::completion)++completion;
        }return true;
    };
    check(runtime.bind(world.actors[1],visual.binding(),poses,error),error);
    auto definition=attack();definition.cooldown_seconds=.5;
    definition.cooldown_timing=CooldownTiming::attack_departure;
    check(runtime.begin(1,invalid_actor_id,definition,error),error);
    check(state==5&&attack_focus==1&&runtime.owns_pose(1)&&combat.active_attack_valid(1)&&
        !combat.active_target_valid(1)&&world.actors[1].target_id==invalid_actor_id,
        "Null melee did not focus Attack5 without a fabricated target");
    check(!runtime.begin(1,invalid_actor_id,definition,error),"Duplicate null swing was accepted");
    std::vector<DamageEvent> events;
    check(runtime.update(.05,events,error),error);
    check(events.empty()&&world.damage_queries==0&&world.eligibility_queries==0&&
        world.actors[2].health==10&&runtime.owns_pose(1)&&state==5,
        "Null hand event queried damage/RNG or interrupted authored swing");
    check(runtime.update(.15,events,error),error);
    check(state==3&&completion==1&&!runtime.owns_pose(1)&&!combat.attacking(1)&&
        world.actors[1].action==CharacterAction::idle&&std::abs(combat.cooldown_remaining(1)-.5)<1e-10,
        "Null swing failed authored End/Idle/departure cooldown");
    check(!runtime.begin(1,invalid_actor_id,definition,error),"Null swing bypassed cooldown");
    check(runtime.update(.5,events,error),error);
    auto ranged=definition;ranged.geometry=AttackGeometry::ranged_band;
    check(!runtime.begin(1,invalid_actor_id,ranged,error),"Null ranged attack was accepted");
    world.actors[1].health=0;
    check(!runtime.begin(1,invalid_actor_id,definition,error),"Dead owner started null swing");
    world.actors[1].health=10;world.actors[1].action=CharacterAction::hurt;
    check(!runtime.begin(1,invalid_actor_id,definition,error),"Hurt owner started null swing");
    world.actors[1].action=CharacterAction::idle;world.actors[1].attack_ids.clear();
    check(!runtime.begin(1,invalid_actor_id,definition,error),"Unbound owner started null swing");
    world.actors[1].attack_ids={definition.id};world.actors[2].health=0;
    check(!runtime.begin(1,2,definition,error),"Dead target was treated as null");
    world.actors[2].health=10;world.actors[2].transform.position={10,0,0};
    check(!runtime.begin(1,2,definition,error),"Far target bypassed range");
    world.actors[2].transform.position={1,0,0};
    check(runtime.begin(1,2,definition,error),error);
    check(runtime.update(.05,events,error)&&events.size()==1&&world.actors[2].health==7&&
        world.damage_queries==1,"Live-target hand marker no longer applies real damage");
    runtime.clear();
}
void invalidated_nullable_melee_swing() {
    for(int mode=0;mode<3;++mode){
        TestWorld world;world.actors[1]=actor(1);world.actors[2]=actor(2);
        Visual visual;visual.add("attack",0,200,50);visual.add("death",0,100,1000);
        CombatSystem combat(world);ActorCombatRuntime runtime(combat);std::string error;
        std::int32_t state=3;CombatPoseBindings poses;poses.death_clip_id="death";
        poses.transition_state=[&](std::int32_t& out,std::string&){out=state;return true;};
        poses.transition=[&](const CombatRuntimeTransition& t,std::string&){
            if(t.stage==CombatRuntimeTransitionStage::after_change)state=t.to_state;return true;
        };
        check(runtime.bind(world.actors[1],visual.binding(),poses,error),error);
        check(runtime.begin(1,invalid_actor_id,attack(),error),error);
        if(mode==0)world.actors[1].health=0;
        if(mode==1)world.actors[1].target_id=2;
        if(mode==2)world.actors[1].action=CharacterAction::moving;
        check(!combat.active_attack_valid(1),"Invalidated null run retained admission validity");
        // Direct duplicate/event delivery must run cleanup before null no-op.
        MarkerOccurrence marker;marker.generation=1;marker.marker.name="attack_mainhand";
        DamageEvent event;check(combat.consume_marker(1,marker,event,error),error);
        check(!combat.attacking(1)&&!event.applied&&world.damage_queries==0,
            "Dead/retargeted/inactive null run survived direct marker delivery");
        std::vector<DamageEvent> events;check(runtime.update(.05,events,error),error);
        check(events.empty()&&world.damage_queries==0&&
            (mode==0?state==12:state==3&&!runtime.owns_pose(1)),
            "Invalidated null pose failed death/interruption transition");
        runtime.clear();
    }
}
void departure_timing() {
    for (int scenario=0; scenario<5; ++scenario) {
        TestWorld world; world.actors[1]=actor(1); world.actors[2]=actor(2);
        world.actors[2].transform.position={1,0,0};
        Visual first, second;
        for (auto* v : {&first,&second}) { v->add("attack",100,300,150); v->add("react",0,100,1000); v->add("death",0,200,1000); }
        CombatSystem combat(world); ActorCombatRuntime runtime(combat);
        std::string error; std::vector<DamageEvent> events;
        CombatPoseBindings rates; if (scenario==1) rates.clip_rates["attack"]=2;
        check(runtime.bind(world.actors[1],first.binding(),rates,error),error);
        check(runtime.bind(world.actors[2],second.binding(),{"react","death",{}},error),error);
        auto definition=attack(); definition.cooldown_seconds=.5; definition.cooldown_timing=CooldownTiming::attack_departure;
        check(runtime.begin(1,2,definition,error),error);
        check(combat.cooldown_remaining(1)==0,"Runtime started departure timer on entry");
        if (scenario==0 || scenario==1) {
            check(runtime.update(.3,events,error),error);
            const double expected = scenario==0 ? .4 : .3;
            check(std::abs(combat.cooldown_remaining(1)-expected)<1e-10,"Clip-end frame remainder used wrong delay clock");
        } else if (scenario==2) {
            check(runtime.update(.02,events,error),error);
            world.actors[1].action=CharacterAction::moving;
            check(runtime.update(.1,events,error),error);
            check(std::abs(combat.cooldown_remaining(1)-.4)<1e-10,"External interruption missed full frame remainder");
        } else if (scenario==3) {
            check(runtime.begin(2,1,definition,error),error);
            check(runtime.update(.2,events,error),error);
            check(events.size()==1 && std::abs(combat.cooldown_remaining(2)-.35)<1e-10,
                  "Damage reaction departure used incorrect remainder");
        } else {
            world.damage=100;
            check(runtime.begin(2,1,definition,error),error);
            check(runtime.update(.2,events,error),error);
            check(events.size()==1 && events[0].target_died && combat.cooldown_remaining(2)==0,
                  "Death retained gate or stale pending hit");
            check(std::abs(combat.cooldown_remaining(1)-.35)<1e-10,
                  "Target-death departure used incorrect remaining wall time");
        }
    }
}
void source_clock_timing() {
    TestWorld world;world.actors[1]=actor(1);world.actors[2]=actor(2);world.actors[2].transform.position={1,0,0};
    Visual visual;visual.add("attack",100,300,150);
    CombatSystem combat(world);ActorCombatRuntime runtime(combat);std::string error;
    auto binding=visual.binding();bool delivered=false;
    binding.source_clock=[](){return true;};
    binding.take_source_events=[&](std::vector<SourceCombatMarker>& out,std::string&){out.clear();if(visual.elapsed>=.25&&!delivered){out.push_back({0,"attack_mainhand"});delivered=true;}return true;};
    binding.source_finished=[&](){return visual.elapsed>=.4;};
    auto incomplete=binding;incomplete.source_finished={};
    check(!runtime.bind(world.actors[1],incomplete,{},error),"Partial source clock accepted");
    CombatPoseBindings poses;poses.clip_rates["attack"]=9;
    check(runtime.bind(world.actors[1],binding,poses,error),error);
    auto definition=attack();definition.cooldown_seconds=.5;definition.cooldown_timing=CooldownTiming::attack_departure;
    check(runtime.begin(1,2,definition,error),error);std::vector<DamageEvent> events;
    check(runtime.update(.2,events,error)&&events.empty()&&runtime.owns_pose(1),"Validation timeline replaced actual source clock");
    check(runtime.update(.06,events,error)&&events.size()==1&&world.actors[2].health==7&&runtime.owns_pose(1),"Actual source event did not drive damage");
    check(std::abs(visual.elapsed-.26)<1e-10,"Source wall clock was scaled twice");
    check(runtime.update(.15,events,error)&&events.empty()&&!runtime.owns_pose(1),"Actual source completion did not release attack");
    check(std::abs(combat.cooldown_remaining(1)-.5)<1e-10,"Departure delay did not start at actual source callback frame");
}
void restored_terminal_death() {
    TestWorld world;world.actors[1]=actor(1);world.actors[1].health=0;world.actors[1].action=CharacterAction::dead;
    world.actors[1].transform.position={12,34,56};Visual visual;visual.add("death",100,300,150);
    CombatSystem combat(world);ActorCombatRuntime runtime(combat);std::string error;unsigned holds=0;
    auto binding=visual.binding();binding.source_clock=[](){return true;};
    binding.take_source_events=[](std::vector<SourceCombatMarker>& output,std::string&){output.clear();return true;};
    binding.source_finished=[](){return false;};
    binding.hold_terminal=[&](const std::string& clip,std::string&){check(clip=="death","Wrong restored terminal clip");++holds;visual.elapsed=.2;return true;};
    CombatPoseBindings poses;poses.death_clip_id="death";poses.restore_dead_terminal=true;
    check(runtime.bind(world.actors[1],binding,poses,error),error);std::vector<DamageEvent> events;
    check(runtime.owns_pose(1)&&holds==1,"Restored corpse did not acquire terminal pose");
    check(runtime.update(2,events,error)&&visual.updates==0&&world.actors[1].transform.position==std::array<float,3>{12,34,56},"Restored death replayed visual displacement");
    world.actors[1].health=10;world.actors[1].action=CharacterAction::idle;
    check(runtime.update(0,events,error)&&!runtime.owns_pose(1),"Revived corpse retained terminal hold");
}
void synchronous_source_delivery() {
    for(int scenario=0;scenario<5;++scenario) {
        TestWorld world;world.actors[1]=actor(1);world.actors[2]=actor(2);
        world.actors[2].transform.position={1,0,0};
        Visual first,second;first.add("attack",100,300,150);
        second.add("react",0,100,1000);second.add("death",0,200,1000);
        CombatSystem combat(world);ActorCombatRuntime runtime(combat);
        std::string error;std::vector<DamageEvent> events;bool forwarded=false;
        auto binding=first.binding();binding.source_clock=[](){return true;};
        binding.source_finished=[&](){return scenario==2;};
        binding.take_source_events=[](std::vector<SourceCombatMarker>&,std::string&)->bool {
            throw std::runtime_error("Synchronous source also drained legacy event queue");
        };
        binding.update_source=[&](double seconds,const SourceCombatMarkerSink& sink,std::string& e) {
            first.elapsed+=seconds;++first.updates;
            check(sink({0,"attack_mainhand"},e),e);
            // Models helper return followed by source state-event forwarding:
            // shared HP, action and pose must already be published here.
            check(world.actors[2].health==7&&world.actors[2].action==CharacterAction::hurt
                &&second.selected=="react"&&events.size()==1,"Source state callback observed deferred damage");
            forwarded=true;
            if(scenario==1||scenario==2) {
                check(runtime.interrupt(1),"Source callback could not interrupt attack");
                if(scenario==2)check(runtime.begin(1,2,attack(),e),e);
                check(sink({1,"attack_mainhand"},e)&&world.actors[2].health==7,
                    "Stale source callback damaged after interruption/replacement");
            } else if(scenario==3) {
                world.available=false;
                // Even a host which accidentally ignores the sink failure must
                // not hide the runtime's incremental error.
                check(!sink({1,"attack_mainhand"},e),"Unavailable synchronous provider succeeded");
                e.clear();return true;
            } else if(scenario==4) {
                check(!sink({1,"attack_mainhand"},e),"Empty-error reaction selection failure succeeded");
                e.clear();return true;
            } else check(sink({0,"attack_mainhand"},e)&&events.size()==1,"Duplicate source marker applied twice");
            return true;
        };
        check(runtime.bind(world.actors[1],binding,{},error),error);
        auto target_binding=second.binding();int reaction_selections=0;
        const auto select_target=target_binding.select;
        target_binding.select=[&,select_target](const std::string& name,bool loop,std::string& e) {
            if(scenario==4&&name=="react"&&++reaction_selections==2){e.clear();return false;}
            return select_target(name,loop,e);
        };
        check(runtime.bind(world.actors[2],target_binding,{"react","death",{}},error),error);
        check(runtime.begin(1,2,attack(),error),error);
        const bool success=runtime.update(.06,events,error);
        check(forwarded&&events.size()==(scenario==4?2u:1u)&&world.actors[2].health==(scenario==4?4:7),"Synchronous source delivery lost incremental hit");
        if(scenario==3||scenario==4)check(!success&&!error.empty()&&!runtime.owns_pose(1),"Ignored sink failure was hidden");
        else check(success,error);
        if(scenario==1)check(!runtime.owns_pose(1),"Interrupted source attack retained ownership");
        if(scenario==2)check(runtime.owns_pose(1)&&combat.attacking(1)&&world.actors[1].action_elapsed_seconds==0,
            "Old source completion or elapsed overwrote replacement attack");
    }
}
void source_outcome_reactions() {
    for(const std::uint32_t outcome:{std::uint32_t(1),std::uint32_t(2),std::uint32_t(4),std::uint32_t(0)}) {
        TestWorld world;world.actors[1]=actor(1);world.actors[2]=actor(2);
        world.actors[2].transform.position={1,0,0};world.outcomes=outcome;
        if(outcome==1||outcome==2)world.damage=0;
        Visual first,second;
        for(auto* v:{&first,&second}){v->add("attack",100,300,150);v->add("react",0,100,1000);}
        CombatSystem combat(world);ActorCombatRuntime runtime(combat);std::string error;
        check(runtime.bind(world.actors[1],first.binding(),{},error),error);
        check(runtime.bind(world.actors[2],second.binding(),{"react",{},{}},error),error);
        auto timedAttack=attack();timedAttack.cooldown_seconds=.5;timedAttack.cooldown_timing=CooldownTiming::attack_departure;
        check(runtime.begin(1,2,timedAttack,error)&&runtime.begin(2,1,timedAttack,error),error);
        std::vector<DamageEvent> events;
        check(runtime.update(.06,events,error),error);
        check(combat.attacking(1)&&combat.attacking(2)
              &&world.actors[1].target_id==2&&world.actors[2].target_id==1,
              "Miss, dodge, or ordinary damage interrupted an active attack/target");
        check(runtime.owns_pose(1)&&runtime.owns_pose(2)
              &&first.selected=="attack"&&second.selected=="attack",
              "Miss, dodge, or ordinary damage replaced an attack cursor");
        check(combat.cooldown_remaining(1)==0&&combat.cooldown_remaining(2)==0
              &&std::abs(first.elapsed-.06)<1e-10&&std::abs(second.elapsed-.06)<1e-10,
              "Miss/dodge/block/ordinary damage changed attack timing or cooldown");
        if(outcome==0||outcome==4)check(world.actors[1].health<10&&world.actors[2].health<10,
            "Positive ordinary damage did not apply");
        else check(world.actors[1].health==10&&world.actors[2].health==10,
            "Miss/dodge changed health");
    }
    TestWorld world;world.actors[1]=actor(1);world.actors[2]=actor(2);
    world.actors[2].transform.position={1,0,0};world.outcomes=0x10;
    Visual first,second;
    for(auto* v:{&first,&second}){v->add("attack",100,300,150);v->add("react",0,100,1000);}
    CombatSystem combat(world);ActorCombatRuntime runtime(combat);std::string error;
    check(runtime.bind(world.actors[1],first.binding(),{},error),error);
    unsigned rootTakes=0,rootApplies=0;Vec3 delivered{};bool deliveredMoveGo=false;
    auto victimVisual=second.binding();
    victimVisual.take_root_motion=[&](){++rootTakes;return Vec3{1,2,3};};
    victimVisual.apply_motion=[&](Vec3 delta,bool moveGo,std::string&){++rootApplies;delivered=delta;deliveredMoveGo=moveGo;return true;};
    CombatPoseBindings victimPoses;victimPoses.react_clip_id="react";victimPoses.react_move_go=true;
    check(runtime.bind(world.actors[2],victimVisual,victimPoses,error),error);
    auto timedAttack=attack();timedAttack.cooldown_seconds=.5;timedAttack.cooldown_timing=CooldownTiming::attack_departure;
    check(runtime.begin(1,2,timedAttack,error)&&runtime.begin(2,1,timedAttack,error),error);
    std::vector<DamageEvent> events;check(runtime.update(.06,events,error),error);
    check(events.size()==1&&events[0].source_outcomes&&*events[0].source_outcomes==0x10,
          "Explicit source Injure result was not carried to the hit event");
    check(events[0].source_mask&&*events[0].source_mask==0x22aab5u,
          "Marker hit truncated or lost the original 32-bit source mask");
    check(!combat.attacking(2)&&world.actors[2].target_id==invalid_actor_id
          &&runtime.owns_pose(2)&&second.selected=="react"
          &&world.actors[2].action==CharacterAction::hurt,
          "Explicit source Injure did not start the configured reaction");
    check(std::abs(combat.cooldown_remaining(2)-.49)<1e-10,
          "Explicit Injure did not start the victim's departure cooldown at interruption");
    check(rootTakes==1&&rootApplies==1&&deliveredMoveGo
          &&delivered.x==1&&delivered.y==2&&delivered.z==3,
          "Direct Injure pose did not drain/apply its same-sample root motion once");
    TestWorld unknown;unknown.actors[1]=actor(1);unknown.actors[2]=actor(2);
    unknown.actors[2].transform.position={1,0,0};unknown.outcomes.reset();Visual attacker,defender;
    attacker.add("attack",100,300,150);defender.add("react",0,100,1000);
    CombatSystem unknown_combat(unknown);ActorCombatRuntime unknown_runtime(unknown_combat);
    check(unknown_runtime.bind(unknown.actors[1],attacker.binding(),{},error),error);
    check(unknown_runtime.bind(unknown.actors[2],defender.binding(),{"react",{},{}},error),error);
    check(unknown_runtime.begin(1,2,attack(),error),error);
    check(unknown_runtime.update(.06,events,error),error);
    check(events.size()==1&&!events[0].source_outcomes&&!unknown_runtime.owns_pose(2)
          &&defender.selected.empty()&&unknown.actors[2].action==CharacterAction::idle,
          "Outcome-unknown legacy damage invented an Injure reaction");
}
void source_injury_gate() {
    TestWorld world;world.actors[1]=actor(1);world.actors[2]=actor(2);
    world.actors[2].transform.position={1,0,0};
    Visual attacker,defender;attacker.add("short",0,80,50);
    defender.add("long",0,10000,5000);defender.add("react",0,100,1000);
    CombatSystem combat(world);ActorCombatRuntime runtime(combat);std::string error;
    check(runtime.bind(world.actors[1],attacker.binding(),{},error),error);
    auto victimVisual=defender.binding();
    CombatPoseBindings poses;poses.react_clip_id="react";poses.source_injury_gate_enabled=true;
    check(runtime.bind(world.actors[2],victimVisual,poses,error),error);
    auto shortAttack=attack();shortAttack.animation_clip_id="short";
    auto longAttack=attack();longAttack.animation_clip_id="long";
    check(runtime.begin(1,2,shortAttack,error)&&runtime.begin(2,1,longAttack,error),error);
    std::vector<DamageEvent> events;check(runtime.update(.06,events,error),error);
    check(!combat.attacking(2)&&defender.selected=="react","Fresh source Injure was not admitted");
    check(runtime.update(.091,events,error),error);
    check(!runtime.owns_pose(2)&&world.actors[2].action==CharacterAction::idle,
          "Injure animation did not return the actor to an attackable state");

    check(runtime.begin(1,2,shortAttack,error)&&runtime.begin(2,1,longAttack,error),error);
    check(runtime.update(.06,events,error),error);
    check(combat.attacking(2)&&runtime.owns_pose(2)&&defender.selected=="long"
          &&world.actors[2].target_id==1&&combat.cooldown_remaining(2)==0,
          "Positive source injury gate failed to preserve attack cursor/target/cooldown");
    check(std::abs(defender.elapsed-.06)<1e-10
          &&std::abs(world.actors[2].action_elapsed_seconds-.06f)<1e-6f,
          "Suppressed Injury reset the victim's animation/action clock");
    check(runtime.update(2.849,events,error),error);
    check(combat.attacking(2)&&runtime.owns_pose(2),"Gate tick advanced or replaced the victim attack clock");
    check(runtime.begin(1,2,shortAttack,error),error);
    check(runtime.update(.06,events,error),error);
    check(!combat.attacking(2)&&defender.selected=="react"
          &&world.actors[2].target_id==invalid_actor_id,
          "Injure was not admitted after the 3000ms source gate expired");
}
void calculated_result_application() {
    TestWorld world;world.actors[1]=actor(1);world.actors[2]=actor(2);
    world.actors[2].transform.position={1,0,0};
    Visual attacker,target;attacker.add("attack",0,200,150);
    target.add("attack",0,200,150);target.add("react",0,100,1000);target.add("death",0,120,1000);
    CombatSystem combat(world);ActorCombatRuntime runtime(combat);std::string error;
    check(runtime.bind(world.actors[1],attacker.binding(),{},error),error);
    CombatPoseBindings target_poses;target_poses.react_clip_id="react";target_poses.death_clip_id="death";
    target_poses.source_injury_gate_enabled=true;
    check(runtime.bind(world.actors[2],target.binding(),target_poses,error),error);
    auto timed=attack();timed.animation_clip_id="attack";timed.cooldown_seconds=.7;
    timed.cooldown_timing=CooldownTiming::attack_departure;
    check(runtime.begin(1,2,timed,error)&&runtime.begin(2,1,timed,error),error);
    std::vector<DamageEvent> receipts;
    check(runtime.update(.02,receipts,error),error);
    const auto attacker_time=world.actors[1].action_elapsed_seconds;
    const auto target_time=world.actors[2].action_elapsed_seconds;
    const auto attacker_cursor=attacker.elapsed,target_cursor=target.elapsed;
    const auto queries_before_apply=world.eligibility_queries;
    DamageEvent applied;
    const std::uint32_t slow_mask=0x80000100u;
    check(runtime.apply_calculated_hit(1,2,"faery-spell:4","spell-occurrence-0",2,
        std::uint32_t(0x100),slow_mask,applied,error),error);
    check(applied.applied&&applied.health_removed==2&&applied.source_outcomes==0x100
          &&applied.source_mask==slow_mask&&applied.source_id=="faery-spell:4"
          &&applied.marker_name=="spell-occurrence-0"&&world.actors[2].health==8
          &&world.eligibility_queries==queries_before_apply,
          "Calculated result did not preserve full source fields and apply once");
    check(combat.attacking(1)&&combat.attacking(2)&&world.actors[1].target_id==2
          &&world.actors[2].target_id==1&&attacker.selected=="attack"&&target.selected=="attack"
          &&world.actors[1].action_elapsed_seconds==attacker_time
          &&world.actors[2].action_elapsed_seconds==target_time
          &&attacker.elapsed==attacker_cursor&&target.elapsed==target_cursor
          &&combat.cooldown_remaining(1)==0&&combat.cooldown_remaining(2)==0,
          "Non-Injure calculated hit changed either attack cursor, target, or cooldown");

    // Explicit Injure uses exactly the same source gate and victim transition as
    // a marker result; the spell's attacker attack remains untouched.
    check(runtime.apply_calculated_hit(1,2,"faery-spell:4","spell-occurrence-1",1,
        std::uint32_t(0x10),std::uint32_t(0x22aab5),applied,error),error);
    check(applied.applied&&combat.attacking(1)&&!combat.attacking(2)
          &&world.actors[1].target_id==2&&world.actors[2].target_id==invalid_actor_id
          &&attacker.selected=="attack"&&target.selected=="react"
          &&std::abs(combat.cooldown_remaining(2)-.7)<1e-10,
          "Calculated Injure did not share the marker victim reaction path");

    // A lethal precomputed result uses the same death pose path without asking
    // the marker resolver to calculate the amount again.
    check(runtime.apply_calculated_hit(1,2,"faery-spell:4","spell-occurrence-2",20,
        std::uint32_t(0),std::uint32_t(0x22aab5),applied,error),error);
    check(applied.target_died&&!world.actors[2].alive()&&target.selected=="death"
          &&runtime.owns_pose(2)&&combat.attacking(1)&&world.actors[1].target_id==2,
          "Calculated lethal result did not publish death while preserving attacker state");

    const auto before=world.actors[2].health;
    check(!runtime.apply_calculated_hit(1,2,"faery-spell:4","bad-nan",
        std::numeric_limits<float>::quiet_NaN(),std::uint32_t(0),{},applied,error)
        &&!applied.applied&&world.actors[2].health==before,
        "Invalid calculated result mutated health");

    TestWorld invalid;invalid.actors[1]=actor(1);invalid.actors[2]=actor(2);
    CombatSystem invalid_combat(invalid);DamageEvent rejected;
    check(!invalid_combat.apply_calculated_hit(1,2,"","id",1,{}, {},rejected,error)
          &&!rejected.applied&&invalid.actors[2].health==10,
          "Calculated result accepted missing occurrence provenance");
}
}

int main() {
    try {
        nullable_melee_swing();
        invalidated_nullable_melee_swing();
        departure_timing();
        source_clock_timing();
        restored_terminal_death();
        synchronous_source_delivery();
        source_outcome_reactions();
        source_injury_gate();
        calculated_result_application();
        TestWorld world; world.actors[1] = actor(1); world.actors[2] = actor(2);
        world.actors[2].transform.position = {1,0,0};
        Visual first, second;
        for (auto* visual : {&first, &second}) {
            visual->add("attack", 100, 300, 150);
            visual->add("react", 0, 100, 1000);
            visual->add("death", 0, 200, 1000);
        }
        CombatSystem combat(world); ActorCombatRuntime runtime(combat);
        std::string error; std::vector<DamageEvent> events;
        check(runtime.bind(world.actors[1], first.binding(), {}, error), error);
        check(runtime.bind(world.actors[2], second.binding(), {"react","death",{}}, error), error);
        check(runtime.update(.1, events, error), error);
        check(first.updates == 0 && second.updates == 0, "Runtime sampled unowned locomotion");
        check(runtime.begin(1,2,attack(),error), error);
        check(runtime.owns_pose(1) && !runtime.owns_pose(2), "Pose ownership not acquired");
        check(runtime.update(.049,events,error) && events.empty(), "Damage before authored source offset");
        check(runtime.update(.001,events,error) && events.size()==1, "Missing authored source offset hit");
        check(world.actors[2].health==7 && second.selected=="react", "Shared damage/reaction not applied");
        check(runtime.update(.150,events,error), error);
        check(events.empty() && !runtime.owns_pose(1) && !runtime.owns_pose(2), "Actual clip end/reaction end did not release");
        check(std::abs(first.elapsed-.2)<1e-10, "Visual sampled past authored clip end");
        auto unknown = attack(); unknown.damage_markers[0].marker_name = "missing";
        check(!runtime.begin(1,2,unknown,error) && !combat.attacking(1), "Missing markers accepted");
        check(runtime.begin(1,2,attack(),error), error);
        check(runtime.update(.02,events,error), error);
        world.actors[1].action = CharacterAction::moving;
        const auto sampled = first.updates;
        check(runtime.update(.2,events,error) && events.empty(), "Interrupted action delivered damage");
        check(first.updates==sampled && !runtime.owns_pose(1), "Interrupted action retained visual ownership");
        check(runtime.begin(1,2,attack(),error), error);
        check(runtime.update(.1,events,error) && events.size()==1, "Large delta lost marker");
        check(std::abs(second.elapsed-.05)<1e-9, "Reaction missed remaining frame time");
        check(runtime.update(.1,events,error), error);
        check(runtime.begin(1,2,attack(),error), error);
        const auto before = world.actors[2].health;
        events = {DamageEvent{}};
        check(!runtime.update(std::numeric_limits<double>::quiet_NaN(),events,error)
              && events.size()==1 && world.actors[2].health==before, "Invalid delta mutated output/state");
        check(runtime.interrupt(1), "Explicit interruption failed");
        check(runtime.update(0,events,error) && events.empty(), "Interrupted initial marker escaped");
        world.damage = 100;
        check(runtime.begin(1,2,attack(),error), error);
        check(runtime.update(.1,events,error) && events.size()==1 && events[0].target_died, "Lethal hit missing");
        check(second.selected=="death" && runtime.owns_pose(2), "Death clip not acquired");
        check(runtime.update(1,events,error), error);
        check(runtime.owns_pose(2) && std::abs(second.elapsed-.2)<1e-10, "Death terminal pose not held");
        world.actors[2].health=world.actors[2].max_health; world.actors[2].action=CharacterAction::idle;
        check(runtime.update(0,events,error) && !runtime.owns_pose(2), "Revival retained death ownership");
        check(runtime.unbind(2), "Unbind failed");
        check(runtime.bind(world.actors[2], second.binding(), {}, error), error);
        world.damage=10;
        // Same attack definitions, data and runtime apply to the other actor too.
        check(runtime.begin(2,1,attack(),error) && runtime.begin(1,2,attack(),error), error);
        check(runtime.update(.2,events,error) && events.size()==1 && events[0].attacker==1,
              "Simultaneous lethal markers were not deterministically cancelled");
        runtime.clear();
        check(!runtime.owns_pose(1) && !runtime.owns_pose(2), "Clear retained ownership");
        world.actors[1].health=10; world.actors[1].action=CharacterAction::idle;
        world.actors[2].health=10; world.actors[2].action=CharacterAction::idle;
        world.damage=3;
        CombatPoseBindings rates; rates.clip_rates["attack"] = 2;
        check(runtime.bind(world.actors[1], first.binding(), rates, error), error);
        check(runtime.bind(world.actors[2], second.binding(), {}, error), error);
        check(runtime.begin(1,2,attack(),error), error);
        check(runtime.update(.024,events,error) && events.empty(), "Rate-scaled marker fired early");
        check(runtime.update(.001,events,error) && events.size()==1, "Rate-scaled marker missed");
        check(runtime.update(.075,events,error) && !runtime.owns_pose(1), "Rate-scaled clip end failed");
        check(std::abs(first.elapsed-.2)<1e-10, "Rate-scaled visual source clock mismatch");
        world.available=false;
        check(runtime.begin(1,2,attack(),error), error);
        const auto unchanged = world.actors[2].health;
        check(!runtime.update(.1,events,error) && !runtime.owns_pose(1)
              && world.actors[2].health==unchanged, "Missing damage provider did not safely cancel");
        runtime.clear();
        std::cout << "actor combat runtime tests passed\n";
    } catch (const std::exception& e) { std::cerr << e.what() << '\n'; return 1; }
}
