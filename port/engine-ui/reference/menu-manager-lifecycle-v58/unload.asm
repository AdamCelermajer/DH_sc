
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0042d4bc <_ZN11MenuManager10UnloadMenuEi>:
  42d4bc: e3510001     	cmp	r1, #1
  42d4c0: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  42d4c4: e1a07001     	mov	r7, r1
  42d4c8: e1a06000     	mov	r6, r0
  42d4cc: 0a00000a     	beq	0x42d4fc <_ZN11MenuManager10UnloadMenuEi+0x40> @ imm = #0x28
  42d4d0: e3510003     	cmp	r1, #3
  42d4d4: e59030f4     	ldr	r3, [r0, #0xf4]
  42d4d8: 83a05000     	movhi	r5, #0
  42d4dc: 9a000009     	bls	0x42d508 <_ZN11MenuManager10UnloadMenuEi+0x4c> @ imm = #0x24
  42d4e0: ebff9a6a     	bl	0x413e90 <_ZN16FlashAnimManager11GetInstanceEv> @ imm = #-0x19658
  42d4e4: e1a01005     	mov	r1, r5
  42d4e8: ebff98d4     	bl	0x413840 <_ZN16FlashAnimManager17ResetScanForAnimsEP6MenuFX> @ imm = #-0x19cb0
  42d4ec: e59600f4     	ldr	r0, [r6, #0xf4]
  42d4f0: e1a01007     	mov	r1, r7
  42d4f4: e8bd41f0     	pop	{r4, r5, r6, r7, r8, lr}
  42d4f8: ea00298f     	b	0x437b3c <_ZN16MultiMenuManager13UnloadSWFFileEi> @ imm = #0xa63c
  42d4fc: eb009a28     	bl	0x453da4 <_ZN16MenuCharMenu_Map11GetInstanceEv> @ imm = #0x268a0
  42d500: eb00978b     	bl	0x453334 <_ZN16MenuCharMenu_Map13ClearAllIconsEv> @ imm = #0x25e2c
  42d504: e59630f4     	ldr	r3, [r6, #0xf4]
  42d508: e0833107     	add	r3, r3, r7, lsl #2
  42d50c: e5935134     	ldr	r5, [r3, #0x134]
  42d510: e3550000     	cmp	r5, #0
  42d514: 0afffff1     	beq	0x42d4e0 <_ZN11MenuManager10UnloadMenuEi+0x24> @ imm = #-0x3c
  42d518: e5964064     	ldr	r4, [r6, #0x64]
  42d51c: e5961068     	ldr	r1, [r6, #0x68]
  42d520: e3a08000     	mov	r8, #0
  42d524: e1540001     	cmp	r4, r1
  42d528: 0affffec     	beq	0x42d4e0 <_ZN11MenuManager10UnloadMenuEi+0x24> @ imm = #-0x50
  42d52c: e5943000     	ldr	r3, [r4]
  42d530: e5932004     	ldr	r2, [r3, #0x4]
  42d534: e1550002     	cmp	r5, r2
  42d538: 12844004     	addne	r4, r4, #4
  42d53c: 1afffff8     	bne	0x42d524 <_ZN11MenuManager10UnloadMenuEi+0x68> @ imm = #-0x20
  42d540: e5838004     	str	r8, [r3, #0x4]
  42d544: e5943000     	ldr	r3, [r4]
  42d548: e5d3207d     	ldrb	r2, [r3, #0x7d]
  42d54c: e1a00003     	mov	r0, r3
  42d550: e3520000     	cmp	r2, #0
  42d554: 0a000002     	beq	0x42d564 <_ZN11MenuManager10UnloadMenuEi+0xa8> @ imm = #0x8
  42d558: e5933000     	ldr	r3, [r3]
  42d55c: e1a0e00f     	mov	lr, pc
  42d560: e593f004     	ldr	pc, [r3, #0x4]
  42d564: e5963068     	ldr	r3, [r6, #0x68]
  42d568: e2841004     	add	r1, r4, #4
  42d56c: e1510003     	cmp	r1, r3
  42d570: 0a000002     	beq	0x42d580 <_ZN11MenuManager10UnloadMenuEi+0xc4> @ imm = #0x8
  42d574: e0532001     	subs	r2, r3, r1
  42d578: 01a01003     	moveq	r1, r3
  42d57c: 1a000002     	bne	0x42d58c <_ZN11MenuManager10UnloadMenuEi+0xd0> @ imm = #0x8
  42d580: e2411004     	sub	r1, r1, #4
  42d584: e5861068     	str	r1, [r6, #0x68]
  42d588: eaffffe5     	b	0x42d524 <_ZN11MenuManager10UnloadMenuEi+0x68> @ imm = #-0x6c
  42d58c: e1a00004     	mov	r0, r4
  42d590: ebfb8268     	bl	0x30df38 <.plt+0x1c4>   @ imm = #-0x11f660
  42d594: e5961068     	ldr	r1, [r6, #0x68]
  42d598: eafffff8     	b	0x42d580 <_ZN11MenuManager10UnloadMenuEi+0xc4> @ imm = #-0x20
