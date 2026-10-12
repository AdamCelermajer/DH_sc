#include "original_filesystem_fields_v50.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <algorithm>
using namespace dh2::loader;
static void ck(bool b,const char* m){if(!b)throw std::runtime_error(m);}
static std::string decode(const std::string& s){if(s=="-")return {};std::string out;for(std::size_t i=0;i<s.size();i+=2)out.push_back(static_cast<char>(std::stoul(s.substr(i,2),nullptr,16)));return out;}
int main(int argc,char**argv){try{ck(argc==2,"Required ARM fixture");std::ifstream f(argv[1]);ck(bool(f),"Missing ARM fixture");OriginalWorkingDirectoryV50 working;ck(std::all_of(working.source_buffer.begin(),working.source_buffer.end(),[](char c){return c==0;}),"Original boot global was not zeroed");auto globalA=source_filesystem_globals_v50();auto globalB=source_filesystem_globals_v50();ck(globalA==globalB,"Source process-global working-directory duplicated between resource owners");bool changed{};std::string ge;ck(globalA->observe_chdir_result("Actual/status/fixture",0,changed,ge)&&changed&&globalB->value()=="Actual/status/fixture","Source static global failed to retain setter state across aliases");unsigned ctors{},setters{};std::string line,e;
while(std::getline(f,line)){std::istringstream in(line);std::string kind,a,b,c;in>>kind;
 if(kind=="C"){in>>a;OriginalFileSystemPathsV50 paths;ck(construct_original_filesystem_paths_v50(decode(a),paths,e)&&e.empty(),"Source C1 rejected valid RES_PATH");for(const auto* p:{&paths.root2c,&paths.copy130,&paths.resources234,&paths.trace338,&paths.save43c}){in>>b;ck(std::string(p->data(),p->size())==decode(b),"Native/source original C1 buffers differ");}++ctors;}
 else{int status{},want{};in>>status>>a>>want>>b;bool source{};ck(working.observe_chdir_result(decode(a),status,source,e)&&source==bool(want)&&e.empty(),"Source chdir result changed");ck(std::string(working.source_buffer.data(),working.source_buffer.size())==decode(b),"Source shared directory whole buffer differs");++setters;}}
ck(ctors==5&&setters==16,"ARM case fixture incomplete");auto old=working.source_buffer;bool source=true;ck(!working.observe_chdir_result(std::string(1024,'x'),0,source,e)&&old==working.source_buffer,"Overflow source domain mutated shared state");OriginalFileSystemPathsV50 paths;paths.root2c[0]='X';ck(!construct_original_filesystem_paths_v50(std::string(251,'x'),paths,e)&&paths.root2c[0]=='X',"Source trace overflow domain got fabricated constructor state");
std::cout<<"PASS original_ARM/native_C1_cases="<<ctors<<" setter_cases="<<setters<<" initial_global1024_zero=1 exact_tail_preservation=1 constructor_resources234_zero=1 host_chdir_performed=0 production_cache_binding=0\n";
}catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}
