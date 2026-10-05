
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00318e5c <UserProperties::_ParseKeyValue(char*, char*)>:
  318e5c: e92d47f0     	push	{r4, r5, r6, r7, r8, r9, r10, lr}
  318e60: e5d13000     	ldrb	r3, [r1]
  318e64: e59fc168     	ldr	r12, [pc, #0x168]       @ 0x318fd4 <UserProperties::_ParseKeyValue(char*, char*)+0x178>
  318e68: e1a07000     	mov	r7, r0
  318e6c: e3530000     	cmp	r3, #0
  318e70: e08fc00c     	add	r12, pc, r12
  318e74: e1a0a002     	mov	r10, r2
  318e78: 0a000041     	beq	0x318f84 <UserProperties::_ParseKeyValue(char*, char*)+0x128> @ imm = #0x104
  318e7c: e59f2154     	ldr	r2, [pc, #0x154]        @ 0x318fd8 <UserProperties::_ParseKeyValue(char*, char*)+0x17c>
  318e80: e1a08001     	mov	r8, r1
  318e84: e79c2002     	ldr	r2, [r12, r2]
  318e88: e5920000     	ldr	r0, [r2]
  318e8c: e6af2073     	sxtb	r2, r3
  318e90: e3720001     	cmn	r2, #1
  318e94: e6e01072     	uxtab	r1, r0, r2
  318e98: 0a00003a     	beq	0x318f88 <UserProperties::_ParseKeyValue(char*, char*)+0x12c> @ imm = #0xe8
  318e9c: e5d12001     	ldrb	r2, [r1, #0x1]
  318ea0: e3120007     	tst	r2, #7
  318ea4: 0a000037     	beq	0x318f88 <UserProperties::_ParseKeyValue(char*, char*)+0x12c> @ imm = #0xdc
  318ea8: e3530000     	cmp	r3, #0
  318eac: 0a000034     	beq	0x318f84 <UserProperties::_ParseKeyValue(char*, char*)+0x128> @ imm = #0xd0
  318eb0: e5d84001     	ldrb	r4, [r8, #0x1]
  318eb4: e2885001     	add	r5, r8, #1
  318eb8: e3540000     	cmp	r4, #0
  318ebc: 0a000013     	beq	0x318f10 <UserProperties::_ParseKeyValue(char*, char*)+0xb4> @ imm = #0x4c
  318ec0: e6af3074     	sxtb	r3, r4
  318ec4: e3730001     	cmn	r3, #1
  318ec8: 0a000010     	beq	0x318f10 <UserProperties::_ParseKeyValue(char*, char*)+0xb4> @ imm = #0x40
  318ecc: e6e03073     	uxtab	r3, r0, r3
  318ed0: e5d33001     	ldrb	r3, [r3, #0x1]
  318ed4: e3130007     	tst	r3, #7
  318ed8: 11a05008     	movne	r5, r8
  318edc: 0a00000b     	beq	0x318f10 <UserProperties::_ParseKeyValue(char*, char*)+0xb4> @ imm = #0x2c
  318ee0: e5d54002     	ldrb	r4, [r5, #0x2]
  318ee4: e3540000     	cmp	r4, #0
  318ee8: e6af3074     	sxtb	r3, r4
  318eec: 0a000029     	beq	0x318f98 <UserProperties::_ParseKeyValue(char*, char*)+0x13c> @ imm = #0xa4
  318ef0: e3730001     	cmn	r3, #1
  318ef4: e6e02073     	uxtab	r2, r0, r3
  318ef8: 0a000026     	beq	0x318f98 <UserProperties::_ParseKeyValue(char*, char*)+0x13c> @ imm = #0x98
  318efc: e5d23001     	ldrb	r3, [r2, #0x1]
  318f00: e2855001     	add	r5, r5, #1
  318f04: e3130007     	tst	r3, #7
  318f08: 1afffff4     	bne	0x318ee0 <UserProperties::_ParseKeyValue(char*, char*)+0x84> @ imm = #-0x30
  318f0c: e2855001     	add	r5, r5, #1
  318f10: e3a03000     	mov	r3, #0
  318f14: e35a0000     	cmp	r10, #0
  318f18: e5c53000     	strb	r3, [r5]
  318f1c: 0a000025     	beq	0x318fb8 <UserProperties::_ParseKeyValue(char*, char*)+0x15c> @ imm = #0x94
  318f20: e59f60b4     	ldr	r6, [pc, #0xb4]         @ 0x318fdc <UserProperties::_ParseKeyValue(char*, char*)+0x180>
  318f24: e1a0000a     	mov	r0, r10
  318f28: e08f6006     	add	r6, pc, r6
  318f2c: e1a01006     	mov	r1, r6
  318f30: ebffd727     	bl	0x30ebd4 <.plt+0xe60>   @ imm = #-0xa364
  318f34: e3500000     	cmp	r0, #0
  318f38: 0a000018     	beq	0x318fa0 <UserProperties::_ParseKeyValue(char*, char*)+0x144> @ imm = #0x60
  318f3c: e2809003     	add	r9, r0, #3
  318f40: e1a01006     	mov	r1, r6
  318f44: e1a00009     	mov	r0, r9
  318f48: ebffd721     	bl	0x30ebd4 <.plt+0xe60>   @ imm = #-0xa37c
  318f4c: e3590000     	cmp	r9, #0
  318f50: 13500000     	cmpne	r0, #0
  318f54: e1a06000     	mov	r6, r0
  318f58: 13a03000     	movne	r3, #0
  318f5c: 03a03001     	moveq	r3, #1
  318f60: 0a00000e     	beq	0x318fa0 <UserProperties::_ParseKeyValue(char*, char*)+0x144> @ imm = #0x38
  318f64: e5c03000     	strb	r3, [r0]
  318f68: e1a01008     	mov	r1, r8
  318f6c: e1a00007     	mov	r0, r7
  318f70: e1a02009     	mov	r2, r9
  318f74: ebffffa7     	bl	0x318e18 <UserProperties::AddProperty(char const*, char const*)> @ imm = #-0x164
  318f78: e3a03025     	mov	r3, #37
  318f7c: e5c63000     	strb	r3, [r6]
  318f80: e5c54000     	strb	r4, [r5]
  318f84: e8bd87f0     	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
  318f88: e5f83001     	ldrb	r3, [r8, #0x1]!
  318f8c: e3530000     	cmp	r3, #0
  318f90: 1affffbd     	bne	0x318e8c <UserProperties::_ParseKeyValue(char*, char*)+0x30> @ imm = #-0x10c
  318f94: e8bd87f0     	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
  318f98: e2855002     	add	r5, r5, #2
  318f9c: eaffffdb     	b	0x318f10 <UserProperties::_ParseKeyValue(char*, char*)+0xb4> @ imm = #-0x94
  318fa0: e1a00007     	mov	r0, r7
  318fa4: e1a01008     	mov	r1, r8
  318fa8: e1a0200a     	mov	r2, r10
  318fac: ebffff99     	bl	0x318e18 <UserProperties::AddProperty(char const*, char const*)> @ imm = #-0x19c
  318fb0: e5c54000     	strb	r4, [r5]
  318fb4: eafffff2     	b	0x318f84 <UserProperties::_ParseKeyValue(char*, char*)+0x128> @ imm = #-0x38
  318fb8: e59f2020     	ldr	r2, [pc, #0x20]         @ 0x318fe0 <UserProperties::_ParseKeyValue(char*, char*)+0x184>
  318fbc: e1a00007     	mov	r0, r7
  318fc0: e1a01008     	mov	r1, r8
  318fc4: e08f2002     	add	r2, pc, r2
  318fc8: ebffff92     	bl	0x318e18 <UserProperties::AddProperty(char const*, char const*)> @ imm = #-0x1b8
  318fcc: e5c54000     	strb	r4, [r5]
  318fd0: eaffffeb     	b	0x318f84 <UserProperties::_ParseKeyValue(char*, char*)+0x128> @ imm = #-0x54
  318fd4: 20 bc 67 00  	.word	0x0067bc20
  318fd8: dc 1d 00 00  	.word	0x00001ddc
  318fdc: 48 58 5a 00  	.word	0x005a5848
  318fe0: 44 28 5b 00  	.word	0x005b2844
