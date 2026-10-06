
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003ced50 <CharAI::CharAI()>:
  3ced50: e59f314c     	ldr	r3, [pc, #0x14c]        @ 0x3ceea4 <CharAI::CharAI()+0x154>
  3ced54: e59f214c     	ldr	r2, [pc, #0x14c]        @ 0x3ceea8 <CharAI::CharAI()+0x158>
  3ced58: e92d4030     	push	{r4, r5, lr}
  3ced5c: e08f3003     	add	r3, pc, r3
  3ced60: e7932002     	ldr	r2, [r3, r2]
  3ced64: e1a04000     	mov	r4, r0
  3ced68: e3a01000     	mov	r1, #0
  3ced6c: e2822008     	add	r2, r2, #8
  3ced70: e5842000     	str	r2, [r4]
  3ced74: e59f2130     	ldr	r2, [pc, #0x130]        @ 0x3ceeac <CharAI::CharAI()+0x15c>
  3ced78: e3a00001     	mov	r0, #1
  3ced7c: e3e0c000     	mvn	r12, #0
  3ced80: e1a05004     	mov	r5, r4
  3ced84: e5c40055     	strb	r0, [r4, #0x55]
  3ced88: e5841008     	str	r1, [r4, #0x8]
  3ced8c: e584100c     	str	r1, [r4, #0xc]
  3ced90: e5c41018     	strb	r1, [r4, #0x18]
  3ced94: e584101c     	str	r1, [r4, #0x1c]
  3ced98: e5841020     	str	r1, [r4, #0x20]
  3ced9c: e5c41024     	strb	r1, [r4, #0x24]
  3ceda0: e5841028     	str	r1, [r4, #0x28]
  3ceda4: e5c4102c     	strb	r1, [r4, #0x2c]
  3ceda8: e5841030     	str	r1, [r4, #0x30]
  3cedac: e5841034     	str	r1, [r4, #0x34]
  3cedb0: e584103c     	str	r1, [r4, #0x3c]
  3cedb4: e5841040     	str	r1, [r4, #0x40]
  3cedb8: e5841044     	str	r1, [r4, #0x44]
  3cedbc: e5c41049     	strb	r1, [r4, #0x49]
  3cedc0: e5c4004a     	strb	r0, [r4, #0x4a]
  3cedc4: e5c4004b     	strb	r0, [r4, #0x4b]
  3cedc8: e5c4104c     	strb	r1, [r4, #0x4c]
  3cedcc: e5c4004d     	strb	r0, [r4, #0x4d]
  3cedd0: e5841050     	str	r1, [r4, #0x50]
  3cedd4: e5c40054     	strb	r0, [r4, #0x54]
  3cedd8: e5841058     	str	r1, [r4, #0x58]
  3ceddc: e1a00004     	mov	r0, r4
  3cede0: e5841060     	str	r1, [r4, #0x60]
  3cede4: e584c010     	str	r12, [r4, #0x10]
  3cede8: e584c014     	str	r12, [r4, #0x14]
  3cedec: e584c038     	str	r12, [r4, #0x38]
  3cedf0: e5e5105c     	strb	r1, [r5, #0x5c]!
  3cedf4: e5845068     	str	r5, [r4, #0x68]
  3cedf8: e5845064     	str	r5, [r4, #0x64]
  3cedfc: e584106c     	str	r1, [r4, #0x6c]
  3cee00: e5841080     	str	r1, [r4, #0x80]
  3cee04: e5e0107c     	strb	r1, [r0, #0x7c]!
  3cee08: e7935002     	ldr	r5, [r3, r2]
  3cee0c: e1a02004     	mov	r2, r4
  3cee10: e5840088     	str	r0, [r4, #0x88]
  3cee14: e5840084     	str	r0, [r4, #0x84]
  3cee18: e584108c     	str	r1, [r4, #0x8c]
  3cee1c: e5841098     	str	r1, [r4, #0x98]
  3cee20: e28400ac     	add	r0, r4, #172
  3cee24: e5e21094     	strb	r1, [r2, #0x94]!
  3cee28: e58420a0     	str	r2, [r4, #0xa0]
  3cee2c: e58400b0     	str	r0, [r4, #0xb0]
  3cee30: e584c0cc     	str	r12, [r4, #0xcc]
  3cee34: e5c410d1     	strb	r1, [r4, #0xd1]
  3cee38: e584209c     	str	r2, [r4, #0x9c]
  3cee3c: e58410a4     	str	r1, [r4, #0xa4]
  3cee40: e58400ac     	str	r0, [r4, #0xac]
  3cee44: e58410b4     	str	r1, [r4, #0xb4]
  3cee48: e58410b8     	str	r1, [r4, #0xb8]
  3cee4c: e58410bc     	str	r1, [r4, #0xbc]
  3cee50: e58410c0     	str	r1, [r4, #0xc0]
  3cee54: e58410c4     	str	r1, [r4, #0xc4]
  3cee58: e58410c8     	str	r1, [r4, #0xc8]
  3cee5c: e5c410d0     	strb	r1, [r4, #0xd0]
  3cee60: e5951018     	ldr	r1, [r5, #0x18]
  3cee64: e5952010     	ldr	r2, [r5, #0x10]
  3cee68: e24dd00c     	sub	sp, sp, #12
  3cee6c: e2413004     	sub	r3, r1, #4
  3cee70: e1520003     	cmp	r2, r3
  3cee74: e58d4004     	str	r4, [sp, #0x4]
  3cee78: 0a000006     	beq	0x3cee98 <CharAI::CharAI()+0x148> @ imm = #0x18
  3cee7c: e5824000     	str	r4, [r2]
  3cee80: e5953010     	ldr	r3, [r5, #0x10]
  3cee84: e2833004     	add	r3, r3, #4
  3cee88: e5853010     	str	r3, [r5, #0x10]
  3cee8c: e1a00004     	mov	r0, r4
  3cee90: e28dd00c     	add	sp, sp, #12
  3cee94: e8bd8030     	pop	{r4, r5, pc}
  3cee98: e28d0004     	add	r0, sp, #4
  3cee9c: ebfffe5b     	bl	0x3ce810 <std::deque<CharAI*, std::allocator<CharAI*>>::_M_push_back_aux_v(CharAI* const&) (.clone.17)> @ imm = #-0x694
  3ceea0: eafffff9     	b	0x3cee8c <CharAI::CharAI()+0x13c> @ imm = #-0x1c
  3ceea4: 34 5d 5c 00  	.word	0x005c5d34
  3ceea8: 4c 46 00 00  	.word	0x0000464c
  3ceeac: ac 49 00 00  	.word	0x000049ac
