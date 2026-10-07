
.local-inputs/libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00825000 <COnlineImpl::COnlineImpl()>:
  825000: e92d4070     	push	{r4, r5, r6, lr}
  825004: e59f4020     	ldr	r4, [pc, #0x20]         @ 0x82502c <COnlineImpl::COnlineImpl()+0x2c>
  825008: e1a05000     	mov	r5, r0
  82500c: ebff6209     	bl	0x7fd838 <COnline::COnline()> @ imm = #-0x277dc
  825010: e59f3018     	ldr	r3, [pc, #0x18]         @ 0x825030 <COnlineImpl::COnlineImpl()+0x30>
  825014: e08f4004     	add	r4, pc, r4
  825018: e1a00005     	mov	r0, r5
  82501c: e7943003     	ldr	r3, [r4, r3]
  825020: e2833008     	add	r3, r3, #8
  825024: e5853000     	str	r3, [r5]
  825028: e8bd8070     	pop	{r4, r5, r6, pc}
  82502c: 7c fa 16 00  	.word	0x0016fa7c
  825030: a8 19 00 00  	.word	0x000019a8
