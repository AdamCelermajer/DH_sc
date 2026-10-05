#include "character_menu_actions_owner_v1.hpp"
#include <stdexcept>
#include <cstring>
namespace dh2::ui {
CharacterMenuActionsOwnerV1::CharacterMenuActionsOwnerV1(CharacterMenuActionsGraphV1 graph):graph_(std::move(graph)){
    if(!graph_.owner)throw std::invalid_argument("source menu actions require retained graph/provider owner");
}
bool CharacterMenuActionsOwnerV1::equipment(std::string& e)const{
    e.clear();if(!graph_.equipment||!graph_.equipment->ready()||!graph_.equipment->inventory()){
        e="source menu requires the actual ready equipment graph";return false;}
    // Buff grouping belongs to the retained V3 producer and can relocate after
    // a skill update. Borrow its current projection afresh before gear/stat
    // recalculation; never retain a detached copy of Buff sheets.
    if(graph_.skills&&graph_.skills->ready()){
        if(graph_.skills->session().properties()!=graph_.equipment->properties()){
            e="menu equipment and V3 properties differ";return false;}
        auto& source=graph_.skills->session().property_view();auto* target=graph_.equipment->property_view();
        target->groups=source.groups;target->group_count=source.group_count;
    }
    return true;
}
bool CharacterMenuActionsOwnerV1::skills(std::string& e)const{
    if(!equipment(e))return false;
    auto* inventory=graph_.equipment->inventory();
    if(!graph_.skills||!graph_.skills->ready()||!graph_.save||
       graph_.save->character()!=inventory->character()||
       graph_.skills->session().timers().owner!=inventory->character()||
       graph_.skills->session().properties()!=graph_.equipment->properties()){
        e="source menu requires the same live V3 skills, save and equipment/property authority";return false;}
    if(!graph_.save_binding){e="source menu requires actual V3 native save binding verification";return false;}
    if(!graph_.save_binding(*graph_.save,*graph_.skills,e))return false;
    return true;
}
const data::SkillRecord* CharacterMenuActionsOwnerV1::skill(std::int32_t row,std::string& e)const{
    if(!skills(e))return nullptr;
    if(!graph_.skill_tables||!graph_.skill_list_index){e="source GetCharSkill requires actual retained class skill list";return nullptr;}
    auto list=*graph_.skill_list_index;
    const auto& lists=graph_.skill_tables.lists();const auto& records=graph_.skill_tables.skills();
    if(list<0||std::size_t(list)>=lists.size())list=3; // original GetSkillsID fallback
    if(std::size_t(list)>=lists.size()||row<0||std::size_t(row)>=lists[list].size()){
        e="source GetCharSkill assertion domain unsupported";return nullptr;}
    auto id=lists[list][row];if(id<0||std::size_t(id)>=records.size()){
        e="source class skill list references unavailable record";return nullptr;}
    return &records[id];
}
bool CharacterMenuActionsOwnerV1::equip(std::uint32_t slot,std::uint32_t item,std::string& e){return equipment(e)&&graph_.equipment->equip(slot,item,e);}
bool CharacterMenuActionsOwnerV1::unequip(std::uint32_t slot,std::string& e){return equipment(e)&&graph_.equipment->unequip(slot,e);}
bool CharacterMenuActionsOwnerV1::swap(std::string& e){
    if(!equipment(e)||!graph_.equipment->swap(e))return false;
    if(!graph_.swap_hud){e="source NativeSwapEquipment requires actual HUD AS callbacks";return false;}
    return graph_.swap_hud("DisplayRightHud",e)&&graph_.swap_hud("FillActionIcon",e);
}
bool CharacterMenuActionsOwnerV1::assign_stat(std::uint32_t stat,std::string& e){
    if(!equipment(e))return false;
    if(stat>3){e.clear();return true;} // original NativeStatsAssignPoint no-op dispatch
    if(graph_.stats.state!=graph_.equipment->properties().get()||graph_.stats.view!=graph_.equipment->property_view()){
        e="source menu stat action borrowed detached properties";return false;}
    return character_menu_assign_stat_v1(graph_.stats,stat,e);
}
bool CharacterMenuActionsOwnerV1::update_skills(void* c,std::uintptr_t owner,std::string& e){
    auto& self=*static_cast<CharacterMenuActionsOwnerV1*>(c);
    if(!self.skills(e)||owner!=self.graph_.skills->session().timers().owner)return false;
    if(self.graph_.skills->update()<0){e=self.graph_.skills->error();return false;}return true;
}
bool CharacterMenuActionsOwnerV1::equip_skill(std::int32_t slot,std::int32_t row,std::string& e){
    auto* record=skill(row,e);if(!record)return false;
    auto& state=*graph_.equipment->properties();
    // IsSkillAvailable → record SlotID != -1 → saved level > 0.
    if((state.resolved[19]>>8)<std::int32_t(record->scalar.words[8])||
       record->scalar.words[18]==UINT32_MAX||graph_.save->skill_level(std::uint32_t(row))<=0){e.clear();return true;}
    data::SavedSkillUpdateServicesV1 update{this,update_skills};
    return graph_.save->set_skill_in_slot(slot,std::uint32_t(row),update,e);
}
bool CharacterMenuActionsOwnerV1::train_skill(std::int32_t row,std::int32_t& points,std::string& e){
    if(!skills(e))return false;
    player::InitialGrantServices16V2 native{this,increment_service};const auto& provider=graph_.increment.invoke?graph_.increment:native;
    std::int32_t result{};
    auto rc=dh2_player_increment_skill_v2(&result,graph_.equipment->inventory()->character(),row,0,&provider);
    if(rc){e="source menu IncSkill required service failed";return false;}
    // Source ignores IncSkill's bool and rereads the live remaining points.
    return skill_points(points,e);
}
bool CharacterMenuActionsOwnerV1::can_increment(std::uint32_t row,bool& out,std::string& e)const{
    if(!skills(e))return false;
    if(!graph_.save->skills_initialized()||graph_.save->skills().empty()){out=false;return true;}
    if(row>=graph_.save->skills().size()){e="Source CanIncSkill saved row assertion domain unsupported";return false;}
    auto* record=skill(std::int32_t(row),e);if(!record)return false;
    auto raw=std::uint32_t(graph_.equipment->properties()->resolved[19]>>8)-record->scalar.words[8];std::int32_t difference;std::memcpy(&difference,&raw,4);
    out=graph_.save->skill_level(row)<=difference;return true;
}
int CharacterMenuActionsOwnerV1::increment_service(void* p,const player::InitialGrantRequest32V2* q,player::InitialGrantResponse8V2* r){
    auto& self=*static_cast<CharacterMenuActionsOwnerV1*>(p);auto& g=self.graph_;std::string e;*r={};
    if(!self.skills(e)||q->owner!=g.equipment->inventory()->character())return -1;
    using namespace player;auto row=std::uint32_t(q->arguments[0]);auto* view=g.equipment->property_view();
    switch(q->operation){
    case has_savegame:r->value=g.save!=nullptr;return 0;
    case has_saved_rows:r->value=g.save->skills_initialized()&&!g.save->skills().empty();return 0;
    case property_integer:if(row>=224)return -1;r->value=g.equipment->properties()->resolved[row]>>8;return 0;
    case skill_available:{auto* s=self.skill(std::int32_t(row),e);if(!s)return -1;r->value=(g.equipment->properties()->resolved[19]>>8)>=std::int32_t(s->scalar.words[8]);return 0;}
    case skill_limit:{const char* keys[]{"MaxSkillLevelBNormal","MaxSkillLevelCHard","MaxSkillLevelDVeryHard"};if(row>2)return -1;return g.skills->session().constant("CharacterDesign",keys[row],r->value);}
    case difficulty_unlocked:r->value=g.save->unlocked_difficulty();return 0;
    case saved_level_read:if(row>=g.save->skills().size())return -1;r->value=g.save->skill_level(row);return 0;
    case player::can_increment:{bool value;if(!self.can_increment(row,value,e))return -1;r->value=value;return 0;}
    case property_add:{auto raw=std::uint32_t(q->arguments[1])<<8;std::int32_t delta;std::memcpy(&delta,&raw,4);return dh2_property_add(view,std::int32_t(row),delta)?-1:0;}
    case saved_level_increment:if(row>=g.save->skills().size())return -1;return g.save->set_skill_level(row,g.save->skill_level(row)+1,e)?0:-1;
    case update_all_skills:return g.skills->update()<0?-1:0;
    case properties_recalculate:{if(!self.equipment(e)||!g.stats.classes||!g.stats.class_count)return -1;return dh2_class_recalc_base(g.stats.classes,g.stats.class_count,g.equipment->properties()->base.data(),view)?-1:0;}
    case potion_capacity_store:return g.potion_capacity_store&&g.potion_capacity_store(std::uint8_t(row),e)?0:-1;
    case debug_load:return g.stats.debug_load&&g.stats.debug_load(e)?0:-1;
    case debug_query:{bool value;return g.stats.debug_query&&g.stats.debug_query("isTracingChar_Stats",value,e)?0:-1;}
    default:return -1;
    }
}
bool CharacterMenuActionsOwnerV1::append_equipped_skills(const std::function<bool(std::int32_t,std::string&)>& append,std::string& e){
    if(!skills(e))return false;if(!append){e="source menu requires actual AS array append sink";return false;}
    for(std::int32_t slot=0;slot<3;++slot)if(!append(graph_.save->skill_in_slot(slot),e))return false;return true;
}
bool CharacterMenuActionsOwnerV1::skill_points(std::int32_t& out,std::string& e)const{
    if(!skills(e))return false;out=graph_.equipment->properties()->resolved[157]>>8;e.clear();return true;
}
}
