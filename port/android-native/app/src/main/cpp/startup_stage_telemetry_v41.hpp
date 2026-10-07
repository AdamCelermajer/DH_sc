#pragma once
#include <array>
#include <cstdint>
#include <mutex>
#include <string>
namespace dh2::startup_v41 {
enum class Stage:unsigned {activity_create,asset_listing,surface_create,wait_valid_surface,native_initialize,shader_validation,ui_gpu_initialize,front_initialize,surface_resize,load_selected,java_asset_read,java_provenance_read,jni_input_copy,world_load,design_assets,world_parse,object_resources,texture_decode,texture_upload,player_construct,level_c1,world_publish,hud_load,ui_constants,ui_localization,swf_parse,ui_bitmap_decode,ui_bitmap_upload,ui_font_upload,hud_bind,first_world_submit,first_hud_submit,native_draw,menu_launch,count};
enum class Request:unsigned {cold_start,context_restore,demo_launch,source_campaign};
enum class Outcome:unsigned {measured_return,cancelled,failed};
constexpr unsigned stage_count=unsigned(Stage::count),active_limit=64,event_limit=128;
const char* name(Stage) noexcept;
struct Aggregate {std::uint64_t calls{},failures{},inclusive_ns{},exclusive_ns{},max_ns{},bytes_in{},bytes_out{},first_start_ns{},last_end_ns{},thread_id{};bool mixed_threads{};};
struct Event {std::uint64_t ticket{},parent{},start_ns{},end_ns{},thread_id{},bytes_in{},bytes_out{};Stage stage{};bool success{};};
struct Snapshot {
 std::uint64_t generation{},native_begin_ns{},native_end_ns{},java_origin_ns{},late_closes{},dropped_starts{},dropped_history{},invalid_events{},open_spans{},invalid_nesting{};
 Request request{};Outcome outcome{};bool finished{},numeric_overflow{};
 std::array<Aggregate,stage_count> stages{};std::array<Event,event_limit> events{};unsigned event_count{};
};
// Measurement owner only: no Level/GS/menu/progress or rendering state mutation.
// All recording storage is fixed. JSON is allocated only when explicitly read.
class Recorder {
public:
 using Value=std::uint64_t(*)(void*);
 explicit Recorder(Value clock=nullptr,Value thread=nullptr,void* context=nullptr);
 std::uint64_t begin(Request,std::uint64_t java_origin_ns=0) noexcept;
 std::uint64_t start(std::uint64_t generation,Stage,std::uint64_t bytes_in=0) noexcept;
 std::uint64_t start_current(Stage,std::uint64_t bytes_in=0) noexcept;
 bool end(std::uint64_t ticket,bool success=true,std::uint64_t bytes_out=0) noexcept;
 bool finish(std::uint64_t generation,Outcome) noexcept;
 Snapshot snapshot() const noexcept;
private:
 struct Active {Event event{};std::uint64_t children_ns{};bool occupied{};};
 mutable std::mutex mutex_;Value clock_,thread_;void* context_;Snapshot state_{};
 std::array<Active,active_limit> active_{};std::uint64_t next_ticket_{1},event_sequence_{};
 std::uint64_t start_locked(std::uint64_t,Stage,std::uint64_t) noexcept;
 void add(std::uint64_t&,std::uint64_t) noexcept;
};
Recorder& global() noexcept;
std::string json(const Snapshot&,bool include_events=false);
class Scope {
 Recorder* recorder_;std::uint64_t ticket_,bytes_out_{};int exceptions_;bool success_=true;
public:
 explicit Scope(Stage,std::uint64_t bytes_in=0);
 Scope(Recorder&,Stage,std::uint64_t bytes_in=0);
 Scope(const Scope&)=delete;Scope& operator=(const Scope&)=delete;
 ~Scope();void fail() noexcept{success_=false;}void output_bytes(std::uint64_t value) noexcept{bytes_out_=value;}
};
}
