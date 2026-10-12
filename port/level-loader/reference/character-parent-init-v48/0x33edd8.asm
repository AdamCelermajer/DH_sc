
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0033edd8 <ConditionData::ConditionData()>:
  33edd8: e59f204c     	ldr	r2, [pc, #0x4c]         @ 0x33ee2c <ConditionData::ConditionData()+0x54>
  33eddc: e59fc04c     	ldr	r12, [pc, #0x4c]        @ 0x33ee30 <ConditionData::ConditionData()+0x58>
  33ede0: e1a03000     	mov	r3, r0
  33ede4: e08f2002     	add	r2, pc, r2
  33ede8: e792c00c     	ldr	r12, [r2, r12]
  33edec: e92d4010     	push	{r4, lr}
  33edf0: e28cc008     	add	r12, r12, #8
  33edf4: e1a04000     	mov	r4, r0
  33edf8: e483c004     	str	r12, [r3], #4
  33edfc: e1a00003     	mov	r0, r3
  33ee00: e5843014     	str	r3, [r4, #0x14]
  33ee04: e5843018     	str	r3, [r4, #0x18]
  33ee08: e3a01010     	mov	r1, #16
  33ee0c: ebff4a1a     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x2d798
  33ee10: e5942014     	ldr	r2, [r4, #0x14]
  33ee14: e3a03000     	mov	r3, #0
  33ee18: e1a00004     	mov	r0, r4
  33ee1c: e5c23000     	strb	r3, [r2]
  33ee20: e5c43020     	strb	r3, [r4, #0x20]
  33ee24: e584301c     	str	r3, [r4, #0x1c]
  33ee28: e8bd8010     	pop	{r4, pc}
  33ee2c: ac 5c 65 00  	.word	0x00655cac
  33ee30: 58 2e 00 00  	.word	0x00002e58
