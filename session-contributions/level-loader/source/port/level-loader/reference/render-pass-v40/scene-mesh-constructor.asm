
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00646588 <glitch::collada::CMeshSceneNode::CMeshSceneNode(boost::intrusive_ptr<glitch::collada::IMesh> const&, glitch::collada::SNode*, int, glitch::core::vector3d<float> const&, glitch::core::quaternion const&, glitch::core::vector3d<float> const&)>:
  646588: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  64658c: e59f60d4     	ldr	r6, [pc, #0xd4]         @ 0x646668 <glitch::collada::CMeshSceneNode::CMeshSceneNode(boost::intrusive_ptr<glitch::collada::IMesh> const&, glitch::collada::SNode*, int, glitch::core::vector3d<float> const&, glitch::core::quaternion const&, glitch::core::vector3d<float> const&)+0xe0>
  646590: e59fe0d4     	ldr	lr, [pc, #0xd4]         @ 0x64666c <glitch::collada::CMeshSceneNode::CMeshSceneNode(boost::intrusive_ptr<glitch::collada::IMesh> const&, glitch::collada::SNode*, int, glitch::core::vector3d<float> const&, glitch::core::quaternion const&, glitch::core::vector3d<float> const&)+0xe4>
  646594: e59fc0d4     	ldr	r12, [pc, #0xd4]        @ 0x646670 <glitch::collada::CMeshSceneNode::CMeshSceneNode(boost::intrusive_ptr<glitch::collada::IMesh> const&, glitch::collada::SNode*, int, glitch::core::vector3d<float> const&, glitch::core::quaternion const&, glitch::core::vector3d<float> const&)+0xe8>
  646598: e08f6006     	add	r6, pc, r6
  64659c: e796500e     	ldr	r5, [r6, lr]
  6465a0: e796c00c     	ldr	r12, [r6, r12]
  6465a4: e3a07001     	mov	r7, #1
  6465a8: e595e024     	ldr	lr, [r5, #0x24]
  6465ac: e28cc008     	add	r12, r12, #8
  6465b0: e580713c     	str	r7, [r0, #0x13c]
  6465b4: e580e000     	str	lr, [r0]
  6465b8: e580c138     	str	r12, [r0, #0x138]
  6465bc: e51ec00c     	ldr	r12, [lr, #-0xc]
  6465c0: e595e028     	ldr	lr, [r5, #0x28]
  6465c4: e24dd008     	sub	sp, sp, #8
  6465c8: e1a08001     	mov	r8, r1
  6465cc: e780e00c     	str	lr, [r0, r12]
  6465d0: e59dc024     	ldr	r12, [sp, #0x24]
  6465d4: e1a07002     	mov	r7, r2
  6465d8: e2851008     	add	r1, r5, #8
  6465dc: e58dc000     	str	r12, [sp]
  6465e0: e59dc028     	ldr	r12, [sp, #0x28]
  6465e4: e1a02003     	mov	r2, r3
  6465e8: e59d3020     	ldr	r3, [sp, #0x20]
  6465ec: e1a04000     	mov	r4, r0
  6465f0: e58dc004     	str	r12, [sp, #0x4]
  6465f4: ebfd4ab1     	bl	0x5990c0 <glitch::scene::ISceneNode::ISceneNode(int, glitch::core::vector3d<float> const&, glitch::core::quaternion const&, glitch::core::vector3d<float> const&)> @ imm = #-0xad53c
  6465f8: e5952004     	ldr	r2, [r5, #0x4]
  6465fc: e5951014     	ldr	r1, [r5, #0x14]
  646600: e59f306c     	ldr	r3, [pc, #0x6c]         @ 0x646674 <glitch::collada::CMeshSceneNode::CMeshSceneNode(boost::intrusive_ptr<glitch::collada::IMesh> const&, glitch::collada::SNode*, int, glitch::core::vector3d<float> const&, glitch::core::quaternion const&, glitch::core::vector3d<float> const&)+0xec>
  646604: e5842000     	str	r2, [r4]
  646608: e512201c     	ldr	r2, [r2, #-0x1c]
  64660c: e7963003     	ldr	r3, [r6, r3]
  646610: e5950018     	ldr	r0, [r5, #0x18]
  646614: e7841002     	str	r1, [r4, r2]
  646618: e5941000     	ldr	r1, [r4]
  64661c: e2832f4a     	add	r2, r3, #296
  646620: e283301c     	add	r3, r3, #28
  646624: e511100c     	ldr	r1, [r1, #-0xc]
  646628: e7840001     	str	r0, [r4, r1]
  64662c: e5842138     	str	r2, [r4, #0x138]
  646630: e5843000     	str	r3, [r4]
  646634: e5847130     	str	r7, [r4, #0x130]
  646638: e5983000     	ldr	r3, [r8]
  64663c: e1a00004     	mov	r0, r4
  646640: e3a01002     	mov	r1, #2
  646644: e3530000     	cmp	r3, #0
  646648: e5843134     	str	r3, [r4, #0x134]
  64664c: 15932004     	ldrne	r2, [r3, #0x4]
  646650: 12822001     	addne	r2, r2, #1
  646654: 15832004     	strne	r2, [r3, #0x4]
  646658: ebfd42cf     	bl	0x59719c <glitch::scene::ISceneNode::setAutomaticCulling(glitch::scene::E_CULLING_TYPE)> @ imm = #-0xaf4c4
  64665c: e1a00004     	mov	r0, r4
  646660: e28dd008     	add	sp, sp, #8
  646664: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  646668: f8 e4 34 00  	.word	0x0034e4f8
  64666c: 00 27 00 00  	.word	0x00002700
  646670: 44 2b 00 00  	.word	0x00002b44
  646674: 7c 43 00 00  	.word	0x0000437c
