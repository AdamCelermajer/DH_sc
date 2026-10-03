#include "physical_world.hpp"
#include "native_body.hpp"
#include <array>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <vector>
using namespace dh2::physical;
namespace {
void require(bool v,const char* m){if(!v)throw std::runtime_error(m);}
struct Reader {
 std::vector<unsigned char> bytes;std::size_t at=0;
 explicit Reader(const char* p){std::ifstream f(p,std::ios::binary);bytes={std::istreambuf_iterator<char>(f),{}};}
 void read(void* p,std::size_t n){require(at<=bytes.size()&&n<=bytes.size()-at,"Truncated world reference");std::memcpy(p,bytes.data()+at,n);at+=n;}
 unsigned word(){unsigned u;read(&u,4);return u;}
};
struct Event {unsigned kind,who,other,length;unsigned char data[16];};
static_assert(sizeof(Event)==32);
std::vector<Event> events;
struct Context {unsigned who,accept;float velocity[2];};
void emit(unsigned kind,Context* a,Context* b,const void* data,unsigned n){Event e{kind,a->who,b?b->who:0,n,{}};if(n)std::memcpy(e.data,data,n);events.push_back(e);}
unsigned test(void* raw,void* other,const Filter* a,const Filter* b){auto* c=static_cast<Context*>(raw);Filter filters[2]{*a,*b};emit(4,c,static_cast<Context*>(other),filters,16);return c->accept;}
void velocity(void* raw,float* v){auto* c=static_cast<Context*>(raw);std::memcpy(v,c->velocity,8);emit(5,c,nullptr,nullptr,0);}
void contact(void* raw,ContactEvent kind,void* other,const float* point,unsigned instigator){unsigned char data[12]{};const auto event=static_cast<unsigned>(kind);if(point)std::memcpy(data,point,8);std::memcpy(data+(point?8:0),&instigator,4);emit(event,static_cast<Context*>(raw),static_cast<Context*>(other),data,point?12:4);}
WorldObject object(Context* c){return {c,test,contact,velocity,{0,0},0,0};}
}
int main(int argc,char** argv){
 if(argc!=2)return 2;
 try {
  Reader r(argv[1]);require(r.word()==0x31575750,"Invalid world reference");const unsigned cases=r.word(),timing=r.word();unsigned observations=0;
  for(unsigned i=0;i<cases;++i){
   const auto op=r.word(),present=r.word();Context c[2]{{0,r.word(),{}},{1,r.word(),{}}};Filter f[2];r.read(f,16);float values[2][5];r.read(values,40);float coordinate[2];r.read(coordinate,8);const auto expected=r.word(),count=r.word();
   WorldObject o[2]{object(&c[0]),object(&c[1])};WorldShape s[2];
   for(unsigned j=0;j<2;++j){std::memcpy(o[j].position,values[j],8);o[j].mass=values[j][2];std::memcpy(c[j].velocity,values[j]+3,8);s[j]={present&(1u<<j)?&o[j]:nullptr,f[j]};}
   events.clear();int result;
   if(op==4)result=dh2_physical_world_should_collide(&s[0],&s[1]);
   else {WorldContact p{{s[0],s[1]},{coordinate[0],coordinate[1]}};result=dh2_physical_world_contact(&p,op);}
   require(result==static_cast<int>(expected)&&events.size()==count,"World return/callback count differs");
   for(const auto& event:events){Event reference;r.read(&reference,32);require(!std::memcmp(&reference,&event,32),"World callback arguments/order differ");}
   observations+=count;
  }
  for(unsigned i=0;i<timing;++i){auto ms=r.word();const auto dt=r.word(),iterations=r.word();float value;unsigned actual;require(dh2_physical_world_step_arguments(&value,&actual,ms)==0,"World timing rejected");unsigned bits;std::memcpy(&bits,&value,4);require(bits==dt&&actual==iterations,"World Step arguments differ");}
  require(r.at==r.bytes.size(),"Trailing world reference");
  // The recovered definitions now create genuine bodies/shapes, derive mass,
  // pin, unpin and advance in the actual source-built native world.
  NativeWorld world;const float bounds[4]{-100,-100,100,100};world.load(bounds);Context context{0,1,{}};auto owner=object(&context);
  CharacterBodyInput input{};input.owner=&owner;input.new_physical=&owner;input.is_player=1;input.character_type=1;input.absolute_bounds[2]=72;input.absolute_bounds[3]=60;
  CharacterBodyConfig config{};require(dh2_character_body_config(&config,&input)==0&&config.enabled,"Character configuration failed");
  auto* body=world.create_character(config,&owner);require(body&&body->GetShapeList()&&body->IsStatic()&&body->IsSleeping()&&body->IsBullet(),"Real character creation/pinning failed");NativeBody native{body,config.radius,config.pinned};
  const float speed[2]{4,0};require(dh2_native_body_set_linear(&native,speed)==0,"Native velocity failed");world.update(16);require(body->GetPosition().x==0,"Pinned body moved");
  require(dh2_native_body_unpin(&native)==0,"Character unpin failed");world.update(16);require(body->GetPosition().x==0.064f&&body->GetLinearVelocity().x==4,"Source world update did not advance genuine body");
  world.destroy(body);require(!body&&world.backend()->GetBodyCount()==1,"World destroy did not null body");world.destroy(body);require(world.create(nullptr)==nullptr,"Null definition created body");world.clear();require(!world.backend(),"World clear retained backend");world.load(bounds);require(world.backend()->GetBodyCount()==1,"World reload ground body mismatch");
  std::cout<<"{\"comparisons\":"<<cases+timing<<",\"ordered_callback_observations\":"<<observations<<",\"genuine_character_world_fixture\":true,\"mismatches\":0}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 3;}
}
