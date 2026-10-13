// Batch g1-game-app-LuaScript-5015a8-1 (LuaScript, part 2 of 3), redo r2a-game-app-LuaScript-5015a8-1.
// Declarations are in dh2/LuaScript.hpp.
//
// Binding argument order. The Hex-Rays listing names the first parameter `this` and types the third as
// ReturnValues, but the code uses them as: the first is the argument list (asm: LDR R5, [R0,#4] reads the
// Value storage; ReturnValues::pushInteger is called with R1), the second is the ReturnValues that receives
// the result (MOV R6, R1 ... MOV R0, R6 before pushInteger), and the third is the LuaScript (a3+0x34 is
// m_vfTable). The header's static signature (args, ret, self) follows that use.
//
// Status of the bindings whose bodies are below (redo): reconstructed: _GetInt, _ToFixed, _BitOr, _BitAnd,
// _AddToVFTable. partial: _PlayMusic, _PlaySound, _StopSound, because VoxSoundManager declares none of the calls
// they make in this tree. The other bindings in this file belong to other batches and keep their own notes.
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <limits>

#include "dh2/Application.hpp"
#include "dh2/Level.hpp"
#include "dh2/LuaScript.hpp"
// PlayerManager pulls in net-transport, whose deps/byte_array.hpp also defines CustomFree, which pydata's stream_reader.hpp
// defines too (the same workaround as dh2/Character.hpp). The renames cover only the net-transport include.
#define CustomFree CustomFree_net_transport
#define MemoryHintState MemoryHintState_net_transport
#include "dh2/PlayerManager.hpp"
#undef CustomFree
#undef MemoryHintState
#include "dh2/Random.hpp"
#include "dh2/global/IsMultiplayerGame.hpp"  // dword_99F76C: the PlayerManager global
#include "dh2/sfc/script/lua/Arguments.hpp"
#include "dh2/sfc/script/lua/ReturnValues.hpp"
#include "dh2/sfc/script/lua/Value.hpp"

#include "glitch-core-io/core/strings.hpp"
#include "pydata-accessors/member_names.hpp"

namespace {

// Value::m_type tags (Value.hpp): 3 is a number, 4 is a string.
constexpr int kLuaTypeNumber = 3;
constexpr int kLuaTypeString = 4;

// Float-to-int32 as the ARM target converts it: out-of-range values saturate and NaN gives 0. The same rule is
// luascript_g1::FloatToInt32 in g1-game-app-LuaScript-5015a8-2.cpp.
int ToInt32(float value) {
    if (std::isnan(value)) return 0;
    if (value >= 2147483648.0f) return std::numeric_limits<int>::max();
    if (value <= -2147483648.0f) return std::numeric_limits<int>::min();
    return static_cast<int>(value);
}

// Arrays::Sounds id of the name in args[0] (-1 when absent). The sound bindings start with this lookup.
int SoundIdOf(const sfc::script::lua::Arguments& args) {
    return Arrays::GetMemberIDByString<Arrays::Sounds>(args[0].getString());
}

// Number of values in an argument list: the original's (end - begin) / 112 over std::vector<Value>.
std::size_t ArgCount(const sfc::script::lua::Arguments& args) {
    return args.m_values->size();
}

}  // namespace

// 0x37d990 LuaScript::SetInt(char const*, int)
// The original hashes the name with glitch::core::hashString, finds or inserts the entry keyed by that hash in
// m_ints, and stores the value. operator[] does the same insert-or-find. The pseudocode's unsigned return value
// (the hash, or the result of the rb-tree insert) is not part of the declared void signature, so none is returned.
void LuaScript::SetInt(const char* name, int value) {
    const unsigned int key = glitch::core::hashString(name);
    m_ints[key] = value;
}

