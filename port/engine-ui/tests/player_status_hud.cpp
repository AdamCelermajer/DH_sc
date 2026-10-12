// Reuse the actual original font/localization fixture; its controlled texture
// sink is explicitly a host projection. GLES composition is checked on Android.
#define main localized_provider_fixture_main
#include "hud_freetype_provider.cpp"
#undef main
#include "../player_status_hud.hpp"
#include "../../game-data/vitals.hpp"
#include <algorithm>
#include <cmath>

struct Connected : Test {
    int width=2400,height=1080;
    unsigned begins=0,ends=0,strips=0;
    static bool orientation(void*,std::int32_t& out,std::string&){out=0;return true;}
    static bool dimensions(void* context,std::int32_t& w,std::int32_t& h,std::string&){
        auto& t=*static_cast<Connected*>(context);w=t.width;h=t.height;return true;
    }
    static bool draw_status(void* context,const SwfDraw& command,std::string& error){
        auto& t=*static_cast<Connected*>(context);
        if(command.kind==SwfDraw::begin){
            check(command.viewport[2]==t.width&&command.viewport[3]==t.height,"Actual source viewport dimensions lost");
            check(std::isfinite(command.bounds[0])&&std::isfinite(command.bounds[3]),"Nonfinite source display rectangle");++t.begins;
        }
        if(command.kind==SwfDraw::end)++t.ends;
        if(command.kind==SwfDraw::triangle_strip)++t.strips;
        return Test::draw(context,command,error);
    }
};
int main(int argc,char** argv){try{
    if(argc!=6)return 2;
    Connected t;t.swfs=argv[1];t.fonts=argv[2];t.assets=argv[3];t.initialize(argv[4]);
    using namespace dh2::data;
    const std::string data=argv[5];std::string error;
    auto view=[](const std::vector<std::uint8_t>& value){return Bytes{value.data(),value.size()};};
    auto a=Test::file(data+"/character_properties_pyarray.bin"),n=Test::file(data+"/character_properties_pyarraynames.bin"),s=Test::file(data+"/character_properties_pystructnames.bin");
    CharacterTable characters;PropertyRules rules;
    check(load_characters(view(a),view(n),view(s),characters,error)&&load_property_rules(characters,rules,error),error.c_str());
    a=Test::file(data+"/character_classes_pyarray.bin");n=Test::file(data+"/character_classes_pyarraynames.bin");s=Test::file(data+"/character_classes_pystructnames.bin");
    ClassTables classes;check(load_classes(view(a),view(n),view(s),classes,error),error.c_str());
    auto row=std::find(characters.names.begin(),characters.names.end(),"KnightPlayerBase");
    check(row!=characters.names.end(),"Actual world player preset unavailable");
    PropertyState player;reset_properties(rules,player,&characters.rows.at(row-characters.names.begin()));SpawnVitals spawn;
    check(recalc_properties_with_class(classes,rules,player,error)&&initialize_spawn_vitals(rules,player,spawn,error),error.c_str());
    const auto initial=player.resolved;
    check(initial[38]>0&&initial[43]>0&&initial[34]!=0,"Actual player HUD denominator unavailable");
    SwfHudFreetypeProvider fonts({&t,Test::font_read,nullptr},1,Test::bitmap_probe);
    SwfMovie movie;SwfServices services;services.context=&t;services.read=Test::read;
    services.texture=Test::texture;services.image=Test::image;services.draw=Connected::draw_status;
    services.native_call=Test::native;services.stencil=Test::stencil;services.glyphs=fonts.borrowed_provider();
    const ViewportState64 seed{{0,9600,0,6400},{0,0,480,320},{0,0,480,320},1,0,0};FlashCamera40 camera{};
    auto load=[&](){check(movie.load({"dqshared_droid.swf"},"dqhud_droid.swf",services,error),error.c_str());
        check(movie.connect_viewport(seed,{&t,Connected::orientation,Connected::dimensions},error)&&movie.update_viewport(camera,error)&&movie.advance(0,error),error.c_str());};
    load();PlayerStatusHud hud(movie);
    constexpr const char* hash="a4ffacd1abdf7c9b2ba19c46ebb81c60c100458731a4cdba5880391b9c11b238";
    check(hud.bind(hash,error),error.c_str());
    unsigned states=0;
    auto update=[&](){check(hud.update(player.resolved.data(),player.resolved.size(),reinterpret_cast<std::uintptr_t>(&player),error),error.c_str());
        check(hud.character()==reinterpret_cast<std::uintptr_t>(&player),"Player identity lost");
        for(unsigned i=0;i<5;++i){std::string path=hud_value_clip_path(HudValueClip(i));if(i!=4)path="_root.menu_HUD_0."+path;
            SwfClipInfo clip;check(movie.clip(path.c_str(),clip,error)&&clip.frame==hud.frames()[i],"Source status frame did not reach actual retained sprite");}
        check(movie.display_source_clip("_root.menu_HUD_0.HUDelements.HealthBars",error)&&movie.display_source_clip("_root.HurtCorners",error),error.c_str());++states;};
    update();check(hud.frames()[0]==99&&hud.frames()[1]==99,"Actual full player not reflected by source HUD");
    auto properties=property_view(rules,player);
    check(dh2_property_add(&properties,36,-initial[38]/2)==0&&dh2_property_add(&properties,41,-initial[43]/3)==0,"Actual property mutation failed");
    update();check(hud.frames()[0]<99&&hud.frames()[1]<99&&hud.frames()[3]==hud.frames()[0]&&hud.frames()[4]==hud.frames()[0],"Damage/mana did not drive all original status clips");
    const auto prior_mp_frame=hud.frames()[1];
    check(dh2_property_add(&properties,41,-player.resolved[41])==0,"Actual mana depletion failed");
    update();check(hud.frames()[1]==prior_mp_frame,"Source negative MP request did not preserve prior authored frame");
    VitalsChange healed,mana;check(dh2_vitals_regen(&properties,0,-1,&healed)==0&&dh2_vitals_regen(&properties,1,-1,&mana)==0,"Actual refill producer failed");
    update();check(hud.frames()[0]==99&&hud.frames()[1]==99,"Actual refill did not reach HUD");
    for(auto size:std::vector<std::pair<int,int>>{{1920,1080},{1080,2400},{1280,800},{2400,1080}}){t.width=size.first;t.height=size.second;check(movie.update_viewport(camera,error),error.c_str());update();}
    auto frames=hud.frames();check(!hud.update(nullptr,0,1,error)&&hud.frames()==frames,"Malformed live sheet accepted");
    auto invalid=player.resolved;invalid[38]=0;check(!hud.update(invalid.data(),invalid.size(),1,error)&&error.find("zero-division")!=std::string::npos,"Missing integer runtime silently fabricated");
    SwfHudClip handle;check(movie.hud_bind("_root.menu_HUD_0.HUDelements.HealthBars.player.bar_hp",hash,handle,error),error.c_str());
    load();check(!movie.hud_goto(handle,0,{},error)&&error.find("different retained movie")!=std::string::npos,"Stale graph handle accepted after reload");
    check(!hud.update(player.resolved.data(),player.resolved.size(),1,error),"Status owner silently reused replaced graph");
    hud.release();handle=SwfHudClip{};check(hud.bind(hash,error),error.c_str());update();
    check(t.begins==t.ends&&t.strips>0&&t.alpha>0&&t.packed>0&&hud.dirty_nodes()>0,"Connected source rendering/provider/scheduling missing");
    std::cout<<"{\"validation\":\"PASS\",\"actual_player_preset\":\"KnightPlayerBase\",\"actual_property_changes_and_refill\":true,\"connected_status_states\":"<<states<<",\"source_viewport_resizes\":4,\"retained_clip_observations\":"<<states*5<<",\"failure_and_graph_ownership_guards\":4,\"draw_pairs\":"<<t.begins<<",\"triangle_strips\":"<<t.strips<<",\"initial_hp_raw\":"<<initial[36]<<",\"initial_mp_raw\":"<<initial[41]<<",\"initial_max_xp_raw\":"<<initial[34]<<",\"packed_glyphs\":"<<t.packed<<"}\n";return 0;
}catch(const std::exception& ex){std::cerr<<ex.what()<<'\n';return 1;}}
