// Batch g1-game-character-Character-ee9946-11: twenty Lua static bindings of Character, registered by
// Character::createBindings (the Has*/IsMasterHostPlayer/TargetInMeleeRange queries, the timer Start/Stop/Pause/Resume
// calls, RegisterAnim, EnableSpotTargeting, SetLevel, RotateBy, SetKnockBackState, RegenHP, RegenMP and UseMana).
//
// Each binding has the Lua static signature (const sfc::script::lua::Arguments&, sfc::script::lua::ReturnValues&,
// void* userData), and userData is the Character. The Hex-Rays listings name the first pointer `this`, the second `a2`
// and the third `a3`, and type them shifted by one. The assembly passes userData in the third register (r2), so the
// bodies use it as the Character. Offsets in comments are the original 32-bit ones.
//
// BLOCKED ON THE LUA LAYER. sfc::script::lua::Arguments (operator[], size), Value (getBool, getNumber, getUInteger,
// getUserData, the tag word at Value +4) and ReturnValues (pushBoolean, pushInteger) have no declarations in the tree:
// only the forward declarations in dh2/Character.hpp exist. Every binding that reads an argument or pushes a result is
// therefore PARTIAL, and its comment gives the rest of the control flow from the listing. The numeric bindings gate on
// "argument count != 0 and args[0] tag == 3 (number)", the boolean ones on tag 1, and a tag 7 Value is userdata.
//
// PRIVATE CharAI MEMBERS. CharAI::m_target (+0x40) and CharAI::m_master (+0x50) are declared after `private:` in
// dh2/CharAI.hpp, so Character cannot read them. The bindings that test them (_HasMaster, _HasTarget,
// _IsMasterHostPlayer, _TargetInMeleeRange) are PARTIAL until that header adds a public accessor or a friend.
//
// Declarations come from dh2/Character.hpp, which this batch does not change: every method and member used here is
// already declared there (ItemInventory, CharTimers, CharAI::AI_* and CharAI::AI_HasAggro are public; the v2Controllable
// m_controller at +0x378 and the CharProperties RecalcProperties are public too).

#include "dh2/Character.hpp"

// 0x3b6f3c _ZN9Character10_HasMasterERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
// Character::_HasMaster(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL: the master test and the push are not made. The listing reads the CharAI master pointer (+0x50 in CharAI,
// Character +0x418), which is private in dh2/CharAI.hpp (layout request). Result: m_master != nullptr, as a boolean.
void Character::_HasMaster(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/,
                           void* userData) {
    const Character* self = static_cast<Character*>(userData);
    // const bool hasMaster = self->m_master != nullptr;   CharAI::m_master is private
    // Not reconstructed: ret.pushBoolean(hasMaster)
    (void)self;
}

// 0x3b6f50 _ZN9Character10_HasTargetERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
// Character::_HasTarget(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL: the target test and the push are not made. The listing reads CharAI::m_target (+0x40 in CharAI, Character
// +0x408), which is private in dh2/CharAI.hpp (layout request). Result: m_target != nullptr, as a boolean.
void Character::_HasTarget(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/,
                           void* userData) {
    const Character* self = static_cast<Character*>(userData);
    // const bool hasTarget = self->m_target != nullptr;   CharAI::m_target is private
    // Not reconstructed: ret.pushBoolean(hasTarget)
    (void)self;
}

// 0x3b6f64 _ZN9Character17_CanAttackInMeleeERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
// Character::_CanAttackInMelee(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL: the push is not made (no ReturnValues declaration). The value is ItemInventory::CanMeleeAttack() on the
// ItemInventory base at +0x37c, as the listing calls it. No argument is read.
void Character::_CanAttackInMelee(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/,
                                  void* userData) {
    Character* self = static_cast<Character*>(userData);
    const bool canAttack = self->ItemInventory::CanMeleeAttack();
    (void)canAttack;  // Not reconstructed: ret.pushBoolean(canAttack)
}

