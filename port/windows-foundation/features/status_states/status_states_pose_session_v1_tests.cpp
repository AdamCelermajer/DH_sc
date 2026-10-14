// v21.8 w3-pose: the Stunned9 / Scared8 pose through the shared CombatSession source-state path, driven by the status owner.
// Real original tables and clips (Swamp lizard CharAnimTable 34, KnightPlayerBase row), a live CombatSession with a player and a lizard.
//   - the status owner's event path starts the stun pose only when the REAL state accepts event 50012 (Idle yes, Injured no) and
//     re-raises it later (CharStateMachine::Update), the pose is the pre_stunned entry clip then the infinite stunned loop,
//   - the pose ends in Idle3 when the bit clears (CSStunned::OnUpdate), the scare pose is the infinite scared leaf,
//   - the player takes the same path (state 9 / 8, same owner), a stun replaces a scare, a repeated stun does not restart the pose.
#define main b013_attack_continuity_original_main
#include "../combat/b013_attack_continuity_v1_tests.cpp"
#undef main

#include "../combat/session_status_pose_v1.hpp"
#include "status_states_session_v1.hpp"

using namespace dh::foundation::combat;
namespace st = dh::foundation::status_states;

namespace {
void update_frames(CombatSession& session, st::StatusStateSessionV1& status, unsigned frames, std::string& error) {
    for (unsigned frame = 0; frame < frames; ++frame) {
        if (!session.update(1.0 / 60.0, {}, Vec3{0, 0, 0}, 0, error)) throw std::runtime_error("session update: " + error);
        if (!status.bind(session, error)) throw std::runtime_error("status bind: " + error);
        status.advance(session, 1000.0 / 60.0);
    }
}

SkeletalPose pose_of(const CombatSession& session, ActorId id) {
    const auto* visual = session.retained_actor_visual_borrow(id);
    SkeletalPose pose;
    std::string error;
    check(visual && visual->current_local_pose(pose, error), "current local pose: " + error);
    return pose;
}

// Sum of absolute quaternion / translation differences over every authored node.
double pose_dist(const SkeletalPose& a, const SkeletalPose& b) {
    check(a.size() == b.size() && !a.empty(), "pose node counts differ");
    double total = 0;
    for (std::size_t i = 0; i < a.size(); ++i) {
        for (int k = 0; k < 4; ++k) total += std::abs(a[i].quaternion[k] - b[i].quaternion[k]);
        for (int k = 0; k < 3; ++k) total += std::abs(a[i].translation[k] - b[i].translation[k]);
    }
    return total;
}

bool contains(const std::string& text, const char* part) { return text.find(part) != std::string::npos; }

void run(const char* root) {
    AssetCatalog assets(root);
    OriginalPropertyDatabase db;
    OriginalMeleeBindings melee;
    std::string error;
    check(load_original_property_tables(assets, "original-cache/data/pydata", db, error), error);
    check(melee.load(assets, "original-melee-bindings.xml", error), error);
    ActorCustomization custom;
    custom.allow_missing_animation_targets = true;
    OriginalCombatVisualPlan visual_plan;
    check(build_original_combat_visual_plan(assets, melee, "KnightPlayerBase", custom, "w3p-player", visual_plan, error), error);
    const auto readPy = [&](const char* name) { return read(assets, std::string("original-cache/data/pydata/") + name); };
    auto clip_keys = read(assets, "data/animations_dictionary_pyarraynames.bin");
    auto clip_values = readPy("animations_dictionary_pyarray.bin");
    dh2::data::Dictionary clip_dictionary;
    check(dh2::data::load_dictionary(bytes(clip_keys), bytes(clip_values), clip_dictionary, error), error);
    auto anim_records = readPy("animations_pyarray.bin"), anim_names = readPy("animations_pyarraynames.bin"),
         anim_fields = readPy("animations_pystructnames.bin");
    dh2::data::AnimationTables animations;
    check(dh2::data::load_animation_tables(bytes(anim_records), bytes(anim_names), bytes(anim_fields), clip_dictionary, animations, error), error);
    combat::RuntimePlayerProfileAttackBankPlanV1 attack_bank;
    CharacterVisualConfig actor_visual;
    std::vector<std::pair<std::string, std::string>> source_clips;
    prepare_player_attack(assets, "KnightPlayerBase", visual_plan, attack_bank, actor_visual, source_clips, false, error);
    ActorProfileLibrary profiles;
    check(profiles.load(assets, "actor-profiles-v2.xml", error), error);
    const auto* player_profile = profiles.find("KnightPlayerBase");
    const auto* lizard_profile = profiles.find("Swamp_LizadMan_Type1");
    check(player_profile && lizard_profile, "source actor profiles missing");
    auto item_records = readPy("loot_table_pyarray.bin"), item_names = readPy("loot_table_pyarraynames.bin"),
         item_fields = readPy("loot_table_pystructnames.bin");
    dh2::data::ItemTable items;
    check(dh2::data::load_items(bytes(item_records), bytes(item_names), bytes(item_fields), items, error), error);
    auto* constants = dh2_script_constants_create();
    check(constants, "Source animation constants unavailable");
    auto constants_bytes = read(assets, "data/animations_pycst.bin");
    dh2_script_constants_reload reload{};
    check(dh2_script_constants_load(constants, constants_bytes.data(), static_cast<std::uint32_t>(constants_bytes.size()), &reload) == 0,
          "Source constants failed");
    equipment_menu::RuntimePlayerLocomotionConstantsV1 stance_constants;
    check(equipment_menu::load_runtime_player_locomotion_constants_v1(
              [&](const char* group, const char* key, std::int32_t& value, std::string& e) {
                  if (dh2_script_constants_get(constants, group, key, &value)) { e = "Missing source stance constant"; return false; }
                  return true;
              }, stance_constants, error), error);
    dh2_script_constants_destroy(constants);
    const auto main_item = dh2::data::item_id(items, "Longsword01");
    check(main_item >= 0, "Knight source main-hand item missing");
    equipment_menu::RuntimePlayerLocomotionV1 locomotion;
    check(equipment_menu::resolve_runtime_player_locomotion_v1(std::stoi(player_profile->animation_table), items, main_item, -1, 0,
              stance_constants, animations, clip_dictionary, locomotion, error), error);

    SessionStatusAnimationBankV1 player_bank, lizard_bank;
    check(build_session_status_animation_bank_v1(assets, animations, clip_dictionary, std::stoi(player_profile->animation_table),
              locomotion.stance, std::uint32_t(stance_constants.stanced_list_mask), actor_visual, "w3p-player-status", player_bank, error), error);
    CharacterVisualConfig lizard_visual;
    lizard_visual.model_path = lizard_profile->model_uri;
    check(build_session_status_animation_bank_v1(assets, animations, clip_dictionary, std::stoi(lizard_profile->animation_table), 0,
              std::uint32_t(stance_constants.stanced_list_mask), lizard_visual, "w3p-lizard-status", lizard_bank, error), error);
    // The program derivation follows the source hierarchy: stun = pre_stunned entry + infinite stunned leaf; scare = one infinite leaf.
    check(lizard_bank.stun_program.has_entry && lizard_bank.stun_program.has_loop && !lizard_bank.stun_program.loop_only &&
          lizard_bank.stun_program.entry_group_path == std::vector<std::size_t>{0} &&
          lizard_bank.stun_program.loop_leaf_path == (std::vector<std::size_t>{1, 0}), "lizard stun program is entry {0} + loop {1,0}");
    check(lizard_bank.scare_program.has_loop && lizard_bank.scare_program.loop_only && !lizard_bank.scare_program.has_entry,
          "lizard scare program is a loop-only leaf");
    check(contains(lizard_bank.stun_description, "lizardman_pre_stunned") && contains(lizard_bank.stun_description, "lizardman_stunned") &&
          contains(lizard_bank.stun_description, "lizardman_post_stunned"), "lizard stun hierarchy is pre / stunned loop / post");
    check(player_bank.stun_program.has_loop && contains(player_bank.stun_description, "prince_pre_stunned"), "player stun hierarchy");

    CombatSessionConfig config;
    config.playerId = 1;
    config.playerProfileId = "KnightPlayerBase";
    config.tableRoot = "original-cache/data/pydata";
    config.playerVisualConfig = actor_visual;
    config.diagnosticRngSeed = 1;
    config.mainItemId = "Longsword01";
    config.equippedItemIds = {"StartingSuit", "StartingBoots", "StartingGloves", "Longsword01"};
    CombatSessionProfile player;
    player.sequenceAction = attack_bank.static_selection;
    player.sourceAttackBank = attack_bank.bank;
    player.sourceAttackPolicies = attack_bank.sequence_policies;
    player.sourceAttackStateSelection = false;
    player.sourceAnimationClips = source_clips;
    player.sourceAnimationClips.insert(player.sourceAnimationClips.end(), player_bank.plan.config.clips.begin(), player_bank.plan.config.clips.end());
    player.customization = custom;
    player.initialIdle = {"Idle", 0, {0}};
    player.damageMarkerNames = {"attack_mainhand"};
    player.retainedPhaseClock = true;
    player.propertyOptions = {256, true};
    player.reaction = CombatSessionChoice{"Injured", 0, {0}};
    player.death = CombatSessionChoice{"Died", 0, {0}};
    player.motionRoot = "auto";
    config.profiles.emplace("KnightPlayerBase", player);
    CombatSessionProfile enemy;
    enemy.action = {"Attack", 0, {0, 1}};
    enemy.sequenceAction = OriginalAttackSelection{"Attack", 0, {0, 1}};
    enemy.initialIdle = {"Idle", 0, {0}};
    enemy.damageMarkerNames = {"attack_mainhand"};
    enemy.customization = custom;
    enemy.propertyOptions = {20 * 256, true};
    enemy.reaction = CombatSessionChoice{"Injured", 0, {0}};
    enemy.death = CombatSessionChoice{"Died", 0, {0}};
    enemy.motionRoot = "auto";
    enemy.retainedPhaseClock = true;
    enemy.sourceAnimationClips.insert(enemy.sourceAnimationClips.end(), lizard_bank.plan.config.clips.begin(), lizard_bank.plan.config.clips.end());
    config.profiles.emplace("Swamp_LizadMan_Type1", enemy);

    ActorPopulation population;
    PopulationActor placed;
    placed.profileId = "Swamp_LizadMan_Type1";
    placed.definition.stableId = 2;
    placed.definition.sourceId = "w3p-lizard";
    placed.definition.placement = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, -100, 0, 1};
    placed.transform = placed.definition.placement;
    population.actors().push_back(std::move(placed));
    CharacterVisual visual;
    CombatSession session;
    check(session.initialize(assets, db, melee, config, visual, population, {0, 0, 0}, custom, error), error);
    session.set_motion_handler([](ActorState& actor, Vec3 delta, bool enabled, std::string&) {
        if (enabled) { actor.transform.position[0] += delta.x; actor.transform.position[1] += delta.y; actor.transform.position[2] += delta.z; }
        return true;
    });
    check(session.update(0, {}, Vec3{0, 0, 0}, 0, error), error);

