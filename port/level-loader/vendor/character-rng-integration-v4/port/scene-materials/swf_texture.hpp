#pragma once
#include <array>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
namespace dh2::scene {
// Complete FileSystemBase.ApplyFilenameHacks string projection. The original
// WorkingDirectory snapshot is explicit; actual archive/disk/cache owners follow.
bool swf_texture_filename(std::string_view working_directory,std::string_view request,
                          std::string& output,std::string& error);
// CZipReader.findFile preparation; archive flags are supplied by its real owner.
// No physical-file/first-match or archive registration policy is invented here.
bool swf_texture_archive_key(std::string_view request,bool ignore_case,bool ignore_path,
                             std::string& output,std::string& error);
struct SwfTextureState8 {std::uint32_t packed;std::uint16_t dirty,reserved;};
static_assert(sizeof(SwfTextureState8)==8);
// Exact ITexture.setWrap packed bits, including its source Z-axis condition.
void swf_texture_set_wrap(SwfTextureState8&,std::uint32_t requested) noexcept;
// Call only after the genuine texture bind/setParameter service and source
// current texture reread. Missing texture/parameter means no vector request.
std::optional<std::array<float,4>> swf_texture_diffuse(bool texture_present,
    std::uint16_t parameter_id,std::uint32_t current_texture_flags) noexcept;
// Original GLES2 table projection; no texture/GPU mutation. Values outside
// the proved logical enum domain fail; modern extension fallbacks are not added.
bool swf_texture_gl_wrap(std::uint32_t code,std::uint32_t& output) noexcept;
bool swf_texture_gl_filter(std::uint32_t code,std::uint32_t& output) noexcept;
struct SwfSourceRenderState76 {std::array<std::uint32_t,19> words;};
struct SwfRenderState32 {std::array<std::uint32_t,8> words;};
static_assert(sizeof(SwfSourceRenderState76)==76&&sizeof(SwfRenderState32)==32);
// Complete original renderpass constructor projection; serialized source state
// bytes must pass through this conversion before interpreting driver bitfields.
SwfRenderState32 swf_render_state(const SwfSourceRenderState76&) noexcept;
struct SwfBlend16 {std::uint32_t enabled,equation,source,destination;};
// Validated original GL table projection. Other GPU states remain in state32.
bool swf_render_blend(const SwfRenderState32&,SwfBlend16&) noexcept;
struct SwfCxform32 {std::array<float,8> terms;}; // [multiply,add] per R,G,B,A
struct SwfBitmapColor40 {SwfCxform32 clamped;std::array<std::uint8_t,4> rgba;std::uint32_t additive;};
static_assert(sizeof(SwfCxform32)==32&&sizeof(SwfBitmapColor40)==40);
std::array<std::uint8_t,4> swf_solid_color(const SwfCxform32&,const std::array<std::uint8_t,4>&) noexcept;
SwfBitmapColor40 swf_bitmap_color(const SwfCxform32&) noexcept;
}
