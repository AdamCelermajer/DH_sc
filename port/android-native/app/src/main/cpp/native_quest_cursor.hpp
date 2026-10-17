#pragma once
#include "player_profile_index_v1.hpp"
#include "player_saved_quests_v1.hpp"

namespace dh2::native::quests {
// Native memory transport for the retained WHOLE campaign stream. Savegame::load
// seeks to the section's absolute offset before QEST delivery; both Quest logs
// and every Objective reader must borrow this same identity and cursor.
// This is a platform adapter, not a recovered IStream/assertion implementation.
// Partial reads report their actual byte count; the selected typed readers own
// assertion ordering. Seeking beyond the retained file is a native safety guard.
class Cursor {
 data::PlayerProfileIndexV1::Borrow profile_;
 data::player_saved_quests_v1::StreamRef stream_;
 std::uint64_t position_=0;
 bool matches(const data::player_saved_quests_v1::StreamRef&) const noexcept;
 bool protected_storage(const void*,std::size_t) const noexcept;
public:
 Cursor(data::PlayerProfileIndexV1::Borrow,const char* section);
 Cursor(const Cursor&)=delete;Cursor& operator=(const Cursor&)=delete;
 data::player_saved_quests_v1::StreamRef& stream() noexcept{return stream_;}
 bool tell(const data::player_saved_quests_v1::StreamRef&,std::uint64_t*) const noexcept;
 bool seek(const data::player_saved_quests_v1::StreamRef&,std::uint64_t absolute) noexcept;
 bool read(const data::player_saved_quests_v1::StreamRef&,void*,std::uint64_t requested,std::uint64_t* delivered) noexcept;
};
}
