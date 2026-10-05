#pragma once
#include "character_target_search.hpp"
namespace dh2::target_search {
// Optional backup iterator Get producer. It resolves each stored identity at
// the actual iteration point, immediately before the existing GetChar query.
struct SnapshotResolve16V5 {void* context;int(*invoke)(void*,const Entry16*,Object48**);};
extern "C" int dh2_target_search_snapshot_v5(List40*,const Registry8*,float radius,
 float cone,const float captured_origin[3],const Services16*,const SnapshotResolve16V5*);
}
