#pragma once
#include "../game-data/data.hpp"
#include <map>
#include <memory>
namespace dh2::world {
// Retained authored projection of the selected LevelConfig XML record. This
// bounded additive format is not the complete original XML object factory.
class LevelConfigMusicOwnerV1 {
 std::string source_;std::map<std::string,std::string> attributes_;
 std::uint8_t combat_music_enabled_{};
public:
 static std::unique_ptr<LevelConfigMusicOwnerV1> load(data::Bytes,std::string&);
 const std::map<std::string,std::string>& attributes()const noexcept{return attributes_;}
 const std::string& source()const noexcept{return source_;}
 const std::uint8_t* combat_music_enabled()const noexcept{return &combat_music_enabled_;}
 std::uintptr_t identity()const noexcept{return reinterpret_cast<std::uintptr_t>(this);}
 const std::string* attribute(const char*)const noexcept;
};
}
