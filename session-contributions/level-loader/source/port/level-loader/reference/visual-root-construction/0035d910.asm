
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0035d910 <RootSceneNode::RootSceneNode(glitch::collada::CColladaDatabase const&)>:
  35d910: e92d4070     	push	{r4, r5, r6, lr}
  35d914: e1a05001     	mov	r5, r1
  35d918: e2811004     	add	r1, r1, #4
  35d91c: e1a04000     	mov	r4, r0
  35d920: eb0bf7c7     	bl	0x65b844 <glitch::collada::CRootSceneNode::CRootSceneNode(glitch::collada::CColladaDatabase const&)> @ imm = #0x2fdf1c
  35d924: e5952000     	ldr	r2, [r5]
  35d928: e2843f6f     	add	r3, r4, #444
  35d92c: e1a00003     	mov	r0, r3
  35d930: e5842000     	str	r2, [r4]
  35d934: e595c034     	ldr	r12, [r5, #0x34]
  35d938: e512201c     	ldr	r2, [r2, #-0x1c]
  35d93c: e3a01010     	mov	r1, #16
  35d940: e3a06000     	mov	r6, #0
  35d944: e784c002     	str	r12, [r4, r2]
  35d948: e5942000     	ldr	r2, [r4]
  35d94c: e595c038     	ldr	r12, [r5, #0x38]
  35d950: e512200c     	ldr	r2, [r2, #-0xc]
  35d954: e784c002     	str	r12, [r4, r2]
  35d958: e58431cc     	str	r3, [r4, #0x1cc]
  35d95c: e58431d0     	str	r3, [r4, #0x1d0]
  35d960: ebfecf45     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x4c2ec
  35d964: e59421cc     	ldr	r2, [r4, #0x1cc]
  35d968: e2843f75     	add	r3, r4, #468
  35d96c: e1a00003     	mov	r0, r3
  35d970: e5c26000     	strb	r6, [r2]
  35d974: e3a01010     	mov	r1, #16
  35d978: e58431e4     	str	r3, [r4, #0x1e4]
  35d97c: e58431e8     	str	r3, [r4, #0x1e8]
  35d980: ebfecf3d     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x4c30c
  35d984: e59421e4     	ldr	r2, [r4, #0x1e4]
  35d988: e3a03001     	mov	r3, #1
  35d98c: e1a00004     	mov	r0, r4
  35d990: e5c26000     	strb	r6, [r2]
  35d994: e5c43208     	strb	r3, [r4, #0x208]
  35d998: e5c4620a     	strb	r6, [r4, #0x20a]
  35d99c: e5c461ec     	strb	r6, [r4, #0x1ec]
  35d9a0: e58461f0     	str	r6, [r4, #0x1f0]
  35d9a4: e58461f4     	str	r6, [r4, #0x1f4]
  35d9a8: e58461f8     	str	r6, [r4, #0x1f8]
  35d9ac: e58461fc     	str	r6, [r4, #0x1fc]
  35d9b0: e5c43200     	strb	r3, [r4, #0x200]
  35d9b4: e5846204     	str	r6, [r4, #0x204]
  35d9b8: e5c46209     	strb	r6, [r4, #0x209]
  35d9bc: e8bd8070     	pop	{r4, r5, r6, pc}
