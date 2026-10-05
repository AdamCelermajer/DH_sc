
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003eff98 <Level::_LoadCharStates()>:
  3eff98: e59f3070     	ldr	r3, [pc, #0x70]         @ 0x3f0010 <Level::_LoadCharStates()+0x78>
  3eff9c: e59f2070     	ldr	r2, [pc, #0x70]         @ 0x3f0014 <Level::_LoadCharStates()+0x7c>
  3effa0: e92d4070     	push	{r4, r5, r6, lr}
  3effa4: e08f3003     	add	r3, pc, r3
  3effa8: e7932002     	ldr	r2, [r3, r2]
  3effac: e5926038     	ldr	r6, [r2, #0x38]
  3effb0: e5b64060     	ldr	r4, [r6, #0x60]!
  3effb4: e1560004     	cmp	r6, r4
  3effb8: 0a00000e     	beq	0x3efff8 <Level::_LoadCharStates()+0x60> @ imm = #0x38
  3effbc: e5945008     	ldr	r5, [r4, #0x8]
  3effc0: e2550000     	subs	r0, r5, #0
  3effc4: 0a000008     	beq	0x3effec <Level::_LoadCharStates()+0x54> @ imm = #0x20
  3effc8: ebfed5ed     	bl	0x3a5784 <Character::GetPreSetAIState() const> @ imm = #-0x4a84c
  3effcc: e2503000     	subs	r3, r0, #0
  3effd0: 0a000009     	beq	0x3efffc <Level::_LoadCharStates()+0x64> @ imm = #0x24
  3effd4: e2850e4f     	add	r0, r5, #1264
  3effd8: e3530011     	cmp	r3, #17
  3effdc: e280000c     	add	r0, r0, #12
  3effe0: e3a01000     	mov	r1, #0
  3effe4: 0a000004     	beq	0x3efffc <Level::_LoadCharStates()+0x64> @ imm = #0x10
  3effe8: ebff4684     	bl	0x3c1a00 <CharStateMachine::SM_SetIdleState(bool)> @ imm = #-0x2e5f0
  3effec: e5944000     	ldr	r4, [r4]
  3efff0: e1560004     	cmp	r6, r4
  3efff4: 1afffff0     	bne	0x3effbc <Level::_LoadCharStates()+0x24> @ imm = #-0x40
  3efff8: e8bd8070     	pop	{r4, r5, r6, pc}
  3efffc: e2850e4f     	add	r0, r5, #1264
  3f0000: e280000c     	add	r0, r0, #12
  3f0004: ebff4696     	bl	0x3c1a64 <CharStateMachine::SM_SetPreSpawnState()> @ imm = #-0x2e5a8
  3f0008: e5944000     	ldr	r4, [r4]
  3f000c: eafffff7     	b	0x3efff0 <Level::_LoadCharStates()+0x58> @ imm = #-0x24
  3f0010: ec 4a 5a 00  	.word	0x005a4aec
  3f0014: f4 37 00 00  	.word	0x000037f4
