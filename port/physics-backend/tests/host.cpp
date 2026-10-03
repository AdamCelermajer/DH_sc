#include "scene.hpp"
#include <Box2D.h>
#include <fstream>
#include <cstring>
#include <cmath>
#include <iostream>
#include <stdexcept>
using namespace dh2::backend_audit;
static void require(bool value,const char* message){if(!value)throw std::runtime_error(message);}
template<class T>static T read(std::ifstream& in){T value{};in.read(reinterpret_cast<char*>(&value),sizeof value);require(bool(in),"fixture truncated");return value;}
static bool floats(const void* x,const void* y,std::size_t size){const auto* a=static_cast<const unsigned char*>(x);const auto* b=static_cast<const unsigned char*>(y);for(std::size_t i=0;i<size;i+=4){float p,q;std::memcpy(&p,a+i,4);std::memcpy(&q,b+i,4);if(std::memcmp(a+i,b+i,4)&&!(std::isnan(p)&&std::isnan(q)))return false;}return true;}
static bool equal(const Snapshot& a,const Snapshot& b){if(std::memcmp(&a,&b,28)||!floats(&a.inverse_dt,&b.inverse_dt,4))return false;for(int i=0;i<4;++i){if(std::memcmp(&a.bodies[i],&b.bodies[i],16)||!floats(a.bodies[i].values,b.bodies[i].values,132))return false;}return true;}
int main(int argc,char** argv){try{require(argc==2,"full backend audit needs original fixture");std::ifstream in(argv[1],std::ios::binary);require(read<std::uint32_t>(in)==0x31504842,"fixture magic");const auto count=read<std::uint32_t>(in);std::uint64_t steps=0,events=0;
 for(std::uint32_t i=0;i<count;++i){const auto scene=read<Scene>(in);const auto expected=read<Snapshot>(in);Snapshot actual{};const auto allocated_before=b2_byteCount;require(dh2_backend_audit_scene(&actual,&scene)==0,"scene rejected");if(!equal(expected,actual)){std::cerr<<"backend scene "<<i<<"\n";const auto* a=reinterpret_cast<const std::uint32_t*>(&expected);const auto* b=reinterpret_cast<const std::uint32_t*>(&actual);for(int j=0;j<156;++j)if(a[j]!=b[j])std::cerr<<j<<" "<<std::hex<<a[j]<<" "<<b[j]<<std::dec<<"\n";throw std::runtime_error("backend state differs");}require(b2_byteCount==allocated_before,"backend leaked allocation accounting");steps+=scene.steps;events+=actual.added+actual.persisted+actual.removed+actual.resolved;}
 require(in.peek()==std::char_traits<char>::eof(),"fixture suffix");std::cout<<"{\"scenes\":"<<count<<",\"world_steps\":"<<steps<<",\"contact_listener_events\":"<<events<<",\"allocation_accounting_balanced\":true,\"mismatches\":0}\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<"\n";return 1;}}
