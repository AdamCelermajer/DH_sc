#include "runtime_item_resources_v1.hpp"

namespace dh::foundation::inventory {
std::unique_ptr<SourceMetadataOwnerV1> RuntimeItemResourcesV1::inventory(SourceMetadataOwnerV1::RowBorrow rows){
    return std::make_unique<SourceMetadataOwnerV1>(loot.items(),definitions,presentation,
        shared_from_this(),*text,std::move(rows));
}
bool bind_runtime_item_resources_v1(dh2::data::LootTablesV2::Borrow loot,
 dh2::data::ItemPowerTablesV5::Borrow definitions,
 dh2::data::LootPowerResourcesV7::Borrow power_resources,
 std::shared_ptr<const OriginalPropertyDatabase> properties,
 std::shared_ptr<dh2::ui::ItemTextOwnerV5> existing_text,
 std::shared_ptr<const void> localization_pin,
 std::shared_ptr<RuntimeItemResourcesV1>& out,std::string& error){
    if(!loot||!definitions||!power_resources||!properties||!existing_text||!localization_pin||loot.items().rows.empty()){
        error="Existing runtime Loot/Power/properties/Text/localization ownership unavailable";return false;
    }
    if(!power_resources.powers()||&power_resources.powers().rows()!=&definitions.rows()){
        error="Runtime loot power resources use a different ItemPower snapshot";return false;
    }
    dh2::data::ItemInstanceV1 probe;probe.id=0;const auto services=existing_text->services();
    if(!services.metadata||services.metadata(services.context,probe,error)!=&loot.items().rows[0]){
        error="Existing runtime ItemText uses a different ItemTable snapshot";return false;
    }
    if(out){
        if(&out->loot.items()!=&loot.items()||&out->definitions.rows()!=&definitions.rows()||
           &out->power_resources.lists()!=&power_resources.lists()||
           &out->power_resources.quantities()!=&power_resources.quantities()||out->properties.get()!=properties.get()||
           out->text.get()!=existing_text.get()||out->localization_pin.get()!=localization_pin.get()){
            error="Invalidate player/shop caches before replacing runtime item resources";return false;
        }
        error.clear();return true;
    }
    auto next=std::make_shared<RuntimeItemResourcesV1>();next->loot=std::move(loot);
    next->definitions=std::move(definitions);next->power_resources=std::move(power_resources);
    next->properties=std::move(properties);next->text=std::move(existing_text);
    next->localization_pin=std::move(localization_pin);
    next->presentation=std::make_shared<dh2::data::ItemPresentationOwnerV5>(next->definitions);
    out=std::move(next);error.clear();return true;
}
dh2::data::LootCreationServicesV8 RuntimeItemLootCreationV1::services(){
    if(!resources_)return {};
    return {entry_,{this,power},resources_->text->services(),this,query,nullptr,nullptr};
}
bool RuntimeItemLootCreationV1::power(void* raw,const dh2::data::LootPowerRequestV7& q,std::int32_t& value,std::string& error){
    auto& self=*static_cast<RuntimeItemLootCreationV1*>(raw);
    if(!self.resources_){error="Retired runtime item power resources";return false;}
    using Op=dh2::data::LootPowerOperationV7;
    if(q.operation==Op::add_power){
        if(!q.item){error="Source AddPower item receiver missing";return false;}
        return self.resources_->presentation->add_power(*q.item,q.power,q.difficulty,self.resources_->text->services(),error);
    }
    if(!self.entry_.invoke){error="Existing same-session loot Debug provider unavailable";return false;}
    const auto operation=q.operation==Op::debug_load?dh2::data::LootEntryOperationV8::debug_load:dh2::data::LootEntryOperationV8::debug_query;
    return self.entry_.invoke(self.entry_.context,{operation,q.source_caller,q.key},value,error);
}
bool RuntimeItemLootCreationV1::query(void* raw,const dh2::data::LootCreationQueryV8& q,dh2::data::LootCreationResponseV8& response,std::string& error){
    auto& self=*static_cast<RuntimeItemLootCreationV1*>(raw);
    if(!self.resources_||!self.query_){error="Current same-session level/PlayerManager query unavailable";return false;}
    return self.query_(q,response,error);
}
bool RuntimeItemLootCreationV1::release_generated(void* raw,dh2::data::ItemInstanceV1& item,std::string& error){
    auto& self=*static_cast<RuntimeItemLootCreationV1*>(raw);
    if(!self.resources_){error="Generated item cleanup resource owner expired";return false;}
    return self.resources_->presentation->forget(item,error);
}
}
