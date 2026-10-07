#pragma once
#include <canonical_door_v27.hpp>
#include <canonical_trigger_object_v28.hpp>
#include <openable_container_owner_v1.hpp>
#include <destructible_container_data_v16.hpp>
#include <array>
namespace dh2::loader {
// One native immutable successor of PyDataArrays' registered grouped file.
// Every family view aliases the SAME snapshot; no table or World duplication.
class GameObjectArraysOwnerV81 final {
 struct Snapshot;std::shared_ptr<const Snapshot> snapshot_;
public:
 struct GroupSpan {const char* name{};std::uint32_t count{};std::size_t records_offset{},records_size{},names_offset{},names_size{};bool names_read{};};
 struct Borrow {
  std::shared_ptr<const void> receiver;
  world::DoorTableBorrowV77 doors;
  world::TriggerObjectTableBorrowV78 triggers;
  std::shared_ptr<const world::OpenableContainerTableV1> openable;
  std::shared_ptr<const world::DestructibleContainerTableV16> destructible;
  std::shared_ptr<const data::GameObjectDictionaryV11> dictionary;
 };
 bool initialize(const std::uint8_t* records,std::size_t,const std::uint8_t* names,std::size_t,
  const std::uint8_t* dictionary,std::size_t,const std::uint8_t* dictionary_names,std::size_t,std::string&);
 bool borrow(Borrow&,std::string&)const;
 bool ready()const noexcept{return bool(snapshot_);}
};
}
