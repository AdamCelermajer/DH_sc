
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00491ab0 <_ZN3rnd4Tile8TrySpawnEPKNS_4ExitES3_S3_RNS_8ListElemE>:
  491ab0: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  491ab4: e1a04003     	mov	r4, r3
  491ab8: e59f3118     	ldr	r3, [pc, #0x118]        @ 0x491bd8 <_ZN3rnd4Tile8TrySpawnEPKNS_4ExitES3_S3_RNS_8ListElemE+0x128>
  491abc: e594c014     	ldr	r12, [r4, #0x14]
  491ac0: e59f1114     	ldr	r1, [pc, #0x114]        @ 0x491bdc <_ZN3rnd4Tile8TrySpawnEPKNS_4ExitES3_S3_RNS_8ListElemE+0x12c>
  491ac4: e08f3003     	add	r3, pc, r3
  491ac8: e59cc000     	ldr	r12, [r12]
  491acc: e7931001     	ldr	r1, [r3, r1]
  491ad0: e1a05000     	mov	r5, r0
  491ad4: e595e084     	ldr	lr, [r5, #0x84]
  491ad8: e791c10c     	ldr	r12, [r1, r12, lsl #2]
  491adc: e59f10fc     	ldr	r1, [pc, #0xfc]         @ 0x491be0 <_ZN3rnd4Tile8TrySpawnEPKNS_4ExitES3_S3_RNS_8ListElemE+0x130>
  491ae0: e3520000     	cmp	r2, #0
  491ae4: e1a0c08c     	lsl	r12, r12, #1
  491ae8: e7930001     	ldr	r0, [r3, r1]
  491aec: e28cc001     	add	r12, r12, #1
  491af0: e24dd00c     	sub	sp, sp, #12
  491af4: e080118c     	add	r1, r0, r12, lsl #3
  491af8: e7d0918c     	ldrb	r9, [r0, r12, lsl #3]
  491afc: e5d1b001     	ldrb	r11, [r1, #0x1]
  491b00: e5d1a005     	ldrb	r10, [r1, #0x5]
  491b04: e5d16004     	ldrb	r6, [r1, #0x4]
  491b08: e5d18002     	ldrb	r8, [r1, #0x2]
  491b0c: e5d1c006     	ldrb	r12, [r1, #0x6]
  491b10: e5d10007     	ldrb	r0, [r1, #0x7]
  491b14: e5d11003     	ldrb	r1, [r1, #0x3]
  491b18: e189740b     	orr	r7, r9, r11, lsl #8
  491b1c: e186640a     	orr	r6, r6, r10, lsl #8
  491b20: e595b088     	ldr	r11, [r5, #0x88]
  491b24: e1877808     	orr	r7, r7, r8, lsl #16
  491b28: e186680c     	orr	r6, r6, r12, lsl #16
  491b2c: e5949008     	ldr	r9, [r4, #0x8]
  491b30: e594a00c     	ldr	r10, [r4, #0xc]
  491b34: e1877c01     	orr	r7, r7, r1, lsl #24
  491b38: e1866c00     	orr	r6, r6, r0, lsl #24
  491b3c: e087700e     	add	r7, r7, lr
  491b40: e086600b     	add	r6, r6, r11
  491b44: e0697007     	rsb	r7, r9, r7
  491b48: e06a6006     	rsb	r6, r10, r6
  491b4c: e595808c     	ldr	r8, [r5, #0x8c]
  491b50: 0a000007     	beq	0x491b74 <_ZN3rnd4Tile8TrySpawnEPKNS_4ExitES3_S3_RNS_8ListElemE+0xc4> @ imm = #0x1c
  491b54: e592c008     	ldr	r12, [r2, #0x8]
  491b58: e592300c     	ldr	r3, [r2, #0xc]
  491b5c: e1a00008     	mov	r0, r8
  491b60: e5921010     	ldr	r1, [r2, #0x10]
  491b64: e087700c     	add	r7, r7, r12
  491b68: e0866003     	add	r6, r6, r3
  491b6c: ebf9f40c     	bl	0x30eba4 <.plt+0xe30>   @ imm = #-0x182fd0
  491b70: e1a08000     	mov	r8, r0
  491b74: e5943004     	ldr	r3, [r4, #0x4]
  491b78: e5951004     	ldr	r1, [r5, #0x4]
  491b7c: e1a02007     	mov	r2, r7
  491b80: e1a00003     	mov	r0, r3
  491b84: e593c000     	ldr	r12, [r3]
  491b88: e2811008     	add	r1, r1, #8
  491b8c: e1a03006     	mov	r3, r6
  491b90: e594a010     	ldr	r10, [r4, #0x10]
  491b94: e1a0e00f     	mov	lr, pc
  491b98: e59cf010     	ldr	pc, [r12, #0x10]
  491b9c: e3500000     	cmp	r0, #0
  491ba0: 0a00000a     	beq	0x491bd0 <_ZN3rnd4Tile8TrySpawnEPKNS_4ExitES3_S3_RNS_8ListElemE+0x120> @ imm = #0x28
  491ba4: e1a0100a     	mov	r1, r10
  491ba8: e1a00008     	mov	r0, r8
  491bac: ebf9f1fe     	bl	0x30e3ac <.plt+0x638>   @ imm = #-0x183808
  491bb0: e59dc030     	ldr	r12, [sp, #0x30]
  491bb4: e5941004     	ldr	r1, [r4, #0x4]
  491bb8: e1a02007     	mov	r2, r7
  491bbc: e58d0000     	str	r0, [sp]
  491bc0: e1a03006     	mov	r3, r6
  491bc4: e1a00005     	mov	r0, r5
  491bc8: e58dc004     	str	r12, [sp, #0x4]
  491bcc: ebffff77     	bl	0x4919b0 <_ZN3rnd4Tile5SpawnERNS_5BlockEiifRNS_8ListElemE> @ imm = #-0x224
  491bd0: e28dd00c     	add	sp, sp, #12
  491bd4: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  491bd8: cc 2f 50 00  	.word	0x00502fcc
  491bdc: b8 1b 00 00  	.word	0x00001bb8
  491be0: fc 43 00 00  	.word	0x000043fc
