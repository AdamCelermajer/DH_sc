
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0040ac58 <LightBase::LightBase(ObjectBase::GO_IDS)>:
  40ac58: e92d4070     	push	{r4, r5, r6, lr}
  40ac5c: e59f50a0     	ldr	r5, [pc, #0xa0]         @ 0x40ad04 <LightBase::LightBase(ObjectBase::GO_IDS)+0xac>
  40ac60: e1a04000     	mov	r4, r0
  40ac64: ebfcd1a9     	bl	0x33f310 <ObjectBase::ObjectBase(ObjectBase::GO_IDS)> @ imm = #-0xcb95c
  40ac68: e59f1098     	ldr	r1, [pc, #0x98]         @ 0x40ad08 <LightBase::LightBase(ObjectBase::GO_IDS)+0xb0>
  40ac6c: e08f5005     	add	r5, pc, r5
  40ac70: e3a03000     	mov	r3, #0
  40ac74: e7951001     	ldr	r1, [r5, r1]
  40ac78: e2842f5a     	add	r2, r4, #360
  40ac7c: e3a06000     	mov	r6, #0
  40ac80: e281c008     	add	r12, r1, #8
  40ac84: e2810084     	add	r0, r1, #132
  40ac88: e2811078     	add	r1, r1, #120
  40ac8c: e5841004     	str	r1, [r4, #0x4]
  40ac90: e5840024     	str	r0, [r4, #0x24]
  40ac94: e5843160     	str	r3, [r4, #0x160]
  40ac98: e1a00002     	mov	r0, r2
  40ac9c: e5843124     	str	r3, [r4, #0x124]
  40aca0: e5843128     	str	r3, [r4, #0x128]
  40aca4: e584312c     	str	r3, [r4, #0x12c]
  40aca8: e5843134     	str	r3, [r4, #0x134]
  40acac: e5843138     	str	r3, [r4, #0x138]
  40acb0: e584313c     	str	r3, [r4, #0x13c]
  40acb4: e5843140     	str	r3, [r4, #0x140]
  40acb8: e5843144     	str	r3, [r4, #0x144]
  40acbc: e5843148     	str	r3, [r4, #0x148]
  40acc0: e584314c     	str	r3, [r4, #0x14c]
  40acc4: e5843150     	str	r3, [r4, #0x150]
  40acc8: e5843154     	str	r3, [r4, #0x154]
  40accc: e5843158     	str	r3, [r4, #0x158]
  40acd0: e584315c     	str	r3, [r4, #0x15c]
  40acd4: e584c000     	str	r12, [r4]
  40acd8: e5846120     	str	r6, [r4, #0x120]
  40acdc: e5842178     	str	r2, [r4, #0x178]
  40ace0: e584217c     	str	r2, [r4, #0x17c]
  40ace4: e3a01010     	mov	r1, #16
  40ace8: ebfc1a63     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0xf9674
  40acec: e5943178     	ldr	r3, [r4, #0x178]
  40acf0: e1a00004     	mov	r0, r4
  40acf4: e5c36000     	strb	r6, [r3]
  40acf8: e3a03001     	mov	r3, #1
  40acfc: e5c43085     	strb	r3, [r4, #0x85]
  40ad00: e8bd8070     	pop	{r4, r5, r6, pc}
  40ad04: 24 9e 58 00  	.word	0x00589e24
  40ad08: e0 07 00 00  	.word	0x000007e0
