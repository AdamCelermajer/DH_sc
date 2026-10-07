#include "character_stance.hpp"
extern "C" int dh2_character_anim_stance(std::int32_t* out,const dh2::character::StanceFacts16* f){
 using namespace dh2::character;
 if(!out||!f||(f->predicates&~63u)||f->reserved[0]||f->reserved[1])return -1;
 std::int32_t candidate=0;
 if(f->predicates&stance_is_player){
  if(f->predicates&stance_has_staff)candidate=3;
  else if(f->predicates&stance_has_bow)candidate=4;
  else if(f->predicates&stance_dual_wielding)candidate=2;
  else if(f->predicates&stance_has_two_hander)candidate=1;
  else if(!(f->predicates&stance_has_main_hand))candidate=5;
 }
 *out=candidate<f->count?candidate:0;return 1;
}
