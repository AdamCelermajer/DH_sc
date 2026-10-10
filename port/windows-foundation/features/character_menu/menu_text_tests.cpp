#include "menu_text.hpp"
#include "../../asset_catalog.hpp"
#include "../../content_paths.hpp"
#include "../../original_actor_properties.hpp"
#include "../../../engine-ui/text_layout_v1.hpp"
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <cmath>
#include <string_view>
using namespace dh::foundation;using namespace dh::foundation::character_menu;
static void check(bool yes,const char* why){if(!yes)throw std::runtime_error(why);}
int main(int argc,char** argv){try{
    MenuTextField field;field.font_id=103;field.source_height=20;
    field.local_bounds={-2,98,-2,30};field.matrix={1,0,0,1,100,40};field.leading=-2;
    field.margins={3,5,2};std::string error;std::array<float,2> baseline{9,11};
    check(layout_menu_text(field,30,baseline,error)&&baseline==std::array<float,2>{105,60},
          "source firstline baseline used fake halfheight/device glyph bounds/paragraph leading");
    field.align=1;check(layout_menu_text(field,30,baseline,error)&&baseline==std::array<float,2>{161,60},"source right alignment WIDTH_FUDGE differs");
    field.align=2;check(layout_menu_text(field,30,baseline,error)&&baseline==std::array<float,2>{133,60},"source centred alignment differs");
    field.matrix={0,2,-3,0,400,50};
    check(layout_menu_text(field,30,baseline,error)&&baseline==std::array<float,2>{340,116},"source field transform flattened into bounds");
    field.matrix={1,0,0,1,0,0};field.align=0;
    auto current=*original_menu_font(103);current.descent=2048;current.leading=1024;
    check(layout_menu_text(field,30,current,1,baseline,error)&&baseline[1]==19,"borrowed source DefineFont3 metrics ignored");
    current.define_font3=false;
    check(layout_menu_text(field,30,current,1,baseline,error)&&baseline[1]==0,"source DefineFont2 font units denominator differs");
    auto old=baseline;field.font_id=999;
    check(!layout_menu_text(field,30,baseline,error)&&baseline==old,"unknown font guessed fallback");
    field.font_id=103;current.source_id=287;
    check(!layout_menu_text(field,30,current,1,baseline,error)&&baseline==old,"wrong borrowed font source identity accepted");
    check(original_menu_font(287)&&original_menu_font(287)->bold&&
          std::string(original_menu_font(287)->original_ttf_uri)=="data/Fontin SmallCaps.ttf"&&
          !original_menu_font(103)->has_layout,"actual source font metadata drifted");
    check(argc==2,"actual staged shared assets required");AssetCatalog assets(argv[1]);MenuLocalization localization;
    check(localization.load(assets,"original-cache/data",0,error),error.c_str());CharacterState profile;profile.name="Actual Save";
    // The real GameplayMenus English sheet stores the Active label as ^1...^r.
    // Color constants come from data/fonts_pycst.bin, not common_text_pycst.bin;
    // exercise the shared HudText owner/provider against bytes from that corpus.
    const auto gameplaymenus=read_content(assets,"original-cache/data/text/gameplaymenus.english");
    const std::string gameplaymenus_bytes(reinterpret_cast<const char*>(gameplaymenus.data()),gameplaymenus.size());
    constexpr std::string_view active_source="^1Active^r";
    check(gameplaymenus_bytes.find(active_source)!=std::string::npos,
          "actual English GameplayMenus corpus lost the authored ^1Active^r source text");
    dh2::ui::HudTextV1* shared_text=nullptr;dh2::ui::HudTextEnvironmentV1 text_environment;
    check(localization.borrow_text(shared_text,text_environment,error)&&shared_text,
          "menu localization did not expose its shared HudText source owner");
    std::string colored_active;bool colors_changed=false;
    check(dh2::ui::localization_colors(std::string(active_source),false,text_environment.localization,
                                      colored_active,colors_changed,error),error.c_str());
    check(colors_changed&&colored_active=="<font color=\"#9CFF9A\">Active</font>",
          "actual FontTextColors.one did not produce the original #9CFF9A Active markup");
    std::string menu_source_symbol;
    check(localization.symbol("GAMEPLAYMENUS_CLASS_00",nullptr,menu_source_symbol,error),error.c_str());
    dh2::ui::HudTextV1* shared_text_after=nullptr;dh2::ui::HudTextEnvironmentV1 text_environment_after;
    check(localization.borrow_text(shared_text_after,text_environment_after,error)&&shared_text_after==shared_text,
          "font-color lookup replaced the existing shared MenuLocalization/HudText owner");
    std::string label="preserved";
    check(localization.label("menu_CharacterSheetNew/Strength/StrengthText/text",&profile,label,error),error.c_str());
    if(label!="STRENGTH")throw std::runtime_error("actual source strength label: "+label);
    std::string source_title;
    check(localization.symbol("MENU_HELP_04_TITLE",&profile,source_title,error)&&
          localization.label("menu_CharacterSheetNew/Title/txt_title",&profile,label,error)&&label==source_title&&!label.empty(),
          "source Init Title.txt_title mapping differs from menu_title.txt_title");
    check(localization.label("menu_CharacterSheetNew/PointsLeftText/text",&profile,label,error)&&!label.empty(),
          "source points-left label failed");
    profile.stats.level=2;
    std::string class_name,level_label,class_level;
    check(localization.symbol("GAMEPLAYMENUS_CLASS_00",&profile,class_name,error)&&!class_name.empty(),error.c_str());
    check(localization.symbol("GAMEPLAYMENUS_LEVEL",&profile,level_label,error)&&level_label=="Lvl:",
          "source class/level display label differs from original GAMEPLAYMENUS_LEVEL");
    check(localization.class_level(class_name,&profile,class_level,error)&&class_level=="Warrior Lvl: 2",
          "Native class-level string value differs from source display construction");
    OriginalPropertyDatabase source_tables;
    check(load_original_property_tables(assets,"original-cache/data/pydata",source_tables,error),error.c_str());
    const std::pair<std::int32_t,const char*> class_rows[]={{263,"GAMEPLAYMENUS_CLASS_00"},
        {325,"GAMEPLAYMENUS_CLASS_01"},{290,"GAMEPLAYMENUS_CLASS_02"}};
    for(const auto& item:class_rows){
        check(source_tables.characters.names[static_cast<std::size_t>(item.first)]==
              (item.first==263?"KnightPlayerBase":item.first==290?"MagePlayerBase":"RoguePlayerBase"),
              "native class id no longer indexes its original CharacterTable row");
        check(source_tables.characters.rows[static_cast<std::size_t>(item.first)][26]==
              (item.first==263?77:item.first==290?94:120),
              "CharacterTable ClassID property no longer differs from SG_GetPlayerClass row index");
        std::string native_header,source_class;
        check(localization.character_class_level(source_tables.characters,item.first,&profile,native_header,error),error.c_str());
        check(localization.symbol(item.second,&profile,source_class,error),error.c_str());
        const auto expected=source_class+" "+level_label+" 2";
        if(native_header!=expected)throw std::runtime_error("Native class row "+std::to_string(item.first)+
            " OID "+std::to_string(source_tables.characters.rows[static_cast<std::size_t>(item.first)][5])+ 
            " localized to '"+native_header+"'; source StrID oracle is '"+expected+"'");
    }
    class_level="preserved";
    check(!localization.class_level(class_name,nullptr,class_level,error)&&class_level=="preserved",
          "class-level formatter accepted a missing profile or partially published output");
    const std::pair<const char*,const char*> stats_labels[]={
        {"menu_CharacterSheetStats/GAMEPLAYMENUS_STATISTICS/text","GAMEPLAYMENUS_STATISTICS"},
        {"menu_CharacterSheetStats/AttackStats/text","GAMEPLAYMENUS_ATTACK_RATING_OFFENSE"},
        {"menu_CharacterSheetStats/DefenceStats/text","GAMEPLAYMENUS_DEFENCE_RATING_DEFENCE"},
        {"menu_CharacterSheetStats/ArmorStats/text","GAMEPLAYMENUS_ARMOR_DEFENCE"},
        {"menu_CharacterSheetStats/CriticalStats/text","GAMEPLAYMENUS_SUMMARY_M_CRIT"},
        {"menu_CharacterSheetStats/ResistancesTxt/text","GAMEPLAYMENUS_RESISTANCES"},
        {"menu_CharacterSheetStats/RH/RightHandDamage/text","GAMEPLAYMENUS_SUMMARY_R_HAND"},
        {"menu_CharacterSheetStats/LH/LeftHandDamage/text","GAMEPLAYMENUS_SUMMARY_L_HAND"},
        {"menu_CharacterSheetStats/TWOH/TwoHDamage/text","GAMEPLAYMENUS_SUMMARY_2_HAND"}
    };
    for(const auto& item:stats_labels){
        check(original_menu_label_symbol(item.first)&&std::string(original_menu_label_symbol(item.first))==item.second,
              "original CharacterSheetStats path mapped to the wrong source symbol");
        check(localization.label(item.first,&profile,label,error)&&!label.empty(),error.c_str());
    }
    check(localization.label("menu_CharacterSheetNew/unknown",&profile,label,error)&&label.empty(),"unproved path invented label");
    label="preserved";
    check(!localization.symbol("GAMEPLAYMENUS_NOT_A_REAL_SOURCE_SYMBOL",&profile,label,error)&&label=="preserved",
          "missing source localization silently published notfound");
    Presenter presenter;presenter.open();ActorState actor;OriginalCombatProperties props;
    Bindings bindings;bindings.actor=&actor;bindings.properties=&props;bindings.character=&profile;
    bindings.text=[&](const auto& path,auto& value,auto& e){return localization.label(path,&profile,value,e);};
    Frame frame;check(presenter.frame(bindings,960,640,frame,error),error.c_str());
    bool found=false;
    for(const auto& text:frame.text){
        // Independently call recovered Android align_line kernel on actual
        // SWF field margins/RECT. This tests source semantics, not just a copy
        // of the helper's implementation or assumed field-centering shortcut.
        dh2::ui::text_v1::State source;source.rect_min=text.field.local_bounds[0]*20;
        source.rect_max=text.field.local_bounds[1]*20;source.right=text.field.margins[1]*20;
        dh2::ui::text_v1::Record record;record.has_x=true;
        record.x=std::max(0.f,text.field.margins[0]+text.field.margins[2])*20;
        source.records.push_back(record);
        check(dh2::ui::text_v1::align_line(source,static_cast<std::int32_t>(text.field.align),0,record.x+33*20,error),error.c_str());
        check(layout_menu_text(text.field,33,baseline,error),error.c_str());
        const auto& matrix=text.field.matrix;
        const float original_x=source.records[0].x/20;
        const float original_y=text.field.source_height; // proved actualfont103/287 metricdelta0
        check(std::abs(baseline[0]-(matrix[0]*original_x+matrix[2]*original_y+matrix[4]))<.001f&&
              std::abs(baseline[1]-(matrix[1]*original_x+matrix[3]*original_y+matrix[5]))<.001f,
              "actual menu field layout disagrees with recovered Android align_line kernel");
        if(text.value=="STRENGTH"){
        found=true;check(layout_menu_text(text.field,33,baseline,error),error.c_str());
        check(std::isfinite(baseline[0])&&std::isfinite(baseline[1]),"actual source field invalid baseline");
        }
    }
    check(found,"presenter did not borrow actual localized static label");
    check(!localization.load(assets,"missing-source-data",0,error)&&
          localization.symbol("GAMEPLAYMENUS_STRENGTH",&profile,label,error)&&label=="STRENGTH",
          "failed corpus reload replaced previous original labels");
    std::cout<<"original menu text native tests PASS\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
