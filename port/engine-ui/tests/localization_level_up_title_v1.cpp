#include "../localization.hpp"
extern "C" {
#include "../../pydata-constants/constants.h"
}
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <stdexcept>

using Raw=std::vector<std::uint8_t>;
static void require(bool ok,const char* message){if(!ok)throw std::runtime_error(message);}
static Raw read(const std::filesystem::path& path){
 std::ifstream file(path,std::ios::binary);if(!file)throw std::runtime_error("missing source asset: "+path.string());
 return {std::istreambuf_iterator<char>(file),{}};
}
struct Context {
 std::filesystem::path data;dh2_pycst_view common{},fonts{};std::map<std::uintptr_t,std::string> leases;
 std::uintptr_t next_lease{};unsigned symbol_sheet{},english_sheet{},player_queries{};
 dh2::ui::Localization* owner{};bool probe_reentry{};std::string reentry_error;
 static bool open(void* p,const char* uri,bool& found,Raw& bytes,std::uintptr_t& lease,std::string& error){
  auto& self=*static_cast<Context*>(p);
  if(self.probe_reentry&&self.owner){self.probe_reentry=false;std::string nested;self.owner->string_id(0,self.services(),nested,self.reentry_error);}
  const auto path=self.data.parent_path()/uri;
  std::ifstream file(path,std::ios::binary);found=bool(file);lease=0;if(!found)return true;
  bytes={std::istreambuf_iterator<char>(file),{}};lease=++self.next_lease;self.leases.emplace(lease,uri);
  if(std::strcmp(uri,"text/global.symbols")==0)++self.symbol_sheet;
  if(std::strcmp(uri,"text/global.english")==0)++self.english_sheet;
  return true;
 }
 static bool close(void* p,std::uintptr_t lease,std::string& error){
  auto& self=*static_cast<Context*>(p);if(!self.leases.erase(lease)){error="invalid localization lease";return false;}return true;
 }
 static bool debug(void*,const char* key,std::string& error){if(std::strcmp(key,"isTracingStringManager")){error="unexpected StringManager trace key";return false;}return true;}
 static bool constant(void* p,const char* group,const char* key,std::uint32_t& value,std::string& error){
  auto& self=*static_cast<Context*>(p);dh2_pycst_result result{};
  const auto* table=std::strcmp(group,"FontTextColors")==0?&self.fonts:&self.common;
  if(dh2_pycst_get(table,group,std::strlen(group),key,std::strlen(key),&result)||!result.found){
   error=std::string("missing original constant ")+group+"."+key;return false;
  }
  std::memcpy(&value,&result.value,sizeof(value));return true;
 }
 static bool no_player(void* p,std::uintptr_t& character,std::string&){
  ++static_cast<Context*>(p)->player_queries;character=0;return true;
 }
 static bool player_name(void*,std::uintptr_t,std::string&,std::string& error){
  error="source title must not query a nonexistent player";return false;
 }
 dh2::ui::LocalizationServices services(){return {this,open,close,debug,constant,no_player,player_name};}
};
int main(int argc,char** argv){try{
 require(argc==2,"expected cache data/pydata directory");
 const std::filesystem::path cache=argv[1];Context context;context.data=cache;
 auto bytes=read(cache/"common_text_pyarray.bin"),names=read(cache/"common_text_pyarraynames.bin"),schema=read(cache/"common_text_pystructnames.bin");
 dh2::ui::Localization cold;std::string cold_value,cold_error;
 require(!cold.ready()&&!cold.string_id(0,context.services(),cold_value,cold_error)&&
         cold_error=="Localization ID lookup before StringManager initialization",
         "uninitialized StringManager lookup has a distinct lifecycle diagnostic");
 auto constants=read(cache/"common_text_pycst.bin"),colors=read(cache/"fonts_pycst.bin");
 require(dh2_pycst_open(&context.common,constants.data(),constants.size())==0,"load source common text constants");
 require(dh2_pycst_open(&context.fonts,colors.data(),colors.size())==0,"load source font colors");
 dh2::ui::Localization owner;std::string error;context.owner=&owner;context.probe_reentry=true;
 require(owner.load({bytes.data(),bytes.size()},{names.data(),names.size()},{schema.data(),schema.size()},error),"load original StringManager metadata");
 require(owner.switch_pack(0,false,error),"select original English locale");
 dh2::ui::LocalizationResult title;
 require(owner.native_string("GLOBAL_LEVEL_UP_TITLE",context.services(),title,error),error.c_str());
 require(title.found&&title.text=="LEVEL UP!","source symbol ID resolves the exact original English title");
 require(context.reentry_error=="Localization ID lookup reentry unsupported",
         "true callback recursion remains rejected while the outer source lookup completes");
 require(context.symbol_sheet==1&&context.english_sheet==1,"native symbol lookup reads its symbols and selected locale sheets once");
 require(context.player_queries==1&&context.leases.empty(),"source null-player branch and file lease cleanup");
 std::cout<<"{\"validation\":\"PASS\",\"symbol\":\"GLOBAL_LEVEL_UP_TITLE\",\"english\":\"LEVEL UP!\",\"source_symbol_sheet_reads\":1,\"english_sheet_reads\":1,\"player_name_substitution\":\"null-player branch\"}\n";
 return 0;
}catch(const std::exception& error){std::cerr<<error.what()<<'\n';return 1;}}
