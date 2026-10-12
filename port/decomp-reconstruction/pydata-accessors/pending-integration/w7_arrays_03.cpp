// Arrays::X::read(IStreamBase*) for the nine readable tables of slice arrays_03. The shared body is
// ReadElementArray in include/pydata-accessors/w7_arrays_03.hpp. Each function's address and mangled
// symbol are in the comment above it.
#include "pydata-accessors/w7_arrays_03.hpp"

namespace Arrays {

Structs::Projectile* ProjectileTable::members = nullptr;
std::uint32_t ProjectileTable::size = 0;
Structs::HintPage* HintPages::members = nullptr;
std::uint32_t HintPages::size = 0;
Structs::HelpPage* HelpPages::members = nullptr;
std::uint32_t HelpPages::size = 0;
Structs::TriggerPlate* TriggerPlates::members = nullptr;
std::uint32_t TriggerPlates::size = 0;
Structs::LiftableObject* LiftableObjects::members = nullptr;
std::uint32_t LiftableObjects::size = 0;
Structs::GameObjectDamage* GameObjectDamager::members = nullptr;
std::uint32_t GameObjectDamager::size = 0;
Structs::FontColorDef* FontPalette::members = nullptr;
std::uint32_t FontPalette::size = 0;
Structs::CharEffect* CharEffectTable::members = nullptr;
std::uint32_t CharEffectTable::size = 0;
Structs::GameOption* GameOptionTable::members = nullptr;
std::uint32_t GameOptionTable::size = 0;

// 0x4b9ab0 _ZN6Arrays15ProjectileTable4readEP11IStreamBase
void ProjectileTable::read(IStreamBase* stream) {
    ReadElementArray(members, size, stream, &ProjectileTable::finalize);
}

// 0x4bae14 _ZN6Arrays9HintPages4readEP11IStreamBase
void HintPages::read(IStreamBase* stream) {
    ReadElementArray(members, size, stream, &HintPages::finalize);
}

// 0x4baf58 _ZN6Arrays9HelpPages4readEP11IStreamBase
void HelpPages::read(IStreamBase* stream) {
    ReadElementArray(members, size, stream, &HelpPages::finalize);
}

// 0x4bb1e8 _ZN6Arrays13TriggerPlates4readEP11IStreamBase
void TriggerPlates::read(IStreamBase* stream) {
    ReadElementArray(members, size, stream, &TriggerPlates::finalize);
}

// 0x4bb844 _ZN6Arrays15LiftableObjects4readEP11IStreamBase
void LiftableObjects::read(IStreamBase* stream) {
    ReadElementArray(members, size, stream, &LiftableObjects::finalize);
}

// 0x4bb978 _ZN6Arrays17GameObjectDamager4readEP11IStreamBase
void GameObjectDamager::read(IStreamBase* stream) {
    ReadElementArray(members, size, stream, &GameObjectDamager::finalize);
}

// 0x4bbe9c _ZN6Arrays11FontPalette4readEP11IStreamBase
void FontPalette::read(IStreamBase* stream) {
    ReadElementArray(members, size, stream, &FontPalette::finalize);
}

// 0x4bc3c0 _ZN6Arrays15CharEffectTable4readEP11IStreamBase
void CharEffectTable::read(IStreamBase* stream) {
    ReadElementArray(members, size, stream, &CharEffectTable::finalize);
}

// 0x4bc8e0 _ZN6Arrays15GameOptionTable4readEP11IStreamBase
void GameOptionTable::read(IStreamBase* stream) {
    ReadElementArray(members, size, stream, &GameOptionTable::finalize);
}

}  // namespace Arrays
