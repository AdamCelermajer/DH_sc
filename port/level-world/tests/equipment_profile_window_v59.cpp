// Reuse the actual resource/text fixtures; all current owners are rebuilt by
// the eventual runner. No historical ABI library is used for changed Gear.
#define main prior_equipment_fixture_main_v59
#include "player_equipment_render_owner_v1.cpp"
#undef main
#include "../player_save_inventory_writer_v45.hpp"
int main(int argc,char** argv){try{
 ck(argc==9);auto source_design=design_input(argv[1]);views(source_design);
 dh2::character::CharacterGameDesign design;ck(design.initialize(source_design.view,e));
 dh2::skinning::VisualSkinResourcesV6 resources;ck(resources.load(file(argv[7]),e));
 auto gold=file(argv[8]);Reader original{gold};ck(original.u()==0x35564547&&original.u()==6);
 unsigned classes{},windows{},failures{};
 for(unsigned k=0;k<6;++k){original.u();auto selected=original.u(),n=original.u();PropertyState initial;
  original.copy(&initial,sizeof initial);
  for(unsigned j=0;j<n;++j){for(unsigned w=0;w<5;++w)original.u();PropertyState ignored;original.copy(&ignored,sizeof ignored);}
  if(selected)continue;
  Platform p;p.items=argv[2];p.powers=argv[3];p.env.assets=argv[4];p.env.private_files=argv[5];p.weapons=argv[6];
  auto constants=file(p.env.assets+"/original-cache/data/pydata/common_text_pycst.bin");dh2_script_constants_reload receipt{};
  ck(!dh2_script_constants_load(p.env.constants,constants.data(),constants.size(),&receipt));
  auto scene=resources.borrow().factory_scene();auto props=std::make_shared<PropertyState>(initial);LootRandom8V2 rng{1,0};
  auto input=[&](){PlayerEquipmentRenderInputsV1 v;v.design=design.borrow();v.properties=props;v.random=&rng;
   v.character=0x100000001ULL;v.potion_capacity=12;v.language_pack=0;v.resources=resources.borrow();v.live_scene=&scene;
   v.assets={&p,asset};v.debug=p.env.debug;v.debug_files={&p.env,debug_open,debug_close};
   v.text_environment.localization={&p.env,text_open,text_close,text_debug,text_constant,nullptr,nullptr};v.world={&p,world};v.required={&p,required};return v;};
  PlayerEquipmentRenderOwnerV1 first(input());ck(first.initialize(e));
  dh2::data::LootTablesV2::Borrow loot;dh2::data::ItemPowerTablesV5::Borrow powers;dh2::data::ItemTextServicesV5 text;LootRandom8V2* borrowed{};
  ck(first.loot_sources_v8(loot,powers,text,borrowed,e));dh2::level::SavegameStreamV2 saved;
  ck(dh2::level::player_save_inventory_writer_v45(*first.inventory(),powers.names(),saved,e));
  const auto expected=first.inventory()->items().size();ck(expected>0);
  props=std::make_shared<PropertyState>(initial);rng={1,0};unsigned calls{};
  auto restored_input=input();restored_input.source_profile_load_v59=[&](auto& gear,auto& error){
   ++calls;ck(gear.inventory()&&gear.inventory()->items().empty()&&gear.property_view());
   std::string premature;ck(!gear.finish_initial_grants_v60(premature));
   dh2::data::InventoryLoadReceiptV1 actual;ck(gear.load_saved_section_v59({saved.bytes().data(),saved.bytes().size()},actual,error)&&actual.completed);
   auto count=gear.inventory()->items().size();ck(count==expected);
   dh2::data::InventoryLoadReceiptV1 rejected;std::string nested;
   ck(!gear.load_saved_section_v59({saved.bytes().data(),saved.bytes().size()},rejected,nested)&&gear.inventory()->items().size()==count);
   return true;
  };
  PlayerEquipmentRenderOwnerV1 restored(std::move(restored_input));
  ck(restored.prepare_restore_v60(e)&&restored.prepared_v60()&&!restored.ready());
  ck(calls==1&&restored.inventory()->items().size()==expected);
  const auto restore_rng_calls=rng.calls;
  ck(!restored.prepare_restore_v60(e)&&rng.calls==restore_rng_calls&&calls==1);
  restored.project_potion_capacity(12); // Same owner, between source phases.
  ck(restored.finish_initial_grants_v60(e)&&restored.ready());
  auto completed_count=restored.inventory()->items().size();const auto grant_rng_calls=rng.calls;
  ck(!restored.finish_initial_grants_v60(e)&&rng.calls==grant_rng_calls&&restored.inventory()->items().size()==completed_count);
  ck(calls==1&&restored.inventory()->items().size()==expected&&restored.inventory()->properties()==props);++windows;
  dh2::data::InventoryLoadReceiptV1 outside;ck(!restored.load_saved_section_v59({saved.bytes().data(),saved.bytes().size()},outside,e));
  // Failed source profile delivery does not invoke the authored starting kit.
  props=std::make_shared<PropertyState>(initial);rng={1,0};auto failed_input=input();
  failed_input.source_profile_load_v59=[](auto&,auto& error){error="Required deliberate actual profile failure";return false;};
  PlayerEquipmentRenderOwnerV1 failed(std::move(failed_input));ck(!failed.initialize(e)&&!failed.ready());
  ck(!failed.prepared_v60()&&!failed.finish_initial_grants_v60(e));
  ck(failed.inventory()&&failed.inventory()->items().empty()&&rng.calls==0);++failures;
  auto duplicate=input();duplicate.saved_gear_v50=[](auto&,auto&,auto&,auto&){return true;};
  duplicate.source_profile_load_v59=[](auto&,auto&){return true;};auto calls_before=p.assets;
  PlayerEquipmentRenderOwnerV1 dual(std::move(duplicate));ck(!dual.initialize(e)&&!dual.inventory()&&p.assets==calls_before);++failures;++classes;
 }
 ck(original.at==gold.size()&&classes==3&&windows==3&&failures==6);
 std::cout<<"PASS "<<checks<<" actual-cache pre-Grant Gear window checks; explicit World selector fixture; no full campaign publication claim\n";return 0;
 }catch(const std::exception& x){std::cerr<<x.what()<<'\n';return 1;}}
