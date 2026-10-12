#include "net-session-online/c_lobby_parameter_and_query.hpp"

#include <cstring>

#include "net-session-online/deps/xp_api.hpp"

// 0x836e3c _ZN23CLobbyParameterAndQueryC1Ev
// 0x837224 _ZN23CLobbyParameterAndQueryC2Ev
// Partial. The original opens "oconf.bar", reads it and parses it with
// XP_API_PARSE_DATA, then ends in a non-returning tail call. The file loader is
// not a stand-in yet, so only the not-found path is kept.
CLobbyParameterAndQuery::CLobbyParameterAndQuery() {
    XP_DEBUG_OUT("[CLobbyParameterAndQuery] not found \n", "oconf.bar");
}

// 0x836d00 _ZN23CLobbyParameterAndQueryD1Ev
// 0x836dac _ZN23CLobbyParameterAndQueryD2Ev
CLobbyParameterAndQuery::~CLobbyParameterAndQuery() {
    for (char* value : values_) {
        delete[] value;
    }
}


// 0x836a20 _ZN23CLobbyParameterAndQuery13PackParameterERi
char* CLobbyParameterAndQuery::PackParameter(std::int32_t& packedLength) {
    packedLength = 0;
    if (count_ <= 0) {
        return nullptr;
    }
    std::int32_t total = 0;
    for (std::int32_t i = 0; i < count_; ++i) {
        total += lengths_[i];
    }
    if (total == 0) {
        return nullptr;
    }

    // Buffer holds the count byte, the payload and one trailing zero.
    char* out = new char[static_cast<std::size_t>(total) + 2]();
    // Count byte: the number of parameters with a non-zero length. The
    // original counts down from count_ once for each empty parameter.
    std::int32_t nonEmpty = count_;
    std::int32_t offset = 1;
    for (std::int32_t i = 0; i < count_; ++i) {
        const std::int32_t len = lengths_[i];
        if (len == 0) {
            --nonEmpty;
            continue;
        }
        std::memcpy(out + offset, values_[i], static_cast<std::size_t>(len));
        offset += len;
    }
    out[0] = static_cast<char>(nonEmpty);
    packedLength = offset;
    return out;
}

// 0x836b10 _ZN23CLobbyParameterAndQuery17AddQueryConditionEi11compareOptTPc
// Partial. Only the bounds guard is recovered. The original's tail call after
// the guard is not identified, so nothing is stored.
void CLobbyParameterAndQuery::AddQueryCondition(std::int32_t index, std::int32_t /*compareOpt*/,
                                                const char* value) {
    if (index < 0 || index >= count_ || value == nullptr) {
        return;
    }
}

// 0x836c0c _ZN23CLobbyParameterAndQuery17SetParameterValueEiPc
// Partial. Same guard as AddQueryCondition; the store is not recovered.
void CLobbyParameterAndQuery::SetParameterValue(std::int32_t index, const char* value) {
    if (index < 0 || index >= count_ || value == nullptr) {
        return;
    }
}

void CLobbyParameterAndQuery::Reserve(std::int32_t count) {
    count_ = count;
    values_.assign(count > 0 ? static_cast<std::size_t>(count) : 0, nullptr);
    lengths_.assign(count > 0 ? static_cast<std::size_t>(count) : 0, 0);
}

void CLobbyParameterAndQuery::StoreParameter(std::int32_t index, const char* data,
                                             std::int32_t length) {
    if (index < 0 || index >= count_) {
        return;
    }
    char*& slot = values_[static_cast<std::size_t>(index)];
    delete[] slot;
    slot = nullptr;
    lengths_[static_cast<std::size_t>(index)] = length;
    if (length > 0 && data != nullptr) {
        slot = new char[static_cast<std::size_t>(length)];
        std::memcpy(slot, data, static_cast<std::size_t>(length));
    }
}
