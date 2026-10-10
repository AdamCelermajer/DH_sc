#pragma once
#include "../physics/session_actor_transition_v1.hpp"
#include "../enemy_ai/runtime_enemy_navigation_v1.hpp"
#include "../../source_module_floors.hpp"
#include "../../../level-world/navigation_heading.hpp"
#include <map>

// Isolated actual-body witness; no user save/window or impulse is involved.
struct B035NativeWitness {
    struct Cells { std::array<float,3> destination{};std::uintptr_t attached{},visual{};
        std::uint32_t gate=0;bool locked=false,idle_suppressed=false;
        std::optional<std::uint32_t> attack_timer; };
    struct Obstacles {
        std::array<dh2::navigation::ObstacleEntry,8> entries{};
        std::array<unsigned,8> floors{};
        dh2::navigation::ObstacleRegistry registry{entries.data(),0,8,floors.data(),0,8};
    };
    CombatSession& session;
    std::shared_ptr<dh2::physical::NativeWorld> native=std::make_shared<dh2::physical::NativeWorld>();
    PlayableActorBodies bodies;
    std::shared_ptr<dh2::floors::World> floor=std::make_shared<dh2::floors::World>();
    std::shared_ptr<Obstacles> obstacles=std::make_shared<Obstacles>();
    std::map<ActorId,Cells> cells;
    std::shared_ptr<dh::foundation::enemy_ai::RuntimeEnemyNavigationV1> navigation;
    std::shared_ptr<dh::foundation::physics::SessionActorTransitionConsumerV1> transition;
    unsigned locks=0,unlocks=0,look_and_sneak=0,motion_samples=0;
    std::vector<std::uint32_t> gate_writes;
    float authored_xy=0;
    std::array<float,3> start{};

