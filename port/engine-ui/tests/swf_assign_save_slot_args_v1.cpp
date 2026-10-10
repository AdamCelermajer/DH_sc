#include "swf_menu_save_slots.hpp"
#include "swf_frame_connection.hpp"
#include "swf_input_history.hpp"
#include "gameswf/gameswf_environment.h"
#include "gameswf/gameswf_movie_def.h"
#include "gameswf/gameswf_player.h"
#include "gameswf/gameswf_root.h"
#include <cstdio>
#include <stdexcept>

namespace {
void check(bool value,const char* message){if(!value)throw std::runtime_error(message);}
}

int main(){
 try{
  gameswf::gc_ptr<gameswf::player> player=new gameswf::player;
  auto history=std::make_shared<dh2::ui::SwfInputHistory>();
  dh2::ui::SwfFrameConnection frames;std::string owner_error;
  check(history->bind(player.get_ptr(),owner_error)&&frames.bind(player.get_ptr(),history,owner_error),
        "source frame owners unavailable for GameSWF callback fixture");
  gameswf::gc_ptr<gameswf::movie_def_impl> definition=new gameswf::movie_def_impl(
      player.get_ptr(),gameswf::DO_NOT_LOAD_BITMAPS,gameswf::DO_NOT_LOAD_FONT_SHAPES);
  definition->set_frame_count(1);definition->m_playlist.resize(1);definition->m_init_action_list.resize(1);
  gameswf::gc_ptr<gameswf::root> root=definition->create_root();
  for(const std::int32_t slot:{3,2}){
   gameswf::as_environment env(player.get_ptr());
   // This is the SWF's authored NativeAssignSaveSlotToPlayer(0, SlotID)
   // operand order: GameSWF's fn_call::arg(0) then sees the final SlotID push.
   env.push(gameswf::as_value(0));
   env.push(gameswf::as_value(slot));
   gameswf::as_value result,call_this;
   gameswf::fn_call fn(&result,call_this,&env,2,env.get_top_index());
   check(fn.arg(0).to_number()==slot&&fn.arg(1).to_number()==0,
         "vendor GameSWF fn_call stack indexing changed");
   std::int32_t local=-99,selected=-99;std::string error;
   check(dh2::ui::swf_front_assign_save_slot_args_v1(fn,local,selected,error),
         "production NativeAssign argument decoder rejected valid authored stack");
   check(local==0&&selected==slot,
         "NativeAssign did not preserve local-player index and selected SlotID");
   env.drop(2);
  }
  std::puts("PASS | actual GameSWF fn_call stack | creation slot3 and occupied slot2 decode local=0, selected=slot");
  return 0;
 }catch(const std::exception& error){
  std::fprintf(stderr,"FAIL | %s\n",error.what());return 1;
 }
}
