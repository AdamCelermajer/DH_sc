#pragma once
#include "combat_text.hpp"
#include "combat_text_design.hpp"
#include "../../combat_session.hpp"
#include "../../../engine-ui/combat_flash_inputs_v1.hpp"
namespace dh::foundation {
enum class CombatTextDeliveryMode { after_update, synchronous_snapshot };
struct CombatTextLiveServices {
    CombatTextDesignQueries design;
    // SAME source GetTargetPosition: node180 && enabled80 ->184, otherwise160.
    std::function<bool(ActorId,std::array<float,3>&,std::string&)> target_position;
    // SAME source relative AABB maxZ158 - minZ14c; no mesh-height invention.
    std::function<bool(ActorId,float&,std::string&)> bounds_height;
    CombatTextPresenter::Project project;
    std::function<bool(const std::vector<CombatTextGlyph>&,std::string&)> glyph_draw;
    bool synchronous_resolution_capture=false;
    CombatTextDeliveryMode delivery=CombatTextDeliveryMode::after_update;
    // Optional read-only receipt observer; never a second damage producer.
    std::function<bool(const CombatTextDisplayEvent&,std::string&)> display_event;
};
const char* original_combat_text_font_resource() noexcept;
float original_combat_text_frame_rate() noexcept;
// Source Update: gameplay phases2..26 ->33ms; other phases use movie fps.
bool original_combat_text_interval(std::int32_t load_phase,std::int32_t&,std::string& error);

// All actor/result queries reacquire the SAME session/world on every call.
// Owns only design/font/presentation events, never actor HP, RNG or damage.
class CombatTextLiveAdapter {
public:
    bool load(const AssetCatalog&,CombatSession&,CombatTextLiveServices,std::string& error);
    // Invoke once AFTER each real CombatSession.update. Ticket is the caller's
    // existing frame/update serial; repeating it performs no duplicate work.
    // Host path uses actual host milliseconds and original SWF frame rate. It
    // makes no claim to own original Application/Level source clock fields.
    bool after_host_update(std::uint64_t ticket,std::uint32_t actual_dt_ms,
                           float viewport_scale_x,float viewport_scale_y,std::string& error);
    bool after_source_update(std::uint64_t ticket,const dh2::ui::CombatFlashTickBorrowV1&,
                             float viewport_scale_x,float viewport_scale_y,std::string& error);
    // Optional real result observer. With synchronous_snapshot mode invoke for
    // EVERY actual result at its application callback, before later motion.
    // Freezes source position/bounds/color/text; current-camera projection still
    // occurs when after_*_update enqueues it. No exact Flash-call timing claim.
    bool capture_resolution(const PlayableCombatResolution&,std::string& error);
    bool capture_synchronous_result(const PlayableCombatResolution&r,std::string&e){return capture_resolution(r,e);}
    const std::vector<CombatTextDisplayEvent>& captured_events()const noexcept{return captured_;}
    bool draw(float viewport_scale_x,float viewport_scale_y,std::string& error);
    void clear_for_reload() noexcept;
    std::size_t active_count() const noexcept {return presenter_.active_count();}
private:
    bool update(std::uint64_t,std::uint32_t,std::int32_t,float,float,std::string&);
    dh2::character::skills::CombatTextServicesV1 queries();
    CombatSession* session_=nullptr;
    CombatTextLiveServices services_;
    CombatTextDesign design_;
    HudGlyphFont font_;
    CombatTextPresenter presenter_;
    std::vector<CombatTextDisplayEvent> pending_;
    std::vector<CombatTextDisplayEvent> captured_;
    std::optional<std::uint64_t> last_ticket_;
};
} // namespace dh::foundation
