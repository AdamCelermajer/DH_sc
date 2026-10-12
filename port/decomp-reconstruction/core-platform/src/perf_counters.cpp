#include "core-platform/perf_counters.hpp"

#include "core-platform/deps/debug_switches.hpp"
#include "core-platform/memory.hpp"

namespace {

// The original's node block, taken in the constructor (0x3e84 bytes).
constexpr unsigned kNodeBlockSize = 0x3e84;

// Every counter entry point checks the same debug switch, after reloading the switch file.
bool perfCountersEnabled() {
    DebugSwitches::s_inst.load();
    return DebugSwitches::s_inst.GetSwitch("EnablePerfCounters");
}

}  // namespace

// 0x311730 PerfCounters::CounterEntry::CounterEntry(PerfCounters::CounterEntry const&)
PerfCounters::CounterEntry::CounterEntry(const CounterEntry& other)
    : name_(other.name_),
      value_(other.value_),
      extra1_(other.extra1_),
      extra2_(other.extra2_),
      history_(other.history_) {}

// 0x3110c4 / 0x311118 PerfCounters::PerfCounters()
// Partial: the original reserves the history vector and takes its 0x3e84-byte node
// block with a memory hint. The block is taken here with CustomAlloc; std::list
// allocates the entry nodes itself.
PerfCounters::PerfCounters() : buffer_(CustomAlloc(kNodeBlockSize)) {}

// 0x3115c4 / 0x311620 PerfCounters::~PerfCounters()
// Partial: frees the node block as the original does; the entries and the history are
// released by the member destructors.
PerfCounters::~PerfCounters() {
    if (buffer_ != nullptr) {
        CustomFree(buffer_);
    }
}

// 0x312584 PerfCounters::GetEntry(std::string const&)
// Partial: the original compares the name lengths and then the bytes, and does not
// handle a missing name (its search runs off the end). Here a missing name gives nullptr.
PerfCounters::CounterEntry* PerfCounters::GetEntry(const std::string& name) {
    for (CounterEntry& entry : entries_) {
        if (entry.name_ == name) {
            return &entry;
        }
    }
    return nullptr;
}

// 0x312734 PerfCounters::SetCounterValue(std::string const&, float, float, float)
// Partial: a missing counter is ignored here; the original uses the lookup result
// without a check.
void PerfCounters::SetCounterValue(const std::string& name, float value, float extra1, float extra2) {
    if (!perfCountersEnabled()) {
        return;
    }
    if (CounterEntry* entry = GetEntry(name)) {
        entry->value_ = value;
        entry->extra1_ = extra1;
        entry->extra2_ = extra2;
    }
}

// 0x31177c PerfCounters::Update(float)
// Partial: the layout of the sampled histories is inferred from the loop. While the
// switch is on, each counter appends its current value to its history, this object
// appends its own time, and the time advances by dt.
void PerfCounters::Update(float dt) {
    if (!perfCountersEnabled()) {
        return;
    }
    for (CounterEntry& entry : entries_) {
        entry.history_.push_back(entry.value_);
    }
    history_.push_back(time_);
    time_ = time_ + dt;
}

// 0x311914 PerfCounters::Draw()
// Partial: the original binds the debug material and lays out the bars through the
// glitch video driver, which is not part of this build. The visible checks are kept:
// the switch must be on and at least one counter must exist before anything is drawn.
void PerfCounters::Draw() {
    if (!perfCountersEnabled() || entries_.empty()) {
        return;
    }
}

// Not in the original (see the header).
PerfCounters::CounterEntry& PerfCounters::AddEntry(const std::string& name) {
    entries_.emplace_back();
    entries_.back().name_ = name;
    return entries_.back();
}
