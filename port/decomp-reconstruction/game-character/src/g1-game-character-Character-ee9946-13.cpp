// Batch g1-game-character-Character-ee9946-13 (Character, part 14 of 20): twenty Lua bindings of Character.
// Declarations are in dh2/Character.hpp, which this batch does not change. Each binding has the static signature
// (const Arguments& args, ReturnValues& ret, void* userData), and userData is the Character. The Hex-Rays listings name
// the first pointer `this` (it is the argument list: Arguments +4 holds the Value vector), the second `a2` (the
// ReturnValues that the push calls take) and the third `a3` (the Character: the controller is read at +0x378 from it).
//
// Status: every binding is PARTIAL, and none is written. Each one reads Lua arguments (Arguments size and operator[],
// the Value type word at +4, getBool, getNumber, getUInteger, getPointer, getUserData) and most push a result
// (ReturnValues::pushInteger, pushBoolean, pushPointer). The sfc::script::lua classes Arguments, Value and ReturnValues
// have no declarations in the tree (dh2/sfc/script/lua/ holds only UserData.hpp), so the bodies cannot be compiled yet.
// The definitions exist so that the binder can name them (batch -12 does the same). Each comment gives the control flow
// from the listing, with the assembly where it was checked, and what is missing.
//
// Lua type tags, Value +4 (vendor-lua LUA_T*): 0 nil, 1 boolean, 2 light userdata, 3 number, 4 string, 7 userdata.
// Argument i sits at byte offset 112 * i of the Arguments vector (32-bit stride; the listing's +116 word is args[1]).
//
// Layout facts used (32-bit offsets, Character-relative; all declared in the owner headers except where noted):
//   +0x160 GameObject m_position              +0x378 v2Controllable m_controller
//   +0x37c ItemInventory (base)               +0x3c8 CharAI (base)
//   +0x408 CharAI m_target (CharAI +0x40)     private in CharAI.hpp (layout request)
//   +0x47c CharAI m_skillScripts (+0xb4)      private in CharAI.hpp (layout request), vector<CharAISkillScript*>
//   +0x488 CharAI m_spellScripts (+0xc0)      private in CharAI.hpp (layout request), vector<CharAISkillScript*>
//   +0x4fc CharStateMachine (base)            +0x560 CharProperties (base)
//   +0xff4 CharProperties m_props (+0xa94)    CharProperties::_GetProperty is private (layout request)
//
// No test: every body depends on the Lua accessors above. The arithmetic that the bindings perform on engine values
// (the flee point, the head-to offset, the projectile placement) is written out in the comments of this file.

#include "dh2/Character.hpp"

// 0x3b8ed8 _ZN9Character7_LookAtERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
// PARTIAL. Not written. With at least one argument and args[0] a light userdata (tag 2) or userdata (tag 7):
// v2Controller::Cmd_LookAt(GameObject*) runs on the controller at self +0x378 with target = getUserData(args[0]).
// Other cases do nothing. Missing: the Arguments and Value accessors (size, operator[], tag, getUserData).
void Character::_LookAt([[maybe_unused]] const sfc::script::lua::Arguments& args,
                        [[maybe_unused]] sfc::script::lua::ReturnValues& ret, [[maybe_unused]] void* userData) {}

// 0x3b8f38 _ZN9Character10_SetTargetERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
// PARTIAL. Not written. Same argument test as _LookAt (args[0] tag 2 or 7). CharAI::AI_SetTarget(getUserData(args[0]),
// false) runs on the CharAI base (self +0x3c8; AI_SetTarget(GameObject*, bool) at 0x3d6890). The force flag is 0.
// Missing: the Arguments and Value accessors.
void Character::_SetTarget([[maybe_unused]] const sfc::script::lua::Arguments& args,
                           [[maybe_unused]] sfc::script::lua::ReturnValues& ret, [[maybe_unused]] void* userData) {}

