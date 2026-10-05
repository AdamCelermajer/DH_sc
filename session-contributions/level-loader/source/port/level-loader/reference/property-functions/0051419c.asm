
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0051419c <PropertyMap::LoadTemplate()>:
  51419c: e59f3068     	ldr	r3, [pc, #0x68]         @ 0x51420c <PropertyMap::LoadTemplate()+0x70>
  5141a0: e59f2068     	ldr	r2, [pc, #0x68]         @ 0x514210 <PropertyMap::LoadTemplate()+0x74>
  5141a4: e52de004     	str	lr, [sp, #-0x4]!
  5141a8: e08f3003     	add	r3, pc, r3
  5141ac: e7932002     	ldr	r2, [r3, r2]
  5141b0: e24dd00c     	sub	sp, sp, #12
  5141b4: e5922000     	ldr	r2, [r2]
  5141b8: e3520002     	cmp	r2, #2
  5141bc: 03a03000     	moveq	r3, #0
  5141c0: 05833000     	streq	r3, [r3]
  5141c4: 0a000001     	beq	0x5141d0 <PropertyMap::LoadTemplate()+0x34> @ imm = #0x4
  5141c8: e3520001     	cmp	r2, #1
  5141cc: 0a000001     	beq	0x5141d8 <PropertyMap::LoadTemplate()+0x3c> @ imm = #0x4
  5141d0: e28dd00c     	add	sp, sp, #12
  5141d4: e8bd8000     	ldm	sp!, {pc}
  5141d8: e59f0034     	ldr	r0, [pc, #0x34]         @ 0x514214 <PropertyMap::LoadTemplate()+0x78>
  5141dc: e59f1034     	ldr	r1, [pc, #0x34]         @ 0x514218 <PropertyMap::LoadTemplate()+0x7c>
  5141e0: e59f2034     	ldr	r2, [pc, #0x34]         @ 0x51421c <PropertyMap::LoadTemplate()+0x80>
  5141e4: e7930000     	ldr	r0, [r3, r0]
  5141e8: e59f3030     	ldr	r3, [pc, #0x30]         @ 0x514220 <PropertyMap::LoadTemplate()+0x84>
  5141ec: e3a0c009     	mov	r12, #9
  5141f0: e08f1001     	add	r1, pc, r1
  5141f4: e08f2002     	add	r2, pc, r2
  5141f8: e08f3003     	add	r3, pc, r3
  5141fc: e28000a8     	add	r0, r0, #168
  514200: e58dc000     	str	r12, [sp]
  514204: ebf7e77e     	bl	0x30e004 <.plt+0x290>   @ imm = #-0x206208
  514208: eafffff0     	b	0x5141d0 <PropertyMap::LoadTemplate()+0x34> @ imm = #-0x40
  51420c: e8 08 48 00  	.word	0x004808e8
  514210: c0 39 00 00  	.word	0x000039c0
  514214: c0 19 00 00  	.word	0x000019c0
  514218: e8 a1 3a 00  	.word	0x003aa1e8
  51421c: 74 a3 3a 00  	.word	0x003aa374
  514220: 68 7e 3c 00  	.word	0x003c7e68
