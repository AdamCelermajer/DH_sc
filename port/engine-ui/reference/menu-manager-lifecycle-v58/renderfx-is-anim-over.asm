
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

007a7dd4 <_ZN8RenderFX10IsAnimOverEPN7gameswf9characterE>:
  7a7dd4: e92d4070     	push	{r4, r5, r6, lr}
  7a7dd8: e2514000     	subs	r4, r1, #0
  7a7ddc: 0a00001c     	beq	0x7a7e54 <_ZN8RenderFX10IsAnimOverEPN7gameswf9characterE+0x80> @ imm = #0x70
  7a7de0: e5943000     	ldr	r3, [r4]
  7a7de4: e1a00004     	mov	r0, r4
  7a7de8: e3a01002     	mov	r1, #2
  7a7dec: e1a0e00f     	mov	lr, pc
  7a7df0: e593f008     	ldr	pc, [r3, #0x8]
  7a7df4: e3500000     	cmp	r0, #0
  7a7df8: 0a000015     	beq	0x7a7e54 <_ZN8RenderFX10IsAnimOverEPN7gameswf9characterE+0x80> @ imm = #0x54
  7a7dfc: e5943000     	ldr	r3, [r4]
  7a7e00: e1a00004     	mov	r0, r4
  7a7e04: e1a0e00f     	mov	lr, pc
  7a7e08: e593f138     	ldr	pc, [r3, #0x138]
  7a7e0c: e5943000     	ldr	r3, [r4]
  7a7e10: e1a00004     	mov	r0, r4
  7a7e14: e1a0e00f     	mov	lr, pc
  7a7e18: e593f13c     	ldr	pc, [r3, #0x13c]
  7a7e1c: e5943000     	ldr	r3, [r4]
  7a7e20: e1a00004     	mov	r0, r4
  7a7e24: e1a0e00f     	mov	lr, pc
  7a7e28: e593f138     	ldr	pc, [r3, #0x138]
  7a7e2c: e5943000     	ldr	r3, [r4]
  7a7e30: e1a05000     	mov	r5, r0
  7a7e34: e1a00004     	mov	r0, r4
  7a7e38: e1a0e00f     	mov	lr, pc
  7a7e3c: e593f13c     	ldr	pc, [r3, #0x13c]
  7a7e40: e2400001     	sub	r0, r0, #1
  7a7e44: e1550000     	cmp	r5, r0
  7a7e48: b3a00000     	movlt	r0, #0
  7a7e4c: a3a00001     	movge	r0, #1
  7a7e50: e8bd8070     	pop	{r4, r5, r6, pc}
  7a7e54: e3a00000     	mov	r0, #0
  7a7e58: e8bd8070     	pop	{r4, r5, r6, pc}
