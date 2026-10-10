#include "equipment_adapter.hpp"
#include "../../../game-data/player_equipment_v3.hpp"
#include <algorithm>
#include <array>
#include <set>

namespace dh::foundation {
namespace {
// ARM ASR #8, including negative fractional values.
std::int32_t integer(std::int32_t raw) { return raw >= 0 ? raw/256 : -1 - (-(std::int64_t(raw)+1))/256; }
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
    constexpr unsigned properties[]{19,149,150,151,152};
    for(unsigned i=0;i<5;++i)if(item.record.words[29+i]>integer(p.resolved[properties[i]]))return false;
    return true;
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
bool EquipmentAdapter::change(const std::string* requested,unsigned remove,std::string& error){
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
    EquipmentState72V3 state{1,inventory.data(),std::uint32_t(inventory.size()),0,
        {active.data(),alternate.data()},9,std::uint32_t(options_.dual_wield),std::uint32_t(options_.one_hand_two_hander),0,
        rows.data(),std::uint32_t(rows.size()),0};
    EquipmentServices16V3 services{nullptr,service};
    if(requested){
        auto found=std::find_if(next.inventory.begin(),next.inventory.end(),[&](const auto& x){return x.instance_id==*requested;});
        if(found==next.inventory.end())return fail("Requested item is not owned");
        auto index=std::size_t(found-next.inventory.begin());
        if(found->quantity!=1||rows[owned[index].id].stackable)return fail("Stack equipment ownership producer is unavailable");
        if(!equipment_meets_requirements(items_.rows[owned[index].id],props,options_.online_requirements_bypass))return fail("Original item requirements are not met");
        if(remove<9){
            if(dh2_equipment_to_slot_v3(&state,remove,std::uint32_t(index),0,&services))return fail("Source selected-slot equip failed");
        }else{
            std::int32_t result=0;
            if(dh2_equipment_auto_v3(&result,&state,std::uint32_t(index),&services)||!result)return fail("Source automatic equip did not accept item");
        }
    }else if(remove<9){if(dh2_equipment_from_slot_v3(&state,remove,-1,&services))return fail("Source unequip failed");}
    else if(remove!=9)return fail("Invalid source equipment slot");
    next.equipment.clear();actor.equipment.clear();
    PropertyRules rules;
    if(!load_property_rules(database_.characters,rules,error))return false;
    if(dh2_gear_reset_v5(props.sheets.gear.data(),rules.defaults.data()))return fail("Source gear reset failed");
    for(unsigned slot=0;slot<9;++slot)if(active[slot]){
        auto index=std::size_t(active[slot]-slots.data());const auto& item=next.inventory[index];
        next.equipment.push_back({options_.slots[slot],item.instance_id,0,static_cast<std::int32_t>(slot)});
        actor.equipment.push_back({options_.slots[slot],item.definition_id,item.instance_id});
        if(dh2_gear_stats_v5(props.sheets.gear.data(),rules.defaults.data(),&items_.rows[owned[index].id].record,slot==2))return fail("Source gear stat load failed");
        if(options_.powers){std::vector<GearPowerProperty12V5> powers;
            if(!options_.powers(item,powers,error))return false;
            GearPowerView16V5 view{powers.data(),std::uint32_t(powers.size()),0};
            if(dh2_gear_power_v5(props.sheets.gear.data(),rules.defaults.data(),&view,slot==2))return fail("Source gear power load failed");
        }
    }
    if(!recalc_properties_with_class(database_.classes,rules,props.sheets,error))return false;
    auto view=property_view(rules,props.sheets);
    if(dh2_gear_validate_vitals_v5(&view))return fail("Source gear vital validation failed");
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
