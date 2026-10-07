#pragma once
#include "xml_document_v1.hpp"
#include <array>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>
namespace dh2::loader {class CanonicalLevelContextV1;}
namespace dh2::application {
// Primitive CAttributes getters on the SAME attribute receiver/index.
struct TweakAttributesV90 {
 std::shared_ptr<void> owner;
 std::function<bool(std::int32_t,std::string&,std::string&)> name,string_value;
 std::function<bool(std::int32_t,float&,std::string&)> float_value;
 std::function<bool(std::int32_t,std::uint32_t&,std::string&)> color_value;
};
struct TweakLightV90 {
 std::shared_ptr<void> owner;std::uintptr_t identity{};
 std::function<bool(const std::array<float,3>&,std::string&)> attenuation,ambient,diffuse,specular;
};
struct PlayerLightTweakServicesV90 {
 std::shared_ptr<void> provider; // Independent native authority; never App/World.
 std::function<bool(std::uintptr_t,TweakLightV90&,std::string&)> light;
 std::function<bool(const std::array<std::uint8_t,4>&,std::string&)> driver_fog_color;
 std::function<bool(float,float,std::string&)> driver_fog_distances;
 // NULL current Level is a genuine source return, represented by false out.
 std::function<bool(std::shared_ptr<loader::CanonicalLevelContextV1>&,std::string&)> current_level;
 // Fresh GetLevelConfig per call; scalar stores preserve source B/R/G order.
 std::function<bool(const std::shared_ptr<loader::CanonicalLevelContextV1>&,std::uint32_t,const std::array<float,3>&,std::string&)> config_color;
 std::function<bool(const std::shared_ptr<loader::CanonicalLevelContextV1>&,std::uint32_t,std::int32_t,std::string&)> config_int;
 std::function<bool(const std::array<float,3>&,std::string&)> scene_direction;
 std::function<bool(const std::shared_ptr<loader::CanonicalLevelContextV1>&,std::string&)> level_enable_fog;
 // Genuine __aeabi_f2iz only needed outside bounded finite signed-int domain.
 std::function<bool(float,std::int32_t&,std::string&)> exceptional_float_to_int;
};
class PlayerLightTweakerOwnerV90 {
 struct Mapping {std::uint32_t type,offset;void* cell;};
 struct Group {std::string name;std::vector<std::string> variables;std::vector<Group> children;};
 std::map<std::string,Mapping> mappings4_;
 Group group1c_;std::string name54_; // source50 points at this SAME group1c.
 std::array<std::array<float,3>,5> attenuation7c_{};
 std::array<std::array<std::uint8_t,4>,5> ambientb8_{},diffusecc_{},speculare0_{};
 // No original C1 stores at f4/110/114. Deliberately not value-initialized.
 std::array<std::uint8_t,4> fogf4_;std::string light_typef8_;
 float start110_,end114_;float flag118_{0.0f};std::array<float,3> direction11c_{};
 std::array<std::uintptr_t,5> lights128_{};
 bool color_produced_{},start_produced_{},end_produced_{},attempted_{},constructed_{},published_label_{},closed_{},busy_{};
 bool register_cell(const std::string&,std::uint32_t,std::uint32_t,void*,std::string&);
 bool on_set_value(const std::string&,const PlayerLightTweakServicesV90&,std::string&);
public:
 PlayerLightTweakerOwnerV90() {} // User-provided: NEVER zero-fill unproducedf4/110/114.
 ~PlayerLightTweakerOwnerV90();
 PlayerLightTweakerOwnerV90(const PlayerLightTweakerOwnerV90&)=delete;
 using Read=std::function<bool(const std::string&,bool&,std::vector<std::uint8_t>&,std::string&)>;
 // C1 registration -> source loadXML. Authored XML values are NOT assigned.
 bool construct(const Read&,std::string&);
 bool assign_light_type_after_publication(const std::string&,std::string&);
 bool set_value(const TweakAttributesV90&,std::int32_t,const PlayerLightTweakServicesV90&,std::string&);
 // Exact source startup writes occur through these SAME cells. The notifier
 // records completed writes only; it does not produce bytes or call onSet.
 std::uint8_t* fog_color_f4()noexcept{return closed_?nullptr:fogf4_.data();}
 float* fog_start110()noexcept{return closed_?nullptr:&start110_;}
 float* fog_end114()noexcept{return closed_?nullptr:&end114_;}
 float* fog_direction11c()noexcept{return closed_?nullptr:direction11c_.data();}
 void source_fog_written()noexcept{color_produced_=start_produced_=end_produced_=true;}
 std::uintptr_t identity()const noexcept{return reinterpret_cast<std::uintptr_t>(this);}
 bool constructed()const noexcept{return constructed_&&!closed_;}
 const std::string& tweakable_name54()const noexcept{return name54_;}
 const std::string& light_typef8()const noexcept{return light_typef8_;}
 // Future ORIGINAL publication must write these actual pointer slots and
 // lend a matching real LightBase owner via Services.light. C1 leaves NULL.
 std::uintptr_t* source_light128(std::size_t i)noexcept{return !closed_&&i<5?&lights128_[i]:nullptr;}
 //Actual LightPoint.AssignTweaker40b828 writes these SAME registered cells.
 bool source_assign_light_v113(std::uint32_t,std::uintptr_t,const std::array<float,3>&,
  const std::array<float,3>& ambient,const std::array<float,3>& diffuse,
  const std::array<float,3>& specular,std::string&);
 // Source D1: derived stringf8, base vector70(empty), name54, group1c,
 // mappings4. Device6c is borrowed; no device/light grab/drop was in D1.
 bool close(std::string&);
};
}
