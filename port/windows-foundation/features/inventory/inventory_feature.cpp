#include "inventory_feature.hpp"
#include <algorithm>
#include <limits>

namespace dh::foundation::inventory {
namespace {
bool equipped(const CharacterState& owner,const std::string& id) {
    return std::any_of(owner.equipment.begin(),owner.equipment.end(),[&](const auto& x){return x.item_instance_id==id;});
}
bool valid(const CharacterState& owner,std::string& error) {
    auto result=validate_character_state(owner);
    if(!result.ok()){error=result.errors.front();return false;}
    return true;
}
const dh2::data::Item* metadata(const dh2::data::ItemTable& table,const std::string& id,std::string& error) {
    auto* row=dh2::data::item(table,dh2::data::item_id(table,id));
    if(!row)error="Actual ItemTable definition unavailable: "+id;
    return row;
}
}
bool Presenter::select(const std::string& id,std::string& error) {
    if(!id.empty()&&std::none_of(owner_.inventory.begin(),owner_.inventory.end(),[&](const auto& x){return x.instance_id==id;})){
        error="Inventory selection identity unavailable";return false;
    }
    selected_=id;error.clear();return true;
}
bool Presenter::present(View& output,std::string& error) {
    if(!valid(owner_,error))return false;
    if(sort_==Sort::name){error="Required original localized ItemInstance name sort unavailable";return false;}
    View next;next.gold=owner_.gold;
    bool selection_found=false;
    for(const auto& entry:owner_.inventory){
        const auto* data=metadata(table_,entry.definition_id,error);if(!data)return false;
        const bool selected=entry.instance_id==selected_;selection_found|=selected;
        next.rows.push_back({entry.instance_id,entry.definition_id,data->name,data->icon_name,entry.quantity,
                             dh2::data::item_type(*data),data->record.words[7]!=0,equipped(owner_,entry.instance_id),selected,data->record.words[17]});
    }
    // Presentation sorting never reorders saved storage or equipment indices.
    if(sort_!=Sort::owned_order)std::stable_sort(next.rows.begin(),next.rows.end(),[&](const Row& a,const Row& b){
        switch(sort_){case Sort::definition:return a.definition_id<b.definition_id;
        case Sort::name:return a.name_key<b.name_key;case Sort::type:return a.type<b.type;
        default:return false;}
    });
    if(!selection_found)selected_.clear();output=std::move(next);error.clear();return true;
}
bool Presenter::pickup(const InventoryItem& incoming,std::string& retained,std::string& error) {
    if(!valid(owner_,error))return false;
    const auto* data=metadata(table_,incoming.definition_id,error);if(!data)return false;
    if(dh2::data::item_type(*data)==13){error="Required original gold-loot valuation unavailable";return false;}
    if(incoming.quantity==0){error="Pickup quantity must be positive";return false;}
    if(std::any_of(owner_.inventory.begin(),owner_.inventory.end(),[&](const auto& x){return x.instance_id==incoming.instance_id;})){
        error="Pickup instance already belongs to inventory";return false;
    }
    auto proposed=owner_;std::string chosen=incoming.instance_id;
    auto target=proposed.inventory.end();
    if(data->record.words[7])target=std::find_if(proposed.inventory.begin(),proposed.inventory.end(),[&](const auto& x){return x.definition_id==incoming.definition_id;});
    if(target!=proposed.inventory.end()){
        if(incoming.quantity>std::numeric_limits<std::uint32_t>::max()-target->quantity){error="Foundation stack quantity overflow";return false;}
        // Validate incoming identity even when merged; never silently accept malformed IDs.
        auto identity_check=make_default_character();identity_check.inventory.push_back(incoming);
        if(!valid(identity_check,error))return false;
        target->quantity+=incoming.quantity;chosen=target->instance_id;
    }else proposed.inventory.push_back(incoming);
    if(!valid(proposed,error))return false;
    owner_.inventory.swap(proposed.inventory);retained=std::move(chosen);error.clear();return true;
}
bool Presenter::drop(const std::string& id,std::uint32_t quantity,InventoryItem& removed,std::string& error) {
    if(!valid(owner_,error))return false;
    const auto found=std::find_if(owner_.inventory.begin(),owner_.inventory.end(),[&](const auto& x){return x.instance_id==id;});
    if(found==owner_.inventory.end()||quantity==0||quantity>found->quantity){error="Invalid drop identity or quantity";return false;}
    if(!metadata(table_,found->definition_id,error))return false;
    if(equipped(owner_,id)){error="Required equipment un/equip service before drop";return false;}
    auto result=*found;result.quantity=quantity;
    if(quantity==found->quantity){owner_.inventory.erase(found);if(selected_==id)selected_.clear();}
    else found->quantity-=quantity;
    removed=std::move(result);error.clear();return true;
}
bool Presenter::credit_gold(std::uint64_t amount,std::string& error){
    if(amount>std::numeric_limits<std::uint64_t>::max()-owner_.gold){error="Foundation currency overflow";return false;}
    owner_.gold+=amount;error.clear();return true;
}
bool Presenter::debit_gold(std::uint64_t amount,std::string& error){
    if(amount>owner_.gold){error="Insufficient currency";return false;}owner_.gold-=amount;error.clear();return true;
}
}
