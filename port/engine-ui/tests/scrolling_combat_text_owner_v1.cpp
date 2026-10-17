#include "../scrolling_combat_text_owner_v1.hpp"
#include "../scrolling_combat_text_bridge_v1.hpp"
#include "../scrolling_combat_text_projection_v1.hpp"
#include "../scrolling_combat_text_position_v1.hpp"
#include "../../scene-materials/scene.hpp"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

namespace {
using namespace dh2::ui;
struct Fixture {
    std::vector<std::string> calls;
    std::vector<std::string> events;
    bool follower{};
    bool dual{};
    bool local{};
    bool fail_property{};
    std::int32_t fear{256};
    std::int32_t slow{256};
    static bool is_follower(void* raw, std::uintptr_t, bool& out, std::string&) {
        auto& f = *static_cast<Fixture*>(raw); f.calls.push_back("follower");
        out = f.follower; return true;
    }
    static bool position(void* raw, std::uintptr_t, float out[3], std::string&) {
        auto& f = *static_cast<Fixture*>(raw); f.calls.push_back("position");
        out[0] = 1.f; out[1] = 2.f; out[2] = 9.f; return true;
    }
    static bool property(void* raw, std::uintptr_t, std::int32_t id,
                         std::int32_t& out, std::string& error) {
        auto& f = *static_cast<Fixture*>(raw); f.calls.push_back("property:" + std::to_string(id));
        if (f.fail_property) { error = "property fixture failure"; return false; }
        out = (id == 143 || id == 187) ? f.fear : f.slow; return true;
    }
    static bool dual_wield(void* raw, std::uintptr_t, bool& out, std::string&) {
        auto& f = *static_cast<Fixture*>(raw); f.calls.push_back("dual"); out = f.dual; return true;
    }
    static bool local_player(void* raw, std::uintptr_t, bool& out, std::string&) {
        auto& f = *static_cast<Fixture*>(raw); f.calls.push_back("local"); out = f.local; return true;
    }
    static bool constant(void* raw, const char* group, const char* name,
                         std::int32_t& out, std::string&) {
        auto& f = *static_cast<Fixture*>(raw);
        f.calls.push_back(std::string("constant:") + group + ":" + name);
        out = static_cast<std::int32_t>(f.calls.size()); return true;
    }
    static bool localized(void* raw, std::int32_t id, std::string& out, std::string&) {
        auto& f = *static_cast<Fixture*>(raw); f.calls.push_back("string:" + std::to_string(id));
        out = "localized-" + std::to_string(id); return true;
    }
    static bool style(void* raw, const char* name, std::int32_t& out, std::string&) {
        auto& f = *static_cast<Fixture*>(raw); f.calls.push_back(std::string("style:") + name);
        out = static_cast<std::int32_t>(f.calls.size()); return true;
    }
    static bool play_text(void* raw, std::int32_t, const float xyz[3],
                         const char* text, std::int32_t, std::string&) {
        auto& f = *static_cast<Fixture*>(raw);
        if (xyz[0] != 1.f || xyz[1] != 2.f || xyz[2] != 9.f) return false;
        f.events.push_back(std::string("text:") + text); return true;
    }
    static bool play_value(void* raw, std::int32_t, const float xyz[3],
                          std::int32_t value, std::int32_t, std::string&) {
        auto& f = *static_cast<Fixture*>(raw);
        if (xyz[0] != 1.f || xyz[1] != 2.f || xyz[2] != 9.f) return false;
        f.events.push_back("value:" + std::to_string(value)); return true;
    }
    static bool localized_formatted(void* raw,std::int32_t id,std::int32_t amount,
                                    std::string& out,std::string&) {
        auto& f=*static_cast<Fixture*>(raw);
        f.calls.push_back("format:"+std::to_string(id)+":"+std::to_string(amount));
        out="xp-"+std::to_string(amount);return true;
    }
    static bool decode_color(void*,std::int32_t color,std::uint8_t rgba[4],std::string&) {
        decode_scrolling_combat_text_color_rgb_v1(color,rgba);return true;
    }
    static bool play_screen(void* raw,const char* style,std::uint32_t slot,float x,float y,
                            const char* text,const std::uint8_t rgba[4],std::string&) {
        auto& f=*static_cast<Fixture*>(raw);
        if(!style||slot!=UINT32_MAX||std::fabs(x-50.55f)>0.1f||
           std::fabs(y-39.11f)>0.1f||rgba[3]!=255)return false;
        f.events.push_back(std::string("screen:")+style+":"+text+":"+
            std::to_string(rgba[0])+":"+std::to_string(rgba[1])+":"+
            std::to_string(rgba[2]));return true;
    }
    ScrollingCombatTextServicesV1 services() {
        return {this, is_follower, position, property, dual_wield, local_player,
                constant, localized, style, play_text, play_value,nullptr,nullptr,
                localized_formatted};
    }
};
struct PositionFixture {
    ScrollingCombatTextPositionFactsV1 facts{};
    static bool lookup(void* raw,std::uintptr_t,
                       ScrollingCombatTextPositionFactsV1& out,std::string&) {
        out=static_cast<PositionFixture*>(raw)->facts;return true;
    }
};
void require(bool condition, const char* message) {
    if (!condition) { std::cerr << message << '\n'; std::exit(1); }
}
}

