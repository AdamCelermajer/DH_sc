// LuaScript Lua callbacks, batch g1-game-app-LuaScript-5015a8-2 (part 3 of 3).
//
// Every callback here takes sfc::script::lua::Arguments and sfc::script::lua::ReturnValues. Those types have no
// header in this tree (no dh2/sfc/script/lua/Arguments.hpp, ReturnValues.hpp or Value.hpp), so the definitions
// below are stubs and their ledger rows are partial. The value rules the callbacks apply are in luascript_g1 and
// are tested by tests/g1-game-app-LuaScript-5015a8-2_test.cpp. When the sfc headers exist, each stub body is the
// Hex-Rays logic described in its comment, written with those helpers.
//
// The pydata and object globals the callbacks read are Application members, reached through ApplicationGetInstance()
// (dh2/global/ctrl_camera.hpp; its definition is the integrator's). Singleton<Application>::s_inst is 0x99f72c:
//   dword_99F758 = s_inst +0x2c = Application::m_pyDataConstants (PyDataConstants, pydata-accessors w7_pydata12.hpp)
//   dword_99F75C = s_inst +0x30 = Application::m_pyDataArrays    (PyDataArrays, pydata-accessors w7_pydata12.hpp)
//   dword_99F764 = s_inst +0x38 = Application::m_objectManager   (ObjectManager, dh2/ObjectManager.hpp)
// All three are declared in dh2/Application.hpp.

#include <cmath>
#include <cstddef>
#include <limits>
#include <optional>
#include <utility>
#include <vector>

#include "dh2/LuaScript.hpp"

namespace luascript_g1 {

// Float-to-integer conversions as the ARM target performs them: out-of-range values saturate and NaN gives 0.
int FloatToInt32(float value) {
    if (std::isnan(value)) return 0;
    if (value >= 2147483648.0f) return std::numeric_limits<int>::max();
    if (value <= -2147483648.0f) return std::numeric_limits<int>::min();
    return static_cast<int>(value);
}

unsigned FloatToUInt32(float value) {
    if (std::isnan(value) || value <= 0.0f) return 0u;
    if (value >= 4294967296.0f) return 0xFFFFFFFFu;
    return static_cast<unsigned>(value);
}

struct FixedParts {
    int integer;   // pushed first: (int)value >> 8 (arithmetic shift, as the ARM compiler emits)
    float scaled;  // pushed second: (float)(int)value * 2^-8, a single-precision multiply (exact)
};

// _FromFixed arithmetic. Hex-Rays prints the multiplier as 0.0039062, but the asm is `MOV R1,#0x3B800000;
// BL __aeabi_fmul`. 0x3B800000 is 2^-8 exactly (0.00390625f), so the product is exact and stays single precision.
FixedParts SplitFixed(float value) {
    const int raw = FloatToInt32(value);
    FixedParts parts{};
    parts.integer = raw >> 8;
    parts.scaled = static_cast<float>(raw) * 0.00390625f;
    return parts;
}

// _BitXOr: (int)args[1] ^ (int)args[0].
int BitXOr(float first, float second) {
    return FloatToInt32(second) ^ FloatToInt32(first);
}

// _BitNot: ~(int)args[0].
int BitNot(float first) {
    return ~FloatToInt32(first);
}

// Selector of _GetCurrentLevelRange: the first argument truncated to int when it is a number, else 0.
int LevelRangeSelector(bool firstIsNumber, float firstValue) {
    return firstIsNumber ? FloatToInt32(firstValue) : 0;
}

// Pair pushed for a selector. The six ints are the Structs::LevelDeclaration fields m_w12..m_w17 (+0x30..+0x44,
// pydata-accessors structs_records.hpp). They are plain ints so the rule can be tested without the accessor's
// out-of-line object code. Selector 1 gives (m_w16, m_w13) = (+0x40, +0x34), 2 gives (m_w17, m_w14) = (+0x44, +0x38),
// and 0 gives (m_w15, m_w12) = (+0x3c, +0x30). Any other selector pushes nothing.
std::optional<std::pair<int, int>> LevelRangeFor(int selector, int w12, int w13, int w14, int w15, int w16, int w17) {
    switch (selector) {
        case 1: return std::make_pair(w16, w13);
        case 2: return std::make_pair(w17, w14);
        case 0: return std::make_pair(w15, w12);
        default: return std::nullopt;
    }
}

// Pair pushed by _GetCurrentLevelRange when the current level has no list index (m_levelListIndex == -1).
constexpr std::pair<int, int> kNoLevelRange{-1, -1};

// _GetGameObjectsByType selection. matches[i] is true when the i-th node in tree order has a non-zero object id
// (node +0x2c), resolves to a non-null GameObject, and that object's class name equals the requested name. The first
// `skip` matches are passed over, then matches are picked until 15 have been picked (the original loop stops once the
// pushed count passes 14).
constexpr std::size_t kMaxTypeMatches = 15;
std::vector<std::size_t> SelectTypeMatches(const std::vector<bool>& matches, unsigned skip) {
    std::vector<std::size_t> picked;
    unsigned skipped = 0;
    for (std::size_t i = 0; i < matches.size(); ++i) {
        if (!matches[i]) continue;
        if (skip <= skipped) {
            picked.push_back(i);
            if (picked.size() >= kMaxTypeMatches) break;
        } else {
            ++skipped;
        }
    }
    return picked;
}

// _SetGameType accepts 0..3. The assert text "gt < Constants::GameTypes::COUNT" gives COUNT = 4.
constexpr unsigned kGameTypesCount = 4;
bool IsValidGameType(unsigned value) {
    return value < kGameTypesCount;
}

}  // namespace luascript_g1

