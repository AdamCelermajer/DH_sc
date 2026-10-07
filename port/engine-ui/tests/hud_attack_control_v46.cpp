#include "../hud_attack_control_v46.hpp"
#include <array>
#include <cassert>
#include <cstring>
#include <fstream>
#include <vector>
using namespace dh2::ui;
struct Fixture {
 HudAttackHeldFieldsV46 fields{};std::uint8_t joystick=1,click=7,blocked=0,enabled=1,cached=1;
 std::uintptr_t ooi=0,level=0,root=100,character=200,controller=12345;
 std::vector<unsigned> trace;std::array<std::uintptr_t,3> command{};unsigned fail{};
 bool hit(unsigned n,std::string& e){trace.push_back(n);if(fail==n){e="fixture.required";return false;}return true;}
 HudAttackServicesV46 services(){return {this,
  [](void* p,std::uint8_t& v,std::string& e){auto& f=*static_cast<Fixture*>(p);v=f.blocked;return f.hit(1,e);},
  [](void* p,std::uint8_t v,std::string& e){auto& f=*static_cast<Fixture*>(p);assert(v==bool(f.blocked));return f.hit(2,e);},
  [](void* p,std::uintptr_t& id,const std::uint8_t*& v,std::string& e){auto& f=*static_cast<Fixture*>(p);id=f.level;v=f.level?&f.enabled:nullptr;return f.hit(3,e);},
  [](void* p,std::uintptr_t& v,std::string& e){auto& f=*static_cast<Fixture*>(p);v=f.root;return f.hit(4,e);},
  [](void* p,std::uint8_t& v,std::string& e){auto& f=*static_cast<Fixture*>(p);v=f.cached;return f.hit(5,e);},
  [](void* p,std::string& e){return static_cast<Fixture*>(p)->hit(6,e);},
  [](void* p,std::int32_t index,bool dummy,HudAttackActorBorrowV46& v,std::string& e){auto& f=*static_cast<Fixture*>(p);assert(index==0&&!dummy);v={f.character,f.controller,&f.ooi,&f.click};return f.hit(7,e);},
  [](void* p,std::uintptr_t ctrl,std::uintptr_t arg,std::string& e){auto& f=*static_cast<Fixture*>(p);f.command={1,ctrl,arg};return f.hit(8,e);},
  [](void* p,std::uintptr_t ctrl,std::uintptr_t arg,std::string& e){auto& f=*static_cast<Fixture*>(p);f.command={2,ctrl,arg};return f.hit(9,e);}
 };}
 HudAttackActorBorrowV46 actor(){return {character,controller,&ooi,&click};}
 bool update(std::string& e){return hud_attack_update_v46(fields,joystick,services(),e);}
};
int main(int argc,char** argv){assert(argc==2);std::ifstream in(argv[1],std::ios::binary);assert(in);std::uint32_t count;in.read(reinterpret_cast<char*>(&count),4);assert(count==320);unsigned checks=0;std::string error;
 for(unsigned n=0;n<count;++n){std::array<std::uint32_t,14> r{};in.read(reinterpret_cast<char*>(r.data()),56);assert(in);
  Fixture f;f.fields.held9=r[0];std::memcpy(&f.fields.pending_x7c,&r[1],4);std::memcpy(&f.fields.pending_y80,&r[2],4);bool consumed=false;
  assert(hud_attack_event_v46(f.fields,r[3],consumed,error));std::uint32_t x,y;std::memcpy(&x,&f.fields.pending_x7c,4);std::memcpy(&y,&f.fields.pending_y80,4);
  assert(f.fields.held9==r[6]&&x==r[7]&&y==r[8]&&consumed==bool(r[9]));
  f.fields.held9=r[0];f.ooi=r[4];f.click=r[5];assert(hud_attack_dispatch_v46(f.fields,f.actor(),f.services(),error));
  assert(f.command[0]==r[10]&&f.command[1]==r[11]&&f.command[2]==r[12]&&f.click==r[13]);++checks;
 }
 assert(in.peek()==std::char_traits<char>::eof());
 {Fixture f;f.fields.held9=1;assert(f.update(error)&&f.command[0]==2&&f.click==0&&f.fields.held9);assert(f.trace==std::vector<unsigned>({1,2,3,4,5,7,9}));++checks;}
 {Fixture f;f.fields.held9=1;f.level=99;f.enabled=0;assert(f.update(error)&&!f.fields.held9&&!f.joystick&&f.command[0]==0);assert(f.trace==std::vector<unsigned>({1,2,3}));++checks;}
 {Fixture f;f.fields.held9=1;f.ooi=999;assert(f.update(error)&&f.command[0]==1&&f.command[2]==0&&f.click==7);++checks;}
 {Fixture f;f.fields.held9=1;f.cached=0;f.fields.pending_x7c=42;assert(f.update(error)&&f.fields.pending_x7c==-1);assert(f.trace==std::vector<unsigned>({1,2,3,4,5,6,7,9}));++checks;}
 {Fixture f;f.fields.held9=1;f.root=0;assert(f.update(error)&&!f.command[0]&&f.fields.held9);++checks;}
 {Fixture f;f.fields.held9=1;f.character=0;assert(f.update(error)&&!f.command[0]&&f.fields.held9);++checks;}
 {Fixture f;f.fields.held9=1;f.controller=0;assert(!f.update(error)&&f.click==0&&f.fields.held9);++checks;}
 for(unsigned failure:{1,2,3,4,5,7,9}){Fixture f;f.fields.held9=1;f.fail=failure;assert(!f.update(error)&&error=="fixture.required");assert(f.trace.back()==failure);++checks;}
 {Fixture f;f.fields.held9=1;bool consumed;assert(hud_attack_event_v46(f.fields,7,consumed,error)&&!f.fields.held9);assert(f.update(error)&&!f.command[0]);++checks;}
 assert(checks==335);
}
