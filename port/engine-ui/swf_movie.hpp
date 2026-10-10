#pragma once
#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <functional>
#include "swf_viewport_connection.hpp"
#include "hud_sprite_core.hpp"
#include "swf_actionscript_connection.hpp"
#include "swf_input_connection.hpp"

namespace gameswf { struct font; struct glyph_provider; }
namespace dh2::ui {
class SwfControllerStorageV91;
class SwfFrameConnection;
// Native upstream GameSWF facade. Coordinates are SWF twips, not pixels.
struct SwfMatrix { float value[6]{1,0,0,0,1,0}; }; // row-major 2x3
struct SwfColorTransform { float value[8]{1,0,1,0,1,0,1,0}; }; // RGBA [multiply,add]
struct SwfTexture { std::uintptr_t identity{}; std::int32_t width{},height{}; };
struct SwfFill {
 enum Kind : std::uint32_t { disabled,color,bitmap } kind{disabled};
 std::uint8_t rgba[4]{255,255,255,255}; SwfTexture texture{};
 SwfMatrix uv{}; std::uint32_t wrap{},blend{};
};
struct SwfDraw {
 enum Kind : std::uint32_t { begin,end,triangles,triangle_strip,line_strip,bitmap_quad,
                           mask_begin,mask_end,mask_disable,antialias } kind{};
 SwfMatrix matrix{}; SwfColorTransform color_transform{};
 SwfFill fill{},line{}; float line_width{};
 std::vector<float> xy; // exact converted signed16 vertices; quad uses four corners
 float rect[4]{},uv_rect[4]{},bounds[4]{}; // xmin,xmax,ymin,ymax
 std::int32_t viewport[4]{}; std::uint8_t background[4]{};
 bool enabled{};
};
struct SwfValue {
 enum Kind : std::uint32_t { undefined,boolean,number,text } kind{};
 double numeric{}; std::string string;
};
struct SwfServices {
 void* context{};
 // Returned bytes are copied into owned stream backing before the callback returns.
 bool (*read)(void*,const char* uri,std::vector<std::uint8_t>& bytes,std::string& error){};
 // Original substitute_bitmap_character callback receives export NAME and source dimensions.
 // Provider owns texture lifetime through all movies using it. Null identity rejects delivery.
 bool (*texture)(void*,const char* name,std::int32_t width,std::int32_t height,SwfTexture&,std::string&){};
 // Embedded bitmap/glyph upload. Pixels are borrowed only for this synchronous call.
 bool (*image)(void*,std::int32_t width,std::int32_t height,std::uint32_t channels,
               const std::uint8_t* pixels,std::int32_t pitch,SwfTexture&,std::string&){};
 bool (*draw)(void*,const SwfDraw&,std::string&){};
 bool (*native_call)(void*,const char* name,const std::vector<SwfValue>&,SwfValue&,std::string&){};
 // Required for a backend using stencil-dependent queries, separate from mask commands.
 bool (*stencil)(void*,const float bounds[4],std::uint8_t pattern,bool& result,std::string&){};
 void (*diagnostic)(void*,bool error,const char* message){};
 // Borrowed provider; facade installs it only during this movie's scoped core calls.
 gameswf::glyph_provider* glyphs{};
 // Additional original native entry points are installed BEFORE shared/root
 // initialization. Callback arguments/results keep actual AS tags/objects.
 // Owner must retain context and all services used by these entry points.
 std::vector<std::string> native_actions;
 std::shared_ptr<void> native_owner;
 bool (*native_action)(void*,const char*,const gameswf::fn_call&,std::string&){};
 // Install exact-player construction/setter observers before loading ANY
 // shared/root character. Provider uses weak leases to avoid owning a cycle;
 // native_owner retains it through the graph's complete teardown.
 bool (*graph_start)(void*,const SwfAsLease&,std::string&){};
 //Reached original RenderFX.SetTextBufferingEnabled: SAME native root byte85.
 //Typed real engine leaf; owner retained by native_owner, no replacement root.
 std::function<bool(const SwfAsLease&,bool,std::string&)> source_text_buffering_v98;
 std::function<bool(std::string&)> source_continue_v98; //SAME source Session failure/scope gate
 // RenderFX.ClearFonts: retire only this renderer's text images/cache and
 // blank its live edit fields. The exact texture owner performs removal.
 std::function<bool(const SwfTexture&,std::string&)> release_image_v119;
 //Scoped original fscommand(character*,command,arg) transport. The facade
 //checks the originating character's actual player before this callback.
 //Dispatch occurs only for actual positive RenderFX listenerfc; the exact
 //captured receiver and its live lease are retained through the call. NULL
 //listener is the original no-op, including during initial movie loading.
 //Arguments are borrowed unchanged for this synchronous call. false means
 //provider failure; true+handled=false is genuine source-unhandled, not a
 //missing positive leaf. native_owner retains the provider through teardown.
 //std::function captures remain valid when wrappers rebase service.context.
 std::function<bool(std::uintptr_t actual_listener_fc,const char*,const char*,bool& handled,std::string&)> source_fscommand_v114;
 //A positive source handler without its virtual4 transport is an error.
 //The actual dispatcher reports unknown commands with handled=false and
 //required positive effects with false, rather than disguising failures.
};
struct SwfClipInfo { std::int32_t id{},depth{},frame{},frames{};bool visible{};SwfMatrix local{},world{}; };
class SwfHudClip {
 public:
 std::uintptr_t identity() const noexcept {return reinterpret_cast<std::uintptr_t>(binding_.sprite);}
 private:
 friend class SwfMovie;
 std::shared_ptr<void> owner_;
 HudSpriteCoreBindingV1 binding_{};
};
class SwfMovie {
 public:
 SwfMovie(); ~SwfMovie(); SwfMovie(SwfMovie&&) noexcept; SwfMovie& operator=(SwfMovie&&) noexcept;
 SwfMovie(const SwfMovie&)=delete;SwfMovie& operator=(const SwfMovie&)=delete;
 // Load shared files first in supplied order, retaining one player/global AS namespace.
 // No GPU, font resolver, game globals or missing native functions are fabricated.
 bool load(const std::vector<std::string>& shared,const std::string& movie,const SwfServices&,std::string&);
 //Original RenderFX.Load: supplied URI/workdir, no eager shared preload/frame.
 bool load_source_resource_v98(const char* actual_uri,const SwfServices&,std::string&);
 bool source_set_text_buffering_v98(bool,std::string&);
 bool source_register_native_actions_v119(const std::vector<std::string>&,std::string&);
 bool source_preload_glyphs_v119(const char* actual_path,const char* codes,std::string&);
 // Full MenuFX resource operation on the actual constructor controller owner.
 // Caller supplies reached renderer virtualA4 and same owned state arrays.
 bool unload_menu_resource_v91(struct SwfMenuUnloadServicesV91&,std::string&);
 std::shared_ptr<SwfControllerStorageV91> controller_storage_v91()const noexcept;
 // InputSession's external connection adopts the same constructor storage.
 // Weak generation callbacks must not form a movie -> generation -> movie cycle.
 void bind_controller_context_clear_v91(std::function<bool(std::string&)>);

