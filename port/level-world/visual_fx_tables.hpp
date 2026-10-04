#pragma once
#include "visual_fx_preload.hpp"
#include "../game-data/effects_tables.hpp"
#include <memory>
#include <vector>
namespace dh2::fx {
// Borrow one immutable decoded effects snapshot for the full level lifetime.
// Only step File/Redir enter registration; remaining metadata stays owned in
// that same snapshot for future factory/playback. No FX instance is created.
class PreloadBacking final {
 data::EffectsTables::Borrow source_;
 std::vector<std::vector<PreloadStep8>> steps_;
 std::vector<PreloadSet16> sets_;
 PreloadTable16 table_{};
 explicit PreloadBacking(data::EffectsTables::Borrow);
public:
 static std::unique_ptr<PreloadBacking> create(data::EffectsTables::Borrow,std::string&);
 PreloadBacking(const PreloadBacking&)=delete;
 PreloadBacking& operator=(const PreloadBacking&)=delete;
 PreloadBacking(PreloadBacking&&)=delete;
 PreloadBacking& operator=(PreloadBacking&&)=delete;
 const PreloadTable16& table()const noexcept{return table_;}
 const data::EffectsTables::Borrow& source()const noexcept{return source_;}
};
}
