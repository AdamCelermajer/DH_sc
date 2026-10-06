#include "openable_container_data_connection_v11.hpp"
namespace dh2::world {
bool OpenableContainerDataConnectionV11::load(const std::uint8_t* records,std::size_t n,const std::uint8_t* names,std::size_t nn,const std::uint8_t* dict,std::size_t dn,const std::uint8_t* dictnames,std::size_t dnn,std::string& e){
 if(ready_){e="Retained OpenableContainers cache cannot reload under live receivers";return false;}
 if(!containers_.load(records,n,names,nn,e)||!dictionary_.load(dict,dn,dictnames,dnn,e))return false;ready_=true;return true;
}
bool OpenableContainerDataConnectionV11::resolve_row(const std::string& name,std::int32_t& id,OpenableContainerRowV1& out,std::string& e)const{if(!ready_){e="Required actual OpenableContainers/GameObjectDict cache";return false;}if(name.empty()){id=-1;out={};return true;}return containers_.resolve(name,id,out,e);}
bool OpenableContainerDataConnectionV11::visual_asset(CanonicalGameObjectBaseOwnerV1& base,std::int32_t id,std::string& e)const{
 if(!ready_){e="Required loaded GameObjectDict for Container visual";return false;}const std::string* path{};if(!dictionary_.file(id,path,e))return false;
 auto* bdae=base.string(0x290);if(!bdae){e="Required SAME canonical Container CString290";return false;}
 // Original39f98c strlen before source CString.assign(begin,end).
 bdae->assign(path->c_str());return true;
}
}
