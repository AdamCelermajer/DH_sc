#include "session_actor_transition_v1.hpp"

#include "../../asset_catalog.hpp"
#include "../../content_paths.hpp"
#include "../../original_actor_collision_filter.hpp"
#include "../../original_combat_visual_plan.hpp"
#include "../../original_melee_bindings.hpp"
#include "../../original_actor_properties.hpp"
#include "../../features/enemy_ai/runtime_enemy_navigation_v1.hpp"
#include "../../features/combat/runtime_player_profile_attack_bank_v1.hpp"
#include "../../features/equipment/runtime_player_locomotion_v1.hpp"
#include "../../features/skills_animation/skill_animation_program.hpp"
#include "../../../script-runtime/script_constants.hpp"
#include "../../../game-data/skill_tables.hpp"
#include "../../../level-world/character_animation_ai.hpp"

#include <array>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <optional>
#include <stdexcept>
#include <tuple>

using namespace dh::foundation;
using namespace dh::foundation::physics;
namespace {
void check(bool value,const std::string& message){if(!value)throw std::runtime_error(message);}
struct Slots { std::array<float,3> destination{}; std::uintptr_t attached{},visual{}; };

struct Fixture {
    AssetCatalog assets;
    OriginalPropertyDatabase database;
    OriginalMeleeBindings melee;
    ActorCustomization customization;
    OriginalCombatVisualPlan player_plan,enemy_plan;
    combat::RuntimePlayerProfileAttackBankPlanV1 player_attack_bank;
    skills_animation::SkillAnimationPrograms player_skill_bank;
    dh2::data::SkillTables skill_table_owner;
    dh2::data::SkillTables::Borrow skill_tables;
    dh2::data::AnimationTables animation_tables;
    dh2::data::Dictionary animation_dictionary;
    std::vector<std::int32_t> knight_skill_table_ids;
    std::int32_t cast_state_sequence=-1,alternate_skill_table_id=-1,alternate_skill_root=-1;
    bool skill_moving=false,alternate_skill_moving=false;
    CombatSessionConfig config;
    ActorPopulation population;
    CharacterVisual player_visual;
    CombatSession session;
    std::string error;
    std::shared_ptr<dh2::physical::NativeWorld> native;
    PlayableActorBodies bodies;
    std::map<ActorId,Slots> slots;
    std::map<ActorId,bool> idle_suppressed;
    std::shared_ptr<enemy_ai::RuntimeEnemyNavigationV1> navigation;
    std::shared_ptr<SessionActorTransitionConsumerV1> consumer;
    std::vector<std::string> calls;
    std::size_t dead_filter_calls=0,dead_reset_calls=0,attack_gate_calls=0,attack_timer_calls=0;
    std::optional<bool> attack_root_moving;
    std::map<ActorId,std::uint32_t> source_gate528;
    std::uint64_t active_skill_generation=0;
    std::int32_t active_skill_table_id=-1;
    std::int32_t active_skill_root=-1;
    std::size_t skill_timer_scheduled=0;
    std::size_t skill_timer_expired=0;
    unsigned skill_source_finished=0;
    unsigned skill_source_departed=0;

