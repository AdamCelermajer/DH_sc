#include "authored_actor_save_migration_v1.hpp"
#include "source_can_respawn_v1.hpp"
#include <cassert>
#include <iostream>
#undef assert
#define assert(condition) do { if(!(condition)) { std::cerr << "assertion failed: " #condition << '\\n'; return 2; } } while(false)
using namespace dh::foundation;
using namespace dh::foundation::persistence::death_flow;
PersistedPlayableActor record(ActorId id,const char* def,float hp,bool dead=false) {PersistedPlayableActor p;p.actor.id=id;p.actor.definition_id=def;p.actor.health=hp;p.actor.source_dead=dead;return p;}
DeathCheckpointActorV1 deathRecord(ActorId id,const char* def){DeathCheckpointActorV1 r;r.id=id;r.definition_id=def;r.source_cs_state=3;return r;}
int main(){
 std::string e;GameSave saved;saved.version=4;saved.controlled_actor_id=1;saved.level_uri="map";
 saved.actors={record(1,"player",20),record(2,"existing",0,true)};
 auto initial=std::vector<PersistedPlayableActor>{record(1,"player",100),record(2,"existing",100),record(3,"newA",47),record(4,"newB",47)};
 assert(migrate_missing_authored_actor_records_v1(saved,initial,{3,4},e));
 assert(saved.actors.size()==4&&saved.actors[1].actor.source_dead&&saved.actors[1].actor.health==0&&saved.actors[0].actor.health==20);
 saved.actors[2].actor.health=0;saved.actors[2].actor.source_dead=true;
 assert(migrate_missing_authored_actor_records_v1(saved,initial,{3,4},e));assert(saved.actors[2].actor.source_dead&&saved.actors[2].actor.health==0);
 auto bad=saved;bad.actors.pop_back();auto before=bad.actors.size();assert(!migrate_missing_authored_actor_records_v1(bad,initial,{3},e)&&bad.actors.size()==before);
 bad=saved;bad.actors[0].actor.definition_id="other";assert(!migrate_missing_authored_actor_records_v1(bad,initial,{3,4},e)&&bad.actors[0].actor.definition_id=="other");
 bad=saved;bad.actors.push_back(record(99,"script-dynamic",1));assert(migrate_missing_authored_actor_records_v1(bad,initial,{3,4},e));assert(bad.actors.back().actor.id==99);
 assert(!migrate_missing_authored_actor_records_v1(saved,initial,{3,3},e));
 assert(!migrate_missing_authored_actor_records_v1(saved,initial,{1},e));
 assert(!migrate_missing_authored_actor_records_v1(saved,initial,{999},e));
 DeathCheckpointSnapshotV1 checkpoint;checkpoint.controlled_actor_id=1;checkpoint.checkpoint_name="old";
 checkpoint.actors={deathRecord(1,"player"),deathRecord(2,"old")};checkpoint.actors[1].source_cs_state=8;checkpoint.actors[1].source_dead=true;
 auto deathInitial=std::vector<DeathCheckpointActorV1>{deathRecord(1,"player"),deathRecord(2,"old"),deathRecord(3,"new")};
 assert(migrate_missing_authored_checkpoint_actor_records_v1(checkpoint,deathInitial,{3},e));
 assert(checkpoint.actors.size()==3&&checkpoint.actors[1].source_dead&&checkpoint.actors[1].source_cs_state==8&&checkpoint.checkpoint_name=="old");
 checkpoint.actors[2].source_dead=true;checkpoint.actors[2].source_cs_state=8;
 assert(migrate_missing_authored_checkpoint_actor_records_v1(checkpoint,deathInitial,{3},e));assert(checkpoint.actors[2].source_dead&&checkpoint.actors[2].source_cs_state==8);
 auto badCheckpoint=checkpoint;badCheckpoint.actors[2].definition_id="other";assert(!migrate_missing_authored_checkpoint_actor_records_v1(badCheckpoint,deathInitial,{3},e));
 assert(!migrate_missing_authored_checkpoint_actor_records_v1(checkpoint,deathInitial,{3,3},e));
 assert(!migrate_missing_authored_checkpoint_actor_records_v1(checkpoint,deathInitial,{1},e));
 assert(!migrate_missing_authored_checkpoint_actor_records_v1(checkpoint,deathInitial,{99},e));
 const auto gateFalseNoProperty=source_can_respawn_v1(0,std::nullopt,std::nullopt);
 const auto gateFalseSpawned=source_can_respawn_v1(5120,true,false);
 const auto gateTrueAuthoredNoGroup=source_can_respawn_v1(5120,false,false);
 const auto gateUnknownGroup=source_can_respawn_v1(5120,false,true);
 const auto gateUnknownProvenance=source_can_respawn_v1(5120,std::nullopt,false);
 assert(gateFalseNoProperty&&!*gateFalseNoProperty);
 assert(gateFalseSpawned&&!*gateFalseSpawned);
 assert(gateTrueAuthoredNoGroup&&*gateTrueAuthoredNoGroup);
 assert(!gateUnknownGroup&&!gateUnknownProvenance);
 std::cout<<"19 source save/checkpoint migration and CanRespawn branches passed\n";
}
