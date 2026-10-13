// Batch g1-game-app-LuaScript-5015a8-1 (LuaScript, part 2 of 3).
// Declarations are in dh2/LuaScript.hpp.
//
// Binding argument order. The Hex-Rays listing names the first parameter `this` and types the third as
// ReturnValues, but the code uses them as: the first is the argument list (asm: LDR R5, [R0,#4] reads the
// Value storage; ReturnValues::pushInteger is called with R1), the second is the ReturnValues that receives
// the result (MOV R6, R1 ... MOV R0, R6 before pushInteger), and the third is the LuaScript (a3+0x34 is
// m_vfTable). The header's static signature (args, ret, self) follows that use.
//
// Status of this batch:
//   reconstructed: SetInt, GetInt (member), _PopVFTable, _Trace.
//   partial: every binding that reads a Lua argument or pushes a result. sfc::script::lua::Arguments, Value
//     and ReturnValues have no headers in this tree (LuaScript.hpp only forward-declares them), so those
//     bodies are not written. Each partial note lists what the original does and what is missing.
#include "dh2/LuaScript.hpp"

#include "glitch-core-io/core/strings.hpp"

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
// PARTIAL. Not written. The original pushes one integer: the PlayerManager at the global dword_99F76C, field
// +0x6c4 (Hex-Rays *(dword_99F76C + 1732)), which is PlayerManager::m_numCharacters. It does not call
// PlayerManager::GetNumPlayers. Missing: ReturnValues::pushInteger (no header in this tree). The global
// dword_99F76C is declared in dh2/global/IsMultiplayerGame.hpp and is not used here yet.
void LuaScript::_GetNumPlayers([[maybe_unused]] const sfc::script::lua::Arguments& args,
                               [[maybe_unused]] sfc::script::lua::ReturnValues& ret, [[maybe_unused]] void* self) {}

// 0x37cc00 LuaScript::_GetHostPlayerLevel(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. Not written. The original pushes the integer at +0x330 of the hosting player:
// *((_DWORD*)PlayerManager::GetHostingPlayer(dword_99F76C) + 204). Missing: the PlayerInfo layout (dh2/PlayerInfo.hpp
// does not exist; layout request for +0x330 int) and ReturnValues::pushInteger.
void LuaScript::_GetHostPlayerLevel([[maybe_unused]] const sfc::script::lua::Arguments& args,
                                    [[maybe_unused]] sfc::script::lua::ReturnValues& ret, [[maybe_unused]] void* self) {}

// 0x37ddc8 LuaScript::_IsPlayerCharacter(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. Not written. The original compares the local player's character with the pointer in the first argument:
// PlayerManager::GetLocalPlayer(dword_99F76C, 0, true)->character_660 (PlayerInfo +0x660) == Value::getPointer(args[0]),
// then pushBoolean(ret, result). Missing: dh2/PlayerInfo.hpp (character_660 at +0x660), the Arguments and Value
// accessors (Arguments::operator[], Value::getPointer) and ReturnValues::pushBoolean.
void LuaScript::_IsPlayerCharacter([[maybe_unused]] const sfc::script::lua::Arguments& args,
                                   [[maybe_unused]] sfc::script::lua::ReturnValues& ret, [[maybe_unused]] void* self) {}

// 0x37de5c LuaScript::_SetInt(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. Not written. With more than one argument, the original calls SetInt on the LuaScript (third parameter)
// with name = Value::getString(args[0]) and value = (int)Value::getNumber(args[1]). With fewer, nothing happens.
// Missing: the Arguments and Value accessors. The Hex-Rays prototype returns a Value pointer (args[0] on the early
// path); the header declares the binding void, so no value is returned.
void LuaScript::_SetInt([[maybe_unused]] const sfc::script::lua::Arguments& args,
                        [[maybe_unused]] sfc::script::lua::ReturnValues& ret, [[maybe_unused]] void* self) {}

