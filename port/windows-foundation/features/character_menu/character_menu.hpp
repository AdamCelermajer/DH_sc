#pragma once
#include "../../hud_geometry.hpp"
#include "../../actor_state.hpp"
#include "../../original_combat_properties.hpp"
#include <functional>
namespace dh::foundation::character_menu {
enum class Tab { stats,equipment,skills,faery,quest }; // P16 QUESTUI: quest = the Quest Log tab (btnQuestLogTab)
enum class Action { none,stats,equipment,skills,faery,close,quest };
struct MenuTextField {
    std::string path;std::uint32_t character_id=0,font_id=0;float source_height=0;
    std::array<float,4> bounds{};std::array<std::uint8_t,4> rgba{};unsigned align=0;
    // Actual field transform/local RECT and paragraph spacing, in source
    // pixels. Root may retain source typography rather than flattening bounds.
    std::array<float,6> matrix{1,0,0,1,0,0};
    std::array<float,4> local_bounds{};
    std::array<float,3> margins{};float leading=0;
};
struct MenuSolidBatch {
    HudGeometryBatch geometry;std::array<float,4> rgba{1,1,1,1};
    // Insert after this actual bitmap role to retain source display-list order.
    std::string after_bitmap_role;
};
struct MenuArt { std::vector<HudGeometryBatch> batches;std::vector<MenuTextField> text_fields;std::vector<MenuSolidBatch> solids; };
struct MenuHitZone { Action action=Action::none;std::string path;std::vector<HudGeometryVertex> triangles; };
const MenuArt& original_menu_art(Tab,bool has_stat_points=true);
const std::vector<MenuHitZone>& original_menu_hit_zones();
// Original FlashCamera.Update -> SetViewport(driverW/H), SetBounds mode0:
// independently scaled axes, no fit-letterbox. `scale` is raster detail only.
struct MenuViewTransform { float scale=1,x=0,y=0,scale_x=1,scale_y=1; };
struct PresentedText { MenuTextField field;std::string value; };
struct Frame {
    MenuViewTransform transform;
    HudGeometry art;
    std::vector<PresentedText> text;
    std::vector<MenuSolidBatch> solids;
};
// Copies are per-frame projection only. Borrowed providers retain authoritative
// inventory/skills/character/profile ownership; no menu mutation of gameplay.
struct Bindings {
    const ActorState* actor=nullptr;
    const OriginalCombatProperties* properties=nullptr;
    const CharacterState* character=nullptr;
    // Actual localized class name (not numeric class ID rendered as a name).
    std::string class_label;
    std::function<bool(const std::string& source_field_path,std::string& value,std::string& error)> text;
    // Equipment/skill owners append their own original-art batches/source-field
    // projections in authored480x320 space. Presenter does not own their models.
    std::function<bool(Tab,Frame&,std::string&)> content;
};
class Presenter {
    bool open_=false;Tab tab_=Tab::stats;
public:
    void open() noexcept {open_=true;tab_=Tab::stats;}
    // Optional close policy (Preview 15 Stats confirmation): consulted by every close
    // path (Back, Escape, profile key). Returning false keeps the menu open.
    std::function<bool()> close_guard;
    void close() {if(close_guard&&!close_guard())return;open_=false;}
    bool is_open() const noexcept{return open_;}
    Tab tab() const noexcept{return tab_;}
    bool select(Tab,std::string& error);
    // Shared PC/touch release transport. Same transform as draw. Empty/outside
    // clicks while open are consumed by root modal routing; actionnone mutates nothing.
    Action hit_test(float screen_x,float screen_y,int width,int height)const noexcept;
    Action release(float screen_x,float screen_y,int width,int height) noexcept;
    bool frame(const Bindings&,int width,int height,Frame&,std::string& error)const;
    static bool viewport(int width,int height,MenuViewTransform&,std::string& error);
};
// Exact NativeGetStats fixed-point integer projection (ARM ASR8, floor for
// negative sentinel values); this differs from a floating UI vitals gauge.
std::int32_t source_stat_integer(std::int32_t raw) noexcept;
}
