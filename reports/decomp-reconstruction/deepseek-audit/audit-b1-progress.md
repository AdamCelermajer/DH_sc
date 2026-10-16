# Audit B1 progress (Haiku auditor B1, read-only)

Worklist: reports/decomp-reconstruction/deepseek-audit/worklist-b1.csv (715 rows, indices 0-714).
Output: reports/decomp-reconstruction/deepseek-audit/audit-b1.csv (header plus 3 findings).
No source, header, ledger, test, git or build changes were made.

## Status: all 715 rows reviewed
- Original read from recovered/native/hexrays-all chunk files; reconstruction body read from port/decomp-reconstruction.
- Findings: 3 rows in audit-b1.csv.
  - 0x3c91c8 CharAnimator::DecAnimSetUsers: medium, missing null guards on the visual object, the animation controller and
    the returned set (listing verified). 0x3c9168 IncAnimSetUsers has the same gap (listing verified; not on this worklist).
  - 0x3699fc VoxSoundManager::LoadSound: medium, the original accepts sound == sound_count_ (CMP R1,R3; BLE, listing verified)
    and writes one past the end of sounds_. The port keeps the same bound. Reproduced, not introduced.
  - 0x81a58c CSignIn::CSignIn: low, the capacity word (100) at +0x24 is not stored; the header marks it partial. Listing NOT checked.

## Unmatched or hand-checked addresses
- No address comment in source, checked by hand in the headers: 0x327a0c (CMsgRaisedEvent::SetProperties), 0x81a58c (CSignIn),
  0x43f8d0 (tRoomInfo operator=, compiler default), 0x843c60 (channel ctor, defaulted), 0x894904 / 0x894cec / 0x894980
  (SZipFileEntry copy ctor, dtor, ctor: no reconstruction body found, members default).
- 0x877680 setup_malloc: upstream stb_vorbis (third-party), not compared.
- NativeGetPlayerChar 0x43c388: defined in g1-game-app-global-f216b6-9.cpp (the grep missed it).

## Rows only partly compared (truncated bodies, checked against the visible part and the header notes)
- Long bodies whose tails were not read: CMatching/CMatchingLocal rows (0x805c1c, 0x807bec, 0x80585c tail), NetStruct
  Load/Acknowledged/Lost (0x814774, 0x814d28), NetBitStream/NetStructByteArray Read (0x8182e0), CPacketManager::ReceivePackets,
  ReassignPlayer, GetListenNetworkId, VoxNativeSubDecoder mix/decode (0x8844e8, 0x885258, 0x888050, 0x875cc8),
  VoxSoundManager::PlayMusic/Pause/Play tails, LoadXML (0x88d344), DecoderNativeCursor ctor/reset, EmitterObj Pause/Play.
- CDataStats<int>::GetSum (0x825be8): rb-tree walk order was matched by description, not step by step.

## Items to know about (not reconstruction divergences)
- 0x86fe00 DecoderMPC8Cursor::Seek: the listing passes 0 as the 64-bit target (MOV R2,#0; MOV R3,#0) and ignores the requested
  sample. The port reproduces it (seeks to 0). This is an original quirk worth a look if the seek path matters.
- 0x370430 / 0x8182e0 / 0x7fee00 NetStructByteArray::Read: the original copies a peer-controlled count into a fixed stack buffer
  (overflow for count > N). The port reads into a heap vector. Documented in the source.
- 0x47124c VisualObject::ApplyPosition: stale PARTIAL comment; the body calls m_owner->SetPosition(pos, false) and matches the
  listing.
- 0x8651c0 / 0x865078: dirty-slot and field mapping checked against the header offsets (+0x9c position, +0xa8 direction,
  +0xb4 velocity); they match.

## Recurring false-positive sources in this pass
- Hex-Rays `this` that is really the first argument of a static function (LootEntry, RenderFX::Find, ClearFonts).
- Dropped call arguments (Invoke's int, Cmd_MoveTo's position, GetTargetPosition in the ARM listing).
- Stale PARTIAL comments in finished code (ApplyPosition; NetStructArray::ProcessLostPacket).
- Return-value leftovers of void slots (not observable).
