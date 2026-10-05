
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00470c24 <VisualObject::SetPosition(Point3D<float> const&)>:
  470c24: e92d4010     	push	{r4, lr}
  470c28: e1a04000     	mov	r4, r0
  470c2c: e5900008     	ldr	r0, [r0, #0x8]
  470c30: e24dd010     	sub	sp, sp, #16
  470c34: e3500000     	cmp	r0, #0
  470c38: 0a00000f     	beq	0x470c7c <VisualObject::SetPosition(Point3D<float> const&)+0x58> @ imm = #0x3c
  470c3c: e5903000     	ldr	r3, [r0]
  470c40: e591e000     	ldr	lr, [r1]
  470c44: e591c004     	ldr	r12, [r1, #0x4]
  470c48: e5912008     	ldr	r2, [r1, #0x8]
  470c4c: e59330a4     	ldr	r3, [r3, #0xa4]
  470c50: e28d1004     	add	r1, sp, #4
  470c54: e58de004     	str	lr, [sp, #0x4]
  470c58: e58dc008     	str	r12, [sp, #0x8]
  470c5c: e58d200c     	str	r2, [sp, #0xc]
  470c60: e12fff33     	blx	r3
  470c64: e5943008     	ldr	r3, [r4, #0x8]
  470c68: e3a01000     	mov	r1, #0
  470c6c: e1a00003     	mov	r0, r3
  470c70: e5933000     	ldr	r3, [r3]
  470c74: e1a0e00f     	mov	lr, pc
  470c78: e593f0b8     	ldr	pc, [r3, #0xb8]
  470c7c: e28dd010     	add	sp, sp, #16
  470c80: e8bd8010     	pop	{r4, pc}
