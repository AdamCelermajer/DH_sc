
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0034552c <ObjectManager::InitPost()>:
  34552c: e92d40f0     	push	{r4, r5, r6, r7, lr}
  345530: e59f5374     	ldr	r5, [pc, #0x374]        @ 0x3458ac <ObjectManager::InitPost()+0x380>
  345534: e24dd024     	sub	sp, sp, #36
  345538: e1a04000     	mov	r4, r0
  34553c: e08f5005     	add	r5, pc, r5
  345540: e595601c     	ldr	r6, [r5, #0x1c]
  345544: e2166001     	ands	r6, r6, #1
  345548: 0a000034     	beq	0x345620 <ObjectManager::InitPost()+0xf4> @ imm = #0xd0
  34554c: e59f535c     	ldr	r5, [pc, #0x35c]        @ 0x3458b0 <ObjectManager::InitPost()+0x384>
  345550: e08f5005     	add	r5, pc, r5
  345554: e5956024     	ldr	r6, [r5, #0x24]
  345558: e2166001     	ands	r6, r6, #1
  34555c: 0a000038     	beq	0x345644 <ObjectManager::InitPost()+0x118> @ imm = #0xe0
  345560: e28d6010     	add	r6, sp, #16
  345564: e1a00006     	mov	r0, r6
  345568: ebffe7e7     	bl	0x33f50c <ObjectHandle::ObjectHandle()> @ imm = #-0x6064
  34556c: e594207c     	ldr	r2, [r4, #0x7c]
  345570: e3520000     	cmp	r2, #0
  345574: 1a000008     	bne	0x34559c <ObjectManager::InitPost()+0x70> @ imm = #0x20
  345578: e59f3334     	ldr	r3, [pc, #0x334]        @ 0x3458b4 <ObjectManager::InitPost()+0x388>
  34557c: e5942068     	ldr	r2, [r4, #0x68]
  345580: e08f3003     	add	r3, pc, r3
  345584: e5832028     	str	r2, [r3, #0x28]
  345588: e5942014     	ldr	r2, [r4, #0x14]
  34558c: e5832020     	str	r2, [r3, #0x20]
  345590: e594207c     	ldr	r2, [r4, #0x7c]
  345594: e2822001     	add	r2, r2, #1
  345598: e584207c     	str	r2, [r4, #0x7c]
  34559c: e59f5314     	ldr	r5, [pc, #0x314]        @ 0x3458b8 <ObjectManager::InitPost()+0x38c>
  3455a0: e2841068     	add	r1, r4, #104
  3455a4: e08f5005     	add	r5, pc, r5
  3455a8: e5953028     	ldr	r3, [r5, #0x28]
  3455ac: e1510003     	cmp	r1, r3
  3455b0: 0a000063     	beq	0x345744 <ObjectManager::InitPost()+0x218> @ imm = #0x18c
  3455b4: e3520001     	cmp	r2, #1
  3455b8: 0a0000b0     	beq	0x345880 <ObjectManager::InitPost()+0x354> @ imm = #0x2c0
  3455bc: e59f52f8     	ldr	r5, [pc, #0x2f8]        @ 0x3458bc <ObjectManager::InitPost()+0x390>
  3455c0: e284100c     	add	r1, r4, #12
  3455c4: e08f5005     	add	r5, pc, r5
  3455c8: e5953020     	ldr	r3, [r5, #0x20]
  3455cc: e1510003     	cmp	r1, r3
  3455d0: 0a00004c     	beq	0x345708 <ObjectManager::InitPost()+0x1dc> @ imm = #0x130
  3455d4: e3520003     	cmp	r2, #3
  3455d8: 0a000070     	beq	0x3457a0 <ObjectManager::InitPost()+0x274> @ imm = #0x1c0
  3455dc: e3520004     	cmp	r2, #4
  3455e0: 0a000020     	beq	0x345668 <ObjectManager::InitPost()+0x13c> @ imm = #0x80
  3455e4: e593200c     	ldr	r2, [r3, #0xc]
  3455e8: e3520000     	cmp	r2, #0
  3455ec: 1a000001     	bne	0x3455f8 <ObjectManager::InitPost()+0xcc> @ imm = #0x4
  3455f0: ea00005d     	b	0x34576c <ObjectManager::InitPost()+0x240> @ imm = #0x174
  3455f4: e1a02003     	mov	r2, r3
  3455f8: e5923008     	ldr	r3, [r2, #0x8]
  3455fc: e3530000     	cmp	r3, #0
  345600: 1afffffb     	bne	0x3455f4 <ObjectManager::InitPost()+0xc8> @ imm = #-0x14
  345604: e1a03002     	mov	r3, r2
  345608: e59f22b0     	ldr	r2, [pc, #0x2b0]        @ 0x3458c0 <ObjectManager::InitPost()+0x394>
  34560c: e3a00000     	mov	r0, #0
  345610: e08f2002     	add	r2, pc, r2
  345614: e5823020     	str	r3, [r2, #0x20]
  345618: e28dd024     	add	sp, sp, #36
  34561c: e8bd80f0     	pop	{r4, r5, r6, r7, pc}
  345620: e285701c     	add	r7, r5, #28
  345624: e1a00007     	mov	r0, r7
  345628: ebff244f     	bl	0x30e76c <.plt+0x9f8>   @ imm = #-0x36ec4
  34562c: e3500000     	cmp	r0, #0
  345630: 0affffc5     	beq	0x34554c <ObjectManager::InitPost()+0x20> @ imm = #-0xec
  345634: e5856020     	str	r6, [r5, #0x20]
  345638: e1a00007     	mov	r0, r7
  34563c: ebff24fe     	bl	0x30ea3c <.plt+0xcc8>   @ imm = #-0x36c08
  345640: eaffffc1     	b	0x34554c <ObjectManager::InitPost()+0x20> @ imm = #-0xfc
  345644: e2857024     	add	r7, r5, #36
  345648: e1a00007     	mov	r0, r7
  34564c: ebff2446     	bl	0x30e76c <.plt+0x9f8>   @ imm = #-0x36ee8
  345650: e3500000     	cmp	r0, #0
  345654: 0affffc1     	beq	0x345560 <ObjectManager::InitPost()+0x34> @ imm = #-0xfc
  345658: e5856028     	str	r6, [r5, #0x28]
  34565c: e1a00007     	mov	r0, r7
  345660: ebff24f5     	bl	0x30ea3c <.plt+0xcc8>   @ imm = #-0x36c2c
  345664: eaffffbd     	b	0x345560 <ObjectManager::InitPost()+0x34> @ imm = #-0x10c
  345668: e593502c     	ldr	r5, [r3, #0x2c]
  34566c: e3550000     	cmp	r5, #0
  345670: 0affffdb     	beq	0x3455e4 <ObjectManager::InitPost()+0xb8> @ imm = #-0x94
  345674: e59f0248     	ldr	r0, [pc, #0x248]        @ 0x3458c4 <ObjectManager::InitPost()+0x398>
  345678: e595105c     	ldr	r1, [r5, #0x5c]
  34567c: e08f0000     	add	r0, pc, r0
  345680: ebff2418     	bl	0x30e6e8 <.plt+0x974>   @ imm = #-0x36fa0
  345684: e3500000     	cmp	r0, #0
  345688: 1a00006c     	bne	0x345840 <ObjectManager::InitPost()+0x314> @ imm = #0x1b0
  34568c: e3a0300c     	mov	r3, #12
  345690: e28d0020     	add	r0, sp, #32
  345694: e5203004     	str	r3, [r0, #-0x4]!
  345698: eb0f0e08     	bl	0x708ec0 <___ZNSt12__node_alloc11_M_allocateERj_veneer> @ imm = #0x3c3820
  34569c: e5805008     	str	r5, [r0, #0x8]
  3456a0: e5943028     	ldr	r3, [r4, #0x28]
  3456a4: e2842024     	add	r2, r4, #36
  3456a8: e880000c     	stm	r0, {r2, r3}
  3456ac: e5830000     	str	r0, [r3]
  3456b0: e5840028     	str	r0, [r4, #0x28]
  3456b4: e1a00005     	mov	r0, r5
  3456b8: eb014561     	bl	0x396c44 <RoomZone::InitObjectList()> @ imm = #0x51584
  3456bc: e5d530ac     	ldrb	r3, [r5, #0xac]
  3456c0: e3530000     	cmp	r3, #0
  3456c4: 1a000053     	bne	0x345818 <ObjectManager::InitPost()+0x2ec> @ imm = #0x14c
  3456c8: e59530a8     	ldr	r3, [r5, #0xa8]
  3456cc: e3530000     	cmp	r3, #0
  3456d0: 0a000050     	beq	0x345818 <ObjectManager::InitPost()+0x2ec> @ imm = #0x140
  3456d4: e2846044     	add	r6, r4, #68
  3456d8: e1a00006     	mov	r0, r6
  3456dc: ebfff6a1     	bl	0x343168 <std::allocator<std::priv::_List_node<ObjectBase*>>::allocate(unsigned int, void const*) (.clone.20)> @ imm = #-0x257c
  3456e0: e5805008     	str	r5, [r0, #0x8]
  3456e4: e5942048     	ldr	r2, [r4, #0x48]
  3456e8: e59f31d8     	ldr	r3, [pc, #0x1d8]        @ 0x3458c8 <ObjectManager::InitPost()+0x39c>
  3456ec: e5806000     	str	r6, [r0]
  3456f0: e5802004     	str	r2, [r0, #0x4]
  3456f4: e08f3003     	add	r3, pc, r3
  3456f8: e5820000     	str	r0, [r2]
  3456fc: e5840048     	str	r0, [r4, #0x48]
  345700: e5933020     	ldr	r3, [r3, #0x20]
  345704: eaffffb6     	b	0x3455e4 <ObjectManager::InitPost()+0xb8> @ imm = #-0x128
  345708: e2822001     	add	r2, r2, #1
  34570c: e3520004     	cmp	r2, #4
  345710: e584207c     	str	r2, [r4, #0x7c]
  345714: 13a00001     	movne	r0, #1
  345718: 1affffbe     	bne	0x345618 <ObjectManager::InitPost()+0xec> @ imm = #-0x108
  34571c: e5943014     	ldr	r3, [r4, #0x14]
  345720: e284002c     	add	r0, r4, #44
  345724: e5853020     	str	r3, [r5, #0x20]
  345728: ebfffecf     	bl	0x34526c <std::priv::_List_base<ObjectBase*, std::allocator<ObjectBase*>>::clear()> @ imm = #-0x4c4
  34572c: e2840044     	add	r0, r4, #68
  345730: ebfffecd     	bl	0x34526c <std::priv::_List_base<ObjectBase*, std::allocator<ObjectBase*>>::clear()> @ imm = #-0x4cc
  345734: e2840034     	add	r0, r4, #52
  345738: ebfffecb     	bl	0x34526c <std::priv::_List_base<ObjectBase*, std::allocator<ObjectBase*>>::clear()> @ imm = #-0x4d4
  34573c: e3a00000     	mov	r0, #0
  345740: eaffffb4     	b	0x345618 <ObjectManager::InitPost()+0xec> @ imm = #-0x130
  345744: e3520001     	cmp	r2, #1
  345748: 1affff9b     	bne	0x3455bc <ObjectManager::InitPost()+0x90> @ imm = #-0x194
  34574c: e5943014     	ldr	r3, [r4, #0x14]
  345750: e3a02002     	mov	r2, #2
  345754: e584207c     	str	r2, [r4, #0x7c]
  345758: e5853020     	str	r3, [r5, #0x20]
  34575c: e594207c     	ldr	r2, [r4, #0x7c]
  345760: e2822001     	add	r2, r2, #1
  345764: e584207c     	str	r2, [r4, #0x7c]
  345768: eaffff93     	b	0x3455bc <ObjectManager::InitPost()+0x90> @ imm = #-0x1b4
  34576c: e5931004     	ldr	r1, [r3, #0x4]
  345770: e591000c     	ldr	r0, [r1, #0xc]
  345774: e1500003     	cmp	r0, r3
  345778: 1a000005     	bne	0x345794 <ObjectManager::InitPost()+0x268> @ imm = #0x14
  34577c: e1a03001     	mov	r3, r1
  345780: e5911004     	ldr	r1, [r1, #0x4]
  345784: e591200c     	ldr	r2, [r1, #0xc]
  345788: e1530002     	cmp	r3, r2
  34578c: 0afffffa     	beq	0x34577c <ObjectManager::InitPost()+0x250> @ imm = #-0x18
  345790: e593200c     	ldr	r2, [r3, #0xc]
  345794: e1510002     	cmp	r1, r2
  345798: 11a03001     	movne	r3, r1
  34579c: eaffff99     	b	0x345608 <ObjectManager::InitPost()+0xdc> @ imm = #-0x19c
  3457a0: e28d4004     	add	r4, sp, #4
  3457a4: e593102c     	ldr	r1, [r3, #0x2c]
  3457a8: e1a00004     	mov	r0, r4
  3457ac: ebffe75c     	bl	0x33f524 <ObjectHandle::ObjectHandle(ObjectBase*)> @ imm = #-0x6290
  3457b0: e59d2008     	ldr	r2, [sp, #0x8]
  3457b4: e2866004     	add	r6, r6, #4
  3457b8: e59d3004     	ldr	r3, [sp, #0x4]
  3457bc: e4862004     	str	r2, [r6], #4
  3457c0: e5942008     	ldr	r2, [r4, #0x8]
  3457c4: e28d4010     	add	r4, sp, #16
  3457c8: e1a00004     	mov	r0, r4
  3457cc: e3a01000     	mov	r1, #0
  3457d0: e5862000     	str	r2, [r6]
  3457d4: e58d3010     	str	r3, [sp, #0x10]
  3457d8: ebffe978     	bl	0x33fdc0 <ObjectHandle::GetObject(bool)> @ imm = #-0x5a20
  3457dc: e3500000     	cmp	r0, #0
  3457e0: 0a00000a     	beq	0x345810 <ObjectManager::InitPost()+0x2e4> @ imm = #0x28
  3457e4: e3a01001     	mov	r1, #1
  3457e8: e1a00004     	mov	r0, r4
  3457ec: ebffe973     	bl	0x33fdc0 <ObjectHandle::GetObject(bool)> @ imm = #-0x5a34
  3457f0: e5903000     	ldr	r3, [r0]
  3457f4: e1a0e00f     	mov	lr, pc
  3457f8: e593f01c     	ldr	pc, [r3, #0x1c]
  3457fc: e3a01001     	mov	r1, #1
  345800: e1a00004     	mov	r0, r4
  345804: ebffe96d     	bl	0x33fdc0 <ObjectHandle::GetObject(bool)> @ imm = #-0x5a4c
  345808: e3a01000     	mov	r1, #0
  34580c: ebffe3b0     	bl	0x33e6d4 <ObjectBase::TestEnableCondition(bool)> @ imm = #-0x7140
  345810: e5953020     	ldr	r3, [r5, #0x20]
  345814: eaffff72     	b	0x3455e4 <ObjectManager::InitPost()+0xb8> @ imm = #-0x238
  345818: e5d530d0     	ldrb	r3, [r5, #0xd0]
  34581c: e3530000     	cmp	r3, #0
  345820: 1a00001d     	bne	0x34589c <ObjectManager::InitPost()+0x370> @ imm = #0x74
  345824: e59530cc     	ldr	r3, [r5, #0xcc]
  345828: e3530000     	cmp	r3, #0
  34582c: 1affffa8     	bne	0x3456d4 <ObjectManager::InitPost()+0x1a8> @ imm = #-0x160
  345830: e59f3094     	ldr	r3, [pc, #0x94]         @ 0x3458cc <ObjectManager::InitPost()+0x3a0>
  345834: e08f3003     	add	r3, pc, r3
  345838: e5933020     	ldr	r3, [r3, #0x20]
  34583c: eaffff68     	b	0x3455e4 <ObjectManager::InitPost()+0xb8> @ imm = #-0x260
  345840: e5953000     	ldr	r3, [r5]
  345844: e1a00005     	mov	r0, r5
  345848: e1a0e00f     	mov	lr, pc
  34584c: e593f038     	ldr	pc, [r3, #0x38]
  345850: e3500000     	cmp	r0, #0
  345854: 0affff98     	beq	0x3456bc <ObjectManager::InitPost()+0x190> @ imm = #-0x1a0
  345858: e284602c     	add	r6, r4, #44
  34585c: e1a00006     	mov	r0, r6
  345860: ebfff640     	bl	0x343168 <std::allocator<std::priv::_List_node<ObjectBase*>>::allocate(unsigned int, void const*) (.clone.20)> @ imm = #-0x2700
  345864: e5805008     	str	r5, [r0, #0x8]
  345868: e5943030     	ldr	r3, [r4, #0x30]
  34586c: e5806000     	str	r6, [r0]
  345870: e5803004     	str	r3, [r0, #0x4]
  345874: e5830000     	str	r0, [r3]
  345878: e5840030     	str	r0, [r4, #0x30]
  34587c: eaffff8e     	b	0x3456bc <ObjectManager::InitPost()+0x190> @ imm = #-0x1c8
  345880: e5930008     	ldr	r0, [r3, #0x8]
  345884: eb011400     	bl	0x38a88c <Module::LoadModule() const> @ imm = #0x45000
  345888: e5953028     	ldr	r3, [r5, #0x28]
  34588c: e5933000     	ldr	r3, [r3]
  345890: e5853028     	str	r3, [r5, #0x28]
  345894: e594207c     	ldr	r2, [r4, #0x7c]
  345898: eaffff47     	b	0x3455bc <ObjectManager::InitPost()+0x90> @ imm = #-0x2e4
  34589c: e59f302c     	ldr	r3, [pc, #0x2c]         @ 0x3458d0 <ObjectManager::InitPost()+0x3a4>
  3458a0: e08f3003     	add	r3, pc, r3
  3458a4: e5933020     	ldr	r3, [r3, #0x20]
  3458a8: eaffff4d     	b	0x3455e4 <ObjectManager::InitPost()+0xb8> @ imm = #-0x2cc
  3458ac: 00 c9 65 00  	.word	0x0065c900
  3458b0: ec c8 65 00  	.word	0x0065c8ec
  3458b4: bc c8 65 00  	.word	0x0065c8bc
  3458b8: 98 c8 65 00  	.word	0x0065c898
  3458bc: 78 c8 65 00  	.word	0x0065c878
  3458c0: 2c c8 65 00  	.word	0x0065c82c
  3458c4: f4 ac 57 00  	.word	0x0057acf4
  3458c8: 48 c7 65 00  	.word	0x0065c748
  3458cc: 08 c6 65 00  	.word	0x0065c608
  3458d0: 9c c5 65 00  	.word	0x0065c59c