// 0x37da30 LuaScript::GetInt(char const*) const
// Looks the hash up in m_ints and returns the stored value. An absent name is inserted with value 0 and 0 is
// returned: the original calls LuaScript::SetInt(this, name, 0) on that path, which is the same insert. The map
// is mutable so the const member can perform that insert.
int LuaScript::GetInt(const char* name) const {
    const unsigned int key = glitch::core::hashString(name);
    const auto it = m_ints.find(key);
    if (it != m_ints.end()) {
        return it->second;
    }
    m_ints[key] = 0;
    return 0;
}

// 0x37dc44 LuaScript::_PopVFTable(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// Restores the overrides that _AddToVFTable replaced while m_vfTableCollecting was set. The saved map (+0x4c) is
// walked in key order. A non-empty saved string is copied back into m_vfTable (operator[] then assign). An empty
// one takes the erase branch: the original tests the length with memcmp and then `|| length`, so an empty string
// removes the key from m_vfTable if it is present. Afterwards the saved map is emptied (the original erases the
// tree and resets its header, but only when the node count is non-zero, which clear() reproduces), and the
// collecting flag is cleared. Only the LuaScript is used: the argument list and the return value are not read.
void LuaScript::_PopVFTable(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/,
                            void* self) {
    LuaScript* script = static_cast<LuaScript*>(self);
    for (const auto& saved : script->m_vfTableSaved) {
        if (!saved.second.empty()) {
            script->m_vfTable[saved.first] = saved.second;
        } else {
            script->m_vfTable.erase(saved.first);
        }
    }
    script->m_vfTableSaved.clear();
    script->m_vfTableCollecting = false;
}

// 0x37ee80 LuaScript::_Trace(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// The original is a bare return (asm: BX LR, 4 bytes). The Lua Trace binding does nothing.
void LuaScript::_Trace([[maybe_unused]] const sfc::script::lua::Arguments& args,
                       [[maybe_unused]] sfc::script::lua::ReturnValues& ret, [[maybe_unused]] void* self) {}

// 0x37cbd8 LuaScript::_GetNumPlayers(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// Pushes one integer: PlayerManager::m_numCharacters (+0x6c4, Hex-Rays *(dword_99F76C + 1732)) of the PlayerManager
// global. It does not call PlayerManager::GetNumPlayers.
void LuaScript::_GetNumPlayers(const sfc::script::lua::Arguments&, sfc::script::lua::ReturnValues& ret, void*) {
    ret.pushInteger(dword_99F76C->m_numCharacters);
}

// 0x37cc00 LuaScript::_GetHostPlayerLevel(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// Pushes the integer at PlayerInfo +0x330 of the hosting player (Hex-Rays *((_DWORD*)GetHostingPlayer(dword_99F76C) + 204)).
// That offset is the value of PlayerInfo::m_characterLevel (block at +0x310, value at block +0x20), read with GetValue().
void LuaScript::_GetHostPlayerLevel(const sfc::script::lua::Arguments&, sfc::script::lua::ReturnValues& ret, void*) {
    ret.pushInteger(dword_99F76C->GetHostingPlayer()->m_characterLevel.GetValue());
}

// 0x37ddc8 LuaScript::_IsPlayerCharacter(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// Pushes whether the local player's Character is the pointer in the first argument. The local player is read first
// (PlayerManager::GetLocalPlayer(0, true), then its Character at PlayerInfo +0x660, PlayerInfo::unk_660), and args[0]
// after it, so an empty argument list throws std::out_of_range from Arguments::operator[], as the original does.
void LuaScript::_IsPlayerCharacter(const sfc::script::lua::Arguments& args, sfc::script::lua::ReturnValues& ret, void*) {
    PlayerInfo* const localPlayer = dword_99F76C->GetLocalPlayer(0, true);
    Character* const character = localPlayer->unk_660;
    ret.pushBoolean(character == args[0].getPointer());
}

// 0x37de5c LuaScript::_SetInt(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// With more than one argument, calls SetInt on the LuaScript passed as the third parameter, with name =
// args[0] as a string and value = (int)args[1]. With fewer, nothing happens. The Hex-Rays prototype returns a Value
// pointer (args[0] on the early path); the header declares the binding void, so no value is returned.
void LuaScript::_SetInt(const sfc::script::lua::Arguments& args, sfc::script::lua::ReturnValues&, void* self) {
    if (ArgCount(args) > 1) {
        const char* const name = args[0].getString();
        const int value = ToInt32(args[1].getNumber());
        static_cast<LuaScript*>(self)->SetInt(name, value);
    }
}

