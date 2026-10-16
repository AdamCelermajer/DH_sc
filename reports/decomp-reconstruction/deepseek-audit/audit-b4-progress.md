# audit-b4 progress (read-only bug hunt, worklist-b4.csv, 718 rows)

Method: original body from recovered/native/hexrays-all (Hex-Rays), reconstruction body from port/decomp-reconstruction
(located by address comment; symbol search for the rest). Suspicious cases were checked against the ARM listing
(tools/decomp_asm.py). Round-1 false-positive classes (dropped/mislabelled arguments, void vs int returns, register residue,
stale PARTIAL comments) were not reported.

Rows done: 718 / 718 (every worklist row was viewed; no row was skipped).
Findings: 2 (audit-b4.csv):
  - 0x80a128 CMessage::IsExpired: port uses <=, listing uses unsigned < (CMP/MOVCC). Low.
  - 0x89a9a4 ALicenseCheck::CallJNIFuncChar: port clamps the copy to max, original copies the full array. Low (defensive).

Coverage quality. Rows fully compared against both bodies and, where suspicious, the listing: most of the set.
Rows where the body is truncated in the viewer or the port is a re-implementation whose equivalence was only partly traced
(treat as NOT fully verified):
  - 418 CDataStats::GetAverage (walk taken as reverse in-order iteration per the port header; window test and average checked)
  - 420 CAndroidSocket::ConnectByName, 421 GetLocalIP (#else path), 453 ProcessConnectionRequest, 454 PacketReceiverCallback,
    490 CNetPlayerInfo ctor, 502-504, 522-524 CPacketManager ctor/dtor/PreparePacketData, 528-536 tails, 541 NetStruct::SetEnabled,
    556/557 IsTransportAvailable / TerminateInternal tails, 568/569 DecoderMSWavCursor ParseFile and ctor,
    570 VoxMSWavSubDecoderIMAADPCM ctor, 586 DoCallbackAT, 590-592 FillBufferMono16/Stereo mix kernels,
    594-596 spatial helpers, 610-617 emitter loops, FileLimited::Seek, OpenFile, 642-644 emulate-segment functions, 677 AddEmitter,
    709-710 EmitterObj::LoadAsync/Update, 713-714 SetPlayCursor/ProcessNonNativeData (port body only partly read).
Not reported (documented in the port, undefined in the original, or no observable effect):
  - 287 Container::InitFinal: PODecor bool argument is register residue in the original (clobbered by operator new); port passes false.
  - 401 CMatchingLocal::ProcessLostPacket: return value is the last void call's register residue; port returns 1.
  - 688 VoxEngineInternal::SetOutputMode: listing shows mode (R1) is forwarded to the driver; port matches (Hex-Rays dropped it).
  - 368/369, 436, 555 (CTransportFactory), 560 (GetHostname): void/bool return or dispatch differences invisible to callers.
  - 414 GLXPlayerWebComponent and 435 CConnection ctors: member default initialisers checked against the original stores; match.
  - 543/544 NetStruct dtor/ctor: members default-initialised as in the original; match.
  - 554-555 NetStructString / InitializeTransport: match.
Unmatched addresses (no address comment in src/, checked by symbol): 0x7fd838 COnline C2 (header comment, defaults match),
  0x842d70 item copy ctor (= default, fields copied in order), 0x81ebc8 tFriendInfo ctor (= default), 0x4a0040 OnlineGameState C1
  (header comment, defaults match), 0x83026c GLXPlayerSocket D2 (unique_ptr members free the two heap buffers),
  0x805730 / 0x8056c0 NetStructArray ProcessAcknowledgedPacket / HasDataToSend (checked: element stride 344, slot 16 / 10),
  0x80e6a8 NetBitStreamArray D1 (not reviewed in full), 0x7fea58 NetStructArray::Erase (partly), 0x3a681c NetStructInterpolatedRangedFloat ctor (not reviewed in full),
  0x84560c / 0x845688 Thread::Stop / Start (not reviewed in full), 0x863870 PriorityBank D1 (not reviewed in full),
  0x87871c init_blocksize and 0x87e750 stb_vorbis_decode_memory (vendored stb_vorbis, not reviewed in full),
  0x877738 setup_temp_free (checked: buffer-range test matches).
