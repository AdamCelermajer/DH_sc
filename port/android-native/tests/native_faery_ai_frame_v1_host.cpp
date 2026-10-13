// Host audit of the Faery CharAI::Update owner (native_faery_ai_frame_v1).
//
// A real Coordinator and the recovered CharAI kernels run against a scripted
// two-Character world (the Faery and its Player). The scripted Host records
// the Character::RaiseEvent calls and the acquisition scan, and plays the
// follower.luac SetMaster response to event 7 so the source order of
// _UpdateTarget, _UpdateMaster, _UpdateAggro and CharAI::OnUpdate can be
// checked without a renderer or a Lua VM.
#include "../app/src/main/cpp/native_faery_ai_frame_v1.hpp"

#include <cmath>
#include <cstdio>
#include <stdexcept>
#include <vector>

namespace {
using namespace dh2::native::faery_ai_frame_v1;

unsigned checks = 0;
void check(bool value, const char* message) {
    if (!value) throw std::runtime_error(message);
    ++checks;
}

constexpr std::uintptr_t kFaery = 0x1001, kPlayer = 0x2002, kAi = 0x3003,
                         kController = 0x4004;
constexpr float kView = 2300.f, kInteract = 250.f, kMelee = 200.f,
                kPlayerMelee = 200.f;

struct World final : Host {
    float faery[3]{0, 0, 0};
    float player[3]{100, 0, 0};
    bool player_dead = false;
    dh2::character_ai_initialization::State* ai = nullptr;
    std::vector<std::uint32_t> events;
    std::vector<std::uintptr_t> payloads;
    unsigned scans = 0, on_updates = 0, zoning_disabled = 0, visibility_syncs = 0;
    float last_scan_radius = 0.f;
    bool auto_master = true;  // the follower.luac OnFriendSpotted -> SetMaster reply

    bool character_state(std::int32_t& state) override { state = 3; return true; }
    bool target_position(std::uintptr_t id, float out[3]) override {
        const float* p = id == kFaery ? faery : id == kPlayer ? player : nullptr;
        if (!p) return false;
        out[0] = p[0]; out[1] = p[1]; out[2] = p[2];
        return true;
    }
    bool is_dead(std::uintptr_t id, bool& dead) override {
        if (id != kPlayer) return false;
        dead = player_dead;
        return true;
    }
    bool ai_row(float& view, float& no_aggro, float& interact) override {
        view = no_aggro = kView;
        interact = kInteract;
        return true;
    }
    bool melee_radius(bool owner_ai, std::uintptr_t id, float& radius) override {
        if (owner_ai) { radius = kMelee; return true; }
        if (id != kPlayer) return false;
        radius = kPlayerMelee;
        return true;
    }
    bool interaction_type(std::uintptr_t subject, std::uintptr_t, std::uint32_t& type) override {
        // Character::GetInteractionType of a friendly, living Player: 3.
        if (subject != kPlayer) return false;
        type = 3;
        return true;
    }
    bool interaction_spot(std::uintptr_t id, float out[3], bool& node) override {
        node = false;
        return target_position(id, out);
    }
    bool resolve_object(std::uintptr_t id) override { return id == kPlayer; }
    bool debug_switch(std::uint32_t, std::uint32_t& value) override { value = 0; return true; }
    bool can_range_attack(bool& value) override { value = false; return true; }
    bool char_ai_id(std::uint32_t& value) override { value = 20; return true; }
    bool raise_event(std::uint32_t event, std::uintptr_t payload) override {
        events.push_back(event);
        payloads.push_back(payload);
        if (event == 7 && auto_master && !ai->master_50) {
            ai->master_50 = payload;  // follower_OnFriendSpotted: SetMaster
            ai->byte_54 = 1;          // master alive (host player branch)
            ai->byte_55 = 1;          // in sight
        }
        return true;
    }
    bool aggro_scan(float radius) override {
        ++scans;
        last_scan_radius = radius;
        // Player is the closest Character in view: friend -> event 7.
        return raise_event(7, kPlayer);
    }
    bool ais_script_on_update() override { ++on_updates; return true; }
    bool ais_flags(std::uint32_t& flags) override { flags = 0; return true; }
    bool disable_zoning() override { ++zoning_disabled; return true; }
    bool sync_visibility() override { ++visibility_syncs; return true; }

    unsigned count(std::uint32_t event) const {
        unsigned n = 0;
        for (const auto value : events) n += value == event;
        return n;
    }
};

dh2::character::Facts coordinator_facts(void*) { return {}; }
void coordinator_service(void*, dh2::character::State*, const dh2::character::Request*) {}

struct Fixture {
    dh2::character::Coordinator coordinator{kFaery};
    dh2::character_ai_initialization::State ai{};
    Owner owner;
    World world;
    Fixture() {
        ai.identity = kAi;
        ai.owner_04 = kFaery;
        ai.active_ais_1c = 0x5005;
        world.ai = &ai;
        check(owner.bind(kFaery, kAi, kController, &ai), "frame owner binds");
        dh2::character::CoordinatorBindings bindings{};
        bindings.context = this;
        bindings.facts = &coordinator_facts;
        bindings.services = {this, &coordinator_service};
        bindings.ai_identity = kAi;
        bindings.ai_owner_projection = owner.projection();
        bindings.controller_identity = kController;
        coordinator.bind(bindings);
        // CSIdle with the AI-update flag (bit 8) set, as CSIdle::OnFocus 0x2380.
        coordinator.state.current = 3;
        coordinator.state.flags = 0x2380;
    }
    Status tick(Result* result, std::uint32_t blocked = 0) {
        FrameInput input;
        input.global_blocked = blocked;
        input.visual = 0x6006;
        return owner.tick(coordinator, world, input, result);
    }
};
}  // namespace

