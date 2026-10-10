#include "../game_save.hpp"
#include "../asset_catalog.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>

using namespace dh::foundation;
namespace {
void check(bool value,const std::string& message){if(!value)throw std::runtime_error(message);}
ActorState make_actor(ActorId id,const char* definition,const OriginalCombatProperties& properties){
    ActorState actor;actor.id=id;actor.definition_id=definition;
    actor.faction_id=properties.sheets.resolved[0];
    actor.health=original_signed256(properties.sheets.resolved[36]);
    actor.max_health=original_signed256(properties.sheets.resolved[38]);
    actor.resource=original_signed256(properties.sheets.resolved[41]);
    actor.max_resource=original_signed256(properties.sheets.resolved[43]);
    return actor;
}
std::vector<char> read_bytes(const std::filesystem::path& path){
    std::ifstream stream(path,std::ios::binary);
    return {std::istreambuf_iterator<char>(stream),{}};
}
void write_bytes(const std::filesystem::path& path,const std::vector<char>& bytes){
    std::ofstream stream(path,std::ios::binary|std::ios::trunc);
    stream.write(bytes.data(),static_cast<std::streamsize>(bytes.size()));
    if(!stream)throw std::runtime_error("Could not write test checkpoint");
}
std::uint64_t checksum(const std::vector<char>& bytes,std::size_t count){
    std::uint64_t hash=14695981039346656037ull;
    for(std::size_t i=0;i<count;++i){hash^=static_cast<unsigned char>(bytes[i]);hash*=1099511628211ull;}
    return hash;
}
}

