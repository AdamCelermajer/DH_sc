// Batch g1-game-app-global-f216b6-1, object factories (<global>, part 2 of 22). Companion of
// g1-game-app-global-f216b6-1.cpp, split off because the dh2 game headers included here clash with
// net-session-online's c_msg_fixed.hpp in one translation unit (NetBitStream is defined by both).
// GetNewInstance<T>() builds one game object for the object class registry. Each original allocates with
// operator new (asm: operator new(size, hint 0)) and runs the class constructor with a GO_IDS constant that
// the asm passes in R1. The constants below are read from the asm, because the Hex-Rays listing drops them.
// Declarations: dh2/global/object_factory.hpp.
#include "dh2/global/object_factory.hpp"

#include "dh2/Character.hpp"
#include "dh2/Door.hpp"
#include "dh2/Projectile.hpp"

// 0x340800 ObjectBase* GetNewInstance<Character>()
// Asm: operator new(0x1f90, hint 0), then Character::Character(GO_IDS 0).
template <>
ObjectBase* GetNewInstance<Character>() {
    return new Character(static_cast<ObjectBase::GO_IDS>(0));
}

// 0x340824 ObjectBase* GetNewInstance<Door>()
// Asm: operator new(0x6d0, hint 0), then Door::Door(GO_IDS 2).
template <>
ObjectBase* GetNewInstance<Door>() {
    return new Door(static_cast<ObjectBase::GO_IDS>(2));
}

// 0x340d14 ObjectBase* GetNewInstance<Projectile>()
// Asm: operator new(0x3d4, hint 0), then Projectile::Projectile(GO_IDS 9).
template <>
ObjectBase* GetNewInstance<Projectile>() {
    return new Projectile(static_cast<ObjectBase::GO_IDS>(9));
}

// The factories below are PARTIAL stubs. Each class header is absent from game/include/dh2 at the end of
// this batch, so the class cannot be constructed. Each stub returns nullptr until dh2/<Class>.hpp lands.
// The original for each is: operator new(size, hint 0), then <Class>::<Class>(GO_IDS n), where n is the
// constant shown (read from the asm). Replace the body with that call once the header exists.

// 0x340ca8 ObjectBase* GetNewInstance<LevelConfig>(). Original: LevelConfig(GO_IDS 4). Waits on dh2/LevelConfig.hpp.
template <>
ObjectBase* GetNewInstance<LevelConfig>() {
    return nullptr;
}

// 0x340ccc ObjectBase* GetNewInstance<SoundEmitter>(). Original: SoundEmitter(GO_IDS 0x14). Waits on dh2/SoundEmitter.hpp.
template <>
ObjectBase* GetNewInstance<SoundEmitter>() {
    return nullptr;
}

// 0x340cf0 ObjectBase* GetNewInstance<LaserTypeProjectile>(). Original: LaserTypeProjectile(GO_IDS 0xa). Waits on dh2/LaserTypeProjectile.hpp.
template <>
ObjectBase* GetNewInstance<LaserTypeProjectile>() {
    return nullptr;
}

// 0x340d38 ObjectBase* GetNewInstance<ItemObject>(). Original: ItemObject(GO_IDS 3). Waits on dh2/ItemObject.hpp.
template <>
ObjectBase* GetNewInstance<ItemObject>() {
    return nullptr;
}

// 0x340d5c ObjectBase* GetNewInstance<DestructibleContainer>(). Original: DestructibleContainer(GO_IDS 1). Waits on dh2/DestructibleContainer.hpp.
template <>
ObjectBase* GetNewInstance<DestructibleContainer>() {
    return nullptr;
}

// 0x340d80 ObjectBase* GetNewInstance<SlotContainer>(). Original: SlotContainer(GO_IDS 8). Waits on dh2/SlotContainer.hpp.
template <>
ObjectBase* GetNewInstance<SlotContainer>() {
    return nullptr;
}

// 0x340da4 ObjectBase* GetNewInstance<OpenableContainer>(). Original: OpenableContainer(GO_IDS 7). Waits on dh2/OpenableContainer.hpp.
template <>
ObjectBase* GetNewInstance<OpenableContainer>() {
    return nullptr;
}

// 0x340dc8 ObjectBase* GetNewInstance<LiftableObject>(). Original: LiftableObject(GO_IDS 6). Waits on dh2/LiftableObject.hpp.
template <>
ObjectBase* GetNewInstance<LiftableObject>() {
    return nullptr;
}

// 0x340dec ObjectBase* GetNewInstance<SpawnSpot>(). Original: SpawnSpot(GO_IDS 0x14). Waits on dh2/SpawnSpot.hpp.
template <>
ObjectBase* GetNewInstance<SpawnSpot>() {
    return nullptr;
}

// 0x340e10 ObjectBase* GetNewInstance<SpawnPoint>(). Original: SpawnPoint(GO_IDS 0xd). Waits on dh2/SpawnPoint.hpp.
template <>
ObjectBase* GetNewInstance<SpawnPoint>() {
    return nullptr;
}

// 0x340e34 ObjectBase* GetNewInstance<ProjectileTrap>(). Original: ProjectileTrap(GO_IDS 0x11). Waits on dh2/ProjectileTrap.hpp.
template <>
ObjectBase* GetNewInstance<ProjectileTrap>() {
    return nullptr;
}

// 0x340e58 ObjectBase* GetNewInstance<TimerTrap>(). Original: TimerTrap(GO_IDS 0x10). Waits on dh2/TimerTrap.hpp.
template <>
ObjectBase* GetNewInstance<TimerTrap>() {
    return nullptr;
}
