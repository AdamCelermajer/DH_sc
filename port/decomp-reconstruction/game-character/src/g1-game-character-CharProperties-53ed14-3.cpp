// CharProperties: the property-name table, buff declarations and buff instances, the slow and damage-over-time buffs,
// the ClassFunc operations, the class loader and the gear loaders (batch g1-game-character-CharProperties-53ed14-3,
// part 4 of 4).
//
// Layout facts used here, from the owner header and the 32-bit original:
//   - Property id i is the word m_w(i+1) of a sheet (Structs::CharacterProperties). m_props is the final sheet
//     (+0xa94) and m_gearProps the gear sheet (+0x710).
//   - m_buffs (+0xe18) is an ordered map keyed by class id. Each BuffDecl holds the deque of its live BuffInst instances.
//   - A BuffInst is a sheet with its level (+0x384), timer id (+0x388), owner (+0x38c) and BuffDecl (+0x390). Its
//     property 172 (word 173, +0x2b4) holds the buff level shifted left by 8, and so does property 172 of m_props.
//   - Arrays::ClassTable rows are Structs::ClassFuncList (m_w1 = count, m_w2 = the ClassFunc elements). A ClassFunc is
//     m_w1 (+4) target property, m_w2 (+8) type, m_w3 (+12), m_w4 (+16) and m_w5 (+20).
//   - Arrays::ItemTable rows are Structs::Item (164 bytes): m_w22 (+0x58) gear type, m_w25 (+0x64), m_w35 (+0x8c), m_w36 (+0x90).
//   - Arrays::ItemPowerTable rows are Structs::ItemPowerRef: m_w3 (+0xc) effect count, m_w4 (+0x10) the effect elements
//     (Structs::AttrBonusEntry, m_w1 = type at +0x4, m_w2 = value at +0x8).
//   - Table elements are host records reached through a typed pointer, as in the other CharProperties units.
#include "dh2/CharProperties.hpp"

#include <cctype>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <map>
#include <new>
#include <string>
#include <vector>

#include "core-platform/deps/application.hpp"
#include "dh2/Character.hpp"
#include "dh2/VisualFXManager.hpp"
#include "pydata-accessors/w7_arrays27.hpp"
#include "pydata-accessors/w7_structs_2.hpp"
#include "pydata-accessors/w7_structs_3.hpp"
#include "pydata-accessors/w7_structs_5.hpp"

// Engine assert level, declared in core-platform/memory.hpp. That header is not included here (see the note in
// g1-game-character-CharProperties-53ed14-0.cpp).
extern int gAssertLevel;

