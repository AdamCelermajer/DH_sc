#include "source_item_descriptors.hpp"
#include <cstring>
namespace dh::foundation::inventory {
bool source_bare_item_descriptors(const InventoryItem& owned,const dh2::data::ItemTable& table,
 const dh2::data::ItemTextServicesV5& upstream,SourceDescriptors& output,std::string& error){
 const auto id=dh2::data::item_id(table,owned.definition_id);const auto* record=dh2::data::item(table,id);
 if(!record){error="Original bare item metadata absent";return false;}
 if(!upstream.invoke){error="Required shared source ItemTextServicesV5 StringManager/formatter unavailable";return false;}
 if(owned.quantity==0||owned.quantity>32767){error="Bare text quantity outside original positive signed16 domain";return false;}
 if(dh2::data::item_type(*record)==13){error="Gold item descriptors require actual generated source valuation";return false;}
 struct Context{const dh2::data::ItemTable& table;const dh2::data::ItemTextServicesV5& upstream;}ctx{table,upstream};
 dh2::data::ItemTextServicesV5 services{&ctx,
  [](void* p,const dh2::data::ItemInstanceV1& item,std::string& error){const auto* record=dh2::data::item(static_cast<Context*>(p)->table,item.id);if(!record)error="Original descriptor metadata became invalid";return record;},
  [](void* p,dh2::data::ItemInstanceV1& item,const dh2::data::ItemTextRequestV5& request,dh2::data::ItemTextResponseV5& response,std::string& output,std::string& error){const auto& u=static_cast<Context*>(p)->upstream;return u.invoke(u.context,item,request,response,output,error);}};
 dh2::data::ItemInstanceV1 projection;projection.id=id;projection.quantity=std::uint16_t(owned.quantity);
 // Source unpowered _CreateItem/SetValue: ARM32 wrapping product of the two
 // actual original metadata words, never a guessed price or generated power.
 const auto value=std::uint32_t(record->record.words[27])*std::uint32_t(record->record.words[28]);std::memcpy(&projection.value,&value,4);
 if(!dh2::data::item_update_name_v5(projection,services,error)||!dh2::data::item_update_stats_v5(projection,services,error)||!dh2::data::item_update_requirements_v5(projection,services,error))return false;
 output={std::move(projection.name),std::move(projection.description),std::move(projection.requirements)};error.clear();return true;
}
bool source_owned_item_descriptor(const dh2::data::ItemInstanceV1& item,const dh2::data::ItemPresentationOwnerV5* powers,const std::string& field,std::string& output,std::string& error){
 std::string result;
 if(field.find("ItemInfo1")!=std::string::npos)result=item.description;
 else if(field.find("ItemReq")!=std::string::npos||field.find("EquipedItemReq")!=std::string::npos)result=item.requirements;
 else {for(unsigned i=0;i<4;++i){const auto key="ItemInfo"+std::to_string(i+2);if(field.find(key)==std::string::npos)continue;
   if(i<item.powers.size()){if(!powers){error="Powered item requires same original ItemPresentationOwnerV5";return false;}const auto* values=powers->powers(item);if(!values||values->size()!=item.powers.size()){error="Source powered item presentation owner is not synchronized";return false;}if((*values)[i].id!=item.powers[i]){error="Source powered item presentation identity mismatch";return false;}result=(*values)[i].description;}
   break;
  }}
 output=std::move(result);error.clear();return true;
}
bool SourceInstanceDescriptorProvider::present(const InventoryItem& owned,SourceOwnedDescriptors& output,std::string& error)const{
    if(!resolve_){error="Canonical source ItemInstance identity resolver unavailable";return false;}
    SourceInstanceLease lease;if(!resolve_(owned.instance_id,lease,error))return false;
    if(!lease.owner||!lease.item||!lease.owns||!lease.owns(lease.item)||!lease.text_owner||!lease.descriptors_current){error="Required same-owner current ItemInstance and ItemTextOwner lease unavailable";return false;}
    std::int16_t quantity;std::memcpy(&quantity,&lease.item->quantity,2); // Original LDRSH GetQty.
    const auto id=dh2::data::item_id(table_,owned.definition_id);if(id<0||lease.item->id!=id||quantity<0||std::uint32_t(quantity)!=owned.quantity){error="Canonical source item identity/quantity differs from selected owned item";return false;}
    // Nullopt is the legacy/known-bare representation. A native power list
    // without its persisted metadata is stale projection, never a bare ring.
    if(!owned.source&&!lease.item->powers.empty()){
        error="Canonical source item has powers but persistent generated-item metadata is absent";return false;
    }
    if(owned.source){
        if(!lease.power_definitions||lease.item->value!=owned.source->value||
           lease.item->identified!=owned.source->identified){
            error="Canonical source item value/identified metadata differs from persistent inventory or current power table unavailable";return false;
        }
        const auto& names=lease.power_definitions.names();
        std::vector<std::string> actual_names;actual_names.reserve(lease.item->powers.size());
        const auto* owned_powers=lease.powers?lease.powers->powers(*lease.item):nullptr;
        if(!lease.item->powers.empty()&&(!owned_powers||owned_powers->size()!=lease.item->powers.size())){
            error="Canonical ordered powers have no same-owner ItemPresentation lease";return false;
        }
        for(std::size_t i=0;i<lease.item->powers.size();++i){
            const auto power_id=lease.item->powers[i];
            if(power_id<0||static_cast<std::size_t>(power_id)>=names.size()||
               names[static_cast<std::size_t>(power_id)].empty()||
               (owned_powers&&(*owned_powers)[i].id!=power_id)){
                error="Canonical ordered ItemPower ID is stale or detached from current table";return false;
            }
            actual_names.push_back(names[static_cast<std::size_t>(power_id)]);
        }
        if(actual_names!=owned.source->power_names){
            error="Canonical source item ordered power names differ from persistent inventory";return false;
        }
    }
    SourceOwnedDescriptors next;next.base={lease.item->name,lease.item->description,lease.item->requirements};next.quantity=quantity;next.value=lease.item->value;next.identified=lease.item->identified!=0;
    if(!lease.item->powers.empty()){
        if(!lease.powers){error="Canonical powered ItemPresentationOwnerV5 unavailable";return false;}const auto* powers=lease.powers->powers(*lease.item);
        if(!powers||powers->size()!=lease.item->powers.size()){error="Canonical source power descriptors detached from actual instance";return false;}
        for(std::size_t i=0;i<powers->size();++i){const auto id=lease.item->powers[i];if((*powers)[i].id!=id||id<0||!lease.power_definitions||static_cast<std::size_t>(id)>=lease.power_definitions.names().size()||lease.power_definitions.names()[static_cast<std::size_t>(id)].empty()){error="Canonical source power identity/name mismatch";return false;}next.powers.push_back((*powers)[i].description);}
    }
    output=std::move(next);error.clear();return true;
}
bool SourceInstanceDescriptorProvider::name(const InventoryItem& owned,std::string& output,std::string& error)const{SourceOwnedDescriptors value;if(!present(owned,value,error))return false;output=std::move(value.base.name);return true;}
bool SourceInstanceDescriptorProvider::field(const InventoryItem& owned,const std::string& path,std::string& output,std::string& error)const{
    SourceOwnedDescriptors value;if(!present(owned,value,error))return false;std::string result;
    if(path.find("ItemInfo1")!=std::string::npos)result=value.base.stats;
    else if(path.find("ItemReq")!=std::string::npos||path.find("EquipedItemReq")!=std::string::npos)result=value.base.requirements;
    else for(unsigned i=0;i<4;++i)if(path.find("ItemInfo"+std::to_string(i+2))!=std::string::npos){if(i<value.powers.size())result=value.powers[i];break;}
    output=std::move(result);error.clear();return true;
}
}
