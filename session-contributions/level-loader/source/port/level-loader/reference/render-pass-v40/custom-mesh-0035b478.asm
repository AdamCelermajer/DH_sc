
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0035b478 <BaseMeshSceneNode<glitch::collada::CMeshSceneNode>::~BaseMeshSceneNode()>:
  35b478: e92d4070     	push	{r4, r5, r6, lr}
  35b47c: e59f5060     	ldr	r5, [pc, #0x60]         @ 0x35b4e4 <BaseMeshSceneNode<glitch::collada::CMeshSceneNode>::~BaseMeshSceneNode()+0x6c>
  35b480: e59f3060     	ldr	r3, [pc, #0x60]         @ 0x35b4e8 <BaseMeshSceneNode<glitch::collada::CMeshSceneNode>::~BaseMeshSceneNode()+0x70>
  35b484: e59f6060     	ldr	r6, [pc, #0x60]         @ 0x35b4ec <BaseMeshSceneNode<glitch::collada::CMeshSceneNode>::~BaseMeshSceneNode()+0x74>
  35b488: e08f5005     	add	r5, pc, r5
  35b48c: e7953003     	ldr	r3, [r5, r3]
  35b490: e7956006     	ldr	r6, [r5, r6]
  35b494: e1a04000     	mov	r4, r0
  35b498: e2832f4a     	add	r2, r3, #296
  35b49c: e283301c     	add	r3, r3, #28
  35b4a0: e2861004     	add	r1, r6, #4
  35b4a4: e5803000     	str	r3, [r0]
  35b4a8: e580213c     	str	r2, [r0, #0x13c]
  35b4ac: eb0bab8b     	bl	0x6462e0 <glitch::collada::CMeshSceneNode::~CMeshSceneNode()> @ imm = #0x2eae2c
  35b4b0: e5962030     	ldr	r2, [r6, #0x30]
  35b4b4: e59f3034     	ldr	r3, [pc, #0x34]         @ 0x35b4f0 <BaseMeshSceneNode<glitch::collada::CMeshSceneNode>::~BaseMeshSceneNode()+0x78>
  35b4b8: e5961034     	ldr	r1, [r6, #0x34]
  35b4bc: e5842000     	str	r2, [r4]
  35b4c0: e7953003     	ldr	r3, [r5, r3]
  35b4c4: e512200c     	ldr	r2, [r2, #-0xc]
  35b4c8: e1a00004     	mov	r0, r4
  35b4cc: e2833008     	add	r3, r3, #8
  35b4d0: e7841002     	str	r1, [r4, r2]
  35b4d4: e584313c     	str	r3, [r4, #0x13c]
  35b4d8: ebfed3d8     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x4b0a0
  35b4dc: e1a00004     	mov	r0, r4
  35b4e0: e8bd8070     	pop	{r4, r5, r6, pc}
  35b4e4: 08 96 63 00  	.word	0x00639608
  35b4e8: 5c 3d 00 00  	.word	0x00003d5c
  35b4ec: 40 42 00 00  	.word	0x00004240
  35b4f0: 44 2b 00 00  	.word	0x00002b44
