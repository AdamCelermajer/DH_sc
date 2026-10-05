#include "character_world_npc_properties_v1.hpp"
#include <algorithm>
#include <cstring>
namespace dh2::character {
namespace {
struct Reader {
 const std::uint8_t* data;std::size_t size,at{};
 bool word(std::uint32_t& value){if(size-at<4)return false;std::memcpy(&value,data+at,4);at+=4;return true;}
 bool text(std::string& value){std::uint32_t n;if(!word(n)||n>256||size-at<n)return false;for(unsigned i=0;i<n;++i)if(data[at+i]<32||data[at+i]>126)return false;value.assign(reinterpret_cast<const char*>(data+at),n);at+=n;return true;}
};
bool key_equal(const ActorInitializationKey& a,const ActorInitializationKey& b){return a.room==b.room&&a.name==b.name&&a.character==b.character;}
constexpr std::uint8_t original[]={0x36,0x49,0x8e,0xb8,0x18,0x0f,0xfb,0x74,0x75,0x9e,0x63,0x05,0xe9,0x59,0x6d,0xb9,0x99,0xf1,0x85,0x83,0xd4,0x60,0xf3,0xb8,0x53,0x4a,0xbc,0xb6,0x02,0x2f,0x5e,0x80};
}
bool CharacterWorldNpcPropertiesV1::load(const std::uint8_t* data,std::size_t size,
 const ActorInitializationDigest& digest,const std::vector<ActorInitializationKey>& actual){
 error_.clear();if(!data||size<80||size>65536||actual.size()!=11){error_="required current Crypt object property inventory";return false;}
 Reader r{data,size};std::uint32_t magic,version,total,count;
 if(!r.word(magic)||!r.word(version)||!r.word(total)||!r.word(count)||magic!=0x314f5043||version!=1||total!=size||count!=11||
 std::memcmp(data+16,digest.data(),32)||std::memcmp(data+48,original,32)){
  error_="source NPC properties header/digest mismatch";return false;
 }
 r.at=80;std::vector<ActorInitializationKey> keys;keys.reserve(11);
 for(unsigned i=0;i<count;++i){
  ActorInitializationKey key;std::string raw;std::uint32_t visible_present,static_present;
  if(!r.word(key.room)||!r.text(key.name)||!r.text(key.character)||key.name.empty()||key.character.empty()||
  !r.word(visible_present)||visible_present||!r.text(raw)||!raw.empty()||!r.word(static_present)||static_present||!r.text(raw)||!raw.empty()||
  std::count_if(actual.begin(),actual.end(),[&](const auto& a){return key_equal(a,key);})!=1||
  std::any_of(keys.begin(),keys.end(),[&](const auto& a){return a.room==key.room&&a.name==key.name;})){
   error_="source NPC visible/static override or actor binding unsupported";return false;
  }keys.push_back(std::move(key));
 }
 if(r.at!=size){error_="trailing source NPC property bytes";return false;}
 keys_=std::move(keys);return true;
}
bool CharacterWorldNpcPropertiesV1::initialize(std::uint32_t room,const std::string& name,
 const std::string& character,WorldNpcObjectFieldsV1& fields){
 error_.clear();ActorInitializationKey requested{room,name,character};
 if(std::none_of(keys_.begin(),keys_.end(),[&](const auto& key){return key_equal(key,requested);})){error_="required actual source NPC property record";return false;}
 if(fields.visible_written||fields.static84){error_="source property initialization requires fresh ObjectBase fields";return false;}
 // ObjectBase::DeclareProperties register visible default1, static constructor
 // default0. Source LoadDefaultProperties invokes SetToDefaultValue before XML
 // overrides. This exact inventory has no override for either descriptor.
 fields.visible80=1;fields.visible_written=true;fields.static84=0;return true;
}
}
