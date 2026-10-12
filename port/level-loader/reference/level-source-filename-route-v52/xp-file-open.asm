
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0082b214 <XP_API_FILE_OPEN>:
  82b214: e92d45f0     	push	{r4, r5, r6, r7, r8, r10, lr}
  82b218: e59f40dc     	ldr	r4, [pc, #0xdc]         @ 0x82b2fc <XP_API_FILE_OPEN+0xe8>
  82b21c: e59f60dc     	ldr	r6, [pc, #0xdc]         @ 0x82b300 <XP_API_FILE_OPEN+0xec>
  82b220: e59f20dc     	ldr	r2, [pc, #0xdc]         @ 0x82b304 <XP_API_FILE_OPEN+0xf0>
  82b224: e08f4004     	add	r4, pc, r4
  82b228: e7943006     	ldr	r3, [r4, r6]
  82b22c: e7942002     	ldr	r2, [r4, r2]
  82b230: e1a07001     	mov	r7, r1
  82b234: e59f10cc     	ldr	r1, [pc, #0xcc]         @ 0x82b308 <XP_API_FILE_OPEN+0xf4>
  82b238: e24ddf67     	sub	sp, sp, #412
  82b23c: e593c000     	ldr	r12, [r3]
  82b240: e28d5004     	add	r5, sp, #4
  82b244: e1a03000     	mov	r3, r0
  82b248: e5922000     	ldr	r2, [r2]
  82b24c: e08f1001     	add	r1, pc, r1
  82b250: e1a00005     	mov	r0, r5
  82b254: e58dc194     	str	r12, [sp, #0x194]
  82b258: ebeb8e21     	bl	0x30eae4 <.plt+0xd70>   @ imm = #-0x51c77c
  82b25c: e59f10a8     	ldr	r1, [pc, #0xa8]         @ 0x82b30c <XP_API_FILE_OPEN+0xf8>
  82b260: e3a00004     	mov	r0, #4
  82b264: e1a02005     	mov	r2, r5
  82b268: e08f1001     	add	r1, pc, r1
  82b26c: ebeb8d02     	bl	0x30e67c <.plt+0x908>   @ imm = #-0x51cbf8
  82b270: e59f3098     	ldr	r3, [pc, #0x98]         @ 0x82b310 <XP_API_FILE_OPEN+0xfc>
  82b274: e3a08000     	mov	r8, #0
  82b278: e794a003     	ldr	r10, [r4, r3]
  82b27c: e79a3108     	ldr	r3, [r10, r8, lsl #2]
  82b280: e3530000     	cmp	r3, #0
  82b284: 0a000004     	beq	0x82b29c <XP_API_FILE_OPEN+0x88> @ imm = #0x10
  82b288: e2888001     	add	r8, r8, #1
  82b28c: e3580008     	cmp	r8, #8
  82b290: 1afffff9     	bne	0x82b27c <XP_API_FILE_OPEN+0x68> @ imm = #-0x1c
  82b294: e3a00000     	mov	r0, #0
  82b298: ea000008     	b	0x82b2c0 <XP_API_FILE_OPEN+0xac> @ imm = #0x20
  82b29c: e3580008     	cmp	r8, #8
  82b2a0: 0afffffb     	beq	0x82b294 <XP_API_FILE_OPEN+0x80> @ imm = #-0x14
  82b2a4: e1a01007     	mov	r1, r7
  82b2a8: e1a00005     	mov	r0, r5
  82b2ac: ebeb8c95     	bl	0x30e508 <.plt+0x794>   @ imm = #-0x51cdac
  82b2b0: e2507000     	subs	r7, r0, #0
  82b2b4: 178a7108     	strne	r7, [r10, r8, lsl #2]
  82b2b8: 12880001     	addne	r0, r8, #1
  82b2bc: 0a000006     	beq	0x82b2dc <XP_API_FILE_OPEN+0xc8> @ imm = #0x18
  82b2c0: e7943006     	ldr	r3, [r4, r6]
  82b2c4: e59d2194     	ldr	r2, [sp, #0x194]
  82b2c8: e5933000     	ldr	r3, [r3]
  82b2cc: e1520003     	cmp	r2, r3
  82b2d0: 1a000008     	bne	0x82b2f8 <XP_API_FILE_OPEN+0xe4> @ imm = #0x20
  82b2d4: e28ddf67     	add	sp, sp, #412
  82b2d8: e8bd85f0     	pop	{r4, r5, r6, r7, r8, r10, pc}
  82b2dc: e59f1030     	ldr	r1, [pc, #0x30]         @ 0x82b314 <XP_API_FILE_OPEN+0x100>
  82b2e0: e1a02005     	mov	r2, r5
  82b2e4: e3a00004     	mov	r0, #4
  82b2e8: e08f1001     	add	r1, pc, r1
  82b2ec: ebeb8ce2     	bl	0x30e67c <.plt+0x908>   @ imm = #-0x51cc78
  82b2f0: e1a00007     	mov	r0, r7
  82b2f4: eafffff1     	b	0x82b2c0 <XP_API_FILE_OPEN+0xac> @ imm = #-0x3c
  82b2f8: ebeb8c04     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0x51cff0
  82b2fc: 6c 98 16 00  	.word	0x0016986c
  82b300: ac 40 00 00  	.word	0x000040ac
  82b304: 00 06 00 00  	.word	0x00000600
  82b308: bc 58 09 00  	.word	0x000958bc
  82b30c: b8 11 0e 00  	.word	0x000e11b8
  82b310: d4 13 00 00  	.word	0x000013d4
  82b314: 50 11 0e 00  	.word	0x000e1150

0082b318 <XP_API_ATOF>:
  82b318: e3a01000     	mov	r1, #0
  82b31c: eaeb8ce2     	b	0x30e6ac <.plt+0x938>   @ imm = #-0x51cc78

0082b320 <XP_API_ATOI>:
  82b320: eaeb8b5b     	b	0x30e094 <.plt+0x320>   @ imm = #-0x51d294

0082b324 <XP_API_STRCHR>:
  82b324: e3520001     	cmp	r2, #1
  82b328: 0a000000     	beq	0x82b330 <XP_API_STRCHR+0xc> @ imm = #0x0
  82b32c: eaeb8c3c     	b	0x30e424 <.plt+0x6b0>   @ imm = #-0x51cf10
  82b330: eaeb8e3c     	b	0x30ec28 <.plt+0xeb4>   @ imm = #-0x51c710

0082b334 <XP_API_STRSTR>:
  82b334: eaeb8e26     	b	0x30ebd4 <.plt+0xe60>   @ imm = #-0x51c768

0082b338 <XP_API_STRCAT>:
  82b338: eaeb8e94     	b	0x30ed90 <.plt+0x101c>  @ imm = #-0x51c5b0

0082b33c <XP_API_STRNCPY>:
  82b33c: eaeb8ab8     	b	0x30de24 <.plt+0xb0>    @ imm = #-0x51d520
