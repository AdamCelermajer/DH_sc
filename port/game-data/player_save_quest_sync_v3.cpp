#include "player_save_quest_sync_v3.hpp"
namespace dh2::data {
namespace {bool required(bool present,const char* text,std::string& e){if(present)return true;e=std::string("Required original Save quest sync ")+text;return false;}}
bool PlayerSaveQuestSyncOwnerV3::try_sync(const PlayerSaveQuestSyncServicesV3& s,std::string& e){
 if(!required(bool(s.owner)&&bool(s.online),"GetOnline owner",e))return false;
 bool online=false;if(!s.online(online,e))return false;
 if(!online){save_.quest_sync_ready14_=1;return true;} // whole original offline leaf467a04/0c
 if(!required(bool(s.is_local_player),"IsLocalPlayer",e))return false;
 bool local=false;if(!s.is_local_player(save_.character(),local,e))return false;
 if(!local){if(!required(bool(s.nonlocal_assert),"nonlocal Debug assertion",e)||!s.nonlocal_assert(e))return false;}
 if(save_.quest_sync_ready14_)return true;
 if(!required(bool(s.local_hosting),"IsLocalPlayerHosting",e))return false;
 bool host=false;if(!s.local_hosting(host,e))return false;
 if(host){if(!required(bool(s.pack_send_volatile_quests),"volatile Pack/Send message body",e)||!s.pack_send_volatile_quests(save_,e))return false;save_.quest_sync_ready14_=1;return required(bool(s.destroy_quest_stream),"StreamBuffer destructor",e)&&s.destroy_quest_stream(e);}
 if(!required(bool(s.current_level),"GetCurrentLevel",e))return false;
 std::uintptr_t level=0;std::int32_t phase=0;if(!s.current_level(level,phase,e))return false;
 if(!required(bool(s.hosting_ready710),"PlayerManager hosting byte710",e))return false;
 std::uint8_t ready=0;if(!s.hosting_ready710(ready,e))return false;
 if(!ready||!level||phase!=38)return true;
 return required(bool(s.receive_quest_sync),"ReceiveQuestSync",e)&&s.receive_quest_sync(save_,e);
}
bool PlayerSaveQuestSyncOwnerV3::unpack_sync(const PlayerSaveQuestUnpackServicesV3& s,std::string& e){
 if(!required(bool(s.owner)&&bool(s.initialize_volatile_quests),"volatile InitQuests",e)||!s.initialize_volatile_quests(save_,e))return false;
 if(!required(bool(s.seek_stream_zero),"StreamBuffer Seek(0)",e)||!s.seek_stream_zero(e))return false;
 if(!required(bool(s.unpack_volatile_quests),"volatile UnpackQuests",e)||!s.unpack_volatile_quests(save_,true,e))return false;
 if(!required(bool(s.compile_volatile_quests),"volatile CompileQuests",e)||!s.compile_volatile_quests(save_,true,e))return false;
 save_.quest_sync_ready14_=1;return true;
}
}