    st::StatusStateSessionV1 status;
    bool lizard_has_bank = true;
    unsigned service_calls = 0;
    status.set_pose_service({[&](CombatSession& current, ActorId id, bool stun, bool forced, std::uint64_t generation, bool& started, std::string& e) {
        ++service_calls;
        started = false;
        const auto& bank = id == 1 ? player_bank : lizard_bank;
        if (id == 2 && !lizard_has_bank) return true;
        return play_session_status_pose_v1(current, id, stun, forced, generation, bank, {}, started, e);
    }});
    check(status.bind(session, error), error);
    check(status.owner().external_state_authority(), "the pose service switches the owner to external state authority");

    // 0) the 50012 / 50013 acceptance table (IDA OnInit registrations)
    for (const int state : {1, 3, 4, 5, 8, 13, 15, 18, 19}) check(source_status_event_accepted_v1(state, 50012, 0), "state accepts event 50012");
    for (const int state : {7, 9, 10, 11, 12, 0, 2, 17}) check(!source_status_event_accepted_v1(state, 50012, 0), "state rejects event 50012");
    for (const int state : {3, 4, 5, 13, 15, 18, 19}) check(source_status_event_accepted_v1(state, 50013, 0), "state accepts event 50013");
    for (const int state : {1, 7, 8, 9, 10, 11, 12}) check(!source_status_event_accepted_v1(state, 50013, 0), "state rejects event 50013");
    check(!source_status_event_accepted_v1(6, 50012, 0x6341u) && source_status_event_accepted_v1(6, 50012, 0x10000u),
          "Skill6 is interruptible only with focus-flag bit 16 (CSM_Interrupted)");

