#include "selected_world_camera_config_v20.hpp"
namespace dh2::world {
bool SelectedWorldCameraConfigV20::load(std::string uri,std::vector<std::uint8_t> bytes,CanonicalPropertyMapV1& map,CanonicalObjectManagerV1& manager,SelectedWorldCameraServicesV20 services,std::string& e){
 if(attempted_){e="Selected LevelConfig attempt cannot replay source InitPost prefix";return false;}attempted_=true;
 if(!services.world||!services.debug_switch||!services.publish||!services.register_object){e="Required actual selected-world LevelConfig debug/registration/publication services";return false;}
 if(!document_.capture_level_buffer(std::move(uri),std::move(bytes),e))return false;auto xml=document_.borrow();if(!xml.parsed()){e=xml.diagnostic().message;return false;}
 bool found=false;for(std::uint32_t i=0;i<xml.elements().size();++i){const auto& element=xml.elements()[i];auto* type=element.attribute("gametype");if(type&&*type=="LevelConfig"){
  if(found){e="Selected MLX has multiple LevelConfig declarations; full ordered loader required";return false;}found=true;element_=i;}}
 if(!found){e="Selected MLX has no actual LevelConfig declaration";return false;}
 LevelConfigServicesV1 native;native.owner=services.world;native.debug_switch=services.debug_switch;
 auto slot=std::make_shared<std::weak_ptr<CanonicalLevelConfigV1>>();
 native.set_level_config=[slot,publish=std::move(services.publish)](std::uintptr_t id,std::string& error){auto config=slot->lock();if(!config||id!=config->identity()){error="Required SAME selected LevelConfig identity";return false;}return publish(std::move(config),error);};
 config_=std::make_shared<CanonicalLevelConfigV1>(services.world,std::move(native));auto object=config_->canonical(config_);*object.class_name20="LevelConfig";
 *slot=config_;
 const auto& element=xml.elements()[element_];auto* name=element.attribute("name");if(!name){e="Source LevelConfig Add requires actual name attribute";return false;}
 if(!services.register_object(object,*name,"LevelConfig",e))return false;
 auto* published=manager.object(object.shared_handle->key);if(!published||published->identity!=object.identity||published->shared_handle!=object.shared_handle){e="Required registration into SAME canonical World manager";return false;}
 auto actor=config_->properties();if(!map.init_properties(actor,e))return false;
 CanonicalSourceObjectRequestV1 request;request.source_lease=std::make_shared<loader::XmlDocumentV1::Borrow>(xml);request.source_context=&xml;request.element=element_;
 request.attribute=[](void* p,std::uint32_t id,const char* key)->const char*{const auto& b=*static_cast<loader::XmlDocumentV1::Borrow*>(p);if(id>=b.elements().size())return nullptr;auto* value=b.elements()[id].attribute(key);return value?value->c_str():nullptr;};
 auto* templ=element.attribute("template");if(templ&&!map.set_template(actor,templ->c_str(),e))return false;
 if(!map.load_defaults(actor,e)||!map.load_overrides(actor,request,e)||!config_->init_post(e))return false;ready_=true;return true;
}
bool SelectedWorldCameraConfigV20::camera(camera::CameraLevelConfigV19& out,std::string& e)const{
 if(!ready_||!config_){e="Required completed selected-world LevelConfig InitPost/publication";return false;}
 auto* file=config_->string(0x24c);auto* set=config_->string(0x264);auto* node=config_->string(0x27c);auto* near=config_->integer(0x294);auto* far=config_->integer(0x298);auto* sky=config_->string(0x234);
 if(!file||!set||!node||!near||!far||!sky){e="Required actual LevelConfig camera fields";return false;}
 out={{*file,*set,*node,*near,*far},*sky};return true;
}
}
