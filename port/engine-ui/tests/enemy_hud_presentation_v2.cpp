// Actual retained droid HUD sprites and source enemy body. Gameplay/query and
// localization services are explicit host fixtures; this is not live combat.
#define main complete_hud_fixture_main
#include "hud_manager_core.cpp"
#undef main
#include "../enemy_status_hud_v1.hpp"
struct EnemyTest : Test {
 HudManagerActor first{},second{},local{};
 float hp=1.f;
 unsigned target_reads{},checks{};
 bool dead{},boss{},debug{},third_null{};
 int fail_op=-1;
 EnemyTest(){first.identity=2;first.name_symbol=12;first.debug_name="CultistDebug";first.network_id=91;second=first;second.identity=3;second.name_symbol=13;local.identity=1;local.target=&first;}
 static bool string(void*,std::int32_t id,std::string& out,bool& is_null,std::string&){out=id==12?"Cultist":"Ghost";is_null=false;return true;}
 static int world(void* p,HudManagerState*,const HudManagerRequest* q,HudManagerResponse* out){
  auto& t=*static_cast<EnemyTest*>(p);using Op=HudManagerOperation;
  if(int(q->operation)==t.fail_op)return 0;
  switch(q->operation){
   case Op::is_dead:out->value=t.dead;break;
   case Op::target_character:out->identity=t.target?reinterpret_cast<std::uintptr_t>(&t.second):0;if(++t.target_reads==3&&t.third_null)out->identity=0;break;
   case Op::is_character:case Op::is_monster:out->value=1;break;
   case Op::is_boss:out->value=t.boss;break;
   case Op::level:out->value=7;break;
   case Op::debug_load:break;
   case Op::debug_switch:out->value=t.debug;break;
   case Op::hp_fraction:out->fraction=t.hp;break;
   default:return 0;
  }return 1;
 }
 void expect(bool value,const char* message){require(value,message);++checks;}
};

