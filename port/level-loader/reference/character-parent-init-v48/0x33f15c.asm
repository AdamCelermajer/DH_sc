
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0033f15c <ObjectBase::ObjectBase(ObjectBase::GO_IDS)>:
  33f15c: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  33f160: e59f6198     	ldr	r6, [pc, #0x198]        @ 0x33f300 <ObjectBase::ObjectBase(ObjectBase::GO_IDS)+0x1a4>
  33f164: e59f2198     	ldr	r2, [pc, #0x198]        @ 0x33f304 <ObjectBase::ObjectBase(ObjectBase::GO_IDS)+0x1a8>
  33f168: e59f3198     	ldr	r3, [pc, #0x198]        @ 0x33f308 <ObjectBase::ObjectBase(ObjectBase::GO_IDS)+0x1ac>
  33f16c: e08f6006     	add	r6, pc, r6
  33f170: e7962002     	ldr	r2, [r6, r2]
  33f174: e7963003     	ldr	r3, [r6, r3]
  33f178: e1a04000     	mov	r4, r0
  33f17c: e2822008     	add	r2, r2, #8
  33f180: e2830008     	add	r0, r3, #8
  33f184: e2843008     	add	r3, r4, #8
  33f188: e5842000     	str	r2, [r4]
  33f18c: e5840004     	str	r0, [r4, #0x4]
  33f190: e1a07001     	mov	r7, r1
  33f194: e1a00003     	mov	r0, r3
  33f198: e5843018     	str	r3, [r4, #0x18]
  33f19c: e584301c     	str	r3, [r4, #0x1c]
  33f1a0: e3a01010     	mov	r1, #16
  33f1a4: ebff4934     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x2db30
  33f1a8: e59f215c     	ldr	r2, [pc, #0x15c]        @ 0x33f30c <ObjectBase::ObjectBase(ObjectBase::GO_IDS)+0x1b0>
  33f1ac: e5941018     	ldr	r1, [r4, #0x18]
  33f1b0: e3a05000     	mov	r5, #0
  33f1b4: e7962002     	ldr	r2, [r6, r2]
  33f1b8: e5c15000     	strb	r5, [r1]
  33f1bc: e2843030     	add	r3, r4, #48
  33f1c0: e2821074     	add	r1, r2, #116
  33f1c4: e2820008     	add	r0, r2, #8
  33f1c8: e2822068     	add	r2, r2, #104
  33f1cc: e8840005     	stm	r4, {r0, r2}
  33f1d0: e5841024     	str	r1, [r4, #0x24]
  33f1d4: e1a00003     	mov	r0, r3
  33f1d8: e5845020     	str	r5, [r4, #0x20]
  33f1dc: e5c45028     	strb	r5, [r4, #0x28]
  33f1e0: e5c45029     	strb	r5, [r4, #0x29]
  33f1e4: e584502c     	str	r5, [r4, #0x2c]
  33f1e8: e5843040     	str	r3, [r4, #0x40]
  33f1ec: e5843044     	str	r3, [r4, #0x44]
  33f1f0: e3a01010     	mov	r1, #16
  33f1f4: ebff4920     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x2db80
  33f1f8: e5942040     	ldr	r2, [r4, #0x40]
  33f1fc: e2843048     	add	r3, r4, #72
  33f200: e1a00003     	mov	r0, r3
  33f204: e5c25000     	strb	r5, [r2]
  33f208: e3a01010     	mov	r1, #16
  33f20c: e5843058     	str	r3, [r4, #0x58]
  33f210: e584305c     	str	r3, [r4, #0x5c]
  33f214: ebff4918     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x2dba0
  33f218: e5942058     	ldr	r2, [r4, #0x58]
  33f21c: e2843068     	add	r3, r4, #104
  33f220: e3e06000     	mvn	r6, #0
  33f224: e5c25000     	strb	r5, [r2]
  33f228: e3a01010     	mov	r1, #16
  33f22c: e1a00003     	mov	r0, r3
  33f230: e5c45060     	strb	r5, [r4, #0x60]
  33f234: e5843078     	str	r3, [r4, #0x78]
  33f238: e584307c     	str	r3, [r4, #0x7c]
  33f23c: e5846064     	str	r6, [r4, #0x64]
  33f240: ebff490d     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x2dbcc
  33f244: e5943078     	ldr	r3, [r4, #0x78]
  33f248: e284008c     	add	r0, r4, #140
  33f24c: e5c35000     	strb	r5, [r3]
  33f250: e3a03001     	mov	r3, #1
  33f254: e5c4308a     	strb	r3, [r4, #0x8a]
  33f258: e5c45081     	strb	r5, [r4, #0x81]
  33f25c: e5c45084     	strb	r5, [r4, #0x84]
  33f260: e5c45085     	strb	r5, [r4, #0x85]
  33f264: e5c45086     	strb	r5, [r4, #0x86]
  33f268: e5c45088     	strb	r5, [r4, #0x88]
  33f26c: e5c45089     	strb	r5, [r4, #0x89]
  33f270: ebfffec1     	bl	0x33ed7c <ConditionData::ConditionData()> @ imm = #-0x4fc
  33f274: e28400b0     	add	r0, r4, #176
  33f278: ebfffebf     	bl	0x33ed7c <ConditionData::ConditionData()> @ imm = #-0x504
  33f27c: e28430d4     	add	r3, r4, #212
  33f280: e1a00003     	mov	r0, r3
  33f284: e58430e4     	str	r3, [r4, #0xe4]
  33f288: e58430e8     	str	r3, [r4, #0xe8]
  33f28c: e3a01010     	mov	r1, #16
  33f290: ebff48f9     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x2dc1c
  33f294: e59430e4     	ldr	r3, [r4, #0xe4]
  33f298: e1a01005     	mov	r1, r5
  33f29c: e3a0000c     	mov	r0, #12
  33f2a0: e5c35000     	strb	r5, [r3]
  33f2a4: e3a03000     	mov	r3, #0
  33f2a8: e5843114     	str	r3, [r4, #0x114]
  33f2ac: e5c450f0     	strb	r5, [r4, #0xf0]
  33f2b0: e5c450f1     	strb	r5, [r4, #0xf1]
  33f2b4: e5c450f8     	strb	r5, [r4, #0xf8]
  33f2b8: e58450fc     	str	r5, [r4, #0xfc]
  33f2bc: e5845100     	str	r5, [r4, #0x100]
  33f2c0: e5845104     	str	r5, [r4, #0x104]
  33f2c4: e5c4510c     	strb	r5, [r4, #0x10c]
  33f2c8: e5c45118     	strb	r5, [r4, #0x118]
  33f2cc: e5c45119     	strb	r5, [r4, #0x119]
  33f2d0: e584511c     	str	r5, [r4, #0x11c]
  33f2d4: e58470f4     	str	r7, [r4, #0xf4]
  33f2d8: e5846110     	str	r6, [r4, #0x110]
  33f2dc: e58460ec     	str	r6, [r4, #0xec]
  33f2e0: e5846108     	str	r6, [r4, #0x108]
  33f2e4: ebff44a1     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x2ed7c
  33f2e8: e1a05000     	mov	r5, r0
  33f2ec: eb000086     	bl	0x33f50c <ObjectHandle::ObjectHandle()> @ imm = #0x218
  33f2f0: e584502c     	str	r5, [r4, #0x2c]
  33f2f4: e1a00004     	mov	r0, r4
  33f2f8: e5854004     	str	r4, [r5, #0x4]
  33f2fc: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  33f300: 24 59 65 00  	.word	0x00655924
  33f304: 8c 10 00 00  	.word	0x0000108c
  33f308: dc 3b 00 00  	.word	0x00003bdc
  33f30c: 84 3b 00 00  	.word	0x00003b84