// 0x37df30 LuaScript::_Rand(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// With more than one argument: min = (int)args[0], max = (int)args[1], then pushes Random::GetRandom(max - min, false)
// + min as an integer. The GetRandom call is the ordinary-stream clone at 0x37bc2c, so the synced flag is false. The
// original also calls Arguments::operator[](1) with its result discarded when args[0] is a number (asm: MOV R1,#1,
// BL operator[]). That call can only throw when the list is too short, which the count check above excludes, so it
// has no effect and is not repeated. The integer arithmetic wraps as on the 32-bit target.
void LuaScript::_Rand(const sfc::script::lua::Arguments& args, sfc::script::lua::ReturnValues& ret, void*) {
    if (ArgCount(args) > 1) {
        const int min = ToInt32(args[0].getNumber());
        const int max = ToInt32(args[1].getNumber());
        const int span = static_cast<int>(static_cast<std::uint32_t>(max) - static_cast<std::uint32_t>(min));
        const std::uint32_t draw = Random::GetRandom(span, false);
        ret.pushInteger(static_cast<int>(draw + static_cast<std::uint32_t>(min)));
    }
}

// 0x37e068 LuaScript::_DivFixed(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. With more than one argument: a = (int)args[0], b = (int)args[1] >> 8, then pushes a / b as an integer. The
// divide is __aeabi_idiv (asm), not an SDIV instruction, so a zero divisor goes to the runtime's __aeabi_idiv0, whose
// result is not recovered. For b == 0 nothing is pushed here (the missing result). INT_MIN / -1 wraps as on the target.
void LuaScript::_DivFixed(const sfc::script::lua::Arguments& args, sfc::script::lua::ReturnValues& ret, void*) {
    if (ArgCount(args) > 1) {
        const int a = ToInt32(args[0].getNumber());
        const int b = ToInt32(args[1].getNumber()) >> 8;
        if (b != 0) {
            ret.pushInteger(static_cast<int>(static_cast<std::int64_t>(a) / b));
        }
        // PARTIAL: for b == 0 the __aeabi_idiv0 result is not pushed (not recovered).
    }
}

// 0x37e1a4 LuaScript::_MulFixed(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// With more than one argument: a = (int)args[0], b = (int)args[1], then pushes (a * b) >> 8 as an integer. The product
// is a 32-bit wrapping multiply and the shift is arithmetic.
void LuaScript::_MulFixed(const sfc::script::lua::Arguments& args, sfc::script::lua::ReturnValues& ret, void*) {
    if (ArgCount(args) > 1) {
        const int a = ToInt32(args[0].getNumber());
        const int b = ToInt32(args[1].getNumber());
        const int product = static_cast<int>(static_cast<std::uint32_t>(a) * static_cast<std::uint32_t>(b));
        ret.pushInteger(product >> 8);
    }
}

// 0x37e2d8 LuaScript::_RandF(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// With more than one argument: lo = args[0] and hi = args[1] as floats, then pushes lo + (float)(int)GetRandom(
// (int)(hi - lo), false) as a number. The GetRandom call is the ordinary-stream clone at 0x37bc2c, as in _Rand.
void LuaScript::_RandF(const sfc::script::lua::Arguments& args, sfc::script::lua::ReturnValues& ret, void*) {
    if (ArgCount(args) > 1) {
        const float lo = args[0].getNumber();
        const float hi = args[1].getNumber();
        const int span = ToInt32(hi - lo);
        const int draw = static_cast<int>(Random::GetRandom(span, false));
        ret.pushNumber(lo + static_cast<float>(draw));
    }
}

