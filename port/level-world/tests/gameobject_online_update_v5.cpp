#include "gameobject_online_update_v5.hpp"
#include <iostream>
#include <stdexcept>
using namespace dh2;
struct Fixture {bool online{},host{},not_owned{};unsigned queries{},hosts{},virtuals{},fail{};
 static bool query(void* raw,bool& out,std::string& e){auto& f=*static_cast<Fixture*>(raw);++f.queries;out=f.online;if(f.fail==f.queries){e="GetOnline failure";return false;}return true;}
 static bool hosting(void* raw,bool& out,std::string& e){auto& f=*static_cast<Fixture*>(raw);++f.hosts;out=f.host;if(f.fail==3){e="hosting failure";return false;}return true;}
 static bool owned(void* raw,std::uintptr_t,bool& out,std::string& e){auto& f=*static_cast<Fixture*>(raw);++f.virtuals;out=f.not_owned;if(f.fail==4){e="virtual failure";return false;}return true;}
};
int main(){try{unsigned checks{};auto check=[&](bool yes){++checks;if(!yes)throw std::runtime_error("check "+std::to_string(checks));};std::string e;
 for(int mode=0;mode<5;++mode){actor::RuntimeState runtime{};auto lease=std::make_shared<int>(1);world::CanonicalGameObjectBaseOwnerV1 base(reinterpret_cast<std::uintptr_t>(&runtime),3,lease,runtime);Fixture f;f.online=mode>0;f.host=mode==2;f.not_owned=mode==4;if(mode>=2)*base.pointer(0x100)=1; // Explicit non-NULL NetStruct input fixture, never a constructed network owner claim.
  world::GameObjectOnlineUpdateServicesV5 s{&f,Fixture::query,Fixture::hosting,Fixture::owned};check(world::gameobject_require_online_update_v5(base,s,e));check(f.queries==(mode>=2?2u:1u));check(f.hosts==(mode>=2?1u:0u));check(f.virtuals==(mode>=3?1u:0u));check(*base.byte(0x119)==(mode==2||mode==3));
 }
 for(unsigned failure=1;failure<=4;++failure){actor::RuntimeState runtime{};world::CanonicalGameObjectBaseOwnerV1 base(reinterpret_cast<std::uintptr_t>(&runtime),3,std::make_shared<int>(1),runtime);*base.pointer(0x100)=1;Fixture f;f.online=true;f.fail=failure;world::GameObjectOnlineUpdateServicesV5 s{&f,Fixture::query,Fixture::hosting,Fixture::owned};e.clear();check(!world::gameobject_require_online_update_v5(base,s,e)&&!e.empty()&&*base.byte(0x119)==0);}
 std::cout<<"PASS whole RequireOnlineUpdate same-base field, real ctor-null net gate, fresh queries/host/virtual/write order and required failures; positive network receiver inputs explicitfixtures checks="<<checks<<'\n';return 0;
 }catch(const std::exception& ex){std::cerr<<ex.what();return 1;}}