// 0x3b8f9c _ZN9Character20_GetCurrentSkillInfoERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
// PARTIAL. Not written. Needs at least one argument. Logic from the listing:
//   If args[0] is not a number (tag other than 3), idx = (unsigned) getNumber(args[0]). When the count word at +4 of
//   GetCharSkillList() is <= idx, the binding returns with nothing pushed. Otherwise args[0] is read again.
//   skill = (int) getNumber(args[0]). The number path has no range check at all.
//   GetCharSkill(skill) runs and its result is discarded. Then pushInteger(ret, SG_GetSkillLevel(skill)).
// Missing: the Arguments and Value accessors, ReturnValues::pushInteger, and the skill list layout (GetCharSkillList
// returns const void*, so the count word is untyped).
void Character::_GetCurrentSkillInfo([[maybe_unused]] const sfc::script::lua::Arguments& args,
                                     [[maybe_unused]] sfc::script::lua::ReturnValues& ret,
                                     [[maybe_unused]] void* userData) {}

// 0x3b9084 _ZN9Character10_SetMasterERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
// PARTIAL. Not written. Same argument test as _LookAt (args[0] tag 2 or 7). CharAI::AI_SetMaster(getUserData(args[0]))
// runs on the CharAI base (self +0x3c8; AI_SetMaster(Character*) at 0x3d4d80). The void binding returns nothing.
// Missing: the Arguments and Value accessors.
void Character::_SetMaster([[maybe_unused]] const sfc::script::lua::Arguments& args,
                           [[maybe_unused]] sfc::script::lua::ReturnValues& ret, [[maybe_unused]] void* userData) {}

// 0x3b90e4 _ZN9Character24_SetSpellCooldownTimerIdERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
// PARTIAL. Not written. With no argument nothing happens. The timer id comes from args[0]: a number gives
// getUInteger(args[0]), nil gives -1, and any other tag returns without a write. Then, for each index i below the
// count word at +4 of GetCharFaeryList(), the pointer CharAI::m_spellScripts[i] (self +0x488, the vector begin, no
// range check) gets its +0x18 word set to that id, when the pointer is non-null.
// Missing: the Arguments and Value accessors; CharAI::m_spellScripts is private (layout request); the faery list is
// untyped (const void*). The +0x18 field is m_lastSkillId in CharAISkillScript.hpp, see the notes of this batch.
void Character::_SetSpellCooldownTimerId([[maybe_unused]] const sfc::script::lua::Arguments& args,
                                         [[maybe_unused]] sfc::script::lua::ReturnValues& ret,
                                         [[maybe_unused]] void* userData) {}

// 0x3b91a0 _ZN9Character5_FleeERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
// PARTIAL. Not written. Checked in the assembly (0x3b91a0). With args[0] a light userdata (tag 2) or userdata (tag 7),
// the listing's getUserData result is not used, so only the tag gates the body. When CharAI m_target (self +0x408)
// is non-null, the point is reflected through the target:
//   d = GetTargetPosition(self) - GetTargetPosition(m_target)   (component-wise, three float subtractions)
//   pos = GetTargetPosition(self) + d                           (three float additions)
//   v2Controller::Cmd_MoveTo(const Point3D&) runs on the controller at self +0x378 with pos.
// Missing: the Arguments and Value tag accessor; CharAI::m_target is private (layout request).
void Character::_Flee([[maybe_unused]] const sfc::script::lua::Arguments& args,
                      [[maybe_unused]] sfc::script::lua::ReturnValues& ret, [[maybe_unused]] void* userData) {}

// 0x3b92a4 _ZN9Character16_SpellCombatRollERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
// PARTIAL. Not written. Checked in the assembly (0x3b92a4). First, faery = SG_GetCurrentFaerieId(self, -1), always.
// With args[0] a light userdata (tag 2) or userdata (tag 7), and target = getUserData(args[0]) non-null:
//   h = target->GetHandle() (ObjectBase::GetHandle on the target, the sret buffer), c = (Character*) h.
//   If c is non-null: spell = GetCharFaery(faery); AttackResult r; F_SpellAttack(r, self, c, spell[+0x08], 0);
//     F_ApplyResult(r, self, c, false); pushInteger(ret, r.m_damage).
//   If c is null: if target->GetInteractionType(self) (vtable +0x90) equals 8, target->Interact(self) (vtable +0x98)
//     runs and pushBoolean(ret, false). Otherwise nothing is pushed.
// Missing: the Arguments and Value accessors (tag, getUserData), ReturnValues::pushInteger and pushBoolean. The faery
// word at +0x08 is Structs::Faery m_w2 in pydata-accessors (name not confirmed here). F_SpellAttack and F_ApplyResult
// take the AttackResult in the first argument register in the binary, so they are static there (see the notes).
void Character::_SpellCombatRoll([[maybe_unused]] const sfc::script::lua::Arguments& args,
                                 [[maybe_unused]] sfc::script::lua::ReturnValues& ret,
                                 [[maybe_unused]] void* userData) {}

