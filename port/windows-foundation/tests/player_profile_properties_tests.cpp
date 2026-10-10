#include "../player_profile_properties.hpp"
#include "../asset_catalog.hpp"
#include "../save_store.hpp"
#include "../game_save.hpp"
#include "../features/equipment/equipment_adapter.hpp"
#include <iostream>
#include <stdexcept>
using namespace dh::foundation;
static void check(bool ok,const std::string& message){if(!ok)throw std::runtime_error(message);}
static CharacterState profile(const char* row,const OriginalActorProperties& p){
    CharacterState c;c.id="projection-fixture";c.name="Profile";c.class_id=row;
    c.stats.level=1;c.stats.health=p.health;c.stats.max_health=p.max_health;
    c.stats.resource=p.resource;c.stats.max_resource=p.max_resource;
    c.stats.strength=original_signed256(p.sheets.resolved[149]);
    c.stats.dexterity=original_signed256(p.sheets.resolved[150]);
    c.stats.endurance=original_signed256(p.sheets.resolved[151]);
    c.stats.energy=original_signed256(p.sheets.resolved[152]);
    c.source_endurance_energy_known=true;c.source_points_known=true;
    return c;
}
int main(int argc,char** argv){try{
    check(argc==3,"Supply shared assets and isolated output directory");AssetCatalog assets(argv[1]);
    OriginalPropertyDatabase db;std::string error;
    check(load_original_property_tables(assets,"original-cache/data/pydata",db,error),error);
    const std::string root="original-cache/data/pydata/";
    auto b=assets.read(root+"loot_table_pyarray.bin"),n=assets.read(root+"loot_table_pyarraynames.bin"),f=assets.read(root+"loot_table_pystructnames.bin");
    dh2::data::ItemTable items;
    check(dh2::data::load_items({b.data(),b.size()},{n.data(),n.size()},{f.data(),f.size()},items,error),error);
    for(const auto* row:{"KnightPlayerBase","RoguePlayerBase","MagePlayerBase"}){
        OriginalActorProperties fresh;check(resolve_original_fresh_player(db,row,fresh,error),error);
        auto c=profile(row,fresh);c.experience=8;c.source_stat_points=2;c.source_skill_points=3;
        OriginalCombatProperties prepared,out;
        check(build_original_combat_properties(db,row,{256,false},{},{},prepared,error),error);
        check(prepared.sheets.resolved[36]<0,"Startup sentinel fixture missing");
        check(project_player_profile_properties(db,c,prepared,out,error),error);
        check(out.sheets.resolved[36]==fresh.sheets.resolved[36]&&out.sheets.resolved[41]==fresh.sheets.resolved[41],"Fresh vitals differ from source creation");
        check(out.sheets.resolved[33]==8*256&&out.sheets.resolved[148]==2*256&&out.sheets.resolved[157]==3*256,"Progression did not reach source sheet");
        check(out.sheets.gear==prepared.sheets.gear,"Projection changed authored gear");
        check(out.sheets.base==prepared.sheets.base,"Repeated source class derivation compounded authored base");
        OriginalCombatProperties repeat;check(project_player_profile_properties(db,c,out,repeat,error),error);
        check(repeat.sheets.saved==out.sheets.saved&&repeat.sheets.resolved==out.sheets.resolved,"Projection accumulated on repeat");
        auto allocated=c;allocated.stats.strength+=1;allocated.stats.dexterity+=2;
        check(project_player_profile_properties(db,allocated,out,repeat,error),error);
        check(repeat.sheets.resolved[149]==out.sheets.resolved[149]+256&&repeat.sheets.resolved[150]==out.sheets.resolved[150]+512,
              "Allocated attributes did not reach class formulas");
        auto allocatedAgain=repeat;check(project_player_profile_properties(db,allocated,repeat,allocatedAgain,error),error);
        check(allocatedAgain.sheets.resolved==repeat.sheets.resolved&&allocatedAgain.sheets.saved==repeat.sheets.saved,"Allocated attributes accumulated on repeat");
        auto fractional=out;dh2::data::PropertyRules rules;check(dh2::data::load_property_rules(db.characters,rules,error),error);
        auto view=dh2::data::property_view(rules,fractional.sheets);check(!dh2_property_add(&view,33,127),"XP fraction setup failed");
        check(project_player_profile_properties(db,c,fractional,repeat,error)&&repeat.sheets.resolved[33]==8*256+127,"Matching integer XP lost its live fraction");
        c.stats.health-=1.25f;c.stats.resource-=0.5f;
        check(project_player_profile_properties(db,c,out,repeat,error),error);
        check(repeat.sheets.resolved[36]==fresh.sheets.resolved[36]-320&&repeat.sheets.resolved[41]==fresh.sheets.resolved[41]-128,"Partial vitals were refilled");
        const auto path=std::filesystem::path(argv[2])/(std::string(row)+".dhsave");
        check(save_character(path,c,error),error);CharacterState loaded;
        check(load_character(path,loaded,error),error);
        OriginalCombatProperties reloaded;check(project_player_profile_properties(db,loaded,prepared,reloaded,error),error);
        check(reloaded.sheets.resolved==repeat.sheets.resolved,"Character disk restart changed projection");
        auto unknown=c;unknown.source_points_known=false;unknown.source_endurance_energy_known=false;
        check(project_player_profile_properties(db,unknown,prepared,repeat,error),error);
        for(const auto p:{148u,157u,151u,152u})check(repeat.sheets.resolved[p]==prepared.sheets.resolved[p],"Unknown legacy fields overwrote source state");
        auto bad=c;bad.stats.level=2;auto preserved=repeat;
        check(!project_player_profile_properties(db,bad,prepared,repeat,error)&&repeat.sheets.resolved==preserved.sheets.resolved,"Wrong level mutated output");
        bad=c;bad.class_id=std::string(row)=="KnightPlayerBase"?"RoguePlayerBase":"KnightPlayerBase";
        check(!project_player_profile_properties(db,bad,prepared,repeat,error)&&repeat.sheets.resolved==preserved.sheets.resolved,"Wrong source class accepted");
        bad=c;bad.stats.health=bad.stats.max_health+1;
        check(!project_player_profile_properties(db,bad,prepared,repeat,error)&&repeat.sheets.resolved==preserved.sheets.resolved,"Invalid vitals mutated output");
        bad=c;bad.stats.max_health+=100;bad.stats.health=bad.stats.max_health;
        check(!project_player_profile_properties(db,bad,prepared,repeat,error)&&repeat.sheets.resolved==preserved.sheets.resolved,"Profile bypassed source vital maximum");
        // Reuse the actual production equipment adapter on the same projected
        // source sheets, including the original class-specific starter weapon.
        c.inventory={{"weapon",std::string(row)=="RoguePlayerBase"?"Dagger01":std::string(row)=="MagePlayerBase"?"Staff01":"Longsword01",1}};
        c.equipment.clear();ActorState actor;actor.class_id=c.class_id;
        OriginalCombatProperties live;check(project_player_profile_properties(db,c,prepared,live,error),error);
        EquipmentAdapterOptions options;options.prepare_canonical_properties=[&](const ActorState&,dh2::data::PropertyState& s,std::string&){s=live.sheets;return true;};
        options.publish_combat=[&](const ActorState&,const OriginalCombatProperties& p,std::string&){live=p;return true;};
        EquipmentAdapter adapter(c,actor,live,items,db,options);
        for(unsigned i=0;i<3;++i){
            check(adapter.auto_equip("weapon",error),error);check(adapter.unequip(1,error),error);
            check(live.sheets.resolved[36]==fresh.sheets.resolved[36]-320&&live.sheets.resolved[41]==fresh.sheets.resolved[41]-128,"Equipment recalculation reset projected current vitals");
            check(live.sheets.resolved[33]==8*256&&live.sheets.resolved[157]==3*256,"Equipment recalculation reset progression");
        }
        dh2::data::AiTables ai;check(load_original_ai_tables(assets,root,ai,error),error);
        dh2::data::CombatRandom rng{991,17};PlayableActorWorld world(ai,rng);
        actor.id=1;actor.definition_id=row;actor.persistent_character_id=c.id;
        actor.faction_id=live.sheets.resolved[0];
        check(world.bind_actor(actor,live,{true,true,std::nullopt},error),error);
        auto* stable=world.find_actor(1);const auto before=rng;
        check(world.update_combat_properties(1,reloaded,*world.traits(1),error),error);
        check(world.find_actor(1)==stable&&rng.seed==before.seed&&rng.calls==before.calls&&
              world.combat_properties(1)->sheets.resolved==reloaded.sheets.resolved,
              "Publication replaced actor/RNG or lost profile properties");
        GameSave captured;const std::string level="profile-projection-fixture";
        check(capture_game_save(level,1,c,world,captured,error),error);
        const auto gamePath=std::filesystem::path(argv[2])/(std::string(row)+".game-save");
        check(save_game(gamePath,captured,error),error);GameSave checkpoint;
        check(load_game(gamePath,checkpoint,error),error);
        check(restore_game_save(checkpoint,level,world,c,error),error);
        check(world.combat_properties(1)->sheets.resolved==reloaded.sheets.resolved&&
              world.combat_properties(1)->sheets.saved==reloaded.sheets.saved&&
              world.find_actor(1)->health==c.stats.health&&rng.seed==before.seed&&rng.calls==before.calls,
              "Same-world GameSave projection/RNG restore differed");
        std::cout<<row<<" source/profile startup, partial vitals, points/XP, fraction, disk, repeat, equipment, same-world checkpoint and rejection PASS\n";
    }
    return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
