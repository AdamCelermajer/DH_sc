#include "net-session-online/glx_player_message.hpp"

namespace {
// The original checks the list pointer (non-null) and the index against the count.
// The index is taken as unsigned: a negative index fails the test.
template <typename T>
bool InRange(const std::vector<T>& list, std::int32_t index, std::int32_t count) {
    return index >= 0 && index < count && static_cast<std::size_t>(index) < list.size();
}
}  // namespace

// 0x834f54 _ZN16GLXPlayerMessage21getCurrentMsgPositionEv
std::int32_t GLXPlayerMessage::getCurrentMsgPosition() const {
    for (std::int32_t i = 0;; ++i) {
        if (i >= getMsgListCount()) {
            return -1;
        }
        if (getMsgID(i) == getCurrentMsgID()) {
            return i;
        }
    }
}

// 0x834d34 _ZN16GLXPlayerMessage11getMsgCountEii
// The six box groups are 12 bytes apart from +0x6c, so the type picks a group.
std::int32_t GLXPlayerMessage::getMsgCount(std::int32_t type, std::int32_t box) const {
    if (static_cast<std::uint32_t>(box) < 3) {
        const std::int32_t group = BoxIndex(type);
        if (group >= 0) {
            return boxCounts_[group][box];
        }
    }
    return -1;
}

// The original tests the type values 1, 2, 4, 8, 0x10 and 0x20 in that order.
std::int32_t GLXPlayerMessage::BoxIndex(std::int32_t type) {
    switch (type) {
        case 1: return 0;
        case 2: return 1;
        case 4: return 2;
        case 8: return 3;
        case 0x10: return 4;
        case 0x20: return 5;
        default: return -1;
    }
}

// 0x834dc8 _ZN16GLXPlayerMessage8getMsgIDEi
std::int32_t GLXPlayerMessage::getMsgID(std::int32_t index) const {
    if (InRange(ids_, index, messageCount_)) {
        return ids_[static_cast<std::size_t>(index)];
    }
    return -1;
}

// 0x834df8 _ZN16GLXPlayerMessage11getMsgTitleEi
const char* GLXPlayerMessage::getMsgTitle(std::int32_t index) const {
    if (InRange(titles_, index, messageCount_)) {
        return titles_[static_cast<std::size_t>(index)].c_str();
    }
    return nullptr;
}

// 0x834e28 _ZN16GLXPlayerMessage18getMsgSenderNumberEi
std::int32_t GLXPlayerMessage::getMsgSenderNumber(std::int32_t index) const {
    if (InRange(senderNumbers_, index, messageCount_)) {
        return senderNumbers_[static_cast<std::size_t>(index)];
    }
    return 0;
}

// 0x834e58 _ZN16GLXPlayerMessage12getMsgSenderEi
std::int32_t GLXPlayerMessage::getMsgSender(std::int32_t index) const {
    if (InRange(senders_, index, messageCount_)) {
        return senders_[static_cast<std::size_t>(index)];
    }
    return 0;
}

// 0x834e88 _ZN16GLXPlayerMessage23getMsgDesUserNumberListEi
std::int32_t GLXPlayerMessage::getMsgDesUserNumberList(std::int32_t index) const {
    if (InRange(desUserNumbers_, index, messageCount_)) {
        return desUserNumbers_[static_cast<std::size_t>(index)];
    }
    return 0;
}

// 0x834eb8 _ZN16GLXPlayerMessage21getMsgDesUserNameListEi
std::int32_t GLXPlayerMessage::getMsgDesUserNameList(std::int32_t index) const {
    if (InRange(desUserNames_, index, messageCount_)) {
        return desUserNames_[static_cast<std::size_t>(index)];
    }
    return 0;
}

// 0x834ee8 _ZN16GLXPlayerMessage17getMsgCreatedTimeEi
std::int32_t GLXPlayerMessage::getMsgCreatedTime(std::int32_t index) const {
    if (InRange(createdTimes_, index, messageCount_)) {
        return createdTimes_[static_cast<std::size_t>(index)];
    }
    return 0;
}

// 0x834f18 _ZN16GLXPlayerMessage15getMsgReadStateEi
bool GLXPlayerMessage::getMsgReadState(std::int32_t index) const {
    if (InRange(readStates_, index, messageCount_)) {
        return readStates_[static_cast<std::size_t>(index)] != 0;
    }
    return false;
}

// 0x834fa4 _ZN16GLXPlayerMessage10getMsgTypeEi
std::int32_t GLXPlayerMessage::getMsgType(std::int32_t index) const {
    if (InRange(types_, index, messageCount_)) {
        return types_[static_cast<std::size_t>(index)];
    }
    return -1;
}

// 0x834fd4 _ZN16GLXPlayerMessage13clearMsgCountEv
void GLXPlayerMessage::clearMsgCount() {
    for (auto& group : boxCounts_) {
        for (auto& value : group) {
            value = 0;
        }
    }
}

void GLXPlayerMessage::SetMessageForTest(std::int32_t index, std::int32_t id, std::int32_t type,
                                         std::int32_t sender, bool read, const char* title) {
    const auto i = static_cast<std::size_t>(index);
    const auto grow = [i](auto& list) {
        if (list.size() <= i) list.resize(i + 1);
    };
    grow(ids_); grow(titles_); grow(senders_); grow(types_); grow(readStates_);
    ids_[i] = id;
    types_[i] = type;
    senders_[i] = sender;
    readStates_[i] = read ? 1 : 0;
    titles_[i] = title != nullptr ? title : "";
}

void GLXPlayerMessage::SetBoxCountForTest(std::int32_t type, std::int32_t box, std::int32_t value) {
    const std::int32_t group = BoxIndex(type);
    if (group >= 0 && box >= 0 && box < 3) {
        boxCounts_[group][box] = value;
    }
}
