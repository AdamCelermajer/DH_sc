#pragma once
#include <cstdint>
namespace dh2::character {
// Exact source current17 CSM_Spawn inputs. GroupInfo::CanSpawn3d24fc is an
// original mov r0,0/bx lr body; a nonnull source group therefore rejects.
struct SpawnPermission16 {std::uintptr_t group;std::uint32_t auto_spawn,reserved;};
static_assert(sizeof(SpawnPermission16)==16);
}
extern "C" {
//1 complete,-1 malformed atomic. Result preserves the raw source auto_spawn
// byte0..255 when groupnull, else0. This is ONLY current17; Limbus0 CanRespawn
// remains a distinct required backend. No current/state/next word is changed.
int dh2_character_pre_spawn_permission(std::uint32_t*,const dh2::character::SpawnPermission16*);
}
