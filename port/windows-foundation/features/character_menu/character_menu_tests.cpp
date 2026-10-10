#include "character_menu.hpp"
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <cmath>
using namespace dh::foundation;
using namespace dh::foundation::character_menu;
static void check(bool yes,const char* message){if(!yes)throw std::runtime_error(message);}
int main(){try{
    Presenter menu;std::string error;Frame frame;
    ActorState actor;actor.id=7;actor.persistent_character_id="actual-character";
    actor.health=87.5f;actor.max_health=165.09765625f;actor.resource=13.25f;actor.max_resource=27.25f;
    CharacterState character;character.id="actual-character";character.name="Saved Name";character.gold=471;
    OriginalCombatProperties properties;properties.sheets.resolved[149]=17*256+127;
    properties.sheets.resolved[150]=11*256;properties.sheets.resolved[151]=13*256;
    properties.sheets.resolved[152]=7*256;properties.sheets.resolved[148]=3*256;
    properties.sheets.resolved[33]=41*256;properties.sheets.resolved[34]=201*256;
    Bindings bindings;bindings.actor=&actor;bindings.properties=&properties;bindings.character=&character;bindings.class_label="Borrowed Class";
    check(!menu.frame(bindings,960,640,frame,error)&&!menu.is_open(),"closed menu produced frame");
    check(!menu.select(Tab::skills,error),"closed menu accepted tab");
    check(source_stat_integer(-1)==-1&&source_stat_integer(-257)==-2&&source_stat_integer(257)==1,
          "source ARM signed256 integer sentinel projection differs");
    menu.open();check(menu.is_open()&&menu.tab()==Tab::stats,"portrait open did not choose original stats tab");
    const auto& stats_art=original_menu_art(Tab::stats,true);
    const auto& no_points_art=original_menu_art(Tab::stats,false);
    const auto& equipment_art=original_menu_art(Tab::equipment,true);
    const auto& faery_art=original_menu_art(Tab::faery,true);
    auto role_count=[](const MenuArt& art,const char* needle){return std::count_if(art.batches.begin(),art.batches.end(),[&](const auto& b){return b.role.find(needle)!=std::string::npos;});};
    check(role_count(stats_art,"btnCharacterSheet/TabIcon")>0&&
          role_count(stats_art,"btnInventoryTab/TabIcon")==0&&
          role_count(equipment_art,"btnInventoryTab/TabIcon")>0,
          "source active tab did not sample full-alpha heart icon for the selected page");
    check(role_count(faery_art,"btnFaeriesTab/TabIcon")>0&&
          std::none_of(faery_art.batches.begin(),faery_art.batches.end(),[](const auto& b){return b.role.find("menu_FaerySheet")!=std::string::npos;}),
          "Faery tab route duplicated page art owned by its separate source provider");
    auto disabled_overlay_count=[](const MenuArt& art){return std::count_if(art.solids.begin(),art.solids.end(),[](const auto& s){return s.geometry.shape_id==315;});};
    check(disabled_overlay_count(stats_art)==0&&disabled_overlay_count(no_points_art)==4,
          "source zero-stat-point art did not retain the exact deactivated training overlays");
    check(menu.frame(bindings,960,640,frame,error)&&!frame.art.batches.empty()&&frame.transform.scale==2,
          "original menu art frame missing");
    bool hp=false,mp=false,xp=false,strength=false,name=false,class_name=false;
    for(const auto& field:frame.text){
        if(field.field.path.find("HpTextBox")!=std::string::npos){hp=true;check(field.value=="87 / 165","actual live HP incorrectly read stale property sheet");}
        if(field.field.path.find("MpTextBox")!=std::string::npos){mp=true;check(field.value=="13 / 27","actual live MP incorrectly read stale property sheet");}
        if(field.field.path.find("ExpTextBox")!=std::string::npos){xp=true;check(field.value=="41 / 201","source XP/max XP fields projected incorrectly");}
        if(field.field.path.find("player_class")!=std::string::npos){class_name=true;check(field.value=="Borrowed Class","same-owner localized class label was not projected");}
        if(field.field.path.find("Stat_Strength")!=std::string::npos){strength=true;check(field.value=="17","original strength property truncated differently");}
        if(field.field.path.find("player_name")!=std::string::npos){name=true;check(field.value=="Saved Name","same profile name not projected");}
    }
    check(hp&&mp&&xp&&strength&&name&&class_name,"actual source character header/stat text placements absent");
    properties.sheets.resolved[148]=0;
    check(menu.frame(bindings,960,640,frame,error),error.c_str());
    check(std::count_if(frame.solids.begin(),frame.solids.end(),[](const auto& s){return s.geometry.shape_id==315;})==4,
          "zero stat points did not select four source deactivated training-button overlays");
    properties.sheets.resolved[148]=3*256;
    for(const auto& zone:original_menu_hit_zones()){
        // P16 map controls answer only on the Map tab; checked below.
        if(zone.action==Action::map_legend||zone.action==Action::map_reset_zoom)continue;
        check(zone.triangles.size()>=3&&zone.triangles.size()%3==0,"original source hit contour malformed");
        auto x=(zone.triangles[0].x+zone.triangles[1].x+zone.triangles[2].x)/3;
        auto y=(zone.triangles[0].y+zone.triangles[1].y+zone.triangles[2].y)/3;
        for(const auto size:{std::pair<int,int>{960,640},{1920,1080},{480,900}}){
            MenuViewTransform transform;check(Presenter::viewport(size.first,size.second,transform,error),"viewport rejected");
            check(transform.x==0&&transform.y==0&&transform.scale_x*480==size.first&&transform.scale_y*320==size.second,
                  "source mode0 viewport left letterbox/gameplay exposed");
            check(menu.hit_test(x*transform.scale_x+transform.x,y*transform.scale_y+transform.y,size.first,size.second)==zone.action,
                  "PC/touch aspect viewport hit disagrees with authored contour");
        }
    }
    check(menu.release(-2000,-2000,960,640)==Action::none&&menu.is_open()&&menu.tab()==Tab::stats,"outside modal click changed state");
    auto faery_zone=std::find_if(original_menu_hit_zones().begin(),original_menu_hit_zones().end(),[](const auto& z){return z.action==Action::faery;});
    check(faery_zone!=original_menu_hit_zones().end(),"source Faery tab hit contour absent");
    const auto faery_x=(faery_zone->triangles[0].x+faery_zone->triangles[1].x+faery_zone->triangles[2].x)/3;
    const auto faery_y=(faery_zone->triangles[0].y+faery_zone->triangles[1].y+faery_zone->triangles[2].y)/3;
    check(menu.release(faery_x*2,faery_y*2,960,640)==Action::faery&&menu.tab()==Tab::faery,
          "source Faery tab release did not select the provider route");
    check(menu.select(Tab::stats,error),"source Stats tab did not return from Faery");
    // P16 QUESTUI Quest Log tab (5th): its own hit zone selects the quest page, and the Map tab's
    // zone (6th) is a different control, so the two tabs never answer for each other.
    auto quest_zone=std::find_if(original_menu_hit_zones().begin(),original_menu_hit_zones().end(),[](const auto& z){return z.action==Action::quest;});
    check(quest_zone!=original_menu_hit_zones().end(),"source Quest Log tab hit contour absent");
    const auto quest_x=(quest_zone->triangles[0].x+quest_zone->triangles[1].x+quest_zone->triangles[2].x)/3;
    const auto quest_y=(quest_zone->triangles[0].y+quest_zone->triangles[1].y+quest_zone->triangles[2].y)/3;
    check(menu.release(quest_x*2,quest_y*2,960,640)==Action::quest&&menu.tab()==Tab::quest,"Quest Log tab release did not select the quest page");
    check(menu.select(Tab::stats,error),"source Stats tab did not return from Quest Log");
    // P16 MAPFIX: Quest Log (369-432) and Map (421-484) overlap by about 11 px in sheet space. The nearest tab centre
    // answers inside the overlap, and each tab answers in its exclusive part (window size 960x640 = sheet x2).
    check(menu.hit_test(380*2,12*2,960,640)==Action::quest,"Quest Log exclusive part not hit");
    check(menu.hit_test(470*2,12*2,960,640)==Action::map,"Map exclusive part not hit");
    check(menu.hit_test(418*2,12*2,960,640)==Action::quest,"Quest Log tab edge (left of the seam at 420.95) not answered by Quest Log");
    check(menu.hit_test(424*2,12*2,960,640)==Action::map,"Map tab (right of the seam at 420.95) not answered by Map");
    // P16 Map tab: its tab icon hit zone selects it; the legend toggles and the reset request
    // answer only there, and the legend popup appears in the frame only while it is shown.
    auto map_zone=std::find_if(original_menu_hit_zones().begin(),original_menu_hit_zones().end(),[](const auto& z){return z.action==Action::map;});
    check(map_zone!=original_menu_hit_zones().end(),"source Map tab hit contour absent");
    // P16 QUESTUI2: the Quest Log zone ends where the Map zone starts (they overlapped by 11.5 authored px before).
    {
        const auto x_range=[](const auto& zone){
            float lo=zone.triangles[0].x,hi=lo;
            for(const auto& v:zone.triangles){lo=std::min(lo,v.x);hi=std::max(hi,v.x);}
            return std::pair<float,float>{lo,hi};
        };
        const auto quest_range=x_range(*quest_zone),map_range=x_range(*map_zone);
        check(quest_range.second<=map_range.first+1e-3f,"Quest Log and Map tab hit zones overlap");
    }
    const auto map_x=(map_zone->triangles[0].x+map_zone->triangles[1].x+map_zone->triangles[2].x)/3;
    const auto map_y=(map_zone->triangles[0].y+map_zone->triangles[1].y+map_zone->triangles[2].y)/3;
    check(menu.release(map_x*2,map_y*2,960,640)==Action::map&&menu.tab()==Tab::map,"Map tab release did not select the Map page");
    auto legend_zone=std::find_if(original_menu_hit_zones().begin(),original_menu_hit_zones().end(),[](const auto& z){return z.action==Action::map_legend;});
    check(legend_zone!=original_menu_hit_zones().end(),"Map Show legend hit contour absent");
    const auto legend_x=(legend_zone->triangles[0].x+legend_zone->triangles[1].x+legend_zone->triangles[2].x)/3;
    const auto legend_y=(legend_zone->triangles[0].y+legend_zone->triangles[1].y+legend_zone->triangles[2].y)/3;
    check(menu.hit_test(legend_x*2,legend_y*2,960,640)==Action::map_legend,"Map legend control not hit on Map tab");
    check(menu.frame(bindings,960,640,frame,error),error.c_str());
    check(!menu.map_legend_shown(),"legend shown before its control was used");
    check(menu.release(legend_x*2,legend_y*2,960,640)==Action::map_legend&&menu.map_legend_shown(),"Show legend did not toggle the popup");
    check(menu.release(legend_x*2,legend_y*2,960,640)==Action::map_legend&&!menu.map_legend_shown(),"Show legend did not toggle back");
    check(menu.release(legend_x*2,legend_y*2,960,640)==Action::map_legend&&menu.map_legend_shown(),"Show legend third toggle");
    auto reset_zone=std::find_if(original_menu_hit_zones().begin(),original_menu_hit_zones().end(),[](const auto& z){return z.action==Action::map_reset_zoom;});
    check(reset_zone!=original_menu_hit_zones().end(),"Map Reset zoom hit contour absent");
    const auto reset_x=(reset_zone->triangles[0].x+reset_zone->triangles[1].x+reset_zone->triangles[2].x)/3;
    const auto reset_y=(reset_zone->triangles[0].y+reset_zone->triangles[1].y+reset_zone->triangles[2].y)/3;
    check(!menu.take_map_reset_zoom(),"reset request present before use");
    check(menu.release(reset_x*2,reset_y*2,960,640)==Action::map_reset_zoom&&menu.take_map_reset_zoom(),"Reset zoom request not raised on Map tab");
    check(!menu.take_map_reset_zoom(),"reset request not consumed");
    check(menu.select(Tab::stats,error)&&menu.hit_test(legend_x*2,legend_y*2,960,640)!=Action::map_legend,"Map control hit outside the Map tab");
    check(menu.select(Tab::map,error)&&menu.map_legend_shown(),"legend state lost across tabs");
    check(menu.select(Tab::stats,error)&&menu.select(Tab::equipment,error)&&menu.frame(bindings,960,640,frame,error),"original equipment pane missing");
    const auto retained_name=character.name;const auto retained_health=actor.health;
    unsigned calls=0;bindings.content=[&](Tab tab,Frame& f,std::string&){
        check(tab==Tab::equipment,"borrowed content given wrong tab");++calls;
        MenuTextField field;field.path="borrowed-live-inventory";f.text.push_back({field,std::to_string(character.inventory.size())});return true;
    };
    character.inventory.push_back({"saved-instance","source-definition",2});
    check(menu.frame(bindings,960,640,frame,error)&&calls==1&&frame.text.back().value=="1",
          "inventory projection uses copied authority");
    character.inventory.clear();check(menu.frame(bindings,960,640,frame,error)&&frame.text.back().value=="0",
          "inventory owner change did not reach menu");
    const auto retained_batches=frame.art.batches.size();
    bindings.content=[](Tab,Frame&,std::string& e){e="borrowed content failure";return false;};
    check(!menu.frame(bindings,960,640,frame,error)&&frame.art.batches.size()==retained_batches,
          "failed borrowed projection partially published frame");
    bindings.content={};character.id="wrong-profile";
    check(!menu.frame(bindings,960,640,frame,error),"different saved character presented as current actor");
    character.id="actual-character";
    check(!menu.select(static_cast<Tab>(99),error)&&menu.tab()==Tab::equipment,"unsupported source tab mutated menu");
    menu.close();check(character.name==retained_name&&actor.health==retained_health,"UI changed gameplay owner");
    std::cout<<"character menu native tests PASS\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
