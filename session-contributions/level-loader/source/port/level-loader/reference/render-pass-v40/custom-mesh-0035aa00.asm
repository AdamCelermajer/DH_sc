
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0035aa00 <BaseMeshSceneNode<glitch::collada::CMeshSceneNode>::~BaseMeshSceneNode()>:
  35aa00: e59f3038     	ldr	r3, [pc, #0x38]         @ 0x35aa40 <BaseMeshSceneNode<glitch::collada::CMeshSceneNode>::~BaseMeshSceneNode()+0x40>
  35aa04: e59f2038     	ldr	r2, [pc, #0x38]         @ 0x35aa44 <BaseMeshSceneNode<glitch::collada::CMeshSceneNode>::~BaseMeshSceneNode()+0x44>
  35aa08: e59f1038     	ldr	r1, [pc, #0x38]         @ 0x35aa48 <BaseMeshSceneNode<glitch::collada::CMeshSceneNode>::~BaseMeshSceneNode()+0x48>
  35aa0c: e08f3003     	add	r3, pc, r3
  35aa10: e7932002     	ldr	r2, [r3, r2]
  35aa14: e7931001     	ldr	r1, [r3, r1]
  35aa18: e92d4010     	push	{r4, lr}
  35aa1c: e282cf4a     	add	r12, r2, #296
  35aa20: e282201c     	add	r2, r2, #28
  35aa24: e1a04000     	mov	r4, r0
  35aa28: e5802000     	str	r2, [r0]
  35aa2c: e580c13c     	str	r12, [r0, #0x13c]
  35aa30: e2811004     	add	r1, r1, #4
  35aa34: eb0bae29     	bl	0x6462e0 <glitch::collada::CMeshSceneNode::~CMeshSceneNode()> @ imm = #0x2eb8a4
  35aa38: e1a00004     	mov	r0, r4
  35aa3c: e8bd8010     	pop	{r4, pc}
  35aa40: 84 a0 63 00  	.word	0x0063a084
  35aa44: 5c 3d 00 00  	.word	0x00003d5c
  35aa48: 40 42 00 00  	.word	0x00004240