// 0x37e420 LuaScript::_PlayMusic(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL (redo). Written: the sound id is Arrays::GetMemberIDByString<Arrays::Sounds>(args[0] string), and the
// binding returns when it is -1. args[1] is read as the number the original passes on (it throws std::out_of_range
// when there is no second argument, as the original does). The current level comes from
// Application::GetCurrentLevel, and its music id (Level +0x11c, m_musicSoundId) is compared with the id.
// Missing: VoxSoundManager::PlayMusic(s_instance, id, 1, 0, volume), and when the ids match, SetMusicState("ambient")
// or SetMusicState("combat") chosen by the byte at VoxSoundManager +0x31. vox-audio/include/vox-audio/vox/sound_manager.hpp
// declares only SetIPodShuffle, UpdateIPodAsync and ConvertVisual3DToSound3D, so there is no s_instance, PlayMusic or
// SetMusicState to call (layout request: VoxSoundManager declarations and the +0x31 byte).
void LuaScript::_PlayMusic(const sfc::script::lua::Arguments& args, sfc::script::lua::ReturnValues& /*ret*/,
                           void* /*self*/) {
    const int soundId = SoundIdOf(args);
    if (soundId == -1) {
        return;
    }
    [[maybe_unused]] const int volume = ToInt32(args[1].getNumber());
    // PARTIAL: VoxSoundManager::PlayMusic(VoxSoundManager::s_instance, soundId, 1, 0, volume) is not written.
    const Level* level = ApplicationGetInstance().GetCurrentLevel();
    if (level != nullptr && soundId == level->m_musicSoundId) {
        // PARTIAL: VoxSoundManager::SetMusicState("ambient" or "combat") is not written (see above).
    }
}

// 0x37e578 LuaScript::_PlaySound(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL (redo). Written: args[3] is read first, which throws std::out_of_range when there are fewer than four
// arguments, as the original does. The sound id is Arrays::GetMemberIDByString<Arrays::Sounds>(args[0] string). When
// it is not -1, args[1] is read as a bool and args[2] as a number: the two values the original passes to Play.
// Missing: VoxSoundManager::StopMusic(s_instance, false), which runs when args[3] is true, and
// VoxSoundManager::Play(s_instance, id, flag, value, 0, 0). The VoxSoundManager declarations are absent (see 0x37e420).
void LuaScript::_PlaySound(const sfc::script::lua::Arguments& args, sfc::script::lua::ReturnValues& /*ret*/,
                           void* /*self*/) {
    if (args[3].getBool()) {
        // PARTIAL: VoxSoundManager::StopMusic(VoxSoundManager::s_instance, false) is not written.
    }
    const int soundId = SoundIdOf(args);
    if (soundId == -1) {
        return;
    }
    [[maybe_unused]] const bool flag = args[1].getBool();
    [[maybe_unused]] const int value = ToInt32(args[2].getNumber());
    // PARTIAL: VoxSoundManager::Play(VoxSoundManager::s_instance, soundId, flag, value, 0, 0) is not written.
}

// 0x37e730 LuaScript::_StopSound(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL (redo). Written: the sound id is Arrays::GetMemberIDByString<Arrays::Sounds>(args[0] string). When it is not
// -1, args[1] is read as a number, and the original passes its int value on to Stop.
// Missing: VoxSoundManager::Stop(s_instance, id, value, a5, a6, a7). The Hex-Rays prototype passes three stack arguments
// that are not recovered, and the VoxSoundManager::Stop declaration is absent (see 0x37e420).
void LuaScript::_StopSound(const sfc::script::lua::Arguments& args, sfc::script::lua::ReturnValues& /*ret*/,
                           void* /*self*/) {
    const int soundId = SoundIdOf(args);
    if (soundId == -1) {
        return;
    }
    [[maybe_unused]] const int value = ToInt32(args[1].getNumber());
    // PARTIAL: VoxSoundManager::Stop(VoxSoundManager::s_instance, soundId, value, ...) is not written.
}

