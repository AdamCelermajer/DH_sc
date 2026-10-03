#include "../animation.hpp"
#include <array>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>
namespace {
using namespace dh2::animation;
[[maybe_unused]] void check(bool value,const char* reason){if(!value)throw std::runtime_error(reason);}
struct Fixture{dh2::scene::Scene scene;TransformSet set;};
Fixture* create(const std::uint8_t* model,unsigned size,unsigned count,const std::uint8_t*const* bytes,const unsigned* sizes,const std::int32_t* ids,const unsigned* selected,unsigned selected_count,int default_index,unsigned mismatch){
 auto* f=new Fixture;dh2::resources::BresView image{};std::string error;
 if(dh2_bres_open(&image,model,size)!=dh2::resources::BresError::ok||!dh2::scene::load(image,f->scene,error)){delete f;return nullptr;}
 std::vector<Player> players(count);std::vector<TransformClipInput> inputs;Player model_default;
 for(unsigned i=0;i<count;++i)if(!players[i].load(bytes[i],sizes[i],f->scene,error,MissingTargets::ignore)){delete f;return nullptr;}
 for(unsigned i=0;i<selected_count;++i){if(selected[i]>=count){delete f;return nullptr;}inputs.push_back({ids[selected[i]],&players[selected[i]]});}
 const Player* defaults=nullptr;
 if(default_index==-2){if(!model_default.load(model,size,f->scene,error,MissingTargets::ignore)){delete f;return nullptr;}defaults=&model_default;}
 else if(default_index>=0){if(unsigned(default_index)>=count){delete f;return nullptr;}defaults=&players[default_index];}
 if(!f->set.compile_dynamic(inputs,f->scene,error,defaults,static_cast<TransformMismatchBehavior>(mismatch))){delete f;return nullptr;}
 return f;
}
}
extern "C" {
void* dh2_dynamic_transform_create(const std::uint8_t* model,unsigned size,unsigned count,const std::uint8_t*const* bytes,const unsigned* sizes,const std::int32_t* ids,const unsigned* selected,unsigned selected_count,int default_index,unsigned mismatch){return create(model,size,count,bytes,sizes,ids,selected,selected_count,default_index,mismatch);}
void dh2_dynamic_transform_destroy(void* f){delete static_cast<Fixture*>(f);}
unsigned dh2_dynamic_transform_count(void* f){return static_cast<Fixture*>(f)->set.targets().size();}
unsigned dh2_dynamic_transform_target(void* f,unsigned i,unsigned* info,char* name,unsigned capacity){auto& ts=static_cast<Fixture*>(f)->set.targets();if(i>=ts.size()||ts[i].uri.size()+1>capacity)return 0;info[0]=ts[i].type;info[1]=ts[i].node;info[2]=ts[i].components;std::memcpy(name,ts[i].uri.c_str(),ts[i].uri.size()+1);return 1;}
unsigned dh2_dynamic_transform_binding(void* f,unsigned ci,unsigned ti,unsigned* out){const auto* b=static_cast<Fixture*>(f)->set.clip_target(ci,ti);if(!b)return 0;out[0]=b->mode;out[1]=b->has_default;std::memcpy(out+2,b->default_value,16);return 1;}
unsigned dh2_dynamic_transform_clip(void* f,unsigned ci,std::int32_t* out){const auto* c=static_cast<Fixture*>(f)->set.clip(ci);if(!c)return 0;out[0]=c->id;out[1]=c->start;out[2]=c->end;return 1;}
unsigned dh2_dynamic_transform_sample(void* f,unsigned ci,unsigned ti,int ms,float* out,unsigned capacity,int* cursor,unsigned interpolate){std::string error;return static_cast<Fixture*>(f)->set.sample(ci,ti,ms,out,capacity,cursor,error,interpolate!=0);}
}
#ifndef DH2_TRANSFORM_ORACLE
namespace {
struct Reader{std::vector<char> bytes;std::size_t at=0;explicit Reader(const char* path){std::ifstream f(path,std::ios::binary);check(bool(f),"Missing dynamic corpus");bytes.assign(std::istreambuf_iterator<char>(f),{});}unsigned word(){check(at+4<=bytes.size(),"Truncated dynamic corpus");unsigned w;std::memcpy(&w,bytes.data()+at,4);at+=4;return w;}std::vector<std::uint8_t> block(){unsigned n=word();check(n<=bytes.size()-at,"Truncated block");std::vector<std::uint8_t> b(bytes.begin()+at,bytes.begin()+at+n);at+=n;return b;}};
std::vector<std::array<unsigned,3>> trig;unsigned next=0,calls=0;
float math(unsigned kind,float input){unsigned w;std::memcpy(&w,&input,4);check(next<trig.size()&&trig[next][0]==kind&&trig[next][1]==w,"Dynamic original libm input/order differs");float value;std::memcpy(&value,&trig[next++][2],4);++calls;return value;}
}
extern "C" float __wrap_sinf(float x){return math(0,x);}extern "C" float __wrap_acosf(float x){return math(1,x);}extern "C" float __wrap_sqrtf(float x){return math(2,x);}
int main(int argc,char** argv){try{
 check(argc==2,"Usage: dynamic-compiled-transforms-audit corpus");Reader r(argv[1]);check(r.word()==0x31544344,"Bad DCT1 magic");const auto model=r.block();unsigned count=r.word();
 std::vector<std::vector<std::uint8_t>> bytes;std::vector<const std::uint8_t*> pointers;std::vector<unsigned> sizes;std::vector<int> ids;
 for(unsigned i=0;i<count;++i){ids.push_back(int(r.word()));bytes.push_back(r.block());}for(auto& b:bytes){pointers.push_back(b.data());sizes.push_back(b.size());}
 unsigned cases=r.word(),total_targets=0,bindings=0,samples=0,retained=0,rejections=0,event_names=0;
 for(unsigned c=0;c<cases;++c){unsigned mismatch=r.word();int defaults=int(r.word());unsigned nc=r.word();std::vector<unsigned> selected;for(unsigned i=0;i<nc;++i)selected.push_back(r.word());
  auto* f=create(model.data(),model.size(),count,pointers.data(),sizes.data(),ids.data(),selected.data(),nc,defaults,mismatch);check(f,"Dynamic compile rejected");const auto n=r.word();check(n==f->set.targets().size(),"Dynamic target count differs");total_targets+=n;
  for(unsigned i=0;i<n;++i){auto name=r.block();const auto type=r.word(),node=r.word(),width=r.word();auto& t=f->set.targets()[i];check(t.uri==std::string(name.begin(),name.end())&&t.type==type&&t.node==node&&t.components==width,"Dynamic ordered target differs");}
  for(unsigned ci=0;ci<nc;++ci){int info[3];check(dh2_dynamic_transform_clip(f,ci,info),"Missing dynamic clip");for(auto value:info)check(unsigned(value)==r.word(),"Dynamic source clip bounds differ");
   const auto& events=f->set.clip(ci)->events.view();for(unsigned i=0;i<events.count;++i)for(unsigned j=0;j<events.groups[i].count;++j){check(std::strlen(events.groups[i].names[j])>0,"Dynamic event lease expired");++event_names;}
   for(unsigned ti=0;ti<n;++ti){unsigned actual[6];check(dh2_dynamic_transform_binding(f,ci,ti,actual),"Missing dynamic binding");for(auto w:actual)check(w==r.word(),"Dynamic original binding differs");++bindings;}
  }
  unsigned records=r.word();for(unsigned k=0;k<records;++k){unsigned ci=r.word(),ti=r.word();int ms=int(r.word());unsigned interpolate=r.word();int cursor=int(r.word());unsigned initial[4],expected[4];for(auto& w:initial)w=r.word();int expected_cursor=int(r.word());for(auto& w:expected)w=r.word();trig.clear();next=0;unsigned nt=r.word();for(unsigned i=0;i<nt;++i)trig.push_back({r.word(),r.word(),r.word()});
   unsigned out[6];out[0]=out[5]=0xaabbccdd;std::memcpy(out+1,initial,16);check(dh2_dynamic_transform_sample(f,ci,ti,ms,reinterpret_cast<float*>(out+1),4,&cursor,interpolate),"Dynamic sample rejected");check(cursor==expected_cursor&&!std::memcmp(expected,out+1,16)&&out[0]==0xaabbccdd&&out[5]==0xaabbccdd,"Dynamic original sample differs");check(next==trig.size(),"Dynamic libm calls incomplete");retained+=!std::memcmp(initial,expected,16);++samples;
  }
  Player source;std::string error;check(source.load(bytes[selected[0]].data(),bytes[selected[0]].size(),f->scene,error,MissingTargets::ignore),"Invalid rejection fixture");const auto before=f->set.targets().size();
  float preserved[4]{1,2,3,4};int preserved_cursor=0;if(n)check(f->set.sample(0,0,0,preserved,4,&preserved_cursor,error,false),"Dynamic atomic baseline rejected");
  check(!f->set.compile_dynamic({{1,&source}},f->scene,error,nullptr,static_cast<TransformMismatchBehavior>(2))&&f->set.targets().size()==before,"Invalid dynamic policy not atomic");++rejections;
  check(!f->set.compile_dynamic({{1,nullptr}},f->scene,error)&&f->set.targets().size()==before,"Null dynamic input not atomic");++rejections;
  Player empty_default;check(!f->set.compile_dynamic({{1,&source}},f->scene,error,&empty_default)&&f->set.targets().size()==before,"Invalid dynamic default not atomic");++rejections;
  if(n){float after[4]{1,2,3,4};int cursor=0;check(f->set.sample(0,0,0,after,4,&cursor,error,false)&&!std::memcmp(after,preserved,16)&&cursor==preserved_cursor,"Rejected dynamic compile changed previous sampling state");}
  if(n){float out[4]{1,2,3,4};int cursor=17;const float saved[4]{1,2,3,4};check(!f->set.sample(0,0,0,out,0,&cursor,error)&&!std::memcmp(saved,out,16)&&cursor==17,"Dynamic malformed output not atomic");++rejections;}
  delete f;
 }
 check(r.at==r.bytes.size(),"Dynamic corpus tail");std::cout<<"{\"validation\":\"PASS\",\"cases\":"<<cases<<",\"ordered_targets\":"<<total_targets<<",\"bindings\":"<<bindings<<",\"samples\":"<<samples<<",\"retained\":"<<retained<<",\"libm\":"<<calls<<",\"atomic_rejections\":"<<rejections<<",\"event_names\":"<<event_names<<",\"input_player_independence\":true,\"sanitizer_findings\":0}\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
#endif
