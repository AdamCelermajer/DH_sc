
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0039c68c <TriggerZoneExitLevel::~TriggerZoneExitLevel()>:
  39c68c: e59f2048     	ldr	r2, [pc, #0x48]         @ 0x39c6dc <TriggerZoneExitLevel::~TriggerZoneExitLevel()+0x50>
  39c690: e59f3048     	ldr	r3, [pc, #0x48]         @ 0x39c6e0 <TriggerZoneExitLevel::~TriggerZoneExitLevel()+0x54>
  39c694: e92d4010     	push	{r4, lr}
  39c698: e08f2002     	add	r2, pc, r2
  39c69c: e7923003     	ldr	r3, [r2, r3]
  39c6a0: e1a04000     	mov	r4, r0
  39c6a4: e2800b02     	add	r0, r0, #2048
  39c6a8: e28320f4     	add	r2, r3, #244
  39c6ac: e2831008     	add	r1, r3, #8
  39c6b0: e28330e8     	add	r3, r3, #232
  39c6b4: e884000a     	stm	r4, {r1, r3}
  39c6b8: e5842024     	str	r2, [r4, #0x24]
  39c6bc: ebfddcba     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0x88d18
  39c6c0: e2840e7d     	add	r0, r4, #2000
  39c6c4: e280000c     	add	r0, r0, #12
  39c6c8: ebfddcb7     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0x88d24
  39c6cc: e1a00004     	mov	r0, r4
  39c6d0: ebfffc4b     	bl	0x39b804 <TriggerZone::~TriggerZone()> @ imm = #-0xed4
  39c6d4: e1a00004     	mov	r0, r4
  39c6d8: e8bd8010     	pop	{r4, pc}
  39c6dc: f8 83 5f 00  	.word	0x005f83f8
  39c6e0: f4 4b 00 00  	.word	0x00004bf4