// 0x37e814 LuaScript::_BitOr(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// Needs two or more arguments, all of them numbers (Value::m_type 3). The original leaves its loop at the first
// non-number, and with fewer than two arguments it never enters the loop, so nothing is pushed in those cases. When
// every argument is a number, the result is the OR of (int)getNumber over all of them, pushed with pushInteger.
void LuaScript::_BitOr(const sfc::script::lua::Arguments& args, sfc::script::lua::ReturnValues& ret,
                       void* /*self*/) {
    const unsigned int count = static_cast<unsigned int>(args.m_values->size());
    if (count < 2) {
        return;
    }
    for (unsigned int i = 0; i < count; ++i) {
        if (args[i].m_type != kLuaTypeNumber) {
            return;
        }
    }
    int result = ToInt32(args[0].getNumber());
    for (unsigned int i = 1; i < count; ++i) {
        result |= ToInt32(args[i].getNumber());
    }
    ret.pushInteger(result);
}

// 0x37e9ec LuaScript::_BitAnd(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// Same argument rules as _BitOr (two or more arguments, all numbers, otherwise nothing is pushed). The result is the
// AND of (int)getNumber over all of them, pushed with pushInteger.
void LuaScript::_BitAnd(const sfc::script::lua::Arguments& args, sfc::script::lua::ReturnValues& ret,
                        void* /*self*/) {
    const unsigned int count = static_cast<unsigned int>(args.m_values->size());
    if (count < 2) {
        return;
    }
    for (unsigned int i = 0; i < count; ++i) {
        if (args[i].m_type != kLuaTypeNumber) {
            return;
        }
    }
    int result = ToInt32(args[0].getNumber());
    for (unsigned int i = 1; i < count; ++i) {
        result &= ToInt32(args[i].getNumber());
    }
    ret.pushInteger(result);
}

// 0x37ebc4 LuaScript::_ToFixed(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// With one or more arguments, pushes (int)getNumber(args[0]) << 8. The shift is done on the unsigned value, which gives
// the wrapping result the ARM code produces (a left shift of a negative int is undefined in C++17).
void LuaScript::_ToFixed(const sfc::script::lua::Arguments& args, sfc::script::lua::ReturnValues& ret,
                         void* /*self*/) {
    if (args.m_values->empty()) {
        return;
    }
    const int value = ToInt32(args[0].getNumber());
    ret.pushInteger(static_cast<int>(static_cast<unsigned int>(value) << 8));
}

// 0x37ec14 LuaScript::_GetInt(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// With one or more arguments, pushes the script-side integer named by args[0]: LuaScript::GetInt on the self object
// (the member is reconstructed above). Nothing is pushed without an argument.
void LuaScript::_GetInt(const sfc::script::lua::Arguments& args, sfc::script::lua::ReturnValues& ret, void* self) {
    if (args.m_values->empty()) {
        return;
    }
    const LuaScript* script = static_cast<const LuaScript*>(self);
    ret.pushInteger(script->GetInt(args[0].getString()));
}

// 0x37ec70 LuaScript::_AddToVFTable(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// Needs two arguments, both strings (Value::m_type 4), otherwise it returns. key = glitch::core::hashString(args[0]).
// When m_vfTableCollecting is set and the key is not yet in m_vfTableSaved, the current override is saved first:
// m_vfTableSaved[key] = m_vfTable[key] (operator[] inserts an empty string for an absent key). Then
// m_vfTable[key] = args[1]. _PopVFTable above restores the saved entries.
void LuaScript::_AddToVFTable(const sfc::script::lua::Arguments& args, sfc::script::lua::ReturnValues& /*ret*/,
                              void* self) {
    if (args.m_values->size() <= 1 || args[0].m_type != kLuaTypeString || args[1].m_type != kLuaTypeString) {
        return;
    }
    LuaScript* script = static_cast<LuaScript*>(self);
    const unsigned int key = glitch::core::hashString(args[0].getString());
    if (script->m_vfTableCollecting && script->m_vfTableSaved.find(key) == script->m_vfTableSaved.end()) {
        script->m_vfTableSaved[key] = script->m_vfTable[key];
    }
    script->m_vfTable[key] = args[1].getString();
}
