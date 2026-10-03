#include "../animation_bank.hpp"
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <stdexcept>
using namespace dh2::data;
namespace {
void check(bool v,const char* m){if(!v)throw std::runtime_error(m);}
std::string quote(const std::string& s){std::string out="\"";for(char c:s){if(c=='"'||c=='\\')out+='\\';out+=c;}return out+'"';}
std::string hex(const AnimationBankDigest& d){const char* digits="0123456789abcdef";std::string s;for(auto b:d){s+=digits[b>>4];s+=digits[b&15];}return s;}
std::string dump(const AnimationBank& b){std::ostringstream o;o<<"{\"character\":"<<quote(b.character)<<",\"animation_table\":"<<b.animation_table<<",\"animation_set_id\":"<<b.animation_set_id<<",\"template_clip_id\":"<<b.template_clip_id<<",\"cache_sha256\":"<<quote(hex(b.cache_sha256))<<",\"original_sha256\":"<<quote(hex(b.original_sha256))<<",\"producer_sha256\":"<<quote(hex(b.producer_sha256))<<",\"manifest_sha256\":"<<quote(hex(b.manifest_sha256))<<",\"identity_policy\":"<<b.identity_policy<<",\"registration_requests\":[";for(std::size_t i=0;i<b.registration_requests.size();++i){if(i)o<<',';o<<b.registration_requests[i];}o<<"],\"first_unique_resource_order\":[";for(std::size_t i=0;i<b.resources.size();++i){if(i)o<<',';o<<b.resources[i].clip_id;}o<<"],\"resources\":[";for(std::size_t i=0;i<b.resources.size();++i){if(i)o<<',';const auto& r=b.resources[i];o<<"{\"clip_id\":"<<r.clip_id<<",\"bytes\":"<<r.bytes<<",\"sha256\":"<<quote(hex(r.sha256))<<",\"authored_path\":"<<quote(r.authored_path)<<",\"asset\":"<<quote(r.asset)<<",\"cache_entry\":"<<quote(r.cache_entry)<<'}';}return o.str()+"]}";}
void put(std::vector<std::uint8_t>& b,std::size_t at,std::uint32_t v){for(unsigned i=0;i<4;++i)b[at+i]=std::uint8_t(v>>(i*8));}
unsigned word(const std::vector<std::uint8_t>& b,std::size_t at){return b[at]|(unsigned(b[at+1])<<8)|(unsigned(b[at+2])<<16)|(unsigned(b[at+3])<<24);}
struct Span {std::size_t word_at,at,size;};
}
int main(int argc,char** argv){try {
 if(argc!=3)return 2;
 std::ifstream f(argv[1],std::ios::binary);check(bool(f),"Asset missing");std::vector<std::uint8_t> raw{std::istreambuf_iterator<char>(f),{}};AnimationBank bank;std::string error;check(load_animation_bank({raw.data(),raw.size()},bank,error),error.c_str());check(bank.resources.size()==116&&bank.registration_requests.size()==158&&bank.template_clip_id==1111,"Real bank counts differ");const auto expected=dump(bank);std::ofstream output(argv[2]);output<<expected<<'\n';check(bool(output),"Output missing");output.close();unsigned lookups=0;
 for(std::size_t i=0;i<bank.resources.size();++i){const auto id=bank.resources[i].clip_id;check(animation_resource(bank,id)==&bank.resources[i]&&animation_resource_index(bank,id)==static_cast<std::int32_t>(i)&&animation_resource_identity(bank,id)==i+1,"Identity/lookup differs");++lookups;}
 for(auto id:bank.registration_requests){check(animation_resource(bank,id)!=nullptr,"Ordered lookup missing");++lookups;}
 check(!animation_resource(bank,-1)&&animation_resource_index(bank,0x7fffffff)==-1&&!animation_resource_identity(bank,-1),"Missing lookup differs");lookups+=3;
 unsigned rejects=0;auto reject=[&](Bytes bytes){check(!load_animation_bank(bytes,bank,error)&&!error.empty()&&dump(bank)==expected,"Malformed input committed");++rejects;};
 reject({nullptr,raw.size()});reject({reinterpret_cast<const std::uint8_t*>(~std::uintptr_t(0)-1),4});reject({raw.data(),2*1024*1024+1});
 auto truncate=[&](std::size_t n){auto b=raw;b.resize(n);if(n>=12)put(b,8,static_cast<unsigned>(n));reject({b.data(),b.size()});};
 for(std::size_t n=0;n<164;++n)truncate(n);
 for(std::size_t n=164;n<raw.size();n+=97)truncate(n);
 std::size_t at=164;std::vector<std::size_t> records;std::vector<Span> texts;auto scan_text=[&](){const auto n=word(raw,at);texts.push_back({at,at+4,n});truncate(at);truncate(at+4+n-1);at+=4+n;};scan_text();for(unsigned i=0;i<116;++i){records.push_back(at);at+=40;scan_text();scan_text();scan_text();}const auto request_at=at;
 auto mutate=[&](std::size_t pos,unsigned v){auto b=raw;put(b,pos,v);reject({b.data(),b.size()});};
 mutate(0,0);mutate(4,2);mutate(8,0);mutate(12,0);mutate(24,1040);mutate(28,0);mutate(28,65537);mutate(32,0);mutate(32,65537);mutate(records[0],0xffffffff);mutate(records[1],1111);mutate(records[0]+4,0);mutate(records[0]+4,64*1024*1024+1);mutate(request_at,1040);mutate(request_at+4,0x7fffffff);
 for(const auto& t:texts){mutate(t.word_at,0);mutate(t.word_at,4097);}for(unsigned i=1;i<4;++i){auto b=raw;b[texts[i].at]='/';reject({b.data(),b.size()});b=raw;b[texts[i].at]=0;reject({b.data(),b.size()});}
 for(unsigned i=0;i<5;++i){auto b=raw;const auto pos=i<4?36+i*32:records[0]+8;std::memset(b.data()+pos,0,32);reject({b.data(),b.size()});}
 bool duplicate_test=false;for(std::size_t i=0;i<116&&!duplicate_test;++i)for(std::size_t j=i+1;j<116&&!duplicate_test;++j){const auto a=texts[1+i*3+1],b=texts[1+j*3+1];if(a.size==b.size){auto bad=raw;std::memcpy(bad.data()+b.at,bad.data()+a.at,a.size);reject({bad.data(),bad.size()});duplicate_test=true;}}check(duplicate_test,"No duplicate-path fixture");
 auto trailing=raw;trailing.push_back(0);put(trailing,8,static_cast<unsigned>(trailing.size()));reject({trailing.data(),trailing.size()});
 // Output ownership survives destruction of the source bytes.
 raw.clear();raw.shrink_to_fit();check(dump(bank)==expected,"Bank retained input pointers");
 std::cout<<"{\"validation\":\"PASS\",\"resources\":116,\"registration_requests\":158,\"lookup_checks\":"<<lookups<<",\"atomic_rejection_checks\":"<<rejects<<",\"owned_input_checks\":1,\"mismatches\":0}\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 3;}}
