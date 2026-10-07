#include "level_savegame_objects_v2.hpp"
#include <cstring>
namespace dh2::level {
namespace {bool fail(std::string& e,const char* why){if(e.empty())e=why;return false;}
bool query(const LevelSaveObjectBorrowV2& a,bool player,bool& out,std::string& e){auto fn=player?a.is_player:a.is_character;return fn?fn(a.context,out,e):fail(e,"Required same save receiver virtual type query");}}
bool level_savegame_save_objects_v2(SavegameStreamV2& stream,const LevelSaveObjectsServicesV2& s,std::string& e){
 e.clear();auto initial=stream.tell_write();std::uint32_t count=0;if(!stream.write_u32(0,e))return false;
 std::uintptr_t iterator=0;LevelSaveObjectBorrowV2 a;bool found=false;if(!s.first||!s.first(s.context,iterator,a,found,e))return fail(e,"Required same ObjectManager first iterator");
 while(found){if(a.identity){if(!a.checkpoint28)return fail(e,"Required same checkpoint-enabled byte28");if(*a.checkpoint28){bool online=false;if(!s.network_online||!s.network_online(s.context,online,e))return fail(e,"Required source Network byte5 for saved object");bool accepted=true;
   if(online){bool character=false,player=false;if(!query(a,false,character,e))return false;if(character){if(!query(a,true,player,e))return false;if(player){bool local=false;if(!s.locally_controlled||!s.locally_controlled(s.context,a.identity,local,e))return fail(e,"Required same PlayerManager IsLocallyControlled");if(!local)accepted=false;else{if(!a.disabled81)return fail(e,"Required same source disabled81");if(*a.disabled81)accepted=false;}}}}
   if(accepted){++count;if(!a.gametype48||!a.map_name||!a.room64)return fail(e,"Required same source save strings/room64");if(!stream.write_string(*a.gametype48,e))return false;
    bool character=false,player=false;if(!query(a,false,character,e))return false;if(character&&!query(a,true,player,e))return false;
    const std::string special="PlayerCharacter_0";const auto& name=(character&&player&&*a.map_name!=special)?special:*a.map_name;
    if(!stream.write_string(name,e)||!stream.write_u32(static_cast<std::uint32_t>(*a.room64),e))return false;auto length_at=stream.tell_write();if(!stream.write_u64(0,e))return false;
    if(!a.save||!a.save(a.context,stream,e))return fail(e,"Required whole same object Save virtual10");auto end=stream.tell_write();if(end<length_at+8)return fail(e,"Source object Save returned invalid write position");auto length=end-length_at-8;stream.seek_write(length_at);if(!stream.write_u64(length,e))return false;stream.seek_write(length_at+8+length);
   }
  }}if(!s.next||!s.next(s.context,iterator,a,found,e))return fail(e,"Required same ObjectManager next iterator");}
 // This source callback intentionally does not restore the ending cursor.
 stream.seek_write(initial);return stream.write_u32(count,e);
}
bool level_savegame_load_objects_v2(SavegameStreamV2& stream,LevelSavegameFieldsV1& f,const LevelSaveObjectsServicesV2& s,std::string& e){
 e.clear();if(f.initializing38)return true;std::uint32_t count=0;if(!stream.read_u32(count,e))return false;
 for(std::uint32_t i=0;i<count;++i){std::string gametype,name;std::uint32_t room;std::uint64_t length;if(!stream.read_string(gametype,e)||!stream.read_string(name,e)||!stream.read_u32(room,e)||!stream.read_u64(length,e))return false;
  auto start=stream.tell();if(length>UINT64_MAX-start||start+length>stream.size())return fail(e,"Required source saved-object payload end domain");LevelSaveObjectBorrowV2 a;
  if(name=="PlayerCharacter_0"){if(!s.player||!s.player(s.context,0,true,a,e))return fail(e,"Required same PlayerManager GetPlayer(0,true)");}
  else{std::int32_t signed_room;std::memcpy(&signed_room,&room,4);if(!s.by_name||!s.by_name(s.context,name.c_str(),signed_room,false,nullptr,a,e))return fail(e,"Required same ObjectManager GetObjectByName(name,room,false,NULL)");}
  if(a.identity){if(!a.load||!a.load(a.context,stream,e))return fail(e,"Required whole same object Load virtual14");}
  if(stream.tell()!=start+length)stream.seek(start+length);
 }
 return true;
}
}
