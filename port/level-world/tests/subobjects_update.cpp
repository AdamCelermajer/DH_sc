#include "subobjects_update.hpp"
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <vector>
using namespace dh2::subobjects;
namespace {
void require(bool b,const char* message){if(!b)throw std::runtime_error(message);}
struct Reader {
 std::vector<unsigned char> bytes;std::size_t at=0;
 explicit Reader(const char* file){std::ifstream f(file,std::ios::binary);bytes={std::istreambuf_iterator<char>(f),{}};}
 void read(void* out,std::size_t n){require(at<=bytes.size()&&n<=bytes.size()-at,"Truncated subobjects reference");if(n)std::memcpy(out,bytes.data()+at,n);at+=n;}
 unsigned word(){unsigned v;read(&v,4);return v;}
};
bool same(const void* left,const void* right,std::size_t size){
 auto a=static_cast<const unsigned char*>(left),b=static_cast<const unsigned char*>(right);
 for(std::size_t at=0;at<size;at+=4){unsigned x,y;float xf,yf;std::memcpy(&x,a+at,4);std::memcpy(&y,b+at,4);std::memcpy(&xf,&x,4);std::memcpy(&yf,&y,4);if(x!=y&&!(std::isnan(xf)&&std::isnan(yf)))return false;}return true;
}
struct Fixture {unsigned body,valid,camera_present,camera_enabled,camera_valid,transform_apply,reserved;float root[3],validated[3],camera_position[3];};
struct EventRow {unsigned event;std::vector<unsigned char> payload;};
struct Context {State* state;dh2::physical::BodyState* body;Fixture fixture;std::vector<EventRow> events;};
std::uint32_t service(void* ptr,unsigned event,float* value){
 auto& c=*static_cast<Context*>(ptr);auto& s=*c.state;auto& f=c.fixture;const void* payload=nullptr;unsigned size=0;float camera_args[6];
 if(event==visual_sync_position||event==validate_position){payload=s.position;size=12;}
 else if(event==visual_sync_rotation){payload=&s.rotation;size=4;}
 else if(event==camera_can_move){std::memcpy(camera_args,s.position,12);std::memcpy(camera_args+3,s.heading,12);payload=camera_args;size=24;}
 else if(event==apply_body_transform||event==physical_set_velocity||event==camera_set_free||event==get_speed){payload=value;size=event==apply_body_transform?12:event==physical_set_velocity?8:4;}
 EventRow row{event,std::vector<unsigned char>(size)};if(size)std::memcpy(row.payload.data(),payload,size);c.events.push_back(row);
 if(event==visual_apply_position)std::memcpy(s.position,f.root,12);
 else if(event==validate_position){std::memcpy(value,f.validated,12);return f.valid;}
 else if(event==apply_body_transform&&f.transform_apply){std::memcpy(c.body->position,value,8);c.body->angle=value[2];}
 else if(event==camera_get){value[0]=float(f.camera_enabled);return f.camera_present;}
 else if(event==camera_can_move)return f.camera_valid;
 else if(event==camera_position)std::memcpy(value,f.camera_position,12);
 return 0;
}
}
int main(int argc,char** argv){
 if(argc!=2)return 2;
 try{
  Reader gold(argv[1]);require(gold.word()==0x31554253,"Invalid subobjects reference");const auto count=gold.word();require(count&&count<100000,"Subobjects case budget");unsigned calls=0;
  for(unsigned i=0;i<count;++i){
   const auto length=gold.word();const auto start=gold.at;require(length<10000,"Subobjects record budget");State state{},expected_state{};dh2::physical::BodyState body{},expected_body{};Policy policy{};Fixture fixture{};Result expected{};
   gold.read(&state,sizeof(state));gold.read(&body,sizeof(body));gold.read(&policy,sizeof(policy));gold.read(&fixture,sizeof(fixture));gold.read(&expected_state,sizeof(expected_state));gold.read(&expected_body,sizeof(expected_body));gold.read(&expected,sizeof(expected));
   std::vector<EventRow> events;const auto event_count=gold.word();require(event_count<=32,"Subobjects event budget");for(unsigned j=0;j<event_count;++j){const auto e=gold.word(),size=gold.word();require(size<=24&&size%4==0,"Subobjects payload budget");EventRow row{e,std::vector<unsigned char>(size)};gold.read(row.payload.data(),size);events.push_back(row);}
   require(gold.at-start==length,"Subobjects record length");Context context{&state,&body,fixture,{}};Services services{&context,service};dh2::physical::TransformRequest transform{};Request request{&state,fixture.body?&body:nullptr,fixture.body?&transform:nullptr,&policy,&services};Result result{};const auto saved_policy=policy;
   require(dh2_subobjects_update(&result,&request)==0,"Native subobjects rejected fixture");require(!std::memcmp(&policy,&saved_policy,sizeof(policy)),"Subobjects mutated policy");require(same(&state,&expected_state,sizeof(state)),"Subobjects state mismatch");require(same(&body,&expected_body,sizeof(body)),"Subobjects body mismatch");require(!std::memcmp(&result,&expected,sizeof(result)),"Subobjects decision mismatch");require(context.events.size()==events.size(),"Subobjects ordered call count");
   for(unsigned j=0;j<events.size();++j){require(events[j].event==context.events[j].event&&events[j].payload.size()==context.events[j].payload.size()&&same(events[j].payload.data(),context.events[j].payload.data(),events[j].payload.size()),"Subobjects ordered payload mismatch");++calls;}
  }
  require(gold.at==gold.bytes.size(),"Trailing subobjects reference");std::cout<<"{\"comparisons\":"<<count<<",\"ordered_calls\":"<<calls<<",\"mismatches\":0,\"service_backends_are_fixtures\":true}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 3;}
}
