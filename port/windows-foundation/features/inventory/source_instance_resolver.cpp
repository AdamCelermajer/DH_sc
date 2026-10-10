#include "source_instance_resolver.hpp"
#include <algorithm>
#include <cstring>

namespace dh::foundation::inventory {
using namespace dh2::data;
struct SourceInventoryInstanceResolver::State {
    struct Entry {
        const ItemInstanceV1* item{};
        std::uint32_t index{};
        std::string definition;
        std::uint32_t quantity{};
    };
    SourceInventoryGraphBorrow borrow;
    mutable std::mutex mutex;
    const FreshInventoryOwnedV4* inventory{};
    const void* owner_identity{};
    const void* services_identity{};
    const ItemPresentationOwnerV5* powers{};
    const dh2::ui::ItemTextOwnerV5* text_owner{};
    std::map<std::string,Entry> active;
    std::map<std::string,const ItemInstanceV1*> known_ids;
    std::map<const ItemInstanceV1*,std::string> known_items;
    bool projection_valid{};
    explicit State(SourceInventoryGraphBorrow f):borrow(std::move(f)){}
};
namespace {
bool graph(const SourceInventoryGraphBorrow& borrow,SourceInventoryGraphLease& out,std::string& error){
    if(!borrow){error="Completed native source Character graph borrower unavailable";return false;}
    SourceInventoryGraphLease next;
    if(!borrow(next,error))return false;
    if(!next.owner||!next.services_owner||!next.inventory||!next.powers||!next.text_owner||!next.descriptors_current){
        error="Required completed native inventory/Power/text owner lease unavailable";return false;
    }
    out=std::move(next);error.clear();return true;
}
const ItemInstanceV1* item_at(const FreshInventoryOwnedV4& inventory,std::size_t index){
    const auto& rows=inventory.items();
    return index<rows.size()&&rows[index]&&rows[index]->item?rows[index]->item.get():nullptr;
}
bool source_row(const FreshInventoryOwnedV4& inventory,const InventoryItem& projected,
                std::uint32_t index,SourceInventoryInstanceResolver::State::Entry& out,std::string& error){
    const auto* instance=item_at(inventory,index);
    if(!instance){error="Native source inventory slot has no actual ItemInstance";return false;}
    const auto source_id=item_id(inventory.table(),projected.definition_id);
    const auto* metadata=item(inventory.table(),source_id);
    std::int16_t quantity;std::memcpy(&quantity,&instance->quantity,sizeof(quantity));
    if(projected.instance_id.empty()||source_id<0||!metadata||instance->id!=source_id||quantity<=0||
       std::uint32_t(quantity)!=projected.quantity){
        error="Stable inventory ID projection differs from actual native order/definition/quantity";return false;
    }
    out={instance,index,projected.definition_id,projected.quantity};return true;
}
bool still_owned(const FreshInventoryOwnedV4& inventory,const ItemInstanceV1* item,
                 std::uint32_t index,const std::string& definition,std::uint32_t quantity){
    if(item_at(inventory,index)!=item)return false;
    const auto id=item_id(inventory.table(),definition);if(id<0||item->id!=id)return false;
    std::int16_t actual;std::memcpy(&actual,&item->quantity,sizeof(actual));
    return actual>0&&std::uint32_t(actual)==quantity;
}
struct PinnedLease {
    SourceInventoryGraphLease graph;
    std::shared_ptr<SourceInventoryInstanceResolver::State> state;
    std::string id;
    SourceInventoryInstanceResolver::State::Entry entry;
};
bool resolve_state(const std::shared_ptr<SourceInventoryInstanceResolver::State>& state,
                   const std::string& id,SourceInstanceLease& output,std::string& error){
    SourceInventoryGraphLease current;if(!graph(state->borrow,current,error))return false;
    SourceInventoryInstanceResolver::State::Entry entry;
    {
        std::lock_guard<std::mutex> lock(state->mutex);
        if(state->inventory!=current.inventory||state->owner_identity!=current.owner.get()||
           state->services_identity!=current.services_owner.get()||state->powers!=current.powers||
           state->text_owner!=current.text_owner){error="Native source Character/Inventory/Power/Text owner changed; rebuild instance resolver";return false;}
        const auto found=state->active.find(id);if(found==state->active.end()){error="Stable source instance ID is not bound to current inventory";return false;}
        if(!state->projection_valid){error="Native source inventory projection was invalidated pending refresh";return false;}
        entry=found->second;
    }
    if(!still_owned(*current.inventory,entry.item,entry.index,entry.definition,entry.quantity)){
        error="Stable source instance lease expired after native inventory mutation";return false;
    }
    auto pin=std::make_shared<PinnedLease>();pin->graph=std::move(current);pin->state=state;pin->id=id;pin->entry=entry;
    SourceInstanceLease next;next.owner=pin;next.item=entry.item;next.powers=pin->graph.powers;
    next.text_owner=pin->graph.text_owner;next.source_index=entry.index;next.descriptors_current=pin->graph.descriptors_current;
    next.owns=[pin](const ItemInstanceV1* candidate){
        if(candidate!=pin->entry.item)return false;
        std::lock_guard<std::mutex> lock(pin->state->mutex);
        if(!pin->state->projection_valid)return false;
        const auto found=pin->state->active.find(pin->id);
        if(found==pin->state->active.end()||found->second.item!=candidate||
           found->second.index!=pin->entry.index||found->second.quantity!=pin->entry.quantity)return false;
        return still_owned(*pin->graph.inventory,candidate,pin->entry.index,
                           pin->entry.definition,pin->entry.quantity);
    };
    output=std::move(next);error.clear();return true;
}
}
SourceInventoryInstanceResolver::SourceInventoryInstanceResolver(SourceInventoryGraphBorrow borrow)
 :state_(std::make_shared<State>(std::move(borrow))){}
bool SourceInventoryInstanceResolver::projection_bound()const noexcept{
    std::lock_guard<std::mutex> lock(state_->mutex);return state_->projection_valid;
}
void SourceInventoryInstanceResolver::invalidate_projection()noexcept{
    std::lock_guard<std::mutex> lock(state_->mutex);
    state_->projection_valid=false;state_->active.clear();
}
bool SourceInventoryInstanceResolver::bind_projection(const CharacterState& projection,std::string& error){
    SourceInventoryGraphLease current;if(!graph(state_->borrow,current,error))return false;
    const auto& native=*current.inventory;
    if(native.items().size()>UINT32_MAX||projection.inventory.size()!=native.items().size()){
        error="CharacterState inventory projection count differs from actual FreshInventoryOwnedV4";return false;
    }
    std::map<std::string,State::Entry> next;std::map<const ItemInstanceV1*,std::string> reverse;
    for(std::size_t i=0;i<projection.inventory.size();++i){
        State::Entry entry;if(!source_row(native,projection.inventory[i],static_cast<std::uint32_t>(i),entry,error))return false;
        if(!next.emplace(projection.inventory[i].instance_id,entry).second||!reverse.emplace(entry.item,projection.inventory[i].instance_id).second){
            error="Duplicate stable ID or native ItemInstance in source inventory projection";return false;
        }
    }
    std::lock_guard<std::mutex> lock(state_->mutex);
    if(state_->inventory&&(state_->inventory!=current.inventory||state_->owner_identity!=current.owner.get()||
       state_->services_identity!=current.services_owner.get()||state_->powers!=current.powers||
       state_->text_owner!=current.text_owner)){
        error="Native Character/Inventory/Power/Text owner replaced; create a fresh source instance resolver";return false;
    }
    for(const auto& pair:next){
        const auto known=state_->known_ids.find(pair.first);
        if(known!=state_->known_ids.end()&&known->second!=pair.second.item){error="Stable source instance ID cannot be rebound to a different native item";return false;}
        const auto reverse_known=state_->known_items.find(pair.second.item);
        if(reverse_known!=state_->known_items.end()&&reverse_known->second!=pair.first){error="Native source item already has a different stable instance ID";return false;}
    }
    for(const auto& pair:next){state_->known_ids[pair.first]=pair.second.item;state_->known_items[pair.second.item]=pair.first;}
    state_->inventory=current.inventory;state_->owner_identity=current.owner.get();
    state_->services_identity=current.services_owner.get();state_->powers=current.powers;
    state_->text_owner=current.text_owner;state_->active=std::move(next);state_->projection_valid=true;error.clear();return true;
}
bool SourceInventoryInstanceResolver::resolve(const std::string& id,SourceInstanceLease& lease,std::string& error)const{
    return resolve_state(state_,id,lease,error);
}
std::function<bool(const std::string&,SourceInstanceLease&,std::string&)>
SourceInventoryInstanceResolver::resolver_callback()const{
    auto state=state_;return [state](const auto& id,auto& lease,auto& error){return resolve_state(state,id,lease,error);};
}
} // namespace dh::foundation::inventory
