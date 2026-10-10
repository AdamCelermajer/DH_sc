#include "source_character_item_resources.hpp"

namespace dh::foundation::inventory {
using dh2::world::CanonicalCharacterCandidateRecordV60;
using dh2::character::SourceItemResourcesV88;

bool SourceCharacterItemResourcesV1::publish(
    const std::shared_ptr<CanonicalCharacterCandidateRecordV60>& record,
    std::shared_ptr<const SourceItemResourcesV88> items,std::string& error) {
    if(!record||!items||!items->ready()||!record->services.loot_tables||
       !record->services.design||!record->design||!record->design.characters()||
       !items->loot()||!items->definitions()||!items->presentation()) {
        error="Required initialized source record/cache/design and existing SourceItemResourcesV88";return false;
    }
    const auto candidate_loot=record->services.loot_tables->borrow();
    if(!candidate_loot||&candidate_loot.items()!=&items->loot().items()) {
        error="SourceItemResourcesV88 must use this candidate's already-loaded CharacterCandidateCache Loot table";return false;
    }
    if(record->equipment||record->prepared_equipment_v60||record->inventory_transferred) {
        error="Typed SourceItemResources association must be published before Gear input/Load4 transfer";return false;
    }
    std::lock_guard<std::mutex> lock(mutex_);
    auto found=by_record_.find(record.get());
    if(found!=by_record_.end()) {
        const auto same=found->second.record.lock();
        if(same&&same.get()!=record.get()) {
            error="Canonical Character record address was reused while its item resource association remains live";return false;
        }
        if(same&&found->second.items.get()!=items.get()) {
            error="Canonical Character cannot replace its published SourceItemResourcesV88 owner";return false;
        }
    }
    by_record_[record.get()]={record,std::move(items)};error.clear();return true;
}

bool SourceCharacterItemResourcesV1::borrow(
    const CanonicalCharacterCandidateRecordV60& record,
    std::shared_ptr<const SourceItemResourcesV88>& out,std::string& error) const {
    std::lock_guard<std::mutex> lock(mutex_);
    const auto found=by_record_.find(&record);
    if(found==by_record_.end()) {error="No typed SourceItemResourcesV88 association for this canonical Character";return false;}
    const auto same=found->second.record.lock();
    if(!same||same.get()!=&record||!found->second.items||!found->second.items->ready()) {
        error="Expired or mismatched typed Character/SourceItemResources association";return false;
    }
    out=found->second.items;error.clear();return true;
}

SourceItemResourcesBorrow SourceCharacterItemResourcesV1::borrower() {
    const auto self=shared_from_this();
    return [self](const CanonicalCharacterCandidateRecordV60& record,
                  std::shared_ptr<const SourceItemResourcesV88>& out,std::string& error) {
        return self->borrow(record,out,error);
    };
}

bool SourceCharacterItemResourcesV1::configure_gear_inputs(
    const std::shared_ptr<CanonicalCharacterCandidateRecordV60>& record,
    const std::shared_ptr<const SourceItemResourcesV88>& items,
    dh2::player::PlayerEquipmentRenderInputsV1& input,
    const dh2::ui::HudTextEnvironmentV1& text_environment,
    std::vector<std::shared_ptr<void>> host_services,std::string& error) {
    if(!record||!items||!items->ready()||!items->loot()||!items->definitions()||
       !items->presentation()) {error="Required exact ready SourceItemResourcesV88 for Player Gear";return false;}
    if(input.immutable_loot_v88&&
       &input.immutable_loot_v88.items()!=&items->loot().items()) {
        error="Player Gear immutable Loot borrow belongs to a different cache";return false;
    }
    const auto definitions=items->definitions();
    if(input.immutable_powers_v88&&
       &input.immutable_powers_v88.rows()!=&definitions.rows()) {
        error="Player Gear immutable Power borrow belongs to a different SourceItemResourcesV88";return false;
    }
    if(!publish(record,items,error))return false;
    auto pins=std::make_shared<SourceCharacterGearResourcePinsV1>();
    pins->items=items;
    if(input.services_lease_v62)host_services.push_back(std::move(input.services_lease_v62));
    pins->host_services=std::move(host_services);
    input.immutable_loot_v88=items->loot();
    input.immutable_powers_v88=definitions;
    input.text_environment=text_environment;
    input.services_lease_v62=std::move(pins);
    error.clear();return true;
}

bool SourceCharacterItemResourcesV1::release_after_character_unpublication(
    const CanonicalCharacterCandidateRecordV60& record,std::string& error) {
    if(record.equipment||record.prepared_equipment_v60) {
        error="Cannot release item resource pins while the same Character Gear still borrows them";return false;
    }
    std::lock_guard<std::mutex> lock(mutex_);
    const auto found=by_record_.find(&record);
    if(found==by_record_.end()){error.clear();return true;}
    const auto same=found->second.record.lock();
    if(same&&same.get()!=&record){error="Typed item resource registry record identity mismatch";return false;}
    by_record_.erase(found);error.clear();return true;
}

} // namespace dh::foundation::inventory