    // 1) lizard stun from Idle: pose starts, entry clip then the infinite loop, ends in Idle when the bit clears
    check(session.original_actor_state(2) == 3, "lizard starts in Idle3");
    update_frames(session, status, 10, error);
    const auto idle_a = pose_of(session, 2);
    update_frames(session, status, 17, error);
    const auto idle_b = pose_of(session, 2);
    const double idle_motion = pose_dist(idle_a, idle_b);
    bool applied = false;
    check(status.script_status(session, 2, true, 6000.0, applied, error) && applied, "stun applied through the script status entry");
    check(session.original_actor_state(2) == 3, "the pose is not played inside the applying call");
    update_frames(session, status, 1, error);
    check(session.original_actor_state(2) == 9 && session.owns_pose(2), "stun pose entered Stunned9 through the Session");
    check(!status.can_act(*session.actor(2)), "a stunned lizard cannot act");
    check(status.report().poses_started == 1, "one pose started");
    update_frames(session, status, 3, error);
    const auto entry_a = pose_of(session, 2);
    update_frames(session, status, 6, error);
    const auto entry_b = pose_of(session, 2);
    std::cout << "  pose motion: idle(17f)=" << idle_motion << " entry(6f)=" << pose_dist(entry_a, entry_b)
              << " entry-vs-idle=" << pose_dist(entry_b, idle_b) << '\n';
    check(pose_dist(entry_a, entry_b) > 0.0, "the pre_stunned entry clip is advancing");
    check(pose_dist(entry_b, idle_b) > 4.0 * idle_motion, "the stun entry pose is far from the idle pose");
    // the entry clip closes; the infinite stunned leaf follows and the state stays Stunned9 (CSStunned registers no event 34)
    update_frames(session, status, 90, error);
    check(session.original_actor_state(2) == 9 && session.owns_pose(2), "state stayed Stunned9 after the entry clip ended");
    const auto loop_a = pose_of(session, 2);
    update_frames(session, status, 9, error);
    const auto loop_b = pose_of(session, 2);
    std::cout << "  stunned loop motion(9f)=" << pose_dist(loop_a, loop_b) << " loop-vs-idle=" << pose_dist(loop_b, idle_b) << '\n';
    check(pose_dist(loop_a, loop_b) > 0.0, "the stunned loop keeps animating (not a frozen last frame)");
    check(pose_dist(loop_b, idle_b) > 2.0 * idle_motion, "the stunned loop pose differs from the idle pose");
    // a second stun while stunned neither restarts the timer nor the pose
    check(status.script_status(session, 2, true, 9000.0, applied, error) && applied, "second stun call accepted");
    update_frames(session, status, 2, error);
    check(status.report().poses_started == 1, "a repeated stun does not restart the pose");
    check(status.owner().stun_left_ms(2) < 6000.0, "a repeated stun does not restart the timer (SM_SetStunState sets it once)");
    update_frames(session, status, 260, error);
    check(session.original_actor_state(2) == 3 && !session.owns_pose(2), "the bit clearing returns the lizard to Idle3 (CSStunned::OnUpdate)");
    check(status.can_act(*session.actor(2)), "the lizard acts again");
    check(status.report().poses_ended == 1, "one pose ended");
    update_frames(session, status, 3, error);
    check(pose_dist(pose_of(session, 2), loop_b) > 2.0 * idle_motion, "after the stun the actor is back on its idle clip");

