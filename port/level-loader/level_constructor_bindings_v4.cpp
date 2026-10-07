#include "level_constructor_bindings_v4.hpp"
namespace dh2::loader {
namespace {
template<class T>bool same(const std::shared_ptr<void>& supplied,const std::shared_ptr<T>& actual){
 return supplied&&actual&&supplied.get()==actual.get()&&!supplied.owner_before(actual)&&!actual.owner_before(supplied);
}
}
std::shared_ptr<LevelConstructorBindingsV4> LevelConstructorBindingsV4::create(LevelConstructorApplicationV4 a,std::string& error){
 if(!a.owner||!a.debug_level_load_count||!a.module_id_global||!a.levels||!a.lua_cache||!a.private_vm_limit){error="Required actual Application Level/Lua cache/static owners";return {};}
 error.clear();return std::shared_ptr<LevelConstructorBindingsV4>(new LevelConstructorBindingsV4(std::move(a)));
}
LevelConstructorServicesV3 LevelConstructorBindingsV4::services(){
 auto self=shared_from_this();auto& a=application_;
 LevelConstructorServicesV3 s;s.application=a.owner;s.debug_level_load_count=a.debug_level_load_count;s.module_id_global=a.module_id_global;s.levels=a.levels;
 s.online_byte5=a.online_byte5;s.local_player_hosting=a.local_player_hosting;s.player_manager_byte719=a.player_manager_byte719;s.online_state34=a.online_state34;s.matching_is_host=a.matching_is_host;
 s.construct_script=[self](std::uintptr_t level_identity,bool deferred,std::shared_ptr<void>& out,std::string& error){
  if(!level_identity||self->script_attempted_){error="Same Level LuaScript constructor already attempted or invalid";return false;}
  self->script_attempted_=true;
  auto unique=scripts::GenericLuaScriptOwnerV13::create(deferred,self->application_.lua_cache,self->application_.lua,self->application_.private_vm_limit,error);
  if(!unique)return false;
  std::shared_ptr<scripts::GenericLuaScriptOwnerV13> script(std::move(unique));self->script_=script;out=std::move(script);return true;
 };
 s.script_assign_path=[self](const auto& supplied,const char* path,std::size_t length,std::string& error){
  auto script=self->script();if(!same(supplied,script)||!path){error="Required SAME retained Level LuaScript path receiver";return false;}
  return script->assign_path(std::string(path,length),error);
 };
 s.script_load=[self](const auto& supplied,const char* path,std::string& error){
  auto script=self->script();if(!same(supplied,script)){error="Required SAME retained Level LuaScript Load receiver";return false;}
  bool source_loaded{};
  // Level C1 ignores LuaScript::Load's original result. Missing files or Lua
  // errors may yield source_loaded=false; mandatory native delivery failure is
  // separate and must not be swallowed by that original ignored result.
  return script->load(path,source_loaded,error);
 };
 s.allocate_save=[self](auto& allocation,std::string& error){
  if(self->save_attempted_){error="Same LevelSavegame allocation already attempted";return false;}
  self->save_attempted_=true;
  if(!self->application_.saves.files.storage_lease){error="Required retained actual FileManager provider storage";return false;}
  auto save=std::make_shared<level::LevelSavegameRuntimeV1>(self->application_.saves);self->save_=save;
  self->pending_save_v114_=save;allocation=std::move(save);return true;
 };
 s.construct_save=[self](const auto& allocation,const LevelSaveConstructionV3& request,std::string& error){
  auto save=self->save();if(!same(allocation,save)){error="Required SAME allocated LevelSavegame C1 receiver";return false;}
  if(!save->owner().construct({reinterpret_cast<const void*>(request.level_identity),request.seed,request.difficulty,request.row,request.mode,request.source_flag},error))return false;
  self->pending_save_v114_.reset();return true;
 };
 return s;
}
bool LevelConstructorBindingsV4::release_unpublished_prefix_v114(LevelConstructorBorrowV3 b,std::string& e){
 if(!b.owner||!b.identity||!b.fields){e="Required actual unpublished Level constructor fields";return false;}
 auto& fields=*b.fields;
 // Reverse completed C1 resources. Each primitive retains its own failure
 // prefix; only a successful destructor permits dropping the matching lease.
 auto save=save_.lock();
 if(fields.save_ec&&!same(fields.save_ec,save)){e="Unpublished Level save receiver changed";return false;}
 if(pending_save_v114_&&pending_save_v114_!=save){e="Failed Level save allocation changed";return false;}
 if(save&&!save->owner().release(e))return false;
 fields.save_ec.reset();pending_save_v114_.reset();save_.reset();
 auto script=script_.lock();
 if(fields.script44&&!same(fields.script44,script)){e="Unpublished Level Lua receiver changed";return false;}
 if(script&&!script->close_source_v88(e))return false;
 fields.script44.reset();script_.reset();
 if(fields.events&&!fields.events->flush(e))return false;
 fields.events.reset();e.clear();return true;
}
}
