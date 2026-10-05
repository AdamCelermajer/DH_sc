
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048c018 <rnd::EndPath::Impl::Impl(rnd::EndPath const&, rnd::Rule::Impl*)>:
  48c018: e92d4070     	push	{r4, r5, r6, lr}
  48c01c: e59f4028     	ldr	r4, [pc, #0x28]         @ 0x48c04c <rnd::EndPath::Impl::Impl(rnd::EndPath const&, rnd::Rule::Impl*)+0x34>
  48c020: e1a05000     	mov	r5, r0
  48c024: e1a06001     	mov	r6, r1
  48c028: ebffffa3     	bl	0x48bebc <rnd::Rule::Impl::Impl(rnd::Rule const&, rnd::Rule::Impl*)> @ imm = #-0x174
  48c02c: e59f301c     	ldr	r3, [pc, #0x1c]         @ 0x48c050 <rnd::EndPath::Impl::Impl(rnd::EndPath const&, rnd::Rule::Impl*)+0x38>
  48c030: e08f4004     	add	r4, pc, r4
  48c034: e5856044     	str	r6, [r5, #0x44]
  48c038: e7943003     	ldr	r3, [r4, r3]
  48c03c: e1a00005     	mov	r0, r5
  48c040: e2833008     	add	r3, r3, #8
  48c044: e5853000     	str	r3, [r5]
  48c048: e8bd8070     	pop	{r4, r5, r6, pc}
  48c04c: 60 8a 50 00  	.word	0x00508a60
  48c050: 5c 37 00 00  	.word	0x0000375c
