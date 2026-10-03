#include "health.hpp"
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
using namespace dh2::data;
namespace {
std::vector<std::uint8_t> read(const std::string& p){std::ifstream f(p,std::ios::binary);return {std::istreambuf_iterator<char>(f),{}};}
void check(bool v,const char* e){if(!v)throw std::runtime_error(e);}
template<class T>T load(const std::uint8_t* p){T v;std::memcpy(&v,p,sizeof v);return v;}
bool same(const PropertyState& a,const PropertyState& b){return a.base==b.base&&a.saved==b.saved&&a.gear==b.gear&&a.resolved==b.resolved;}
}
int main(int argc,char** argv){try{
 if(argc!=3)return 2;const std::string folder=argv[1];auto raw=read(folder+"/character_properties_pyarray.bin");check(raw.size()>=1800,"Missing original property rules");PropertyRules rules;
 std::memcpy(rules.defaults.data(),raw.data()+4,896);std::memcpy(rules.types.data(),raw.data()+900,896);
 const auto reference=read(argv[2]);check(reference.size()>=4,"Missing original health reference");const auto count=load<std::uint32_t>(reference.data());check(reference.size()==4+std::size_t(count)*7216,"Health reference dimensions differ");
 unsigned kills=0,cues=0,skips=0;PropertyState state{};HealthChange change{};
 for(unsigned i=0;i<count;++i){const auto* r=reference.data()+4+std::size_t(i)*7216;std::size_t at=0;
  for(auto* sheet:{&state.base,&state.saved,&state.gear,&state.resolved}){std::memcpy(sheet->data(),r+at,896);at+=896;}
  auto view=property_view(rules,state);HealthRequest request{&view,load<std::uint32_t>(r+3584),load<std::uint32_t>(r+3588),load<std::int32_t>(r+3592),load<std::uint32_t>(r+3596)};
  check(dh2_health_hit(&change,&request)==0,"Valid original health fixture rejected");at=3600;
  for(const auto* sheet:{&state.base,&state.saved,&state.gear,&state.resolved}){check(std::memcmp(sheet->data(),r+at,896)==0,"Health owner differs from original instructions");at+=896;}
  check(sizeof change==32&&std::memcmp(&change,r+7184,32)==0,"Health change differs from original instructions");kills+=change.kill_requested;cues+=change.low_health_cue;skips+=change.skipped_dead;
 }
 const auto before=state;auto view=property_view(rules,state);HealthRequest request{&view,256,4096,0,1};HealthChange output{1,2,3,4,5,6,7,8};const auto sentinel=output;
 check(dh2_health_hit(&output,&request)==1&&same(state,before)&&std::memcmp(&output,&sentinel,32)==0,"Invalid health flags changed state/output");request.facts=0;request.low_health_armed=2;
 check(dh2_health_hit(&output,&request)==1&&same(state,before)&&std::memcmp(&output,&sentinel,32)==0,"Invalid health hysteresis changed state/output");request.low_health_armed=1;view.types=nullptr;
 check(dh2_health_hit(&output,&request)==1&&same(state,before)&&std::memcmp(&output,&sentinel,32)==0&&dh2_health_hit(nullptr,&request)==1&&dh2_health_hit(&output,nullptr)==1,"Invalid health view changed state/output");
 std::cout<<"{\"original_health_cases\":"<<count<<",\"property_words_per_case\":896,\"health_change_words_per_case\":8,\"kill_requests\":"<<kills<<",\"low_health_cues\":"<<cues<<",\"dead_skips\":"<<skips<<",\"invalid_input_atomic\":true}\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 3;}}
