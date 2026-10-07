#include "../shader_sources.hpp"
#include <cstring>
#include <iostream>
#include <fstream>
#include <iterator>
#include <stdexcept>
using namespace dh2::scene;
namespace {
struct Reader {const std::uint8_t* p;std::size_t n,at=0;std::uint32_t word(){if(n-at<4)throw std::runtime_error("short input");std::uint32_t x;std::memcpy(&x,p+at,4);at+=4;return x;}std::string str(){const auto k=word();if(k>n-at)throw std::runtime_error("short text");std::string s(reinterpret_cast<const char*>(p+at),k);at+=k;return s;}};
void put(std::vector<std::uint8_t>& v,std::uint32_t x){for(unsigned i=0;i<4;++i)v.push_back(std::uint8_t(x>>(i*8)));}
void append(std::vector<std::uint8_t>& v,const std::string& s){v.insert(v.end(),s.begin(),s.end());}
void check(bool b){if(!b)throw std::runtime_error("shader audit failed");}
}
extern "C" std::uint32_t dh2_shader_sources_test(std::uint32_t op,const std::uint8_t* input,std::uint32_t size,std::uint8_t* output){
    try {Reader r{input,size};std::vector<std::uint8_t> v;std::string e,result;
        if(op==0){const auto flags=r.word(),type=r.word(),has=r.word();auto name=r.str(),caller=r.str(),extra=r.str(),body=r.str();ShaderSourcePlan p;if(!shader_source_plan(flags,type,name,caller,has?&extra:nullptr,body,p,e))return ~0u;put(v,p.gl_type);put(v,8);put(v,std::uint32_t(p.cache_name.size()));for(const auto& s:p.chunks)put(v,std::uint32_t(s.size()));append(v,p.cache_name);v.push_back(0);for(const auto& s:p.chunks){append(v,s);v.push_back(0);}}
        else if(op==1){const auto has=r.word();auto a=r.str(),b=r.str(),c=r.str(),extra=r.str();if(!shader_code_name(a,b,c,has?&extra:nullptr,result,e))return ~0u;put(v,std::uint32_t(result.size()));append(v,result);}
        else if(op==2){auto a=r.str();if(!shader_config_text(a,result,e))return ~0u;put(v,std::uint32_t(result.size()));append(v,result);}
        else return ~0u;if(r.at!=r.n)return ~0u;if(!output)return ~0u;std::memcpy(output,v.data(),v.size());return std::uint32_t(v.size());
    }catch(...){return ~0u;}
}
#ifndef DH2_SHADER_SOURCES_ORACLE
namespace {std::vector<std::uint8_t> file(const char* p){std::ifstream f(p,std::ios::binary);check(bool(f));return {std::istreambuf_iterator<char>(f),{}};}}
int main(int argc,char**argv){try {check(argc==3);auto gold=file(argv[1]);Reader r{gold.data(),gold.size()};check(r.word()==0x31534853);const auto count=r.word();std::vector<std::uint8_t> out(1024*1024);
    for(std::uint32_t i=0;i<count;++i){auto op=r.word(),n=r.word();check(n<=r.n-r.at);auto input=r.p+r.at;r.at+=n;const auto size=r.word();check(size<=r.n-r.at);check(dh2_shader_sources_test(op,input,n,out.data())==size);check(std::memcmp(out.data(),r.p+r.at,size)==0);r.at+=size;}check(r.at==r.n);
    auto bytes=file(argv[2]);ShaderSourcePack pack;std::string error;check(pack.load(bytes.data(),bytes.size(),error));check(pack.members().size()==34);auto*vs=pack.find("GameSWFVS.glsl");auto*fs=pack.find("GameSWFFS.glsl");check(vs&&fs&&vs->bytes.size()==410&&fs->bytes.size()==276);ShaderSourcePlan plan;
    check(shader_source_plan(0,4,vs->name,{},nullptr,std::string_view(reinterpret_cast<const char*>(vs->bytes.data()),vs->bytes.size()),plan,error));const auto retained=plan.chunks;bytes.clear();check(plan.chunks==retained&&pack.find("GameSWFVS.glsl")->bytes.size()==410);
    const auto oldname=plan.cache_name;check(!shader_source_plan(0,4,std::string("x\0y",3),{},nullptr,"body",plan,error)&&plan.cache_name==oldname);check(!shader_source_plan(0,4,"x",{},nullptr,std::string("x\0y",3),plan,error)&&plan.chunks==retained);
    std::string target="saved";check(!shader_config_text(std::string("\0",1),target,error)&&target=="saved");
    bytes=file(argv[2]);unsigned guards=3;for(auto at:std::vector<std::size_t>{0,14,18,22,30,bytes.size()-22,bytes.size()-10,bytes.size()-6}){auto bad=bytes;bad[at]^=1;check(!pack.load(bad.data(),bad.size(),error));check(pack.members().size()==34&&pack.find("GameSWFVS.glsl")->bytes.size()==410);++guards;}
    check(!pack.load(nullptr,0,error));++guards;check(!pack.load(bytes.data(),21,error));++guards;
    std::cout<<"{\"validation\":\"PASS\",\"original_gold_cases\":"<<count<<",\"stored_pack_members\":34,\"atomic_guards\":"<<guards<<",\"owned_body_and_pack\":true,\"mismatches\":0}\n";
    }catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}
#endif