    B035NativeWitness(AssetCatalog& assets,OriginalMeleeBindings& melee,CombatSession& current,
                     const CharacterVisualConfig& visual):session(current){
        std::string error;
        const auto bytes=assets.read("original-cache/data/3d/modules/swamp/swamp.bdae");
        dh2::resources::BresView view{};
        check(dh2_bres_open(&view,bytes.data(),bytes.size())==dh2::resources::BresError::ok,"Swamp floor BRES");
        dh2::scene::Scene scene;check(dh2::scene::load(view,scene,error),error);
        unsigned index=UINT32_MAX;
        for(unsigned i=0;i<scene.instances.size();++i){const auto node=scene.instances[i].node_index;
            if(node<scene.graph.size()&&scene.graph[node].name.find("floor")!=std::string::npos){index=i;break;}}
        check(index!=UINT32_MAX,"Swamp source floor instance");
        OriginalSourceFloorBinding binding;binding.instance=index;binding.room=0;
        binding.mesh_local_quaternion={0,0,0,1};binding.mesh_local_scale={1,1,1};
        check(append_original_module_floors(view,scene,{binding},*floor,error),error);
        check(dh2::floors::build_graph(*floor,error)&&dh2::floors::post_load(*floor,error),error);
        bool found=false;
        for(const auto& triangle:floor->records.front()->triangles){
            for(unsigned k=0;k<3;++k)start[k]=(triangle.points[0][k]+triangle.points[1][k]+triangle.points[2][k])/3.f;
            float height=start[2];if(dh2::floors::height(*floor,start.data(),height)){start[2]=height;found=true;break;}}
        check(found,"Swamp actual floor point");
        session.actor(1)->transform.position=start;
        // Keep attacker away from the victim; contacts remain genuine native
        // contacts, but cannot make this focused Push test into another attack.
        session.actor(2)->transform.position={start[0]+100,start[1],start[2]};
        const float bounds[]{-300000.f,-300000.f,300000.f,300000.f};native->load(bounds);
        OriginalCombatVisualPlan enemy_plan;ActorCustomization custom;custom.allow_missing_animation_targets=true;
        check(build_original_combat_visual_plan(assets,melee,"Swamp_LizadMan_Type1",custom,"b035-native-source-body",enemy_plan,error),error);
        for(ActorId id:{ActorId{1},ActorId{2}}){
            auto* actor=session.actor(id);const auto* properties=session.world()->combat_properties(id);
            OriginalActorBodyPlanInput input;input.properties=&properties->sheets;input.ai=session.original_ai_tables();
            input.position={actor->transform.position[0],actor->transform.position[1],actor->transform.position[2]};input.source_name=id==1?"KnightPlayerBase":"Swamp_LizadMan_Type1";
            input.owner_identity=id;input.visual=id==1?visual:enemy_plan.config;input.visual.use_authored_modular_defaults=true;
            OriginalActorBodyPlan plan;check(make_original_actor_body_plan(assets,input,plan,error),error);
            auto lease=std::const_pointer_cast<void>(session.actor_binding_lease().lock());
            auto& cell=cells[id];OriginalActorPhysicalBindings physical;
            physical.actor_lease=lease;physical.world_lease=native;physical.data_lease=lease;
            physical.readonly_properties=&properties->sheets;physical.position160=actor->transform.position.data();
            physical.destination1a8=cell.destination.data();physical.attached2e0=&cell.attached;physical.visual2d8=&cell.visual;
            physical.world=native.get();physical.ai=session.original_ai_tables();
            physical.static84=[](std::uint8_t& out,std::string&){out=0;return true;};
            physical.is_player=[name=input.source_name](std::int32_t type,bool& out,std::string&){out=original_actor_source_is_player(type,name);return true;};
            physical.debug_switch=[](const char*,bool& out,std::string&){out=false;return true;};
            physical.filter=[](void*,const auto&,const auto&,bool& out,std::string&){out=true;return true;};
            physical.contact=[](dh2::physical::ContactEvent,void*,unsigned,std::string&){return true;};
            check(bodies.bind(*actor,*properties,plan,std::move(physical),error),error);
            check(bodies.set_pinned(id,true,error),error);actor->source_flags520=0x2380u;
        }
        for(ActorId id:{ActorId{1},ActorId{2}})
            check(bodies.initialize_navigation(id,{floor,obstacles,floor.get(),&obstacles->registry},error),error);
        navigation=dh::foundation::enemy_ai::make_runtime_enemy_navigation_v1({});
        dh::foundation::physics::SessionActorTransitionConsumerConfigV1 config;
        config.bodies=&bodies;config.navigation=navigation.get();
        config.source.read_idle_suppressed538=[&](ActorId id,bool& out,std::string&){out=cells.at(id).idle_suppressed;return true;};
        config.source.clear_idle_suppressed538=[&](ActorId id,std::string&){cells.at(id).idle_suppressed=false;return true;};
        const auto delay=[&](ActorId id){const auto* properties=session.world()->combat_properties(id);
            const auto* ai=dh2::data::ai_props(session.world()->factions(),properties->sheets.resolved[1]);
            check(ai&&ai->attack_delay>=0,"Actual source AttackDelay");return std::uint32_t(ai->attack_delay);};
        config.source.attack_focus_delay_gate=[&,delay](const CombatSessionActorTransition& event,std::string&){if(delay(event.actor))cells.at(event.actor).gate|=1u;return true;};
        config.source.attack_blur_delay_timer=[&,delay](const CombatSessionActorTransition& event,std::string&){if(delay(event.actor))cells.at(event.actor).attack_timer=delay(event.actor);return true;};
        config.source.knockback_read_gate528=[&](ActorId id,std::uint32_t& out,std::string&){out=cells.at(id).gate;return true;};
        config.source.knockback_write_gate528=[&](ActorId id,std::uint32_t gate,std::string&){cells.at(id).gate=gate;gate_writes.push_back(gate);return true;};
        config.source.knockback_controller_lock=[&](ActorId id,bool locked,std::string&){cells.at(id).locked=locked;if(locked)++locks;else ++unlocks;return true;};
        config.source.knockback_look_at_cancel_sneaking=[&](const CombatSessionActorTransition& event,std::string&){
            check(event.actor==1&&event.source_other_actor==2&&cells.at(1).locked,"Source LookAt/CancelSneaking ordering/attacker");
            auto* actor=session.actor(event.actor);const auto* attacker=session.actor(event.source_other_actor);
            float direction[]{attacker->transform.position[0]-actor->transform.position[0],attacker->transform.position[1]-actor->transform.position[1],0};
            float heading=actor->transform.rotation[2];check(dh2_nav_look_towards(&heading,direction)==0,"Source LookAt heading");
            actor->transform.rotation[2]=heading;++look_and_sneak;return true;};
        config.source.source_is_player=[&](ActorId id,bool& out,std::string&){out=session.world()->traits(id)->is_player;return true;};
        config.source.dead_focus_physical_filter=[&](ActorId id,std::string& e){return bodies.set_source_physical_filter(id,[&](ActorId current){return session.actor(current);},0,0x51c,3,false,e);};
        transition=dh::foundation::physics::make_session_actor_transition_consumer_v1(std::move(config));
        check(transition->bind(session,error),error);
        check(session.set_motion_phase_handler([&](ActorState& actor,std::uint64_t frame,double dt,
            const std::vector<CombatSessionMotionSample>& samples,std::string& e){
            bool stepped=false;auto lookup=[&](ActorId id){return session.actor(id);};
            if(!bodies.step_world(*native,frame,std::uint32_t(dt*1000),lookup,stepped,e))return false;
            PlayableActorBodies::PhysicsPositionResult imported;
            if(!bodies.reconcile_physics_position(actor.id,lookup,actor.id==1,imported,e))return false;
            const bool accepted=imported.xy_changed&&(!imported.floor_checked||imported.floor_valid);
            auto& cell=cells.at(actor.id);if(cell.attack_timer){
                const auto elapsed=std::uint32_t(dt*1000);
                if(elapsed>=*cell.attack_timer){cell.attack_timer.reset();cell.gate&=~1u;}else *cell.attack_timer-=elapsed;
            }
            for(const auto& sample:samples)if(sample.move_go&&!accepted&&(*actor.source_flags520&1u)){
                OriginalActorNavigationMoveResult result;
                if(!bodies.move_grounded(actor.id,{actor.transform.position[0],actor.transform.position[1],actor.transform.position[2]},sample.authored_motion,result,e))return false;
                actor.transform.position={result.position.x,result.position.y,result.position.z};
                if(actor.id==1){++motion_samples;authored_xy+=std::hypot(sample.authored_motion.x,sample.authored_motion.y);}
            }
            return bodies.set_position(actor.id,actor.transform.position,false,e);
        },error),error);
    }
};
