#include "source_metadata_owner_v1.hpp"
#include <algorithm>
#include <limits>

namespace dh::foundation::inventory {
namespace {
bool same(const InventoryItem& a,const InventoryItem& b){
    return a.instance_id==b.instance_id&&a.definition_id==b.definition_id&&a.quantity==b.quantity&&
      a.source&&b.source&&a.source->value==b.source->value&&a.source->identified==b.source->identified&&
      a.source->power_names==b.source->power_names;
}
struct Node {
    InventoryItem row;
    dh2::data::ItemInstanceV1 item;
    std::shared_ptr<dh2::data::ItemPresentationOwnerV5> presentation;
    std::shared_ptr<const void> resources;
    ~Node(){if(presentation){std::string ignored;presentation->forget(item,ignored);}}
};
}
struct SourceMetadataOwnerV1::State {
    const dh2::data::ItemTable* table{};
    dh2::data::ItemPowerTablesV5::Borrow definitions;
    std::shared_ptr<dh2::data::ItemPresentationOwnerV5> presentation;
    std::shared_ptr<const void> resources;
    dh2::ui::ItemTextOwnerV5* text{};
    RowBorrow borrow;
    std::map<std::string,std::shared_ptr<Node>> nodes;
    std::uint64_t generation{};
    bool active{true};
};
SourceMetadataOwnerV1::SourceMetadataOwnerV1(const dh2::data::ItemTable& table,
 dh2::data::ItemPowerTablesV5::Borrow definitions,
 std::shared_ptr<dh2::data::ItemPresentationOwnerV5> presentation,
 std::shared_ptr<const void> resources,dh2::ui::ItemTextOwnerV5& text,RowBorrow borrow)
 :state_(std::make_shared<State>()){
    state_->table=&table;state_->definitions=std::move(definitions);
    state_->presentation=std::move(presentation);state_->resources=std::move(resources);
    state_->text=&text;state_->borrow=std::move(borrow);
}
SourceMetadataOwnerV1::~SourceMetadataOwnerV1(){invalidate();}
void SourceMetadataOwnerV1::invalidate()noexcept{
    state_->active=false;++state_->generation;state_->nodes.clear();
}
bool SourceMetadataOwnerV1::resolve(const InventoryItem& row,SourceInstanceLease& out,std::string& error){
    auto s=state_;
    if(!s->active||!s->definitions||!s->presentation||!s->resources||!s->text||!s->borrow){error="Canonical saved-item resources/row owner unavailable";return false;}
    const auto actual_definitions=s->presentation->definitions();
    if(!actual_definitions||&actual_definitions.rows()!=&s->definitions.rows()||&actual_definitions.names()!=&s->definitions.names()){
        error="Canonical presentation and power-property definitions have different source owners";return false;
    }
    InventoryItem current;
    if(!s->borrow(row.instance_id,current,error)||!same(row,current)){error="Canonical saved-item row expired or metadata absent";return false;}
    if(row.instance_id.empty()||!row.quantity||row.quantity>std::uint32_t(INT16_MAX)||row.source->identified>1||row.source->power_names.size()>65536){error="Saved native item fields outside source domain";return false;}
    const auto id=dh2::data::item_id(*s->table,row.definition_id);
    if(id<0||!dh2::data::item(*s->table,id)){error="Saved native ItemTable identity unavailable";return false;}
    std::shared_ptr<Node> node;
    const auto found=s->nodes.find(row.instance_id);
    if(found!=s->nodes.end()&&same(found->second->row,row))node=found->second;
    else {
        node=std::make_shared<Node>();node->presentation=s->presentation;node->resources=s->resources;
        node->row=row;node->item.id=id;node->item.quantity=std::uint16_t(row.quantity);
        node->item.value=row.source->value;
        const auto text=s->text->services();
        if(!text.metadata||text.metadata(text.context,node->item,error)!=dh2::data::item(*s->table,id)){
            error="Canonical ItemText and saved-item ItemTable have different source owners";return false;
        }
        if(!dh2::data::item_update_name_v5(node->item,text,error)||
           !dh2::data::item_update_stats_v5(node->item,text,error)||
           !dh2::data::item_update_requirements_v5(node->item,text,error))return false;
        const auto& names=s->definitions.names();
        std::vector<std::int32_t> saved_ids;
        for(const auto& name:row.source->power_names){
            const auto p=std::find(names.begin(),names.end(),name);
            if(p==names.end()){error="Saved ItemPower name absent from canonical definitions";return false;}
            if(!s->presentation->add_power(node->item,std::int32_t(p-names.begin()),0,text,error))return false;
            saved_ids.push_back(std::int32_t(p-names.begin()));
        }
        if(!s->presentation->restore_saved_power_order(node->item,saved_ids,error))return false;
        node->item.identified=row.source->identified;
        // Preserve exact authenticated saved order: repeated source AddPower
        // sorting is not idempotent for every multi-power generated sequence.
        for(std::size_t i=0;i<node->item.powers.size();++i)
            if(names[std::size_t(node->item.powers[i])]!=row.source->power_names[i]){error="Saved ItemPower order differs from recovered AddPower result";return false;}
        s->nodes[row.instance_id]=node;
    }
    SourceInstanceLease next;next.owner=node;next.item=&node->item;
    next.powers=s->presentation.get();next.power_definitions=s->definitions;
    next.text_owner=s->text;next.descriptors_current=true;
    const auto generation=s->generation;
    next.owns=[s,node,generation](const dh2::data::ItemInstanceV1* p){
        if(!s->active||s->generation!=generation||p!=&node->item)return false;
        const auto f=s->nodes.find(node->row.instance_id);
        if(f==s->nodes.end()||f->second.get()!=node.get())return false;
        InventoryItem live;std::string ignored;
        return s->borrow(node->row.instance_id,live,ignored)&&same(node->row,live);
    };
    out=std::move(next);error.clear();return true;
}
bool SourceMetadataOwnerV1::properties(const InventoryItem& row,
 std::vector<dh2::data::GearPowerProperty12V5>& out,std::string& error){
    SourceInstanceLease lease;if(!resolve(row,lease,error))return false;
    std::vector<dh2::data::GearPowerProperty12V5> next;
    for(const auto id:lease.item->powers){
        if(id<0||std::size_t(id)>=lease.power_definitions.rows().size()){error="Canonical ItemPower property identity invalid";return false;}
        const auto& properties=lease.power_definitions.rows()[std::size_t(id)].properties;
        next.insert(next.end(),properties.begin(),properties.end());
    }
    if(!lease.owns(lease.item)){error="Canonical item expired while projecting effects";return false;}
    out=std::move(next);error.clear();return true;
}
bool SourceMetadataOwnerV1::rank(const InventoryItem& row,std::int32_t& value,std::uint32_t& count,std::string& error){
    SourceInstanceLease lease;if(!resolve(row,lease,error))return false;
    if(!lease.owns(lease.item)){error="Canonical item expired while resolving auto-equip rank";return false;}
    value=lease.item->value;count=std::uint32_t(lease.item->powers.size());error.clear();return true;
}
}
