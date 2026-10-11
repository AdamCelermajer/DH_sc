#include "door_commands_v1.hpp"

#include <iostream>

namespace dh::foundation::doors {
namespace {

bool string_field(const OriginalCampaignCommand& c, unsigned offset, std::string& out) {
    const auto found = c.strings.find(offset);
    if (found == c.strings.end()) return false;
    out = found->second;
    return true;
}

} // namespace

bool DoorCommandsV1::command(CampaignCommandPhase phase, const OriginalCampaignCommand& c, int module, bool skip,
                             bool& handled, bool& blocking, std::string& error) {
    error.clear();
    handled = false;
    blocking = false;
    if (c.kind != 54 && c.kind != 55) return true;
    handled = true;
    if (phase == CampaignCommandPhase::update) return true;  // ScriptCmd::Update is the source no-op

    const auto executed = executed_.find(&c);
    if (phase == CampaignCommandPhase::is_blocking) {
        if (executed == executed_.end() || !executed->second.wait) return true;
        blocking = doors_.blocking(executed->second.door);
        return true;
    }

    std::string name;
    if (!string_field(c, 12, name)) {
        error = "Missing authored Door command string 12";
        return false;
    }
    std::uint32_t wait = 0;
    const auto waitField = c.scalars.find(20);
    if (waitField != c.scalars.end()) {
        if (waitField->second > 1) {
            error = "Authored Door command wait byte is outside its source range";
            return false;
        }
        wait = waitField->second;
    }
    std::size_t index = 0;
    if (!doors_.find_by_name(name, index) || !doors_.present(index)) {  // absent (deleted by its gate) = no-op
        std::cout << "[door] command kind=" << c.kind << " declaration=" << name << " status=unknown_declaration module=" << module << '\n';
        executed_.erase(&c);
        return true;
    }
    const bool before = doors_.state(index) == DoorStateV1::opened;
    std::string doorError;
    const bool ok = c.kind == 54 ? doors_.open(index, skip, doorError) : doors_.close(index, skip, doorError);
    if (!ok) {
        error = doorError;
        return false;
    }
    std::cout << "[door] command kind=" << c.kind << (c.kind == 54 ? " open" : " close") << " declaration=" << name
              << " skip=" << (skip ? 1 : 0) << " wait=" << wait << " state=" << int(before) << "->" << int(doors_.state(index))
              << " busy=" << (doors_.busy(index) ? 1 : 0) << '\n';
    Executed next;
    next.door = index;
    next.wait = wait != 0;
    executed_[&c] = next;
    return true;
}

} // namespace dh::foundation::doors
