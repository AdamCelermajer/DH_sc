#include "../combat_flash_inputs_v1.hpp"
#include "../../level-world/character_combat_follower_v1.hpp"
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <stdexcept>
using namespace dh2;
static void require(bool b,const char* e){if(!b)throw std::runtime_error(e);}
static std::vector<std::uint8_t> read(const std::string& p){std::ifstream f(p,std::ios::binary);require(bool(f),p.c_str());return {std::istreambuf_iterator<char>(f),{}};}
struct Files {unsigned opens{},closes{};bool present{};static int open(void* p,const char* name,std::uintptr_t* out){auto& f=*static_cast<Files*>(p);require(std::string(name)=="DebugSwitches.savegame","source Debug filename changed");++f.opens;*out=f.present?1:0;return 0;}static int close(void* p,std::uintptr_t id){require(id==1,"wrong actual Debug handle");++static_cast<Files*>(p)->closes;return 0;}};
int main(int argc,char** argv){try {
 require(argc==2,"actual AI assets directory");unsigned checks{};
 std::uint32_t dt=17;std::int32_t phase=0;ui::CombatFlashTickBorrowV1 borrow{&dt,1,&phase};ui::CombatFlashTickInputsV1 tick{};
 for(int value:{0,1,2,26,27,38}){phase=value;dt+=3;require(ui::combat_flash_tick_inputs_v1(&tick,borrow)==1&&tick.application_dt==dt&&tick.load_phase==phase,"tick detached from original dt/load-phase authority");++checks;}
 borrow.load_phase=nullptr;const auto before=tick;require(ui::combat_flash_tick_inputs_v1(&tick,borrow)<0&&tick.load_phase==before.load_phase&&tick.application_dt==before.application_dt,"missing selected-level phase published synthetic input");++checks;
 borrow.selected_level=0;require(ui::combat_flash_tick_inputs_v1(&tick,borrow)==1&&tick.load_phase==0,"genuine no-current-level FPS branch failed");++checks;
 Files files;character::DebugFileServices24 services{&files,Files::open,Files::close};std::unique_ptr<character::DebugSwitches,decltype(&dh2_character_debug_destroy)> debug(dh2_character_debug_create(),dh2_character_debug_destroy);bool disabled=true;
 require(ui::combat_flash_debug_gate_v1(&disabled,debug.get(),&services)==1&&!disabled&&files.opens==1&&files.closes==0,"source missing-file Debug default failed");++checks;
 disabled=true;require(ui::combat_flash_debug_gate_v1(&disabled,debug.get(),&services)==1&&!disabled&&files.opens==1,"retained source Debug reloaded or changed insertedfalse");++checks;
 Files existing;existing.present=true;character::DebugFileServices24 existing_services{&existing,Files::open,Files::close};std::unique_ptr<character::DebugSwitches,decltype(&dh2_character_debug_destroy)> rejected(dh2_character_debug_create(),dh2_character_debug_destroy);disabled=true;require(ui::combat_flash_debug_gate_v1(&disabled,rejected.get(),&existing_services)==-3&&disabled&&existing.closes==1,"unimplemented existing Debug file masked with false");++checks;
 const std::string base=argv[1];std::vector<std::uint8_t> bytes[6];const char* names[]{"ai_pyarray.bin","ai_pyarraynames.bin","ai_pystructnames.bin","ai_factions_pyarray.bin","ai_factions_pyarraynames.bin","ai_factions_pystructnames.bin"};for(unsigned i=0;i<6;++i)bytes[i]=read(base+"/"+names[i]);data::AiTables ai;std::string error;require(data::load_ai({bytes[0].data(),bytes[0].size()},{bytes[1].data(),bytes[1].size()},{bytes[2].data(),bytes[2].size()},{bytes[3].data(),bytes[3].size()},{bytes[4].data(),bytes[4].size()},{bytes[5].data(),bytes[5].size()},ai,error),error.c_str());
 std::int32_t resolved[224]{};character::skills::SkillTargetCharacterV6 c{};c.identity=123;c.resolved=resolved;character::skills::WorldTargetActorBorrowV1 actor{};actor.identity=c.identity;actor.character=&c;unsigned followers{};
 for(unsigned i=0;i<ai.rows.size();++i){resolved[1]=int(i);bool follower{};require(character::skills::character_combat_follower_v1(&follower,actor,ai)==1&&follower==(ai.rows[i].type==2),"follower did not query same canonical AI row type");followers+=follower;++checks;}
 require(followers>0,"actual source AI follower rows absent");for(int id:{-1,int(ai.rows.size())}){resolved[1]=id;bool follower{};require(character::skills::character_combat_follower_v1(&follower,actor,ai)==1&&follower==(ai.rows[8].type==2),"source signed AI id fallback8 differs");++checks;}
 bool unchanged=true;actor.character=nullptr;require(character::skills::character_combat_follower_v1(&unchanged,actor,ai)<0&&unchanged,"missing canonical character faked nonfollower");++checks;
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"actual_ai_rows\":"<<ai.rows.size()<<",\"actual_follower_rows\":"<<followers<<",\"same_borrowed_dt_phase\":true,\"retained_source_debug\":true,\"types\":[";for(unsigned i=0;i<ai.rows.size();++i){if(i)std::cout<<',';std::cout<<ai.rows[i].type;}std::cout<<"]}\n";return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
