#include "runtime_effects_factory_v1.hpp"
#include "runtime_swing_fx_observer_v1.hpp"
#include "../skills_animation/skill_animation_program.hpp"
#include "../../../game-data/data.hpp"
#include "../../original_actor_properties.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <memory>
#include <set>
#include <stdexcept>

using namespace dh::foundation;
using namespace dh::foundation::effects;

namespace {
unsigned checks{};
void check(bool value, const std::string& message) {
    ++checks;
    if (!value) throw std::runtime_error("check " + std::to_string(checks) + ": " +
                                         (message.empty() ? "condition failed" : message));
}
std::vector<std::uint8_t> read(const std::filesystem::path& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) throw std::runtime_error("missing source input: " + path.string());
    return {std::istreambuf_iterator<char>(in), {}};
}
dh2::data::Bytes bytes(const std::vector<std::uint8_t>& value) {
    return {value.data(), value.size()};
}
struct StarterWeaponFxAudit {
    std::string profile, main_item, off_item;
    std::int32_t main_id{-1}, off_id{-1};
    std::array<std::int32_t, 2> main_words{{-1, -1}}, off_words{{-1, -1}};
    std::array<std::int32_t, 2> traits_main_words{{-1, -1}};
    std::vector<std::string> session_equipment;
    bool same_session_main_bound{}, same_session_off_slot{}, same_session_off_config_matches{},
         session_dual_wield{};
    std::int32_t session_off_damage_class{-1};
};
StarterWeaponFxAudit audit_starter_weapons(const AssetCatalog& assets,
    const OriginalPropertyDatabase& database, const OriginalMeleeBindings& melee,
    const dh2::data::ItemTable& items, const std::string& profile_id,
    const std::string& main_id, const std::string& off_id, std::uint32_t session_id) {
    StarterWeaponFxAudit result;
    result.profile=profile_id; result.main_item=main_id; result.off_item=off_id;
    result.main_id=dh2::data::item_id(items,main_id);
    result.off_id=off_id.empty()?-1:dh2::data::item_id(items,off_id);
    check(result.main_id>=0 && (off_id.empty()||result.off_id>=0),
          "source starter weapon row is absent from the actual ItemTable");
    const auto* main=dh2::data::item(items,result.main_id);
    const auto* off=result.off_id<0?nullptr:dh2::data::item(items,result.off_id);
    check(main && (result.off_id<0||off),"source starter ItemRecord lookup failed");
    result.main_words={{main->record.words[5],main->record.words[6]}};
    if(off) result.off_words={{off->record.words[5],off->record.words[6]}};

    ActorCustomization customization;
    customization.skin_id_contains=profile_id=="RoguePlayerBase"?
        "_default_rogue-mesh-skin":"_default_mage-mesh-skin";
    customization.expected_controller_count=4;
    customization.allow_missing_animation_targets=true;
    OriginalCombatVisualPlan plan;
    std::string error;
    check(build_original_combat_visual_plan(assets,melee,profile_id,customization,
          "effects-starter-audit-"+profile_id,plan,error),error);
    const auto* idle=plan.phase("Idle",0,{0});
    check(idle && !idle->clipName.empty(),"source class idle phase required for same-session starter audit");

    CombatSessionConfig config;
    config.diagnosticRngSeed=20261009u+session_id;
    config.playerId=session_id;
    config.playerProfileId=profile_id;
    config.tableRoot="original-cache/data/pydata";
    config.mainItemId=main_id;
    config.offItemId=off_id;
    config.equippedItemIds={main_id};
    if(!off_id.empty()) config.equippedItemIds.push_back(off_id);
    config.playerVisualConfig=plan.config;
    config.playerVisualConfig.motion_node_id="auto";
    config.playerVisualConfig.consume_root_motion=true;
    CombatSessionProfile profile;
    profile.initialIdle={"Idle",0,{0}};
    profile.animationOnly=true;
    profile.customization=customization;
    profile.motionRoot="auto";
    config.profiles.emplace(profile_id,std::move(profile));
    ActorPopulation population;
    CharacterVisual visual;
    CombatSession session;
    check(session.initialize(assets,database,melee,config,visual,population,
          {0,0,0},customization,error),"same-session source starter binding: "+error);
    const auto* state=session.actor(session.player_id());
    const auto* traits=session.world()->traits(session.player_id());
    check(state&&traits&&traits->main_item,"source starter Session traits are absent");
    result.traits_main_words={{traits->main_item->words[5],traits->main_item->words[6]}};
    result.same_session_main_bound=std::any_of(state->equipment.begin(),state->equipment.end(),
        [&](const ActorEquipmentReference& item){return item.slot=="main_hand"&&item.definition_id==main_id;});
    result.same_session_off_slot=off_id.empty()?std::none_of(state->equipment.begin(),state->equipment.end(),
        [](const ActorEquipmentReference& item){return item.slot=="off_hand";}):
        std::any_of(state->equipment.begin(),state->equipment.end(),
        [&](const ActorEquipmentReference& item){return item.slot=="off_hand"&&item.definition_id==off_id;});
    for(const auto& item:state->equipment)
        result.session_equipment.push_back(item.slot+":"+item.definition_id);
    const auto* combat=session.world()->combat_properties(session.player_id());
    check(combat,"source starter Session combat properties are absent");
    result.session_dual_wield=combat->facts.dual_wield;
    result.session_off_damage_class=combat->facts.off_damage_class;
    result.same_session_off_config_matches=off_id.empty()?!combat->facts.dual_wield:combat->facts.dual_wield;
    result.same_session_off_config_matches=result.same_session_off_config_matches&&
        combat->facts.off_damage_class==(off?off->record.words[37]:-1);
    check(result.same_session_main_bound&&result.same_session_off_config_matches,
          "source starter Session main/off-hand combat facts do not match the selected ItemTable rows");
    check(result.traits_main_words==result.main_words,
          "source starter Session main-item traits differ from the actual main ItemRecord");
    if(off) {
        const auto found=std::find_if(state->equipment.begin(),state->equipment.end(),
            [](const ActorEquipmentReference& item){return item.slot=="off_hand";});
        if(off_id==main_id) {
            const auto matching=std::count_if(state->equipment.begin(),state->equipment.end(),
                [&](const ActorEquipmentReference& item){return item.definition_id==off_id;});
            const auto main_hand=std::find_if(state->equipment.begin(),state->equipment.end(),
                [](const ActorEquipmentReference& item){return item.slot=="main_hand";});
            const auto resolve_exact_item=[&](const ActorEquipmentReference& item) {
                const auto id=dh2::data::item_id(items,item.definition_id);
                return id==result.main_id && dh2::data::item(items,id)==main;
            };
            check(matching==2 && main_hand!=state->equipment.end() &&
                  found!=state->equipment.end() && main_hand!=found &&
                  main_hand->definition_id==main_id && found->definition_id==off_id &&
                  resolve_exact_item(*main_hand) && resolve_exact_item(*found),
                  "identical source daggers must remain distinct main/off-hand entries resolving the same exact ItemTable row");
        } else {
            check(found!=state->equipment.end()&&dh2::data::item(items,dh2::data::item_id(items,found->definition_id))==off,
                  "Session off-hand did not resolve to its exact original ItemTable row");
        }
        result.off_words={{off->record.words[5],off->record.words[6]}};
    }
    return result;
}
struct RenderCpuFixture {
    std::uint32_t next_texture{100};
    unsigned uploads{}, releases{}, camera_calls{}, driver_calls{}, submissions{};
    unsigned texture_alpha_min{255}, texture_alpha_max{};
    TextureImage decoded_texture;
    std::shared_ptr<const EffectRenderFrame> frame;
    bool upload(const TextureImage& image, std::uint32_t& id, std::string& error) {
        if (!image.width || !image.height ||
            image.rgba.size() != std::size_t(image.width) * image.height * 4) {
            error = "original decoded texture dimensions/data differ";
            return false;
        }
        id = ++next_texture;
        ++uploads;
        decoded_texture = image;
        for (std::size_t i = 3; i < image.rgba.size(); i += 4) {
            texture_alpha_min = std::min(texture_alpha_min,
                                         static_cast<unsigned>(image.rgba[i]));
            texture_alpha_max = std::max(texture_alpha_max,
                                         static_cast<unsigned>(image.rgba[i]));
        }
        error.clear();
        return true;
    }
    void release(std::uint32_t) {
        ++releases;
    }
    static bool camera(void* raw, const dh2::scene::Scene&, float[16], float[3],
                       std::string& error) {
        ++static_cast<RenderCpuFixture*>(raw)->camera_calls;
        error = "Swoosh mesh fixture unexpectedly requested a particle camera";
        return false;
    }
    static bool bashdown_test_camera(void* raw, const dh2::scene::Scene&,
                       float view[16], float position[3], std::string& error) {
        ++static_cast<RenderCpuFixture*>(raw)->camera_calls;
        std::fill(view, view + 16, 0.0f);
        view[0] = view[5] = view[10] = view[15] = 1.0f;
        position[0] = position[1] = 0.0f;
        position[2] = 10.0f;
        error.clear();
        return true;
    }
    static bool driver(void* raw, std::uint32_t&, std::string& error) {
        ++static_cast<RenderCpuFixture*>(raw)->driver_calls;
        error = "Swoosh mesh fixture unexpectedly requested particle driver policy";
        return false;
    }
    static bool submit(void* raw, std::shared_ptr<const EffectRenderFrame> frame,
                       std::string& error) {
        auto& self = *static_cast<RenderCpuFixture*>(raw);
        self.frame = std::move(frame);
        ++self.submissions;
        error.clear();
        return true;
    }
};
struct TimelineUvSampleV1 {
    std::size_t samples{}, nonblack_samples{};
    double max_texture_channel{}, max_modulated_channel{};
    std::array<float,4> material_color{};
    std::array<float,2> uv_min{{std::numeric_limits<float>::max(),
                                std::numeric_limits<float>::max()}};
    std::array<float,2> uv_max{{-std::numeric_limits<float>::max(),
                                -std::numeric_limits<float>::max()}};
};
struct TimelinePhaseV1 {
    std::int32_t absolute_ms{};
    std::size_t packet_count{};
    TimelineUvSampleV1 uv;
    std::array<double,3> world_min{{std::numeric_limits<double>::max(),
        std::numeric_limits<double>::max(),std::numeric_limits<double>::max()}};
    std::array<double,3> world_max{{-std::numeric_limits<double>::max(),
        -std::numeric_limits<double>::max(),-std::numeric_limits<double>::max()}};
    double projected_area_pixels{};
    std::array<float,6> source_texture_matrix_2d{};
};
std::array<float,6> texture_matrix_2d(const dh2::math::Matrix4f& matrix) {
    return {{matrix.m[0],matrix.m[1],matrix.m[4],matrix.m[5],matrix.m[8],matrix.m[9]}};
}
std::array<float,6> texture_matrix_2d(const float* matrix) {
    return {{matrix[0],matrix[1],matrix[4],matrix[5],matrix[8],matrix[9]}};
}
TimelineUvSampleV1 sample_baked_uvs(const EffectRenderFrame& frame,
                                    const TextureImage& image) {
    TimelineUvSampleV1 result;
    const auto wrap=[](long long i,std::uint32_t size) {
        const auto n=static_cast<long long>(size);const auto r=i%n;
        return static_cast<std::uint32_t>(r<0?r+n:r);
    };
    for(const auto& packet:frame.packets) {
        if(packet.mesh.ranges.empty()) continue;
        const auto& material=packet.mesh.ranges.front().material;
        result.material_color=material.color;
        std::vector<std::array<double,2>> points;
        points.reserve(packet.mesh.vertices.size()+packet.mesh.indices.size()/3);
        for(const auto& vertex:packet.mesh.vertices) {
            points.push_back({vertex.u,vertex.v});
            result.uv_min[0]=std::min(result.uv_min[0],vertex.u);
            result.uv_min[1]=std::min(result.uv_min[1],vertex.v);
            result.uv_max[0]=std::max(result.uv_max[0],vertex.u);
            result.uv_max[1]=std::max(result.uv_max[1],vertex.v);
        }
        for(std::size_t i=0;i+2<packet.mesh.indices.size();i+=3) {
            const auto& a=packet.mesh.vertices[packet.mesh.indices[i]];
            const auto& b=packet.mesh.vertices[packet.mesh.indices[i+1]];
            const auto& c=packet.mesh.vertices[packet.mesh.indices[i+2]];
            points.push_back({(double(a.u)+b.u+c.u)/3.0,(double(a.v)+b.v+c.v)/3.0});
        }
        for(const auto& point:points) {
            const double x=point[0]*image.width-0.5,y=point[1]*image.height-0.5;
            const auto x0=static_cast<long long>(std::floor(x));
            const auto y0=static_cast<long long>(std::floor(y));
            const double fx=x-std::floor(x),fy=y-std::floor(y);
            double rgb[3]{};
            for(unsigned oy=0;oy<2;++oy)for(unsigned ox=0;ox<2;++ox) {
                const auto xi=wrap(x0+ox,image.width),yi=wrap(y0+oy,image.height);
                const auto pixel=(std::size_t(yi)*image.width+xi)*4;
                const double weight=(ox?fx:1-fx)*(oy?fy:1-fy);
                for(unsigned channel=0;channel<3;++channel)
                    rgb[channel]+=image.rgba[pixel+channel]*weight;
            }
            ++result.samples;
            const double source_channel=std::max({rgb[0],rgb[1],rgb[2]});
            if(source_channel>0.5) ++result.nonblack_samples;
            result.max_texture_channel=std::max(result.max_texture_channel,source_channel);
            for(unsigned channel=0;channel<3;++channel)
                result.max_modulated_channel=std::max(result.max_modulated_channel,
                    rgb[channel]*material.color[channel]);
        }
    }
    return result;
}
void measure_phase_geometry(const EffectRenderFrame& frame, TimelinePhaseV1& phase,
    double target_x,double target_y,double eye_z,double tangent,
    double width,double height) {
    for(const auto& packet:frame.packets) {
        struct P { double x{},y{},depth{}; };
        std::vector<P> projected(packet.mesh.vertices.size());
        for(std::size_t i=0;i<packet.mesh.vertices.size();++i) {
            const auto& v=packet.mesh.vertices[i];const auto& m=packet.world;
            const double x=m[0]*v.position.x+m[4]*v.position.y+m[8]*v.position.z+m[12];
            const double y=m[1]*v.position.x+m[5]*v.position.y+m[9]*v.position.z+m[13];
            const double z=m[2]*v.position.x+m[6]*v.position.y+m[10]*v.position.z+m[14];
            for(unsigned axis=0;axis<3;++axis) {
                const double value=axis==0?x:axis==1?y:z;
                phase.world_min[axis]=std::min(phase.world_min[axis],value);
                phase.world_max[axis]=std::max(phase.world_max[axis],value);
            }
            const double depth=eye_z-z;
            projected[i]={(x-target_x)/(depth*tangent*(width/height))*0.5*width+0.5*width,
                          (y-target_y)/(depth*tangent)*0.5*height+0.5*height,depth};
        }
        for(std::size_t i=0;i+2<packet.mesh.indices.size();i+=3) {
            const auto& a=projected[packet.mesh.indices[i]];
            const auto& b=projected[packet.mesh.indices[i+1]];
            const auto& c=projected[packet.mesh.indices[i+2]];
            phase.projected_area_pixels+=std::abs((b.x-a.x)*(c.y-a.y)-
                                                   (b.y-a.y)*(c.x-a.x))*0.5;
        }
    }
}
}

