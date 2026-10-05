
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

004914ac <rnd::Tile::Print()>:
  4914ac: e92d4070     	push	{r4, r5, r6, lr}
  4914b0: e590300c     	ldr	r3, [r0, #0xc]
  4914b4: e1a06000     	mov	r6, r0
  4914b8: e3530000     	cmp	r3, #0
  4914bc: da000008     	ble	0x4914e4 <rnd::Tile::Print()+0x38> @ imm = #0x20
  4914c0: e1a05000     	mov	r5, r0
  4914c4: e3a04000     	mov	r4, #0
  4914c8: e5950010     	ldr	r0, [r5, #0x10]
  4914cc: ebfffff6     	bl	0x4914ac <rnd::Tile::Print()> @ imm = #-0x28
  4914d0: e596300c     	ldr	r3, [r6, #0xc]
  4914d4: e2844001     	add	r4, r4, #1
  4914d8: e2855004     	add	r5, r5, #4
  4914dc: e1530004     	cmp	r3, r4
  4914e0: cafffff8     	bgt	0x4914c8 <rnd::Tile::Print()+0x1c> @ imm = #-0x20
  4914e4: e8bd8070     	pop	{r4, r5, r6, pc}
