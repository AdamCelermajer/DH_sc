#include "level_savegame_runtime_v1.hpp"
namespace dh2::level {
LevelSavegameRuntimeV1::LevelSavegameRuntimeV1(LevelSavegameApplicationV1 a):application_(a),owner_({this,create,section,load,online,hosting,flag,save,destroy,checkpoint_valid}){}
bool LevelSavegameRuntimeV1::same_cache(void* p,std::string& e)const{auto c=cache_.lock();if(!c||c.get()!=p){e="Required same retained Level Savegame cache";return false;}return true;}
bool LevelSavegameRuntimeV1::create(void* p,const std::string& f,bool raw,std::shared_ptr<void>& out,std::string& e){auto& t=*static_cast<LevelSavegameRuntimeV1*>(p);auto c=std::make_shared<LevelSavegameCacheV1>(t.application_.files);if(!c->construct(f,raw,e))return false;t.cache_=c;out=std::move(c);return true;}
bool LevelSavegameRuntimeV1::section(void* p,void* c,const char* n,LevelSavegameSectionV1 s,LevelSavegameFieldsV1& f,std::string& e){auto& t=*static_cast<LevelSavegameRuntimeV1*>(p);return t.same_cache(c,e)&&static_cast<LevelSavegameCacheV1*>(c)->register_section(n,s,f,e);}
bool LevelSavegameRuntimeV1::load(void* p,void* c,const char* n,LevelSavegameSectionV1 s,LevelSavegameFieldsV1& f,std::string& e){auto& t=*static_cast<LevelSavegameRuntimeV1*>(p);return t.same_cache(c,e)&&static_cast<LevelSavegameCacheV1*>(c)->load_section(n,s,f,e);}
bool LevelSavegameRuntimeV1::online(void* p,bool& v,std::string& e){auto& a=static_cast<LevelSavegameRuntimeV1*>(p)->application_;if(!a.network_online){e="Required source Network byte5";return false;}return a.network_online(a.context,v,e);}
bool LevelSavegameRuntimeV1::hosting(void* p,bool& v,std::string& e){auto& a=static_cast<LevelSavegameRuntimeV1*>(p)->application_;if(!a.is_hosting){e="Required same source PlayerManager IsHosting";return false;}return a.is_hosting(a.context,v,e);}
bool LevelSavegameRuntimeV1::flag(void* p,std::uint8_t& v,std::string& e){auto& a=static_cast<LevelSavegameRuntimeV1*>(p)->application_;if(!a.manager_flag719){e="Required same source PlayerManager byte719";return false;}return a.manager_flag719(a.context,v,e);}
bool LevelSavegameRuntimeV1::save(void* p,void* c,std::string& e){auto& t=*static_cast<LevelSavegameRuntimeV1*>(p);if(!t.same_cache(c,e))return false;if(!t.application_.save_all){e="Required whole source Savegame saveAll serialization/jobs/file delivery";return false;}return t.application_.save_all(t.application_.context,*static_cast<LevelSavegameCacheV1*>(c),t.owner_.fields(),e);}
bool LevelSavegameRuntimeV1::checkpoint_valid(void* p,void* c,LevelSavegameFieldsV1& fields,std::uint32_t seed,std::int32_t difficulty,std::int32_t row,bool& valid,std::string& e){
 auto& t=*static_cast<LevelSavegameRuntimeV1*>(p);
 if(!t.same_cache(c,e)||&fields!=&t.owner_.fields()){if(e.empty())e="Required SAME checkpoint cache/fields";return false;}
 if(!t.application_.validate_checkpoint){e="Required genuine Main ValidateCheckpoint filename/cache/INFO primitive";return false;}
 return t.application_.validate_checkpoint(t.application_.context,*static_cast<LevelSavegameCacheV1*>(c),fields,seed,difficulty,row,valid,e);
}
bool LevelSavegameRuntimeV1::destroy(void* p,void* c,std::string& e){auto& t=*static_cast<LevelSavegameRuntimeV1*>(p);if(!t.same_cache(c,e))return false;t.cache_.reset();return true;}
}
