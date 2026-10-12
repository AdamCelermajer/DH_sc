
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003699fc <_ZN15VoxSoundManager9LoadSoundEi>:
  3699fc: e92d45f0     	push	{r4, r5, r6, r7, r8, r10, lr}
  369a00: e59f411c     	ldr	r4, [pc, #0x11c]        @ 0x369b24 <_ZN15VoxSoundManager9LoadSoundEi+0x128>
  369a04: e59f311c     	ldr	r3, [pc, #0x11c]        @ 0x369b28 <_ZN15VoxSoundManager9LoadSoundEi+0x12c>
  369a08: e59f511c     	ldr	r5, [pc, #0x11c]        @ 0x369b2c <_ZN15VoxSoundManager9LoadSoundEi+0x130>
  369a0c: e08f4004     	add	r4, pc, r4
  369a10: e7942003     	ldr	r2, [r4, r3]
  369a14: e7943005     	ldr	r3, [r4, r5]
  369a18: e24ddf8b     	sub	sp, sp, #556
  369a1c: e5d22000     	ldrb	r2, [r2]
  369a20: e5933000     	ldr	r3, [r3]
  369a24: e1a07000     	mov	r7, r0
  369a28: e3520000     	cmp	r2, #0
  369a2c: e1a06001     	mov	r6, r1
  369a30: e58d3224     	str	r3, [sp, #0x224]
  369a34: 1a000004     	bne	0x369a4c <_ZN15VoxSoundManager9LoadSoundEi+0x50> @ imm = #0x10
  369a38: e3510000     	cmp	r1, #0
  369a3c: ba000002     	blt	0x369a4c <_ZN15VoxSoundManager9LoadSoundEi+0x50> @ imm = #0x8
  369a40: e590301c     	ldr	r3, [r0, #0x1c]
  369a44: e1510003     	cmp	r1, r3
  369a48: da000006     	ble	0x369a68 <_ZN15VoxSoundManager9LoadSoundEi+0x6c> @ imm = #0x18
  369a4c: e7943005     	ldr	r3, [r4, r5]
  369a50: e59d2224     	ldr	r2, [sp, #0x224]
  369a54: e5933000     	ldr	r3, [r3]
  369a58: e1520003     	cmp	r2, r3
  369a5c: 1a00002f     	bne	0x369b20 <_ZN15VoxSoundManager9LoadSoundEi+0x124> @ imm = #0xbc
  369a60: e28ddf8b     	add	sp, sp, #556
  369a64: e8bd85f0     	pop	{r4, r5, r6, r7, r8, r10, pc}
  369a68: e28dc020     	add	r12, sp, #32
  369a6c: e58dc000     	str	r12, [sp]
  369a70: e28dc01c     	add	r12, sp, #28
  369a74: e28d3018     	add	r3, sp, #24
  369a78: e58dc004     	str	r12, [sp, #0x4]
  369a7c: e2800064     	add	r0, r0, #100
  369a80: e28dc014     	add	r12, sp, #20
  369a84: e28d2010     	add	r2, sp, #16
  369a88: e58dc008     	str	r12, [sp, #0x8]
  369a8c: eb147f18     	bl	0x8896f4 <_ZNK3vox15VoxSoundPackXML17GetDataSourceInfoEiRPKcRNS_11FormatTypesERiS6_RNS_21VoxSourceLoadingFlagsE> @ imm = #0x51fc60
  369a90: e597301c     	ldr	r3, [r7, #0x1c]
  369a94: e1560003     	cmp	r6, r3
  369a98: caffffeb     	bgt	0x369a4c <_ZN15VoxSoundManager9LoadSoundEi+0x50> @ imm = #-0x54
  369a9c: e5973008     	ldr	r3, [r7, #0x8]
  369aa0: e7933106     	ldr	r3, [r3, r6, lsl #2]
  369aa4: e3530000     	cmp	r3, #0
  369aa8: 1affffe7     	bne	0x369a4c <_ZN15VoxSoundManager9LoadSoundEi+0x50> @ imm = #-0x64
  369aac: e59f307c     	ldr	r3, [pc, #0x7c]         @ 0x369b30 <_ZN15VoxSoundManager9LoadSoundEi+0x134>
  369ab0: e28da024     	add	r10, sp, #36
  369ab4: e1a0000a     	mov	r0, r10
  369ab8: e7943003     	ldr	r3, [r4, r3]
  369abc: e5931000     	ldr	r1, [r3]
  369ac0: ebfe9296     	bl	0x30e520 <.plt+0x7ac>   @ imm = #-0x5b5a8
  369ac4: e1a0000a     	mov	r0, r10
  369ac8: ebfe90e1     	bl	0x30de54 <.plt+0xe0>    @ imm = #-0x5bc7c
  369acc: e59f1060     	ldr	r1, [pc, #0x60]         @ 0x369b34 <_ZN15VoxSoundManager9LoadSoundEi+0x138>
  369ad0: e3a0200d     	mov	r2, #13
  369ad4: e08a0000     	add	r0, r10, r0
  369ad8: e08f1001     	add	r1, pc, r1
  369adc: ebfe9361     	bl	0x30e868 <.plt+0xaf4>   @ imm = #-0x5b27c
  369ae0: e59d1010     	ldr	r1, [sp, #0x10]
  369ae4: e1a0000a     	mov	r0, r10
  369ae8: ebfe94a8     	bl	0x30ed90 <.plt+0x101c>  @ imm = #-0x5ad60
  369aec: e3a01004     	mov	r1, #4
  369af0: e3a00028     	mov	r0, #40
  369af4: ebfe9a9d     	bl	0x310570 <_Znwj15MemoryHintState> @ imm = #-0x5958c
  369af8: e59dc020     	ldr	r12, [sp, #0x20]
  369afc: e59d3014     	ldr	r3, [sp, #0x14]
  369b00: e1a0100a     	mov	r1, r10
  369b04: e59d2018     	ldr	r2, [sp, #0x18]
  369b08: e1a08000     	mov	r8, r0
  369b0c: e58dc000     	str	r12, [sp]
  369b10: eb1416b9     	bl	0x86f5fc <_ZN3vox8VoxUtils24LoadDataSourceFromFileExEPKciNS_21VoxSourceLoadingFlagsEi> @ imm = #0x505ae4
  369b14: e5973008     	ldr	r3, [r7, #0x8]
  369b18: e7838106     	str	r8, [r3, r6, lsl #2]
  369b1c: eaffffca     	b	0x369a4c <_ZN15VoxSoundManager9LoadSoundEi+0x50> @ imm = #-0xd8
  369b20: ebfe91fa     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0x5b818
  369b24: 84 b0 62 00  	.word	0x0062b084
  369b28: 30 3b 00 00  	.word	0x00003b30
  369b2c: ac 40 00 00  	.word	0x000040ac
  369b30: 00 06 00 00  	.word	0x00000600
  369b34: 98 74 55 00  	.word	0x00557498
