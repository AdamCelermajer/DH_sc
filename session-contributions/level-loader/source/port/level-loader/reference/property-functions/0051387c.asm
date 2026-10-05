
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0051387c <PropertyMap::SetProperty(char const*, char const*)>:
  51387c: e92d4010     	push	{r4, lr}
  513880: e1a03001     	mov	r3, r1
  513884: e24dd008     	sub	sp, sp, #8
  513888: e1a01000     	mov	r1, r0
  51388c: e1a04002     	mov	r4, r2
  513890: e1a0000d     	mov	r0, sp
  513894: e1a02003     	mov	r2, r3
  513898: ebffffee     	bl	0x513858 <PropertyMap::GetProperty(char const*)> @ imm = #-0x48
  51389c: e3540000     	cmp	r4, #0
  5138a0: e59d3000     	ldr	r3, [sp]
  5138a4: e59d1004     	ldr	r1, [sp, #0x4]
  5138a8: 0a00000a     	beq	0x5138d8 <PropertyMap::SetProperty(char const*, char const*)+0x5c> @ imm = #0x28
  5138ac: e3510000     	cmp	r1, #0
  5138b0: 0a000006     	beq	0x5138d0 <PropertyMap::SetProperty(char const*, char const*)+0x54> @ imm = #0x18
  5138b4: e3530000     	cmp	r3, #0
  5138b8: 0a000004     	beq	0x5138d0 <PropertyMap::SetProperty(char const*, char const*)+0x54> @ imm = #0x10
  5138bc: e1a00003     	mov	r0, r3
  5138c0: e1a02004     	mov	r2, r4
  5138c4: e5933000     	ldr	r3, [r3]
  5138c8: e1a0e00f     	mov	lr, pc
  5138cc: e593f004     	ldr	pc, [r3, #0x4]
  5138d0: e28dd008     	add	sp, sp, #8
  5138d4: e8bd8010     	pop	{r4, pc}
  5138d8: e3510000     	cmp	r1, #0
  5138dc: 0afffffb     	beq	0x5138d0 <PropertyMap::SetProperty(char const*, char const*)+0x54> @ imm = #-0x14
  5138e0: e3530000     	cmp	r3, #0
  5138e4: 0afffff9     	beq	0x5138d0 <PropertyMap::SetProperty(char const*, char const*)+0x54> @ imm = #-0x1c
  5138e8: e1a00003     	mov	r0, r3
  5138ec: e5933000     	ldr	r3, [r3]
  5138f0: e1a0e00f     	mov	lr, pc
  5138f4: e593f00c     	ldr	pc, [r3, #0xc]
  5138f8: eafffff4     	b	0x5138d0 <PropertyMap::SetProperty(char const*, char const*)+0x54> @ imm = #-0x30
