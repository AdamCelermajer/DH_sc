
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

004acb88 <int Arrays::GetMemberIDByString<Arrays::v2Conditions>(char const*)>:
  4acb88: e59f3060     	ldr	r3, [pc, #0x60]         @ 0x4acbf0 <int Arrays::GetMemberIDByString<Arrays::v2Conditions>(char const*)+0x68>
  4acb8c: e59f2060     	ldr	r2, [pc, #0x60]         @ 0x4acbf4 <int Arrays::GetMemberIDByString<Arrays::v2Conditions>(char const*)+0x6c>
  4acb90: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  4acb94: e08f3003     	add	r3, pc, r3
  4acb98: e7932002     	ldr	r2, [r3, r2]
  4acb9c: e1a06000     	mov	r6, r0
  4acba0: e5925000     	ldr	r5, [r2]
  4acba4: e3550000     	cmp	r5, #0
  4acba8: 0a00000e     	beq	0x4acbe8 <int Arrays::GetMemberIDByString<Arrays::v2Conditions>(char const*)+0x60> @ imm = #0x38
  4acbac: e59f2044     	ldr	r2, [pc, #0x44]         @ 0x4acbf8 <int Arrays::GetMemberIDByString<Arrays::v2Conditions>(char const*)+0x70>
  4acbb0: e3a04000     	mov	r4, #0
  4acbb4: e7933002     	ldr	r3, [r3, r2]
  4acbb8: e5937000     	ldr	r7, [r3]
  4acbbc: ea000002     	b	0x4acbcc <int Arrays::GetMemberIDByString<Arrays::v2Conditions>(char const*)+0x44> @ imm = #0x8
  4acbc0: e2844001     	add	r4, r4, #1
  4acbc4: e1540005     	cmp	r4, r5
  4acbc8: 0a000006     	beq	0x4acbe8 <int Arrays::GetMemberIDByString<Arrays::v2Conditions>(char const*)+0x60> @ imm = #0x18
  4acbcc: e7971104     	ldr	r1, [r7, r4, lsl #2]
  4acbd0: e1a00006     	mov	r0, r6
  4acbd4: ebf985d0     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0x19e8c0
  4acbd8: e3500000     	cmp	r0, #0
  4acbdc: 1afffff7     	bne	0x4acbc0 <int Arrays::GetMemberIDByString<Arrays::v2Conditions>(char const*)+0x38> @ imm = #-0x24
  4acbe0: e1a00004     	mov	r0, r4
  4acbe4: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  4acbe8: e3e00000     	mvn	r0, #0
  4acbec: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  4acbf0: fc 7e 4e 00  	.word	0x004e7efc
  4acbf4: 64 2f 00 00  	.word	0x00002f64
  4acbf8: 54 35 00 00  	.word	0x00003554
