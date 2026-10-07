#include "actor_initialization.hpp"
#include "character_native_fsm.hpp"
#include <algorithm>
#include <cstring>
#include <set>
#include <stdexcept>
#include <utility>
namespace dh2::character {
namespace {
ActorInitializationDigest digest(const char* hex){ActorInitializationDigest out{};for(std::size_t i=0;i<32;++i){auto value=[](char c){return c>='0'&&c<='9'?c-'0':c-'a'+10;};out[i]=std::uint8_t(value(hex[i*2])*16+value(hex[i*2+1]));}return out;}
struct SourceProof {std::uint32_t room,bytes;const char* entry;const char* hash;};
const SourceProof sources[]{
 {0,10841,"com.gameloft.android.GAND.GloftD2SS/files/data/3d/modules/crypt/mgp/crypt_cemetery_entrance_01.mgp","f827a7627268dbb807c538940cdf66e1b1b72aeecb34c1fd558b62cb03cb84c9"},
 {3,3523,"com.gameloft.android.GAND.GloftD2SS/files/data/3d/modules/crypt/mgp/crypt_corner_ne_02.mgp","095665bc7250b6700837f359d620adcdaed52647a0e61b83db499b64eb6efdfa"},
 {7,10307,"com.gameloft.android.GAND.GloftD2SS/files/data/3d/modules/crypt/mgp/crypt_straight_c_ns_01.mgp","c88e2e8028fe8bdf18bb703f2f0db69c9a6df7b8060f282d4d5f20fb455f04a2"}};
struct RecordProof {std::uint32_t room;const char* name;const char* character;bool direct;};
const RecordProof records[]{
 {0,"_prim_tmp_cultist01","Crypt_Skeleton",false},{0,"_prim_tmp_cultist03","Crypt_Skeleton",false},
 {0,"_prim_tmp_cultist05","CryptSlime",false},{0,"_prim_tmp_cultist06","CryptSlime",false},
 {0,"_prim_tmp_cultist07","Crypt_Skeleton",false},{0,"_prim_tmp_cultist08","Crypt_Skeleton",false},
 {3,"_prim_Monster","CryptSlime",true},{3,"_prim_Monster02","CryptSlime_RE",true},
 {3,"_prim_Monster05","CryptSlime",true},{7,"_prim_Monster_Ghost05","Crypt_Ghost",true},
 {7,"_prim_Monster_Ghost06","Crypt_Ghost",true}};
const RecordProof* proof(std::uint32_t room,const std::string& name){for(const auto& r:records)if(room==r.room&&name==r.name)return &r;return nullptr;}
void require(bool ok,const char* error){if(!ok)throw std::runtime_error(error);}
class Reader {
 const std::uint8_t* bytes_;std::size_t size_,at_=0;
 public:Reader(const std::uint8_t* p,std::size_t n):bytes_(p),size_(n){}
 const std::uint8_t* read(std::size_t n){require(n<=size_-at_,"CAI1 truncated input");const auto* p=bytes_+at_;at_+=n;return p;}
 std::uint32_t word(){const auto* p=read(4);return std::uint32_t(p[0])|(std::uint32_t(p[1])<<8)|(std::uint32_t(p[2])<<16)|(std::uint32_t(p[3])<<24);}
 std::int32_t integer(){const auto word_value=word();std::int32_t value;std::memcpy(&value,&word_value,4);return value;}
 ActorInitializationDigest hash(){ActorInitializationDigest result{};std::memcpy(result.data(),read(32),32);return result;}
 std::string text(){const auto n=word();require(n<=4096,"CAI1 string exceeds bounded metadata limit");const auto* p=read(n);for(std::uint32_t i=0;i<n;++i)require(p[i]>=32&&p[i]<=126,"CAI1 string encoding rejected");return {reinterpret_cast<const char*>(p),n};}
 AuthoredText authored(){const auto present=word();require(present<=1,"CAI1 presence rejected");AuthoredText a{present!=0,text()};require(a.present||a.text.empty(),"CAI1 absent value contains text");return a;}
 bool finished()const{return at_==size_;}
};
void authored_matches(const AuthoredText& value,bool present,const char* text){require(value.present==present&&value.text==text,"CAI1 raw authoring differs from captured Crypt inventory");}
}
bool load_actor_initialization(const std::uint8_t* input,std::size_t size,const ActorInitializationDigest& actual_digest,const std::vector<ActorInitializationKey>& keys,ActorInitialization& out,std::string& error){
 error.clear();try {
  require(input&&size>=160&&size<=1024*1024,"CAI1 input rejected");
  require(keys.size()==11,"CAI1 expected monster count rejected");
  std::set<std::pair<std::uint32_t,std::string>> expected;
  for(const auto& k:keys){const auto* p=proof(k.room,k.name);require(p&&k.character==p->character&&expected.emplace(k.room,k.name).second,"CAI1 descriptor monster keys rejected");}
  Reader r(input,size);require(!std::memcmp(r.read(4),"CAI1",4)&&r.word()==1&&r.word()==size,"CAI1 header rejected");
  require(r.word()==11&&r.word()==3&&r.word()==7&&!r.word()&&!r.word(),"CAI1 header counts/reserved rejected");
  ActorInitialization candidate;candidate.cache_sha256=r.hash();candidate.descriptor_sha256=r.hash();candidate.original_sha256=r.hash();candidate.default_capture_sha256=r.hash();
  require(candidate.cache_sha256==digest("3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679")&&candidate.original_sha256==digest("36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80")&&candidate.default_capture_sha256==digest("dbff29cffe8c865eed384a0907722dee9ecf6dd272096ebcbab3cbee544cc2e7"),"CAI1 original/cache/default provenance rejected");
  require(candidate.descriptor_sha256==actual_digest&&std::any_of(actual_digest.begin(),actual_digest.end(),[](std::uint8_t b){return b!=0;}),"CAI1 actual DACT digest rejected");
  for(const auto& source:sources){ActorInitializationSource s;s.room=r.word();s.bytes=r.word();s.sha256=r.hash();s.cache_entry=r.text();require(s.room==source.room&&s.bytes==source.bytes&&s.cache_entry==source.entry&&s.sha256==digest(source.hash),"CAI1 source member binding rejected");candidate.sources.push_back(std::move(s));}
  std::set<std::pair<std::uint32_t,std::string>> seen;
  for(unsigned i=0;i<11;++i){ActorInitializationRecord row;row.room=r.word();row.source_index=r.word();row.name=r.text();row.character=r.text();row.requested_template=r.authored();for(auto& a:row.authored)a=r.authored();
   const auto* p=proof(row.room,row.name);require(p&&row.character==p->character&&seen.emplace(row.room,row.name).second&&expected.count({row.room,row.name}),"CAI1 record/descriptor binding rejected");
   require(row.source_index<candidate.sources.size()&&candidate.sources[row.source_index].room==row.room,"CAI1 record source binding rejected");
   authored_matches(row.requested_template,true,"Monster");authored_matches(row.authored[actor_ai_state],p->direct,p->direct?"Idle":"");
   for(std::size_t j=actor_ai_state_visible;j<=actor_spawn_view_radius;++j)authored_matches(row.authored[j],false,"");
   authored_matches(row.authored[actor_char_group],p->direct,"");authored_matches(row.authored[actor_char_group_role],p->direct,p->direct?"Normal":"");
   auto& v=row.resolved;v.ai_state=r.text();const auto visible=r.word(),automatic=r.word();require(visible==1&&automatic==1,"CAI1 resolved bool default rejected");v.ai_state_visible=visible!=0;v.auto_spawn=automatic!=0;v.spawn_delay={r.integer(),r.integer()};const auto radius=r.word();std::memcpy(&v.spawn_view_radius,&radius,4);v.preset_state=r.integer();v.char_group=r.text();v.char_group_role=r.text();
   std::int32_t preset=-1;require(dh2_character_native_fsm_preset_state(&preset,v.ai_state.c_str())==1,"CAI1 source preset getter failed");
   require(v.ai_state==row.authored[actor_ai_state].text&&v.spawn_delay==std::array<std::int32_t,2>{0,0}&&radius==0&&v.preset_state==preset&&v.preset_state==3&&v.char_group.empty()&&v.char_group_role==row.authored[actor_char_group_role].text,"CAI1 resolved source defaults rejected");
   candidate.records.push_back(std::move(row));
  }
  require(r.finished()&&seen==expected,"CAI1 trailing/unmatched input rejected");out=std::move(candidate);return true;
 }catch(const std::exception& e){error=e.what();return false;}
}
const ActorInitializationRecord* actor_initialization(const ActorInitialization& snapshot,std::uint32_t room,const std::string& name)noexcept{for(const auto& r:snapshot.records)if(r.room==room&&r.name==name)return &r;return nullptr;}
}
