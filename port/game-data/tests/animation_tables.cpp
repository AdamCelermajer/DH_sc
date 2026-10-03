#include "animation_tables.hpp"
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <random>
#include <stdexcept>
using namespace dh2::data;
std::vector<std::uint8_t> read(const std::string& name){std::ifstream f(name,std::ios::binary);return {std::istreambuf_iterator<char>(f),{}};}
Bytes view(const std::vector<std::uint8_t>& b){return {b.data(),b.size()};}
void check(bool value,const char* message){if(!value)throw std::runtime_error(message);}
struct Writer{
 std::vector<std::uint8_t> bytes;
 void word(std::uint32_t value){for(unsigned i=0;i<4;++i)bytes.push_back(value>>(8*i));}
 void boolean(bool value){bytes.push_back(value?1:0);}
 void real(float value){std::uint32_t bits;std::memcpy(&bits,&value,4);word(bits);}
 void integers(const std::vector<std::int32_t>& values){word(values.size());for(auto value:values)word(value);}
};
int main(int argc,char** argv){try{
 if(argc!=2)return 2;std::string root=argv[1],error;auto records=read(root+"/animations_pyarray.bin"),names=read(root+"/animations_pyarraynames.bin"),fields=read(root+"/animations_pystructnames.bin"),keys=read(root+"/animations_dictionary_pyarraynames.bin"),paths=read(root+"/animations_dictionary_pyarray.bin");
 Dictionary clips;AnimationTables table;check(load_dictionary(view(keys),view(paths),clips,error),error.c_str());check(load_animation_tables(view(records),view(names),view(fields),clips,table,error),error.c_str());
 check(table.sequences.size()==785&&table.cameras.size()==3&&table.characters.size()==80&&clips.values.size()==1447,"Unexpected original dimensions");
 check(table.sequence_end==57952&&table.camera_end==58028&&table.data_consumed==72812,"Original section boundaries differ");
 Writer w;w.word(table.sequences.size());unsigned steps=0,redirects=0,states=0;
 for(const auto& sequence:table.sequences){w.word(sequence.loop);w.word(sequence.steps.size());for(const auto& s:sequence.steps){++steps;redirects+=s.redir==1;w.boolean(s.anchor_fx);w.word(s.anim);w.word(s.blend_out);w.word(s.cam);w.boolean(s.cam_dir);w.word(s.fx);w.boolean(s.move_go);w.integers(s.random_cam);w.word(s.redir);w.word(s.sound);w.real(s.speed);w.boolean(s.swoosh);}w.word(sequence.type);}
 w.word(table.cameras.size());for(const auto& camera:table.cameras){w.integers(camera.cam_anims);w.word(camera.crit);w.word(camera.idle);w.word(camera.shake);w.word(camera.template_id);}
 w.word(table.characters.size());for(const auto& c:table.characters)for(unsigned i=0;i<37;++i){if(i==15||i==31)w.integers(c.fields[i]);else{check(c.fields[i].size()==1,"Scalar dimension");w.word(c.fields[i][0]);}for(auto ref:c.fields[i])states+=ref>=0;}
 check(w.bytes==records,"Complete native roundtrip differs from original bytes");
 auto* skeleton=animation_state(table,62,"Idle");auto* slime=animation_state(table,64,"Idle");auto* ghost=animation_state(table,24,"Idle");
 check(skeleton==&table.sequences[596]&&skeleton->type==2&&skeleton->steps.size()==2&&skeleton->steps[0].anim==1194&&skeleton->steps[1].anim==1195,"Skeleton idle selection differs");
 check(slime==&table.sequences[613]&&slime->type==2&&slime->steps.size()==6&&slime->steps.back().anim==1259,"Slime idle alternatives differ");
 check(ghost==&table.sequences[210]&&ghost->steps.size()==1&&ghost->steps[0].speed==1.3f,"Ghost playback speed differs");
 auto* walk=animation_state(table,62,"Walk");auto* attack=animation_state(table,62,"Attack");auto* died=animation_state(table,62,"Died");
 check(walk==&table.sequences[604]&&attack==&table.sequences[590]&&died==&table.sequences[594],"Original state IDs differ");
 check(animation_clip(skeleton->steps[0],clips)&&animation_clip(skeleton->steps[0],clips)->find("skeleton_idle_01.bdae")!=std::string::npos,"Skeleton clip path differs");
 check(attack->steps[0].redir==1&&!animation_clip(attack->steps[0],clips),"Redirect mistaken for clip ID");
 check(!animation_state(table,-1,"Idle")&&!animation_state(table,80,"Idle")&&!animation_state(table,62,"absent")&&!animation_state(table,62,"Idle",1)&&!animation_state(table,62,"Interact",9),"Invalid state lookup accepted");
 AnimationRandom random;AnimationStart start;
 unsigned selected=0,empty=0;for(unsigned i=0;i<table.sequences.size();++i){bool ok=choose_animation_start(table,i,random,start,error);selected+=ok;empty+=!ok;if(ok)check(animation_clip(start.step,clips)&&start.layers.size()<=3,"Start selection failed to resolve clip");else check(start.layers.empty(),"Failed selection leaked layers");}
 random={1,0};check(choose_animation_start(table,596,random,start,error)&&start.step.anim==1194&&random.calls==1,"Original first skeleton random choice differs");
 check(choose_animation_start(table,590,random,start,error)&&start.layers.size()==2&&start.step.redir==0,"Original attack redirect did not resolve");
 auto recursive=table;recursive.sequences[0].steps.resize(1);recursive.sequences[0].type=0;recursive.sequences[0].steps[0].redir=1;recursive.sequences[0].steps[0].anim=0;
 const auto before=random;check(!choose_animation_start(recursive,0,random,start,error)&&start.layers.empty()&&before.seed==random.seed&&before.calls==random.calls,"Recursive redirect did not fail atomically");
 check(choose_animation_start(table,613,random,start,error,false)&&start.layers.back().second==0&&before.calls==random.calls,"Random-disabled selection differs");
 unsigned rejected=0;std::mt19937 rng(20261002);
 for(unsigned i=0;i<3000;++i){auto a=records,b=names,c=fields;auto* selected=i%3==0?&a:i%3==1?&b:&c;if(i%2)selected->resize(rng()%selected->size());else(*selected)[rng()%selected->size()]^=1u<<(rng()%8);
  AnimationTables changed;bool ok=load_animation_tables(view(a),view(b),view(c),clips,changed,error);if(!ok){++rejected;check(changed.sequences.empty()&&changed.characters.empty()&&changed.sequence_names.empty()&&changed.data_consumed==0,"Failed load leaked partial state");}}
 auto extra=records;extra.push_back(0);AnimationTables invalid;check(!load_animation_tables(view(extra),view(names),view(fields),clips,invalid,error),"Suffix accepted");
 std::cout<<"{\"sequences\":785,\"cameras\":3,\"characters\":80,\"clip_paths\":1447,\"steps\":"<<steps<<",\"redirects\":"<<redirects<<",\"positive_state_links\":"<<states<<",\"sequence_end\":57952,\"camera_end\":58028,\"consumed\":72812,\"native_roundtrip_matches_original\":true,\"starts_resolved\":"<<selected<<",\"starts_without_clip\":"<<empty<<",\"recursive_redirect_rejected\":true,\"mutations_and_truncations\":3000,\"rejected\":"<<rejected<<"}\n";
 return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<"\n";return 3;}}
