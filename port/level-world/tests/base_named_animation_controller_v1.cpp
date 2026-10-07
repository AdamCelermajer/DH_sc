#include "base_named_animation_controller_v1.hpp"
#include <cassert>
#include <iostream>
int main(){using namespace dh2::world;dh2::timeline::State t{};t.scale=1;t.library_present=1;t.initialized=1;t.start_ms=10;t.end_ms=200;t.current_ms=90;
 std::uint32_t flags=0;std::int32_t current=1,extra=7;bool active=true;unsigned new_calls=0,lookups=0;
 BaseNamedAnimationBorrowV1 b;b.timeline=&t;b.scene_flags11c=&flags;b.timeline_library_count=3;b.applicator_extra_ms=&extra;
 b.timeline_find_name=[&](auto name,auto& id,auto&){++lookups;id=std::string(name)=="absent"?-1:1;return true;};
 b.animator_current=[&](auto& id,auto&){id=current;return true;};
 b.animator_find_name=[](auto name,auto& id,auto&){id=std::string(name)=="activate"?2:1;return true;};
 b.animator_select=[&](auto id,auto&){current=id;return true;};
 b.timeline_get_loop=[&](auto& v,auto&){v=active;return true;};
 b.root_new_anim=[&](auto displacement,auto&){assert(!displacement);++new_calls;return true;};
 std::string e;bool accepted=true;
 assert(base_named_animation_play_v1(b,"absent",false,accepted,e)&&!accepted&&current==1&&new_calls==0);
 assert(base_named_animation_play_v1(b,"idle",false,accepted,e)&&accepted&&t.current_ms==90&&flags==0x200&&new_calls==1);
 active=false;assert(base_named_animation_play_v1(b,"idle",true,accepted,e)&&accepted&&t.current_ms==17&&t.loop==1&&t.scale==1);
 t.current_ms=80;assert(base_named_animation_play_v1(b,"activate",false,accepted,e)&&accepted&&current==2&&t.current_ms==80);
 b.timeline_library_count=0;auto n=lookups;assert(base_named_animation_play_v1(b,"activate",false,accepted,e)&&lookups==n);
 base_named_animation_set_callbacks_v1();assert(current==2&&flags==0x200);
 b.root_new_anim={};assert(!base_named_animation_play_v1(b,"idle",false,accepted,e));
 std::cout<<"Source base named-animation missing/active/inactive/changed/library branches PASS (declared animator fixtures)\n";
}
