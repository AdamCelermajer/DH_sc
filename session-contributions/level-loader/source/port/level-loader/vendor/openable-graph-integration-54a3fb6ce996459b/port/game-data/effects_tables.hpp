#pragma once
#include "data.hpp"
#include <memory>
namespace dh2::data {
struct EffectStep {
 std::int32_t file=-1,loop=0,play_time=0,pool_size=0,redir=0;
 std::uint8_t force_cancel=0,orient_once=0,orient_with_anchor=0,scale_with_anchor=0,self_illum=0;
 std::uint32_t speed_bits=0;
 std::string subobject;
};
struct EffectSet {
 std::uint8_t force_cache=0;
 std::int32_t loop=0,type=0;
 std::vector<EffectStep> steps;
};
struct CharacterEffects {
 std::int32_t blood_death=-1,blood=-1,footprint=-1,swoosh=-1;
 std::uint8_t trigger_floor_fx=0;
};
struct FootstepEffects {
 std::int32_t effect=-1;
 std::string floor_type;
 std::vector<std::int32_t> run_sounds,walk_sounds;
};
// Owned exact-cache tables. Reader allocation/STL layout is a native port
// contract; all source scalar bytes, signed words, float bits and array order
// are retained. This does not build effect instances, pools or scene nodes.
class EffectsTables {
 struct Snapshot;
 std::shared_ptr<const Snapshot> snapshot_;
public:
 class Borrow {
  friend class EffectsTables;
  std::shared_ptr<const Snapshot> snapshot_;
  explicit Borrow(std::shared_ptr<const Snapshot> s):snapshot_(std::move(s)){}
 public:
  Borrow()=default;
  explicit operator bool()const noexcept{return bool(snapshot_);}
  const std::vector<EffectSet>& sets()const;
  const std::vector<CharacterEffects>& characters()const;
  const std::vector<FootstepEffects>& footsteps()const;
  const std::vector<std::string>& set_names()const;
  const std::vector<std::string>& character_names()const;
  const std::vector<std::string>& footstep_names()const;
  const Dictionary& dictionary()const;
  std::size_t set_end()const;
  std::size_t character_end()const;
  std::size_t data_consumed()const;
  std::size_t names_consumed()const;
  std::size_t schema_consumed()const;
 };
 // Failure preserves the previous snapshot. Reload is rejected while a view
 // pins it; borrowed rows/paths remain stable after loader/input destruction.
 bool load(Bytes records,Bytes names,Bytes schema,Bytes dictionary_names,
           Bytes dictionary_paths,std::string& error);
 Borrow borrow()const{return Borrow(snapshot_);}
};
}
