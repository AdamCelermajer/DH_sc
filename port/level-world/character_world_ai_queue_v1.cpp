#include "character_world_ai_queue_v1.hpp"
#include <algorithm>
#include <cstring>
namespace dh2::character {
std::shared_ptr<CharacterWorldAIQueueV1> character_ai_queue_v105(){
 static const auto owner=std::make_shared<CharacterWorldAIQueueV1>();return owner;
}
bool CharacterWorldAIQueueV1::add(std::uintptr_t ai) {
 if(!ai || std::find(actors_.begin(),actors_.end(),ai)!=actors_.end())return false;
 actors_.push_back(ai);return true;
}
bool CharacterWorldAIQueueV1::remove(std::uintptr_t ai) {
 auto it=std::find(actors_.begin(),actors_.end(),ai);
 if(it==actors_.end())return false;
 actors_.erase(it);return true;
}
bool CharacterWorldAIQueueV1::read(const WorldAIQueueServicesV1& s,std::uintptr_t ai,WorldAIQueueQueryV1 q,std::int32_t& v) {
 if(!s.query){error_="AI queue requires actual Character/controller query provider";return false;}
 if(!s.query(s.context,ai,q,v,error_)){if(error_.empty())error_="AI queue Character observation unavailable";return false;}
 return true;
}
void CharacterWorldAIQueueV1::rotate(){auto ai=actors_.front();actors_.pop_front();actors_.push_back(ai);}
bool CharacterWorldAIQueueV1::advance(std::int32_t dt,const WorldAIQueueServicesV1& s) {
 error_.clear();
 if(countdown_>0){std::uint32_t bits=static_cast<std::uint32_t>(countdown_)-static_cast<std::uint32_t>(dt);std::memcpy(&countdown_,&bits,4);return true;}
 countdown_=180;
 if(actors_.size()<=1)return true;
 std::size_t remaining=actors_.size();
 for(;;){
  rotate();if(--remaining==0)return true;
  auto ai=actors_.front();std::int32_t v;
  if(!read(s,ai,WorldAIQueueQueryV1::Forced,v))return false;
  if(!v){
   if(!read(s,ai,WorldAIQueueQueryV1::GlobalBlocked,v))return false;if(v)continue;
   if(!read(s,ai,WorldAIQueueQueryV1::Locked,v))return false;if(v)continue;
  }
  if(!read(s,ai,WorldAIQueueQueryV1::Dead,v))return false;if(v)continue;
  if(!read(s,ai,WorldAIQueueQueryV1::RemotelyUpdated,v))return false;if(v)return true;
  if(!read(s,ai,WorldAIQueueQueryV1::Byte80,v))return false;if(!v)continue;
  if(!read(s,ai,WorldAIQueueQueryV1::Zoned,v))return false;if(!v)return true;
  if(!read(s,ai,WorldAIQueueQueryV1::Byte2ee,v))return false;if(!v)return true;
  if(!read(s,ai,WorldAIQueueQueryV1::Byte2f0,v))return false;if(v)return true;
  if(!read(s,ai,WorldAIQueueQueryV1::Byte2ee,v))return false;if(!v)return true;
 }
}
bool CharacterWorldAIQueueV1::is_my_turn(std::uintptr_t ai,const WorldAIQueueServicesV1& s,bool& result) {
 error_.clear();result=false;
 if(!ai || actors_.empty() || std::find(actors_.begin(),actors_.end(),ai)==actors_.end()){
  error_="IsMyTurn requires a live constructor-registered AI and nonempty source queue";return false;
 }
 if(countdown_<=0 && actors_.front()==ai){result=true;return true;}
 std::int32_t v;
 if(!read(s,ai,WorldAIQueueQueryV1::Follower,v))return false;if(v){result=true;return true;}
 if(!read(s,ai,WorldAIQueueQueryV1::Faerie,v))return false;if(v){result=true;return true;}
 if(!read(s,ai,WorldAIQueueQueryV1::Player,v))return false;result=v!=0;return true;
}
}
