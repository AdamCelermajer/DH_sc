
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

004899e4 <rnd::Block::GetFlag() const>:
  4899e4: e52d4004     	str	r4, [sp, #-0x4]!
  4899e8: e590c05c     	ldr	r12, [r0, #0x5c]
  4899ec: e35c0000     	cmp	r12, #0
  4899f0: d3a00000     	movle	r0, #0
  4899f4: da00000a     	ble	0x489a24 <rnd::Block::GetFlag() const+0x40> @ imm = #0x28
  4899f8: e3a03000     	mov	r3, #0
  4899fc: e1a02000     	mov	r2, r0
  489a00: e3a04001     	mov	r4, #1
  489a04: e1a00003     	mov	r0, r3
  489a08: e5921074     	ldr	r1, [r2, #0x74]
  489a0c: e2833001     	add	r3, r3, #1
  489a10: e153000c     	cmp	r3, r12
  489a14: e5911000     	ldr	r1, [r1]
  489a18: e2822f4b     	add	r2, r2, #300
  489a1c: e1800114     	orr	r0, r0, r4, lsl r1
  489a20: 1afffff8     	bne	0x489a08 <rnd::Block::GetFlag() const+0x24> @ imm = #-0x20
  489a24: e8bd0010     	ldm	sp!, {r4}
  489a28: e12fff1e     	bx	lr
