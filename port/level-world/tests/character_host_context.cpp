#include "character_host_context.hpp"
#include "../../game-data/level_tables.hpp"
#include <algorithm>
#include <array>
#include <cstring>
#include <dlfcn.h>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <vector>
using namespace dh2::character;
namespace {
unsigned checks=0;void check(bool value){++checks;if(!value)throw std::runtime_error("host context check "+std::to_string(checks));}
std::uint32_t word(std::istream& in){std::uint32_t v=0;in.read(reinterpret_cast<char*>(&v),4);check(bool(in));return v;}
using Event=std::array<std::uint32_t,3>;
struct Receiver {
 HostPlayer8 player{};HostLevel8 level{};bool present=true,mutate=false,swap_on_second=false;int fail=-1;unsigned row_reads=0;
 std::array<std::vector<LevelRangeRow24>,2> rows;const LevelRangeRow24* borrowed_rows=nullptr;unsigned borrowed_count=0;
 std::vector<Event> trace;
 Receiver(){for(auto& row:rows)row.resize(3);const std::int32_t originals[3][6]={{10,47,76,8,45,74},{12,49,78,9,46,75},{INT32_MIN,INT32_MAX,-1,0,-3,123456789}};
  for(unsigned i=0;i<3;++i)for(unsigned j=0;j<6;++j){std::memcpy(reinterpret_cast<char*>(&rows[0][i])+j*4,&originals[i][j],4);auto v=std::uint32_t(originals[i][j])+1000u;std::memcpy(reinterpret_cast<char*>(&rows[1][i])+j*4,&v,4);}
 }
 static int invoke(void* p,const HostContextRequest16* request,HostContextResponse16* response){auto& t=*static_cast<Receiver*>(p);check(!request->reserved);t.trace.push_back({request->service,request->operation,std::uint32_t(request->value)});if(int(t.trace.size())-1==t.fail)return 1;
  if(request->service==host_get_player)response->data=&t.player;
  else if(request->service==host_get_current_level)response->data=t.present?&t.level:nullptr;
  else if(request->service==host_get_range_rows){++t.row_reads;const bool swapped=(t.mutate&&std::any_of(t.trace.begin(),t.trace.end(),[](const Event& e){return e[0]==host_push_integer;}))||(t.swap_on_second&&t.row_reads>1);response->data=t.borrowed_rows?t.borrowed_rows:t.rows[swapped].data();response->count=t.borrowed_rows?t.borrowed_count:t.rows[swapped].size();}
  else if(request->service==host_push_integer){if(t.mutate&&std::count_if(t.trace.begin(),t.trace.end(),[](const Event& e){return e[0]==host_push_integer;})==1){t.level={2,99};t.player.cached_level=99;}}
  else check(false);
  return 0;
 }
 HostContextServices16 services(){return {this,invoke};}
};
std::vector<std::uint8_t> file(const std::string& path){std::ifstream f(path,std::ios::binary);check(bool(f));return {std::istreambuf_iterator<char>(f),{}};}
dh2::data::Bytes bytes(const std::vector<std::uint8_t>& v){return {v.data(),v.size()};}
}
int main(int argc,char** argv){try{
 check(argc==3);std::ifstream in(argv[1],std::ios::binary);check(word(in)==0x32544348);auto count=word(in),row_count=word(in);check(row_count==54);std::vector<LevelRangeRow24> gold_rows(row_count);in.read(reinterpret_cast<char*>(gold_rows.data()),gold_rows.size()*24);check(bool(in));unsigned ordered=0;
 for(unsigned i=0;i<count;++i){Receiver t;t.rows[0]=gold_rows;t.rows[1]=gold_rows;for(auto& row:t.rows[1])for(unsigned j=0;j<6;++j){std::uint32_t v;std::memcpy(&v,reinterpret_cast<char*>(&row)+j*4,4);v+=1000;std::memcpy(reinterpret_cast<char*>(&row)+j*4,&v,4);}std::array<unsigned,10> h{};for(auto& v:h)v=word(in);t.level={std::int32_t(h[1]),std::int32_t(h[8])};t.mutate=h[5];t.present=h[6];t.player.cached_level=std::int32_t(h[7]);std::vector<Event> expected(h[9]);for(auto& e:expected)for(auto& v:e)v=word(in);std::vector<dh2_script_value> args(std::max(1u,h[4]));args[0].type=h[2];std::memcpy(&args[0].number,&h[3],4);auto s=t.services();check(dh2_character_host_context_query(h[0],h[4]?args.data():nullptr,h[4],&s)==1&&t.trace==expected);ordered+=h[9];
 }check(in.peek()==EOF);
 unsigned prefixes=0,guards=0;
 for(int fail=0;fail<5;++fail){Receiver t;t.level.row_index=0;t.fail=fail;auto s=t.services();check(dh2_character_host_context_query(2,nullptr,0,&s)==-2&&t.trace.size()==unsigned(fail+1));++prefixes;}
 Receiver t;auto s=t.services();for(auto op:{3u,UINT32_MAX}){check(dh2_character_host_context_query(op,nullptr,0,&s)==-1&&t.trace.empty());++guards;}
 check(dh2_character_host_context_query(0,nullptr,0,nullptr)==-1&&t.trace.empty());++guards;
 check(dh2_character_host_context_query(0,nullptr,1,&s)==-1&&t.trace.empty());++guards;
 check(dh2_character_host_context_query(0,nullptr,1048577,&s)==-1&&t.trace.empty());++guards;
 t.present=false;check(dh2_character_host_context_query(2,nullptr,0,&s)==-2&&t.trace.size()==1);++guards;t.trace.clear();t.present=true;t.level.row_index=-2;check(dh2_character_host_context_query(2,nullptr,0,&s)==-2&&t.trace.size()==2);++guards;t.trace.clear();t.level.row_index=3;check(dh2_character_host_context_query(2,nullptr,0,&s)==-2&&t.trace.size()==2);++guards;
 // Genuine current decoded cache, independently source-reader audited.
 const std::string data=argv[2];auto records=file(data+"/levels_pyarray.bin"),names=file(data+"/levels_pyarraynames.bin"),schema=file(data+"/levels_pystructnames.bin");dh2::data::LevelTables table;std::string error;check(dh2::data::load_levels(bytes(records),bytes(names),bytes(schema),table,error)&&table.levels.size()==51);
 std::vector<LevelRangeRow24> borrowed(table.levels.size());for(unsigned i=0;i<borrowed.size();++i)std::memcpy(&borrowed[i],table.levels[i].scalar.words+12,24);
 auto crypt=std::find(table.level_names.begin(),table.level_names.end(),"GOTHICUS_CRYPT_01");check(crypt!=table.level_names.end());const auto crypt_index=std::int32_t(crypt-table.level_names.begin());const std::int32_t expected[6]={10,47,76,8,45,74};check(!std::memcmp(&borrowed[crypt_index],expected,24));
 // The cache sync calls the existing genuine native Character.GetLevel. It
 // reads resolved19 ASR8 and does not replace stale cache reads automatically.
 dh2::data::PropertyRules rules{};dh2::data::PropertyState properties{};auto view=dh2::data::property_view(rules,properties);unsigned syncs=0;
 for(auto raw:{INT32_MIN,-257,-256,-1,0,255,256,4352,INT32_MAX}){properties.resolved[19]=raw;properties.base[19]=999*256;HostPlayer8 player{-99,0};check(!dh2_character_host_context_sync_level(&player,&view)&&player.cached_level==(raw>=0?raw/256:-1-((-1-raw)/256)));++syncs;}
 HostPlayer8 retained{37,0};check(dh2_character_host_context_sync_level(&retained,nullptr)==1&&retained.cached_level==37);++guards;retained.reserved=1;check(dh2_character_host_context_sync_level(&retained,&view)==1&&retained.cached_level==37);++guards;
 const auto before=properties.resolved;check(dh2_character_host_context_sync_level(reinterpret_cast<HostPlayer8*>(properties.resolved.data()),&view)==1&&properties.resolved==before);++guards;
 Receiver vm_receiver;vm_receiver.borrowed_rows=borrowed.data();vm_receiver.borrowed_count=borrowed.size();vm_receiver.level={crypt_index,0};vm_receiver.player.cached_level=17;
 HostContextBindings16 binding{vm_receiver.services()};auto* vm=dh2_script_vm_create_empty(4*1024*1024);check(vm&&!dh2_script_vm_open_source_libraries(vm)&&dh2_script_vm_stack_size(vm)==5&&!dh2_character_host_context_bind(vm,&binding));unsigned vm_cases=0;
 auto call=[&](const char* name,const dh2_script_value* args,unsigned n,const std::vector<float>& expected_values){std::array<dh2_script_value,2> out{};unsigned returned=0;check(!dh2_script_vm_call(vm,name,args,n,out.data(),out.size(),&returned)&&returned==expected_values.size());for(unsigned i=0;i<returned;++i)check(out[i].type==DH2_SCRIPT_NUMBER&&out[i].number==expected_values[i]);check(dh2_script_vm_stack_size(vm)==5);++vm_cases;};
 // Actual globals are looked up dynamically on each Lua invocation. This is
 // the actual three-global monster-init dependency, not its remaining Init.
 const char* program="function Host() return GetHostPlayerLevel('ignored',false,17) end function Difficulty() return GetHostPlayerDifficulty(nil) end function Range(t) return GetCurrentLevelRange(t,'ignored') end function TableRange(id) return GetCurrentLevelRange({_this=id}) end function Prefix(t) local p=GetHostPlayerLevel();local d=GetHostPlayerDifficulty();local a,b=GetCurrentLevelRange(d);return p,a end";
 check(!dh2_script_vm_load_source_file(vm,program,std::strlen(program)));
 for(auto cached:{-1,0,17,16777217,INT32_MAX}){vm_receiver.player.cached_level=cached;call("Host",nullptr,0,{float(cached)});}
 vm_receiver.present=false;call("Difficulty",nullptr,0,{0});vm_receiver.present=true;
 for(int tier=0;tier<3;++tier){vm_receiver.level.difficulty=tier;call("Difficulty",nullptr,0,{float(tier)});dh2_script_value a{};a.type=3;a.number=float(tier);call("Range",&a,1,{float(expected[3+tier]),float(expected[tier])});}
 for(unsigned kind:{0u,1u,4u,2u}){dh2_script_value a{};a.type=kind;a.boolean=1;a.text="2";a.text_bytes=1;a.identity=0xabcdef0123456789ull;call("Range",&a,1,{8,10});}
 {dh2_script_value a{};a.type=2;a.identity=0xabcdef0123456789ull;call("TableRange",&a,1,{8,10});}
 for(float number:{-1.f,3.f,1.999f,2.999f}){dh2_script_value a{};a.type=3;a.number=number;call("Range",&a,1,number<0||number>=3?std::vector<float>{}:std::vector<float>{float(expected[3+int(number)]),float(expected[int(number)])});}
 vm_receiver.level.row_index=-1;call("Range",nullptr,0,{-1,-1});vm_receiver.level.row_index=crypt_index;vm_receiver.player.cached_level=17;vm_receiver.level.difficulty=2;call("Prefix",nullptr,0,{17,74});
 // Explicitly selected live cache producer, no invented automatic refresh.
 properties.resolved[19]=21*256+255;check(!dh2_character_host_context_sync_level(&vm_receiver.player,&view));call("Host",nullptr,0,{21});properties.resolved[19]=30*256;call("Host",nullptr,0,{21});
 // Whole decoded table for all actual row/tier combinations through real VM.
 unsigned range_cases=0;for(unsigned row=0;row<borrowed.size();++row)for(unsigned tier=0;tier<3;++tier){vm_receiver.level.row_index=row;dh2_script_value a{};a.type=3;a.number=float(tier);call("Range",&a,1,{float(borrowed[row].minimum[tier]),float(borrowed[row].maximum[tier])});++range_cases;}
 vm_receiver.present=false;std::array<dh2_script_value,2> out{};unsigned returned=0;check(dh2_script_vm_call(vm,"Range",nullptr,0,out.data(),2,&returned)!=0&&std::strstr(dh2_script_vm_error(vm),"host context source provider"));vm_receiver.present=true;vm_receiver.level={crypt_index,0};call("Range",nullptr,0,{8,10});
 dh2_script_vm_destroy(vm);Dl_info module{},world{},runtime{},decoder{};check(dladdr(reinterpret_cast<void*>(&dh2_character_host_context_query),&module)&&dladdr(reinterpret_cast<void*>(&dh2_character_get_level),&world)&&dladdr(reinterpret_cast<void*>(&dh2_script_vm_bind_source_values),&runtime)&&dladdr(reinterpret_cast<void*>(&dh2_level_decode_record),&decoder));
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"original_cases\":"<<count<<",\"ordered_services\":"<<ordered<<",\"native_failure_prefixes\":"<<prefixes<<",\"native_guards\":"<<guards<<",\"genuine_PropertyView_cache_syncs\":"<<syncs<<",\"genuine_VM_cases\":"<<vm_cases<<",\"decoded_Level_rows\":"<<borrowed.size()<<",\"decoded_VM_row_tiers\":"<<range_cases<<",\"Crypt01_ranges\":[8,10,45,47,74,76],\"module_library\":\""<<module.dli_fname<<"\",\"GetLevel_library\":\""<<world.dli_fname<<"\",\"runtime_library\":\""<<runtime.dli_fname<<"\",\"level_decoder_library\":\""<<decoder.dli_fname<<"\",\"source_active_Application_session_selected\":false,\"whole_monster_Init_executed\":false}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
