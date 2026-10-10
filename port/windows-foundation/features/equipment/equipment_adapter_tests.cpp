#include "equipment_adapter.hpp"
#include "equipment_menu.hpp"
#include "../../asset_catalog.hpp"
#include <algorithm>
#include <iostream>
#include <stdexcept>
using namespace dh::foundation;
static void check(bool x,const std::string& e){if(!x)throw std::runtime_error(e);}
int main(int argc,char** argv){try{
    check(argc==2,"Supply staged original asset root");AssetCatalog assets(argv[1]);std::string error;
    OriginalPropertyDatabase database;check(load_original_property_tables(assets,"original-cache/data/pydata",database,error),error);
    OriginalActorProperties properties;check(resolve_original_fresh_player(database,"KnightPlayerBase",properties,error),error);
    const auto root=std::filesystem::path("original-cache/data/pydata");
    auto b=assets.read(root/"loot_table_pyarray.bin"),n=assets.read(root/"loot_table_pyarraynames.bin"),f=assets.read(root/"loot_table_pystructnames.bin");
    dh2::data::ItemTable table;check(dh2::data::load_items({b.data(),b.size()},{n.data(),n.size()},{f.data(),f.size()},table,error),error);
    CharacterState character;ActorState actor;
    character.inventory={{"sword","Longsword01",1},{"suit","StartingSuit",1},{"boots","StartingBoots",1},{"gloves","StartingGloves",1},{"sword2","Longsword01",1}};
    EquipmentAdapter adapter(character,actor,properties,table,database);
    const auto original=properties.sheets.resolved;
    for(const auto* id:{"sword","suit","boots","gloves"})check(adapter.equip(id,error),error);
    check(character.equipment.size()==4&&actor.equipment.size()==4,"Equipment ownership projections differ");
    check(properties.sheets.gear[79]==3072&&properties.sheets.gear[80]==3840,"Original Longsword01 damage differs");
    check(properties.sheets.gear[71]==1024,"Original suit/boots/gloves armor differs");
    auto expected=properties.sheets.resolved; const auto gear=properties.sheets.gear; dh2::data::PropertyRules rules; check(dh2::data::load_property_rules(database.characters,rules,error),error);
    for(unsigned i=0;i<100;++i){auto reference=properties.sheets;check(dh2::data::recalc_properties_with_class(database.classes,rules,reference,error),error);auto view=dh2::data::property_view(rules,reference);check(!dh2_gear_validate_vitals_v5(&view),"Reference source vital clamp failed");check(adapter.equip(i%2?"sword":"sword2",error),error);check(properties.sheets.gear==gear,"Repeated swap accumulated derived gear");check(properties.sheets.resolved==reference.resolved,"Source recalculation replay differs");expected=properties.sheets.resolved;}
    check(character.equipment.size()==4,"Repeated swap duplicated bindings");
    const auto before=character.equipment;check(!adapter.equip("missing",error),"Missing instance accepted");
    check(properties.sheets.resolved==expected&&character.equipment.size()==before.size(),"Failure mutated equipment or properties");
    auto sword=table.rows[dh2::data::item_id(table,"Longsword01")];
    sword.record.words[29]=2;check(!equipment_meets_requirements(sword,properties),"Original level requirement ignored");
    check(equipment_meets_requirements(sword,properties,true),"Explicit source online predicate bypass differs");
    sword.record.words[29]=0;sword.record.words[30]=1;auto raw=properties;raw.sheets.resolved[149]=255;
    check(!equipment_meets_requirements(sword,raw),"Fractional attribute requirement rounded up");
    raw.sheets.resolved[149]=256;check(equipment_meets_requirements(sword,raw),"Exact requirement boundary rejected");
    EquipmentAdapterOptions options;options.visuals=[](const auto&,const auto&,auto&,std::string& e){e="fixture visual unavailable";return false;};
    EquipmentAdapter visual(character,actor,properties,table,database,options);
    check(!visual.unequip(1,error),"Incomplete visual owner accepted");check(properties.sheets.resolved==expected,"Failed visual staging mutated sheets");
    for(auto slot:{0u,1u,3u,4u})check(adapter.unequip(slot,error),error);
    check(character.equipment.empty()&&actor.equipment.empty(),"Unequip left stale bindings");
    check(properties.sheets.gear==rules.defaults,"Unequip failed to reset source gear sheet");for(auto p:{71u,79u,80u})check(properties.sheets.resolved[p]==original[p],"Unequip left stale armor or damage");
    check(adapter.unequip(1,error),error);check(!adapter.unequip(100,error),"Invalid source slot accepted");
    character.inventory.push_back({"stack","Longsword01",2});check(!adapter.equip("stack",error),"Unimplemented split producer silently accepted");
    // Concrete same-owner combat publication seam, independent of menu redraw.
    OriginalCombatProperties combat;combat.sheets=properties.sheets;
    EquipmentAdapterOptions connected;unsigned publications=0;bool reject=false;
    connected.prepare_canonical_properties=[&](const ActorState& actual_actor,dh2::data::PropertyState& candidate,std::string&){
        check(&actual_actor==&actor,"Canonical vitals service received a detached actor");
        candidate=combat.sheets;return true;
    };
    connected.publish_combat=[&](const ActorState& candidate,const OriginalCombatProperties& p,std::string& e){
        if(reject){e="fixture combat owner rejects preparation";return false;}
        combat=p;++publications;check(!candidate.equipment.empty(),"Prepared actor ownership differs");return true;
    };
    EquipmentAdapter live(character,actor,combat,table,database,connected);
    const auto before_combat=combat.sheets;
    reject=true;check(!live.equip("sword",error),"Rejected actual combat publication accepted");
    check(character.equipment.empty()&&actor.equipment.empty()&&combat.sheets.resolved==before_combat.resolved,"Failed publication mutated owner");
    reject=false;check(live.equip("sword",error),error);check(publications==1&&combat.facts.main_damage_class==table.rows[dh2::data::item_id(table,"Longsword01")].record.words[37],"Shared combat facts not published");
    equipment_menu::Options menu_options;
    menu_options.item_name=[](const auto&,const auto& source,std::string& text,std::string&){text="fixture source textOID "+std::to_string(source.record.words[17]);return true;};
    menu_options.empty_name=[](std::string& text,std::string&){text="fixture GLOBAL_EMPTY";return true;};
    equipment_menu::Presenter menu(character,table,combat.sheets,live,menu_options);
    const auto frame_before=combat.sheets;const auto base_before=combat.sheets.base;
    for(unsigned i=0;i<10;++i){character_menu::Frame frame;
        frame.art.batches.push_back({"menu_InventorySheetMain/inv_anim/btn_potions/source",999,{}});
        check(menu.frame(frame,error),error);check(frame.text.size()==9,"Actual slot text projections missing");
        check(frame.art.batches.front().shape_id==999,"Equipment frame discarded potion owner art");
    }
    check(combat.sheets.resolved==frame_before.resolved&&combat.sheets.base==base_before&&publications==1,"Menu redraw mutated original class sheets");
    std::vector<equipment_menu::OwnedSelection> menu_view;check(menu.view(menu_view,error),error);
    check(menu_view.size()==character.inventory.size()&&menu_view.front().name_text_oid==table.rows[dh2::data::item_id(table,"Longsword01")].record.words[17],"Equipment view copied private ownership or wrong name field");
    check(equipment_menu::original_slot_art().size()==9,"Original category slot export incomplete");
    check(equipment_menu::original_slot_art()[0].art.batches.back().shape_id!=equipment_menu::original_slot_art()[1].art.batches.back().shape_id,"Source category icons remained torso placeholders");
    for(const auto& slot:equipment_menu::original_slot_art()){
        const auto& hit=slot.hit_triangles;check(hit.size()>=3,"Source MovieClip hit contours absent");
        float x=(hit[0].x+hit[1].x+hit[2].x)/3,y=(hit[0].y+hit[1].y+hit[2].y)/3;
        check(menu.hit_test(x,y)==slot.source_slot,"Authored source slot hit maps incorrectly");
    }
    check(menu.select_slot(1,error),error);check(menu.selected_instance()=="sword","Actual equipped instance selection not retained");
    check(!menu.select_instance("missing",error),"Menu selected nonexistent ownership");
    check(menu.select_slot(0,error),error);check(menu.view_for_selected_slot(menu_view,error),error);
    check(menu_view.size()==1&&menu_view.front().definition_id=="StartingSuit","Source torso candidate filter differs");
    check(menu.select_slot(2,error),error);check(menu.view_for_selected_slot(menu_view,error),error);
    check(menu_view.empty(),"Source ordinary sword incorrectly offered as offhand");
    auto dual_before=combat.sheets.resolved[202];combat.sheets.resolved[202]=256;
    check(menu.view_for_selected_slot(menu_view,error),error);check(menu_view.size()==3,"Source property202 dual-wield candidate conversion differs");
    combat.sheets.resolved[202]=dual_before;
    character.inventory.push_back({"potion","Potion0",1});check(menu.select_slot(9,error),error);
    check(menu.view_for_selected_slot(menu_view,error),error);check(menu_view.size()==1&&menu_view.front().definition_id=="Potion0","Source category9 valuables projection differs");
    check(!menu.unequip_selected_slot(error),"Source valuables category became equipment slot9");
    check(menu.select_slot(1,error),error);check(menu.select_instance("sword2",error),error);
    check(menu.equip_selected(error),error);check(publications==2,"Selected-slot action did not publish exactly once");
    check(character.equipment.size()==1&&character.equipment.front().slot=="slot1"&&character.equipment.front().item_instance_id=="sword2","Selected actual instance did not replace requested source slot");

    // Reproduce the real Rogue starter ownership from the original class loot:
    // slot 1 is unequipped while the second Dagger01 remains in slot 2, then
    // the removed source instance is auto-equipped after a validated menu frame.
    OriginalActorProperties rogue_properties;
    check(resolve_original_fresh_player(database,"RoguePlayerBase",rogue_properties,error),error);
    check(rogue_properties.sheets.resolved[36]==37580&&rogue_properties.sheets.resolved[41]==10304,
          "Rogue source initial vital cells differ");
    CharacterState rogue;rogue.id="rogue-source-fixture";rogue.name="Rogue";rogue.class_id="RoguePlayerBase";
    rogue.stats.level=1;
    rogue.stats.health=rogue_properties.health;rogue.stats.max_health=rogue_properties.max_health;
    rogue.stats.resource=rogue_properties.resource;rogue.stats.max_resource=rogue_properties.max_resource;
    rogue.stats.strength=original_signed256(rogue_properties.sheets.resolved[149]);
    rogue.stats.dexterity=original_signed256(rogue_properties.sheets.resolved[150]);
    rogue.inventory={{"starter-item-0","StartingSuitRogue",1},
        {"starter-item-1","StartingBootsRogue",1},{"starter-item-2","StartingGlovesRogue",1},
        {"starter-item-3","Dagger01",1},{"starter-item-4","Dagger01",1}};
    rogue.equipment={{"slot0","starter-item-0",0,0},{"slot3","starter-item-1",0,3},
        {"slot4","starter-item-2",0,4},{"slot1","starter-item-3",0,1},
        {"slot2","starter-item-4",0,2}};
    for(const auto* definition:{"StartingSuitRogue","StartingBootsRogue","StartingGlovesRogue","Dagger01"})
        check(dh2::data::item_id(table,definition)>=0,std::string("Original Rogue starter item missing: ")+definition);
    ActorState rogue_actor;rogue_actor.class_id=rogue.class_id;
    rogue_actor.health=rogue_properties.health;rogue_actor.max_health=rogue_properties.max_health;
    rogue_actor.resource=rogue_properties.resource;rogue_actor.max_resource=rogue_properties.max_resource;
    EquipmentAdapter rogue_adapter(rogue,rogue_actor,rogue_properties,table,database);
    check(rogue_adapter.unequip(1,error),error);
    const auto validate_next_menu_frame=[&](){
        const auto validation=validate_character_state(rogue);
        check(validation.ok(),validation.errors.empty()?"Rogue next menu frame rejected":validation.errors.front());
        const auto& resolved=rogue_properties.sheets.resolved;
        const float expected_health=std::max(0.0f,original_signed256(resolved[36]));
        const float expected_max_health=std::max(0.0f,original_signed256(resolved[38]));
        const float expected_resource=std::max(0.0f,original_signed256(resolved[41]));
        const float expected_max_resource=std::max(0.0f,original_signed256(resolved[43]));
        check(rogue.stats.health==expected_health&&rogue_actor.health==expected_health&&
              rogue_properties.health==expected_health&&rogue.stats.max_health==expected_max_health&&
              rogue_actor.max_health==expected_max_health&&rogue_properties.max_health==expected_max_health&&
              rogue.stats.resource==expected_resource&&rogue_actor.resource==expected_resource&&
              rogue_properties.resource==expected_resource&&rogue.stats.max_resource==expected_max_resource&&
              rogue_actor.max_resource==expected_max_resource&&rogue_properties.max_resource==expected_max_resource,
              "Rogue semantic vitals differ from nonnegative source-cell projection");
    };
    validate_next_menu_frame();
    const auto after_unequip_source=rogue_properties.sheets.resolved;
    check(rogue_adapter.auto_equip("starter-item-3",error),error);
    validate_next_menu_frame();
    check(rogue_properties.sheets.resolved!=after_unequip_source,
          "Rogue auto-equip did not recalculate source property cells");
    check(rogue.equipment.size()==5&&rogue_actor.equipment.size()==5,
          "Rogue unequip→frame→auto-equip did not restore five actual bindings");
    std::cout<<"equipment_adapter PASS: original cache gear, 100 swaps, requirements, rollback, Rogue unequip/frame/auto-equip vitals\n";
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}


