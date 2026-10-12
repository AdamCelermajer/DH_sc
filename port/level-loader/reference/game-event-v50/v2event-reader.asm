
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

004ebc38 <Structs::v2Event::read(IStreamBase*)>:
  4ebc38: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  4ebc3c: e1a04000     	mov	r4, r0
  4ebc40: e24dd008     	sub	sp, sp, #8
  4ebc44: e1a00001     	mov	r0, r1
  4ebc48: e1a05001     	mov	r5, r1
  4ebc4c: e59f6254     	ldr	r6, [pc, #0x254]        @ 0x4ebea8 <Structs::v2Event::read(IStreamBase*)+0x270>
  4ebc50: e2841004     	add	r1, r4, #4
  4ebc54: ebfdb50d     	bl	0x459090 <void StreamReader::readAs<int>(IStreamBase*, int*)> @ imm = #-0x92bcc
  4ebc58: e3a03001     	mov	r3, #1
  4ebc5c: e3530000     	cmp	r3, #0
  4ebc60: e58d3004     	str	r3, [sp, #0x4]
  4ebc64: e08f6006     	add	r6, pc, r6
  4ebc68: 1a00000f     	bne	0x4ebcac <Structs::v2Event::read(IStreamBase*)+0x74> @ imm = #0x3c
  4ebc6c: e2843005     	add	r3, r4, #5
  4ebc70: e2842006     	add	r2, r4, #6
  4ebc74: e5d20001     	ldrb	r0, [r2, #0x1]
  4ebc78: e5531001     	ldrb	r1, [r3, #-0x1]
  4ebc7c: e1520003     	cmp	r2, r3
  4ebc80: e0201001     	eor	r1, r0, r1
  4ebc84: e5431001     	strb	r1, [r3, #-0x1]
  4ebc88: e5d20001     	ldrb	r0, [r2, #0x1]
  4ebc8c: e0211000     	eor	r1, r1, r0
  4ebc90: e5c21001     	strb	r1, [r2, #0x1]
  4ebc94: e5530001     	ldrb	r0, [r3, #-0x1]
  4ebc98: e2422001     	sub	r2, r2, #1
  4ebc9c: e0211000     	eor	r1, r1, r0
  4ebca0: e5431001     	strb	r1, [r3, #-0x1]
  4ebca4: e2833001     	add	r3, r3, #1
  4ebca8: 8afffff1     	bhi	0x4ebc74 <Structs::v2Event::read(IStreamBase*)+0x3c> @ imm = #-0x3c
  4ebcac: e1a00005     	mov	r0, r5
  4ebcb0: e2841008     	add	r1, r4, #8
  4ebcb4: ebfbcd39     	bl	0x3df1a0 <void StreamReader::readAs<unsigned int>(IStreamBase*, unsigned int*)> @ imm = #-0x10cb1c
  4ebcb8: e3a03001     	mov	r3, #1
  4ebcbc: e3530000     	cmp	r3, #0
  4ebcc0: e58d3004     	str	r3, [sp, #0x4]
  4ebcc4: 1a00000f     	bne	0x4ebd08 <Structs::v2Event::read(IStreamBase*)+0xd0> @ imm = #0x3c
  4ebcc8: e2843009     	add	r3, r4, #9
  4ebccc: e284200a     	add	r2, r4, #10
  4ebcd0: e5d20001     	ldrb	r0, [r2, #0x1]
  4ebcd4: e5531001     	ldrb	r1, [r3, #-0x1]
  4ebcd8: e1520003     	cmp	r2, r3
  4ebcdc: e0201001     	eor	r1, r0, r1
  4ebce0: e5431001     	strb	r1, [r3, #-0x1]
  4ebce4: e5d20001     	ldrb	r0, [r2, #0x1]
  4ebce8: e0211000     	eor	r1, r1, r0
  4ebcec: e5c21001     	strb	r1, [r2, #0x1]
  4ebcf0: e5530001     	ldrb	r0, [r3, #-0x1]
  4ebcf4: e2422001     	sub	r2, r2, #1
  4ebcf8: e0211000     	eor	r1, r1, r0
  4ebcfc: e5431001     	strb	r1, [r3, #-0x1]
  4ebd00: e2833001     	add	r3, r3, #1
  4ebd04: 8afffff1     	bhi	0x4ebcd0 <Structs::v2Event::read(IStreamBase*)+0x98> @ imm = #-0x3c
  4ebd08: e594000c     	ldr	r0, [r4, #0xc]
  4ebd0c: e3500000     	cmp	r0, #0
  4ebd10: 0a000000     	beq	0x4ebd18 <Structs::v2Event::read(IStreamBase*)+0xe0> @ imm = #0x0
  4ebd14: ebf891c9     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x1db8dc
  4ebd18: e5940008     	ldr	r0, [r4, #0x8]
  4ebd1c: e3a01001     	mov	r1, #1
  4ebd20: e3a07000     	mov	r7, #0
  4ebd24: e0800001     	add	r0, r0, r1
  4ebd28: ebf8920f     	bl	0x31056c <operator new[](unsigned int, MemoryHintState)> @ imm = #-0x1db7c4
  4ebd2c: e5942008     	ldr	r2, [r4, #0x8]
  4ebd30: e1a01000     	mov	r1, r0
  4ebd34: e584000c     	str	r0, [r4, #0xc]
  4ebd38: e1a03007     	mov	r3, r7
  4ebd3c: e1a00005     	mov	r0, r5
