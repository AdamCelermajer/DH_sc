#include "../actor_blended_playback.hpp"
#include <cassert>
#include <iostream>
using namespace dh2;
struct Trace {unsigned count{};bool reenter{};};
static void observed(void* p,actor::BlendedPlayback& playback,const actor::BlendedPlaybackEvent& event){
 auto& t=*static_cast<Trace*>(p);assert(event.event.handoff.event_id==0x22);
 assert(playback.sequence_closed==1&&playback.animation_depth()==0);
 assert(playback.slots[0].timeline.scale==0&&playback.slots[1].timeline.scale==0);
 assert(playback.current_timeline().current_ms==playback.current_timeline().end_ms);
 ++t.count;if(t.reenter){playback.sequence_closed=0;playback.slots[0].timeline.scale=2;}
}
int main(){
 // Explicit retained-owner field fixture; no fabricated authored resource/event.
 actor::BlendedPlayback playback;std::string error;Trace trace;
 playback.observer={&trace,observed};playback.blend.current=1;
 for(auto& slot:playback.slots){slot.timeline.library_present=0;assert(!dh2_timeline_range(&slot.timeline,50,450,1));slot.timeline.scale=3;}
 playback.sequence_closed=0;
 assert(playback.stop_immediate_v1(false,error)&&trace.count==0&&playback.sequence_closed==0);
 assert(playback.slots[0].timeline.scale==3);
 assert(playback.stop_immediate_v1(true,error)&&trace.count==1&&playback.sequence_closed==1);
 assert(playback.stop_immediate_v1(true,error)&&trace.count==1);
 playback.sequence_closed=0;trace.reenter=true;
 assert(playback.stop_immediate_v1(true,error)&&trace.count==2);
 assert(playback.sequence_closed==0&&playback.slots[0].timeline.scale==2);
 playback.blend.current=2;assert(!playback.stop_immediate_v1(true,error));
 std::cout<<"PASS retained immediate stop: null visual, selected end, all scales, once22, reentry, invalid slot\n";
}
