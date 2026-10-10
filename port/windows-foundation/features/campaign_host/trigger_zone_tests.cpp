// P16 HOST trigger-zone tests. Arguments:
//   1. campaign asset directory (original-campaign.xml of the Swamp package)
//   2. original-cache root with data/scene/001_swamp.mlx (Swamp declarations)
//   3. Android assets root with data/scene/003_darkwood.mlx (second level, not Swamp)
#include "trigger_zones.hpp"
#include <chrono>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <stdexcept>

using namespace dh::foundation;
using namespace dh::foundation::campaign_host;
namespace fs = std::filesystem;

namespace {

void check(bool value, const std::string& message) { if (!value) throw std::runtime_error(message); }

bool near(float a, float b, float tolerance = 0.01f) { return std::fabs(a - b) <= tolerance; }

// Counts script starts by name. Commands complete at once, so a running script ends on the next tick.
struct Counter {
    std::map<std::string, int> starts;
    std::map<std::string, int> move_outs;
    void bind(OriginalCampaignRuntime& runtime) {
        OriginalCampaignServices services;
        services.admit_start = [this, &runtime](int id, int, bool, bool& admitted, std::string&) {
            admitted = true;
            ++starts[runtime.scripts().at(static_cast<std::size_t>(id)).name];
            return true;
        };
        services.command = [](CampaignCommandPhase phase, const OriginalCampaignCommand&, int, bool& blocking, std::string&) {
            blocking = false;
            return phase != CampaignCommandPhase::update;
        };
        runtime.bind(std::move(services));
    }
};

// Drives the runtime until every script has finished (waits use a large clock step).
void settle(OriginalCampaignRuntime& runtime) {
    std::string error;
    for (int i = 0; i < 100; ++i) {
        check(runtime.tick(100000, error), "tick: " + error);
        bool any = false;
        for (std::size_t id = 0; id < runtime.scripts().size(); ++id) any = any || runtime.running(static_cast<int>(id));
        if (!any) return;
    }
    throw std::runtime_error("scripts did not settle");
}

std::array<float,3> centre(const ActorDefinition& d) { return {d.placement[12], d.placement[13], d.placement[14]}; }

const ActorDefinition* find_zone(const std::vector<ActorDefinition>& defs, const std::string& name) {
    for (const auto& d : defs) if (d.gametype == "TriggerZone" && d.name == name) return &d;
    return nullptr;
}

const TriggerZone* fed(const TriggerZoneSet& set, const std::string& name) {
    for (const auto& z : set.zones()) if (z.name == name) return &z;
    return nullptr;
}

void helper_geometry() {
    const auto box = zone_box({10, 20, 30}, {1, 5.55f, 1});
    check(near(box.min[0], -90) && near(box.max[0], 110), "x half extent is 100 * scale");
    check(near(box.min[1], 20 - 555) && near(box.max[1], 20 + 555), "y half extent follows the authored scale");
    check(point_inside(box, {10, 20, 30}) && point_inside(box, box.max) && !point_inside(box, {box.max[0] + 0.1f, 20, 30}), "inclusive box test");
    // P16 CINE2: tutorial_treasure (obj_4of4 _prim_TriggerZone): scale 12.2957,1,1, rotation 0,0,-90, at
    // (-2033.31,254.846,287.618). Placed columns for rotation -90 about Z: axis0 = 12.2957*(0,-1,0), axis1 = (1,0,0).
    const std::array<float,16> placed{0, -12.2957f, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, -2033.31f, 254.846f, 287.618f, 1};
    const auto rotated = oriented_zone_box(placed);
    check(near(rotated.half[0], 1229.57f, 0.1f) && near(rotated.half[1], 100) && near(rotated.half[2], 100), "rotated half extents follow the authored scale");
    check(point_inside(rotated, {-2033.31f, 254.846f + 1000, 287.618f}), "rotated zone: long axis runs along world Y");
    check(!point_inside(rotated, {-2033.31f, 254.846f + 1300, 287.618f}), "rotated zone ends at 100 * scale along Y");
    check(point_inside(rotated, {-2033.31f + 90, 254.846f, 287.618f}) && !point_inside(rotated, {-2033.31f + 150, 254.846f, 287.618f}),
          "rotated zone is narrow along world X");
    check(near(rotated.min[1], 254.846f - 1229.57f, 0.1f) && near(rotated.max[0], -2033.31f + 100), "axis-aligned bounds of the rotated box");
    std::array<float,3> v{};
    check(parse_vec3("1.5,-2,0", v) && near(v[1], -2), "vector parse");
    check(!parse_vec3("1,2", v) && !parse_vec3("1,2,3,4", v) && !parse_vec3("1,x,3", v), "malformed vector rejected");
}

// Swamp declarations through the real campaign bank: edge, count and re-entry behaviour.
void swamp_edges(const std::string& campaign, const std::string& cache) {
    AssetCatalog catalogue(cache);
    std::vector<ActorDefinition> defs;
    std::string error;
    check(load_actor_definitions(catalogue, "data/scene/001_swamp.mlx", defs, error), "Swamp declarations: " + error);

    OriginalCampaignRuntime runtime;
    check(runtime.load(AssetCatalog(campaign), "original-campaign.xml", error), "campaign: " + error);
    Counter counter;
    counter.bind(runtime);
    TriggerZoneSet zones;
    check(zones.build(defs, runtime, error), "build: " + error);

    const auto* lizard = fed(zones, "_prim_TriggerZone_LizManIntro");
    check(lizard != nullptr, "LizardMan_Intro zone is fed from the Swamp declarations");
    check(lizard->script == "LizardMan_Intro" && lizard->key.find("obj_3of4_brdwalk_sw_00.mgp::_prim_TriggerZone_LizManIntro") != std::string::npos, "zone keeps its authored script and source key");
    const auto* lizard_def = find_zone(defs, "_prim_TriggerZone_LizManIntro");
    check(lizard_def && near(lizard->box.min[1], centre(*lizard_def)[1] - 555.f, 0.5f), "zone box is centred on the declared position");
    const auto c = centre(*lizard_def);
    const std::array<float,3> outside{c[0] + 5000, c[1], c[2]};

    // Feed contacts through the zone set exactly as the host frame does.
    zones.update(runtime, outside, true, -1);
    check(counter.starts["LizardMan_Intro"] == 0, "outside the zone nothing starts");
    zones.update(runtime, c, true, -1);
    check(counter.starts["LizardMan_Intro"] == 1, "entering starts the script once");
    zones.update(runtime, c, true, -1);
    check(counter.starts["LizardMan_Intro"] == 1, "staying inside is edge-triggered");
    settle(runtime);
    zones.update(runtime, outside, true, -1);   // leave
    zones.update(runtime, c, true, -1);         // re-enter
    check(counter.starts["LizardMan_Intro"] == 1, "triggercount 1 blocks re-entry after activation");

    // triggercount -1 (unlimited): every entry starts the script again once the previous run has finished.
    const auto* camp = find_zone(defs, "_prim_Zone_EnterLoc_MerchantCamp");
    check(camp != nullptr && fed(zones, "_prim_Zone_EnterLoc_MerchantCamp") != nullptr, "MerchantCamp location zone is declared and fed");
    const auto camp_centre = centre(*camp);
    const std::array<float,3> camp_outside{camp_centre[0] + 1e6f, camp_centre[1], camp_centre[2]};
    settle(runtime);
    zones.update(runtime, camp_centre, true, -1);
    settle(runtime);
    zones.update(runtime, camp_outside, true, -1);
    settle(runtime);
    zones.update(runtime, camp_centre, true, -1);
    check(counter.starts["enterLocation_MerchantCamp"] == 2, "unlimited zone starts on each entry");

// Activation-conditioned zones are not evaluated, so they are not fed.
    bool listed = false;
    for (const auto& line : zones.skipped()) listed = listed || line.find("activate_cond is not evaluated") != std::string::npos;
    check(listed, "zones with unevaluated activate_cond are reported, not fed");
    std::cout << "swamp: fed=" << zones.zones().size() << " not-fed=" << zones.skipped().size() << '\n';
}

// Second level: declarations only, scripts supplied by a fixture bank built from the level's own zone names.
// A failed cutscene (abandoned by the host) must not disable any zone: contacts keep working afterwards.
void failure_keeps_zones(const std::string& campaign, const std::string& cache) {
    AssetCatalog catalogue(cache);
    std::vector<ActorDefinition> defs;
    std::string error;
    check(load_actor_definitions(catalogue, "data/scene/001_swamp.mlx", defs, error), "declarations: " + error);
    OriginalCampaignRuntime runtime;
    check(runtime.load(AssetCatalog(campaign), "original-campaign.xml", error), "campaign: " + error);
    OriginalCampaignServices services;
    services.admit_start = [](int, int, bool, bool& admitted, std::string&) { admitted = true; return true; };
    services.command = [](CampaignCommandPhase, const OriginalCampaignCommand&, int, bool& blocking, std::string& e) {
        blocking = false;
        e = "injected command failure";
        return false;
    };
    runtime.bind(std::move(services));
    TriggerZoneSet zones;
    check(zones.build(defs, runtime, error), "build: " + error);
    const auto* lizard = fed(zones, "_prim_TriggerZone_LizManIntro");
    check(lizard != nullptr, "LizardMan zone fed");
    const auto c = lizard->box;
    const std::array<float,3> centre_point{(c.min[0] + c.max[0]) * 0.5f, (c.min[1] + c.max[1]) * 0.5f, (c.min[2] + c.max[2]) * 0.5f};
    zones.update(runtime, centre_point, true, -1);
    bool failed = false;
    for (int i = 0; i < 5 && !failed; ++i) failed = !runtime.tick(0, error);
    check(failed, "injected failure reaches the executor");
    check(runtime.abandon_running_scripts() >= 1, "failed script (and its child) is abandoned");
    for (const auto& zone : zones.zones()) check(!zone.disabled, "failure must not disable zone " + zone.name);
    const std::array<float,3> far_point{c.max[0] + 100000.f, centre_point[1], centre_point[2]};
    zones.update(runtime, far_point, true, -1);
    for (const auto& zone : zones.zones()) check(!zone.disabled, "zone stays enabled after a contact cycle: " + zone.name);
    check(runtime.trigger_contact(lizard->key, false, true, -1, error), "contact is accepted again after abandon");
}

void second_level_loader(const std::string& android) {
    AssetCatalog catalogue(android);
    std::vector<ActorDefinition> defs;
    std::string error;
    check(load_actor_definitions(catalogue, "data/scene/003_darkwood.mlx", defs, error), "darkwood declarations: " + error);

    std::set<std::string> names;
    for (const auto& d : defs) {
        if (d.gametype != "TriggerZone") continue;
        for (const char* key : {"script", "script_move_out"}) {
            const auto found = d.properties.find(key);
            if (found != d.properties.end() && !found->second.empty()) names.insert(found->second);
        }
    }
    check(!names.empty(), "darkwood declares scripted zones");

    const auto dir = fs::temp_directory_path() / ("dh-p16host-zones-" + std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));
    fs::create_directories(dir);
    struct Cleanup { fs::path p; ~Cleanup() { std::error_code ec; fs::remove_all(p, ec); } } cleanup{dir};
    {
        std::ofstream out(dir / "original-campaign.xml", std::ios::binary);
        out << "<originalCampaign version=\"1\">\n<scripts scope=\"level\">\n";
        int id = 0;
        for (const auto& name : names) {
            out << "<script id=\"" << id++ << "\" name=\"" << name << "\">\n"
                << "<command index=\"0\" kind=\"26\" className=\"Script_Wait\"><scalar offset=\"8\" bits=\"0\" /></command>\n"
                << "</script>\n";
        }
        out << "</scripts>\n</originalCampaign>\n";
        check(out.good(), "fixture write");
    }
    OriginalCampaignRuntime runtime;
    check(runtime.load(AssetCatalog(dir), "original-campaign.xml", error), "fixture campaign: " + error);
    Counter counter;
    counter.bind(runtime);
    TriggerZoneSet zones;
    check(zones.build(defs, runtime, error), "darkwood build: " + error);
    check(!zones.zones().empty(), "darkwood zones are fed through registered declarations");
    check(runtime.triggers().size() == zones.zones().size(), "declarations register exactly the fed occurrences");

