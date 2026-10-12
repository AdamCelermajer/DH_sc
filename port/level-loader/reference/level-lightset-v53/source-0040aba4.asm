
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0040aba4 <LightBase::LightBase(ObjectBase::GO_IDS)>:
  40aba4: e92d4070     	push	{r4, r5, r6, lr}
  40aba8: e59f50a0     	ldr	r5, [pc, #0xa0]         @ 0x40ac50 <LightBase::LightBase(ObjectBase::GO_IDS)+0xac>
  40abac: e1a04000     	mov	r4, r0
  40abb0: ebfcd1d6     	bl	0x33f310 <ObjectBase::ObjectBase(ObjectBase::GO_IDS)> @ imm = #-0xcb8a8
  40abb4: e59f1098     	ldr	r1, [pc, #0x98]         @ 0x40ac54 <LightBase::LightBase(ObjectBase::GO_IDS)+0xb0>
  40abb8: e08f5005     	add	r5, pc, r5
  40abbc: e3a03000     	mov	r3, #0
  40abc0: e7951001     	ldr	r1, [r5, r1]
  40abc4: e2842f5a     	add	r2, r4, #360
  40abc8: e3a06000     	mov	r6, #0
  40abcc: e281c008     	add	r12, r1, #8
  40abd0: e2810084     	add	r0, r1, #132
  40abd4: e2811078     	add	r1, r1, #120
  40abd8: e5841004     	str	r1, [r4, #0x4]
  40abdc: e5840024     	str	r0, [r4, #0x24]
  40abe0: e5843160     	str	r3, [r4, #0x160]
  40abe4: e1a00002     	mov	r0, r2
  40abe8: e5843124     	str	r3, [r4, #0x124]
  40abec: e5843128     	str	r3, [r4, #0x128]
  40abf0: e584312c     	str	r3, [r4, #0x12c]
  40abf4: e5843134     	str	r3, [r4, #0x134]
  40abf8: e5843138     	str	r3, [r4, #0x138]
  40abfc: e584313c     	str	r3, [r4, #0x13c]
  40ac00: e5843140     	str	r3, [r4, #0x140]
  40ac04: e5843144     	str	r3, [r4, #0x144]
  40ac08: e5843148     	str	r3, [r4, #0x148]
  40ac0c: e584314c     	str	r3, [r4, #0x14c]
  40ac10: e5843150     	str	r3, [r4, #0x150]
  40ac14: e5843154     	str	r3, [r4, #0x154]
  40ac18: e5843158     	str	r3, [r4, #0x158]
  40ac1c: e584315c     	str	r3, [r4, #0x15c]
  40ac20: e584c000     	str	r12, [r4]
  40ac24: e5846120     	str	r6, [r4, #0x120]
  40ac28: e5842178     	str	r2, [r4, #0x178]
  40ac2c: e584217c     	str	r2, [r4, #0x17c]
  40ac30: e3a01010     	mov	r1, #16
  40ac34: ebfc1a90     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0xf95c0
  40ac38: e5943178     	ldr	r3, [r4, #0x178]
  40ac3c: e1a00004     	mov	r0, r4
  40ac40: e5c36000     	strb	r6, [r3]
  40ac44: e3a03001     	mov	r3, #1
  40ac48: e5c43085     	strb	r3, [r4, #0x85]
  40ac4c: e8bd8070     	pop	{r4, r5, r6, pc}
  40ac50: d8 9e 58 00  	.word	0x00589ed8
  40ac54: e0 07 00 00  	.word	0x000007e0
