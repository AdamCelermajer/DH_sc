#include "startup_stage_telemetry_v41.hpp"
#include <algorithm>
#include <chrono>
#include <exception>
#include <functional>
#include <limits>
#include <sstream>
#include <thread>
#if defined(__linux__)
#include <unistd.h>
#include <sys/syscall.h>
#endif
namespace dh2::startup_v41 {
namespace {
std::uint64_t clock_now(void*){return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();}
std::uint64_t thread_now(void*){
#if defined(__linux__)
 return static_cast<std::uint64_t>(::syscall(SYS_gettid));
#else
 return std::hash<std::thread::id>{}(std::this_thread::get_id());
#endif
}
constexpr const char* names[]{"activity_create","asset_listing","surface_create","wait_valid_surface","native_initialize","shader_validation","ui_gpu_initialize","front_initialize","surface_resize","load_selected","java_asset_read","java_provenance_read","jni_input_copy","world_load","design_assets","world_parse","object_resources","texture_decode","texture_upload","player_construct","level_c1","world_publish","hud_load","ui_constants","ui_localization","swf_parse","ui_bitmap_decode","ui_bitmap_upload","ui_font_upload","hud_bind","first_world_submit","first_hud_submit","native_draw","menu_launch"};
static_assert(sizeof(names)/sizeof(names[0])==stage_count);
}
const char* name(Stage stage) noexcept{return unsigned(stage)<stage_count?names[unsigned(stage)]:"invalid";}
Recorder::Recorder(Value clock,Value thread,void* context):clock_(clock?clock:clock_now),thread_(thread?thread:thread_now),context_(context){}
void Recorder::add(std::uint64_t& value,std::uint64_t amount) noexcept{
 if(amount>std::numeric_limits<std::uint64_t>::max()-value){value=std::numeric_limits<std::uint64_t>::max();state_.numeric_overflow=true;}
 else value+=amount;
}
std::uint64_t Recorder::begin(Request request,std::uint64_t java_origin) noexcept{
 std::lock_guard<std::mutex> lock(mutex_);const auto generation=state_.generation+1;
 if(!generation||unsigned(request)>unsigned(Request::source_campaign)){return 0;}
 state_={};state_.generation=generation;state_.request=request;state_.native_begin_ns=clock_(context_);state_.java_origin_ns=java_origin;active_={};event_sequence_=0;return generation;
}
std::uint64_t Recorder::start_locked(std::uint64_t generation,Stage stage,std::uint64_t bytes) noexcept{
 if(!state_.generation||state_.finished||generation!=state_.generation){return 0;}
 if(unsigned(stage)>=stage_count){++state_.invalid_events;return 0;}
 Active* slot=nullptr;for(auto& active:active_){if(!active.occupied){slot=&active;break;}}
 if(!slot||!next_ticket_){++state_.dropped_starts;return 0;}
 const auto thread=thread_(context_);std::uint64_t parent=0;
 for(const auto& active:active_){if(active.occupied&&active.event.thread_id==thread&&active.event.ticket>parent){parent=active.event.ticket;}}
 *slot={};slot->occupied=true;slot->event={next_ticket_++,parent,clock_(context_),0,thread,bytes,0,stage,false};return slot->event.ticket;
}
std::uint64_t Recorder::start(std::uint64_t generation,Stage stage,std::uint64_t bytes) noexcept{std::lock_guard<std::mutex> lock(mutex_);return start_locked(generation,stage,bytes);}
std::uint64_t Recorder::start_current(Stage stage,std::uint64_t bytes) noexcept{std::lock_guard<std::mutex> lock(mutex_);return start_locked(state_.generation,stage,bytes);}
bool Recorder::end(std::uint64_t ticket,bool success,std::uint64_t bytes) noexcept{
 if(!ticket){return false;}
 std::lock_guard<std::mutex> lock(mutex_);Active* slot=nullptr;
 for(auto& active:active_){if(active.occupied&&active.event.ticket==ticket){slot=&active;break;}}
 if(!slot||state_.finished){++state_.late_closes;return false;}
 auto event=slot->event;event.end_ns=clock_(context_);event.success=success;event.bytes_out=bytes;
 if(event.end_ns<event.start_ns){++state_.invalid_events;event.end_ns=event.start_ns;}
 const auto duration=event.end_ns-event.start_ns;
 bool ordered=true;for(const auto& active:active_){if(active.occupied&&active.event.parent==ticket){ordered=false;}}
 if(!ordered){++state_.invalid_nesting;}
 auto& aggregate=state_.stages[unsigned(event.stage)];add(aggregate.calls,1);add(aggregate.failures,success?0:1);add(aggregate.inclusive_ns,duration);
 add(aggregate.exclusive_ns,ordered&&slot->children_ns<=duration?duration-slot->children_ns:0);aggregate.max_ns=std::max(aggregate.max_ns,duration);add(aggregate.bytes_in,event.bytes_in);add(aggregate.bytes_out,event.bytes_out);
 if(aggregate.calls==1){aggregate.first_start_ns=event.start_ns;aggregate.thread_id=event.thread_id;}
 else if(aggregate.thread_id!=event.thread_id){aggregate.mixed_threads=true;}
 aggregate.last_end_ns=event.end_ns;
 if(event.parent){for(auto& active:active_){if(active.occupied&&active.event.ticket==event.parent){add(active.children_ns,duration);break;}}}
 slot->occupied=false;
 // Keep the first32 completions and a rolling last96: initialization anchors
 // survive large texture/glyph workloads while totals include every completion.
 const auto index=event_sequence_<32?event_sequence_:32+(event_sequence_-32)%96;
 state_.events[index]=event;if(event_sequence_>=event_limit){++state_.dropped_history;}
 ++event_sequence_;state_.event_count=unsigned(std::min<std::uint64_t>(event_sequence_,event_limit));return true;
}
bool Recorder::finish(std::uint64_t generation,Outcome outcome) noexcept{
 std::lock_guard<std::mutex> lock(mutex_);if(!generation||generation!=state_.generation||state_.finished||unsigned(outcome)>unsigned(Outcome::failed)){return false;}
 state_.finished=true;state_.outcome=outcome;state_.native_end_ns=clock_(context_);for(const auto& active:active_){state_.open_spans+=active.occupied;}
 return true;
}
Snapshot Recorder::snapshot() const noexcept{
 std::lock_guard<std::mutex> lock(mutex_);auto result=state_;
 if(event_sequence_>event_limit){const auto start=(event_sequence_-32)%96;for(unsigned i=0;i<96;++i){result.events[32+i]=state_.events[32+(start+i)%96];}}
 return result;
}
Recorder& global() noexcept{static Recorder owner;return owner;}
Scope::Scope(Stage stage,std::uint64_t bytes):Scope(global(),stage,bytes){}
Scope::Scope(Recorder& recorder,Stage stage,std::uint64_t bytes):recorder_(&recorder),ticket_(recorder.start_current(stage,bytes)),exceptions_(std::uncaught_exceptions()){}
Scope::~Scope(){recorder_->end(ticket_,success_&&std::uncaught_exceptions()==exceptions_,bytes_out_);}
std::string json(const Snapshot& snapshot,bool events){
 std::ostringstream out;out<<"{\"schema\":\"dh2-startup-v41\",\"generation\":"<<snapshot.generation<<",\"request\":"<<unsigned(snapshot.request)<<",\"finished\":"<<(snapshot.finished?"true":"false")<<",\"outcome\":"<<unsigned(snapshot.outcome)<<",\"native_begin_ns\":"<<snapshot.native_begin_ns<<",\"native_end_ns\":"<<snapshot.native_end_ns<<",\"java_origin_ns\":"<<snapshot.java_origin_ns<<",\"late_closes\":"<<snapshot.late_closes<<",\"dropped_starts\":"<<snapshot.dropped_starts<<",\"dropped_history\":"<<snapshot.dropped_history<<",\"invalid_events\":"<<snapshot.invalid_events<<",\"invalid_nesting\":"<<snapshot.invalid_nesting<<",\"open_spans\":"<<snapshot.open_spans<<",\"numeric_overflow\":"<<(snapshot.numeric_overflow?"true":"false")<<",\"stages\":[";
 bool comma=false;for(unsigned i=0;i<stage_count;++i){const auto& row=snapshot.stages[i];if(!row.calls){continue;}if(comma){out<<',';}comma=true;
 out<<"{\"stage\":\""<<names[i]<<"\",\"calls\":"<<row.calls<<",\"failures\":"<<row.failures<<",\"inclusive_ns\":"<<row.inclusive_ns<<",\"exclusive_ns\":"<<row.exclusive_ns<<",\"max_ns\":"<<row.max_ns<<",\"bytes_in\":"<<row.bytes_in<<",\"bytes_out\":"<<row.bytes_out<<",\"first_start_ns\":"<<row.first_start_ns<<",\"last_end_ns\":"<<row.last_end_ns<<",\"thread_id\":"<<row.thread_id<<",\"mixed_threads\":"<<(row.mixed_threads?"true":"false")<<'}';}
 out<<']';if(events){out<<",\"events\":[";for(unsigned i=0;i<snapshot.event_count;++i){const auto& row=snapshot.events[i];if(i){out<<',';}out<<"{\"ticket\":"<<row.ticket<<",\"parent\":"<<row.parent<<",\"stage\":\""<<name(row.stage)<<"\",\"start_ns\":"<<row.start_ns<<",\"end_ns\":"<<row.end_ns<<",\"thread_id\":"<<row.thread_id<<",\"success\":"<<(row.success?"true":"false")<<'}';}out<<']';}out<<'}';return out.str();
}
}
