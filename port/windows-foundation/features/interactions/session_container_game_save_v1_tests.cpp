#include "world_object_container_state_v1.hpp"
#include "../../game_save.hpp"
#include "../../actor_definitions.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>

using namespace dh::foundation;
using namespace dh::foundation::interactions;

namespace {
void check(bool value,const std::string& message){
    if(!value)throw std::runtime_error(message);
}
bool room_from_authored_definition(void*,const ActorDefinition& definition,
                                   std::int32_t& room,std::string& error){
    const auto found=definition.properties.find("room");
    if(found==definition.properties.end()){
        error="ActorDefinition has no source room64; current generic scene data cannot resolve the OBJS room";
        return false;
    }
    try{
        std::size_t parsed=0;const auto value=std::stol(found->second,&parsed,10);
        if(parsed!=found->second.size()||value<INT32_MIN||value>INT32_MAX)
            throw std::out_of_range("room");
        room=static_cast<std::int32_t>(value);error.clear();return true;
    }catch(...){error="ActorDefinition room property is not an exact signed 32-bit room";return false;}
}
const ActorDefinition& find_definition(const std::vector<ActorDefinition>& definitions,
                                      ObjectId id){
    const auto found=std::find_if(definitions.begin(),definitions.end(),
        [&](const auto& definition){return definition.stableId==id;});
    check(found!=definitions.end(),"Authored neutral chest definition missing");
    return *found;
}
void set_world_state(PlayableActorWorld& world,ObjectId id,std::uint8_t state,
                     std::string& error){
    const auto* object=world.find_object(id);check(object,"Container object missing while changing test state");
    SourceContainerObjsFieldsV1 fields;
    check(read_source_container_objs_v1(*object,fields,error),error);
    fields.state394=state;std::vector<std::uint8_t> payload;
    check(encode_source_container_objs_v1(fields,payload,error),error);
    check(world.set_object_component(id,source_container_objs_component_v1,
        std::move(payload),error),error);
}
}