// 0x3b6f84 _ZN9Character7_HasBowERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
// Character::_HasBow(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL: the push is not made. The value is ItemInventory::HasBow() on the ItemInventory base at +0x37c.
void Character::_HasBow(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/,
                        void* userData) {
    Character* self = static_cast<Character*>(userData);
    const bool hasBow = self->ItemInventory::HasBow();
    (void)hasBow;  // Not reconstructed: ret.pushBoolean(hasBow)
}

// 0x3b6fa4 _ZN9Character10_HasShieldERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
// Character::_HasShield(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL: the push is not made. The value is ItemInventory::HasShield() on the ItemInventory base at +0x37c.
void Character::_HasShield(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/,
                           void* userData) {
    Character* self = static_cast<Character*>(userData);
    const bool hasShield = self->ItemInventory::HasShield();
    (void)hasShield;  // Not reconstructed: ret.pushBoolean(hasShield)
}

// 0x3b6fc4 _ZN9Character19_IsMasterHostPlayerERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
// Character::_IsMasterHostPlayer(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL: the push is not made, and the master test needs CharAI::m_master (private, layout request). Control flow of
// the listing: when m_master is null the result is false and the player manager is not read. Otherwise the result is
// PlayerManager::GetHostingPlayer() on the PlayerManager global dword_99F76C (dh2/global/IsMultiplayerGame.hpp), its
// PlayerInfo word at +0x660 (PlayerInfo::unk_660, read as the hosting character), compared with m_master.
void Character::_IsMasterHostPlayer(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/,
                                    void* userData) {
    const Character* self = static_cast<Character*>(userData);
    // if (self->m_master != nullptr)   CharAI::m_master is private
    //     isHost = dword_99F76C->GetHostingPlayer()->unk_660 == m_master;   (PlayerInfo +0x660, see the header comment)
    // Not reconstructed: ret.pushBoolean(isHost)
    (void)self;
}

// 0x3b7020 _ZN9Character19_TargetInMeleeRangeERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
// Character::_TargetInMeleeRange(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL: the push and the target read are not made. The listing calls CharAI::AI_IsInMeleeRange on the CharAI base
// (+0x3c8, public) with CharAI::m_target (+0x40, private: layout request) and pushes the result. There is no null
// test on the target in the listing.
void Character::_TargetInMeleeRange(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/,
                                    void* userData) {
    Character* self = static_cast<Character*>(userData);
    // const bool inRange = self->CharAI::AI_IsInMeleeRange(self->m_target);   CharAI::m_target is private
    // Not reconstructed: ret.pushBoolean(inRange)
    (void)self;
}

// 0x3b7044 _ZN9Character9_HasAggroERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
// Character::_HasAggro(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL: the push is not made. The value is CharAI::AI_HasAggro() on the CharAI base at +0x3c8 (public).
void Character::_HasAggro(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/,
                          void* userData) {
    Character* self = static_cast<Character*>(userData);
    const bool hasAggro = self->CharAI::AI_HasAggro();
    (void)hasAggro;  // Not reconstructed: ret.pushBoolean(hasAggro)
}

// 0x3b7064 _ZN9Character10_StopTimerERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
// Character::_StopTimer(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL: the TMR_Stop call is not made. The gate is "argument count != 0 and args[0] tag == 3". The timer id is
// Value::getUInteger(args[0]), and the call is CharTimers::TMR_Stop(id) on the CharTimers base at +0x3b4. Nothing is
// pushed.
void Character::_StopTimer(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/,
                           void* userData) {
    Character* self = static_cast<Character*>(userData);
    // if (args.count != 0 && args[0].tag == 3)  self->CharTimers::TMR_Stop(getUInteger(args[0]));
    // The argument decode is not declared (see the file comment).
    (void)self;
}

// 0x3b70d8 _ZN9Character13_RegisterAnimERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
// Character::_RegisterAnim(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL: the call is not made. The gate is "argument count != 0 and args[0] tag == 3". The animation dictionary id is
// (int)Value::getNumber(args[0]), passed to CharAnimator::ANIM_AddAnimDictToSet on the CharAnimator base at +0x49c.
void Character::_RegisterAnim(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/,
                              void* userData) {
    Character* self = static_cast<Character*>(userData);
    // if (args.count != 0 && args[0].tag == 3)
    //     self->CharAnimator::ANIM_AddAnimDictToSet(static_cast<int>(getNumber(args[0])));
    (void)self;
}

