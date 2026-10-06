#include "savegame_file_gate_v2.hpp"
namespace dh2::level {
bool SavegameFileGateV2::read(void* p,const std::string& n,bool& found,std::vector<std::uint8_t>& b,std::string& e){return static_cast<SavegameFileGateV2*>(p)->read_file(n,found,b,e);}
bool SavegameFileGateV2::objects(void* p,data::Bytes b,std::uint64_t offset,LevelSavegameFieldsV1& fields,std::string& e){auto& t=*static_cast<SavegameFileGateV2*>(p);if(!t.objects_){e="Required whole same cached OBJS receiver service";return false;}return t.objects_(t.object_context_,b,offset,fields,e);}
bool SavegameFileGateV2::read_file(const std::string& n,bool& found,std::vector<std::uint8_t>& b,std::string& e){if(!jobs_||!platform_.read_file||!platform_.storage_lease){e="Required retained actual Application file storage/jobs";return false;}if(!jobs_->flush(n.c_str(),e))return false;return platform_.read_file(platform_.context,n,found,b,e);}
bool SavegameFileGateV2::cache_services(LevelSavegameCacheServicesV1& out,std::string& e){auto self=weak_from_this().lock();if(!self||!jobs_||!platform_.read_file||!platform_.storage_lease){e="Required shared actual FileManager provider gate";return false;}out={this,read,nullptr,objects,std::move(self)};return true;}
}
