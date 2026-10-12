#pragma once
#include "canonical_gameobject_base_owner_v1.hpp"
#include "openable_container_property_connection_v1.hpp"
namespace dh2::world {
// Logical original NetStructContainer constructor. Owns real declared members,
// not an emulated foreign vtable. Network codecs/transmission are separate.
class ContainerNetStructV1 {
public:
 struct Member {
  std::uint32_t mask{};
  std::uint64_t raw138{};
  std::int32_t raw140=-1,raw144=-1;
  std::uint32_t raw148{};
  std::uint8_t changed14c{};
  std::int32_t value150{};
 };
private:
 std::array<Member,3> members_{};
 std::array<Member*,64> declared_{};
 std::uint32_t count104_{};
 std::uint8_t byte108_{},byte10c_{},byte124_{},byte125_{};
 std::uint32_t raw110_{},count11c_{},raw128_{};
 std::uintptr_t left114_{},right118_{};
public:
 ContainerNetStructV1();
 ContainerNetStructV1(const ContainerNetStructV1&)=delete;
 std::uint32_t count()const noexcept{return count104_;}
 Member* member(std::size_t i)noexcept{return i<count104_?declared_[i]:nullptr;}
 const Member* member(std::size_t i)const noexcept{return i<count104_?declared_[i]:nullptr;}
 // Source offsets within NetStructBase, for the eventual real network codec.
 // An unsupported field is absent, rather than an invented storage location.
 std::uint8_t* byte(std::uint32_t offset)noexcept{
  switch(offset){case 0x108:return &byte108_;case 0x10c:return &byte10c_;
   case 0x124:return &byte124_;case 0x125:return &byte125_;default:return nullptr;}
 }
 std::uint32_t* raw(std::uint32_t offset)noexcept{
  switch(offset){case 0x110:return &raw110_;case 0x11c:return &count11c_;
   case 0x128:return &raw128_;default:return nullptr;}
 }
 std::uintptr_t* pointer(std::uint32_t offset)noexcept{
  switch(offset){case 0x114:return &left114_;case 0x118:return &right118_;default:return nullptr;}
 }
};
// Actual class receiver for factory index25/GO_ID7. Runtime belongs to its
// one retained canonical context. This class never copies pose/PF/lifecycle.
class CanonicalOpenableContainerV1 {
 CanonicalGameObjectBaseOwnerV1 base_;
 std::array<ContainerNetStructV1,2> network_;
 OpenableContainerFieldsV1 fields_;
 OpenableContainerPropertyConnectionV1 property_;
 OpenableContainerOwnerV1 receiver_;
public:
 CanonicalOpenableContainerV1(std::shared_ptr<void> world_pin,actor::RuntimeState&,
                            OpenableContainerServicesV1);
 CanonicalOpenableContainerV1(const CanonicalOpenableContainerV1&)=delete;
 CanonicalGameObjectBaseOwnerV1& base()noexcept{return base_;}
 OpenableContainerFieldsV1& fields()noexcept{return fields_;}
 OpenableContainerOwnerV1& receiver()noexcept{return receiver_;}
 ContainerNetStructV1& network(std::size_t index){return network_.at(index);}
 CanonicalObjectBorrowV1 canonical(std::shared_ptr<void> receiver_lease){return base_.canonical(std::move(receiver_lease));}
 CanonicalPropertyActorV1 properties()noexcept;
};
}
