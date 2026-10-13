// Batch g1-game-character-Character-ee9946-12 (Character, part 13 of 20): twenty Lua bindings of Character.
// Declarations are in dh2/Character.hpp. Each binding is a static member with the signature
// (const Arguments& args, ReturnValues& ret, void* userData), and userData is the Character.
//
// Argument order. The Hex-Rays listing names the first parameter `this` and types the third as a Character, but the
// code uses them as: the first is the argument list (asm 0x3b8d98: LDR R3, [R0,#4] reads the Value storage), the second
// is the ReturnValues that receives the result (pushBoolean, pushInteger or pushPointer), and the third is the Character
// (userData). _HasMana calls Character::HasMana on it and _GetPropBonus* read CharProperties at +0x560. The header
// signature follows that use.
//
// Status of this batch: every binding is PARTIAL. Each one starts by reading a Lua argument (Arguments::operator[],
// Value::getNumber, getUInteger, getBool, getPointer, getString or getUserData, with the type tag at Value +4), and most
// push a result (ReturnValues::pushBoolean, pushInteger or pushPointer). sfc::script::lua::Arguments, Value and
// ReturnValues have no headers in this tree (dh2/sfc/script/lua/ holds only UserData.hpp), so the bodies are not written.
// Each comment gives what the original does (Hex-Rays, with the assembly where noted) and what is missing. The
// definitions exist so that the binder can refer to them; they do nothing until the accessors are declared. The
// missing headers are dh2/sfc/script/lua/Arguments.hpp, Value.hpp and ReturnValues.hpp (game-app category).
//
// Lua type tags used by the Hex-Rays tests on Value's type word at +4 (values from vendor-lua lua.h, LUA_T*):
// 0 nil, 1 boolean, 2 light userdata, 3 number, 4 string, 7 userdata.
//
// No test: every body depends on the Lua accessors above, and the bodies touch only engine members.
//
// Layout facts used by the comments (all declared in the owner headers, no layout request):
//   self +0x378  v2Controllable::m_controller (v2Controllable at +0x374, member at +0x04)
//   self +0x415  CharAI unk_4d (CharAI at +0x3c8, member +0x4d, ctor 1)
//   self +0x520  CharStateMachine m_stateFlags (CharStateMachine at +0x4fc, member +0x24)
//   self +0x1390 CharProperties unk_e30 (CharProperties at +0x560, member +0xe30, ctor 0)

#include "dh2/Character.hpp"

// 0x3b78ec _ZN9Character8_HasManaERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
// PARTIAL. Not written. With at least one argument and args[0] a number: cost = (int)getNumber(args[0]), then
// pushBoolean(ret, Character::HasMana(self, cost)) (HasMana(int) const at 0x3bd40c). Otherwise nothing is pushed.
// Missing: the Arguments and Value accessors, ReturnValues::pushBoolean.
void Character::_HasMana([[maybe_unused]] const sfc::script::lua::Arguments& args,
                         [[maybe_unused]] sfc::script::lua::ReturnValues& ret, [[maybe_unused]] void* userData) {}

// 0x3b7974 _ZN9Character16_AllowSkillBreakERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
// PARTIAL. Not written. With at least one argument and args[0] a boolean: allow = getBool(args[0]). The word
// m_stateFlags at self +0x520 loses bit 0x10000 when allow is true and gains it when false. The header names bits 10
// and 11 of that word only (scare and stun), so bit 0x10000 has no name yet. The binder's int return (allow) is dropped
// by the void signature. Missing: the Arguments and Value accessors.
void Character::_AllowSkillBreak([[maybe_unused]] const sfc::script::lua::Arguments& args,
                                 [[maybe_unused]] sfc::script::lua::ReturnValues& ret, [[maybe_unused]] void* userData) {}

// 0x3b79ec _ZN9Character14_AllowRotationERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
// PARTIAL. Not written. Same shape as _AllowSkillBreak: with a boolean args[0], the word m_stateFlags at self +0x520
// loses bit 0x4000 when allow is true and gains it when false. Bit 0x4000 is not named in CharStateMachine.hpp.
// Missing: the Arguments and Value accessors.
void Character::_AllowRotation([[maybe_unused]] const sfc::script::lua::Arguments& args,
                               [[maybe_unused]] sfc::script::lua::ReturnValues& ret, [[maybe_unused]] void* userData) {}