 //Actual global movie delivery scopes, including native callbacks before World C1.
 static bool source_dispatch_pending_v115(bool&,std::string&);
 bool source_update_v93(std::int32_t milliseconds,bool source_advance_flag,std::string&);
 //Actual RenderFX.SetEventListener7a7c6c store. Transport borrows the retained
 //native receiver weakly so a resource cannot keep its manager/world alive.
 bool source_set_event_listener_v68(std::uintptr_t,std::weak_ptr<void>,
       std::function<bool(SwfEvent48&,std::string&)>,
       std::function<bool(SwfEvent48&,bool&,std::string&)>,std::string&);
 bool source_raw_event_position_v68(std::int32_t cursor,int&,int&,std::string&);
 bool source_bind_fs_command_v114(decltype(SwfServices::source_fscommand_v114),std::string&);
 bool source_event_focus_v68(std::uintptr_t,std::uint32_t,std::string&);
 bool advance(float seconds,std::string&); // caller supplies seconds once
 bool advance_frames(std::int32_t milliseconds,SwfFrameConnection&,std::string&);
 bool display(std::int32_t x,std::int32_t y,std::int32_t width,std::int32_t height,std::string&);
 bool display_clip(const char* path,std::string&); // brackets draw using current root viewport
 bool display_clip(const char* path,std::int32_t x,std::int32_t y,std::int32_t width,std::int32_t height,std::string&);
 bool clip(const char* path,SwfClipInfo&,std::string&);
 bool set_number(const char* path,double,std::string&);
 bool set_visible(const char* path,bool,std::string&);
 // PostLoad/RegisterState visibility stage only; does not fabricate native
 // menu instances, run Create, or install stack/lifecycle ownership.
 bool hide_menu_state_clips(std::vector<std::string>& names,std::string&);
 // Original RenderFX.ClearFonts on this exact player/root.
 bool source_reset_fonts_v119(std::string&);
 // Live connections retain the exact Impl graph and execute inside its core
 // Scope. Authored source bounds replace the inspection viewport setter.
 bool connect_viewport(const ViewportState64&,const SwfViewportDriver&,std::string&);
  bool update_viewport(FlashCamera40&,std::string&);
  bool set_source_bounds(const std::int32_t xywh[4],std::int32_t aspect_mode,std::string&);
 bool display_source_clip(const char* path,std::string&);
 // Modern letterboxed panel policy: clip authored overscan to the movie stage,
 // retaining the existing source projection and pointer conversion.
 bool display_source_stage_clip_v5(const char* path,std::string&);
  bool screen_to_logical(float point[2],std::string&);
  bool source_display_rectangle(float rectangle[4],std::int32_t viewport[4],std::string&);
 bool hud_bind(const char* path,const char* verified_movie_sha256,SwfHudClip&,std::string&);
 bool hud_goto(const SwfHudClip&,std::int32_t,const HudSpriteCoreServices&,std::string&);
 bool hud_play(const SwfHudClip&,std::int32_t,const HudSpriteCoreServices&,std::string&);
 // Execute a connected manager/input batch in one existing core Scope.
 bool action_script(void*,bool (*apply)(void*,SwfAsGraph&,std::string&),std::string&);
 // Explicit synchronous menu-manager dispatch, including another renderer
 // called from an authored/native callback. Restores the caller's providers
 // and retains errors across same-renderer nesting. Ordinary facade calls
 // continue to reject recursive entry.
 bool menu_action_script(void*,bool (*apply)(void*,SwfAsGraph&,std::string&),std::string&);
   // Real upstream character display hook, invoked after its authored children.
  // Context must outlive this movie; the movie pins the character and hook.
  bool menu_display_callback(const char* path,void* context,
      bool (*draw)(void*,const SwfDraw&,std::string&),std::string&);
  bool menu_input_context(const char* path,std::string&);
 bool menu_input_behavior(std::uint32_t flags,std::string&);
 bool connect_input(const char* context,std::shared_ptr<SwfInputHistory>,std::uint32_t flags,
                    std::uint32_t& selection,const SwfViewportDriver&,const SwfInputCoreServices&,std::string&);
 bool input_rectangle(const std::int32_t xywh[4],std::string&);
 bool input_cursor(const SwfCursor16&,std::string&);
 bool input_cursor_slot_v120(const SwfCursor16&,std::uint32_t,std::string&);
 bool input_cancel(float x,float y,std::string&);
 bool input_advance(std::int32_t milliseconds,std::string&);
 bool input_raw_position(int& x,int& y,std::string&);
 gameswf::font* borrowed_font(std::int32_t resource_id) const; // invalidated by destruction/reload
 const std::vector<std::string>& diagnostics() const;
 std::uintptr_t player_identity() const noexcept; // retained graph identity, no VM operation
 private:
 bool load_resource_v98(const std::vector<std::string>&,const std::string&,const SwfServices&,bool source,std::string&);
 struct Impl;std::shared_ptr<Impl> impl_;
 std::function<bool(std::string&)> external_context_clear_v91_;
 std::shared_ptr<SwfViewportConnection> viewport_;
 std::shared_ptr<SwfAsGraph> action_script_;
 std::shared_ptr<SwfInputConnection> input_;
};
} // namespace dh2::ui
