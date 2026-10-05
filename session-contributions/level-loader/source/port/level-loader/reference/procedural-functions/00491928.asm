
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00491928 <rnd::Tile::RemoveNeighbors()>:
  491928: e92d4070     	push	{r4, r5, r6, lr}
  49192c: e590300c     	ldr	r3, [r0, #0xc]
  491930: e1a04000     	mov	r4, r0
  491934: e3530000     	cmp	r3, #0
  491938: da00000a     	ble	0x491968 <rnd::Tile::RemoveNeighbors()+0x40> @ imm = #0x28
  49193c: e2432001     	sub	r2, r3, #1
  491940: e584200c     	str	r2, [r4, #0xc]
  491944: e2833003     	add	r3, r3, #3
  491948: e7945103     	ldr	r5, [r4, r3, lsl #2]
  49194c: e1a00005     	mov	r0, r5
  491950: ebfffff4     	bl	0x491928 <rnd::Tile::RemoveNeighbors()> @ imm = #-0x30
  491954: e1a00005     	mov	r0, r5
  491958: ebffffd2     	bl	0x4918a8 <rnd::Tile::Unspawn()> @ imm = #-0xb8
  49195c: e594300c     	ldr	r3, [r4, #0xc]
  491960: e3530000     	cmp	r3, #0
  491964: cafffff4     	bgt	0x49193c <rnd::Tile::RemoveNeighbors()+0x14> @ imm = #-0x30
  491968: e8bd8070     	pop	{r4, r5, r6, pc}