// 0x37df30 LuaScript::_Rand(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. Not written. With more than one argument: min = (int)Value::getNumber(args[0]),
// max = (int)Value::getNumber(args[1]), then pushInteger(ret, Random::GetRandom(max - min, false) + min).
// The GetRandom call is the clone at 0x37bc2c, which advances Random::s_seed (the ordinary stream) and counts the
// ordinary draw, so the synchronized flag is false. The original also calls Arguments::operator[](1) with its
// result discarded when args[0] is a number (asm: MOV R1,#1, BL operator[]); its effect is not recovered.
// Missing: the Arguments and Value accessors, ReturnValues::pushInteger.
void LuaScript::_Rand([[maybe_unused]] const sfc::script::lua::Arguments& args,
                      [[maybe_unused]] sfc::script::lua::ReturnValues& ret, [[maybe_unused]] void* self) {}

// 0x37e068 LuaScript::_DivFixed(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. Not written. With more than one argument: a = (int)Value::getNumber(args[0]),
// b = (int)Value::getNumber(args[1]), then pushInteger(ret, a / (b >> 8)). The divide is __aeabi_idiv (asm), not an
// SDIV instruction, so the divisor-zero result comes from the runtime's __aeabi_idiv0, which is not recovered.
// Missing: the Arguments and Value accessors, ReturnValues::pushInteger, the zero-divisor result.
void LuaScript::_DivFixed([[maybe_unused]] const sfc::script::lua::Arguments& args,
                          [[maybe_unused]] sfc::script::lua::ReturnValues& ret, [[maybe_unused]] void* self) {}

// 0x37e1a4 LuaScript::_MulFixed(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. Not written. With more than one argument: a = (int)Value::getNumber(args[0]),
// b = (int)Value::getNumber(args[1]), then pushInteger(ret, (a * b) >> 8) with a 32-bit product (wrapping) and an
// arithmetic shift. Missing: the Arguments and Value accessors, ReturnValues::pushInteger.
void LuaScript::_MulFixed([[maybe_unused]] const sfc::script::lua::Arguments& args,
                          [[maybe_unused]] sfc::script::lua::ReturnValues& ret, [[maybe_unused]] void* self) {}

// 0x37e2d8 LuaScript::_RandF(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. Not written. With more than one argument: lo = Value::getNumber(args[0]) (float),
// hi = Value::getNumber(args[1]) (float), then pushNumber(ret, lo + (float)(int)Random::GetRandom((int)(hi - lo), false)).
// The GetRandom call is the same ordinary-stream clone as _Rand (0x37bc2c). Missing: the Arguments and Value
// accessors, ReturnValues::pushNumber.
void LuaScript::_RandF([[maybe_unused]] const sfc::script::lua::Arguments& args,
                       [[maybe_unused]] sfc::script::lua::ReturnValues& ret, [[maybe_unused]] void* self) {}

// 0x37e420 LuaScript::_PlayMusic(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. Not written. id = Arrays::GetMemberIDByString<Arrays::Sounds>(Value::getString(args[0])) (declared in
// pydata-accessors/member_names.hpp). When id != -1: VoxSoundManager::PlayMusic(s_instance, id, 1, 0,
// (int)Value::getNumber(args[1])). Then level = Application::GetCurrentLevel(Singleton<Application>::s_inst); when
// the level is non-null and id == level +0x11c (Level::m_musicSoundId), the music state is set to "ambient" when the
// VoxSoundManager byte at +0x31 is set and to "combat" otherwise. Missing: the Arguments and Value accessors;
// VoxSoundManager declarations for s_instance, PlayMusic and SetMusicState, and the byte at +0x31 (layout
// request); a declaration for Singleton<Application>::s_inst (dword_99F72C), which no header declares yet.
void LuaScript::_PlayMusic([[maybe_unused]] const sfc::script::lua::Arguments& args,
                           [[maybe_unused]] sfc::script::lua::ReturnValues& ret, [[maybe_unused]] void* self) {}

// 0x37e578 LuaScript::_PlaySound(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. Not written. The original reads args[3] first and throws std::out_of_range (vector) when there are fewer
// than four. If Value::getBool(args[3]) is set, VoxSoundManager::StopMusic(s_instance, false) runs. Then
// id = Arrays::GetMemberIDByString<Arrays::Sounds>(Value::getString(args[0])); when id != -1,
// VoxSoundManager::Play(s_instance, id, Value::getBool(args[1]), (int)Value::getNumber(args[2]), 0, 0).
// Missing: the Arguments and Value accessors; VoxSoundManager declarations for s_instance, StopMusic and Play.
void LuaScript::_PlaySound([[maybe_unused]] const sfc::script::lua::Arguments& args,
                           [[maybe_unused]] sfc::script::lua::ReturnValues& ret, [[maybe_unused]] void* self) {}

