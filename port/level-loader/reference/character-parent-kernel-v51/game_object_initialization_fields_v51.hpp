#pragma once
#include "canonical_gameobject_base_owner_v1.hpp"
#include "character_parent_init_fields_v48.hpp"
#include <functional>
namespace dh2::world {
// Storage capability over existing actual cells. No base/runtime allocation.
struct GameObjectInitializationFieldsV51 {
 std::shared_ptr<const void> receiver_lease;
 std::uintptr_t identity_value{};
 const std::int32_t* probability{};std::int32_t* difficulty{};
 float* relative{};float* absolute{};
 std::function<const std::uint8_t*(std::uint32_t)> borrow_byte;
 std::function<std::uintptr_t*(std::uint32_t)> borrow_pointer;
 std::function<const std::string*(std::uint32_t)> borrow_string;
 std::function<float*(std::uint32_t)> borrow_vector;
 std::function<float*(std::uint32_t)> borrow_scalar;
 std::function<std::string*()> borrow_pf_name;
 std::function<bool(std::string&)> checkpoint_clear;
 std::function<bool(std::int32_t,std::string&)> idle_sound_store16;
 std::uintptr_t identity()const noexcept{return identity_value;}
 const std::int32_t* probability274()const noexcept{return probability;}
 std::int32_t* difficulty_ec()const noexcept{return difficulty;}
 const std::uint8_t* byte(std::uint32_t o)const{return borrow_byte?borrow_byte(o):nullptr;}
 std::uintptr_t* pointer(std::uint32_t o)const{return borrow_pointer?borrow_pointer(o):nullptr;}
 const std::string* string(std::uint32_t o)const{return borrow_string?borrow_string(o):nullptr;}
 float* vector3(std::uint32_t o)const{return borrow_vector?borrow_vector(o):nullptr;}
 float* scalar(std::uint32_t o)const{return borrow_scalar?borrow_scalar(o):nullptr;}
 float* relative_aabb144()const noexcept{return relative;}
 float* absolute_aabb12c()const noexcept{return absolute;}
 std::string* pf_name254()const{return borrow_pf_name?borrow_pf_name():nullptr;}
 bool clear_checkpoint(std::string& e)const{if(!checkpoint_clear){e="Required original GameObject checkpoint+28 field";return false;}return checkpoint_clear(e);}
 bool store_idle_sound16(std::int32_t id,std::string& e)const{if(!idle_sound_store16){e="Required original GameObject idle sound id+370 field";return false;}return idle_sound_store16(id,e);}
 void update_absolute_aabb()const{const auto* p=vector3(0x160);for(unsigned i=0;i<6;++i)absolute[i]=relative[i]+p[i%3];}
};
inline GameObjectInitializationFieldsV51 canonical_gameobject_fields_v51(CanonicalGameObjectBaseOwnerV1& base,std::shared_ptr<const void> lease){
 GameObjectInitializationFieldsV51 b;b.receiver_lease=std::move(lease);b.identity_value=base.identity();b.probability=base.integer(0x274);b.difficulty=base.integer(0xec);b.relative=base.relative_aabb144();b.absolute=base.absolute_aabb12c();
 b.borrow_byte=[&base](auto o){return base.byte(o);};b.borrow_pointer=[&base](auto o){return base.pointer(o);};b.borrow_string=[&base](auto o){return base.string(o);};b.borrow_vector=[&base](auto o){return base.vector3(o);};b.borrow_scalar=[&base](auto o){return base.scalar(o);};b.borrow_pf_name=[&base]{return base.string(0x254);};
 b.checkpoint_clear=[&base](auto& e){auto* p=base.byte(0x28);if(!p){e="Required original GameObject checkpoint+28 field";return false;}*p=0;return true;};
 // Existing generic owner uses an int32 logical projection for this source
 // signed16 ID. Preserve it; no extra halfword or copied authority is added.
 b.idle_sound_store16=[&base](auto id,auto& e){auto* p=base.integer(0x370);if(!p){e="Required original GameObject idle sound id+370 field";return false;}*p=static_cast<std::int16_t>(static_cast<std::uint16_t>(id));return true;};return b;
}
inline GameObjectInitializationFieldsV51 character_gameobject_fields_v51(const character::CharacterParentInitFieldsV48& f){
 GameObjectInitializationFieldsV51 b;b.receiver_lease=f.receiver_lease;b.identity_value=f.identity;b.probability=f.probability274;b.difficulty=f.min_difficulty_ec;b.relative=f.relative144;b.absolute=f.absolute12c;
 b.borrow_byte=[f](auto o)->const std::uint8_t*{switch(o){case 0x28:return f.checkpoint28;case 0x60:return f.facultative60;case 0x10c:return f.optimization10c;case 0x15c:return f.skip_checkpoint15c;case 0xac:return f.conditions[0].tested;case 0xd0:return f.conditions[1].tested;default:return nullptr;}};
 b.borrow_pointer=[f](auto o)->std::uintptr_t*{switch(o){case 0x2dc:return f.physical2dc;case 0x2d8:return f.visual2d8;case 0xa8:return f.conditions[0].compiled;case 0xcc:return f.conditions[1].compiled;default:return nullptr;}};
 b.borrow_string=[f](auto o)->const std::string*{switch(o){case 0x90:return f.conditions[0].name;case 0xb4:return f.conditions[1].name;case 0xd4:return f.min_difficulty_d4;case 0x358:return f.idle_sound358;default:return nullptr;}};
 b.borrow_vector=[f](auto o)->float*{switch(o){case 0x120:return f.scale120;case 0x160:return f.position160;case 0x16c:return f.rotation16c;case 0x144:return f.relative144;case 0x12c:return f.absolute12c;case 0x1a8:return f.destination1a8;default:return nullptr;}};
 b.borrow_scalar=[f](auto o)->float*{return o==0x178?f.heading178:nullptr;};
 b.checkpoint_clear=[f](auto& e){if(!f.checkpoint28){e="Required original GameObject checkpoint+28 field";return false;}*f.checkpoint28=0;return true;};
 b.idle_sound_store16=[f](auto id,auto& e){if(!f.idle_sound370){e="Required original GameObject idle sound id+370 field";return false;}*f.idle_sound370=static_cast<std::int16_t>(static_cast<std::uint16_t>(id));return true;};
 // InitFinal-only Character fields are intentionally absent. This view lends
 // qualified parent InitPost storage and never forces InitFinal.
 return b;
}
}
