#include "session_container_modern_drop_v1.hpp"

#include "../../../game-data/loot_power_creation_v7.hpp"

namespace dh::foundation::interactions {
namespace {
bool fail(std::string& error,const char* message){error=message;return false;}
bool same_owner(const std::shared_ptr<const void>& a,
                const std::shared_ptr<const void>& b)noexcept{
    return a&&b&&!a.owner_before(b)&&!b.owner_before(a);
}
}

SessionContainerModernDropV1::SessionContainerModernDropV1(
    CombatSession& session,SessionContainerModernDropInputsV1 inputs)
    :session_(&session),inputs_(std::move(inputs)),
     lease_(session.actor_binding_lease().lock()){}

bool SessionContainerModernDropV1::create(
    CombatSession& session,SessionContainerModernDropInputsV1 inputs,
    std::shared_ptr<SessionContainerModernDropV1>& output,std::string& error){
    output.reset();error.clear();
    if(!session.world()||!session.actor_binding_lease().lock())
        return fail(error,"Modern container drop requires a live same-session world lease");
    if(!inputs.tables||!inputs.powers||!inputs.store)
        return fail(error,"Modern container drop requires original loot tables/powers and existing item store");
    if(!inputs.store->uses_loot_snapshot(inputs.tables))
        return fail(error,"Modern container drop store does not retain the supplied source LootTables snapshot");
    try{
        output=std::shared_ptr<SessionContainerModernDropV1>(
            new SessionContainerModernDropV1(session,std::move(inputs)));
    }catch(...){return fail(error,"Modern container drop binding allocation failed");}
    if(!output->lease_)return fail(error,"Modern container drop Session lease expired during creation");
    return true;
}

bool SessionContainerModernDropV1::current(std::string& error)const{
    if(!session_||!lease_||!session_->world())
        return fail(error,"Modern container drop lost its same-session world");
    const auto now=session_->actor_binding_lease().lock();
    if(!same_owner(lease_,now))
        return fail(error,"Modern container drop has a stale Session lease; recreate after restore");
    error.clear();return true;
}

bool SessionContainerModernDropV1::current_object(
    const ActorDefinition& definition,WorldObject*& output,std::string& error)const{
    output=nullptr;
    if(!current(error))return false;
    if(!definition.stableId||definition.stableId==invalid_actor_id||definition.name.empty())
        return fail(error,"Modern container object requires exact authored stable ID and name");
    auto* object=session_->world()->find_object(definition.stableId);
    if(!object||object->id!=definition.stableId||object->name!=definition.name)
        return fail(error,"Authored container definition does not match a current same-world WorldObject");
    output=object;error.clear();return true;
}

bool SessionContainerModernDropV1::services(
    SourceContainerLootServicesV1& output,std::string& error){
    if(!current(error))return false;
    output={};
    output.owner=shared_from_this();
    output.tables=inputs_.tables;
    output.powers=inputs_.powers;
    output.entry=inputs_.entry;
    output.context=this;
    output.with_gameplay_rng=&SessionContainerModernDropV1::with_rng;
    output.drop_item_with_rng=&SessionContainerModernDropV1::publish;
    error.clear();return true;
}

bool SessionContainerModernDropV1::with_rng(
    void* raw,const SourceContainerLootRngOperationV1& operation,
    std::string& error){
    auto* self=static_cast<SessionContainerModernDropV1*>(raw);
    if(!self||!operation)return fail(error,"Modern container drop requires a live scoped RNG operation");
    if(!self->current(error))return false;
    bool called=false;
    const bool ok=self->session_->world()->with_loot_random(
        [&](dh2::data::LootRandom8V2& random,std::string& inner_error){
            if(called)return fail(inner_error,"Container loot attempted to reuse one Session RNG loan");
            called=true;
            if(!self->current(inner_error))return false;
            const bool result=operation(random,inner_error);
            if(!self->current(inner_error))return false;
            return result;
        },error);
    if(!ok)return false;
    if(!called)return fail(error,"Session RNG owner did not execute the container selection operation");
    error.clear();return true;
}

bool SessionContainerModernDropV1::publish(
    void* raw,const SourceContainerDropItemV1& item,
    dh2::data::LootRandom8V2& random,std::string& error){
    auto* self=static_cast<SessionContainerModernDropV1*>(raw);
    if(!self||!self->inputs_.store)
        return fail(error,"Modern container drop has no existing WorldItemAdapter store");
    if(!self->current(error))return false;
    if(item.source_state||!item.source_object||!item.source_definition||
       item.source_actor!=item.source_definition->stableId||
       item.source_object->id!=item.source_actor||
       item.source_object->name!=item.source_definition->name||
       item.source_position!=item.source_object->transform.position)
        return fail(error,"Modern container drop requires the exact neutral source definition/object/transform");
    const auto* current_object=self->session_->world()->find_object(item.source_actor);
    if(current_object!=item.source_object)
        return fail(error,"Modern container drop source pointer is not current WorldObject storage");
    const bool opener_is_actor=self->session_->actor(item.opener_actor)!=nullptr;
    const bool opener_is_object=self->session_->world()->find_object(item.opener_actor)!=nullptr;
    if(item.opener_actor==invalid_actor_id||(!opener_is_actor&&!opener_is_object))
        return fail(error,"Modern container drop opener is not a current same-session actor or object");
    if(!item.selected.item||!item.selected.entry||!item.selected.quantity||
       item.selected.id<0||static_cast<std::size_t>(item.selected.id)>=self->inputs_.tables.items().rows.size()||
       &self->inputs_.tables.items().rows[static_cast<std::size_t>(item.selected.id)]!=item.selected.item)
        return fail(error,"Modern container drop lacks the exact borrowed ItemTable row/entry/quantity");

    loot::RuntimeWorldItemRecordV1 record;
    record.source_actor=item.source_actor;
    record.killer_actor=item.opener_actor;
    record.loot_table=item.loot_table;
    record.item_id=item.selected.id;
    record.quantity=item.selected.quantity;
    record.authored_item=item.selected.item;
    record.authored_entry=item.selected.entry;
    if(record.authored_item->record.words[22]==13){
        if(!self->inputs_.gold_bonus256)
            return fail(error,"GoldStack publication requires a source-backed opener value-bonus provider");
        std::int32_t bonus256{};
        if(!self->inputs_.gold_bonus256(self->inputs_.gold_bonus_context,
                item.opener_actor,bonus256,error))return false;
        std::int32_t value{};
        if(dh2_loot_item_value_v7(&value,&random,&record.authored_item->record,
                                  nullptr,0,bonus256)!=0)
            return fail(error,"Original GoldStack value kernel rejected the source item");
        record.resolved_gold_value=value;
    }
    loot::RuntimeWorldItemIdV1 published{};
    if(!self->inputs_.store->publish_source_object_drop(
            record,*item.source_definition,*item.source_object,published,error))return false;
    error.clear();return true;
}

} // namespace dh::foundation::interactions