// 0x3b93cc _ZN9Character9_EndSkillERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
// PARTIAL. Not written. With args[0] a number: skill = getUInteger(args[0]). When skill is below the count word at +4 of
// GetCharSkillList(), CharAI::AI_EndSkill((unsigned) getNumber(args[0])) runs on the CharAI base (self +0x3c8,
// 0x3d8474). Other cases do nothing. Missing: the Arguments and Value accessors, and the untyped skill list.
void Character::_EndSkill([[maybe_unused]] const sfc::script::lua::Arguments& args,
                          [[maybe_unused]] sfc::script::lua::ReturnValues& ret, [[maybe_unused]] void* userData) {}

// 0x3b94ac _ZN9Character10_StopSkillERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
// PARTIAL. Not written. Two forms, chosen by the tag of args[0]:
//   number: CharAI::AI_EndSkill((unsigned) getNumber(args[0])) runs with no range check.
//   other (including nil): idx = getNumber(args[0]) as a float. When the count word at +4 of GetCharSkillList() is
//     greater than (unsigned) idx, AI_EndSkill((unsigned) getNumber(args[0])) runs. Otherwise nothing.
// With no argument nothing happens. Missing: the Arguments and Value accessors and the untyped skill list. Compare
// _EndSkill, which tests the index with getUInteger instead.
void Character::_StopSkill([[maybe_unused]] const sfc::script::lua::Arguments& args,
                           [[maybe_unused]] sfc::script::lua::ReturnValues& ret, [[maybe_unused]] void* userData) {}

// 0x3b9580 _ZN9Character13_SetStunStateERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
// PARTIAL. Not written. Needs args[0] a number. duration = (unsigned) getNumber(args[0]). flag = getBool(args[1]) when
// there are at least two arguments and args[1] is a boolean (tag 1), otherwise true. Then
// CharStateMachine::SM_SetStunState(duration, flag, nullptr, false) runs on the CharStateMachine at self +0x4fc
// (0x3c5ffc). Other cases do nothing. Missing: the Arguments and Value accessors.
void Character::_SetStunState([[maybe_unused]] const sfc::script::lua::Arguments& args,
                              [[maybe_unused]] sfc::script::lua::ReturnValues& ret, [[maybe_unused]] void* userData) {}

// 0x3b96b0 _ZN9Character14_SetScareStateERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
// PARTIAL. Not written. Same shape as _SetStunState: args[0] a number gives the duration, args[1] a boolean gives the
// flag (default true), and CharStateMachine::SM_SetScareState(duration, flag, nullptr, false) runs on the
// CharStateMachine at self +0x4fc (0x3c6144). Missing: the Arguments and Value accessors.
void Character::_SetScareState([[maybe_unused]] const sfc::script::lua::Arguments& args,
                               [[maybe_unused]] sfc::script::lua::ReturnValues& ret, [[maybe_unused]] void* userData) {}

