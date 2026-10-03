#include "animation_scheduler.hpp"
#include <fstream>
#include <iostream>
#include <iterator>
#include <set>
#include <stdexcept>
using namespace dh2::data;
std::vector<std::uint8_t> read(const std::string& path){std::ifstream f(path,std::ios::binary);return {std::istreambuf_iterator<char>(f),{}};}
Bytes bytes(const std::vector<std::uint8_t>& b){return {b.data(),b.size()};}
void check(bool b,const char* why){if(!b)throw std::runtime_error(why);}
int main(int argc,char** argv){try{
 if(argc!=2)return 2;std::string root=argv[1],error;auto a=read(root+"/animations_pyarray.bin"),b=read(root+"/animations_pyarraynames.bin"),c=read(root+"/animations_pystructnames.bin"),d=read(root+"/animations_dictionary_pyarraynames.bin"),e=read(root+"/animations_dictionary_pyarray.bin");Dictionary clips;AnimationTables table;
 check(load_dictionary(bytes(d),bytes(e),clips,error),error.c_str());check(load_animation_tables(bytes(a),bytes(b),bytes(c),clips,table,error),error.c_str());
 unsigned attack_completions=0;for(int character:{62,64,24}){
  AnimationRandom rng;AnimationScheduler scheduler;const auto* state=animation_state(table,character,"Attack");check(state,"Missing attack");
  check(scheduler.start(table,state-table.sequences.data(),rng,error),error.c_str());check(scheduler.frames().size()==2,"Attack did not redirect");unsigned ended=0;
  while(scheduler.active()){check(animation_clip(scheduler.clip(),clips),"Missing attack clip path");check(scheduler.complete(table,rng,error),error.c_str());check(++ended<=3,"Attack failed to finish after three stages");}
  check(ended==3,"Attack stage count differs");attack_completions+=ended;
  state=animation_state(table,character,"Died");check(scheduler.start(table,state-table.sequences.data(),rng,error),error.c_str());auto last=scheduler.clip().anim;check(scheduler.complete(table,rng,error)&&!scheduler.active()&&scheduler.clip().anim==last,"Death did not finish with last clip retained");
 }
 AnimationRandom rng;AnimationScheduler idle;check(idle.start(table,596,rng,error),error.c_str());std::set<int> idle_clips{idle.clip().anim};for(unsigned i=0;i<200;++i){check(idle.complete(table,rng,error)&&idle.active(),"Infinite idle stopped");idle_clips.insert(idle.clip().anim);}check(idle_clips.size()==2&&rng.calls==201,"Idle did not reselect original alternatives");
 idle.stop_loop();check(idle.complete(table,rng,error)&&!idle.active(),"StopLoop did not finish current layer");
 AnimationTables synthetic;synthetic.sequences.resize(2);auto& sequence=synthetic.sequences[0];sequence.type=1;sequence.loop=2;for(int id:{10,11,12}){AnimationStep step;step.anim=id;sequence.steps.push_back(step);}
 AnimationScheduler ordered;check(ordered.start(synthetic,0,rng,error),error.c_str());std::vector<int> played;while(ordered.active()){played.push_back(ordered.clip().anim);check(ordered.complete(synthetic,rng,error),error.c_str());check(played.size()<=9,"Finite loop did not stop");}
 check(played==std::vector<int>({10,11,12,10,11,12,10,11,12}),"Finite sequence loop differs");
 auto& parent=synthetic.sequences[1];parent.type=1;parent.loop=0;AnimationStep redirect;redirect.redir=1;redirect.anim=0;AnimationStep last;last.anim=20;parent.steps={redirect,last};sequence.loop=0;
 check(ordered.start(synthetic,1,rng,error),error.c_str());played.clear();while(ordered.active()){played.push_back(ordered.clip().anim);check(ordered.complete(synthetic,rng,error),error.c_str());check(played.size()<=4,"Parent did not advance after child");}check(played==std::vector<int>({10,11,12,20}),"Parent sequence unwind differs");
 check(idle.start(table,596,rng,error),error.c_str());auto before=rng;auto previous=idle.clip().anim;synthetic.sequences[0].steps={redirect};
 check(!idle.start(synthetic,0,rng,error)&&idle.active()&&idle.clip().anim==previous&&rng.seed==before.seed&&rng.calls==before.calls,"Recursive start changed active state");
 unsigned completed=0,starts=0,unsupported=0;for(unsigned id=0;id<table.sequences.size();++id){AnimationScheduler s;auto r=rng;if(!s.start(table,id,r,error)){++unsupported;continue;}++starts;
  for(unsigned i=0;i<40&&s.active();++i){if(!s.complete(table,r,error)){++unsupported;break;}++completed;check(s.frames().size()<=3,"Scheduler depth exceeded");}}
 std::cout<<"{\"original_sequences\":785,\"original_starts\":"<<starts<<",\"completion_calls\":"<<completed<<",\"empty_or_event_only_starts_or_steps\":"<<unsupported<<",\"three_monster_attacks_completed\":"<<attack_completions<<",\"death_completes\":true,\"idle_reselection_calls\":200,\"finite_sequence_callbacks\":9,\"nested_parent_callbacks\":4,\"recursive_redirect_rejected_atomically\":true}\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 3;}}
