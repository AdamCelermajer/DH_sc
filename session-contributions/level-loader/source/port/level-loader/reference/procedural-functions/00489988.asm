
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00489988 <rnd::Block::IsStraight() const>:
  489988: e590205c     	ldr	r2, [r0, #0x5c]
  48998c: e59f3044     	ldr	r3, [pc, #0x44]         @ 0x4899d8 <rnd::Block::IsStraight() const+0x50>
  489990: e3520002     	cmp	r2, #2
  489994: e08f3003     	add	r3, pc, r3
  489998: 13a00000     	movne	r0, #0
  48999c: 112fff1e     	bxne	lr
  4899a0: e59021a0     	ldr	r2, [r0, #0x1a0]
  4899a4: e59f1030     	ldr	r1, [pc, #0x30]         @ 0x4899dc <rnd::Block::IsStraight() const+0x54>
  4899a8: e592c000     	ldr	r12, [r2]
  4899ac: e59f202c     	ldr	r2, [pc, #0x2c]         @ 0x4899e0 <rnd::Block::IsStraight() const+0x58>
  4899b0: e7932002     	ldr	r2, [r3, r2]
  4899b4: e7933001     	ldr	r3, [r3, r1]
  4899b8: e5901074     	ldr	r1, [r0, #0x74]
  4899bc: e792210c     	ldr	r2, [r2, r12, lsl #2]
  4899c0: e5910000     	ldr	r0, [r1]
  4899c4: e7933202     	ldr	r3, [r3, r2, lsl #4]
  4899c8: e1500003     	cmp	r0, r3
  4899cc: 13a00000     	movne	r0, #0
  4899d0: 03a00001     	moveq	r0, #1
  4899d4: e12fff1e     	bx	lr
  4899d8: fc b0 50 00  	.word	0x0050b0fc
  4899dc: fc 43 00 00  	.word	0x000043fc
  4899e0: b8 1b 00 00  	.word	0x00001bb8
