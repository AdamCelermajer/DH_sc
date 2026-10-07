#include "menu_manager_unload_v58.hpp"
#include "menu_manager_update_v58.hpp"
#include "authored_menu_application_fields_v3.hpp"
#include <cassert>
#include <vector>
#include <algorithm>
#include <iostream>
using namespace dh2::ui;
int main(){
 std::string error;auto manager=std::make_shared<int>(1);
 const auto movie=static_cast<std::uintptr_t>(0x123456789ull);
 std::vector<MenuReceiverBorrowV58> directory{{1,movie,1},{2,99,1},{3,movie,0}};
 std::vector<int> trace;bool append=true,fail_delete=false;
 auto find=[&](std::uintptr_t id)->MenuReceiverBorrowV58&{
  auto it=std::find_if(directory.begin(),directory.end(),[&](const auto& m){return m.identity==id;});
  assert(it!=directory.end());return *it;
 };
 MenuManagerUnloadServicesV58 unload;unload.actual_manager=manager;
 unload.clear_map_icons=[&](auto&){trace.push_back(10);return true;};
 unload.movie_slot=[&](auto slot,auto& out,auto&){trace.push_back(20+slot);if(slot!=0)out={manager,movie};return true;};
 unload.registry_count=[&](auto& out,auto&){out=directory.size();return true;};
 unload.registry_at=[&](auto i,auto& out,auto&){out=directory.at(i);return true;};
 unload.clear_render=[&](auto id,auto&){trace.push_back(100+id);find(id).render=0;return true;};
 unload.read_owned7d=[&](auto id,auto& out,auto&){assert(!find(id).render);trace.push_back(200+id);out=find(id).owned7d;return true;};
 unload.deleting_virtual4=[&](auto id,auto&){assert(!find(id).render);trace.push_back(300+id);
  if(fail_delete)return false;
  if(append){append=false;directory.push_back({4,movie,1});}return true;};
 unload.erase_registry=[&](auto i,auto id,auto&){assert(directory.at(i).identity==id);trace.push_back(400+id);directory.erase(directory.begin()+i);return true;};
 unload.reset_scan_for_anims=[&](const auto& borrowed,auto&){assert(!borrowed.identity||borrowed.identity==movie);trace.push_back(500);return true;};
 unload.unload_swf_file=[&](auto id,auto&){trace.push_back(600+id);return true;};
 MenuManagerUnloadV58 loading(unload);
 assert(loading.unload(1,error));
 assert((trace==std::vector<int>{10,21,101,201,301,401,103,203,403,104,204,304,404,500,601}));
 assert(directory.size()==1&&directory[0].identity==2&&directory[0].render==99);
 trace.clear();assert(loading.unload(0,error));assert((trace==std::vector<int>{20,500,600}));
 trace.clear();assert(loading.unload(-1,error));assert((trace==std::vector<int>{500,599}));
 directory.push_back({5,movie,1});trace.clear();fail_delete=true;
 assert(!loading.unload(2,error));assert((trace==std::vector<int>{22,105,205,305}));
 assert(directory.size()==2&&!find(5).render); // reached prefix persists; no movie unload

 std::vector<int> update_trace;bool early=false,in_game=true;std::uint8_t hud_gate=1;
 int level_reads=0,count_reads=0;std::vector<std::uintptr_t> receivers{71,72};
 MenuManagerUpdateServicesV58 update;update.actual_manager=manager;
 update.source_prefix=[&](bool& out,std::int32_t& dt,auto&){out=early;dt=17;update_trace.push_back(1);return true;};
 update.movie_slot=[&](auto slot,auto& out,auto&){update_trace.push_back(10+slot);out={manager,movie+slot};return true;};
 update.current_level=[&](auto& out,auto&){++level_reads;out={manager,123,&hud_gate};return true;};
 update.movie_virtual10=[&](const auto& m,auto dt,bool flag,auto&){assert(dt==17&&!flag);update_trace.push_back(20+m.identity-movie);return true;};
 update.currently_in_game_view=[&](bool& out,auto&){out=in_game;update_trace.push_back(30);return true;};
 update.info_hud_update=[&](auto&){update_trace.push_back(31);return true;};
 update.hud_controls_update=[&](auto&){update_trace.push_back(32);return true;};
 update.flash_anim_update=[&](auto&){update_trace.push_back(33);return true;};
 update.get_num_menus=[&](auto& out,auto&){++count_reads;out=receivers.size();return true;};
 update.registry_at=[&](auto i,auto& out,auto&){out=receivers.at(i);return true;};
 update.menu_is_visible=[&](auto id,bool& visible,auto&){visible=id==71;
  if(id==71){receivers[0]=73;receivers.push_back(74);}return true;};
 update.menu_virtual1c=[&](auto id,auto&){assert(id==73);update_trace.push_back(40);return true;};
 MenuManagerUpdateV58 frames(update);
 assert(frames.update(true,error));assert(level_reads==2&&count_reads==1);
 assert((update_trace==std::vector<int>{1,13,23,30,31,32,33,40})); // captured count; reread receiver
 receivers={71,72};update_trace.clear();level_reads=0;count_reads=0;hud_gate=0;in_game=false;
 assert(frames.update(false,error));assert(level_reads==5&&count_reads==1);
 assert((update_trace==std::vector<int>{1,10,20,11,21,12,22,13,30,33,40}));
 early=true;update_trace.clear();assert(frames.update(true,error));assert((update_trace==std::vector<int>{1}));
 update.source_prefix={};MenuManagerUpdateV58 required(update);error.clear();assert(!required.update(true,error));assert(!error.empty());

 AuthoredMenuApplicationFieldsV3 application;
 assert(!application.application_ec());auto source_ec=std::make_shared<std::uint8_t>(1);
 std::weak_ptr<std::uint8_t> retained=source_ec;
 assert(application.bind_application_ec_v58(source_ec,source_ec.get(),error));
 assert(*source_ec==1); // binding does not initialize/reset existing byte
 application.store_application_ec_v58(0);assert(*source_ec==0);
 *source_ec=1;assert(*application.application_ec()==1); // outside GS/App write stays visible
 auto other=std::make_shared<std::uint8_t>(0);
 assert(!application.bind_application_ec_v58(other,other.get(),error));
 source_ec.reset();assert(!retained.expired());application.store_application_ec_v58(0);
 assert(*retained.lock()==0);
 std::cout<<"Menu unload/update ordering, live mutation and SAME App byte fixture checks passed\n";
}
