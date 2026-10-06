#pragma once
#include "loot_tables_v2.hpp"
#include <memory>
namespace dh2::loader {
// Original CharTemplate rows preserve repeated CharInfoName IDs: repetitions
// are the authored selection weights, not redundant data to deduplicate.
class CharacterTemplateAssetsV35 {
 struct Snapshot;std::shared_ptr<const Snapshot> snapshot_;
public:
 class Borrow {
  friend class CharacterTemplateAssetsV35;std::shared_ptr<const Snapshot> snapshot_;
  explicit Borrow(std::shared_ptr<const Snapshot> p):snapshot_(std::move(p)){}
 public:
  Borrow()=default;explicit operator bool()const noexcept{return bool(snapshot_);}
  const std::vector<std::string>& names()const;
  const std::vector<std::vector<std::int32_t>>& rows()const;
  // Non-player, nonempty char_template branch of SafeGetCharPropsId3b3d38.
  // The caller lends the SAME Character caches13c8/13ca and application RNG
  // channel0. No private RNG, player/save branch, spawn roll or activation.
  bool select(const std::string&,std::int16_t& properties13c8,std::int16_t& template13ca,
              data::LootRandom8V2&,std::string&)const;
 };
 // Bounded decoding of the complete original template stream/name/schema.
 // Atomic malformed failure; pinned immutable snapshots survive owner death.
 // Reload policy is a native ownership guard, not original Application policy.
 bool load(data::Bytes records,data::Bytes names,data::Bytes schema,std::string&);
 Borrow borrow()const{return Borrow(snapshot_);}
};
}
