#include "merchant_shop_v1.hpp"
#include "../../../game-data/item_presentation_v5.hpp"
#include <algorithm>
#include <memory>

namespace dh::foundation::merchant {
namespace {
struct GeneratedBridge {
    const dh2::data::LootCreationServicesV8* source{};
    const dh2::data::ItemPowerTablesV5::Borrow* definitions{};
    ReleaseGeneratedItemPresentation release{};void* release_context{};
    std::vector<InventoryItem>* output{};const dh2::data::ItemTable* items{};
    std::uint32_t serial{};
};
bool generated_query(void* raw,const dh2::data::LootCreationQueryV8& q,dh2::data::LootCreationResponseV8& out,std::string& e){
    auto& b=*static_cast<GeneratedBridge*>(raw);if(!b.source->query){e="Merchant AddLoot requires the original current-level/player query";return false;}
    return b.source->query(b.source->context,q,out,e);
}
bool generated_create(void* raw,std::int32_t id,std::unique_ptr<dh2::data::ItemInstanceV1>& out,std::string& e){
    auto& b=*static_cast<GeneratedBridge*>(raw);if(!b.source->text.metadata||!b.source->text.invoke){e="Merchant AddLoot requires original ItemText initialization services";return false;}
    auto item=std::make_unique<dh2::data::ItemInstanceV1>();item->id=id;item->quantity=1;
    if(!dh2::data::item_update_name_v5(*item,b.source->text,e)||!dh2::data::item_update_stats_v5(*item,b.source->text,e)||!dh2::data::item_update_requirements_v5(*item,b.source->text,e))return false;
    out=std::move(item);return true;
}
bool generated_store(void* raw,std::unique_ptr<dh2::data::ItemInstanceV1>& item,std::string& e){
    auto& b=*static_cast<GeneratedBridge*>(raw);if(!item||!b.definitions||!*b.definitions||!b.output||!b.items){e="Merchant AddLoot generated-item destination unavailable";return false;}
    const auto* row=dh2::data::item(*b.items,item->id);if(!row||item->signed_quantity()<=0||std::size_t(item->id)>=b.items->identifiers.size()){e="Merchant AddLoot produced invalid ItemInstance identity/quantity";return false;}
    SourceItemMetadata source;source.value=item->value;source.identified=item->identified;
    const auto& names=(*b.definitions).names();for(const auto id:item->powers){if(id<0||std::size_t(id)>=names.size()){e="Generated Merchant ItemPower ID lacks source name";return false;}source.power_names.push_back(names[std::size_t(id)]);}
    InventoryItem stored;stored.instance_id="merchant-roll-"+std::to_string(++b.serial);stored.definition_id=b.items->identifiers[std::size_t(item->id)];stored.quantity=std::uint32_t(item->signed_quantity());stored.source=std::move(source);
    b.output->push_back(std::move(stored));
    if(!item->powers.empty()){
        if(!b.release){e="Powered Merchant AddLoot item requires ItemPresentationOwner release before destroying native instance";return false;}
        if(!b.release(b.release_context,*item,e))return false;
    }
    item.reset();e.clear();return true;
}
}
bool roll_stock_instances(const MerchantRow& row,const dh2::data::LootTablesV2::Borrow& tables,
 const dh2::data::LootPowerResourcesV7::Borrow& powers,const dh2::data::ItemPowerTablesV5::Borrow& definitions,
 const dh2::data::LootCreationServicesV8& source,ReleaseGeneratedItemPresentation release,void* release_context,
 bool give_all,const WithLootRandom& with_rng,std::vector<InventoryItem>& output,std::string& error){
    if(!tables||!definitions||!with_rng||!release){error="Merchant generated stock requires actual tables, ItemPower definitions, cleanup owner and same-session RNG";return false;}
    if(!source.entry.invoke||!source.power.invoke||!source.text.invoke||!source.text.metadata||!source.query){error="Merchant generated stock requires source AddLoot entry/power/text/query providers";return false;}
    // Source add order and RNG draws are retained, but failed later rows must
    // not publish an incomplete stock list to the merchant model.
    std::vector<InventoryItem> generated;
    GeneratedBridge bridge{&source,&definitions,release,release_context,&generated,&tables.items(),0};
    for(const auto loot:row.merchandise){
        const std::function<bool(dh2::data::LootRandom8V2&,std::string&)> op=[&](dh2::data::LootRandom8V2& rng,std::string& e){
            dh2::data::LootCreationV8 creation(tables,powers,definitions,rng);
            dh2::data::LootCreationServicesV8 bound{source.entry,source.power,source.text,&bridge,generated_query,generated_create,generated_store};
            const bool ok=creation.add(loot,0,0,-1,false,give_all,bound,e);
            if(auto pending=creation.release_pending_item()){
                const auto failure=e;std::string cleanup;
                if(!release(release_context,*pending,cleanup)){e="Failed Merchant AddLoot presentation cleanup: "+cleanup;return false;}
                e=failure;
            }
            return ok;
        };
        if(!with_rng(op,error))return false;
    }
    output=std::move(generated);
    error.clear();return true;
}
bool roll_stock(const MerchantRow& row, const dh2::data::LootTablesV2::Borrow& tables, const dh2::data::LootPowerResourcesV7::Borrow& powers,
                const dh2::data::LootEntryServicesV8& entry, const WithLootRandom& with_rng,
                std::vector<std::pair<std::int32_t, std::uint32_t>>& out, std::string& error) {
    if (!tables || !with_rng) { error = "Merchant stock requires the loot tables and the session loot RNG"; return false; }
    std::vector<std::pair<std::int32_t, std::uint32_t>> next;
    for (const auto loot : row.merchandise) {
        const std::function<bool(dh2::data::LootRandom8V2&, std::string&)> op = [&](dh2::data::LootRandom8V2& rng, std::string& e) {
            dh2::data::LootTableSelectionV8 table(tables, powers, rng, entry);
            std::vector<const dh2::data::LootEntry32V2*> entries;
            if (!table.select(loot, entries, e)) return false;
            dh2::data::LootItemSelectionV8 selection(tables, rng, entry);
            std::vector<dh2::data::LootItemInfoV8> items;
            if (!selection.expand(entries, false, items, e)) return false;
            for (const auto& i : items) if (i.item) next.emplace_back(std::int32_t(i.id), std::uint32_t(i.quantity ? i.quantity : 1));
            return true;
        };
        if (!with_rng(op, error)) return false;
    }
    out = std::move(next); error.clear(); return true;
}

} // namespace dh::foundation::merchant