// 0x37e730 LuaScript::_StopSound(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. Not written. id = Arrays::GetMemberIDByString<Arrays::Sounds>(Value::getString(args[0])); when id != -1,
// VoxSoundManager::Stop(s_instance, id, (int)Value::getNumber(args[1]), ...). The Hex-Rays prototype passes three
// more stack arguments to Stop (its a5..a7), which are not recovered. Missing: the Arguments and Value accessors;
// the VoxSoundManager::Stop declaration and its remaining parameters.
void LuaScript::_StopSound([[maybe_unused]] const sfc::script::lua::Arguments& args,
                           [[maybe_unused]] sfc::script::lua::ReturnValues& ret, [[maybe_unused]] void* self) {}

// 0x37e814 LuaScript::_BitOr(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. Not written. Needs two or more arguments. Every argument must be a number (type tag 3); the first
// non-number argument ends the function without a result. When all are numbers, the result is the OR of
// (int)Value::getNumber over all of them, pushed with pushInteger. With one argument or none, nothing is pushed.
// Missing: the Arguments and Value accessors (including the type tag at Value +4), ReturnValues::pushInteger.
void LuaScript::_BitOr([[maybe_unused]] const sfc::script::lua::Arguments& args,
                       [[maybe_unused]] sfc::script::lua::ReturnValues& ret, [[maybe_unused]] void* self) {}

// 0x37e9ec LuaScript::_BitAnd(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. Not written. Same argument rules as _BitOr (two or more numbers, otherwise no result). The result is the
// AND of (int)Value::getNumber over all arguments, pushed with pushInteger.
// Missing: the Arguments and Value accessors, ReturnValues::pushInteger.
void LuaScript::_BitAnd([[maybe_unused]] const sfc::script::lua::Arguments& args,
                        [[maybe_unused]] sfc::script::lua::ReturnValues& ret, [[maybe_unused]] void* self) {}

// 0x37ebc4 LuaScript::_ToFixed(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. Not written. With at least one argument: pushInteger(ret, (int)Value::getNumber(args[0]) << 8).
// Missing: the Arguments and Value accessors, ReturnValues::pushInteger.
void LuaScript::_ToFixed([[maybe_unused]] const sfc::script::lua::Arguments& args,
                         [[maybe_unused]] sfc::script::lua::ReturnValues& ret, [[maybe_unused]] void* self) {}

// 0x37ec14 LuaScript::_GetInt(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. Not written. With at least one argument: pushInteger(ret, LuaScript::GetInt(self, Value::getString(args[0]))).
// The member GetInt above is reconstructed. Missing: the Arguments and Value accessors, ReturnValues::pushInteger.
void LuaScript::_GetInt([[maybe_unused]] const sfc::script::lua::Arguments& args,
                        [[maybe_unused]] sfc::script::lua::ReturnValues& ret, [[maybe_unused]] void* self) {}

// 0x37ec70 LuaScript::_AddToVFTable(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. Not written. Needs two arguments of string type (tag 4 at Value +4 for args[0] and args[1]); otherwise it
// returns. key = glitch::core::hashString(Value::getString(args[0])). When self->m_vfTableCollecting is set and key is
// not in m_vfTableSaved, the current value is saved first: m_vfTableSaved[key] = m_vfTable[key] (operator[] inserts an
// empty string when the key is absent). Then m_vfTable[key] = Value::getString(args[1]). _PopVFTable above undoes the
// saved entries. Missing: the Arguments and Value accessors (Arguments::operator[], Value::getString, the type tag).
void LuaScript::_AddToVFTable([[maybe_unused]] const sfc::script::lua::Arguments& args,
                              [[maybe_unused]] sfc::script::lua::ReturnValues& ret, [[maybe_unused]] void* self) {}
