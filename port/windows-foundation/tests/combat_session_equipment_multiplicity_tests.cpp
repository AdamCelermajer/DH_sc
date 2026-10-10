#include "../combat_session.hpp"
#include "../game_save.hpp"
#include <iostream>
#include <stdexcept>
using namespace dh::foundation;
static void check(bool ok,const std::string& message){if(!ok)throw std::runtime_error(message);}
int main(int argc,char** argv){try{
    check(argc==2,"Supply original shared assets");AssetCatalog assets(argv[1]);std::string error;
    OriginalPropertyDatabase db;OriginalMeleeBindings bindings;
    check(load_original_property_tables(assets,"original-cache/data/pydata",db,error),error);
    check(bindings.load(assets,"original-melee-bindings.xml",error),error);
    CombatSessionConfig config;config.diagnosticRngSeed=19;config.playerId=1;
    config.playerProfileId="RoguePlayerBase";config.tableRoot="original-cache/data/pydata";
    config.mainItemId=config.offItemId="Dagger01";
    config.equippedItemIds={"StartingSuitRogue","StartingBootsRogue","StartingGlovesRogue","Dagger01","Dagger01"};
    ActorCustomization customization;customization.allow_missing_animation_targets=true;
    OriginalCombatVisualPlan plan;check(build_original_combat_visual_plan(assets,bindings,config.playerProfileId,customization,"gear-regression",plan,error),error);
    config.playerVisualConfig=plan.config;
    config.playerVisualConfig.motion_node_id="auto";config.playerVisualConfig.consume_root_motion=true;
    CombatSessionProfile policy;policy.animationOnly=true;policy.propertyOptions={256,false};policy.initialIdle={"Idle",0,{0}};
    config.profiles.emplace(config.playerProfileId,policy);
    const std::string root=config.tableRoot+"/";
    auto b=assets.read(root+"loot_table_pyarray.bin"),n=assets.read(root+"loot_table_pyarraynames.bin"),f=assets.read(root+"loot_table_pystructnames.bin");
    dh2::data::ItemTable items;check(dh2::data::load_items({b.data(),b.size()},{n.data(),n.size()},{f.data(),f.size()},items,error),error);
    std::vector<OriginalEquippedItem> expectedItems;
    for(std::size_t i=0;i<config.equippedItemIds.size();++i){const auto* item=dh2::data::item(items,dh2::data::item_id(items,config.equippedItemIds[i]));check(item,"Original source item absent");expectedItems.push_back({item->record,i==4,{}});}
    const auto dagger=items.rows.at(dh2::data::item_id(items,"Dagger01")).record;
    OriginalCombatFacts facts;facts.original_state=policy.originalCombatState;
    check(original_combat_equipment_facts(&dagger,&dagger,facts,error),error);
    OriginalCombatProperties reference;check(build_original_combat_properties(db,config.playerProfileId,policy.propertyOptions,expectedItems,facts,reference,error),error);
    CharacterVisual visual;ActorPopulation population;CombatSession session;
    check(session.initialize(assets,db,bindings,config,visual,population,{0,0,0},customization,error),error);
    const auto* actual=session.world()->combat_properties(1);
    check(actual&&actual->sheets.gear==reference.sheets.gear,"Session deduplicated identical equipped definitions or assigned both to one hand");
    check(session.actor(1)->equipment.size()==5&&actual->facts.dual_wield,"Session lost an equipped occurrence");
    auto state=make_default_character("dual-gear","Rogue","RoguePlayerBase");
    session.actor(1)->persistent_character_id=state.id;
    GameSave checkpoint;check(capture_game_save("gear-multiplicity",1,state,*session.world(),checkpoint,error),error);
    // Reinitialize from the original occurrence list, as production R does.
    check(session.initialize(assets,db,bindings,config,visual,population,{0,0,0},customization,error),error);
    session.actor(1)->persistent_character_id=state.id;
    session.detach_for_restore();
    check(restore_game_save(checkpoint,"gear-multiplicity",*session.world(),state,error),error);
    check(session.rebind_after_restore(error),error);
    check(session.world()->combat_properties(1)->sheets.gear==reference.sheets.gear,"Strict restored gear differs");
    // Older CLI callers can supply only the selectors, or just one occurrence;
    // complete the two hand roles once, without duplicating a complete list.
    for(const unsigned count:{0u,1u,2u}){
        config.equippedItemIds.assign(count,"Dagger01");
        check(session.initialize(assets,db,bindings,config,visual,population,{0,0,0},customization,error),error);
        const auto* props=session.world()->combat_properties(1);
        for(const auto p:{79u,80u,81u,82u})check(props->sheets.gear[p]==reference.sheets.gear[p],"Hand completion omitted/duplicated dagger stats");
        check(session.actor(1)->equipment.size()==2,"Hand completion occurrence count differs");
    }
    std::cout<<"PASS actual Rogue five source occurrences, same-definition main/off sheets, strict reinit/checkpoint restore, legacy hand completion\n";
    return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
