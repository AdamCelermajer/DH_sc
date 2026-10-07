#include "character_world_peer_properties_v1.hpp"
#include <cstring>
#include <algorithm>
namespace dh2::character {
bool CharacterWorldPeerPropertiesV1::load(const std::uint8_t* data,std::size_t size,const ActorInitializationDigest& digest,const std::vector<objects::Record>& actual){
 error_.clear();if(!data||size<80||size>65536){error_="Required actual Crypt decor property inventory";return false;}
 std::size_t at=0;auto word=[&](std::uint32_t& n){if(size-at<4)return false;std::memcpy(&n,data+at,4);at+=4;return true;};
 std::uint32_t magic,version,total,count;if(!word(magic)||!word(version)||!word(total)||!word(count)||magic!=0x31505043||version!=1||total!=size||count!=84||std::memcmp(data+16,digest.data(),32)){error_="Decor inventory header/actual DACT mismatch";return false;}
 const std::uint8_t original[]={0x36,0x49,0x8e,0xb8,0x18,0x0f,0xfb,0x74,0x75,0x9e,0x63,0x05,0xe9,0x59,0x6d,0xb9,0x99,0xf1,0x85,0x83,0xd4,0x60,0xf3,0xb8,0x53,0x4a,0xbc,0xb6,0x02,0x2f,0x5e,0x80};if(std::memcmp(data+48,original,32)){error_="Decor inventory original mismatch";return false;}
 at=80;std::vector<Key> keys;for(unsigned i=0;i<count;++i){Key k{};std::uint32_t n;if(!word(k.index)||!word(k.room)||!word(n)||!n||n>256||size-at<n){error_="Decor inventory row span";return false;}k.name.assign(reinterpret_cast<const char*>(data+at),n);at+=n;if(k.index>=actual.size()||actual[k.index].kind!=2||actual[k.index].room!=k.room||actual[k.index].name!=k.name||std::any_of(keys.begin(),keys.end(),[&](const auto& x){return x.index==k.index;})){error_="Decor property identity mismatch";return false;}keys.push_back(std::move(k));}
 if(at!=size||std::count_if(actual.begin(),actual.end(),[](const auto& a){return a.kind==2;})!=84){error_="Decor inventory not complete";return false;}keys_=std::move(keys);return true;
}
bool CharacterWorldPeerPropertiesV1::initialize_decor(std::uint32_t i,WorldNpcObjectFieldsV1& f,std::uint32_t& type){error_.clear();if(std::none_of(keys_.begin(),keys_.end(),[&](const auto& k){return k.index==i;})||f.visible_written||f.static84){error_="Required fresh authored AnimatedDecor owner";return false;}type=0x14;f.static84=1;f.visible80=1;f.visible_written=true;return true;}
bool character_object_default_properties_v1(WorldNpcObjectFieldsV1& f,std::string& e){if(f.visible_written||f.static84){e="Required fresh Character source property-default stage";return false;}f.visible80=1;f.visible_written=true;return true;}
}
