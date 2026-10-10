#pragma once
#include <array>
#include <cstdint>
namespace dh2::ui {
// MenuManager::Init 42f304 and s_SWFfileToLoad 956a64. Width is the
// nativeSetPhone physical pixel cell, not a scaled authored viewport.
struct SourceMenuVariantV132 {
 std::array<const char*,4> uri{};
 bool base_hud{};
 const char* hud_digest{};
 std::array<float,4> movie_rect{0.f,9600.f,0.f,6400.f};
};
inline SourceMenuVariantV132 source_menu_variant_v132(std::int32_t width,std::int32_t language,bool htc,bool no_igp){
 SourceMenuVariantV132 out;
 if(width==854)out.uri={"data/menus/dqshared_droid.swf","data/menus/dqcharmenu_droid.swf","data/menus/dqmenus_droid.swf","data/menus/dqhud_droid.swf"};
 else if(width==800)out.uri={"data/menus/dqshared_i9000.swf","data/menus/dqcharmenu_i9000.swf",htc?"data/menus/dqmenus_i9000.swf":no_igp?"data/menus/dqmenus_i9000_lg.swf":"data/menus/dqmenus_i9000.swf","data/menus/dqhud_i9000.swf"};
 else {out.uri={"data/menus/dqshared.swf","data/menus/dqcharmenu.swf",width==960?"data/menus/dqmenus.swf":language==5?"data/menus/dqmenus_Kor.swf":language==4?"data/menus/dqmenus_jp.swf":"data/menus/dqmenus.swf","data/menus/dqhud.swf"};out.base_hud=true;out.movie_rect={0.f,20480.f,0.f,15360.f};}
 out.hud_digest=out.base_hud?"3ab455733367acf657df6eaaec39c93fed34cbcf519255e02cf488c07a96f2bb":"a4ffacd1abdf7c9b2ba19c46ebb81c60c100458731a4cdba5880391b9c11b238";
 return out;
}
}
