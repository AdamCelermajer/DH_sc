// Batch g1-game-app-global-f216b6-4 (<global>, part 5 of 22): the GetNewState<CS*>() state factories and the
// armor-set trophy checks TrophyCheckArmorSet{Decay,Phantasm,...,Lordly}(ItemInventory*).
// Declarations: dh2/global/GetNewState.hpp and dh2/global/TrophyCheckArmorSet.hpp.

#include "dh2/global/GetNewState.hpp"
#include "dh2/global/TrophyCheckArmorSet.hpp"

// 0x3c0550 CharState* GetNewState<CSStunned>()
// PARTIAL: stub returns nullptr. The original returns &Singleton<CSStunned>::s_inst. Waits on two things:
// (1) no dh2/Singleton.hpp declares the Singleton<T>::s_inst template static (ELF _ZN9SingletonI9CSStunnedE6s_instE);
// (2) dh2/CSStunned.hpp does not compile: OnInit/OnFocus/OnBlur/OnUpdate/OnEvent are marked override, but
// dh2/CharState.hpp declares only unk_vslot_08..unk_vslot_18. Layout request: CharState slot names.
template <>
CharState* GetNewState<CSStunned>() {
    return nullptr;
}

// 0x3c056c CharState* GetNewState<CSKnockedBack>()
// PARTIAL: stub returns nullptr. Original: &Singleton<CSKnockedBack>::s_inst. Waits on dh2/Singleton.hpp and on
// dh2/CSKnockedBack.hpp (same override mismatch as CSStunned, see GetNewState<CSStunned>).
template <>
CharState* GetNewState<CSKnockedBack>() {
    return nullptr;
}

// 0x3c0588 CharState* GetNewState<CSInjured>()
// PARTIAL: stub returns nullptr. Original: &Singleton<CSInjured>::s_inst. Waits on dh2/Singleton.hpp and on
// dh2/CSInjured.hpp (CharState slot names).
template <>
CharState* GetNewState<CSInjured>() {
    return nullptr;
}

// 0x3c05a4 CharState* GetNewState<CSDead>()
// PARTIAL: stub returns nullptr. Original: &Singleton<CSDead>::s_inst. Waits on dh2/Singleton.hpp and on
// dh2/CSDead.hpp (CharState slot names).
template <>
CharState* GetNewState<CSDead>() {
    return nullptr;
}

// 0x3c05c0 CharState* GetNewState<CSInteract>()
// PARTIAL: stub returns nullptr. Original: &Singleton<CSInteract>::s_inst. Waits on dh2/Singleton.hpp and on
// dh2/CSInteract.hpp (CharState slot names).
template <>
CharState* GetNewState<CSInteract>() {
    return nullptr;
}

// 0x3c05dc CharState* GetNewState<CSAnim>()
// PARTIAL: stub returns nullptr. Original: &Singleton<CSAnim>::s_inst. Waits on dh2/Singleton.hpp and on
// dh2/CSAnim.hpp (CharState slot names).
template <>
CharState* GetNewState<CSAnim>() {
    return nullptr;
}

// 0x3c05f8 CharState* GetNewState<CSReviving>()
// PARTIAL: stub returns nullptr. Original: &Singleton<CSReviving>::s_inst. Waits on dh2/Singleton.hpp and on
// dh2/CSReviving.hpp (CharState slot names).
template <>
CharState* GetNewState<CSReviving>() {
    return nullptr;
}

// 0x3c0614 CharState* GetNewState<CSRevived>()
// PARTIAL: stub returns nullptr. Original: &Singleton<CSRevived>::s_inst. Waits on dh2/Singleton.hpp and on
// dh2/CSRevived.hpp (CharState slot names).
template <>
CharState* GetNewState<CSRevived>() {
    return nullptr;
}

// 0x3c0630 CharState* GetNewState<CSPreSpawn>()
// PARTIAL: stub returns nullptr. Original: &Singleton<CSPreSpawn>::s_inst. Waits on dh2/Singleton.hpp and on
// dh2/CSPreSpawn.hpp (CharState slot names).
template <>
CharState* GetNewState<CSPreSpawn>() {
    return nullptr;
}

// 0x3c064c CharState* GetNewState<CSLiftingIdle>()
// PARTIAL: stub returns nullptr. Original: &Singleton<CSLiftingIdle>::s_inst. Waits on dh2/Singleton.hpp and on
// dh2/CSLiftingIdle.hpp (CharState slot names).
template <>
CharState* GetNewState<CSLiftingIdle>() {
    return nullptr;
}

// 0x3c0668 CharState* GetNewState<CSLiftingMove>()
// PARTIAL: stub returns nullptr. Original: &Singleton<CSLiftingMove>::s_inst (Hex-Rays types the return as int*;
// the demangled symbol says CharState*). Waits on dh2/Singleton.hpp and on dh2/CSLiftingMove.hpp (CharState slot names).
template <>
CharState* GetNewState<CSLiftingMove>() {
    return nullptr;
}

