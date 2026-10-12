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

// Helper, not in the original: builds the record that both setters store. The
// original allocates len + 5 bytes (the extra one stays zero), records len + 4 as
// the slot length, and frees the previous slot value before the new one is set.
void CLobbyParameterAndQuery::StoreRecord(std::int32_t index, std::uint8_t kind, const char* value) {
    const std::size_t slot = static_cast<std::size_t>(index);
    const std::size_t valueLen = std::strlen(value);

    char* record = new char[valueLen + 5]();
    record[0] = static_cast<char>(index);                          // slot index, low byte
    record[1] = static_cast<char>(kind);                           // compareOpt or type
    record[2] = static_cast<char>((valueLen >> 8) & 0xff);         // length, high byte
    record[3] = static_cast<char>(valueLen & 0xff);                // length, low byte
    std::memcpy(record + 4, value, valueLen);

    delete[] values_[slot];
    values_[slot] = record;
    lengths_[slot] = static_cast<std::int32_t>(valueLen + 4);
}

// 0x836b10 _ZN23CLobbyParameterAndQuery17AddQueryConditionEi11compareOptTPc
// The original's compareOpt argument is a char; the record keeps its low byte.
void CLobbyParameterAndQuery::AddQueryCondition(std::int32_t index, std::int32_t compareOpt,
                                                const char* value) {
    if (index < 0 || index >= count_ || value == nullptr) {
        return;
    }
    StoreRecord(index, static_cast<std::uint8_t>(compareOpt), value);
}

// 0x836c0c _ZN23CLobbyParameterAndQuery17SetParameterValueEiPc
// The second record byte is the parameter type from the +0x08 table.
void CLobbyParameterAndQuery::SetParameterValue(std::int32_t index, const char* value) {
    if (index < 0 || index >= count_ || value == nullptr) {
        return;
    }
    StoreRecord(index, types_[static_cast<std::size_t>(index)], value);
}

void CLobbyParameterAndQuery::Reserve(std::int32_t count) {
    count_ = count;
    const std::size_t n = count > 0 ? static_cast<std::size_t>(count) : 0;
    types_.assign(n, 0);
    values_.assign(n, nullptr);
    lengths_.assign(n, 0);
}

void CLobbyParameterAndQuery::SetParameterType(std::int32_t index, std::uint8_t type) {
    if (index < 0 || index >= count_) {
        return;
    }
    types_[static_cast<std::size_t>(index)] = type;
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
