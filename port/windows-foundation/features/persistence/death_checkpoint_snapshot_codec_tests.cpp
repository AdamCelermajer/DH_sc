#include <cmath>
#include "death_checkpoint_snapshot_v1.hpp"
#include "../../character_state.hpp"

#include <cstdlib>
#include <iostream>
#include <limits>
#include <stdexcept>

using namespace dh::foundation;
using namespace dh::foundation::persistence::death_flow;
namespace dh::foundation {
bool validate_game_save(const GameSave&,std::string& error){error.clear();return true;}
}
namespace {
void check(bool value,const char* message){if(!value)throw std::runtime_error(message);}
std::vector<std::uint8_t> quest_blob(const std::string& id,std::uint32_t initial_state){
    std::vector<std::uint8_t> bytes{'C','Q','P','G',1,0,0,0,1,0,0,0,
        static_cast<std::uint8_t>(id.size()),0,0,0};
    bytes.insert(bytes.end(),id.begin(),id.end());
    for(int i=0;i<6;++i){bytes.push_back(1);bytes.insert(bytes.end(),{255,255,255,255,1,0,0,0,
            static_cast<std::uint8_t>(initial_state),0,0,0});}
    return bytes;
}
DeathCheckpointSnapshotV1 sample(){
    DeathCheckpointSnapshotV1 s;s.level_uri="data/scene/003_darkwood.mlx";s.checkpoint_name="Checkpoint_A";
    s.controlled_actor_id=1;s.checkpoint_key_position={12,34,56};s.revive_position={7,8,9};
    s.qest_present=true;s.qest=quest_blob("player-fixture",1);
    DeathCheckpointActorV1 a;a.id=1;a.definition_id="Character_Hero";a.player=true;
    a.transform.position={1,2,3};a.source_cs_state=3;a.source_dead=false;s.actors.push_back(a);
    WorldObject o;o.id=42;o.name="Chest_A";o.visual.model="chest_closed";o.visual.material="oak";
    o.visual.animation="idle";o.visual.visible=true;o.state_components["container_v1"]={1,0,0};s.objects.push_back(o);
    return s;
}
struct Facts { bool unknown_body=false;bool fail_init=false;std::vector<std::string> calls; };
bool current_state(void* p,ActorId,bool& limbus,bool& prespawn,std::optional<bool>& can,
                   std::optional<bool>& physical,std::string&){
    auto& facts=*static_cast<Facts*>(p);limbus=false;prespawn=false;can=std::nullopt;
    physical=facts.unknown_body?std::nullopt:std::optional<bool>(true);return true;
}
bool apply_state(void* p,const SourceCharacterRestorePlanV1& plan,ActorState& actor,std::string& error){
    static_cast<Facts*>(p)->calls.push_back("apply-state");
    for(const auto op:plan.ordered_operations){
        if(op==SourceCharacterRestoreOpV1::restore_serialized_dead_bit)actor.action=plan.saved_dead?CharacterAction::dead:CharacterAction::idle;
        if(op==SourceCharacterRestoreOpV1::set_source_dead_true)actor.action=CharacterAction::dead;
        if(op==SourceCharacterRestoreOpV1::set_idle_zero)actor.action=CharacterAction::idle;
    }
    error.clear();return true;
}
bool initialize_qest(void* p,std::vector<std::uint8_t>& bytes,std::string& error){
    auto& facts=*static_cast<Facts*>(p);facts.calls.push_back("init-qest");
    if(facts.fail_init){error="INITQUESTS fixture failure";return false;}
    bytes=quest_blob("player-fixture",4);error.clear();return true;
}
void sync_fixture_sheets(PersistedPlayableActor& p){
    auto& r=p.combat.sheets.resolved;const auto& a=p.actor;
    r[0]=a.faction_id;r[36]=std::int32_t(std::llround(double(a.health)*256.0));r[38]=std::int32_t(std::llround(double(a.max_health)*256.0));
    r[41]=std::int32_t(std::llround(double(a.resource)*256.0));r[43]=std::int32_t(std::llround(double(a.max_resource)*256.0));
}
ActorState actor(ActorId id,const char* definition,bool player,float hp){
    ActorState a;a.id=id;a.definition_id=definition;a.class_id=player?"KnightPlayerBase":"NpcBase";
    a.faction_id=0;a.health=hp;a.max_health=100;a.resource=8;a.max_resource=20;
    a.persistent_character_id=player?std::optional<std::string>("player-fixture"):std::nullopt;
    a.source_physical_present=true;return a;
}
WorldObject object(ObjectId id,const char* name,const char* model,const char* material){
    WorldObject o;o.id=id;o.name=name;o.visual.model=model;o.visual.material=material;
    o.visual.animation="idle";o.visual.visible=true;o.transform.position={9,8,7};return o;
}
OriginalCombatProperties combat(){OriginalCombatProperties p;p.sheets.resolved[0]=0;return p;}
void add_actor(PlayableActorWorld& world,ActorState a,bool player){
    const auto props=combat();std::string error;
    check(world.bind_actor(std::move(a),props,{player,true,std::nullopt},error),"fixture source actor binds");
}
DeathCheckpointRestoreServicesV1 restore_services(Facts& f){
    DeathCheckpointRestoreServicesV1 s;s.context=&f;s.current_source_states=current_state;
    s.initialize_source_qest=initialize_qest;s.apply_source_state=apply_state;return s;
}
void test_snapshot_codec_and_strict_validation(){
    auto original=sample();std::string error;std::vector<std::uint8_t> bytes;
    check(encode_death_checkpoint_snapshot_v1(original,bytes,error),"source checkpoint snapshot encodes");
    DeathCheckpointSnapshotV1 decoded;
    check(decode_death_checkpoint_snapshot_v1(bytes,decoded,error),"source checkpoint snapshot decodes");
    check(decoded.level_uri==original.level_uri&&decoded.checkpoint_name==original.checkpoint_name&&
          decoded.controlled_actor_id==1&&decoded.checkpoint_key_position==original.checkpoint_key_position&&
          decoded.revive_position==original.revive_position&&decoded.qest==original.qest&&
          decoded.actors.size()==1&&decoded.actors[0].source_cs_state==3&&
          decoded.actors[0].source_dead==false&&decoded.objects.size()==1&&
          decoded.objects[0].visual.animation=="idle"&&decoded.objects[0].state_components==original.objects[0].state_components,
          "round trip retains checkpoint key/revive, QEST, CS/dead and object visual/state payloads");
    bytes.back()^=0x80;
    check(!decode_death_checkpoint_snapshot_v1(bytes,decoded,error),"checksum corruption is rejected");
    auto duplicate=original;duplicate.actors.push_back(duplicate.actors.front());
    check(!validate_death_checkpoint_snapshot_v1(duplicate,error),"duplicate source actor identity is rejected");
    auto invalidQest=original;invalidQest.qest_present=false;
    check(!validate_death_checkpoint_snapshot_v1(invalidQest,error),"QEST bytes cannot be smuggled when QEST is absent");
    auto invalidTransform=original;invalidTransform.actors[0].transform.position[0]=std::numeric_limits<float>::infinity();
    check(!validate_death_checkpoint_snapshot_v1(invalidTransform,error),"nonfinite source transform is rejected");
}
void test_capture_applies_exact_source_checkpoint_families(){
    GameSave game;game.version=4; // current save format (objects need >= 2; physical assignments need >= 3)
    game.level_uri="data/scene/003_darkwood.mlx";game.controlled_actor_id=1;
    game.character.id="player-fixture";game.character.name="Death Checkpoint Test";game.character.class_id="KnightPlayerBase";
    game.character.source_quest_progress_cqpg=quest_blob("player-fixture",1);game.character.gold=444;game.character.inventory.push_back({"sword","Sword",1});
    game.character.stats.health=51;game.character.stats.max_health=100;game.character.stats.resource=8;game.character.stats.max_resource=20; // mirror the controlled Character vitals
    PersistedPlayableActor hero;hero.actor=actor(1,"KnightPlayerBase",true,51);hero.traits.is_player=true;game.actors.push_back(hero);
    PersistedPlayableActor npc;npc.actor=actor(2,"Bandit",false,31);game.actors.push_back(npc);
    PersistedPlayableActor zoneActor;zoneActor.actor=actor(3,"AreaZone",false,1);game.actors.push_back(zoneActor);
    for(auto& p:game.actors)sync_fixture_sheets(p); // original sheets must mirror live vitals (validate_game_save)
    auto savedChest=object(10,"Chest","chest","old-material");savedChest.state_components["container_v1"]={1,2};game.objects.push_back(savedChest);
    auto skippedModule=object(11,"SkippedModule","module","stone");game.objects.push_back(skippedModule);
    auto areaZone=object(12,"TriggerZone","zone","none");game.objects.push_back(areaZone);
    struct CaptureFacts{};CaptureFacts ctx;
    const auto actorFacts=[](void*,ActorId id,SourceCheckpointObjectFamilyV1& family,bool& skip,std::string&){
        family=id==3?SourceCheckpointObjectFamilyV1::area_zone:SourceCheckpointObjectFamilyV1::character;skip=false;return true;};
    const auto actorState=[](void*,ActorId id,std::uint32_t& state,bool& dead,std::string&){state=id+2;dead=id==2;return true;};
    const auto objectFacts=[](void*,ObjectId id,SourceCheckpointObjectFamilyV1& family,bool& skip,std::string&){
        family=id==10?SourceCheckpointObjectFamilyV1::container:
               id==11?SourceCheckpointObjectFamilyV1::module:SourceCheckpointObjectFamilyV1::area_zone;
        skip=id==11;return true;};
    DeathCheckpointCaptureServicesV1 providers{&ctx,actorFacts,actorState,objectFacts};
    DeathCheckpointSnapshotV1 snapshot;std::string error;
    check(capture_death_checkpoint_snapshot_v1(game,game.level_uri,"Checkpoint_A",{12,34,56},{7,8,9},providers,snapshot,error),
          "source checkpoint capture accepts exact family facts");
    check(snapshot.actors.size()==2&&snapshot.actors[0].id==1&&snapshot.actors[0].source_cs_state==3&&
          snapshot.actors[1].id==2&&snapshot.actors[1].source_dead&&snapshot.objects.size()==1&&
          snapshot.objects[0].id==10&&snapshot.qest==quest_blob("player-fixture",1),
          "capture includes eligible Characters/Containers, excludes generic Zone and skip_checkpoint module, and keeps QEST only");
    check(snapshot.qest!=std::vector<std::uint8_t>{}&&game.character.gold==444&&game.character.inventory.size()==1,
          "checkpoint projection does not mutate or copy profile gold/inventory into the subset");
}
void test_restore_same_world_identity_and_profile_boundaries(){
    dh2::data::AiTables tables;tables.rows.resize(51);tables.factions.resize(11);
    dh2::data::CombatRandom random{123,7};PlayableActorWorld world(tables,random);
    add_actor(world,actor(1,"KnightPlayerBase",true,17),true);
    add_actor(world,actor(2,"Bandit",false,24),false);
    add_actor(world,actor(3,"Untracked",false,33),false);
    auto current=object(10,"Chest","chest","current-material");current.visual.animation="open";current.visual.visible=false;
    std::string error;check(world.bind_object(current,error),"matched checkpoint object binds");
    check(world.bind_object(object(12,"CurrentOnly","rock","rock"),error),"unsaved existing object binds");
    auto snapshot=sample();snapshot.actors.clear();snapshot.objects.clear();
    DeathCheckpointActorV1 savedPlayer;savedPlayer.id=1;savedPlayer.definition_id="KnightPlayerBase";
    savedPlayer.player=true;savedPlayer.source_cs_state=3;savedPlayer.source_dead=false;
    savedPlayer.transform.position={1,2,3};snapshot.actors.push_back(savedPlayer);
    DeathCheckpointActorV1 savedNpc;savedNpc.id=2;savedNpc.definition_id="Bandit";
    savedNpc.source_cs_state=3;savedNpc.npc_vitals_present=true;savedNpc.health=6;savedNpc.max_health=50;
    savedNpc.resource=2;savedNpc.max_resource=10;savedNpc.transform.position={4,5,6};snapshot.actors.push_back(savedNpc);
    DeathCheckpointActorV1 missingActor;missingActor.id=4;missingActor.definition_id="Missing";
    missingActor.source_cs_state=3;missingActor.transform.position={100,100,100};snapshot.actors.push_back(missingActor);
    auto savedChest=object(10,"Chest","chest","checkpoint-material");
    savedChest.visual.animation="closed";savedChest.visual.visible=true;savedChest.transform.position={20,30,40};
    savedChest.state_components["container_v1"]={3,4,5};snapshot.objects.push_back(savedChest);
    snapshot.objects.push_back(object(11,"MissingChest","chest","wood"));

    auto* livePlayer=world.find_actor(1);livePlayer->equipment.push_back({"main","SourceSword",std::string("owned-sword")});
    livePlayer->attack_ids={"attack-main"};
    const auto playerBefore=*livePlayer;const auto actorCount=world.actors().size();
    const auto objectCount=world.objects().size();const auto rngBefore=world.random_state();
    CharacterState character=make_default_character("player-fixture","Checkpoint Hero","KnightPlayerBase");
    character.gold=712;character.experience=8934;character.inventory.push_back({"owned-sword","SourceSword",1});
    character.equipment.push_back({"main","owned-sword",0,0});character.skills.push_back({"Slash",3});
    character.source_quest_progress_cqpg={77,77};
    Facts facts;auto services=restore_services(facts);DeathCheckpointRestoreReceiptV1 receipt;
    if(!restore_death_checkpoint_snapshot_v1(snapshot,snapshot.level_uri,world,character,services,receipt,error))
        throw std::runtime_error("same-identity checkpoint restore failed: "+error);
    check(receipt.complete&&receipt.actors_restored==2&&receipt.actors_missing_skipped==1&&
          receipt.objects_restored==1&&receipt.objects_missing_skipped==1&&receipt.qest_restored,
          "receipt reports same-name matches and skips missing checkpoint identities");
    if(world.actors().size()!=actorCount||world.objects().size()!=objectCount||!world.find_actor(3)||!world.find_object(12))
        throw std::runtime_error("world entity mismatch actor_ids="+[&](){std::string ids;for(const auto& a:world.actors())ids+=std::to_string(a.first)+",";return ids;}()+" actors="+std::to_string(world.actors().size())+"/"+
            std::to_string(actorCount)+" objects="+std::to_string(world.objects().size())+"/"+
            std::to_string(objectCount)+" actor3="+std::to_string(world.find_actor(3)!=nullptr)+
            " object12="+std::to_string(world.find_object(12)!=nullptr));
    check(world.random_state().seed==rngBefore.seed&&world.random_state().calls==rngBefore.calls,
          "restore preserves world RNG/procedural stream");
    const auto* restoredPlayer=world.find_actor(1);const auto* restoredNpc=world.find_actor(2);
    check(restoredPlayer->transform.position==std::array<float,3>{1,2,3}&&
          restoredPlayer->health==playerBefore.health&&restoredPlayer->max_health==playerBefore.max_health&&
          restoredPlayer->resource==playerBefore.resource&&restoredPlayer->equipment.size()==1&&
          restoredPlayer->attack_ids==playerBefore.attack_ids,
          "player checkpoint pose/state restore preserves live HP/MP, gear and attack/profile bindings");
    check(restoredNpc->transform.position==std::array<float,3>{4,5,6}&&restoredNpc->health==6&&
          restoredNpc->max_health==50&&restoredNpc->resource==2&&restoredNpc->max_resource==10,
          "source non-player Character restores saved pose and source vitals");
    const auto* chest=world.find_object(10);
    check(chest->transform.position==std::array<float,3>{20,30,40}&&
          chest->visual.material=="checkpoint-material"&&chest->visual.animation=="closed"&&
          chest->visual.visible&&chest->state_components.at("container_v1")==std::vector<std::uint8_t>{3,4,5},
          "matched world object restores transform, complete visual and feature state in place");
    check(character.gold==712&&character.experience==8934&&character.inventory.size()==1&&
          character.equipment.size()==1&&character.skills.size()==1&&
          character.source_quest_progress_cqpg==snapshot.qest,
          "QEST restore is the only CharacterState field replaced by serialized checkpoint data");

    auto noQest=snapshot;noQest.qest_present=false;noQest.qest.clear();
    character.source_quest_progress_cqpg={88,88};
    check(restore_death_checkpoint_snapshot_v1(noQest,noQest.level_uri,world,character,services,receipt,error)&&
          character.source_quest_progress_cqpg==quest_blob("player-fixture",4),
          "missing QEST runs source INITQUESTS and does not preserve mutated live quest bytes");

    const auto beforeFailureActor=*world.find_actor(1);const auto beforeFailureCharacter=character;
    auto mismatch=snapshot;mismatch.objects[0].visual.model="different-model";
    check(!restore_death_checkpoint_snapshot_v1(mismatch,mismatch.level_uri,world,character,services,receipt,error)&&
          world.find_actor(1)->transform.position==beforeFailureActor.transform.position&&
          character.gold==beforeFailureCharacter.gold&&
          character.source_quest_progress_cqpg==beforeFailureCharacter.source_quest_progress_cqpg,
          "identity mismatch fails before live world/profile mutation");
    Facts initFailure;initFailure.fail_init=true;auto brokenServices=restore_services(initFailure);
    check(!restore_death_checkpoint_snapshot_v1(snapshot,snapshot.level_uri,world,character,brokenServices,receipt,error)&&
          world.find_actor(1)->transform.position==beforeFailureActor.transform.position,
          "INITQUESTS provider failure leaves live world unchanged");
    Facts unknownBody;unknownBody.unknown_body=true;auto unknownServices=restore_services(unknownBody);
    check(!restore_death_checkpoint_snapshot_v1(snapshot,snapshot.level_uri,world,character,unknownServices,receipt,error)&&
          world.find_actor(1)->transform.position==beforeFailureActor.transform.position,
          "saved-alive Character restore fails closed when current physical-body presence is unknown");
}
}
int main(){try{test_snapshot_codec_and_strict_validation();test_capture_applies_exact_source_checkpoint_families();test_restore_same_world_identity_and_profile_boundaries();std::cout<<"PASS: source checkpoint capture families, subset codec, same-identity restore, QEST reset, profile/RNG preservation and failure atomicity\n";}
catch(const std::exception& e){std::cerr<<e.what()<<'\n';return EXIT_FAILURE;}return EXIT_SUCCESS;}
