// Stand-in for the DebugSwitches singleton, which belongs to the debug category.
// Only the lookups that CXPlayerManager::Init makes are declared: load() reloads the switch
// file in the original, and GetSwitch reads one named switch. Every switch defaults to off.
#pragma once

#include <map>
#include <string>

class DebugSwitches {
public:
    static DebugSwitches s_inst;

    void load() {}
    bool GetSwitch(const std::string& name) const {
        auto it = switches_.find(name);
        return it != switches_.end() && it->second;
    }
    void SetSwitch(const std::string& name, bool on) { switches_[name] = on; }

private:
    std::map<std::string, bool> switches_;
};

inline DebugSwitches DebugSwitches::s_inst;
