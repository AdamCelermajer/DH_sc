#include "level_savegame_cache_v1.hpp"
#include <cstring>
namespace dh2::level {
namespace {
bool valid_header(const std::vector<std::uint8_t>& b){return b.size()>3&&!(b[0]==255&&b[1]==255&&b[2]==255&&b[3]==255);}
unsigned slot(LevelSavegameSectionV1 s){return s==LevelSavegameSectionV1::info?0:1;}
const char* tag(LevelSavegameSectionV1 s){return s==LevelSavegameSectionV1::info?"INFO":"OBJS";}
}
bool LevelSavegameCacheV1::construct(const std::string& name,bool raw,std::string& e){
 e.clear();if(attempted_){e="Source Savegame C1 cannot replay";return false;}attempted_=true;filename_=name;
 if(raw){e="LevelSavegame requires source non-raw Savegame C1";return false;}
 if(!services_.read_file){e="Required same Application FileManager open/read";return false;}
 std::vector<std::uint8_t> bytes;bool found=false;
 if(!services_.read_file(services_.context,name,found,bytes,e))return false;
 if(!found||!valid_header(bytes)){
  bytes.clear();found=false;
  if(!services_.read_file(services_.context,name+".bak",found,bytes,e))return false;
  // The original invalid backup is discarded. No synthetic section file.
  if(!found||!valid_header(bytes)){ready_=true;return true;}
 }
 if(!index_.load(data::Bytes{bytes.data(),bytes.size()},e))return false;
 has_file_=true;ready_=true;return true;
}
bool LevelSavegameCacheV1::register_section(const char* name,LevelSavegameSectionV1 s,LevelSavegameFieldsV1& f,std::string& e){
 e.clear();if(!ready_||!name||std::strcmp(name,tag(s))){e="Required exact ready LevelSavegame section";return false;}
 sections_[slot(s)]=&f;return true; // source initSectionInfo overwrites callbacks
}
bool LevelSavegameCacheV1::load_section(const char* name,LevelSavegameSectionV1 s,LevelSavegameFieldsV1& f,std::string& e){
 if(!register_section(name,s,f,e))return false;
 if(!has_file_)return true;auto borrow=index_.borrow();auto* section=borrow.section(name);
 if(!section||!section->size)return true;auto bytes=borrow.payload(name);
 if(s==LevelSavegameSectionV1::info){
  if(bytes.size<4){e="Truncated source INFO int32 stream";return false;}
  std::uint32_t v=std::uint32_t(bytes.data[0])|(std::uint32_t(bytes.data[1])<<8)|(std::uint32_t(bytes.data[2])<<16)|(std::uint32_t(bytes.data[3])<<24);
  std::memcpy(&f.loaded_row2c,&v,4);return true;
 }
 if(f.initializing38)return true; // original __LoadObjects entry guard
 if(services_.load_objects_stream_v2){const auto& whole=borrow.bytes();return services_.load_objects_stream_v2(services_.context,{whole.data(),whole.size()},section->offset,f,e);}
 if(!services_.load_objects){e="Required whole same ObjectManager/PlayerManager OBJS receiver loading";return false;}
 return services_.load_objects(services_.context,bytes,f,e);
}
bool LevelSavegameCacheV1::source_cache_file_v115(std::string& e){
 e.clear();if(!ready_||!services_.read_file){e="Required constructed SAME Savegame cache/FileManager";return false;}
 //315ad0 retires cache1c BEFORE opening the current file. Existing section
 //callbacks and LevelSavegame fields survive a genuine absent/invalid file.
 index_.retire_source_v108();has_file_=false;
 std::vector<std::uint8_t> bytes;bool found=false;
 if(!services_.read_file(services_.context,filename_,found,bytes,e))return false;
 if(!found||!valid_header(bytes)){
  bytes.clear();found=false;
  if(!services_.read_file(services_.context,filename_+".bak",found,bytes,e))return false;
  if(!found||!valid_header(bytes))return true;
 }
 if(!index_.load(data::Bytes{bytes.data(),bytes.size()},e))return false;
 has_file_=true;return true;
}
bool LevelSavegameCacheV1::recache_v2(data::Bytes b,std::string& e){if(!ready_){e="Required ready same Savegame recache owner";return false;}if(!index_.load(b,e))return false;has_file_=true;return true;}
}
