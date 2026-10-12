#include "core-platform/deps/debug_switches.hpp"
#include "core-platform/perf_counters.hpp"

#include <cstdio>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

int main() {
    // With the switch off, nothing is recorded.
    DebugSwitches::s_inst.SetSwitch("EnablePerfCounters", false);
    {
        PerfCounters pc;
        pc.AddEntry("fps");
        pc.SetCounterValue("fps", 60.f, 1.f, 2.f);
        CHECK(pc.GetEntry("fps")->value_ == 0.f);
        pc.Update(0.5f);
        CHECK(pc.GetEntry("fps")->history_.empty());
    }

    // With the switch on, values are stored and each Update samples them.
    DebugSwitches::s_inst.SetSwitch("EnablePerfCounters", true);
    {
        PerfCounters pc;
        PerfCounters::CounterEntry& fps = pc.AddEntry("fps");
        (void)fps;
        pc.AddEntry("mem");
        pc.SetCounterValue("fps", 60.f, 1.f, 2.f);
        PerfCounters::CounterEntry* e = pc.GetEntry("fps");
        CHECK(e != nullptr);
        if (e != nullptr) {
            CHECK(e->value_ == 60.f && e->extra1_ == 1.f && e->extra2_ == 2.f);
            CHECK(e->history_.empty());
        }
        CHECK(pc.GetEntry("missing") == nullptr);
        pc.SetCounterValue("missing", 5.f, 0.f, 0.f);  // ignored, no crash

        pc.Update(0.5f);
        pc.SetCounterValue("fps", 30.f, 0.f, 0.f);
        pc.Update(0.25f);
        e = pc.GetEntry("fps");
        CHECK(e != nullptr && e->history_.size() == 2);
        if (e != nullptr && e->history_.size() == 2) {
            CHECK(e->history_[0] == 60.f && e->history_[1] == 30.f);
        }
        CHECK(pc.GetEntry("mem")->history_.size() == 2);

        // The copy constructor takes the name, the three values and the history.
        PerfCounters::CounterEntry copy(*e);
        CHECK(copy.name_ == "fps" && copy.value_ == 30.f && copy.history_.size() == 2);

        // Switching off stops the samples again.
        DebugSwitches::s_inst.SetSwitch("EnablePerfCounters", false);
        pc.Update(1.f);
        CHECK(pc.GetEntry("fps")->history_.size() == 2);
        DebugSwitches::s_inst.SetSwitch("EnablePerfCounters", true);
        pc.Draw();  // the rendering is not reconstructed; this only checks it returns
    }

    std::printf("%s\n", failures ? "perf_counters: FAILED" : "perf_counters: ok");
    return failures != 0;
}
