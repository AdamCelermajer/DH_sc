#pragma once
#include "data.hpp"
#include <memory>
namespace dh2::data {
struct SkillProjection76 {std::uint32_t words[19];};
struct SkillSpan16 {const std::uint8_t* data;std::uint32_t bytes,reserved;};
struct SkillSpans48 {SkillSpan16 display,script,icon;};
struct SkillSummary16 {std::uint32_t lists,skills,list_end,records_end;};
static_assert(sizeof(SkillProjection76)==76&&sizeof(SkillSpan16)==16&&sizeof(SkillSpans48)==48);
// Header and three source pointer words are zero in the scalar projection;
// vector counts/string lengths and raw bool bytes remain exact. All dynamic
// payloads are owned separately and retain embedded NUL/string suffix bytes.
struct SkillRecord {SkillProjection76 scalar;std::vector<std::int32_t> display_props;
 std::string script,icon;};
class SkillTables {
 struct Snapshot;std::shared_ptr<const Snapshot> snapshot_;
public:
 class Borrow {
  friend class SkillTables;std::shared_ptr<const Snapshot> snapshot_;
  explicit Borrow(std::shared_ptr<const Snapshot> p):snapshot_(std::move(p)){}
 public:
  Borrow()=default;explicit operator bool()const noexcept{return bool(snapshot_);}
  const std::vector<std::vector<std::int32_t>>& lists()const;
  const std::vector<SkillRecord>& skills()const;
  const std::vector<std::string>& list_names()const;
  const std::vector<std::string>& skill_names()const;
  const std::vector<std::string>& list_fields()const;
  const std::vector<std::string>& skill_fields()const;
  std::int32_t list_index(const char*)const;std::int32_t skill_index(const char*)const;
  std::int32_t list_field(const char*)const;std::int32_t skill_field(const char*)const;
  std::size_t list_table_consumed()const;std::size_t records_consumed()const;
  std::size_t names_consumed()const;std::size_t schema_consumed()const;
 };
 // Complete first SkillList and following Skill table plus two name/schema
 // blocks. No synthesized IDs/defaults/reference rewriting. Error is atomic;
 // reload is denied while a Borrow pins. Suffixes are left for their owner.
 bool load(Bytes records,Bytes names,Bytes schema,std::string& error);
 Borrow borrow()const{return Borrow(snapshot_);}
};
}
// 0success/1malformed, atomic outputs. Input may be unaligned; outputs must be
// aligned and disjoint from inputs/each other. Dynamic spans borrow input bytes.
extern "C" unsigned dh2_skill_decode_record(dh2::data::SkillProjection76*,
 dh2::data::SkillSpans48*,std::uint32_t* used,const std::uint8_t*,std::uint32_t);
extern "C" unsigned dh2_skill_decode_list(dh2::data::SkillSpan16*,std::uint32_t* used,
 const std::uint8_t*,std::uint32_t);
extern "C" unsigned dh2_skill_tables_measure(dh2::data::SkillSummary16*,
 const std::uint8_t*,std::uint32_t);
