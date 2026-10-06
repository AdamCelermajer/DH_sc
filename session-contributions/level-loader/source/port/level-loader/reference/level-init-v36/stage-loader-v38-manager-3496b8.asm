
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003496b8 <ObjectManager::Flush()>:
  3496b8: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  3496bc: e1a04000     	mov	r4, r0
  3496c0: e24dd008     	sub	sp, sp, #8
  3496c4: e2805060     	add	r5, r0, #96
  3496c8: e5906060     	ldr	r6, [r0, #0x60]
  3496cc: ea000004     	b	0x3496e4 <ObjectManager::Flush()+0x2c> @ imm = #0x10
  3496d0: e5960008     	ldr	r0, [r6, #0x8]
  3496d4: eb0108e4     	bl	0x38ba6c <GameObject::FlushTargetList()> @ imm = #0x42390
  3496d8: e5960008     	ldr	r0, [r6, #0x8]
  3496dc: ebffd1b4     	bl	0x33ddb4 <ObjectBase::Delete()> @ imm = #-0xb930
  3496e0: e5966000     	ldr	r6, [r6]
  3496e4: e1550006     	cmp	r5, r6
  3496e8: 1afffff8     	bne	0x3496d0 <ObjectManager::Flush()+0x18> @ imm = #-0x20
  3496ec: e5946060     	ldr	r6, [r4, #0x60]
  3496f0: ea000004     	b	0x349708 <ObjectManager::Flush()+0x50> @ imm = #0x10
  3496f4: e5960008     	ldr	r0, [r6, #0x8]
  3496f8: e3500000     	cmp	r0, #0
  3496fc: 12800ff2     	addne	r0, r0, #968
  349700: eb020711     	bl	0x3cb34c <CharAI::_UpdatePointers()> @ imm = #0x81c44
  349704: e5966000     	ldr	r6, [r6]
  349708: e1550006     	cmp	r5, r6
  34970c: 1afffff8     	bne	0x3496f4 <ObjectManager::Flush()+0x3c> @ imm = #-0x20
  349710: e5946014     	ldr	r6, [r4, #0x14]
  349714: e284700c     	add	r7, r4, #12
  349718: e3a08000     	mov	r8, #0
  34971c: e1560007     	cmp	r6, r7
  349720: 0a00001e     	beq	0x3497a0 <ObjectManager::Flush()+0xe8> @ imm = #0x78
  349724: e596302c     	ldr	r3, [r6, #0x2c]
  349728: e3530000     	cmp	r3, #0
  34972c: 0a000004     	beq	0x349744 <ObjectManager::Flush()+0x8c> @ imm = #0x10
  349730: e1a00003     	mov	r0, r3
  349734: e5933000     	ldr	r3, [r3]
  349738: e1a0e00f     	mov	lr, pc
  34973c: e593f004     	ldr	pc, [r3, #0x4]
  349740: e586802c     	str	r8, [r6, #0x2c]
  349744: e596200c     	ldr	r2, [r6, #0xc]
  349748: e3520000     	cmp	r2, #0
  34974c: 0a000005     	beq	0x349768 <ObjectManager::Flush()+0xb0> @ imm = #0x14
  349750: e1a06002     	mov	r6, r2
  349754: e5963008     	ldr	r3, [r6, #0x8]
  349758: e3530000     	cmp	r3, #0
  34975c: 0affffee     	beq	0x34971c <ObjectManager::Flush()+0x64> @ imm = #-0x48
  349760: e1a06003     	mov	r6, r3
  349764: eafffffa     	b	0x349754 <ObjectManager::Flush()+0x9c> @ imm = #-0x18
  349768: e5963004     	ldr	r3, [r6, #0x4]
  34976c: e593100c     	ldr	r1, [r3, #0xc]
  349770: e1560001     	cmp	r6, r1
  349774: 1a000005     	bne	0x349790 <ObjectManager::Flush()+0xd8> @ imm = #0x14
  349778: e1a06003     	mov	r6, r3
  34977c: e5933004     	ldr	r3, [r3, #0x4]
  349780: e593200c     	ldr	r2, [r3, #0xc]
  349784: e1520006     	cmp	r2, r6
  349788: 0afffffa     	beq	0x349778 <ObjectManager::Flush()+0xc0> @ imm = #-0x18
  34978c: e596200c     	ldr	r2, [r6, #0xc]
  349790: e1520003     	cmp	r2, r3
  349794: 11a06003     	movne	r6, r3
  349798: e1560007     	cmp	r6, r7
  34979c: 1affffe0     	bne	0x349724 <ObjectManager::Flush()+0x6c> @ imm = #-0x80
  3497a0: e2840090     	add	r0, r4, #144
  3497a4: ebfff05a     	bl	0x345914 <std::priv::_List_base<GameObject*, std::allocator<GameObject*>>::clear()> @ imm = #-0x3e98
  3497a8: e594301c     	ldr	r3, [r4, #0x1c]
  3497ac: e3530000     	cmp	r3, #0
  3497b0: 1a0000ce     	bne	0x349af0 <ObjectManager::Flush()+0x438> @ imm = #0x338
  3497b4: e1a00005     	mov	r0, r5
  3497b8: e1a06004     	mov	r6, r4
  3497bc: ebfff0f2     	bl	0x345b8c <std::priv::_List_base<Character*, std::allocator<Character*>>::clear()> @ imm = #-0x3c38
  3497c0: e5b60068     	ldr	r0, [r6, #0x68]!
  3497c4: e1500006     	cmp	r0, r6
  3497c8: 1a000001     	bne	0x3497d4 <ObjectManager::Flush()+0x11c> @ imm = #0x4
  3497cc: ea000006     	b	0x3497ec <ObjectManager::Flush()+0x134> @ imm = #0x18
  3497d0: e1a00005     	mov	r0, r5
  3497d4: e5905000     	ldr	r5, [r0]
  3497d8: e3a0100c     	mov	r1, #12
  3497dc: eb0efdc7     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x3bf71c
  3497e0: e1550006     	cmp	r5, r6
  3497e4: 1afffff9     	bne	0x3497d0 <ObjectManager::Flush()+0x118> @ imm = #-0x1c
  3497e8: e1a00006     	mov	r0, r6
  3497ec: e584006c     	str	r0, [r4, #0x6c]
  3497f0: e5840068     	str	r0, [r4, #0x68]
  3497f4: e1a06004     	mov	r6, r4
  3497f8: e2840c01     	add	r0, r4, #256
  3497fc: ebffee9a     	bl	0x34526c <std::priv::_List_base<ObjectBase*, std::allocator<ObjectBase*>>::clear()> @ imm = #-0x4598
  349800: e5b60120     	ldr	r0, [r6, #0x120]!
  349804: e1560000     	cmp	r6, r0
  349808: 1a000001     	bne	0x349814 <ObjectManager::Flush()+0x15c> @ imm = #0x4
  34980c: ea000005     	b	0x349828 <ObjectManager::Flush()+0x170> @ imm = #0x14
  349810: e1a00005     	mov	r0, r5
  349814: e5905000     	ldr	r5, [r0]
  349818: e3a0100c     	mov	r1, #12
  34981c: eb0efdb7     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x3bf6dc
  349820: e1560005     	cmp	r6, r5
  349824: 1afffff9     	bne	0x349810 <ObjectManager::Flush()+0x158> @ imm = #-0x1c
  349828: e5943118     	ldr	r3, [r4, #0x118]
  34982c: e5846124     	str	r6, [r4, #0x124]
  349830: e5846120     	str	r6, [r4, #0x120]
  349834: e3530000     	cmp	r3, #0
  349838: 1a0000a2     	bne	0x349ac8 <ObjectManager::Flush()+0x410> @ imm = #0x288
  34983c: e5943174     	ldr	r3, [r4, #0x174]
  349840: e3530000     	cmp	r3, #0
  349844: 0a000008     	beq	0x34986c <ObjectManager::Flush()+0x1b4> @ imm = #0x20
  349848: e2845f59     	add	r5, r4, #356
  34984c: e1a00005     	mov	r0, r5
  349850: e5941168     	ldr	r1, [r4, #0x168]
  349854: ebfff1aa     	bl	0x345f04 <std::priv::_Rb_tree<short, std::less<short>, std::pair<short const, std::set<short, std::less<short>, std::allocator<short>>>, std::priv::_Select1st<std::pair<short const, std::set<short, std::less<short>, std::allocator<short>>>>, std::priv::_MapTraitsT<std::pair<short const, std::set<short, std::less<short>, std::allocator<short>>>>, std::allocator<std::pair<short const, std::set<short, std::less<short>, std::allocator<short>>>>>::_M_erase(std::priv::_Rb_tree_node_base*)> @ imm = #-0x3958
  349858: e3a03000     	mov	r3, #0
  34985c: e5845170     	str	r5, [r4, #0x170]
  349860: e5843174     	str	r3, [r4, #0x174]
  349864: e584516c     	str	r5, [r4, #0x16c]
  349868: e5843168     	str	r3, [r4, #0x168]
  34986c: e594318c     	ldr	r3, [r4, #0x18c]
  349870: e3530000     	cmp	r3, #0
  349874: 0a000008     	beq	0x34989c <ObjectManager::Flush()+0x1e4> @ imm = #0x20
  349878: e2845f5f     	add	r5, r4, #380
  34987c: e1a00005     	mov	r0, r5
  349880: e5941180     	ldr	r1, [r4, #0x180]
  349884: ebfff19e     	bl	0x345f04 <std::priv::_Rb_tree<short, std::less<short>, std::pair<short const, std::set<short, std::less<short>, std::allocator<short>>>, std::priv::_Select1st<std::pair<short const, std::set<short, std::less<short>, std::allocator<short>>>>, std::priv::_MapTraitsT<std::pair<short const, std::set<short, std::less<short>, std::allocator<short>>>>, std::allocator<std::pair<short const, std::set<short, std::less<short>, std::allocator<short>>>>>::_M_erase(std::priv::_Rb_tree_node_base*)> @ imm = #-0x3988
  349888: e3a03000     	mov	r3, #0
  34988c: e5845188     	str	r5, [r4, #0x188]
  349890: e584318c     	str	r3, [r4, #0x18c]
  349894: e5845184     	str	r5, [r4, #0x184]
  349898: e5843180     	str	r3, [r4, #0x180]
  34989c: e59431a4     	ldr	r3, [r4, #0x1a4]
  3498a0: e3530000     	cmp	r3, #0
  3498a4: 0a000008     	beq	0x3498cc <ObjectManager::Flush()+0x214> @ imm = #0x20
  3498a8: e2845f65     	add	r5, r4, #404
  3498ac: e1a00005     	mov	r0, r5
  3498b0: e5941198     	ldr	r1, [r4, #0x198]
  3498b4: ebfff192     	bl	0x345f04 <std::priv::_Rb_tree<short, std::less<short>, std::pair<short const, std::set<short, std::less<short>, std::allocator<short>>>, std::priv::_Select1st<std::pair<short const, std::set<short, std::less<short>, std::allocator<short>>>>, std::priv::_MapTraitsT<std::pair<short const, std::set<short, std::less<short>, std::allocator<short>>>>, std::allocator<std::pair<short const, std::set<short, std::less<short>, std::allocator<short>>>>>::_M_erase(std::priv::_Rb_tree_node_base*)> @ imm = #-0x39b8
  3498b8: e3a03000     	mov	r3, #0
  3498bc: e58451a0     	str	r5, [r4, #0x1a0]
  3498c0: e58431a4     	str	r3, [r4, #0x1a4]
  3498c4: e584519c     	str	r5, [r4, #0x19c]
  3498c8: e5843198     	str	r3, [r4, #0x198]
  3498cc: e2840f4a     	add	r0, r4, #296
  3498d0: ebfff05d     	bl	0x345a4c <std::priv::_List_base<int, std::allocator<int>>::clear()> @ imm = #-0x3e8c
  3498d4: e1a06004     	mov	r6, r4
  3498d8: e2840e13     	add	r0, r4, #304
  3498dc: ebfff05a     	bl	0x345a4c <std::priv::_List_base<int, std::allocator<int>>::clear()> @ imm = #-0x3e98
  3498e0: e5b60080     	ldr	r0, [r6, #0x80]!
  3498e4: e1560000     	cmp	r6, r0
  3498e8: 1a000001     	bne	0x3498f4 <ObjectManager::Flush()+0x23c> @ imm = #0x4
  3498ec: ea000005     	b	0x349908 <ObjectManager::Flush()+0x250> @ imm = #0x14
  3498f0: e1a00005     	mov	r0, r5
  3498f4: e5905000     	ldr	r5, [r0]
  3498f8: e3a0100c     	mov	r1, #12
  3498fc: eb0efd7f     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x3bf5fc
  349900: e1560005     	cmp	r6, r5
  349904: 1afffff9     	bne	0x3498f0 <ObjectManager::Flush()+0x238> @ imm = #-0x1c
  349908: e2845088     	add	r5, r4, #136
  34990c: e1a00005     	mov	r0, r5
  349910: e5846084     	str	r6, [r4, #0x84]
  349914: e5846080     	str	r6, [r4, #0x80]
  349918: ebffeffd     	bl	0x345914 <std::priv::_List_base<GameObject*, std::allocator<GameObject*>>::clear()> @ imm = #-0x400c
  34991c: e1a01005     	mov	r1, r5
  349920: e1a00004     	mov	r0, r4
  349924: ebffe39d     	bl	0x3427a0 <ObjectManager::AddRoomObjects(std::list<GameObject*, std::allocator<GameObject*>>*)> @ imm = #-0x718c
  349928: e5943158     	ldr	r3, [r4, #0x158]
  34992c: e3a05000     	mov	r5, #0
  349930: e5845138     	str	r5, [r4, #0x138]
  349934: e1530005     	cmp	r3, r5
  349938: e584513c     	str	r5, [r4, #0x13c]
  34993c: e5845140     	str	r5, [r4, #0x140]
  349940: 1a000057     	bne	0x349aa4 <ObjectManager::Flush()+0x3ec> @ imm = #0x15c
  349944: e28d1008     	add	r1, sp, #8
  349948: e3a05000     	mov	r5, #0
  34994c: e5215004     	str	r5, [r1, #-0x4]!
  349950: e1a00007     	mov	r0, r7
  349954: e5c45160     	strb	r5, [r4, #0x160]
  349958: ebfffef3     	bl	0x34952c <ObjectListItem& std::map<int, ObjectListItem, std::less<int>, std::allocator<std::pair<int const, ObjectListItem>>>::operator[]<int>(int const&)> @ imm = #-0x434
  34995c: e3a03001     	mov	r3, #1
  349960: e584304c     	str	r3, [r4, #0x4c]
  349964: e284003c     	add	r0, r4, #60
  349968: e5845058     	str	r5, [r4, #0x58]
  34996c: e5845050     	str	r5, [r4, #0x50]
  349970: e5845054     	str	r5, [r4, #0x54]
  349974: ebffee3c     	bl	0x34526c <std::priv::_List_base<ObjectBase*, std::allocator<ObjectBase*>>::clear()> @ imm = #-0x4710
  349978: e284002c     	add	r0, r4, #44
  34997c: ebffee3a     	bl	0x34526c <std::priv::_List_base<ObjectBase*, std::allocator<ObjectBase*>>::clear()> @ imm = #-0x4718
  349980: e2840044     	add	r0, r4, #68
  349984: ebffee38     	bl	0x34526c <std::priv::_List_base<ObjectBase*, std::allocator<ObjectBase*>>::clear()> @ imm = #-0x4720
  349988: e2840034     	add	r0, r4, #52
  34998c: ebffee36     	bl	0x34526c <std::priv::_List_base<ObjectBase*, std::allocator<ObjectBase*>>::clear()> @ imm = #-0x4728
  349990: e1a06004     	mov	r6, r4
  349994: e2840070     	add	r0, r4, #112
  349998: ebfff07b     	bl	0x345b8c <std::priv::_List_base<Character*, std::allocator<Character*>>::clear()> @ imm = #-0x3e14
  34999c: e5b60024     	ldr	r0, [r6, #0x24]!
  3499a0: e1500006     	cmp	r0, r6
  3499a4: 1a000001     	bne	0x3499b0 <ObjectManager::Flush()+0x2f8> @ imm = #0x4
  3499a8: ea000006     	b	0x3499c8 <ObjectManager::Flush()+0x310> @ imm = #0x18
  3499ac: e1a00005     	mov	r0, r5
  3499b0: e5905000     	ldr	r5, [r0]
  3499b4: e3a0100c     	mov	r1, #12
  3499b8: eb0efd50     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x3bf540
  3499bc: e1550006     	cmp	r5, r6
  3499c0: 1afffff9     	bne	0x3499ac <ObjectManager::Flush()+0x2f4> @ imm = #-0x1c
  3499c4: e1a00006     	mov	r0, r6
  3499c8: e3a05000     	mov	r5, #0
  3499cc: e5840028     	str	r0, [r4, #0x28]
  3499d0: e5840024     	str	r0, [r4, #0x24]
  3499d4: e584507c     	str	r5, [r4, #0x7c]
  3499d8: e1a00004     	mov	r0, r4
  3499dc: ebffeebe     	bl	0x3454dc <ObjectManager::FlushAllOrphanRenderObjects()> @ imm = #-0x4508
  3499e0: e59430a8     	ldr	r3, [r4, #0xa8]
  3499e4: e1530005     	cmp	r3, r5
  3499e8: 0a000007     	beq	0x349a0c <ObjectManager::Flush()+0x354> @ imm = #0x1c
  3499ec: e2846098     	add	r6, r4, #152
  3499f0: e1a00006     	mov	r0, r6
  3499f4: e594109c     	ldr	r1, [r4, #0x9c]
  3499f8: ebfff053     	bl	0x345b4c <std::priv::_Rb_tree<int, std::less<int>, std::pair<int const, std::list<int, std::allocator<int>>>, std::priv::_Select1st<std::pair<int const, std::list<int, std::allocator<int>>>>, std::priv::_MapTraitsT<std::pair<int const, std::list<int, std::allocator<int>>>>, std::allocator<std::pair<int const, std::list<int, std::allocator<int>>>>>::_M_erase(std::priv::_Rb_tree_node_base*)> @ imm = #-0x3eb4
  3499fc: e58460a4     	str	r6, [r4, #0xa4]
  349a00: e58450a8     	str	r5, [r4, #0xa8]
  349a04: e58460a0     	str	r6, [r4, #0xa0]
  349a08: e584509c     	str	r5, [r4, #0x9c]
  349a0c: e59430c0     	ldr	r3, [r4, #0xc0]
  349a10: e3530000     	cmp	r3, #0
  349a14: 0a000008     	beq	0x349a3c <ObjectManager::Flush()+0x384> @ imm = #0x20
  349a18: e28450b0     	add	r5, r4, #176
  349a1c: e1a00005     	mov	r0, r5
  349a20: e59410b4     	ldr	r1, [r4, #0xb4]
  349a24: ebfff156     	bl	0x345f84 <std::priv::_Rb_tree<int, std::less<int>, std::pair<int const, short>, std::priv::_Select1st<std::pair<int const, short>>, std::priv::_MapTraitsT<std::pair<int const, short>>, std::allocator<std::pair<int const, short>>>::_M_erase(std::priv::_Rb_tree_node_base*)> @ imm = #-0x3aa8
  349a28: e3a03000     	mov	r3, #0
  349a2c: e58450bc     	str	r5, [r4, #0xbc]
  349a30: e58430c0     	str	r3, [r4, #0xc0]
  349a34: e58450b8     	str	r5, [r4, #0xb8]
  349a38: e58430b4     	str	r3, [r4, #0xb4]
  349a3c: e59430d8     	ldr	r3, [r4, #0xd8]
  349a40: e3530000     	cmp	r3, #0
  349a44: 0a000008     	beq	0x349a6c <ObjectManager::Flush()+0x3b4> @ imm = #0x20
  349a48: e28450c8     	add	r5, r4, #200
  349a4c: e1a00005     	mov	r0, r5
  349a50: e59410cc     	ldr	r1, [r4, #0xcc]
  349a54: ebfff14a     	bl	0x345f84 <std::priv::_Rb_tree<int, std::less<int>, std::pair<int const, short>, std::priv::_Select1st<std::pair<int const, short>>, std::priv::_MapTraitsT<std::pair<int const, short>>, std::allocator<std::pair<int const, short>>>::_M_erase(std::priv::_Rb_tree_node_base*)> @ imm = #-0x3ad8
  349a58: e3a03000     	mov	r3, #0
  349a5c: e58450d4     	str	r5, [r4, #0xd4]
  349a60: e58430d8     	str	r3, [r4, #0xd8]
  349a64: e58450d0     	str	r5, [r4, #0xd0]
  349a68: e58430cc     	str	r3, [r4, #0xcc]
  349a6c: e59430f0     	ldr	r3, [r4, #0xf0]
  349a70: e3530000     	cmp	r3, #0
  349a74: 0a000008     	beq	0x349a9c <ObjectManager::Flush()+0x3e4> @ imm = #0x20
  349a78: e28450e0     	add	r5, r4, #224
  349a7c: e1a00005     	mov	r0, r5
  349a80: e59410e4     	ldr	r1, [r4, #0xe4]
  349a84: ebfff13e     	bl	0x345f84 <std::priv::_Rb_tree<int, std::less<int>, std::pair<int const, short>, std::priv::_Select1st<std::pair<int const, short>>, std::priv::_MapTraitsT<std::pair<int const, short>>, std::allocator<std::pair<int const, short>>>::_M_erase(std::priv::_Rb_tree_node_base*)> @ imm = #-0x3b08
  349a88: e3a03000     	mov	r3, #0
  349a8c: e58430f0     	str	r3, [r4, #0xf0]
  349a90: e58450ec     	str	r5, [r4, #0xec]
  349a94: e58450e8     	str	r5, [r4, #0xe8]
  349a98: e58430e4     	str	r3, [r4, #0xe4]
  349a9c: e28dd008     	add	sp, sp, #8
  349aa0: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  349aa4: e2846f52     	add	r6, r4, #328
  349aa8: e1a00006     	mov	r0, r6
  349aac: e594114c     	ldr	r1, [r4, #0x14c]
  349ab0: ebfff077     	bl	0x345c94 <std::priv::_Rb_tree<int, std::less<int>, std::pair<int const, int>, std::priv::_Select1st<std::pair<int const, int>>, std::priv::_MapTraitsT<std::pair<int const, int>>, std::allocator<std::pair<int const, int>>>::_M_erase(std::priv::_Rb_tree_node_base*)> @ imm = #-0x3e24
  349ab4: e5846154     	str	r6, [r4, #0x154]
  349ab8: e5845158     	str	r5, [r4, #0x158]
  349abc: e5846150     	str	r6, [r4, #0x150]
  349ac0: e584514c     	str	r5, [r4, #0x14c]
  349ac4: eaffff9e     	b	0x349944 <ObjectManager::Flush()+0x28c> @ imm = #-0x188
  349ac8: e2845f42     	add	r5, r4, #264
  349acc: e1a00005     	mov	r0, r5
  349ad0: e594110c     	ldr	r1, [r4, #0x10c]
  349ad4: ebffef7e     	bl	0x3458d4 <std::priv::_Rb_tree<short, std::less<short>, std::pair<short const, std::list<ObjectBase*, std::allocator<ObjectBase*>>>, std::priv::_Select1st<std::pair<short const, std::list<ObjectBase*, std::allocator<ObjectBase*>>>>, std::priv::_MapTraitsT<std::pair<short const, std::list<ObjectBase*, std::allocator<ObjectBase*>>>>, std::allocator<std::pair<short const, std::list<ObjectBase*, std::allocator<ObjectBase*>>>>>::_M_erase(std::priv::_Rb_tree_node_base*)> @ imm = #-0x4208
  349ad8: e3a03000     	mov	r3, #0
  349adc: e5845114     	str	r5, [r4, #0x114]
  349ae0: e5843118     	str	r3, [r4, #0x118]
  349ae4: e5845110     	str	r5, [r4, #0x110]
  349ae8: e584310c     	str	r3, [r4, #0x10c]
  349aec: eaffff52     	b	0x34983c <ObjectManager::Flush()+0x184> @ imm = #-0x2b8
  349af0: e1a00007     	mov	r0, r7
  349af4: e5941010     	ldr	r1, [r4, #0x10]
  349af8: ebfff8f5     	bl	0x347ed4 <std::priv::_Rb_tree<int, std::less<int>, std::pair<int const, ObjectListItem>, std::priv::_Select1st<std::pair<int const, ObjectListItem>>, std::priv::_MapTraitsT<std::pair<int const, ObjectListItem>>, std::allocator<std::pair<int const, ObjectListItem>>>::_M_erase(std::priv::_Rb_tree_node_base*)> @ imm = #-0x1c2c
  349afc: e3a03000     	mov	r3, #0
  349b00: e584301c     	str	r3, [r4, #0x1c]
  349b04: e5847014     	str	r7, [r4, #0x14]
  349b08: e5843010     	str	r3, [r4, #0x10]
  349b0c: e5847018     	str	r7, [r4, #0x18]
  349b10: eaffff27     	b	0x3497b4 <ObjectManager::Flush()+0xfc> @ imm = #-0x364
