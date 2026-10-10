#include "equipment_adapter.hpp"
#include "../../../game-data/player_equipment_v3.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <set>

namespace dh::foundation {
namespace {
int service(void*,dh2::data::EquipmentState72V3*,const dh2::data::EquipmentRequest40V3* q,
            dh2::data::EquipmentResponse16V3* r) {
    // This adapter supports ordinary individually owned equipment. Stackable
    // equipment is rejected before source callbacks can merge/split ownership.
    if(q->operation==dh2::data::has_like) {r->value=0;return 0;}
    return -1;
}
}
bool equipment_meets_requirements(const dh2::data::Item& item,const OriginalActorProperties& p,bool bypass) noexcept {
    return equipment_meets_requirements(item,p.sheets,bypass);
}
bool equipment_meets_requirements(const dh2::data::Item& item,const dh2::data::PropertyState& p,bool bypass) noexcept {
    if(bypass)return true;
    // ItemInstance::IsEquippableBy (003fa330): level cell (Character+4164 = property 19) >= req<<8, and for each attribute
    // (base Stat_* 149..152) + (Prereq_* 153..156, Character+4700..) >= req<<8. Raw 8.8 compare; B057: Prereq_* was ignored.
    constexpr unsigned properties[]{19,149,150,151,152};
    for(unsigned i=0;i<5;++i){
        std::int64_t have=p.resolved[properties[i]];
        if(i>0)have+=p.resolved[properties[i]+4];
        if(have<(std::int64_t(item.record.words[29+i])<<8))return false;
    }
    return true;
}
namespace {
const char* const class_requirement_names[9]{"KnightPlayerBase","KnightPlayerBase_Berserker","KnightPlayerBase_Paladin",
    "RoguePlayerBase","RoguePlayerBase_Assassin","RoguePlayerBase_Archer","MagePlayerBase","MagePlayerBase_Necromancer","MagePlayerBase_Illusionist"};
// The player's CharacterTable row name when it is a source class row; empty for legacy ClassTables profiles.
std::string source_player_class(const ActorState& actor,const OriginalPropertyDatabase& database){
    const auto& names=database.characters.names;
    return std::find(names.begin(),names.end(),actor.definition_id)==names.end()?std::string():actor.definition_id;
}
}
bool equipment_class_allows(const dh2::data::Item& item,const std::string& player_class_id) noexcept {
    const auto required=item.record.words[34];
    if(required<1||required>9||player_class_id.empty())return true;
    return player_class_id==class_requirement_names[required-1];
}
bool equipment_equippable_by(const dh2::data::Item& item,const dh2::data::PropertyState& p,bool bypass,const std::string& player_class_id) noexcept {
    return bypass||(equipment_meets_requirements(item,p,false)&&equipment_class_allows(item,player_class_id));
}
std::int32_t equipment_sort_score(std::int32_t value,const dh2::data::Item& item,const std::string& player_class_id) noexcept {
    // SortByValueAndClass switches on the Character+5064 CharacterTable row (263 Knight, 264 Berserker, 265 Paladin,
    // 290 Mage, 291 Illusionist, 292 Necromancer, 325 Rogue, 326 Archer, 327 Assassin) and scales by item float +32/+48/
    // +44/+36/+64/+60/+40/+56/+52, i.e. record words 8/12/11/9/16/15/10/14/13.
    struct Entry{const char* name;unsigned word;};
    static constexpr Entry table[]{{"KnightPlayerBase",8},{"KnightPlayerBase_Berserker",12},{"KnightPlayerBase_Paladin",11},
        {"MagePlayerBase",9},{"MagePlayerBase_Illusionist",16},{"MagePlayerBase_Necromancer",15},
        {"RoguePlayerBase",10},{"RoguePlayerBase_Archer",14},{"RoguePlayerBase_Assassin",13}};
    for(const auto& entry:table)if(player_class_id==entry.name){
        float multiplier;std::memcpy(&multiplier,&item.record.words[entry.word],4);
        volatile float scaled=float(value)*multiplier;
        // ARM __aeabi_f2iz: NaN maps to 0, finite out-of-domain values saturate.
        if(std::isnan(scaled))return 0;if(scaled>=2147483648.f)return INT32_MAX;if(scaled<=-2147483648.f)return INT32_MIN;
        return std::int32_t(scaled);
    }
    return value;
}
EquipmentAdapter::EquipmentAdapter(CharacterState& c,ActorState& a,OriginalActorProperties& p,
 const dh2::data::ItemTable& i,const OriginalPropertyDatabase& d,EquipmentAdapterOptions o)
 :character_(c),actor_(a),properties_(&p),items_(i),database_(d),options_(std::move(o)){}
EquipmentAdapter::EquipmentAdapter(CharacterState& c,ActorState& a,const OriginalCombatProperties& p,
 const dh2::data::ItemTable& i,const OriginalPropertyDatabase& d,EquipmentAdapterOptions o)
 :character_(c),actor_(a),combat_(&p),items_(i),database_(d),options_(std::move(o)){}
bool EquipmentAdapter::equip(const std::string& id,std::string& e){return change(&id,9,e);}
bool EquipmentAdapter::auto_equip(const std::string& id,std::string& e){return change(&id,9,e);}
bool EquipmentAdapter::equip_to_slot(const std::string& id,unsigned slot,std::string& e){if(slot>=9){e="Invalid source equipment slot";return false;}return change(&id,slot,e);}
bool EquipmentAdapter::unequip(unsigned slot,std::string& e){if(slot>=9){e="Invalid source equipment slot";return false;}return change(nullptr,slot,e);}
bool EquipmentAdapter::refresh(std::string& e){return change(nullptr,9,e);}
bool EquipmentAdapter::auto_equip_slot(unsigned slot,std::string& e){if(slot>=9){e="Invalid source equipment slot";return false;}return change(nullptr,slot,e,int(slot));}
bool EquipmentAdapter::auto_equip_all(std::string& e){return change(nullptr,9,e,auto_all);}
bool EquipmentAdapter::change(const std::string* requested,unsigned remove,std::string& error,int auto_mode){
    error.clear();
    const auto fail=[&](const char* text){error=text;return false;};
    if(options_.slots.size()!=9||std::set<std::string>(options_.slots.begin(),options_.slots.end()).size()!=9)
        return fail("Source equipment requires nine distinct slot names");
    if(character_.inventory.size()>character_collection_limit)return fail("Inventory limit exceeded");
    if(combat_&&!options_.publish_combat)return fail("Shared combat equipment requires actual owner publication service");
    CharacterState next=character_; ActorState actor=actor_; OriginalActorProperties props;
    if(properties_)props=*properties_;else props.sheets=combat_->sheets;
    if(options_.prepare_canonical_properties&&!options_.prepare_canonical_properties(actor_,props.sheets,error))return false;
    using namespace dh2::data;
    std::vector<EquipmentRow12V3> rows; rows.reserve(items_.rows.size());
    for(const auto& row:items_.rows)rows.push_back({row.record.words[22],row.record.words[26],std::uint32_t(row.record.words[7])});
    std::vector<EquipmentItem16V3> owned(next.inventory.size());
    std::vector<EquipmentSlot16V3> slots(next.inventory.size());
    std::vector<EquipmentSlot16V3*> inventory(next.inventory.size());
    std::array<EquipmentSlot16V3*,9> active{},alternate{};
    std::set<std::string> ids;
    for(std::size_t i=0;i<next.inventory.size();++i){
        const auto& item=next.inventory[i]; const auto id=item_id(items_,item.definition_id);
        if(id<0||!item.quantity||!ids.insert(item.instance_id).second)return fail("Invalid owned item identity or definition");
        if(item.quantity>32767)return fail("Quantity exceeds source signed16 projection");
        owned[i]={id,std::int16_t(item.quantity),0,i+1}; slots[i]={&owned[i],{-1,-1},{}}; inventory[i]=&slots[i];
    }
    for(const auto& binding:next.equipment){
        const auto s=std::find(options_.slots.begin(),options_.slots.end(),binding.slot);
        const auto i=std::find_if(next.inventory.begin(),next.inventory.end(),[&](const auto& x){return x.instance_id==binding.item_instance_id;});
        if(s==options_.slots.end()||i==next.inventory.end())return fail("Equipment binding is not owned or has unknown slot");
        auto index=std::size_t(i-next.inventory.begin()),slot=std::size_t(s-options_.slots.begin());
        if((binding.equipment_set>=0&&binding.equipment_set!=0)||
           (binding.source_slot>=0&&std::size_t(binding.source_slot)!=slot))
            return fail("Equipment binding source set/slot metadata differs from the supported active source binding");
        if(active[slot]||slots[index].slots[0]!=-1)return fail("Duplicate equipment binding");
        if(owned[index].quantity!=1||rows[owned[index].id].stackable)return fail("Stack equipment ownership producer is unavailable");
        active[slot]=&slots[index];slots[index].slots[0]=std::int8_t(slot);
    }
    // Source property cells 202/203 drive the dual-wield and one-hand-two-hander policy of ItemInventory
    // (Character+4896/+4900 read by IsItemEquippable/_EquipItemToSlot, i.e. resolved[202]/[203]); the
    // adapter options can only add to them.
    const auto policy_flag=[&](bool option,std::size_t property){return std::uint32_t(option||props.sheets.resolved[property]!=0);};
    EquipmentState72V3 state{1,inventory.data(),std::uint32_t(inventory.size()),0,
        {active.data(),alternate.data()},9,policy_flag(options_.dual_wield,202),policy_flag(options_.one_hand_two_hander,203),0,
        rows.data(),std::uint32_t(rows.size()),0};
    EquipmentServices16V3 services{nullptr,service};
    PropertyRules rules;
    if(!load_property_rules(database_.characters,rules,error))return false;
    // Character::UpdateGearsProperties: rebuild the gear sheet from the currently equipped slots and recalculate.
    const auto reload_gear=[&]()->bool{
        if(dh2_gear_reset_v5(props.sheets.gear.data(),rules.defaults.data()))return fail("Source gear reset failed");
        for(unsigned slot=0;slot<9;++slot)if(active[slot]){
            auto index=std::size_t(active[slot]-slots.data());const auto& item=next.inventory[index];
            if(dh2_gear_stats_v5(props.sheets.gear.data(),rules.defaults.data(),&items_.rows[owned[index].id].record,slot==2))return fail("Source gear stat load failed");
            if(options_.powers){std::vector<GearPowerProperty12V5> powers;
                if(!options_.powers(item,powers,error))return false;
                GearPowerView16V5 view{powers.data(),std::uint32_t(powers.size()),0};
                if(dh2_gear_power_v5(props.sheets.gear.data(),rules.defaults.data(),&view,slot==2))return fail("Source gear power load failed");
            }
        }
        if(!recalc_properties_with_class(database_.classes,rules,props.sheets,error))return false;
        state.flag1320=policy_flag(options_.dual_wield,202);state.flag1324=policy_flag(options_.one_hand_two_hander,203);
        return true;
    };
    // Each Character::UnEquipItemFromSlot / EquipSlotAuto wrapper repeats UpdateGearsProperties then ValidateHPMP.
    const auto character_step=[&]()->bool{
        if(!reload_gear())return false;
        auto view=property_view(rules,props.sheets);
        if(dh2_gear_validate_vitals_v5(&view))return fail("Source gear vital validation failed");
        return true;
    };
    const std::string player_class=source_player_class(actor_,database_);
    // ItemInventory::_EquipSlotAuto over ItemInventory::GetItemListForSlot/IsItemEquippable.
    const auto equip_best=[&](unsigned slot)->bool{
        struct Candidate{std::size_t index;std::int32_t score;std::size_t powers;std::string name;};
        std::vector<Candidate> list;
        for(std::size_t i=0;i<next.inventory.size();++i){
            const auto& item=items_.rows[owned[i].id];
            auto target=item.record.words[26];
            if(target==-1)continue;
            if(item.record.words[22]!=4&&item.record.words[22]!=5&&target==1&&props.sheets.resolved[202])target=-3;
            const bool offered=target>=0&&target<9?unsigned(target)==slot:target==-3?(slot==1||slot==2):
                target==-2?(slot==5||slot==6):(target==-4&&slot==1);
            if(!offered)continue;
            Candidate c{i,0,0,next.inventory[i].definition_id};
            // Bare-item ItemInstance value: ARM32 wrapping product of the two actual metadata words (same projection as
            // source_bare_item_descriptors); powered-item values are not persisted by CharacterState.
            c.score=equipment_sort_score(std::int32_t(std::uint32_t(item.record.words[27])*std::uint32_t(item.record.words[28])),item,player_class);
            if(options_.powers){std::vector<GearPowerProperty12V5> powers;if(!options_.powers(next.inventory[i],powers,error))return false;c.powers=powers.size();}
            if(options_.item_name&&!options_.item_name(next.inventory[i],item,c.name,error))return false;
            list.push_back(std::move(c));
        }
        // II_Item_sortbyname/ItemInstance::operator<: more powers first, then strcmp of the name. std::sort is
        // unstable in the original; stable_sort only decides between indistinguishable items.
        std::stable_sort(list.begin(),list.end(),[](const Candidate& a,const Candidate& b){
            if(a.score!=b.score)return a.score>b.score;
            if(a.powers!=b.powers)return a.powers>b.powers;
            return std::strcmp(a.name.c_str(),b.name.c_str())<0;});
        for(const auto& c:list){
            if(!equipment_equippable_by(items_.rows[owned[c.index].id],props.sheets,options_.online_requirements_bypass,player_class))continue;
            if(slots[c.index].slots[0]!=-1||slots[c.index].slots[1]!=-1)continue;
            if(next.inventory[c.index].quantity!=1||rows[owned[c.index].id].stackable)return fail("Stack equipment ownership producer is unavailable");
            if(dh2_equipment_to_slot_v3(&state,slot,std::uint32_t(c.index),0,&services))return fail("Source automatic slot equip failed");
            return true;
        }
        return true;
    };
    const auto unequip_slot=[&](unsigned slot)->bool{
        if(dh2_equipment_from_slot_v3(&state,slot,-1,&services))return fail("Source unequip failed");
        return character_step();
    };
    const auto auto_slot=[&](unsigned slot)->bool{return equip_best(slot)&&character_step();};
    if(requested){
        auto found=std::find_if(next.inventory.begin(),next.inventory.end(),[&](const auto& x){return x.instance_id==*requested;});
        if(found==next.inventory.end())return fail("Requested item is not owned");
        auto index=std::size_t(found-next.inventory.begin());
        if(found->quantity!=1||rows[owned[index].id].stackable)return fail("Stack equipment ownership producer is unavailable");
        if(!equipment_equippable_by(items_.rows[owned[index].id],props.sheets,options_.online_requirements_bypass,player_class))return fail("Original item requirements are not met");
        if(remove<9){
            if(dh2_equipment_to_slot_v3(&state,remove,std::uint32_t(index),0,&services))return fail("Source selected-slot equip failed");
        }else{
            std::int32_t result=0;
            if(dh2_equipment_auto_v3(&result,&state,std::uint32_t(index),&services)||!result)return fail("Source automatic equip did not accept item");
        }
    }else if(auto_mode>=0){
        if(!unequip_slot(unsigned(auto_mode))||!auto_slot(unsigned(auto_mode)))return false;
    }else if(auto_mode==auto_all){
        for(unsigned slot=0;slot<9;++slot)if(!unequip_slot(slot))return false;
        for(unsigned slot=9;slot>0;--slot)if(!auto_slot(slot-1))return false;
        for(unsigned slot=9;slot>0;--slot)if(!active[slot-1]&&slot-1!=1&&slot-1!=2&&!auto_slot(slot-1))return false;
    }else if(remove<9){if(dh2_equipment_from_slot_v3(&state,remove,-1,&services))return fail("Source unequip failed");}
    else if(remove!=9)return fail("Invalid source equipment slot");
    next.equipment.clear();actor.equipment.clear();
    for(unsigned slot=0;slot<9;++slot)if(active[slot]){
        auto index=std::size_t(active[slot]-slots.data());const auto& item=next.inventory[index];
        next.equipment.push_back({options_.slots[slot],item.instance_id,0,static_cast<std::int32_t>(slot)});
        actor.equipment.push_back({options_.slots[slot],item.definition_id,item.instance_id});
    }
    if(!character_step())return false;
    const auto& v=props.sheets.resolved;
    // The signed source cells retain unavailable/negative sentinels. Runtime
    // CharacterState/ActorState vitals are semantic values and follow the
    // animation-only projection: clamp those values without rewriting sheets.
    props.health=actor.health=next.stats.health=std::max(0.0f,original_signed256(v[36]));
    props.max_health=actor.max_health=next.stats.max_health=std::max(0.0f,original_signed256(v[38]));
    props.resource=actor.resource=next.stats.resource=std::max(0.0f,original_signed256(v[41]));
    props.max_resource=actor.max_resource=next.stats.max_resource=std::max(0.0f,original_signed256(v[43]));
    props.walk_multiplier=original_speed_modifier(v[46]);props.rotation_multiplier=original_speed_modifier(v[47]);
    props.turn_radians_per_second=12.566370964050293f*props.rotation_multiplier;
    next.stats.strength=original_signed256(v[149]);next.stats.dexterity=original_signed256(v[150]);
    EquipmentAttachmentSet attachments;
    if(options_.visuals){
        if(!options_.assets||!options_.body||!options_.attachments)return fail("Visual refresh requires assets, body and attachment owner");
        std::vector<EquipmentVisualDefinition> definitions;
        if(!options_.visuals(next,actor,definitions,error)||!attachments.load(*options_.assets,definitions,error)||
           !attachments.update(*options_.body,error))return false;
    }
    if(combat_){
        OriginalCombatProperties candidate=*combat_;candidate.sheets=props.sheets;
        const ItemRecord164* main=nullptr;const ItemRecord164* off=nullptr;
        if(active[1])main=&items_.rows[active[1]->item->id].record;
        if(active[2])off=&items_.rows[active[2]->item->id].record;
        if(!original_combat_equipment_facts(main,off,candidate.facts,error)||
           !options_.publish_combat(actor,candidate,error))return false;
    }
    character_=std::move(next);actor_=std::move(actor);if(properties_)*properties_=std::move(props);
    if(options_.visuals)*options_.attachments=std::move(attachments);
    return true;
}
}
