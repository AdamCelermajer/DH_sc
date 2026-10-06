#pragma once
#include "character_level.hpp"
#include "../game-data/ai.hpp"
#include "../game-data/game_design_tables.hpp"
#include "../game-data/level_tables.hpp"
#include "../script-runtime/script_constants.hpp"
#include <array>
#include <memory>

namespace dh2::character {
struct GameDesignTableInput48 {data::Bytes records,names,schema;};
struct GameDesignInputs256 {
 GameDesignTableInput48 characters,classes,ai,factions,levels;
 // Exact caller-supplied load order; no inferred Application startup order.
 const data::Bytes* constants=nullptr;
 std::uint32_t constant_count=0,reserved=0;
};
struct GameDesignRegistration32 {
 const char* group;
 std::uint32_t source_call,source_getter;
 const data::DesignNames16* members;
 std::uint64_t reserved;
};
struct GameDesignConstantLoad24 {
 std::int32_t status;
 std::uint32_t bytes;
 dh2_script_constants_reload source;
};
static_assert(sizeof(GameDesignTableInput48)==48&&sizeof(GameDesignInputs256)==256);
static_assert(sizeof(GameDesignRegistration32)==32&&sizeof(GameDesignConstantLoad24)==24);

// Native ownership adapter over individually proved cache/registry/constant
// kernels. Snapshot is pinned on the heap; no live Application reload policy.
class CharacterGameDesign {
 struct Snapshot;
 std::shared_ptr<Snapshot> snapshot_;
public:
 class Borrow {
  friend class CharacterGameDesign;
  std::shared_ptr<const Snapshot> snapshot_;
  explicit Borrow(std::shared_ptr<const Snapshot> snapshot):snapshot_(std::move(snapshot)){}
 public:
  Borrow()=default;
  Borrow(const Borrow&)=delete;
  Borrow& operator=(const Borrow&)=delete;
  Borrow(Borrow&&)=default;
  Borrow& operator=(Borrow&&)=default;
  explicit operator bool() const{return bool(snapshot_);}
  const data::CharacterTable* characters() const;
  const data::ClassTables* classes() const;
  const data::AiTables* ai() const;
  const data::LevelTables* levels() const;
  const data::PropertyRules* rules() const;
  const std::vector<data::ClassRow>* class_rows() const;
  const std::array<GameDesignRegistration32,6>* registrations() const;
  const std::vector<GameDesignConstantLoad24>* constant_loads() const;
  const dh2_script_design_bindings* design() const;
  // Output changed only on success. Caller owns the mutable PropertyView and
  // its genuine base/saved/gear/resolved/buff backing for the entire model use.
  bool level_model(LevelModel32&,data::PropertyView&,std::string& error) const;
  int bind(dh2_script_vm*) const;
 };
 CharacterGameDesign()=default;
 CharacterGameDesign(const CharacterGameDesign&)=delete;
 CharacterGameDesign& operator=(const CharacterGameDesign&)=delete;
 CharacterGameDesign(CharacterGameDesign&&)=delete;
 CharacterGameDesign& operator=(CharacterGameDesign&&)=delete;
 // Copies all streams before decoding. Any failure leaves old snapshot intact.
 // Reject reload while any Borrow exists. A Borrow also retains the snapshot
 // after owner destruction: caller must retain it through VM close/finalizers.
 bool initialize(const GameDesignInputs256&,std::string& error);
 Borrow borrow() const{return Borrow(snapshot_);}
 bool ready() const{return bool(snapshot_);}
};
}
