
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048c080 <rnd::EndPath::Impl::Impl(rnd::EndPath const&, rnd::Rule::Impl*)>:
  48c080: e92d4070     	push	{r4, r5, r6, lr}
  48c084: e59f4028     	ldr	r4, [pc, #0x28]         @ 0x48c0b4 <rnd::EndPath::Impl::Impl(rnd::EndPath const&, rnd::Rule::Impl*)+0x34>
  48c088: e1a05000     	mov	r5, r0
  48c08c: e1a06001     	mov	r6, r1
  48c090: ebffff89     	bl	0x48bebc <rnd::Rule::Impl::Impl(rnd::Rule const&, rnd::Rule::Impl*)> @ imm = #-0x1dc
  48c094: e59f301c     	ldr	r3, [pc, #0x1c]         @ 0x48c0b8 <rnd::EndPath::Impl::Impl(rnd::EndPath const&, rnd::Rule::Impl*)+0x38>
  48c098: e08f4004     	add	r4, pc, r4
  48c09c: e5856044     	str	r6, [r5, #0x44]
  48c0a0: e7943003     	ldr	r3, [r4, r3]
  48c0a4: e1a00005     	mov	r0, r5
  48c0a8: e2833008     	add	r3, r3, #8
  48c0ac: e5853000     	str	r3, [r5]
  48c0b0: e8bd8070     	pop	{r4, r5, r6, pc}
  48c0b4: f8 89 50 00  	.word	0x005089f8
  48c0b8: 5c 37 00 00  	.word	0x0000375c
