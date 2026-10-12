
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0038a88c <Module::LoadModule() const>:
  38a88c: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  38a890: e59f91fc     	ldr	r9, [pc, #0x1fc]        @ 0x38aa94 <Module::LoadModule() const+0x208>
  38a894: e59f31fc     	ldr	r3, [pc, #0x1fc]        @ 0x38aa98 <Module::LoadModule() const+0x20c>
  38a898: e59fb1fc     	ldr	r11, [pc, #0x1fc]       @ 0x38aa9c <Module::LoadModule() const+0x210>
  38a89c: e08f9009     	add	r9, pc, r9
  38a8a0: e7992003     	ldr	r2, [r9, r3]
  38a8a4: e799300b     	ldr	r3, [r9, r11]
  38a8a8: e24dd084     	sub	sp, sp, #132
  38a8ac: e5924000     	ldr	r4, [r2]
  38a8b0: e5933000     	ldr	r3, [r3]
  38a8b4: e1a05000     	mov	r5, r0
  38a8b8: e3540000     	cmp	r4, #0
  38a8bc: e58d307c     	str	r3, [sp, #0x7c]
  38a8c0: 0a00005d     	beq	0x38aa3c <Module::LoadModule() const+0x1b0> @ imm = #0x174
  38a8c4: e1a00004     	mov	r0, r4
  38a8c8: e595140c     	ldr	r1, [r5, #0x40c]
  38a8cc: eb019269     	bl	0x3ef278 <Level::SetObjectModuleId(int)> @ imm = #0x649a4
  38a8d0: e5953160     	ldr	r3, [r5, #0x160]
  38a8d4: e28d6064     	add	r6, sp, #100
  38a8d8: e1a00006     	mov	r0, r6
  38a8dc: e5843160     	str	r3, [r4, #0x160]
  38a8e0: e5953164     	ldr	r3, [r5, #0x164]
  38a8e4: e3a01010     	mov	r1, #16
  38a8e8: e28d704c     	add	r7, sp, #76
  38a8ec: e5843164     	str	r3, [r4, #0x164]
  38a8f0: e5953168     	ldr	r3, [r5, #0x168]
  38a8f4: e3a08000     	mov	r8, #0
  38a8f8: e5843168     	str	r3, [r4, #0x168]
  38a8fc: e58d6074     	str	r6, [sp, #0x74]
  38a900: e58d6078     	str	r6, [sp, #0x78]
  38a904: ebfe1b5c     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x79290
  38a908: e59d3074     	ldr	r3, [sp, #0x74]
  38a90c: e1a00007     	mov	r0, r7
  38a910: e3a01010     	mov	r1, #16
  38a914: e5c38000     	strb	r8, [r3]
  38a918: e58d705c     	str	r7, [sp, #0x5c]
  38a91c: e58d7060     	str	r7, [sp, #0x60]
  38a920: ebfe1b55     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x792ac
  38a924: e59d305c     	ldr	r3, [sp, #0x5c]
  38a928: e1a02007     	mov	r2, r7
  38a92c: e1a00005     	mov	r0, r5
  38a930: e5c38000     	strb	r8, [r3]
  38a934: e1a01006     	mov	r1, r6
  38a938: ebfffe93     	bl	0x38a38c <Module::_ChooseXmls(std::string&, std::string&) const> @ imm = #-0x5b4
  38a93c: e59d3074     	ldr	r3, [sp, #0x74]
  38a940: e59d2078     	ldr	r2, [sp, #0x78]
  38a944: e1520003     	cmp	r2, r3
  38a948: 0a000012     	beq	0x38a998 <Module::LoadModule() const+0x10c> @ imm = #0x48
  38a94c: e59f814c     	ldr	r8, [pc, #0x14c]        @ 0x38aaa0 <Module::LoadModule() const+0x214>
  38a950: e28d5034     	add	r5, sp, #52
  38a954: e28da018     	add	r10, sp, #24
  38a958: e08f8008     	add	r8, pc, r8
  38a95c: e1a01008     	mov	r1, r8
  38a960: e1a0200a     	mov	r2, r10
  38a964: e1a00005     	mov	r0, r5
  38a968: ebfe25df     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0x76884
  38a96c: e1a01006     	mov	r1, r6
  38a970: e1a02005     	mov	r2, r5
  38a974: e1a00004     	mov	r0, r4
  38a978: eb01a470     	bl	0x3f3b40 <Level::LoadFile(std::string const&, std::string const&)> @ imm = #0x691c0
  38a97c: e1a03000     	mov	r3, r0
  38a980: e1a00005     	mov	r0, r5
  38a984: e58d300c     	str	r3, [sp, #0xc]
  38a988: ebfe2407     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0x76fe4
  38a98c: e59d300c     	ldr	r3, [sp, #0xc]
  38a990: e3530000     	cmp	r3, #0
  38a994: 0afffff0     	beq	0x38a95c <Module::LoadModule() const+0xd0> @ imm = #-0x40
  38a998: e59d305c     	ldr	r3, [sp, #0x5c]
  38a99c: e59d2060     	ldr	r2, [sp, #0x60]
  38a9a0: e1520003     	cmp	r2, r3
  38a9a4: 0a000012     	beq	0x38a9f4 <Module::LoadModule() const+0x168> @ imm = #0x48
  38a9a8: e59f80f4     	ldr	r8, [pc, #0xf4]         @ 0x38aaa4 <Module::LoadModule() const+0x218>
  38a9ac: e28d501c     	add	r5, sp, #28
  38a9b0: e28da014     	add	r10, sp, #20
  38a9b4: e08f8008     	add	r8, pc, r8
  38a9b8: e1a01008     	mov	r1, r8
  38a9bc: e1a0200a     	mov	r2, r10
  38a9c0: e1a00005     	mov	r0, r5
  38a9c4: ebfe25c8     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0x768e0
  38a9c8: e1a01007     	mov	r1, r7
  38a9cc: e1a02005     	mov	r2, r5
  38a9d0: e1a00004     	mov	r0, r4
  38a9d4: eb01a459     	bl	0x3f3b40 <Level::LoadFile(std::string const&, std::string const&)> @ imm = #0x69164
  38a9d8: e1a03000     	mov	r3, r0
  38a9dc: e1a00005     	mov	r0, r5
  38a9e0: e58d300c     	str	r3, [sp, #0xc]
  38a9e4: ebfe23f0     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0x77040
  38a9e8: e59d300c     	ldr	r3, [sp, #0xc]
  38a9ec: e3530000     	cmp	r3, #0
  38a9f0: 0afffff0     	beq	0x38a9b8 <Module::LoadModule() const+0x12c> @ imm = #-0x40
  38a9f4: e3a03000     	mov	r3, #0
  38a9f8: e5843168     	str	r3, [r4, #0x168]
  38a9fc: e5843160     	str	r3, [r4, #0x160]
  38aa00: e5843164     	str	r3, [r4, #0x164]
  38aa04: e3e01000     	mvn	r1, #0
  38aa08: e1a00004     	mov	r0, r4
  38aa0c: eb019219     	bl	0x3ef278 <Level::SetObjectModuleId(int)> @ imm = #0x64864
  38aa10: e1a00007     	mov	r0, r7
  38aa14: ebfe23e4     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0x77070
  38aa18: e1a00006     	mov	r0, r6
  38aa1c: ebfe23e2     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0x77078
  38aa20: e799300b     	ldr	r3, [r9, r11]
  38aa24: e59d207c     	ldr	r2, [sp, #0x7c]
  38aa28: e5933000     	ldr	r3, [r3]
  38aa2c: e1520003     	cmp	r2, r3
  38aa30: 1a000016     	bne	0x38aa90 <Module::LoadModule() const+0x204> @ imm = #0x58
  38aa34: e28dd084     	add	sp, sp, #132
  38aa38: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  38aa3c: e59f3064     	ldr	r3, [pc, #0x64]         @ 0x38aaa8 <Module::LoadModule() const+0x21c>
  38aa40: e7993003     	ldr	r3, [r9, r3]
  38aa44: e5933000     	ldr	r3, [r3]
  38aa48: e3530002     	cmp	r3, #2
  38aa4c: 05844000     	streq	r4, [r4]
  38aa50: 0affff9b     	beq	0x38a8c4 <Module::LoadModule() const+0x38> @ imm = #-0x194
  38aa54: e3530001     	cmp	r3, #1
  38aa58: 1affff99     	bne	0x38a8c4 <Module::LoadModule() const+0x38> @ imm = #-0x19c
  38aa5c: e59f0048     	ldr	r0, [pc, #0x48]         @ 0x38aaac <Module::LoadModule() const+0x220>
  38aa60: e59f1048     	ldr	r1, [pc, #0x48]         @ 0x38aab0 <Module::LoadModule() const+0x224>
  38aa64: e59f2048     	ldr	r2, [pc, #0x48]         @ 0x38aab4 <Module::LoadModule() const+0x228>
  38aa68: e7990000     	ldr	r0, [r9, r0]
  38aa6c: e59f3044     	ldr	r3, [pc, #0x44]         @ 0x38aab8 <Module::LoadModule() const+0x22c>
  38aa70: e3a0c054     	mov	r12, #84
  38aa74: e08f1001     	add	r1, pc, r1
  38aa78: e08f2002     	add	r2, pc, r2
  38aa7c: e08f3003     	add	r3, pc, r3
  38aa80: e28000a8     	add	r0, r0, #168
  38aa84: e58dc000     	str	r12, [sp]
  38aa88: ebfe0d5d     	bl	0x30e004 <.plt+0x290>   @ imm = #-0x7ca8c
  38aa8c: eaffff8c     	b	0x38a8c4 <Module::LoadModule() const+0x38> @ imm = #-0x1d0
  38aa90: ebfe0e1e     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0x7c788
  38aa94: f4 a1 60 00  	.word	0x0060a1f4
  38aa98: 64 1d 00 00  	.word	0x00001d64
  38aa9c: ac 40 00 00  	.word	0x000040ac
  38aaa0: d8 5a 53 00  	.word	0x00535ad8
  38aaa4: 7c 5a 53 00  	.word	0x00535a7c
  38aaa8: c0 39 00 00  	.word	0x000039c0
  38aaac: c0 19 00 00  	.word	0x000019c0
  38aab0: 64 39 53 00  	.word	0x00533964
  38aab4: d8 78 53 00  	.word	0x005378d8
  38aab8: f4 77 53 00  	.word	0x005377f4

0038aabc <ObjectSearcher::IObjectList::~IObjectList()>:
  38aabc: e12fff1e     	bx	lr

0038aac0 <GameObject::IsUpdatable() const>:
  38aac0: e3a00001     	mov	r0, #1
  38aac4: e12fff1e     	bx	lr

0038aac8 <GameObject::UpdateAbsoluteAABB()>:
  38aac8: e92d47f0     	push	{r4, r5, r6, r7, r8, r9, r10, lr}
  38aacc: e1a04000     	mov	r4, r0
  38aad0: e594a148     	ldr	r10, [r4, #0x148]
  38aad4: e5900144     	ldr	r0, [r0, #0x144]
  38aad8: e594814c     	ldr	r8, [r4, #0x14c]
  38aadc: e5947150     	ldr	r7, [r4, #0x150]
  38aae0: e5946154     	ldr	r6, [r4, #0x154]
  38aae4: e5945158     	ldr	r5, [r4, #0x158]
  38aae8: e5941160     	ldr	r1, [r4, #0x160]
  38aaec: e584012c     	str	r0, [r4, #0x12c]
  38aaf0: e584a130     	str	r10, [r4, #0x130]
  38aaf4: e5848134     	str	r8, [r4, #0x134]
  38aaf8: e5847138     	str	r7, [r4, #0x138]
  38aafc: e584613c     	str	r6, [r4, #0x13c]
  38ab00: e5845140     	str	r5, [r4, #0x140]
  38ab04: ebfe1026     	bl	0x30eba4 <.plt+0xe30>   @ imm = #-0x7bf68
  38ab08: e5941164     	ldr	r1, [r4, #0x164]
  38ab0c: e584012c     	str	r0, [r4, #0x12c]
  38ab10: e1a0000a     	mov	r0, r10
  38ab14: ebfe1022     	bl	0x30eba4 <.plt+0xe30>   @ imm = #-0x7bf78
  38ab18: e5941168     	ldr	r1, [r4, #0x168]
  38ab1c: e5840130     	str	r0, [r4, #0x130]
  38ab20: e1a00008     	mov	r0, r8
  38ab24: ebfe101e     	bl	0x30eba4 <.plt+0xe30>   @ imm = #-0x7bf88
  38ab28: e5941160     	ldr	r1, [r4, #0x160]
  38ab2c: e5840134     	str	r0, [r4, #0x134]
  38ab30: e1a00007     	mov	r0, r7
  38ab34: ebfe101a     	bl	0x30eba4 <.plt+0xe30>   @ imm = #-0x7bf98
  38ab38: e5941164     	ldr	r1, [r4, #0x164]
  38ab3c: e5840138     	str	r0, [r4, #0x138]
  38ab40: e1a00006     	mov	r0, r6
  38ab44: ebfe1016     	bl	0x30eba4 <.plt+0xe30>   @ imm = #-0x7bfa8
  38ab48: e5941168     	ldr	r1, [r4, #0x168]
  38ab4c: e584013c     	str	r0, [r4, #0x13c]
  38ab50: e1a00005     	mov	r0, r5
  38ab54: ebfe1012     	bl	0x30eba4 <.plt+0xe30>   @ imm = #-0x7bfb8
  38ab58: e5840140     	str	r0, [r4, #0x140]
  38ab5c: e8bd87f0     	pop	{r4, r5, r6, r7, r8, r9, r10, pc}

0038ab60 <GameObject::MeetCondition() const>:
  38ab60: e3a00001     	mov	r0, #1
  38ab64: e12fff1e     	bx	lr

0038ab68 <GameObject::IsTouching(Point3D<float> const&) const>:
  38ab68: e92d4070     	push	{r4, r5, r6, lr}
  38ab6c: e5915000     	ldr	r5, [r1]
  38ab70: e1a06001     	mov	r6, r1
  38ab74: e1a04000     	mov	r4, r0
  38ab78: e1a01005     	mov	r1, r5
  38ab7c: e590012c     	ldr	r0, [r0, #0x12c]
  38ab80: ebfe0f89     	bl	0x30e9ac <.plt+0xc38>   @ imm = #-0x7c1dc
  38ab84: e3500000     	cmp	r0, #0
  38ab88: 0a00001d     	beq	0x38ac04 <GameObject::IsTouching(Point3D<float> const&) const+0x9c> @ imm = #0x74
  38ab8c: e1a00005     	mov	r0, r5
  38ab90: e5941138     	ldr	r1, [r4, #0x138]
  38ab94: ebfe0f84     	bl	0x30e9ac <.plt+0xc38>   @ imm = #-0x7c1f0
  38ab98: e3500000     	cmp	r0, #0
  38ab9c: 0a000018     	beq	0x38ac04 <GameObject::IsTouching(Point3D<float> const&) const+0x9c> @ imm = #0x60
  38aba0: e5965004     	ldr	r5, [r6, #0x4]
  38aba4: e5940130     	ldr	r0, [r4, #0x130]
  38aba8: e1a01005     	mov	r1, r5
  38abac: ebfe0f7e     	bl	0x30e9ac <.plt+0xc38>   @ imm = #-0x7c208
  38abb0: e3500000     	cmp	r0, #0
  38abb4: 0a000012     	beq	0x38ac04 <GameObject::IsTouching(Point3D<float> const&) const+0x9c> @ imm = #0x48
  38abb8: e1a00005     	mov	r0, r5
  38abbc: e594113c     	ldr	r1, [r4, #0x13c]
  38abc0: ebfe0f79     	bl	0x30e9ac <.plt+0xc38>   @ imm = #-0x7c21c
  38abc4: e3500000     	cmp	r0, #0
  38abc8: 0a00000d     	beq	0x38ac04 <GameObject::IsTouching(Point3D<float> const&) const+0x9c> @ imm = #0x34
  38abcc: e5965008     	ldr	r5, [r6, #0x8]
  38abd0: e5940134     	ldr	r0, [r4, #0x134]
  38abd4: e1a01005     	mov	r1, r5
  38abd8: ebfe0f73     	bl	0x30e9ac <.plt+0xc38>   @ imm = #-0x7c234
  38abdc: e3500000     	cmp	r0, #0
  38abe0: 0a000007     	beq	0x38ac04 <GameObject::IsTouching(Point3D<float> const&) const+0x9c> @ imm = #0x1c
  38abe4: e1a00005     	mov	r0, r5
  38abe8: e5941140     	ldr	r1, [r4, #0x140]
  38abec: ebfe0f6e     	bl	0x30e9ac <.plt+0xc38>   @ imm = #-0x7c248
  38abf0: e3500000     	cmp	r0, #0
  38abf4: e3a00000     	mov	r0, #0
  38abf8: 13a00001     	movne	r0, #1
  38abfc: e6ef0070     	uxtb	r0, r0
  38ac00: e8bd8070     	pop	{r4, r5, r6, pc}
  38ac04: e3a00000     	mov	r0, #0
  38ac08: e8bd8070     	pop	{r4, r5, r6, pc}

0038ac0c <GameObject::IsNearby(Point3D<float> const&, float) const>:
  38ac0c: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  38ac10: e1a03000     	mov	r3, r0
  38ac14: e590412c     	ldr	r4, [r0, #0x12c]
  38ac18: e1a05002     	mov	r5, r2
  38ac1c: e1a00002     	mov	r0, r2
  38ac20: e5932140     	ldr	r2, [r3, #0x140]
  38ac24: e24dd00c     	sub	sp, sp, #12
  38ac28: e1a07001     	mov	r7, r1
  38ac2c: e3a014c2     	mov	r1, #-1040187392
  38ac30: e58d2004     	str	r2, [sp, #0x4]
  38ac34: e2811732     	add	r1, r1, #13107200
  38ac38: e593a130     	ldr	r10, [r3, #0x130]
  38ac3c: e593b134     	ldr	r11, [r3, #0x134]
  38ac40: e5938138     	ldr	r8, [r3, #0x138]
  38ac44: e593913c     	ldr	r9, [r3, #0x13c]
  38ac48: ebfe1047     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0x7bee4
  38ac4c: e1a06000     	mov	r6, r0
  38ac50: e1a01006     	mov	r1, r6
  38ac54: e1a00004     	mov	r0, r4
  38ac58: ebfe0fd1     	bl	0x30eba4 <.plt+0xe30>   @ imm = #-0x7c0bc
  38ac5c: e5974000     	ldr	r4, [r7]
  38ac60: e1a01004     	mov	r1, r4
  38ac64: ebfe0f50     	bl	0x30e9ac <.plt+0xc38>   @ imm = #-0x7c2c0
  38ac68: e3500000     	cmp	r0, #0
  38ac6c: 0a00000b     	beq	0x38aca0 <GameObject::IsNearby(Point3D<float> const&, float) const+0x94> @ imm = #0x2c
  38ac70: e3a01442     	mov	r1, #1107296256
  38ac74: e1a00005     	mov	r0, r5
  38ac78: e2811732     	add	r1, r1, #13107200
  38ac7c: ebfe103a     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0x7bf18
  38ac80: e1a05000     	mov	r5, r0
  38ac84: e1a01005     	mov	r1, r5
  38ac88: e1a00008     	mov	r0, r8
  38ac8c: ebfe0fc4     	bl	0x30eba4 <.plt+0xe30>   @ imm = #-0x7c0f0
  38ac90: e1a01004     	mov	r1, r4
  38ac94: ebfe0e06     	bl	0x30e4b4 <.plt+0x740>   @ imm = #-0x7c7e8
  38ac98: e3500000     	cmp	r0, #0
  38ac9c: 1a000002     	bne	0x38acac <GameObject::IsNearby(Point3D<float> const&, float) const+0xa0> @ imm = #0x8
  38aca0: e3a00000     	mov	r0, #0
  38aca4: e28dd00c     	add	sp, sp, #12
  38aca8: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  38acac: e5974004     	ldr	r4, [r7, #0x4]
  38acb0: e1a01006     	mov	r1, r6
  38acb4: e1a0000a     	mov	r0, r10
  38acb8: ebfe0fb9     	bl	0x30eba4 <.plt+0xe30>   @ imm = #-0x7c11c
  38acbc: e1a01004     	mov	r1, r4
  38acc0: ebfe0f39     	bl	0x30e9ac <.plt+0xc38>   @ imm = #-0x7c31c
  38acc4: e3500000     	cmp	r0, #0
  38acc8: 0afffff4     	beq	0x38aca0 <GameObject::IsNearby(Point3D<float> const&, float) const+0x94> @ imm = #-0x30
  38accc: e1a01005     	mov	r1, r5
  38acd0: e1a00009     	mov	r0, r9
  38acd4: ebfe0fb2     	bl	0x30eba4 <.plt+0xe30>   @ imm = #-0x7c138
  38acd8: e1a01004     	mov	r1, r4
  38acdc: ebfe0df4     	bl	0x30e4b4 <.plt+0x740>   @ imm = #-0x7c830
  38ace0: e3500000     	cmp	r0, #0
  38ace4: 0affffed     	beq	0x38aca0 <GameObject::IsNearby(Point3D<float> const&, float) const+0x94> @ imm = #-0x4c
  38ace8: e5974008     	ldr	r4, [r7, #0x8]
  38acec: e1a01006     	mov	r1, r6
  38acf0: e1a0000b     	mov	r0, r11
  38acf4: ebfe0faa     	bl	0x30eba4 <.plt+0xe30>   @ imm = #-0x7c158
  38acf8: e1a01004     	mov	r1, r4
  38acfc: ebfe0f2a     	bl	0x30e9ac <.plt+0xc38>   @ imm = #-0x7c358
  38ad00: e3500000     	cmp	r0, #0
  38ad04: 0affffe5     	beq	0x38aca0 <GameObject::IsNearby(Point3D<float> const&, float) const+0x94> @ imm = #-0x6c
  38ad08: e1a01005     	mov	r1, r5
  38ad0c: e59d0004     	ldr	r0, [sp, #0x4]
  38ad10: ebfe0fa3     	bl	0x30eba4 <.plt+0xe30>   @ imm = #-0x7c174
  38ad14: e1a01004     	mov	r1, r4
  38ad18: ebfe0de5     	bl	0x30e4b4 <.plt+0x740>   @ imm = #-0x7c86c
  38ad1c: e3500000     	cmp	r0, #0
  38ad20: e3a00000     	mov	r0, #0
  38ad24: 13a00001     	movne	r0, #1
  38ad28: e6ef0070     	uxtb	r0, r0
  38ad2c: eaffffdc     	b	0x38aca4 <GameObject::IsNearby(Point3D<float> const&, float) const+0x98> @ imm = #-0x90

0038ad30 <GameObject::IsInteractiveEx(GameObject*) const>:
  38ad30: e92d4070     	push	{r4, r5, r6, lr}
  38ad34: e5903000     	ldr	r3, [r0]
  38ad38: e1a04000     	mov	r4, r0
  38ad3c: e1a05001     	mov	r5, r1
  38ad40: e1a0e00f     	mov	lr, pc
  38ad44: e593f088     	ldr	pc, [r3, #0x88]
  38ad48: e3500000     	cmp	r0, #0
  38ad4c: 1a000000     	bne	0x38ad54 <GameObject::IsInteractiveEx(GameObject*) const+0x24> @ imm = #0x0
  38ad50: e8bd8070     	pop	{r4, r5, r6, pc}
  38ad54: e1a00004     	mov	r0, r4
  38ad58: e1a01005     	mov	r1, r5
  38ad5c: e5943000     	ldr	r3, [r4]
  38ad60: e1a0e00f     	mov	lr, pc
  38ad64: e593f090     	ldr	pc, [r3, #0x90]
  38ad68: e2900001     	adds	r0, r0, #1
  38ad6c: 13a00001     	movne	r0, #1
  38ad70: e8bd8070     	pop	{r4, r5, r6, pc}

0038ad74 <GameObject::GetInteractionType(GameObject*) const>:
  38ad74: e3e00000     	mvn	r0, #0
  38ad78: e12fff1e     	bx	lr

0038ad7c <GameObject::GetInteractionRadius() const>:
  38ad7c: e92d4070     	push	{r4, r5, r6, lr}
  38ad80: e1a04000     	mov	r4, r0
  38ad84: e5901144     	ldr	r1, [r0, #0x144]
  38ad88: e5900150     	ldr	r0, [r0, #0x150]
  38ad8c: ebfe0d86     	bl	0x30e3ac <.plt+0x638>   @ imm = #-0x7c9e8
  38ad90: e5941148     	ldr	r1, [r4, #0x148]
  38ad94: e1a05000     	mov	r5, r0
  38ad98: e5940154     	ldr	r0, [r4, #0x154]
  38ad9c: ebfe0d82     	bl	0x30e3ac <.plt+0x638>   @ imm = #-0x7c9f8
  38ada0: e1a04000     	mov	r4, r0
  38ada4: e1a01004     	mov	r1, r4
  38ada8: e1a00005     	mov	r0, r5
  38adac: ebfe0d51     	bl	0x30e2f8 <.plt+0x584>   @ imm = #-0x7cabc
  38adb0: e3500000     	cmp	r0, #0
  38adb4: 11a05004     	movne	r5, r4
  38adb8: e1a00005     	mov	r0, r5
  38adbc: e3a0143f     	mov	r1, #1056964608
  38adc0: ebfe0fe9     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0x7c05c
  38adc4: e8bd8070     	pop	{r4, r5, r6, pc}

0038adc8 <GameObject::SetScale(Point3D<float> const&)>:
  38adc8: e5913000     	ldr	r3, [r1]
  38adcc: e5803120     	str	r3, [r0, #0x120]
  38add0: e5913004     	ldr	r3, [r1, #0x4]
  38add4: e5803124     	str	r3, [r0, #0x124]
  38add8: e5913008     	ldr	r3, [r1, #0x8]
  38addc: e5803128     	str	r3, [r0, #0x128]
  38ade0: e12fff1e     	bx	lr

0038ade4 <GameObject::IsBatchVisible() const>:
  38ade4: e59032d8     	ldr	r3, [r0, #0x2d8]
  38ade8: e3530000     	cmp	r3, #0
  38adec: 15933008     	ldrne	r3, [r3, #0x8]
  38adf0: 05d002f0     	ldrbeq	r0, [r0, #0x2f0]
  38adf4: 1593011c     	ldrne	r0, [r3, #0x11c]
  38adf8: 12000001     	andne	r0, r0, #1
  38adfc: e12fff1e     	bx	lr

0038ae00 <SimpleTypeProperty<int>::IsDefaultValue(void*)>:
  38ae00: e5902004     	ldr	r2, [r0, #0x4]
  38ae04: e5903020     	ldr	r3, [r0, #0x20]
  38ae08: e7910002     	ldr	r0, [r1, r2]
  38ae0c: e1500003     	cmp	r0, r3
  38ae10: 13a00000     	movne	r0, #0
  38ae14: 03a00001     	moveq	r0, #1
  38ae18: e12fff1e     	bx	lr

0038ae1c <SimpleTypeProperty<int>::SetToDefaultValue(void*)>:
  38ae1c: e5902020     	ldr	r2, [r0, #0x20]
  38ae20: e5903004     	ldr	r3, [r0, #0x4]
  38ae24: e7812003     	str	r2, [r1, r3]
  38ae28: e12fff1e     	bx	lr

0038ae2c <GameObject::UpdateIdleSound()>:
  38ae2c: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  38ae30: e59f42a0     	ldr	r4, [pc, #0x2a0]        @ 0x38b0d8 <GameObject::UpdateIdleSound()+0x2ac>
  38ae34: e59f62a0     	ldr	r6, [pc, #0x2a0]        @ 0x38b0dc <GameObject::UpdateIdleSound()+0x2b0>
  38ae38: e24dd02c     	sub	sp, sp, #44
  38ae3c: e08f4004     	add	r4, pc, r4
  38ae40: e7943006     	ldr	r3, [r4, r6]
  38ae44: e1a05000     	mov	r5, r0
  38ae48: e5932000     	ldr	r2, [r3]
  38ae4c: e3520000     	cmp	r2, #0
  38ae50: 0a000014     	beq	0x38aea8 <GameObject::UpdateIdleSound()+0x7c> @ imm = #0x50
  38ae54: e5d08373     	ldrb	r8, [r0, #0x373]
  38ae58: e3580000     	cmp	r8, #0
  38ae5c: 1a000013     	bne	0x38aeb0 <GameObject::UpdateIdleSound()+0x84> @ imm = #0x4c
  38ae60: e59f3278     	ldr	r3, [pc, #0x278]        @ 0x38b0e0 <GameObject::UpdateIdleSound()+0x2b4>
  38ae64: e794a003     	ldr	r10, [r4, r3]
  38ae68: e1a0000a     	mov	r0, r10
  38ae6c: ebfe51c8     	bl	0x31f594 <Application::GetCurrentLevel() const> @ imm = #-0x6b8e0
  38ae70: e3500000     	cmp	r0, #0
  38ae74: 0a00000b     	beq	0x38aea8 <GameObject::UpdateIdleSound()+0x7c> @ imm = #0x2c
  38ae78: e59a0040     	ldr	r0, [r10, #0x40]
  38ae7c: e1a01008     	mov	r1, r8
  38ae80: e3a02001     	mov	r2, #1
  38ae84: ebff8d7b     	bl	0x36e478 <PlayerManager::GetLocalPlayer(int, bool)> @ imm = #-0x1ca14
  38ae88: e5903660     	ldr	r3, [r0, #0x660]
  38ae8c: e3530000     	cmp	r3, #0
  38ae90: 0a000004     	beq	0x38aea8 <GameObject::UpdateIdleSound()+0x7c> @ imm = #0x10
  38ae94: e1a0000a     	mov	r0, r10
  38ae98: ebfe51bd     	bl	0x31f594 <Application::GetCurrentLevel() const> @ imm = #-0x6b90c
  38ae9c: e5903130     	ldr	r3, [r0, #0x130]
  38aea0: e3530026     	cmp	r3, #38
  38aea4: 0a00000c     	beq	0x38aedc <GameObject::UpdateIdleSound()+0xb0> @ imm = #0x30
  38aea8: e28dd02c     	add	sp, sp, #44
  38aeac: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  38aeb0: e5d02372     	ldrb	r2, [r0, #0x372]
  38aeb4: e3520000     	cmp	r2, #0
  38aeb8: 0afffffa     	beq	0x38aea8 <GameObject::UpdateIdleSound()+0x7c> @ imm = #-0x18
  38aebc: e3a02000     	mov	r2, #0
  38aec0: e5c02372     	strb	r2, [r0, #0x372]
  38aec4: e3a02e37     	mov	r2, #880
  38aec8: e19010f2     	ldrsh	r1, [r0, r2]
  38aecc: e5930000     	ldr	r0, [r3]
  38aed0: e3a02ffa     	mov	r2, #1000
  38aed4: ebff7c44     	bl	0x369fec <VoxSoundManager::Stop(int, int)> @ imm = #-0x20ef0
  38aed8: eafffff2     	b	0x38aea8 <GameObject::UpdateIdleSound()+0x7c> @ imm = #-0x38
  38aedc: e59f1200     	ldr	r1, [pc, #0x200]        @ 0x38b0e4 <GameObject::UpdateIdleSound()+0x2b8>
  38aee0: e59f2200     	ldr	r2, [pc, #0x200]        @ 0x38b0e8 <GameObject::UpdateIdleSound()+0x2bc>
  38aee4: e59a002c     	ldr	r0, [r10, #0x2c]
  38aee8: e08f1001     	add	r1, pc, r1
  38aeec: e08f2002     	add	r2, pc, r2
  38aef0: eb04e739     	bl	0x4c4bdc <PyDataConstants::getConstant(char const*, char const*) const> @ imm = #0x139ce4
  38aef4: ebfe0e9a     	bl	0x30e964 <.plt+0xbf0>   @ imm = #-0x7c598
  38aef8: e59f31ec     	ldr	r3, [pc, #0x1ec]        @ 0x38b0ec <GameObject::UpdateIdleSound()+0x2c0>
  38aefc: e1a07000     	mov	r7, r0
  38af00: e1a01008     	mov	r1, r8
  38af04: e7943003     	ldr	r3, [r4, r3]
  38af08: e59a0040     	ldr	r0, [r10, #0x40]
  38af0c: e3a02001     	mov	r2, #1
  38af10: e593c008     	ldr	r12, [r3, #0x8]
  38af14: e593e000     	ldr	lr, [r3]
  38af18: e5933004     	ldr	r3, [r3, #0x4]
  38af1c: e58dc024     	str	r12, [sp, #0x24]
  38af20: e58de01c     	str	lr, [sp, #0x1c]
  38af24: e58d3020     	str	r3, [sp, #0x20]
  38af28: ebff8d52     	bl	0x36e478 <PlayerManager::GetLocalPlayer(int, bool)> @ imm = #-0x1cab8
  38af2c: e5903660     	ldr	r3, [r0, #0x660]
  38af30: e3530000     	cmp	r3, #0
  38af34: 0a000063     	beq	0x38b0c8 <GameObject::UpdateIdleSound()+0x29c> @ imm = #0x18c
  38af38: e1a01008     	mov	r1, r8
  38af3c: e59a0040     	ldr	r0, [r10, #0x40]
  38af40: e3a02001     	mov	r2, #1
  38af44: ebff8d4b     	bl	0x36e478 <PlayerManager::GetLocalPlayer(int, bool)> @ imm = #-0x1cad4
  38af48: e5903660     	ldr	r3, [r0, #0x660]
  38af4c: e5931160     	ldr	r1, [r3, #0x160]
  38af50: e58d101c     	str	r1, [sp, #0x1c]
  38af54: e593a164     	ldr	r10, [r3, #0x164]
  38af58: e58da020     	str	r10, [sp, #0x20]
  38af5c: e5938168     	ldr	r8, [r3, #0x168]
  38af60: e58d8024     	str	r8, [sp, #0x24]
  38af64: e5950160     	ldr	r0, [r5, #0x160]
  38af68: ebfe0d0f     	bl	0x30e3ac <.plt+0x638>   @ imm = #-0x7cbc4
  38af6c: e1a0100a     	mov	r1, r10
  38af70: e1a09000     	mov	r9, r0
  38af74: e5950164     	ldr	r0, [r5, #0x164]
  38af78: ebfe0d0b     	bl	0x30e3ac <.plt+0x638>   @ imm = #-0x7cbd4
  38af7c: e1a01008     	mov	r1, r8
  38af80: e1a0b000     	mov	r11, r0
  38af84: e5950168     	ldr	r0, [r5, #0x168]
  38af88: ebfe0d07     	bl	0x30e3ac <.plt+0x638>   @ imm = #-0x7cbe4
  38af8c: e1a01009     	mov	r1, r9
  38af90: e1a0a000     	mov	r10, r0
  38af94: e1a00009     	mov	r0, r9
  38af98: ebfe0f73     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0x7c234
  38af9c: e1a0100b     	mov	r1, r11
  38afa0: e1a08000     	mov	r8, r0
  38afa4: e1a0000b     	mov	r0, r11
  38afa8: ebfe0f6f     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0x7c244
  38afac: e1a01000     	mov	r1, r0
  38afb0: e1a00008     	mov	r0, r8
  38afb4: ebfe0efa     	bl	0x30eba4 <.plt+0xe30>   @ imm = #-0x7c418
  38afb8: e1a0100a     	mov	r1, r10
  38afbc: e1a08000     	mov	r8, r0
  38afc0: e1a0000a     	mov	r0, r10
  38afc4: ebfe0f68     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0x7c260
  38afc8: e1a01000     	mov	r1, r0
  38afcc: e1a00008     	mov	r0, r8
  38afd0: ebfe0ef3     	bl	0x30eba4 <.plt+0xe30>   @ imm = #-0x7c434
  38afd4: ebfe0e32     	bl	0x30e8a4 <.plt+0xb30>   @ imm = #-0x7c738
  38afd8: ebfe0c78     	bl	0x30e1c0 <.plt+0x44c>   @ imm = #-0x7ce20
  38afdc: ebfe0daf     	bl	0x30e6a0 <.plt+0x92c>   @ imm = #-0x7c944
  38afe0: e5d53372     	ldrb	r3, [r5, #0x372]
  38afe4: e1a08000     	mov	r8, r0
  38afe8: e3530000     	cmp	r3, #0
  38afec: 0a000016     	beq	0x38b04c <GameObject::UpdateIdleSound()+0x220> @ imm = #0x58
  38aff0: e1a00007     	mov	r0, r7
  38aff4: e1a01008     	mov	r1, r8
  38aff8: ebfe0e6b     	bl	0x30e9ac <.plt+0xc38>   @ imm = #-0x7c654
  38affc: e3500000     	cmp	r0, #0
  38b000: 0affffa8     	beq	0x38aea8 <GameObject::UpdateIdleSound()+0x7c> @ imm = #-0x160
  38b004: e1a00007     	mov	r0, r7
  38b008: e3a01000     	mov	r1, #0
  38b00c: ebfe0cb9     	bl	0x30e2f8 <.plt+0x584>   @ imm = #-0x7cd1c
  38b010: e3500000     	cmp	r0, #0
  38b014: 0affffa3     	beq	0x38aea8 <GameObject::UpdateIdleSound()+0x7c> @ imm = #-0x174
  38b018: e7943006     	ldr	r3, [r4, r6]
  38b01c: e3a02000     	mov	r2, #0
  38b020: e5c52372     	strb	r2, [r5, #0x372]
  38b024: e5930000     	ldr	r0, [r3]
  38b028: e3a03e37     	mov	r3, #880
  38b02c: e19510f3     	ldrsh	r1, [r5, r3]
  38b030: e3a02ffa     	mov	r2, #1000
  38b034: e28d301c     	add	r3, sp, #28
  38b038: e58d7000     	str	r7, [sp]
  38b03c: ebff7c75     	bl	0x36a218 <VoxSoundManager::Stop3D(int, int, Point3D<float>*, float)> @ imm = #-0x20e2c
  38b040: e5d53372     	ldrb	r3, [r5, #0x372]
  38b044: e3530000     	cmp	r3, #0
  38b048: 1affff96     	bne	0x38aea8 <GameObject::UpdateIdleSound()+0x7c> @ imm = #-0x1a8
  38b04c: e1a01008     	mov	r1, r8
  38b050: e1a00007     	mov	r0, r7
  38b054: ebfe0ca7     	bl	0x30e2f8 <.plt+0x584>   @ imm = #-0x7cd64
  38b058: e3500000     	cmp	r0, #0
  38b05c: 0affff91     	beq	0x38aea8 <GameObject::UpdateIdleSound()+0x7c> @ imm = #-0x1bc
  38b060: e1a00007     	mov	r0, r7
  38b064: e3a01000     	mov	r1, #0
  38b068: ebfe0ca2     	bl	0x30e2f8 <.plt+0x584>   @ imm = #-0x7cd78
  38b06c: e3500000     	cmp	r0, #0
  38b070: 0affff8c     	beq	0x38aea8 <GameObject::UpdateIdleSound()+0x7c> @ imm = #-0x1d0
  38b074: e7943006     	ldr	r3, [r4, r6]
  38b078: e3a0c001     	mov	r12, #1
  38b07c: e5c5c372     	strb	r12, [r5, #0x372]
  38b080: e5957160     	ldr	r7, [r5, #0x160]
  38b084: e5956164     	ldr	r6, [r5, #0x164]
  38b088: e5954168     	ldr	r4, [r5, #0x168]
  38b08c: e5930000     	ldr	r0, [r3]
