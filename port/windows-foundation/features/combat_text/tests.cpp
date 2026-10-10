#include "combat_text.hpp"
#include "combat_text_design.hpp"
#include "../../source_level_config.hpp"
#include "../../../level-world/character_debug_stdio_v136.hpp"
#include <cstdio>
#include <filesystem>
#include <iostream>
#include <stdexcept>
using namespace dh::foundation;
void check(bool value,const std::string&message){if(!value)throw std::runtime_error(message);}
struct Fixture {
    CombatTextDesign design;bool follower=false,player=false,dual=false;std::int32_t status=0;
    static int follows(void*p,std::uintptr_t,bool*out){*out=static_cast<Fixture*>(p)->follower;return 0;}
    static int position(void*,std::uintptr_t actor,float*out){out[0]=float(actor);out[1]=10;out[2]=20;return 0;}
    static int height(void*,std::uintptr_t,float*out){*out=30;return 0;}
    static int property(void*p,std::uintptr_t,int,std::int32_t*out){*out=static_cast<Fixture*>(p)->status;return 0;}
    static int duals(void*p,std::uintptr_t,bool*out){*out=static_cast<Fixture*>(p)->dual;return 0;}
    static int players(void*p,std::uintptr_t,bool*out){*out=static_cast<Fixture*>(p)->player;return 0;}
    static int constant(void*p,const char*g,const char*k,std::int32_t*out){return static_cast<Fixture*>(p)->design.constant(g,k,out);}
    static int localized(void*p,std::int32_t id,const char**out){return static_cast<Fixture*>(p)->design.localized(id,out);}
    dh2::character::skills::CombatTextServicesV1 services(){return {this,follows,position,height,property,duals,players,constant,localized,nullptr};}
};
int main(int argc,char**argv){try{
    check(argc==2,"Supply repository root");const auto repo=std::filesystem::path(argv[1]);
    AssetCatalog assets(repo/"port/windows-foundation/features/combat_text/assets");
    auto debug=std::shared_ptr<dh2::character::DebugSwitches>(dh2_character_debug_create(),dh2_character_debug_destroy);
    auto directory=std::make_shared<std::filesystem::path>(assets.root());
    dh2::character::DebugFileServices24 files{directory.get(),
        [](void*p,const char*name,std::uintptr_t*out){*out=0;const auto path=*static_cast<std::filesystem::path*>(p)/name;auto*file=std::fopen(path.string().c_str(),"rb");if(!file)return std::filesystem::exists(path)?1:0;*out=reinterpret_cast<std::uintptr_t>(file);return 0;},
        [](void*,std::uintptr_t file){return std::fclose(reinterpret_cast<std::FILE*>(file));}};
    const auto query=source_level_config_debug_switch(debug,directory,files,dh2::character::debug_stdio_services_v136(nullptr));
    CombatTextDesignQueries queries;
    queries.debug=[query](const char*key,std::string&e){bool ignored=false;return query(key,ignored,e);};
    // Explicit no-player localization boundary fixture; words do not substitute names.
    queries.player_character=[](std::uintptr_t&out,std::string&){out=0;return true;};
    queries.player_name=[](std::uintptr_t,std::string&,std::string&e){e="No player in source text fixture";return false;};
    Fixture fixture;std::string error;check(fixture.design.load(assets,queries,error),error);
    PlayableCombatResolution r;r.attacker=1;r.victim=2;r.melee.original.amount=0;
    std::vector<CombatTextDisplayEvent>events;
    check(select_combat_text(r,fixture.services(),events,error)&&events.empty(),"Zero damage invented MISS/BLOCK");
    r.melee.original.outcomes=1;check(select_combat_text(r,fixture.services(),events,error),fixture.design.error()+error);
    check(events.size()==1&&events[0].text=="Miss"&&events[0].style=="anim_sct_block","Original Miss localization/style differs");
    std::int32_t missColor=0;check(fixture.design.constant("ScrollingCombatText","MissColor",&missColor)==0&&events[0].argb==missColor,"Actual source Miss color lost");
    check(events[0].position.z==50,"Original target bounds height was not added");
    r.melee.original.outcomes=2;check(select_combat_text(r,fixture.services(),events,error)&&events[0].text=="Dodge","Dodge flags/localization differs");
    r.melee.original.outcomes=4;r.melee.original.amount=4352;check(select_combat_text(r,fixture.services(),events,error),error);
    check(events.size()==2&&events[0].text=="Block"&&events[1].text=="17","Partial BLOCK plus native signed256 number lost");
    std::int32_t damageColor=0;check(fixture.design.constant("ScrollingCombatText","DamageColor",&damageColor)==0&&events[1].argb==damageColor,"Actual source numeric color lost");
    r.melee.original.outcomes=1|4;check(select_combat_text(r,fixture.services(),events,error)&&events.size()==1&&events[0].text=="Miss","Original outcome precedence changed");
    r.melee.original.outcomes=8;check(select_combat_text(r,fixture.services(),events,error)&&events[0].style=="anim_sct_crit","Critical source style differs");
    fixture.dual=true;r.melee.original.mask=0x04000000;check(select_combat_text(r,fixture.services(),events,error)&&events[0].style=="anim_sct_critleft","Actual dual-hand source style differs");
    r.melee.original.mask=0x20000000;check(select_combat_text(r,fixture.services(),events,error)&&events[0].style=="anim_sct_dot","DOT source style differs");
    fixture.follower=true;check(select_combat_text(r,fixture.services(),events,error)&&events.empty(),"Follower suppression lost");fixture.follower=false;fixture.dual=false;
    r.melee.original.outcomes=0;r.melee.original.mask=0;check(select_combat_text(r,fixture.services(),events,error),error);
    CombatTextPresenter presenter;auto project=[](Vec3,float&x,float&y,std::string&){x=240;y=160;return true;};
    check(presenter.enqueue(events[0],project,1,1,error),error);check(presenter.update_ms(33,33,error),error);
    HudGlyphFont font;check(font.load(repo/".local-inputs/windows-shared-assets/data/Fontin SmallCaps.ttf",error),error);
    std::vector<CombatTextGlyph>first,next;check(presenter.geometry(font,1,1,first,error)&&!first.empty(),error);
    check(presenter.update_ms(1,33,error)&&presenter.geometry(font,1,1,next,error),error);
    check(first[0].xy!=next[0].xy,"Original frame movement was discarded");
    check(next[0].rgba[3]==1,"Invented fade differs from native ARGB override");
    for(unsigned i=0;i<14;++i)check(presenter.enqueue(events[0],project,1,1,error),error);
    check(presenter.active_count()==12,"Original bounded queue capacity changed");
    check(presenter.update_ms(100000,33,error)&&presenter.active_count()==0,"Original finite style frames did not expire");
    check(!presenter.update_ms(1,0,error),"Invalid zero frame interval accepted");
    // Exercise every extracted HUD style through the same original-font path,
    // including authored left/right scale matrices and word-bearing clips.
    const char* styles[]={"anim_sct_normaldamage","anim_sct_xp","anim_sct_normaldamageleft",
        "anim_sct_normaldamageright","anim_sct_crit","anim_sct_critleft","anim_sct_critright",
        "anim_sct_block","anim_sct_stun","anim_sct_dot"};
    for(const auto*name:styles){
        const auto*style=original_combat_text_style(name);
        check(style&&style->font==512&&style->height==16&&style->frame_count>1,
              std::string("Original Font512 style metadata unavailable: ")+name);
        CombatTextDisplayEvent event=events[0];event.style=name;
        event.text=std::string(name)=="anim_sct_block"?"Block":std::string(name)=="anim_sct_stun"?"Stun":"32";
        CombatTextPresenter clip;check(clip.enqueue(event,project,1,1,error),error);
        check(clip.geometry(font,1,1,first,error)&&first.size()==event.text.size(),error);
        for(std::size_t frame=1;frame<style->frame_count;++frame){
            check(clip.update_ms(frame==1?34:33,33,error),error);
            check(clip.active_count()==1&&clip.geometry(font,1,1,next,error)&&next.size()==first.size(),
                  std::string("Original style frame lost glyphs: ")+name);
            for(const auto&glyph:next)check(glyph.rgba[3]==1,"Source native color override acquired an invented fade");
        }
        check(clip.update_ms(33,33,error)&&clip.active_count()==0,std::string("Style did not expire at its authored final frame: ")+name);
    }
    std::cout<<"PASS original outcome flags/priority, actual Miss/Dodge/Block localization/colors, signed256 numbers, authored style motion/font geometry, queue/expiration\n";return 0;
}catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}
