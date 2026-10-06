#include "canonical_receiver_transport_v1.hpp"
#include "canonical_cached_file_v1.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace dh2;using namespace dh2::loader;using namespace dh2::world;
static void check(bool ok,const std::string& error){if(!ok)throw std::runtime_error(error);}
// Explicit class/field/continuation fixtures: these are not game actors.
struct Receiver {
 const char* class_name{};std::string templ;std::uint32_t type{2};std::uint8_t across{};std::int32_t room{-1};
 target_providers::Handle16 handle{0,UINT32_MAX,0};std::map<std::uint32_t,CanonicalPropertyValueV1> fields;
 unsigned post_calls{},position_calls{},set_calls{};bool last_update{};
 static bool read(void*,std::uint32_t o,std::uint8_t& out,std::string& e){if(o!=0x84){e="declared fixture bool unavailable";return false;}out=0;return true;}
 template<class T>static bool write(void* p,std::uint32_t o,T value,std::string&){auto& r=*static_cast<Receiver*>(p);r.fields[o]=value;return true;}
 static bool name(void* p,const char* n,std::string& e){return write<const std::string&>(p,0x30,std::string(n),e);}
 static bool archetype(void* p,const char* n,std::string& e){return write<const std::string&>(p,0x48,std::string(n),e);}
 static bool character(void* p,std::uintptr_t& out,std::string&){out=reinterpret_cast<std::uintptr_t>(p);return true;}
 CanonicalObjectBorrowV1 canonical(std::shared_ptr<void> lease){return {reinterpret_cast<std::uintptr_t>(this),std::move(lease),&handle,&type,&across,&room,this,name,archetype,character,&class_name,nullptr};}
 CanonicalPropertyActorV1 properties(){return {class_name,&templ,{this,read,write<std::uint8_t>,write<std::int32_t>,write<float>,write<const std::string&>,write<const std::array<float,3>&>,write<const std::array<std::int32_t,2>&>,write<const CanonicalPoint3ListV1&>}};}
};
struct Context {
 std::vector<std::pair<std::string,std::uint32_t>> catalog;
 std::weak_ptr<Receiver> last;std::shared_ptr<Receiver> force_same;
 bool fail{},unavailable{},missing_property{},missing_handle{},throw_after{},continuations{true};unsigned unknown_calls{};
 std::function<void(std::uintptr_t)> self_erase;
};
static bool construct(void* p,const CanonicalFactoryEntryV1& entry,const CanonicalSourceObjectRequestV1&,CanonicalClassReceiverV1& out,std::string& error){
 auto& c=*static_cast<Context*>(p);c.catalog.emplace_back(entry.name,entry.original_address);
 if(c.unavailable){error="declared unavailable actual constructor: ";error+=entry.name;return false;}
 auto r=c.force_same?c.force_same:std::make_shared<Receiver>();c.last=r;out=canonical_class_receiver_v1(r);
 if(c.missing_property)out.properties={};if(c.missing_handle)out.object.shared_handle=nullptr;
 if(c.continuations){
  out.init_post=[r](std::string&){++r->post_calls;return true;};
  out.is_game_object=[](bool& value,std::string&){value=true;return true;};
  out.position=[r](std::array<float,3>& value,std::string& e){++r->position_calls;auto i=r->fields.find(0x160);if(i==r->fields.end()){e="declared fixture position not produced";return false;}value=std::get<std::array<float,3>>(i->second);return true;};
  out.set_position=[r](const std::array<float,3>& value,bool update,std::string&){++r->set_calls;r->last_update=update;r->fields[0x160]=value;return true;};
 }
 if(c.self_erase)out.init_post=[r,erase=c.self_erase](std::string&){erase(reinterpret_cast<std::uintptr_t>(r.get()));++r->post_calls;return true;};
 if(c.throw_after)throw std::runtime_error("declared constructor exception after receiver assignment");
 if(c.fail){error="declared constructor continuation failure";return false;}return true;
}
static CanonicalReceiverTransportServicesV1 services(std::shared_ptr<Context> c){return {c,c.get(),construct,
 [](void* p,const char*,std::string& e){++static_cast<Context*>(p)->unknown_calls;e="declared unknown-type diagnostic failure";return false;}};}
