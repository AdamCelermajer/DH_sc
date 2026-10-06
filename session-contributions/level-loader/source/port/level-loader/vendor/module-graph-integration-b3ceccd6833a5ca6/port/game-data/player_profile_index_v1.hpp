#pragma once
#include "data.hpp"
#include <memory>
namespace dh2::data {
struct ProfileSection12V1 {std::uint8_t tag[4];std::uint32_t offset,size;};
struct ProfileIndexSpan24V1 {const std::uint8_t* bytes;std::uint32_t size,cursor,source_count,reserved;};
struct ProfileIndexServices16V1 {void* context;bool (*section)(void*,const ProfileSection12V1*);};
static_assert(sizeof(ProfileSection12V1)==12&&sizeof(ProfileIndexSpan24V1)==24&&sizeof(ProfileIndexServices16V1)==16);
// Source campaign cache's count/size/four-byte-tag/payload layout. This is not
// the raw dh2_settings.savegame option stream. Filename/file backup providers,
// section dispatch order and fresh-player producers are external ownership.
class PlayerProfileIndexV1 {
 struct Snapshot;std::shared_ptr<const Snapshot> snapshot_;
public:
 class Borrow {
  friend class PlayerProfileIndexV1;std::shared_ptr<const Snapshot> snapshot_;
  explicit Borrow(std::shared_ptr<const Snapshot> p):snapshot_(std::move(p)){}
 public:
  Borrow()=default;explicit operator bool()const noexcept{return bool(snapshot_);}
  const std::vector<std::uint8_t>& bytes()const;
  const std::vector<ProfileSection12V1>& source_sections()const;
  // Source C-string tag identity and duplicate last-write behavior. Payload
  // view remains alive with this Borrow. Unknown tag is a genuine miss.
  const ProfileSection12V1* section(const char* tag)const noexcept;
  Bytes payload(const char* tag)const noexcept;
 };
 // Safe atomic reader; borrowed snapshot denies reload. Malformed/truncated
 // spans and -1 corruption marker fail, requiring the genuine file/backup
 // owner. Does not silently treat them as an empty/new character profile.
 bool load(Bytes,std::string&);
 Borrow borrow()const{return Borrow(snapshot_);}
};
}
// Executes the source index loop with explicit section-storage delivery.
// 0 delivered,-1 malformed native entry,-2 truncation/unsafe seek,-3 provider
// refusal,-4 source corruption marker. Reached callback prefix is retained.
extern "C" int dh2_player_profile_v1_index(dh2::data::ProfileIndexSpan24V1*,const dh2::data::ProfileIndexServices16V1*) noexcept;
