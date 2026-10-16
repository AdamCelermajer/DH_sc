# Audit B3 progress (read-only bug hunt, worklist-b3.csv)

Output: `audit-b3.csv` (1 finding row). No source, header, ledger or test was edited; no builds, no git, no subagents.
Scratch helper (outside the repo): `b3_show.py` in the session scratchpad.

## Status: 718 of 718 rows visited (worklist order), 1 finding

Per row: the original body from `recovered/native/hexrays-all` (up to the next `// =====`), the reconstruction from the
`0x<addr>` comment in `port/decomp-reconstruction/*/src` (falling back to the header comments), then offsets checked against the
headers. The ARM listing (`tools/decomp_asm.py`) was checked wherever a Hex-Rays argument, return or compare looked odd.

Depth varies by row, and this matters for the result:
- Full comparison (body read in full, offsets and calls checked): categories core-platform, game-anim, most of game-app,
  game-character, game-items-save, game-ui, game-world, net-session-online, and the small net-transport rows.
- Entry-level only (the first lines of the original and of the reconstruction, plus the reconstruction's own note): long
  bodies over ~60 pseudo-lines, mainly vox-audio (for example 557 UpdateEmitters, 559 DataObj::Update, 564 DecoderMPC8Cursor::Decode,
  610, 611, 645, 650, 653, 659, 710, 711) and several net-transport state machines (420 Connect, 421 ConnectTo, 444, 445, 522,
  533, 538, 550 ConnectTransport). Their reconstructions carry listing-verified notes; those notes were treated as the check and
  not re-derived. Treat these as "no mismatch seen in the visible part", not as cleared.
- Rows 716 and 717 (`stb_vorbis_decode_filename`, `inverse_mdct_slow`) are upstream third-party code (`vox-audio/upstream`),
  not reconstructed: no recon body, expected.

## Findings (audit-b3.csv)

| severity | function | kind | note |
|---|---|---|---|
| low | MenuManager::GetMenuBelow (0x42d19c) | other, undefined in original | Original's last loop pass reads one word past the end of m_menus (listing 0x42d1c8 `LDR R1,[R4,#4]` with no bound). Port stops at the last real element. Left as is. |

Confirmed no other finding. Several candidates were checked and rejected, most of them by the listing:
- TouchScreenBase::getDimension(Rct&) (0x33abbc): stores at +4..+10 because the Rct omits the vptr (documented in game_types.hpp).
- CharAI::OnPreAttack (0x3d0ed4) returns an int: the slot-41 target is void in every override, so the value is a leftover.
- NativePopMenu (0x43b158): PopMenu(false) matches (R1 = 0 from the earlier CMP).
- CSIdle::OnBlur (0x3c2d3c): idle flag at Character +0x538 = CharStateMachine +0x3c, matches.
- NetBitStream / CNetPlayerInfo / CNetworkId GetLocalNetworkId (0x829694): the |8 on the flags word is equivalent (memset before).
- PacketReceiver / ProcessRoomStatusMessage / ReadHeader field order (0x815400): matches (matching_id read before connection_id, as the original's [4] before [3]).
- SequentialGroup::Reset (0x881da0): all eight word moves matched once the layout (words_[k] at +0xc+4k, index_ +0x30, prev_index_ +0x34) is applied.
- CMatching GetMemberIndex (0x7fe59c): lowest-set-bit loop matches the listing's unsigned behaviour.
- CConnectionManager::ProcessConnectionFinalize, GetPlayCursor (documented partial, deliberate unlock difference), CBluetoothUnreliable::Open (channel stored at +4, listing confirms).
- CMatching::WritePacketData / ProcessServerMessage / ProcessChangeServerMessage: stale "Partial" comments on code that is now complete (the same class of stale comment as round 1).

## Known defects noted in code (not re-reported, already documented by their owners)
- `CMatching` virtual declaration order does not match the binary (c_matching_base.hpp, "KNOWN DEFECT, DO NOT FIX IN A BATCH PASS").
- DeleteInstance (VoxSoundManager, 0x36b048): the port clears s_instance where the original does not; documented in the source as undefined behaviour in the original.

## Unmatched rows
- 716 stb_vorbis_decode_filename and 717 inverse_mdct_slow: upstream stb_vorbis source, no reconstruction by design.
- Every other row matched an address comment (src or header).

## Stale-comment pattern (for the lead)
Several reconstruction files still say "PARTIAL" or "stub" where the body is complete. Confirmed examples: the
ProcessServerMessage note in net-session-online/src/c_matching_gl_live_packets.cpp:9 (the body exists in e2 file and calls AddServer);
the IrrFactory::createFileSystem header note at game-world/src/g1-game-world-pack1-bc81ef-0.cpp:10 (the body does `new FileSystemWin32`).
Round 1 showed the same pattern. Updating those comments would make the next audit cheaper; no source was edited here.
