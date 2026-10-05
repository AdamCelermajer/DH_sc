
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0047295c <VisualObject::SetParent(GameObject*)>:
  47295c: e92d4070     	push	{r4, r5, r6, lr}
  472960: e59f409c     	ldr	r4, [pc, #0x9c]         @ 0x472a04 <VisualObject::SetParent(GameObject*)+0xa8>
  472964: e3510000     	cmp	r1, #0
  472968: e5801004     	str	r1, [r0, #0x4]
  47296c: e1a05000     	mov	r5, r0
  472970: e08f4004     	add	r4, pc, r4
  472974: 0a000013     	beq	0x4729c8 <VisualObject::SetParent(GameObject*)+0x6c> @ imm = #0x4c
  472978: e5d13084     	ldrb	r3, [r1, #0x84]
  47297c: e3530000     	cmp	r3, #0
  472980: 1a000007     	bne	0x4729a4 <VisualObject::SetParent(GameObject*)+0x48> @ imm = #0x1c
  472984: e1a00005     	mov	r0, r5
  472988: ebfc6439     	bl	0x38ba74 <VisualObject::Sync()> @ imm = #-0xe6f1c
  47298c: e59f3074     	ldr	r3, [pc, #0x74]         @ 0x472a08 <VisualObject::SetParent(GameObject*)+0xac>
  472990: e5950008     	ldr	r0, [r5, #0x8]
  472994: e3a01001     	mov	r1, #1
  472998: e7942003     	ldr	r2, [r4, r3]
  47299c: e8bd4070     	pop	{r4, r5, r6, lr}
  4729a0: ea026eb7     	b	0x50e484 <RecursiveSetBoolOnNode(glitch::scene::ISceneNode*, bool, void (*)(glitch::scene::ISceneNode*, bool))> @ imm = #0x9badc
  4729a4: e1a00001     	mov	r0, r1
  4729a8: e5913000     	ldr	r3, [r1]
  4729ac: e1a0e00f     	mov	lr, pc
  4729b0: e593f080     	ldr	pc, [r3, #0x80]
  4729b4: e3500000     	cmp	r0, #0
  4729b8: 0a000003     	beq	0x4729cc <VisualObject::SetParent(GameObject*)+0x70> @ imm = #0xc
  4729bc: e5953004     	ldr	r3, [r5, #0x4]
  4729c0: e3530000     	cmp	r3, #0
  4729c4: 1affffee     	bne	0x472984 <VisualObject::SetParent(GameObject*)+0x28> @ imm = #-0x48
  4729c8: e8bd8070     	pop	{r4, r5, r6, pc}
  4729cc: e1a00005     	mov	r0, r5
  4729d0: ebfc6427     	bl	0x38ba74 <VisualObject::Sync()> @ imm = #-0xe6f64
  4729d4: e5956008     	ldr	r6, [r5, #0x8]
  4729d8: e5963000     	ldr	r3, [r6]
  4729dc: e1a00006     	mov	r0, r6
  4729e0: e59340a4     	ldr	r4, [r3, #0xa4]
  4729e4: e1a0e00f     	mov	lr, pc
  4729e8: e593f0a0     	ldr	pc, [r3, #0xa0]
  4729ec: e1a01000     	mov	r1, r0
  4729f0: e1a00006     	mov	r0, r6
  4729f4: e12fff34     	blx	r4
  4729f8: e5950008     	ldr	r0, [r5, #0x8]
  4729fc: e8bd4070     	pop	{r4, r5, r6, lr}
  472a00: ea027206     	b	0x50f220 <OptimizeStatic(glitch::scene::ISceneNode*)> @ imm = #0x9c818
  472a04: 20 21 52 00  	.word	0x00522120
  472a08: 74 30 00 00  	.word	0x00003074
