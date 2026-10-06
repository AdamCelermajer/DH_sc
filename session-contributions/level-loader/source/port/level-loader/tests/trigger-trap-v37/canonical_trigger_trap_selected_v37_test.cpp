#include "canonical_trigger_trap_v37.hpp"
#include "../../canonical_auxiliary_families_v16.hpp"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
using namespace dh2;
using namespace dh2::world;
struct Fixture {
 std::shared_ptr<int> world=std::make_shared<int>(1);
 actor::RuntimeState runtime{};
 std::shared_ptr<CanonicalTriggerTrapV37> owner;
 CanonicalClassReceiverV1 receiver;
 std::unique_ptr<loader::CanonicalAuxiliaryFamiliesV16> families;
 std::array<float,3> zero{};
 CanonicalPropertySourceServicesV1 property_source;
 CanonicalPropertyMapV1 properties;
 std::map<std::string,std::string> attributes;
 int network_calls{},position_calls{},template_assertions{};
 Fixture():property_source{},properties([&]{property_source.context=this;property_source.position_rotation_default=&zero;
  property_source.load_template_assertion=[](void* p,std::string&){++static_cast<Fixture*>(p)->template_assertions;return true;};return property_source;}()){}
 static const char* attr(void* p,std::uint32_t,const char* n){auto& a=static_cast<Fixture*>(p)->attributes;auto i=a.find(n);return i==a.end()?nullptr:i->second.c_str();}
 static Fixture& self(void* p){return *static_cast<Fixture*>(p);}
 CanonicalClassServicesV1 services(){CanonicalClassServicesV1 s{};s.context=this;
  s.construct=[](void* p,const CanonicalFactoryEntryV1& f,const CanonicalSourceObjectRequestV1& q,CanonicalObjectBorrowV1& out,std::string&){auto& x=self(p);assert(std::string(f.name)=="TriggerTrap"&&f.original_address==0x340e7c);
   loader::CanonicalAuxiliaryInputsV16 providers;providers.world=x.world;
    providers.trigger_trap=[&x](const auto&,const std::shared_ptr<loader::CanonicalTriggerTrapRecordV37>& record,GameObjectInitializationServicesV1& init,TriggerTrapServicesV37& trap,std::string&){
     init.owner=x.world;trap.owner=x.world;std::weak_ptr<loader::CanonicalTriggerTrapRecordV37> weak=record;
     init.set_position=[&x,weak](const float* v,bool dst,std::string&){auto actual=weak.lock();assert(actual&&dst);++x.position_calls;std::copy_n(v,3,actual->runtime.subobjects.position);return true;};return true;};
    x.families=std::make_unique<loader::CanonicalAuxiliaryFamiliesV16>(std::move(providers));
    std::string error;if(!x.families->construct(f,q,x.receiver,error))return false;
    const auto record=x.families->trigger_traps().back();x.owner=std::shared_ptr<CanonicalTriggerTrapV37>(record,record->owner.get());
    assert(x.owner->base().identity()==x.receiver.object.identity&&x.receiver.object.lease);
    out=x.receiver.object;return true;};
  s.init_properties=[](void* p,const CanonicalObjectBorrowV1& o,std::string& e){auto& x=self(p);assert(o.identity==x.owner->base().identity());auto a=x.receiver.properties();return x.properties.init_properties(a,e);};
  s.set_template=[](void* p,const CanonicalObjectBorrowV1&,const char* n,std::string& e){auto& x=self(p);auto a=x.receiver.properties();return x.properties.set_template(a,n,e);};
  s.load_defaults=[](void* p,const CanonicalObjectBorrowV1&,std::string& e){auto& x=self(p);auto a=x.receiver.properties();return x.properties.load_defaults(a,e);};
  s.load_overrides=[](void* p,const CanonicalObjectBorrowV1&,const CanonicalSourceObjectRequestV1& q,std::string& e){auto& x=self(p);auto a=x.receiver.properties();return x.properties.load_overrides(a,q,e);};
  s.init_post=[](void* p,const CanonicalObjectBorrowV1&,std::string& e){return self(p).receiver.init_post(e);};
  s.is_game_object=[](void* p,const CanonicalObjectBorrowV1&,bool& out,std::string& e){return self(p).receiver.is_game_object(out,e);};
  s.position=[](void* p,const CanonicalObjectBorrowV1&,std::array<float,3>& out,std::string& e){return self(p).receiver.position(out,e);};
  s.set_position=[](void* p,const CanonicalObjectBorrowV1&,const std::array<float,3>& v,bool dst,std::string& e){return self(p).receiver.set_position(v,dst,e);};return s;
 }
};
int main(int argc,char** argv){assert(argc==2);std::string e;
 auto world=std::make_shared<int>(1);actor::RuntimeState runtime{};GameObjectInitializationServicesV1 init{};TriggerTrapServicesV37 trap{};
 auto owner=std::make_shared<CanonicalTriggerTrapV37>(world,runtime,init,trap);
 assert(owner->base().type_f4()==15&&owner->base().lifecycle().static84==1&&owner->base().lifecycle().updating85==1);
 assert((owner->physical()&&!owner->trigger()&&owner->dimensions()==std::array<float,3>{}));
 assert(owner->source_contacts388().empty()&&owner->source_victims3c8().empty()&&owner->source_previous_victims3e0().empty());
 assert(*owner->source_integer(0x3a0)==0&&*owner->source_integer(0x3a4)==0&&*owner->source_integer(0x3c0)==-1&&*owner->source_integer(0x3fc)==-1);
 assert(owner->source_integer(0x3f4)==nullptr&&!owner->source_timer3f4()&&owner->source_owner3f8()==0&&owner->source_string(0x3a8)->empty());
 assert(*owner->source_byte(0x3c4)==0&&*owner->source_byte(0x400)==0&&*owner->source_byte(0x401)==0);
 assert(!owner->init_post(e)&&e.find("39dee0")!=std::string::npos);e.clear();assert(!owner->update(e));e.clear();assert(!owner->init_final(e));e.clear();assert(!owner->destroy(e));
 auto borrow=CanonicalTriggerTrapV37::factory_receiver(owner,world);auto identity=owner->base().identity();owner.reset();assert(borrow.object.identity==identity&&borrow.object.lease&&borrow.source_lease);std::array<float,3> p{};assert(borrow.position(p,e));
 std::ifstream input(argv[1]);assert(input);std::vector<std::map<std::string,std::string>> cases;std::map<std::string,std::string> attrs;std::string line;
 while(std::getline(input,line)){if(!line.empty()&&line.back()=='\r')line.pop_back();if(line.empty()){if(!attrs.empty()){cases.push_back(attrs);attrs.clear();}continue;}auto at=line.find('\t');assert(at!=std::string::npos);attrs[line.substr(0,at)]=line.substr(at+1);}if(!attrs.empty())cases.push_back(attrs);assert(!cases.empty());
 for(auto& attributes:cases){Fixture f;f.attributes=attributes;
  CanonicalObjectManagerServicesV1 ms;ms.context=&f;ms.assign_network_id=[](void* p,CanonicalObjectBorrowV1& o,std::string&){auto& x=Fixture::self(p);assert(o.identity==x.owner->base().identity());++x.network_calls;return true;};
  CanonicalObjectManagerV1 manager(ms);CanonicalSourceObjectRequestV1 q;q.source_lease=f.world;q.source_context=&f;q.attribute=Fixture::attr;q.runtime_module_id=17;q.module_offset={1000.f,-2000.f,33.f};
  CanonicalObjectFactoryAttemptV1 attempt(q);assert(attempt.execute(manager,f.services(),e));assert(attempt.stage()==CanonicalFactoryStageV1::complete);assert(manager.source_count50()==1&&manager.characters().empty()&&manager.modules().empty()&&manager.pending().empty());
  assert(f.owner->base().room64()==17&&f.network_calls==1&&f.position_calls==1);assert(f.owner->source_string(0x3a8)&&*f.owner->source_string(0x3a8)==attributes.at("data"));
  assert((f.owner->dimensions()==std::array<float,3>{200.f,200.f,200.f}));assert(!f.receiver.init_post(e)); // Main whole activation is still required.
  assert(!attempt.execute(manager,f.services(),e));
 }
 int destroys=0;trap.owner=world;trap.whole_destroy=[&](CanonicalTriggerTrapV37& o,std::string&){assert(o.base().type_f4()==15);++destroys;return true;};
 auto dying=std::make_shared<CanonicalTriggerTrapV37>(world,runtime,init,trap);assert(dying->destroy(e)&&destroys==1);assert(!dying->destroy(e)&&destroys==1);
 std::cout<<"PASS original_authored_cases="<<cases.size()<<" same_factory_manager_properties=1 missing_activation_explicit=1 retained_identity=1 destructor_no_replay=1\n";
}
