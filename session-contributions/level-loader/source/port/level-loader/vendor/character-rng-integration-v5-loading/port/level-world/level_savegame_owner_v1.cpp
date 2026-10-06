#include "level_savegame_owner_v1.hpp"
#include <cstdio>
namespace dh2::level {
namespace {struct Guard{bool& busy;explicit Guard(bool& b):busy(b){busy=true;}~Guard(){busy=false;}};}
bool LevelSavegameOwnerV1::fail(std::string& e,const char* text)const{if(e.empty())e=text;return false;}
std::string LevelSavegameOwnerV1::filename(std::uint32_t seed,std::int32_t difficulty,std::int32_t row,std::int32_t mode){
 char out[1024];std::snprintf(out,sizeof(out),"%s%03u_%01u_%03u_%03u%s","dh2_",seed,static_cast<unsigned>(mode),static_cast<unsigned>(difficulty<0?0:difficulty),static_cast<unsigned>(row<0?0:row),"_level.savegame");return out;
}
std::string LevelSavegameOwnerV1::checkpoint_filename(std::uint32_t seed,std::int32_t mode,bool alternate){
 char out[128];std::snprintf(out,sizeof(out),"%s%03u_%01u%s%s","dh2_",seed,static_cast<unsigned>(mode),alternate?"_multi":"_single","_level.checkpoint");return out;
}
bool LevelSavegameOwnerV1::construct(const LevelSavegameRequestV1& q,std::string& e){
 e.clear();if(attempted_||busy_)return fail(e,"LevelSavegame C1 cannot replay");
 if(!q.level)return fail(e,"Required same Level identity");
 attempted_=true;Guard guard(busy_);
 fields_.level8=q.level;fields_.mode0c=q.mode;fields_.string10.clear();fields_.row28=q.row;
 fields_.loaded_row2c=fields_.field30=fields_.field34=-1;fields_.initializing38=1;fields_.inhibit_save39=0;phase_=1;
 filename_=q.checkpoint?checkpoint_filename(q.seed,q.mode,false):filename(q.seed,q.difficulty,q.row,q.mode);phase_=2;
 std::shared_ptr<void> constructed;
 if(!services_.construct_cache||!services_.construct_cache(services_.context,filename_,false,constructed,e)||!constructed)return fail(e,"Required source Savegame C1/cache-file owner");
 cache_=std::move(constructed);phase_=3; // source publication is after nested C1
 if(!services_.register_section||!services_.register_section(services_.context,cache_.get(),"INFO",LevelSavegameSectionV1::info,fields_,e))return fail(e,"Required source INFO section registration");phase_=4;
 if(!services_.register_section(services_.context,cache_.get(),"OBJS",LevelSavegameSectionV1::objects,fields_,e))return fail(e,"Required source OBJS section registration");phase_=5;
 fields_.initializing38=0;ready_=true;phase_=6;return true;
}
bool LevelSavegameOwnerV1::load(std::string& e){
 e.clear();if(!ready_||busy_||released_)return fail(e,"Required ready LevelSavegame Load owner");Guard guard(busy_);
 if(!services_.load_section||!services_.load_section(services_.context,cache_.get(),"INFO",LevelSavegameSectionV1::info,fields_,e))return fail(e,"Required source INFO load");
 if(!services_.load_section(services_.context,cache_.get(),"OBJS",LevelSavegameSectionV1::objects,fields_,e))return fail(e,"Required source OBJS load");return true;
}
bool LevelSavegameOwnerV1::save(std::string& e){
 e.clear();if(busy_||released_)return fail(e,"LevelSavegame Save owner is unavailable");
 if(!cache_||fields_.inhibit_save39)return true;Guard guard(busy_);
 bool online=false;if(!services_.network_online||!services_.network_online(services_.context,online,e))return fail(e,"Required Network byte5");
 if(online){bool hosting=false;if(!services_.is_hosting||!services_.is_hosting(services_.context,hosting,e))return fail(e,"Required same PlayerManager IsHosting");if(!hosting)return true;
  std::uint8_t flag=0;if(!services_.manager_flag719||!services_.manager_flag719(services_.context,flag,e))return fail(e,"Required same PlayerManager byte719");if(flag)return true;}
 if(!services_.save_all||!services_.save_all(services_.context,cache_.get(),e))return fail(e,"Required source Savegame saveAll/jobs/file owner");return true;
}
bool LevelSavegameOwnerV1::release(std::string& e){
 e.clear();if(busy_)return fail(e,"LevelSavegame lifetime is busy");if(released_)return true;Guard guard(busy_);
 if(cache_){if(!services_.destroy_cache||!services_.destroy_cache(services_.context,cache_.get(),e))return fail(e,"Required source Savegame destructor/job lifetime");cache_.reset();}
 fields_.string10.clear();ready_=false;released_=true;return true;
}
}
