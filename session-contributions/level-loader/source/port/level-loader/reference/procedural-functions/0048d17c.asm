
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048d17c <rnd::RootRule::NewImpl(rnd::Rule::Impl*) const>:
  48d17c: e92d4070     	push	{r4, r5, r6, lr}
  48d180: e1a05000     	mov	r5, r0
  48d184: e3a00048     	mov	r0, #72
  48d188: ebfa0cb1     	bl	0x310454 <CustomAlloc(unsigned int)> @ imm = #-0x17cd3c
  48d18c: e1a01005     	mov	r1, r5
  48d190: e1a04000     	mov	r4, r0
  48d194: ebffffc6     	bl	0x48d0b4 <rnd::RootRule::Impl::Impl(rnd::RootRule const&)> @ imm = #-0xe8
  48d198: e1a00004     	mov	r0, r4
  48d19c: e8bd8070     	pop	{r4, r5, r6, pc}