    explicit Fixture(const char* root):assets(root){
        check(load_original_property_tables(assets,"original-cache/data/pydata",database,error),error);
        check(melee.load(assets,"original-melee-bindings.xml",error),error);
        customization.allow_missing_animation_targets=true;
        check(build_original_combat_visual_plan(assets,melee,"KnightPlayerBase",customization,
              "transition-body-player",player_plan,error),error);
        check(build_original_combat_visual_plan(assets,melee,"Swamp_LizadMan_Type1",customization,
              "transition-body-enemy",enemy_plan,error),error);
        const auto read=[&](const char* name){return read_content(assets,std::string("original-cache/data/pydata/")+name);};
        const auto bytes=[](const auto& buffer){return dh2::data::Bytes{buffer.data(),buffer.size()};};
        const auto clipNames=read_content(assets,"data/animations_dictionary_pyarraynames.bin");
        const auto clipValues=read("animations_dictionary_pyarray.bin");
        check(dh2::data::load_dictionary(bytes(clipNames),bytes(clipValues),animation_dictionary,error),error);
        const auto records=read("animations_pyarray.bin"),names=read("animations_pyarraynames.bin"),fields=read("animations_pystructnames.bin");
        check(dh2::data::load_animation_tables(bytes(records),bytes(names),bytes(fields),animation_dictionary,animation_tables,error),error);
        const auto skillRecords=read("skills_pyarray.bin"),skillNames=read("skills_pyarraynames.bin"),skillFields=read("skills_pystructnames.bin");
        check(skill_table_owner.load(bytes(skillRecords),bytes(skillNames),bytes(skillFields),error),error);
        skill_tables=skill_table_owner.borrow();
        const auto treeField=std::find(database.characters.fields.begin(),database.characters.fields.end(),"SkillTree");
        const auto knightRow=std::find(database.characters.names.begin(),database.characters.names.end(),"KnightPlayerBase");
        check(treeField!=database.characters.fields.end()&&knightRow!=database.characters.names.end(),
              "Actual Knight SkillTree source row is missing");
        const auto knightClass=static_cast<std::size_t>(knightRow-database.characters.names.begin());
        const auto treeColumn=static_cast<std::size_t>(treeField-database.characters.fields.begin());
        const auto skillListId=database.characters.rows.at(knightClass).at(treeColumn);
        check(skillListId>=0&&static_cast<std::size_t>(skillListId)<skill_tables.lists().size(),
              "Actual Knight SkillList id is outside loaded source SkillTables");
        knight_skill_table_ids=skill_tables.lists()[static_cast<std::size_t>(skillListId)];
        check(knight_skill_table_ids.size()>0&&knight_skill_table_ids[0]==7&&
              skill_tables.skill_names().at(7)=="BashDown"&&
              skill_tables.skills().at(7).script=="prince_warrior_bashdown"&&
              skill_tables.skills().at(7).scalar.words[1]==347,
              "Knight SkillList position0 no longer resolves to actual BashDown SkillTable7/Skill347");
        skill_moving=(skill_tables.skills().at(7).scalar.words[2]&0xffu)!=0;
        const auto itemRecords=read("loot_table_pyarray.bin"),itemNames=read("loot_table_pyarraynames.bin"),itemFields=read("loot_table_pystructnames.bin");
        dh2::data::ItemTable items;check(dh2::data::load_items(bytes(itemRecords),bytes(itemNames),bytes(itemFields),items,error),error);
        const auto constantBytes=read_content(assets,"data/animations_pycst.bin");
        auto* constants=dh2_script_constants_create();check(constants,"Source constants unavailable");
        dh2_script_constants_reload reload{};
        check(dh2_script_constants_load(constants,constantBytes.data(),static_cast<std::uint32_t>(constantBytes.size()),&reload)==0,
              "Source constants failed");
        equipment_menu::RuntimePlayerLocomotionConstantsV1 stance;
        const bool stanceOk=equipment_menu::load_runtime_player_locomotion_constants_v1(
            [&](const char* group,const char* key,std::int32_t& value,std::string& e){
                if(dh2_script_constants_get(constants,group,key,&value)){e="Missing source constant";return false;}return true;
            },stance,error);
        dh2_script_constants_destroy(constants);check(stanceOk,error);
        ActorProfileLibrary profiles;check(profiles.load(assets,"actor-profiles-v2.xml",error),error);
        const auto* sourceKnight=profiles.find("KnightPlayerBase");check(sourceKnight,"Actual Knight profile missing");
        equipment_menu::RuntimePlayerLocomotionV1 equipped;
        const auto longsword=dh2::data::item_id(items,"Longsword01");check(longsword>=0,"Source Longsword missing");
        check(equipment_menu::resolve_runtime_player_locomotion_v1(std::stoi(sourceKnight->animation_table),items,
            longsword,-1,0,stance,animation_tables,animation_dictionary,equipped,error),error);
        check(combat::plan_runtime_player_profile_attack_bank_v1(assets,*sourceKnight,equipped,
            stance.stanced_list_mask,animation_tables,animation_dictionary,player_plan,"transition-body-player",player_attack_bank,error),error);
        OriginalActorProperties playerProperties;
        check(resolve_original_actor_properties(database.characters,database.classes,"KnightPlayerBase",
              {256,true},playerProperties,error),error);
        const auto animationTable=dh2_character_animation_table_id(
            playerProperties.sheets.resolved[2],static_cast<std::int32_t>(animation_tables.characters.size()));
        check(animationTable>=0&&static_cast<std::size_t>(animationTable)<animation_tables.characters.size(),
              "Actual Knight CharAnimTable row is unavailable");
        const auto& spellRoots=animation_tables.characters[static_cast<std::size_t>(animationTable)].fields.at(31);
        check(!spellRoots.empty(),"Actual Knight CharAnimTable has no Cast roots");
        std::string cast_root_error;
        for(const auto root:spellRoots){
            if(root<0)continue;
            skills_animation::SkillAnimationPrograms candidate;
            if(skills_animation::build_skill_animation_programs(assets,animation_tables,animation_dictionary,
                    player_plan.config,{347,root},"transition-body-skill",candidate,cast_root_error)){
                player_skill_bank=std::move(candidate);cast_state_sequence=root;break;
            }
        }
        check(cast_state_sequence>=0,"No actual Knight Cast root could be retained: "+cast_root_error);
        bool found_opposite_skill=false;
        for(const auto tableId:knight_skill_table_ids){
            if(tableId<0||static_cast<std::size_t>(tableId)>=skill_tables.skills().size()||tableId==7)continue;
            const auto& row=skill_tables.skills()[static_cast<std::size_t>(tableId)];
            const bool moving=(row.scalar.words[2]&0xffu)!=0;
            const auto rawRoot=static_cast<std::int32_t>(row.scalar.words[1]);
            if(moving==skill_moving||rawRoot<0||static_cast<std::size_t>(rawRoot)>=animation_tables.sequences.size())continue;
            skills_animation::SkillAnimationPrograms candidate;std::string candidateError;
            if(!skills_animation::build_skill_animation_programs(assets,animation_tables,animation_dictionary,
                    player_plan.config,{347,cast_state_sequence,rawRoot},"transition-body-skill",candidate,candidateError))continue;
            alternate_skill_table_id=tableId;alternate_skill_root=rawRoot;alternate_skill_moving=moving;
            player_skill_bank=std::move(candidate);found_opposite_skill=true;break;
        }
        check(found_opposite_skill,"Knight source SkillList has no loadable opposite SkillTable moving-byte root for both gate branches");
        config.diagnosticRngSeed=31;config.playerId=1;config.playerProfileId="KnightPlayerBase";
        config.mainItemId="Longsword01";config.equippedItemIds={"Longsword01"};
        config.tableRoot="original-cache/data/pydata";config.playerVisualConfig=player_skill_bank.plan.config;
        config.playerVisualConfig.motion_node_id="auto";
        CombatSessionProfile player;player.initialIdle={"Idle",0,{0}};
        player.sequenceAction=player_attack_bank.static_selection;
        player.sourceAttackBank=player_attack_bank.bank;
        player.sourceAttackPolicies=player_attack_bank.sequence_policies;
        player.sourceAttackStateSelection=true;
        player.sourceAnimationClips=player_skill_bank.plan.config.clips;
        player.customization=customization;
        player.damageMarkerNames={"attack_mainhand"};player.retainedPhaseClock=true;
        player.propertyOptions={256,true};config.profiles.emplace(config.playerProfileId,player);
        CombatSessionProfile enemy;enemy.initialIdle={"Idle",0,{0}};
        enemy.sequenceAction=OriginalAttackSelection{"Attack",0,{0}};
        enemy.reaction=CombatSessionChoice{"Injured",0,{0}};
        enemy.death=CombatSessionChoice{"Died",0,{0}};enemy.retainedPhaseClock=true;
        enemy.damageMarkerNames={"attack_mainhand"};enemy.retainedPhaseClock=true;
        enemy.customization=customization;enemy.propertyOptions={256,true};enemy.motionRoot="auto";
        config.profiles.emplace("Swamp_LizadMan_Type1",enemy);
        PopulationActor placed;placed.profileId="Swamp_LizadMan_Type1";
        placed.definition.stableId=2;placed.definition.sourceId="transition-body-enemy-source";
        placed.definition.placement={1,0,0,0,0,1,0,0,0,0,1,0,0,-20,0,1};
        placed.transform=placed.definition.placement;population.actors().push_back(std::move(placed));
        check(session.initialize(assets,database,melee,config,player_visual,population,{0,0,0},customization,error),error);
        check(session.update(0,{},Vec3{0,0,0},0,error),error);
        check(session.original_actor_state(1)==3&&session.original_actor_state(2)==3,
              "Actual source player/enemy did not begin in Idle3");
        bind_bodies();
        navigation=enemy_ai::make_runtime_enemy_navigation_v1({});
        SessionActorTransitionConsumerConfigV1 consumer_config;
        consumer_config.bodies=&bodies;consumer_config.navigation=navigation.get();
        consumer_config.source.read_idle_suppressed538=[&](ActorId id,bool& value,std::string&){
            value=idle_suppressed.at(id);calls.push_back("idle-get:"+std::to_string(id));return true;};
        consumer_config.source.clear_idle_suppressed538=[&](ActorId id,std::string&){
            idle_suppressed.at(id)=false;calls.push_back("idle-clear:"+std::to_string(id));return true;};
        consumer_config.source.stop_after_route_drop=[&](ActorState& actor,
            dh2::navigation::NavigationObject& pf,bool& position_from_physics,std::string&){
            check(&pf==bodies.navigation(actor.id),"Stop provider did not receive this actor's actual PF object");
            calls.push_back("stop-prefix:"+std::to_string(actor.id));
            // The test witness covers only current source owner fields available
            // in this reconstruction; the callback stands for the actual
            // GameObject destination/heading/path cells absent from ActorState.
            pf.motion.position[0]=actor.transform.position[0];
            pf.motion.position[1]=actor.transform.position[1];
            pf.motion.position[2]=actor.transform.position[2];
            slots.at(actor.id).destination=actor.transform.position;
            check(actor.source_flags520.has_value(),"Stop predicate lost actual source flags520");
            position_from_physics=((*actor.source_flags520&2u)!=0);
            calls.push_back(std::string("stop-physics-predicate:")+
                (position_from_physics?"1":"0")+":"+std::to_string(actor.id));
            return true;};
        consumer_config.source.attack_focus_delay_gate=[&](const CombatSessionActorTransition& event,std::string&){
            ++attack_gate_calls;attack_root_moving=event.source_attack_moving;
            calls.push_back("attack-focus-gate:"+std::to_string(event.actor));return true;};
        consumer_config.source.attack_blur_delay_timer=[&](const CombatSessionActorTransition& event,std::string&){
            ++attack_timer_calls;calls.push_back("attack-blur-timer:"+std::to_string(event.actor));return true;};
        consumer_config.source.skill_focus_moving_byte=[&](const CombatSessionActorTransition& event,bool& value,std::string& detail){
            if(event.actor!=1||event.generation!=active_skill_generation||active_skill_table_id<0||
               static_cast<std::size_t>(active_skill_table_id)>=skill_tables.skills().size()){
                detail="Skill moving byte did not resolve the same current source actor/generation/table";return false;}
            const auto& row=skill_tables.skills()[static_cast<std::size_t>(active_skill_table_id)];
            if(row.scalar.words[1]!=static_cast<std::uint32_t>(active_skill_root)||
               std::find(knight_skill_table_ids.begin(),knight_skill_table_ids.end(),active_skill_table_id)==knight_skill_table_ids.end()){
                detail="Skill moving byte did not resolve the actual selected Knight SkillTable/root pair";return false;}
            value=(row.scalar.words[2]&0xffu)!=0;
            calls.push_back("skill-moving:"+std::to_string(active_skill_table_id)+":"+(value?"1":"0"));
            detail.clear();return true;};
        consumer_config.source.skill_focus_gate528=[&](const CombatSessionActorTransition& event,bool moving,std::string& detail){
            if(event.actor!=1||event.generation!=active_skill_generation){detail="Skill gate receipt is stale";return false;}
            auto& gate=source_gate528[event.actor];gate=(gate&~0x140u)|(moving?0x100u:0u);
            calls.push_back("skill-gate528:"+std::to_string(gate));detail.clear();return true;};
        consumer_config.source.skill_blur_timer10_event48=[&](const CombatSessionActorTransition& event,bool& started,std::string& detail){
            if(event.actor!=1){detail="Skill Blur timer lacks current source actor";return false;}
            const auto found=source_gate528.find(event.actor);
            if(found==source_gate528.end()){detail="Skill Blur timer lacks actual source gate528 owner";return false;}
            started=(found->second&0x100u)!=0;
            if(started){++skill_timer_scheduled;calls.push_back("skill-timer10-event48:"+std::to_string(event.generation));}
            else calls.push_back("skill-blur-immediate-pin:"+std::to_string(event.generation));
            detail.clear();return true;};
        consumer_config.source.source_is_player=[&](ActorId id,bool& value,std::string&){
            const auto* traits=session.world()->traits(id);if(!traits)return false;value=traits->is_player;return true;};
        consumer_config.source.dead_focus_physical_filter=[&](ActorId id,std::string& detail){
            check(bodies.physical(id)!=nullptr,"Dead Focus filter provider lost the same live body");
            if(!bodies.set_source_physical_filter(id,[&](ActorId current){return session.actor(current);},
                 0,0x51c,3,false,detail))return false;
            ++dead_filter_calls;calls.push_back("dead-focus-filter:"+std::to_string(id));return true;};
        consumer_config.source.dead_blur_reset_filter=[&](ActorId id,std::string& detail){
            if(!bodies.reset_source_physical_filter(id,[&](ActorId current){return session.actor(current);},detail))return false;
            ++dead_reset_calls;calls.push_back("dead-blur-reset:"+std::to_string(id));return true;};
        consumer=make_session_actor_transition_consumer_v1(std::move(consumer_config));
        check(consumer->bind(session,error),error);
        source_gate528[1]=0; // The test source-owner fixture's constructor value.
        for(ActorId id:{ActorId{1},ActorId{2}}){
            check(session.bind_actor_locomotion(id,"transition-walk",{"Walk",0,{0}},1.0,true,error),
                  "Bind Walk actor "+std::to_string(id)+": "+error);
            check(session.bind_actor_locomotion(id,"transition-idle",{"Idle",0,{0}},1.0,false,error),
                  "Bind Idle actor "+std::to_string(id)+": "+error);
            check(bodies.set_pinned(id,true,error),error);
            idle_suppressed[id]=false;
        }
    }

