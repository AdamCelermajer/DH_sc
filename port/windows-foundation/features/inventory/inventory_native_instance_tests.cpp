#include "source_instance_resolver.hpp"
#include "source_item_descriptors.hpp"
#include "../character_menu/menu_text.hpp"
#include "../../asset_catalog.hpp"
#include "../../../engine-ui/item_text_owner_v5.hpp"
#include "../../../game-data/item_presentation_v5.hpp"
#include "../../../game-data/loot_tables_v2.hpp"
#include "../../../level-world/player_save_inventory_writer_v45.hpp"
#include <algorithm>
#include <iostream>
#include <stdexcept>

using namespace dh::foundation;
using namespace dh2::data;
using namespace dh::foundation::inventory;
static void check(bool value,const std::string& error){if(!value)throw std::runtime_error(error);}
static std::vector<std::uint8_t> asset(AssetCatalog& assets,const char* path){return assets.read(path);}

struct ActualTextEffects {
    dh2::ui::ItemTextOwnerV5* text{};
    ItemPresentationOwnerV5* powers{};
    const ItemPowerTablesV5::Borrow* power_tables{};
    static bool invoke(void* raw,FreshInventoryOwnedV4&,const OwnedInventoryRequestV4& q,
                       OwnedInventoryResponseV4& out,std::string& error){
        auto& self=*static_cast<ActualTextEffects*>(raw);out={};
        switch(q.operation){
        case OwnedInventoryOperationV4::update_name:
            return q.item&&item_update_name_v5(*q.item,self.text->services(),error);
        case OwnedInventoryOperationV4::update_stats:
            return q.item&&item_update_stats_v5(*q.item,self.text->services(),error);
        case OwnedInventoryOperationV4::update_requirements:
            return q.item&&item_update_requirements_v5(*q.item,self.text->services(),error);
        case OwnedInventoryOperationV4::add_power:
            return q.item&&q.argument>=0&&self.powers->add_power(*q.item,q.argument,0,self.text->services(),error);
        case OwnedInventoryOperationV4::debug_load:
        case OwnedInventoryOperationV4::debug_query:
        case OwnedInventoryOperationV4::inventory_full:
        case OwnedInventoryOperationV4::gold_notifications:
        case OwnedInventoryOperationV4::full_notifications:
            out.value=0;return true;
        default:error="Unexpected operation in native inventory identity fixture";return false;
        }
    }
};

struct SharedSourceServices {
    std::shared_ptr<ItemPresentationOwnerV5> powers;
    std::shared_ptr<dh2::ui::ItemTextOwnerV5> text;
};

