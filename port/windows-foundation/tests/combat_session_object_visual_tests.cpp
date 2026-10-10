#include "../combat_session.hpp"
#include "../retained_animation_owner.hpp"
#include "../game_save.hpp"
#include "../features/interactions/world_object_container_state_v1.hpp"
#include <algorithm>
#include <iostream>
#include <stdexcept>
using namespace dh::foundation;
using namespace dh::foundation::interactions;
static void check(bool ok,const std::string& message){if(!ok)throw std::runtime_error(message);}
static std::vector<Vec3> positions(const CharacterVisual& v){std::vector<Vec3> out;for(const auto& m:v.meshes())for(const auto& vertex:m.vertices)out.push_back(vertex.position);return out;}
static bool same(const std::vector<Vec3>& a,const std::vector<Vec3>& b){if(a.size()!=b.size())return false;for(std::size_t i=0;i<a.size();++i)if(a[i].x!=b[i].x||a[i].y!=b[i].y||a[i].z!=b[i].z)return false;return true;}
int main(int argc,char** argv){try{
    check(argc==3,"Supply original shared assets and isolated object assets");AssetCatalog assets(argv[1]),objects(argv[2]);std::string error;
    OriginalPropertyDatabase db;OriginalMeleeBindings bindings;
    check(load_original_property_tables(assets,"original-cache/data/pydata",db,error),error);
    check(bindings.load(assets,"original-melee-bindings.xml",error),error);
    ActorCustomization customization;customization.allow_missing_animation_targets=true;
    OriginalCombatVisualPlan plan;check(build_original_combat_visual_plan(assets,bindings,"KnightPlayerBase",customization,"object-session-player",plan,error),error);
    CombatSessionConfig config;config.diagnosticRngSeed=19;config.playerId=1;config.playerProfileId="KnightPlayerBase";
    config.tableRoot="original-cache/data/pydata";config.playerVisualConfig=plan.config;
    CombatSessionProfile policy;policy.action={"AttackStatic",0,{0,1}};policy.initialIdle={"Idle",0,{0}};policy.damageMarkerNames={"attack_mainhand"};policy.propertyOptions={256,true};
    config.profiles.emplace("KnightPlayerBase",policy);
    CharacterVisual player;ActorPopulation population;CombatSession session;
    check(session.initialize(assets,db,bindings,config,player,population,{0,0,0},customization,error),error);
    const std::string level="original-cache/data/scene/001_swamp.mlx";
    std::vector<ActorDefinition> definitions;check(load_actor_definitions(assets,level,definitions,error),error);
    const ObjectId ids[]{4308955945491066525ull,17396591008448001070ull};
    const char* models[]{"go_chest_swamp.bdae","go_swamp_urn_breakable.bdae"};
    unsigned opened=0,finished=0;std::vector<ObjectId> eventOrder;std::map<ObjectId,std::uint64_t> generations;
    auto setState=[&](ObjectId id,std::uint8_t state){SourceContainerObjsFieldsV1 fields;
        check(read_source_container_objs_v1(*session.world()->find_object(id),fields,error),error);
        fields.state394=state;std::vector<std::uint8_t> bytes;check(encode_source_container_objs_v1(fields,bytes,error),error);
        check(session.world()->set_object_component(id,source_container_objs_component_v1,std::move(bytes),error),error);
    };
    for(unsigned i=0;i<2;++i){
        const auto d=std::find_if(definitions.begin(),definitions.end(),[&](const auto& value){return value.stableId==ids[i];});
        check(d!=definitions.end(),"Actual source object definition absent");
        WorldObject object;object.id=ids[i];object.name=d->name;object.visual.model=models[i];
        object.transform.position={d->placement[12],d->placement[13],d->placement[14]};
        check(bind_source_container_objs_v1(object,{1,1,static_cast<std::int32_t>(i),2},error),error);
        check(session.world()->bind_object(object,error),error);
        check(session.bind_object_visual(ids[i],objects,error),error);
        check(!session.bind_object_visual(ids[i],objects,error),"Duplicate neutral visual accepted");
        CombatSessionObjectAnimationServices services;
        services.event=[&](ObjectId id,const RetainedAnimationEvent& event,std::string&){
            if(event.name=="opened"){++opened;eventOrder.push_back(id);generations[id]=event.generation;setState(id,3);}
            return true;
        };
        services.finished=[&](ObjectId id,std::uint64_t generation,bool loop,std::string& e){
            check(!loop&&generation==generations.at(id),"Source completion generation/loop differs");
            ++finished;setState(id,4);bool accepted=false;
            check(session.set_object_scene_flags(id,0,0x400u,e),e);
            // This is the real production callback seam; clip selection during
            // the reached completion must not corrupt iteration/event cursors.
            return session.play_object_clip(id,"idleactive",false,accepted,e)&&accepted;
        };
        check(session.bind_object_animation_services(ids[i],std::move(services),error),error);
        bool accepted=false;check(session.play_object_clip(ids[i],"activate",false,accepted,error)&&accepted,error);
        check(session.set_object_scene_flags(ids[i],0x400u,0,error),error);
    }
    check(session.world()->actors().size()==1&&session.world()->objects().size()==2&&!session.actor(ids[0])&&!session.world()->combat_properties(ids[0]),"Object was admitted as a character");
    const auto start=positions(*session.retained_object_visual_borrow(ids[0]));const auto rng=session.world()->random_state();InputActions input;
    for(unsigned i=0;i<10;++i)check(session.update(0,input,{0,0,0},0,error),error);
    check(opened==0&&same(start,positions(*session.retained_object_visual_borrow(ids[0]))),"Zero-time Session advanced object");
    check(session.update(.15,input,{0,0,0},0,error),error);
    check(opened==2&&eventOrder==std::vector<ObjectId>{ids[0],ids[1]}&&!same(start,positions(*session.retained_object_visual_borrow(ids[0]))),"Source objects did not advance once in registered order");
    check(session.update(1.6,input,{0,0,0},0,error),error);
    check(finished==2,"Source completion did not reach both object consumers");
    const auto held=positions(*session.retained_object_visual_borrow(ids[0]));bool accepted=true;
    check(session.play_object_clip(ids[0],"missing",false,accepted,error)&&!accepted&&same(held,positions(*session.retained_object_visual_borrow(ids[0]))),"Unknown object clip mutated playback");
    check(session.set_object_scene_flags(ids[0],0x200u,0,error),error);
    check(session.update(.1,input,{0,0,0},0,error)&&same(held,positions(*session.retained_object_visual_borrow(ids[0]))),"Source update-disable flag ignored");
    check(session.world()->random_state().seed==rng.seed&&session.world()->random_state().calls==rng.calls,"Object presentation drew gameplay RNG");
    auto state=make_default_character("object-session","Player","KnightPlayerBase");session.actor(1)->persistent_character_id=state.id;
    GameSave save;check(capture_game_save(level,1,state,*session.world(),save,error)&&save.version==2,error);
    session.detach_for_restore();check(!session.retained_object_visual_borrow(ids[0])&&!session.play_object_clip(ids[0],"activate",false,accepted,error),"Detached object visual accepted work");
    check(restore_game_save(save,level,*session.world(),state,error)&&session.rebind_after_restore(error),error);
    check(!session.retained_object_visual_borrow(ids[0]),"Restore retained stale object callbacks/pose");
    for(const auto id:ids){SourceContainerObjsFieldsV1 fields;check(read_source_container_objs_v1(*session.world()->find_object(id),fields,error)&&fields.state394==4,error);
        check(session.bind_object_visual(id,objects,error)&&session.restore_object_pose(id,"idleactive",false,error),error);
    }
    check(session.update(.05,input,{0,0,0},0,error)&&opened==2&&finished==2,"Silent source object restore replayed opening/rewards");
    unsigned failureCalls=0;
    CombatSessionObjectAnimationServices failed;
    failed.event=[&](ObjectId,const RetainedAnimationEvent& event,std::string& e){if(event.name=="opened"){++failureCalls;e="fixture consumer failed after reached marker";return false;}return true;};
    check(session.bind_object_animation_services(ids[0],std::move(failed),error),error);
    check(session.play_object_clip(ids[0],"activate",false,accepted,error)&&accepted,error);
    check(!session.update(.15,input,{0,0,0},0,error)&&failureCalls==1&&error=="fixture consumer failed after reached marker","Failed source object consumer did not expose reached prefix");
    check(session.update(0,input,{0,0,0},0,error)&&failureCalls==1,"Failed object marker replayed on retry");
    unsigned removals=0;CombatSessionObjectAnimationServices removed;
    removed.event=[&](ObjectId id,const RetainedAnimationEvent& event,std::string&){if(event.name=="opened"){++removals;check(session.world()->remove_object(id)&&session.unbind_object_visual(id),"Reached consumer could not remove its same object");}return true;};
    check(session.bind_object_animation_services(ids[1],std::move(removed),error),error);
    check(session.play_object_clip(ids[1],"activate",false,accepted,error)&&accepted,error);
    check(session.update(.15,input,{0,0,0},0,error)&&removals==1&&!session.retained_object_visual_borrow(ids[1]),"Source object removal invalidated live frame iteration");
    check(session.world()->remove_object(ids[0])&&!session.retained_object_visual_borrow(ids[0]),"Removed object remained borrowable");
    check(session.update(0,input,{0,0,0},0,error)&&session.unbind_object_visual(ids[0]),error);
    std::cout<<"PASS actual authored chest/urn in one Session; neutral IDs, ordered source pose/events/completion, pause/flags, exact RNG, component restore/no replay and removal\n";
    return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
