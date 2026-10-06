
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003a31e8 <Character::GetCharModelId() const>:
  3a31e8: e3013004     	movw	r3, #0x1004
  3a31ec: e7900003     	ldr	r0, [r0, r3]
  3a31f0: e59f3028     	ldr	r3, [pc, #0x28]         @ 0x3a3220 <Character::GetCharModelId() const+0x38>
  3a31f4: e3500000     	cmp	r0, #0
  3a31f8: e08f3003     	add	r3, pc, r3
  3a31fc: aa000001     	bge	0x3a3208 <Character::GetCharModelId() const+0x20> @ imm = #0x4
  3a3200: e3e00000     	mvn	r0, #0
  3a3204: e12fff1e     	bx	lr
  3a3208: e59f2014     	ldr	r2, [pc, #0x14]         @ 0x3a3224 <Character::GetCharModelId() const+0x3c>
  3a320c: e7933002     	ldr	r3, [r3, r2]
  3a3210: e5933000     	ldr	r3, [r3]
  3a3214: e1500003     	cmp	r0, r3
  3a3218: b12fff1e     	bxlt	lr
  3a321c: eafffff7     	b	0x3a3200 <Character::GetCharModelId() const+0x18> @ imm = #-0x24
  3a3220: 98 18 5f 00  	.word	0x005f1898
  3a3224: 0c 3c 00 00  	.word	0x00003c0c