// 0x3b97e0 _ZN9Character24_SetSkillCooldownTimerIdERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
// PARTIAL. Not written. Needs at least two arguments. If args[0] is not a number it is read as a skill index: the binding
// returns when the count word at +4 of GetCharSkillList() is <= (unsigned) getNumber(args[0]). Then args[1] must be a
// number or nil, and any other tag returns without a write. slot = (int) getNumber(args[0]);
// script = CharAI::m_skillScripts[slot] (self +0x47c, vector begin, no range check). When script is non-null, its +0x18
// word becomes getUInteger(args[1]) for a number, and -1 for nil.
// Missing: the Arguments and Value accessors; CharAI::m_skillScripts is private (layout request); the +0x18 name
// conflict (see the notes).
void Character::_SetSkillCooldownTimerId([[maybe_unused]] const sfc::script::lua::Arguments& args,
                                         [[maybe_unused]] sfc::script::lua::ReturnValues& ret,
                                         [[maybe_unused]] void* userData) {}

// 0x3b99d0 _ZN9Character21_SpawnSkillProjectileERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
// PARTIAL. Not written. Needs at least two arguments, args[0] a number and args[1] a number (the projectile id). The id
// must be below Arrays::ProjectileTable::size (getUInteger). script = CharAI::m_skillScripts[(int) getNumber(args[0])]
// (self +0x47c, no range check). A null script raises the assertion 'script && "WOW! If a non-existing script was able to
// call this method, MIRACLE!"' (Character_LuaCallbacks.cpp line 916; at assert level 2 the original stores to address 0)
// and the call goes on with the null script. The spawn, by argument count and tag:
//   fewer than three arguments, or args[2] neither boolean nor number:
//     projectile = CharAISkillScript::SpawnProjectile(script, getUInteger(args[1]), 0)
//     The literal 0 selects an overload with the same bit pattern for false (bool, 0x3da510) or 0.0f (float,
//     0x3da494). The overload is not resolved here.
//   args[2] a boolean: SpawnProjectile(script, getUInteger(args[1]), getBool(args[2])) (bool overload, 0x3da510).
//   args[2] a number: SpawnProjectile(script, getUInteger(args[1]), getNumber(args[2])) (float overload, 0x3da494).
// Then, when there are three or more arguments and args[2] is userdata (tag 7): z = projectile m_position.z (+0x168,
// read before anything else), the userdata's m_position x and y (+0x160, +0x164) are taken, and
// GameObject::SetPosition(projectile, (x, y, z), true) runs. When there are four or more arguments and args[3] is
// userdata, GameObject::SetDestination(projectile, args[3] m_position) runs. Finally pushPointer(ret, projectile), even
// when the spawn returned nothing.
// Missing: the Arguments and Value accessors, ReturnValues::pushPointer, CharAI::m_skillScripts (private, layout
// request), Arrays::ProjectileTable::size (pydata-accessors). CharAISkillScript::SpawnProjectile is declared to return
// int in CharAISkillScript.hpp, but the listing uses the result as a projectile object (see the notes). The assertion
// message is not reproduced.
void Character::_SpawnSkillProjectile([[maybe_unused]] const sfc::script::lua::Arguments& args,
                                      [[maybe_unused]] sfc::script::lua::ReturnValues& ret,
                                      [[maybe_unused]] void* userData) {}

// 0x3b9d8c _ZN9Character8_GetPropERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
// PARTIAL. Not written. Checked in the assembly (0x3b9d8c). Needs args[0] a number with id = getUInteger(args[0]) at most
// 0xDF (223). Then, by the tag of args[1] (one argument counts as no second argument):
//   args[1] a light userdata (tag 2): sheet = getPointer(args[1]). When it is non-null, pushInteger(ret,
//     CharProperties::PROPS_GetFromSheet(id, sheet)) (0x3b55d4). A null sheet pushes nothing.
//   args[1] a boolean (tag 1): sheet = CharProperties::s_temp when the boolean is true, else self +0xff4 (m_props).
//     pushInteger(ret, _GetProperty(sheet, id)).
//   any other case: sheet = self +0xff4 (m_props); pushInteger(ret, _GetProperty(sheet, id)).
// _GetProperty is CharProperties::_GetProperty(const Structs::CharacterProperties&, int) const (0x3dedb4), declared in the
// private part of CharProperties.hpp (layout request). Missing: the Arguments and Value accessors, ReturnValues::pushInteger.
void Character::_GetProp([[maybe_unused]] const sfc::script::lua::Arguments& args,
                         [[maybe_unused]] sfc::script::lua::ReturnValues& ret, [[maybe_unused]] void* userData) {}

