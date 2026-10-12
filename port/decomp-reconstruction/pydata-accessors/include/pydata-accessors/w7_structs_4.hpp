#pragma once

// Structs::ItemPowerRef and the 40 Structs::ItemPower* classes that derive from it (slice structs_4,
// pydata-accessors). Destructors only: D1/D2 is the destructor and D0 the deleting destructor
// (DeletingDestructor). The element array is the word at +0x10 (m_w4); its element stride is 16 and its
// block starts 8 bytes before the first element (see the .cpp). No other field is touched by these rows,
// so m_w1..m_w3 (+0x4..+0x8) are not declared. The element type is pydata::ArrayElement.
// Name clash: member_names.hpp declares a name-table struct Structs::ItemPowerRef, so do not include
// both headers in one translation unit (the same caveat as the Arrays w7 headers).
#include <cstdint>

namespace Structs {

class ItemPowerRef {
public:
    virtual ~ItemPowerRef();
    void* m_w4 = nullptr;  // +0x10: element array (16-byte elements), see DestroyElementArray
};

void DeletingDestructor(ItemPowerRef* self);

// Derived from ItemPowerRef, no fields of its own.
class ItemPowerWandDamageBonus : public ItemPowerRef {
public:
    ~ItemPowerWandDamageBonus() override;
};
void DeletingDestructor(ItemPowerWandDamageBonus* self);

// Derived from ItemPowerRef, no fields of its own.
class ItemPowerStaffDamageBonus : public ItemPowerRef {
public:
    ~ItemPowerStaffDamageBonus() override;
};
void DeletingDestructor(ItemPowerStaffDamageBonus* self);

// Derived from ItemPowerRef, no fields of its own.
class ItemPowerRangeDamageBonus : public ItemPowerRef {
public:
    ~ItemPowerRangeDamageBonus() override;
};
void DeletingDestructor(ItemPowerRangeDamageBonus* self);

// Derived from ItemPowerRef, no fields of its own.
class ItemPowerDaggerDamageBonus : public ItemPowerRef {
public:
    ~ItemPowerDaggerDamageBonus() override;
};
void DeletingDestructor(ItemPowerDaggerDamageBonus* self);

// Derived from ItemPowerRef, no fields of its own.
class ItemPowerMaceDamageBonus : public ItemPowerRef {
public:
    ~ItemPowerMaceDamageBonus() override;
};
void DeletingDestructor(ItemPowerMaceDamageBonus* self);

// Derived from ItemPowerRef, no fields of its own.
class ItemPowerAxeDamageBonus : public ItemPowerRef {
public:
    ~ItemPowerAxeDamageBonus() override;
};
void DeletingDestructor(ItemPowerAxeDamageBonus* self);

// Derived from ItemPowerRef, no fields of its own.
class ItemPowerSwordDamageBonus : public ItemPowerRef {
public:
    ~ItemPowerSwordDamageBonus() override;
};
void DeletingDestructor(ItemPowerSwordDamageBonus* self);

// Derived from ItemPowerRef, no fields of its own.
class ItemPowerSpellToHit : public ItemPowerRef {
public:
    ~ItemPowerSpellToHit() override;
};
void DeletingDestructor(ItemPowerSpellToHit* self);

// Derived from ItemPowerRef, no fields of its own.
class ItemPowerSpellCritical : public ItemPowerRef {
public:
    ~ItemPowerSpellCritical() override;
};
void DeletingDestructor(ItemPowerSpellCritical* self);

// Derived from ItemPowerRef, no fields of its own.
class ItemPowerManaLeech : public ItemPowerRef {
public:
    ~ItemPowerManaLeech() override;
};
void DeletingDestructor(ItemPowerManaLeech* self);

// Derived from ItemPowerRef, no fields of its own.
class ItemPowerLifeLeech : public ItemPowerRef {
public:
    ~ItemPowerLifeLeech() override;
};
void DeletingDestructor(ItemPowerLifeLeech* self);

// Derived from ItemPowerRef, no fields of its own.
class ItemPowerResistAll : public ItemPowerRef {
public:
    ~ItemPowerResistAll() override;
};
void DeletingDestructor(ItemPowerResistAll* self);

// Derived from ItemPowerRef, no fields of its own.
class ItemPowerResistElectrical : public ItemPowerRef {
public:
    ~ItemPowerResistElectrical() override;
};
void DeletingDestructor(ItemPowerResistElectrical* self);

// Derived from ItemPowerRef, no fields of its own.
class ItemPowerResistAir : public ItemPowerRef {
public:
    ~ItemPowerResistAir() override;
};
void DeletingDestructor(ItemPowerResistAir* self);

// Derived from ItemPowerRef, no fields of its own.
class ItemPowerResistWater : public ItemPowerRef {
public:
    ~ItemPowerResistWater() override;
};
void DeletingDestructor(ItemPowerResistWater* self);

// Derived from ItemPowerRef, no fields of its own.
class ItemPowerResistEarth : public ItemPowerRef {
public:
    ~ItemPowerResistEarth() override;
};
void DeletingDestructor(ItemPowerResistEarth* self);

// Derived from ItemPowerRef, no fields of its own.
class ItemPowerResistFire : public ItemPowerRef {
public:
    ~ItemPowerResistFire() override;
};
void DeletingDestructor(ItemPowerResistFire* self);

// Derived from ItemPowerRef, no fields of its own.
class ItemPowerDR : public ItemPowerRef {
public:
    ~ItemPowerDR() override;
};
void DeletingDestructor(ItemPowerDR* self);

// Derived from ItemPowerRef, no fields of its own.
class ItemPowerAR : public ItemPowerRef {
public:
    ~ItemPowerAR() override;
};
void DeletingDestructor(ItemPowerAR* self);

// Derived from ItemPowerRef, no fields of its own.
class ItemPowerToHit : public ItemPowerRef {
public:
    ~ItemPowerToHit() override;
};
void DeletingDestructor(ItemPowerToHit* self);

// Derived from ItemPowerRef, no fields of its own.
class ItemPowerDamage : public ItemPowerRef {
public:
    ~ItemPowerDamage() override;
};
void DeletingDestructor(ItemPowerDamage* self);

// Derived from ItemPowerRef, no fields of its own.
class ItemPowerArmor : public ItemPowerRef {
public:
    ~ItemPowerArmor() override;
};
void DeletingDestructor(ItemPowerArmor* self);

// Derived from ItemPowerRef, no fields of its own.
class ItemPowerBlock : public ItemPowerRef {
public:
    ~ItemPowerBlock() override;
};
void DeletingDestructor(ItemPowerBlock* self);

// Derived from ItemPowerRef, no fields of its own.
class ItemPowerDodge : public ItemPowerRef {
public:
    ~ItemPowerDodge() override;
};
void DeletingDestructor(ItemPowerDodge* self);

// Derived from ItemPowerRef, no fields of its own.
class ItemPowerCritical : public ItemPowerRef {
public:
    ~ItemPowerCritical() override;
};
void DeletingDestructor(ItemPowerCritical* self);

// Derived from ItemPowerRef, no fields of its own.
class ItemPowerMPRegen : public ItemPowerRef {
public:
    ~ItemPowerMPRegen() override;
};
void DeletingDestructor(ItemPowerMPRegen* self);

// Derived from ItemPowerRef, no fields of its own.
class ItemPowerHPRegen : public ItemPowerRef {
public:
    ~ItemPowerHPRegen() override;
};
void DeletingDestructor(ItemPowerHPRegen* self);

// Derived from ItemPowerRef, no fields of its own.
class ItemPowerMP : public ItemPowerRef {
public:
    ~ItemPowerMP() override;
};
void DeletingDestructor(ItemPowerMP* self);

// Derived from ItemPowerRef, no fields of its own.
class ItemPowerHP : public ItemPowerRef {
public:
    ~ItemPowerHP() override;
};
void DeletingDestructor(ItemPowerHP* self);

// Derived from ItemPowerRef, no fields of its own.
class ItemPowerFireDotDamage : public ItemPowerRef {
public:
    ~ItemPowerFireDotDamage() override;
};
void DeletingDestructor(ItemPowerFireDotDamage* self);

// Derived from ItemPowerRef, no fields of its own.
class ItemPowerWaterDotDamage : public ItemPowerRef {
public:
    ~ItemPowerWaterDotDamage() override;
};
void DeletingDestructor(ItemPowerWaterDotDamage* self);

// Derived from ItemPowerRef, no fields of its own.
class ItemPowerEarthDotDamage : public ItemPowerRef {
public:
    ~ItemPowerEarthDotDamage() override;
};
void DeletingDestructor(ItemPowerEarthDotDamage* self);

// Derived from ItemPowerRef, no fields of its own.
class ItemPowerElectricalDamage : public ItemPowerRef {
public:
    ~ItemPowerElectricalDamage() override;
};
void DeletingDestructor(ItemPowerElectricalDamage* self);

// Derived from ItemPowerRef, no fields of its own.
class ItemPowerAirDamage : public ItemPowerRef {
public:
    ~ItemPowerAirDamage() override;
};
void DeletingDestructor(ItemPowerAirDamage* self);

// Derived from ItemPowerRef, no fields of its own.
class ItemPowerWaterDamage : public ItemPowerRef {
public:
    ~ItemPowerWaterDamage() override;
};
void DeletingDestructor(ItemPowerWaterDamage* self);

// Derived from ItemPowerRef, no fields of its own.
class ItemPowerEarthDamage : public ItemPowerRef {
public:
    ~ItemPowerEarthDamage() override;
};
void DeletingDestructor(ItemPowerEarthDamage* self);

// Derived from ItemPowerRef, no fields of its own.
class ItemPowerFireDamage : public ItemPowerRef {
public:
    ~ItemPowerFireDamage() override;
};
void DeletingDestructor(ItemPowerFireDamage* self);

// Derived from ItemPowerRef, no fields of its own.
class ItemPowerEnergy : public ItemPowerRef {
public:
    ~ItemPowerEnergy() override;
};
void DeletingDestructor(ItemPowerEnergy* self);

// Derived from ItemPowerRef, no fields of its own.
class ItemPowerEndurance : public ItemPowerRef {
public:
    ~ItemPowerEndurance() override;
};
void DeletingDestructor(ItemPowerEndurance* self);

// Derived from ItemPowerRef, no fields of its own.
class ItemPowerDexterity : public ItemPowerRef {
public:
    ~ItemPowerDexterity() override;
};
void DeletingDestructor(ItemPowerDexterity* self);

}  // namespace Structs