int main(int argc,char** argv){try{
    check(argc==3,"Supply original asset root and private checkpoint path");
    AssetCatalog assets(argv[1]);std::string error;
    constexpr const char* root="original-cache/data/pydata/";
    OriginalPropertyDatabase database;dh2::data::AiTables ai;
    check(load_original_property_tables(assets,root,database,error),error);
    check(load_original_ai_tables(assets,root,ai,error),error);
    OriginalCombatProperties knight,lizard;
    check(build_original_combat_properties(database,"KnightPlayerBase",{256,true},{},{},knight,error),error);
    check(build_original_combat_properties(database,"Swamp_LizadMan_Type1",{std::nullopt,true},{},{},lizard,error),error);

    dh2::data::CombatRandom rng{0x51a7u,29};PlayableActorWorld world(ai,rng);
    auto player=make_actor(1,"KnightPlayerBase",knight);player.persistent_character_id="physical-presence-test";
    player.attack_ids={"original-main-leaf"};player.source_physical_present=true;
    auto corpse=make_actor(2,"Swamp_LizadMan_Type1",lizard);corpse.health=0;corpse.action=CharacterAction::dead;
    // Model the value the host publishes after its End34 body-removal path;
    // this fixture does not invoke that producer or persist its animation cursor.
    // The corpse remains in the same PlayableActorWorld actor roster.
    corpse.source_physical_present=false;
    auto unknown=make_actor(3,"Swamp_LizadMan_Type1",lizard);
    check(world.bind_actor(player,knight,{true,true,std::nullopt},error),error);
    check(world.bind_actor(corpse,lizard,{false,true,std::nullopt},error),error);
    check(world.bind_actor(unknown,lizard,{false,true,std::nullopt},error),error);
    check(world.bind_source("main",{},error),error);
    auto character=make_default_character("physical-presence-test","Test","warrior");
    const std::string level="original/physical-presence-test";

    GameSave captured;
    check(capture_game_save(level,1,character,world,captured,error),error);
    check(captured.version==3&&captured.actors.size()==3&&
          captured.actors[0].actor.source_physical_present==std::optional<bool>(true)&&
          captured.actors[1].actor.source_physical_present==std::optional<bool>(false)&&
          !captured.actors[2].actor.source_physical_present,
          "Capture did not select v3 or preserve true/false/unknown without inference");

    const std::filesystem::path path(argv[2]);
    check(save_game(path,captured,error),error);
    const auto v3bytes=read_bytes(path);GameSave loaded;
    check(load_game(path,loaded,error),error);
    check(loaded.version==3&&loaded.actors[0].actor.source_physical_present==std::optional<bool>(true)&&
          loaded.actors[1].actor.source_physical_present==std::optional<bool>(false)&&
          !loaded.actors[2].actor.source_physical_present&&loaded.actors[1].actor.health==0&&
          loaded.actors[1].actor.action==CharacterAction::dead,
          "V3 disk roundtrip conflated absent body, present body, unknown, or dead actor state");

    // Older versions remain exactly actor-only and never guess body presence
    // from a dead/zero-HP actor. Their current-format bytes round-trip exactly.
    for(const std::uint32_t old_version:{1u,2u}){
        auto old=captured;old.version=old_version;old.objects.clear();
        for(auto& actor:old.actors)actor.actor.source_physical_present.reset();
        const auto oldpath=std::filesystem::path(path.string()+".v"+std::to_string(old_version));
        const auto rewritepath=std::filesystem::path(path.string()+".v"+std::to_string(old_version)+".rewrite");
        check(save_game(oldpath,old,error),error);const auto oldbytes=read_bytes(oldpath);
        GameSave reread;check(load_game(oldpath,reread,error),error);
        check(reread.version==old_version&&reread.actors[1].actor.health==0&&
              !reread.actors[0].actor.source_physical_present&&
              !reread.actors[1].actor.source_physical_present,
              "Legacy checkpoint inferred body presence from live/dead actor state");
        check(save_game(rewritepath,reread,error),error);
        check(read_bytes(rewritepath)==oldbytes,"Legacy v1/v2 bytes changed on read/write roundtrip");
        std::filesystem::remove(oldpath);std::filesystem::remove(rewritepath);
    }
    auto invalid=captured;invalid.version=2;
    check(!save_game(path,invalid,error)&&read_bytes(path)==v3bytes,
          "Known v3 body facts were silently dropped into a legacy version or damaged existing file");

    // Successful same-roster restore publishes the three-way fact together
    // with actor state and the shared random stream.
    world.find_actor(1)->source_physical_present=false;
    world.find_actor(2)->source_physical_present=true;
    world.find_actor(3)->source_physical_present=false;
    world.find_actor(1)->health=1;world.find_actor(2)->health=1;
    rng={7,3};character.name="mutated";
    check(restore_game_save(loaded,level,world,character,error),error);
    check(world.find_actor(1)->source_physical_present==std::optional<bool>(true)&&
          world.find_actor(2)->source_physical_present==std::optional<bool>(false)&&
          !world.find_actor(3)->source_physical_present&&world.find_actor(2)->health==0&&
          rng.seed==0x51a7u&&rng.calls==29&&character.name=="Test",
          "Same-roster v3 restore did not atomically restore body facts/actor/character/RNG");

    // Wrong roster, checksum, truncation, and unsupported versions must leave
    // every current actor fact, CharacterState and RNG untouched.
    const auto saved_rng=rng;const auto saved_name=character.name;
    const auto saved_true=world.find_actor(1)->source_physical_present;
    auto extra=make_actor(4,"Swamp_LizadMan_Type1",lizard);
    check(world.bind_actor(extra,lizard,{false,true,std::nullopt},error),error);
    const auto before_failure=rng;
    check(!restore_game_save(loaded,level,world,character,error)&&
          rng.seed==before_failure.seed&&rng.calls==before_failure.calls&&
          world.find_actor(1)->source_physical_present==saved_true&&character.name==saved_name,
          "Wrong-roster restore partially published actor facts, CharacterState, or RNG");

    GameSave protected_output=loaded;protected_output.level_uri="sentinel/unchanged";
    auto damaged=v3bytes;damaged[damaged.size()/2]^=1;
    const auto malformed=std::filesystem::path(path.string()+".bad");write_bytes(malformed,damaged);
    const auto world_before_bad_load=rng;const auto actor_before_bad_load=world.find_actor(1)->source_physical_present;
    check(!load_game(malformed,protected_output,error)&&protected_output.level_uri=="sentinel/unchanged"&&
          protected_output.actors[0].actor.source_physical_present==std::optional<bool>(true)&&
          rng.seed==world_before_bad_load.seed&&rng.calls==world_before_bad_load.calls&&
          world.find_actor(1)->source_physical_present==actor_before_bad_load&&character.name==saved_name,
          "Checksum failure changed caller output");
    damaged=v3bytes;damaged.resize(12);write_bytes(malformed,damaged);
    check(!load_game(malformed,protected_output,error)&&protected_output.level_uri=="sentinel/unchanged"&&
          rng.seed==world_before_bad_load.seed&&rng.calls==world_before_bad_load.calls&&
          world.find_actor(1)->source_physical_present==actor_before_bad_load&&character.name==saved_name,
          "Truncated checkpoint changed caller output or was accepted");
    damaged=v3bytes;for(unsigned i=0;i<4;++i)damaged[8+i]=static_cast<char>((4u>>(8*i))&0xffu);
    const auto invalid_hash=checksum(damaged,damaged.size()-8);
    for(unsigned i=0;i<8;++i)damaged[damaged.size()-8+i]=static_cast<char>(invalid_hash>>(8*i));
    write_bytes(malformed,damaged);
    check(!load_game(malformed,protected_output,error)&&protected_output.level_uri=="sentinel/unchanged"&&
          rng.seed==world_before_bad_load.seed&&rng.calls==world_before_bad_load.calls&&
          world.find_actor(1)->source_physical_present==actor_before_bad_load&&character.name==saved_name,
          "Unsupported on-disk version changed output or live world state");
    invalid=loaded;invalid.version=4;
    check(!restore_game_save(invalid,level,world,character,error)&&
          rng.seed==before_failure.seed&&rng.calls==before_failure.calls&&
          world.find_actor(1)->source_physical_present==saved_true&&character.name==saved_name,
          "Unsupported version changed current world/CharacterState/RNG");
    check(saved_rng.seed==0x51a7u&&saved_rng.calls==29,"Test fixture RNG changed unexpectedly");
    std::filesystem::remove(malformed);
    std::cout<<"PASS GameSave v3 true/false/unknown presence, actor-only v1/v2 byte roundtrip, strict same-roster restore, failure atomicity; no death-cursor persistence claim\n";
    return 0;
}catch(const std::exception& exception){std::cerr<<exception.what()<<'\n';return 1;}}