// Armor-set trophy checks. The original looks at the five equipment slots of the inventory (3, 4, 8, 0 and 7) and
// reads each equipped item through ItemInstance::GetItem() (the Structs::Item row, 164 bytes, at
// Arrays::ItemTable::members + 164 * index) or ItemInstance::GetItemId() (the table index). They all end with
// Arrays::GetMemberIDByString<Arrays::TrophyTable>(name) when the set matches and return -1 otherwise.
// Every function below is PARTIAL: dh2/ItemInstance.hpp does not exist in the tree yet, so the item reads cannot be
// written (ItemInstance::GetItem() const and GetItemId() const are needed). Stubs return -1, which is the original's
// "no armor set" result, so no trophy is unlocked. The sets, in the original's order:
//   same m_w34 (+0x88 of the Structs::Item row) in all five slots: Decay 8 "gear_necromancer", Phantasm 9
//   "gear_illusionist", Shadow 5 "gear_assassin", Marksman 6 "gear_archer", Angelic 3 "gear_paladin", Rage 2
//   "gear_berserker"; item id in each slot's list: Archmage "gear_mage", Jester "gear_rogue", Lordly "gear_warrior"
//   (the id lists are in the ledger notes).

// 0x3fc6e8 int TrophyCheckArmorSetDecay(ItemInventory*)
// PARTIAL: stub returns -1. Original: all five slots' Structs::Item m_w34 == 8, then "gear_necromancer". Waits on dh2/ItemInstance.hpp.
int TrophyCheckArmorSetDecay(ItemInventory* inventory) {
    (void)inventory;
    return -1;
}

// 0x3fc790 int TrophyCheckArmorSetPhantasm(ItemInventory*)
// PARTIAL: stub returns -1. Original: all five slots' m_w34 == 9, then "gear_illusionist". Waits on dh2/ItemInstance.hpp.
int TrophyCheckArmorSetPhantasm(ItemInventory* inventory) {
    (void)inventory;
    return -1;
}

// 0x3fc83c int TrophyCheckArmorSetShadow(ItemInventory*)
// PARTIAL: stub returns -1. Original: all five slots' m_w34 == 5, then "gear_assassin". Waits on dh2/ItemInstance.hpp.
int TrophyCheckArmorSetShadow(ItemInventory* inventory) {
    (void)inventory;
    return -1;
}

// 0x3fc8e8 int TrophyCheckArmorSetMarksman(ItemInventory*)
// PARTIAL: stub returns -1. Original: all five slots' m_w34 == 6, then "gear_archer". Waits on dh2/ItemInstance.hpp.
int TrophyCheckArmorSetMarksman(ItemInventory* inventory) {
    (void)inventory;
    return -1;
}

// 0x3fc994 int TrophyCheckArmorSetAngelic(ItemInventory*)
// PARTIAL: stub returns -1. Original: all five slots' m_w34 == 3, then "gear_paladin". Waits on dh2/ItemInstance.hpp.
int TrophyCheckArmorSetAngelic(ItemInventory* inventory) {
    (void)inventory;
    return -1;
}

// 0x3fca40 int TrophyCheckArmorSetRage(ItemInventory*)
// PARTIAL: stub returns -1. Original: all five slots' m_w34 == 2, then "gear_berserker". Waits on dh2/ItemInstance.hpp.
int TrophyCheckArmorSetRage(ItemInventory* inventory) {
    (void)inventory;
    return -1;
}

// 0x3fcaec int TrophyCheckArmorSetArchmage(ItemInventory*)
// PARTIAL: stub returns -1. Original: slot 3 id in {289,290,291}, slot 4 in {325,326,327}, slot 8 in {361,362,363},
// slot 0 in {217,218,219} and slot 7 in {253,254,255}, then "gear_mage". Waits on dh2/ItemInstance.hpp.
int TrophyCheckArmorSetArchmage(ItemInventory* inventory) {
    (void)inventory;
    return -1;
}

// 0x3fcc98 int TrophyCheckArmorSetJester(ItemInventory*)
// PARTIAL: stub returns -1. Original: slot 3 id in {583,584,585}, slot 4 in {619,620,621}, slot 8 in {655,656,657},
// slot 0 in {511,512,513} and slot 7 in {547,548,549}, then "gear_rogue". Waits on dh2/ItemInstance.hpp.
int TrophyCheckArmorSetJester(ItemInventory* inventory) {
    (void)inventory;
    return -1;
}

// 0x3fce48 int TrophyCheckArmorSetLordly(ItemInventory*)
// PARTIAL: stub returns -1. Original: slot 3 id in {844,845,846}, slot 4 in {880,881,882}, slot 8 in {916,917,918},
// slot 0 in {772,773,774} and slot 7 in {808,809,810}, then "gear_warrior". Waits on dh2/ItemInstance.hpp.
int TrophyCheckArmorSetLordly(ItemInventory* inventory) {
    (void)inventory;
    return -1;
}
