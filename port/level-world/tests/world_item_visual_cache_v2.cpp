#define DH2_ITEM_FACTORY_FIXTURE_ONLY
#include "canonical_item_factory_v2.cpp"
#include "../world_item_visual_v2.hpp"
#include "../base_index_animation_controller_v2.hpp"
#include "../visual_aabb_dispatch_scope_v3.hpp"
int main(int argc,char** argv){try{
 check(argc==4);std::string error;LootTablesV2 tables;LootAudioVisualV8 av;
 auto b=file(std::string(argv[1])+"/loot_table_pyarray.bin"),n=file(std::string(argv[1])+"/loot_table_pyarraynames.bin"),s=file(std::string(argv[1])+"/loot_table_pystructnames.bin");check(tables.load(span(b),span(n),span(s),error),error);
 auto ab=file(std::string(argv[2])+"/loot_audiovisual_pyarray.bin"),an=file(std::string(argv[2])+"/loot_audiovisual_pyarraynames.bin"),as=file(std::string(argv[2])+"/loot_audiovisual_pystructnames.bin");check(av.load(span(ab),span(an),span(as),error),error);
 ItemFactoryFixture fixture;auto lease=std::make_shared<int>(1);CanonicalItemFactoryServicesV2 fs{&fixture,ItemFactoryFixture::resolve,ItemFactoryFixture::condition,ItemFactoryFixture::debug,{&fixture,ItemFactoryFixture::item,{},{},nullptr,nullptr}};
 CanonicalItemFactoryV2 factory(fixture.manager,fixture.map,lease,tables.borrow(),av.borrow(),fs);fixture.factory=&factory;
 unsigned visuals=0;for(std::size_t i=0;i<=av.borrow().rows().size();++i){
  std::shared_ptr<dh2::character::RetainedWorldItemObjectV1> item;check(factory.spawn("Item",("ActualLootVisual"+std::to_string(i)).c_str(),false,true,item,error),error);
  auto fields=item->base().properties().fields;check(fields.write_vector3(fields.context,0x120,{1,1,1},error),error);
  auto bytes=file(std::string(argv[3])+"/itemdrops.bdae");unsigned registered=0,released=0;
  RetainedGameObjectVisualServicesV1 services;services.owner=lease;
  services.read_asset=[&](const std::string& model,auto& out,bool& found,auto&){check(model=="data/3D/GameObjects/itemdrops.bdae");out=bytes;found=true;return true;};
  services.register_root=[&](auto root,auto&){check(root!=0);++registered;return true;};services.force_register=[](auto,auto&){return true;};services.release_root=[&](auto root,auto&){check(root!=0);++released;return true;};services.update_pf=[](auto&){return true;};
  RetainedGameObjectVisualV1 visual(item->base(),services);
  VisualAabbDispatchScopeV3 dispatch({&item->base(),&item->base(),[](void* p,const float*,bool flat,std::string& e){return item_relative_aabb_v3(*static_cast<CanonicalGameObjectBaseOwnerV1*>(p),flat,e);}});
  const auto& rows=av.borrow().rows();const std::string xref=i<rows.size()?rows[i].visual:"dummy_itemdrop_bag";
  check(visual.initialize("data/3D/GameObjects/itemdrops.bdae",xref.c_str(),error),"AV "+std::to_string(i)+" xref "+xref+": "+error);
  check(visual.ready()&&visual.root_identity()&&registered==1);check(visual.apply_mesh_box(error),error);
  check(item->base().relative_aabb144()[0]==(visual.marker().found?-100.f:-225.f));
  if(visual.root_animator_present()){auto borrow=visual.named_animation(lease);bool accepted=false;check(base_index_animation_play_v2(borrow,0,false,accepted,error),error);}
  check(visual.update(100,error),error);check(visual.release(error)&&released==1,error);++visuals;
 }
 std::cout<<"Actual ItemDrops BRES/all AV xrefs/fallback/indexClip/bounds PASS visuals="<<visuals<<" checks="<<checks<<"; Scene/PF/Handle/condition endpoints explicit fixtures\n";return 0;
 }catch(const std::exception& e){std::cerr<<"check "<<checks<<" "<<e.what()<<'\n';return 1;}}
