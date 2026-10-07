// Positive canonical pool/drop/animation test with actual cached item metadata,
// localized constructor text, authored BDAE and font/color tables. External
// device/condition/network/current-Level facts remain explicit fixture input.
#define main cached_gear_helpers_v23
#include "../../game-data/tests/player_gear_cache_v5.cpp"
#undef main
#include "../world_loot_canonical_bindings_v44.hpp"
#include "../canonical_item_factory_v2.hpp"
#include "../canonical_point3d_globals_v1.hpp"
#include "../world_item_live_owner_v5.hpp"
#include "../../engine-ui/character_menu_font_palette_v1.hpp"
using namespace dh2::character;using namespace dh2::world;
struct PositiveV23 {
 dh2::player::PlayerManagerOwnerV1 players{{nullptr,player_source}};
 std::shared_ptr<dh2::loader::CanonicalLevelContextV1> current_gs;
 std::unique_ptr<WorldLootCanonicalBindingsV44> bindings;
 unsigned full_notifications{};bool notifications_fail{};
 std::int32_t movement_state_id{-1};bool state_failure{};bool state_null{};
 bool foreign_peer{};CanonicalObjectBorrowV1 foreign_peer_copy;
 static bool peer_fields(void* p,std::uintptr_t id,LootPhysicalPeerBorrowV44& out,std::string& e){auto& f=*static_cast<PositiveV23*>(p);auto item=f.items?f.items->factory().find(id):nullptr;if(!item){e="Declared unknown peer source";return false;}auto* actual=f.manager.object(item->base().shared_handle().key);if(f.foreign_peer){f.foreign_peer_copy=*actual;actual=&f.foreign_peer_copy;}out.object=actual;out.visible80=item->base().byte(0x80);out.receiver_lease=item;return true;}
 static bool peer(void* p,void* address,LootPhysicalPeerBorrowV44& out,std::string& e){auto& f=*static_cast<PositiveV23*>(p);for(auto id:f.manager.pending()){auto* graph=f.items?f.items->graph(id):nullptr;if(graph&&address==&graph->physical())return peer_fields(p,id,out,e);}e="Declared unknown physical callback address";return false;}
 static bool state(void* p,std::uintptr_t id,const std::int32_t*& out,std::shared_ptr<void>& pin,std::string& e){auto& f=*static_cast<PositiveV23*>(p);ck(id==reinterpret_cast<std::uintptr_t>(&f));if(f.state_failure){e="Declared SM state failure";return false;}out=f.state_null?nullptr:&f.movement_state_id;pin=f.lease;return true;}
 static bool player_source(void*,const dh2::player::PlayerManagerRequestV1& q,dh2::player::PlayerManagerResponseV1& out,std::string& e){using O=dh2::player::PlayerManagerOperationV1;if(q.operation==O::construct_player_info){return q.player!=nullptr;}if(q.operation==O::online_enabled){out.value=0;return true;}e="Unsupported declared PM fixture branch";return false;}
 static WorldItemLiveOwnerV5* live(void* p){return static_cast<PositiveV23*>(p)->items;}
 static bool notification(void* p,LootTemporaryInventoryV8& inv,ItemInstanceV1& item,std::string& e){auto& f=*static_cast<PositiveV23*>(p);ck(f.items&&inv.items().size()==100&&inv.peek(99)==&item);++f.full_notifications;if(f.notifications_fail){e="Declared notification failure after 100th insert";return false;}return true;}
 CanonicalObjectManagerV1 manager;
 CanonicalPropertyMapV1 properties{{nullptr,&canonical_vec3_origin_v1(),nullptr}};
 CanonicalItemFactoryV2* factory{};
 dh2::physical::NativeWorld physics;dh2::navigation::CollisionWorld geometry{};
 dh2::navigation::ObstacleRegistry obstacles{};
 std::shared_ptr<void> lease=std::make_shared<int>(1);
 std::shared_ptr<GameObjectSceneRootRegistryV1> roots=std::make_shared<GameObjectSceneRootRegistryV1>();
 Raw bdae;CharacterGameDesign* design{};dh2::ui::CharacterMenuFontPaletteV1 palette;
 TextEnvironment* environment{};WorldItemLiveOwnerV5* items{};
 unsigned sound_early{},pf_init{},color_reads{};
 dh2::sound::VoxPlay3DOwnerV2 vox{{this,vox_service}};
 PositiveV23():manager({this,nullptr,nullptr,nullptr,destroy,network,nullptr}){}
 static bool destroy(void* p,CanonicalObjectBorrowV1& b,std::string&){static_cast<PositiveV23*>(p)->factory->erased(b.identity);return true;}
 static bool network(void*,CanonicalObjectBorrowV1&,std::string&){return true;} // actual network/Add transport fixture
 static bool resolve(void* p,dh2::target_providers::Handle16& h,bool,const CanonicalObjectBorrowV1*& out,std::string&){out=static_cast<PositiveV23*>(p)->manager.object(h.key);return true;}
 static bool condition(void*,const CanonicalObjectBorrowV1&,bool v,std::string&){ck(v);return true;} // constructor-empty source Condition fixture
 static bool unknown(void*,const char*,std::string& e){e="Unknown canonical type";return false;}
 static int vox_service(void* p,const dh2::sound::VoxPlay3DRequestV2& q,dh2::sound::VoxPlay3DResponseV2& out){auto& f=*static_cast<PositiveV23*>(p);
  if(q.operation==dh2::sound::VoxPlay3DOperationV2::disabled){out.value=0;return 0;}
  if(q.operation==dh2::sound::VoxPlay3DOperationV2::current_level){++f.sound_early;out.identity=reinterpret_cast<std::uintptr_t>(&f);out.value=0;return 0;} // explicit actual C1 phase0 input, no audio-play receipt
  return -1;
 }
 static bool services(void* p,RetainedWorldItemObjectV1& item,WorldItemGraphServicesV3& s,std::string&){auto& f=*static_cast<PositiveV23*>(p);
  s.visual.owner=f.lease;s.visual.read_asset=[&f](const std::string& path,Raw& b,bool& found,std::string&){ck(path=="data/3D/GameObjects/itemdrops.bdae");b=f.bdae;found=true;return true;};
  s.initialization.owner=f.lease;s.initialization.condition_init=[&item](auto offset,auto&){ck(item.base().string(offset+4)->empty()&&!*item.base().pointer(offset+0x1c));return true;};
  s.initialization.check_spawn_probability=[](auto& out,auto&){out=0;return true;};s.initialization.device_high_performance=[](auto& out,auto&){out=true;return true;};
  s.initialization.init_pf_object=[&f,&item](bool stat,const float* pos,float radius,std::uintptr_t user,std::string& e){ck(user==item.base().identity());dh2::navigation::ObjectInitRequest q{&f.geometry,&item.runtime().object,user,{pos[0],pos[1],pos[2]},radius,std::uint32_t(stat),0};if(dh2_nav_init_object(&q)){e="Actual PF InitObject failed";return false;}++f.pf_init;return true;};
  s.initialization.light_set_id=[](const std::string&,std::int32_t& id,std::string&){id=-1;return true;}; // declared empty source LightSet table fixture: unmatched authored name returns -1
  s.position.owner=f.lease;s.physical.owner=f.lease;s.physical.debug=[&f](const char* key,bool& out,std::string&){dh2::character::DebugFileServices24 ds{f.environment,debug_open,debug_close};std::uint32_t value{};if(dh2_character_debug_load(f.environment->debug,&ds)!=1||dh2_character_debug_get(&value,f.environment->debug,key,&ds)!=1)return false;out=value!=0;return true;};
  s.color.design=f.design->borrow();s.color.context=&f;s.color.font_text_color=[](void* raw,std::int32_t row,std::uint32_t& color,std::string& e){auto& f=*static_cast<PositiveV23*>(raw);++f.color_reads;return f.palette.text_color(row,color,e);};
  s.vox=&f.vox;s.vox_identity=reinterpret_cast<std::uintptr_t>(&f);return true;
 }
 static bool debug(void* raw,const LootEntryRequestV8& q,std::int32_t& out,std::string&){auto& f=*static_cast<PositiveV23*>(raw);dh2::character::DebugFileServices24 ds{f.environment,debug_open,debug_close};if(q.operation==LootEntryOperationV8::debug_load)return dh2_character_debug_load(f.environment->debug,&ds)==1;if(q.operation==LootEntryOperationV8::debug_query){std::uint32_t value{};if(dh2_character_debug_get(&value,f.environment->debug,q.key,&ds)!=1)return false;std::memcpy(&out,&value,4);return true;}return false;}
};
struct DesignFixtureV23 {std::array<std::array<Raw,3>,5> rows;std::vector<Raw> constants;std::vector<Bytes> views;GameDesignInputs256 input{};
 void load(const char* path){auto b=file(path);Reader r{b};ck(r.u()==0x314f4447);auto blob=[&](){Raw v(r.u());r.copy(v.data(),v.size());return v;};for(auto& table:rows)for(auto& x:table)x=blob();auto n=r.u();for(unsigned i=0;i<n;++i){blob();constants.push_back(blob());}n=r.u();for(unsigned i=0;i<n;++i)blob();ck(r.at==b.size());GameDesignTableInput48* dest[]{&input.characters,&input.classes,&input.ai,&input.factions,&input.levels};for(unsigned i=0;i<5;++i)*dest[i]={{rows[i][0].data(),rows[i][0].size()},{rows[i][1].data(),rows[i][1].size()},{rows[i][2].data(),rows[i][2].size()}};for(auto& x:constants)views.push_back({x.data(),x.size()});input.constants=views.data();input.constant_count=views.size();}
};
#define ck(value) ((value)?++checks:throw std::runtime_error(std::string("Positive loot check ")+ #value + "; " + e))
int positive145_v44(int argc,char** argv){try{
 std::string e;ck(argc==7);LootTablesV2 tables;LootAudioVisualV8 av;
 auto b=file(std::string(argv[1])+"/loot_table_pyarray.bin"),n=file(std::string(argv[1])+"/loot_table_pyarraynames.bin"),s=file(std::string(argv[1])+"/loot_table_pystructnames.bin");ck(tables.load({b.data(),b.size()},{n.data(),n.size()},{s.data(),s.size()},e));
 b=file(std::string(argv[2])+"/loot_audiovisual_pyarray.bin");n=file(std::string(argv[2])+"/loot_audiovisual_pyarraynames.bin");s=file(std::string(argv[2])+"/loot_audiovisual_pystructnames.bin");ck(av.load({b.data(),b.size()},{n.data(),n.size()},{s.data(),s.size()},e));
 DesignFixtureV23 input;input.load(argv[4]);CharacterGameDesign design;ck(design.initialize(input.input,e));
 TextEnvironment env;env.assets=argv[5];env.private_files=argv[6];auto c=file(env.assets+"/original-cache/data/pydata/common_text_pycst.bin");dh2_script_constants_reload receipt{};ck(!dh2_script_constants_load(env.constants,c.data(),c.size(),&receipt));
 dh2::ui::HudTextV1 texts;b=file(env.assets+"/original-cache/data/pydata/common_text_pyarray.bin");n=file(env.assets+"/original-cache/data/pydata/common_text_pyarraynames.bin");s=file(env.assets+"/original-cache/data/pydata/common_text_pystructnames.bin");ck(texts.load({b.data(),b.size()},{n.data(),n.size()},{s.data(),s.size()},e)&&texts.switch_pack(0,false,e));
 auto db=design.borrow();std::int32_t automatic{},expected_automatic{};ck(world_loot_design_constant_v44(db.design(),"PickUpType","Automatic",automatic,e));ck(db.design()->lookup(db.design()->context,0,"PickUpType","Automatic",&expected_automatic)==0&&automatic==expected_automatic);automatic=123;ck(world_loot_design_constant_v44(db.design(),"V44_missing_group","V44_missing_key",automatic,e)&&automatic==0);automatic=123;ck(!world_loot_design_constant_v44(nullptr,"PickUpType","Automatic",automatic,e)&&automatic==123);dh2::ui::HudTextEnvironmentV1 te{{&env,text_open,text_close,text_debug,text_constant,nullptr,nullptr}};dh2::ui::ItemTextOwnerV5 text(tables.borrow().items(),*db.characters(),texts,te);
 PositiveV23 f;ck(f.players.initialize(e));WorldLootCanonicalServicesV44 canonical_source;canonical_source.application_lease=f.lease;canonical_source.context=&f;canonical_source.items=PositiveV23::live;canonical_source.character_state=PositiveV23::state;canonical_source.physical_peer=PositiveV23::peer;canonical_source.character_fields=PositiveV23::peer_fields;f.bindings=std::make_unique<WorldLootCanonicalBindingsV44>(f.manager,f.players,dh2::loader::CanonicalGSLevelGlobalSlotV1{f.lease,&f.current_gs},canonical_source);f.design=&design;f.environment=&env;f.bdae=file(std::string(argv[3])+"/itemdrops.bdae");b=file(env.assets+"/original-cache/data/pydata/fonts_pyarray.bin");s=file(env.assets+"/original-cache/data/pydata/fonts_pystructnames.bin");ck(f.palette.load({b.data(),b.size()},{s.data(),s.size()},e));float bounds[]{-10000,-10000,10000,10000};f.physics.load(bounds);
 WorldItemLiveServicesV5 services;services.world=f.lease;services.roots=f.roots;services.context=&f;services.graph_services=PositiveV23::services;services.factory.context=&f;services.factory.resolve=PositiveV23::resolve;services.factory.test_enable_condition=PositiveV23::condition;services.factory.unknown_type_debug=PositiveV23::unknown;services.factory.item.inventory_debug={&f,PositiveV23::debug};services.factory.item.context=&f;services.factory.item.full_notifications=PositiveV23::notification;services.factory=f.bindings->factory_services(std::move(services.factory));
 WorldItemLiveOwnerV5 owner(f.manager,f.properties,f.physics,&f.geometry,&f.obstacles,tables.borrow(),av.borrow(),services);f.items=&owner;f.factory=&owner.factory();ck(owner.precache(e));ck(owner.retained_count()==145&&f.roots->roots().size()==145);
 for(auto object:f.manager.pending()){auto item=owner.factory().find(object);ck(item&&item->runtime().object.user==0);ck(*item->base().byte(0xac)==1);ck(!item->base().lifecycle().enabled8a&&!item->base().lifecycle().updating85);}ck(f.pf_init==0);
 LootCurrentLevelV23 current;ck(f.bindings->current_level(current,e)&&!current.identity&&!current.receiver_lease);
 // Fixture input selects an ACTUAL cache potion item, not a monster award or
 // fabricated template. Its constructor/name/stats/requirements all execute.
 std::int32_t id=-1;for(std::size_t i=0;i<tables.borrow().items().rows.size();++i){auto& row=tables.borrow().items().rows[i];if(item_type(row)==14&&row.record.words[21]>=0&&std::size_t(row.record.words[21])<av.borrow().rows().size()){id=static_cast<std::int32_t>(i);break;}}ck(id>=0);
 LootTemporaryInventoryV8 temp(tables.borrow());std::unique_ptr<ItemInstanceV1> instance;ck(temp.create(id,instance,text.services(),e));auto* same=instance.get();ck(!same->name.empty());ck(temp.store(instance,{&f,PositiveV23::debug},nullptr,nullptr,e));
 float source[]{100,200,0},destination[]{180,250,0};LootItemObjectBorrowV8 spawned;ck(owner.pool().manager().spawn(temp,0,reinterpret_cast<std::uintptr_t>(&f),source,destination,0,spawned,e));ck(temp.items().empty());auto actual=owner.factory().find(spawned.identity);ck(actual&&actual->inventory().peek()==same&&actual->fields().audio_drop3b4==av.borrow().rows()[actual->fields().category3ac].audio_drop);ck(f.sound_early==1&&f.color_reads==1);auto* graph=owner.graph(spawned.identity);ck(graph&&graph->physical().native().body&&graph->physical().secondary_shape());
 ck(actual->runtime().object.user==0&&f.pf_init==0);
 v47_item_target_check(*graph,owner,f.manager,text.services());
 // TestEnableCondition has already stored enabled8a before virtual+44/+48.
 // A SetEnable(route) workaround skips this event and leaves stale flags.
 auto canonical=f.manager.object(actual->base().shared_handle().key);ck(canonical);
 WorldItemPhysicalServicesV2 projected;ck(f.bindings->physical_services(*actual,projected,e));
 std::uintptr_t projected_id{};std::uint32_t projected_type{};std::uint8_t peer_visible{};
 ck(projected.resolve(spawned.identity,projected_id,projected_type,e)&&projected_id==spawned.identity&&projected_type==3);
 ck(!projected.resolve(123,projected_id,projected_type,e));
 ck(projected.peer_owner(&graph->physical(),projected_id,e)&&projected_id==spawned.identity);
 ck(projected.peer_visible(spawned.identity,peer_visible,e)&&peer_visible==1);
 projected_id=123;ck(projected.as_character(spawned.identity,projected_id,e)&&!projected_id);
 ck(!projected.character_ooi(spawned.identity,projected_id,e));
 ck(!projected.peer_owner(reinterpret_cast<void*>(123),projected_id,e));
 f.foreign_peer=true;ck(!projected.peer_owner(&graph->physical(),projected_id,e));f.foreign_peer=false;
 *actual->base().byte(0xf1)=1;ck(f.bindings->test_enable_condition(*canonical,true,e));
 ck(!actual->base().lifecycle().enabled8a&&!actual->base().lifecycle().updating85);
 ck(!*actual->base().byte(0x80)&&(actual->runtime().object.motion.object_flags&8u)==0);
 ck(actual->base().lifecycle().disabled373&&graph->source_filter_disabled26_v4()==1);
 *actual->base().byte(0xf1)=0;ck(f.bindings->test_enable_condition(*canonical,true,e));
 ck(actual->base().lifecycle().enabled8a&&actual->base().lifecycle().updating85&&*actual->base().byte(0x80));
 ck(!actual->base().lifecycle().disabled373&&graph->source_filter_disabled26_v4()==0);
 // A graph failure preserves enabled8a written before the event, along with
 // updating85/visible80 prefixes. It cannot be treated as a false condition.
 auto* physical=actual->base().pointer(0x2dc);const auto same_physical=*physical;
 *physical=0x123;*actual->base().byte(0xf1)=1;
 ck(!f.bindings->test_enable_condition(*canonical,true,e));
 ck(!actual->base().lifecycle().enabled8a&&!actual->base().lifecycle().updating85&&!*actual->base().byte(0x80));
 *physical=same_physical;*actual->base().byte(0xf1)=0;ck(f.bindings->test_enable_condition(*canonical,true,e));
 WorldLootCanonicalBindingsV44 missing(f.manager,f.players,{},canonical_source);
 current.identity=123;ck(!missing.current_level(current,e)&&current.identity==123);
 WorldItemRequestV1 moving;moving.object=spawned.identity;moving.character=reinterpret_cast<std::uintptr_t>(&f);moving.operation=WorldItemOperationV1::is_moving;
 for(auto state_id:{-1,0,3,4,5,18,19,20}){f.movement_state_id=state_id;bool handled{};std::int32_t value=-99;ck(f.bindings->outer_item(moving,value,handled,e)&&handled&&value==(state_id==4||state_id==19));}
 f.state_null=true;bool handled{};std::int32_t moving_value=-99;ck(f.bindings->outer_item(moving,moving_value,handled,e)&&handled&&!moving_value);
 f.state_failure=true;moving_value=88;ck(!f.bindings->outer_item(moving,moving_value,handled,e)&&moving_value==88);f.state_failure=false;f.state_null=false;
 ck(owner.sample_visuals(0,e));auto visual=graph->visual().visual();ck(visual&&visual->ready()&&(visual->scene_flags()&1u));auto before=visual->scene().graph;ck(owner.sample_visuals(250,e));ck(visual->root_animator_present());ck(owner.pool().manager().despawn(spawned.identity,e));ck(actual->inventory().items().empty()&&!actual->base().lifecycle().enabled8a);ck(owner.sample_visuals(300,e));ck(!(visual->scene_flags()&1u));

 // Actual >99/InfiniteInventory=false notification through InitAgain transfer.
 // The factory context now belongs to V44, while Item.context is this fixture.
 for(unsigned n=0;n<99;++n){std::unique_ptr<ItemInstanceV1> extra;ck(temp.create(id,extra,text.services(),e));ck(actual->inventory().store(extra,{&f,PositiveV23::debug},nullptr,nullptr,e));}
 ck(actual->inventory().items().size()==99);std::unique_ptr<ItemInstanceV1> overflow;
 ck(temp.create(id,overflow,text.services(),e));auto* overflow_identity=overflow.get();ck(temp.store(overflow,{&f,PositiveV23::debug},nullptr,nullptr,e));
 f.notifications_fail=true;ck(!actual->init_again(temp,0,reinterpret_cast<std::uintptr_t>(&f),e));
 ck(f.full_notifications==1&&actual->inventory().items().size()==100&&actual->inventory().peek(99)==overflow_identity);
 ck(actual->failed());ck(!actual->init_again(temp,0,0,e));
 // Failed source transfer retains its reached ownership prefix. Never retry.
 // Source cleanup accesses the actual inventory/graph rather than Item.call.
 ck(actual->inventory().remove_all_owned_v2(nullptr,nullptr,e));
 ck(graph->physical().detach(e));
 ck(owner.pool().manager().flush(e));auto ids=f.manager.pending();for(auto object:ids)ck(owner.erased(object,e));ck(f.roots->roots().empty());f.physics.clear();ck(env.opened==env.closed);
 std::cout<<"PASS late-stage29 canonical145, NO InitFinal/PF init, canonical Spawn/TestEnableCondition/virtual44+48, localized text/drop/body/visibility/reuse and full-notification failure prefix checks="<<checks<<"; canonical GS NULL/missing branches tested; sound phase0/device/network/light/selected-item inputs declared fixtures; Gear pickup and actual monster award not claimed\n";return 0;
 }catch(const std::exception& ex){std::cerr<<ex.what()<<'\n';return 1;}}
