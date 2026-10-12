
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0047a5a8 <ObjectiveList::_resetData()>:
  47a5a8: e3a0c030     	mov	r12, #48
  47a5ac: e3a03001     	mov	r3, #1
  47a5b0: e24dd008     	sub	sp, sp, #8
  47a5b4: e1a0100c     	mov	r1, r12
  47a5b8: e1a02003     	mov	r2, r3
  47a5bc: e58dc000     	str	r12, [sp]
  47a5c0: e58d3004     	str	r3, [sp, #0x4]
  47a5c4: e28dd008     	add	sp, sp, #8
  47a5c8: eaffffdc     	b	0x47a540 <ObjectiveList::LoopOnAll(void (Objective::*)())> @ imm = #-0x90

0047a5cc <ObjectiveList::RemoveObjectiveMarkers()>:
  47a5cc: e3a0c014     	mov	r12, #20
  47a5d0: e3a03001     	mov	r3, #1
  47a5d4: e24dd008     	sub	sp, sp, #8
  47a5d8: e1a0100c     	mov	r1, r12
  47a5dc: e1a02003     	mov	r2, r3
  47a5e0: e58dc000     	str	r12, [sp]
  47a5e4: e58d3004     	str	r3, [sp, #0x4]
  47a5e8: e28dd008     	add	sp, sp, #8
  47a5ec: eaffffd3     	b	0x47a540 <ObjectiveList::LoopOnAll(void (Objective::*)())> @ imm = #-0xb4

0047a5f0 <ObjectiveList::Unregister()>:
  47a5f0: e3a0c01c     	mov	r12, #28
  47a5f4: e3a03001     	mov	r3, #1
  47a5f8: e24dd008     	sub	sp, sp, #8
  47a5fc: e1a0100c     	mov	r1, r12
  47a600: e1a02003     	mov	r2, r3
  47a604: e58dc000     	str	r12, [sp]
  47a608: e58d3004     	str	r3, [sp, #0x4]
  47a60c: e28dd008     	add	sp, sp, #8
  47a610: eaffffca     	b	0x47a540 <ObjectiveList::LoopOnAll(void (Objective::*)())> @ imm = #-0xd8

0047a614 <ObjectiveList::Register()>:
  47a614: e3a0c018     	mov	r12, #24
  47a618: e3a03001     	mov	r3, #1
  47a61c: e24dd008     	sub	sp, sp, #8
  47a620: e1a0100c     	mov	r1, r12
  47a624: e1a02003     	mov	r2, r3
  47a628: e58dc000     	str	r12, [sp]
  47a62c: e58d3004     	str	r3, [sp, #0x4]
  47a630: e28dd008     	add	sp, sp, #8
  47a634: eaffffc1     	b	0x47a540 <ObjectiveList::LoopOnAll(void (Objective::*)())> @ imm = #-0xfc
