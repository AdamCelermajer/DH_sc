// CRoomSearchFilter: the search criteria for a room list request.
//
// Source addresses (32-bit ARM, Hex-Rays bodies):
//   ctor 0x8191fc (C1/C2 0x81923c, same body), copy ctor 0x81a428 / 0x81a478, Copy 0x81a3e4,
//   Clear 0x81927c, IsFilterActive(tROOM_ATTRIBUTES_INT) 0x8192a0, IsFilterActive(BIN) 0x8192b8,
//   TestIntValues 0x819488, TestBinValues 0x819510, TestRoomAttributes(CRoomAttributes&) 0x819548,
//   SetSearchAttributeInt 0x819cf8, SetSearchAttributeBin 0x819edc.
//
// Layout (original 32-bit offsets, kept as comments; sizeof is 36 on 32-bit):
//   +0x00 vptr
//   +0x04 std::vector<tSearchFilterInt> (begin +0x04, end +0x08, capacity +0x0c). Entries are 12 bytes.
//   +0x10 std::vector<tSearchFilterBin> (begin +0x10, end +0x14, capacity +0x18). Entries are 76 bytes.
//   +0x1c int filter-active mask (bit = int attribute id). Read by IsFilterActive(int).
//   +0x20 bin filter-active mask (bit = bin attribute id). Read by IsFilterActive(bin).
//
// tSearchFilterInt (12 bytes): +0x00 attribute id, +0x04 operator, +0x08 value (int).
//   Operator values 0..5 follow the TestIntValues switch: ==, !=, <, <=, >, >=. Other values never match.
//   The enum names are ours; the original ROOM_SEARCH_OPERATOR names are not recovered.
// tSearchFilterBin (76 bytes): +0x00 attribute id, +0x04 operator (any nonzero value inverts the test),
//   +0x08 value bytes (64 bytes inline, inferred from the 76-byte stride), +0x48 value size.
//
// Match rule (TestRoomAttributes 0x819548): every int filter must hold on an ACTIVE int attribute of the room,
//   then every bin filter must hold on an ACTIVE bin attribute. An inactive attribute fails the filter.
//   An empty int list and an empty bin list both pass.
//
// Deviations and open points:
//   - Copy (0x81a3e4) copies the two vectors only. The two masks at +0x1c and +0x20 are not copied, as decompiled.
//   - Clear (0x81927c) only resets the vector ends. The masks are not cleared, as decompiled.
//   - The ctors (0x8191fc, 0x81923c) do not write +0x1c or +0x20. The port zero-initialises them.
//     Whether the original relies on the allocator is not identified.
//   - SetSearchAttributeInt / SetSearchAttributeBin and their Erase helpers are not declared here.
#pragma once

#include <array>
#include <cstdint>
#include <cstring>
#include <vector>

#include "net-session-online/c_room_attributes.hpp"

class CRoomSearchFilter {
public:
    enum ROOM_SEARCH_OPERATOR : int {
        kOpEqual = 0,
        kOpNotEqual = 1,
        kOpLess = 2,
        kOpLessEqual = 3,
        kOpGreater = 4,
        kOpGreaterEqual = 5,
    };

    struct tSearchFilterInt {
        std::int32_t attribute = 0;   // +0x00
        std::int32_t op = 0;          // +0x04 ROOM_SEARCH_OPERATOR
        std::int32_t value = 0;       // +0x08
    };

    struct tSearchFilterBin {
        std::int32_t attribute = 0;                 // +0x00
        std::int32_t negate = 0;                    // +0x04 (nonzero inverts the comparison)
        std::array<std::uint8_t, 64> value{};       // +0x08 (capacity inferred from the stride)
        std::int32_t size = 0;                      // +0x48
    };