// 0x3b9f44 _ZN9Character7_AttackERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
// PARTIAL. Not written. With no argument: when CharAI m_target (self +0x408, private in CharAI.hpp) is non-null,
// v2Controller::Cmd_Attack(target) runs on the controller at self +0x378. With at least one argument and args[0] a light
// userdata (tag 2) or userdata (tag 7): Cmd_Attack(getUserData(args[0])) runs on the same controller. Other tags do
// nothing. Missing: the Arguments and Value accessors (size, tag, getUserData); CharAI::m_target (layout request).
void Character::_Attack([[maybe_unused]] const sfc::script::lua::Arguments& args,
                        [[maybe_unused]] sfc::script::lua::ReturnValues& ret, [[maybe_unused]] void* userData) {}

// 0x3b9fbc _ZN9Character16_SkillCombatRollERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
// PARTIAL. Not written. Checked in the assembly (0x3b9fbc). Needs at least two arguments with args[0] a number and
// skill = getUInteger(args[0]) below the count word at +4 of GetCharSkillList(), and args[1] a light userdata (tag 2) or
// userdata (tag 7) with target = getUserData(args[1]) non-null. Then:
//   h = target->GetHandle(), c = (Character*) h.
//   If c is non-null: rec = GetCharSkill(skill), a Structs::Skill record (+0x14 m_w5, +0x1c m_w7 = flags word).
//     If (flags & 0x800000): if ItemInventory::HasMainHandWeapon() (self +0x37c), r = F_SkillAttack(r, self, c, flags,
//       rec[+0x14]), F_ApplyResult(r, self, c, false) and pushInteger(ret, r.m_damage). Then, independently, if
//       HasOffHandWeapon(), the same with flags | 0x4000000 and the same pushes.
//     Otherwise: r = F_SkillAttack(r, self, c, flags, rec[+0x14]), F_ApplyResult(r, self, c, false) (the suppress flag is
//       flags & 0x800000, which is 0 here) and pushInteger(ret, r.m_damage).
//   If c is null: if target->GetInteractionType(self) equals 8, target->Interact(self) runs and pushBoolean(ret, false).
// Missing: the Arguments and Value accessors, ReturnValues::pushInteger and pushBoolean, the names of Structs::Skill
// m_w5 and m_w7, and the F_SkillAttack declaration (static in the binary, see the notes).
void Character::_SkillCombatRoll([[maybe_unused]] const sfc::script::lua::Arguments& args,
                                 [[maybe_unused]] sfc::script::lua::ReturnValues& ret,
                                 [[maybe_unused]] void* userData) {}

// 0x3ba298 _ZN9Character20_SetProjectileTargetERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
// PARTIAL. Not written. This binding does not read the Character: the listing takes only the argument list. With two or
// more arguments, args[0] a light userdata (tag 2) and args[1] a userdata (tag 7): p = getPointer(args[0]), and when p is
// non-null, the projectile's m_target (Projectile +0x384, GameObject*, dh2/Projectile.hpp line 60) = getUserData(args[1]).
// Missing: the Arguments and Value accessors (size, tag, getPointer, getUserData). Include dh2/Projectile.hpp when wired.
void Character::_SetProjectileTarget([[maybe_unused]] const sfc::script::lua::Arguments& args,
                                     [[maybe_unused]] sfc::script::lua::ReturnValues& ret,
                                     [[maybe_unused]] void* userData) {}

