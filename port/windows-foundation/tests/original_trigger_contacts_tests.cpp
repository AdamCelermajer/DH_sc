#include "../original_trigger_contacts.hpp"
#include <iostream>
#include <stdexcept>
using namespace dh::foundation;using namespace dh2::world;
void check(bool x,const std::string&e){if(!x)throw std::runtime_error(e);}
struct PlayerBody {float position[3]{210,0,0};float bounds[6]{190,-10,-10,220,10,10};std::uintptr_t physical=900;};
int main(){try{
 OriginalTriggerContacts contacts;auto body=std::make_shared<PlayerBody>();auto pin=std::make_shared<int>(1);std::string error;bool dead=false,missing_body=false,online=false,remote=false;std::vector<std::pair<int,int>> starts;unsigned local_queries=0,peer_queries=0;std::vector<std::string> empty_names;
 unsigned physical_queries=0;
 auto borrowed=original_trigger_peer_provider([body](std::uintptr_t id,OriginalTriggerActorBorrow&out,std::string&){out={body,id,body->position,body->bounds,&body->physical};return true;},
  [&](std::uintptr_t id,OriginalTriggerPhysicalBorrow&out,std::string&){++physical_queries;out={pin,id,nullptr};return true;});
 ZonePeerBorrowV83 source_peer;check(borrowed(10,source_peer,error),error);
 check(source_peer.position160==body->position&&source_peer.absolute12c==body->bounds&&source_peer.physical2dc==&body->physical,"Borrower copied source field authority");
 body->bounds[0]=191;check(source_peer.absolute12c[0]==191,"Live source AABB update not visible");body->bounds[0]=190;
 float radius=0;check(!source_peer.physical_radius(radius,error)&&physical_queries==1,"Missing actual native body accepted");
 auto absent=original_trigger_peer_provider({},{});check(!absent(10,source_peer,error),"Missing source actor provider accepted");
 auto peer=[&](std::uintptr_t id,ZonePeerBorrowV83&out,std::string&){++peer_queries;check(id==10,"Wrong source player identity");out.receiver=body;out.identity=id;out.position160=body->position;out.absolute12c=missing_body?nullptr:body->bounds;out.physical2dc=&body->physical;out.physical_radius=[](float&r,std::string&){r=15;return true;};return true;};
 auto make=[&](const std::string&key,int room){
  auto runtime=std::make_shared<dh2::actor::RuntimeState>();auto weak=std::make_shared<std::weak_ptr<CanonicalTriggerZoneV22>>();TriggerZoneServicesV22 s;s.owner=pin;s.initialization.owner=pin;s.initialization.difficulty_names=&empty_names;s.initialization.sound_names=&empty_names;
  s.spawn_roll_probability=[](int&roll,int&prob,std::string&){roll=0;prob=100;return true;};s.initialization.condition_init=[](unsigned,std::string&){return true;};s.initialization.check_spawn_probability=[](int&roll,std::string&){roll=0;return true;};
  s.initialization.set_position=[weak](const float*,bool destination,std::string&){check(destination,"Source initialization destination flag changed");auto p=weak->lock();check(bool(p),"Actual initialization receiver missing");p->base().update_absolute_aabb();return true;};
  s.initialization.device_high_performance=[](bool&out,std::string&){out=true;return true;};s.initialization.load_visual=[](std::string&){return true;};
  s.set_bounding_box=[weak](const std::array<float,6>&box,bool position,std::string&){check(!position,"Canonical source SetBoundingBox flag changed");auto p=weak->lock();check(bool(p),"Source owner missing");std::copy(box.begin(),box.end(),p->base().relative_aabb144());p->base().update_absolute_aabb();return true;};
  s.script_id=[](const char*,bool common,int&id,std::string&){check(!common,"Original noncommon script lookup flag changed");id=17;return true;};s.script_flags=[](int,std::uint8_t&flag,std::string&){flag=0;return true;};
  s.local_player=[&](int index,bool flag,TriggerLocalPlayerV22&out,std::string&){++local_queries;check(index==0&&flag,"Source local-player query changed");out={10,std::uint8_t(dead)};return true;};s.player_count=[](int&out,std::string&){out=1;return true;};s.player_character=[](int index,bool flag,std::uintptr_t&out,std::string&){check(index==0&&flag,"Source player getter changed");out=10;return true;};
  s.online5=[&](bool&out,std::string&){out=online;return true;};s.virtual54=[&](bool&out,std::string&){out=remote;return true;};s.require_online_update=[](std::string&){return true;};s.app_delta_ms=[](int&out,std::string&){out=16;return true;};s.is_character=[](std::uintptr_t,bool&out,std::string&){out=true;return true;};s.is_player=s.is_character;
  s.script_running=[](int,bool&out,std::string&){out=false;return true;};s.start_script=[&](int id,int actual_room,std::string&){starts.emplace_back(id,actual_room);return true;};
  check(contacts.prepare(key,room,peer,s,error),error);auto zone=std::make_shared<CanonicalTriggerZoneV22>(pin,*runtime,s);*weak=zone;
  zone->base().room64()=room;zone->base().lifecycle().enabled8a=1;zone->base().vector3(0x120)[0]=2;zone->base().vector3(0x120)[1]=zone->base().vector3(0x120)[2]=1;
  check(zone->write_string(0x724,"source-script",error),error);check(contacts.attach(key,zone,runtime,error),error);check(contacts.source_dimensions(key,std::nullopt,error),error);check(contacts.initialize(key,error),error);return zone;
 };
 auto first=make("occurrence0::same-source.mgp/trigger",7);auto second=make("occurrence1::same-source.mgp/trigger",8);
 check(first->base().absolute_aabb12c()[0]==-200&&first->base().absolute_aabb12c()[3]==200,"Original default200 times authoredscale2 bbox");
 bool hit=false;check(contacts.touching("occurrence0::same-source.mgp/trigger",10,hit,error)&&hit,"Body overlaps even though center outside: center approximation regression");
 // Source touching uses GameObject AABB independently of physical presence;
 // source Inside reads the actual nullable slot and returns false before radius.
 body->physical=0;check(contacts.touching("occurrence0::same-source.mgp/trigger",10,hit,error)&&hit,"Source NULL physical incorrectly gates AABB touching");
 ZoneCollisionServicesV83 source_collision;source_collision.peer=borrowed;bool inside=true;auto before_radius=physical_queries;
 check(zone_is_inside_v83(first->base(),0,10,source_collision,inside,error)&&!inside&&physical_queries==before_radius,"NULL physical source Inside should return before native radius lookup");body->physical=900;
 dead=true;auto prior=peer_queries;check(contacts.update_all(error)&&starts.empty()&&peer_queries==prior,"Dead-player gate reached contacts");dead=false;
 online=true;remote=true;check(contacts.update_all(error)&&starts.empty(),"Remote online authority gate bypassed");online=remote=false;
 body->position[0]=190;body->bounds[0]=170;body->bounds[3]=200;check(contacts.collision_begin("occurrence0::same-source.mgp/trigger",10,error)&&contacts.collision_begin("occurrence0::same-source.mgp/trigger",10,error),error);check(*first->source_integer(0x3a0)==1,"Duplicate collision count incremented");body->position[0]=210;body->bounds[0]=190;body->bounds[3]=220;
 check(contacts.update_all(error),error);check(starts==std::vector<std::pair<int,int>>({{17,7},{17,8}}),"Occurrence/source construction scope or order lost");check(first->activation_count()==1&&second->activation_count()==1,"Actual canonical activation counts lost");
 check(*first->source_integer(0x3a0)==0,"Original IsInside center predicate must prune contact while IsTouching AABB activates");
 check(contacts.update_all(error)&&starts.size()==2,"Source single-fire count gate restarted");
 // Preserve the recovered owner's CanActivate timer gate and its exact ordering.
 *first->source_integer(0x3a8)=-1;*first->source_integer(0x3b8)=100;prior=peer_queries;
 check(contacts.update("occurrence0::same-source.mgp/trigger",error)&&peer_queries==prior&&first->timer()==100,"Source delay gate/order was changed by adapter");
 missing_body=true;check(!contacts.touching("occurrence0::same-source.mgp/trigger",10,hit,error)&&error.find("actual peer body")!=std::string::npos,"Unbound source AABB accepted");missing_body=false;
 TriggerZoneServicesV22 s;check(!contacts.prepare("unknown-room",std::nullopt,peer,s,error),"Unproven room ordinal accepted");check(!contacts.prepare("occurrence0::same-source.mgp/trigger",7,peer,s,error),"Duplicate instance key accepted");contacts.clear();check(!contacts.owner("occurrence0::same-source.mgp/trigger"),"Clear retained source occurrence");
 std::cout<<"Original trigger contacts tests passed canonicalOwners=2 repeatedSourceIndependent=true actualAABBBoundary=true sourceGates=true\n";
}catch(const std::exception&e){std::cerr<<e.what()<<"\n";return 1;}}

