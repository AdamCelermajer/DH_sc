
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003aeac0 <Character::GetCharFaery(int) const>:
  3aeac0: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  3aeac4: e24dd008     	sub	sp, sp, #8
  3aeac8: e1a05001     	mov	r5, r1
  3aeacc: ebfffeb3     	bl	0x3ae5a0 <Character::GetCharFaeryListId() const> @ imm = #-0x534
  3aead0: e59f41b8     	ldr	r4, [pc, #0x1b8]        @ 0x3aec90 <Character::GetCharFaery(int) const+0x1d0>
  3aead4: e59f31b8     	ldr	r3, [pc, #0x1b8]        @ 0x3aec94 <Character::GetCharFaery(int) const+0x1d4>
  3aead8: e3a0600c     	mov	r6, #12
  3aeadc: e08f4004     	add	r4, pc, r4
  3aeae0: e7943003     	ldr	r3, [r4, r3]
  3aeae4: e3550000     	cmp	r5, #0
  3aeae8: e5933000     	ldr	r3, [r3]
  3aeaec: e0263096     	mla	r6, r6, r0, r3
  3aeaf0: ba000031     	blt	0x3aebbc <Character::GetCharFaery(int) const+0xfc> @ imm = #0xc4
  3aeaf4: e59f719c     	ldr	r7, [pc, #0x19c]        @ 0x3aec98 <Character::GetCharFaery(int) const+0x1d8>
  3aeaf8: e59f119c     	ldr	r1, [pc, #0x19c]        @ 0x3aec9c <Character::GetCharFaery(int) const+0x1dc>
  3aeafc: e59f219c     	ldr	r2, [pc, #0x19c]        @ 0x3aeca0 <Character::GetCharFaery(int) const+0x1e0>
  3aeb00: e7943007     	ldr	r3, [r4, r7]
  3aeb04: e08f1001     	add	r1, pc, r1
  3aeb08: e08f2002     	add	r2, pc, r2
  3aeb0c: e593002c     	ldr	r0, [r3, #0x2c]
  3aeb10: eb045831     	bl	0x4c4bdc <PyDataConstants::getConstant(char const*, char const*) const> @ imm = #0x1160c4
  3aeb14: e1550000     	cmp	r5, r0
  3aeb18: aa000028     	bge	0x3aebc0 <Character::GetCharFaery(int) const+0x100> @ imm = #0xa0
  3aeb1c: e7943007     	ldr	r3, [r4, r7]
  3aeb20: e59f117c     	ldr	r1, [pc, #0x17c]        @ 0x3aeca4 <Character::GetCharFaery(int) const+0x1e4>
  3aeb24: e59f217c     	ldr	r2, [pc, #0x17c]        @ 0x3aeca8 <Character::GetCharFaery(int) const+0x1e8>
  3aeb28: e593002c     	ldr	r0, [r3, #0x2c]
  3aeb2c: e08f1001     	add	r1, pc, r1
  3aeb30: e08f2002     	add	r2, pc, r2
  3aeb34: e5967004     	ldr	r7, [r6, #0x4]
  3aeb38: eb045827     	bl	0x4c4bdc <PyDataConstants::getConstant(char const*, char const*) const> @ imm = #0x11609c
  3aeb3c: e1570000     	cmp	r7, r0
  3aeb40: 0a000008     	beq	0x3aeb68 <Character::GetCharFaery(int) const+0xa8> @ imm = #0x20
  3aeb44: e59f3160     	ldr	r3, [pc, #0x160]        @ 0x3aecac <Character::GetCharFaery(int) const+0x1ec>
  3aeb48: e7943003     	ldr	r3, [r4, r3]
  3aeb4c: e5933000     	ldr	r3, [r3]
  3aeb50: e3530002     	cmp	r3, #2
  3aeb54: 03a03000     	moveq	r3, #0
  3aeb58: 05833000     	streq	r3, [r3]
  3aeb5c: 0a000001     	beq	0x3aeb68 <Character::GetCharFaery(int) const+0xa8> @ imm = #0x4
  3aeb60: e3530001     	cmp	r3, #1
  3aeb64: 0a00002b     	beq	0x3aec18 <Character::GetCharFaery(int) const+0x158> @ imm = #0xac
  3aeb68: e59f2140     	ldr	r2, [pc, #0x140]        @ 0x3aecb0 <Character::GetCharFaery(int) const+0x1f0>
  3aeb6c: e5963008     	ldr	r3, [r6, #0x8]
  3aeb70: e3a08024     	mov	r8, #36
  3aeb74: e7947002     	ldr	r7, [r4, r2]
  3aeb78: e7930105     	ldr	r0, [r3, r5, lsl #2]
  3aeb7c: e5973000     	ldr	r3, [r7]
  3aeb80: e0203098     	mla	r0, r8, r0, r3
  3aeb84: e5903020     	ldr	r3, [r0, #0x20]
  3aeb88: e1530005     	cmp	r3, r5
  3aeb8c: 0a000008     	beq	0x3aebb4 <Character::GetCharFaery(int) const+0xf4> @ imm = #0x20
  3aeb90: e59f3114     	ldr	r3, [pc, #0x114]        @ 0x3aecac <Character::GetCharFaery(int) const+0x1ec>
  3aeb94: e7943003     	ldr	r3, [r4, r3]
  3aeb98: e5933000     	ldr	r3, [r3]
  3aeb9c: e3530002     	cmp	r3, #2
  3aeba0: 03a03000     	moveq	r3, #0
  3aeba4: 05833000     	streq	r3, [r3]
  3aeba8: 0a000001     	beq	0x3aebb4 <Character::GetCharFaery(int) const+0xf4> @ imm = #0x4
  3aebac: e3530001     	cmp	r3, #1
  3aebb0: 0a000025     	beq	0x3aec4c <Character::GetCharFaery(int) const+0x18c> @ imm = #0x94
  3aebb4: e28dd008     	add	sp, sp, #8
  3aebb8: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  3aebbc: e59f70d4     	ldr	r7, [pc, #0xd4]         @ 0x3aec98 <Character::GetCharFaery(int) const+0x1d8>
  3aebc0: e59f30e4     	ldr	r3, [pc, #0xe4]         @ 0x3aecac <Character::GetCharFaery(int) const+0x1ec>
  3aebc4: e7943003     	ldr	r3, [r4, r3]
  3aebc8: e5933000     	ldr	r3, [r3]
  3aebcc: e3530002     	cmp	r3, #2
  3aebd0: 03a03000     	moveq	r3, #0
  3aebd4: 05833000     	streq	r3, [r3]
  3aebd8: 0affffcf     	beq	0x3aeb1c <Character::GetCharFaery(int) const+0x5c> @ imm = #-0xc4
  3aebdc: e3530001     	cmp	r3, #1
  3aebe0: 1affffcd     	bne	0x3aeb1c <Character::GetCharFaery(int) const+0x5c> @ imm = #-0xcc
  3aebe4: e59f00c8     	ldr	r0, [pc, #0xc8]         @ 0x3aecb4 <Character::GetCharFaery(int) const+0x1f4>
  3aebe8: e59f10c8     	ldr	r1, [pc, #0xc8]         @ 0x3aecb8 <Character::GetCharFaery(int) const+0x1f8>
  3aebec: e59f20c8     	ldr	r2, [pc, #0xc8]         @ 0x3aecbc <Character::GetCharFaery(int) const+0x1fc>
  3aebf0: e7940000     	ldr	r0, [r4, r0]
  3aebf4: e59f30c4     	ldr	r3, [pc, #0xc4]         @ 0x3aecc0 <Character::GetCharFaery(int) const+0x200>
  3aebf8: e3a0c03e     	mov	r12, #62
  3aebfc: e08f1001     	add	r1, pc, r1
  3aec00: e08f2002     	add	r2, pc, r2
  3aec04: e08f3003     	add	r3, pc, r3
  3aec08: e28000a8     	add	r0, r0, #168
  3aec0c: e58dc000     	str	r12, [sp]
  3aec10: ebfd7cfb     	bl	0x30e004 <.plt+0x290>   @ imm = #-0xa0c14
  3aec14: eaffffc0     	b	0x3aeb1c <Character::GetCharFaery(int) const+0x5c> @ imm = #-0x100
  3aec18: e59f0094     	ldr	r0, [pc, #0x94]         @ 0x3aecb4 <Character::GetCharFaery(int) const+0x1f4>
  3aec1c: e59f10a0     	ldr	r1, [pc, #0xa0]         @ 0x3aecc4 <Character::GetCharFaery(int) const+0x204>
  3aec20: e59f20a0     	ldr	r2, [pc, #0xa0]         @ 0x3aecc8 <Character::GetCharFaery(int) const+0x208>
  3aec24: e7940000     	ldr	r0, [r4, r0]
  3aec28: e59f309c     	ldr	r3, [pc, #0x9c]         @ 0x3aeccc <Character::GetCharFaery(int) const+0x20c>
  3aec2c: e3a0c03f     	mov	r12, #63
  3aec30: e08f1001     	add	r1, pc, r1
  3aec34: e08f2002     	add	r2, pc, r2
  3aec38: e08f3003     	add	r3, pc, r3
  3aec3c: e28000a8     	add	r0, r0, #168
  3aec40: e58dc000     	str	r12, [sp]
  3aec44: ebfd7cee     	bl	0x30e004 <.plt+0x290>   @ imm = #-0xa0c48
  3aec48: eaffffc6     	b	0x3aeb68 <Character::GetCharFaery(int) const+0xa8> @ imm = #-0xe8
  3aec4c: e59f0060     	ldr	r0, [pc, #0x60]         @ 0x3aecb4 <Character::GetCharFaery(int) const+0x1f4>
  3aec50: e59f1078     	ldr	r1, [pc, #0x78]         @ 0x3aecd0 <Character::GetCharFaery(int) const+0x210>
  3aec54: e59f2078     	ldr	r2, [pc, #0x78]         @ 0x3aecd4 <Character::GetCharFaery(int) const+0x214>
  3aec58: e7940000     	ldr	r0, [r4, r0]
  3aec5c: e59f3074     	ldr	r3, [pc, #0x74]         @ 0x3aecd8 <Character::GetCharFaery(int) const+0x218>
  3aec60: e08f2002     	add	r2, pc, r2
  3aec64: e3a0c040     	mov	r12, #64
  3aec68: e08f3003     	add	r3, pc, r3
  3aec6c: e08f1001     	add	r1, pc, r1
  3aec70: e28000a8     	add	r0, r0, #168
  3aec74: e58dc000     	str	r12, [sp]
  3aec78: ebfd7ce1     	bl	0x30e004 <.plt+0x290>   @ imm = #-0xa0c7c
  3aec7c: e5962008     	ldr	r2, [r6, #0x8]
  3aec80: e5973000     	ldr	r3, [r7]
  3aec84: e7920105     	ldr	r0, [r2, r5, lsl #2]
  3aec88: e0203098     	mla	r0, r8, r0, r3
  3aec8c: eaffffc8     	b	0x3aebb4 <Character::GetCharFaery(int) const+0xf4> @ imm = #-0xe0
  3aec90: b4 5f 5e 00  	.word	0x005e5fb4
  3aec94: 54 3e 00 00  	.word	0x00003e54
  3aec98: f4 37 00 00  	.word	0x000037f4
  3aec9c: 74 4d 51 00  	.word	0x00514d74
  3aeca0: 70 e2 51 00  	.word	0x0051e270
  3aeca4: 4c 4d 51 00  	.word	0x00514d4c
  3aeca8: 48 e2 51 00  	.word	0x0051e248
  3aecac: c0 39 00 00  	.word	0x000039c0
  3aecb0: f8 0e 00 00  	.word	0x00000ef8
  3aecb4: c0 19 00 00  	.word	0x000019c0
  3aecb8: dc f7 50 00  	.word	0x0050f7dc
  3aecbc: 88 4c 51 00  	.word	0x00514c88
  3aecc0: f4 4b 51 00  	.word	0x00514bf4
  3aecc4: a8 f7 50 00  	.word	0x0050f7a8
  3aecc8: c4 4c 51 00  	.word	0x00514cc4
  3aeccc: c0 4b 51 00  	.word	0x00514bc0
  3aecd0: 6c f7 50 00  	.word	0x0050f76c
  3aecd4: 08 4d 51 00  	.word	0x00514d08
  3aecd8: 90 4b 51 00  	.word	0x00514b90