// 0x3ba388 _ZN9Character9_PlayAnimERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
// PARTIAL. Not written. Checked in the assembly (0x3ba388, which reads the static counter and the table bases). Needs
// args[0] a number, and anim = getUInteger(args[0]) below Arrays::AnimDict::size. The anim value is not used otherwise.
// A function-local static counter SLOT takes the old value v (then SLOT = v + 1):
//   v in 0..3: state = v + 16, rowA = state, rowB = v + 21.
//   v = 4 (the new value is 5, so the counter is reset to 0): state = 15, rowA = 15, rowB = 20.
// Steps, written at each row's steps pointer +8 (the 32-bit record of Arrays::AnimTable, 20-byte stride, +8 StepsSize,
// +12 steps pointer): with two or more arguments and args[1] a number, rowA gets getNumber(args[0]) and rowB gets
// getNumber(args[1]). Otherwise rowA gets getNumber(args[0]) and rowB gets -1.
// Then CharStateMachine::SM_SetAnimState(state, loop, false) runs on the CharStateMachine at self +0x4fc (0x3c1aa8), with
// loop = getBool(args[2]) when there are three or more arguments and args[2] is a boolean, otherwise false.
// Two asserts (level 1 messages, Character_LuaCallbacks.cpp lines 1232 and 1233) check that rowA has StepsSize 2 and rowB
// has StepsSize 1. Missing: the Arguments and Value accessors; Arrays::AnimDict::size and Arrays::AnimTable
// (pydata-accessors). The SLOT counter must stay a function-local static in the port.
void Character::_PlayAnim([[maybe_unused]] const sfc::script::lua::Arguments& args,
                          [[maybe_unused]] sfc::script::lua::ReturnValues& ret, [[maybe_unused]] void* userData) {}

// 0x3ba71c _ZN9Character7_HeadToERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
// PARTIAL. Not written. The argument count chooses the form (the position arithmetic is from the listing):
//   one argument, a light userdata (tag 2) or userdata (tag 7): v2Controller::Cmd_MoveTo(getUserData(args[0])) on the
//     controller at self +0x378 (GameObject overload). One argument of any other tag: nothing.
//   two arguments, or no argument: nothing.
//   three or more arguments: needs args[0] a number, or args[1] a number, or args[2] a number. Otherwise nothing.
//     Relative form, when there are four or more arguments and args[3] is boolean true:
//       L = GameObject::GetLookAtVec(self), K = Vec3f_K (the 3-float global at 0x99f878; (0, 0, 1) per the game-world notes),
//       pos = self m_position (+0x160) + t * (K x L) + f * L + u * K, with t = getNumber(args[0]), f = getNumber(args[1]),
//       u = getNumber(args[2]). For K = (0, 0, 1), K x L is (-Ly, Lx, 0).
//     Absolute form otherwise: pos = (getNumber(args[0]), getNumber(args[1]), getNumber(args[2])).
//   Then v2Controller::Cmd_HeadTo(const Point3D&) runs on the controller at self +0x378 with pos.
// Missing: the Arguments and Value accessors. Vec3f_K has no declaration in the tree.
void Character::_HeadTo([[maybe_unused]] const sfc::script::lua::Arguments& args,
                        [[maybe_unused]] sfc::script::lua::ReturnValues& ret, [[maybe_unused]] void* userData) {}

// 0x3babdc _ZN9Character15_ApplyPropClassERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
// PARTIAL. Not written. Needs args[0] a number, classId = getUInteger(args[0]). The binding returns when classId is above
// Arrays::ClassTable::size. The listing's test is '>', so an id equal to the size passes; the port keeps that. Then, by
// the argument count and the tag of args[1]:
//   no second argument (count at most one), or args[1] neither light userdata nor boolean:
//     CharProperties::PROPS_ApplyClass((unsigned) getNumber(args[0]), false) (0x3df3b8) on the CharProperties base (+0x560).
//     A count of zero returns before this step.
//   args[1] a boolean (tag 1): PROPS_ApplyClass(classId, getBool(args[1])).
//   args[1] a light userdata (tag 2): sheet = getPointer(args[1]). When it is non-null,
//     PROPS_ApplyClassToSheet(classId, sheet) (0x3df314) and then RecalcProperties(true) (0x3e0810) run. A null sheet
//     does nothing.
// Missing: the Arguments and Value accessors (size, tag, getUInteger, getBool, getNumber, getPointer) and
// Arrays::ClassTable::size (pydata-accessors).
void Character::_ApplyPropClass([[maybe_unused]] const sfc::script::lua::Arguments& args,
                                [[maybe_unused]] sfc::script::lua::ReturnValues& ret,
                                [[maybe_unused]] void* userData) {}