    void bind_bodies(){
        native=std::make_shared<dh2::physical::NativeWorld>();
        const float bounds[]{-3000.f,-3000.f,3000.f,3000.f};native->load(bounds);
        for(const auto& source:std::array<std::pair<ActorId,std::string>,2>{{{1,"KnightPlayerBase"},{2,"Swamp_LizadMan_Type1"}}}){
            auto* actor=session.actor(source.first);const auto* facts=session.world()->combat_properties(source.first);
            check(actor&&facts,"Body fixture lacks same Session actor/properties");
            OriginalActorBodyPlanInput input;input.properties=&facts->sheets;input.ai=session.original_ai_tables();
            input.position={actor->transform.position[0],actor->transform.position[1],actor->transform.position[2]};
            input.source_name=source.second;input.owner_identity=source.first;
            input.visual=source.first==1?player_plan.config:enemy_plan.config;
            input.visual.use_authored_modular_defaults=true;
            OriginalActorBodyPlan plan;check(make_original_actor_body_plan(assets,input,plan,error),error);
            check(plan.physical_enabled,"Actual source player/enemy body plan was disabled");
            auto lease=std::const_pointer_cast<void>(session.actor_binding_lease().lock());
            check(lease!=nullptr,"Current actor binding lease is absent");
            auto& slot=slots[source.first];OriginalActorPhysicalBindings physical;
            physical.actor_lease=lease;physical.world_lease=native;physical.data_lease=lease;
            physical.readonly_properties=&facts->sheets;physical.destination1a8=slot.destination.data();
            physical.attached2e0=&slot.attached;physical.visual2d8=&slot.visual;physical.world=native.get();
            physical.ai=session.original_ai_tables();
            physical.static84=[](std::uint8_t& out,std::string&){out=0;return true;};
            physical.is_player=[name=source.second](std::int32_t type,bool& out,std::string&){
                out=original_actor_source_is_player(type,name);return true;};
            physical.debug_switch=[](const char*,bool& out,std::string&){out=false;return true;};
            physical.filter=[](void*,const auto&,const auto&,bool& allowed,std::string&){allowed=true;return true;};
            physical.contact=[](dh2::physical::ContactEvent,void*,unsigned,std::string&){return true;};
            check(bodies.bind(*actor,*facts,plan,std::move(physical),error),error);
            const auto* registered=bodies.physical(source.first);
            check(registered&&registered->native().body,"Source body did not create an actual NativeWorld receiver");
        }
    }
};

void run_source_player_and_enemy(const char* assets){
    Fixture f(assets);
    for(ActorId id:{ActorId{1},ActorId{2}}){
        auto* actor=f.session.actor(id);const auto* body=f.bodies.physical(id);
        const auto* pf=f.bodies.navigation(id);
        check(actor&&body&&pf,"Source actor did not retain body/PF identity");
        auto* native_body=body->native().body;
        check(native_body,"Source actor did not retain its NativeWorld body");
        const auto body_identity=body->actor_identity();const auto* pf_identity=pf;
        check(native_body->GetMass()==0.f,"Initial source Pin did not set zero mass");
        check(f.session.select_actor_locomotion(id,"transition-walk",f.error),f.error);
        check(f.session.original_actor_state(id)==4,"Move4 source state was not committed");
        check(!f.calls.empty()&&f.calls.back()=="idle-clear:"+std::to_string(id),
              "Idle Blur did not clear Character+0x538 before Move Focus");
        dh2::physical::NativeBodyObservation observed{};
        check(dh2_native_body_observe(&observed,&body->native())==0&&observed.pinned==0&&observed.mass>0.f,
              "Move4 focus suffix did not unpin the actual same-Session body");
        check(actor->source_flags520&&*actor->source_flags520==0x23c1&&
              actor->source_movement_type&&*actor->source_movement_type==0,
              "Move4 focus prefix did not publish source flags/type");

        native_body->SetLinearVelocity(b2Vec2(2.0f,-3.0f));native_body->SetAngularVelocity(.25f);
        native_body->SetXForm(b2Vec2(4.0f,-6.0f),.4f);
        dh2::physical::NativeBodyObservation before_blur{};
        check(dh2_native_body_observe(&before_blur,&body->native())==0,
              "Could not capture source body before Move Blur");
        const auto stop_count=f.calls.size();
        check(f.session.select_actor_locomotion(id,"transition-idle",f.error),f.error);
        check(f.session.original_actor_state(id)==3,"Idle3 source state was not committed");
        check(f.calls.size()>=stop_count+3&&f.calls[stop_count]=="stop-prefix:"+std::to_string(id),
              "Move Blur did not release route before the caller-owned GameObject.Stop remainder");
        check(f.calls[stop_count+1]=="stop-physics-predicate:0:"+std::to_string(id),
              "Move Blur did not use the source flags520 bit1 IsUpdatingPositionFromPhysics predicate");
        check(f.calls[stop_count+2]=="idle-get:"+std::to_string(id),
              "Idle Focus source suppression getter ran out of order");
        check(f.bodies.physical(id)==body&&f.bodies.physical(id)->actor_identity()==body_identity&&
              f.bodies.navigation(id)==pf_identity,
              "Transition replaced same-Session body/PF identity");
        check(dh2_native_body_observe(&observed,&body->native())==0&&observed.pinned==1&&
              observed.mass==0.f&&observed.sleeping==before_blur.sleeping&&
              observed.linear_velocity[0]==before_blur.linear_velocity[0]&&
              observed.linear_velocity[1]==before_blur.linear_velocity[1]&&
              observed.angular_velocity==before_blur.angular_velocity,
              "Move Blur failed to skip physical Stop when source flags520 bit1 is clear");
        check(actor->source_flags520&&*actor->source_flags520==0x2380&&
              actor->source_movement_type&&*actor->source_movement_type==0,
              "Idle3 focus did not store flags2380 while retaining source movement type");
        check(f.navigation->report().routes_released==0,
              "Route-only release unexpectedly fabricated a nonempty route cache");
        std::cout<<(id==1?"player":"enemy")<<" state=Move4>Idle3 source_flags=0x"
                 <<std::hex<<*actor->source_flags520<<std::dec<<" body_id="<<body_identity
                 <<" mass="<<observed.mass<<" pinned="<<observed.pinned
                 <<" StopPhysics=0 PF_same=1\n";
    }
}

void run_attack_injury_dead(const char* assets){
    Fixture f(assets);
    const auto* player_body=f.bodies.physical(1);const auto* enemy_body=f.bodies.physical(2);
    check(player_body&&enemy_body,"Attack/Injury/Dead fixture lost actual source bodies");
    const auto player_identity=player_body->actor_identity(),enemy_identity=enemy_body->actor_identity();
    check(f.session.request_actor_attack(1,2,0.0,f.error),f.error);
    check(f.session.original_actor_state(1)==5,"Real same-Session attack request did not enter Attack5");
    auto* player=f.session.actor(1);
    check(player&&player->source_flags520&&*player->source_flags520==0x2341u,
          "Attack5 focus did not publish source flags2341");
    check(f.attack_gate_calls==1&&f.attack_root_moving&&!*f.attack_root_moving&&
          f.calls.back()=="attack-focus-gate:1",
          "Attack5 focus gate did not carry CombatSession's prepared static root fact");
    dh2::physical::NativeBodyObservation player_observation{};
    check(dh2_native_body_observe(&player_observation,&player_body->native())==0&&
          player_observation.pinned==1&&player_observation.mass==0.f&&
          f.bodies.physical(1)==player_body&&player_body->actor_identity()==player_identity,
          "Attack5 did not preserve/pin the actual source player's same body");

    const auto* player_properties=f.session.world()->combat_properties(1);
    check(player_properties,"Actual source player combat facts are unavailable");
    CombatSessionSourceHit hit;hit.attacker=1;hit.target=2;
    hit.binding_lease=f.session.actor_binding_lease();hit.source_id="transition-real-injury";
    hit.marker_name="attack_mainhand";hit.mask=0x22aab5u;
    hit.category=player_properties->facts.main_damage_class;hit.element=-1;
    bool injury_admitted=false;DamageEvent injury;
    for(std::uint64_t generation=1;generation<=1000&&!injury_admitted;++generation){
        f.session.actor(2)->health=f.session.actor(2)->max_health;
        hit.generation=generation;
        check(f.session.apply_source_result(hit,injury,f.error),f.error);
        injury_admitted=injury.applied&&injury.source_outcomes&&
            ((*injury.source_outcomes&0x10u)!=0)&&!injury.target_died&&
            f.session.original_actor_state(2)==11;
    }
    check(injury_admitted,"Actual source result search did not admit Injury11");
    auto* enemy=f.session.actor(2);
    check(enemy&&enemy->source_flags520&&*enemy->source_flags520==0x2b41u,
          "Injury11 focus did not publish source flags2b41");
    dh2::physical::NativeBodyObservation enemy_observation{};
    check(dh2_native_body_observe(&enemy_observation,&enemy_body->native())==0&&
          f.bodies.physical(2)==enemy_body&&enemy_body->actor_identity()==enemy_identity&&
          enemy_observation.pinned==1&&enemy_observation.mass==0.f,
          "Injury11 changed the actual body's identity or source pin state");
    const auto gate_calls_before_duplicate=f.attack_gate_calls;
    check(f.session.apply_source_result(hit,injury,f.error)&&!injury.applied,f.error);
    check(f.attack_gate_calls==gate_calls_before_duplicate&&f.dead_filter_calls==0,
          "Duplicate Injury source occurrence replayed a transition effect");

    CombatSessionSourceHit lethal;lethal.attacker=1;lethal.target=2;
    lethal.binding_lease=f.session.actor_binding_lease();lethal.source_id="transition-real-death";
    lethal.marker_name="attack_mainhand";lethal.mask=0x20080000u;lethal.category=-1;
    lethal.generation=1;lethal.direct_amount=static_cast<std::int32_t>(
        std::ceil(enemy->health*256.0f));
    DamageEvent dead;check(f.session.apply_source_result(lethal,dead,f.error),f.error);
    check(dead.target_died&&f.session.original_actor_state(2)==12,
          "Known-good actual lethal source result did not enter Dead12");
    check(enemy->source_flags520&&*enemy->source_flags520==0x241u,
          "Nonplayer Dead12 focus did not publish source flags241");
    check(f.dead_filter_calls==1&&f.calls.back()=="dead-focus-filter:2",
          "Dead12 with its current body did not call the typed physical filter exactly once");
    check(f.bodies.physical(2)==enemy_body&&enemy_body->actor_identity()==enemy_identity&&
          enemy_body->native().body!=nullptr,
          "Dead12 transition entry detached/removed the current physical body");
    const auto* dead_shape=enemy_body->native().body->GetShapeList();
    check(dead_shape&&dead_shape->GetFilterData().groupIndex==0&&
          dead_shape->GetFilterData().categoryBits==0x51c&&
          dead_shape->GetFilterData().maskBits==3,
          "Dead12 did not apply its exact source filter to the current physical body");
    check(dh2_native_body_observe(&enemy_observation,&enemy_body->native())==0&&
          enemy_observation.pinned==1&&enemy_observation.mass==0.f,
          "Dead12 transition entry changed the actual body's pin/mass state");
    check(f.session.apply_source_result(lethal,dead,f.error)&&!dead.applied,f.error);
    check(f.dead_filter_calls==1&&f.attack_timer_calls==0,
          "Duplicate Died occurrence replayed filter/timer effects");
    std::cout<<"attack_state=5 flags=0x2341 body_id="<<player_identity
             <<" injury_state=11 flags=0x2b41 death_state=12 flags=0x241 body_id="
             <<enemy_identity<<" dead_filter_calls="<<f.dead_filter_calls
             <<" no_detach=1 duplicate=0\n";
}

void run_dead_without_physical_receiver(const char* assets){
    Fixture f(assets);
    check(f.bodies.physical(2)&&f.bodies.physical(2)->native().body,
          "Absent-receiver witness did not begin with an actual source body");
    check(f.bodies.remove_physical(2,f.error),f.error);
    check(f.bodies.physical(2)&&!f.bodies.physical(2)->native().body,
          "Absent-receiver witness did not remove only the physical receiver");
    CombatSessionSourceHit lethal;lethal.attacker=1;lethal.target=2;
    lethal.binding_lease=f.session.actor_binding_lease();lethal.source_id="transition-dead-absent-body";
    lethal.marker_name="attack_mainhand";lethal.mask=0x20080000u;lethal.category=-1;
    lethal.generation=1;lethal.direct_amount=static_cast<std::int32_t>(
        std::ceil(f.session.actor(2)->health*256.0f));
    DamageEvent dead;check(f.session.apply_source_result(lethal,dead,f.error),f.error);
    check(dead.target_died&&f.session.original_actor_state(2)==12,
          "Known-good lethal source result did not reach Dead12 without a body");
    check(f.dead_filter_calls==0&&f.bodies.physical(2)&&!f.bodies.physical(2)->native().body,
          "Dead12 attempted physical filter work without a current receiver");
    std::cout<<"death_without_body state=12 filter_calls=0 receiver_absent=1\n";
}

void run_skill_cast_physical_branches(const char* assets){
    Fixture f(assets);
    const auto* body=f.bodies.physical(1);
    check(body&&body->native().body,"Skill/Cast fixture lacks the actual Knight NativeWorld body");
    const auto identity=body->actor_identity();
    const auto* receiver=body->native().body;
    auto observe=[&](bool pinned){
        dh2::physical::NativeBodyObservation state{};
        check(dh2_native_body_observe(&state,&body->native())==0,"Could not inspect Skill/Cast body");
        check(state.pinned==(pinned?1:0)&&f.bodies.physical(1)==body&&
              body->actor_identity()==identity&&body->native().body==receiver,
              "Skill/Cast physical branch replaced body identity or changed source pin incorrectly");
    };
    auto play=[&](std::int32_t root,std::int32_t state,std::int32_t table_id,std::uint64_t generation){
        f.active_skill_generation=generation;f.active_skill_table_id=table_id;f.active_skill_root=root;
        OriginalAttackSelection selection;selection.state=skills_animation::skill_sequence_state(root);
        f.skill_source_finished=0;f.skill_source_departed=0;
        CombatSessionStateAnimationServices services;
        services.finished=[&f,state](ActorId id,std::string& detail){
            check(id==1&&f.session.original_actor_state(1)==state,"Skill/Cast Post ran outside its active source state");
            ++f.skill_source_finished;f.session.actor(1)->action=CharacterAction::idle;detail.clear();return true;};
        services.departed=[&f,state](ActorId id,std::int32_t from,std::int32_t to,std::string& detail){
            check(id==1&&from==state&&to==3,"Skill/Cast interruption Post received the wrong source transition");
            ++f.skill_source_departed;detail.clear();return true;};
        services.checkpoint=[](std::string& detail){detail.clear();return true;};
        f.session.actor(1)->action=CharacterAction::casting;
        const CombatSessionSourceSequencePolicy policy{state,state==6?0x6341u:0x6301u,generation};
        check(f.session.play_actor_source_sequence(1,f.player_skill_bank.plan,f.player_skill_bank.policies,
            selection,std::move(services),policy,f.error),f.error);
        check(f.session.original_actor_state(1)==state,"Actual source sequence did not enter requested Skill/Cast state");
    };
    auto advance_to_idle=[&](std::int32_t source_state){
        for(unsigned frame=0;frame<1200&&f.session.original_actor_state(1)!=3;++frame)
            check(f.session.update(1.0/60,{},Vec3{0,0,0},0,f.error),f.error);
        check(f.session.original_actor_state(1)==3,"Actual source Skill/Cast sequence did not complete to Idle3");
        (void)source_state;
    };
    auto expire_skill_timer=[&](std::uint64_t generation){
        check(f.session.update(.02,{},Vec3{0,0,0},0,f.error),f.error);
        check(f.session.original_actor_state(1)==3&&f.bodies.physical(1)&&f.bodies.physical(1)->native().body,
              "Skill event48 timer expired without the same current idle body");
        check(f.source_gate528.at(1)&0x100u,"Skill event48 timer expiry lost gate528 bit0x100");
        check(f.bodies.set_pinned(1,true,f.error),f.error);++f.skill_timer_expired;
        f.calls.push_back("skill-expire10-event48:"+std::to_string(generation));
    };

    // Real Knight SkillList position zero is SkillTable7/BashDown/sequence347.
    check(f.knight_skill_table_ids[0]==7&&f.skill_tables.skills()[7].scalar.words[1]==347,
          "BashDown real source SkillTable/root changed");
    observe(true);
    play(347,6,7,7001);
    check(f.skill_moving&&f.source_gate528.at(1)==0x100u,
          "BashDown Focus did not read its actual moving byte and write gate528 bit0x100");
    check(f.session.actor(1)->source_flags520&&*f.session.actor(1)->source_flags520==0x6341u,
          "BashDown Focus did not apply source flags6341");
    observe(false);
    advance_to_idle(6);
    check(f.skill_source_finished==1&&f.skill_source_departed==0,
          "BashDown normal completion did not dispatch one source Post");
    check(f.skill_timer_scheduled==1&&f.skill_timer_expired==0,
          "Moving BashDown Blur did not schedule exactly one timer10/event0x30 without immediate pin");
    observe(false);
    expire_skill_timer(7001);observe(true);

    // Same actual source SkillList, opposite authored scalar low byte/root.
    const auto opposite_id=f.alternate_skill_table_id;
    const auto opposite_root=f.alternate_skill_root;
    check(opposite_id>=0&&opposite_root>=0&&f.alternate_skill_moving!=f.skill_moving,
          "Actual Knight source SkillList no longer supplies opposite moving-byte branch");
    play(opposite_root,6,opposite_id,7002);
    check(f.source_gate528.at(1)==(f.alternate_skill_moving?0x100u:0u),
          "Opposite source Skill Focus did not update gate528 from its actual SkillTable row");
    observe(false);
    bool departed=false;
    check(f.session.cancel_actor_source_sequence(1,f.session.actor_binding_lease(),7002,3,departed,f.error),f.error);
    check(departed&&f.session.original_actor_state(1)==3,
          "Opposite Skill interruption did not complete its admitted source departure");
    check(f.skill_source_finished==0&&f.skill_source_departed==1,
          "Opposite Skill interruption did not dispatch exactly one departure Post");
    if(f.alternate_skill_moving){
        check(f.skill_timer_scheduled==2,"Moving Skill interruption failed to schedule event48 exactly once");
        observe(false);expire_skill_timer(7002);observe(true);
    }else{
        observe(true);
    }
    const auto scheduled_before_duplicate=f.skill_timer_scheduled;
    check(f.session.cancel_actor_source_sequence(1,f.session.actor_binding_lease(),7002,3,departed,f.error)&&!departed,f.error);
    check(f.skill_timer_scheduled==scheduled_before_duplicate,
          "Duplicate Skill interruption replayed physical gate/timer work");

    // Real Knight CharAnimTable Spell root uses Cast7, which preserves pin.
    play(f.cast_state_sequence,7,-1,7003);
    check(f.session.actor(1)->source_flags520&&*f.session.actor(1)->source_flags520==0x6301u,
          "Cast Focus did not apply source flags6301");
    observe(true);
    advance_to_idle(7);observe(true);
    check(f.skill_source_finished==1&&f.skill_source_departed==0,
          "Normal Cast7 completion did not dispatch exactly one normal Post");
    check(f.skill_timer_scheduled==scheduled_before_duplicate,
          "Cast7 incorrectly entered Skill gate/timer/pin branch");

    // A second real Cast7 playback is interrupted; its physical body stays pinned.
    play(f.cast_state_sequence,7,-1,7004);observe(true);
    check(f.session.cancel_actor_source_sequence(1,f.session.actor_binding_lease(),7004,3,departed,f.error),f.error);
    check(departed&&f.session.original_actor_state(1)==3,"Cast7 interruption did not return to Idle3");
    check(f.skill_source_finished==0&&f.skill_source_departed==1,
          "Cast7 interruption did not dispatch exactly one departure Post");
    observe(true);
    const auto call_count=f.calls.size();
    check(f.session.cancel_actor_source_sequence(1,f.session.actor_binding_lease(),7004,3,departed,f.error)&&!departed,f.error);
    check(f.calls.size()==call_count,"Duplicate Cast7 interruption replayed transition effects");
    std::cout<<"skill347_moving="<<f.skill_moving<<" alternate_table="<<opposite_id
             <<" alternate_moving="<<f.alternate_skill_moving<<" timers="<<f.skill_timer_scheduled
             <<" expiries="<<f.skill_timer_expired<<" cast_root="<<f.cast_state_sequence
             <<" same_body="<<identity<<" PASS\n";
}
}

int main(int argc,char** argv){try{
    check(argc==2,"Supply original shared asset root");
    run_source_player_and_enemy(argv[1]);
    run_attack_injury_dead(argv[1]);
    run_dead_without_physical_receiver(argv[1]);
    run_skill_cast_physical_branches(argv[1]);
    std::cout<<"PASS same-Session player/enemy Move/Idle/Attack/Injury/Dead physical projection, source flags/root fact, duplicate suppression and corpse body retention\n";
    return 0;
}catch(const std::exception& exception){std::cerr<<"FAIL: "<<exception.what()<<'\n';return 1;}}
