// Reuse only the source Gear cache/text asset primitives; this test owns its
// complete PlayerEquipmentRenderOwnerV1 setup and exercises that live Gear.
#define main inventory_projection_unused_gear_cache_main
#include "../../../game-data/tests/player_gear_cache_v5.cpp"
#undef main

#include "source_inventory_projection_v1.hpp"
#include "../../../level-world/player_equipment_render_owner_v1.hpp"
#include "../../../level-world/player_equipment_queries_v1.hpp"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <map>
#include <memory>
#include <stdexcept>

using namespace dh2::player;
using namespace dh::foundation::inventory;

static std::string e;
struct DesignInput {std::array<std::array<Raw,3>,5> tables;std::vector<Raw> constants;std::vector<Bytes> cviews;dh2::character::GameDesignInputs256 view;};
static std::string rd_string(Reader& r){auto n=r.u();std::string s(n,'\0');r.copy(s.data(),n);return s;}
static Raw rd_blob(Reader& r){auto n=r.u();Raw b(n);r.copy(b.data(),n);return b;}
static DesignInput design_input(const char* p){auto b=file(p);Reader r{b};ck(r.u()==0x314f4447);DesignInput out;for(auto& t:out.tables)for(auto& a:t)a=rd_blob(r);auto n=r.u();for(unsigned i=0;i<n;++i){rd_string(r);out.constants.push_back(rd_blob(r));}n=r.u();for(unsigned i=0;i<n;++i)rd_string(r);ck(r.at==b.size());return out;}
static void views(DesignInput& v){dh2::character::GameDesignTableInput48* t[]{&v.view.characters,&v.view.classes,&v.view.ai,&v.view.factions,&v.view.levels};for(unsigned i=0;i<5;++i)*t[i]={{v.tables[i][0].data(),v.tables[i][0].size()},{v.tables[i][1].data(),v.tables[i][1].size()},{v.tables[i][2].data(),v.tables[i][2].size()}};v.cviews.clear();for(auto& b:v.constants)v.cviews.push_back({b.data(),b.size()});v.view.constants=v.cviews.data();v.view.constant_count=v.cviews.size();}
struct Platform {TextEnvironment env;std::string items,powers,weapons;unsigned assets{},world{},notifications{};bool asset_failure{},reentry{};PlayerEquipmentRenderOwnerV1* owner{};};
static dh2::skinning::VisualAssetResultV6 asset(void* p,const char* name,Raw& b,std::string& error){auto& c=*static_cast<Platform*>(p);++c.assets;if(c.asset_failure){error="Deliberate real provider rejection";return dh2::skinning::VisualAssetResultV6::failed;}std::string uri=name,full;if(uri.find("data/pydata/loot_table_")==0)full=c.items+"/"+uri.substr(12);else if(uri.find("data/pydata/item_powers_")==0)full=c.powers+"/"+uri.substr(12);else if(uri.find("data/3d/characters/prince/weapons/")==0){auto base=uri.substr(uri.find_last_of('/')+1);for(auto& a:base)a=char(std::tolower(static_cast<unsigned char>(a)));full=c.weapons+"/"+base;}else full=c.env.assets+"/original-cache/"+uri;if(!std::filesystem::exists(full))return dh2::skinning::VisualAssetResultV6::missing;b=file(full);return dh2::skinning::VisualAssetResultV6::found;}
static bool world(void* p,EquipmentWorldQueryV1 q,std::uintptr_t subject,std::uintptr_t& id,std::int32_t& value,std::string&){auto& c=*static_cast<Platform*>(p);ck(subject==0x100000001ULL);++c.world;id=0;value=0;if(q==EquipmentWorldQueryV1::current_player)id=subject;if(q==EquipmentWorldQueryV1::player_count)value=1;if(c.reentry&&c.owner){std::string error;ck(!c.owner->swap(error));}return true;}
static bool required(void* p,dh2::data::FreshInventoryOwnedV4&,const dh2::data::OwnedInventoryRequestV4&,dh2::data::OwnedInventoryResponseV4&,std::string& error){++static_cast<Platform*>(p)->notifications;error="Required remaining source continuation deliberately unavailable";return false;}