// 0x37ee84 LuaScript::_FromFixed(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// partial: waits on dh2/sfc/script/lua/Arguments.hpp, ReturnValues.hpp and Value.hpp (none in this tree).
// Hex-Rays: when args is not empty, push SplitFixed(args[0]).integer, then push SplitFixed(args[0]).scaled as a
// number. Both pushes read args[0]; the pseudocode re-reads the first element for the second push.
void LuaScript::_FromFixed(const sfc::script::lua::Arguments&, sfc::script::lua::ReturnValues&, void*) {}

// 0x37ef3c LuaScript::_CallPyScript(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// partial: waits on dh2/sfc/script/lua/Arguments.hpp and ReturnValues.hpp (none in this tree).
// Hex-Rays: when args is not empty and args[0] is a string, id = ScriptManager::s_inst.GetIDFromName(name, true).
// When id != -1 and IsScriptRunning(id) is false, call StartScript(id, -1, false). IsScriptRunning is not called
// when id is -1. The pseudocode return value is discarded by the void signature.
void LuaScript::_CallPyScript(const sfc::script::lua::Arguments&, sfc::script::lua::ReturnValues&, void*) {}

// 0x37efe4 LuaScript::_Include(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// partial: waits on dh2/sfc/script/lua/Arguments.hpp and Value.hpp (none in this tree).
// Hex-Rays: when args is not empty and args[0] is a string, the self pointer (the LuaScript) runs Load(name).
// The pseudocode returns the Load result, which the void signature discards.
void LuaScript::_Include(const sfc::script::lua::Arguments&, sfc::script::lua::ReturnValues&, void*) {}

// 0x37f03c LuaScript::_GetGameObjectsByType(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// partial: waits on dh2/sfc/script/lua/Arguments.hpp and ReturnValues.hpp (none in this tree). The container is
// declared: the walked list is ObjectManager::m_objectMap (+0x0c, std::map<int, ObjectListItem>, dh2/ObjectManager.hpp),
// reached from dword_99F764 = s_inst +0x38 = Application::m_objectManager (dh2/Application.hpp). The walk reads the
// object id at node +0x2c (asm only). ObjectListItem is forward-declared and has no layout yet, so the node offset
// and whether it equals the map key are not checkable from a header.
// Hex-Rays: skip = FloatToUInt32(args[1]) when args has more than one value and args[1] is a number, else 0.
// When args[0] is a string, walk the object tree in order: header at +0x0c, leftmost node at +0x14, object id at
// node +0x2c. A node with id 0 is skipped. Otherwise the id resolves to a GameObject, and a non-null GameObject whose
// PropertyMap::GetThisClassName equals the name is a match. SelectTypeMatches(matches, skip) gives the objects to push
// as userdata.
void LuaScript::_GetGameObjectsByType(const sfc::script::lua::Arguments&, sfc::script::lua::ReturnValues&, void*) {}

// 0x37f1f0 LuaScript::_GetCurrentLevelRange(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// partial: waits on dh2/sfc/script/lua/Arguments.hpp and ReturnValues.hpp (none in this tree), and on a typed row
// accessor for Arrays::LevelList. pydata-accessors w7_arrays_00.hpp exposes only the raw Arrays::LevelList::members
// table at a 72-byte stride, so each row must be decoded at the file stride into Structs::LevelDeclaration (fields
// m_w12..m_w17, +0x30..+0x44, structs_records.hpp). The level index is Level::m_levelListIndex (+0x3c, dh2/Level.hpp).
// Hex-Rays: level index = Application::GetCurrentLevel()->m_levelListIndex (Level +0x3c, no null check in the
// original). When it is -1, push kNoLevelRange. Otherwise selector = LevelRangeSelector(args[0] is a number,
// args[0]) and push the pair from LevelRangeFor when it has a value; other selectors push nothing.
void LuaScript::_GetCurrentLevelRange(const sfc::script::lua::Arguments&, sfc::script::lua::ReturnValues&, void*) {}

