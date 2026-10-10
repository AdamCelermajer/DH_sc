#include "equipment_menu.hpp"
#include <algorithm>
#include <cmath>
namespace dh::foundation::equipment_menu {
Presenter::Presenter(const CharacterState& c,const dh2::data::ItemTable& t,const dh2::data::PropertyState& p,
 EquipmentAdapter& a,Options o):owner_(c),items_(t),properties_(p),mutations_(a),options_(std::move(o)){}
bool Presenter::select_instance(const std::string& id,std::string& error){
    if(std::none_of(owner_.inventory.begin(),owner_.inventory.end(),[&](const auto& item){return item.instance_id==id;})){
        error="Equipment selection is not an actual owned instance";return false;
    }selected_=id;error.clear();return true;
}
bool Presenter::select_slot(unsigned slot,std::string& error){
    // Original Details category9 is valuables/potions; it is not an equipment
    // binding slot and therefore never reaches the equipment slot kernel.
    if(slot>9||options_.slots.size()!=9){error="Invalid source inventory category selection";return false;}
    slot_=slot;selected_.clear();
    if(slot<9)for(const auto& binding:owner_.equipment)if(binding.slot==options_.slots[slot])selected_=binding.item_instance_id;
    error.clear();return true;
}
unsigned Presenter::hit_test(float x,float y)const noexcept{
    if(!std::isfinite(x)||!std::isfinite(y))return 10;
    auto edge=[](const auto& a,const auto& b,float px,float py){return (b.x-a.x)*(py-a.y)-(b.y-a.y)*(px-a.x);};
    for(const auto& slot:original_slot_art()) {
      for(std::size_t i=0;i+2<slot.hit_triangles.size();i+=3){
        const auto& a=slot.hit_triangles[i];const auto& b=slot.hit_triangles[i+1];const auto& c=slot.hit_triangles[i+2];
        if(std::abs(edge(a,b,c.x,c.y))<1e-6f)continue;
        auto aa=edge(a,b,x,y),bb=edge(b,c,x,y),cc=edge(c,a,x,y);
        if((aa>=0&&bb>=0&&cc>=0)||(aa<=0&&bb<=0&&cc<=0))return slot.source_slot;
      }
    }
    return 10;
}
bool Presenter::view(std::vector<OwnedSelection>& output,std::string& error)const{
    std::vector<OwnedSelection> next;
    for(const auto& owned:owner_.inventory){
        const auto* item=dh2::data::item(items_,dh2::data::item_id(items_,owned.definition_id));
        if(!item){error="Equipment item source definition missing";return false;}
        OwnedSelection row;row.instance_id=owned.instance_id;row.definition_id=owned.definition_id;row.icon_name=item->icon_name;
        row.name_text_oid=item->record.words[17];row.type=item->record.words[22];row.slotting=item->record.words[26];
        // Exact scalar branch from character_menu_slot_candidate_v1. Its
        // ItemInstance parameter is explicitly unused in the recovered owner;
        // do not fabricate an engine instance just to call the scalar rule.
        auto target=row.slotting;
        if(row.type!=4&&row.type!=5&&target==1&&properties_.resolved[202])target=-3;
        if(slot_==9)row.applicable_to_selected_slot=row.slotting==-1;
        else if(slot_<9&&row.slotting!=-1){
            if(target>=0&&target<9)row.applicable_to_selected_slot=unsigned(target)==slot_;
            else if(target==-3)row.applicable_to_selected_slot=slot_==1||slot_==2;
            else if(target==-2)row.applicable_to_selected_slot=slot_==5||slot_==6;
            else row.applicable_to_selected_slot=target==-4&&slot_==1;
        }
        row.requirements_met=equipment_equippable_by(*item,properties_,options_.online_requirements_bypass,options_.player_class_id);
        for(const auto& binding:owner_.equipment)if(binding.item_instance_id==owned.instance_id)row.equipped=true;
        constexpr unsigned indexes[]{19,149,150,151,152};
        for(unsigned i=0;i<5;++i){row.required[i]=item->record.words[29+i];row.actual[i]=character_menu::source_stat_integer(properties_.resolved[indexes[i]]);}
        next.push_back(std::move(row));
    }output=std::move(next);error.clear();return true;
}
bool Presenter::view_for_selected_slot(std::vector<OwnedSelection>& output,std::string& error)const{
    if(slot_>9){error="No actual source inventory category selected";return false;}
    std::vector<OwnedSelection> next;if(!view(next,error))return false;
    next.erase(std::remove_if(next.begin(),next.end(),[](const auto& item){return !item.applicable_to_selected_slot;}),next.end());
    output=std::move(next);return true;
}
bool Presenter::frame(character_menu::Frame& output,std::string& error)const{
    if(options_.slots.size()!=9){error="Equipment source slot names unavailable";return false;}
    character_menu::Frame next=output;
    auto replaces=[](const std::string& path){
        for(const auto& slot:original_slot_art()) {
            if(path.find(slot.button_path+"/")==0)return true;
        }
        return false;
    };
    next.art.batches.erase(std::remove_if(next.art.batches.begin(),next.art.batches.end(),[&](const auto& b){return replaces(b.role);}),next.art.batches.end());
    next.text.erase(std::remove_if(next.text.begin(),next.text.end(),[&](const auto& t){return replaces(t.field.path);}),next.text.end());
    for(const auto& slot:original_slot_art()){
        const InventoryItem* owned=nullptr;
        for(const auto& binding:owner_.equipment)if(binding.slot==options_.slots[slot.source_slot]){
            auto found=std::find_if(owner_.inventory.begin(),owner_.inventory.end(),[&](const auto& x){return x.instance_id==binding.item_instance_id;});
            if(found==owner_.inventory.end()){error="Equipment menu binding is not owned";return false;}owned=&*found;
        }
        std::string name;
        if(owned){const auto* item=dh2::data::item(items_,dh2::data::item_id(items_,owned->definition_id));
            if(!item){error="Equipped source item definition missing";return false;}
            if(!options_.item_name){error="Equipment native ItemName provider unavailable";return false;}
            if(!options_.item_name(*owned,*item,name,error))return false;
        }else{
            if(!options_.empty_name){error="Equipment GLOBAL_EMPTY localization provider unavailable";return false;}
            if(!options_.empty_name(name,error))return false;
        }
        next.art.batches.insert(next.art.batches.end(),slot.art.batches.begin(),slot.art.batches.end());
        for(const auto& field:slot.art.text_fields)next.text.push_back({field,name});
    }
    output=std::move(next);error.clear();return true;
}
bool Presenter::equip_selected(std::string& e){
    if(selected_.empty()){e="No actual equipment instance selected";return false;}
    if(slot_>=9){e="No actual source equipment slot selected";return false;}
    std::vector<OwnedSelection> candidates;if(!view_for_selected_slot(candidates,e))return false;
    if(std::none_of(candidates.begin(),candidates.end(),[&](const auto& item){return item.instance_id==selected_;})){
        e="Actual selected item is not a source slot candidate";return false;
    }
    return mutations_.equip_to_slot(selected_,slot_,e);
}
bool Presenter::unequip_selected_slot(std::string& e){if(slot_>=9){e="No source equipment slot selected";return false;}return mutations_.unequip(slot_,e);}
}
