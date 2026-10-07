#include "animation_bank.hpp"
#include <algorithm>
#include <cstring>
#include <limits>
#include <stdexcept>
#include <unordered_set>
namespace {
constexpr std::size_t byte_limit=2*1024*1024,count_limit=65536,text_limit=4096;
struct Reader {
 dh2::data::Bytes bytes;std::size_t at=0;
 explicit Reader(dh2::data::Bytes b):bytes(b){const auto p=reinterpret_cast<std::uintptr_t>(b.data);if(!p||b.size>byte_limit||b.size>std::numeric_limits<std::uintptr_t>::max()-p)throw std::runtime_error("Invalid animation bank span");}
 const std::uint8_t* take(std::size_t n){if(n>bytes.size-at)throw std::runtime_error("Truncated animation bank");const auto* p=bytes.data+at;at+=n;return p;}
 std::uint32_t word(){const auto* p=take(4);return p[0]|(std::uint32_t(p[1])<<8)|(std::uint32_t(p[2])<<16)|(std::uint32_t(p[3])<<24);}
 std::int32_t id(){const auto w=word();std::int32_t s;std::memcpy(&s,&w,4);if(s<0)throw std::runtime_error("Negative animation resource ID");return s;}
 dh2::data::AnimationBankDigest digest(){dh2::data::AnimationBankDigest d;std::memcpy(d.data(),take(d.size()),d.size());if(std::all_of(d.begin(),d.end(),[](auto v){return v==0;}))throw std::runtime_error("Empty animation bank digest");return d;}
 std::string text(){const auto n=word();if(!n||n>text_limit)throw std::runtime_error("Invalid animation bank string length");const auto* p=take(n);for(std::size_t i=0;i<n;++i)if(p[i]<32||p[i]>126)throw std::runtime_error("Invalid animation bank text");return {reinterpret_cast<const char*>(p),n};}
};
bool path(const std::string& s){if(s.empty()||s.front()=='/'||s.back()=='/'||s.find('\\')!=std::string::npos||s.find(':')!=std::string::npos)return false;std::size_t at=0;while(at<s.size()){const auto end=s.find('/',at);const auto piece=s.substr(at,end==std::string::npos?s.size()-at:end-at);if(piece.empty()||piece=="."||piece=="..")return false;if(end==std::string::npos)break;at=end+1;}return true;}
}
namespace dh2::data {
bool load_animation_bank(Bytes bytes,AnimationBank& output,std::string& error){
 error.clear();try {
  Reader r(bytes);if(r.word()!=0x31424150||r.word()!=1||r.word()!=bytes.size)throw std::runtime_error("Animation bank header differs");
  AnimationBank next;next.identity_policy=r.word();if(next.identity_policy!=1)throw std::runtime_error("Unsupported animation identity policy");next.animation_table=r.word();next.animation_set_id=r.word();next.template_clip_id=r.id();const auto n=r.word(),requests=r.word();if(!n||n>count_limit||!requests||requests>count_limit||requests<n)throw std::runtime_error("Invalid animation bank counts");
  next.manifest_sha256=r.digest();next.cache_sha256=r.digest();next.original_sha256=r.digest();next.producer_sha256=r.digest();next.character=r.text();next.resources.reserve(n);std::unordered_set<std::int32_t> ids;std::unordered_set<std::string> authored,assets,cache;
  for(std::uint32_t i=0;i<n;++i){AnimationBankResource v;v.clip_id=r.id();v.bytes=r.word();if(!v.bytes||v.bytes>64*1024*1024)throw std::runtime_error("Invalid animation resource size");v.sha256=r.digest();v.authored_path=r.text();v.asset=r.text();v.cache_entry=r.text();if(!path(v.authored_path)||!path(v.asset)||!path(v.cache_entry)||!ids.insert(v.clip_id).second||!authored.insert(v.authored_path).second||!assets.insert(v.asset).second||!cache.insert(v.cache_entry).second)throw std::runtime_error("Duplicate or invalid animation resource path/ID");next.resources.push_back(std::move(v));}
  std::unordered_set<std::int32_t> seen;next.registration_requests.reserve(requests);std::size_t unique=0;
  for(std::uint32_t i=0;i<requests;++i){const auto id=r.id();if(!ids.count(id))throw std::runtime_error("Registration references missing animation");if(seen.insert(id).second){if(unique>=n||next.resources[unique].clip_id!=id)throw std::runtime_error("Animation first-registration order differs");++unique;}next.registration_requests.push_back(id);}
  if(unique!=n||next.registration_requests.front()!=next.template_clip_id||r.at!=bytes.size)throw std::runtime_error("Animation bank coverage/template/trailing bytes differ");
  output=std::move(next);return true;
 }catch(const std::exception& e){error=e.what();return false;}
}
std::int32_t animation_resource_index(const AnimationBank& b,std::int32_t id)noexcept{for(std::size_t i=0;i<b.resources.size();++i)if(b.resources[i].clip_id==id)return static_cast<std::int32_t>(i);return -1;}
const AnimationBankResource* animation_resource(const AnimationBank& b,std::int32_t id)noexcept{const auto i=animation_resource_index(b,id);return i<0?nullptr:&b.resources[static_cast<std::size_t>(i)];}
std::uintptr_t animation_resource_identity(const AnimationBank& b,std::int32_t id)noexcept{const auto i=animation_resource_index(b,id);return i<0||b.identity_policy!=1?0:static_cast<std::uintptr_t>(i)+1;}
}