// 0x37f354 LuaScript::_GetPyCst(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// partial: waits on dh2/sfc/script/lua/Arguments.hpp, ReturnValues.hpp and Value.hpp (none in this tree).
// The instance is dword_99F758 = s_inst +0x2c = Application::m_pyDataConstants (dh2/Application.hpp), reached through
// ApplicationGetInstance() (dh2/global/ctrl_camera.hpp). The class PyDataConstants is in pydata-accessors w7_pydata12.hpp.
// Hex-Rays: when args has at least two values and both are strings, push as an integer
// PyDataConstants::getConstant(instance, args[0], args[1]).
void LuaScript::_GetPyCst(const sfc::script::lua::Arguments&, sfc::script::lua::ReturnValues&, void*) {}

// 0x37f4a8 LuaScript::_GetPyStruct(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// partial: waits on dh2/sfc/script/lua/Arguments.hpp, ReturnValues.hpp and Value.hpp (none in this tree).
// The instance is dword_99F75C = s_inst +0x30 = Application::m_pyDataArrays (dh2/Application.hpp), reached through
// ApplicationGetInstance() (dh2/global/ctrl_camera.hpp). The class PyDataArrays is in pydata-accessors w7_pydata12.hpp.
// Hex-Rays: when args has at least two values and both are strings, push as an integer
// PyDataArrays::GetOID(instance, args[0], args[1]). The body is the same as _GetPyOID.
void LuaScript::_GetPyStruct(const sfc::script::lua::Arguments&, sfc::script::lua::ReturnValues&, void*) {}

// 0x37f5fc LuaScript::_GetPyOID(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// partial: waits on dh2/sfc/script/lua/Arguments.hpp, ReturnValues.hpp and Value.hpp (none in this tree).
// The instance is dword_99F75C = s_inst +0x30 = Application::m_pyDataArrays (dh2/Application.hpp), reached through
// ApplicationGetInstance() (dh2/global/ctrl_camera.hpp).
// Hex-Rays: when args has at least two values and both are strings, push as an integer
// PyDataArrays::GetOID(instance, args[0], args[1]). The body matches _GetPyStruct.
void LuaScript::_GetPyOID(const sfc::script::lua::Arguments&, sfc::script::lua::ReturnValues&, void*) {}

// 0x37f750 LuaScript::_BitXOr(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// partial: waits on dh2/sfc/script/lua/Arguments.hpp, ReturnValues.hpp and Value.hpp (none in this tree).
// Hex-Rays: when args has exactly two values and both are numbers, push BitXOr(args[0], args[1]) as an integer.
void LuaScript::_BitXOr(const sfc::script::lua::Arguments&, sfc::script::lua::ReturnValues&, void*) {}

// 0x37f814 LuaScript::_BitNot(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// partial: waits on dh2/sfc/script/lua/Arguments.hpp, ReturnValues.hpp and Value.hpp (none in this tree).
// Hex-Rays: when args has exactly one value and it is a number, push BitNot(args[0]) as an integer.
void LuaScript::_BitNot(const sfc::script::lua::Arguments&, sfc::script::lua::ReturnValues&, void*) {}

// 0x37f878 LuaScript::_SetGameType(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// partial: waits on dh2/sfc/script/lua/Arguments.hpp and Value.hpp (none in this tree). The store needs a layout
// request: Level +0x150 must be an int game type, but Level.hpp declares it as void* unk_150 (asm: STR R4,[R5,#0x150]).
// Hex-Rays: when args is not empty and args[0] is a number, gt = FloatToUInt32(args[0]). When IsValidGameType(gt)
// holds, level = Application::GetCurrentLevel(). If level is non-null, value = FloatToUInt32(args[0]) again (the
// same argument). If value is valid, store it as the level's game type at +0x150. Otherwise, when gAssertLevel == 2,
// the original writes through a null pointer to crash (std::abort() here). When gAssertLevel == 1 it prints
// "ASSERT(gt < Constants::GameTypes::COUNT) FAILED: <Level.h>:494" to stderr. In every other case the out-of-range
// value is stored. The second read cannot differ from the first, so the out-of-range path is unreachable.
void LuaScript::_SetGameType(const sfc::script::lua::Arguments&, sfc::script::lua::ReturnValues&, void*) {}