// 0x3b7a64 _ZN9Character16_SetIsTargetableERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
// PARTIAL. Not written. With at least one argument and args[0] a boolean: the byte at self +0x415 is set to
// getBool(args[0]). That byte is CharAI unk_4d (CharAI.hpp, +0x4d, ctor 1). The binding name says the flag is
// targetability, which is not confirmed by another reader. Missing: the Arguments and Value accessors.
void Character::_SetIsTargetable([[maybe_unused]] const sfc::script::lua::Arguments& args,
                                 [[maybe_unused]] sfc::script::lua::ReturnValues& ret, [[maybe_unused]] void* userData) {}

// 0x3b7acc _ZN9Character19_GetPropBonusDamageERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
// PARTIAL. Not written. With at least one argument (no type check on args[0]): pushInteger(ret,
// CharProperties::PROPS_GetBonusDamage(self, getBool(args[0]))). PROPS_GetBonusDamage(bool) const is at 0x3df8ac.
// Missing: the Arguments and Value accessors, ReturnValues::pushInteger.
void Character::_GetPropBonusDamage([[maybe_unused]] const sfc::script::lua::Arguments& args,
                                    [[maybe_unused]] sfc::script::lua::ReturnValues& ret, [[maybe_unused]] void* userData) {}

// 0x3b7b28 _ZN9Character23_GetPropBonusCritRatingERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
// PARTIAL. Not written. Same shape as _GetPropBonusDamage: pushInteger(ret, PROPS_GetBonusCritRating(getBool(args[0])))
// (0x3df7c4). Missing: the Arguments and Value accessors, ReturnValues::pushInteger.
void Character::_GetPropBonusCritRating([[maybe_unused]] const sfc::script::lua::Arguments& args,
                                        [[maybe_unused]] sfc::script::lua::ReturnValues& ret, [[maybe_unused]] void* userData) {}

// 0x3b7b84 _ZN9Character25_GetPropBonusAttackRatingERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
// PARTIAL. Not written. Same shape as _GetPropBonusDamage: pushInteger(ret, PROPS_GetBonusAttackRating(getBool(args[0])))
// (0x3df81c). Missing: the Arguments and Value accessors, ReturnValues::pushInteger.
void Character::_GetPropBonusAttackRating([[maybe_unused]] const sfc::script::lua::Arguments& args,
                                          [[maybe_unused]] sfc::script::lua::ReturnValues& ret,
                                          [[maybe_unused]] void* userData) {}

// 0x3b7be0 _ZN9Character11_RemoveDotsERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
// PARTIAL. Not written. The logic as read from the Hex-Rays listing (the element branch is not checked in the
// assembly):
//   n: when args[0] is a number and not negative, n = (int)getNumber(args[0]). A negative number, a non-number or no
//      argument gives n = the word at self +0x1390 (CharProperties unk_e30; its meaning is not recovered).
//   element: when there are at least two arguments and args[1] is a number that is not -2, element = (int)getNumber(args[1]);
//      otherwise element = -1 (any element).
//   loop: when n > 0 and the word at +0x1390 is non-zero, PROPS_RemoveDot(element) runs in a do/while loop that stops
//      after n iterations or when the word at +0x1390 reads zero.
// PROPS_RemoveDot is empty in the shipped build (CharProperties.hpp, 0x3de83c), so the word at +0x1390 is never reduced
// and the loop runs n times. Missing: the Arguments and Value accessors, the meaning of unk_e30, and the assembly check
// of the element branch.
void Character::_RemoveDots([[maybe_unused]] const sfc::script::lua::Arguments& args,
                            [[maybe_unused]] sfc::script::lua::ReturnValues& ret, [[maybe_unused]] void* userData) {}

// 0x3b7dbc _ZN9Character14_DBG_DumpPropsERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
// PARTIAL. Not written. The path is getString(args[0]) when there is an argument and args[0] is a string (tag 4), and
// 'Lua Dump' otherwise. Then CharProperties::DBG_PROPS_DumpToFile(path, true) (0x3de840). That method is empty in the
// shipped build. Missing: the Arguments and Value accessors (getString, type tag).
void Character::_DBG_DumpProps([[maybe_unused]] const sfc::script::lua::Arguments& args,
                               [[maybe_unused]] sfc::script::lua::ReturnValues& ret, [[maybe_unused]] void* userData) {}

// 0x3b7e40 _ZN9Character5_KillERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
// PARTIAL. Not written. The killer is nullptr when there is no argument or args[0] is not userdata (tag 7). Otherwise
// it is getUserData(args[0]). Then v2Controller::Cmd_Kill(killer, false) runs on the controller at self +0x378
// (v2Controllable::m_controller; Cmd_Kill(GameObject*, bool) at 0x40570c). Missing: the Arguments and Value accessors
// (getUserData, type tag).
void Character::_Kill([[maybe_unused]] const sfc::script::lua::Arguments& args,
                      [[maybe_unused]] sfc::script::lua::ReturnValues& ret, [[maybe_unused]] void* userData) {}

