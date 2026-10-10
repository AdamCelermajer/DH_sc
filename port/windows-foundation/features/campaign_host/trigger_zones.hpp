#pragma once
// P16 HOST: generic TriggerZone builder. Zones come from the declarations of the LOADED level
// (load_actor_definitions output: GameObjects and placed Module MGP children) for any map.
// Nothing here names a level, script or position. Contacts are fed to the existing
// OriginalCampaignRuntime::trigger_contact, which owns edge state, counts and delays.
#include "../../actor_definitions.hpp"
#include "../../original_campaign_runtime.hpp"
#include <array>
#include <string>
#include <vector>

namespace dh::foundation::campaign_host {

struct TriggerZoneBox {
    std::array<float,3> min{}, max{};
};

// Authored Block size: the source default is 200 per axis, multiplied by the authored scale
// (canonical_trigger_zone_v22 InitPost), so the half extent is 100 * scale, centred on position.
TriggerZoneBox zone_box(const std::array<float,3>& position,const std::array<float,3>& scale) noexcept;
bool point_inside(const TriggerZoneBox& box,const std::array<float,3>& point) noexcept;
bool parse_vec3(const std::string& text,std::array<float,3>& out) noexcept;

struct TriggerZone {
    std::string key;            // OriginalCampaignRuntime trigger key (source file::name)
    std::string name;           // authored prim name
    std::string script;         // authored level script name
    int script_id=-1;           // resolved level script id in the loaded campaign bank
    TriggerZoneBox box;
    bool inside=false;          // last fed physical fact
    bool was_running=false;     // for start/finish logging only
    bool disabled=false;        // set after a contact error; reported once, the other zones keep working
};

class TriggerZoneSet {
public:
    // Builds zones from declarations. Anything not fed is recorded in skipped() with a reason.
    bool build(const std::vector<ActorDefinition>& declarations,OriginalCampaignRuntime& runtime,std::string& error);
    // One frame of player contact facts (qualified = player alive). Edge/count handled by the runtime.
    // A zone whose contact fails is disabled and reported; other zones are still fed.
    void update(OriginalCampaignRuntime& runtime,const std::array<float,3>& player,bool qualified,int module);
    const std::vector<TriggerZone>& zones() const noexcept { return zones_; }
    const std::vector<std::string>& skipped() const noexcept { return skipped_; }
private:
    std::vector<TriggerZone> zones_;
    std::vector<std::string> skipped_;
};

} // namespace dh::foundation::campaign_host
