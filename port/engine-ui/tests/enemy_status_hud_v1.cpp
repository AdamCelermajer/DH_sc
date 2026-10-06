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
int main(int argc,char** argv){try{
 if(argc!=5){std::cerr<<"usage: enemy_status_hud_v1 swfs fonts cache-data fonts-pycst\n";return 2;}EnemyTest t;t.base=argv[1];t.font_base=argv[2];t.cache_base=argv[3];t.font_constants=argv[4];auto platform=t.retained_text_platform();SwfMovie movie;std::string error;
 require(movie.load({"dqshared_droid.swf"},"dqhud_droid.swf",platform->services(),error)&&movie.advance(0,error),error);
 EnemyStatusHudV1 hud(movie,{&t,EnemyTest::string});
 EnemyHudWorldBorrowV1 borrow{&t.local,{&t,EnemyTest::world},101};
 auto update=[&]{t.target_reads=0;require(hud.update(borrow,error),error);};
 t.target=false;update();t.expect(hud.target()==2&&hud.visible()&&hud.name()=="Cultist"&&hud.level()=="7"&&hud.hp_frame()==99,"Raw target/name/level/full HP did not reach actual movie");
 t.hp=.5f;update();t.expect(hud.hp_frame()==49,"Actual target damage frame missing");
 t.hp=0;update();t.expect(hud.hp_frame()==49,"Source invalid negative frame history was replaced");
 t.hp=1.2f;update();t.expect(hud.hp_frame()==99,"Source enemy upper clamp differs");
 t.target=true;t.boss=true;update();t.expect(t.target_reads==3&&hud.target()==3&&hud.name()=="Ghost"&&hud.level()=="??","Source AI priority/reloads/boss level differs");
 t.target=false;t.boss=false;t.debug=true;update();t.expect(hud.name()=="CultistDebug"&&hud.level()=="91","Genuine debug display branch missing");
 t.target=true;t.third_null=true;update();t.expect(hud.target()==0&&!hud.visible()&&hud.hp_frame()==0,"Third AI query null incorrectly fell back to raw target");
 t.third_null=false;t.target=false;t.debug=false;t.fail_op=int(HudManagerOperation::hp_fraction);
 t.expect(!hud.update(borrow,error)&&hud.visible()&&hud.name()=="Cultist","Reached required failure did not preserve source presentation prefix");error.clear();t.fail_op=-1;update();
 t.dead=true;t.local.target=nullptr;update();t.expect(!hud.visible()&&!hud.target()&&hud.hp_frame()==0,"Actual target loss failed to clear clip");
  t.expect(!hud.update({},error),"Absent retained world was accepted");error.clear();
  // The fixed-button gameplay layout selects HUD2 and hides HUD0. Rebind the
  // enemy owner to that same graph, then switch back without retaining a
  // cached target from the preceding layout.
  t.dead=false;t.local.target=&t.first;t.target=false;t.hp=.5f;
  require(movie.set_visible("_root.menu_HUD_0",false,error)&&movie.set_visible("_root.menu_HUD_2",true,error),error);
  t.target_reads=0;require(hud.update(borrow,error,2),error);
  t.expect(hud.visible()&&hud.target()==2&&hud.hp_frame()==49,"Selected HUD2 enemy was not rebound and populated");
  SwfClipInfo selected;require(movie.clip("_root.menu_HUD_2.HUDelements.HealthBars.enemy",selected,error),error);
  t.expect(selected.visible,"Actual selected HUD2 enemy clip remains hidden");
  t.target_reads=0;require(hud.update(borrow,error,0),error);
  t.expect(hud.target()==2&&hud.hp_frame()==49,"HUD style change retained stale enemy presentation");
 movie=SwfMovie();t.expect(!hud.update(borrow,error),"Expired movie owner silently accepted");
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<t.checks<<",\"actual_droid_display_list\":true,\"source_enemy_body_reused\":true,\"gameplay_and_localization_are_fixtures\":true,\"live_GPU\":false}\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
