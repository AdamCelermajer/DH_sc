
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003d4d80 <CharAI::AI_SetMaster(Character*)>:
  3d4d80: e92d4ff8     	push	{r3, r4, r5, r6, r7, r8, r9, r10, r11, lr}
  3d4d84: e59f310c     	ldr	r3, [pc, #0x10c]        @ 0x3d4e98 <CharAI::AI_SetMaster(Character*)+0x118>
  3d4d88: e3510000     	cmp	r1, #0
  3d4d8c: e5801050     	str	r1, [r0, #0x50]
  3d4d90: e1a04000     	mov	r4, r0
  3d4d94: e08f3003     	add	r3, pc, r3
  3d4d98: 0a00003d     	beq	0x3d4e94 <CharAI::AI_SetMaster(Character*)+0x114> @ imm = #0xf4
  3d4d9c: e59f20f8     	ldr	r2, [pc, #0xf8]         @ 0x3d4e9c <CharAI::AI_SetMaster(Character*)+0x11c>
  3d4da0: e5900004     	ldr	r0, [r0, #0x4]
  3d4da4: e7933002     	ldr	r3, [r3, r2]
  3d4da8: e5939000     	ldr	r9, [r3]
  3d4dac: ebff388e     	bl	0x3a2fec <Character::GetCharAIId() const> @ imm = #-0x31dc8
  3d4db0: e5943050     	ldr	r3, [r4, #0x50]
  3d4db4: e1a0b000     	mov	r11, r0
  3d4db8: e1a00003     	mov	r0, r3
  3d4dbc: e5933000     	ldr	r3, [r3]
  3d4dc0: e1a0e00f     	mov	lr, pc
  3d4dc4: e593f034     	ldr	pc, [r3, #0x34]
  3d4dc8: e2200001     	eor	r0, r0, #1
  3d4dcc: e5c40054     	strb	r0, [r4, #0x54]
  3d4dd0: e5940050     	ldr	r0, [r4, #0x50]
  3d4dd4: ebfefa00     	bl	0x3935dc <GameObject::GetTargetPosition() const> @ imm = #-0x41800
  3d4dd8: e1a05000     	mov	r5, r0
  3d4ddc: e5940004     	ldr	r0, [r4, #0x4]
  3d4de0: ebfef9fd     	bl	0x3935dc <GameObject::GetTargetPosition() const> @ imm = #-0x4180c
  3d4de4: e5901000     	ldr	r1, [r0]
  3d4de8: e1a06000     	mov	r6, r0
  3d4dec: e5950000     	ldr	r0, [r5]
  3d4df0: ebfce56d     	bl	0x30e3ac <.plt+0x638>   @ imm = #-0xc6a4c
  3d4df4: e5961004     	ldr	r1, [r6, #0x4]
  3d4df8: e1a0a000     	mov	r10, r0
  3d4dfc: e5950004     	ldr	r0, [r5, #0x4]
  3d4e00: ebfce569     	bl	0x30e3ac <.plt+0x638>   @ imm = #-0xc6a5c
  3d4e04: e5961008     	ldr	r1, [r6, #0x8]
  3d4e08: e1a08000     	mov	r8, r0
  3d4e0c: e5950008     	ldr	r0, [r5, #0x8]
  3d4e10: ebfce565     	bl	0x30e3ac <.plt+0x638>   @ imm = #-0xc6a6c
  3d4e14: e3a03044     	mov	r3, #68
  3d4e18: e0299b93     	mla	r9, r3, r11, r9
  3d4e1c: e1a07000     	mov	r7, r0
  3d4e20: e599003c     	ldr	r0, [r9, #0x3c]
  3d4e24: e3a03000     	mov	r3, #0
  3d4e28: e5c43055     	strb	r3, [r4, #0x55]
  3d4e2c: e1a01000     	mov	r1, r0
  3d4e30: ebfce7cd     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0xc60cc
  3d4e34: e1a0100a     	mov	r1, r10
  3d4e38: e1a05000     	mov	r5, r0
  3d4e3c: e1a0000a     	mov	r0, r10
  3d4e40: ebfce7c9     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0xc60dc
  3d4e44: e1a01008     	mov	r1, r8
  3d4e48: e1a06000     	mov	r6, r0
  3d4e4c: e1a00008     	mov	r0, r8
  3d4e50: ebfce7c5     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0xc60ec
  3d4e54: e1a01000     	mov	r1, r0
  3d4e58: e1a00006     	mov	r0, r6
  3d4e5c: ebfce750     	bl	0x30eba4 <.plt+0xe30>   @ imm = #-0xc62c0
  3d4e60: e1a01007     	mov	r1, r7
  3d4e64: e1a06000     	mov	r6, r0
  3d4e68: e1a00007     	mov	r0, r7
  3d4e6c: ebfce7be     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0xc6108
  3d4e70: e1a01000     	mov	r1, r0
  3d4e74: e1a00006     	mov	r0, r6
  3d4e78: ebfce749     	bl	0x30eba4 <.plt+0xe30>   @ imm = #-0xc62dc
  3d4e7c: e1a01000     	mov	r1, r0
  3d4e80: e1a00005     	mov	r0, r5
  3d4e84: ebfce51b     	bl	0x30e2f8 <.plt+0x584>   @ imm = #-0xc6b94
  3d4e88: e3500000     	cmp	r0, #0
  3d4e8c: 13a03001     	movne	r3, #1
  3d4e90: 15c43055     	strbne	r3, [r4, #0x55]
  3d4e94: e8bd8ff8     	pop	{r3, r4, r5, r6, r7, r8, r9, r10, r11, pc}
  3d4e98: fc fc 5b 00  	.word	0x005bfcfc
  3d4e9c: 58 07 00 00  	.word	0x00000758
