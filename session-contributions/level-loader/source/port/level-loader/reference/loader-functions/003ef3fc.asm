
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003ef3fc <Level::CleanUpAllSkills()>:
  3ef3fc: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  3ef400: e59f504c     	ldr	r5, [pc, #0x4c]         @ 0x3ef454 <Level::CleanUpAllSkills()+0x58>
  3ef404: e59f304c     	ldr	r3, [pc, #0x4c]         @ 0x3ef458 <Level::CleanUpAllSkills()+0x5c>
  3ef408: e24dd008     	sub	sp, sp, #8
  3ef40c: e08f5005     	add	r5, pc, r5
  3ef410: e7953003     	ldr	r3, [r5, r3]
  3ef414: e59f8040     	ldr	r8, [pc, #0x40]         @ 0x3ef45c <Level::CleanUpAllSkills()+0x60>
  3ef418: e28d7004     	add	r7, sp, #4
  3ef41c: e5936038     	ldr	r6, [r3, #0x38]
  3ef420: e5b64060     	ldr	r4, [r6, #0x60]!
  3ef424: ea000006     	b	0x3ef444 <Level::CleanUpAllSkills()+0x48> @ imm = #0x18
  3ef428: e7953008     	ldr	r3, [r5, r8]
  3ef42c: e5940008     	ldr	r0, [r4, #0x8]
  3ef430: e1a01007     	mov	r1, r7
  3ef434: e3a02001     	mov	r2, #1
  3ef438: e58d3004     	str	r3, [sp, #0x4]
  3ef43c: ebfee1b8     	bl	0x3a7b24 <Character::UnLoadScriptProcess(std::priv::_Rb_tree_iterator<std::pair<int const, Character*>, std::priv::_MapTraitsT<std::pair<int const, Character*>>>, bool)> @ imm = #-0x47920
  3ef440: e5944000     	ldr	r4, [r4]
  3ef444: e1560004     	cmp	r6, r4
  3ef448: 1afffff6     	bne	0x3ef428 <Level::CleanUpAllSkills()+0x2c> @ imm = #-0x28
  3ef44c: e28dd008     	add	sp, sp, #8
  3ef450: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  3ef454: 84 56 5a 00  	.word	0x005a5684
  3ef458: f4 37 00 00  	.word	0x000037f4
  3ef45c: 34 11 00 00  	.word	0x00001134
