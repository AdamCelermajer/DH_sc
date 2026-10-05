
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048bebc <_ZN3rnd4Rule4ImplC2ERKS0_PS1_>:
  48bebc: e92d4070     	push	{r4, r5, r6, lr}
  48bec0: e59f4134     	ldr	r4, [pc, #0x134]        @ 0x48bffc <_ZN3rnd4Rule4ImplC2ERKS0_PS1_+0x140>
  48bec4: e59f3134     	ldr	r3, [pc, #0x134]        @ 0x48c000 <_ZN3rnd4Rule4ImplC2ERKS0_PS1_+0x144>
  48bec8: e1a05000     	mov	r5, r0
  48becc: e08f4004     	add	r4, pc, r4
  48bed0: e7943003     	ldr	r3, [r4, r3]
  48bed4: e3a00000     	mov	r0, #0
  48bed8: e3520000     	cmp	r2, #0
  48bedc: e2833008     	add	r3, r3, #8
  48bee0: e5853000     	str	r3, [r5]
  48bee4: e3a03001     	mov	r3, #1
  48bee8: e5851004     	str	r1, [r5, #0x4]
  48beec: e585302c     	str	r3, [r5, #0x2c]
  48bef0: e5850040     	str	r0, [r5, #0x40]
  48bef4: e5852008     	str	r2, [r5, #0x8]
  48bef8: e585000c     	str	r0, [r5, #0xc]
  48befc: e5850028     	str	r0, [r5, #0x28]
  48bf00: e5850030     	str	r0, [r5, #0x30]
  48bf04: e5850034     	str	r0, [r5, #0x34]
  48bf08: e5850038     	str	r0, [r5, #0x38]
  48bf0c: e585003c     	str	r0, [r5, #0x3c]
  48bf10: 0a000012     	beq	0x48bf60 <_ZN3rnd4Rule4ImplC2ERKS0_PS1_+0xa4> @ imm = #0x48
  48bf14: e592300c     	ldr	r3, [r2, #0xc]
  48bf18: e2831001     	add	r1, r3, #1
  48bf1c: e2833004     	add	r3, r3, #4
  48bf20: e7825103     	str	r5, [r2, r3, lsl #2]
  48bf24: e582100c     	str	r1, [r2, #0xc]
  48bf28: e5950004     	ldr	r0, [r5, #0x4]
  48bf2c: e5902060     	ldr	r2, [r0, #0x60]
  48bf30: e5903064     	ldr	r3, [r0, #0x64]
  48bf34: e1520003     	cmp	r2, r3
  48bf38: 0a000008     	beq	0x48bf60 <_ZN3rnd4Rule4ImplC2ERKS0_PS1_+0xa4> @ imm = #0x20
  48bf3c: e59f10c0     	ldr	r1, [pc, #0xc0]         @ 0x48c004 <_ZN3rnd4Rule4ImplC2ERKS0_PS1_+0x148>
  48bf40: e2800050     	add	r0, r0, #80
  48bf44: e08f1001     	add	r1, pc, r1
  48bf48: ebfc65f4     	bl	0x3a5720 <_ZNKSs7compareEPKc> @ imm = #-0xe6830
  48bf4c: e3500000     	cmp	r0, #0
  48bf50: 1a000004     	bne	0x48bf68 <_ZN3rnd4Rule4ImplC2ERKS0_PS1_+0xac> @ imm = #0x10
  48bf54: e59f30ac     	ldr	r3, [pc, #0xac]         @ 0x48c008 <_ZN3rnd4Rule4ImplC2ERKS0_PS1_+0x14c>
  48bf58: e7943003     	ldr	r3, [r4, r3]
  48bf5c: e585303c     	str	r3, [r5, #0x3c]
  48bf60: e1a00005     	mov	r0, r5
  48bf64: e8bd8070     	pop	{r4, r5, r6, pc}
  48bf68: e5950004     	ldr	r0, [r5, #0x4]
  48bf6c: e59f1098     	ldr	r1, [pc, #0x98]         @ 0x48c00c <_ZN3rnd4Rule4ImplC2ERKS0_PS1_+0x150>
  48bf70: e2800050     	add	r0, r0, #80
  48bf74: e08f1001     	add	r1, pc, r1
  48bf78: ebfc65e8     	bl	0x3a5720 <_ZNKSs7compareEPKc> @ imm = #-0xe6860
  48bf7c: e3500000     	cmp	r0, #0
  48bf80: 1a000005     	bne	0x48bf9c <_ZN3rnd4Rule4ImplC2ERKS0_PS1_+0xe0> @ imm = #0x14
  48bf84: e59f307c     	ldr	r3, [pc, #0x7c]         @ 0x48c008 <_ZN3rnd4Rule4ImplC2ERKS0_PS1_+0x14c>
  48bf88: e1a00005     	mov	r0, r5
  48bf8c: e7943003     	ldr	r3, [r4, r3]
  48bf90: e2833010     	add	r3, r3, #16
  48bf94: e585303c     	str	r3, [r5, #0x3c]
  48bf98: e8bd8070     	pop	{r4, r5, r6, pc}
  48bf9c: e5950004     	ldr	r0, [r5, #0x4]
  48bfa0: e59f1068     	ldr	r1, [pc, #0x68]         @ 0x48c010 <_ZN3rnd4Rule4ImplC2ERKS0_PS1_+0x154>
  48bfa4: e2800050     	add	r0, r0, #80
  48bfa8: e08f1001     	add	r1, pc, r1
  48bfac: ebfc65db     	bl	0x3a5720 <_ZNKSs7compareEPKc> @ imm = #-0xe6894
  48bfb0: e3500000     	cmp	r0, #0
  48bfb4: 1a000004     	bne	0x48bfcc <_ZN3rnd4Rule4ImplC2ERKS0_PS1_+0x110> @ imm = #0x10
  48bfb8: e59f3048     	ldr	r3, [pc, #0x48]         @ 0x48c008 <_ZN3rnd4Rule4ImplC2ERKS0_PS1_+0x14c>
  48bfbc: e7943003     	ldr	r3, [r4, r3]
  48bfc0: e2833020     	add	r3, r3, #32
  48bfc4: e585303c     	str	r3, [r5, #0x3c]
  48bfc8: eaffffe4     	b	0x48bf60 <_ZN3rnd4Rule4ImplC2ERKS0_PS1_+0xa4> @ imm = #-0x70
  48bfcc: e5950004     	ldr	r0, [r5, #0x4]
  48bfd0: e59f103c     	ldr	r1, [pc, #0x3c]         @ 0x48c014 <_ZN3rnd4Rule4ImplC2ERKS0_PS1_+0x158>
  48bfd4: e2800050     	add	r0, r0, #80
  48bfd8: e08f1001     	add	r1, pc, r1
  48bfdc: ebfc65cf     	bl	0x3a5720 <_ZNKSs7compareEPKc> @ imm = #-0xe68c4
  48bfe0: e3500000     	cmp	r0, #0
  48bfe4: 1affffdd     	bne	0x48bf60 <_ZN3rnd4Rule4ImplC2ERKS0_PS1_+0xa4> @ imm = #-0x8c
  48bfe8: e59f3018     	ldr	r3, [pc, #0x18]         @ 0x48c008 <_ZN3rnd4Rule4ImplC2ERKS0_PS1_+0x14c>
  48bfec: e7943003     	ldr	r3, [r4, r3]
  48bff0: e2833030     	add	r3, r3, #48
  48bff4: e585303c     	str	r3, [r5, #0x3c]
  48bff8: eaffffd8     	b	0x48bf60 <_ZN3rnd4Rule4ImplC2ERKS0_PS1_+0xa4> @ imm = #-0xa0
  48bffc: c4 8b 50 00  	.word	0x00508bc4
  48c000: 00 10 00 00  	.word	0x00001000
  48c004: 74 8e 44 00  	.word	0x00448e74
  48c008: fc 43 00 00  	.word	0x000043fc
  48c00c: 4c 8e 44 00  	.word	0x00448e4c
  48c010: 20 8e 44 00  	.word	0x00448e20
  48c014: f8 8d 44 00  	.word	0x00448df8