    // 2) stun while the real state does not accept 50012: deferred until it does (Update re-raise)
    std::string state_error;
    check(session.set_actor_original_state(2, 17, state_error), "put the lizard in PreSpawn17 (a state without a 50012 registration): " + state_error);
    check(status.script_status(session, 2, true, 3000.0, applied, error) && applied, "stun while the state rejects it is recorded");
    update_frames(session, status, 3, error);
    check(session.original_actor_state(2) == 17, "PreSpawn17 does not accept event 50012: no pose yet");
    check(!status.can_act(*session.actor(2)), "the active stun bit still blocks decisions");
    check(session.set_actor_original_state(2, 3, state_error), "back to Idle3: " + state_error);
    update_frames(session, status, 2, error);
    check(session.original_actor_state(2) == 9, "the stun is re-raised once the state accepts it");
    // (a hit leaving Stunned9 for Injured11 is covered by the real run; CSStunned registers 50010 with CSM_Injured)
    update_frames(session, status, 240, error); // ride out the stun
    check(session.original_actor_state(2) == 3, "second stun expired to Idle3");

    // 4) scare: infinite scared leaf, no entry clip; ends in Idle at expiry
    check(status.script_status(session, 2, false, 1200.0, applied, error) && applied, "scare applied");
    update_frames(session, status, 1, error);
    check(session.original_actor_state(2) == 8 && session.owns_pose(2), "scare pose entered Scared8");
    update_frames(session, status, 4, error);
    const auto scare_a = pose_of(session, 2);
    update_frames(session, status, 7, error);
    const auto scare_b = pose_of(session, 2);
    std::cout << "  scared leaf motion(7f)=" << pose_dist(scare_a, scare_b) << " scared-vs-idle=" << pose_dist(scare_b, idle_b) << '\n';
    check(pose_dist(scare_a, scare_b) > 0.0 && pose_dist(scare_b, idle_b) > 2.0 * idle_motion, "the scared pose is a moving leaf that differs from idle");
    update_frames(session, status, 30, error);
    check(session.original_actor_state(2) == 8, "the scared leaf keeps looping");
    // a stun replaces a scare (CSScared registers 50012)
    check(status.script_status(session, 2, true, 600.0, applied, error) && applied, "stun on a scared lizard");
    update_frames(session, status, 2, error);
    check(session.original_actor_state(2) == 9, "stun replaced the scare pose");
    update_frames(session, status, 90, error);
    check(session.original_actor_state(2) == 3, "stun then scare timers end in Idle3");

