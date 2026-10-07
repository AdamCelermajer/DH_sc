#include "loot_publishers_v49_cached.inc"
#include "loot_root_publishers_v49.hpp"
#include "../canonical_decor_v15.hpp"
#include "../retained_scene_visual_connection_v3.hpp"
#include "canonical_decor_physical_connection_v49.hpp"
static void publisher_tests_v49(const char* asset){
 std::string e;auto pin=std::make_shared<int>(1);auto fields=std::make_shared<LootPlayerFieldAssociationV47>(101);
 std::uint8_t source_static=0;PlayerSourceVisibilityV49 visibility(fields,pin,"Character",&source_static);
 CanonicalPropertyMapV1 properties({nullptr,&canonical_vec3_origin_v1(),nullptr,nullptr});
 ck(!fields->visible80()&&visibility.source_enabled8a()==1&&visibility.source_template8().empty());
 ck(visibility.publish_missing_default(properties,e)&&fields->visible80()&&*fields->visible80()==1);
 fields->source_visible_store(0);ck(visibility.publish_missing_default(properties,e)&&!*fields->visible80());
 std::uintptr_t visual=0;PlayerSourceVisibilityServicesV49 visual_services{pin,&visual,nullptr,nullptr};
 visibility.source_enabled8a()=7;ck(visibility.set_visible38b0f0(true,visual_services,e)&&*fields->visible80()==7);
 ck(visibility.set_visible38b0f0(false,visual_services,e)&&!*fields->visible80());
 visual=505;visual_services.sync_visibility4713d0=[](void*,std::uintptr_t id,std::uint8_t value,std::string&){ck(id==505&&value==7);return false;};
 ck(!visibility.set_visible38b0f0(true,visual_services,e)&&*fields->visible80()==7);
 auto before=*fields->visible80();ck(!visibility.set_visible38b0f0(false,{pin,nullptr,nullptr,nullptr},e)&&*fields->visible80()==before);
 auto save=std::make_shared<PlayerSavegameV1>();save->set_character(101);ck(fields->source_save_store_3b36d8(save,e));
 LootCharacterSaveBorrowV44 saved;ck(fields->borrow_save(pin,saved,e)&&saved.save==save.get());
 // Genuine ctor/class/default/visual/Scene/body graph. Device/Debug online
 // inputs are declared fixtures; no complete map/floor publication is claimed.
 struct Record {dh2::actor::RuntimeState runtime{};std::unique_ptr<CanonicalDecorV15> source;};
 auto record=std::make_shared<Record>();
 record->source=std::make_unique<CanonicalDecorV15>(pin,record->runtime,GameObjectInitializationServicesV1{},DecorServicesV15{});
 auto& base=record->source->base();base.class_name20()="Decor";auto property_actor=record->source->properties();
 ck(properties.init_properties(property_actor,e)&&properties.load_defaults(property_actor,e));
 // Explicit authored fixture override, not a production compatibility policy.
 // Default static1 currently reaches the unimplemented OptimizeStatic visual
 // producer, documented separately; do not turn actual map scenery dynamic.
 ck(properties.set_property(property_actor,"static","0",e));
 CanonicalObjectManagerV1 manager({nullptr,nullptr,nullptr,nullptr,nullptr,
  [](void*,CanonicalObjectBorrowV1&,std::string&){return true;},nullptr});
 dh2::target_providers::Handle16 handle;ck(manager.add(record->source->canonical(record),"OriginalDecor","Decor",-1,false,handle,e));
 dh2::physical::NativeWorld physics;float bounds[]{-2000,-2000,2000,2000};physics.load(bounds);
 auto roots=std::make_shared<GameObjectSceneRootRegistryV1>();Raw cached=file(asset);unsigned pf_calls{},debug_calls{};
 RetainedGameObjectVisualServicesV1 visual_source;visual_source.owner=pin;
 visual_source.read_asset=[&](const std::string& path,Raw& bytes,bool& found,std::string&){ck(path=="data/3D/GameObjects/source-decor-v49.bdae");bytes=cached;found=true;return true;};
 visual_source.parent_is_animated=[](bool& animated,std::string&){animated=false;return true;};
 visual_source.update_pf=[&](std::string& error){++pf_calls;CharacterWorldNpcObjectV1 source(base.lifecycle(),base.runtime().object,nullptr,nullptr,base.identity(),{});if(!source.update_pf()){error=source.error();return false;}return true;};
 auto connection=std::make_shared<RetainedSceneVisualConnectionV3>(base,visual_source,roots);
 auto asset_services=connection->services(connection);GameObjectVisualAssetOwnerV1 assets(base,asset_services);
 if(!assets.set_visual("data/3D/GameObjects/source-decor-v49.bdae","",false,e))throw std::runtime_error("Actual V49 visual: "+e);
 ck(true);
 auto actual_visual=connection->attached();ck(actual_visual&&actual_visual->ready()&&actual_visual->marker().found);
 LootPhysicalAssociationsV49 associations(manager);RetainedGameObjectDecorServicesV1 body_services;
 bool no_physics{};
 body_services.owner=pin;body_services.debug_switch=[&](const char* key,bool& value,std::string&){ck(std::string(key)=="MP_NoCollisions"||std::string(key)=="MP_NoPhysics");++debug_calls;value=no_physics&&std::string(key)=="MP_NoPhysics";return true;};
 body_services.update_pf=visual_source.update_pf;
 body_services.peer_owner=[&](void* address,std::uintptr_t& out,std::string& error){LootPhysicalPeerBorrowV44 borrow;if(!associations.peer(address,borrow,error))return false;out=borrow.object?borrow.object->identity:0;return true;};
 body_services.peer_visible80=[&](std::uintptr_t id,std::uint8_t& out,std::string& error){LootPhysicalPeerBorrowV44 borrow;if(!associations.fields(id,borrow,error))return false;out=*borrow.visible80;return true;};
 auto body=std::make_unique<CanonicalPodDecorBodyV49>(base,*actual_visual,physics,body_services);
 ck(*body->source_owner8()==base.identity()&&*base.pointer(0x2dc)==0);
 ck(associations.constructed_pod(*body,record,e));LootPhysicalPeerBorrowV44 borrow;
 ck(associations.peer(body.get(),borrow,e)&&borrow.object==manager.object(handle.key)&&*borrow.visible80==1);
 ck(body->construct(e)&&body->native().body&&*base.pointer(0x2dc)==0&&debug_calls==1);
 const auto source_pf_calls=pf_calls;ck(body->assign(false,e)&&*base.pointer(0x2dc)==reinterpret_cast<std::uintptr_t>(body.get())&&debug_calls==2&&pf_calls==source_pf_calls+1);
 ck(!body->assign(false,e)&&!body->construct(e));dh2::navigation::PhysicalContact contact;
 ck(associations.physical_contact(body.get(),contact,e)&&contact.present&&contact.owner_present&&contact.owner_enabled&&contact.primary.present&&!contact.secondary.present);
 auto rejected=std::make_unique<CanonicalPodDecorBodyV49>(base,*actual_visual,physics,body_services);
 ck(associations.constructed_pod(*rejected,record,e)&&rejected->construct(e));
 const auto old_physical=*base.pointer(0x2dc);const auto prior_updates=pf_calls;no_physics=true;
 ck(rejected->assign(false,e)&&!rejected->native().body&&*base.pointer(0x2dc)==old_physical&&pf_calls==prior_updates);
 associations.released(rejected.get());rejected.reset();no_physics=false;
 // Real NULL-owner physical capability; no manufactured canonical object.
 std::uintptr_t null_owner=0;int physical_receiver{};auto null_pin=std::make_shared<int>(2);
 ck(associations.constructed_null_owner(&physical_receiver,&null_owner,null_pin,e));
 ck(associations.peer(&physical_receiver,borrow,e)&&!borrow.object&&borrow.receiver_lease);
 null_owner=101;ck(!associations.peer(&physical_receiver,borrow,e));null_owner=0;
 dh2::physical::WorldShape empty{nullptr,{0,2,0xffff,1}},decor{&body->transport(),{0,2,0xffff,1}};
 ck(dh2_physical_world_should_collide(&empty,&decor)==1); // must NOT ask null peer services
 ck(body->release(e)&&!*base.pointer(0x2dc));associations.released(body.get());ck(!associations.peer(body.get(),borrow,e));body.reset();
 CanonicalDecorPhysicalConnectionV49 bound(base,physics,associations,record,
  [actual_visual](std::uintptr_t id){return id==reinterpret_cast<std::uintptr_t>(actual_visual.get())?actual_visual:nullptr;},body_services);
 DecorServicesV15 leaves;bound.bind(leaves);std::uintptr_t emitted{};
 const auto constructor_updates=pf_calls;
 ck(leaves.construct_podecor(base,emitted,e)&&emitted&&!*base.pointer(0x2dc)&&pf_calls==constructor_updates);
 ck(leaves.set_physical(emitted,false,e)&&*base.pointer(0x2dc)==emitted&&pf_calls==constructor_updates+1);
 ck(associations.peer(reinterpret_cast<void*>(emitted),borrow,e)&&borrow.object==manager.object(handle.key));
 ck(bound.retained_bodies()==1&&bound.release(e)&&bound.retained_bodies()==0&&!*base.pointer(0x2dc));
 ck(assets.set_visual(0,e)&&connection->discard_unattached(e));ck(roots->roots().empty());physics.clear();
}
int main(int argc,char** argv){if(argc!=8)return 1;if(loot_v47_main(argc-1,argv))return 1;try{
 publisher_tests_v49(argv[7]);std::cout<<"PASS V49 source missing-only PropertyMap/SetVisible/SAME Save and genuine PODecor owner8 split constructor/assignment/NULL-owner/release checks="<<checks<<"; runtime integration pending\n";return 0;
 }catch(const std::exception& ex){std::cerr<<ex.what()<<'\n';return 1;}
}