namespace {

// Source locations of the original's ASSERT lines (Windows project paths, kept verbatim).
constexpr char kCharPropsCpp[] =
    "..\\..\\project_vs2005\\Game/..\\..\\sources\\Game\\Objects\\Characters\\Properties\\CharProperties.cpp";

constexpr int kPropertyCount = 224;               // property ids 0..223
constexpr int kBuffLevelProperty = 172;           // word 173 of a sheet: the buff level, stored as level << 8
constexpr int kBuffMaxStacksDefault = 128;        // maxStacks <= 0 becomes 128
constexpr int kBuffTimerEvent = 54;               // event id passed to CharTimers::TMR_Start for a buff
constexpr char kPropNamesPath[] = "PyData/character_properties_pystructnames.bin";

// Original assert handling. Level 2 writes to address 0 in the original, which crashes; the rebuild aborts. Level 1
// prints the ASSERT line to stderr and carries on. Any other level stays silent.
[[noreturn]] void CrashOnAssert() { std::abort(); }

void ReportAssert(const char* condition, const char* file, int line) {
    std::fprintf(stderr, "ASSERT(%s) FAILED: %s:%d\n", condition, file, line);
}

// 32-bit wrapping arithmetic, as the ARM ADD, MUL and LSL do it (no signed overflow in C++).
int WrapAdd(int a, int b) {
    return static_cast<int>(static_cast<std::uint32_t>(a) + static_cast<std::uint32_t>(b));
}

int WrapMul(int a, int b) {
    return static_cast<int>(static_cast<std::uint32_t>(a) * static_cast<std::uint32_t>(b));
}

int ShiftLeft8(int value) { return static_cast<int>(static_cast<std::uint32_t>(value) << 8); }

// The original's LC_API_STRCASECMP: ASCII case-insensitive equality.
bool EqualIgnoreAsciiCase(const char* a, const char* b) {
    for (;; ++a, ++b) {
        const int ca = std::tolower(static_cast<unsigned char>(*a));
        const int cb = std::tolower(static_cast<unsigned char>(*b));
        if (ca != cb) return false;
        if (ca == 0) return true;
    }
}

// Reads the property-name table: a 32-bit count, then for each name a 32-bit length and that many raw bytes (no
// terminator). Each name is copied to a malloc'd block with a terminating zero (CustomFree frees it later) and appended
// to CharProperties::s_propNames.
void ReadPropNames(IStreamBase* stream) {
    const auto count = StreamReader::readAs<std::uint32_t>(stream);
    for (std::uint32_t i = 0; i < count; ++i) {
        const auto length = StreamReader::readAs<std::uint32_t>(stream);
        char* name = static_cast<char*>(std::malloc(static_cast<std::size_t>(length) + 1));
        StreamReader::readStringEx(stream, name, length);
        name[length] = '\0';
        CharProperties::s_propNames.push_back(name);
    }
}

}  // namespace

// 0x3e19c0 CharProperties::BuffDecl::BuffDecl(CharProperties::BuffDecl const&)
// Copies the class id, the FX pointer, the name and the instance deque. The instances are shared pointers, not copies.
CharProperties::BuffDecl::BuffDecl(const BuffDecl& other)
    : m_id(other.m_id), m_fx(other.m_fx), m_name(other.m_name), m_insts(other.m_insts) {}

// 0x3e1a08 CharProperties::BuffDecl::BuffDecl(void)
// Class id -1, no FX, an empty name and an empty instance deque. The original's string block allocation and deque map
// set-up are the empty state that the standard containers give by default.
CharProperties::BuffDecl::BuffDecl() : m_id(-1), m_fx(nullptr) {}

// 0x3e1374 CharProperties::LoadPropNames(char const*)
// PARTIAL: the original (1) calls DebugSwitches::load() and GetSwitch("isTracingCharProperties") on DebugSwitches::s_inst,
// whose result is unused, and (2) opens the table at `path` through the application file system slot +0x90, closes it
// with ApplicationCloseStream and reads it through a StreamBuffer. Neither is reproduced: dh2/DebugSwitches.hpp keeps
// s_inst private, and no header declares the +0x90 open (ApplicationOpenStream is +0x88). Until the open is wired the
// stream is null and the name table stays empty, so GetNameFromPropID and GetPropIDFromName find nothing.
void CharProperties::LoadPropNames(const char* path) {
    (void)path;  // the original opens kPropNamesPath through the file system (see above)
    IStreamBase* const stream = nullptr;  // PARTIAL: the open of the table file is not wired
    if (stream == nullptr) {
        return;
    }
    ReadPropNames(stream);
    ApplicationCloseStream(stream);  // the original closes the file before it reads the StreamBuffer
}

// 0x3e16e0 CharProperties::GetNameFromPropID(int)
// Loads the table on first use, then returns the name of the property, or nullptr for an id outside the table.
const char* CharProperties::GetNameFromPropID(int propId) {
    if (s_propNames.empty()) {
        LoadPropNames(kPropNamesPath);
    }
    if (propId < 0 || static_cast<std::size_t>(propId) >= s_propNames.size()) {
        return nullptr;
    }
    return s_propNames[static_cast<std::size_t>(propId)];
}

