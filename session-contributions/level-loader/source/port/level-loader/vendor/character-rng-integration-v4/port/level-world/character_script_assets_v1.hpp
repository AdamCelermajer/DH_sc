#pragma once
#include "character_script_session.hpp"
#include "../game-data/skill_tables.hpp"
#include "../game-data/faery_tables.hpp"
#include <functional>

namespace dh2::character {
// Native cache providers. A successful directory/read describes actual cache
// contents; missing files are never replaced with an empty Lua program.
struct ScriptAssetServicesV1 {
 std::function<bool(std::vector<std::string>&,std::string&)> directory;
 std::function<bool(const std::string&,bool&,std::vector<std::uint8_t>&,std::string&)> read;
};
class CharacterScriptAssetsV1 {
 struct Snapshot;
 std::shared_ptr<const Snapshot> snapshot_;
public:
 class Borrow {
  friend class CharacterScriptAssetsV1;
  std::shared_ptr<const Snapshot> snapshot_;
  explicit Borrow(std::shared_ptr<const Snapshot> p):snapshot_(std::move(p)){}
 public:
  Borrow()=default;
  explicit operator bool()const noexcept{return bool(snapshot_);}
  const std::vector<ScriptSessionFile>& files()const;
  const std::vector<std::uint8_t>* find(const std::string&)const;
  data::Bytes common()const;
  // Existing sessions own common/external separately and reject duplicates.
  // Includes retain actual table spellings over the case-folding ZIP provider.
  bool session_files(const std::string& external,std::vector<ScriptSessionFile>&,std::string&)const;
  data::SkillTables::Borrow skills()const;
  data::FaeryTables::Borrow faeries()const;
  const std::vector<std::string>& missing_scripts()const;
  std::size_t bytes()const noexcept;
 };
 // Loads all actual .luac entries, the source Faery tables, and pins the
 // caller's existing SkillTables authority. Atomic; live borrows deny reload.
 // This supplies resources only, not player initialization or a campaign save.
 bool load(const ScriptAssetServicesV1&,data::SkillTables::Borrow,std::string&);
 Borrow borrow()const{return Borrow(snapshot_);}
};
}
