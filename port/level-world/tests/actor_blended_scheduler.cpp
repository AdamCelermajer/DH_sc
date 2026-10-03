#include "../../game-data/animation_scheduler.hpp"
#include <array>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
using namespace dh2;
namespace {
void check(bool b,const char* s){if(!b)throw std::runtime_error(s);}
struct Reader {
 std::vector<std::uint8_t> bytes;std::size_t offset=0;
 explicit Reader(const char* path){std::ifstream f(path,std::ios::binary);check(bool(f),"Missing scheduler corpus");bytes={std::istreambuf_iterator<char>(f),{}};}
 unsigned word(){check(offset+4<=bytes.size(),"Short scheduler corpus");unsigned out;std::memcpy(&out,bytes.data()+offset,4);offset+=4;return out;}
};
using Row=std::array<std::uint32_t,9>;
struct Context {
 unsigned pending=1,closed=0;int queued=-1;unsigned callback_event=0;int callback_request=-1;
 bool advance=false;std::vector<Row> trace;
 void record(data::AnimationScheduler& p,unsigned op,unsigned event,unsigned argument){
  const auto& frames=p.frames();check(!frames.empty(),"Retained source root frame disappeared");const auto& f=frames.back();
  trace.push_back({op,event,argument,pending,closed,unsigned(queued),f.step,unsigned(f.loops),unsigned(frames.size()-1)});
 }
};
void event(void* raw,data::AnimationScheduler& p,unsigned id){
 auto& c=*static_cast<Context*>(raw);if(id==0x22){if(c.closed)return;c.closed=1;}
 c.record(p,0,id,0);if(id==c.callback_event&&c.callback_request!=-1)c.queued=c.callback_request;
}
bool pending(void* raw){return static_cast<Context*>(raw)->queued!=-1;}
bool prepare(void* raw,data::AnimationScheduler& p){auto& c=*static_cast<Context*>(raw);if(c.advance)c.record(p,2,p.frames().back().step,0);return true;}
void initialize(void* raw,data::AnimationScheduler& p,int sequence,unsigned depth){auto& c=*static_cast<Context*>(raw);c.record(p,1,unsigned(sequence),depth);}
bool finish(void* raw,data::AnimationScheduler& p){auto& c=*static_cast<Context*>(raw);c.pending=0;if(c.queued!=-1){c.record(p,1,unsigned(c.queued),0);c.queued=-1;}return true;}
}
int main(int argc,char** argv){try{
 if(argc!=2)return 2;Reader r(argv[1]);check(r.word()==0x31535042,"BPS1 magic differs");const auto count=r.word();unsigned callbacks=0,preparations=0,nested=0,repeated=0;
 for(unsigned i=0;i<count;++i){const auto type=r.word(),steps=r.word();const int loops=int(r.word());const auto depth=r.word(),updates=r.word();const int initial=int(r.word());const auto callback=r.word();const int requested=int(r.word());r.word();const auto traces=r.word();std::vector<Row> expected(traces);for(auto& row:expected)for(auto& word:row)word=r.word();
  data::AnimationTables table;table.sequences.resize(2);auto& root=table.sequences[0];root.type=type;root.loop=loops;root.steps.resize(steps);for(auto& step:root.steps)step.anim=955;
  auto& child=table.sequences[1];child.steps.resize(1);child.steps[0].anim=955;
  if(depth){root.steps[0].redir=1;root.steps[0].anim=1;}
  data::AnimationScheduler scheduler;data::AnimationRandom random;std::string error;check(scheduler.start(table,0,random,error),error.c_str());
  Context context;context.queued=initial;context.callback_event=callback;context.callback_request=requested;context.advance=type==1;
  const data::AnimationCompletionServices services{&context,event,pending,prepare,finish,initialize};
  for(unsigned call=0;call<updates;++call){context.pending=1;check(scheduler.complete_with_services(table,random,error,services),error.c_str());}
  if(context.trace!=expected){std::cerr<<"Scheduler original trace differs row "<<i<<" type="<<type<<" depth="<<depth<<'\n';for(const auto& row:context.trace){for(auto word:row)std::cerr<<word<<',';std::cerr<<'\n';}return 3;}
  check(!context.pending&&context.queued==-1,"Common source tail differs");for(const auto& row:context.trace){callbacks+=row[0]==0;preparations+=row[0]!=0;}nested+=depth!=0;repeated+=updates>1;
 }
 check(r.offset==r.bytes.size(),"Trailing scheduler corpus bytes");std::cout<<"{\"validation\":\"PASS\",\"original_scheduling_cases\":"<<count<<",\"original_nested_unwind_cases\":"<<nested<<",\"retained_closed_repeat_cases\":"<<repeated<<",\"ordered_callbacks\":"<<callbacks<<",\"preparation_services\":"<<preparations<<",\"mismatches\":0}\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 3;}}
