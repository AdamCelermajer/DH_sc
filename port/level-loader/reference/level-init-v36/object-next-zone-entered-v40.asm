
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003460cc <ObjectManager::ProcessNextGameObjectToStartUpdate()>:
  3460cc: e92d4010     	push	{r4, lr}
  3460d0: e1a03000     	mov	r3, r0
  3460d4: e1a04000     	mov	r4, r0
  3460d8: e5b30090     	ldr	r0, [r3, #0x90]!
  3460dc: e1500003     	cmp	r0, r3
  3460e0: 0a00000c     	beq	0x346118 <ObjectManager::ProcessNextGameObjectToStartUpdate()+0x4c> @ imm = #0x30
  3460e4: e5903008     	ldr	r3, [r0, #0x8]
  3460e8: e3530000     	cmp	r3, #0
  3460ec: 0a000002     	beq	0x3460fc <ObjectManager::ProcessNextGameObjectToStartUpdate()+0x30> @ imm = #0x8
  3460f0: e1a00003     	mov	r0, r3
  3460f4: eb011985     	bl	0x38c710 <GameObject::ZoneEntered()> @ imm = #0x46614
  3460f8: e5940090     	ldr	r0, [r4, #0x90]
  3460fc: e5903000     	ldr	r3, [r0]
  346100: e5902004     	ldr	r2, [r0, #0x4]
  346104: e3a0100c     	mov	r1, #12
  346108: e5823000     	str	r3, [r2]
  34610c: e5832004     	str	r2, [r3, #0x4]
  346110: e8bd4010     	pop	{r4, lr}
  346114: ea0f0b79     	b	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x3c2de4
  346118: e8bd8010     	pop	{r4, pc}
