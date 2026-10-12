
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

004559f8 <ScriptCmdImpl::~ScriptCmdImpl()>:
  4559f8: e92d4010     	push	{r4, lr}
  4559fc: e59f305c     	ldr	r3, [pc, #0x5c]         @ 0x455a60 <ScriptCmdImpl::~ScriptCmdImpl()+0x68>
  455a00: e59f205c     	ldr	r2, [pc, #0x5c]         @ 0x455a64 <ScriptCmdImpl::~ScriptCmdImpl()+0x6c>
  455a04: e590100c     	ldr	r1, [r0, #0xc]
  455a08: e08f3003     	add	r3, pc, r3
  455a0c: e7932002     	ldr	r2, [r3, r2]
  455a10: e3510000     	cmp	r1, #0
  455a14: e1a04000     	mov	r4, r0
  455a18: e2822008     	add	r2, r2, #8
  455a1c: e5802000     	str	r2, [r0]
  455a20: 0a00000c     	beq	0x455a58 <ScriptCmdImpl::~ScriptCmdImpl()+0x60> @ imm = #0x30
  455a24: e5913000     	ldr	r3, [r1]
  455a28: e1a00001     	mov	r0, r1
  455a2c: e1a0e00f     	mov	lr, pc
  455a30: e593f008     	ldr	pc, [r3, #0x8]
  455a34: e594300c     	ldr	r3, [r4, #0xc]
  455a38: e3530000     	cmp	r3, #0
  455a3c: 0a000003     	beq	0x455a50 <ScriptCmdImpl::~ScriptCmdImpl()+0x58> @ imm = #0xc
  455a40: e1a00003     	mov	r0, r3
  455a44: e5933000     	ldr	r3, [r3]
  455a48: e1a0e00f     	mov	lr, pc
  455a4c: e593f004     	ldr	pc, [r3, #0x4]
  455a50: e3a03000     	mov	r3, #0
  455a54: e584300c     	str	r3, [r4, #0xc]
  455a58: e1a00004     	mov	r0, r4
  455a5c: e8bd8010     	pop	{r4, pc}
  455a60: 88 f0 53 00  	.word	0x0053f088
  455a64: 94 3b 00 00  	.word	0x00003b94

00455a68 <ScriptCmdImpl::~ScriptCmdImpl()>:
  455a68: e92d4010     	push	{r4, lr}
  455a6c: e59f305c     	ldr	r3, [pc, #0x5c]         @ 0x455ad0 <ScriptCmdImpl::~ScriptCmdImpl()+0x68>
  455a70: e59f205c     	ldr	r2, [pc, #0x5c]         @ 0x455ad4 <ScriptCmdImpl::~ScriptCmdImpl()+0x6c>
  455a74: e590100c     	ldr	r1, [r0, #0xc]
  455a78: e08f3003     	add	r3, pc, r3
  455a7c: e7932002     	ldr	r2, [r3, r2]
  455a80: e3510000     	cmp	r1, #0
  455a84: e1a04000     	mov	r4, r0
  455a88: e2822008     	add	r2, r2, #8
  455a8c: e5802000     	str	r2, [r0]
  455a90: 0a00000c     	beq	0x455ac8 <ScriptCmdImpl::~ScriptCmdImpl()+0x60> @ imm = #0x30
  455a94: e5913000     	ldr	r3, [r1]
  455a98: e1a00001     	mov	r0, r1
  455a9c: e1a0e00f     	mov	lr, pc
  455aa0: e593f008     	ldr	pc, [r3, #0x8]
  455aa4: e594300c     	ldr	r3, [r4, #0xc]
  455aa8: e3530000     	cmp	r3, #0
  455aac: 0a000003     	beq	0x455ac0 <ScriptCmdImpl::~ScriptCmdImpl()+0x58> @ imm = #0xc
  455ab0: e1a00003     	mov	r0, r3
  455ab4: e5933000     	ldr	r3, [r3]
  455ab8: e1a0e00f     	mov	lr, pc
  455abc: e593f004     	ldr	pc, [r3, #0x4]
  455ac0: e3a03000     	mov	r3, #0
  455ac4: e584300c     	str	r3, [r4, #0xc]
  455ac8: e1a00004     	mov	r0, r4
  455acc: e8bd8010     	pop	{r4, pc}
  455ad0: 18 f0 53 00  	.word	0x0053f018
  455ad4: 94 3b 00 00  	.word	0x00003b94
