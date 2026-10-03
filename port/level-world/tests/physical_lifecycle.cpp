#include "physical_lifecycle.hpp"
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <vector>
using namespace dh2::physical;
namespace {
void require(bool v,const char* s){if(!v)throw std::runtime_error(s);}
struct Reader {
 std::vector<unsigned char> bytes;std::size_t at=0;
 explicit Reader(const char* path){std::ifstream f(path,std::ios::binary);bytes={std::istreambuf_iterator<char>(f),{}};}
 void read(void* p,std::size_t n){require(at<=bytes.size()&&n<=bytes.size()-at,"Truncated lifecycle gold");if(n)std::memcpy(p,bytes.data()+at,n);at+=n;}
 unsigned word(){unsigned v;read(&v,4);return v;}
};
bool nan(unsigned x){return (x&0x7f800000u)==0x7f800000u&&(x&0x7fffffu);}
void same(const void* a,const void* b,std::size_t n){for(std::size_t at=0;at<n;at+=4){unsigned x,y;std::memcpy(&x,static_cast<const char*>(a)+at,4);std::memcpy(&y,static_cast<const char*>(b)+at,4);require(x==y||(nan(x)&&nan(y)),"Lifecycle field differs from original gold");}}
struct Event {unsigned kind,id,pinned,type;unsigned char payload[24];};
static_assert(sizeof(Event)==40);
struct Context {LifecycleBody* view;void* broadphase;std::vector<MassData> masses;std::vector<Event> events;};
unsigned ident(void* shape){return static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(shape)-0x100000000ull);}
void record(Context& c,unsigned kind,unsigned id,const void* payload,std::size_t n){Event event{kind,id,c.view->pinned,c.view->type,{}};std::memcpy(event.payload,payload,n);c.events.push_back(event);}
void compute(void* raw,void* shape,MassData* out){auto& c=*static_cast<Context*>(raw);const auto id=ident(shape);require(id<c.masses.size(),"Mass callback identity");*out=c.masses[id];record(c,1,id,out,16);}
void sweep(void* raw,void* shape,const float* center){auto& c=*static_cast<Context*>(raw);require(center==c.view->body->local_center,"Sweep radius center pointer");record(c,2,ident(shape),center,8);}
void refilter(void* raw,void* shape,void* broadphase,const BodyTransform* xf){auto& c=*static_cast<Context*>(raw);require(broadphase==c.broadphase,"Refilter broadphase pointer");record(c,3,ident(shape),xf,24);}
}
int main(int argc,char** argv){
 if(argc!=2)return 2;
 try{
  Reader r(argv[1]);require(r.word()==0x31434c50,"Invalid lifecycle gold");const unsigned cases=r.word();require(cases&&cases<100000,"Lifecycle case budget");unsigned computed=0,swept=0,refiltered=0,rejected=0,type_changes=0;
  const MassCallbacks callbacks{compute,sweep,refilter};
  for(unsigned ci=0;ci<cases;++ci){
   const auto op=r.word(),locked=r.word(),count=r.word(),events=r.word();require(op<4&&locked<=1&&count<=8&&events<=24,"Lifecycle record budget");BodyState state,expected_state;TransformBody body{};LifecycleBody view{};MassData mass;unsigned char expected_extra[48],expected_view[24];
   r.read(&state,48);r.read(reinterpret_cast<char*>(&body)+8,48);r.read(reinterpret_cast<char*>(&view)+8,24);r.read(&mass,16);Context context{&view,reinterpret_cast<void*>(0xb23456789abcdef0ull),std::vector<MassData>(count),{}};if(count)r.read(context.masses.data(),count*16);r.read(&expected_state,48);r.read(expected_extra,48);r.read(expected_view,24);std::vector<Event> expected_events(events);if(events)r.read(expected_events.data(),events*40);body.state=&state;view.body=&body;
   TransformShape chain[8]{};for(unsigned i=0;i<count;++i)chain[i]={reinterpret_cast<void*>(0x100000000ull+i),i+1<count?&chain[i+1]:nullptr};TransformWorld world{&context,context.broadphase,count?chain:nullptr,static_cast<std::uint8_t>(locked),{}};
   const auto original_state=state;const unsigned old_type=view.type;int status;
   switch(op){case 0:status=dh2_physical_set_mass(&view,&mass,&world,&callbacks);break;case 1:status=dh2_physical_mass_from_shapes(&view,&world,&callbacks);break;case 2:status=dh2_physical_pin(&view,&world,&callbacks);break;default:status=dh2_physical_unpin(&view,&world,&callbacks);break;}
   require(status==0,"Lifecycle rejected original input");same(&state,&expected_state,48);same(reinterpret_cast<char*>(&body)+8,expected_extra,48);same(reinterpret_cast<char*>(&view)+8,expected_view,24);require(!std::memcmp(state.linear_velocity,original_state.linear_velocity,24),"Mass lifecycle changed velocity/force");require(context.events.size()==events,"Lifecycle service count");if(events)same(context.events.data(),expected_events.data(),events*40);type_changes+=old_type!=view.type;
   for(const auto& event:context.events){computed+=event.kind==1;swept+=event.kind==2;refiltered+=event.kind==3;}
   if(!rejected){
    view.pinned=2;const auto before_state=state;const auto before_body=body;const auto before_view=view;const auto before_events=context.events.size();
    auto reject=[&](int result){require(result==1&&!std::memcmp(&state,&before_state,48)&&!std::memcmp(&body,&before_body,56)&&!std::memcmp(&view,&before_view,32)&&context.events.size()==before_events,"Malformed lifecycle caller mutated state/services");++rejected;};
    reject(dh2_physical_set_mass(&view,&mass,&world,&callbacks));reject(dh2_physical_mass_from_shapes(&view,&world,&callbacks));reject(dh2_physical_pin(&view,&world,&callbacks));reject(dh2_physical_unpin(&view,&world,&callbacks));
   }
  }
  require(r.at==r.bytes.size(),"Trailing lifecycle gold");std::cout<<"{\"comparisons\":"<<cases<<",\"compute_mass_checks\":"<<computed<<",\"sweep_radius_checks\":"<<swept<<",\"refilter_proxy_checks\":"<<refiltered<<",\"type_changes\":"<<type_changes<<",\"atomic_rejection_checks\":"<<rejected<<",\"mismatches\":0}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 3;}
}
