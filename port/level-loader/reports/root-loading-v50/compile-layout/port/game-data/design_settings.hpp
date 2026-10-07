#pragma once
#include "data.hpp"
#include <array>
#include <memory>
namespace dh2::data {
// Scalar projection of the complete source176-byte row. Header/vtable word0
// is zero; words1..43 retain all serialized IEEE/integer bits without casts.
struct DesignSettingsProjection176 {std::uint32_t words[44];};
static_assert(sizeof(DesignSettingsProjection176)==176);
enum DesignSettingsKind:std::uint32_t {design_float=0,design_integer=1,design_unknown=2};
DesignSettingsKind design_settings_field_kind(std::uint32_t index);
class DesignSettingsOwner {
 struct Snapshot;std::shared_ptr<const Snapshot> snapshot_;
public:
 class Borrow {
  friend class DesignSettingsOwner;std::shared_ptr<const Snapshot> snapshot_;
  explicit Borrow(std::shared_ptr<const Snapshot> p):snapshot_(std::move(p)){}
 public:
  Borrow()=default;
  explicit operator bool()const noexcept{return bool(snapshot_);}
  const std::vector<DesignSettingsProjection176>& rows()const;
  const std::vector<std::string>& row_names()const;
  const std::vector<std::string>& fields()const;
  std::size_t records_consumed()const;
  std::size_t names_consumed()const;
  std::size_t schema_consumed()const;
  std::int32_t field_index(const char*)const;
  std::int32_t row_index(const char*)const;
  const std::uint32_t* word(std::uint32_t row,std::uint32_t field)const;
  const std::uint32_t* enemy_spotted_aggro_bits(std::uint32_t row=0)const{return word(row,11);}
 };
 // Decode only complete source DesignSettings first table and first name/schema
 // blocks; suffix belongs to other source tables. All fields/names are owned.
 // Bounded malformed rejection is atomic; reload denied while any Borrow pins.
 bool load(Bytes records,Bytes names,Bytes schema,std::string& error);
 Borrow borrow()const{return Borrow(snapshot_);}
};
}
// Complete bounded row decoder:0 success,1 malformed/truncated/overlap. Output
// and used change only on success. Source short-read partialwrites are not
// defined as native success. No synthesized default field values.
extern "C" unsigned dh2_design_settings_decode_record(dh2::data::DesignSettingsProjection176*,
 std::uint32_t* used,const std::uint8_t*,std::uint32_t size);
// Complete first-table prefix decode. Count/used/rows are atomic on rejection.
// 0success,1malformed,2insufficient capacity. Zero-row input permits null rows.
extern "C" unsigned dh2_design_settings_decode_table(dh2::data::DesignSettingsProjection176*,
 std::uint32_t capacity,std::uint32_t* count,std::uint32_t* used,const std::uint8_t*,std::uint32_t size);
