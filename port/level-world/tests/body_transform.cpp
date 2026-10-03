#include "../body_transform.hpp"
#include <array>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace dh2::physical;
// Imported trig is the same controlled service as in the two Unicorn CPUs.
// Platform libm sinf/cosf can differ by one ULP; this audit checks the recovered
// kernel's arithmetic and callbacks, not cross-platform libm implementation.
static float imported_angle,imported_cos,imported_sin;
static std::uint64_t trig_calls;
extern "C" float cosf(float angle) noexcept {if(std::memcmp(&angle,&imported_angle,4))std::abort();++trig_calls;return imported_cos;}
extern "C" float sinf(float angle) noexcept {if(std::memcmp(&angle,&imported_angle,4))std::abort();++trig_calls;return imported_sin;}
// GCC may combine the adjacent pure imports in optimized audit builds.
extern "C" void sincosf(float angle,float* sine,float* cosine) noexcept {if(std::memcmp(&angle,&imported_angle,4))std::abort();trig_calls+=2;*sine=imported_sin;*cosine=imported_cos;}
struct Event {std::uint32_t kind,id;BodyTransform transform;};
struct Fixture {std::vector<Event> events;int fail;void* broadphase;};
static void require(bool value,const char* text){if(!value)throw std::runtime_error(text);}
static std::uint32_t sync(void* opaque,void* shape,void* broadphase,const BodyTransform* a,const BodyTransform* b){
 auto& f=*static_cast<Fixture*>(opaque);require(broadphase==f.broadphase&&a==b,"callback ABI");
 const auto id=static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(shape)-0x100000000ull);
 f.events.push_back({1,id,*a});return static_cast<int>(id)!=f.fail;
}
static void destroy(void* opaque,void* shape,void* broadphase){auto& f=*static_cast<Fixture*>(opaque);require(broadphase==f.broadphase,"destroy ABI");f.events.push_back({2,static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(shape)-0x100000000ull),{}});}
static void commit(void* opaque,void* broadphase){auto& f=*static_cast<Fixture*>(opaque);require(broadphase==f.broadphase,"commit ABI");f.events.push_back({3,0,{}});}
static bool equal(const void* expected,const void* actual,std::size_t size){
 const auto* a=static_cast<const unsigned char*>(expected);const auto* b=static_cast<const unsigned char*>(actual);
 for(std::size_t i=0;i<size;i+=4){std::uint32_t x,y;float xf,yf;std::memcpy(&x,a+i,4);std::memcpy(&y,b+i,4);if(x==y)continue;std::memcpy(&xf,&x,4);std::memcpy(&yf,&y,4);if(std::isnan(xf)&&std::isnan(yf))continue;return false;}return true;
}
template<class T>static T read(std::ifstream& in){T value{};in.read(reinterpret_cast<char*>(&value),sizeof value);require(bool(in),"fixture truncated");return value;}
int main(int argc,char** argv){try{
 require(argc==2,"body transform test needs original reference fixture");std::ifstream in(argv[1],std::ios::binary);require(read<std::uint32_t>(in)==0x31544642,"fixture magic");const auto count=read<std::uint32_t>(in);std::uint64_t events=0;
 const TransformCallbacks callbacks{sync,destroy,commit};
 for(std::uint32_t i=0;i<count;++i){
  const auto flags=read<std::uint32_t>(in),locked=read<std::uint32_t>(in),shapes=read<std::uint32_t>(in),event_count=read<std::uint32_t>(in);const auto fail=read<std::int32_t>(in);const auto result=read<std::uint32_t>(in);BodyState state=read<BodyState>(in);require(state.flags==flags,"input flags");
  auto extra=read<std::array<unsigned char,48>>(in);TransformBody body{};body.state=&state;std::memcpy(&body.rotation,extra.data(),48);auto request=read<TransformRequest>(in);const auto expected_state=read<BodyState>(in);auto expected_extra=read<std::array<unsigned char,48>>(in);
  imported_angle=request.angle;std::memcpy(&imported_cos,expected_extra.data(),4);std::memcpy(&imported_sin,expected_extra.data()+4,4);
  Fixture fixture{{},fail,reinterpret_cast<void*>(0xb23456789abcdef0ull)};std::vector<TransformShape> chain(shapes);for(std::size_t j=0;j<chain.size();++j)chain[j]={reinterpret_cast<void*>(0x100000000ull+j),j+1<chain.size()?&chain[j+1]:nullptr};TransformWorld world{&fixture,fixture.broadphase,chain.empty()?nullptr:chain.data(),static_cast<std::uint8_t>(locked),{}};
  require(dh2_body_set_transform(&body,&request,&world,&callbacks)==static_cast<int>(result),"return differs");
  if(!equal(&expected_state,&state,48)||!equal(expected_extra.data(),&body.rotation,48)){std::cerr<<"state fixture "<<i<<"\n";throw std::runtime_error("state differs from original fixture");}
  require(fixture.events.size()==event_count,"event count");for(std::uint32_t j=0;j<event_count;++j){const auto event=read<Event>(in);require(event.kind==fixture.events[j].kind&&event.id==fixture.events[j].id&&equal(&event.transform,&fixture.events[j].transform,24),"ordered callback differs");}events+=event_count;
 }
 require(in.peek()==std::char_traits<char>::eof(),"unexpected fixture suffix");
 // Native contract failures must leave all logical state unchanged.
 BodyState state{};state.linear_velocity[0]=4;TransformBody body{};body.state=&state;TransformRequest request{{1,2},3,1};Fixture fixture{{},-1,nullptr};TransformWorld world{&fixture,nullptr,nullptr,0,{}};const auto before=state;
 require(dh2_body_set_transform(nullptr,&request,&world,&callbacks)==-1,"null guard");body.reserved=1;require(dh2_body_set_transform(&body,&request,&world,&callbacks)==-1,"body reserved guard");body.reserved=0;world.reserved[2]=1;require(dh2_body_set_transform(&body,&request,&world,&callbacks)==-1,"world reserved guard");world.reserved[2]=0;state.flags=0x10000;require(dh2_body_set_transform(&body,&request,&world,&callbacks)==-1,"native flags guard");state.flags=0;TransformCallbacks missing=callbacks;missing.commit=nullptr;require(dh2_body_set_transform(&body,&request,&world,&missing)==-1,"callback guard");require(std::memcmp(&before,&state,sizeof state)==0,"rejection changed state");
 std::cout<<"{\"comparisons\":"<<count<<",\"callback_events\":"<<events<<",\"modeled_trig_import_calls\":"<<trig_calls<<",\"malformed_checks\":5,\"mismatches\":0}\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<"\n";return 1;}}
