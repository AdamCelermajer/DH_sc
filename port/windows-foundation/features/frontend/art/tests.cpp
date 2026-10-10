#include "original_art.hpp"
#include "../flow/menu_flow.hpp"
#include <cmath>
#include <iostream>
using namespace dh::foundation;
using namespace dh::foundation::frontend::art;
namespace flow=dh::foundation::frontend::flow;
int main() {
    const Screen screens[]{Screen::main_menu,Screen::select_class,Screen::enter_name,Screen::start_game};
    for (auto screen:screens) {
        const auto& data=original_art(screen);
        if (data.batches.empty()||data.batches.size()!=data.bitmap_ids.size()||data.text_fields.empty()) return 1;
        for (std::size_t i=0;i<data.batches.size();++i) {
            if (data.bitmap_ids[i]!=0&&std::string(source_texture_file(data.bitmap_ids[i])).empty()) return 2;
            const auto& vertices=data.batches[i].triangles;
            if (vertices.empty()||vertices.size()%3) return 3;
            for (const auto& v:vertices) if (!std::isfinite(v.x)||!std::isfinite(v.y)||v.u<0||v.u>1||v.v<0||v.v>1) return 4;
        }
        for (const auto& region:data.hit_regions) {
            if (region.triangles.empty()||region.triangles.size()%3) return 5;
            const auto& a=region.triangles[0];const auto& b=region.triangles[1];const auto& c=region.triangles[2];
            if (!contains(region,(a.x+b.x+c.x)/3,(a.y+b.y+c.y)/3)) return 6;
        }
        HudGeometry projected; std::string error;
        if (!geometry(screen,960,640,projected,error)||projected.batches.size()!=data.batches.size()) return 7;
        if (projected.batches[0].triangles[0].x!=data.batches[0].triangles[0].x*2) return 8;
        const auto before=projected.batches.size();
        if (geometry(screen,0,640,projected,error)||projected.batches.size()!=before) return 9;
    }
    TextField field;field.path="menu_MainMenu/btn_MENU_OPTIONS/text";
    if (source_english_label(field)!="Options") return 10;
    flow::PresentationFacts facts;
    for(auto screen:screens){
        const char* name=screen==Screen::main_menu?"menu_MainMenu":screen==Screen::enter_name?"menu_EnterName":screen==Screen::select_class?"menu_SelectClass":"menu_StartGame";
        auto state=flow::presentation(name,facts);ScreenArt runtime;std::string error;
        if(!compose(screen,state,runtime,error)){std::cerr<<error;return 11;}
        if(runtime.batches.empty()||runtime.batch_colors.size()!=runtime.batches.size()||runtime.bitmap_ids.size()!=runtime.batches.size())return 12;
        for(const auto& batch:runtime.batches)if(batch.triangles.size()%3)return 13;
        if(screen==Screen::enter_name){
            bool background=false,key=false,glyph=false;
            for(const auto& batch:runtime.batches){background|=batch.role.find("BrownBG")!=std::string::npos;key|=batch.shape_id==73;glyph|=batch.shape_id==90;}
            if(!background||!key||!glyph)return 14;
            for(const auto& region:runtime.hit_regions)if(region.button_path.find("LowerCase")!=std::string::npos)return 15;
            bool gold=false;for(const auto& batch:runtime.batches){if(batch.shape_id==63)gold=true;if(batch.shape_id==65)return 18;}if(!gold)return 19;
        }
        if(screen==Screen::main_menu)for(const auto& f:runtime.text_fields)if(normalize_source_path(f.path).find("PlayerInfos")!=std::string::npos)return 16;
        if(screen==Screen::select_class){std::array<float,4> bounds{};if(!class_scene_bounds(runtime,bounds)||bounds[1]<=bounds[0]||bounds[3]<=bounds[2])return 17;bool plate=false;for(const auto& batch:runtime.batches)plate|=batch.shape_id==402;if(!plate)return 20;std::cout<<"Original class_select bounds "<<bounds[0]<<","<<bounds[1]<<","<<bounds[2]<<","<<bounds[3]<<"\n";}
    }
    std::cout << "Original frontend art: 4 screens, contours, atlas IDs, labels, hit regions, and projection passed\n";
}
