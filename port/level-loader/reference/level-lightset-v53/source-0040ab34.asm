
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0040ab34 <LightBase::~LightBase()>:
  40ab34: e92d4010     	push	{r4, lr}
  40ab38: e59f205c     	ldr	r2, [pc, #0x5c]         @ 0x40ab9c <LightBase::~LightBase()+0x68>
  40ab3c: e59f305c     	ldr	r3, [pc, #0x5c]         @ 0x40aba0 <LightBase::~LightBase()+0x6c>
  40ab40: e5901120     	ldr	r1, [r0, #0x120]
  40ab44: e08f2002     	add	r2, pc, r2
  40ab48: e7923003     	ldr	r3, [r2, r3]
  40ab4c: e1a04000     	mov	r4, r0
  40ab50: e3510000     	cmp	r1, #0
  40ab54: e2832084     	add	r2, r3, #132
  40ab58: e2830008     	add	r0, r3, #8
  40ab5c: e2833078     	add	r3, r3, #120
  40ab60: e8840009     	stm	r4, {r0, r3}
  40ab64: e5842024     	str	r2, [r4, #0x24]
  40ab68: 0a000005     	beq	0x40ab84 <LightBase::~LightBase()+0x50> @ imm = #0x14
  40ab6c: e5913000     	ldr	r3, [r1]
  40ab70: e513000c     	ldr	r0, [r3, #-0xc]
  40ab74: e0810000     	add	r0, r1, r0
  40ab78: ebfc4a81     	bl	0x31d584 <glitch::IReferenceCounted::drop() const> @ imm = #-0xed5fc
  40ab7c: e3a03000     	mov	r3, #0
  40ab80: e5843120     	str	r3, [r4, #0x120]
  40ab84: e2840f5a     	add	r0, r4, #360
  40ab88: ebfc2387     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xf71e4
  40ab8c: e1a00004     	mov	r0, r4
  40ab90: ebfccf80     	bl	0x33e998 <ObjectBase::~ObjectBase()> @ imm = #-0xcc200
  40ab94: e1a00004     	mov	r0, r4
  40ab98: e8bd8010     	pop	{r4, pc}
  40ab9c: 4c 9f 58 00  	.word	0x00589f4c
  40aba0: e0 07 00 00  	.word	0x000007e0
