#include "../game_save.hpp"
#include "../asset_catalog.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <cstdint>
#include <cstring>
#include <utility>
using namespace dh::foundation;
static void check(bool ok,const std::string& message){if(!ok)throw std::runtime_error(message);}
static ActorState actor(ActorId id,const char* definition,const OriginalCombatProperties& props){
    ActorState state;state.id=id;state.definition_id=definition;state.faction_id=props.sheets.resolved[0];
    state.health=original_signed256(props.sheets.resolved[36]);state.max_health=original_signed256(props.sheets.resolved[38]);
    state.resource=original_signed256(props.sheets.resolved[41]);state.max_resource=original_signed256(props.sheets.resolved[43]);return state;
}
static std::vector<char> read(const std::filesystem::path& path){std::ifstream f(path,std::ios::binary);return {std::istreambuf_iterator<char>(f),{}};}
static void write(const std::filesystem::path& path,const std::vector<char>& data){std::ofstream f(path,std::ios::binary|std::ios::trunc);f.write(data.data(),std::streamsize(data.size()));}
static std::uint32_t read_u32(const std::vector<char>& bytes,std::size_t& at){
    if(at>bytes.size()||bytes.size()-at<4)throw std::runtime_error("test legacy parser truncated u32");
    std::uint32_t v=0;for(unsigned i=0;i<4;++i)v|=std::uint32_t(static_cast<unsigned char>(bytes[at++]))<<(8*i);return v;
}
static void skip_text(const std::vector<char>& bytes,std::size_t& at){
    const auto n=read_u32(bytes,at);if(n>bytes.size()-at)throw std::runtime_error("test legacy parser truncated text");at+=n;
}
static std::uint64_t checksum(const std::vector<char>& bytes){
    std::uint64_t h=14695981039346656037ull;for(char c:bytes){h^=static_cast<unsigned char>(c);h*=1099511628211ull;}return h;
}
static std::vector<char> legacy_game_save_v1(const std::vector<char>& v2){
    if(v2.size()<20)throw std::runtime_error("test game save too short");
    const auto payload_end=v2.size()-8;std::size_t at=8;
    (void)read_u32(v2,at);const auto schema_at=at;const auto schema=read_u32(v2,at);
    if(schema!=character_schema_version)throw std::runtime_error("test source save is not character schema v2");
    std::vector<std::pair<std::size_t,std::size_t>> erase_ranges;
    skip_text(v2,at);skip_text(v2,at);skip_text(v2,at);at+=4; // identity and level
    at+=7*4;erase_ranges.emplace_back(at,at+18);at+=18; // v2 source stats/points/known flags
    at+=16; // experience and gold
    auto count=read_u32(v2,at);for(std::uint32_t i=0;i<count;++i){skip_text(v2,at);skip_text(v2,at);at+=4;}
    count=read_u32(v2,at);for(std::uint32_t i=0;i<count;++i){
        skip_text(v2,at);skip_text(v2,at);erase_ranges.emplace_back(at,at+8);at+=8;
    }
    count=read_u32(v2,at);for(std::uint32_t i=0;i<count;++i){skip_text(v2,at);at+=4;}
    count=read_u32(v2,at);for(std::uint32_t i=0;i<count;++i)skip_text(v2,at);
    const auto extension_start=at;
    at+=1;count=read_u32(v2,at);at+=std::size_t(count)*12;
    at+=4+1+3*(4+5*3); // faery list, known flag, three exact five-entry saves
    if(schema>=3){const auto quest_bytes=read_u32(v2,at);at+=quest_bytes;}
    if(at>payload_end)throw std::runtime_error("test v2 extension parser overflow");
    erase_ranges.emplace_back(extension_start,at);
    std::vector<char> legacy(v2.begin(),v2.begin()+static_cast<std::ptrdiff_t>(payload_end));
    std::sort(erase_ranges.rbegin(),erase_ranges.rend());
    for(const auto& range:erase_ranges)legacy.erase(legacy.begin()+static_cast<std::ptrdiff_t>(range.first),legacy.begin()+static_cast<std::ptrdiff_t>(range.second));
    legacy[schema_at]=1;legacy[schema_at+1]=legacy[schema_at+2]=legacy[schema_at+3]=0;
    const auto hash=checksum(legacy);for(unsigned i=0;i<8;++i)legacy.push_back(static_cast<char>(hash>>(8*i)));
    return legacy;
}
int main(int argc,char** argv){try{
    if(argc!=4)throw std::runtime_error("Supply equipment root, population root, test save path");
    AssetCatalog equipment(argv[1]),population(argv[2]);std::string error;const std::string root="original-cache/data/pydata/";
    OriginalPropertyDatabase db;dh2::data::AiTables tables;
    check(load_original_property_tables(equipment,root,db,error),error);
    check(load_original_ai_tables(population,root,tables,error),error);
    const auto data=equipment.read(root+"loot_table_pyarray.bin"),names=equipment.read(root+"loot_table_pyarraynames.bin"),fields=equipment.read(root+"loot_table_pystructnames.bin");
    dh2::data::ItemTable items;check(dh2::data::load_items({data.data(),data.size()},{names.data(),names.size()},{fields.data(),fields.size()},items,error),error);
    const auto sword=items.rows.at(664).record;OriginalCombatFacts facts;
    check(original_combat_equipment_facts(&sword,nullptr,facts,error),error);
    OriginalCombatProperties knight,mob;
    check(build_original_combat_properties(db,"KnightPlayerBase",{256,true},{{sword,false,{}}},facts,knight,error),error);
    check(build_original_combat_properties(db,"Swamp_LizadMan_Type1",{std::nullopt,true},{},{},mob,error),error);
    dh2::data::CombatRandom rng{1234,0};PlayableActorWorld world(tables,rng);
    auto player=actor(1,"KnightPlayerBase",knight);player.persistent_character_id="live-player";
    player.attack_ids={"original-main-leaf"};
    player.equipment.push_back({"main",items.identifiers.at(664),std::string("sword-instance")});
    check(world.bind_actor(player,knight,{true,true,sword},error),error);
    check(world.bind_actor(actor(2,"Swamp_LizadMan_Type1",mob),mob,{false,true,std::nullopt},error),error);
    check(world.bind_source("main",{},error),error);
    auto character=make_default_character("live-player","Player","warrior");
    auto& quest_payload=character.source_quest_progress_cqpg;
    quest_payload={'C','Q','P','G'};
    const auto quest_word=[&](std::uint32_t value){for(unsigned i=0;i<4;++i)quest_payload.push_back(static_cast<std::uint8_t>(value>>(8*i)));};
    quest_word(1);quest_word(64);quest_word(static_cast<std::uint32_t>(character.id.size()));
    quest_payload.insert(quest_payload.end(),character.id.begin(),character.id.end());
    quest_payload.insert(quest_payload.end(),6,0); // Bound, explicitly unknown progress.
    character.stats.endurance=18.5f;character.stats.energy=6.25f;
    character.source_endurance_energy_known=true;
    character.source_stat_points=2;character.source_skill_points=0;character.source_points_known=true;
    character.inventory.push_back({"sword-instance",items.identifiers.at(664),1});
    character.equipment.push_back({"main","sword-instance",0,1});
    character.skills={{"BashDown",0},{"SecondSkill",3},{"BashDown",0}};
    character.skill_slots={{0,0,0},{1,0,2}};character.source_skill_slots_known=true;
    character.source_faery_list_id=1;character.source_faery_state_known=true;
    character.faery_by_difficulty[0].current_faery=2;
    character.faery_by_difficulty[0].faeries[2]={1,7};
    auto* live=world.find_actor(1);live->transform.position={13,27,4};live->action=CharacterAction::attacking;live->target_id=2;
    apply_actor_damage(*live,5.5f);apply_actor_damage(*world.find_actor(2),world.find_actor(2)->health);
    rng.seed=98765;rng.calls=117; // Explicit already-advanced test stream.
    GameSave saved;check(capture_game_save("original/level",1,character,world,saved,error),error);
    check(saved.actors[0].actor.action==CharacterAction::idle&&saved.actors[0].actor.target_id==0&&
        saved.actors[1].actor.action==CharacterAction::dead,"transient normalization/death lost");
    check(saved.actors[1].combat.sheets.resolved[36]==0,"dead enemy original HP sheet not synchronized");
    const std::filesystem::path path(argv[3]);check(save_game(path,saved,error),error);
    GameSave loaded;check(load_game(path,loaded,error),error);
    check(loaded.random.seed==98765&&loaded.random.calls==117&&loaded.actors.size()==2&&
        loaded.actors[0].combat.sheets.base==saved.actors[0].combat.sheets.base&&
        loaded.actors[0].combat.sheets.saved==saved.actors[0].combat.sheets.saved&&
        loaded.actors[0].combat.sheets.gear==saved.actors[0].combat.sheets.gear&&
        loaded.actors[0].traits.main_item->words[35]==12*256&&
        loaded.character.schema_version==character_schema_version&&
        loaded.character.source_quest_progress_cqpg==character.source_quest_progress_cqpg&&
        loaded.character.stats.endurance==18.5f&&loaded.character.stats.energy==6.25f&&
        loaded.character.source_endurance_energy_known&&loaded.character.source_points_known&&
        loaded.character.source_stat_points==2&&loaded.character.source_skill_points==0&&loaded.character.skills[0].rank==0&&
        loaded.character.skill_slots.size()==2&&loaded.character.skill_slots[1].equipment_set==1&&
        loaded.character.skill_slots[1].saved_skill_row==2&&loaded.character.skills[2].id=="BashDown"&&
        loaded.character.equipment[0].equipment_set==0&&loaded.character.equipment[0].source_slot==1&&
        loaded.character.source_faery_state_known&&loaded.character.source_faery_list_id==1&&
        loaded.character.faery_by_difficulty[0].current_faery==2&&
        loaded.character.faery_by_difficulty[0].faeries[2].level==7,
        "durable original data/RNG/source profile v2 roundtrip differs");
    const auto legacy_path=std::filesystem::path(path.string()+".legacy-v1");
    // V1 has no source-row-map marker, so use a genuinely generic unique-ID
    // character for that migration fixture; source duplicate rows are covered
    // by the v2 roundtrip above.
    const auto legacy_source_path=std::filesystem::path(path.string()+".legacy-source-v2");
    auto legacy_source=saved;legacy_source.character.skills={{"BashDown",0},{"SecondSkill",3}};
    legacy_source.character.skill_slots.clear();legacy_source.character.source_skill_slots_known=false;
    check(save_game(legacy_source_path,legacy_source,error),error);
    const auto legacy_bytes=legacy_game_save_v1(read(legacy_source_path));write(legacy_path,legacy_bytes);
    GameSave migrated;check(load_game(legacy_path,migrated,error),error);
    check(migrated.character.schema_version==character_schema_version&&
          migrated.character.id==character.id&&migrated.character.stats.intelligence==character.stats.intelligence&&
          migrated.character.stats.endurance==0&&migrated.character.stats.energy==0&&
          !migrated.character.source_endurance_energy_known&&!migrated.character.source_points_known&&
          migrated.character.source_stat_points==0&&migrated.character.source_skill_points==0&&
          !migrated.character.source_skill_slots_known&&
          !migrated.character.source_faery_state_known&&migrated.character.skill_slots.empty()&&
          migrated.character.source_quest_progress_cqpg.empty()&&
          migrated.character.source_faery_list_id==-1&&migrated.character.equipment[0].equipment_set==-1&&
          migrated.character.equipment[0].source_slot==-1,
          "legacy game save v1 fields/new-field unknown defaults differ");
    check(read(legacy_path)==legacy_bytes,"legacy GameSave read rewrote the file without explicit save");
    check(save_game(legacy_path,migrated,error),error);
    check(read(legacy_path)!=legacy_bytes,"explicit GameSave save did not write schema v2");
    std::filesystem::remove(legacy_path);
    std::filesystem::remove(legacy_source_path);
    live->health=1;live->transform.position={0,0,0};auto* enemy=world.find_actor(2);enemy->health=enemy->max_health;enemy->action=CharacterAction::idle;
    rng.seed=1;rng.calls=0;check(restore_game_save(loaded,"original/level",world,character,error),error);
    live=world.find_actor(1);check(live->transform.position[0]==13&&live->health==saved.character.stats.health&&
        world.find_actor(2)->health==0&&world.find_actor(2)->action==CharacterAction::dead&&rng.seed==98765&&rng.calls==117,
        "live pose/vitals/death/RNG restore differs");
    check(character.stats.endurance==18.5f&&character.stats.energy==6.25f&&
          character.skill_slots.size()==2&&character.skill_slots[1].saved_skill_row==2&&
          character.skills.size()==3&&character.skills[2].id=="BashDown"&&character.skills[0].rank==0&&
          character.faery_by_difficulty[0].faeries[2].level==7,
          "restore lost source profile semantic fields");
    const auto previous_file=read(path);auto invalid=saved;invalid.actors.push_back(invalid.actors.front());
    check(!save_game(path,invalid,error)&&read(path)==previous_file,"invalid overwrite damaged existing save");
    const auto previous_rng=rng;const auto previous_position=live->transform.position;const auto previous_name=character.name;
    invalid=saved;invalid.actors[0].actor.definition_id="different-original-definition";
    check(!restore_game_save(invalid,"original/level",world,character,error)&&rng.seed==previous_rng.seed&&
        world.find_actor(1)->transform.position==previous_position&&character.name==previous_name,"wrong-definition restore mutated live state");
    check(!restore_game_save(saved,"different-level",world,character,error),"different level restore accepted");
    // Simulate a rebuilt host selecting a whole authored action group instead
    // of the old leaf authorization. Failed restore must not roll it backward.
    live=world.find_actor(1);live->attack_ids={"original-main-action-group"};
    check(!restore_game_save(saved,"original/level",world,character,error)&&
        live==world.find_actor(1)&&live->attack_ids.front()=="original-main-action-group"&&
        rng.seed==previous_rng.seed&&rng.calls==previous_rng.calls&&character.name==previous_name,
        "old attack authorization replaced current binding");
    live->attack_ids=saved.actors[0].actor.attack_ids;
    auto changed_faction=knight;changed_faction.sheets.resolved[0]=mob.sheets.resolved[0];
    check(world.update_combat_properties(1,changed_faction,{true,true,sword},error),error);
    check(!restore_game_save(saved,"original/level",world,character,error)&&
        world.find_actor(1)->faction_id==mob.sheets.resolved[0]&&rng.seed==previous_rng.seed&&
        rng.calls==previous_rng.calls,"old faction replaced current binding");
    check(world.update_combat_properties(1,knight,{true,true,sword},error),error);
    // Actual alternate source item, not a fabricated record: the host's current
    // rendered weapon is now original dagger370 while saved checkpoint used sword664.
    const auto dagger=items.rows.at(370).record;OriginalCombatFacts dagger_facts;
    check(original_combat_equipment_facts(&dagger,nullptr,dagger_facts,error),error);
    OriginalCombatProperties dagger_knight;
    check(build_original_combat_properties(db,"KnightPlayerBase",{256,true},{{dagger,false,{}}},
        dagger_facts,dagger_knight,error),error);
    check(world.update_combat_properties(1,dagger_knight,{true,true,dagger},error),error);
    check(!restore_game_save(saved,"original/level",world,character,error)&&
        world.traits(1)->main_item->words[35]==dagger.words[35]&&
        world.combat_properties(1)->sheets.gear==dagger_knight.sheets.gear&&
        rng.seed==previous_rng.seed&&rng.calls==previous_rng.calls&&world.find_actor(1)->transform.position==previous_position,
        "saved sword stats applied to current dagger visual binding");
    check(world.update_combat_properties(1,knight,{true,true,sword},error),error);
    live=world.find_actor(1);live->equipment[0].definition_id=items.identifiers.at(370);
    check(!restore_game_save(saved,"original/level",world,character,error)&&
        live->equipment[0].definition_id==items.identifiers.at(370)&&rng.seed==previous_rng.seed,
        "changed equipped source reference overwritten");
    live->equipment=saved.actors[0].actor.equipment;
    check(world.update_combat_properties(1,knight,{false,true,sword},error),error);
    check(!restore_game_save(saved,"original/level",world,character,error)&&!world.traits(1)->is_player&&
        rng.seed==previous_rng.seed,"changed original actor role overwritten");
    check(world.update_combat_properties(1,knight,{true,true,sword},error),error);
    check(restore_game_save(saved,"original/level",world,character,error),error);
    const auto corrupt_path=std::filesystem::path(path.string()+".corrupt");auto damaged=previous_file;damaged[damaged.size()/2]^=1;write(corrupt_path,damaged);
    auto protected_output=loaded;check(!load_game(corrupt_path,protected_output,error)&&
        protected_output.level_uri==loaded.level_uri&&protected_output.random.seed==loaded.random.seed,"corrupt load changed output");
    damaged=previous_file;damaged.resize(11);write(corrupt_path,damaged);
    check(!load_game(corrupt_path,protected_output,error),"truncated save accepted");
    invalid=saved;invalid.version=4;check(!save_game(path,invalid,error)&&read(path)==previous_file,"unsupported version replaced valid save");
    invalid=saved;invalid.actors[0].actor.source_flags520=0x23c1;
    check(!save_game(path,invalid,error)&&read(path)==previous_file,"Transient source motion flags silently persisted/dropped over valid save");
    invalid=saved;invalid.actors[0].actor.source_target_node180=std::uintptr_t(123);
    check(!save_game(path,invalid,error)&&read(path)==previous_file,"Transient scene-node token replaced a valid durable save");
    invalid=saved;invalid.actors[0].actor.source_target_position184=std::array<float,3>{1,2,3};
    check(!save_game(path,invalid,error)&&read(path)==previous_file,"Derived scene target cache silently persisted over a valid save");
    auto invalid_actors=saved.actors;invalid_actors[1].actor.faction_id=999;
    check(!world.replace_actors(invalid_actors,{1,0},error)&&world.find_actor(2)->health==0&&rng.seed==previous_rng.seed,
        "failed transactional actor restore altered world/RNG");
    std::filesystem::remove(corrupt_path);
    std::cout<<"game_save PASS: original stat/equipment sheets, live vitals/pose, enemy death, exact RNG, transient reset, atomic failures\n";
}catch(const std::exception& ex){std::cerr<<ex.what()<<'\n';return 1;}}
