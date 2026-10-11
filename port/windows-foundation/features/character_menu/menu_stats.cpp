#include "menu_stats.hpp"
#include <algorithm>
#include <cstring>
namespace dh::foundation::character_menu {
namespace {
std::int32_t add(std::int32_t a,std::int32_t b){auto bits=std::uint32_t(a)+std::uint32_t(b);std::int32_t result;std::memcpy(&result,&bits,4);return result;}
bool component(const std::string& path,const char* value){return path.find(std::string("/")+value+"/")!=std::string::npos;}
}
bool project_original_menu_stats(const OriginalCombatProperties& p,NativeMenuStats& output,std::string& error){
    const auto& raw=p.sheets.resolved;const auto& f=p.facts;
    if(f.main_damage_class< -1||f.main_damage_class>=141||f.off_damage_class< -1||f.off_damage_class>=141){
        error="Original menu equipment damage class is outside source bounds";return false;
    }
    auto prop=[&](unsigned i){return source_stat_integer(raw[i]);};
    NativeMenuStats next;
    const auto bonus=f.main_damage_class<0?0:source_stat_integer(add(raw[51+unsigned(f.main_damage_class)],f.dual_wield?raw[58]:0));
    next.attack=add(prop(50),bonus);
    next.critical=f.main_damage_class<0?prop(63):add(prop(63),prop(64+unsigned(f.main_damage_class)));
    next.defense=prop(59);next.armor=prop(71);
    const dh2::data::CombatantView combat{raw.data(),f.main_damage_class,f.off_damage_class,
        unsigned(f.two_hander),unsigned(f.dual_wield),unsigned(f.shield),f.original_state,f.combo_hits};
    std::int32_t damage_bonus[2];
    if(dh2_combat_bonus(&combat,0,&damage_bonus[0])||dh2_combat_bonus(&combat,1,&damage_bonus[1])){
        error="Original menu combat bonus kernel rejected same resolved properties";return false;
    }
    next.damage={add(prop(79),source_stat_integer(damage_bonus[0])),add(prop(80),source_stat_integer(damage_bonus[0])),
        add(prop(81),source_stat_integer(damage_bonus[1])),add(prop(82),source_stat_integer(damage_bonus[1]))};
    for(unsigned i=0;i<5;++i)next.resistance[i]=prop(74+i);
    next.dodge=prop(60);next.block=prop(61);next.melee_hit=prop(207);next.melee_reduction=prop(209);
    next.spell_crit=prop(165);next.spell_hit=prop(208);
    next.spell_bonus={prop(166),prop(167),prop(168),prop(169),prop(170)};
    next.regen_hp=prop(39);next.regen_mp=prop(44);next.leech_hp=prop(132);next.leech_mp=prop(133);
    next.element_type={prop(97),prop(100)};output=next;error.clear();return true;
}
bool is_stat_sheet_path(const std::string& path) noexcept {
    for(const char* sheet:{"menu_CharacterSheetStats/","menu_CharacterSheetDefense/","menu_CharacterSheetOffense/",
                           "menu_CharacterSheetMagic/","menu_CharacterSheetRecovery/"})
        if(path.find(sheet)!=std::string::npos)return true;
    return false;
}
// Sheets whose RH/LH/TWOH weapon-damage blocks follow the original hand-visibility actions (Stats and Offense).
static bool has_weapon_blocks(const std::string& path){
    return path.find("menu_CharacterSheetStats/")!=std::string::npos||path.find("menu_CharacterSheetOffense/")!=std::string::npos;
}
bool original_stats_field(const std::string& path,const NativeMenuStats& stats,std::string& out){
    if(!is_stat_sheet_path(path))return false;
    struct Field{const char* name;std::int32_t value;};
    // Percent suffixes follow the authored UpdateCharInfos actions (0x13204..0x14004 etc.): Critical, Dodge, Block,
    // Melee/Spell to-hit, Spell crit and Leech carry "%"; ratings, armor, reduction, regen and bonuses do not.
    static const char* const percent[]={"Rating_Critical","Rating_Dodge","Rating_Block","Menu_Average_Melee_To_Hit",
        "Menu_Average_Spell_To_Hit","Spell_Rating_Critical","Leech_HP","Leech_MP"};
    const Field fields[]={{"Rating_Attack",stats.attack},{"Rating_Critical",stats.critical},
        {"Rating_Defense",stats.defense},{"Rating_Armor",stats.armor},{"Physical_Armor",stats.armor},
        {"Rating_Dodge",stats.dodge},{"Rating_Block",stats.block},{"Menu_Average_Melee_To_Hit",stats.melee_hit},
        {"Menu_Melee_Damage_Reduction",stats.melee_reduction},{"Menu_Average_Spell_To_Hit",stats.spell_hit},
        {"Spell_Rating_Critical",stats.spell_crit},
        {"Spell_Damage_Bonus_Fire",stats.spell_bonus[0]},{"Spell_Damage_Bonus_Water",stats.spell_bonus[1]},
        {"Spell_Damage_Bonus_Lightning",stats.spell_bonus[2]},{"Spell_Damage_Bonus_Earth",stats.spell_bonus[3]},
        {"Spell_Damage_Bonus_Air",stats.spell_bonus[4]},
        {"Regen_HP",stats.regen_hp},{"Regen_MP",stats.regen_mp},{"Leech_HP",stats.leech_hp},{"Leech_MP",stats.leech_mp},
        {"Resistance_Fire",stats.resistance[0]},{"Resistance_Water",stats.resistance[1]},
        {"Resistance_Lightning",stats.resistance[2]},{"Resistance_Earth",stats.resistance[3]},
        {"Resistance_Air",stats.resistance[4]}};
    for(const auto& field:fields)if(component(path,field.name)){
        out=std::to_string(field.value);
        for(const char* suffix:percent)if(std::strcmp(field.name,suffix)==0){out+="%";break;}
        return true;
    }
    if(component(path,"Damage_Min_Main_Hand")){out=std::to_string(stats.damage[0])+" - "+std::to_string(stats.damage[1]);return true;}
    if(component(path,"Damage_Min_Off_Hand")){out=std::to_string(stats.damage[2])+" - "+std::to_string(stats.damage[3]);return true;}
    return false;
}
bool original_stats_path_visible(const OriginalCombatProperties& p,const NativeMenuStats& stats,const std::string& path){
        if(!has_weapon_blocks(path))return true;
        if(component(path,"LH")&&!p.facts.dual_wield)return false;
        if(component(path,"TWOH")&&!p.facts.two_hander)return false;
        if(component(path,"RH")&&p.facts.two_hander)return false;
        const unsigned hand=component(path,"LH")?1:0;
        if((component(path,"RH")||component(path,"LH")||component(path,"TWOH"))&&stats.element_type[hand]<0&&
           (component(path,"ElementDamageType")||component(path,"Damage_Elemental_Min_Main_Hand")||
            component(path,"Damage_Elemental_Min_Off_Hand")||component(path,"RightHandDamageMagic")||
            component(path,"LeftHandDamageMagic")||component(path,"TwoHDamageMagic")))return false;
        return true;
}
void original_stats_visibility(const OriginalCombatProperties& p,const NativeMenuStats& stats,Frame& frame){
    const auto hidden=[&](const std::string& path){return !original_stats_path_visible(p,stats,path);};
    frame.art.batches.erase(std::remove_if(frame.art.batches.begin(),frame.art.batches.end(),[&](const auto& batch){return hidden(batch.role);}),frame.art.batches.end());
    frame.text.erase(std::remove_if(frame.text.begin(),frame.text.end(),[&](const auto& text){return hidden(text.field.path);}),frame.text.end());
    frame.solids.erase(std::remove_if(frame.solids.begin(),frame.solids.end(),[&](const auto& solid){return hidden(solid.geometry.role);}),frame.solids.end());
}
}
