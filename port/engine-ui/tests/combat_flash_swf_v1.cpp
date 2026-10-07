// Actual cache HUD, native edit text, same font platform and glyph raster.
// World projection and external bitmap delivery remain explicit fixtures.
#define main previous_font_fixture_main
#include "hud_freetype_provider.cpp"
#undef main
#include "../combat_flash_swf_v1.hpp"
#include "../swf_text_font_platform_v1.hpp"
#include "../../level-world/character_combat_text_v1.hpp"
#include "gameswf/gameswf_sprite.h"
struct FlashTest:Test {
 unsigned projections{},red_quads{};gameswf::player* player{};
 SwfMovie* movie{};int surface_width=480,surface_height=320;
 CombatFlashSwfV1* flash{};std::string source_error;
 static int follower(void*,std::uintptr_t,bool* out){*out=false;return 0;}
 static int position(void*,std::uintptr_t,float p[3]){p[0]=240;p[1]=160;p[2]=0;return 0;}
 static int height(void*,std::uintptr_t,float* out){*out=5;return 0;}
 static int dual(void*,std::uintptr_t,bool* out){*out=false;return 0;}
 static int player_flag(void*,std::uintptr_t,bool* out){*out=false;return 0;}
 static int source_constant(void* p,const char* group,const char* key,int* out){return dh2_script_constants_get(static_cast<FlashTest*>(p)->constants,group,key,out);}
 static int enqueue(void* p,const dh2::character::skills::CombatTextRequestV1* q){auto& t=*static_cast<FlashTest*>(p);return t.flash&&t.flash->play(q->style,q->position,q->text,q->number,q->color,q->numeric,t.source_error)?0:-1;}
 static bool draw(void* p,const SwfDraw& d,std::string& e){auto& t=*static_cast<FlashTest*>(p);if(d.kind==SwfDraw::bitmap_quad&&d.fill.rgba[0]==255&&d.fill.rgba[1]==0&&d.fill.rgba[2]==0)++t.red_quads;return Test::draw(p,d,e);}
 static bool start(void* p,const SwfAsLease& l,std::string&){static_cast<FlashTest*>(p)->player=l.player;return true;}
 static bool project(void* p,const float v[3],int* x,int* y,std::string&){++static_cast<FlashTest*>(p)->projections;*x=int(v[0]);*y=int(v[1]);return true;}
 static bool rectangle(void* raw,float r[4],int v[4],std::string& e){auto& t=*static_cast<FlashTest*>(raw);return t.movie&&t.movie->source_display_rectangle(r,v,e);}
 static bool orientation(void*,int& out,std::string&){out=0;return true;}
 static bool dimensions(void* raw,int& x,int& y,std::string&){auto& t=*static_cast<FlashTest*>(raw);x=t.surface_width;y=t.surface_height;return true;}
 static bool movie_bounds(void* raw,SwfAsGraph& graph,std::string& e){auto& seed=*static_cast<ViewportState64*>(raw);SwfAsValue value;gameswf::as_object* object{};if(!graph.root_value(value,e)||!graph.borrow_object(value,object,e))return false;const auto& r=static_cast<gameswf::sprite_instance*>(object)->get_root()->m_def->m_frame_size;seed.movie_rect[0]=r.m_x_min;seed.movie_rect[1]=r.m_x_max;seed.movie_rect[2]=r.m_y_min;seed.movie_rect[3]=r.m_y_max;return true;}
};
int main(int argc,char** argv){try {
 check(argc==5,"usage: combat_flash_swf_v1 swfs fonts cache-data fonts-pycst");
 auto t=std::make_shared<FlashTest>();t->swfs=argv[1];t->fonts=argv[2];t->assets=argv[3];t->initialize(argv[4]);
 {auto design=Test::file(t->assets+"/../../data/design_pycst.bin");dh2_script_constants_reload loaded;check(dh2_script_constants_load(t->constants,design.data(),design.size(),&loaded)==0&&loaded.consumed==design.size(),"actual GameDesign colors failed load");}
 int damage_color{};check(dh2_script_constants_get(t->constants,"ScrollingCombatText","DamageColor",&damage_color)==0&&damage_color==-1,"actual authored damage color missing");
 SwfServices services;services.context=t.get();services.read=Test::read;services.texture=Test::texture;services.image=Test::image;services.draw=FlashTest::draw;services.native_call=Test::native;services.stencil=Test::stencil;services.native_owner=t;services.graph_start=FlashTest::start;
 SwfFontServices fonts{t.get(),Test::font_read,nullptr};TextFontBackendsV2 backends;
 backends.bitmap_face=[](const auto&,TextBitmapFaceV2& out,std::string&){out={};return true;};
 std::string e;
 {SwfTextFontPlatformV1 platform(fonts,services,t,backends,1);
 platform.policy().renderer_feature=[](const auto& c,std::string& e){if(c.kind==edit_text_display_v1::Command::grid_fit)return true;e="fixture has no render cache";return false;};
 SwfMovie movie;check(movie.load({"dqshared_droid.swf"},"dqhud_droid.swf",platform.services(),e)&&movie.advance(0,e)&&movie.display(0,0,480,320,e),e.c_str());
 t->movie=&movie;ViewportState64 seed{};check(movie.action_script(&seed,FlashTest::movie_bounds,e),e.c_str());seed.viewport[2]=seed.bounds[2]=t->surface_width;seed.viewport[3]=seed.bounds[3]=t->surface_height;check(movie.connect_viewport(seed,{t.get(),FlashTest::orientation,FlashTest::dimensions},e),e.c_str());
 {float actual[4]{};int viewport[4]{};check(movie.source_display_rectangle(actual,viewport,e),e.c_str());for(unsigned i=0;i<4;++i)check(actual[i]==seed.movie_rect[i],"Flash rectangle did not borrow actual movie source bounds");check(viewport[2]==480&&viewport[3]==320,"Flash viewport did not borrow named surface");}
 CombatFlashSwfV1 flash(movie,{t.get(),FlashTest::project,FlashTest::rectangle});check(flash.scan(e),e.c_str());
 t->flash=&flash;
 check(flash.queue().styles().size()>=10,"actual authored combat styles missing");
 for(const char* name:{"anim_sct_dot","anim_sct_crit","anim_sct_normaldamage","anim_sct_block"})check(flash.queue().styles()[flash.queue().style_id(name)].name==name,"source style lookup failed");
 for(unsigned i=0;i<flash.queue().styles().size();++i)check(flash.queue().style_id(flash.queue().styles()[i].name.c_str())>=int(i),"source last duplicate style mapping not retained");
 const float at[3]{240,160,0};
 check(flash.play("anim_sct_normaldamage",at,nullptr,17,int(0xffff0000u),true,e),e.c_str());
 check(std::string(flash.queue().contexts()[0].text.data())=="17","native numeric queue text missing");
 const auto before=t->quads;
 check(flash.draw(false,e),e.c_str());check(platform.error().empty(),platform.error().c_str());
 check(t->quads>before,"positive combat glyph draw did not reach real renderer sink");
 check(t->red_quads>0,"source combat ARGB red did not reach actual glyph renderer");
 check(flash.update(66,2,e)&&flash.queue().contexts()[0].frame==1&&flash.queue().contexts()[0].timer==33,"source strict interval elapsed prefix differs");
 check(flash.draw(false,e),e.c_str());
 dh2::character::skills::CombatTextServicesV1 source{};source.context=t.get();source.follower=FlashTest::follower;source.position=FlashTest::position;source.height=FlashTest::height;source.dual_wield=FlashTest::dual;source.is_player=FlashTest::player_flag;source.constant=FlashTest::source_constant;source.enqueue=FlashTest::enqueue;
 dh2::data::CombatResult result;result.amount=4352;check(dh2::character::skills::character_combat_text_v1(result,1,2,source)==1,t->source_error.c_str());check(std::string(flash.queue().contexts()[1].text.data())=="17"&&flash.queue().contexts()[1].color==damage_color,"whole source selector did not deliver actual GameDesign color/value");check(flash.draw(false,e),e.c_str());
 check(platform.policy().buffering,"source buffering state not restored");
 check(flash.play("anim_sct_block",at,"Blocked",0,int(0xff00ff00u),false,e),e.c_str());
 check(flash.play("anim_sct_normaldamage",at,nullptr,28,int(0xffffffffu),true,e),e.c_str());
 const auto& queue=flash.queue();const auto style=queue.style_id("anim_sct_normaldamage");check(queue.styles()[style].instances[1].clip&&queue.styles()[style].instances[1].clip!=queue.styles()[style].instances[0].clip,"actual authored style clone missing");
 for(unsigned n=0;n<160;++n)check(flash.update(66,0,e),e.c_str());
 for(const auto& c:flash.queue().contexts())check(c.flags==0,"finite native scrolling context did not expire");
 check(flash.play("anim_sct_normaldamage",at,nullptr,9,int(0xffffffffu),true,e),e.c_str());check(flash.draw(false,e),e.c_str());
 std::cout<<"{\"validation\":\"PASS\",\"actual_styles\":"<<queue.styles().size()<<",\"real_glyph_quads\":"<<t->quads-before<<",\"source_projection_fixture\":true,\"actual_clone\":true,\"expired_and_reused\":true}\n";
 }
 return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
