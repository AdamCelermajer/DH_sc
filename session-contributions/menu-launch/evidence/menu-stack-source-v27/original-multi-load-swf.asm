
R:\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00437d68 <MultiMenuManager::LoadSWFFile(char const*, int)>:
  437d68: e59f30ac     	ldr	r3, [pc, #0xac]         @ 0x437e1c <MultiMenuManager::LoadSWFFile(char const*, int)+0xb4>
  437d6c: e92d47f0     	push	{r4, r5, r6, r7, r8, r9, r10, lr}
  437d70: e3520002     	cmp	r2, #2
  437d74: e1a04002     	mov	r4, r2
  437d78: e59f20a0     	ldr	r2, [pc, #0xa0]         @ 0x437e20 <MultiMenuManager::LoadSWFFile(char const*, int)+0xb8>
  437d7c: e08f3003     	add	r3, pc, r3
  437d80: e284504c     	add	r5, r4, #76
  437d84: e7933002     	ldr	r3, [r3, r2]
  437d88: 03a02001     	moveq	r2, #1
  437d8c: 13a02000     	movne	r2, #0
  437d90: e5c32000     	strb	r2, [r3]
  437d94: e0805105     	add	r5, r0, r5, lsl #2
  437d98: e595a004     	ldr	r10, [r5, #0x4]
  437d9c: e1a06000     	mov	r6, r0
  437da0: e1a07001     	mov	r7, r1
  437da4: e35a0000     	cmp	r10, #0
  437da8: 0a000001     	beq	0x437db4 <MultiMenuManager::LoadSWFFile(char const*, int)+0x4c> @ imm = #0x4
  437dac: e1a0000a     	mov	r0, r10
  437db0: e8bd87f0     	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
  437db4: e3a01008     	mov	r1, #8
  437db8: e3a00f49     	mov	r0, #292
  437dbc: ebfb61eb     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x127854
  437dc0: e1a08000     	mov	r8, r0
  437dc4: eb0dc246     	bl	0x7a86e4 <MenuFX::MenuFX()> @ imm = #0x370918
  437dc8: e5858004     	str	r8, [r5, #0x4]
  437dcc: e1a0200a     	mov	r2, r10
  437dd0: e5983000     	ldr	r3, [r8]
  437dd4: e1a00008     	mov	r0, r8
  437dd8: e1a01007     	mov	r1, r7
  437ddc: e1a0e00f     	mov	lr, pc
  437de0: e593f008     	ldr	pc, [r3, #0x8]
  437de4: e5950004     	ldr	r0, [r5, #0x4]
  437de8: e3a01001     	mov	r1, #1
  437dec: eb0dbfb0     	bl	0x7a7cb4 <RenderFX::SetTextBufferingEnabled(bool)> @ imm = #0x36fec0
  437df0: e3a01008     	mov	r1, #8
  437df4: e3a00034     	mov	r0, #52
  437df8: ebfb61dc     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x127890
  437dfc: e0864104     	add	r4, r6, r4, lsl #2
  437e00: e1a07000     	mov	r7, r0
  437e04: e5951004     	ldr	r1, [r5, #0x4]
  437e08: ebffd3b0     	bl	0x42ccd0 <MenuFlash2DCamera::MenuFlash2DCamera(MenuFX*)> @ imm = #-0xb140
  437e0c: e5847144     	str	r7, [r4, #0x144]
  437e10: e595a004     	ldr	r10, [r5, #0x4]
  437e14: e1a0000a     	mov	r0, r10
  437e18: e8bd87f0     	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
  437e1c: 14 cd 55 00  	.word	0x0055cd14
  437e20: bc 05 00 00  	.word	0x000005bc
