#include "character_script_commands.hpp"
#include <cstring>
namespace {
using namespace dh2::character;
bool valid(const ScriptCommandState48* s){return s&&s->character&&!s->reserved;}
bool object(const dh2_script_value& a){return a.type==2||a.type==7;}
int invoke(ScriptCommandState48* s,const ScriptCommandServices24* c,std::uint32_t op,
 std::uintptr_t subject,std::uintptr_t target=0,const float* point=nullptr,const float** output=nullptr){
 if(!c||!c->invoke)return -2;
 ScriptCommandRequest40 r{op,0,subject,target,{0,0,0},0};if(point)std::memcpy(r.point,point,12);
 const float* ignored=nullptr;return c->invoke(c->context,s,&r,output?output:&ignored)?-2:1;
}
int number(const ScriptCommandServices24* c,const dh2_script_value& a,float& out){return c&&c->number&&!c->number(c->context,&a,&out)?1:-2;}
int point_command(ScriptCommandState48* s,std::uint32_t op,const dh2_script_value* a,std::uint32_t n,const ScriptCommandServices24* c){
 if(n==1){if(!object(a[0]))return 1;return invoke(s,c,script_controller_move_object,s->controller,a[0].identity);}
 if(n==2)return 1;
 if(!n)return -3;
 // Source accepts the point branch if ANY of the first three is numeric;
 // it subsequently converts all three Values through getNumber in order.
 if(a[0].type!=3&&a[1].type!=3&&a[2].type!=3)return 1;
 float p[3]{0,0,0};
 if(n>3&&a[3].type==1&&a[3].boolean){
  const float* heading=nullptr;
  auto result=invoke(s,c,script_look_vector,s->character,0,nullptr,&heading);if(result<0)return result;
  if(!heading||!s->position||!s->axis)return -2;
  // Source snapshots raw position, heading and the first Vec3f_K values
  // before x conversion, then reloads K for the final z contribution.
  std::memcpy(p,s->position,12);float h[3],k[3];std::memcpy(h,heading,12);std::memcpy(k,s->axis,12);
  float x=0,y=0,z=0;if(number(c,a[0],x)<0)return -2;
  const float cx=(k[1]*h[2])-(k[2]*h[1]);p[0]=p[0]+x*cx;
  const float cy=(k[2]*h[0])-(h[2]*k[0]);p[1]=p[1]+x*cy;
  const float cz=(h[1]*k[0])-(k[1]*h[0]);p[2]=p[2]+x*cz;
  if(number(c,a[1],y)<0)return -2;
  p[0]=p[0]+y*h[0];p[1]=p[1]+y*h[1];p[2]=p[2]+y*h[2];
  if(number(c,a[2],z)<0)return -2;
  p[0]=p[0]+z*s->axis[0];p[1]=p[1]+z*s->axis[1];p[2]=p[2]+z*s->axis[2];
 }else for(unsigned i=0;i<3;++i)if(number(c,a[i],p[i])<0)return -2;
 return invoke(s,c,op==script_head_to?script_controller_head_point:script_controller_move_point,s->controller,0,p);
}
}
extern "C" int dh2_character_script_command(ScriptCommandState48* s,std::uint32_t op,
 const dh2_script_value* a,std::uint32_t n,const ScriptCommandServices24* c,
 dh2_script_value* out,std::uint32_t capacity,std::uint32_t* returned){
 if(!valid(s)||op>script_has_path||!returned||(n&&!a)||(!out&&capacity))return -1;
 for(std::uint32_t i=0;i<n;++i)if(a[i].reserved||(a[i].type==1&&a[i].boolean>1))return -1;
 if(op==script_has_path&&(!out||!capacity))return -1;
 *returned=0;
 if(op==script_has_path){out[0]={};out[0].type=DH2_SCRIPT_BOOLEAN;out[0].boolean=s->path_count!=0;*returned=1;return 1;}
 if(op==script_stop)return invoke(s,c,script_controller_stop,s->controller);
 if(op==script_head_to||op==script_move_to)return point_command(s,op,a,n,c);
 if(op==script_attack){if(n){if(!object(a[0]))return 1;return invoke(s,c,script_controller_attack,s->controller,a[0].identity);}return s->target?invoke(s,c,script_controller_attack,s->controller,s->target):1;}
 if(!n||!object(a[0])||!s->target)return 1;
 // Source Flee projects but DISCARDS the supplied object identity. The
 // direction is computed from the owner's CURRENT target and position.
 const float *first=nullptr,*second=nullptr,*current=nullptr;
 auto status=invoke(s,c,script_target_position,s->character,0,nullptr,&first);if(status<0)return status;
 status=invoke(s,c,script_target_position,s->target,0,nullptr,&second);if(status<0)return status;
 if(!first||!second)return -2;
 float diff[3]{first[0]-second[0],first[1]-second[1],first[2]-second[2]};
 const auto controller=s->controller;
 status=invoke(s,c,script_target_position,s->character,0,nullptr,&current);if(status<0)return status;
 if(!current)return -2;
 float p[3]{diff[0]+current[0],diff[1]+current[1],diff[2]+current[2]};
 return invoke(s,c,script_controller_move_point,controller,0,p);
}
