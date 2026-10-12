
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

004b15f8 <Arrays::v2Conditions::readNames(IStreamBase*)>:
  4b15f8: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  4b15fc: e1a07000     	mov	r7, r0
  4b1600: e24dd01c     	sub	sp, sp, #28
  4b1604: ebffcc9f     	bl	0x4a4888 <Arrays::v2Conditions::finalizeNames()> @ imm = #-0xcd84
  4b1608: e1a00007     	mov	r0, r7
  4b160c: ebf9891f     	bl	0x313a90 <unsigned int StreamReader::readAs<unsigned int>(IStreamBase*)> @ imm = #-0x19db84
  4b1610: e59f616c     	ldr	r6, [pc, #0x16c]        @ 0x4b1784 <Arrays::v2Conditions::readNames(IStreamBase*)+0x18c>
  4b1614: e3a03001     	mov	r3, #1
  4b1618: e3530000     	cmp	r3, #0
  4b161c: e08f6006     	add	r6, pc, r6
  4b1620: e58d0014     	str	r0, [sp, #0x14]
  4b1624: e58d300c     	str	r3, [sp, #0xc]
  4b1628: 1a000012     	bne	0x4b1678 <Arrays::v2Conditions::readNames(IStreamBase*)+0x80> @ imm = #0x48
  4b162c: e28d3014     	add	r3, sp, #20
  4b1630: e2832002     	add	r2, r3, #2
  4b1634: e2833001     	add	r3, r3, #1
  4b1638: e5d20001     	ldrb	r0, [r2, #0x1]
  4b163c: e5531001     	ldrb	r1, [r3, #-0x1]
  4b1640: e1520003     	cmp	r2, r3
  4b1644: e1a04002     	mov	r4, r2
  4b1648: e0201001     	eor	r1, r0, r1
  4b164c: e5431001     	strb	r1, [r3, #-0x1]
  4b1650: e5d20001     	ldrb	r0, [r2, #0x1]
  4b1654: e0211000     	eor	r1, r1, r0
  4b1658: e5c21001     	strb	r1, [r2, #0x1]
  4b165c: e5530001     	ldrb	r0, [r3, #-0x1]
  4b1660: e2422001     	sub	r2, r2, #1
  4b1664: e0211000     	eor	r1, r1, r0
  4b1668: e5431001     	strb	r1, [r3, #-0x1]
  4b166c: e2833001     	add	r3, r3, #1
  4b1670: 8afffff0     	bhi	0x4b1638 <Arrays::v2Conditions::readNames(IStreamBase*)+0x40> @ imm = #-0x40
  4b1674: e59d0014     	ldr	r0, [sp, #0x14]
  4b1678: e59f3108     	ldr	r3, [pc, #0x108]        @ 0x4b1788 <Arrays::v2Conditions::readNames(IStreamBase*)+0x190>
  4b167c: e7963003     	ldr	r3, [r6, r3]
  4b1680: e5933000     	ldr	r3, [r3]
  4b1684: e1530000     	cmp	r3, r0
  4b1688: 0a000001     	beq	0x4b1694 <Arrays::v2Conditions::readNames(IStreamBase*)+0x9c> @ imm = #0x4
  4b168c: e28dd01c     	add	sp, sp, #28
  4b1690: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  4b1694: e1a00100     	lsl	r0, r0, #2
  4b1698: e3a01001     	mov	r1, #1
  4b169c: ebf97bb2     	bl	0x31056c <operator new[](unsigned int, MemoryHintState)> @ imm = #-0x1a1138
  4b16a0: e59f90e4     	ldr	r9, [pc, #0xe4]         @ 0x4b178c <Arrays::v2Conditions::readNames(IStreamBase*)+0x194>
  4b16a4: e59d2014     	ldr	r2, [sp, #0x14]
  4b16a8: e7963009     	ldr	r3, [r6, r9]
  4b16ac: e3520000     	cmp	r2, #0
  4b16b0: e5830000     	str	r0, [r3]
  4b16b4: 0afffff4     	beq	0x4b168c <Arrays::v2Conditions::readNames(IStreamBase*)+0x94> @ imm = #-0x30
  4b16b8: e28da010     	add	r10, sp, #16
  4b16bc: e3a08001     	mov	r8, #1
  4b16c0: e08a1008     	add	r1, r10, r8
  4b16c4: e28a3002     	add	r3, r10, #2
  4b16c8: e3a04000     	mov	r4, #0
  4b16cc: e88d000a     	stm	sp, {r1, r3}
  4b16d0: e1a00007     	mov	r0, r7
  4b16d4: e1a0100a     	mov	r1, r10
  4b16d8: ebfcb6b0     	bl	0x3df1a0 <void StreamReader::readAs<unsigned int>(IStreamBase*, unsigned int*)> @ imm = #-0xd2540
  4b16dc: e3580000     	cmp	r8, #0
  4b16e0: e58d800c     	str	r8, [sp, #0xc]
  4b16e4: 1a00000f     	bne	0x4b1728 <Arrays::v2Conditions::readNames(IStreamBase*)+0x130> @ imm = #0x3c
  4b16e8: e59d3000     	ldr	r3, [sp]
  4b16ec: e59d2004     	ldr	r2, [sp, #0x4]
  4b16f0: e5d20001     	ldrb	r0, [r2, #0x1]
  4b16f4: e5531001     	ldrb	r1, [r3, #-0x1]
  4b16f8: e1520003     	cmp	r2, r3
  4b16fc: e0201001     	eor	r1, r0, r1
  4b1700: e5431001     	strb	r1, [r3, #-0x1]
  4b1704: e5d20001     	ldrb	r0, [r2, #0x1]
  4b1708: e0211000     	eor	r1, r1, r0
  4b170c: e5c21001     	strb	r1, [r2, #0x1]
  4b1710: e5530001     	ldrb	r0, [r3, #-0x1]
  4b1714: e2422001     	sub	r2, r2, #1
  4b1718: e0211000     	eor	r1, r1, r0
  4b171c: e5431001     	strb	r1, [r3, #-0x1]
  4b1720: e2833001     	add	r3, r3, #1
  4b1724: 8afffff1     	bhi	0x4b16f0 <Arrays::v2Conditions::readNames(IStreamBase*)+0xf8> @ imm = #-0x3c
  4b1728: e59d0010     	ldr	r0, [sp, #0x10]
  4b172c: e7965009     	ldr	r5, [r6, r9]
  4b1730: e3a01001     	mov	r1, #1
  4b1734: e0800001     	add	r0, r0, r1
  4b1738: e595b000     	ldr	r11, [r5]
  4b173c: ebf97b8a     	bl	0x31056c <operator new[](unsigned int, MemoryHintState)> @ imm = #-0x1a11d8
  4b1740: e78b0104     	str	r0, [r11, r4, lsl #2]
  4b1744: e5953000     	ldr	r3, [r5]
  4b1748: e59d2010     	ldr	r2, [sp, #0x10]
  4b174c: e1a00007     	mov	r0, r7
  4b1750: e7931104     	ldr	r1, [r3, r4, lsl #2]
  4b1754: e3a03000     	mov	r3, #0
  4b1758: ebf9973d     	bl	0x317454 <StreamReader::readStringEx(IStreamBase*, char*, unsigned long long)> @ imm = #-0x19a30c
  4b175c: e5953000     	ldr	r3, [r5]
  4b1760: e3a01000     	mov	r1, #0
  4b1764: e7932104     	ldr	r2, [r3, r4, lsl #2]
  4b1768: e59d3010     	ldr	r3, [sp, #0x10]
  4b176c: e2844001     	add	r4, r4, #1
  4b1770: e7c21003     	strb	r1, [r2, r3]
  4b1774: e59d3014     	ldr	r3, [sp, #0x14]
  4b1778: e1530004     	cmp	r3, r4
  4b177c: 8affffd3     	bhi	0x4b16d0 <Arrays::v2Conditions::readNames(IStreamBase*)+0xd8> @ imm = #-0xb4
  4b1780: eaffffc1     	b	0x4b168c <Arrays::v2Conditions::readNames(IStreamBase*)+0x94> @ imm = #-0xfc
  4b1784: 74 34 4e 00  	.word	0x004e3474
  4b1788: 64 2f 00 00  	.word	0x00002f64
  4b178c: 54 35 00 00  	.word	0x00003554