// 0x3b7eac _ZN9Character8_SetPropERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
// PARTIAL. Not written. With more than one argument and args[0] a number: id = getUInteger(args[0]), and nothing
// happens when id > 0xDF (223). The original also calls getUInteger on args[0] with the result discarded. Then args[1]
// must be a number, and value = (int)getNumber(args[1]). When there are more than two arguments and args[2] is a light
// userdata (tag 2), the sheet is getPointer(args[2]): PROPS_SetToSheet(id, value, sheet) runs when the sheet is non-null
// (0x3e0614), and nothing runs when it is null. Otherwise PROPS_Set(id, value) runs (0x3e07a0). Missing: the Arguments
// and Value accessors (getUInteger, getNumber, getPointer, type tags).
void Character::_SetProp([[maybe_unused]] const sfc::script::lua::Arguments& args,
                         [[maybe_unused]] sfc::script::lua::ReturnValues& ret, [[maybe_unused]] void* userData) {}

// 0x3b8098 _ZN9Character9_AddAggroERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
// PARTIAL. Not written. Needs more than one argument, args[0] a light userdata (tag 2) or userdata (tag 7), and args[1] a
// number. target = getUserData(args[0]), aggro = getNumber(args[1]) as a float, and CharAI::AI_AddAggro(&self CharAI at
// +0x3c8, target, aggro) runs (0x3d7c68). When that result is greater than 0.0, the original loads DebugSwitches and reads
// the switch 'isTracingThreatChange' with the result dropped, so nothing else happens. Missing: the Arguments and Value
// accessors (getUserData, getNumber, type tags).
void Character::_AddAggro([[maybe_unused]] const sfc::script::lua::Arguments& args,
                          [[maybe_unused]] sfc::script::lua::ReturnValues& ret, [[maybe_unused]] void* userData) {}

// 0x3b82e4 _ZN9Character11_ClearPropsERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
// PARTIAL. Not written. With at least one argument: when args[0] is a light userdata (tag 2) with a non-null pointer,
// PROPS_ResetSheet(sheet) runs and then RecalcProperties(true) (0x3def84, 0x3e0810). When args[0] is a boolean and true,
// PROPS_ResetSheet(nullptr) runs without the recalculation. Every other case does nothing, including a null light
// userdata. Missing: the Arguments and Value accessors (getPointer, getBool, type tags).
void Character::_ClearProps([[maybe_unused]] const sfc::script::lua::Arguments& args,
                            [[maybe_unused]] sfc::script::lua::ReturnValues& ret, [[maybe_unused]] void* userData) {}

// 0x3b842c _ZN9Character11_RemoveBuffERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
// PARTIAL. Not written. With args[0] a number: id = getUInteger(args[0]), and nothing happens unless id is below
// Arrays::ClassTable::size. With a single argument, PROPS_DelBuff(id, nullptr) runs. With two or more arguments and
// args[1] a light userdata (tag 2), PROPS_DelBuff(id, getPointer(args[1])) runs. Other cases do nothing (0x3e101c).
// Missing: the Arguments and Value accessors, and the Arrays::ClassTable size (pydata-accessors).
void Character::_RemoveBuff([[maybe_unused]] const sfc::script::lua::Arguments& args,
                            [[maybe_unused]] sfc::script::lua::ReturnValues& ret, [[maybe_unused]] void* userData) {}

// 0x3b856c _ZN9Character18_GetSkillIDFromOIDERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
// PARTIAL. Not written. Needs at least one argument (checked in the assembly, 0x3b856c). When args[0] is a number,
// oid = (unsigned)getNumber(args[0]) and there is no range check. For any other type, oid = (unsigned)getNumber(args[0])
// and the binding returns without a result when Arrays::SkillTable::size <= oid. Otherwise pushInteger(ret,
// GetSkillIDFromOID((int)oid)) runs (0x3bc62c). Missing: the Arguments and Value accessors, ReturnValues::pushInteger.
void Character::_GetSkillIDFromOID([[maybe_unused]] const sfc::script::lua::Arguments& args,
                                   [[maybe_unused]] sfc::script::lua::ReturnValues& ret, [[maybe_unused]] void* userData) {}

