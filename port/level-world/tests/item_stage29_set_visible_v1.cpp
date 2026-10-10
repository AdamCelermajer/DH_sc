// Direct Stage29 dispatcher regression over the real cached AV/BDAE and
// retained canonical Item factory/graph. Current Level/debug/network/device
// and Projectile leaves are explicit fixture boundaries; this does not claim
// an authored SWAMP Item or whole campaign/device acceptance.
#define DH2_WORLD_ITEM_LIVE_FIXTURE_ONLY
#include "world_item_live_owner_v5.cpp"
#include "../../level-loader/projectile_precache_source_v96.hpp"
using namespace dh2::loader;
static void stage_check(bool value,const char* expression,unsigned line,const std::string& detail){
 check(value,"Stage29 line "+std::to_string(line)+": "+expression+"; "+detail);
}
#define check(value,...) stage_check((value),#value,__LINE__,e)

int main(int argc,char** argv){try{
 std::string e;check(argc==4);LootTablesV2 tables;LootAudioVisualV8 av;
 auto b=file(std::string(argv[1])+"/loot_table_pyarray.bin"),n=file(std::string(argv[1])+"/loot_table_pyarraynames.bin"),s=file(std::string(argv[1])+"/loot_table_pystructnames.bin");check(tables.load(span(b),span(n),span(s),e),e);
 b=file(std::string(argv[2])+"/loot_audiovisual_pyarray.bin");n=file(std::string(argv[2])+"/loot_audiovisual_pyarraynames.bin");s=file(std::string(argv[2])+"/loot_audiovisual_pystructnames.bin");check(av.load(span(b),span(n),span(s),e),e);
 check(av.borrow().rows().size()==29);
 std::int32_t ordinary_id=-1,category=-1;
 for(std::size_t i=0;i<tables.borrow().items().rows.size();++i){const auto& row=tables.borrow().items().rows[i];
  if(item_type(row)!=13&&item_type(row)!=14&&row.record.words[21]>=0&&std::size_t(row.record.words[21])<av.borrow().rows().size()){
   ordinary_id=static_cast<std::int32_t>(i);category=row.record.words[21];break;
  }
 }
 check(ordinary_id>=0&&category>=0);
 LiveFixtureV5 f;f.bytes=file(std::string(argv[3])+"/itemdrops.bdae");float bounds[4]{-10000,-10000,10000,10000};f.physics.load(bounds);
 WorldItemLiveServicesV5 services;services.world=f.lease;services.roots=f.roots;services.context=&f;services.graph_services=LiveFixtureV5::services;services.outer_item=LiveFixtureV5::outer;
 services.factory.context=&f.canonical;services.factory.resolve=ItemFactoryFixture::resolve;services.factory.test_enable_condition=ItemFactoryFixture::condition;services.factory.unknown_type_debug=ItemFactoryFixture::debug;
 WorldItemLiveOwnerV5 owner(f.canonical.manager,f.canonical.map,f.physics,&f.geometry,&f.obstacles,tables.borrow(),av.borrow(),services);f.canonical.factory=&owner.factory();
 struct LevelFixture {std::uint32_t progress30{},state130{28},counter134{7},current138{3};};
 auto level=std::make_shared<LevelFixture>();unsigned trace_calls{},item_calls{},projectile_calls{},create_calls{};
 std::vector<std::int32_t> published;
 auto verify_pool=[&]{
  check(owner.pool().manager().ready()&&owner.retained_count()==145&&f.roots->roots().size()==145);
  for(auto identity:f.canonical.manager.pending()){
   auto item=owner.factory().find(identity);check(item&&item->base().type_f4()==3);
   check(f.canonical.manager.object(item->base().shared_handle().key)->identity==reinterpret_cast<std::uintptr_t>(item.get()));
   // Source virtual40 writes visible80, then DeSpawn explicitly writes85.
   // It never reaches SetEnable/Disabled, filter26 or flag/byte373 changes.
   check(item->base().lifecycle().enabled8a==1&&!item->base().lifecycle().updating85);
   check(!*item->base().byte(0x80)&&!item->base().lifecycle().disabled373);
   // Original PFObjectC2 524538 writes8 at+4; hiding preserves that bit.
   check((item->runtime().object.motion.object_flags&8u)==8u&&item->runtime().object.user==0);
   check(!*item->base().pointer(0x2dc)&&owner.graph(identity)->source_filter_disabled26_v4()==0);
  }
 };
 Stage29PrecacheServicesV96 stage;
 stage.debug={f.lease,[&](const char* key,bool& ignored,std::string&){check(std::string(key)==level_loading_trace_key_v46);++trace_calls;ignored=false;return true;}};
 stage.validate_current=[&](std::string&){check(level->state130==29);return true;};
 stage.actual_item_precache=[&](std::string& error){++item_calls;return owner.precache(error);};
 stage.projectile_sources=[&](ProjectilePrecacheSourcesV96& out,std::string&){
  ++projectile_calls;check(item_calls==1);verify_pool();
  // Explicit unrelated semantic-NULL Projectile fixture; no real Projectile
  // pool/Create/DeSpawn implementation is substituted or changed by this fix.
  out.owner=f.lease;out.validate_current=[](std::string&){return true;};
  out.borrow_manager=[&](ProjectileManagerPrecacheBorrowV96& manager,std::string&){
   manager.owner=f.lease;manager.identity=reinterpret_cast<std::uintptr_t>(f.lease.get());
   for(auto& pool:manager.pools){pool.capacity=[](std::size_t& out,std::string&){out=10;return true;};pool.count=[](std::size_t& out,std::string&){out=0;return true;};}
   manager.create=[&](bool first,bool,std::uintptr_t& out,std::string&){check(!first);++create_calls;out=0;return true;};return true;
  };
  out.table_owner=f.lease;out.table_count=[](std::uint32_t& out,std::string&){out=0;return true;};return true;
 };
 LifecycleServicesV36 lifecycle_services;lifecycle_services.stage_body[29]=stage29_precache_source_v96(std::move(stage));
 lifecycle_services.publish_progress=[&](std::int32_t phase,std::int32_t progress,std::string&){check(std::uint32_t(phase)==level->state130&&std::uint32_t(progress)==level->progress30);published.push_back(phase);return true;};
 lifecycle_services.cancel_and_unload=[&](std::string& error){
  if(!owner.pool().manager().flush(error))return LifecycleStepV36::failed;
  const auto ids=f.canonical.manager.pending();for(auto id:ids)if(!owner.erased(id,error))return LifecycleStepV36::failed;
  f.physics.clear();return LifecycleStepV36::complete;
 };
 LifecycleV36 lifecycle({&level->progress30,&level->state130,&level->counter134,&level->current138},level,{f.lease},std::move(lifecycle_services));
 check(lifecycle.tick()==LifecycleStatusV36::loading&&level->state130==29&&level->progress30==76&&level->counter134==3);
 check(!item_calls&&!trace_calls&&!owner.pool().manager().ready());
 check(lifecycle.tick()==LifecycleStatusV36::loading&&level->state130==30&&level->progress30==78);
 check(trace_calls==1&&item_calls==1&&projectile_calls==1&&create_calls==20&&published==std::vector<std::int32_t>({29,30}));verify_pool();
 char name[64];std::snprintf(name,sizeof(name),"ItemObject_%02u_00",unsigned(category));dh2::target_providers::Handle16 handle;
 check(f.canonical.manager.by_name(name,-1,false,nullptr,handle,e),e);auto* canonical=f.canonical.manager.object(handle.key);check(canonical);
 auto ordinary=owner.factory().find(canonical->identity);check(ordinary&&ordinary->fields().category3ac==category);
 check(*ordinary->base().string(0x290)=="data/3D/GameObjects/itemdrops.bdae"&&*ordinary->base().string(0x2a8)==av.borrow().rows()[category].visual);
 const auto flags=ordinary->runtime().object.motion.object_flags;auto* graph=owner.graph(canonical->identity);check(graph);
 // Exercise the SAME pool SetVisible receiver with a nonzero source byte:
 // native38b0f0 copies byte8a verbatim, rather than rewriting it to bool1.
 ordinary->base().lifecycle().enabled8a=7;auto loan=ordinary->pool_borrow();LootItemRequestV8 visible{LootItemOperationV8::set_visible,&loan};visible.flag=true;
 check(ordinary->pool_operation(visible,e),e);check(*ordinary->base().byte(0x80)==7&&ordinary->base().lifecycle().enabled8a==7);
 check(ordinary->runtime().object.motion.object_flags==flags&&!ordinary->base().lifecycle().disabled373&&!ordinary->base().lifecycle().updating85&&graph->source_filter_disabled26_v4()==0);
 ordinary->base().lifecycle().updating85=1;check(owner.pool().manager().despawn(canonical->identity,e),e);
 check(!*ordinary->base().byte(0x80)&&ordinary->base().lifecycle().enabled8a==7&&!ordinary->base().lifecycle().updating85);
 check(ordinary->runtime().object.motion.object_flags==flags&&!ordinary->base().lifecycle().disabled373&&graph->source_filter_disabled26_v4()==0);
 // Generic SetEnable remains the genuine separate condition producer.
 WorldItemRequestV1 condition;condition.object=canonical->identity;condition.operation=WorldItemOperationV1::enable;condition.flag=false;std::int32_t result{};bool handled{};
 check(graph->route(condition,result,handled,e)&&handled,e);check(!ordinary->base().lifecycle().enabled8a&&ordinary->base().lifecycle().disabled373);
 condition.flag=true;check(graph->route(condition,result,handled,e),e);check(ordinary->base().lifecycle().enabled8a&&ordinary->base().lifecycle().updating85&&!ordinary->base().lifecycle().disabled373);
 check(!owner.precache(e));lifecycle.request_cancel();check(lifecycle.tick()==LifecycleStatusV36::cancelled&&lifecycle.diagnostics().owned_pins==0&&owner.retained_count()==0&&f.roots->roots().empty());
 std::cout<<"PASS direct Stage28->29->30 Item precache145, cached ordinary Item="<<ordinary_id<<" category="<<category<<", SAME canonical factory/receiver, virtual40 SetVisible preserves8a/373/filter/flags, explicit85 ordering and separate SetEnable; Level/debug/device/network/Projectile inputs declared fixtures; no SWAMP-authored Item or whole campaign acceptance; checks="<<checks<<'\n';return 0;
 }catch(const std::exception& ex){std::cerr<<"check "<<checks<<" "<<ex.what()<<'\n';return 1;}}