// 0x3b7144 _ZN9Character20_EnableSpotTargetingERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
// Character::_EnableSpotTargeting(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL: the argument decode is not declared, so no state is written. The listing's control flow, for reference:
//  - gate: args.count != 0 and args[0] tag == 1 (boolean); b0 = getBool(args[0]). Otherwise nothing.
//  - !b0: m_spotTargetPending (+0x14c8) = 0 and m_spotTargetFXId (+0x14ca, int16) = -1.
//  - b0 and (args.count > 1 and args[1] tag == 3 and (uint)getUInteger(args[1]) < Arrays::AnimatedEffectTable::size):
//    m_spotTargetPending = 1; m_spotTargetFXId = (int16)getUInteger(args[1]); the target position is the position of
//    args[2] when args.count > 2 and args[2] tag == 7 (userdata: its GameObject m_position at +0x160), otherwise this
//    Character's m_position (+0x160). Both m_spotTargetPos (+0x14b0) and m_spotTargetValidPos (+0x14bc) get that
//    position. The copies in the listing are word copies of the three floats.
//  - b0 with the second gate failing: nothing is written.
void Character::_EnableSpotTargeting(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/,
                                     void* userData) {
    Character* self = static_cast<Character*>(userData);
    (void)self;  // the writes above need the decoded argument values (Lua layer missing)
}

// 0x3b73a4 _ZN9Character9_SetLevelERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
// Character::_SetLevel(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL: the level is not decoded from the argument, so nothing is stored. The listing, for reference:
//  - gate: args.count != 0 and args[0] tag == 3; value = (float)getNumber(args[0]).
//  - maxLevel = PyDataConstants getConstant("CharacterDesign", "MaxLevelDVeryHard") << 8, the PyDataConstants from
//    ApplicationGetPyDataConstants() (dh2/global/charai_ai.hpp).
//  - level = maxLevel < (int)value ? maxLevel : (int)getNumber(args[0]). The clamp is taken only on the first branch.
//  - the int is stored at Character +0x5b8 (CharProperties +0x58, layout request), then CharProperties::RecalcProperties
//    (true) on the CharProperties base at +0x560, then RegenHP(-1) and RegenMP(-1). The listing returns RegenMP's result;
//    the void slot drops it.
void Character::_SetLevel(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/,
                          void* userData) {
    Character* self = static_cast<Character*>(userData);
    (void)self;  // the level store needs the decoded argument and CharProperties +0x58 (not declared)
}

// 0x3b74a8 _ZN9Character12_ResumeTimerERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
// Character::_ResumeTimer(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL: the call is not made. The gate is "argument count != 0 and args[0] tag == 3"; the call is
// CharTimers::TMR_Resume(getUInteger(args[0])) on the CharTimers base at +0x3b4.
void Character::_ResumeTimer(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/,
                             void* userData) {
    Character* self = static_cast<Character*>(userData);
    // if (args.count != 0 && args[0].tag == 3)  self->CharTimers::TMR_Resume(getUInteger(args[0]));
    (void)self;
}

// 0x3b751c _ZN9Character11_PauseTimerERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
// Character::_PauseTimer(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL: the call is not made. The gate is "argument count != 0 and args[0] tag == 3"; the call is
// CharTimers::TMR_Pause(getUInteger(args[0])) on the CharTimers base at +0x3b4.
void Character::_PauseTimer(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/,
                            void* userData) {
    Character* self = static_cast<Character*>(userData);
    // if (args.count != 0 && args[0].tag == 3)  self->CharTimers::TMR_Pause(getUInteger(args[0]));
    (void)self;
}

