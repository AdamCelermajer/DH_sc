# Verification of the round-2 Haiku audits (owner VF2)

Method as in VERIFY-HAIKU.md: ARM listing first (tools/decomp_asm.py), Hex-Rays second.

| address | function | claim | verdict | fix / reason |
|---|---|---|---|---|
| 0x80a128 | CMessage::IsExpired | port `<=`, listing unsigned `<` | REAL | CMP R4,R0; MOVCS 0 (deadline>=counter = not expired); port now `<`; c_message_test boundary flipped (equality = not expired) |
| 0x3c91c8 / 0x3c9168 | CharAnimator::Dec/IncAnimSetUsers | null checks | FALSE (already fixed by L2) | guards for visual and set are present and match; the controller (+0x2d8 -> +0x38 chain) is not null-checked in the listing either |
| 0x7fe894 | NetStructArray<MemberInfo,32>::Serialize | presence bit on element 0 only | REAL | bit is gated on the array's HasDataToSend (0x7fe7e8: any element, slot +0x28); SerializeMemberArray now loops. Same pattern fixed in CMatchingLocal::WritePacketData (backup array) |
| 0x81a58c | CSignIn::CSignIn | capacity word +0x24 not stored | FALSE | +0x24 is CEventQueueBase::expiryCheck_ = 100, stored by `events_{100}`; stale "Partial" header comment replaced |
| 0x33394c | Console::_setMenuLoadModuleList | dropped argument | FALSE | LDR R1,[R5,#0xA4] is the STLport string's start pointer (0x90+0x14) = module_pattern_.c_str(), as ported |
| 0x7a98f8 | RenderFX::Initialize(IVideoDriver*) | stores this | FALSE | function is static; R0 is the driver (Hex-Rays prototype wrong), port stores the driver |
| 0x52e15c | CXPlayerManager::OnRequestTimeout | log drops request id | REAL | R1 reaches printf untouched; now `printf("...REQUEST TIMEOUT %d\n", requestId)` (format as the sibling OnRequestFailure) |
| 0x86fe00 | DecoderMPC8Cursor::Seek | ignores sample, seeks 0 | FALSE | listing: R1==0 -> -1; else mpc_demux_seek_sample(r2:r3=0); non-zero -> -1; else +0x20=0. Port matches exactly |
| 0x3699fc | VoxSoundManager::LoadSound | off-by-one (`<=`) | UNDEF (original bug, kept) | write at index sound_count_ would overflow the 8-byte-slot host array; deviation: sounds_/unk_04 allocated with one spare slot, logic unchanged |
| 0x42d19c | MenuManager::GetMenuBelow | reads one past end | confirmed documented deviation | port stops at count-1 |
| 0x89a9a4 | ALicenseCheck::CallJNIFuncChar | extra clamp | confirmed documented deviation | length clamped to max |

Partly-verified rows: the progress files list them almost entirely under net-transport and vox kernels (excluded). The only game-* group is the CharAI
script-slot forwards (OnDied 0x3d1000, OnMasterDied 0x3d1b48, OnTargetDied 0x3d1f60, OnDeAggro 0x3d2014): vtable slots +0x24/+0x60/+0x40/+0x3c
(9/24/16/15) and the +0x78 clear in OnTargetDied match the listing and CharAIScript.hpp; the group call in OnDied precedes the script call. All match.

Result: 3 REAL (fixed), 1 UNDEF (guarded), 6 FALSE/confirmed, 2 deviations confirmed.
Tests: build/own-vf2 net-session-online + game-anim + game-ui + vox-audio: 234/234 pass (one link OOM, relinked).
