# Audit B2 progress (read-only bug hunt, worklist-b2.csv)

Output: `audit-b2.csv` (5 findings). No source, header, ledger or test was edited; no builds, no git, no subagents.
Scratch helpers live in the session scratchpad (b2_*), not in the repo.

## Status: 717 of 717 rows visited (worklist order, rows 0-716)

Method per row: original body (hexrays-all chunk up to the next marker) read against the reconstruction body (address comment in
port/decomp-reconstruction/*/src, symbol fallback). The ARM listing (tools/decomp_asm.py) was read where the pseudocode dropped or
mis-typed arguments (Draw3DWireFrameBox, Console::_setMenuLoadModuleList, Script_SetStatic::Execute, Script_SetStatic argument order,
MenuMinimap::Show switch key, RenderFX::Initialize, CConnection/CNetPlayerManager call sites, CTcp/Bluetooth ctors, NetStructArray Serialize,
LCAndroidSocket::ConnectTo, CMsgInitialSetup ctor, IsLifting table).

## Findings (audit-b2.csv): 5 rows

| severity | function | kind | status |
|---|---|---|---|
| medium | NetStructArray<CMatching::MemberInfoNetStruct,32>::Serialize (0x7fe894), recon CMatching::SerializeMemberArray | compare | listing checked: presence bit uses the array's HasDataToSend (any element). Port tests members_[0] only. Real sync difference. |
| low | Console::_setMenuLoadModuleList (0x33394c) | argument | original passes the word at Console+0xA4 to getFiles. Port passes module_pattern_. Layout question, unresolved. |
| low | RenderFX::Initialize(IVideoDriver*) (0x7a98f8) | argument | deprecated overload stores RenderFX `this` in the driver field (listing), port stores the driver. No caller in the port. |
| low | CXPlayerManager::OnRequestTimeout (0x52e15c) | other | timeout log drops the request id the original prints. Log text only. |
| low | CharAnimator::IncAnimSetUsers (0x3c9168) | other | original guards the visual and set pointers, port does not. Caller guarantee not verified. |

## Checked and not reported (examples)
- Draw3DWireFrameBox: renderer-id cache at manager+0x2E is createMaterialRenderer(1), which is idempotent (listing confirms the 0xFFFF test).
- CharAnimator ctor (all 17 stores), CharStateMachine SM_SetAnim / SM_SetSkillState / SM_IsLifting (table bytes at index 4,5 are 1).
- Script_SetStatic::Execute: GetObjectByName(name, module, false, null) matches the listing argument order.
- MenuMinimap::Show: switch key is "IsDisablingMinimap" (listing).
- CSMove / CSAttack / CSInjured / CSAnim OnInit tables, ObjectiveTemplate Compile, SG_DeleteCheckpoint (checkpoint=1, multi=a2), ~40 ScriptCmd/Objective/Condition/Reward factories.
- CConnection::Update (connected test and timeout use the same time base, CheckForLag does not write last_recv).
- KeepAliveConnections: same steady_clock base as online_calls::Now (no clock mismatch).
- ConnectTo / FinishConnectProgress / SetNonBlocking state machine (matches the original paths).
- ReadU32, WriteBit/ReadBit, SetBuffer, CNetPlayerManager dtor, NetStruct HasDataToSend (single-member test is the only divergence, above).
- CSocket / CTcp CreateBySocketId: the original also writes CSocket-vtable slot 5 result to CTcp+0x12; no reader of that byte in the CTcp functions, so not reported.

## Deliberate deviations seen (documented in the recon; not reported)
Out-of-range reads returning fallbacks (GetGoodReputationCount index == count, getUserGamePID), SetRMS_PATH without free (matches the
original), RecvFromUnkownIP logging dropped, IsAlive uses the unlocked check (original self-deadlocks, documented), null guards added
(SendInvitation, ~CAndroidSocket join guard, SendWaitingPackets, GetPlayerInternal), SetBuffer clamp, Select with fd >= 1024 returns -1.

## Unmatched rows
- No worklist row is missing from the port; the NOT-FOUND rows below resolved by header or ledger text.
  NetBitStreamArray dtor, NetStructInterpolatedRangedFloat ctor, CMsgControllerAction ctor, CMsgInitialSetup ctor/SetProperties,
  CMsgGlobalDeath::SetProperties, NetStructString dtor).
- NOT RECONSTRUCTED: 0x8775f4 setup_temp_malloc and 0x876338 stb_vorbis_set_stream (upstream stb_vorbis, vox-audio/upstream). No body in
  port src; upstream library code, not compared.

## Partly compared (entry and visible stores read, long tails not read)
- GraphSparse heap helpers, AlgoAStar, ITest (inline template bodies).
- CNetPlayerManager::AddPlayer (only the server join branch; ledger row cross-checked).
- CConnection::Update (loop body and key compare read, KeyOf helper not read).
- DecoderNativeCursor::Decode, ParseFile, SetImplicitSegmentCues, DecodeCurrentSegmentWithOffset (recon comments cite listing checks; bodies not re-verified).
- ProcessPacketAcknowledgements, SendPacket, ReceivePackets, ImaDecodeBlock (long bodies; recon comments cite listing checks).
- AddData (CDataStats<int>) template body, Connection::addOutgoingPacket return values (recon documents partial returns).
- CharAI script-slot calls (OnDied, OnTargetDied, OnMasterDied, OnDeAggro): matched by vtable offset, not against each script class declaration.
