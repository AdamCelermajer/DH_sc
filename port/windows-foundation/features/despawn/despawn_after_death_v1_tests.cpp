// P16 DESPAWN isolated tests: the death -> corpse -> Despawn clip -> Limbus/slot state machine, with fake owners.
#include "despawn_after_death_v1.hpp"

#include <cstdlib>
#include <iostream>
#include <vector>

using namespace dh::foundation::despawn;

namespace {

int failures = 0;

void expect(bool ok, const char* what) {
    std::cout << (ok ? "PASS " : "FAIL ") << what << '\n';
    if (!ok) ++failures;
}

struct Fake {
    std::vector<std::string> calls;
    std::vector<std::string> lines;
    bool fail_release_body = false;
    bool fail_play = false;
    bool clip_done = false;
    Services services() {
        Services s;
        s.release_body = [this](std::uint64_t a, std::string& e) {
            calls.push_back("release_body " + std::to_string(a));
            if (fail_release_body) { e = "body owner refused"; return false; }
            return true;
        };
        s.play_clip = [this](std::uint64_t a, std::string& e) {
            calls.push_back("play_clip " + std::to_string(a));
            if (fail_play) { e = "clip refused"; return false; }
            return true;
        };
        s.hide = [this](std::uint64_t a, std::string&) {
            calls.push_back("hide " + std::to_string(a));
            return true;
        };
        s.finished = [this](std::uint64_t a, bool& done, std::string&) {
            done = clip_done;
            (void)a;
            return true;
        };
        s.release_slot = [this](std::uint64_t a, std::string&) {
            calls.push_back("release_slot " + std::to_string(a));
            return true;
        };
        s.log = [this](const std::string& line) { lines.push_back(line); };
        return s;
    }
    std::size_t count(const std::string& prefix) const {
        std::size_t n = 0;
        for (const auto& c : calls) if (c.rfind(prefix, 0) == 0) ++n;
        return n;
    }
};

} // namespace

int main() {
    std::string error;

    // 1. Summoned actor with a clip: release at death end, 2000 ms timer, Despawn clip, finish, slot released.
    {
        DespawnAfterDeathV1 owner;
        Fake fake;
        auto services = fake.services();
        expect(owner.track(7, true, true, 2000, error) && error.empty(), "track dying summoned actor");
        expect(!owner.track(7, true, true, 2000, error) && !error.empty(), "duplicate track refused");
        expect(!owner.death_ended(8, services, error), "death end for an untracked actor refused");
        expect(fake.calls.empty(), "refused death end touches no owner");
        expect(owner.death_ended(7, services, error), "death end releases the body");
        expect(fake.count("release_body 7") == 1 && owner.record(7)->phase == Phase::corpse, "corpse phase after release");
        expect(!owner.death_ended(7, services, error), "second death end refused");

        // 2000 ms at 16 ms steps: expires on the 125th step, not before (CharacterDesign.Despawn_Delay).
        bool early = false;
        for (int i = 0; i < 124; ++i) owner.advance(16, services, error);
        early = fake.count("play_clip") != 0;
        expect(!early && owner.record(7)->remaining_ms == 16, "timer not expired after 124 steps (1984 ms)");
        owner.advance(16, services, error);
        expect(fake.count("play_clip 7") == 1 && owner.record(7)->phase == Phase::despawning, "event 46 starts Despawn clip");

        owner.poll(services, error);
        expect(fake.count("release_slot") == 0 && owner.tracked(7), "poll before clip end keeps the record");
        fake.clip_done = true;
        owner.poll(services, error);
        expect(fake.count("release_slot 7") == 1 && !owner.tracked(7), "clip end releases the slot once and removes the record");
        expect(owner.size() == 0, "no leaked records after completion");
        bool logged = false;
        for (const auto& line : fake.lines) if (line.find("DESPAWN complete actor=7") != std::string::npos) logged = true;
        expect(logged, "completion is logged");
    }

    // 2. Actor with no Despawn clip: hidden straight after the timer; summoned slot released; non-summoned not.
    {
        DespawnAfterDeathV1 owner;
        Fake fake;
        auto services = fake.services();
        owner.track(10, true, false, 0, error);
        owner.death_ended(10, services, error);
        owner.advance(0, services, error);
        expect(fake.count("play_clip 10") == 1 && fake.count("hide 10") == 1 && fake.count("release_slot 10") == 1 &&
                   !owner.tracked(10),
               "no clip: enter Despawn, hide, then release slot in the same step");
        owner.track(11, false, false, 2000, error);
        owner.death_ended(11, services, error);
        owner.advance(2000, services, error);
        expect(fake.count("play_clip 11") == 1 && fake.count("hide 11") == 1 && fake.count("release_slot 11") == 0 && !owner.tracked(11),
               "non-summoned no clip: hidden, no slot release");
    }

    // 3. Owner failures are reported and keep the record for a retry; nothing is dropped silently.
    {
        DespawnAfterDeathV1 owner;
        Fake fake;
        auto services = fake.services();
        owner.track(20, true, true, 2000, error);
        fake.fail_release_body = true;
        expect(!owner.death_ended(20, services, error) && error == "body owner refused" &&
                   owner.record(20)->phase == Phase::dying,
               "body release failure keeps dying phase with reason");
        fake.fail_release_body = false;
        expect(owner.death_ended(20, services, error), "retry after body failure succeeds");
        fake.fail_play = true;
        expect(!owner.advance(2000, services, error) && error == "clip refused" && owner.record(20) != nullptr &&
                   owner.record(20)->phase == Phase::corpse,
               "clip refusal reported, record kept in corpse phase");
        fake.fail_play = false;
        expect(owner.advance(0, services, error) && owner.record(20)->phase == Phase::despawning,
               "retry starts the clip");
    }

    // 4. Zero delay and clear(): a world reset drops every record without touching owners.
    {
        DespawnAfterDeathV1 owner;
        Fake fake;
        auto services = fake.services();
        owner.track(30, false, true, 0, error);
        owner.death_ended(30, services, error);
        owner.clear();
        expect(owner.size() == 0 && fake.count("play_clip") == 0 && fake.count("hide") == 0,
               "clear drops transient records without owner effects");
        owner.track(31, false, true, 0, error);
        owner.death_ended(31, services, error);
        owner.advance(0, services, error);
        expect(fake.count("play_clip 31") == 1, "zero delay expires on the next step");
    }

    std::cout << (failures == 0 ? "despawn_after_death_v1 OK" : "despawn_after_death_v1 FAILED") << '\n';
    return failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
