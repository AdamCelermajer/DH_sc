#include "../character_world_player_aggro_v2.hpp"
#include "../level_config_music_owner_v1.hpp"
#include <fstream>
#include <iterator>
#include <iostream>
#include <cassert>
using namespace dh2::character;
static int missing(void*,const char*,std::uintptr_t* out){*out=0;return 0;}
static int close_missing(void*,std::uintptr_t){return -1;}
struct World {
 AIEventOwner48 player_owner{101,0,0,0,0,0,0,0},npc_owner{202,0,0,0,0,0,0,0};
 std::uintptr_t player_methods[51]{},npc_methods[51]{};
 AIEventState64 player{1001,&player_owner,nullptr,1002,player_methods,0,0,0,0,0,0};
 AIEventState64 npc{2001,&npc_owner,nullptr,2002,npc_methods,0,0,0,0,0,0};
 std::uintptr_t config{};const std::uint8_t* enabled{};std::int32_t music_id=-1,weight=7,threshold=1;
 unsigned weight_calls{},level_calls{},trace_calls{};
 World(){player_methods[0x38/4]=0x3ddb70;npc_methods[0x38/4]=0x3dbea4;player_methods[0x3c/4]=0x3dde48;npc_methods[0x3c/4]=0x3dbea8;}
 static int events(void* p,std::uintptr_t id,AIEventState64** out){auto& w=*static_cast<World*>(p);*out=id==101?&w.player:id==202?&w.npc:nullptr;return *out?0:-1;}
 static int online(void*,bool* out){*out=false;return 0;}
 static int level(void* p,PlayerAggroLevelBorrowV1* out){auto& w=*static_cast<World*>(p);++w.level_calls;*out={9001,&w.config,w.enabled,&w.music_id};return 0;}
 static int weight_read(void* p,std::uintptr_t id,std::int32_t* out){auto& w=*static_cast<World*>(p);assert(id==202);++w.weight_calls;*out=w.weight;return 0;}
 static int threshold_read(void* p,std::int32_t* out){*out=static_cast<World*>(p)->threshold;return 0;}
};
int main(int argc,char** argv){assert(argc==2);std::ifstream f(argv[1],std::ios::binary);std::vector<unsigned char> bytes{std::istreambuf_iterator<char>(f),{}};std::string error;
 auto config=dh2::world::LevelConfigMusicOwnerV1::load({bytes.data(),bytes.size()},error);assert(config);
 World w;w.config=config->identity();w.enabled=config->combat_music_enabled();
 auto* debug=dh2_character_debug_create();assert(debug);DebugFileServices24 files{nullptr,missing,close_missing};
 dh2::sound::VoxMusicFieldsV1 sound_fields;dh2::sound::VoxMusicStateOwnerV1 sound(sound_fields,{});PlayerAggroFieldsV1 selected;
 WorldPlayerAggroServicesV2 services{&w,World::events,World::online,nullptr,World::level,World::weight_read,World::threshold_read,nullptr,nullptr,nullptr};
 CharacterWorldPlayerAggroV2 owner({101,&w.player,&selected,&sound},services,*debug,files);
 assert(!owner.notify(dh2::data::aggro_notify_target,202,101));
 assert(selected.count_d0==1&&selected.weight_d4==7&&sound_fields.ambient_31==0&&sound_fields.current_music_24==-1);
 assert(w.weight_calls==1&&w.level_calls==1); // Actual target Player, other NPC.
 assert(!owner.notify(dh2::data::aggro_notify_target,101,202)); // Genuine inherited bx lr.
 assert(selected.count_d0==1&&w.weight_calls==1);
 // Whole death-driven reciprocal notification returns the same selected
 // player's counters/music to ambient, not a duplicate threat HUD projection.
 selected.pending_c4={4,7,9};
 assert(!owner.notify(dh2::data::aggro_notify_target_cleared,202,101));
 assert(selected.count_d0==0&&selected.weight_d4==0&&selected.pending_c4.empty());
 assert(sound_fields.ambient_31==1&&w.weight_calls==2&&w.level_calls==2);
 assert(!owner.notify(dh2::data::aggro_notify_target_cleared,101,202));
 assert(selected.count_d0==0&&w.weight_calls==2); // Exact NPC inherited bx lr.
 assert(!owner.notify(dh2::data::aggro_notify_target,202,101));
 assert(owner.notify(dh2::data::aggro_clear_target,202,101)==-2);
 assert(owner.notify(dh2::data::aggro_notify_target,202,999)==-2);
 // Reached real audio requirement cannot be accepted: counter/weight prefix
 // survives, source ambient store does not execute after SetMusicState fails.
 sound_fields.ambient_31=1;sound_fields.current_music_24=5;
 assert(owner.notify(dh2::data::aggro_notify_target,202,101)<0);
 assert(selected.count_d0==2&&selected.weight_d4==14&&sound_fields.ambient_31==1);
 assert(owner.error().find("Vox music service 1")!=std::string::npos);
 // The AI identity is distinct from Character. A corrupted Character owner
 // cannot be accepted merely because its event table/selected AIS is valid.
 w.player_owner.owner=1001;assert(owner.notify(dh2::data::aggro_notify_target,202,101)==-2);
 assert(selected.count_d0==2);dh2_character_debug_destroy(debug);
 // A source row word with its high bits set remains signed after the raw
 // GetCharAI row+14 load. This fixture proves signed policy, not a row default.
 w.player_owner.owner=101;w.weight=-1;selected.count_d0=selected.weight_d4=0;
 sound_fields.ambient_31=1;sound_fields.current_music_24=5;
 debug=dh2_character_debug_create();assert(debug);
 CharacterWorldPlayerAggroV2 negative({101,&w.player,&selected,&sound},services,*debug,files);
 assert(!negative.notify(dh2::data::aggro_notify_target,202,101));
 assert(selected.count_d0==1&&selected.weight_d4==-1&&sound_fields.ambient_31==1);
 dh2_character_debug_destroy(debug);
 std::cout<<"{\"validation\":\"PASS\",\"checks\":23,\"actual_config\":true,\"source_no_track_branch\":true,\"whole_OnDeAggro\":true,\"same_selected_AIS_counters\":true,\"positive_audio\":false}\n";
}
