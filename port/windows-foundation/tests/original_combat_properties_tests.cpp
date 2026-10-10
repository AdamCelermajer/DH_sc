#include "../original_combat_properties.hpp"
#include "../asset_catalog.hpp"
#include <iostream>
#include <stdexcept>
using namespace dh::foundation;
static void check(bool value, const std::string& message) { if (!value) throw std::runtime_error(message); }
int main(int argc,char** argv) {
 try {
    if (argc != 2) throw std::runtime_error("Supply original equipment asset root");
    AssetCatalog assets(argv[1]); const std::string root="original-cache/data/pydata/";
    OriginalPropertyDatabase db; std::string error;
    check(load_original_property_tables(assets,root,db,error),error);
    const auto data=assets.read(root+"loot_table_pyarray.bin"),
               names=assets.read(root+"loot_table_pyarraynames.bin"),
               fields=assets.read(root+"loot_table_pystructnames.bin");
    dh2::data::ItemTable items;
    check(dh2::data::load_items({data.data(),data.size()},{names.data(),names.size()},
          {fields.data(),fields.size()},items,error),error);
    const auto& sword=items.rows.at(664).record;
    std::cout << "original Sword664 type=" << sword.words[22] << " min=" << sword.words[35]
              << " max=" << sword.words[36] << " category=" << sword.words[37] << '\n';
    check(sword.words[35]==12*256 && sword.words[36]==15*256,"original starter sword range differs");
    OriginalCombatFacts facts;
    check(original_combat_equipment_facts(&sword,nullptr,facts,error),error);
    check(facts.main_damage_class==0 && !facts.shield && !facts.dual_wield,"original starter weapon facts differ");
    std::vector<OriginalEquippedItem> gear;
    // Exact warrior fixed equipped entries; healing potion is not equipped.
    for (const unsigned id : {1079u,1073u,1076u,664u}) gear.push_back({items.rows.at(id).record,false,{}});
    OriginalCombatProperties warrior,mob;
    check(build_original_combat_properties(db,"KnightPlayerBase",{256,true},gear,facts,warrior,error),error);
    check(build_original_combat_properties(db,"Swamp_LizadMan_Type1",{std::nullopt,true},{},{},mob,error),error);
    std::cout << "warrior resolved min=" << warrior.sheets.resolved[79] << " max=" << warrior.sheets.resolved[80]
              << " defense=" << warrior.sheets.resolved[71] << " mob defense=" << mob.sheets.resolved[71] << '\n';
    check(warrior.sheets.gear[79]==12*256 && warrior.sheets.gear[80]==15*256,
          "original equipped sword projection differs");
    OriginalMeleeDamageProvider provider;
    check(provider.bind_actor(1,warrior,error) && provider.bind_actor(2,mob,error),error);
    check(provider.bind_source("main",{},error) && provider.bind_source("off",{true,false},error),error);
    dh2::data::CombatRandom random{1234,0}; OriginalMeleeResolution result;
    unsigned hits=0,misses=0; float min=100000,max=0;
    for (unsigned i=0;i<100;++i) {
        check(provider.resolve("main",1,2,random,result,error),error);
        if (result.original.outcomes&3u) { check(result.damage==0,"miss caused health damage"); ++misses; }
        else { ++hits; min=std::min(min,result.damage); max=std::max(max,result.damage); }
    }
    check(hits>0 && misses>0 && random.calls>100,"original melee outcomes/RNG not exercised");
    std::cout << "original warrior vs lizard hits=" << hits << " misses=" << misses
              << " damage=" << min << ".." << max << " RNGcalls=" << random.calls << '\n';
    check(provider.resolve("main",2,1,random,result,error),error); // same path for NPC attacker
    const auto& dagger = items.rows.at(370).record;
    OriginalCombatFacts rogue_facts;
    check(original_combat_equipment_facts(&dagger,&dagger,rogue_facts,error),error);
    check(rogue_facts.dual_wield && !rogue_facts.shield,"original rogue dual-hand facts differ");
    OriginalCombatProperties rogue;
    check(build_original_combat_properties(db,"RoguePlayerBase",{256,true},
        {{dagger,false,{}},{dagger,true,{}}},rogue_facts,rogue,error),error);
    check(provider.bind_actor(4,rogue,error),error);
    check(provider.resolve("off",4,2,random,result,error),error);
    check(result.original.mask & (1u<<26),"offhand source lost original result mask");
    const auto& staff = items.rows.at(1025).record;
    OriginalCombatFacts mage_facts;
    check(original_combat_equipment_facts(&staff,nullptr,mage_facts,error),error);
    OriginalCombatProperties mage;
    check(build_original_combat_properties(db,"MagePlayerBase",{256,true},
        {{staff,false,{}}},mage_facts,mage,error),error);
    check(provider.bind_actor(5,mage,error) && provider.resolve("main",5,2,random,result,error),error);
    // Compare the shared result path against the actual source kernel on
    // original sheets; no new RNG or fixture damage is introduced.
    const auto combatView=[](const OriginalCombatProperties& p){const auto& f=p.facts;
        return dh2::data::CombatantView{p.sheets.resolved.data(),f.main_damage_class,f.off_damage_class,
            unsigned(f.two_hander),unsigned(f.dual_wield),unsigned(f.shield),f.original_state,f.combo_hits};};
    const auto spellAttacker=combatView(mage),spellVictim=combatView(mob);
    for(unsigned seed=1;seed<=32;++seed){
        dh2::data::CombatRandom actual{seed,0},expected=actual;
        dh2::data::CombatResult source;
        const dh2::data::CombatResultRequest request{&spellAttacker,&spellVictim,&expected,0x1005554au,-1,0,0};
        check(dh2_combat_result(&source,&request)==0,"Source spell-style result request failed");
        OriginalMeleeResolution projected;
        check(provider.resolve_result(5,2,request.mask,request.weapon_category,request.element,
            request.direct_amount,actual,projected,error),error);
        check(actual.seed==expected.seed&&actual.calls==expected.calls&&
            projected.original.amount==source.amount&&projected.original.outcomes==source.outcomes&&
            projected.original.mask==source.mask&&projected.original.element==source.element&&
            projected.original.weapon_category==source.weapon_category&&
            projected.original.dot_element==source.dot_element&&projected.original.dot_amount==source.dot_amount&&
            projected.original.dot_duration==source.dot_duration&&projected.original.hp_leech==source.hp_leech&&
            projected.original.mp_leech==source.mp_leech,"Shared result lost source payload/RNG");
    }
    {const auto saved=random;const auto preserved=result;
        check(!provider.resolve_result(5,999,0x1005554au,-1,0,0,random,result,error),"Missing source result receiver accepted");
        check(random.seed==saved.seed&&random.calls==saved.calls&&result.original.amount==preserved.original.amount,
            "Rejected source result changed stream/output");}
    std::cout<<"Shared skill/spell result PASS exact kernel payload and RNG across32 source-sheet seeds\n";
    auto defense=mob; defense.sheets.resolved[71]+=25600;
    check(provider.bind_actor(3,defense,error),error);
    // Replay identical stream against greater defense; outcome eligibility unchanged.
    bool defended=false;
    for (unsigned seed=1;seed<100 && !defended;++seed) {
        dh2::data::CombatRandom a{seed,0},b=a; OriginalMeleeResolution normal,armored;
        check(provider.resolve("main",1,2,a,normal,error),error);
        check(provider.resolve("main",1,3,b,armored,error),error);
        if (!(normal.original.outcomes&3u)) {
            check(armored.damage<=normal.damage && armored.damage>=1,"original defense reduction/floor differs");
            defended=true;
        }
    }
    check(defended,"defense scenario never hit");
    const auto before_rng=random; const auto before_result=result;
    check(!provider.resolve("missing",1,2,random,result,error),"unbound source accepted");
    check(random.seed==before_rng.seed && random.calls==before_rng.calls &&
          result.original.amount==before_result.original.amount,"failure changed RNG/result");
    provider.remove_actor(2);
    check(!provider.resolve("main",1,2,random,result,error),"removed victim accepted");
    std::cout << "original_combat_properties PASS\n";
 } catch(const std::exception& ex) { std::cerr << ex.what() << '\n'; return 1; }
}
