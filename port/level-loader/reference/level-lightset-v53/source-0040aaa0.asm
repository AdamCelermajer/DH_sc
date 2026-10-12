
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0040aaa0 <LightBase::~LightBase()>:
  40aaa0: e92d4010     	push	{r4, lr}
  40aaa4: e59f205c     	ldr	r2, [pc, #0x5c]         @ 0x40ab08 <LightBase::~LightBase()+0x68>
  40aaa8: e59f305c     	ldr	r3, [pc, #0x5c]         @ 0x40ab0c <LightBase::~LightBase()+0x6c>
  40aaac: e5901120     	ldr	r1, [r0, #0x120]
  40aab0: e08f2002     	add	r2, pc, r2
  40aab4: e7923003     	ldr	r3, [r2, r3]
  40aab8: e1a04000     	mov	r4, r0
  40aabc: e3510000     	cmp	r1, #0
  40aac0: e2832084     	add	r2, r3, #132
  40aac4: e2830008     	add	r0, r3, #8
  40aac8: e2833078     	add	r3, r3, #120
  40aacc: e8840009     	stm	r4, {r0, r3}
  40aad0: e5842024     	str	r2, [r4, #0x24]
  40aad4: 0a000005     	beq	0x40aaf0 <LightBase::~LightBase()+0x50> @ imm = #0x14
  40aad8: e5913000     	ldr	r3, [r1]
  40aadc: e513000c     	ldr	r0, [r3, #-0xc]
  40aae0: e0810000     	add	r0, r1, r0
  40aae4: ebfc4aa6     	bl	0x31d584 <glitch::IReferenceCounted::drop() const> @ imm = #-0xed568
  40aae8: e3a03000     	mov	r3, #0
  40aaec: e5843120     	str	r3, [r4, #0x120]
  40aaf0: e2840f5a     	add	r0, r4, #360
  40aaf4: ebfc23ac     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xf7150
  40aaf8: e1a00004     	mov	r0, r4
  40aafc: ebfccfa5     	bl	0x33e998 <ObjectBase::~ObjectBase()> @ imm = #-0xcc16c
  40ab00: e1a00004     	mov	r0, r4
  40ab04: e8bd8010     	pop	{r4, pc}
  40ab08: e0 9f 58 00  	.word	0x00589fe0
  40ab0c: e0 07 00 00  	.word	0x000007e0
