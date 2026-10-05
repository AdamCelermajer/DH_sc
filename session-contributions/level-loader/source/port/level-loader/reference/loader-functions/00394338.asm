
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00394338 <GameObject::SetVisualObject(VisualObject*)>:
  394338: e92d4070     	push	{r4, r5, r6, lr}
  39433c: e59032d8     	ldr	r3, [r0, #0x2d8]
  394340: e1a04000     	mov	r4, r0
  394344: e1a05001     	mov	r5, r1
  394348: e1530001     	cmp	r3, r1
  39434c: 0a000008     	beq	0x394374 <GameObject::SetVisualObject(VisualObject*)+0x3c> @ imm = #0x20
  394350: e3530000     	cmp	r3, #0
  394354: 0a000005     	beq	0x394370 <GameObject::SetVisualObject(VisualObject*)+0x38> @ imm = #0x14
  394358: e1a00003     	mov	r0, r3
  39435c: e5933000     	ldr	r3, [r3]
  394360: e1a0e00f     	mov	lr, pc
  394364: e593f004     	ldr	pc, [r3, #0x4]
  394368: e3a03000     	mov	r3, #0
  39436c: e58432d8     	str	r3, [r4, #0x2d8]
  394370: e58452d8     	str	r5, [r4, #0x2d8]
  394374: e8bd8070     	pop	{r4, r5, r6, pc}
