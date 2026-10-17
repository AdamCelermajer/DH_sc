#pragma once

#include <cstddef>
#include <cstdint>
#include <deque>
#include <string>
#include <utility>

namespace dh2::ui {

// The single MenuMessageManager<CharMenuTutorialMsg, 1> queue. The authored
// SWF peeks the front record through NativeGetCharMenuTutorialMessage; the
// native and script skip paths remove one or all records from this owner.
class SourceCharMenuTutorialQueueV1 {
public:
    struct Message {
        std::int32_t string_id{-1};
        std::string frame;
        std::string menu;
    };

    bool enqueue(std::int32_t string_id, std::string frame,
                 std::string menu) {
        Message message;
        message.string_id=string_id;
        message.frame=std::move(frame);
        message.menu=std::move(menu);
        messages_.push_back(std::move(message));
        return true;
    }

    bool front(Message& message, bool& found) const {
        found = !messages_.empty();
        message = found ? messages_.front() : Message{};
        return true;
    }

    bool skip_one() {
        if (messages_.empty()) return false;
        messages_.pop_front();
        return true;
    }

    void skip_all() noexcept { messages_.clear(); }
    bool active() const noexcept { return !messages_.empty(); }
    std::size_t size() const noexcept { return messages_.size(); }

private:
    std::deque<Message> messages_;
};

} // namespace dh2::ui
