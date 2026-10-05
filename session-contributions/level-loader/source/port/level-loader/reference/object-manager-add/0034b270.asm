
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0034b270 <ObjectManager::Add(ObjectBase*, char const*, char const*, int, bool)>:
  34b270: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  34b274: e59f427c     	ldr	r4, [pc, #0x27c]        @ 0x34b4f8 <ObjectManager::Add(ObjectBase*, char const*, char const*, int, bool)+0x288>
  34b278: e24dd02c     	sub	sp, sp, #44
  34b27c: e2526000     	subs	r6, r2, #0
  34b280: e08f4004     	add	r4, pc, r4
  34b284: e1a05000     	mov	r5, r0
  34b288: e1a08001     	mov	r8, r1
  34b28c: e1a07003     	mov	r7, r3
  34b290: e59db050     	ldr	r11, [sp, #0x50]
  34b294: e59da054     	ldr	r10, [sp, #0x54]
  34b298: e5dd9058     	ldrb	r9, [sp, #0x58]
  34b29c: 0a000018     	beq	0x34b304 <ObjectManager::Add(ObjectBase*, char const*, char const*, int, bool)+0x94> @ imm = #0x60
  34b2a0: e3570000     	cmp	r7, #0
  34b2a4: 0a00002b     	beq	0x34b358 <ObjectManager::Add(ObjectBase*, char const*, char const*, int, bool)+0xe8> @ imm = #0xac
  34b2a8: e3a04000     	mov	r4, #0
  34b2ac: e3a0c001     	mov	r12, #1
  34b2b0: e1a02007     	mov	r2, r7
  34b2b4: e1a0300a     	mov	r3, r10
  34b2b8: e1a00005     	mov	r0, r5
  34b2bc: e1a01008     	mov	r1, r8
  34b2c0: e58dc000     	str	r12, [sp]
  34b2c4: e58d4004     	str	r4, [sp, #0x4]
  34b2c8: ebfffe74     	bl	0x34aca0 <ObjectManager::GetObjectByName(char const*, int, bool, char const*)> @ imm = #-0x630
  34b2cc: e1a00005     	mov	r0, r5
  34b2d0: e1a01004     	mov	r1, r4
  34b2d4: ebffd2b9     	bl	0x33fdc0 <ObjectHandle::GetObject(bool)> @ imm = #-0xb51c
  34b2d8: e1500004     	cmp	r0, r4
  34b2dc: 0a000032     	beq	0x34b3ac <ObjectManager::Add(ObjectBase*, char const*, char const*, int, bool)+0x13c> @ imm = #0xc8
  34b2e0: e1560004     	cmp	r6, r4
  34b2e4: 0a000003     	beq	0x34b2f8 <ObjectManager::Add(ObjectBase*, char const*, char const*, int, bool)+0x88> @ imm = #0xc
  34b2e8: e1a00006     	mov	r0, r6
  34b2ec: e5963000     	ldr	r3, [r6]
  34b2f0: e1a0e00f     	mov	lr, pc
  34b2f4: e593f004     	ldr	pc, [r3, #0x4]
  34b2f8: e1a00005     	mov	r0, r5
  34b2fc: e28dd02c     	add	sp, sp, #44
  34b300: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  34b304: e59f31f0     	ldr	r3, [pc, #0x1f0]        @ 0x34b4fc <ObjectManager::Add(ObjectBase*, char const*, char const*, int, bool)+0x28c>
  34b308: e7943003     	ldr	r3, [r4, r3]
  34b30c: e5933000     	ldr	r3, [r3]
  34b310: e3530002     	cmp	r3, #2
  34b314: 05866000     	streq	r6, [r6]
  34b318: 0affffe0     	beq	0x34b2a0 <ObjectManager::Add(ObjectBase*, char const*, char const*, int, bool)+0x30> @ imm = #-0x80
  34b31c: e3530001     	cmp	r3, #1
  34b320: 1affffde     	bne	0x34b2a0 <ObjectManager::Add(ObjectBase*, char const*, char const*, int, bool)+0x30> @ imm = #-0x88
  34b324: e59f01d4     	ldr	r0, [pc, #0x1d4]        @ 0x34b500 <ObjectManager::Add(ObjectBase*, char const*, char const*, int, bool)+0x290>
  34b328: e59f11d4     	ldr	r1, [pc, #0x1d4]        @ 0x34b504 <ObjectManager::Add(ObjectBase*, char const*, char const*, int, bool)+0x294>
  34b32c: e59f21d4     	ldr	r2, [pc, #0x1d4]        @ 0x34b508 <ObjectManager::Add(ObjectBase*, char const*, char const*, int, bool)+0x298>
  34b330: e7940000     	ldr	r0, [r4, r0]
  34b334: e59f31d0     	ldr	r3, [pc, #0x1d0]        @ 0x34b50c <ObjectManager::Add(ObjectBase*, char const*, char const*, int, bool)+0x29c>
  34b338: e300c428     	movw	r12, #0x428
  34b33c: e08f1001     	add	r1, pc, r1
  34b340: e08f2002     	add	r2, pc, r2
  34b344: e08f3003     	add	r3, pc, r3
  34b348: e28000a8     	add	r0, r0, #168
  34b34c: e58dc000     	str	r12, [sp]
  34b350: ebff0b2b     	bl	0x30e004 <.plt+0x290>   @ imm = #-0x3d354
  34b354: eaffffd1     	b	0x34b2a0 <ObjectManager::Add(ObjectBase*, char const*, char const*, int, bool)+0x30> @ imm = #-0xbc
  34b358: e59f319c     	ldr	r3, [pc, #0x19c]        @ 0x34b4fc <ObjectManager::Add(ObjectBase*, char const*, char const*, int, bool)+0x28c>
  34b35c: e7943003     	ldr	r3, [r4, r3]
  34b360: e5933000     	ldr	r3, [r3]
  34b364: e3530002     	cmp	r3, #2
  34b368: 05877000     	streq	r7, [r7]
  34b36c: 0affffcd     	beq	0x34b2a8 <ObjectManager::Add(ObjectBase*, char const*, char const*, int, bool)+0x38> @ imm = #-0xcc
  34b370: e3530001     	cmp	r3, #1
  34b374: 1affffcb     	bne	0x34b2a8 <ObjectManager::Add(ObjectBase*, char const*, char const*, int, bool)+0x38> @ imm = #-0xd4
  34b378: e59f0180     	ldr	r0, [pc, #0x180]        @ 0x34b500 <ObjectManager::Add(ObjectBase*, char const*, char const*, int, bool)+0x290>
  34b37c: e59f118c     	ldr	r1, [pc, #0x18c]        @ 0x34b510 <ObjectManager::Add(ObjectBase*, char const*, char const*, int, bool)+0x2a0>
  34b380: e59f218c     	ldr	r2, [pc, #0x18c]        @ 0x34b514 <ObjectManager::Add(ObjectBase*, char const*, char const*, int, bool)+0x2a4>
  34b384: e7940000     	ldr	r0, [r4, r0]
  34b388: e59f3188     	ldr	r3, [pc, #0x188]        @ 0x34b518 <ObjectManager::Add(ObjectBase*, char const*, char const*, int, bool)+0x2a8>
  34b38c: e300c429     	movw	r12, #0x429
  34b390: e08f1001     	add	r1, pc, r1
  34b394: e08f2002     	add	r2, pc, r2
  34b398: e08f3003     	add	r3, pc, r3
  34b39c: e28000a8     	add	r0, r0, #168
  34b3a0: e58dc000     	str	r12, [sp]
  34b3a4: ebff0b16     	bl	0x30e004 <.plt+0x290>   @ imm = #-0x3d3a8
  34b3a8: eaffffbe     	b	0x34b2a8 <ObjectManager::Add(ObjectBase*, char const*, char const*, int, bool)+0x38> @ imm = #-0x108
  34b3ac: e1a01005     	mov	r1, r5
  34b3b0: e288000c     	add	r0, r8, #12
  34b3b4: ebffd233     	bl	0x33fc88 <ObjectListItem& std::map<int, ObjectListItem, std::less<int>, std::allocator<std::pair<int const, ObjectListItem>>>::operator[]<unsigned int>(unsigned int const&)> @ imm = #-0xb734
  34b3b8: e5806018     	str	r6, [r0, #0x18]
  34b3bc: e5983050     	ldr	r3, [r8, #0x50]
  34b3c0: e1a02005     	mov	r2, r5
  34b3c4: e1a01007     	mov	r1, r7
  34b3c8: e2833001     	add	r3, r3, #1
  34b3cc: e5883050     	str	r3, [r8, #0x50]
  34b3d0: e5856004     	str	r6, [r5, #0x4]
  34b3d4: e596c02c     	ldr	r12, [r6, #0x2c]
  34b3d8: e492e004     	ldr	lr, [r2], #4
  34b3dc: e1a00006     	mov	r0, r6
  34b3e0: e1a0300c     	mov	r3, r12
  34b3e4: e483e004     	str	lr, [r3], #4
  34b3e8: e595e004     	ldr	lr, [r5, #0x4]
  34b3ec: e28d4018     	add	r4, sp, #24
  34b3f0: e58ce004     	str	lr, [r12, #0x4]
  34b3f4: e5922004     	ldr	r2, [r2, #0x4]
  34b3f8: e5832004     	str	r2, [r3, #0x4]
  34b3fc: ebfffe05     	bl	0x34ac18 <ObjectBase::SetName(char const*)> @ imm = #-0x7ec
  34b400: e1a0000b     	mov	r0, r11
  34b404: ebff0a92     	bl	0x30de54 <.plt+0xe0>    @ imm = #-0x3d5b8
  34b408: e1a0100b     	mov	r1, r11
  34b40c: e08b2000     	add	r2, r11, r0
  34b410: e2860048     	add	r0, r6, #72
  34b414: ebff1571     	bl	0x3109e0 <std::string::_M_assign(char const*, char const*)> @ imm = #-0x3aa3c
  34b418: e1a01006     	mov	r1, r6
  34b41c: e1a00004     	mov	r0, r4
  34b420: e586a064     	str	r10, [r6, #0x64]
  34b424: ebffca40     	bl	0x33dd2c <ObjectBase::GetHandle()> @ imm = #-0xd700
  34b428: e1a00004     	mov	r0, r4
  34b42c: ebffd2c8     	bl	0x33ff54 <ObjectHandle::operator Character*()> @ imm = #-0xb4e0
  34b430: e2507000     	subs	r7, r0, #0
  34b434: 0a000008     	beq	0x34b45c <ObjectManager::Add(ObjectBase*, char const*, char const*, int, bool)+0x1ec> @ imm = #0x20
  34b438: e2884060     	add	r4, r8, #96
  34b43c: e1a00004     	mov	r0, r4
  34b440: ebffdc92     	bl	0x342690 <std::allocator<std::priv::_List_node<Character*>>::allocate(unsigned int, void const*) (.clone.13)> @ imm = #-0x8db8
  34b444: e5807008     	str	r7, [r0, #0x8]
  34b448: e5983064     	ldr	r3, [r8, #0x64]
  34b44c: e5804000     	str	r4, [r0]
  34b450: e5803004     	str	r3, [r0, #0x4]
  34b454: e5830000     	str	r0, [r3]
  34b458: e5880064     	str	r0, [r8, #0x64]
  34b45c: e28d400c     	add	r4, sp, #12
  34b460: e1a00004     	mov	r0, r4
  34b464: e1a01006     	mov	r1, r6
  34b468: ebffca2f     	bl	0x33dd2c <ObjectBase::GetHandle()> @ imm = #-0xd744
  34b46c: e1a00004     	mov	r0, r4
  34b470: e3a01000     	mov	r1, #0
  34b474: ebffd251     	bl	0x33fdc0 <ObjectHandle::GetObject(bool)> @ imm = #-0xb6bc
  34b478: e2504000     	subs	r4, r0, #0
  34b47c: 0a000002     	beq	0x34b48c <ObjectManager::Add(ObjectBase*, char const*, char const*, int, bool)+0x21c> @ imm = #0x8
  34b480: e59430f4     	ldr	r3, [r4, #0xf4]
  34b484: e3530005     	cmp	r3, #5
  34b488: 0a000005     	beq	0x34b4a4 <ObjectManager::Add(ObjectBase*, char const*, char const*, int, bool)+0x234> @ imm = #0x14
  34b48c: e3590000     	cmp	r9, #0
  34b490: 0affff98     	beq	0x34b2f8 <ObjectManager::Add(ObjectBase*, char const*, char const*, int, bool)+0x88> @ imm = #-0x1a0
  34b494: e1a00008     	mov	r0, r8
  34b498: e1a01006     	mov	r1, r6
  34b49c: ebffdf47     	bl	0x3431c0 <ObjectManager::AssignObjectNetworkId(ObjectBase*)> @ imm = #-0x82e4
  34b4a0: eaffff94     	b	0x34b2f8 <ObjectManager::Add(ObjectBase*, char const*, char const*, int, bool)+0x88> @ imm = #-0x1b0
  34b4a4: e596005c     	ldr	r0, [r6, #0x5c]
  34b4a8: e5962058     	ldr	r2, [r6, #0x58]
  34b4ac: e0602002     	rsb	r2, r0, r2
  34b4b0: e3520006     	cmp	r2, #6
  34b4b4: 1afffff4     	bne	0x34b48c <ObjectManager::Add(ObjectBase*, char const*, char const*, int, bool)+0x21c> @ imm = #-0x30
  34b4b8: e59f105c     	ldr	r1, [pc, #0x5c]         @ 0x34b51c <ObjectManager::Add(ObjectBase*, char const*, char const*, int, bool)+0x2ac>
  34b4bc: e08f1001     	add	r1, pc, r1
  34b4c0: ebff0c46     	bl	0x30e5e0 <.plt+0x86c>   @ imm = #-0x3cee8
  34b4c4: e3500000     	cmp	r0, #0
  34b4c8: 1affffef     	bne	0x34b48c <ObjectManager::Add(ObjectBase*, char const*, char const*, int, bool)+0x21c> @ imm = #-0x44
  34b4cc: e3a0300c     	mov	r3, #12
  34b4d0: e28d0028     	add	r0, sp, #40
  34b4d4: e5203004     	str	r3, [r0, #-0x4]!
  34b4d8: eb0ef678     	bl	0x708ec0 <___ZNSt12__node_alloc11_M_allocateERj_veneer> @ imm = #0x3bd9e0
  34b4dc: e5804008     	str	r4, [r0, #0x8]
  34b4e0: e598306c     	ldr	r3, [r8, #0x6c]
  34b4e4: e2882068     	add	r2, r8, #104
  34b4e8: e880000c     	stm	r0, {r2, r3}
  34b4ec: e5830000     	str	r0, [r3]
  34b4f0: e588006c     	str	r0, [r8, #0x6c]
  34b4f4: eaffffe4     	b	0x34b48c <ObjectManager::Add(ObjectBase*, char const*, char const*, int, bool)+0x21c> @ imm = #-0x70
  34b4f8: 10 98 64 00  	.word	0x00649810
  34b4fc: c0 39 00 00  	.word	0x000039c0
  34b500: c0 19 00 00  	.word	0x000019c0
  34b504: 9c 30 57 00  	.word	0x0057309c
  34b508: d0 4e 57 00  	.word	0x00574ed0
  34b50c: 54 4f 57 00  	.word	0x00574f54
  34b510: 48 30 57 00  	.word	0x00573048
  34b514: 54 5d 59 00  	.word	0x00595d54
  34b518: 00 4f 57 00  	.word	0x00574f00
  34b51c: 74 4f 57 00  	.word	0x00574f74
