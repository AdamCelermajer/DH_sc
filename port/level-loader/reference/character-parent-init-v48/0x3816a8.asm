
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003816a8 <Device::IsProgrammablePipeline()>:
  3816a8: e59f3078     	ldr	r3, [pc, #0x78]         @ 0x381728 <Device::IsProgrammablePipeline()+0x80>
  3816ac: e59f2078     	ldr	r2, [pc, #0x78]         @ 0x38172c <Device::IsProgrammablePipeline()+0x84>
  3816b0: e92d4010     	push	{r4, lr}
  3816b4: e08f3003     	add	r3, pc, r3
  3816b8: e7932002     	ldr	r2, [r3, r2]
  3816bc: e5d22000     	ldrb	r2, [r2]
  3816c0: e3520000     	cmp	r2, #0
  3816c4: 1a000009     	bne	0x3816f0 <Device::IsProgrammablePipeline()+0x48> @ imm = #0x24
  3816c8: e59f2060     	ldr	r2, [pc, #0x60]         @ 0x381730 <Device::IsProgrammablePipeline()+0x88>
  3816cc: e7932002     	ldr	r2, [r3, r2]
  3816d0: e5d22000     	ldrb	r2, [r2]
  3816d4: e3520000     	cmp	r2, #0
  3816d8: 1a000004     	bne	0x3816f0 <Device::IsProgrammablePipeline()+0x48> @ imm = #0x10
  3816dc: e59f2050     	ldr	r2, [pc, #0x50]         @ 0x381734 <Device::IsProgrammablePipeline()+0x8c>
  3816e0: e7932002     	ldr	r2, [r3, r2]
  3816e4: e5d22000     	ldrb	r2, [r2]
  3816e8: e3520000     	cmp	r2, #0
  3816ec: 0a000001     	beq	0x3816f8 <Device::IsProgrammablePipeline()+0x50> @ imm = #0x4
  3816f0: e3a00000     	mov	r0, #0
  3816f4: e8bd8010     	pop	{r4, pc}
  3816f8: e59f2038     	ldr	r2, [pc, #0x38]         @ 0x381738 <Device::IsProgrammablePipeline()+0x90>
  3816fc: e7933002     	ldr	r3, [r3, r2]
  381700: e5933010     	ldr	r3, [r3, #0x10]
  381704: e5933010     	ldr	r3, [r3, #0x10]
  381708: e1a00003     	mov	r0, r3
  38170c: e5933000     	ldr	r3, [r3]
  381710: e1a0e00f     	mov	lr, pc
  381714: e593f05c     	ldr	pc, [r3, #0x5c]
  381718: e3100078     	tst	r0, #120
  38171c: 03a00000     	moveq	r0, #0
  381720: 13a00001     	movne	r0, #1
  381724: e8bd8010     	pop	{r4, pc}
  381728: dc 33 61 00  	.word	0x006133dc
  38172c: 58 44 00 00  	.word	0x00004458
  381730: 68 27 00 00  	.word	0x00002768
  381734: d4 29 00 00  	.word	0x000029d4
  381738: f4 37 00 00  	.word	0x000037f4