int main(int argc,char** argv){try{
    check(argc==3,"Supply original shared assets and an isolated save path");
    AssetCatalog assets(argv[1]);std::string error;
    OriginalPropertyDatabase database;dh2::data::AiTables tables;
    check(load_original_property_tables(assets,"original-cache/data/pydata/",database,error),error);
    check(load_original_ai_tables(assets,"original-cache/data/pydata/",tables,error),error);
    OriginalCombatProperties properties;
    check(build_original_combat_properties(database,"KnightPlayerBase",{256,true},{},{},properties,error),error);
    dh2::data::CombatRandom random{812,5};PlayableActorWorld world(tables,random);
    ActorState player;player.id=1;player.definition_id="KnightPlayerBase";
    player.persistent_character_id="container-save-test";
    player.faction_id=properties.sheets.resolved[0];
    player.health=original_signed256(properties.sheets.resolved[36]);
    player.max_health=original_signed256(properties.sheets.resolved[38]);
    player.resource=original_signed256(properties.sheets.resolved[41]);
    player.max_resource=original_signed256(properties.sheets.resolved[43]);
    check(world.bind_actor(player,properties,{true,true,std::nullopt},error),error);

    const std::string level="original-cache/data/scene/001_swamp.mlx";
    std::vector<ActorDefinition> definitions;
    check(load_actor_definitions(assets,level,definitions,error),error);
    constexpr ObjectId chest_id=4308955945491066525ull;
    const auto& chest=find_definition(definitions,chest_id);
    constexpr ObjectId barrel_id=17396591008448001070ull;
    const auto& barrel=find_definition(definitions,barrel_id);
    check(chest.name=="_prim_OpenableContainer_03"&&
          chest.properties.at("data_desc")=="Swamp_Normal_Chest",
          "Source chest row identity differs");
    check(barrel.name=="_prim_DestructibleContainer_1_322"&&
          barrel.properties.at("data_desc")=="Swamp_Normal_DestructibleBarrel",
          "Source barrel row identity differs");
    auto tuple_candidates=[&](const ActorDefinition& wanted){
        return std::count_if(definitions.begin(),definitions.end(),[&](const auto& d){
            return d.gametype==wanted.gametype&&d.name==wanted.name;
        });
    };
    check(tuple_candidates(chest)==1&&tuple_candidates(barrel)==1,
          "Current 001_swamp authored definitions should uniquely bind chest/barrel names; counts="+
          std::to_string(tuple_candidates(chest))+"/"+std::to_string(tuple_candidates(barrel)));
    // ActorDefinition retains exact IDs, names, modules, paths, and placements,
    // but not native room64. Even though these two keys are unique in the
    // currently loaded map, the source tuple resolver fails closed instead of
    // assigning a synthetic room that could break save compatibility.
    ActorId unresolved{};
    const SourceObjectSaveKeyV1 unresolved_key{chest.gametype,chest.name,-1};
    check(!source_object_id_for_save_key_v1(definitions,unresolved_key,nullptr,
        room_from_authored_definition,unresolved,error)&&unresolved==invalid_actor_id&&
        error.find("no source room64")!=std::string::npos,
        "Missing authored room64 did not fail closed");

    auto make_object=[](const ActorDefinition& definition,const char* model,
                        std::int32_t archetype){
        WorldObject object;object.id=definition.stableId;object.name=definition.name;
        object.transform.position={definition.placement[12],definition.placement[13],definition.placement[14]};
        object.transform.rotation[2]=std::atan2(definition.placement[1],definition.placement[0]);
        object.visual.model=model;
        SourceContainerObjsFieldsV1 fields{1,1,archetype,2};std::string local_error;
        check(bind_source_container_objs_v1(object,fields,local_error),local_error);
        return object;
    };
    auto chest_object=make_object(chest,"data/3D/GameObjects/go_chest_swamp.bdae",0x173);
    auto barrel_object=make_object(barrel,"data/3D/GameObjects/go_swamp_urn_breakable.bdae",0x174);
    check(world.bind_object(chest_object,error),error);
    check(world.bind_object(barrel_object,error),error);
    check(chest_object.id==chest_id&&chest_object.name==chest.name&&
          barrel_object.id==barrel_id&&barrel_object.name==barrel.name,
          "Stable ObjectIds did not pair to their exact authored definitions");

    // The retained BRES interaction fixture proves its source `opened` marker
    // advances the same component 2->3; source completion advances 3->4.
    for(const auto id:{chest_id,barrel_id}){
        set_world_state(world,id,3,error);
        set_world_state(world,id,4,error);
    }
    SourceContainerObjsFieldsV1 completed;
    check(read_source_container_objs_v1(*world.find_object(chest_id),completed,error),error);
    check(completed.visible80==1&&completed.enabled8a==1&&
          completed.archetype270==0x173&&completed.state394==4,
          "Completed source container component differs before save");

    auto character=make_default_character("container-save-test","Container Save Test","warrior");
    const auto rng_before=world.random_state();GameSave captured;
    check(capture_game_save(level,1,character,world,captured,error),error);
    check(captured.version==2&&captured.objects.size()==2&&
          captured.objects[0].id==chest_id&&captured.objects[1].id==barrel_id,
          "GameSave did not capture both authored neutral objects");
    for(const auto& saved:captured.objects)
        check(saved.state_components.at(source_container_objs_component_v1).size()==7,
              "GameSave did not carry the exact seven-byte source codec");
    const std::filesystem::path path(argv[2]);
    check(save_game(path,captured,error),error);
    GameSave loaded;check(load_game(path,loaded,error),error);
    check(loaded.version==2&&loaded.objects.size()==2,
          "On-disk GameSave lost an authored neutral container");
    for(std::size_t i=0;i<loaded.objects.size();++i)
        check(loaded.objects[i].state_components.at(source_container_objs_component_v1)==
              captured.objects[i].state_components.at(source_container_objs_component_v1),
              "On-disk GameSave changed a source container component");

    // A fresh loaded level begins closed/openable; restore replaces its
    // component bytes atomically and does not run the interaction callback.
    for(const auto id:{chest_id,barrel_id})set_world_state(world,id,2,error);
    const auto stored_chest_position=loaded.objects[0].transform.position;
    const auto stored_barrel_position=loaded.objects[1].transform.position;
    check(world.set_object_transform(chest_id,Transform{}),"Could not prepare fresh loaded object");
    check(world.set_object_transform(barrel_id,Transform{}),"Could not prepare fresh loaded barrel");
    check(restore_game_save(loaded,level,world,character,error),error);
    const auto* restored=world.find_object(chest_id);check(restored,"Restored chest is absent");
    const auto* restored_barrel=world.find_object(barrel_id);check(restored_barrel,"Restored barrel is absent");
    check(restored->transform.position==stored_chest_position&&
          restored_barrel->transform.position==stored_barrel_position&&
          world.random_state().seed==rng_before.seed&&
          world.random_state().calls==rng_before.calls,
          "GameSave restore changed source placement or consumed RNG");
    check(read_source_container_objs_v1(*restored,completed,error),error);
    check(completed.state394==4&&completed.archetype270==0x173&&
          source_container_restore_visual_v1(completed.state394)==
              SourceContainerRestoreVisualV1::idleactive,
          "State4 did not restore the completed idleactive container component");
    check(read_source_container_objs_v1(*restored_barrel,completed,error),error);
    check(completed.state394==4&&completed.archetype270==0x174&&
          source_container_restore_visual_v1(completed.state394)==
              SourceContainerRestoreVisualV1::idleactive,
          "State4 did not restore the completed idleactive barrel component");
    std::filesystem::remove(path);
    std::cout<<"PASS actual 001_swamp chest/barrel stable ID-to-definition pairs; room64 absence fails closed; exact seven-byte state2->3->4 components through GameSave v2 disk save/load/restore, idleactive state4 and unchanged RNG\n";
    return 0;
}catch(const std::exception& exception){
    std::cerr<<exception.what()<<'\n';return 1;
}}
