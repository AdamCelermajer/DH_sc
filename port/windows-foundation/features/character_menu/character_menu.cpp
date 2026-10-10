#include "character_menu.hpp"
#include "menu_stats.hpp"
#include <algorithm>
#include <cmath>
#include <limits>
namespace dh::foundation::character_menu {
std::int32_t source_stat_integer(std::int32_t raw) noexcept {
    const auto n=static_cast<std::int64_t>(raw);
    return static_cast<std::int32_t>(n>=0?n/256:-((-n+255)/256));
}
bool Presenter::viewport(int width,int height,MenuViewTransform& out,std::string& error){
    if(width<=0||height<=0){error="Character menu requires positive viewport";return false;}
    MenuViewTransform next;next.scale_x=width/480.f;next.scale_y=height/320.f;
    next.scale=std::max(next.scale_x,next.scale_y);next.x=0;next.y=0;
    out=next;error.clear();return true;
}
bool Presenter::select(Tab next,std::string& error){
    if(!open_){error="Character menu is closed";return false;}
    if(next!=Tab::stats&&next!=Tab::equipment&&next!=Tab::skills&&next!=Tab::faery&&next!=Tab::quest&&next!=Tab::map){error="Unsupported character menu tab";return false;}
    tab_=next;error.clear();return true;
}
namespace {
float edge(const HudGeometryVertex& a,const HudGeometryVertex& b,float x,float y){return (b.x-a.x)*(y-a.y)-(b.y-a.y)*(x-a.x);}
bool contains(const MenuHitZone& zone,float x,float y){
    for(std::size_t i=0;i+2<zone.triangles.size();i+=3){
        const auto& a=zone.triangles[i];const auto& b=zone.triangles[i+1];const auto& c=zone.triangles[i+2];
        const auto area=edge(a,b,c.x,c.y);if(std::abs(area)<1e-6f)continue;
        const auto aa=edge(a,b,x,y),bb=edge(b,c,x,y),cc=edge(c,a,x,y);
        if((aa>=0&&bb>=0&&cc>=0)||(aa<=0&&bb<=0&&cc<=0))return true;
    }return false;
}
bool has(const std::string& path,const char* component){
    return path.find(std::string("/")+component+"/")!=std::string::npos||
        (path.size()>=std::char_traits<char>::length(component)+1&&
         path.compare(path.size()-std::char_traits<char>::length(component),std::char_traits<char>::length(component),component)==0);
}
std::string whole(float value){return std::to_string(static_cast<std::int64_t>(std::floor(value)));}
bool projected(const MenuTextField& field,const Bindings& b,std::string& value){
    const auto& path=field.path;const auto& raw=b.properties->sheets.resolved;
    auto stat=[&](unsigned i){return std::to_string(source_stat_integer(raw[i]));};
    if(has(path,"player_name")){value=b.character?b.character->name:std::string{};return true;}
    if(has(path,"player_class")){value=b.class_label;return true;}
    // Source menu uses integer stat values; actual host vitals are current live
    // actor cells even when pending combat effects have not rebuilt raw sheets.
    if(has(path,"HpTextBox")){value=whole(b.actor->health)+" / "+whole(b.actor->max_health);return true;}
    if(has(path,"MpTextBox")){value=whole(b.actor->resource)+" / "+whole(b.actor->max_resource);return true;}
    if(has(path,"ExpTextBox")){value=stat(33)+" / "+stat(34);return true;}
    struct Stat{const char* component;unsigned index;};
    constexpr Stat stats[]={{"Stat_Strength",149},{"Stat_Dexterity",150},{"Stat_Endurance",151},
        {"Stat_Energy",152},{"Stat_Points",148}};
    for(const auto& item:stats)if(has(path,item.component)){value=stat(item.index);return true;}
    if(has(path,"player_gold")&&b.character){value=std::to_string(b.character->gold);return true;}
    return false;
}
}
Action Presenter::hit_test(float x,float y,int width,int height)const noexcept {
    if(!open_||width<=0||height<=0||!std::isfinite(x)||!std::isfinite(y))return Action::none;
    x/=width/480.f;y/=height/320.f;
    // Actual contour hit regions, including original invisible tab hit shape263.
    for(auto it=original_menu_hit_zones().rbegin();it!=original_menu_hit_zones().rend();++it) {
        // Map controls exist only on the Map page; elsewhere the same screen area is not a control.
        if((it->action==Action::map_legend||it->action==Action::map_reset_zoom)&&tab_!=Tab::map)continue;
        if(contains(*it,x,y))return it->action;
    }
    return Action::none;
}
Action Presenter::release(float x,float y,int width,int height)noexcept {
    const auto action=hit_test(x,y,width,height);
    switch(action){case Action::close:close();break;case Action::stats:tab_=Tab::stats;break;
    case Action::equipment:tab_=Tab::equipment;break;case Action::skills:tab_=Tab::skills;break;
    case Action::faery:tab_=Tab::faery;break;case Action::quest:tab_=Tab::quest;break;case Action::map:tab_=Tab::map;break;
    case Action::map_legend:case Action::map_reset_zoom:map_control(action);break;default:break;}
    return action;
}
Action Presenter::map_control(Action control) noexcept {
    if(!open_||tab_!=Tab::map)return Action::none;
    if(control==Action::map_legend){map_legend_=!map_legend_;return control;}
    if(control==Action::map_reset_zoom){map_reset_requested_=true;return control;}
    return Action::none;
}
bool Presenter::frame(const Bindings& b,int width,int height,Frame& output,std::string& error)const {
    if(!open_){error="Character menu is closed";return false;}
    if(!b.actor||!b.properties){error="Character menu requires same live actor and original properties";return false;}
    if(b.character&&b.actor->persistent_character_id&&*b.actor->persistent_character_id!=b.character->id){
        error="Character menu profile belongs to a different actor";return false;
    }
    const float vitals[]{b.actor->health,b.actor->max_health,b.actor->resource,b.actor->max_resource};
    for(float n:vitals)if(!std::isfinite(n)||std::abs(n)>static_cast<float>(std::numeric_limits<std::int32_t>::max())){
        error="Character menu live vitals are invalid";return false;
    }
    Frame next;if(!viewport(width,height,next.transform,error))return false;
    NativeMenuStats stats;
    bool has_stat_points=true;
    if(tab_==Tab::stats){
        if(!project_original_menu_stats(*b.properties,stats,error))return false;
        has_stat_points=source_stat_integer(b.properties->sheets.resolved[148])>0;
    }
    const auto& authored=original_menu_art(tab_,has_stat_points);next.art.batches=authored.batches;next.solids=authored.solids;
    // Map legend popup: its own authored art, drawn above the map only while it is shown.
    const MenuArt* legend=(tab_==Tab::map&&map_legend_)?&original_map_legend_art():nullptr;
    std::vector<MenuTextField> fields=authored.text_fields;
    if(legend) {
        next.art.batches.insert(next.art.batches.end(),legend->batches.begin(),legend->batches.end());
        next.solids.insert(next.solids.end(),legend->solids.begin(),legend->solids.end());
        fields.insert(fields.end(),legend->text_fields.begin(),legend->text_fields.end());
    }
    for(const auto& field:fields){
        if(tab_==Tab::stats&&!original_stats_path_visible(*b.properties,stats,field.path))continue;
        std::string value;
        if(!projected(field,b,value)&&!(tab_==Tab::stats&&original_stats_field(field.path,stats,value))&&
            b.text&&!b.text(field.path,value,error))return false;
        if(!value.empty())next.text.push_back({field,std::move(value)});
    }
    if(b.content&&!b.content(tab_,next,error))return false;
    if(tab_==Tab::stats)original_stats_visibility(*b.properties,stats,next);
    output=std::move(next);error.clear();return true;
}
}
