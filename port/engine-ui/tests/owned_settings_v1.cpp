#include "../owned_hud_settings_v1.hpp"
#include "../settings_native_files_v1.hpp"
#include "../settings_language_scene_v1.hpp"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <cstring>
#include <stdexcept>
using namespace dh2::ui;
namespace {
using Bytes=std::vector<std::uint8_t>;
Bytes file(const std::string& path){std::ifstream f(path,std::ios::binary);if(!f)throw std::runtime_error("Input missing: "+path);return Bytes(std::istreambuf_iterator<char>(f),{});}
void check(bool b,const std::string& s){if(!b)throw std::runtime_error(s);}
struct Reader {const Bytes& bytes;std::size_t at{};unsigned word(){check(bytes.size()-at>=4,"gold word");unsigned v;std::memcpy(&v,bytes.data()+at,4);at+=4;return v;}Bytes blob(unsigned n){check(n<=bytes.size()-at,"gold bytes");Bytes out(bytes.begin()+at,bytes.begin()+at+n);at+=n;return out;}};
GameOptionBytesV1 span(const Bytes& b){return {b.data(),b.size()};}
LocalizationBytes ls(const Bytes& b){return {b.data(),b.size()};}
struct Fixture {
 bool found{},reject_refresh{},reject_close{},reject_platform{},nested{};unsigned platform{};Bytes bytes;std::vector<std::pair<unsigned,unsigned>> calls;
 static bool open(void* p,const char* name,bool& found,Bytes& bytes,std::uintptr_t& lease,std::string&){auto& x=*static_cast<Fixture*>(p);check(std::string(name)=="dh2_settings.savegame","source filename");x.calls.push_back({1,x.found});found=x.found;bytes=x.bytes;lease=found?0xf123456789abcde0ULL:0;return true;}
 static bool close(void* p,std::uintptr_t lease,std::string& e){auto& x=*static_cast<Fixture*>(p);check(lease==0xf123456789abcde0ULL,"64-bit lease");if(x.reject_close){e="close rejected";return false;}return true;}
 static bool refresh(void* p,OwnedHudSettingsV1& owner,int lang,std::string& e){auto& x=*static_cast<Fixture*>(p);x.calls.push_back({2,static_cast<unsigned>(lang)});if(x.reject_refresh){e="required inventory rejected";return false;}if(x.nested){x.nested=false;owner.set_option("VolumeFX",73);}return true;}
 static bool language(void* p,unsigned& v,std::string& e){auto& x=*static_cast<Fixture*>(p);x.calls.push_back({3,x.platform});v=x.platform;if(x.reject_platform){e="platform rejected";return false;}return true;}
};
}
int main(int argc,char** argv){try{
 check(argc==5,"usage gold designDirectory commonTextDirectory privateTestDirectory");auto gold=file(argv[1]);Reader r{gold};check(r.blob(4)==Bytes({'H','S','V','1'}),"gold header");auto owner_count=r.word(),parser_count=r.word(),row_count=r.word();
 auto design=std::string(argv[2]),text=std::string(argv[3]);auto records=file(design+"/design_pyarray.bin"),names=file(design+"/design_pyarraynames.bin"),schema=file(design+"/design_pystructnames.bin");
 auto tr=file(text+"/common_text_pyarray.bin"),tn=file(text+"/common_text_pyarraynames.bin"),ts=file(text+"/common_text_pystructnames.bin");
 GameOptionTableV1 table;std::string e;check(table.load_design_cache(span(records),span(names),span(schema),e),e);auto backing=table.borrow();check(backing.rows().size()==16&&backing.records_offset()==240&&backing.names_offset()==56,"actual cache offsets");
 unsigned comparisons=0,checks=0,guards=0,failures=0,reentries=0;
 for(unsigned c=0;c<owner_count;++c){
  auto blob=r.blob(r.word());unsigned args[5];for(auto& v:args)v=r.word();unsigned expected[16];for(auto& v:expected)v=r.word();auto tutorial=r.blob(14);unsigned loaded=r.word(),fresh=r.word(),hint=r.word(),orientation=r.word(),cursor=r.word(),count=r.word();std::vector<std::pair<unsigned,unsigned>> calls;for(unsigned i=0;i<count;++i){auto k=r.word(),v=r.word();calls.push_back({k,v});}
  Localization local;check(local.load(ls(tr),ls(tn),ls(ts),e),e);OwnedHudSettingsV1 owner(backing);Fixture fixture;fixture.found=args[0];fixture.platform=args[4];fixture.bytes=std::move(blob);
  SettingsFileServicesV1 files{&fixture,Fixture::open,Fixture::close};SettingsLanguageServicesV1 language{&fixture,Fixture::refresh,Fixture::language,&local};SettingsDeviceFactsV1 device{};device.korean_build=args[2];device.japanese_build=args[3];SettingsLoadReceiptV1 receipt;
  check(owner.load(args[1],files,language,device,receipt,e),"owner case "+std::to_string(c)+": "+e);
  for(unsigned i=0;i<16;++i)check(static_cast<unsigned>(owner.option(backing.names()[i].c_str()))==expected[i],"owner option "+std::to_string(c));
  check(std::memcmp(owner.tutorials().data(),tutorial.data(),14)==0&&owner.loaded()==bool(loaded)&&owner.new_settings()==bool(fresh)&&static_cast<unsigned>(owner.language_hint())==hint&&owner.orientation()==bool(orientation)&&receipt.consumed==cursor&&fixture.calls==calls,"owner flags/order "+std::to_string(c));++comparisons;
  check(owner.option("Missing")==-1&&owner.saved_option("Missing")==0&&!owner.set_option("Missing",1),"miss policy");++checks;
 }
 const std::string keys[]={"Language","VolumeFX","known"};
 for(unsigned c=0;c<parser_count;++c){auto bytes=r.blob(r.word());unsigned values[3];for(auto& v:values)v=r.word();auto cursor=r.word(),recognized=r.word();unsigned expected[3];for(auto& v:expected)v=r.word();auto count=r.word();std::vector<std::string> expected_calls;for(unsigned i=0;i<count;++i){auto b=r.blob(r.word());expected_calls.emplace_back(b.begin(),b.end());}
  struct Lookup {unsigned* values;const std::string* keys;std::vector<std::string> calls;} ctx{values,keys,{}};
  SettingsLookup16V1 lookup{&ctx,[](void* p,const char* name)->int*{auto& x=*static_cast<Lookup*>(p);x.calls.emplace_back(name);for(unsigned i=0;i<3;++i)if(x.keys[i]==name)return reinterpret_cast<int*>(x.values+i);return nullptr;}};
  SettingsParserSpan24V1 s{bytes.data(),static_cast<unsigned>(bytes.size()),0,0,0};check(!dh2_settings_v1_read_options(&s,&lookup)&&s.cursor==cursor&&s.recognized==recognized&&!std::memcmp(values,expected,12)&&ctx.calls==expected_calls,"parser case");++comparisons;
 }
 for(unsigned c=0;c<row_count;++c){auto bytes=r.blob(28),expected=r.blob(32);GameOptionRow32V1 row;unsigned used;check(!dh2_game_option_v1_decode_record(&row,&used,bytes.data(),28)&&used==28&&!std::memcmp(&row,expected.data(),32),"row case");++comparisons;}
 check(r.at==gold.size(),"gold trailing bytes");
 // Real stdio missing/present file, private ownership and retained-load flags.
 auto directory=std::filesystem::path(argv[4]);std::filesystem::create_directories(directory);auto path=directory/"dh2_settings.savegame";check(!std::filesystem::exists(path),"test path already contains source save; refuse overwrite");SettingsNativeFilesV1 native(directory.string());auto native_files=native.services();Localization local;check(local.load(ls(tr),ls(tn),ls(ts),e),e);Fixture f;SettingsLanguageServicesV1 language{&f,Fixture::refresh,Fixture::language,&local};SettingsDeviceFactsV1 device{};OwnedHudSettingsV1 first(backing),second(backing);SettingsLoadReceiptV1 receipt;
 check(first.load(false,native_files,language,device,receipt,e)&&!receipt.found&&!first.loaded()&&first.option("VolumeMusic")==100&&first.language()==-1,"real missing file");++checks;
 Bytes raw(18,0);for(unsigned i=0;i<14;++i)raw[4+i]=i;{std::ofstream out(path,std::ios::binary);out.write(reinterpret_cast<const char*>(raw.data()),raw.size());check(bool(out),"real save fixture write");}
 check(first.load(false,native_files,language,device,receipt,e)&&receipt.found&&first.loaded()&&first.new_settings()&&first.language()==0&&local.pack()==0&&first.file_bytes()==raw,"real existing raw save");++checks;
 check(second.load(false,native_files,language,device,receipt,e)&&first.set_option("VolumeFX",17)&&second.option("VolumeFX")==100,"private maps");++checks;
 std::filesystem::remove(path);check(first.load(false,native_files,language,device,receipt,e)&&first.loaded()&&first.new_settings(),"source flags retained across missing reload");++checks;
 check(!table.load_design_cache(span(records),span(names),span(schema),e),"borrowed reload rejects");++guards;
 for(unsigned n:{0u,1u,239u,240u,244u,691u}){GameOptionTableV1 bad;auto truncated=records;truncated.resize(n);check(!bad.load_design_cache(span(truncated),span(names),span(schema),e)&&!bad.borrow(),"cache atomic truncate");++guards;}
 {GameOptionTableV1 preserved;check(preserved.load_design_cache(span(records),span(names),span(schema),e),e);auto wrong=schema;wrong[0]=42;check(!preserved.load_design_cache(span(records),span(names),span(wrong),e)&&preserved.borrow().rows().size()==16,"failed reload retains owned cache");++guards;}
 {GameOptionRow32V1 row{};row.default_value=77;unsigned used=99;check(dh2_game_option_v1_decode_record(&row,&used,records.data(),27)&&row.default_value==77&&used==99,"row truncation atomic");++guards;check(dh2_game_option_v1_decode_record(&row,reinterpret_cast<unsigned*>(&row),records.data(),28)&&row.default_value==77,"row overlap guard");++guards;}
 f.reject_refresh=true;check(!first.load(false,native_files,language,device,receipt,e)&&first.option("Language")==-1&&e.find("inventory")!=std::string::npos,"required inventory failure prefix");++failures;f.reject_refresh=false;f.nested=true;check(first.load(false,native_files,language,device,receipt,e)&&first.option("VolumeFX")==73,"language service live mutation");++reentries;
 Fixture close;close.found=true;close.bytes=raw;close.reject_close=true;SettingsFileServicesV1 rejected{&close,Fixture::open,Fixture::close};check(!first.load(false,rejected,language,device,receipt,e)&&close.calls==std::vector<std::pair<unsigned,unsigned>>{{1,1}},"close failure prevents language");++failures;
 f.reject_platform=true;check(!first.load(true,native_files,language,device,receipt,e),"platform required failure");++failures;f.reject_platform=false;
 SettingsStartupBindingV1 binding{&first,&native_files,&language,&device};binding.application_identity=0xf100000000000001ULL;binding.savegame_identity=0xf100000000000002ULL;HudStartupState48 state{binding.application_identity,binding.savegame_identity,0,0,0,0,0,0};HudStartupServices16 service{&binding,settings_startup_v1_service};check(!dh2_hud_load_settings(&state,&service),"actual owned startup wrapper without audio");++checks;state.sound=0xf100000000000003ULL;check(dh2_hud_load_settings(&state,&service)==-2&&binding.error.find("audio")!=std::string::npos,"required audio failure");++failures;
 SettingsParserSpan24V1 s{raw.data(),static_cast<unsigned>(raw.size()),0,0,1};SettingsLookup16V1 lookup{nullptr,[](void*,const char*)->int*{return nullptr;}};check(dh2_settings_v1_read_options(&s,&lookup)==-1&&s.cursor==0,"reserved atomic guard");++guards;s.reserved=0;s.size=3;check(dh2_settings_v1_read_options(&s,&lookup)==-2&&s.cursor==0,"truncated prefix");++guards;
 std::cout<<"{\"validation\":\"PASS\",\"comparisons\":"<<comparisons<<",\"owner_cases\":"<<owner_count<<",\"parser_cases\":"<<parser_count<<",\"record_cases\":"<<row_count<<",\"additional_checks\":"<<checks<<",\"atomic_guards\":"<<guards<<",\"required_failure_prefixes\":"<<failures<<",\"synchronous_mutation_cases\":"<<reentries<<",\"real_stdio_missing_and_present\":true,\"mismatches\":0}"<<std::endl;
}catch(const std::exception& e){std::cerr<<e.what()<<std::endl;return 1;}}
