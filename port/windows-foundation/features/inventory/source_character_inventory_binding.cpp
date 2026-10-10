#include "source_character_inventory_binding.hpp"

namespace dh::foundation::inventory {
using dh2::data::ItemPresentationOwnerV5;
using dh2::player::PlayerEquipmentRenderOwnerV1;
using dh2::world::CanonicalCharacterCandidateRecordV60;
using dh::foundation::features::SourceCharacterOwnerAliases;
using dh::foundation::features::SourceCharacterOwnerFactory;

SourceInventoryGraphBorrow source_character_inventory_graph_borrow(
    const SourceCharacterOwnerFactory& factory, std::uintptr_t identity,
    SourceItemResourcesBorrow borrow_resources) {
    return [&factory,identity,borrow_resources=std::move(borrow_resources)](
        SourceInventoryGraphLease& out,std::string& error) mutable {
        SourceCharacterOwnerAliases aliases;
        if(!factory.borrow_completed_character(identity,aliases,error))return false;
        const auto record=aliases.lifetime;
        if(!record||aliases.identity!=identity||record->actor.get()!=aliases.character||
           record->inventory37c!=aliases.inventory37c||!record->equipment||
           record->equipment.get()!=record->prepared_equipment_v60||
           !record->equipment->ready()||record->equipment->inventory()!=aliases.inventory37c) {
            error="Required same completed V60 Character/Gear/inventory graph";return false;
        }
        if(!borrow_resources){error="Typed same-character SourceItemResourcesV88 borrower unavailable";return false;}
        std::shared_ptr<const dh2::character::SourceItemResourcesV88> resources;
        if(!borrow_resources(*record,resources,error))return false;
        if(!resources||!resources->ready()||!resources->presentation()||
           &resources->loot().items()!=&aliases.inventory37c->table()) {
            error="SourceItemResourcesV88 does not match the actual Gear ItemTable";return false;
        }
        const auto* text=record->equipment->item_text_owner_v1();
        const std::shared_ptr<ItemPresentationOwnerV5> presentation=resources->presentation();
        if(!text||!presentation){error="Same prepared Gear ItemText or source Power presentation owner unavailable";return false;}
        SourceInventoryGraphLease next;
        next.owner=std::static_pointer_cast<const void>(record);
        next.services_owner=std::static_pointer_cast<const void>(resources);
        next.inventory=record->equipment->inventory();
        next.powers=presentation.get();
        next.text_owner=text;
        next.descriptors_current=true;
        out=std::move(next);error.clear();return true;
    };
}

SourceCharacterInventoryBinding::SourceCharacterInventoryBinding(
    const SourceCharacterOwnerFactory& factory,std::uintptr_t identity,
    SourceItemResourcesBorrow resources)
 :borrow_(source_character_inventory_graph_borrow(factory,identity,std::move(resources))){}

bool SourceCharacterInventoryBinding::bind_initial(const CharacterState& projection,std::string& error){
    if(resolver_){error="Source Character inventory binding is already initialized";return false;}
    auto next=std::make_unique<SourceInventoryInstanceResolver>(borrow_);
    if(!next->bind_projection(projection,error))return false;
    resolver_=std::move(next);return true;
}
void SourceCharacterInventoryBinding::invalidate_before_native_mutation()noexcept{
    if(resolver_)resolver_->invalidate_projection();
}
bool SourceCharacterInventoryBinding::refresh_after_native_mutation(const CharacterState& projection,std::string& error){
    if(!resolver_){error="Source Character inventory binding is not initialized";return false;}
    if(!resolver_->bind_projection(projection,error)){resolver_->invalidate_projection();return false;}
    return true;
}
bool SourceCharacterInventoryBinding::rebind_after_gear_restore(const CharacterState& projection,std::string& error){
    if(resolver_)resolver_->invalidate_projection();
    resolver_.reset();
    auto next=std::make_unique<SourceInventoryInstanceResolver>(borrow_);
    if(!next->bind_projection(projection,error))return false;
    resolver_=std::move(next);return true;
}
bool SourceCharacterInventoryBinding::projection_bound()const noexcept{
    return resolver_&&resolver_->projection_bound();
}
std::function<bool(const std::string&,SourceInstanceLease&,std::string&)>
SourceCharacterInventoryBinding::resolver_callback()const{
    return resolver_?resolver_->resolver_callback():decltype(resolver_callback()){};
}
} // namespace dh::foundation::inventory