int main() {
    {
        dh2::scene::Scene graph;
        graph.graph.resize(5);
        graph.graph[0].name="actor_root";graph.graph[0].parent=-1;
        graph.graph[0].world[12]=100.f;graph.graph[0].world[13]=20.f;
        graph.graph[0].world[14]=30.f;
        graph.graph[1].name="armature";graph.graph[1].parent=0;
        graph.graph[1].world[12]=110.f;graph.graph[1].world[13]=25.f;
        graph.graph[1].world[14]=31.f;
        graph.graph[2].name="target_node";graph.graph[2].parent=1;
        // SceneBinding's accumulated world matrix, not the node-local point.
        graph.graph[2].world[12]=114.f;graph.graph[2].world[13]=27.f;
        graph.graph[2].world[14]=34.f;
        graph.graph[3].name="target_node";graph.graph[3].parent=0;
        graph.graph[3].world[12]=150.f;graph.graph[3].world[13]=20.f;
        graph.graph[3].world[14]=30.f;
        graph.graph[4].name="second_root";graph.graph[4].parent=-1;
        graph.graph[4].world[12]=200.f;
        bool present=false;std::uint32_t node_index=UINT32_MAX;
        float world[3]={-1.f,-1.f,-1.f};std::string node_error;
        require(resolve_scrolling_combat_target_node_v1(
                    graph,present,node_index,world,node_error)&&present&&
                node_index==2&&world[0]==114.f&&world[1]==27.f&&world[2]==34.f,
                "target_node lookup did not use first depth-first match and its accumulated world position");
        graph.graph[2].name="Target_Node";
        graph.graph[3].name="target_node_suffix";
        present=true;node_index=0;world[0]=9.f;
        require(resolve_scrolling_combat_target_node_v1(
                    graph,present,node_index,world,node_error)&&!present&&
                node_index==UINT32_MAX&&world[0]==0.f&&world[1]==0.f&&world[2]==0.f,
                "missing target_node must exactly report absent source pointer");
    }
    {
        const float vp[16]={1,0,0,0, 0,1,0,0, 0,0,1,1, 0,0,0,0};
        float point[3]={0,0,2},screen[2]{};bool inside=false;std::string projection_error;
        require(project_world_to_screen_pixels_v1(vp,100,80,point,screen,&inside,projection_error)&&
                inside&&screen[0]==50.f&&screen[1]==40.f,
                "positive-forward camera projection did not map center to viewport center");
        point[0]=4.f;
        require(project_world_to_screen_pixels_v1(vp,100,80,point,screen,&inside,projection_error)&&
                !inside&&screen[0]==150.f,
                "offscreen projection must retain source-style unclamped screen coordinates");
        point[2]=-1.f;
        require(!project_world_to_screen_pixels_v1(vp,100,80,point,screen,&inside,projection_error),
                "behind-camera world point should fail closed");
    }
    {
        const float game_object[3]={10.f,20.f,30.f};
        const float bounds[6]={-1.f,-2.f,-3.f,1.f,2.f,7.f};
        dh2::scene::Scene graph;graph.graph.resize(2);
        graph.graph[0].name="actor_root";graph.graph[0].parent=-1;
        graph.graph[0].world[12]=9.f;graph.graph[0].world[13]=8.f;
        graph.graph[0].world[14]=7.f;
        graph.graph[1].name="target_node";graph.graph[1].parent=0;
        graph.graph[1].world[12]=1.f;graph.graph[1].world[13]=2.f;
        graph.graph[1].world[14]=3.f;
        ScrollingCombatTextPositionFactsV1 facts{
            0x1234,game_object,&graph,bounds,1};
        float world[3]{};std::string position_error;
        require(resolve_scrolling_combat_text_position_v1(
                    0x1234,facts,world,position_error)&&
                world[0]==1.f&&world[1]==2.f&&world[2]==13.f,
                "visible target_node anchor plus source Character height delta differs");
        facts.source_visible_80=0;
        require(resolve_scrolling_combat_text_position_v1(
                    0x1234,facts,world,position_error)&&
                world[0]==10.f&&world[1]==20.f&&world[2]==40.f,
                "GetTargetPosition must use GameObject +0x160 when byte +0x80 is false");
        graph.graph[1].name="target_node_suffix";facts.source_visible_80=1;
        require(resolve_scrolling_combat_text_position_v1(
                    0x1234,facts,world,position_error)&&world[0]==10.f,
                "GetTargetPosition must fall back when optional anchor is absent");
        require(!resolve_scrolling_combat_text_position_v1(
                    0x9999,facts,world,position_error),
                "combat text position accepted a different Character identity");
        PositionFixture live{facts};
        ScrollingCombatTextPositionLookupV1 lookup{&live,PositionFixture::lookup};
        require(scrolling_combat_text_position_v1(
                    &lookup,0x1234,world,position_error)&&world[2]==40.f,
                "callback-shaped live position adapter failed");
    }
    using namespace dh2::data;
    Fixture fixture;
    ScrollingCombatTextResultV1 output{};
    std::string error;
    auto result = CombatResult{};
    result.outcomes = 4u | 0x20u | 0x100u | 8u;
    result.mask = 0x04000000u;
    result.amount = 77 * 256;
    fixture.dual = true; fixture.local = true;
    const auto status = apply_scrolling_combat_text_v1(
        result, 0x1000, 0x2000, fixture.services(), output, error);
    require(status == ScrollingCombatTextStatusV1::complete, "source SCT route failed");
    require(fixture.events.size() == 4, "block/fear/slow/damage event order incomplete");
    if(!(fixture.events[0].find("localized-") != std::string::npos &&
            fixture.events[1].find("localized-") != std::string::npos &&
            fixture.events[2].find("localized-") != std::string::npos &&
            fixture.events[3] == "value:77")){
        for(const auto& event:fixture.events)std::cerr<<"event="<<event<<'\n';
        require(false,"source text/value formatting differs");
    }
    require(std::find(fixture.calls.begin(), fixture.calls.end(),
                      "style:anim_sct_critleft") != fixture.calls.end(),
            "offhand critical style not selected");
    require(std::find(fixture.calls.begin(), fixture.calls.end(),
                      "constant:ScrollingCombatText:PlayerCritDamageColor") != fixture.calls.end(),
            "local player critical color not selected");
    require(output.emitted == 4 && output.last_value == 77,
            "source dispatch result lost emitted prefix");

    Fixture xp;
    const auto xp_status=apply_scrolling_combat_xp_v1(0x1000,125,xp.services(),output,error);
    require(xp_status==ScrollingCombatTextStatusV1::complete&&xp.events.size()==1&&
            xp.events[0]=="text:xp-125"&&output.emitted==1&&output.last_value==125,
            "source XP localization/position/style/color dispatch failed");
    require(std::find(xp.calls.begin(),xp.calls.end(),"style:anim_sct_xp")!=xp.calls.end()&&
            std::find(xp.calls.begin(),xp.calls.end(),"constant:ScrollingCombatText:XPColor")!=xp.calls.end(),
            "source XP style/color constants were not used");

    Fixture bridged;
    ScrollingCombatTextBridgeProvidersV1 providers{};
    providers.source_context=&bridged;providers.is_follower=Fixture::is_follower;
    providers.position=Fixture::position;providers.source_property=Fixture::property;
    providers.is_dual_wielding=Fixture::dual_wield;providers.is_local_player=Fixture::local_player;
    providers.ui_context=&bridged;providers.constant=Fixture::constant;
    providers.localized_string=Fixture::localized;providers.localized_formatted_string=Fixture::localized_formatted;
    providers.style_id=Fixture::style;providers.decode_color_rgba=Fixture::decode_color;
    providers.play_authored_screen=Fixture::play_screen;
    const float camera[16]={.1f,0,0,0, 0,.1f,0,0, 0,0,0,1, 0,0,0,0};
    ScrollingCombatTextBridgeV1 bridge;
    require(bridge.bind(providers,camera,100,80,error),"source SCT runtime bridge bind failed");
    result={};result.amount=256;
    const auto bridged_status=apply_scrolling_combat_text_v1(
        result,0x1000,0x2000,bridge.services(),output,error);
    require(bridged_status==ScrollingCombatTextStatusV1::complete&&
            bridged.events.size()==1&&
            bridged.events[0].find("screen:anim_sct_normaldamage:") == 0,
            "source runtime bridge failed projection/style/localization/playback path");
    std::uint8_t color[4]{};
    decode_scrolling_combat_text_color_rgb_v1(static_cast<std::int32_t>(0xff0ed98au),color);
    require(color[0]==0x0e&&color[1]==0xd9&&color[2]==0x8a&&color[3]==255,
            "source XPColor RGB channel mapping changed");

    Fixture missed;
    result = {}; result.outcomes = 1u; result.amount = 500 * 256;
    const auto miss_status = apply_scrolling_combat_text_v1(
        result, 0x1000, 0x2000, missed.services(), output, error);
    require(miss_status == ScrollingCombatTextStatusV1::complete &&
            missed.events.size() == 1 && output.emitted == 1,
            "miss did not terminate at its source return");

    Fixture failed;
    failed.fail_property = true; result = {}; result.outcomes = 4u | 0x20u;
    const auto failed_status = apply_scrolling_combat_text_v1(
        result, 0x1000, 0x2000, failed.services(), output, error);
    require(failed_status == ScrollingCombatTextStatusV1::service_failed &&
            output.emitted == 1 && failed.events.size() == 1,
            "source provider error did not preserve prior display prefix");
    std::cout << "{\"validation\":\"PASS\",\"checks\":21,\"scope\":\"source target_node hierarchy lookup, SCT position/height, dispatch, XPColor, playback bridge, and projection\"}\n";
}
