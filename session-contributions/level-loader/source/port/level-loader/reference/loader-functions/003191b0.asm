
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003191b0 <UserProperties::UserProperties(char const*)>:
  3191b0: e59f2048     	ldr	r2, [pc, #0x48]         @ 0x319200 <UserProperties::UserProperties(char const*)+0x50>
  3191b4: e92d4070     	push	{r4, r5, r6, lr}
  3191b8: e59f5044     	ldr	r5, [pc, #0x44]         @ 0x319204 <UserProperties::UserProperties(char const*)+0x54>
  3191bc: e08f2002     	add	r2, pc, r2
  3191c0: e3a0c000     	mov	r12, #0
  3191c4: e7925005     	ldr	r5, [r2, r5]
  3191c8: e1a03000     	mov	r3, r0
  3191cc: e580c008     	str	r12, [r0, #0x8]
  3191d0: e2855008     	add	r5, r5, #8
  3191d4: e5805000     	str	r5, [r0]
  3191d8: e151000c     	cmp	r1, r12
  3191dc: e5e3c004     	strb	r12, [r3, #0x4]!
  3191e0: e1a04000     	mov	r4, r0
  3191e4: e5803010     	str	r3, [r0, #0x10]
  3191e8: e580c014     	str	r12, [r0, #0x14]
  3191ec: e580300c     	str	r3, [r0, #0xc]
  3191f0: 0a000000     	beq	0x3191f8 <UserProperties::UserProperties(char const*)+0x48> @ imm = #0x0
  3191f4: ebffff90     	bl	0x31903c <UserProperties::_ParseProperties(char const*)> @ imm = #-0x1c0
  3191f8: e1a00004     	mov	r0, r4
  3191fc: e8bd8070     	pop	{r4, r5, r6, pc}
  319200: d4 b8 67 00  	.word	0x0067b8d4
  319204: 10 28 00 00  	.word	0x00002810
