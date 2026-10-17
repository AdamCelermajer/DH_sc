#include "../../game-data/animation_scheduler.hpp"
#include <array>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
using namespace dh2;
namespace {
void check(bool value,const char* message){if(!value)throw std::runtime_error(message);}
struct Reader {std::vector<std::uint8_t> bytes;std::size_t at=0;explicit Reader(const char* path){std::ifstream f(path,std::ios::binary);check(bool(f),"Missing selection corpus");bytes={std::istreambuf_iterator<char>(f),{}};}unsigned word(){check(at+4<=bytes.size(),"Short selection corpus");unsigned v;std::memcpy(&v,bytes.data()+at,4);at+=4;return v;}};
using Row=std::array<std::uint32_t,9>;
struct Context {const data::AnimationTables* table;unsigned pending=0,swap=0;bool swapped=false;std::vector<Row> trace;void record(data::AnimationScheduler& p,unsigned op,unsigned value){const auto& f=p.frames().back();trace.push_back({op,value,pending,1,243,unsigned(f.sequence),f.step,unsigned(f.loops),unsigned(p.frames().size()-1)});}};
void event(void* raw,data::AnimationScheduler& p,unsigned id){auto& c=*static_cast<Context*>(raw);c.record(p,0,id);if(id==c.swap&&!c.swapped){c.swapped=true;std::string error;check(p.swap_sequences(*c.table,1,-1,error)==data::AnimationSwap::metadata,error.c_str());}}
bool prepare(void* raw,data::AnimationScheduler& p){static_cast<Context*>(raw)->record(p,1,unsigned(p.clip().anim));return true;}
struct FrameStepContext {std::vector<unsigned> order;};
void step_event(void* raw,data::AnimationScheduler&,std::uint32_t id){static_cast<FrameStepContext*>(raw)->order.push_back(id==0x26?2:1);}
bool step_prepare(void* raw,data::AnimationScheduler&){static_cast<FrameStepContext*>(raw)->order.push_back(3);return true;}
bool frame_step(void* raw,data::AnimationScheduler& p,std::uint32_t step,std::uint32_t count){
 auto& c=*static_cast<FrameStepContext*>(raw);if(p.frames().empty()||p.frames().back().step!=step||step>=count)return false;
 c.order.push_back(4);return true;
}
}
int main(int argc,char** argv){try{
 if(argc!=2)return 2;Reader r(argv[1]);check(r.word()==0x31535342,"BSS1 magic differs");const auto count=r.word();unsigned callbacks=0,nested=0,swaps=0;
 for(unsigned i=0;i<count;++i){const auto depth=r.word();const int loops=int(r.word());const auto pending=r.word(),swap=r.word();r.word();const auto n=r.word();std::vector<Row> expected(n);for(auto& row:expected)for(auto& v:row)v=r.word();
  data::AnimationTables table;table.sequences.resize(4);for(unsigned seq=0;seq<4;++seq){auto& s=table.sequences[seq];s.type=1;s.loop=loops;s.steps.resize(2);for(auto& step:s.steps){step.anim=depth&&seq<2?int(seq+2):int(955+seq);step.redir=depth&&seq<2;}}
  data::AnimationScheduler scheduler;data::AnimationRandom random;std::string error;Context c{&table,pending,swap};const data::AnimationSelectionServices services{&c,event,prepare};check(scheduler.start_with_services(table,0,random,error,services),error.c_str());
  if(c.trace!=expected){std::cerr<<"Selection original trace differs row "<<i<<'\n';for(const auto& row:c.trace){for(auto v:row)std::cerr<<v<<',';std::cerr<<'\n';}return 3;}callbacks+=n-1;nested+=depth;swaps+=swap!=0;
 }
 data::AnimationTables step_table;step_table.sequences.resize(1);data::AnimationStep selected;selected.anim=777;step_table.sequences[0].steps.push_back(selected);
 data::AnimationScheduler step_scheduler;data::AnimationRandom step_random;FrameStepContext step_context;
 const data::AnimationSelectionServices step_services{&step_context,step_event,step_prepare,frame_step};
 std::string step_error;check(step_scheduler.start_with_services(step_table,0,step_random,step_error,step_services),step_error.c_str());
 check(step_context.order==std::vector<unsigned>({1,2,3,4}),"Frame-step callback did not follow event26 and active-slot prepare");
 check(r.at==r.bytes.size(),"Trailing selection corpus bytes");std::cout<<"{\"validation\":\"PASS\",\"original_selection_cases\":"<<count<<",\"nested_selection_cases\":"<<nested<<",\"ordered_callbacks\":"<<callbacks<<",\"retained_row_swaps\":"<<swaps<<",\"post_store_frame_step_callback\":true,\"mismatches\":0}\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 3;}}
