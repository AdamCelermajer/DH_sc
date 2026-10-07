
.local-inputs/libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00468938 <PlayerSavegame::__LoadLevelEntryPoint(IStreamBase*, void*)>:
  468938: e92d4070     	push	{r4, r5, r6, lr}
  46893c: e1a04001     	mov	r4, r1
  468940: e1a05000     	mov	r5, r0
  468944: e2811040     	add	r1, r1, #64
  468948: ebfc8b82     	bl	0x38b758 <void IStreamBase::readAs<int>(int&)> @ imm = #-0xdd1f8
  46894c: e1a00005     	mov	r0, r5
  468950: e2841044     	add	r1, r4, #68
  468954: ebfc8b7f     	bl	0x38b758 <void IStreamBase::readAs<int>(int&)> @ imm = #-0xdd204
  468958: e1a00005     	mov	r0, r5
  46895c: e2841048     	add	r1, r4, #72
  468960: e8bd4070     	pop	{r4, r5, r6, lr}
  468964: eafc8b7b     	b	0x38b758 <void IStreamBase::readAs<int>(int&)> @ imm = #-0xdd214
