#pragma once
#include "data.hpp"
#include <memory>
namespace dh2::data {
struct FaeryProjection36 {std::uint32_t words[9];};
struct FaerySpan16 {const std::uint8_t* data;std::uint32_t bytes,reserved;};
struct FaerySummary16 {std::uint32_t lists,faeries,list_end,records_end;};
static_assert(sizeof(FaeryProjection36)==36&&sizeof(FaerySpan16)==16&&sizeof(FaerySummary16)==16);
struct FaeryRecord {FaeryProjection36 scalar;std::string script;};
// Immutable native ownership of the original first FaeryList and Faery table.
// Pointer/vtable words are normalized to zero; all raw fields/string bytes stay
// intact. No equipped-faery/SpellList/Application producer is synthesized.
class FaeryTables {
 struct Snapshot;std::shared_ptr<const Snapshot> snapshot_;
public:
 class Borrow {
  friend class FaeryTables;std::shared_ptr<const Snapshot> snapshot_;
  explicit Borrow(std::shared_ptr<const Snapshot> p):snapshot_(std::move(p)){}
 public:
  Borrow()=default;explicit operator bool()const noexcept{return bool(snapshot_);}
  const std::vector<std::vector<std::int32_t>>& lists()const;
  const std::vector<FaeryRecord>& faeries()const;
  const std::vector<std::string>& list_names()const;
  const std::vector<std::string>& faery_names()const;
  const std::vector<std::string>& list_fields()const;
  const std::vector<std::string>& faery_fields()const;
  std::int32_t list_index(const char*)const;std::int32_t faery_index(const char*)const;
  std::size_t list_table_consumed()const;std::size_t records_consumed()const;
  std::size_t names_consumed()const;std::size_t schema_consumed()const;
 };
 // Atomic, retains owned payload bytes. Borrow denies reload and keeps data
 // alive after the facade dies. Subsequent schema sections remain unconsumed.
 bool load(Bytes records,Bytes names,Bytes schema,std::string& error);
 Borrow borrow()const{return Borrow(snapshot_);}
};
}
// 0 valid/1 malformed. Disjoint aligned outputs, unaligned inputs accepted.
extern "C" unsigned dh2_faery_decode_record(dh2::data::FaeryProjection36*,
 dh2::data::FaerySpan16*,std::uint32_t*,const std::uint8_t*,std::uint32_t);
extern "C" unsigned dh2_faery_decode_list(dh2::data::FaerySpan16*,
 std::uint32_t*,const std::uint8_t*,std::uint32_t);
extern "C" unsigned dh2_faery_tables_measure(dh2::data::FaerySummary16*,
 const std::uint8_t*,std::uint32_t);