    // Edge and count for each fed zone whose script no other zone shares, from a settled runtime.
    std::map<std::string,int> uses;
    for (const auto& zone : zones.zones()) ++uses[zone.script];
    int tested = 0;
    for (const auto& zone : zones.zones()) {
        if (uses[zone.script] != 1) continue;
        settle(runtime);
        const std::array<float,3> centre_point{(zone.box.min[0] + zone.box.max[0]) * 0.5f, (zone.box.min[1] + zone.box.max[1]) * 0.5f, (zone.box.min[2] + zone.box.max[2]) * 0.5f};
        const std::array<float,3> far_point{zone.box.max[0] + 100000.f, centre_point[1], centre_point[2]};
        zones.update(runtime, far_point, true, -1);
                const int before = counter.starts[zone.script];
        zones.update(runtime, centre_point, true, -1);
        check(counter.starts[zone.script] == before + 1, zone.name + ": entry starts its script once");
        zones.update(runtime, centre_point, true, -1);
        check(counter.starts[zone.script] == before + 1, zone.name + ": staying inside is edge-triggered");
        ++tested;
    }
    check(tested > 0, "darkwood has zones with unique scripts to test");
    std::cout << "second level (darkwood): fed=" << zones.zones().size() << " not-fed=" << zones.skipped().size() << '\n';
    for (const auto& line : zones.skipped()) std::cout << "  not fed: " << line << '\n';
}

} // namespace

int main(int argc, char** argv) {
    try {
        check(argc == 4, "Supply campaign assets, original-cache root and Android assets root");
        helper_geometry();
        swamp_edges(argv[1], argv[2]);
        failure_keeps_zones(argv[1], argv[2]);
        second_level_loader(argv[3]);
        std::cout << "campaign_trigger_zone tests passed\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "campaign_trigger_zone test failed: " << e.what() << '\n';
        return 1;
    }
}
