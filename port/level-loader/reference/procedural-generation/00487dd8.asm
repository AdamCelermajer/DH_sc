
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00487dd8 <_ZN7Array2dIPN3rnd4TileEEC1Ev>:
  487dd8: e92d4070     	push	{r4, r5, r6, lr}
  487ddc: e3a03000     	mov	r3, #0
  487de0: e1a04000     	mov	r4, r0
  487de4: e3a00008     	mov	r0, #8
  487de8: e1a01000     	mov	r1, r0
  487dec: e1a02003     	mov	r2, r3
  487df0: e584300c     	str	r3, [r4, #0xc]
  487df4: e5843010     	str	r3, [r4, #0x10]
  487df8: e5843014     	str	r3, [r4, #0x14]
  487dfc: e5843018     	str	r3, [r4, #0x18]
  487e00: e584301c     	str	r3, [r4, #0x1c]
  487e04: e5843020     	str	r3, [r4, #0x20]
  487e08: e5843024     	str	r3, [r4, #0x24]
  487e0c: e5843028     	str	r3, [r4, #0x28]
  487e10: e584302c     	str	r3, [r4, #0x2c]
  487e14: e5840030     	str	r0, [r4, #0x30]
  487e18: e284002c     	add	r0, r4, #44
  487e1c: ebfff39d     	bl	0x484c98 <_ZNSaIPSt5dequeIPN3rnd4TileESaIS2_EEE8allocateEjPKv> @ imm = #-0x318c
  487e20: e1a05000     	mov	r5, r0
  487e24: e584002c     	str	r0, [r4, #0x2c]
  487e28: e1a00004     	mov	r0, r4
  487e2c: e5b06030     	ldr	r6, [r0, #0x30]!
  487e30: ebfffab0     	bl	0x4868f8 <_ZNSaISt5dequeIPN3rnd4TileESaIS2_EEE8allocateEjPKv.clone.22> @ imm = #-0x1540
  487e34: e2466001     	sub	r6, r6, #1
  487e38: e1a060a6     	lsr	r6, r6, #1
  487e3c: e7850106     	str	r0, [r5, r6, lsl #2]
  487e40: e0853106     	add	r3, r5, r6, lsl #2
  487e44: e5843018     	str	r3, [r4, #0x18]
  487e48: e7952106     	ldr	r2, [r5, r6, lsl #2]
  487e4c: e5843028     	str	r3, [r4, #0x28]
  487e50: e1a00004     	mov	r0, r4
  487e54: e2823078     	add	r3, r2, #120
  487e58: e5842010     	str	r2, [r4, #0x10]
  487e5c: e5843014     	str	r3, [r4, #0x14]
  487e60: e7953106     	ldr	r3, [r5, r6, lsl #2]
  487e64: e584200c     	str	r2, [r4, #0xc]
  487e68: e2832078     	add	r2, r3, #120
  487e6c: e5842024     	str	r2, [r4, #0x24]
  487e70: e584301c     	str	r3, [r4, #0x1c]
  487e74: e5843020     	str	r3, [r4, #0x20]
  487e78: ebffff4c     	bl	0x487bb0 <_ZN7Array2dIPN3rnd4TileEE5ClearEv> @ imm = #-0x2d0
  487e7c: e1a00004     	mov	r0, r4
  487e80: e8bd8070     	pop	{r4, r5, r6, pc}