int main(int argc,char** argv){try{
 ck(argc==9);
 auto raw=design_input(argv[1]);views(raw);
 dh2::character::CharacterGameDesign design;ck(design.initialize(raw.view,e));
 dh2::skinning::VisualSkinResourcesV6 resources;ck(resources.load(file(argv[7]),e));
 auto gold=file(argv[8]);Reader source{gold};ck(source.u()==0x35564547&&source.u()==6);
 PropertyState initial{};bool selected_fixture=false;
 for(unsigned k=0;k<6&&!selected_fixture;++k){
  (void)source.u();const auto selected=source.u();const auto sheet_count=source.u();
  source.copy(&initial,sizeof initial);
  for(unsigned j=0;j<sheet_count;++j){for(unsigned w=0;w<5;++w)(void)source.u();PropertyState ignored;source.copy(&ignored,sizeof ignored);}
  selected_fixture=selected==0;
 }
 ck(selected_fixture);

 Platform platform;platform.items=argv[2];platform.powers=argv[3];
 platform.env.assets=argv[4];platform.env.private_files=argv[5];platform.weapons=argv[6];
 auto constants=file(platform.env.assets+"/original-cache/data/pydata/common_text_pycst.bin");
 dh2_script_constants_reload receipt{};
 ck(!dh2_script_constants_load(platform.env.constants,constants.data(),constants.size(),&receipt));
 auto properties=std::make_shared<PropertyState>(initial);
 auto scene=resources.borrow().factory_scene();LootRandom8V2 random{1,0};
 PlayerEquipmentRenderInputsV1 input;input.design=design.borrow();input.properties=properties;
 input.random=&random;input.character=0x100000001ULL;input.potion_capacity=12;
 input.language_pack=0;input.resources=resources.borrow();input.live_scene=&scene;
 input.assets={&platform,asset};input.debug=platform.env.debug;
 input.debug_files={&platform.env,debug_open,debug_close};
 input.text_environment.localization={&platform.env,text_open,text_close,text_debug,text_constant,nullptr,nullptr};
 input.world={&platform,world};input.required={&platform,required};
 auto gear=std::make_shared<PlayerEquipmentRenderOwnerV1>(std::move(input));
 platform.owner=gear.get();ck(gear->initialize(e)&&gear->ready());
 ck(gear->item_text_owner_v1()!=nullptr&&gear->inventory()!=nullptr);
 const auto* inventory=gear->inventory();const auto& table=inventory->table();
 std::weak_ptr<const PlayerEquipmentRenderOwnerV1> weak_gear=gear;
 unsigned invalidations=0;
 NativeGearInventoryBorrowV1 borrow=[weak_gear](NativeGearInventoryLeaseV1& loan,std::string& error){
  loan.gear=weak_gear.lock();if(!loan.gear){error="Actual Gear owner expired";return false;}error.clear();return true;
 };
 {
  SourceInventoryProjectionV1 projection(borrow,[&]{++invalidations;});
  SourceInventoryProjectionRowsV1 rows;ck(projection.bind_initial(rows,e));
  ck(rows.inventory.size()==inventory->items().size()&&
     rows.inventory.size()==rows.source_indices.size()&&
     rows.inventory.size()==rows.native_instances.size());
  std::map<const dh2::data::ItemInstanceV1*,std::string> id_by_pointer;
  std::map<std::string,std::string> expected_equipment;
  for(std::size_t i=0;i<rows.inventory.size();++i){
   const auto* item=inventory->items()[i]->item.get();
   ck(rows.native_instances[i]==item&&rows.source_indices[i]==static_cast<std::uint32_t>(i));
   ck(rows.inventory[i].instance_id.find("source-native-ui-v1:")==0);
   ck(rows.inventory[i].definition_id==table.identifiers[std::size_t(item->id)]&&
      rows.inventory[i].quantity==std::uint32_t(item->signed_quantity()));
   ck(id_by_pointer.emplace(item,rows.inventory[i].instance_id).second);
  }
  const auto set=std::size_t(inventory->current_equipment());
  for(std::size_t slot=0;slot<inventory->equipment()[set].size();++slot){
   const auto* owned_slot=inventory->equipment()[set][slot];if(!owned_slot)continue;
   const auto name="slot"+std::to_string(slot);
   expected_equipment.emplace(name,id_by_pointer.at(owned_slot->item.get()));
  }
  ck(rows.equipment.size()==expected_equipment.size());
  for(const auto& binding:rows.equipment)
   ck(expected_equipment.at(binding.slot)==binding.item_instance_id);

  auto target=dh::foundation::make_default_character();target.id="preserved-player";
  target.class_id="KnightPlayerBase";target.gold=777;target.stats.intelligence=41.5f;
  target.skills.push_back({"preserved-skill",3});
  ck(apply_source_inventory_projection_v1(rows,target,e));
  ck(target.inventory.size()==rows.inventory.size()&&target.equipment.size()==rows.equipment.size());
  for(std::size_t i=0;i<rows.inventory.size();++i)
   ck(target.inventory[i].instance_id==rows.inventory[i].instance_id&&
      target.inventory[i].definition_id==rows.inventory[i].definition_id&&
      target.inventory[i].quantity==rows.inventory[i].quantity);
  for(std::size_t i=0;i<rows.equipment.size();++i)
   ck(target.equipment[i].slot==rows.equipment[i].slot&&
      target.equipment[i].item_instance_id==rows.equipment[i].item_instance_id);
  ck(target.id=="preserved-player"&&target.class_id=="KnightPlayerBase"&&target.gold==777&&target.stats.intelligence==41.5f&&
     target.skills.size()==1&&target.skills[0].id=="preserved-skill");

  // Real Gear equip/unequip mutates the selected equipment pointers while
  // preserving the same ItemInstances and their process-local IDs.
  projection.invalidate_before_native_mutation();
  ck(!projection.projection_valid()&&invalidations==1);
  ck(gear->equip(0,0,e));
  SourceInventoryProjectionRowsV1 equipped;ck(projection.refresh_after_native_mutation(equipped,e));
  for(std::size_t i=0;i<equipped.inventory.size();++i)
   ck(equipped.inventory[i].instance_id==id_by_pointer.at(equipped.native_instances[i]));
  ck(std::any_of(equipped.equipment.begin(),equipped.equipment.end(),[](const auto& binding){return binding.slot=="slot0";}));
  projection.invalidate_before_native_mutation();ck(gear->unequip(0,e));
  SourceInventoryProjectionRowsV1 unequipped;ck(projection.refresh_after_native_mutation(unequipped,e));
  for(std::size_t i=0;i<unequipped.inventory.size();++i)
   ck(unequipped.inventory[i].instance_id==id_by_pointer.at(unequipped.native_instances[i]));

  projection.invalidate_before_native_mutation();ck(gear->swap(e));
  SourceInventoryProjectionRowsV1 other_set;ck(projection.refresh_after_native_mutation(other_set,e));
  for(std::size_t i=0;i<other_set.inventory.size();++i)
   ck(other_set.inventory[i].instance_id==id_by_pointer.at(other_set.native_instances[i]));

  const auto previous_epoch=projection.gear_epoch();
  SourceInventoryProjectionRowsV1 restored;ck(projection.rebind_after_gear_restore(restored,e));
  ck(restored.gear_epoch==previous_epoch+1&&projection.projection_valid());
  for(std::size_t i=0;i<restored.inventory.size();++i)
   ck(restored.inventory[i].instance_id!=other_set.inventory[i].instance_id);
  ck(platform.notifications==0);
 }
 gear.reset();ck(weak_gear.expired());
 std::cout<<"source inventory projection PASS actual Gear order/definitions/quantity/equipment, invalidation, equip/restore epochs, Gear text owner\n";
 return 0;
}catch(const std::exception& exception){std::cerr<<exception.what()<<'\n';return 1;}}