    // CRoomSearchFilter::CRoomSearchFilter() 0x8191fc
    CRoomSearchFilter() = default;
    // CRoomSearchFilter::CRoomSearchFilter(const CRoomSearchFilter&) 0x81a428 (Copy only)
    CRoomSearchFilter(const CRoomSearchFilter& other) { Copy(other); }
    CRoomSearchFilter& operator=(const CRoomSearchFilter& other) {
        if (this != &other) Copy(other);
        return *this;
    }
    virtual ~CRoomSearchFilter() = default;

    // CRoomSearchFilter::Clear 0x81927c: the vector ends are reset to their begins. Masks are kept.
    void Clear() {
        intFilters_.clear();
        binFilters_.clear();
    }

    // CRoomSearchFilter::Copy 0x81a3e4: copies the two filter vectors only.
    void Copy(const CRoomSearchFilter& other) {
        intFilters_ = other.intFilters_;
        binFilters_ = other.binFilters_;
    }

    // CRoomSearchFilter::IsFilterActive(tROOM_ATTRIBUTES_INT) 0x8192a0
    bool IsFilterActiveInt(int attribute) const { return (intMask_ & (1u << attribute)) != 0; }
    // CRoomSearchFilter::IsFilterActive(tROOM_ATTRIBUTES_BIN) 0x8192b8
    bool IsFilterActiveBin(int attribute) const { return (binMask_ & (1u << attribute)) != 0; }

    // CRoomSearchFilter::TestIntValues 0x819488
    static bool TestIntValues(std::int32_t roomValue, std::int32_t filterValue, std::int32_t op) {
        switch (op) {
            case kOpEqual: return roomValue == filterValue;
            case kOpNotEqual: return roomValue != filterValue;
            case kOpLess: return roomValue < filterValue;
            case kOpLessEqual: return roomValue <= filterValue;
            case kOpGreater: return roomValue > filterValue;
            case kOpGreaterEqual: return roomValue >= filterValue;
            default: return false;
        }
    }

    // CRoomSearchFilter::TestBinValues 0x819510: equal size and equal bytes, inverted when negate != 0.
    static bool TestBinValues(const void* roomBytes, std::uint32_t roomSize, const void* filterBytes,
                              std::uint32_t filterSize, std::int32_t negate) {
        const bool equal = roomSize == filterSize &&
                           (filterSize == 0 || std::memcmp(roomBytes, filterBytes, filterSize) == 0);
        return negate ? !equal : equal;
    }

    // CRoomSearchFilter::TestRoomAttributes(CRoomAttributes&) 0x819548
    bool TestRoomAttributes(const CRoomAttributes& room) const {
        for (const auto& filter : intFilters_) {
            if (!room.IsAttributeActiveInt(filter.attribute)) return false;
            if (!TestIntValues(room.GetAttributeInt(filter.attribute), filter.value, filter.op)) return false;
        }
        for (const auto& filter : binFilters_) {
            if (!room.IsAttributeActiveBin(filter.attribute)) return false;
            std::uint8_t roomBytes[256];
            const std::uint32_t roomSize = room.GetAttributeBin(filter.attribute, roomBytes, sizeof roomBytes);
            if (!TestBinValues(roomBytes, roomSize, filter.value.data(),
                               static_cast<std::uint32_t>(filter.size), filter.negate)) {
                return false;
            }
        }
        return true;
    }

    // Test access. The original stores these as raw vector ends. The port keeps std::vector.
    std::vector<tSearchFilterInt>& IntFilters() { return intFilters_; }
    std::vector<tSearchFilterBin>& BinFilters() { return binFilters_; }
    void SetIntMask(std::uint32_t mask) { intMask_ = mask; }
    void SetBinMask(std::uint32_t mask) { binMask_ = mask; }

private:
    std::vector<tSearchFilterInt> intFilters_;   // +0x04..+0x0c
    std::vector<tSearchFilterBin> binFilters_;   // +0x10..+0x18
    std::uint32_t intMask_ = 0;                  // +0x1c
    std::uint32_t binMask_ = 0;                  // +0x20
};
