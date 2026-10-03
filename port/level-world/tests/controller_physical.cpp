#include "controller_physical.hpp"
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <vector>
using namespace dh2::navigation;
using namespace dh2::physical;
// Feed the same verified imported trig outputs as the ARM32/ARM64 instruction
// oracle. This isolates recovered orchestration/arithmetic from host-libm ULP.
static float imported_angle,imported_cos,imported_sin;
static unsigned trig_calls=0;
extern "C" float cosf(float angle) noexcept {if(std::memcmp(&angle,&imported_angle,4))std::abort();++trig_calls;return imported_cos;}
extern "C" float sinf(float angle) noexcept {if(std::memcmp(&angle,&imported_angle,4))std::abort();++trig_calls;return imported_sin;}
extern "C" void sincosf(float angle,float* sine,float* cosine) noexcept {if(std::memcmp(&angle,&imported_angle,4))std::abort();trig_calls+=2;*sine=imported_sin;*cosine=imported_cos;}
namespace {
void require(bool v,const char* s){if(!v)throw std::runtime_error(s);}
struct Reader {
 std::vector<unsigned char> bytes;std::size_t at=0;
 explicit Reader(const char* path){std::ifstream f(path,std::ios::binary);bytes={std::istreambuf_iterator<char>(f),{}};}
 void read(void* p,std::size_t n){require(at<=bytes.size()&&n<=bytes.size()-at,"Truncated controller physical gold");std::memcpy(p,bytes.data()+at,n);at+=n;}
 unsigned word(){unsigned v;read(&v,4);return v;}
};
unsigned current_case=0;
bool nan(unsigned u){return (u&0x7f800000u)==0x7f800000u&&(u&0x7fffffu);}
void same(const void* a,const void* b,std::size_t n,const char* label="events"){
 for(std::size_t at=0;at<n;at+=4){unsigned x,y;std::memcpy(&x,static_cast<const char*>(a)+at,4);std::memcpy(&y,static_cast<const char*>(b)+at,4);if(x!=y&&!(nan(x)&&nan(y)))throw std::runtime_error("Controller physical gold mismatch case "+std::to_string(current_case)+" "+label+" word "+std::to_string(at/4)+" actual "+std::to_string(x)+" expected "+std::to_string(y));}
}
struct Event {unsigned kind,id;BodyTransform transform;};
static_assert(sizeof(Event)==32);
struct Context {std::vector<Event> events;int fail;void* broadphase;};
std::uint32_t sync(void* raw,void* identity,void* bp,const BodyTransform* a,const BodyTransform* b){
 auto& c=*static_cast<Context*>(raw);require(bp==c.broadphase&&a==b,"Physical sync arguments/order");const auto id=static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(identity)-0x100000000ull);c.events.push_back({1,id,*a});return int(id)!=c.fail;
}
void destroy(void* raw,void* identity,void* bp){auto& c=*static_cast<Context*>(raw);require(bp==c.broadphase,"Physical destroy args");c.events.push_back({2,static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(identity)-0x100000000ull),{}});}
void commit(void* raw,void* bp){auto& c=*static_cast<Context*>(raw);require(bp==c.broadphase,"Physical commit args");c.events.push_back({3,0,{}});}
void snapshot_path(unsigned char* bytes,const PathObject& p){std::memcpy(bytes,&p,68);std::memcpy(bytes+68,&p.count,4);std::memcpy(bytes+72,&p.owned,4);}
}
int main(int argc,char** argv){
 if(argc!=2)return 2;
 try{
  Reader r(argv[1]);require(r.word()==0x31465043,"Invalid controller physical gold");const auto cases=r.word();require(cases&&cases<100000,"Controller physical case budget");unsigned applied=0,stopped=0,synced=0,destroyed=0,committed=0,rejections=0;
  for(unsigned ci=0;ci<cases;++ci){
   current_case=ci;
   const auto present=r.word(),locked=r.word(),shape_count=r.word(),event_count=r.word();const int failed=static_cast<int>(r.word());require(present<=1&&locked<=1&&shape_count<=8&&event_count<=17,"Physical controller budgets");
   ControllerPolicy policy;PathController state,expected_state;AvoidanceActor actor{};NavigationObject expected_object;unsigned char before_path[76],expected_path[76];BodyState bs,expected_bs;TransformBody body{};unsigned char expected_extra[48];ControllerPhysicalResult result{},expected_result;
   r.read(&policy,16);r.read(&state,56);r.read(&actor.object,64);r.read(before_path,76);r.read(&bs,48);r.read(reinterpret_cast<char*>(&body)+8,48);r.read(&expected_state,56);r.read(&expected_object,64);r.read(expected_path,76);r.read(&expected_result,88);r.read(&expected_bs,48);r.read(expected_extra,48);std::vector<Event> expected_events(event_count);if(event_count)r.read(expected_events.data(),event_count*32);
   imported_angle=bs.angle;std::memcpy(&imported_cos,expected_extra,4);std::memcpy(&imported_sin,expected_extra+4,4);
   PathObject path{};std::memcpy(&path,before_path,68);std::memcpy(&path.count,before_path+68,4);std::memcpy(&path.owned,before_path+72,4);require(!path.count&&!path.owned,"Physical controller corpus requires empty path");actor.physical.present=present;std::uint64_t key=0x100000001ull;AvoidanceScene scene{nullptr,&actor,&key,1,0};ControllerWorkspace workspace{};ControllerRequest request{&state,&path,&actor.object,nullptr,nullptr,&scene,&policy,&workspace,key};body.state=&bs;
   Context context{{},failed,reinterpret_cast<void*>(0xb23456789abcdef0ull)};TransformShape shapes[8]{};for(unsigned i=0;i<shape_count;++i)shapes[i]={reinterpret_cast<void*>(0x100000000ull+i),i+1<shape_count?&shapes[i+1]:nullptr};TransformWorld world{&context,context.broadphase,shape_count?shapes:nullptr,static_cast<std::uint8_t>(locked),{}};TransformCallbacks callbacks{sync,destroy,commit};ControllerPhysicalBinding binding{&body,&world,&callbacks};
   require(dh2_nav_update_path_physical(&result,&request,present?&binding:nullptr)==0,"Physical controller rejected original input");same(&state,&expected_state,56,"controller");same(&actor.object,&expected_object,64,"object");unsigned char actual_path[76];snapshot_path(actual_path,path);same(actual_path,expected_path,76,"path");same(&result,&expected_result,88,"result");same(&bs,&expected_bs,48,"body");same(reinterpret_cast<char*>(&body)+8,expected_extra,48,"transform");require(context.events.size()==expected_events.size(),"Physical callback event count");if(event_count)same(context.events.data(),expected_events.data(),event_count*32);
   applied+=result.stop_applied;stopped+=result.path.stopped;for(const auto& event:context.events){synced+=event.kind==1;destroyed+=event.kind==2;committed+=event.kind==3;}
   if(!rejections){
    auto reject=[&](const ControllerPhysicalBinding* selected){const auto before_state=state;const auto before_body=bs;const auto before_object=actor.object;const auto before_result=result;const auto before_transform=body;const auto before_native_path=path;const auto before_events=context.events.size();require(dh2_nav_update_path_physical(&result,&request,selected)==1&&!std::memcmp(&state,&before_state,56)&&!std::memcmp(&actor.object,&before_object,64)&&!std::memcmp(&result,&before_result,88)&&!std::memcmp(&bs,&before_body,48)&&!std::memcmp(&body,&before_transform,56)&&!std::memcmp(&path,&before_native_path,96)&&before_events==context.events.size(),"Malformed physical binding mutated caller state");++rejections;};
    policy.update_physics=1;actor.physical.present=1;reject(nullptr);actor.physical.present=0;reject(&binding);actor.physical.present=1;body.reserved=1;reject(&binding);body.reserved=0;world.reserved[0]=1;reject(&binding);world.reserved[0]=0;callbacks.commit=nullptr;reject(&binding);callbacks.commit=commit;
   }
  }
  require(r.at==r.bytes.size(),"Trailing controller physical gold");std::cout<<"{\"comparisons\":"<<cases<<",\"stopped\":"<<stopped<<",\"physical_stop_applied\":"<<applied<<",\"shape_sync_checks\":"<<synced<<",\"destroy_proxy_checks\":"<<destroyed<<",\"commit_checks\":"<<committed<<",\"modeled_trig_import_calls\":"<<trig_calls<<",\"atomic_rejection_checks\":"<<rejections<<",\"mismatches\":0}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 3;}
}
