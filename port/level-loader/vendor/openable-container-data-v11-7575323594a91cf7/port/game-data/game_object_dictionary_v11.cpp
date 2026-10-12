#include "game_object_dictionary_v11.hpp"
#include <cstring>
namespace dh2::data {namespace {
bool decode(const std::uint8_t* p,std::size_t size,std::vector<std::string>& out,std::string& e){
 if(!p||size<4){e="Required actual GameObjectDict count stream";return false;}
 auto word=[](const std::uint8_t* b){return std::uint32_t(b[0])|std::uint32_t(b[1])<<8|std::uint32_t(b[2])<<16|std::uint32_t(b[3])<<24;};
 const auto count=word(p);if(count>(size-4)/4){e="Short GameObjectDict record list";return false;}
 std::size_t pos=4;std::vector<std::string> rows;rows.reserve(count);
 for(std::uint32_t i=0;i<count;++i){if(size-pos<4){e="Short GameObjectDict string length";return false;}auto n=word(p+pos);pos+=4;if(n>size-pos){e="Short GameObjectDict string data";return false;}rows.emplace_back(reinterpret_cast<const char*>(p+pos),n);pos+=n;}
 if(pos!=size){e="Unexpected GameObjectDict stream tail";return false;}out=std::move(rows);return true;
}
}
bool GameObjectDictionaryV11::load(const std::uint8_t* records,std::size_t n,const std::uint8_t* names,std::size_t nn,std::string& e){std::vector<std::string> files,keys;if(!decode(records,n,files,e)||!decode(names,nn,keys,e))return false;if(files.size()!=keys.size()){e="GameObjectDict readNames count mismatch";return false;}files_=std::move(files);names_=std::move(keys);loaded_=true;return true;}
bool GameObjectDictionaryV11::file(std::int32_t id,const std::string*& out,std::string& e)const{out=nullptr;if(!loaded_||id<0||std::size_t(id)>=files_.size()){e="Required actual GameObjectDict ColladaFile row";return false;}out=&files_[std::size_t(id)];return true;}
bool GameObjectDictionaryV11::find(const char* name,std::int32_t& id,std::string& e)const{if(!loaded_||!name){e="Required loaded GameObjectDict name namespace";return false;}id=-1;for(std::size_t i=0;i<names_.size();++i)if(!std::strcmp(names_[i].c_str(),name)){id=static_cast<std::int32_t>(i);break;}return true;}
}
