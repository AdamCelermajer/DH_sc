#include "../base_index_animation_controller_v2.hpp"
#include <cassert>
#include <iostream>
int main(){using namespace dh2::world;dh2::timeline::State t{};std::uint32_t flags=0;std::int32_t current=0,extra=5;unsigned selects=0,roots=0;std::string error;bool accepted=false;
 t.start_ms=10;t.end_ms=100;t.current_ms=60;t.scale=1;t.loop=0;
 BaseNamedAnimationBorrowV1 b;b.timeline=&t;b.scene_flags11c=&flags;b.timeline_library_count=1;b.applicator_extra_ms=&extra;
 b.animator_current=[&](auto& out,auto&){out=current;return true;};b.animator_select=[&](auto value,auto&){current=value;++selects;return true;};b.timeline_get_loop=[&](auto& out,auto&){out=t.loop!=0;return true;};b.root_new_anim=[&](bool reset,auto&){assert(!reset);++roots;return true;};
 assert(base_index_animation_play_v2(b,1,false,accepted,error)&&!accepted&&!selects&&!roots);
 assert(base_index_animation_play_v2(b,0,false,accepted,error)&&accepted&&selects==1&&roots==1&&t.current_ms==15&&(flags&0x200));
 t.loop=1;t.current_ms=40;assert(base_index_animation_play_v2(b,0,false,accepted,error)&&accepted&&t.current_ms==40&&!t.loop);
 b.root_new_anim={};assert(!base_index_animation_play_v2(b,0,false,accepted,error)&&!accepted);
 std::cout<<"Base index PlayClip source branch PASS; animator endpoint fixtures explicit\n";
}
