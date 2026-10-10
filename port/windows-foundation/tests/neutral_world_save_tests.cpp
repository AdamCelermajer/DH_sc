#include "../game_save.hpp"
#include "../actor_definitions.hpp"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>
#include <stdexcept>
using namespace dh::foundation;
namespace {
void check(bool value,const std::string& message){if(!value)throw std::runtime_error(message);}
std::vector<char> read(const std::filesystem::path& path){std::ifstream f(path,std::ios::binary);return {std::istreambuf_iterator<char>(f),{}};}
}
int main(int argc,char** argv){try{
    check(argc==3,"Supply shared source assets and isolated save path");AssetCatalog assets(argv[1]);std::string error;
    const std::string tableRoot="original-cache/data/pydata/";
    OriginalPropertyDatabase db;dh2::data::AiTables tables;
    check(load_original_property_tables(assets,tableRoot,db,error),error);
    check(load_original_ai_tables(assets,tableRoot,tables,error),error);
    OriginalCombatProperties props;
    check(build_original_combat_properties(db,"KnightPlayerBase",{256,true},{},{},props,error),error);
    dh2::data::CombatRandom rng{987,13};PlayableActorWorld world(tables,rng);
    ActorState player;player.id=1;player.definition_id="KnightPlayerBase";player.persistent_character_id="neutral-world-test";
    player.faction_id=props.sheets.resolved[0];player.health=original_signed256(props.sheets.resolved[36]);
    player.max_health=original_signed256(props.sheets.resolved[38]);player.resource=original_signed256(props.sheets.resolved[41]);
    player.max_resource=original_signed256(props.sheets.resolved[43]);
    check(world.bind_actor(player,props,{true,true,std::nullopt},error),error);
    auto character=make_default_character("neutral-world-test","Test","warrior");
    const std::string level="original-cache/data/scene/001_swamp.mlx";
    std::vector<ActorDefinition> definitions;check(load_actor_definitions(assets,level,definitions,error),error);
    const ObjectId chestId=4308955945491066525ull,barrelId=17396591008448001070ull;
    auto definition=[&](ObjectId id)->const ActorDefinition&{
        const auto found=std::find_if(definitions.begin(),definitions.end(),[&](const auto& d){return d.stableId==id;});
        check(found!=definitions.end(),"Exact source module occurrence missing");return *found;
    };
    const auto& chest=definition(chestId);const auto& barrel=definition(barrelId);
    check(chest.properties.at("data_desc")=="Swamp_Normal_Chest"&&
          barrel.properties.at("data_desc")=="Swamp_Normal_DestructibleBarrel","Source row identity mismatch");
    check(std::fabs(chest.placement[12]+7013.42f)<.02f&&std::fabs(chest.placement[13]-12982.703f)<.02f&&
          std::fabs(barrel.placement[12]+6346.486f)<.02f,"Source module offset not retained");
    auto object=[](const ActorDefinition& definition,const char* model){
        WorldObject result;result.id=definition.stableId;result.name=definition.name;
        result.transform.position={definition.placement[12],definition.placement[13],definition.placement[14]};
        result.transform.rotation[2]=std::atan2(definition.placement[1],definition.placement[0]);
        result.visual.model=model;return result;
    };
    check(world.bind_object(object(chest,"data/3D/GameObjects/go_chest_swamp.bdae"),error),error);
    check(world.bind_object(object(barrel,"data/3D/GameObjects/go_swamp_urn_breakable.bdae"),error),error);
    check(world.actors().size()==1&&world.objects().size()==2&&!world.find_actor(chestId)&&
          !world.combat_properties(chestId)&&!world.traits(barrelId),"Neutral object became a combat character");
    auto collision=object(chest,"unused");collision.id=1;
    check(!world.bind_object(collision,error),"Object/character ID collision admitted");
    auto actorCollision=player;actorCollision.id=barrelId;
    check(!world.bind_actor(actorCollision,props,{false,false,std::nullopt},error),"Character/object ID collision admitted");
    check(!world.bind_object(object(chest,"unused"),error),"Duplicate source occurrence admitted");
    auto invalid=object(chest,"unused");invalid.id=99;invalid.transform.position[0]=std::numeric_limits<float>::quiet_NaN();
    check(!world.bind_object(invalid,error)&&!world.find_object(99),"Invalid object mutated registry");
    // Opaque fixture bytes prove infrastructure preservation only. The real
    // container feature must supply its versioned codec and event receipts.
    const std::vector<std::uint8_t> component{1,0,0,0,4,0,0,0,0,255,128};
    check(world.set_object_component(chestId,"container-test-v1",component,error),error);
    check(!world.set_object_component(chestId,"oversized",std::vector<std::uint8_t>(world_object_state_limit+1),error)&&
          world.find_object(chestId)->state_components.size()==1,"Rejected component changed persistent state");
    check(!world.set_object_component(chestId,"",{},error),"Empty component key admitted");
    const auto before=rng;GameSave captured;check(capture_game_save(level,1,character,world,captured,error),error);
    check(captured.version==2&&captured.actors.size()==1&&captured.objects.size()==2&&
          rng.seed==before.seed&&rng.calls==before.calls,"Object capture changed RNG or schema scope");
    const std::filesystem::path path(argv[2]);check(save_game(path,captured,error),error);
    const auto bytes=read(path);GameSave loaded;check(load_game(path,loaded,error),error);
    const auto savedChest=std::find_if(loaded.objects.begin(),loaded.objects.end(),[&](const auto& o){return o.id==chestId;});
    check(savedChest!=loaded.objects.end()&&savedChest->state_components.at("container-test-v1")==component&&
          loaded.objects.back().id==barrelId,"Unsigned64 ID or component bytes lost on disk");
    auto moved=world.find_object(chestId)->transform;moved.position={1,2,3};
    check(world.set_object_transform(chestId,moved)&&world.set_object_component(chestId,"container-test-v1",{9},error),error);
    world.find_actor(1)->health=1;rng={7,0};
    check(restore_game_save(loaded,level,world,character,error),error);
    check(world.find_object(chestId)->transform.position==savedChest->transform.position&&
          world.find_object(chestId)->state_components.at("container-test-v1")==component&&
          rng.seed==before.seed&&rng.calls==before.calls&&world.find_actor(1)->health==captured.character.stats.health,
          "Combined actor/object/RNG restore differed");
    auto wrong=loaded;wrong.objects[0].visual.model="wrong-model";
    const auto healthy=world.find_actor(1)->health;
    check(!restore_game_save(wrong,level,world,character,error)&&world.find_actor(1)->health==healthy&&
          world.find_object(chestId)->state_components.at("container-test-v1")==component,
          "Incompatible object restore partially published");
    wrong=loaded;wrong.objects[0].id=1;
    check(!save_game(path,wrong,error)&&read(path)==bytes,"Rejected shared-ID save damaged prior file");
    auto records=loaded.actors;records[0].actor.id=chestId;
    check(!world.replace_state(records,loaded.objects,{2,0},error)&&world.find_actor(1)&&
          world.objects().size()==2&&rng.seed==before.seed&&rng.calls==before.calls,
          "Rejected combined world replacement mutated a subset/RNG");
    auto legacy=loaded;legacy.version=1;legacy.objects.clear();const auto legacyPath=path.string()+".v1";
    check(save_game(legacyPath,legacy,error),error);const auto legacyBytes=read(legacyPath);
    GameSave old;check(load_game(legacyPath,old,error)&&old.version==1&&old.objects.empty()&&read(legacyPath)==legacyBytes,error);
    check(save_game(legacyPath,old,error)&&read(legacyPath)==legacyBytes,"Actor-only version1 wire format changed");
    check(world.set_object_component(chestId,"container-test-v1",{42},error),error);
    check(restore_game_save(old,level,world,character,error)&&
          world.find_object(chestId)->state_components.at("container-test-v1")==std::vector<std::uint8_t>{42},
          "Legacy actor-only restore silently erased neutral objects");
    check(world.remove_object(barrelId)&&!world.find_object(barrelId)&&world.find_actor(1),"Object removal removed character");
    world.clear();check(world.actors().empty()&&world.objects().empty(),"Level clear retained neutral objects");
    std::cout<<"PASS actual authored chest/barrel unsigned64 identities and placement, neutral enrollment, shared-ID rejection, bounded components, v2 atomic save/restore, unchanged v1 wire/restore, no combat/RNG side effects\n";
    return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
