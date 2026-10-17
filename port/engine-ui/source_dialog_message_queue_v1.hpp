#pragma once

#include <cstddef>
#include <cstdint>
#include <deque>
#include <string>
#include <utility>

namespace dh2::ui {

// One MenuMessageManager<DialogMsg, 1> queue. The active SWF peeks the front
// message; its StopDialog FSCommand removes that message and requests a new
// StartDialog callback only when another message remains.
class SourceDialogMessageQueueV1 {
public:
    struct Message {
        std::string title;
        std::string text;
        std::string style_name;
        std::string actor_name;
        std::int32_t style{-1};
    };

    bool enqueue(Message message, bool& start_now) {
        messages_.push_back(std::move(message));
        start_now = messages_.size() == 1;
        return true;
    }

    bool front(Message& message, bool& found) const {
        found = !messages_.empty();
        message = found ? messages_.front() : Message{};
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
    std::deque<Message> messages_;
};

} // namespace dh2::ui
