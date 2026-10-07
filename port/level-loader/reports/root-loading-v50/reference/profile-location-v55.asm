
.local-inputs/libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00468b78 <PlayerSavegame::__LoadLevelName(IStreamBase*, void*)>:
  468b78: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  468b7c: e1a07001     	mov	r7, r1
  468b80: e24dd008     	sub	sp, sp, #8
  468b84: e2811038     	add	r1, r1, #56
  468b88: e1a06000     	mov	r6, r0
  468b8c: ebfaabed     	bl	0x313b48 <void IStreamBase::readAs<unsigned int>(unsigned int&)> @ imm = #-0x15504c
  468b90: e1a05007     	mov	r5, r7
  468b94: e3a04000     	mov	r4, #0
  468b98: e28d8004     	add	r8, sp, #4
  468b9c: e2841014     	add	r1, r4, #20
  468ba0: e0871101     	add	r1, r7, r1, lsl #2
  468ba4: e1a00006     	mov	r0, r6
  468ba8: ebfc8aea     	bl	0x38b758 <void IStreamBase::readAs<int>(int&)> @ imm = #-0xdd458
  468bac: e0871104     	add	r1, r7, r4, lsl #2
  468bb0: e281105c     	add	r1, r1, #92
  468bb4: e1a00006     	mov	r0, r6
  468bb8: ebfc8ae6     	bl	0x38b758 <void IStreamBase::readAs<int>(int&)> @ imm = #-0xdd468
  468bbc: e1a00006     	mov	r0, r6
  468bc0: e1a01008     	mov	r1, r8
  468bc4: ebfc8ae3     	bl	0x38b758 <void IStreamBase::readAs<int>(int&)> @ imm = #-0xdd474
  468bc8: e59d3004     	ldr	r3, [sp, #0x4]
  468bcc: e2844001     	add	r4, r4, #1
  468bd0: e3540003     	cmp	r4, #3
  468bd4: e58530fc     	str	r3, [r5, #0xfc]
  468bd8: e59d3004     	ldr	r3, [sp, #0x4]
  468bdc: e585315c     	str	r3, [r5, #0x15c]
  468be0: e2855004     	add	r5, r5, #4
  468be4: 1affffec     	bne	0x468b9c <PlayerSavegame::__LoadLevelName(IStreamBase*, void*)+0x24> @ imm = #-0x50
  468be8: e28dd008     	add	sp, sp, #8
  468bec: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