int main(int argc, char** argv) {
    try {
        check(argc == 4, "usage: runtime-effects-factory-native <session-assets-root> <effects-table-dir> <source-effect-uri-root>");
        AssetCatalog assets(argv[1]);
        // The source extraction is content-addressed separately from the shared
        // game-data tree. Its test overlay maps the exact original bytes to the
        // virtual URI consumed by the owner; no resource is regenerated.
        AssetCatalog source_effect_assets(argv[3]);
        std::string error;

        OriginalPropertyDatabase database;
        OriginalMeleeBindings melee;
        check(load_original_property_tables(assets, "original-cache/data/pydata", database, error), error);
        check(melee.load(assets, "original-melee-bindings.xml", error), error);
        ActorCustomization customization;
        customization.skin_id_contains = "_default_warrior-mesh-skin";
        customization.expected_controller_count = 4;
        customization.allow_missing_animation_targets = true;
        OriginalCombatVisualPlan visual_plan;
        check(build_original_combat_visual_plan(assets, melee, "KnightPlayerBase",
              customization, "effects-factory-native", visual_plan, error), error);

        // Compile the actual base-Knight first active skill root into this
        // same visual before Session construction. This test selects the
        // retained animation only; it does not synthesize CharAI admission.
        const auto clip_names = read(std::filesystem::path(argv[1]) /
                                     "data/animations_dictionary_pyarraynames.bin");
        const auto clip_values = read(std::filesystem::path(argv[1]) /
                                      "data/animations_dictionary_pyarray.bin");
        const auto animation_records = read(std::filesystem::path(argv[1]) /
            "original-cache/data/pydata/animations_pyarray.bin");
        const auto animation_names = read(std::filesystem::path(argv[1]) /
            "original-cache/data/pydata/animations_pyarraynames.bin");
        const auto animation_fields = read(std::filesystem::path(argv[1]) /
            "original-cache/data/pydata/animations_pystructnames.bin");
        dh2::data::Dictionary clips;
        dh2::data::AnimationTables animations;
        check(dh2::data::load_dictionary(bytes(clip_names), bytes(clip_values), clips, error),
              "load actual AnimDict for Knight BashDown: " + error);
        check(dh2::data::load_animation_tables(bytes(animation_records), bytes(animation_names),
              bytes(animation_fields), clips, animations, error),
              "load actual AnimTable for Knight BashDown: " + error);
        check(animations.sequences.size() > 347 &&
              animations.sequence_names[347] == "Knight_BashDown" &&
              animations.sequences[347].type == 0 && animations.sequences[347].loop == 0 &&
              animations.sequences[347].steps.size() == 1 &&
              animations.sequences[347].steps[0].anim == 1234 &&
              animations.sequences[347].steps[0].fx == 164 &&
              animations.sequences[347].steps[0].anchor_fx &&
              animations.sequences[347].steps[0].move_go,
              "base Knight first-skill source root/FX row differs from authored AnimTable");
        dh::foundation::skills_animation::SkillAnimationPrograms bashdown_program;
        check(dh::foundation::skills_animation::build_skill_animation_programs(
              assets, animations, clips, visual_plan.config, {347}, "effects-bashdown",
              bashdown_program, error), "compile actual Knight BashDown clip: " + error);

        CombatSessionConfig config;
        config.diagnosticRngSeed = 20261009;
        config.playerId = 0x10001;
        config.playerProfileId = "KnightPlayerBase";
        config.tableRoot = "original-cache/data/pydata";
        const auto item_data = assets.read(config.tableRoot + "/loot_table_pyarray.bin");
        const auto item_names = assets.read(config.tableRoot + "/loot_table_pyarraynames.bin");
        const auto item_fields = assets.read(config.tableRoot + "/loot_table_pystructnames.bin");
        dh2::data::ItemTable item_table;
        check(dh2::data::load_items(bytes(item_data), bytes(item_names), bytes(item_fields),
                                    item_table, error), error);
        const auto rogue_dagger_audit=audit_starter_weapons(assets,database,melee,item_table,
            "RoguePlayerBase","Dagger01","Dagger01",0x10002);
        const auto mage_staff_audit=audit_starter_weapons(assets,database,melee,item_table,
            "MagePlayerBase","Staff01","",0x10003);
        check(item_table.identifiers.size() > 664, "original player weapon row missing");
        config.mainItemId = item_table.identifiers[664];
        config.equippedItemIds = {config.mainItemId};
        config.playerVisualConfig = bashdown_program.plan.config;
        config.playerVisualConfig.motion_node_id = "auto";
        config.playerVisualConfig.consume_root_motion = true;
        CombatSessionProfile profile;
        profile.action = {"AttackStatic", 0, {0, 1}};
        profile.initialIdle = {"Idle", 0, {0}};
        profile.damageMarkerNames = {"attack_mainhand"};
        profile.propertyOptions = {std::nullopt, true};
        profile.customization = customization;
        profile.motionRoot = "auto";
        OriginalAttackSelection selected_attack;
        selected_attack.state = "AttackStatic";
        selected_attack.variant = 0;
        profile.sequenceAction = selected_attack;
        profile.retainedPhaseClock = true;
        config.profiles.emplace(config.playerProfileId, std::move(profile));
        ActorPopulation population;
        CombatSessionProfile enemy_profile;
        enemy_profile.action = {"Attack", 0, {0, 1}};
        enemy_profile.initialIdle = {"Idle", 0, {0}};
        enemy_profile.death = CombatSessionChoice{"Died", 0, {0}};
        enemy_profile.damageMarkerNames = {"attack_mainhand"};
        enemy_profile.propertyOptions = {std::nullopt, true};
        enemy_profile.customization.allow_missing_animation_targets = true;
        config.profiles.emplace("Swamp_LizadMan_Type1", std::move(enemy_profile));
        PopulationActor enemy;
        enemy.profileId = "Swamp_LizadMan_Type1";
        enemy.definition.stableId = 2;
        enemy.definition.sourceId = "actual-effects-factory-target";
        enemy.definition.placement = {1,0,0,0,0,1,0,0,0,0,1,0,0,-100,0,1};
        enemy.transform = {2,0,0,0,0,2,0,0,0,0,2,0,0,-100,0,1};
        population.actors().push_back(std::move(enemy));
        CharacterVisual player;
        auto session = std::make_unique<CombatSession>();
        check(session->initialize(assets, database, melee, config, player, population,
                                 {0, 0, 0}, customization, error), error);
        const ActorId actor = session->player_id();
        const auto longsword_id = dh2::data::item_id(item_table, "Longsword01");
        check(longsword_id == 664, "actual Longsword01 source row is not ItemTable index 664");
        const auto* longsword = dh2::data::item(item_table, longsword_id);
        const auto* player_state = session->actor(actor);
        const auto* player_traits = session->world()->traits(actor);
        check(longsword && player_state && player_traits && player_traits->main_item,
              "actual CombatSession Longsword01 row/traits are unavailable");
        const bool equipped_longsword_main = std::any_of(player_state->equipment.begin(),
            player_state->equipment.end(), [](const ActorEquipmentReference& reference) {
                return reference.slot == "main_hand" && reference.definition_id == "Longsword01";
            });
        check(equipped_longsword_main, "actual CombatSession did not equip Longsword01 in main hand");
        check(player_traits->main_item->words[5] == longsword->record.words[5] &&
              player_traits->main_item->words[6] == longsword->record.words[6],
              "same-session equipped traits differ from original Longsword01 ItemRecord words[5/6]");
        const std::array<std::int32_t, 2> longsword_swoosh{{longsword->record.words[5],
                                                            longsword->record.words[6]}};
        const std::array<std::int32_t, 2> session_swoosh{{player_traits->main_item->words[5],
                                                        player_traits->main_item->words[6]}};
        InputActions input;
        input.targetSelect = true;
        check(session->update(0, input, {0,0,0}, 0, error), "select actual session opponent: " + error);
        input.targetSelect = false;
        input.attack = true;
        check(session->update(0, input, {0,0,0}, 0, error), "start actual session attack pose: " + error);
        check(actor == config.playerId && session->retained_actor_pose(actor),
              "actual CombatSession retained actor pose missing");
        const auto* retained_visual = session->retained_actor_visual_borrow(actor);
        check(retained_visual && retained_visual == &player && retained_visual->retained_scene_borrow(),
              "factory must use this session's exact Entry.visual and Scene");

        const auto table_dir = std::filesystem::path(argv[2]);
        const auto table_records = read(table_dir / "effects_pyarray.bin");
        const auto table_names = read(table_dir / "effects_pyarraynames.bin");
        const auto table_schema = read(table_dir / "effects_pystructnames.bin");
        const auto dictionary_names = read(table_dir / "effects_dictionary_pyarraynames.bin");
        const auto dictionary_paths = read(table_dir / "effects_dictionary_pyarray.bin");
        dh2::data::EffectsTables tables;
        check(tables.load(bytes(table_records), bytes(table_names), bytes(table_schema),
                          bytes(dictionary_names), bytes(dictionary_paths), error), error);
        auto table = tables.borrow();
        const auto set = std::find(table.set_names().begin(), table.set_names().end(),
                                   "swoosh_prince_1hand_combo_01");
        check(set != table.set_names().end(), "original authored swoosh effect set absent");
        const auto set_id = static_cast<std::int32_t>(set - table.set_names().begin());
        check(set_id >= 0 && table.sets().at(static_cast<std::size_t>(set_id)).steps.size() > 0,
              "original swoosh set row has no authored steps");

        // Empty PFWorld is a real typed service object in this CPU-only owner
        // test. This mesh-only source set must not request camera/driver policy.
        dh2::navigation::CollisionWorld same_pf_world{};
        RenderCpuFixture cpu;
        RuntimeEffectsFactoryBindingsV1 factory_bindings;
        factory_bindings.assets = &source_effect_assets;
        factory_bindings.tables = table;
        factory_bindings.same_pf_world = &same_pf_world;
        factory_bindings.scene_view = {&cpu, RenderCpuFixture::camera, RenderCpuFixture::driver};
        factory_bindings.textures = {
            [&cpu](const TextureImage& image, std::uint32_t& id, std::string& e) {
                return cpu.upload(image, id, e);
            },
            [&cpu](std::uint32_t id) { cpu.release(id); }};
        factory_bindings.submit = [&cpu](std::shared_ptr<const EffectRenderFrame> f,
                                         std::string& e) {
            return RenderCpuFixture::submit(&cpu, std::move(f), e);
        };
        auto factory = RuntimeEffectsFactoryV1::create(*session, actor, std::move(factory_bindings), error);
        check(factory != nullptr, "actual same-session FX factory creation: " + error);
        check(factory->is_bound_to(*session),"FX factory rejected its live same-session owner");
        CombatSession foreignSession;
        check(!factory->is_bound_to(foreignSession),"FX factory accepted a foreign session");
        check(factory->manager().source_libraries_v63() == nullptr,
              "factory unexpectedly substituted a different App FX-library owner");

        // Load the original AnimTable/AnimDict data and observe the actual
        // CombatSession step cursor. The observer borrows the same manager,
        // ItemTable, EffectsTables and actor binding used by this packet test.
        std::vector<RuntimeSwingFxDiagnosticV1> swing_diagnostics;
        RuntimeSwingFxObserverV1 swing(*session, animations, item_table, table,
            factory->manager(), [&](const RuntimeSwingFxDiagnosticV1& d) {
                swing_diagnostics.push_back(d);
            });
        const auto swing_observer = swing.step_entry_observer();
        std::optional<CombatSessionStepEntry> actual_swoosh_entry;
        std::optional<CombatSessionStepEntry> actual_bashdown_entry;
        session->set_step_entry_observer([&](const CombatSessionStepEntry& entry) {
            if (!actual_swoosh_entry && entry.sequence_id >= 0 &&
                static_cast<std::size_t>(entry.sequence_id) < animations.sequences.size() &&
                entry.step < animations.sequences[static_cast<std::size_t>(entry.sequence_id)].steps.size() &&
                animations.sequences[static_cast<std::size_t>(entry.sequence_id)].steps[entry.step].swoosh)
                actual_swoosh_entry = entry;
            if (!actual_bashdown_entry && entry.sequence_id == 347 && entry.step == 0)
                actual_bashdown_entry = entry;
            swing_observer(entry);
        });

        const auto fresh_manager_creations = factory->manager().cold_creations();
        check(fresh_manager_creations == 0,
              "step-specific attribution requires a fresh source manager with no prior FX instance");

        InputActions presentation_input{};
        std::int32_t source_absolute_ms{};
        for (std::uint64_t frame = 1; frame != 103 && !actual_swoosh_entry &&
             std::none_of(swing_diagnostics.begin(), swing_diagnostics.end(),
                [](const auto& d) { return d.dispatched && !d.source_sets.empty(); }); ++frame) {
            source_absolute_ms=static_cast<std::int32_t>((frame-1)*64);
            check(session->update(0.064, presentation_input, {0,0,0}, 0, error),
                  "advance actual CombatSession AnimTable step: " + error);
            check(factory->runtime().update(frame, source_absolute_ms, 64, error),
                  "advance same-session FX manager after step entry: " + error);
        }
        session->clear_step_entry_observer();
        const auto emitted_it = std::find_if(swing_diagnostics.begin(), swing_diagnostics.end(),
            [](const auto& d) { return d.dispatched && !d.source_sets.empty(); });
        check(emitted_it != swing_diagnostics.end(),
              "actual AnimTable Swoosh did not dispatch its authored Item/step FX set");
        const auto emitted = *emitted_it;
        if (actual_swoosh_entry) {
            const auto& source_step = animations.sequences[static_cast<std::size_t>(actual_swoosh_entry->sequence_id)].steps[actual_swoosh_entry->step];
            check(source_step.swoosh && source_step.fx == emitted.source_sets.front(),
                  "step FX set differs from the actual original AnimTable row");
            const auto expected_set = longsword_swoosh[1] != -1 ? longsword_swoosh[1] : source_step.fx;
            check(emitted.source_sets.front() == expected_set,
                  "actual equipped-item precedence did not select Longsword01 FX or source step fallback");
        }
        check(actual_swoosh_entry.has_value(), "actual Session never entered an original AnimTable step with source FX");
        check(emitted.actor == actor && emitted.occurrence == actual_swoosh_entry->occurrence &&
              emitted.sequence_id == actual_swoosh_entry->sequence_id &&
              emitted.step == actual_swoosh_entry->step,
              "FX source set did not preserve exact same-session actor/step occurrence");
        check(factory->manager().cold_creations() > fresh_manager_creations,
              "fresh source manager did not create a set from this actual AnimTable step");
        const auto creations_after_step = factory->manager().cold_creations();
        swing_observer(*actual_swoosh_entry);
        check(!swing_diagnostics.empty() &&
              swing_diagnostics.back().detail == "Duplicate exact actor/step occurrence suppressed" &&
              factory->manager().cold_creations() == creations_after_step,
              "replayed actual step occurrence emitted a duplicate FX set");

        // Prepare the original source manager after the actual AnimTable step.
        // This borrows the same retained CharacterVisual Scene and keeps its
        // packets/materials alive through the explicit queue callback.
        std::shared_ptr<const EffectRenderFrame> frame;
        check(factory->runtime().prepare_render_frame(frame, error),
              "same-session FX packet preparation after actual step: " + error);
        check(frame && !frame->packets.empty(), "original source FX manager produced no packets");
        // This first occurrence is the generic live-session Swoosh probe; its
        // selected set may come from the equipped-item override. The dedicated
        // root347 case below owns the exact BashDown/set164 URI assertion.
        for (const auto& packet : frame->packets) {
            check(packet.source.kind == EffectDrawKind::authored_mesh,
                  "source mesh-only fixture unexpectedly emitted a particle packet");
            check(packet.source.scene == retained_visual->retained_scene_borrow() ||
                  packet.source.resource_bytes,
                  "packet lost original resource or retained source scene");
            check(!packet.mesh.vertices.empty() && !packet.mesh.indices.empty() &&
                  packet.source_retention, "packet missing actual source geometry/lifetime loan");
            check(packet.mesh.ranges.size() == 1 && packet.mesh.ranges[0].material.sourcePass,
                  "actual original material pass not decoded");
        }
        check(cpu.camera_calls == 0 && cpu.driver_calls == 0,
              "source mesh-only test crossed into an invented particle camera/driver branch");
        check(cpu.uploads > 0, "original material decoder did not request CPU fixture upload");
        check(cpu.submissions == 0, "prepare must leave queue submission to the root caller");
        const auto packet_count = frame->packets.size();
        std::array<double,3> bounds_min{{std::numeric_limits<double>::max(),
            std::numeric_limits<double>::max(), std::numeric_limits<double>::max()}};
        std::array<double,3> bounds_max{{-bounds_min[0],-bounds_min[1],-bounds_min[2]}};
        std::array<double,3> normal_sum{};
        std::array<float,4> vertex_min{{1,1,1,1}}, vertex_max{{0,0,0,0}};
        double normal_dot_min=1.0,normal_dot_max=0.0;
        std::size_t vertex_count{};
        for (const auto& packet : frame->packets) for (const auto& vertex : packet.mesh.vertices) {
            const auto& m = packet.world; const auto& p = vertex.position;
            const std::array<double,3> world{{
                m[0]*p.x+m[4]*p.y+m[8]*p.z+m[12],
                m[1]*p.x+m[5]*p.y+m[9]*p.z+m[13],
                m[2]*p.x+m[6]*p.y+m[10]*p.z+m[14]}};
            for (unsigned axis=0;axis!=3;++axis) {
                bounds_min[axis]=std::min(bounds_min[axis],world[axis]);
                bounds_max[axis]=std::max(bounds_max[axis],world[axis]);
            }
            std::array<double,3> n{{
                m[0]*vertex.normal.x+m[4]*vertex.normal.y+m[8]*vertex.normal.z,
                m[1]*vertex.normal.x+m[5]*vertex.normal.y+m[9]*vertex.normal.z,
                m[2]*vertex.normal.x+m[6]*vertex.normal.y+m[10]*vertex.normal.z}};
            const double length=std::sqrt(n[0]*n[0]+n[1]*n[1]+n[2]*n[2]);
            if(length>0){
                for(unsigned axis=0;axis!=3;++axis)normal_sum[axis]+=n[axis]/length;
                const double dot=std::abs(n[2]/length);
                normal_dot_min=std::min(normal_dot_min,dot);
                normal_dot_max=std::max(normal_dot_max,dot);
            }
            for(unsigned channel=0;channel!=4;++channel){
                vertex_min[channel]=std::min(vertex_min[channel],vertex.color[channel]);
                vertex_max[channel]=std::max(vertex_max[channel],vertex.color[channel]);
            }
            ++vertex_count;
        }
        // Diagnostics are printed after frame/session teardown. Snapshot values
        // rather than retaining references into the released packet.
        const auto first_material = frame->packets.front().mesh.ranges.front().material;
        const auto source_pass = *first_material.sourcePass;
        const auto& first_source = frame->packets.front().source;
        check(first_source.scene && first_source.material<first_source.scene->materials.size() &&
              first_source.source_texture_matrix,
              "source BRES/base and animated UV matrix metadata required");
        const auto source_base_uv_matrix=texture_matrix_2d(
            first_source.scene->materials[first_source.material].texture_matrix);
        const auto source_animated_uv_matrix=texture_matrix_2d(*first_source.source_texture_matrix);
        const std::array<double,3> extents{{bounds_max[0]-bounds_min[0],
            bounds_max[1]-bounds_min[1],bounds_max[2]-bounds_min[2]}};
        // Read-only projection diagnostic for the same source packet and the
        // WGL smoke's bounds-derived 60-degree, 640x480 camera. Mesh UVs here
        // are the already-baked snapshot from AuthoredFxMeshGraphV32; do not
        // multiply by source_texture_matrix again.
        constexpr double diagnostic_width=640.0, diagnostic_height=480.0;
        const double target_x=(bounds_min[0]+bounds_max[0])*0.5;
        const double target_y=(bounds_min[1]+bounds_max[1])*0.5;
        const double target_z=(bounds_min[2]+bounds_max[2])*0.5;
        const double extent=std::max({extents[0],extents[1],extents[2],1.0});
        const double eye_z=target_z+extent*2.8;
        const double tangent=std::tan(3.14159265358979323846/6.0);
        struct ProjectedVertex { double x{},y{},depth{}; float u{},v{}; };
        std::size_t projected_triangles{}, degenerate_world_triangles{},
                    degenerate_screen_triangles{}, covered_pixel_centers{},
                    all_black_filtered_samples{};
        double total_projected_triangle_area{}, sample_rgb_min=255.0, sample_rgb_max=0.0;
        std::set<std::size_t> sampled_texture_texels;
        const auto wrap_index=[](long long i,std::uint32_t size) {
            const long long n=static_cast<long long>(size);
            const long long wrapped=i%n;
            return static_cast<std::uint32_t>(wrapped<0?wrapped+n:wrapped);
        };
        const auto& decoded=cpu.decoded_texture;
        check(decoded.width>0&&decoded.height>0&&
              decoded.rgba.size()==std::size_t(decoded.width)*decoded.height*4,
              "projection diagnostic needs the exact decoded uploaded source texture");
        for(const auto& packet:frame->packets) {
            std::vector<ProjectedVertex> projected(packet.mesh.vertices.size());
            for(std::size_t vi=0;vi<packet.mesh.vertices.size();++vi) {
                const auto& v=packet.mesh.vertices[vi];const auto& m=packet.world;
                const double wx=m[0]*v.position.x+m[4]*v.position.y+m[8]*v.position.z+m[12];
                const double wy=m[1]*v.position.x+m[5]*v.position.y+m[9]*v.position.z+m[13];
                const double wz=m[2]*v.position.x+m[6]*v.position.y+m[10]*v.position.z+m[14];
                const double depth=eye_z-wz;
                const double ndc_x=(wx-target_x)/(depth*tangent*(diagnostic_width/diagnostic_height));
                const double ndc_y=(wy-target_y)/(depth*tangent);
                projected[vi]={(ndc_x*0.5+0.5)*diagnostic_width,
                               (ndc_y*0.5+0.5)*diagnostic_height,depth,v.u,v.v};
            }
            for(const auto& range:packet.mesh.ranges) {
                const auto end=std::min(range.firstIndex+range.indexCount,packet.mesh.indices.size());
                for(std::size_t ii=range.firstIndex;ii+2<end;ii+=3) {
                    ++projected_triangles;
                    const auto ia=packet.mesh.indices[ii],ib=packet.mesh.indices[ii+1],ic=packet.mesh.indices[ii+2];
                    const auto& a=projected[ia];const auto& b=projected[ib];const auto& c=projected[ic];
                    const auto& va=packet.mesh.vertices[ia].position;
                    const auto& vb=packet.mesh.vertices[ib].position;
                    const auto& vc=packet.mesh.vertices[ic].position;
                    const double ux=vb.x-va.x,uy=vb.y-va.y,uz=vb.z-va.z;
                    const double vx=vc.x-va.x,vy=vc.y-va.y,vz=vc.z-va.z;
                    const double cross_x=uy*vz-uz*vy,cross_y=uz*vx-ux*vz,cross_z=ux*vy-uy*vx;
                    if(cross_x*cross_x+cross_y*cross_y+cross_z*cross_z<1e-12)
                        ++degenerate_world_triangles;
                    const double signed_area=(b.x-a.x)*(c.y-a.y)-(b.y-a.y)*(c.x-a.x);
                    const double area=std::abs(signed_area)*0.5;
                    total_projected_triangle_area+=area;
                    if(area<1e-6) { ++degenerate_screen_triangles; continue; }
                    const int xmin=std::max(0,static_cast<int>(std::floor(std::min({a.x,b.x,c.x}))));
                    const int xmax=std::min(static_cast<int>(diagnostic_width)-1,
                        static_cast<int>(std::ceil(std::max({a.x,b.x,c.x}))));
                    const int ymin=std::max(0,static_cast<int>(std::floor(std::min({a.y,b.y,c.y}))));
                    const int ymax=std::min(static_cast<int>(diagnostic_height)-1,
                        static_cast<int>(std::ceil(std::max({a.y,b.y,c.y}))));
                    for(int py=ymin;py<=ymax;++py) for(int px=xmin;px<=xmax;++px) {
                        const double sx=px+0.5,sy=py+0.5;
                        const double wa=((b.x-sx)*(c.y-sy)-(b.y-sy)*(c.x-sx))/signed_area;
                        const double wb=((c.x-sx)*(a.y-sy)-(c.y-sy)*(a.x-sx))/signed_area;
                        const double wc=1.0-wa-wb;
                        if(wa<0.0||wb<0.0||wc<0.0) continue;
                        ++covered_pixel_centers;
                        const double reciprocal=wa/a.depth+wb/b.depth+wc/c.depth;
                        const double u=(wa*a.u/a.depth+wb*b.u/b.depth+wc*c.u/c.depth)/reciprocal;
                        const double v=(wa*a.v/a.depth+wb*b.v/b.depth+wc*c.v/c.depth)/reciprocal;
                        const double tx=u*decoded.width-0.5,ty=v*decoded.height-0.5;
                        const auto x0=static_cast<long long>(std::floor(tx));
                        const auto y0=static_cast<long long>(std::floor(ty));
                        const double fx=tx-std::floor(tx),fy=ty-std::floor(ty);
                        double filtered[3]{};
                        for(unsigned oy=0;oy<2;++oy) for(unsigned ox=0;ox<2;++ox) {
                            const auto xi=wrap_index(x0+ox,decoded.width);
                            const auto yi=wrap_index(y0+oy,decoded.height);
                            const auto texel=(std::size_t(yi)*decoded.width+xi);
                            sampled_texture_texels.insert(texel);
                            const double weight=(ox?fx:1.0-fx)*(oy?fy:1.0-fy);
                            for(unsigned channel=0;channel<3;++channel)
                                filtered[channel]+=decoded.rgba[texel*4+channel]*weight;
                        }
                        bool black=true;
                        for(double channel:filtered) {
                            sample_rgb_min=std::min(sample_rgb_min,channel);
                            sample_rgb_max=std::max(sample_rgb_max,channel);
                            black=black&&channel<0.5;
                        }
                        if(black) ++all_black_filtered_samples;
                    }
                }
            }
        }
        const double normal_length=std::sqrt(normal_sum[0]*normal_sum[0]+
            normal_sum[1]*normal_sum[1]+normal_sum[2]*normal_sum[2]);
        const double mean_normal_abs_view_dot=normal_length>0?
            std::abs(normal_sum[2]/normal_length):0.0; // WGL smoke looks along +Z.

        const auto initial_uv_sample=sample_baked_uvs(*frame,cpu.decoded_texture);
        std::vector<TimelinePhaseV1> later_phase_samples;
        std::int32_t timeline_current{},timeline_start{},timeline_end{};
        bool timeline_view_found=false,timeline_visible=false;
        for(const auto& view:factory->manager().views()) {
            if(view.set!=emitted.source_sets.front()||view.pooled) continue;
            timeline_view_found=true;timeline_current=view.current_ms;
            timeline_start=view.start_ms;timeline_end=view.end_ms;
            timeline_visible=view.state.visible!=0;
            break;
        }
        if(timeline_view_found&&timeline_end>timeline_current) {
            // B041: the source clock must stay monotonic. The earlier phases passed timeline-relative ms
            // as absolute time, so the first call moved the clock backwards and pinned the timeline at its
            // end (current_ms=333) for every phase. Step the real manager at 16 ms app frames from the last
            // source absolute time instead, so the authored offset_u ramp (0 to 1 over 0..333 ms) plays as
            // it does in the live path.
            std::int32_t current_phase=timeline_current;
            for(std::int32_t step=1;step<=64&&current_phase<timeline_end;++step) {
                const auto phase_ms=static_cast<std::int32_t>(source_absolute_ms+16*step);
                check(factory->manager().scene_frame(phase_ms,16,error),
                      "sample source FX scene timeline at 16 ms app steps: "+error);
                for(const auto& view:factory->manager().views()) {
                    if(view.set!=emitted.source_sets.front()||view.pooled) continue;
                    current_phase=view.current_ms;
                    break;
                }
                std::shared_ptr<const EffectRenderFrame> phase_frame;
                check(factory->runtime().prepare_render_frame(phase_frame,error),
                      "prepare source FX timeline phase packet: "+error);
                TimelinePhaseV1 phase;phase.absolute_ms=phase_ms;
                if(phase_frame) {
                    phase.packet_count=phase_frame->packets.size();
                    if(!phase_frame->packets.empty()) {
                        const auto& phase_source=phase_frame->packets.front().source;
                        if(phase_source.source_texture_matrix)
                            phase.source_texture_matrix_2d=texture_matrix_2d(*phase_source.source_texture_matrix);
                        phase.uv=sample_baked_uvs(*phase_frame,cpu.decoded_texture);
                        measure_phase_geometry(*phase_frame,phase,target_x,target_y,eye_z,
                                               tangent,diagnostic_width,diagnostic_height);
                    }
                }
                later_phase_samples.push_back(phase);
                phase_frame.reset();
            }
            // B041: the bright band of the swoosh texture must be sampled at some phase of the life,
            // otherwise the trail is black under additive blending in this path.
            std::size_t bright_phases=0;
            for(const auto& p:later_phase_samples) if(p.uv.nonblack_samples>0) ++bright_phases;
            check(bright_phases>0,"B041: no source FX timeline phase samples the bright swoosh band");
        }
        check(RenderCpuFixture::submit(&cpu, frame, error), error);
        check(cpu.submissions == 1 && cpu.frame && cpu.frame->packets.size() == frame->packets.size(),
              "root queue callback did not receive the same source frame");

        cpu.frame.reset();
        frame.reset();
        factory->clear_original_textures();
        check(cpu.releases == cpu.uploads, "CPU texture handles were not released after frame drain");

        // Reproduce the source base-Knight first-skill effect on this exact
        // Session actor, using a fresh source manager so every resulting view
        // and packet can be attributed to root347's real AnimationStart step.
        const auto effect_table_id = std::find(table.set_names().begin(),
            table.set_names().end(), "skill_dh2_prince_warrior_bash_down");
        check(effect_table_id != table.set_names().end() &&
              effect_table_id - table.set_names().begin() == 164,
              "Knight BashDown effect set name did not resolve to exact source row164");
        std::vector<RuntimeSwingFxDiagnosticV1> bashdown_diagnostics;
        RuntimeEffectsFactoryBindingsV1 bashdown_bindings;
        bashdown_bindings.assets = &source_effect_assets;
        bashdown_bindings.tables = table;
        bashdown_bindings.same_pf_world = &same_pf_world;
        bashdown_bindings.scene_view = {&cpu, RenderCpuFixture::bashdown_test_camera,
            RenderCpuFixture::driver, RuntimeEffectsParticleColorPolicyV1::source_white};
        bashdown_bindings.textures = {
            [&cpu](const TextureImage& image, std::uint32_t& id, std::string& e) {
                return cpu.upload(image, id, e);
            },
            [&cpu](std::uint32_t id) { cpu.release(id); }};
        bashdown_bindings.submit = [&cpu](std::shared_ptr<const EffectRenderFrame> f,
                                          std::string& e) {
            return RenderCpuFixture::submit(&cpu, std::move(f), e);
        };
        auto bashdown_factory = RuntimeEffectsFactoryV1::create(*session, actor,
            std::move(bashdown_bindings), error);
        check(bashdown_factory != nullptr,
              "create a fresh same-session BashDown FX manager: " + error);
        check(bashdown_factory->manager().cold_creations() == 0,
              "BashDown source effect attribution requires a fresh manager");
        // Exercise the anchored path against a non-identity same-session actor
        // transform. Its exact current transform is consumed by the manager's
        // anchor_position/rotation/scale callbacks during the source Play call.
        auto* bashdown_actor = session->actor(actor);
        check(bashdown_actor != nullptr, "same-session BashDown anchor actor is absent");
        bashdown_actor->transform.position = {23.0f, -41.0f, 17.0f};
        bashdown_actor->transform.rotation = {0.21f, -0.37f, 0.63f};
        bashdown_actor->transform.scale = {1.25f, 0.75f, 1.5f};
        RuntimeSwingFxObserverV1 bashdown_swing(*session, animations, item_table, table,
            bashdown_factory->manager(), [&](const RuntimeSwingFxDiagnosticV1& d) {
                bashdown_diagnostics.push_back(d);
            });
        const auto bashdown_observer = bashdown_swing.step_entry_observer();
        actual_bashdown_entry.reset();
        session->set_step_entry_observer([&](const CombatSessionStepEntry& entry) {
            if (entry.sequence_id == 347 && entry.step == 0)
                actual_bashdown_entry = entry;
            bashdown_observer(entry);
        });
        OriginalAttackSelection bashdown_selection;
        bashdown_selection.state = dh::foundation::skills_animation::skill_sequence_state(347);
        bashdown_selection.variant = 0;
        CombatSessionStateAnimationServices bashdown_services;
        bashdown_services.finished = [](ActorId, std::string& e) {
            e.clear(); return true;
        };
        check(session->play_actor_source_sequence(actor, bashdown_program.plan,
              bashdown_program.policies, bashdown_selection, std::move(bashdown_services), error),
              "select exact source Knight BashDown sequence on same CombatSession actor: " + error);
        for (std::uint64_t fx_frame = 1; fx_frame <= 12; ++fx_frame) {
            const auto absolute_ms = source_absolute_ms + static_cast<std::int32_t>(fx_frame * 64);
            check(session->update(0.064, presentation_input, {0,0,0}, 0, error),
                  "advance retained Knight BashDown source sequence: " + error);
            check(bashdown_factory->runtime().update(fx_frame, absolute_ms, 64, error),
                  "advance fresh same-session BashDown FX manager: " + error);
        }
        check(actual_bashdown_entry.has_value() && actual_bashdown_entry->actor == actor &&
              actual_bashdown_entry->sequence_id == 347 && actual_bashdown_entry->step == 0 &&
              actual_bashdown_entry->occurrence != 0,
              "same-session source sequence did not publish actual Knight root347 step0 entry");
        const auto bashdown_dispatch = std::find_if(bashdown_diagnostics.begin(),
            bashdown_diagnostics.end(), [&](const RuntimeSwingFxDiagnosticV1& d) {
                return d.actor == actor && d.occurrence == actual_bashdown_entry->occurrence &&
                    d.sequence_id == 347 && d.step == 0;
            });
        check(bashdown_dispatch != bashdown_diagnostics.end() && bashdown_dispatch->dispatched &&
              bashdown_dispatch->source_sets == std::vector<std::int32_t>{164},
              "actual Knight BashDown AnimationStart step did not dispatch source FX set164");
        check(bashdown_factory->manager().cold_creations() > 0,
              "same-session BashDown step did not create its exact source effect resource instance");
        const auto bashdown_views = bashdown_factory->manager().views();
        const auto bashdown_view = std::find_if(bashdown_views.begin(), bashdown_views.end(),
            [](const dh2::fx::MeshFxViewV1& v) {
                return v.set == 164 && v.uri ==
                    "data/3D/interface/skill_dh2_prince_warrior_bash_down.bdae";
            });
        check(bashdown_view != bashdown_views.end(),
              "source set164 instance did not retain exact Warrior BashDown BDAE URI");
        const auto before_duplicate = bashdown_factory->manager().cold_creations();
        bashdown_observer(*actual_bashdown_entry);
        check(!bashdown_diagnostics.empty() &&
              bashdown_diagnostics.back().detail ==
                  "Duplicate exact actor/step occurrence suppressed" &&
              bashdown_factory->manager().cold_creations() == before_duplicate,
              "replayed BashDown step occurrence created a duplicate source effect");
        auto rejected_animations = animations;
        rejected_animations.sequences[347].steps[0].fx = 1000000;
        std::vector<RuntimeSwingFxDiagnosticV1> rejected_diagnostics;
        RuntimeSwingFxObserverV1 rejected_swing(*session, rejected_animations, item_table,
            table, bashdown_factory->manager(), [&](const RuntimeSwingFxDiagnosticV1& d) {
                rejected_diagnostics.push_back(d);
            });
        rejected_swing.step_entry_observer()(*actual_bashdown_entry);
        const bool callback_failure_detail_preserved = !rejected_diagnostics.empty() &&
            !rejected_diagnostics.back().dispatched &&
            rejected_diagnostics.back().detail.find("anchored PlayAnimFXSet") != std::string::npos &&
            rejected_diagnostics.back().detail.find("outside the original EffectsTables set rows") != std::string::npos;
        check(callback_failure_detail_preserved,
              "anchored step callback failure lost its concrete manager/provider diagnostic: " +
              (rejected_diagnostics.empty() ? std::string("no diagnostic") :
                  rejected_diagnostics.back().detail));
        std::shared_ptr<const EffectRenderFrame> bashdown_frame;
        check(bashdown_factory->runtime().prepare_render_frame(bashdown_frame, error),
              "prepare real source BashDown effect packets: " + error);
        check(bashdown_frame && !bashdown_frame->packets.empty(),
              "source set164 produced no retained render packets");
        std::size_t bashdown_mesh_packets = 0, bashdown_particle_packets = 0;
        for (const auto& packet : bashdown_frame->packets) {
            check(packet.source.resource_uri ==
                  "data/3D/interface/skill_dh2_prince_warrior_bash_down.bdae" &&
                  packet.source.resource_bytes && !packet.source.resource_bytes->empty() &&
                  packet.source_retention && !packet.mesh.vertices.empty(),
                  "BashDown packet lost exact source resource, geometry, or retention");
            if (packet.source.kind == EffectDrawKind::authored_mesh) ++bashdown_mesh_packets;
            else if (packet.source.kind == EffectDrawKind::authored_particle)
                ++bashdown_particle_packets;
        }
        check(bashdown_mesh_packets > 0 && bashdown_particle_packets > 0,
              "actual BashDown source BDAE did not produce its authored mesh and particle packet kinds");
        bashdown_actor->transform.position[0] += 17.0f;
        bashdown_actor->transform.position[1] -= 29.0f;
        bashdown_actor->transform.position[2] += 11.0f;
        check(bashdown_factory->runtime().update(13,
              source_absolute_ms + 13 * 64, 64, error),
              "resample moving same-session BashDown anchor: " + error);
        std::shared_ptr<const EffectRenderFrame> moved_anchor_frame;
        check(bashdown_factory->runtime().prepare_render_frame(moved_anchor_frame, error),
              "prepare moved same-session BashDown anchor packets: " + error);
        check(moved_anchor_frame && moved_anchor_frame->packets.size() == bashdown_frame->packets.size(),
              "moving the BashDown anchor changed retained source packet membership");
        bool checked_moved_mesh = false;
        for (std::size_t i = 0; i < bashdown_frame->packets.size(); ++i) {
            const auto& before = bashdown_frame->packets[i];
            const auto& after = moved_anchor_frame->packets[i];
            check(before.source.kind == after.source.kind && before.source.node == after.source.node,
                  "moving the BashDown anchor reordered or replaced source packets");
            if (before.source.kind != EffectDrawKind::authored_mesh) continue;
            check(std::abs((after.world[12] - before.world[12]) - 17.0f) < 1.0e-3f &&
                  std::abs((after.world[13] - before.world[13]) + 29.0f) < 1.0e-3f &&
                  std::abs((after.world[14] - before.world[14]) - 11.0f) < 1.0e-3f,
                  "BashDown mesh did not follow the actual same-session anchor position");
            checked_moved_mesh = true;
        }
        check(checked_moved_mesh, "BashDown anchor movement had no retained mesh packet");
        bashdown_actor->transform.rotation = {0.43f, -0.12f, 0.91f};
        bashdown_actor->transform.scale = {0.8f, 1.4f, 1.1f};
        check(bashdown_factory->runtime().update(14,
              source_absolute_ms + 14 * 64, 64, error),
              "resample rotated/scaled same-session BashDown anchor: " + error);
        std::shared_ptr<const EffectRenderFrame> transformed_anchor_frame;
        check(bashdown_factory->runtime().prepare_render_frame(transformed_anchor_frame, error),
              "prepare rotated/scaled BashDown anchor packets: " + error);
        const auto& moved_mesh = *std::find_if(moved_anchor_frame->packets.begin(),
            moved_anchor_frame->packets.end(), [](const EffectRenderPacket& packet) {
                return packet.source.kind == EffectDrawKind::authored_mesh;
            });
        const auto& transformed_mesh = *std::find_if(transformed_anchor_frame->packets.begin(),
            transformed_anchor_frame->packets.end(), [](const EffectRenderPacket& packet) {
                return packet.source.kind == EffectDrawKind::authored_mesh;
            });
        float basis_delta = 0.0f;
        for (std::size_t i = 0; i < 12; ++i)
            basis_delta = std::max(basis_delta, std::abs(transformed_mesh.world[i] - moved_mesh.world[i]));
        check(basis_delta > 1.0e-3f,
              "BashDown mesh did not follow the actual same-session anchor rotation/scale");
        check(RenderCpuFixture::submit(&cpu, transformed_anchor_frame, error),
              "root queue callback did not receive the same retained transformed BashDown packet frame: " + error);
        cpu.frame.reset();
        transformed_anchor_frame.reset();
        moved_anchor_frame.reset();
        bashdown_frame.reset();
        bashdown_factory->clear_original_textures();
        check(cpu.releases == cpu.uploads,
              "BashDown source texture handles were not released after frame drain");
        session->clear_step_entry_observer();

        check(session->clear_lifecycle_services(error),
              "clear transient lifecycle services before the FX identity restore fixture: " + error);
        session->clear_diagnostic_controller_admission_provider();
        session->detach_for_restore();
        check(!factory->is_bound_to(*session),"FX factory accepted its detached owner");
        check(session->rebind_after_restore(error),"FX identity restore fixture: "+error);
        check(!factory->is_bound_to(*session),"FX factory accepted a new lease on the same Session address");

        session.reset();
        check(!factory->is_bound_to(foreignSession),"Expired FX factory accepted another live session");
        check(runtime_effects_factory_test_expired_session_guards(*factory, error),
              "destroyed CombatSession weak-lease guard regression: " + error);

        const auto fx_name=[&](std::int32_t id)->std::string {
            return id<0?"":(static_cast<std::size_t>(id)<table.set_names().size()?
                table.set_names()[static_cast<std::size_t>(id)]:"<outside-effects-table>");
        };
        const auto print_starter_audit=[&](const StarterWeaponFxAudit& item) {
            std::cout << "{\"profile\":\"" << item.profile << "\",\"main_item\":\""
                      << item.main_item << "\",\"main_item_table_index\":" << item.main_id
                      << ",\"main_same_session_bound\":" << (item.same_session_main_bound?"true":"false")
                      << ",\"main_item_record_words_5_6\":[" << item.main_words[0] << ',' << item.main_words[1] << ']'
                      << ",\"traits_main_item_words_5_6\":[" << item.traits_main_words[0] << ',' << item.traits_main_words[1] << ']'
                      << ",\"main_item_fx_set_name\":\"" << fx_name(item.main_words[1]) << "\""
                      << ",\"off_item\":\"" << item.off_item << "\",\"off_item_table_index\":" << item.off_id
                      << ",\"off_actor_state_slot_bound\":" << (item.same_session_off_slot?"true":"false")
                      << ",\"same_session_offhand_configuration_matches\":" << (item.same_session_off_config_matches?"true":"false")
                      << ",\"session_dual_wield_fact\":" << (item.session_dual_wield?"true":"false")
                      << ",\"session_off_damage_class\":" << item.session_off_damage_class
                      << ",\"session_equipment\":[";
            for(std::size_t i=0;i<item.session_equipment.size();++i) {
                if(i) std::cout << ',';
                std::cout << "\"" << item.session_equipment[i] << "\"";
            }
            std::cout << ']'
                      << ",\"off_item_record_words_5_6\":[" << item.off_words[0] << ',' << item.off_words[1] << ']'
                      << ",\"off_item_fx_set_name\":\"" << fx_name(item.off_words[1]) << "\"}"
                      ;
        };

        std::cout << std::setprecision(8)
                  << "{\"validation\":\"PASS\",\"session_actor\":" << actor
                  << ",\"source_set\":\"" << *set << "\",\"source_fx_event_seeded\":false"
                  << ",\"playable_weapon_source\":{\"item\":\"Longsword01\",\"item_table_index\":" << longsword_id
                  << ",\"same_session_main_hand_equipped\":true,\"item_record_words_5_6\":["
                  << longsword_swoosh[0] << ',' << longsword_swoosh[1] << "]"
                  << ",\"same_session_traits_main_item_words_5_6\":["
                  << session_swoosh[0] << ',' << session_swoosh[1] << ']'
                  << ",\"source_precedence\":\"main-hand ItemRecord FX; then offhand only if main FX is -1; then AnimTable step.fx only if both item FX values are -1\""
                  << ",\"actual_selected_fx_source\":\""
                  << (longsword_swoosh[1] != -1 ? "Longsword01 ItemRecord word[6]" : "AnimTable step.fx fallback")
                  << "\"}"
                  << ",\"source_starter_item_fx_audit\":[";
        print_starter_audit(rogue_dagger_audit);
        std::cout << ',';
        print_starter_audit(mage_staff_audit);
        std::cout << ']'
                  << ",\"source_packets\":" << packet_count
                  << ",\"bashdown_source_fx\":{\"actor\":" << actual_bashdown_entry->actor
                  << ",\"sequence\":347,\"step\":0,\"occurrence\":"
                  << actual_bashdown_entry->occurrence << ",\"source_set_id\":164"
                  << ",\"source_set_name\":\"skill_dh2_prince_warrior_bash_down\""
                  << ",\"exact_bdae_uri\":\"data/3D/interface/skill_dh2_prince_warrior_bash_down.bdae\""
                  << ",\"mesh_packets\":" << bashdown_mesh_packets
                  << ",\"particle_packets\":" << bashdown_particle_packets
                  << ",\"test_camera_calls\":" << cpu.camera_calls
                  << ",\"same_session\":true,\"duplicate_suppressed\":true"
                  << ",\"anchor_position_followed\":true,\"anchor_rotation_scale_followed\":true"
                  << ",\"same_frame_retained_submission\":true"
                  << ",\"callback_failure_detail_preserved\":"
                  << (callback_failure_detail_preserved?"true":"false") << '}'
                  << ",\"destroyed_session_current_lease_and_camera_adapter_rejected\":true"
                  << ",\"same_session_factory_identity_checked\":true"
                  << ",\"foreign_detached_and_rebound_factory_identity_rejected\":true"
                  << ",\"step_fx_occurrence\":{\"actor\":" << emitted.actor
                  << ",\"sequence\":" << emitted.sequence_id << ",\"step\":" << emitted.step
                  << ",\"occurrence\":" << emitted.occurrence
                  << ",\"source_set_id\":" << emitted.source_sets.front()
                  << ",\"source_set_name\":\"" << table.set_names().at(static_cast<std::size_t>(emitted.source_sets.front())) << "\""
                  << ",\"source_set_type\":" << table.sets().at(static_cast<std::size_t>(emitted.source_sets.front())).type
                  << ",\"fresh_manager\":true,\"step_created_source_instance\":true"
                  << ",\"same_session\":true,\"duplicate_suppressed\":true}"
                  << ",\"wgl_diagnostic_cpu_packet\":{\"vertex_count\":" << vertex_count
                  << ",\"world_aabb_min\":[" << bounds_min[0] << ',' << bounds_min[1] << ',' << bounds_min[2] << ']'
                  << ",\"world_aabb_max\":[" << bounds_max[0] << ',' << bounds_max[1] << ',' << bounds_max[2] << ']'
                  << ",\"world_aabb_extent\":[" << extents[0] << ',' << extents[1] << ',' << extents[2] << ']'
                  << ",\"vertex_color_min\":[" << vertex_min[0] << ',' << vertex_min[1] << ',' << vertex_min[2] << ',' << vertex_min[3] << ']'
                  << ",\"vertex_color_max\":[" << vertex_max[0] << ',' << vertex_max[1] << ',' << vertex_max[2] << ',' << vertex_max[3] << ']'
                  << ",\"transformed_normal_abs_dot_view_z_range\":[" << normal_dot_min << ',' << normal_dot_max << ']'
                  << ",\"mean_transformed_normal_abs_dot_view_z\":" << mean_normal_abs_view_dot
                  << ",\"material\":{\"color\":[" << first_material.color[0] << ',' << first_material.color[1] << ',' << first_material.color[2] << ',' << first_material.color[3] << ']'
                  << ",\"texture_id\":" << first_material.texture
                  << ",\"transparent\":" << (first_material.transparent?"true":"false")
                  << ",\"additive\":" << (first_material.additive?"true":"false")
                  << ",\"alpha_reference\":" << first_material.alphaReference
                  << ",\"source_pass\":{\"blend\":" << (source_pass.blend?"true":"false")
                  << ",\"blend_source\":" << source_pass.blendSource
                  << ",\"blend_destination\":" << source_pass.blendDestination
                  << ",\"depth_test\":" << (source_pass.depthTest?"true":"false")
                  << ",\"depth_write\":" << (source_pass.depthWrite?"true":"false")
                  << ",\"cull\":" << (source_pass.cull?"true":"false")
                  << ",\"cull_face\":" << source_pass.cullFace
                  << ",\"front_face\":" << source_pass.frontFace
                  << ",\"alpha_test\":" << (source_pass.alphaTest?"true":"false") << "}}"
                  << ",\"authored_uv_matrix\":{\"base_bres_scene_material_2d\":["
                  << source_base_uv_matrix[0] << ',' << source_base_uv_matrix[1] << ','
                  << source_base_uv_matrix[2] << ',' << source_base_uv_matrix[3] << ','
                  << source_base_uv_matrix[4] << ',' << source_base_uv_matrix[5]
                  << "],\"animated_matrix_used_for_bake_2d\":["
                  << source_animated_uv_matrix[0] << ',' << source_animated_uv_matrix[1] << ','
                  << source_animated_uv_matrix[2] << ',' << source_animated_uv_matrix[3] << ','
                  << source_animated_uv_matrix[4] << ',' << source_animated_uv_matrix[5]
                  << "],\"matrix_order\":\"AuthoredFxMeshGraphV32 sample replaces per-material matrix from source TextureTransform20V1 track; draw_sources applies current matrix to source UV exactly once\"}"
                  << ",\"decoded_texture_alpha_range\":[" << cpu.texture_alpha_min << ',' << cpu.texture_alpha_max << ']'
                  << ",\"projected_triangle_diagnostics\":{\"camera\":\"WGL bounds-derived camera: 640x480, vertical FOV 60deg, eye +Z at 2.8x packet extent\""
                  << ",\"uv_source\":\"actual packet UVs after source texture matrix bake; no second matrix application\""
                  << ",\"triangle_count\":" << projected_triangles
                  << ",\"degenerate_world_triangles\":" << degenerate_world_triangles
                  << ",\"degenerate_projected_triangles\":" << degenerate_screen_triangles
                  << ",\"sum_projected_triangle_area_pixels\":" << total_projected_triangle_area
                  << ",\"covered_pixel_centers_with_overdraw\":" << covered_pixel_centers
                  << ",\"unique_sampled_texture_texels\":" << sampled_texture_texels.size()
                  << ",\"black_bilinear_rgb_samples\":" << all_black_filtered_samples
                  << ",\"bilinear_rgb_channel_min\":" << (sample_rgb_min==255.0?0.0:sample_rgb_min)
                  << ",\"bilinear_rgb_channel_max\":" << sample_rgb_max
                  << ",\"texture_dimensions\":[" << decoded.width << ',' << decoded.height << ']'
                  << "}"
                  << ",\"source_timeline_phases\":{\"initial\":{\"current_ms\":" << timeline_current
                  << ",\"start_ms\":" << timeline_start << ",\"end_ms\":" << timeline_end
                  << ",\"visible\":" << (timeline_visible?"true":"false")
                  << ",\"packet_count\":" << packet_count
                  << ",\"uv_samples\":" << initial_uv_sample.samples
                  << ",\"nonblack_uv_samples\":" << initial_uv_sample.nonblack_samples
                  << ",\"max_texture_channel\":" << initial_uv_sample.max_texture_channel
                  << ",\"max_modulated_channel\":" << initial_uv_sample.max_modulated_channel
                  << ",\"material_color\":[" << initial_uv_sample.material_color[0] << ','
                  << initial_uv_sample.material_color[1] << ',' << initial_uv_sample.material_color[2] << ','
                  << initial_uv_sample.material_color[3] << "],\"uv_min\":[" << initial_uv_sample.uv_min[0]
                  << ',' << initial_uv_sample.uv_min[1] << "],\"uv_max\":[" << initial_uv_sample.uv_max[0]
                  << ',' << initial_uv_sample.uv_max[1] << "]},\"view_found\":"
                  << (timeline_view_found?"true":"false") << ",\"sampled_later_phases\":[";
        for(std::size_t i=0;i<later_phase_samples.size();++i) {
            if(i) std::cout << ',';
            const auto& phase=later_phase_samples[i];
            std::cout << "{\"absolute_ms\":" << phase.absolute_ms
                      << ",\"packet_count\":" << phase.packet_count
                      << ",\"uv_samples\":" << phase.uv.samples
                      << ",\"nonblack_uv_samples\":" << phase.uv.nonblack_samples
                      << ",\"max_texture_channel\":" << phase.uv.max_texture_channel
                      << ",\"max_modulated_channel\":" << phase.uv.max_modulated_channel
                      << ",\"projected_area_pixels\":" << phase.projected_area_pixels
                      << ",\"world_aabb_min\":[" << phase.world_min[0] << ',' << phase.world_min[1]
                      << ',' << phase.world_min[2] << "],\"world_aabb_max\":[" << phase.world_max[0] << ','
                      << phase.world_max[1] << ',' << phase.world_max[2] << "]"
                      << ",\"material_color\":[" << phase.uv.material_color[0] << ','
                      << phase.uv.material_color[1] << ',' << phase.uv.material_color[2] << ','
                      << phase.uv.material_color[3] << "],\"uv_min\":[" << phase.uv.uv_min[0]
                      << ',' << phase.uv.uv_min[1] << "],\"uv_max\":[" << phase.uv.uv_max[0]
                      << ',' << phase.uv.uv_max[1] << "],\"animated_matrix_2d\":["
                      << phase.source_texture_matrix_2d[0] << ',' << phase.source_texture_matrix_2d[1] << ','
                      << phase.source_texture_matrix_2d[2] << ',' << phase.source_texture_matrix_2d[3] << ','
                      << phase.source_texture_matrix_2d[4] << ',' << phase.source_texture_matrix_2d[5] << "]}";
        }
        std::cout << "]}"
                  << '}'
                  << ",\"cpu_uploads\":" << cpu.uploads
                  << ",\"camera_driver_requests\":0,\"live_gl_upload\":false}\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
}
