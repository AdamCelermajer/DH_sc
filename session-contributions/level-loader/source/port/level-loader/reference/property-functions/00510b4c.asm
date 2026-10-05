
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00510b4c <PropertyMap::GetThisClassName()>:
  510b4c: e92d4010     	push	{r4, lr}
  510b50: e1a04000     	mov	r4, r0
  510b54: e590001c     	ldr	r0, [r0, #0x1c]
  510b58: e59f306c     	ldr	r3, [pc, #0x6c]         @ 0x510bcc <PropertyMap::GetThisClassName()+0x80>
  510b5c: e24dd008     	sub	sp, sp, #8
  510b60: e3500000     	cmp	r0, #0
  510b64: e08f3003     	add	r3, pc, r3
  510b68: 0a000001     	beq	0x510b74 <PropertyMap::GetThisClassName()+0x28> @ imm = #0x4
  510b6c: e28dd008     	add	sp, sp, #8
  510b70: e8bd8010     	pop	{r4, pc}
  510b74: e59f2054     	ldr	r2, [pc, #0x54]         @ 0x510bd0 <PropertyMap::GetThisClassName()+0x84>
  510b78: e7932002     	ldr	r2, [r3, r2]
  510b7c: e5922000     	ldr	r2, [r2]
  510b80: e3520002     	cmp	r2, #2
  510b84: 05800000     	streq	r0, [r0]
  510b88: 0afffff7     	beq	0x510b6c <PropertyMap::GetThisClassName()+0x20> @ imm = #-0x24
  510b8c: e3520001     	cmp	r2, #1
  510b90: 1afffff5     	bne	0x510b6c <PropertyMap::GetThisClassName()+0x20> @ imm = #-0x2c
  510b94: e59f0038     	ldr	r0, [pc, #0x38]         @ 0x510bd4 <PropertyMap::GetThisClassName()+0x88>
  510b98: e59f1038     	ldr	r1, [pc, #0x38]         @ 0x510bd8 <PropertyMap::GetThisClassName()+0x8c>
  510b9c: e59f2038     	ldr	r2, [pc, #0x38]         @ 0x510bdc <PropertyMap::GetThisClassName()+0x90>
  510ba0: e7930000     	ldr	r0, [r3, r0]
  510ba4: e59f3034     	ldr	r3, [pc, #0x34]         @ 0x510be0 <PropertyMap::GetThisClassName()+0x94>
  510ba8: e3a0c092     	mov	r12, #146
  510bac: e08f1001     	add	r1, pc, r1
  510bb0: e28000a8     	add	r0, r0, #168
  510bb4: e08f2002     	add	r2, pc, r2
  510bb8: e08f3003     	add	r3, pc, r3
  510bbc: e58dc000     	str	r12, [sp]
  510bc0: ebf7f50f     	bl	0x30e004 <.plt+0x290>   @ imm = #-0x202bc4
  510bc4: e594001c     	ldr	r0, [r4, #0x1c]
  510bc8: eaffffe7     	b	0x510b6c <PropertyMap::GetThisClassName()+0x20> @ imm = #-0x64
  510bcc: 2c 3f 48 00  	.word	0x00483f2c
  510bd0: c0 39 00 00  	.word	0x000039c0
  510bd4: c0 19 00 00  	.word	0x000019c0
  510bd8: 2c d8 3a 00  	.word	0x003ad82c
  510bdc: 2c b4 3c 00  	.word	0x003cb42c
  510be0: 40 b4 3c 00  	.word	0x003cb440
