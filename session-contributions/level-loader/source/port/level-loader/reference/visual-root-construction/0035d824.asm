
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0035d824 <RootSceneNode::RootSceneNode(glitch::collada::CColladaDatabase const&)>:
  35d824: e92d4070     	push	{r4, r5, r6, lr}
  35d828: e59f50d0     	ldr	r5, [pc, #0xd0]         @ 0x35d900 <RootSceneNode::RootSceneNode(glitch::collada::CColladaDatabase const&)+0xdc>
  35d82c: e59f30d0     	ldr	r3, [pc, #0xd0]         @ 0x35d904 <RootSceneNode::RootSceneNode(glitch::collada::CColladaDatabase const&)+0xe0>
  35d830: e59f20d0     	ldr	r2, [pc, #0xd0]         @ 0x35d908 <RootSceneNode::RootSceneNode(glitch::collada::CColladaDatabase const&)+0xe4>
  35d834: e08f5005     	add	r5, pc, r5
  35d838: e7953003     	ldr	r3, [r5, r3]
  35d83c: e7952002     	ldr	r2, [r5, r2]
  35d840: e3a06001     	mov	r6, #1
  35d844: e593c03c     	ldr	r12, [r3, #0x3c]
  35d848: e2822008     	add	r2, r2, #8
  35d84c: e580220c     	str	r2, [r0, #0x20c]
  35d850: e5806210     	str	r6, [r0, #0x210]
  35d854: e580c000     	str	r12, [r0]
  35d858: e593e040     	ldr	lr, [r3, #0x40]
  35d85c: e51cc00c     	ldr	r12, [r12, #-0xc]
  35d860: e1a02001     	mov	r2, r1
  35d864: e2831004     	add	r1, r3, #4
  35d868: e780e00c     	str	lr, [r0, r12]
  35d86c: e1a04000     	mov	r4, r0
  35d870: eb0bf7f3     	bl	0x65b844 <glitch::collada::CRootSceneNode::CRootSceneNode(glitch::collada::CColladaDatabase const&)> @ imm = #0x2fdfcc
  35d874: e59f3090     	ldr	r3, [pc, #0x90]         @ 0x35d90c <RootSceneNode::RootSceneNode(glitch::collada::CColladaDatabase const&)+0xe8>
  35d878: e2842f6f     	add	r2, r4, #444
  35d87c: e1a00002     	mov	r0, r2
  35d880: e7953003     	ldr	r3, [r5, r3]
  35d884: e58421cc     	str	r2, [r4, #0x1cc]
  35d888: e58421d0     	str	r2, [r4, #0x1d0]
  35d88c: e2832f49     	add	r2, r3, #292
  35d890: e283301c     	add	r3, r3, #28
  35d894: e5843000     	str	r3, [r4]
  35d898: e584220c     	str	r2, [r4, #0x20c]
  35d89c: e3a01010     	mov	r1, #16
  35d8a0: ebfecf75     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x4c22c
  35d8a4: e59421cc     	ldr	r2, [r4, #0x1cc]
  35d8a8: e3a05000     	mov	r5, #0
  35d8ac: e2843f75     	add	r3, r4, #468
  35d8b0: e5c25000     	strb	r5, [r2]
  35d8b4: e1a00003     	mov	r0, r3
  35d8b8: e58431e4     	str	r3, [r4, #0x1e4]
  35d8bc: e58431e8     	str	r3, [r4, #0x1e8]
  35d8c0: e3a01010     	mov	r1, #16
  35d8c4: ebfecf6c     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x4c250
  35d8c8: e59431e4     	ldr	r3, [r4, #0x1e4]
  35d8cc: e1a00004     	mov	r0, r4
  35d8d0: e5c35000     	strb	r5, [r3]
  35d8d4: e5c46208     	strb	r6, [r4, #0x208]
  35d8d8: e5c4520a     	strb	r5, [r4, #0x20a]
  35d8dc: e5c451ec     	strb	r5, [r4, #0x1ec]
  35d8e0: e58451f0     	str	r5, [r4, #0x1f0]
  35d8e4: e58451f4     	str	r5, [r4, #0x1f4]
  35d8e8: e58451f8     	str	r5, [r4, #0x1f8]
  35d8ec: e58451fc     	str	r5, [r4, #0x1fc]
  35d8f0: e5c46200     	strb	r6, [r4, #0x200]
  35d8f4: e5845204     	str	r5, [r4, #0x204]
  35d8f8: e5c45209     	strb	r5, [r4, #0x209]
  35d8fc: e8bd8070     	pop	{r4, r5, r6, pc}
  35d900: 5c 72 63 00  	.word	0x0063725c
  35d904: f4 1a 00 00  	.word	0x00001af4
  35d908: 44 2b 00 00  	.word	0x00002b44
  35d90c: 08 39 00 00  	.word	0x00003908