// 0x3e1758 CharProperties::GetPropIDFromName(char const*)
// Loads the table on first use. Returns the index of the first name equal to `name` (ASCII case-insensitive), or -1.
int CharProperties::GetPropIDFromName(const char* name) {
    if (s_propNames.empty()) {
        LoadPropNames(kPropNamesPath);
    }
    if (name == nullptr) {
        return -1;
    }
    for (std::size_t i = 0; i < s_propNames.size(); ++i) {
        if (EqualIgnoreAsciiCase(name, s_propNames[i])) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

// 0x3e232c CharProperties::PROPS_AddBuff(int,uint,int,uint,int,char const*)
// Adds an instance of buff class classId lasting durationMs, with at most maxStacks instances (128 when maxStacks <= 0),
// at `level`, and returns it. When the class is full, one instance is reused: the first with a lower level is taken and
// its level raised, and among the instances at `level` the one with the smaller elapsed time loses to the one with the
// larger elapsed time. A reused instance has its timer stopped and restarted. Otherwise a new instance is created in the
// class's deque, unless the class is already at maxStacks, in which case nullptr is returned.
// The level is written to property 172 of the instance and of m_props, then the properties are reset and recalculated.
//
// The FX grab is made: a new class declaration with fxId != -1 and no FX yet takes one from
// VisualFXManager::s_inst.GrabAnimFX(fxId, m_character).
// PARTIAL: (1) The FX animation advance (AnimController slot +0x1c, AnimController::PlayClip) is not made. The original
// passes (size - 1, loop = true) and leaves the other two arguments of PlayClip (unused, animIndex) unset at the call
// site, so the call cannot be reproduced. (2) TMR_TimeLeft leaves its out values untouched when the timer is not
// active. The original then compares stack values, and here both values are zeroed first.
Structs::CharacterProperties* CharProperties::PROPS_AddBuff(int classId, unsigned int durationMs, int maxStacks,
                                                             unsigned int level, int fxId, const char* name) {
    if (maxStacks <= 0) {
        maxStacks = kBuffMaxStacksDefault;
    }

    BuffInst* chosen = nullptr;
    const auto node = m_buffs.find(classId);
    if (node != m_buffs.end()) {
        std::deque<BuffInst*>& insts = node->second.m_insts;
        // The original walks the deque only when its size equals maxStacks, and then visits every instance in order.
        if (static_cast<int>(insts.size()) == maxStacks) {
            for (BuffInst* inst : insts) {
                const auto instLevel = static_cast<unsigned int>(inst->m_level);
                if (instLevel >= level) {
                    if (instLevel == level) {
                        bool take = inst->m_timerId == -1 || chosen == nullptr;
                        if (!take) {
                            unsigned int chosenElapsed = 0;
                            unsigned int chosenInterval = 0;
                            m_character->CharTimers::TMR_TimeLeft(static_cast<unsigned int>(chosen->m_timerId),
                                                                  chosenElapsed, chosenInterval);
                            unsigned int instElapsed = 0;
                            unsigned int instInterval = 0;
                            m_character->CharTimers::TMR_TimeLeft(static_cast<unsigned int>(inst->m_timerId),
                                                                  instElapsed, instInterval);
                            take = chosenElapsed < instElapsed;
                        }
                        if (take) {
                            chosen = inst;
                        }
                    }
                } else {
                    chosen = inst;
                    inst->m_level = static_cast<int>(level);
                }
            }
        }
    }

    if (chosen != nullptr) {
        // Reuse: stop the instance's timer; it is restarted below with the new duration.
        m_character->CharTimers::TMR_Stop(static_cast<unsigned int>(chosen->m_timerId));
        chosen->m_timerId = -1;
    } else {
        // New instance. The class's BuffDecl is created on first use and its name is set every time.
        BuffDecl& decl = m_buffs[classId];
        decl.m_id = classId;
        decl.m_name.assign(name, std::strlen(name));
        if (decl.m_fx == nullptr && fxId != -1) {
            decl.m_fx = VisualFXManager::s_inst.GrabAnimFX(fxId, m_character);
        }
        if (maxStacks <= static_cast<int>(decl.m_insts.size())) {
            return nullptr;
        }
        void* memory = std::malloc(sizeof(BuffInst));
        if (memory == nullptr) {
            throw std::bad_alloc();
        }
        BuffInst* created = new (memory) BuffInst();
        created->m_decl = &decl;
        created->m_owner = this;
        created->m_level = static_cast<int>(level);
        created->m_timerId = -1;
        decl.m_insts.push_back(created);
        chosen = created;
    }

    // Start the timer. A failed start removes the instance again.
    if (durationMs != 0) {
        chosen->m_timerId = m_character->CharTimers::TMR_Start(durationMs, 0, kBuffTimerEvent, chosen);
        if (chosen->m_timerId == -1) {
            PROPS_DelBuff(classId, chosen);
            return nullptr;
        }
    }

    // PARTIAL: with an FX and more than one instance, the original advances the FX animation (see the note above).

    PROPS_ResetSheet(chosen);
    _SetProperty(*chosen, kBuffLevelProperty, ShiftLeft8(static_cast<int>(level)));
    _SetProperty(m_props, kBuffLevelProperty, ShiftLeft8(static_cast<int>(level)));
    return chosen;
}

// 0x3e2720 CharProperties::PROPS_AddDot(int,int,int)
// Checks durationMs > 0 and damage >= 0 (ASSERT lines 1077 and 1078), and an element in -1..4 (ASSERT line 1093, with the
// element name "dot_unknown"). The element names are dot_normal (-1), dot_fire, dot_water, dot_lightning, dot_earth and
// dot_air (4). The original then adds the buff with level = damage, maxStacks = 1 and sets property 127 + element + 1 to
// damage on the new instance before it recalculates that property.
//
// PARTIAL: the class id and the FX id come from the index of "AUTO_DOT_01_FIRE" in Arrays::ClassTable and
// Arrays::AnimatedEffectTable (class id = index + element, FX id = index + element). Those names are in
// pydata-accessors/member_names.hpp, which declares the same structs as w7_arrays27.hpp (the header of the table
// members), so the two cannot be included together and the name search is not written. No buff is added and nullptr
// is returned. Also, the original returns RecalcProperty's int value (the m_props word) on success. The header
// declares a sheet pointer, so that return value cannot be reproduced.
Structs::CharacterProperties* CharProperties::PROPS_AddDot(int durationMs, int damage, int element) {
    if (durationMs <= 0) {
        if (gAssertLevel == 2) {
            CrashOnAssert();  // the original writes to address 0 here
        } else if (gAssertLevel == 1) {
            ReportAssert("durationMs > 0", kCharPropsCpp, 1077);
        }
    }
    if (damage < 0) {
        if (gAssertLevel == 2) {
            CrashOnAssert();
        } else if (gAssertLevel == 1) {
            ReportAssert("damage >= 0", kCharPropsCpp, 1078);
        }
    }
    switch (element) {
        case -1:  // dot_normal
        case 0:   // dot_fire
        case 1:   // dot_water
        case 2:   // dot_lightning
        case 3:   // dot_earth
        case 4:   // dot_air
            break;
        default:
            if (gAssertLevel == 2) {
                CrashOnAssert();
            } else if (gAssertLevel == 1) {
                ReportAssert("!\"WTF! Invalid element for the AddDot\"", kCharPropsCpp, 1093);
            }
            break;
    }
    return nullptr;  // PARTIAL: the class and FX lookups by name are not reachable (see above)
}

// 0x3e2a5c CharProperties::PROPS_DebuffSlow(uint)
// A boss is not affected, and a zero duration does nothing. Otherwise the original adds the "debuff_slow" buff of class
// "Debuff_Slow" (found by name in Arrays::ClassTable) with the FX "AUTO_DEBUFF_SLOW" (found in Arrays::AnimatedEffectTable),
// applies the class to the new sheet, and recalculates properties 48, 47 and 46.
// PARTIAL: the two name lookups are not reachable from this unit (see PROPS_AddDot), so nothing is added after the checks.
void CharProperties::PROPS_DebuffSlow(unsigned int durationMs) {
    if (m_character->IsBoss()) {
        return;
    }
    if (durationMs == 0) {
        return;
    }
    // PARTIAL: the class id of "Debuff_Slow" and the FX id of "AUTO_DEBUFF_SLOW" are not found, so no buff is added.
}

// 0x3e2bcc CharProperties::_ApplyGroupOnlyIf(Structs::CharacterProperties &,Structs::ClassFunc *)
// The original body is empty. _LoadClass calls it for type 8 and returns from its walk right after the call.
void CharProperties::_ApplyGroupOnlyIf(Structs::CharacterProperties& /*sheet*/, Structs::ClassFunc* /*fn*/) {}

// 0x3e2bd0 CharProperties::_SetOID(Structs::CharacterProperties &,Structs::ClassFunc *)
// Sets property fn.m_w1 in the sheet to fn.m_w3.
void CharProperties::_SetOID(Structs::CharacterProperties& sheet, Structs::ClassFunc* fn) {
    _SetProperty(sheet, fn->m_w1, fn->m_w3);
}

// 0x3e2bdc CharProperties::_AddValue(Structs::CharacterProperties &,Structs::ClassFunc *)
// Adds fn.m_w3 to property fn.m_w1 of the sheet (32-bit wrapping).
void CharProperties::_AddValue(Structs::CharacterProperties& sheet, Structs::ClassFunc* fn) {
    const int property = _GetProperty(sheet, fn->m_w1);
    _SetProperty(sheet, fn->m_w1, WrapAdd(property, fn->m_w3));
}

// 0x3e2c10 CharProperties::_ScaleWithPercentageForBuff(Structs::CharacterProperties &,Structs::ClassFunc *,bool)
// Property fn.m_w1 becomes (sheet[fn.m_w3] * base) >> 8, where base is the property fn.m_w1 of the sheet, or of m_props
// when useFinal is set.
void CharProperties::_ScaleWithPercentageForBuff(Structs::CharacterProperties& sheet, Structs::ClassFunc* fn,
                                                 bool useFinal) {
    const int percentage = _GetProperty(sheet, fn->m_w3);
    int base = _GetProperty(sheet, fn->m_w1);
    if (useFinal) {
        base = _GetProperty(m_props, fn->m_w1);
    }
    _SetProperty(sheet, fn->m_w1, WrapMul(percentage, base) >> 8);
}

// 0x3e2c78 CharProperties::_ScaleWithPercentage(Structs::CharacterProperties &,Structs::ClassFunc *,bool)
// Property fn.m_w1 becomes (sheet[fn.m_w3] * sheet[fn.m_w1]) >> 8. The original binary does not read the bool argument.
void CharProperties::_ScaleWithPercentage(Structs::CharacterProperties& sheet, Structs::ClassFunc* fn,
                                          bool /*useFinal*/) {
    const int percentage = _GetProperty(sheet, fn->m_w3);
    const int base = _GetProperty(sheet, fn->m_w1);
    _SetProperty(sheet, fn->m_w1, WrapMul(percentage, base) >> 8);
}

// 0x3e2cc0 CharProperties::_AddOtherProp(Structs::CharacterProperties &,Structs::ClassFunc *)
// Property fn.m_w1 becomes its value plus the value of property fn.m_w3 (32-bit wrapping).
void CharProperties::_AddOtherProp(Structs::CharacterProperties& sheet, Structs::ClassFunc* fn) {
    const int property = _GetProperty(sheet, fn->m_w1);
    const int other = _GetProperty(sheet, fn->m_w3);
    _SetProperty(sheet, fn->m_w1, WrapAdd(other, property));
}

// 0x3e2d04 CharProperties::_MinMax(Structs::CharacterProperties &,Structs::ClassFunc *)
// Property fn.m_w1 is clamped to [fn.m_w3, fn.m_w4] as the original does it: below the minimum it becomes the minimum;
// otherwise it is the property value, capped at the maximum.
void CharProperties::_MinMax(Structs::CharacterProperties& sheet, Structs::ClassFunc* fn) {
    const int property = _GetProperty(sheet, fn->m_w1);
    int result = fn->m_w3;
    if (property >= fn->m_w3) {
        result = fn->m_w4;
        if (property < fn->m_w4) {
            result = property;
        }
    }
    _SetProperty(sheet, fn->m_w1, result);
}

// 0x3e2d4c CharProperties::_LinearWithPropPlusBaseValue(Structs::CharacterProperties &,Structs::ClassFunc *,bool)
// Property fn.m_w1 becomes base + multiplier * (source >> 8), where base is fn.m_w3 (or the property's own value when
// fn.m_w3 is -666), source is property fn.m_w4 and multiplier is fn.m_w5. The source is recalculated first unless the
// sheet is s_temp or useFinal is set; with useFinal, or for s_temp, the source is read from m_props.
// RecalcProperty returns the m_props word of the property it recalculated, so the value is read from m_props after it.
void CharProperties::_LinearWithPropPlusBaseValue(Structs::CharacterProperties& sheet, Structs::ClassFunc* fn,
                                                  bool useFinal) {
    int base = fn->m_w3;
    int source = _GetProperty(sheet, fn->m_w4);
    const int multiplier = fn->m_w5;
    if (base == -666) {
        base = _GetProperty(sheet, fn->m_w1);
    }
    if (&sheet == &s_temp) {
        if (!useFinal) {
            _SetProperty(sheet, fn->m_w1, WrapAdd(base, WrapMul(multiplier, source >> 8)));
            return;
        }
    } else if (!useFinal) {
        RecalcProperty(fn->m_w4);
        source = _GetProperty(m_props, fn->m_w4);
        _SetProperty(sheet, fn->m_w1, WrapAdd(base, WrapMul(multiplier, source >> 8)));
        return;
    }
    source = _GetProperty(m_props, fn->m_w4);
    _SetProperty(sheet, fn->m_w1, WrapAdd(base, WrapMul(multiplier, source >> 8)));
}

// 0x3e2e20 CharProperties::_LoadClass(Structs::CharacterProperties &,int,bool)
// Walks the ClassFunc list of class classId (in Arrays::ClassTable, when classId is in range) and applies each entry to
// the sheet, in order, by its type (fn.m_w2):
//   0 group (_Group), 1 linear with the property plus base value, 2 min/max, 4 add another property, 5 scale by a
//   percentage, 6 scale by a percentage for a buff and then add the value, 7 add the value, 8 ApplyGroupOnlyIf and end
//   the walk, 9 set the id to the value. Other types do nothing.
// The bool is passed through to the group and the scale and linear handlers (the original's useTemp/useFinal flag).
void CharProperties::_LoadClass(Structs::CharacterProperties& sheet, int classId, bool useTemp) {
    if (classId < 0 || static_cast<std::uint32_t>(classId) >= Arrays::ClassTable::size) {
        return;
    }
    const auto* row = reinterpret_cast<const Structs::ClassFuncList*>(Arrays::ClassTable::members) + classId;
    const std::uint32_t count = row->m_w1;
    if (count == 0) {
        return;
    }
    auto* functions = static_cast<Structs::ClassFunc*>(row->m_w2);
    for (std::uint32_t i = 0; i < count; ++i) {
        Structs::ClassFunc* fn = &functions[i];
        switch (fn->m_w2) {
            case 0:
                _Group(sheet, fn, useTemp);
                break;
            case 1:
                _LinearWithPropPlusBaseValue(sheet, fn, useTemp);
                break;
            case 2:
                _MinMax(sheet, fn);
                break;
            case 4:
                _AddOtherProp(sheet, fn);
                break;
            case 5:
                _ScaleWithPercentage(sheet, fn, useTemp);
                break;
            case 6:
                _ScaleWithPercentageForBuff(sheet, fn, useTemp);
                _AddValue(sheet, fn);
                break;
            case 7:
                _AddValue(sheet, fn);
                break;
            case 8:
                // The original returns from the walk here: the empty handler is tail-called, so the entries after it are
                // not applied.
                _ApplyGroupOnlyIf(sheet, fn);
                return;
            case 9:
                _SetOID(sheet, fn);
                break;
            default:
                break;
        }
    }
}

// 0x3e3014 CharProperties::_Group(Structs::CharacterProperties &,Structs::ClassFunc *,bool)
// Loads the classes named by fn.m_w3, fn.m_w4 and fn.m_w5 into the sheet, in that order. A value of -1 names no class.
void CharProperties::_Group(Structs::CharacterProperties& sheet, Structs::ClassFunc* fn, bool useFinal) {
    if (fn->m_w3 != -1) {
        _LoadClass(sheet, fn->m_w3, useFinal);
    }
    if (fn->m_w4 != -1) {
        _LoadClass(sheet, fn->m_w4, useFinal);
    }
    if (fn->m_w5 != -1) {
        _LoadClass(sheet, fn->m_w5, useFinal);
    }
}

// 0x3e3154 CharProperties::_LoadGearStats(int,bool)
// Adds the stats of item row itemId to the gear sheet. The gear type (m_w22) selects the properties:
//   0 and 3: the main-hand pair (81, 82) when leftHand, else the off-hand pair (79, 80)
//   1: the main-hand pair (81, 82); 2: the off-hand pair (79, 80)
//   4 and 5: 79 and 80, and 97 with m_w25 << 8; 6: 71 and 61; 7, 8, 9, 10, 12: 71.
// Each value is added to the property (see _AddProperty). Other types add nothing.
void CharProperties::_LoadGearStats(int itemId, bool leftHand) {
    const Structs::Item& row = reinterpret_cast<const Structs::Item*>(Arrays::ItemTable::members)[itemId];
    Structs::CharacterProperties& gear = m_gearProps;
    switch (row.m_w22) {
        case 0:
        case 3:
            if (leftHand) {
                _AddProperty(gear, 81, row.m_w35);
                _AddProperty(gear, 82, row.m_w36);
            } else {
                _AddProperty(gear, 79, row.m_w35);
                _AddProperty(gear, 80, row.m_w36);
            }
            break;
        case 1:
            _AddProperty(gear, 81, row.m_w35);
            _AddProperty(gear, 82, row.m_w36);
            break;
        case 2:
            _AddProperty(gear, 79, row.m_w35);
            _AddProperty(gear, 80, row.m_w36);
            break;
        case 4:
        case 5:
            _AddProperty(gear, 79, row.m_w35);
            _AddProperty(gear, 80, row.m_w36);
            _AddProperty(gear, 97, ShiftLeft8(row.m_w25));
            break;
        case 6:
            _AddProperty(gear, 71, row.m_w35);
            _AddProperty(gear, 61, row.m_w36);
            break;
        case 7:
        case 8:
        case 9:
        case 0xA:
        case 0xC:
            _AddProperty(gear, 71, row.m_w35);
            break;
        default:
            break;
    }
}

// 0x3e32b4 CharProperties::_LoadGearPower(int,bool)
// Applies the effects of power row powerId (Arrays::ItemPowerTable, m_w3 effects in m_w4) to the gear sheet. Each effect
// has a type (m_w1) and a value (m_w2). Most types add the value to one property. Types 7 to 0x13 and 0x1c use the
// main-hand or off-hand id (leftHand picks the second of each pair). Type 9 sets its property instead of adding to it.
// Types 0, 0x11, 0x24 and 0x29 add several properties. Unknown types do nothing.
void CharProperties::_LoadGearPower(int powerId, bool leftHand) {
    const Structs::ItemPowerRef& row = reinterpret_cast<const Structs::ItemPowerRef*>(Arrays::ItemPowerTable::members)[powerId];
    const std::uint32_t count = row.m_w3;
    if (count == 0) {
        return;
    }
    const auto* effects = static_cast<const Structs::AttrBonusEntry*>(row.m_w4);
    // Adds `amount` to the gear property `propId`.
    auto add = [this](int propId, int amount) { _AddProperty(m_gearProps, propId, amount); };
    for (std::uint32_t i = 0; i < count; ++i) {
        const Structs::AttrBonusEntry& effect = effects[i];
        const int value = effect.m_w2;
        switch (effect.m_w1) {
            case 0:
                add(149, value);
                add(150, value);
                add(151, value);
                add(152, value);
                break;
            case 1:
                add(149, value);
                break;
            case 2:
                add(150, value);
                break;
            case 3:
                add(151, value);
                break;
            case 4:
                add(152, value);
                break;
            case 5:
                add(38, value);
                break;
            case 6:
                add(43, value);
                break;
            case 7:
                add(leftHand ? 98 : 95, value);
                break;
            case 8:
                add(leftHand ? 99 : 96, value);
                break;
            case 9:
                _SetProperty(m_gearProps, leftHand ? 100 : 97, value);
                break;
            case 0xA:
                add(leftHand ? 103 : 101, value);
                break;
            case 0xB:
                add(leftHand ? 104 : 102, value);
                break;
            case 0xC:
                add(leftHand ? 107 : 105, value);
                break;
            case 0xD:
                add(leftHand ? 108 : 106, value);
                break;
            case 0xE:
                add(leftHand ? 111 : 109, value);
                break;
            case 0xF:
                add(leftHand ? 112 : 110, value);
                break;
            case 0x10:
                add(leftHand ? 115 : 113, value);
                break;
            case 0x11:
                add(leftHand ? 116 : 114, value);
                break;
            case 0x12:
                add(leftHand ? 119 : 117, value);
                break;
            case 0x13:
                add(leftHand ? 120 : 118, value);
                break;
            case 0x14:
                add(39, value);
                break;
            case 0x15:
                add(44, value);
                break;
            case 0x16:
                add(40, value);
                break;
            case 0x17:
                add(45, value);
                break;
            case 0x18:
                add(63, value);
                break;
            case 0x19:
                add(60, value);
                break;
            case 0x1A:
                add(61, value);
                break;
            case 0x1B:
                add(71, value);
                break;
            case 0x1C:
                if (leftHand) {
                    add(81, value);
                    add(82, value);
                } else {
                    add(79, value);
                    add(80, value);
                }
                break;
            case 0x1D:
                add(50, value);
                break;
            case 0x1E:
                add(59, value);
                break;
            case 0x1F:
                add(74, value);
                break;
            case 0x20:
                add(77, value);
                break;
            case 0x21:
                add(75, value);
                break;
            case 0x22:
                add(78, value);
                break;
            case 0x23:
                add(76, value);
                break;
            case 0x24:
                add(74, value);
                add(77, value);
                add(75, value);
                add(78, value);
                add(76, value);
                break;
            case 0x25:
                add(132, value);
                break;
            case 0x26:
                add(133, value);
                break;
            case 0x27:
                add(165, value);
                break;
            case 0x28:
                add(158, value);
                break;
            case 0x29:
                add(166, value);
                add(169, value);
                add(167, value);
                add(170, value);
                add(168, value);
                break;
            case 0x2A:
                add(170, value);
                break;
            case 0x2B:
                add(167, value);
                break;
            case 0x2C:
                add(168, value);
                break;
            case 0x2D:
                add(169, value);
                break;
            case 0x2E:
                add(166, value);
                break;
            case 0x2F:
                add(195, value);
                break;
            case 0x30:
                add(196, value);
                break;
            default:
                break;
        }
    }
}
