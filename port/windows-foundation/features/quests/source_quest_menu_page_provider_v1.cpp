#include "source_quest_menu_page_provider_v1.hpp"
#include "runtime_quest_menu_art_v1.hpp"
#include <algorithm>
#include <cmath>

namespace dh::foundation {
namespace {
bool fail(std::string& error,const char* message){error=message;return false;}

float cross(float ax,float ay,float bx,float by,float px,float py) {
    return (bx-ax)*(py-ay)-(by-ay)*(px-ax);
}

bool in_triangle(float x,float y,const HudGeometryVertex& a,
                 const HudGeometryVertex& b,const HudGeometryVertex& c) {
    constexpr float epsilon=1e-4f;
    const auto d1=cross(a.x,a.y,b.x,b.y,x,y);
    const auto d2=cross(b.x,b.y,c.x,c.y,x,y);
    const auto d3=cross(c.x,c.y,a.x,a.y,x,y);
    const bool negative=d1 < -epsilon || d2 < -epsilon || d3 < -epsilon;
    const bool positive=d1 > epsilon || d2 > epsilon || d3 > epsilon;
    return !(negative&&positive);
}

bool in_activate_source_hitzone(float x,float y) {
    // DefineShape3 #82 is the original transparent rectangular hit fill
    // ([0,2320]x[0,1039] twips), under page(170,2126), btn_Activate
    // (6356,3300), and hitzone (1.229309,0.861938,-208,-171).
    // The composed root-stage matrix is therefore [1.229309,0,.861938,
    // 6318,5255] in twips. Source menu geometry and incoming authored points
    // are pixels, so perform the final SWF twips/20 conversion here.
    constexpr float a=1.22930908203125f;
    constexpr float d=0.8619384765625f;
    constexpr float tx_twips=6318.f,ty_twips=5255.f;
    constexpr float x0=tx_twips/20.f;
    constexpr float x1=(tx_twips+a*2320.f)/20.f;
    constexpr float y0=ty_twips/20.f;
    constexpr float y1=(ty_twips+d*1039.f)/20.f;
    return x>=x0&&x<=x1&&y>=y0&&y<=y1;
}
}

bool resolve_source_quest_menu_hit_v1(const RuntimeQuestMenuFrameV1& frame,
    float authored_stage_x,float authored_stage_y,SourceQuestMenuHitRouteV1& out,
    std::string& error) {
    if(frame.availability!=RuntimeQuestMenuAvailabilityV1::ready)
        return fail(error,"Quest source hit resolution requires a ready authored page frame");
    if(!std::isfinite(authored_stage_x)||!std::isfinite(authored_stage_y))
        return fail(error,"Quest source hit coordinates must be finite authored-stage pixels");
    SourceQuestMenuHitRouteV1 staged;
    if(authored_stage_x<0.f||authored_stage_x>480.f||
       authored_stage_y<0.f||authored_stage_y>320.f){out=staged;error.clear();return true;}

    // Activate is a sibling control, visible only for a selected Assigned row (activation_visible is set by the page).
    if(frame.selection&&frame.selection->activation_visible&&in_activate_source_hitzone(
           authored_stage_x,authored_stage_y)) {
        staged.handled=true;staged.hit=RuntimeQuestMenuHitV1::activate_release;
        staged.quest=frame.selection->row.id;out=staged;error.clear();return true;
    }

    // Row contours: the unselected button shape (parent 0 = Assigned, parent 1 = Completed).
    const auto& art=original_runtime_quest_menu_art_v1();
    const auto& source_rows=art.row_solids[0];
    const auto hit_shape=std::find_if(source_rows.begin(),source_rows.end(),[](const auto& solid){
        return solid.geometry.shape_id==106&&solid.geometry.role=="btnQuests/d9";
    });
    if(hit_shape==source_rows.end()||hit_shape->geometry.triangles.empty()||
       hit_shape->geometry.triangles.size()%3!=0)
        return fail(error,"Original Quest row hit contour shape 106 is unavailable or malformed");
    bool identity_ok=true;
    auto hit_list=[&](const std::vector<RuntimeQuestMenuRowV1>& rows,std::size_t category)->bool {
        auto parent=art.row_parent_matrices[category];
        if(category==1)parent[5]+=completed_list_shift_v1(frame.rows.size()); // same shift as the drawn Completed list
        for(const auto& row:rows) {
            if(row.id.collection!=frame.collection||row.id.difficulty!=frame.difficulty) {
                identity_ok=false;return false;
            }
            const float offset=static_cast<float>(row.authored_row_index*frame.art.row_step_swf_pixels);
            for(std::size_t i=0;i<hit_shape->geometry.triangles.size();i+=3) {
                auto project=[&](const HudGeometryVertex& vertex){
                    return HudGeometryVertex{
                        parent[0]*vertex.x+parent[2]*vertex.y+parent[4]+parent[2]*offset,
                        parent[1]*vertex.x+parent[3]*vertex.y+parent[5]+parent[3]*offset,
                        0.f,0.f};
                };
                const auto a=project(hit_shape->geometry.triangles[i]);
                const auto b=project(hit_shape->geometry.triangles[i+1]);
                const auto c=project(hit_shape->geometry.triangles[i+2]);
                if(in_triangle(authored_stage_x,authored_stage_y,a,b,c)) {
                    staged.handled=true;staged.hit=RuntimeQuestMenuHitV1::quest_row_release;
                    staged.quest=row.id;return true;
                }
            }
        }
        return false;
    };
    if(!hit_list(frame.rows,0)&&identity_ok)
        hit_list(frame.completed_rows,1);
    if(!identity_ok)
        return fail(error,"Quest source hit row identity differs from the current page owner/category");
    out=staged;error.clear();return true;
}

SourceQuestMenuHitResolverV1 source_quest_menu_hit_resolver_v1(
    const std::shared_ptr<RuntimeQuestCharacterMenuBindingV1>& binding) {
    if(!binding)return {};
    return [binding](float x,float y,SourceQuestMenuHitRouteV1& route,
                     std::string& error){
        const auto* frame=binding->current_frame();
        if(!frame)return fail(error,"Quest source hit resolver lost its runtime page frame");
        return resolve_source_quest_menu_hit_v1(*frame,x,y,route,error);
    };
}

bool bind_source_quest_menu_page_provider_v1(
    const std::shared_ptr<RuntimeQuestCharacterMenuBindingV1>& binding,
    const std::shared_ptr<CharacterState>& character,
    const std::shared_ptr<void>& owner,
    character_menu::SourcePageProviderV1& out,
    std::string& error) {
    auto resolver=source_quest_menu_hit_resolver_v1(binding);
    if(!resolver)return fail(error,"Quest source menu provider requires a retained runtime page binding");
    return bind_source_quest_menu_page_provider_v1(
        binding,character,owner,std::move(resolver),out,error);
}

bool bind_source_quest_menu_page_provider_v1(
    const std::shared_ptr<RuntimeQuestCharacterMenuBindingV1>& binding,
    const std::shared_ptr<CharacterState>& character,
    const std::shared_ptr<void>& owner,
    SourceQuestMenuHitResolverV1 hit_resolver,
    character_menu::SourcePageProviderV1& out,
    std::string& error) {
    if(!binding||!character||!owner||!hit_resolver||
       !binding->retains_source_owner(character.get(),owner))
        return fail(error,"Quest menu page provider requires the same retained CharacterState/source owner and an authored hit resolver");
    if(!binding->load_progress_from_character(error))return false;

    character_menu::SourcePageProviderV1 staged;
    staged.owner=owner;
    staged.ready=[binding](std::string& message){
        return binding->source_menu_page_ready(message);
    };
    staged.append=[binding](character_menu::Frame& frame,std::string& message){
        return binding->append_source_menu_page(frame,message);
    };
    staged.release=[binding,resolver=std::move(hit_resolver)](
        float x,float y,std::string& message) mutable {
        SourceQuestMenuHitRouteV1 route;
        try {
            if(!resolver(x,y,route,message)) {
                if(message.empty())message="Original Quest menu hit resolver rejected the release";
                return false;
            }
        } catch(...) {
            message="Original Quest menu hit resolver threw";
            return false;
        }
        if(!route.handled){message.clear();return true;}
        return binding->route_hit(route.hit,route.quest,message);
    };
    out=std::move(staged);
    error.clear();return true;
}

} // namespace dh::foundation
