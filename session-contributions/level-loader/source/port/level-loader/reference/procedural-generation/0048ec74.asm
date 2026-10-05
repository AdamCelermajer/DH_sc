
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048ec74 <_ZN3rnd8ListRule7ReplaceERNS_8ListElemE>:
  48ec74: e92d4030     	push	{r4, r5, lr}
  48ec78: e5d02018     	ldrb	r2, [r0, #0x18]
  48ec7c: e59f3090     	ldr	r3, [pc, #0x90]         @ 0x48ed14 <_ZN3rnd8ListRule7ReplaceERNS_8ListElemE+0xa0>
  48ec80: e24dd00c     	sub	sp, sp, #12
  48ec84: e3520000     	cmp	r2, #0
  48ec88: e1a04000     	mov	r4, r0
  48ec8c: e1a05001     	mov	r5, r1
  48ec90: e08f3003     	add	r3, pc, r3
  48ec94: 1a00000f     	bne	0x48ecd8 <_ZN3rnd8ListRule7ReplaceERNS_8ListElemE+0x64> @ imm = #0x3c
  48ec98: e5911000     	ldr	r1, [r1]
  48ec9c: e1510000     	cmp	r1, r0
  48eca0: 0a000007     	beq	0x48ecc4 <_ZN3rnd8ListRule7ReplaceERNS_8ListElemE+0x50> @ imm = #0x1c
  48eca4: e59f106c     	ldr	r1, [pc, #0x6c]         @ 0x48ed18 <_ZN3rnd8ListRule7ReplaceERNS_8ListElemE+0xa4>
  48eca8: e7931001     	ldr	r1, [r3, r1]
  48ecac: e5911000     	ldr	r1, [r1]
  48ecb0: e3510002     	cmp	r1, #2
  48ecb4: 05822000     	streq	r2, [r2]
  48ecb8: 0a000001     	beq	0x48ecc4 <_ZN3rnd8ListRule7ReplaceERNS_8ListElemE+0x50> @ imm = #0x4
  48ecbc: e3510001     	cmp	r1, #1
  48ecc0: 0a000006     	beq	0x48ece0 <_ZN3rnd8ListRule7ReplaceERNS_8ListElemE+0x6c> @ imm = #0x18
  48ecc4: e284001c     	add	r0, r4, #28
  48ecc8: e1a01005     	mov	r1, r5
  48eccc: e28dd00c     	add	sp, sp, #12
  48ecd0: e8bd4030     	pop	{r4, r5, lr}
  48ecd4: eaffff9d     	b	0x48eb50 <_ZNSt6vectorIN3rnd8ListElemESaIS1_EE9push_backERKS1_> @ imm = #-0x18c
  48ecd8: e28dd00c     	add	sp, sp, #12
  48ecdc: e8bd8030     	pop	{r4, r5, pc}
  48ece0: e59f0034     	ldr	r0, [pc, #0x34]         @ 0x48ed1c <_ZN3rnd8ListRule7ReplaceERNS_8ListElemE+0xa8>
  48ece4: e59f1034     	ldr	r1, [pc, #0x34]         @ 0x48ed20 <_ZN3rnd8ListRule7ReplaceERNS_8ListElemE+0xac>
  48ece8: e59f2034     	ldr	r2, [pc, #0x34]         @ 0x48ed24 <_ZN3rnd8ListRule7ReplaceERNS_8ListElemE+0xb0>
  48ecec: e7930000     	ldr	r0, [r3, r0]
  48ecf0: e59f3030     	ldr	r3, [pc, #0x30]         @ 0x48ed28 <_ZN3rnd8ListRule7ReplaceERNS_8ListElemE+0xb4>
  48ecf4: e3a0c07f     	mov	r12, #127
  48ecf8: e08f1001     	add	r1, pc, r1
  48ecfc: e08f2002     	add	r2, pc, r2
  48ed00: e08f3003     	add	r3, pc, r3
  48ed04: e28000a8     	add	r0, r0, #168
  48ed08: e58dc000     	str	r12, [sp]
  48ed0c: ebf9fcbc     	bl	0x30e004 <.plt+0x290>   @ imm = #-0x180d10
  48ed10: eaffffeb     	b	0x48ecc4 <_ZN3rnd8ListRule7ReplaceERNS_8ListElemE+0x50> @ imm = #-0x54
  48ed14: 00 5e 50 00  	.word	0x00505e00
  48ed18: c0 39 00 00  	.word	0x000039c0
  48ed1c: c0 19 00 00  	.word	0x000019c0
  48ed20: e0 f6 42 00  	.word	0x0042f6e0
  48ed24: 94 61 44 00  	.word	0x00446194
  48ed28: 28 61 44 00  	.word	0x00446128
