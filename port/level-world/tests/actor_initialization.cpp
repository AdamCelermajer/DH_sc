#include "../actor_initialization.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <stdexcept>
using namespace dh2::character;
namespace {
std::uint64_t checks=0,rejections=0;
void check(bool v){++checks;if(!v)throw std::runtime_error("Actor initialization audit failed at check "+std::to_string(checks));}
std::vector<std::uint8_t> read(const char* name){std::ifstream f(name,std::ios::binary);check(bool(f));return {std::istreambuf_iterator<char>(f),{}};}
unsigned word(const std::uint8_t* p){return unsigned(p[0])|(unsigned(p[1])<<8)|(unsigned(p[2])<<16)|(unsigned(p[3])<<24);}
std::string fixed(const std::uint8_t* p){const auto* end=static_cast<const std::uint8_t*>(std::memchr(p,0,64));check(end!=nullptr);return {reinterpret_cast<const char*>(p),std::size_t(end-p)};}
std::string quoted(const std::string& s){std::string out="\"";for(char c:s){if(c=='\\'||c=='\"')out+='\\';out+=c;}return out+'\"';}
std::string hash(const ActorInitializationDigest& d){std::string out;for(auto b:d){out+="0123456789abcdef"[b>>4];out+="0123456789abcdef"[b&15];}return out;}
const char* fields[]{"ai_state","ai_state_visible","auto_spawn","spawn_delay","spawn_view_radius","char_group","char_group_role"};
std::string projection(const ActorInitialization& data){
 std::ostringstream s;s<<"{\"cache_sha256\":"<<quoted(hash(data.cache_sha256))<<",\"descriptor_sha256\":"<<quoted(hash(data.descriptor_sha256))<<",\"original_sha256\":"<<quoted(hash(data.original_sha256))<<",\"default_capture_sha256\":"<<quoted(hash(data.default_capture_sha256))<<",\"sources\":[";
 bool first=true;for(const auto& p:data.sources){if(!first)s<<',';first=false;s<<"{\"room\":"<<p.room<<",\"bytes\":"<<p.bytes<<",\"cache_entry\":"<<quoted(p.cache_entry)<<",\"sha256\":"<<quoted(hash(p.sha256))<<'}';}
 s<<"],\"records\":[";first=true;
 for(const auto& p:data.records){if(!first)s<<',';first=false;s<<"{\"room\":"<<p.room<<",\"name\":"<<quoted(p.name)<<",\"character\":"<<quoted(p.character)<<",\"source_index\":"<<p.source_index<<",\"requested_template\":"<<(p.requested_template.present?quoted(p.requested_template.text):"null")<<",\"authored\":{";
  for(unsigned j=0;j<7;++j){if(j)s<<',';s<<quoted(fields[j])<<':'<<(p.authored[j].present?quoted(p.authored[j].text):"null");}
  const auto& v=p.resolved;s<<"},\"resolved\":{\"ai_state\":"<<quoted(v.ai_state)<<",\"ai_state_visible\":"<<(v.ai_state_visible?"true":"false")<<",\"auto_spawn\":"<<(v.auto_spawn?"true":"false")<<",\"spawn_delay\":["<<v.spawn_delay[0]<<','<<v.spawn_delay[1]<<"],\"spawn_view_radius\":"<<v.spawn_view_radius<<",\"char_group\":"<<quoted(v.char_group)<<",\"char_group_role\":"<<quoted(v.char_group_role)<<",\"preset_state\":"<<v.preset_state<<"}}";
 }s<<"]}";return s.str();
}
}
int main(int argc,char** argv){try{
 check(argc==3);auto binary=read(argv[1]);const auto descriptor=read(argv[2]);check(descriptor.size()>=16&&!std::memcmp(descriptor.data(),"DACT",4)&&word(descriptor.data()+4)==1&&descriptor.size()==16+std::uint64_t(word(descriptor.data()+8))*256);
 std::vector<ActorInitializationKey> keys;for(std::size_t at=16;at<descriptor.size();at+=256)if(word(descriptor.data()+at)==1)keys.push_back({word(descriptor.data()+at+4),fixed(descriptor.data()+at+8),fixed(descriptor.data()+at+72)});check(keys.size()==11);
 ActorInitializationDigest digest{};check(binary.size()>=160);std::memcpy(digest.data(),binary.data()+64,32);ActorInitialization out;std::string error;
 check(load_actor_initialization(binary.data(),binary.size(),digest,keys,out,error)&&error.empty());const auto original=projection(out);check(out.records.size()==11&&out.sources.size()==3);
 unsigned absent=0,present=0;for(const auto& k:keys){const auto* p=actor_initialization(out,k.room,k.name);check(p&&p->character==k.character&&p->resolved.preset_state==3&&p->resolved.auto_spawn&&p->resolved.ai_state_visible&&p->resolved.spawn_delay==std::array<std::int32_t,2>{0,0});check(!std::signbit(p->resolved.spawn_view_radius)&&p->resolved.spawn_view_radius==0);p->authored[actor_ai_state].present?++present:++absent;}
 check(absent==6&&present==5);check(!actor_initialization(out,9,keys[0].name)&&!actor_initialization(out,keys[0].room,"_PRIM_tmp_cultist01"));
 auto reverse=keys;std::reverse(reverse.begin(),reverse.end());check(load_actor_initialization(binary.data(),binary.size(),digest,reverse,out,error)&&projection(out)==original);
 auto reject=[&](const std::uint8_t* p,std::size_t n,const ActorInitializationDigest& d,const std::vector<ActorInitializationKey>& k){check(!load_actor_initialization(p,n,d,k,out,error)&&!error.empty()&&projection(out)==original);++rejections;};
 for(std::size_t n=0;n<binary.size();++n)reject(binary.data(),n,digest,keys);
 for(std::size_t n=0;n<binary.size();++n){auto mutated=binary;mutated[n]^=1;reject(mutated.data(),mutated.size(),digest,keys);}
 auto trailing=binary;trailing.push_back(0);reject(trailing.data(),trailing.size(),digest,keys);reject(nullptr,binary.size(),digest,keys);
 auto wrong=digest;wrong[0]^=1;reject(binary.data(),binary.size(),wrong,keys);wrong={};reject(binary.data(),binary.size(),wrong,keys);
 auto bad=keys;bad.pop_back();reject(binary.data(),binary.size(),digest,bad);bad=keys;bad.push_back(keys.front());reject(binary.data(),binary.size(),digest,bad);bad=keys;bad.back()=bad.front();reject(binary.data(),binary.size(),digest,bad);
 bad=keys;bad[0].room=9;reject(binary.data(),binary.size(),digest,bad);bad=keys;bad[0].name="missing";reject(binary.data(),binary.size(),digest,bad);bad=keys;bad[0].character="Player";reject(binary.data(),binary.size(),digest,bad);
 std::fill(binary.begin(),binary.end(),0);binary.clear();binary.shrink_to_fit();keys.clear();keys.shrink_to_fit();check(projection(out)==original);
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"atomic_rejections\":"<<rejections<<",\"input_ownership_checks\":1,\"key_order_checks\":1,\"source_raw_presence\":{\"missing_ai_state\":6,\"authored_idle\":5},\"native_projection\":"<<original<<"}\n";return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
