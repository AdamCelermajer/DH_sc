#include "inventory_menu.hpp"
#include <algorithm>
#include <cmath>
namespace dh::foundation::inventory {
namespace {
float edge(const HudGeometryVertex& a,const HudGeometryVertex& b,float x,float y){return (b.x-a.x)*(y-a.y)-(b.y-a.y)*(x-a.x);}
bool inside(const SlotArt& slot,float x,float y){for(std::size_t i=0;i+2<slot.hit_contour.size();i+=3){const auto&a=slot.hit_contour[i];const auto&b=slot.hit_contour[i+1];const auto&c=slot.hit_contour[i+2];if(std::abs(edge(a,b,c.x,c.y))<1e-6f)continue;const auto aa=edge(a,b,x,y),bb=edge(b,c,x,y),cc=edge(c,a,x,y);if((aa>=0&&bb>=0&&cc>=0)||(aa<=0&&bb<=0&&cc<=0))return true;}return false;}
bool prefix(const std::string& path,const std::string& base){return path.compare(0,base.size(),base)==0;}
}
int MenuPresenter::hit_slot(float x,float y)const noexcept{
    if(!std::isfinite(x)||!std::isfinite(y))return -1;
    for(const auto& slot:original_inventory_slots())if(inside(slot,x,y))return int(slot.source_slot);
    return -1;
}
bool MenuPresenter::release_slot(float x,float y,std::string& error){const auto slot=hit_slot(x,y);if(slot<0){error.clear();return true;}requested_slot_=slot;error.clear();return true;}
bool MenuPresenter::content(const MenuBindings& b,character_menu::Frame& output,std::string& error){return apply(b,output,false,error);}
bool MenuPresenter::potions_content(const MenuBindings& b,character_menu::Frame& output,std::string& error){return apply(b,output,true,error);}
bool MenuPresenter::apply(const MenuBindings& b,character_menu::Frame& output,bool potions_only,std::string& error){
    if(b.equipment_slots.size()!=9||!b.symbol){error="Inventory menu requires actual slot and StringManager bindings";return false;}
    View view;if(!items_.present(view,error))return false;
    auto next=output;
    for(const auto& slot:original_inventory_slots()){
        if(potions_only&&slot.source_slot!=9)continue;
        const Row* owned=nullptr;
        if(slot.source_slot<9){const auto binding=std::find_if(owner_.equipment.begin(),owner_.equipment.end(),[&](const auto& value){return value.slot==b.equipment_slots[slot.source_slot];});
            if(binding!=owner_.equipment.end()){const auto entry=std::find_if(view.rows.begin(),view.rows.end(),[&](const auto& row){return row.instance_id==binding->item_instance_id;});if(entry!=view.rows.end())owned=&*entry;}}
        else {const auto potion=std::find_if(view.rows.begin(),view.rows.end(),[](const auto& row){return row.type==14;});if(potion!=view.rows.end())owned=&*potion;}
        std::string name;
        if(slot.source_slot==9){if(!b.potions){error="Required source potion integer localization provider unavailable";return false;}if(!b.potions(owned?owned->quantity:0,name,error))return false;}
        else {if(owned){if(!b.item_name){error="Required source ItemInstance name provider unavailable";return false;}if(!b.item_name(*owned,name,error))return false;}
            if(name.empty()&&!b.symbol("GLOBAL_EMPTY",name,error))return false;}
        next.art.batches.erase(std::remove_if(next.art.batches.begin(),next.art.batches.end(),[&](const auto& batch){return prefix(batch.role,slot.path+"/btimg/");}),next.art.batches.end());
        next.art.batches.insert(next.art.batches.end(),slot.icons.begin(),slot.icons.end());
        for(const auto& field:slot.fields){next.text.erase(std::remove_if(next.text.begin(),next.text.end(),[&](const auto& text){return text.field.path==field.path;}),next.text.end());next.text.push_back({field,name});}
        if(owned&&b.item_color){unsigned color;if(!b.item_color(*owned,color,error))return false;if(color>=5){error="Original ItemColor outside source frame labels";return false;}
            next.art.batches.erase(std::remove_if(next.art.batches.begin(),next.art.batches.end(),[&](const auto& batch){return prefix(batch.role,slot.path+"/btfill/");}),next.art.batches.end());
            next.art.batches.insert(next.art.batches.end(),slot.color_fills[color].begin(),slot.color_fills[color].end());}
    }
    output=std::move(next);error.clear();return true;
}
}
