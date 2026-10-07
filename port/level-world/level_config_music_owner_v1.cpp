#include "level_config_music_owner_v1.hpp"
#include <cstring>
#include <stdexcept>
namespace dh2::world {namespace {
struct Reader {data::Bytes bytes;std::size_t at{};
 std::uint32_t word(){if(at>bytes.size||bytes.size-at<4)throw std::runtime_error("Truncated authored LevelConfig projection");auto*p=bytes.data+at;at+=4;return std::uint32_t(p[0])|(std::uint32_t(p[1])<<8)|(std::uint32_t(p[2])<<16)|(std::uint32_t(p[3])<<24);}
 std::string text(){auto length=word();if(length>65536||at>bytes.size||length>bytes.size-at)throw std::runtime_error("Malformed authored LevelConfig string");std::string s(reinterpret_cast<const char*>(bytes.data+at),length);at+=length;if(s.find('\0')!=std::string::npos)throw std::runtime_error("NUL in authored LevelConfig attribute");return s;}
};}
std::unique_ptr<LevelConfigMusicOwnerV1> LevelConfigMusicOwnerV1::load(data::Bytes bytes,std::string&error){error.clear();try{
 if(!bytes.data||bytes.size<16||std::memcmp(bytes.data,"LCM1",4))throw std::runtime_error("Required actual LevelConfig source projection");Reader r{bytes};r.at=4;if(r.word()!=1||r.word()!=bytes.size)throw std::runtime_error("Malformed LevelConfig projection version/span");auto count=r.word();if(!count||count>1024)throw std::runtime_error("Malformed LevelConfig attribute count");auto owner=std::make_unique<LevelConfigMusicOwnerV1>();owner->source_=r.text();if(owner->source_.empty())throw std::runtime_error("Missing actual LevelConfig source identity");
 for(unsigned i=0;i<count;++i){auto key=r.text();auto value=r.text();if(key.empty()||!owner->attributes_.emplace(key,value).second)throw std::runtime_error("Duplicate authored LevelConfig attribute");}
 if(r.at!=bytes.size||owner->attributes_["gametype"]!="LevelConfig"||owner->attributes_["name"]!="level_config")throw std::runtime_error("Wrong selected authored LevelConfig record");
 auto field=owner->attribute("combat_music_enabled");if(!field||(*field!="0"&&*field!="1"))throw std::runtime_error("Required exact authored combat_music_enabled bool");owner->combat_music_enabled_=*field=="1";return owner;
 }catch(const std::exception&e){error=e.what();return {};}}
const std::string* LevelConfigMusicOwnerV1::attribute(const char*key)const noexcept{if(!key)return nullptr;auto i=attributes_.find(key);return i==attributes_.end()?nullptr:&i->second;}
}
