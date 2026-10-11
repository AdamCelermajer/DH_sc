// P16 DESPAWN isolated tests: the death -> corpse -> Despawn clip -> Limbus/slot state machine, with fake owners.
#include "despawn_after_death_v1.hpp"

#include <cstdlib>
#include <iostream>
#include <set>
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
    bool fail_hide = false;
    bool fail_slot = false;
    bool slot_busy = true;
    bool slot_failed = false;
    bool unknown_slot = false;
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
        s.hide = [this](std::uint64_t a, std::string& e) {
            calls.push_back("hide " + std::to_string(a));
            if(fail_hide){e="hide refused";return false;}
            return true;
        };
        s.finished = [this](std::uint64_t a, bool& done, std::string&) {
            done = clip_done;
            (void)a;
            return true;
        };
        s.release_slot = [this](std::uint64_t a, std::string& e) {
            calls.push_back("release_slot " + std::to_string(a));
            if(unknown_slot){e="unknown slot";return false;}
            if(!slot_busy){e="slot not busy";return false;}
            if(slot_failed||fail_slot){e="slot refused";return false;}
            slot_busy=false;
            return true;
        };
        const auto strict_release=s.release_slot;
        s.reconcile_restored_slot = [this,strict_release](std::uint64_t a, std::string& e) {
            calls.push_back("reconcile_restored_slot " + std::to_string(a));
            if(unknown_slot){e="unknown slot";return false;}
            if(slot_failed){e="slot refused";return false;}
            if(!slot_busy){e.clear();return true;}
            return strict_release(a,e);
        };
        s.respawn = [this](std::uint64_t a, std::string&) {
            calls.push_back("respawn " + std::to_string(a));
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

    // 5. Respawnable authored actor (GetRespawnDelay > 0): hidden, waits for event 47 in Limbus, then respawns once.
    //    A summoned actor never respawns; its slot is released instead.
    {
        DespawnAfterDeathV1 owner;
        Fake fake;
        auto services = fake.services();
        owner.track(40, false, false, 2000, error, 5000);
        owner.death_ended(40, services, error);
        owner.advance(2000, services, error);
        expect(fake.count("hide 40") == 1 && owner.tracked(40) && owner.record(40)->phase == Phase::awaiting_respawn &&
                   fake.count("respawn") == 0,
               "respawnable actor waits in Limbus after the hide");
        owner.advance(4999, services, error);
        expect(fake.count("respawn") == 0 && owner.tracked(40), "respawn timer does not fire early");
        owner.advance(1, services, error);
        expect(fake.count("respawn 40") == 1 && !owner.tracked(40), "event 47 respawns exactly once and clears the record");
        owner.advance(10000, services, error);
        expect(fake.count("respawn 40") == 1, "a finished respawn is not repeated");

        owner.track(41, true, false, 2000, error, 5000);
        owner.death_ended(41, services, error);
        owner.advance(2000, services, error);
        expect(fake.count("respawn 41") == 0 && !owner.tracked(41) && fake.count("release_slot 41") == 1,
               "a summoned actor never respawns; its slot is released");
    }

    // 6. Respawn through the clip path: the Limbus transition after the Despawn clip also schedules the respawn.
    {
        DespawnAfterDeathV1 owner;
        Fake fake;
        auto services = fake.services();
        owner.track(42, false, true, 0, error, 1000);
        owner.death_ended(42, services, error);
        owner.advance(0, services, error);
        fake.clip_done = true;
        owner.poll(services, error);
        expect(owner.tracked(42) && owner.record(42)->phase == Phase::awaiting_respawn,
               "clip completion schedules the respawn in Limbus");
        owner.advance(1000, services, error);
        expect(fake.count("respawn 42") == 1 && !owner.tracked(42), "clip path respawns after event 47");
    }

    // A saved source-dead actor enters Limbus directly. No retained timeline
    // advancement/End34 or another Despawn_Delay is required after a load.
    {
        DespawnAfterDeathV1 owner;Fake fake;auto services=fake.services();
        owner.track(50,false,false,2000,error);owner.death_ended(50,services,error);
        owner.advance(500,services,error);fake.calls.clear();
        expect(owner.restore_dead(50,false,0,services,error)&&!owner.tracked(50),
               "restored dead replaces a pending corpse timer with immediate Limbus");
        expect(fake.count("hide 50")==1&&fake.count("release_body")==0&&fake.count("play_clip")==0,
               "restore hides once without new death End or animation");
        expect(owner.restore_dead(50,false,0,services,error)&&owner.advance(10000,services,error)&&
               fake.count("hide 50")==1,"duplicate restore and updates do not repeat retirement");
        owner.clear();
        expect(owner.restore_dead(50,false,0,services,error)&&fake.count("hide 50")==2,
               "a replacement world can retire the same stable actor identity again");
        expect(owner.track(50,false,false,2000,error)&&owner.death_ended(50,services,error)&&
               owner.advance(1999,services,error)&&fake.count("hide 50")==2,
               "new actual death after restore retains the source 2000ms delay");
        expect(owner.advance(1,services,error)&&fake.count("hide 50")==3,"new death hides at the exact delay");
    }
    {
        DespawnAfterDeathV1 owner;Fake fake;auto services=fake.services();fake.fail_hide=true;
        expect(!owner.restore_dead(60,true,0,services,error)&&owner.tracked(60)&&
               owner.record(60)->phase==Phase::restoring&&fake.count("release_slot")==0,
               "restore hide refusal retains the record and does not free a visible slot");
        fake.fail_hide=false;fake.fail_slot=true;
        expect(!owner.restore_dead(60,true,0,services,error)&&owner.record(60)->restored_hidden,
               "slot refusal retains the reached hide prefix");
        const auto hidden=fake.count("hide 60");fake.fail_slot=false;
        expect(owner.restore_dead(60,true,0,services,error)&&!owner.tracked(60)&&fake.count("hide 60")==hidden,
               "slot retry completes without repeating reached hide");
        owner.restore_dead(60,true,0,services,error);
        expect(fake.count("release_slot 60")==2,"completed restored slot is not released twice");
        // A world replacement has reset the transient pool: an owned but free
        // slot is already retired. The restored path reconciles it quietly,
        // while ordinary release still rejects the same duplicate.
        fake.calls.clear();fake.slot_busy=false;fake.slot_failed=false;fake.unknown_slot=false;
        expect(owner.restore_dead(62,true,0,services,error)&&!owner.tracked(62)&&
               fake.count("hide 62")==1&&fake.count("reconcile_restored_slot 62")==1&&
               fake.count("release_slot 62")==0,
               "restored dead summon accepts an already-free owned slot once");
        expect(!services.release_slot(62,error)&&error=="slot not busy",
               "ordinary duplicate slot release remains strict after restore reconciliation");
        fake.slot_failed=true;
        expect(!owner.restore_dead(65,true,0,services,error)&&owner.record(65)&&
               owner.record(65)->restored_hidden&&error=="slot refused",
               "restored reconciliation rejects a failed slot even when its busy bit is clear");
        fake.slot_failed=false;
        fake.unknown_slot=true;
        expect(!owner.restore_dead(63,true,0,services,error)&&owner.tracked(63)&&
               owner.record(63)->restored_hidden&&error=="unknown slot",
               "restored retirement still rejects an unknown slot after retaining its hide prefix");
        fake.unknown_slot=false;fake.slot_busy=true;fake.fail_slot=true;
        expect(!owner.restore_dead(64,true,0,services,error)&&owner.record(64)&&
               owner.record(64)->restored_hidden&&error=="slot refused",
               "restored retirement still rejects a failed busy slot");
        fake.fail_slot=false;
        expect(owner.restore_dead(61,false,1000,services,error)&&owner.record(61)->phase==Phase::awaiting_respawn&&
               fake.count("play_clip")==0&&owner.advance(999,services,error)&&fake.count("respawn 61")==0&&
               owner.advance(1,services,error)&&fake.count("respawn 61")==1,
               "admitted respawn delay starts at restored Limbus without a corpse delay");
    }
    // Restoring an alive world discards later death timers; no hidden side effect
    // may survive the owning world's replacement.
    {
        DespawnAfterDeathV1 owner;Fake fake;auto services=fake.services();
        owner.track(70,false,false,2000,error);owner.death_ended(70,services,error);owner.clear();
        expect(owner.advance(10000,services,error)&&fake.count("hide 70")==0,
               "world reset cancels a later kill timer before an alive snapshot resumes");
    }

    std::cout << (failures == 0 ? "despawn_after_death_v1 OK" : "despawn_after_death_v1 FAILED") << '\n';
    return failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
