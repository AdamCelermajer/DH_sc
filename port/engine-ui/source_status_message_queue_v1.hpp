#pragma once

#include <cstddef>
#include <deque>
#include <string>
#include <utility>

namespace dh2::ui {

// One MenuMessageManager<StatusMsg, 4> FIFO. The authored HUD peeks the head
// in NativeGetNextStatusMessage; NativeStopMessage("status") removes it and
// starts the next authored onStatusMessage callback when one remains.
class SourceStatusMessageQueueV1 {
public:
    bool enqueue(std::string text, bool& start_now) {
        messages_.push_back(std::move(text));
        start_now = messages_.size() == 1;
        return true;
    }

    bool front(std::string& text, bool& found) const {
        found = !messages_.empty();
        text = found ? messages_.front() : std::string{};
        return true;
    }

    bool stop(bool& start_next) {
        start_next = false;
        if (messages_.empty()) return false;
        messages_.pop_front();
        start_next = !messages_.empty();
        return true;
    }

    void flush() noexcept { messages_.clear(); }
    bool active() const noexcept { return !messages_.empty(); }
    std::size_t size() const noexcept { return messages_.size(); }

private:
    std::deque<std::string> messages_;
};

} // namespace dh2::ui
