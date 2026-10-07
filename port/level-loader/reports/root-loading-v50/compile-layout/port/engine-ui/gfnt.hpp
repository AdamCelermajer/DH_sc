#pragma once
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>
namespace dh2::ui {
struct GfntInput16 {const std::uint8_t* bytes;std::uint32_t size,reserved;};
// Original bitmap_glyph_metrics words. bearing/baseline are unsigned source
// words; advance is __aeabi_f2iz after the source float32 scaling sequence.
struct GfntMetrics20 {std::uint32_t bearing_x,baseline,width,height;std::int32_t advance;};
struct GfntGlyph32 {GfntMetrics20 metrics;std::uint32_t pitch;float source_font_scale;std::uint32_t reserved;};
static_assert(sizeof(GfntInput16)==16&&sizeof(GfntMetrics20)==20&&sizeof(GfntGlyph32)==32);
struct GfntRaster {GfntGlyph32 source{};std::vector<std::uint8_t> rgba;float advance_twips=0;};
// Owns immutable actual GFNT bytes. Borrow remains valid after the facade or
// caller buffer dies. No font-name resolver, atlas, text layout or GPU owner.
class GfntFont {
 struct Snapshot;std::shared_ptr<const Snapshot> snapshot_;
public:
 class Borrow {
  friend class GfntFont;std::shared_ptr<const Snapshot> snapshot_;
  explicit Borrow(std::shared_ptr<const Snapshot> p):snapshot_(std::move(p)){}
 public:
  Borrow()=default;explicit operator bool()const noexcept{return bool(snapshot_);}
  std::uint32_t first_codepoint()const;std::uint32_t glyph_slots()const;
  // 1 present,0 source miss,-1 malformed. Failure/miss preserve the output.
  int raster(GfntRaster&,std::uint32_t codepoint,std::int32_t font_height,std::string&)const;
 };
 bool load(const std::uint8_t*,std::size_t,std::string&);
 Borrow borrow()const{return Borrow(snapshot_);}
};
}
// Caller-owned disjoint outputs. Atomic guards include malformed RLE/bounds.
// 1 present,0 source miss,-1 malformed. Pixel bytes are original RGBA; no
// premultiply, tint, padding, orientation or channel conversion is applied.
extern "C" int dh2_gfnt_raster(dh2::ui::GfntGlyph32*,std::uint8_t*,std::size_t,
 const dh2::ui::GfntInput16*,std::uint32_t,std::int32_t);
