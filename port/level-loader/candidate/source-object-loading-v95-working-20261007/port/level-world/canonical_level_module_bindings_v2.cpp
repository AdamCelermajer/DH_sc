#include "canonical_level_module_bindings_v2.hpp"
#include <canonical_loading_receiver_v95.hpp>
#include <cstring>
#include <algorithm>
namespace dh2::world {
CanonicalClassReceiverV1* CanonicalLevelModuleBindingsV2::find(std::uintptr_t id)noexcept{auto i=receivers_.find(id);return i==receivers_.end()?nullptr:&i->second;}
const CanonicalLevelConfigV1* CanonicalLevelModuleBindingsV2::level_config(std::uintptr_t id)const noexcept{auto i=configs_.find(id);return i==configs_.end()?nullptr:i->second.get();}
CanonicalModuleRecordV2* CanonicalLevelModuleBindingsV2::module(std::uintptr_t id)noexcept{auto i=modules_.find(id);return i==modules_.end()?nullptr:i->second.get();}
bool CanonicalLevelModuleBindingsV2::construct_receiver(const CanonicalFactoryEntryV1& f,const CanonicalSourceObjectRequestV1& q,CanonicalClassReceiverV1& out,std::string& e){
 if(!f.name||(std::strcmp(f.name,"LevelConfig")&&std::strcmp(f.name,"Module")&&std::strcmp(f.name,"Block"))){e="Required owned LevelConfig/Module/Block source constructor";return false;}
 bool actual=false;for(const auto& entry:canonical_factories_v1())if(entry.original_address==f.original_address&&!std::strcmp(entry.name,f.name)){actual=true;break;}
 if(!actual){e="Required exact canonical catalog constructor entry";return false;}
 CanonicalObjectBorrowV1 object;if(!construct(this,f,q,object,e))return false;auto* receiver=find(object.identity);
 if(!receiver){e="Required retained LevelConfig/Module constructor result";return false;}out=*receiver;return true;
}
bool CanonicalLevelModuleBindingsV2::construct(void* p,const CanonicalFactoryEntryV1& f,const CanonicalSourceObjectRequestV1& q,CanonicalObjectBorrowV1& out,std::string& e){
 auto& s=*static_cast<CanonicalLevelModuleBindingsV2*>(p);const bool config=!std::strcmp(f.name,"LevelConfig"),module=!std::strcmp(f.name,"Module")||!std::strcmp(f.name,"Block");
 if(!config&&!module){if(!s.predecessor_.construct){e="Required predecessor actual class factory";return false;}return s.predecessor_.construct(s.predecessor_.context,f,q,out,e);}
 if(!s.construction_.candidate||!q.source_lease){e="Required SAME retained candidate/source declaration";return false;}
 CanonicalClassReceiverV1 r;
 if(config){
  auto receiver=std::make_shared<CanonicalLevelConfigV1>(s.construction_.candidate,s.construction_.level_config);
  r=canonical_class_receiver_v1(receiver);
  r.source_loading_fields_v95=[receiver](auto& out,auto& e){return receiver->source_loading_fields_v95(receiver,out,e);};
  r.source_is_updatable_v95=[receiver](bool& value,std::string& e){(void)receiver;value=false;e.clear();return true;};
  r.init_post=[receiver](std::string& e){return receiver->init_post(e);};r.is_game_object=[](bool& value,std::string&){value=false;return true;};s.configs_.emplace(r.object.identity,std::move(receiver));
 }else{
  if(!s.construction_.module_globals||!s.construction_.module_services){e="Required SAME Module static counter/concrete InitPost services";return false;}
  auto record=std::make_shared<CanonicalModuleRecordV2>();record->declaration=q.source_lease;
  GameObjectInitializationServicesV1 init;ModuleInitServicesV1 derived;
  if(!s.construction_.module_services(q,record,init,derived,e))return false;
  // Providers are retained even if a later reached callback is absent. The
  // source constructor never silently performs InitPost/visual/PF work.
  record->receiver=std::make_unique<CanonicalModuleV1>(s.construction_.candidate,record->runtime,*s.construction_.module_globals,init,derived);
  r.object=record->receiver->canonical(record);
  r.properties=[record]{return record->receiver->properties();};r.init_post=[record](std::string& e){return record->receiver->init_post(e);};r.is_game_object=[](bool& value,std::string&){value=true;return true;};
  bind_gameobject_loading_v95(std::shared_ptr<CanonicalModuleV1>(record,record->receiver.get()),false,r);
  r.position=[record](std::array<float,3>& v,std::string& e){const auto* position=record->receiver->base().vector3(0x160);if(!position){e="Required SAME Module position160";return false;}std::copy_n(position,3,v.begin());return true;};
  r.set_position=[callback=init.set_position](const std::array<float,3>& v,bool update,std::string& e){if(!callback){e="Required SAME Module GameObject::SetPosition393db4";return false;}return callback(v.data(),update,e);};s.modules_.emplace(r.object.identity,std::move(record));
 }
 r.source_lease=q.source_lease;out=r.object;s.receivers_.emplace(out.identity,std::move(r));return true;
}
#define OWN_OR_DELEGATE(member,args) auto& s=*static_cast<CanonicalLevelModuleBindingsV2*>(p);auto* r=s.find(o.identity);if(!r){if(!s.predecessor_.member){e="Required predecessor " #member;return false;}return s.predecessor_.member args;}
bool CanonicalLevelModuleBindingsV2::init_properties(void* p,const CanonicalObjectBorrowV1& o,std::string& e){OWN_OR_DELEGATE(init_properties,(s.predecessor_.context,o,e));auto a=r->properties();return s.properties_.init_properties(a,e);}
bool CanonicalLevelModuleBindingsV2::set_template(void* p,const CanonicalObjectBorrowV1& o,const char* n,std::string& e){OWN_OR_DELEGATE(set_template,(s.predecessor_.context,o,n,e));auto a=r->properties();return s.properties_.set_template(a,n,e);}
bool CanonicalLevelModuleBindingsV2::defaults(void* p,const CanonicalObjectBorrowV1& o,std::string& e){OWN_OR_DELEGATE(load_defaults,(s.predecessor_.context,o,e));auto a=r->properties();return s.properties_.load_defaults(a,e);}
bool CanonicalLevelModuleBindingsV2::overrides(void* p,const CanonicalObjectBorrowV1& o,const CanonicalSourceObjectRequestV1& q,std::string& e){OWN_OR_DELEGATE(load_overrides,(s.predecessor_.context,o,q,e));auto a=r->properties();return s.properties_.load_overrides(a,q,e);}
bool CanonicalLevelModuleBindingsV2::init_post(void* p,const CanonicalObjectBorrowV1& o,std::string& e){OWN_OR_DELEGATE(init_post,(s.predecessor_.context,o,e));return r->init_post(e);}
bool CanonicalLevelModuleBindingsV2::is_game_object(void* p,const CanonicalObjectBorrowV1& o,bool& v,std::string& e){OWN_OR_DELEGATE(is_game_object,(s.predecessor_.context,o,v,e));return r->is_game_object(v,e);}
bool CanonicalLevelModuleBindingsV2::position(void* p,const CanonicalObjectBorrowV1& o,std::array<float,3>& v,std::string& e){OWN_OR_DELEGATE(position,(s.predecessor_.context,o,v,e));if(!r->position){e="Source LevelConfig is not a GameObject";return false;}return r->position(v,e);}
bool CanonicalLevelModuleBindingsV2::set_position(void* p,const CanonicalObjectBorrowV1& o,const std::array<float,3>& v,bool update,std::string& e){OWN_OR_DELEGATE(set_position,(s.predecessor_.context,o,v,update,e));if(!r->set_position){e="Source LevelConfig is not a GameObject";return false;}return r->set_position(v,update,e);}
#undef OWN_OR_DELEGATE
bool CanonicalLevelModuleBindingsV2::unknown(void* p,const char* n,std::string& e){auto& s=*static_cast<CanonicalLevelModuleBindingsV2*>(p);if(!s.predecessor_.unknown_type_debug){e="Required SAME unknown type Debug";return false;}return s.predecessor_.unknown_type_debug(s.predecessor_.context,n,e);}
CanonicalClassServicesV1 CanonicalLevelModuleBindingsV2::services()noexcept{return {this,construct,init_properties,set_template,defaults,overrides,init_post,is_game_object,position,set_position,unknown};}
}
