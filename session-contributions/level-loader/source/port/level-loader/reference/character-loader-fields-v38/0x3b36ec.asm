
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003b36ec <Character::SafeGetCharPropsTemplateId()>:
  3b36ec: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  3b36f0: e30133ac     	movw	r3, #0x13ac
  3b36f4: e7905003     	ldr	r5, [r0, r3]
  3b36f8: e30133a8     	movw	r3, #0x13a8
  3b36fc: e7902003     	ldr	r2, [r0, r3]
  3b3700: e59f3088     	ldr	r3, [pc, #0x88]         @ 0x3b3790 <Character::SafeGetCharPropsTemplateId()+0xa4>
  3b3704: e1a08000     	mov	r8, r0
  3b3708: e1520005     	cmp	r2, r5
  3b370c: e08f3003     	add	r3, pc, r3
  3b3710: 0a00001b     	beq	0x3b3784 <Character::SafeGetCharPropsTemplateId()+0x98> @ imm = #0x6c
  3b3714: e59f2078     	ldr	r2, [pc, #0x78]         @ 0x3b3794 <Character::SafeGetCharPropsTemplateId()+0xa8>
  3b3718: e7932002     	ldr	r2, [r3, r2]
  3b371c: e5926000     	ldr	r6, [r2]
  3b3720: e3560000     	cmp	r6, #0
  3b3724: 0a000011     	beq	0x3b3770 <Character::SafeGetCharPropsTemplateId()+0x84> @ imm = #0x44
  3b3728: e59f2068     	ldr	r2, [pc, #0x68]         @ 0x3b3798 <Character::SafeGetCharPropsTemplateId()+0xac>
  3b372c: e3a04000     	mov	r4, #0
  3b3730: e7933002     	ldr	r3, [r3, r2]
  3b3734: e5937000     	ldr	r7, [r3]
  3b3738: ea000002     	b	0x3b3748 <Character::SafeGetCharPropsTemplateId()+0x5c> @ imm = #0x8
  3b373c: e2844001     	add	r4, r4, #1
  3b3740: e1540006     	cmp	r4, r6
  3b3744: 0a000009     	beq	0x3b3770 <Character::SafeGetCharPropsTemplateId()+0x84> @ imm = #0x24
  3b3748: e7971104     	ldr	r1, [r7, r4, lsl #2]
  3b374c: e1a00005     	mov	r0, r5
  3b3750: ebfd6af1     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0xa543c
  3b3754: e3500000     	cmp	r0, #0
  3b3758: 1afffff7     	bne	0x3b373c <Character::SafeGetCharPropsTemplateId()+0x50> @ imm = #-0x24
  3b375c: e6ff4074     	uxth	r4, r4
  3b3760: e30133ca     	movw	r3, #0x13ca
  3b3764: e6bf0074     	sxth	r0, r4
  3b3768: e18840b3     	strh	r4, [r8, r3]
  3b376c: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  3b3770: e30f4fff     	movw	r4, #0xffff
  3b3774: e30133ca     	movw	r3, #0x13ca
  3b3778: e3e00000     	mvn	r0, #0
  3b377c: e18840b3     	strh	r4, [r8, r3]
  3b3780: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  3b3784: e30133ca     	movw	r3, #0x13ca
  3b3788: e19000f3     	ldrsh	r0, [r0, r3]
  3b378c: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  3b3790: 84 13 5e 00  	.word	0x005e1384
  3b3794: c8 0b 00 00  	.word	0x00000bc8
  3b3798: 3c 17 00 00  	.word	0x0000173c