// 0x3b7590 _ZN9Character11_StartTimerERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
// Character::_StartTimer(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL: the start call and the integer push are not made. The gate is "argument count != 0 and args[0] tag == 3".
// The duration is getUInteger(args[0]). The repeat is -1 when the argument count is above 1 and getBool(args[1]) holds,
// otherwise 0. Both calls pass event 53 and a null user data to CharTimers::TMR_Start on the CharTimers base at +0x3b4.
// When the returned id is not -1 the listing pushes it: ret.pushInteger(id), which is not declared.
void Character::_StartTimer(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/,
                            void* userData) {
    Character* self = static_cast<Character*>(userData);
    // if (args.count != 0 && args[0].tag == 3) {
    //     const int repeat = (args.count > 1 && getBool(args[1])) ? -1 : 0;
    //     const int id = self->CharTimers::TMR_Start(getUInteger(args[0]), repeat, 53, nullptr);
    //     if (id != -1) ret.pushInteger(id);   (not declared)
    // }
    (void)self;
}

// 0x3b7680 _ZN9Character18_SetKnockBackStateERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
// Character::_SetKnockBackState(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL: the state call is not made, because the flag comes from the argument. The flag is true when the argument
// count is 0 or args[0] is not a boolean (tag 1), and getBool(args[0]) otherwise. The call is
// CharStateMachine::SM_SetKnockBackState(flag, nullptr, false) on the CharStateMachine base at +0x4fc (public). The
// listing returns its int result; the void binding drops it.
void Character::_SetKnockBackState(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/,
                                   void* userData) {
    Character* self = static_cast<Character*>(userData);
    // bool knockBack = true;
    // if (args.count != 0 && args[0].tag == 1) knockBack = getBool(args[0]);
    // self->CharStateMachine::SM_SetKnockBackState(knockBack, nullptr, false);
    (void)self;
}

// 0x3b7704 _ZN9Character9_RotateByERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
// Character::_RotateBy(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL: the command is not sent, because the angle comes from the argument. The gate is "argument count != 0 and
// args[0] tag == 3". The controller is v2Controllable::m_controller at +0x378 (public). The angle is
// getNumber(args[0]) times the float 0x3c8efa35 (pi/180, the kCharAIRadPerDegree bit pattern; Hex-Rays prints 0.017453),
// and the call is v2Controller::Cmd_RotateBy(controller, angle).
void Character::_RotateBy(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/,
                          void* userData) {
    Character* self = static_cast<Character*>(userData);
    v2Controller* controller = self->m_controller;
    // if (args.count != 0 && args[0].tag == 3)
    //     controller->Cmd_RotateBy(getNumber(args[0]) * 0.0174532925f);
    (void)controller;
}

// 0x3b7774 _ZN9Character8_RegenMPERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
// Character::_RegenMP(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL: the call is not made. The gate is "argument count != 0 and args[0] tag == 3"; the amount is
// (int)getNumber(args[0]), passed to Character::RegenMP.
void Character::_RegenMP(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/,
                         void* userData) {
    Character* self = static_cast<Character*>(userData);
    // if (args.count != 0 && args[0].tag == 3)  self->RegenMP(static_cast<int>(getNumber(args[0])));
    (void)self;
}

// 0x3b77ec _ZN9Character8_RegenHPERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
// Character::_RegenHP(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL: the call is not made. The gate is "argument count != 0 and args[0] tag == 3"; the amount is
// (int)getNumber(args[0]), passed to Character::RegenHP.
void Character::_RegenHP(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/,
                         void* userData) {
    Character* self = static_cast<Character*>(userData);
    // if (args.count != 0 && args[0].tag == 3)  self->RegenHP(static_cast<int>(getNumber(args[0])));
    (void)self;
}

// 0x3b7864 _ZN9Character8_UseManaERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
// Character::_UseMana(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL: the call and the push are not made. The gate is "argument count != 0 and args[0] tag == 3"; the cost is
// (int)getNumber(args[0]), passed to Character::UseMana, whose bool result is pushed with ret.pushBoolean. Nothing is
// pushed when the gate fails.
void Character::_UseMana(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/,
                         void* userData) {
    Character* self = static_cast<Character*>(userData);
    // if (args.count != 0 && args[0].tag == 3) {
    //     const bool used = self->UseMana(static_cast<int>(getNumber(args[0])));
    //     ret.pushBoolean(used);   (not declared)
    // }
    (void)self;
}
