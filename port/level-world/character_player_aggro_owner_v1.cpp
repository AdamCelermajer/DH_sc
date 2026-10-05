#include "character_player_aggro_owner_v1.hpp"
#include <cstring>
#include <stdexcept>
namespace dh2::character {namespace {
std::int32_t wrap(std::uint32_t value){std::int32_t out;std::memcpy(&out,&value,4);return out;}
bool sound_valid(const PlayerAggroSoundBorrowV1&s){return s.identity&&s.ambient_31&&s.level_music_32;}
}
CharacterPlayerAggroOwnerV1::CharacterPlayerAggroOwnerV1(std::uintptr_t ais,std::uintptr_t player,PlayerAggroFieldsV1&fields,DebugSwitches&debug,const DebugFileServices24&files,PlayerAggroServicesV1 services):selected_ais_(ais),player_(player),fields_(fields),debug_(debug),files_(files),services_(services){if(!ais||!player)throw std::invalid_argument("Required actual selected AISPlayer and player");}
int CharacterPlayerAggroOwnerV1::ask(std::uint32_t op,std::uintptr_t other,PlayerAggroResponseV1&out,const char*name,std::int32_t music,std::int32_t loop,std::int32_t flag,std::int32_t fade){out={};PlayerAggroRequestV1 q{op,player_,other,name,fields_.count_d0,fields_.weight_d4,music,loop,flag,fade};if(!services_.invoke||services_.invoke(services_.context,&q,&out)){error_="Required AISPlayer aggro service "+std::to_string(op);return -2;}return 0;}
int CharacterPlayerAggroOwnerV1::tracing(std::uintptr_t other,bool before){std::uint32_t value{};if(dh2_character_debug_load(&debug_,&files_)!=1||dh2_character_debug_get(&value,&debug_,"isTracingAggroCount",&files_)!=1){error_="Required AISPlayer isTracingAggroCount Debug owner";return -2;}if(value){PlayerAggroResponseV1 ignored;if(ask(player_aggro_trace_v1,other,ignored,before?"before":"after"))return -2;}return 0;}
int CharacterPlayerAggroOwnerV1::on_aggro(std::uintptr_t other){
 error_.clear();if(!other)return -1; // AISDefault base is genuine empty.
 if(tracing(other,true))return -2;
 fields_.count_d0=wrap(std::uint32_t(fields_.count_d0)+1u);
 PlayerAggroResponseV1 r;if(ask(player_aggro_online_v1,other,r))return -2;
 if(r.word){if(ask(player_aggro_local_player_v1,other,r))return -2;if(!r.word)return 0;}
 if(ask(player_aggro_current_level_v1,other,r))return -2;auto level=r.level;
 if(ask(player_aggro_other_weight_v1,other,r))return -2;
 fields_.weight_d4=wrap(std::uint32_t(fields_.weight_d4)+std::uint32_t(r.integer));
 if(level.identity){
  if(!level.config){error_="Required exact Level LevelConfig pointer field";return -2;}
  if(!*level.config){if(ask(player_aggro_config_assert_v1,other,r))return -2;error_="Original AISPlayer reached missing Level LevelConfig assertion";return -2;}
  if(!level.config_music_enabled){error_="Required exact LevelConfig music-enabled byte";return -2;}
  if(*level.config_music_enabled){
   if(ask(player_aggro_sound_v1,other,r))return -2;auto sound=r.sound;
   if(!sound_valid(sound)){error_="Required exact VoxSoundManager fields";return -2;}
   if(*sound.ambient_31){
    if(ask(player_aggro_threshold_v1,other,r))return -2;
    if(fields_.weight_d4>=r.integer){if(ask(player_aggro_set_music_state_v1,other,r,"combat"))return -2;*sound.ambient_31=0;}
   }
  }
 }
 return tracing(other,false);
}
int CharacterPlayerAggroOwnerV1::on_deaggro(std::uintptr_t other){
 error_.clear();if(!other)return -1;
 if(tracing(other,true))return -2;
 fields_.count_d0=wrap(std::uint32_t(fields_.count_d0)-1u);
 PlayerAggroResponseV1 r;if(ask(player_aggro_online_v1,other,r))return -2;
 if(r.word){if(ask(player_aggro_local_player_v1,other,r))return -2;if(!r.word)return 0;}
 // Original calls GetCurrentLevel even though this first result is discarded.
 if(ask(player_aggro_current_level_v1,other,r))return -2;
 if(ask(player_aggro_other_weight_v1,other,r))return -2;
 fields_.weight_d4=wrap(std::uint32_t(fields_.weight_d4)-std::uint32_t(r.integer));
 if(ask(player_aggro_sound_v1,other,r))return -2;auto sound=r.sound;
 if(!sound_valid(sound)){error_="Required exact VoxSoundManager fields";return -2;}
 if(!*sound.ambient_31&&!fields_.weight_d4){
  if(ask(player_aggro_set_music_state_v1,other,r,"ambient"))return -2;
  const auto level_music=*sound.level_music_32;*sound.ambient_31=1;
  if(level_music){
   if(ask(player_aggro_current_level_v1,other,r))return -2;
   if(!r.level.identity||!r.level.music_id){error_="Required current Level music ID field";return -2;}
   const auto music=*r.level.music_id;
   if(music>=0&&ask(player_aggro_play_music_v1,other,r,nullptr,music,1,0,2000))return -2;
  }
 }
 if(!fields_.count_d0&&!*sound.level_music_32)fields_.pending_c4.clear();
 return tracing(other,false);
}
}