int main(int argc,char** argv){try{
    check(argc==2,"Supply original assets root");AssetCatalog assets(argv[1]);std::string error;
    OriginalPropertyDatabase properties;check(load_original_property_tables(assets,"original-cache/data/pydata",properties,error),error);
    auto lr=asset(assets,"original-cache/data/pydata/loot_table_pyarray.bin");
    auto ln=asset(assets,"original-cache/data/pydata/loot_table_pyarraynames.bin");
    auto ls=asset(assets,"original-cache/data/pydata/loot_table_pystructnames.bin");
    LootTablesV2 loot;check(loot.load({lr.data(),lr.size()},{ln.data(),ln.size()},{ls.data(),ls.size()},error),error);
    auto pr=asset(assets,"original-cache/data/pydata/item_powers_pyarray.bin");
    auto pn=asset(assets,"original-cache/data/pydata/item_powers_pyarraynames.bin");
    auto ps=asset(assets,"original-cache/data/pydata/item_powers_pystructnames.bin");
    ItemPowerTablesV5 power_tables;check(power_tables.load({pr.data(),pr.size()},{pn.data(),pn.size()},{ps.data(),ps.size()},error),error);
    CharacterState projection=make_default_character();projection.inventory.clear();
    character_menu::MenuLocalization localization;check(localization.load(assets,"original-cache/data",0,error),error);
    check(localization.bind_profile(&projection,error),error);
    dh2::ui::HudTextV1* shared_text=nullptr;dh2::ui::HudTextEnvironmentV1 environment;
    check(localization.borrow_text(shared_text,environment,error),error);
    const auto& table=loot.borrow().items();
    auto text=std::make_shared<dh2::ui::ItemTextOwnerV5>(table,properties.characters,*shared_text,environment);
    auto presentations=std::make_shared<ItemPresentationOwnerV5>(power_tables.borrow());
    auto service_owners=std::make_shared<SharedSourceServices>();service_owners->powers=presentations;service_owners->text=text;
    ActualTextEffects effect_context{text.get(),presentations.get(),nullptr};
    OwnedInventoryServicesV4 effects{&effect_context,ActualTextEffects::invoke};
    auto source_properties=std::make_shared<PropertyState>();LootRandom8V2 random{1,0};
    auto original=std::make_shared<FreshInventoryOwnedV4>(0x2345,loot.borrow(),random,-1,source_properties);
    const auto sword_id=item_id(table,"Longsword01"),potion_id=item_id(table,"Potion0");
    check(sword_id>=0&&potion_id>=0,"Actual source inventory fixture rows absent");
    std::unique_ptr<ItemInstanceV1> sword;check(original->create_item(sword_id,1,sword,effects,error),error);
    const auto& powers=power_tables.borrow().rows();const auto power=std::find_if(powers.begin(),powers.end(),[](const auto& row){return row.scalars.description>0;});
    check(power!=powers.end(),"Actual source Power description missing");
    check(presentations->add_power(*sword,std::int32_t(power-powers.begin()),0,text->services(),error),error);
    std::int32_t index=-1;check(original->add_item(sword,true,false,index,effects,error)&&index==0&&!sword,error);
    std::unique_ptr<ItemInstanceV1> potion;check(original->create_item(potion_id,3,potion,effects,error),error);
    check(original->add_item(potion,true,false,index,effects,error)&&index==1&&!potion,error);
    check(original->items().size()==2&&original->items()[0]->item->powers.size()==1,"Actual FreshInventoryOwnedV4 order/power setup failed");

    std::weak_ptr<FreshInventoryOwnedV4> weak_original=original;
    SourceInventoryGraphBorrow borrow=[weak_original,service_owners](SourceInventoryGraphLease& out,std::string& e){
        auto inventory=weak_original.lock();if(!inventory){e="Expired completed native inventory owner";return false;}
        out.owner=inventory;out.services_owner=service_owners;out.inventory=inventory.get();
        out.powers=service_owners->powers.get();out.text_owner=service_owners->text.get();out.descriptors_current=true;e.clear();return true;
    };
    SourceInventoryInstanceResolver resolver(borrow);
    check(!resolver.projection_bound(),"Unbound native source resolver reported ready");
    projection.inventory={{"saved-item-0","Longsword01",1},{"saved-item-1","Potion0",3}};
    check(resolver.bind_projection(projection,error),error);
    check(resolver.projection_bound(),"Successfully bound source projection did not report ready");
    SourceInstanceLease first;check(resolver.resolve("saved-item-0",first,error),error);
    check(first.item==original->items()[0]->item.get()&&first.source_index==0&&first.owns(first.item),"Stable ID did not resolve to actual ordered native instance");
    SourceInstanceLease second;check(resolver.resolve("saved-item-1",second,error),error);
    check(second.item==original->items()[1]->item.get()&&second.source_index==1&&second.item->signed_quantity()==3,
          "Stable ID did not preserve the next actual native row and quantity");
    SourceInstanceDescriptorProvider provider(table,resolver.resolver_callback());SourceOwnedDescriptors descriptors;
    check(provider.present(projection.inventory[0],descriptors,error),error);
    check(descriptors.base.name==first.item->name&&descriptors.base.stats==first.item->description&&
          descriptors.value==first.item->value&&descriptors.powers.size()==1&&
          descriptors.powers[0]==presentations->powers(*first.item)->front().description,
          "Resolver synthesized or detached actual ItemInstance descriptors");
    auto wrong_quantity=projection;wrong_quantity.inventory[1].quantity=2;
    check(!resolver.bind_projection(wrong_quantity,error),"Resolver accepted a projected quantity different from native signed16 storage");
    auto changed_id=projection;changed_id.inventory[0].instance_id="replacement-id";
    check(!resolver.bind_projection(changed_id,error),"Resolver rebound one native item to a different stable ID");
    SourceInstanceLease unchanged;check(resolver.resolve("saved-item-0",unchanged,error),error);
    check(unchanged.item==first.item&&unchanged.source_index==0,"Rejected projection changed the previous identity map");

    // Native Gear/pickup callers invalidate before mutation. Leases stop being
    // actionable until a bind publishes the actual post-mutation stack/order.
    resolver.invalidate_projection();
    check(!resolver.projection_bound(),"Invalidated native projection still reported ready");
    check(!first.owns(first.item),"Native mutation invalidation left an old lease actionable");
    SourceInstanceLease blocked;
    check(!resolver.resolve("saved-item-0",blocked,error),"Resolver allowed lookup before post-mutation projection refresh");
    std::unique_ptr<ItemInstanceV1> extra_potion;
    check(original->create_item(potion_id,1,extra_potion,effects,error),error);
    check(original->add_item(extra_potion,true,false,index,effects,error)&&index==2&&!extra_potion,
          error.empty()?"Actual source potion stack mutation returned unexpected index/ownership":error);
    projection.inventory.push_back({"picked-up-item","Potion0",1});
    check(resolver.bind_projection(projection,error),error);
    check(resolver.projection_bound(),"Refreshed native projection did not report ready");
    SourceInstanceLease picked_up;
    check(resolver.resolve("picked-up-item",picked_up,error),error);
    check(picked_up.item==original->items()[2]->item.get()&&
          picked_up.source_index==2&&picked_up.item->signed_quantity()==1,
          "Post-mutation projection did not bind the actual added source row");

    // Exercise the actual native GEAR writer/reader. Projected stable IDs follow
    // the restored source item order; no instance metadata is added to the save.
    std::vector<std::string> power_names=power_tables.borrow().names();
    dh2::level::SavegameStreamV2 saved;check(dh2::level::player_save_inventory_writer_v45(*original,power_names,saved,error),error);
    auto restored=std::make_shared<FreshInventoryOwnedV4>(0x2345,loot.borrow(),random,-1,source_properties);
    OwnedInventoryServicesV4 restored_effects{&effect_context,ActualTextEffects::invoke};InventoryLoadReceiptV1 receipt;
    check(restored->load_saved_section_v50({saved.bytes().data(),saved.bytes().size()},power_names,restored_effects,receipt,error),error);
    check(receipt.completed&&receipt.items_retained==3&&restored->items().size()==3&&
          restored->items()[0]->item->id==sword_id&&restored->items()[0]->item->quantity==1&&
          restored->items()[0]->item->powers==original->items()[0]->item->powers&&
          restored->items()[1]->item->id==potion_id&&restored->items()[1]->item->quantity==3&&
          restored->items()[2]->item->id==potion_id&&restored->items()[2]->item->quantity==1,
          "Actual source GEAR save order/quantity/power roundtrip changed");
    std::weak_ptr<FreshInventoryOwnedV4> weak_restored=restored;
    SourceInventoryGraphBorrow restored_borrow=[weak_restored,service_owners](SourceInventoryGraphLease& out,std::string& e){
        auto inventory=weak_restored.lock();if(!inventory){e="Expired restored native inventory owner";return false;}
        out.owner=inventory;out.services_owner=service_owners;out.inventory=inventory.get();
        out.powers=service_owners->powers.get();out.text_owner=service_owners->text.get();out.descriptors_current=true;e.clear();return true;
    };
    SourceInventoryInstanceResolver restored_resolver(restored_borrow);
    check(restored_resolver.bind_projection(projection,error),error);
    SourceInstanceLease restored_item;check(restored_resolver.resolve("saved-item-0",restored_item,error),error);
    check(restored_item.item==restored->items()[0]->item.get()&&restored_item.item!=first.item&&
          restored_item.source_index==0&&restored_item.owns(restored_item.item),"Stable save ID did not rebind to restored native owner/order");
    SourceInstanceLease restored_potion;check(restored_resolver.resolve("saved-item-1",restored_potion,error),error);
    check(restored_potion.item==restored->items()[1]->item.get()&&restored_potion.source_index==1&&
          restored_potion.item->signed_quantity()==3,"Source GEAR restore changed stable row-one quantity mapping");
    SourceInstanceLease restored_pickup;check(restored_resolver.resolve("picked-up-item",restored_pickup,error),error);
    check(restored_pickup.item==restored->items()[2]->item.get()&&restored_pickup.source_index==2&&
          restored_pickup.item->signed_quantity()==1,"Source GEAR restore changed added row identity/order");
    restored_pickup={};
    restored_potion={};
    restored.reset();check(!weak_restored.expired(),"Resolved source lease did not pin its actual native inventory owner");
    restored_item={};check(weak_restored.expired(),"Expired native owner remained pinned after its source lease ended");
    SourceInstanceLease expired;check(!restored_resolver.resolve("saved-item-0",expired,error)&&
          error.find("Expired restored native inventory owner")!=std::string::npos,"Expired source owner did not reject resolver lookup");
    std::cout<<"native source instance resolver PASS ordered GEAR restore, actual quantity/power descriptors, pinned owner lifetime\n";
    return 0;
}catch(const std::exception& exception){std::cerr<<exception.what()<<'\n';return 1;}}
