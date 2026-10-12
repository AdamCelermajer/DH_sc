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

// 0x3110c4 / 0x311118 _ZN12PerfCountersC2Ev / _ZN12PerfCountersC1Ev
// Partial: the listing takes the 0x3e84-byte node block with operator new[] (hint 0,
// the overload is not identified here) and reserves the history vector. The block is
// taken here with CustomAlloc; std::list allocates the entry nodes itself.
PerfCounters::PerfCounters() : buffer_(CustomAlloc(kNodeBlockSize)) {}

// 0x3115c4 / 0x311620 _ZN12PerfCountersD1Ev / _ZN12PerfCountersD2Ev
// Frees the node block, as the listing does; the entries and the history are released
// by the member destructors (the listing clears the list and frees the history storage).
PerfCounters::~PerfCounters() {
    if (buffer_ != nullptr) {
        CustomFree(buffer_);
    }
}

// 0x312584 _ZN12PerfCounters8GetEntryERKSs
// A name that is not present is created: appended to the list with its history sized to
// the sample count (the number of Update calls, which is history_.size()), all zeros.
// The listing's construction of the new entry's history is ambiguous in Hex-Rays, so
// the value and extra fields are left at zero here. Partial for that reason.
PerfCounters::CounterEntry* PerfCounters::GetEntry(const std::string& name) {
    for (CounterEntry& entry : entries_) {
        if (entry.name_ == name) {
            return &entry;
        }
    }
    entries_.emplace_back();
    CounterEntry& created = entries_.back();
    created.name_ = name;
    created.history_.assign(history_.size(), 0.0f);
    return &created;
}

// 0x312734 _ZN12PerfCounters15SetCounterValueERKSsfff
// Gated by the EnablePerfCounters switch. Sets value_, extra1_ and extra2_ on the entry
// for name, creating it when missing (GetEntry does that).
void PerfCounters::SetCounterValue(const std::string& name, float value, float extra1, float extra2) {
    if (!perfCountersEnabled()) {
        return;
    }
    CounterEntry* entry = GetEntry(name);
    entry->value_ = value;
    entry->extra1_ = extra1;
    entry->extra2_ = extra2;
}

// 0x31177c _ZN12PerfCounters6UpdateEf
// While the switch is on, each counter appends its current value to its history, this
// object appends its own time to its history, and the time advances by dt.
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

// 0x311914 _ZN12PerfCounters4DrawEv
// Partial: the original binds the debug material and lays out the bars through the
// glitch video driver (the debug-draw owner, dword_99F73C, is in another category). Not
// reproduced. The checks are kept: the switch must be on and at least one counter must
// exist before anything is drawn.
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
