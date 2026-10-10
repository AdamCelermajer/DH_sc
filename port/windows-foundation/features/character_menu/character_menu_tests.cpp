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
    check(menu.select(Tab::equipment,error)&&menu.frame(bindings,960,640,frame,error),"original equipment pane missing");
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
