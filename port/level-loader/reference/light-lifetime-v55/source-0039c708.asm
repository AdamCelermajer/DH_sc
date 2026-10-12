
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0039c708 <TriggerZoneExitLevel::~TriggerZoneExitLevel()>:
  39c708: e59f2048     	ldr	r2, [pc, #0x48]         @ 0x39c758 <TriggerZoneExitLevel::~TriggerZoneExitLevel()+0x50>
  39c70c: e59f3048     	ldr	r3, [pc, #0x48]         @ 0x39c75c <TriggerZoneExitLevel::~TriggerZoneExitLevel()+0x54>
  39c710: e92d4010     	push	{r4, lr}
  39c714: e08f2002     	add	r2, pc, r2
  39c718: e7923003     	ldr	r3, [r2, r3]
  39c71c: e1a04000     	mov	r4, r0
  39c720: e2800b02     	add	r0, r0, #2048
  39c724: e28320f4     	add	r2, r3, #244
  39c728: e2831008     	add	r1, r3, #8
  39c72c: e28330e8     	add	r3, r3, #232
  39c730: e884000a     	stm	r4, {r1, r3}
  39c734: e5842024     	str	r2, [r4, #0x24]
  39c738: ebfddc9b     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0x88d94
  39c73c: e2840e7d     	add	r0, r4, #2000
  39c740: e280000c     	add	r0, r0, #12
  39c744: ebfddc98     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0x88da0
  39c748: e1a00004     	mov	r0, r4
  39c74c: ebfffc2c     	bl	0x39b804 <TriggerZone::~TriggerZone()> @ imm = #-0xf50
  39c750: e1a00004     	mov	r0, r4
  39c754: e8bd8010     	pop	{r4, pc}
  39c758: 7c 83 5f 00  	.word	0x005f837c
  39c75c: f4 4b 00 00  	.word	0x00004bf4
