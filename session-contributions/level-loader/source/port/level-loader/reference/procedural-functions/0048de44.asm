
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048de44 <rnd::Rule::NewRule(TiXmlNode*)>:
  48de44: e92d4070     	push	{r4, r5, r6, lr}
  48de48: e5916034     	ldr	r6, [r1, #0x34]
  48de4c: e59f1130     	ldr	r1, [pc, #0x130]        @ 0x48df84 <rnd::Rule::NewRule(TiXmlNode*)+0x140>
  48de50: e24dd008     	sub	sp, sp, #8
  48de54: e1a05000     	mov	r5, r0
  48de58: e08f1001     	add	r1, pc, r1
  48de5c: e1a00006     	mov	r0, r6
  48de60: ebfa0220     	bl	0x30e6e8 <.plt+0x974>   @ imm = #-0x17f780
  48de64: e59f311c     	ldr	r3, [pc, #0x11c]        @ 0x48df88 <rnd::Rule::NewRule(TiXmlNode*)+0x144>
  48de68: e2504000     	subs	r4, r0, #0
  48de6c: e08f3003     	add	r3, pc, r3
  48de70: 1a00000b     	bne	0x48dea4 <rnd::Rule::NewRule(TiXmlNode*)+0x60> @ imm = #0x2c
  48de74: e59f2110     	ldr	r2, [pc, #0x110]        @ 0x48df8c <rnd::Rule::NewRule(TiXmlNode*)+0x148>
  48de78: e7932002     	ldr	r2, [r3, r2]
  48de7c: e5922000     	ldr	r2, [r2]
  48de80: e3520002     	cmp	r2, #2
  48de84: 05844000     	streq	r4, [r4]
  48de88: 01a00004     	moveq	r0, r4
  48de8c: 0a000002     	beq	0x48de9c <rnd::Rule::NewRule(TiXmlNode*)+0x58> @ imm = #0x8
  48de90: e3520001     	cmp	r2, #1
  48de94: 0a00002c     	beq	0x48df4c <rnd::Rule::NewRule(TiXmlNode*)+0x108> @ imm = #0xb0
  48de98: e3a00000     	mov	r0, #0
  48de9c: e28dd008     	add	sp, sp, #8
  48dea0: e8bd8070     	pop	{r4, r5, r6, pc}
  48dea4: e59f10e4     	ldr	r1, [pc, #0xe4]         @ 0x48df90 <rnd::Rule::NewRule(TiXmlNode*)+0x14c>
  48dea8: e1a00006     	mov	r0, r6
  48deac: e08f1001     	add	r1, pc, r1
  48deb0: ebfa020c     	bl	0x30e6e8 <.plt+0x974>   @ imm = #-0x17f7d0
  48deb4: e2501000     	subs	r1, r0, #0
  48deb8: 0a00000d     	beq	0x48def4 <rnd::Rule::NewRule(TiXmlNode*)+0xb0> @ imm = #0x34
  48debc: e59f10d0     	ldr	r1, [pc, #0xd0]         @ 0x48df94 <rnd::Rule::NewRule(TiXmlNode*)+0x150>
  48dec0: e1a00006     	mov	r0, r6
  48dec4: e08f1001     	add	r1, pc, r1
  48dec8: ebfa0206     	bl	0x30e6e8 <.plt+0x974>   @ imm = #-0x17f7e8
  48decc: e2501000     	subs	r1, r0, #0
  48ded0: 1a00000f     	bne	0x48df14 <rnd::Rule::NewRule(TiXmlNode*)+0xd0> @ imm = #0x3c
  48ded4: e3a00090     	mov	r0, #144
  48ded8: ebfa09a4     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x17d970
  48dedc: e1a02005     	mov	r2, r5
  48dee0: e1a04000     	mov	r4, r0
  48dee4: e5951004     	ldr	r1, [r5, #0x4]
  48dee8: ebffffa4     	bl	0x48dd80 <rnd::ForceBlock::ForceBlock(rnd::RootRule&, rnd::Rule*)> @ imm = #-0x170
  48deec: e1a00004     	mov	r0, r4
  48def0: eaffffe9     	b	0x48de9c <rnd::Rule::NewRule(TiXmlNode*)+0x58> @ imm = #-0x5c
  48def4: e3a00090     	mov	r0, #144
  48def8: ebfa099c     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x17d990
  48defc: e1a02005     	mov	r2, r5
  48df00: e1a04000     	mov	r4, r0
  48df04: e5951004     	ldr	r1, [r5, #0x4]
  48df08: ebffffba     	bl	0x48ddf8 <rnd::Path::Path(rnd::RootRule&, rnd::Rule*)> @ imm = #-0x118
  48df0c: e1a00004     	mov	r0, r4
  48df10: eaffffe1     	b	0x48de9c <rnd::Rule::NewRule(TiXmlNode*)+0x58> @ imm = #-0x7c
  48df14: e59f107c     	ldr	r1, [pc, #0x7c]         @ 0x48df98 <rnd::Rule::NewRule(TiXmlNode*)+0x154>
  48df18: e1a00006     	mov	r0, r6
  48df1c: e08f1001     	add	r1, pc, r1
  48df20: ebfa01f0     	bl	0x30e6e8 <.plt+0x974>   @ imm = #-0x17f840
  48df24: e2501000     	subs	r1, r0, #0
  48df28: 1affffda     	bne	0x48de98 <rnd::Rule::NewRule(TiXmlNode*)+0x54> @ imm = #-0x98
  48df2c: e3a00088     	mov	r0, #136
  48df30: ebfa098e     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x17d9c8
  48df34: e1a02005     	mov	r2, r5
  48df38: e1a04000     	mov	r4, r0
  48df3c: e5951004     	ldr	r1, [r5, #0x4]
  48df40: ebffff74     	bl	0x48dd18 <rnd::EndPath::EndPath(rnd::RootRule&, rnd::Rule*)> @ imm = #-0x230
  48df44: e1a00004     	mov	r0, r4
  48df48: eaffffd3     	b	0x48de9c <rnd::Rule::NewRule(TiXmlNode*)+0x58> @ imm = #-0xb4
  48df4c: e59f0048     	ldr	r0, [pc, #0x48]         @ 0x48df9c <rnd::Rule::NewRule(TiXmlNode*)+0x158>
  48df50: e59f1048     	ldr	r1, [pc, #0x48]         @ 0x48dfa0 <rnd::Rule::NewRule(TiXmlNode*)+0x15c>
  48df54: e59f2048     	ldr	r2, [pc, #0x48]         @ 0x48dfa4 <rnd::Rule::NewRule(TiXmlNode*)+0x160>
  48df58: e7930000     	ldr	r0, [r3, r0]
  48df5c: e59f3044     	ldr	r3, [pc, #0x44]         @ 0x48dfa8 <rnd::Rule::NewRule(TiXmlNode*)+0x164>
  48df60: e300c189     	movw	r12, #0x189
  48df64: e08f1001     	add	r1, pc, r1
  48df68: e28000a8     	add	r0, r0, #168
  48df6c: e08f2002     	add	r2, pc, r2
  48df70: e08f3003     	add	r3, pc, r3
  48df74: e58dc000     	str	r12, [sp]
  48df78: ebfa0021     	bl	0x30e004 <.plt+0x290>   @ imm = #-0x17ff7c
  48df7c: e1a00004     	mov	r0, r4
  48df80: eaffffc5     	b	0x48de9c <rnd::Rule::NewRule(TiXmlNode*)+0x58> @ imm = #-0xec
  48df84: c0 6f 44 00  	.word	0x00446fc0
  48df88: 24 6c 50 00  	.word	0x00506c24
  48df8c: c0 39 00 00  	.word	0x000039c0
  48df90: bc 6f 44 00  	.word	0x00446fbc
  48df94: ac 6f 44 00  	.word	0x00446fac
  48df98: 64 6f 44 00  	.word	0x00446f64
  48df9c: c0 19 00 00  	.word	0x000019c0
  48dfa0: 74 04 43 00  	.word	0x00430474
  48dfa4: fc 05 43 00  	.word	0x004305fc
  48dfa8: b8 6e 44 00  	.word	0x00446eb8
