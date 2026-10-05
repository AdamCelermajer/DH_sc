#pragma once
#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include "swf_viewport_connection.hpp"
#include "hud_sprite_core.hpp"
#include "swf_actionscript_connection.hpp"
#include "swf_input_connection.hpp"

namespace gameswf { struct font; struct glyph_provider; }
namespace dh2::ui {
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
 // Live connections retain the exact Impl graph and execute inside its core
 // Scope. Authored source bounds replace the inspection viewport setter.
 bool connect_viewport(const ViewportState64&,const SwfViewportDriver&,std::string&);
 bool update_viewport(FlashCamera40&,std::string&);
 bool display_source_clip(const char* path,std::string&);
 bool screen_to_logical(float point[2],std::string&);
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
 bool input_cancel(float x,float y,std::string&);
 bool input_advance(std::int32_t milliseconds,std::string&);
 bool input_raw_position(int& x,int& y,std::string&);
 gameswf::font* borrowed_font(std::int32_t resource_id) const; // invalidated by destruction/reload
 const std::vector<std::string>& diagnostics() const;
 std::uintptr_t player_identity() const noexcept; // retained graph identity, no VM operation
 private: struct Impl;std::shared_ptr<Impl> impl_;
 std::shared_ptr<SwfViewportConnection> viewport_;
 std::shared_ptr<SwfAsGraph> action_script_;
 std::shared_ptr<SwfInputConnection> input_;
};
} // namespace dh2::ui