static assets::ZipAssetPackV1 pack(const char* path){
 auto f=std::make_shared<std::ifstream>(path,std::ios::binary|std::ios::ate);check(bool(*f),"cache unavailable");
 assets::ZipBackingV1 b;b.owner=f;b.bytes=std::uint64_t(f->tellg());
 b.read=[f](std::uint64_t at,void* out,std::size_t n,std::string& e){f->clear();f->seekg(std::streamoff(at));f->read(static_cast<char*>(out),std::streamsize(n));if(!*f){e="cache read failure";return false;}return true;};
 assets::ZipAssetPackV1 z;std::string e;check(z.mount(std::move(b),"com.gameloft.android.GAND.GloftD2SS/files/",e),e);return z;
}
static LevelFileWalkStepV1 finish(CanonicalCachedFileV1& file,const std::string& uri,const char* root){for(unsigned n=0;n<1000;++n){auto s=file.step(uri,root);if(s!=LevelFileWalkStepV1::pending)return s;}throw std::runtime_error("source failed to terminate");}
int main(int argc,char** argv){if(argc!=3)return 2;try{
 unsigned checks=0;std::string error;std::array<float,3> zero{};
 CanonicalPropertyMapV1 properties({nullptr,&zero,nullptr,nullptr});auto c=std::make_shared<Context>();
 CanonicalReceiverTransportV1 transport(properties,services(c));auto api=transport.services();
 auto source_pin=std::make_shared<int>(7);CanonicalSourceObjectRequestV1 request;request.source_lease=source_pin;
 {
  auto missing=services(c);missing.owner.reset();CanonicalReceiverTransportV1 no_owner(properties,missing);auto s=no_owner.services();CanonicalObjectBorrowV1 out;out.identity=123;
  check(!s.construct(s.context,canonical_factories_v1().back(),request,out,error)&&out.identity==123&&c->catalog.empty()&&no_owner.retained_count()==0,"missing owner called constructor or changed output");++checks;
 }
 c->fail=true;std::vector<CanonicalObjectBorrowV1> prefixes;
 for(const auto& entry:canonical_factories_v1()){
  CanonicalObjectBorrowV1 out;check(!api.construct(api.context,entry,request,out,error)&&out.identity&&out.lease&&error=="declared constructor continuation failure","catalog constructor prefix lost");
  const CanonicalClassReceiverV1* retained=nullptr;check(transport.receiver(out,retained,error)&&retained->source_lease==source_pin,"constructor source lease lost");prefixes.push_back(out);
 }
 check(c->catalog.size()==33&&transport.retained_count()==33,"catalog dispatch was limited to actor subset");
 for(std::size_t i=0;i<33;++i)check(c->catalog[i].first==canonical_factories_v1()[i].name&&c->catalog[i].second==canonical_factories_v1()[i].original_address,"constructor changed original catalog entry");++checks;
 {
  const CanonicalClassReceiverV1* out=nullptr;check(transport.receiver(prefixes[0],out,error)&&out,"failed prefix dispatch unavailable");
  auto preserved=out;transport.erased(prefixes[0].identity);check(!transport.receiver(prefixes[0],out,error)&&out==preserved&&transport.retained_count()==32,"erase/failed lookup altered another receiver");++checks;
 }
 {
  c->force_same=std::static_pointer_cast<Receiver>(prefixes[1].lease);CanonicalObjectBorrowV1 out;out.identity=999;
  check(!api.construct(api.context,canonical_factories_v1()[1],request,out,error)&&out.identity==999&&transport.retained_count()==32,"live identity constructor replaced existing receiver");
  c->force_same.reset();++checks;
 }
 {
  c->fail=false;c->missing_property=true;CanonicalObjectBorrowV1 out;
  check(!api.construct(api.context,canonical_factories_v1()[9],request,out,error)&&out.identity&&transport.retained_count()==33,"incomplete constructor lost retained prefix");
  check(!api.init_properties(api.context,out,error),"missing actual property producer fabricated success");c->missing_property=false;++checks;
 }
 {
  c->missing_handle=true;CanonicalObjectBorrowV1 out;check(!api.construct(api.context,canonical_factories_v1()[9],request,out,error)&&out.identity,"missing Handle fabricated constructor success");c->missing_handle=false;++checks;
 }
 {
  c->continuations=false;CanonicalObjectBorrowV1 out;check(api.construct(api.context,canonical_factories_v1()[9],request,out,error),error);*out.class_name20="Character";
  check(api.init_properties(api.context,out,error)&&api.load_defaults(api.context,out,error),error);
  bool game=true;std::array<float,3> value{9,9,9};
  check(!api.init_post(api.context,out,error)&&!api.is_game_object(api.context,out,game,error)&&game,"missing lifecycle fabricated result");
  check(!api.position(api.context,out,value,error)&&value==std::array<float,3>{9,9,9}&&!api.set_position(api.context,out,value,true,error),"missing pose continuation changed receiver");c->continuations=true;++checks;
 }
 {
  CanonicalObjectBorrowV1 out;check(api.construct(api.context,canonical_factories_v1()[9],request,out,error),error);*out.class_name20="Character";
  check(api.init_properties(api.context,out,error)&&api.load_defaults(api.context,out,error)&&api.init_post(api.context,out,error),error);
  std::array<float,3> value{4,5,6};bool game=false;check(api.is_game_object(api.context,out,game,error)&&game&&api.set_position(api.context,out,value,false,error)&&api.position(api.context,out,value,error),error);
  auto actual=std::static_pointer_cast<Receiver>(out.lease);check(value==std::array<float,3>{4,5,6}&&actual->post_calls==1&&actual->set_calls==1&&!actual->last_update,"same receiver continuation dispatch changed");++checks;
 }
 check(!api.unknown_type_debug(api.context,"unregistered",error)&&c->unknown_calls==1&&error=="declared unknown-type diagnostic failure","unknown Debug failure hidden");++checks;
 {
  auto lease=std::make_shared<int>(8);std::weak_ptr<const void> weak=lease;CanonicalSourceObjectRequestV1 q;q.source_lease=lease;CanonicalObjectBorrowV1 out;
  check(api.construct(api.context,canonical_factories_v1()[9],q,out,error),error);lease.reset();q.source_lease.reset();check(!weak.expired(),"receiver failed to retain XML lease");transport.erased(out.identity);check(weak.expired(),"erased receiver leaked XML lease");++checks;
 }
 {
  c->throw_after=true;CanonicalObjectBorrowV1 out;const auto count=transport.retained_count();
  check(!api.construct(api.context,canonical_factories_v1()[9],request,out,error)&&out.identity&&error=="declared constructor exception after receiver assignment"&&transport.retained_count()==count+1,"constructor exception lost actual prefix");
  c->throw_after=false;++checks;
 }
 {
  c->self_erase=[&](std::uintptr_t identity){transport.erased(identity);};CanonicalObjectBorrowV1 out;
  check(api.construct(api.context,canonical_factories_v1()[9],request,out,error),error);const auto count=transport.retained_count();
  check(api.init_post(api.context,out,error)&&std::static_pointer_cast<Receiver>(out.lease)->post_calls==1&&transport.retained_count()==count-1,"callback lifetime did not survive its own receiver removal");
  c->self_erase={};++checks;
 }
 auto original=pack(argv[1]),fixtures=pack(argv[2]);
 {
  auto context=std::make_shared<Context>();context->unavailable=true;CanonicalReceiverTransportV1 receivers(properties,services(context));CanonicalObjectManagerV1 manager({});
  unsigned parses=0;CanonicalCachedFileV1 file(original,manager,receivers.services(),{[&](bool ok,std::string&){++parses;return ok;},[](std::string&){return true;}},{context},ObjectEntryRouteV1::level);
  check(finish(file,"data/scene/001_swamp.mlx","Level")==LevelFileWalkStepV1::failed&&file.attempts().size()==1&&context->catalog.size()==1,"unfiltered original SWAMP did not reach actual constructor provider");
  check(context->catalog[0].first=="LevelConfig"&&context->catalog[0].second==0x340ca8&&file.attempts()[0]->factory_attempt()->prefix()==CanonicalFactoryStageV1::empty&&manager.source_count50()==0,"SWAMP unavailable constructor prefix changed");
  check(finish(file,"data/scene/001_swamp.mlx","Level")==LevelFileWalkStepV1::failed&&parses==1&&context->catalog.size()==1,"SWAMP failed constructor replayed");++checks;
 }
 {
  auto context=std::make_shared<Context>();CanonicalReceiverTransportV1 receivers(properties,services(context));unsigned duplicates=0,networks=0;
  // Count declared manager-service calls while retaining actual manager behavior.
  CanonicalObjectManagerServicesV1 mservices;struct ManagerContext {CanonicalReceiverTransportV1* transport;unsigned* duplicates;unsigned* networks;};ManagerContext mc{&receivers,&duplicates,&networks};mservices.context=&mc;
  mservices.destroy_duplicate=[](void* p,CanonicalObjectBorrowV1& object,std::string&){auto& m=*static_cast<ManagerContext*>(p);++*m.duplicates;m.transport->erased(object.identity);return true;};
  mservices.assign_network_id=[](void* p,CanonicalObjectBorrowV1&,std::string&){++*static_cast<ManagerContext*>(p)->networks;return true;};
  CanonicalObjectManagerV1 manager(mservices);CanonicalModuleContextV1 module{context,7,77,{11,22,33}};
  CanonicalCachedFileV1 file(fixtures,manager,receivers.services(),{[](bool ok,std::string&){return ok;},[](std::string&){return true;}},module,ObjectEntryRouteV1::level);
  check(finish(file,"transport.mgp","Module")==LevelFileWalkStepV1::complete&&file.attempts().size()==2&&manager.source_count50()==1&&receivers.retained_count()==1,"same manager duplicate Add source path changed");
  auto handle=file.attempts()[0]->factory_attempt()->handle();auto* object=manager.object(handle.key);check(object&&object->identity==file.attempts()[1]->factory_attempt()->handle().cached&&duplicates==1&&networks==1,"duplicate Add constructed replacement world identity");
  const CanonicalClassReceiverV1* actual=nullptr;check(receivers.receiver(*object,actual,error),error);std::array<float,3> position;check(actual->position(position,error)&&position==std::array<float,3>{15,27,39},"overrides/module offset did not reach same OLD duplicate receiver");++checks;
  check(context->last.expired()&&manager.characters().size()==1&&actual->source_lease==file.attempts()[0]->source().request().source_lease,"duplicate deletion/lifetime or retained source dispatch changed");++checks;
 }
 std::cout<<"{\"validation\":\"PASS\",\"receiver_transport_checks\":"<<checks<<",\"catalog_entries_forwarded\":33,\"class_construction_fixtures\":true,\"full_loader_verified\":false}\n";
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