int main() {
    // 1. A Faerie without a master searches the CharacterList within ViewRadius
    //    (AITable +0x3c, no aggro -> +0x40; both 2300) and takes the friend.
    {
        Fixture f;
        Result result;
        check(f.tick(&result) == Status::complete, "first frame completes");
        check(f.world.scans == 1 && f.world.last_scan_radius == kView, "no-master Faery scans its ViewRadius");
        check(f.world.count(7) == 1 && f.ai.master_50 == kPlayer, "friend spotted -> SetMaster");
        check(f.world.zoning_disabled == 1 && f.world.visibility_syncs == 1,
              "CharAI::OnUpdate runs DisableZoning/SyncVisibility once a master is set");
        check(f.world.on_updates == 0, "no OnUpdate script (flags bit0 clear)");
        // 2. With a master, _UpdateAggro returns before any search.
        Result second;
        check(f.tick(&second) == Status::complete, "second frame completes");
        check(f.world.scans == 1, "a Faerie with a master never searches");
    }
    // 3. _UpdateMaster ranges: a friend master uses AI_IsInInteractionRange.
    //    remaining = distance - Faery melee 200 - Player melee 200 <= InteractRadius 250.
    {
        Fixture f;
        f.ai.master_50 = kPlayer; f.ai.byte_54 = 1; f.ai.byte_55 = 1;
        Result result;
        f.world.player[0] = 600;  // 600 - 400 = 200 <= 250: in range -> event 25
        check(f.tick(&result) == Status::complete, "in-range frame completes");
        check(f.world.events.size() == 1 && f.world.events[0] == 25, "in interaction range raises 25 (Stop)");
        f.world.events.clear();
        f.world.player[0] = 700;  // 700 - 400 = 300 > 250: out of range -> event 22
        check(f.tick(&result) == Status::complete, "out-of-range frame completes");
        check(f.world.events.size() == 1 && f.world.events[0] == 22, "out of range raises 22 (MoveTo) every tick");
        f.world.events.clear();
        check(f.tick(&result) == Status::complete && f.world.count(22) == 1, "event 22 repeats each tick");
    }
    // 4. Sight: > ViewRadius raises 20 once (WarpBehind) then nothing; back in sight raises 21.
    {
        Fixture f;
        f.ai.master_50 = kPlayer; f.ai.byte_54 = 1; f.ai.byte_55 = 1;
        Result result;
        f.world.player[0] = 2400;
        check(f.tick(&result) == Status::complete, "out-of-sight frame completes");
        check(f.world.events.size() == 1 && f.world.events[0] == 20, "out of sight raises 20 only");
        check(f.ai.byte_55 == 0, "sight cache byte cleared");
        f.world.events.clear();
        check(f.tick(&result) == Status::complete && f.world.events.empty(),
              "out of sight raises nothing further");
        f.world.player[0] = 1000;
        check(f.tick(&result) == Status::complete, "back in sight completes");
        check(f.world.events.size() == 2 && f.world.events[0] == 21 && f.world.events[1] == 22,
              "back in sight raises 21 then the range event");
    }
    // 5. Master death edge, and nothing while the master is dead.
    {
        Fixture f;
        f.ai.master_50 = kPlayer; f.ai.byte_54 = 1; f.ai.byte_55 = 1;
        Result result;
        f.world.player_dead = true;
        check(f.tick(&result) == Status::complete, "master death frame completes");
        check(f.world.events.size() == 1 && f.world.events[0] == 18, "master death raises 18 only");
    }
    // 6. CharAI::Update gates: global block, controller lock, policy bit.
    {
        Fixture f;
        Result result;
        check(f.tick(&result, 1) == Status::complete, "blocked frame completes");
        check(f.world.scans == 0 && f.world.events.empty(), "v2Controller::s_blocked skips the frame");
        f.coordinator.state.controller_locked = 1;
        check(f.tick(&result) == Status::complete, "locked frame completes");
        check(f.world.scans == 0, "a locked controller skips the frame");
        f.coordinator.state.controller_locked = 0;
        f.coordinator.state.flags = 0x2280;  // bit 0x100 clear
        check(f.tick(&result) == Status::complete && f.world.scans == 0,
              "Character flags 0x100 clear disables CharAI::Update");
    }
    std::printf("native_faery_ai_frame_v1 host audit passed (%u checks)\n", checks);
    return 0;
}
