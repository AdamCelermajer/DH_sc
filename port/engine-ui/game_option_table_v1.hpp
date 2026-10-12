#pragma once
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>
namespace dh2::ui {
struct GameOptionBytesV1 {const std::uint8_t* data{};std::size_t size{};};
struct GameOptionRow32V1 {
 std::int32_t header_zero{},default_value{},label{},maximum{},minimum{},step{},type{},value_string{};
};
static_assert(sizeof(GameOptionRow32V1)==32);
// Owns the complete third table of original design cache, preserving all seven
// signed serialized words and exact ordered names. Snapshot survives its owner.
class GameOptionTableV1 {
 struct Snapshot;std::shared_ptr<const Snapshot> snapshot_;
public:
 class Borrow {
  friend class GameOptionTableV1;std::shared_ptr<const Snapshot> snapshot_;
  explicit Borrow(std::shared_ptr<const Snapshot> p):snapshot_(std::move(p)){}
 public:
  Borrow()=default;explicit operator bool()const noexcept{return bool(snapshot_);}
  const std::vector<GameOptionRow32V1>& rows()const;
  const std::vector<std::string>& names()const;
  const std::vector<std::string>& fields()const;
  std::size_t records_offset()const;std::size_t names_offset()const;
  std::size_t records_consumed()const;std::size_t names_consumed()const;
 };
 bool load_design_cache(GameOptionBytesV1 records,GameOptionBytesV1 names,GameOptionBytesV1 schema,std::string&);
 Borrow borrow()const{return Borrow(snapshot_);}
};
}
// Isolated complete source row decoder; rejects malformed/overlap atomically.
extern "C" unsigned dh2_game_option_v1_decode_record(dh2::ui::GameOptionRow32V1*,
 std::uint32_t* used,const std::uint8_t*,std::uint32_t size) noexcept;
