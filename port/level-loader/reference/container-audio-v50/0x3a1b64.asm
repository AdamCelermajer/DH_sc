
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003a1b64 <_ZN17OpenableContainer8InitPostEv>:
  3a1b64: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  3a1b68: e1a08000     	mov	r8, r0
  3a1b6c: ebfff767     	bl	0x39f910 <_ZN9Container8InitPostEv> @ imm = #-0x2264
  3a1b70: e5985704     	ldr	r5, [r8, #0x704]
  3a1b74: e5982700     	ldr	r2, [r8, #0x700]
  3a1b78: e59f3060     	ldr	r3, [pc, #0x60]         @ 0x3a1be0 <_ZN17OpenableContainer8InitPostEv+0x7c>
  3a1b7c: e1550002     	cmp	r5, r2
  3a1b80: e08f3003     	add	r3, pc, r3
  3a1b84: 0a000012     	beq	0x3a1bd4 <_ZN17OpenableContainer8InitPostEv+0x70> @ imm = #0x48
  3a1b88: e59f2054     	ldr	r2, [pc, #0x54]         @ 0x3a1be4 <_ZN17OpenableContainer8InitPostEv+0x80>
  3a1b8c: e7932002     	ldr	r2, [r3, r2]
  3a1b90: e5926000     	ldr	r6, [r2]
  3a1b94: e3560000     	cmp	r6, #0
  3a1b98: 0a00000e     	beq	0x3a1bd8 <_ZN17OpenableContainer8InitPostEv+0x74> @ imm = #0x38
  3a1b9c: e59f2044     	ldr	r2, [pc, #0x44]         @ 0x3a1be8 <_ZN17OpenableContainer8InitPostEv+0x84>
  3a1ba0: e3a04000     	mov	r4, #0
  3a1ba4: e7933002     	ldr	r3, [r3, r2]
  3a1ba8: e5937000     	ldr	r7, [r3]
  3a1bac: ea000002     	b	0x3a1bbc <_ZN17OpenableContainer8InitPostEv+0x58> @ imm = #0x8
  3a1bb0: e2844001     	add	r4, r4, #1
  3a1bb4: e1540006     	cmp	r4, r6
  3a1bb8: 0a000006     	beq	0x3a1bd8 <_ZN17OpenableContainer8InitPostEv+0x74> @ imm = #0x18
  3a1bbc: e7971104     	ldr	r1, [r7, r4, lsl #2]
  3a1bc0: e1a00005     	mov	r0, r5
  3a1bc4: ebfdb1d4     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0x938b0
  3a1bc8: e3500000     	cmp	r0, #0
  3a1bcc: 1afffff7     	bne	0x3a1bb0 <_ZN17OpenableContainer8InitPostEv+0x4c> @ imm = #-0x24
  3a1bd0: e5884710     	str	r4, [r8, #0x710]
  3a1bd4: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  3a1bd8: e3e04000     	mvn	r4, #0
  3a1bdc: eafffffb     	b	0x3a1bd0 <_ZN17OpenableContainer8InitPostEv+0x6c> @ imm = #-0x14
  3a1be0: 10 2f 5f 00  	.word	0x005f2f10
  3a1be4: 60 0d 00 00  	.word	0x00000d60
  3a1be8: 54 1c 00 00  	.word	0x00001c54
