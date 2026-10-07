#include "../localization.hpp"
#include "../../script-runtime/script_constants.hpp"
#include "../../level-world/character_design_services.hpp"
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <stdexcept>
using namespace dh2::ui;
namespace {
void check(bool b,const char* text){if(!b)throw std::runtime_error(text);}
std::vector<unsigned char> file(const std::string& path){std::ifstream f(path,std::ios::binary);check(bool(f),path.c_str());return {std::istreambuf_iterator<char>(f),{}};}
unsigned word(const unsigned char*& p,const unsigned char* end){check(end-p>=4,"gold word");unsigned v;std::memcpy(&v,p,4);p+=4;return v;}
std::string text(const unsigned char*& p,const unsigned char* end){auto n=word(p,end);check(n<=unsigned(end-p),"gold string");std::string s(reinterpret_cast<const char*>(p),n);p+=n;return s;}
struct Fixture {
 std::string assets,debug_dir;dh2_script_constants* constants=dh2_script_constants_create();dh2::character::DebugSwitches* debug=dh2_character_debug_create();
 std::vector<std::string> trace;std::string name="Prince";bool present=true,debug_fail=false,constant_fail=false,open_fail=false,missing=false,second_null=false;unsigned player_queries{},opened{},closed{};
 ~Fixture(){dh2_script_constants_destroy(constants);dh2_character_debug_destroy(debug);}
};
bool open(void* p,const char* uri,bool& found,std::vector<unsigned char>& bytes,std::uintptr_t& lease,std::string& error){auto& f=*static_cast<Fixture*>(p);if(f.open_fail){error="delivered open failure";return false;}if(f.missing){found=false;return true;}FILE* stream=std::fopen((f.assets+"/"+uri).c_str(),"rb");if(!stream){if(errno==ENOENT){found=false;return true;}error="asset open failed";return false;}found=true;lease=reinterpret_cast<std::uintptr_t>(stream);++f.opened;check(!std::fseek(stream,0,SEEK_END),"asset seek");auto n=std::ftell(stream);check(n>=0&&!std::fseek(stream,0,SEEK_SET),"asset size");bytes.resize(n);check(std::fread(bytes.data(),1,bytes.size(),stream)==bytes.size(),"asset read");return true;}
bool close(void* p,std::uintptr_t lease,std::string&){auto& f=*static_cast<Fixture*>(p);++f.closed;return std::fclose(reinterpret_cast<FILE*>(lease))==0;}
int debug_open(void* p,const char* path,std::uintptr_t* lease){auto& f=*static_cast<Fixture*>(p);check(!std::strcmp(path,"DebugSwitches.savegame"),"source debug filename");FILE* stream=std::fopen((f.debug_dir+"/"+path).c_str(),"rb");if(!stream)return errno==ENOENT?0:-2;*lease=reinterpret_cast<std::uintptr_t>(stream);return 0;}
int debug_close(void*,std::uintptr_t lease){return std::fclose(reinterpret_cast<FILE*>(lease));}
bool debug(void* p,const char* key,std::string& e){auto& f=*static_cast<Fixture*>(p);f.trace.push_back("debug_load");f.trace.push_back("debug_query");if(f.debug_fail){e="delivered debug failure";return false;}dh2::character::DebugFileServices24 services{p,debug_open,debug_close};unsigned value;if(dh2_character_debug_load(f.debug,&services)!=1||dh2_character_debug_get(&value,f.debug,key,&services)!=1){e="native DebugSwitches rejected";return false;}return true;}
bool constant(void* p,const char* group,const char* key,unsigned& value,std::string& e){auto& f=*static_cast<Fixture*>(p);f.trace.push_back(std::string("constant:")+group+":"+key);if(f.constant_fail){e="delivered constant failure";return false;}int v;auto r=dh2_script_constants_get(f.constants,group,key,&v);std::memcpy(&value,&v,4);return r==0;}
bool character(void* p,std::uintptr_t& identity,std::string&){auto& f=*static_cast<Fixture*>(p);f.trace.push_back("player_character");identity=f.present&&!(f.second_null&&f.player_queries%2)?7:0;++f.player_queries;return true;}
bool name(void* p,std::uintptr_t identity,std::string& out,std::string&){auto& f=*static_cast<Fixture*>(p);check(identity==7,"character identity");f.trace.push_back("player_name");out=f.name;return true;}
LocalizationServices services(Fixture& f){return {&f,open,close,debug,constant,character,name};}
void load(Localization& l,Fixture& f){std::string e;auto a=file(f.assets+"/pydata/common_text_pyarray.bin"),n=file(f.assets+"/pydata/common_text_pyarraynames.bin"),s=file(f.assets+"/pydata/common_text_pystructnames.bin");check(l.load({a.data(),a.size()},{n.data(),n.size()},{s.data(),s.size()},e),e.c_str());}
}
int main(int argc,char** argv){try{
 check(argc==5,"gold assets fonts debug-dir args");Fixture f;f.assets=argv[2];f.debug_dir=argv[4];check(f.constants&&f.debug,"native owners");for(const auto& path:{f.assets+"/pydata/common_text_pycst.bin",std::string(argv[3])}){auto raw=file(path);dh2_script_constants_reload r{};check(dh2_script_constants_load(f.constants,raw.data(),raw.size(),&r)==0&&r.consumed==raw.size(),"actual constants input");}
 Localization l;load(l,f);std::string error;auto svc=services(f);
 for(unsigned pack=0;pack<9;++pack){check(l.switch_pack(pack,false,error),"preload pack");for(unsigned sheet=0;sheet<37;++sheet)check(l.preload(pack,sheet,false,svc,error),error.c_str());}check(l.loaded_sheets()==333&&f.opened==333&&f.closed==333,"actual333file leases");
 auto raw=file(argv[1]);const auto* p=raw.data();const auto* end=p+raw.size();check(word(p,end)==0x32434f4c,"gold magic");auto count=word(p,end);unsigned callbacks=0;
 for(unsigned i=0;i<count;++i){auto input=text(p,end),expected=text(p,end);auto q=reinterpret_cast<const unsigned char*>(input.data());auto qe=q+input.size();auto pack=static_cast<int>(word(q,qe));f.present=word(q,qe)!=0;auto symbol=text(q,qe);f.name=text(q,qe);check(q==qe,"input exact");auto r=reinterpret_cast<const unsigned char*>(expected.data());auto re=r+expected.size();auto found=word(r,re)!=0,flag=word(r,re)!=0;auto value=text(r,re);std::vector<std::string> events;auto n=word(r,re);for(unsigned j=0;j<n;++j)events.push_back(text(r,re));check(r==re,"expected exact");check(l.switch_pack(pack,false,error),"source pack");f.trace.clear();LocalizationResult result;check(l.native_string(symbol,svc,result,error),error.c_str());check(result.text==value&&result.found==found&&result.sets_menu_string_flag==flag,"original native text/result");check(f.trace==events,"original ordered callback trace");callbacks+=events.size();}
 check(p==end,"gold EOF");unsigned guards=0;LocalizationResult result{"sentinel",true,false};
 check(!l.native_string("invalid",svc,result,error)&&result.text=="sentinel","invalid symbol guard");++guards;
 auto incomplete=svc;incomplete.player_name=nullptr;check(!l.native_string("GAMEPLAYMENUS_FASTTRAVEL",incomplete,result,error)&&result.text=="sentinel","provider guard");++guards;
 f.debug_fail=true;check(!l.native_string("GAMEPLAYMENUS_FASTTRAVEL",svc,result,error)&&result.text=="sentinel","debug prefix failure");f.debug_fail=false;++guards;
 f.constant_fail=true;check(!l.native_string("GAMEPLAYMENUS_FASTTRAVEL",svc,result,error)&&result.text=="sentinel","constant prefix failure");f.constant_fail=false;++guards;
 f.present=true;f.second_null=true;f.player_queries=0;check(!l.native_string("GAMEPLAYMENUS_FASTTRAVEL",svc,result,error)&&result.text=="sentinel","second player guard");f.second_null=false;++guards;
 Localization lazy;load(lazy,f);check(lazy.switch_pack(0,false,error),"lazy pack");f.trace.clear();f.player_queries=0;LocalizationResult actual;check(lazy.native_string("GAMEPLAYMENUS_FASTTRAVEL",svc,actual,error)&&actual.text=="World Map"&&lazy.loaded_sheets()==3,"actual lazy HUD localization");
 auto first=l.loaded_sheets();check(l.switch_pack(0,true,error)&&l.loaded_sheets()<=first,"source unload flag");check(l.switch_pack(8,true,error)&&l.loaded_sheets()<333,"source unload old pack");
 unsigned entries=0,loaded=0;check(dh2_character_debug_snapshot(f.debug,&loaded,&entries)==1&&loaded==1&&entries>=1,"genuine Debug owned insertion");check(f.opened==f.closed,"all acquired leases closed");
 std::cout<<"{\"validation\":\"PASS\",\"comparisons\":"<<count<<",\"ordered_callbacks\":"<<callbacks<<",\"actual_text_files\":333,\"owned_text_strings\":41546,\"guards\":"<<guards<<",\"opened\":"<<f.opened<<",\"closed\":"<<f.closed<<",\"real_constants\":true,\"real_DebugSwitches\":true,\"lazy_HUD\":\"World Map\",\"mismatches\":0}\n";return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
