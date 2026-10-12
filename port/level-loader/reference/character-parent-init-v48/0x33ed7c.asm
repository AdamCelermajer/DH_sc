
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0033ed7c <ConditionData::ConditionData()>:
  33ed7c: e59f204c     	ldr	r2, [pc, #0x4c]         @ 0x33edd0 <ConditionData::ConditionData()+0x54>
  33ed80: e59fc04c     	ldr	r12, [pc, #0x4c]        @ 0x33edd4 <ConditionData::ConditionData()+0x58>
  33ed84: e1a03000     	mov	r3, r0
  33ed88: e08f2002     	add	r2, pc, r2
  33ed8c: e792c00c     	ldr	r12, [r2, r12]
  33ed90: e92d4010     	push	{r4, lr}
  33ed94: e28cc008     	add	r12, r12, #8
  33ed98: e1a04000     	mov	r4, r0
  33ed9c: e483c004     	str	r12, [r3], #4
  33eda0: e1a00003     	mov	r0, r3
  33eda4: e5843014     	str	r3, [r4, #0x14]
  33eda8: e5843018     	str	r3, [r4, #0x18]
  33edac: e3a01010     	mov	r1, #16
  33edb0: ebff4a31     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x2d73c
  33edb4: e5942014     	ldr	r2, [r4, #0x14]
  33edb8: e3a03000     	mov	r3, #0
  33edbc: e1a00004     	mov	r0, r4
  33edc0: e5c23000     	strb	r3, [r2]
  33edc4: e5c43020     	strb	r3, [r4, #0x20]
  33edc8: e584301c     	str	r3, [r4, #0x1c]
  33edcc: e8bd8010     	pop	{r4, pc}
  33edd0: 08 5d 65 00  	.word	0x00655d08
  33edd4: 58 2e 00 00  	.word	0x00002e58
