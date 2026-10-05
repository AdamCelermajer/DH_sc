
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048c210 <rnd::Path::NewImpl(rnd::Rule::Impl*) const>:
  48c210: e92d4070     	push	{r4, r5, r6, lr}
  48c214: e1a06000     	mov	r6, r0
  48c218: e3a0004c     	mov	r0, #76
  48c21c: e1a05001     	mov	r5, r1
  48c220: ebfa108b     	bl	0x310454 <CustomAlloc(unsigned int)> @ imm = #-0x17bdd4
  48c224: e1a01006     	mov	r1, r6
  48c228: e1a04000     	mov	r4, r0
  48c22c: e1a02005     	mov	r2, r5
  48c230: ebffffbe     	bl	0x48c130 <rnd::Path::Impl::Impl(rnd::Path const&, rnd::Rule::Impl*)> @ imm = #-0x108
  48c234: e1a00004     	mov	r0, r4
  48c238: e8bd8070     	pop	{r4, r5, r6, pc}
