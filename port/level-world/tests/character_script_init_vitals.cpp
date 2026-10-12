#include "../character_script_init_vitals.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <cstring>
using namespace dh2::character;
namespace {
unsigned checks=0;void check(bool v){++checks;if(!v)throw std::runtime_error("Vitals check "+std::to_string(checks));}
struct Reader {std::ifstream file;explicit Reader(const char* path):file(path,std::ios::binary){check(bool(file));}template<class T>T get(){T v{};file.read(reinterpret_cast<char*>(&v),sizeof(v));check(bool(file));return v;}};
using Sheet=std::array<std::int32_t,224>;using Row=std::array<std::uint32_t,7>;
struct Fixture {
 std::array<Sheet,6> sheets;std::array<std::uint32_t,3> params;std::vector<Row> calls;unsigned phase=36,fail_at=0;
 dh2::data::PropertyView view(){return {sheets[0].data(),sheets[1].data(),sheets[2].data(),sheets[3].data(),sheets[4].data(),sheets[5].data(),nullptr,0};}
 void event(unsigned op,unsigned field,std::uint32_t delta){calls.push_back({op,field,delta,unsigned(sheets[5][36]),unsigned(sheets[5][41]),unsigned(sheets[5][38]),unsigned(sheets[5][43])});if(calls.size()==params[0]){std::int32_t v;std::memcpy(&v,&params[2],4);sheets[5][params[1]]=v;}}
 static int debug(void* p,unsigned op,const char* name,unsigned* out){auto& c=*static_cast<Fixture*>(p);if(op==dh2::fx::debug_load){check(!name);for(const auto& r:c.calls)if(r[0]==4&&r[1]==36)c.phase=41;}else check(op==dh2::fx::debug_switch&&!std::strcmp(name,"isTracingChar_Stats"));c.event(op,c.phase,0);*out=0;return op==c.fail_at?1:0;}
};
Fixture* active=nullptr;
}
extern "C" unsigned __real_dh2_property_add(dh2::data::PropertyView*,std::int32_t,std::int32_t);
extern "C" unsigned __wrap_dh2_property_add(dh2::data::PropertyView* view,std::int32_t property,std::int32_t amount){if(active)active->event(4,property,unsigned(amount));return __real_dh2_property_add(view,property,amount);}
int main(int argc,char** argv){try{
 check(argc==2);Reader reader(argv[1]);check(reader.get<unsigned>()==0x31495356);auto cases=reader.get<unsigned>();unsigned calls=0;
 for(unsigned i=0;i<cases;++i){Fixture c{};c.sheets=reader.get<std::array<Sheet,6>>();c.params=reader.get<std::array<unsigned,3>>();auto expected=reader.get<std::array<Sheet,4>>();auto result=reader.get<ScriptInitVitals24>();auto n=reader.get<unsigned>();std::vector<Row> trace;for(unsigned j=0;j<n;++j)trace.push_back(reader.get<Row>());auto view=c.view();auto hp=c.sheets[5][36],maximum=c.sheets[5][38];auto delta=maximum;auto sum=std::uint32_t(hp)+std::uint32_t(delta);std::int32_t signed_sum;std::memcpy(&signed_sum,&sum,4);if(signed_sum>maximum){auto difference=std::uint32_t(maximum)-std::uint32_t(hp);std::memcpy(&delta,&difference,4);}c.phase=delta>0?36:41;
  const ScriptInitVitals32 binding{0xabcdef0123456789ull,&view,{&c,Fixture::debug}};ScriptInitVitals24 out{};active=&c;check(dh2_character_script_init_vitals(&out,&binding)==1);active=nullptr;check(!std::memcmp(&out,&result,24));for(unsigned j=0;j<4;++j)check(c.sheets[j+2]==expected[j]);check(c.calls==trace);calls+=n;
 }
 check(reader.file.peek()==EOF);Fixture c{};c.sheets[1][36]=c.sheets[1][41]=8;c.sheets[5][38]=c.sheets[5][43]=256;auto view=c.view();ScriptInitVitals32 binding{1,&view,{&c,Fixture::debug}};ScriptInitVitals24 out{},before{};unsigned guards=0;
 auto reject=[&](ScriptInitVitals24* result,const ScriptInitVitals32* b){auto old=c.sheets;before=out;auto n=c.calls.size();check(dh2_character_script_init_vitals(result,b)==-1);check(c.sheets==old&&c.calls.size()==n&&!std::memcmp(&out,&before,24));++guards;};
 reject(nullptr,&binding);reject(&out,nullptr);auto invalid=binding;invalid.owner=0;reject(&out,&invalid);invalid=binding;invalid.debug.call=nullptr;reject(&out,&invalid);reject(reinterpret_cast<ScriptInitVitals24*>(&binding),&binding);invalid=binding;invalid.properties=nullptr;reject(&out,&invalid);
 for(auto p:{view.defaults,view.types,view.base,static_cast<const int*>(view.saved),view.gear,static_cast<const int*>(view.resolved)})reject(reinterpret_cast<ScriptInitVitals24*>(const_cast<int*>(p)),&binding);
 Sheet buff{};std::array<const std::int32_t*,4> pointers{buff.data(),nullptr,nullptr,nullptr};std::array<dh2::data::PropertyBuffGroup,2> groups{{{pointers.data(),1},{nullptr,0}}};view.groups=groups.data();view.group_count=1;
 reject(reinterpret_cast<ScriptInitVitals24*>(groups.data()),&binding);reject(reinterpret_cast<ScriptInitVitals24*>(pointers.data()),&binding);reject(reinterpret_cast<ScriptInitVitals24*>(buff.data()),&binding);check(groups[0].sheets==pointers.data()&&groups[0].count==1&&pointers[0]==buff.data());view.groups=nullptr;view.group_count=0;
 unsigned failures=0;for(unsigned op:{1,3}){c.calls.clear();c.fail_at=op;auto old=c.sheets;check(dh2_character_script_init_vitals(&out,&binding)==-2&&c.sheets==old&&c.calls.size()==op/2+1);++failures;}
 std::cout<<"{\"validation\":\"PASS\",\"gold_cases\":"<<cases<<",\"ordered_debug_property_services\":"<<calls<<",\"atomic_guards\":"<<guards<<",\"required_failure_prefixes\":"<<failures<<",\"checks\":"<<checks<<",\"mismatches\":0}\n";
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