struct PresentationTest:EnemyTest {
 EnemyHudPresentationV2 pose{};unsigned projections{};
 float expected[2]{};
 float visible[4]{},last_bounds[4]{};int width=960,height=540;
 static bool contained(void* p,SwfAsGraph& g,std::string& e){auto& t=*static_cast<PresentationTest*>(p);SwfAsValue root,value;gameswf::as_object* object{};if(!g.root_value(root,e)||!g.find_target(root,"_root.menu_HUD_0.HUDelements.HealthBars.enemy",value,e)||!g.borrow_object(value,object,e))return false;auto* c=static_cast<gameswf::character*>(object);gameswf::rect world;c->get_bound(&world);c->get_parent()->get_world_matrix().transform(&world);require(world.m_x_min>=t.visible[0]-.1f&&world.m_x_max<=t.visible[1]+.1f&&world.m_y_min>=t.visible[2]-.1f&&world.m_y_max<=t.visible[3]+.1f,"actual full enemy name/bar bounds exceed viewport");const float b[4]{world.m_x_min,world.m_x_max,world.m_y_min,world.m_y_max};for(unsigned i=0;i<4;++i)t.last_bounds[i]=b[i];return true;}
 static bool measure(void* p,SwfAsGraph& g,std::string& e){auto& t=*static_cast<PresentationTest*>(p);SwfAsValue root,value;gameswf::as_object* object{};if(!g.root_value(root,e)||!g.find_target(root,"_root.menu_HUD_0.HUDelements.HealthBars.enemy",value,e)||!g.borrow_object(value,object,e))return false;auto* c=static_cast<gameswf::character*>(object);gameswf::rect bound;c->get_bound(&bound);gameswf::point anchor((bound.m_x_min+bound.m_x_max)*.5f,bound.m_y_max),world;c->get_parent()->get_world_matrix().transform(&world,anchor);require(std::abs(world.m_x-t.expected[0])<.1f&&std::abs(world.m_y-t.expected[1])<.1f,"Actual enemy bar bottom-centre does not match selected enemy head");return true;}
 static bool project(void* p,std::uintptr_t id,EnemyHudPresentationV2& out,std::string&){auto& t=*static_cast<PresentationTest*>(p);out=t.pose;out.identity=id;++t.projections;return true;}
 static bool orientation(void*,int& out,std::string&){out=0;return true;}
 static bool dimensions(void* p,int& x,int& y,std::string&){auto& t=*static_cast<PresentationTest*>(p);x=t.width;y=t.height;return true;}
 static bool bind(void* p,SwfAsGraph& g,std::string& e){auto& seed=*static_cast<ViewportState64*>(p);SwfAsValue v;gameswf::as_object* object{};if(!g.root_value(v,e)||!g.borrow_object(v,object,e))return false;auto* s=static_cast<gameswf::sprite_instance*>(object);const auto& r=s->get_root()->m_def->m_frame_size;seed.movie_rect[0]=r.m_x_min;seed.movie_rect[1]=r.m_x_max;seed.movie_rect[2]=r.m_y_min;seed.movie_rect[3]=r.m_y_max;return true;}
};
int main(int argc,char** argv){try{
 require(argc==2,"cache directory");PresentationTest t;t.base=argv[1];SwfMovie movie;std::string e;require(movie.load({"dqshared_droid.swf"},"dqhud_droid.swf",t.services(),e)&&movie.advance(0,e),e);
 ViewportState64 seed{};require(movie.action_script(&seed,PresentationTest::bind,e),e);seed.viewport[2]=seed.bounds[2]=960;seed.viewport[3]=seed.bounds[3]=540;
 require(movie.connect_viewport(seed,{&t,PresentationTest::orientation,PresentationTest::dimensions},e),e);
 EnemyStatusHudV1 hud(movie,{&t,EnemyTest::string});EnemyHudWorldBorrowV1 borrow{&t.local,{&t,EnemyTest::world},101,{&t,PresentationTest::project}};
 t.pose={0,100,100,false,true,{730,190}};
 auto update=[&]{t.target_reads=0;require(hud.update(borrow,e),e);};
 update();require(hud.visible()&&hud.hp_frame()==99,"positive target not displayed");
 t.expected[0]=t.pose.screen_pixels[0];t.expected[1]=t.pose.screen_pixels[1];require(movie.screen_to_logical(t.expected,e),e);t.expected[0]*=20;t.expected[1]*=20;require(movie.action_script(&t,PresentationTest::measure,e),e);
 t.pose.raw_hp=1;update();require(hud.visible()&&hud.hp_frame()==0,"positive subpercent HP retained stale source frame");
 t.pose.raw_hp=0;update();require(!hud.visible(),"zero HP target visible");
 t.pose.raw_hp=100;t.pose.dead=true;update();require(!hud.visible(),"dead target visible");
 t.pose.dead=false;update();require(hud.visible()&&hud.hp_frame()==99,"same target reacquire not restored");
 t.pose.on_screen=false;update();require(!hud.visible(),"offscreen target visible");
 t.pose.on_screen=true;t.pose.screen_pixels[0]=240;t.pose.screen_pixels[1]=330;update();require(hud.visible(),"moved target missing");
 t.expected[0]=240;t.expected[1]=330;require(movie.screen_to_logical(t.expected,e),e);t.expected[0]*=20;t.expected[1]*=20;require(movie.action_script(&t,PresentationTest::measure,e),e);update();require(movie.action_script(&t,PresentationTest::measure,e),e);
 auto containment=[&]{float first[2]{0,0},last[2]{float(t.width),float(t.height)};require(movie.screen_to_logical(first,e)&&movie.screen_to_logical(last,e),e);t.visible[0]=first[0]*20;t.visible[1]=last[0]*20;t.visible[2]=first[1]*20;t.visible[3]=last[1]*20;require(movie.action_script(&t,PresentationTest::contained,e),e);};
 t.pose.screen_pixels[0]=958;t.pose.screen_pixels[1]=3;update();containment();float bounds[4];for(unsigned i=0;i<4;++i)bounds[i]=t.last_bounds[i];update();containment();for(unsigned i=0;i<4;++i)require(std::abs(bounds[i]-t.last_bounds[i])<.1f,"edge containment drifts on repeated update");
 t.width=640;t.height=360;seed.viewport[2]=seed.bounds[2]=t.width;seed.viewport[3]=seed.bounds[3]=t.height;require(movie.connect_viewport(seed,{&t,PresentationTest::orientation,PresentationTest::dimensions},e),e);t.pose.screen_pixels[0]=639;t.pose.screen_pixels[1]=1;update();containment();update();containment();
 t.local.target=nullptr;update();require(!hud.visible()&&!hud.target(),"target loss did not hide");
 std::cout<<"{\"validation\":\"PASS\",\"actual_droid_clip\":true,\"death_zeroHP_reacquire\":true,\"source_viewport\":true,\"projection_fixture\":true,\"queries\":"<<t.projections<<"}\n";return 0;
 }catch(const std::exception& ex){std::cerr<<ex.what()<<'\n';return 1;}}
