
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048cfcc <rnd::ForceBlock::NewImpl(rnd::Rule::Impl*) const>:
  48cfcc: e92d4070     	push	{r4, r5, r6, lr}
  48cfd0: e1a06000     	mov	r6, r0
  48cfd4: e3a00048     	mov	r0, #72
  48cfd8: e1a05001     	mov	r5, r1
  48cfdc: ebfa0d1c     	bl	0x310454 <CustomAlloc(unsigned int)> @ imm = #-0x17cb90
  48cfe0: e1a01006     	mov	r1, r6
  48cfe4: e1a04000     	mov	r4, r0
  48cfe8: e1a02005     	mov	r2, r5
  48cfec: ebffffc7     	bl	0x48cf10 <rnd::ForceBlock::Impl::Impl(rnd::ForceBlock const&, rnd::Rule::Impl*)> @ imm = #-0xe4
  48cff0: e1a00004     	mov	r0, r4
  48cff4: e8bd8070     	pop	{r4, r5, r6, pc}
