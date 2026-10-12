
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003454dc <ObjectManager::FlushAllOrphanRenderObjects()>:
  3454dc: e92d4070     	push	{r4, r5, r6, lr}
  3454e0: e1a05000     	mov	r5, r0
  3454e4: e5b54004     	ldr	r4, [r5, #0x4]!
  3454e8: e3a06000     	mov	r6, #0
  3454ec: e1550004     	cmp	r5, r4
  3454f0: 0a00000a     	beq	0x345520 <ObjectManager::FlushAllOrphanRenderObjects()+0x44> @ imm = #0x28
  3454f4: e5943008     	ldr	r3, [r4, #0x8]
  3454f8: e3530000     	cmp	r3, #0
  3454fc: 0a000004     	beq	0x345514 <ObjectManager::FlushAllOrphanRenderObjects()+0x38> @ imm = #0x10
  345500: e1a00003     	mov	r0, r3
  345504: e5933000     	ldr	r3, [r3]
  345508: e1a0e00f     	mov	lr, pc
  34550c: e593f004     	ldr	pc, [r3, #0x4]
  345510: e5846008     	str	r6, [r4, #0x8]
  345514: e5944000     	ldr	r4, [r4]
  345518: e1550004     	cmp	r5, r4
  34551c: 1afffff4     	bne	0x3454f4 <ObjectManager::FlushAllOrphanRenderObjects()+0x18> @ imm = #-0x30
  345520: e1a00005     	mov	r0, r5
  345524: e8bd4070     	pop	{r4, r5, r6, lr}
  345528: eaffff4f     	b	0x34526c <std::priv::_List_base<ObjectBase*, std::allocator<ObjectBase*>>::clear()> @ imm = #-0x2c4
