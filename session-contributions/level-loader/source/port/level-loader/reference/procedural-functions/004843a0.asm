
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

004843a0 <rnd::RandomGenerator::UnloadTiles()>:
  4843a0: e92d4010     	push	{r4, lr}
  4843a4: e1a04000     	mov	r4, r0
  4843a8: e5900114     	ldr	r0, [r0, #0x114]
  4843ac: e3500000     	cmp	r0, #0
  4843b0: 0a000002     	beq	0x4843c0 <rnd::RandomGenerator::UnloadTiles()+0x20> @ imm = #0x8
  4843b4: eb00353b     	bl	0x4918a8 <rnd::Tile::Unspawn()> @ imm = #0xd4ec
  4843b8: e3a03000     	mov	r3, #0
  4843bc: e5843114     	str	r3, [r4, #0x114]
  4843c0: e8bd8010     	pop	{r4, pc}
