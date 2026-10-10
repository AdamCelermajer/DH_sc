#pragma once
#include "../../playable_actor_world.hpp"
#include "../../hud_glyphs.hpp"
#include "../../renderer.hpp"
#include "../../../level-world/character_combat_text_v1.hpp"
#include <functional>

namespace dh::foundation {
struct CombatTextDisplayEvent {
    Vec3 position;
    std::string style, text;
    std::int32_t argb=0;
    bool numeric=false;
};
// Uses genuine original outcome flags and caller-owned actor/design/string
// services. enqueue is replaced with an owned event sink. Reached display
// prefixes survive later provider failure, matching the original selector.
bool select_combat_text(const PlayableCombatResolution&,
    const dh2::character::skills::CombatTextServicesV1&,
    std::vector<CombatTextDisplayEvent>& output,std::string& error);

struct CombatTextStyle {
    const char* name;
    const std::array<float,6>* frames;
    std::size_t frame_count;
    std::uint32_t font;
    float height;
    std::array<float,4> bounds;
    std::array<float,3> font_metrics;
    unsigned align;
};
const CombatTextStyle* original_combat_text_style(const std::string&) noexcept;
struct CombatTextGlyph {
    HudGlyphQuad glyph;
    std::array<float,8> xy{}; // screen corners TL,TR,BR,BL
    std::array<float,4> rgba{};
};
class CombatTextPresenter {
public:
    using Project=std::function<bool(Vec3,float&,float&,std::string&)>;
    // Original Flash captures projected location once at enqueue. Scale maps
    // source HUD480x320 to the caller's actual viewport; no camera invented.
    bool enqueue(const CombatTextDisplayEvent&,const Project&,float scale_x,float scale_y,std::string& error);
    bool update_ms(std::uint32_t dt,std::int32_t interval,std::string& error);
    bool geometry(HudGlyphFont& original_font,float scale_x,float scale_y,
                  std::vector<CombatTextGlyph>& output,std::string& error) const;
    void clear() noexcept;
    std::size_t active_count() const noexcept;
private:
    struct Context {bool active=false;CombatTextDisplayEvent event;const CombatTextStyle* style=nullptr;std::int32_t x=0,y=0,timer=0;std::size_t frame=0;};
    std::array<Context,12> contexts_{}; // original queue capacity, full queue drops
};
} // namespace dh::foundation
