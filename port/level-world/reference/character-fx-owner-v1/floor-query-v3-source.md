AnimatedFX::SyncIrrData 0x492aa0 (1004 bytes) calls PFWorld::GetFloorHeightAt
0x525508 with position, NULL height, normal in/out, NULL room, NULL floor and
includeSpecial=false. The original ignores its boolean result.

The no-anchor branch initializes the temporary normal to zero (492cf0..d00)
before the call at492e78. The anchor branch initially copies GameObject+1ec
(492d08..20); a zero squared length skips the query. For nonzero anchor normal,
the call at492da8 preserves the incoming normal on a miss. If the returned
normal is zero, the original writes (0,0,1) to the temporary (492df8..e08).

Crucially, both branches jump to492be8: visual SetPosition0x470c24 followed by
return. There is NO normal-to-rotation conversion or matrix store. Rotation is
only supplied by fixed rotation or the actual anchor rotation branch
(492b3c..bb8 /492cd4..ce0). The temporary floor normal is discarded by original
source too. CharacterMeshFxOwnerV2 therefore must not invent floor alignment.

character_fx_floor_query_v3 is the real PFWorld adapter. A normal ordinary miss
completes the source call; invalid native input remains a required failure. It
does not manufacture an up normal. Renderer binds the same live level
native_floor->collision_world and rebinds on retained world replacement.