    // 5) the player takes the same path (the original does not gate the tail on IsPlayer)
    const auto idle_player = pose_of(session, 1);
    check(status.script_status(session, 1, true, 800.0, applied, error) && applied, "stun on the player");
    update_frames(session, status, 1, error);
    check(session.original_actor_state(1) == 9 && session.owns_pose(1), "player in Stunned9");
    update_frames(session, status, 5, error);
    check(pose_dist(pose_of(session, 1), idle_player) > 0.0, "the player pose left its idle pose");
    check(!status.can_act(*session.actor(1)), "a stunned player cannot act");
    update_frames(session, status, 100, error);
    check(session.original_actor_state(1) == 3 && status.can_act(*session.actor(1)), "the player returns to Idle3 when the stun ends");
    check(status.script_status(session, 1, false, 500.0, applied, error) && applied, "scare on the player");
    update_frames(session, status, 2, error);
    check(session.original_actor_state(1) == 8, "player in Scared8");
    update_frames(session, status, 60, error);
    check(session.original_actor_state(1) == 3, "player scare ended");

    // 6) an actor whose row has no bank: the owner keeps the bit (decisions blocked) but plays nothing and does not retry every frame
    lizard_has_bank = false;
    const unsigned calls_before = service_calls;
    check(status.script_status(session, 2, true, 400.0, applied, error) && applied, "stun on an actor without a pose bank");
    update_frames(session, status, 10, error);
    check(session.original_actor_state(2) == 3 && !status.can_act(*session.actor(2)), "no pose, but the stun bit still blocks decisions");
    check(service_calls == calls_before + 1, "the refused pose is not retried every frame");
    update_frames(session, status, 40, error);
    check(status.can_act(*session.actor(2)), "the bank-less stun expires");
    lizard_has_bank = true;

    // 7) a stunned actor that dies leaves the pose through the death path and the owner drops its status
    lizard_has_bank = true;
    check(status.script_status(session, 2, true, 5000.0, applied, error) && applied, "stun before death");
    update_frames(session, status, 20, error);
    check(session.original_actor_state(2) == 9, "stunned before death");
    check(session.script_kill_actor(2, error), "kill the stunned lizard: " + error);
    update_frames(session, status, 4, error);
    check(!session.actor(2)->alive() && session.original_actor_state(2) != 9, "the dead lizard is not left in Stunned9");
    check(!status.owner().stunned(2), "the owner dropped the dead actor's status");
    update_frames(session, status, 40, error);
    std::cout << "poses started=" << status.report().poses_started << " ended=" << status.report().poses_ended
              << " failures=" << status.report().pose_failures << '\n';
    check(status.report().pose_failures == 0, "no pose failure");
}
} // namespace

int main(int argc, char** argv) {
    try {
        check(argc == 2, "Supply unified original assets root");
        run(argv[1]);
        std::cout << "PASS w3-pose status pose: Stunned9 pre_stunned + infinite stunned loop, Scared8 infinite leaf, real-state acceptance, "
                     "Idle at expiry, player and monster on one path\n";
        return 0;
    } catch (const std::exception& e) { std::cerr << "FAIL: " << e.what() << '\n'; return 1; }
}
