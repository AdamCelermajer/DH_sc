#include "character_range_capability.hpp"
#include <cstddef>
#include <cstring>

namespace dh2::character_range_capability { namespace {
struct Range {std::uintptr_t start,end;};
bool range(const void* p,std::size_t n,std::size_t alignment,Range& r) {
    const auto at=reinterpret_cast<std::uintptr_t>(p);
    if(!p || at%alignment || at>UINTPTR_MAX-n)return false;
    r={at,at+n};return true;
}
bool overlaps(Range a,Range b){return a.start<b.end && b.start<a.end;}
std::uint32_t raw(std::int32_t value){std::uint32_t w;std::memcpy(&w,&value,4);return w;}
std::uint32_t asr8(std::uint32_t w){return (w>>8)|((w&0x80000000)?0xff000000:0);}
struct Engine {
    Services services;Result* result;Range controls[2],outputs[3],entry;bool writing;
    bool metadata(const void* p,std::size_t n,std::size_t alignment,bool entry_wrapper=false) const {
        Range r;if(!range(p,n,alignment,r))return false;
        if(overlaps(r,entry) && (!entry_wrapper || r.start!=entry.start || r.end!=entry.end))return false;
        for(auto control:controls)if(overlaps(r,control))return false;
        if(writing)for(auto output:outputs)if(overlaps(r,output))return false;
        return true;
    }
    bool scalars(const void* p,std::size_t n,std::size_t alignment) const {
        Range r;if(!range(p,n,alignment,r))return false;
        if(overlaps(r,entry))return false;
        for(auto control:controls)if(overlaps(r,control))return false;
        return true;
    }
    bool character(Character* c) const {
        if(!metadata(c,sizeof(*c),alignof(Character),true) || !c->identity ||
           !scalars(c->properties,sizeof(Properties),alignof(Properties)))return false;
        Range wrapper,props;range(c,sizeof(*c),alignof(Character),wrapper);
        range(c->properties,sizeof(Properties),alignof(Properties),props);
        return !overlaps(wrapper,props);
    }
    bool inventory(Inventory* i) const {
        if(!metadata(i,sizeof(*i),alignof(Inventory),true) || !i->identity ||
           !metadata(i->view,sizeof(InventoryView),alignof(InventoryView)))return false;
        Range wrapper,view;range(i,sizeof(*i),alignof(Inventory),wrapper);
        range(i->view,sizeof(InventoryView),alignof(InventoryView),view);
        return !overlaps(wrapper,view);
    }
    Status main_hand(Inventory* inventory,std::uint32_t slot,const Instance*& instance,bool absent_allowed) {
        if(!this->inventory(inventory))return Status::invalid_source_fact;
        const auto* view=inventory->view;
        if(!view->count || view->count>128 || slot>=view->count ||
           !metadata(view->sets,std::size_t(view->count)*sizeof(EquipSet),alignof(EquipSet)))return Status::invalid_source_fact;
        const auto* reference=view->sets[slot].main_hand;
        if(!reference){instance=nullptr;return absent_allowed?Status::complete:Status::invalid_source_fact;}
        if(!metadata(reference,sizeof(*reference),alignof(decltype(*reference))))return Status::invalid_source_fact;
        instance=*reference;
        if(!metadata(instance,sizeof(*instance),alignof(Instance)))return Status::invalid_source_fact;
        return Status::complete;
    }
    Status selected(Inventory* inventory,std::uint32_t& slot) {
        if(!this->inventory(inventory))return Status::invalid_source_fact;
        const auto current=inventory->view->current_set;
        if(current<-128 || current>127)return Status::invalid_source_fact;
        slot=raw(current);result->last_slot=slot;return Status::complete;
    }
    Status item(const Instance* instance,Item*& row) {
        // Real44B GetItem loads instance+4 BEFORE its GOT/global table load.
        const auto id=raw(instance->item_id);result->last_item_id=id;++result->item_queries;
        if(!services.capture_items)return Status::service_unavailable;
        const ItemTable* table=nullptr;++result->calls;
        try {if(services.capture_items(services.context,instance,id,&table))return Status::service_failed;}
        catch(...){return Status::service_failed;}
        if(!metadata(table,sizeof(*table),alignof(ItemTable)) || !table->count ||
           table->count>65536 || id>=table->count ||
           !scalars(table->rows,std::size_t(table->count)*sizeof(Item),alignof(Item)))return Status::invalid_source_fact;
        Range rows,info;range(table->rows,std::size_t(table->count)*sizeof(Item),alignof(Item),rows);
        range(table,sizeof(*table),alignof(ItemTable),info);
        if(overlaps(rows,info))return Status::invalid_source_fact;
        row=&table->rows[id];return Status::complete;
    }
    Status ranged(Inventory* inventory,std::uint32_t& value) {
        std::uint32_t slot;auto status=selected(inventory,slot);if(status!=Status::complete)return status;
        const Instance* instance=nullptr;status=main_hand(inventory,slot,instance,true);
        if(status!=Status::complete)return status;
        if(!instance){value=0;return Status::complete;}
        Item* row=nullptr;status=item(instance,row);if(status!=Status::complete)return status;
        if(row->words[22]==4){value=1;return Status::complete;}
        // Same cached signed-byte index; fresh table/reference/instance/GetItem.
        status=main_hand(inventory,slot,instance,false);if(status!=Status::complete)return status;
        status=item(instance,row);if(status!=Status::complete)return status;
        value=row->words[22]==5;return Status::complete;
    }
    Status character_bool(Character* c,std::uint32_t& value) {
        if(!character(c))return Status::invalid_source_fact;
        if(c->properties->words[32]!=-1){value=1;return Status::complete;}
        return ranged(c->inventory,value); // nonvirtual400014→HasRangedWeapon
    }
    Status inventory_outputs(Inventory* inventory,std::uint32_t* minimum,
                             std::uint32_t* maximum,std::uint32_t* projectile,std::uint32_t& value) {
        if(!this->inventory(inventory))return Status::invalid_source_fact;
        std::uint32_t capability=0;Status status;
        // Captured genuine vtable slot8 binding, not a supplied capability flag.
        switch(inventory->binding) {
        case InventoryBinding::base_inventory:status=ranged(inventory,capability);break;
        case InventoryBinding::character_inventory:
            if(!character(inventory->character_owner) || inventory->character_owner->inventory!=inventory)
                return Status::invalid_source_fact;
            status=character_bool(inventory->character_owner,capability);break;
        default:return Status::unsupported_virtual;
        }
        if(status!=Status::complete)return status;
        if(!capability){value=0;return Status::complete;}
        std::uint32_t slot;status=selected(inventory,slot);if(status!=Status::complete)return status;
        const Instance* instance=nullptr;status=main_hand(inventory,slot,instance,false);
        if(status!=Status::complete)return status;
        Item* row=nullptr;status=item(instance,row);if(status!=Status::complete)return status;
        *minimum=raw(row->words[38]);++result->stores;
        *maximum=raw(row->words[39]);++result->stores;
        *projectile=raw(row->words[40]);++result->stores;
        value=1;return Status::complete;
    }
};
bool prepare(const void* entry,std::size_t entry_size,std::size_t entry_alignment,
             const Services* services,Result* result,bool writing,
             std::uint32_t* minimum,std::uint32_t* maximum,std::uint32_t* projectile,Engine& engine) {
    Range owner;
    if(!range(services,sizeof(*services),alignof(Services),engine.controls[0]) ||
       !range(result,sizeof(*result),alignof(Result),engine.controls[1]) ||
       !range(entry,entry_size,entry_alignment,owner) || overlaps(engine.controls[0],engine.controls[1]) ||
       overlaps(owner,engine.controls[0]) || overlaps(owner,engine.controls[1]))return false;
    engine.writing=writing;
    engine.entry=owner;
    if(writing) {
        std::uint32_t* pointers[3]{minimum,maximum,projectile};
        for(unsigned i=0;i<3;++i) {
            if(!range(pointers[i],4,alignof(std::uint32_t),engine.outputs[i]) || overlaps(engine.outputs[i],owner))return false;
            for(auto control:engine.controls)if(overlaps(engine.outputs[i],control))return false;
        }
    }
    engine.services=*services;engine.result=result;return true;
}
}
Status character_parameters(Character* c,std::uint32_t* minimum,std::uint32_t* maximum,
    std::uint32_t* projectile,const Services* services,Result* result) {
    Engine engine{};
    if(!prepare(c,sizeof(*c),alignof(Character),services,result,true,minimum,maximum,projectile,engine))return Status::invalid_argument;
    if(!engine.character(c))return Status::invalid_source_fact;
    *result={};
    if(c->properties->words[32]!=-1) {
        *minimum=asr8(raw(c->properties->words[30]));++result->stores;
        *maximum=asr8(raw(c->properties->words[31]));++result->stores;
        *projectile=raw(c->properties->words[32]);++result->stores;
        result->value=1;return Status::complete;
    }
    return engine.inventory_outputs(c->inventory,minimum,maximum,projectile,result->value);
}
Status inventory_parameters(Inventory* inventory,std::uint32_t* minimum,std::uint32_t* maximum,
    std::uint32_t* projectile,const Services* services,Result* result) {
    Engine engine{};
    if(!prepare(inventory,sizeof(*inventory),alignof(Inventory),services,result,true,minimum,maximum,projectile,engine))return Status::invalid_argument;
    *result={};return engine.inventory_outputs(inventory,minimum,maximum,projectile,result->value);
}
Status character_can_range(Character* c,const Services* services,Result* result) {
    Engine engine{};
    if(!prepare(c,sizeof(*c),alignof(Character),services,result,false,nullptr,nullptr,nullptr,engine))return Status::invalid_argument;
    *result={};return engine.character_bool(c,result->value);
}
Status has_ranged_weapon(Inventory* inventory,const Services* services,Result* result) {
    Engine engine{};
    if(!prepare(inventory,sizeof(*inventory),alignof(Inventory),services,result,false,nullptr,nullptr,nullptr,engine))return Status::invalid_argument;
    *result={};return engine.ranged(inventory,result->value);
}
} // namespace dh2::character_range_capability
