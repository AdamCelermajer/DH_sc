#include "../actor_blended_playback.hpp"
#include <array>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
using namespace dh2;
namespace {void check(bool v,const char* m){if(!v)throw std::runtime_error(m);}struct Reader{std::vector<std::uint8_t> bytes;std::size_t at=0;explicit Reader(const char* path){std::ifstream f(path,std::ios::binary);check(bool(f),"Missing control corpus");bytes={std::istreambuf_iterator<char>(f),{}};}unsigned word(){check(at+4<=bytes.size(),"Short control corpus");unsigned v;std::memcpy(&v,bytes.data()+at,4);at+=4;return v;}};}
int main(int argc,char** argv){try{
 if(argc!=2)return 2;Reader r(argv[1]);check(r.word()==0x31435342,"BSC1 magic differs");const auto count=r.word();unsigned setters=0,skips=0,stops=0,getters=0;
 for(unsigned i=0;i<count;++i){const auto depth=r.word(),closed=r.word(),stop=r.word();const int loops=int(r.word());const auto step=r.word(),op=r.word(),arg=r.word();std::array<unsigned,4> expected;for(auto& v:expected)v=r.word();
  data::AnimationTables table;table.sequences.resize(depth+1);for(unsigned seq=0;seq<=depth;++seq){auto& s=table.sequences[seq];s.type=1;s.loop=loops;s.steps.resize(seq+2);for(auto& row:s.steps){row.anim=seq<depth?int(seq+1):955;row.redir=seq<depth;}}
  actor::BlendedPlayback p;data::AnimationRandom random;std::string error;check(p.scheduler.start(table,0,random,error),error.c_str());p.scheduler.set_step(step);p.sequence_closed=closed;p.stop_requested=stop;
  unsigned value=0;switch(op){case 0:p.set_step(arg);++setters;break;case 1:p.skip_next_step();++skips;break;case 2:p.stop_loop(arg!=0);++stops;break;case 3:value=p.step_index();++getters;break;case 4:value=p.step_count(table);++getters;break;default:return 3;}
  const auto& frame=p.scheduler.frames().back();const std::array<unsigned,4> actual{unsigned(frame.loops),frame.step,unsigned(p.stop_requested),value};if(actual!=expected){std::cerr<<"Control original differs row "<<i<<'\n';return 3;}check(p.animation_depth()==depth,"Live depth getter differs");
 }
 check(r.at==r.bytes.size(),"Trailing control corpus bytes");std::cout<<"{\"validation\":\"PASS\",\"original_control_cases\":"<<count<<",\"metadata_setters\":"<<setters<<",\"metadata_skips\":"<<skips<<",\"stop_loop_calls\":"<<stops<<",\"closed_getters\":"<<getters<<",\"mismatches\":0}\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 3;}}
