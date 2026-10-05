
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0035cc0c <RootSceneNode::ResetPositionFromFile()>:
  35cc0c: e92d45f0     	push	{r4, r5, r6, r7, r8, r10, lr}
  35cc10: e59050f4     	ldr	r5, [r0, #0xf4]
  35cc14: e24dd03c     	sub	sp, sp, #60
  35cc18: e1a08000     	mov	r8, r0
  35cc1c: e3550000     	cmp	r5, #0
  35cc20: 0a00002b     	beq	0x35ccd4 <RootSceneNode::ResetPositionFromFile()+0xc8> @ imm = #0xac
  35cc24: e2556004     	subs	r6, r5, #4
  35cc28: 0a000029     	beq	0x35ccd4 <RootSceneNode::ResetPositionFromFile()+0xc8> @ imm = #0xa4
  35cc2c: e5153004     	ldr	r3, [r5, #-0x4]
  35cc30: e3a04000     	mov	r4, #0
  35cc34: e1a00006     	mov	r0, r6
  35cc38: e59330a4     	ldr	r3, [r3, #0xa4]
  35cc3c: e28d102c     	add	r1, sp, #44
  35cc40: e58d402c     	str	r4, [sp, #0x2c]
  35cc44: e58d4030     	str	r4, [sp, #0x30]
  35cc48: e58d4034     	str	r4, [sp, #0x34]
  35cc4c: e12fff33     	blx	r3
  35cc50: e515c004     	ldr	r12, [r5, #-0x4]
  35cc54: e28da004     	add	r10, sp, #4
  35cc58: e1a02004     	mov	r2, r4
  35cc5c: e1a03004     	mov	r3, r4
  35cc60: e1a01004     	mov	r1, r4
  35cc64: e1a0000a     	mov	r0, r10
  35cc68: e59c709c     	ldr	r7, [r12, #0x9c]
  35cc6c: ebffff59     	bl	0x35c9d8 <glitch::core::quaternion::set(float, float, float)> @ imm = #-0x29c
  35cc70: e1a00006     	mov	r0, r6
  35cc74: e1a0100a     	mov	r1, r10
  35cc78: e12fff37     	blx	r7
  35cc7c: e5153004     	ldr	r3, [r5, #-0x4]
  35cc80: e3a025fe     	mov	r2, #1065353216
  35cc84: e1a00006     	mov	r0, r6
  35cc88: e5933094     	ldr	r3, [r3, #0x94]
  35cc8c: e28d1020     	add	r1, sp, #32
  35cc90: e58d2028     	str	r2, [sp, #0x28]
  35cc94: e58d2020     	str	r2, [sp, #0x20]
  35cc98: e58d2024     	str	r2, [sp, #0x24]
  35cc9c: e12fff33     	blx	r3
  35cca0: e5983000     	ldr	r3, [r8]
  35cca4: e1a00008     	mov	r0, r8
  35cca8: e28d1014     	add	r1, sp, #20
  35ccac: e59330a4     	ldr	r3, [r3, #0xa4]
  35ccb0: e58d401c     	str	r4, [sp, #0x1c]
  35ccb4: e58d4014     	str	r4, [sp, #0x14]
  35ccb8: e58d4018     	str	r4, [sp, #0x18]
  35ccbc: e12fff33     	blx	r3
  35ccc0: e5153004     	ldr	r3, [r5, #-0x4]
  35ccc4: e1a00006     	mov	r0, r6
  35ccc8: e3a01000     	mov	r1, #0
  35cccc: e1a0e00f     	mov	lr, pc
  35ccd0: e593f0b8     	ldr	pc, [r3, #0xb8]
  35ccd4: e28dd03c     	add	sp, sp, #60
  35ccd8: e8bd85f0     	pop	{r4, r5, r6, r7, r8, r10, pc}
