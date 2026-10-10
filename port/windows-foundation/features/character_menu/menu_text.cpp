#include "menu_text.hpp"
#include "../../../engine-ui/localization_parse_ex_v1.hpp"
#include "../../asset_catalog.hpp"
#include "../../content_paths.hpp"
#include "../../../engine-ui/localization.hpp"
#include "../../../script-runtime/script_constants.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <stdexcept>
namespace dh::foundation::character_menu {
const MenuFontDefinition* original_menu_font(std::uint32_t id)noexcept {
    // Actual DefineFont3 flags04/05, both HasLayout0. Source get_fontfile42b38c
    // default Latin branch resolves data/<NAME>.ttf, ignoring style suffix.
    static const MenuFontDefinition fonts[]={{103,"Fontin SmallCaps","data/Fontin SmallCaps.ttf",false,false,true,false,0,0},
        {287,"Fontin SmallCaps","data/Fontin SmallCaps.ttf",true,false,true,false,0,0}};
    for(const auto& font:fonts)if(font.source_id==id)return &font;return nullptr;
}
bool layout_menu_text(const MenuTextField& field,float advance,const MenuFontDefinition& font,
    float root_scale,std::array<float,2>& baseline,std::string& error){
    if(font.source_id!=field.font_id||!std::isfinite(advance)||advance<0||
       !std::isfinite(field.source_height)||field.source_height<=0||field.align>2||
       !std::isfinite(font.descent)||!std::isfinite(font.leading)||!std::isfinite(root_scale)||root_scale<=0){
        error="Source menu font/run identity or metrics unavailable";return false;
    }
    for(float n:field.local_bounds)if(!std::isfinite(n)){error="Invalid source menu local RECT";return false;}
    for(float n:field.matrix)if(!std::isfinite(n)){error="Invalid source menu text matrix";return false;}
    for(float n:field.margins)if(!std::isfinite(n)){error="Invalid source menu paragraph margins";return false;}
    const auto& rect=field.local_bounds;
    if(rect[1]<=rect[0]||rect[3]<=rect[2]){error="Empty source menu text RECT";return false;}
    // Actual Android format_text7.8efb8 clears x/y0 before append_text.
    // Stock GameSWF adds RECT minima; this recovered source does NOT.
    float x=std::max(0.f,field.margins[0]+field.margins[2]);
    const float extra=(rect[1]-rect[0]-field.margins[1])-(x+advance)-4.f;
    if(field.align==1)x+=extra;else if(field.align==2)x+=extra*.5f;
    const float font_scale=field.source_height/(root_scale*1024.f)/(font.define_font3?20.f:1.f);
    const float y=field.source_height+(font.leading-font.descent)*font_scale;
    const auto& m=field.matrix;const std::array<float,2> next{m[0]*x+m[2]*y+m[4],m[1]*x+m[3]*y+m[5]};
    if(!std::isfinite(next[0])||!std::isfinite(next[1])){error="Source menu text baseline overflow";return false;}
    baseline=next;error.clear();return true;
}
bool layout_menu_text(const MenuTextField& field,float advance,std::array<float,2>& baseline,std::string& error){
    const auto* font=original_menu_font(field.font_id);
    if(!font){error="Source menu font definition is unavailable";return false;}
    // This is solely the proved first-line prefix with actual font103/287
    // metricdelta0. Root scale cancels; no native root owner is fabricated.
    if(font->descent!=0||font->leading!=0){error="Nonzero menu font metrics require actual root scale borrower";return false;}
    return layout_menu_text(field,advance,*font,1,baseline,error);
}
const char* original_menu_label_symbol(const std::string& path)noexcept {
    struct Label {const char* source_path;const char* symbol;};
    // Original CharacterSheetNew.Init source actions77070..77299.
    static const Label labels[]{
        {"menu_CharacterSheetNew/Title/","MENU_HELP_04_TITLE"},
        {"/PointsLeftText/","GAMEPLAYMENUS_POINTS_LEFT"},
        {"/Strength/StrengthText/","GAMEPLAYMENUS_STRENGTH"},
        {"/Dexterity/DexterityText/","GAMEPLAYMENUS_DEXTERITY"},
        {"/Endurance/EnduranceText/","GAMEPLAYMENUS_ENDURANCE"},
        {"/Energy/EnergyText/","GAMEPLAYMENUS_ENERGY"}
        ,{"menu_CharacterSheetStats/GAMEPLAYMENUS_STATISTICS/","GAMEPLAYMENUS_STATISTICS"}
        ,{"menu_CharacterSheetStats/AttackStats/","GAMEPLAYMENUS_ATTACK_RATING_OFFENSE"}
        ,{"menu_CharacterSheetStats/DefenceStats/","GAMEPLAYMENUS_DEFENCE_RATING_DEFENCE"}
        ,{"menu_CharacterSheetStats/ArmorStats/","GAMEPLAYMENUS_ARMOR_DEFENCE"}
        ,{"menu_CharacterSheetStats/CriticalStats/","GAMEPLAYMENUS_SUMMARY_M_CRIT"}
        ,{"menu_CharacterSheetStats/ResistancesTxt/","GAMEPLAYMENUS_RESISTANCES"}
        ,{"menu_CharacterSheetStats/RH/RightHandDamage/","GAMEPLAYMENUS_SUMMARY_R_HAND"}
        ,{"menu_CharacterSheetStats/LH/LeftHandDamage/","GAMEPLAYMENUS_SUMMARY_L_HAND"}
        ,{"menu_CharacterSheetStats/TWOH/TwoHDamage/","GAMEPLAYMENUS_SUMMARY_2_HAND"}
        // InventorySheetMain onShow actions (authored-actions.txt 00019d7f..00019dcf): NativeGetStringFromSymbol.
        ,{"menu_InventorySheetMain/btn_GAMEPLAYMENUS_AUTOEQUIP_ALL/","GAMEPLAYMENUS_AUTOEQUIP_ALL"}
        ,{"menu_InventorySheetMain/Title/","GAMEPLAYMENUS_INVENTORY_TITLE"}
        // P16 map (MenuCharMenu_Map sheet menu_MapSheet): page title and control captions by symbol.
        ,{"menu_MapSheet/menu_title/","GAMEPLAYMENUS_MAP_TITLE"}
        ,{"menu_MapSheet/btn_Legend/TextBox/","MENU_SHOW_LEGEND"}
        ,{"menu_MapSheet/btn_ResetZoom/TextBox/","MENU_MAP_RESET_ZOOM"}
        ,{"menu_MapSheet/LegendPopup/WarningBox/Title","MENU_MAP_LEGEND"}
    };
    // P16 map legend captions: iconTextN of MenuCharMenu_Map LegendPopup/WarningBox, matched by exact index
    // (substring matching would also catch iconText10..12). Order is the authored legend reading order
    // (left column 1..5 = Checkpoint, Entrance, Exit, NPC, Merchant; right column 7..9 = Character, Enemy,
    // Objective). P16 MAPFIX: index 10 is the Arrow row (icon 13, right column row 4 at y 196.75). Its caption
    // "Unexplored Area" is gameplaymenus GAMEPLAYMENUS_LEGEND_ARROW (string 530 after "Zone" 529).
    static const std::string legendPrefix="menu_MapSheet/LegendPopup/WarningBox/iconText";
    if(path.compare(0,legendPrefix.size(),legendPrefix)==0&&path.size()>legendPrefix.size()) {
        const auto digits=path.substr(legendPrefix.size());
        if(digits.find_first_not_of("0123456789")!=std::string::npos) return nullptr;
        switch(std::stoi(digits)) {
        case 1: return "MENU_MAP_CHECKPOINT";
        case 2: return "MENU_MAP_ENTRANCE";
        case 3: return "MENU_MAP_EXIT";
        case 4: return "MENU_MAP_QUESTGIVER";
        case 5: return "MENU_MAP_MERCHANT";
        case 7: return "MENU_MAP_CHAR";
        case 8: return "MENU_MAP_ENEMIES";
        case 9: return "MENU_MAP_OBJ";
        case 10: return "GAMEPLAYMENUS_LEGEND_ARROW";
        default: return nullptr;
        }
    }
    for(const auto& label:labels)if(path.find(label.source_path)!=std::string::npos)return label.symbol;
    return nullptr;
}
struct MenuLocalization::Impl {
    std::unique_ptr<AssetCatalog> assets;std::string root;dh2::ui::HudTextV1 localization;
    dh2_script_constants* constants=dh2_script_constants_create();const CharacterState* profile=nullptr;bool ready=false;
    ~Impl(){dh2_script_constants_destroy(constants);}
    dh2::ui::LocalizationServices services(){
        dh2::ui::LocalizationServices s;s.context=this;
        s.open=[](void* c,const char* uri,bool& found,std::vector<std::uint8_t>& bytes,std::uintptr_t& lease,std::string& e){
            auto& self=*static_cast<Impl*>(c);found=false;lease=0;
            try{bytes=read_content(*self.assets,self.root+"/"+uri);found=true;lease=1;e.clear();return true;}
            catch(const std::exception& x){e=x.what();return false;}
        };
        s.close=[](void*,std::uintptr_t lease,std::string& e){if(lease!=1){e="Invalid original menu corpus lease";return false;}return true;};
        // Explicit standalone text decoder debug endpoint, not a reconstructed
        // DebugSwitches gameplay owner. Source ignores returned tracing value.
        s.debug=[](void*,const char*,std::string&){return true;};
        s.constant=[](void* c,const char* group,const char* key,std::uint32_t& out,std::string& e){
            std::int32_t raw=0;if(dh2_script_constants_get(static_cast<Impl*>(c)->constants,group,key,&raw)){
                e="Missing original menu text constant";return false;}
            std::memcpy(&out,&raw,4);return true;
        };
        s.player_character=[](void* c,std::uintptr_t& out,std::string&){out=reinterpret_cast<std::uintptr_t>(static_cast<Impl*>(c)->profile);return true;};
        s.player_name=[](void* c,std::uintptr_t actual,std::string& out,std::string& e){
            const auto* profile=static_cast<Impl*>(c)->profile;
            if(!profile||actual!=reinterpret_cast<std::uintptr_t>(profile)){e="Menu text player borrower changed";return false;}
            out=profile->name;return true;
        };return s;
    }
};
MenuLocalization::MenuLocalization()=default;MenuLocalization::~MenuLocalization()=default;
MenuLocalization::MenuLocalization(MenuLocalization&&)noexcept=default;
MenuLocalization& MenuLocalization::operator=(MenuLocalization&&)noexcept=default;
bool MenuLocalization::load(const AssetCatalog& assets,const std::string& root,std::int32_t pack,std::string& error){
    try{
        auto next=std::make_unique<Impl>();if(!next->constants)throw std::runtime_error("Menu text constant owner unavailable");
        next->assets=std::make_unique<AssetCatalog>(assets);next->root=normalize_content_uri(root);
        auto read=[&](const char* file){return read_content(assets,next->root+"/pydata/"+file);};
        auto records=read("common_text_pyarray.bin"),names=read("common_text_pyarraynames.bin"),schema=read("common_text_pystructnames.bin");
        const auto load_constants=[&](const std::vector<std::uint8_t>& bytes,const char* label){
            dh2_script_constants_reload receipt{};
            if(bytes.size()>UINT32_MAX||
               dh2_script_constants_load(next->constants,bytes.data(),static_cast<std::uint32_t>(bytes.size()),&receipt)||
               receipt.consumed!=bytes.size()||receipt.groups_complete==0){
                error=std::string("Original menu ")+label+" constants rejected";return false;
            }
            return true;
        };
        const auto common_constants=read("common_text_pycst.bin");
        const auto font_constants=read_content(assets,"data/fonts_pycst.bin");
        if(!load_constants(common_constants,"common_text")||!load_constants(font_constants,"font"))return false;
        // The source preload converts ^0..^9 using FontTextColors from the
        // separate fonts_pycst asset.  The native constants loader returns a
        // successful zero for an absent key, so explicitly reject a missing
        // source colour group instead of publishing the resulting black tag.
        std::int32_t source_color_one=0;
        if(dh2_script_constants_get(next->constants,"FontTextColors","one",&source_color_one)!=0||
           source_color_one==0){
            error="Original menu font constants omit FontTextColors.one";return false;
        }
        auto span=[](const auto& b){return dh2::ui::LocalizationBytes{b.data(),b.size()};};
        if(!next->localization.load(span(records),span(names),span(schema),error)||!next->localization.switch_pack(pack,false,error))return false;
        next->ready=true;impl_=std::move(next);error.clear();return true;
    }catch(const std::exception& x){error=x.what();return false;}
}
bool MenuLocalization::symbol(const std::string& symbol,const CharacterState* profile,std::string& value,std::string& error){
    if(!impl_||!impl_->ready){error="Original menu localization corpus is unbound";return false;}
    const auto* previous_profile=impl_->profile;
    impl_->profile=profile;dh2::ui::LocalizationResult result;auto services=impl_->services();
    const bool ok=impl_->localization.native_string(symbol,services,result,error);impl_->profile=previous_profile;
    if(!ok)return false;if(!result.found){error="Original menu localization symbol absent: "+symbol;return false;}
    value=std::move(result.text);error.clear();return true;
}
bool MenuLocalization::class_level(const std::string& localized_class,const CharacterState* profile,
    std::string& value,std::string& error){
    if(!profile){error="Original class-level header requires the same saved profile";return false;}
    if(localized_class.empty()){value.clear();error.clear();return true;}
    std::string level_label;
    if(!symbol("GAMEPLAYMENUS_LEVEL",profile,level_label,error))return false;
    // NativeGetSaveSlotDetails and its sibling slot-list path build precisely:
    // class name + space + GAMEPLAYMENUS_LEVEL + space + sprintf("%d", level).
    value=localized_class+" "+level_label+" "+std::to_string(profile->stats.level);
    error.clear();return true;
}
bool MenuLocalization::character_class_level(const dh2::data::CharacterTable& characters,
    std::int32_t source_class_id,const CharacterState* profile,std::string& value,std::string& error){
    if(!profile){error="Original class-level header requires the same saved profile";return false;}
    if(source_class_id<0||static_cast<std::size_t>(source_class_id)>=characters.rows.size()||
       characters.names.size()!=characters.rows.size()){
        error="Native Character::GetClassName class row is outside the source CharacterTable";return false;
    }
    const auto field=std::find(characters.fields.begin(),characters.fields.end(),"ClassString");
    if(field==characters.fields.end()||static_cast<std::size_t>(field-characters.fields.begin())!=5){
        error="Native Character::GetClassName CharacterTable field layout differs";return false;
    }
    const auto oid=characters.rows[static_cast<std::size_t>(source_class_id)][5];
    std::string localized_class;
    if(!string_id(oid,localized_class,error))return false;
    return class_level(localized_class,profile,value,error);
}
bool MenuLocalization::label(const std::string& path,const CharacterState* profile,std::string& value,std::string& error){
    const auto* symbol_name=original_menu_label_symbol(path);
    if(!symbol_name){value.clear();error.clear();return true;}
    return symbol(symbol_name,profile,value,error);
}
bool MenuLocalization::string_id(std::int32_t id,std::string& value,std::string& error){
    if(!impl_||!impl_->ready||id<0){error="Original menu numeric text source unavailable";return false;}
    std::string next;auto services=impl_->services();bool is_null=false;
    if(!impl_->localization.integer_string(id,services,next,is_null,error))return false;
    if(is_null||next=="#!WTF!#"||next=="#!SNL!#"){error="Original numeric menu text OID has no localized value";return false;}
    value=std::move(next);error.clear();return true;
}
bool MenuLocalization::constant(const char* group,const char* key,std::int32_t& value,std::string& error){
    if(!impl_||!impl_->ready||!group||!key){error="Original menu localization corpus is unbound";return false;}
    std::int32_t raw=0;
    if(dh2_script_constants_get(impl_->constants,group,key,&raw)){error="Missing original menu text constant";return false;}
    value=raw;error.clear();return true;
}
bool MenuLocalization::parsed_symbol(const std::string& symbol,std::int32_t number,std::string& value,std::string& error){
    if(!impl_||!impl_->ready){error="Original menu localization corpus is unbound";return false;}
    dh2::ui::HudTextV1* text=nullptr;dh2::ui::HudTextEnvironmentV1 environment;std::string next;
    if(!borrow_text(text,environment,error))return false;
    const std::vector<dh2::ui::LocalizationArgumentV1> arguments{dh2::ui::LocalizationArgumentV1{static_cast<float>(number),false,{}}};
    if(!text->parsed_string_v4(symbol,arguments,environment,next,error))return false;
    value=std::move(next);error.clear();return true;
}
bool MenuLocalization::bind_profile(const CharacterState* profile,std::string& error){
    if(!impl_||!impl_->ready){error="Original menu text cache is unbound";return false;}
    impl_->profile=profile;error.clear();return true;
}
bool MenuLocalization::borrow_text(dh2::ui::HudTextV1*& text,dh2::ui::HudTextEnvironmentV1& environment,std::string& error){
    if(!impl_||!impl_->ready){error="Original menu text cache is unbound";return false;}
    dh2::ui::HudTextEnvironmentV1 next;next.localization=impl_->services();
    text=&impl_->localization;environment=std::move(next);error.clear();return true;
}
}
