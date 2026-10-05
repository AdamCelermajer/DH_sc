
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048c054 <rnd::EndPath::NewImpl(rnd::Rule::Impl*) const>:
  48c054: e92d4070     	push	{r4, r5, r6, lr}
  48c058: e1a06000     	mov	r6, r0
  48c05c: e3a00048     	mov	r0, #72
  48c060: e1a05001     	mov	r5, r1
  48c064: ebfa10fa     	bl	0x310454 <CustomAlloc(unsigned int)> @ imm = #-0x17bc18
  48c068: e1a01006     	mov	r1, r6
  48c06c: e1a04000     	mov	r4, r0
  48c070: e1a02005     	mov	r2, r5
  48c074: ebffffe7     	bl	0x48c018 <rnd::EndPath::Impl::Impl(rnd::EndPath const&, rnd::Rule::Impl*)> @ imm = #-0x64
  48c078: e1a00004     	mov	r0, r4
  48c07c: e8bd8070     	pop	{r4, r5, r6, pc}