// 0x3b8648 _ZN9Character11_ClearAggroERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
// PARTIAL. Not written. With at least one argument and args[0] a light userdata (tag 2) or userdata (tag 7):
// CharAI::AI_ClearAggro(&self CharAI at +0x3c8, getUserData(args[0])) runs (0x3d6d68). Otherwise nothing.
// Missing: the Arguments and Value accessors (getUserData, type tags).
void Character::_ClearAggro([[maybe_unused]] const sfc::script::lua::Arguments& args,
                            [[maybe_unused]] sfc::script::lua::ReturnValues& ret, [[maybe_unused]] void* userData) {}

// 0x3b86a8 _ZN9Character11_CreateBuffERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
// PARTIAL. Not written. Argument decoding from the Hex-Rays listing (the optional-argument chain is not checked in the
// assembly):
//   args[0] must be a number, and getUInteger(args[0]) must be below Arrays::ClassTable::size. classId = (int)getNumber(args[0]).
//   args[1] (durationMs): a number gives getUInteger(args[1]); nil or missing gives 0.
//   args[2] (the third PROPS_AddBuff argument, named maxStacks in the header): nil or missing gives 1; a boolean gives
//      !getBool(args[2]); any other gives (int)getNumber(args[2]).
//   args[3] (level): a number gives getUInteger(args[3]); otherwise 0.
//   args[4] (fxId): nil or missing gives -1; a number, or any other non-nil value whose getUInteger is below
//      Arrays::AnimatedEffectTable::size, gives (int)getNumber(args[4]); otherwise -1.
//   args[5] (name): a string gives getString(args[5]); otherwise ''. Missing when there are fewer than six arguments.
// Then PROPS_AddBuff(classId, durationMs, args2, level, fxId, name) runs (0x3e232c), and pushPointer(ret, result) runs when
// the result is non-null. Missing: the Arguments and Value accessors, ReturnValues::pushPointer, the Arrays tables'
// size words (pydata-accessors), and the assembly check of the optional chain.
void Character::_CreateBuff([[maybe_unused]] const sfc::script::lua::Arguments& args,
                            [[maybe_unused]] sfc::script::lua::ReturnValues& ret, [[maybe_unused]] void* userData) {}

// 0x3b8bd8 _ZN9Character8_DoSkillERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
// PARTIAL. Not written. With args[0] a number: skill = getUInteger(args[0]), and when skill is below the count of the
// skill list (GetCharSkillList(), its count is the word at +4 of the returned object, Hex-Rays *(list + 4)), then
// CharAI::AI_UseSkill(&self CharAI at +0x3c8, (unsigned)getNumber(args[0])) runs (0x3d8868). Otherwise nothing.
// Missing: the Arguments and Value accessors, and the skill list object's count word (GetCharSkillList is declared
// const void* in dh2/Character.hpp).
void Character::_DoSkill([[maybe_unused]] const sfc::script::lua::Arguments& args,
                         [[maybe_unused]] sfc::script::lua::ReturnValues& ret, [[maybe_unused]] void* userData) {}

// 0x3b8cb8 _ZN9Character11_BeginSkillERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
// PARTIAL. Not written. Same argument test as _DoSkill: with args[0] a number and skill = getUInteger(args[0]) below the
// count of GetCharSkillList(), then CharAI::AI_BeginSkill(&self CharAI at +0x3c8, skill) runs (0x3d86bc). The Hex-Rays
// listing types the value as a Character pointer, but AI_BeginSkill takes the skill index as unsigned int. Missing: the
// Arguments and Value accessors, and the skill list count word.
void Character::_BeginSkill([[maybe_unused]] const sfc::script::lua::Arguments& args,
                            [[maybe_unused]] sfc::script::lua::ReturnValues& ret, [[maybe_unused]] void* userData) {}

// 0x3b8d98 _ZN9Character11_WarpBehindERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
// PARTIAL. Not written. Checked in the assembly (0x3b8d98). With at least one argument and args[0] a light userdata
// (tag 2) or userdata (tag 7), target = getUserData(args[0]). When target is non-null:
//   look = GameObject::GetLookAtVec(target); the three components are negated; dir = Point3D::normalize() of the negated look.
//   offset = dir * 100.0f on each component (the constant 0x42C80000 is 100.0f).
//   pos = GameObject::GetTargetPosition(target) + offset (three float additions in the assembly).
//   v2Controller::Cmd_WarpTo(pos) runs on the controller at self +0x378 (0x405318).
// The Hex-Rays listing drops the addition and passes no position, so the assembly is the source for the sum.
// Missing: the Arguments and Value accessors (getUserData, type tags).
void Character::_WarpBehind([[maybe_unused]] const sfc::script::lua::Arguments& args,
                            [[maybe_unused]] sfc::script::lua::ReturnValues& ret, [[maybe_unused]] void* userData) {}
