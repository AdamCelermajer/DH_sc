
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00491440 <_ZN3rnd4Tile11RemoveChildEPS0_>:
  491440: e590300c     	ldr	r3, [r0, #0xc]
  491444: e52d4004     	str	r4, [sp, #-0x4]!
  491448: e3530000     	cmp	r3, #0
  49144c: da00000d     	ble	0x491488 <_ZN3rnd4Tile11RemoveChildEPS0_+0x48> @ imm = #0x34
  491450: e5902010     	ldr	r2, [r0, #0x10]
  491454: e1520001     	cmp	r2, r1
  491458: 03a02000     	moveq	r2, #0
  49145c: 0a00000b     	beq	0x491490 <_ZN3rnd4Tile11RemoveChildEPS0_+0x50> @ imm = #0x2c
  491460: e1a0c000     	mov	r12, r0
  491464: e3a02000     	mov	r2, #0
  491468: ea000002     	b	0x491478 <_ZN3rnd4Tile11RemoveChildEPS0_+0x38> @ imm = #0x8
  49146c: e59c4010     	ldr	r4, [r12, #0x10]
  491470: e1540001     	cmp	r4, r1
  491474: 0a000005     	beq	0x491490 <_ZN3rnd4Tile11RemoveChildEPS0_+0x50> @ imm = #0x14
  491478: e2822001     	add	r2, r2, #1
  49147c: e1520003     	cmp	r2, r3
  491480: e28cc004     	add	r12, r12, #4
  491484: 1afffff8     	bne	0x49146c <_ZN3rnd4Tile11RemoveChildEPS0_+0x2c> @ imm = #-0x20
  491488: e8bd0010     	ldm	sp!, {r4}
  49148c: e12fff1e     	bx	lr
  491490: e2431001     	sub	r1, r3, #1
  491494: e580100c     	str	r1, [r0, #0xc]
  491498: e2833003     	add	r3, r3, #3
  49149c: e7903103     	ldr	r3, [r0, r3, lsl #2]
  4914a0: e2822004     	add	r2, r2, #4
  4914a4: e7803102     	str	r3, [r0, r2, lsl #2]
  4914a8: eafffff6     	b	0x491488 <_ZN3rnd4Tile11RemoveChildEPS0_+0x48> @ imm = #-0x28
