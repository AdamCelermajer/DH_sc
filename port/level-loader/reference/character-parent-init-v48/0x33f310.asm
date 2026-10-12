
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0033f310 <ObjectBase::ObjectBase(ObjectBase::GO_IDS)>:
  33f310: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  33f314: e59f6198     	ldr	r6, [pc, #0x198]        @ 0x33f4b4 <ObjectBase::ObjectBase(ObjectBase::GO_IDS)+0x1a4>
  33f318: e59f2198     	ldr	r2, [pc, #0x198]        @ 0x33f4b8 <ObjectBase::ObjectBase(ObjectBase::GO_IDS)+0x1a8>
  33f31c: e59f3198     	ldr	r3, [pc, #0x198]        @ 0x33f4bc <ObjectBase::ObjectBase(ObjectBase::GO_IDS)+0x1ac>
  33f320: e08f6006     	add	r6, pc, r6
  33f324: e7962002     	ldr	r2, [r6, r2]
  33f328: e7963003     	ldr	r3, [r6, r3]
  33f32c: e1a04000     	mov	r4, r0
  33f330: e2822008     	add	r2, r2, #8
  33f334: e2830008     	add	r0, r3, #8
  33f338: e2843008     	add	r3, r4, #8
  33f33c: e5842000     	str	r2, [r4]
  33f340: e5840004     	str	r0, [r4, #0x4]
  33f344: e1a07001     	mov	r7, r1
  33f348: e1a00003     	mov	r0, r3
  33f34c: e5843018     	str	r3, [r4, #0x18]
  33f350: e584301c     	str	r3, [r4, #0x1c]
  33f354: e3a01010     	mov	r1, #16
  33f358: ebff48c7     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x2dce4
  33f35c: e59f215c     	ldr	r2, [pc, #0x15c]        @ 0x33f4c0 <ObjectBase::ObjectBase(ObjectBase::GO_IDS)+0x1b0>
  33f360: e5941018     	ldr	r1, [r4, #0x18]
  33f364: e3a05000     	mov	r5, #0
  33f368: e7962002     	ldr	r2, [r6, r2]
  33f36c: e5c15000     	strb	r5, [r1]
  33f370: e2843030     	add	r3, r4, #48
  33f374: e2821074     	add	r1, r2, #116
  33f378: e2820008     	add	r0, r2, #8
  33f37c: e2822068     	add	r2, r2, #104
  33f380: e8840005     	stm	r4, {r0, r2}
  33f384: e5841024     	str	r1, [r4, #0x24]
  33f388: e1a00003     	mov	r0, r3
  33f38c: e5845020     	str	r5, [r4, #0x20]
  33f390: e5c45028     	strb	r5, [r4, #0x28]
  33f394: e5c45029     	strb	r5, [r4, #0x29]
  33f398: e584502c     	str	r5, [r4, #0x2c]
  33f39c: e5843040     	str	r3, [r4, #0x40]
  33f3a0: e5843044     	str	r3, [r4, #0x44]
  33f3a4: e3a01010     	mov	r1, #16
  33f3a8: ebff48b3     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x2dd34
  33f3ac: e5942040     	ldr	r2, [r4, #0x40]
  33f3b0: e2843048     	add	r3, r4, #72
  33f3b4: e1a00003     	mov	r0, r3
  33f3b8: e5c25000     	strb	r5, [r2]
  33f3bc: e3a01010     	mov	r1, #16
  33f3c0: e5843058     	str	r3, [r4, #0x58]
  33f3c4: e584305c     	str	r3, [r4, #0x5c]
  33f3c8: ebff48ab     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x2dd54
  33f3cc: e5942058     	ldr	r2, [r4, #0x58]
  33f3d0: e2843068     	add	r3, r4, #104
  33f3d4: e3e06000     	mvn	r6, #0
  33f3d8: e5c25000     	strb	r5, [r2]
  33f3dc: e3a01010     	mov	r1, #16
  33f3e0: e1a00003     	mov	r0, r3
  33f3e4: e5c45060     	strb	r5, [r4, #0x60]
  33f3e8: e5843078     	str	r3, [r4, #0x78]
  33f3ec: e584307c     	str	r3, [r4, #0x7c]
  33f3f0: e5846064     	str	r6, [r4, #0x64]
  33f3f4: ebff48a0     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x2dd80
  33f3f8: e5943078     	ldr	r3, [r4, #0x78]
  33f3fc: e284008c     	add	r0, r4, #140
  33f400: e5c35000     	strb	r5, [r3]
  33f404: e3a03001     	mov	r3, #1
  33f408: e5c4308a     	strb	r3, [r4, #0x8a]
  33f40c: e5c45081     	strb	r5, [r4, #0x81]
  33f410: e5c45084     	strb	r5, [r4, #0x84]
  33f414: e5c45085     	strb	r5, [r4, #0x85]
  33f418: e5c45086     	strb	r5, [r4, #0x86]
  33f41c: e5c45088     	strb	r5, [r4, #0x88]
  33f420: e5c45089     	strb	r5, [r4, #0x89]
  33f424: ebfffe54     	bl	0x33ed7c <ConditionData::ConditionData()> @ imm = #-0x6b0
  33f428: e28400b0     	add	r0, r4, #176
  33f42c: ebfffe52     	bl	0x33ed7c <ConditionData::ConditionData()> @ imm = #-0x6b8
  33f430: e28430d4     	add	r3, r4, #212
  33f434: e1a00003     	mov	r0, r3
  33f438: e58430e4     	str	r3, [r4, #0xe4]
  33f43c: e58430e8     	str	r3, [r4, #0xe8]
  33f440: e3a01010     	mov	r1, #16
  33f444: ebff488c     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x2ddd0
  33f448: e59430e4     	ldr	r3, [r4, #0xe4]
  33f44c: e1a01005     	mov	r1, r5
  33f450: e3a0000c     	mov	r0, #12
  33f454: e5c35000     	strb	r5, [r3]
  33f458: e3a03000     	mov	r3, #0
  33f45c: e5843114     	str	r3, [r4, #0x114]
  33f460: e5c450f0     	strb	r5, [r4, #0xf0]
  33f464: e5c450f1     	strb	r5, [r4, #0xf1]
  33f468: e5c450f8     	strb	r5, [r4, #0xf8]
  33f46c: e58450fc     	str	r5, [r4, #0xfc]
  33f470: e5845100     	str	r5, [r4, #0x100]
  33f474: e5845104     	str	r5, [r4, #0x104]
  33f478: e5c4510c     	strb	r5, [r4, #0x10c]
  33f47c: e5c45118     	strb	r5, [r4, #0x118]
  33f480: e5c45119     	strb	r5, [r4, #0x119]
  33f484: e584511c     	str	r5, [r4, #0x11c]
  33f488: e58470f4     	str	r7, [r4, #0xf4]
  33f48c: e5846110     	str	r6, [r4, #0x110]
  33f490: e58460ec     	str	r6, [r4, #0xec]
  33f494: e5846108     	str	r6, [r4, #0x108]
  33f498: ebff4434     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x2ef30
  33f49c: e1a05000     	mov	r5, r0
  33f4a0: eb000019     	bl	0x33f50c <ObjectHandle::ObjectHandle()> @ imm = #0x64
  33f4a4: e584502c     	str	r5, [r4, #0x2c]
  33f4a8: e1a00004     	mov	r0, r4
  33f4ac: e5854004     	str	r4, [r5, #0x4]
  33f4b0: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  33f4b4: 70 57 65 00  	.word	0x00655770
  33f4b8: 8c 10 00 00  	.word	0x0000108c
  33f4bc: dc 3b 00 00  	.word	0x00003bdc
  33f4c0: 84 3b 00 00  	.word	0x00003b84
