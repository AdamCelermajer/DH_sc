# PlayerInfo local RemoveCharacter destination tail V114

READY source-only existing-owner delta. Apply player-info-local-tail-v114.patch to CURRENT port/level-world/player_network_local_owner_v4.hpp/.cpp. Successor pair is context only. No new TU/PM/Net owner; Root target already compiles this existing CPP. No shared edits, tests/builds/validation or network/platform work.

Exact public call:
PlayerNetworkLocalOwnerV4::source_remove_character_local_tail_v114(PlayerInfoFieldsV1&,std::string&)

Order from IDA0037/00371d80.c:
1. Actual member4a0.virtual1c assigns integer0 to value4c0 ALWAYS.
2. Existing SAME selected virtual50/is_local.
3. Local only: class member360/value380=-1; name member2b0/valueCString2d0 empty; level member310/value330=-1.
4. Actual NetStructByteArray36 member3b0: native-width unique buffer3d0 +size3d4 ->SetBuffer(NULL,0).
5. XP member428/value448=0; GOLD member478/value498=0.
6. SetIngame member4c8/value4e5=false, reusing historical existing character_visible4e5_v70 cell.

Important correction:4a0 is NOT NetStructByteArray32. Addresspoint9635e8+1c resolves36d9f0 NetStructMemberType<int>.SetValue; C1 tag32 is integer member type. Do not invent a32-byte array. Ingame base is4c8; earlier identity comments calling4c0 aBool were inaccurate.

Missing typed members/buffer are added to SAME PlayerInfoNetworkIdentityV4. Existing derived C1/Reset hook produces actual36-byte ff buffer (373bdc), not an invented empty body;3/30 skill buffers are preserved. Destination value changes use existing process source_change_counter_v99 / SetChanged814f84 (dirty1c, masks10/14=member18, timestamp8=counter++); equal values retain native early return.

ByteArray36 empty assignment: ifoldsizepositive, actual old storage release ->size0 ->real new[0] ->destination SetChanged. Failed zero-allocation retains buffer_empty_pending receipt before later fields; retry completes that same allocation/dirty suffix. Equal size0 is genuine no-op. Buffer pointers are native-width C++ storage; no ARM pointer casts.

Scope/unfinished:
This implements actual DESTINATION member setter kernels and their same-record backing. Original temporary wrapper constructor/vptr and undefined stack-value-dependent temporary dirty/counter effects are NOT reconstructed or claimed. No serialization/member-declaration delivery/list/backend. Parent owns full RemoveCharacter body, actual source virtuals/gameplay removal and call integration. Genuine missing/replaced same receiver/constructor production fails. Source-first initial allocation failure remains reached constructor prefix, not fake success.

Hold for Root combined integrated milestone; no further work.
