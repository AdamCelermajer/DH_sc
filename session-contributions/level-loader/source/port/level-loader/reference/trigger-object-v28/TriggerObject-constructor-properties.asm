
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00340f0c <ObjectBase* GetNewInstance<TriggerObject>()>:
  340f0c: e92d4010     	push	{r4, lr}
  340f10: e3a01000     	mov	r1, #0
  340f14: e3a00e79     	mov	r0, #1936
  340f18: ebff3d94     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x309b0
  340f1c: e1a04000     	mov	r4, r0
  340f20: eb0163b2     	bl	0x399df0 <TriggerObject::TriggerObject()> @ imm = #0x58ec8
  340f24: e1a00004     	mov	r0, r4
  340f28: e8bd8010     	pop	{r4, pc}


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00399d70 <TriggerObject::DeclareProperties()>:
  399d70: e92d4070     	push	{r4, r5, r6, lr}
  399d74: e1a04000     	mov	r4, r0
  399d78: ebfffb42     	bl	0x398a88 <Trigger::DeclareProperties()> @ imm = #-0x12f8
  399d7c: e59f105c     	ldr	r1, [pc, #0x5c]         @ 0x399de0 <TriggerObject::DeclareProperties()+0x70>
  399d80: e2845004     	add	r5, r4, #4
  399d84: e2842e71     	add	r2, r4, #1808
  399d88: e1a00005     	mov	r0, r5
  399d8c: e08f1001     	add	r1, pc, r1
  399d90: e2822008     	add	r2, r2, #8
  399d94: ebfe9478     	bl	0x33ef7c <void PropertyMap::AddProperty<std::string>(char const*, std::string&)> @ imm = #-0x5ae20
  399d98: e59f1044     	ldr	r1, [pc, #0x44]         @ 0x399de4 <TriggerObject::DeclareProperties()+0x74>
  399d9c: e2842e73     	add	r2, r4, #1840
  399da0: e1a00005     	mov	r0, r5
  399da4: e2822004     	add	r2, r2, #4
  399da8: e08f1001     	add	r1, pc, r1
  399dac: ebfe9472     	bl	0x33ef7c <void PropertyMap::AddProperty<std::string>(char const*, std::string&)> @ imm = #-0x5ae38
  399db0: e59f1030     	ldr	r1, [pc, #0x30]         @ 0x399de8 <TriggerObject::DeclareProperties()+0x78>
  399db4: e1a00005     	mov	r0, r5
  399db8: e2842e75     	add	r2, r4, #1872
  399dbc: e08f1001     	add	r1, pc, r1
  399dc0: ebfe946d     	bl	0x33ef7c <void PropertyMap::AddProperty<std::string>(char const*, std::string&)> @ imm = #-0x5ae4c
  399dc4: e59f1020     	ldr	r1, [pc, #0x20]         @ 0x399dec <TriggerObject::DeclareProperties()+0x7c>
  399dc8: e2842e76     	add	r2, r4, #1888
  399dcc: e1a00005     	mov	r0, r5
  399dd0: e08f1001     	add	r1, pc, r1
  399dd4: e282200c     	add	r2, r2, #12
  399dd8: e8bd4070     	pop	{r4, r5, r6, lr}
  399ddc: eafe9466     	b	0x33ef7c <void PropertyMap::AddProperty<std::string>(char const*, std::string&)> @ imm = #-0x5ae68
  399de0: cc 8d 52 00  	.word	0x00528dcc
  399de4: 30 7c 52 00  	.word	0x00527c30
  399de8: a4 8d 52 00  	.word	0x00528da4
  399dec: a0 8d 52 00  	.word	0x00528da0


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00399df0 <TriggerObject::TriggerObject()>:
  399df0: e92d4070     	push	{r4, r5, r6, lr}
  399df4: e3a02000     	mov	r2, #0
  399df8: e3a03001     	mov	r3, #1
  399dfc: e3a01014     	mov	r1, #20
  399e00: e59f50e4     	ldr	r5, [pc, #0xe4]         @ 0x399eec <TriggerObject::TriggerObject()+0xfc>
  399e04: e1a04000     	mov	r4, r0
  399e08: ebfffc71     	bl	0x398fd4 <Trigger::Trigger(ObjectBase::GO_IDS, bool, bool)> @ imm = #-0xe3c
  399e0c: e59f20dc     	ldr	r2, [pc, #0xdc]         @ 0x399ef0 <TriggerObject::TriggerObject()+0x100>
  399e10: e08f5005     	add	r5, pc, r5
  399e14: e2843e71     	add	r3, r4, #1808
  399e18: e7952002     	ldr	r2, [r5, r2]
  399e1c: e2833008     	add	r3, r3, #8
  399e20: e1a00003     	mov	r0, r3
  399e24: e282c008     	add	r12, r2, #8
  399e28: e28210f4     	add	r1, r2, #244
  399e2c: e28220e8     	add	r2, r2, #232
  399e30: e584c000     	str	r12, [r4]
  399e34: e5842004     	str	r2, [r4, #0x4]
  399e38: e5841024     	str	r1, [r4, #0x24]
  399e3c: e5843728     	str	r3, [r4, #0x728]
  399e40: e584372c     	str	r3, [r4, #0x72c]
  399e44: e3a01010     	mov	r1, #16
  399e48: ebfdde0b     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x887d4
  399e4c: e5942728     	ldr	r2, [r4, #0x728]
  399e50: e3a05000     	mov	r5, #0
  399e54: e3e06000     	mvn	r6, #0
  399e58: e1a03004     	mov	r3, r4
  399e5c: e5c25000     	strb	r5, [r2]
  399e60: e5a36730     	str	r6, [r3, #0x730]!
  399e64: e2833004     	add	r3, r3, #4
  399e68: e1a00003     	mov	r0, r3
  399e6c: e5843744     	str	r3, [r4, #0x744]
  399e70: e5843748     	str	r3, [r4, #0x748]
  399e74: e3a01010     	mov	r1, #16
  399e78: ebfdddff     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x88804
  399e7c: e5942744     	ldr	r2, [r4, #0x744]
  399e80: e2843e75     	add	r3, r4, #1872
  399e84: e1a00003     	mov	r0, r3
  399e88: e5c25000     	strb	r5, [r2]
  399e8c: e3a01010     	mov	r1, #16
  399e90: e5843760     	str	r3, [r4, #0x760]
  399e94: e5843764     	str	r3, [r4, #0x764]
  399e98: e584674c     	str	r6, [r4, #0x74c]
  399e9c: ebfdddf6     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x88828
  399ea0: e1a03004     	mov	r3, r4
  399ea4: e5b32760     	ldr	r2, [r3, #0x760]!
  399ea8: e3a01010     	mov	r1, #16
  399eac: e283300c     	add	r3, r3, #12
  399eb0: e5c25000     	strb	r5, [r2]
  399eb4: e1a00003     	mov	r0, r3
  399eb8: e584377c     	str	r3, [r4, #0x77c]
  399ebc: e5843780     	str	r3, [r4, #0x780]
  399ec0: ebfddded     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x8884c
  399ec4: e594277c     	ldr	r2, [r4, #0x77c]
  399ec8: e3a03001     	mov	r3, #1
  399ecc: e1a00004     	mov	r0, r4
  399ed0: e5c25000     	strb	r5, [r2]
  399ed4: e5c45084     	strb	r5, [r4, #0x84]
  399ed8: e5c43085     	strb	r3, [r4, #0x85]
  399edc: e5c45784     	strb	r5, [r4, #0x784]
  399ee0: e5845788     	str	r5, [r4, #0x788]
  399ee4: e5c43028     	strb	r3, [r4, #0x28]
  399ee8: e8bd8070     	pop	{r4, r5, r6, pc}
  399eec: 80 ac 5f 00  	.word	0x005fac80
  399ef0: 64 2b 00 00  	.word	0x00002b64


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00399ef4 <TriggerObject::TriggerObject()>:
  399ef4: e92d4070     	push	{r4, r5, r6, lr}
  399ef8: e3a02000     	mov	r2, #0
  399efc: e3a03001     	mov	r3, #1
  399f00: e3a01014     	mov	r1, #20
  399f04: e59f50e4     	ldr	r5, [pc, #0xe4]         @ 0x399ff0 <TriggerObject::TriggerObject()+0xfc>
  399f08: e1a04000     	mov	r4, r0
  399f0c: ebfffc30     	bl	0x398fd4 <Trigger::Trigger(ObjectBase::GO_IDS, bool, bool)> @ imm = #-0xf40
  399f10: e59f20dc     	ldr	r2, [pc, #0xdc]         @ 0x399ff4 <TriggerObject::TriggerObject()+0x100>
  399f14: e08f5005     	add	r5, pc, r5
  399f18: e2843e71     	add	r3, r4, #1808
  399f1c: e7952002     	ldr	r2, [r5, r2]
  399f20: e2833008     	add	r3, r3, #8
  399f24: e1a00003     	mov	r0, r3
  399f28: e282c008     	add	r12, r2, #8
  399f2c: e28210f4     	add	r1, r2, #244
  399f30: e28220e8     	add	r2, r2, #232
  399f34: e584c000     	str	r12, [r4]
  399f38: e5842004     	str	r2, [r4, #0x4]
  399f3c: e5841024     	str	r1, [r4, #0x24]
  399f40: e5843728     	str	r3, [r4, #0x728]
  399f44: e584372c     	str	r3, [r4, #0x72c]
  399f48: e3a01010     	mov	r1, #16
  399f4c: ebfdddca     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x888d8
  399f50: e5942728     	ldr	r2, [r4, #0x728]
  399f54: e3a05000     	mov	r5, #0
  399f58: e3e06000     	mvn	r6, #0
  399f5c: e1a03004     	mov	r3, r4
  399f60: e5c25000     	strb	r5, [r2]
  399f64: e5a36730     	str	r6, [r3, #0x730]!
  399f68: e2833004     	add	r3, r3, #4
  399f6c: e1a00003     	mov	r0, r3
  399f70: e5843744     	str	r3, [r4, #0x744]
  399f74: e5843748     	str	r3, [r4, #0x748]
  399f78: e3a01010     	mov	r1, #16
  399f7c: ebfdddbe     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x88908
  399f80: e5942744     	ldr	r2, [r4, #0x744]
  399f84: e2843e75     	add	r3, r4, #1872
  399f88: e1a00003     	mov	r0, r3
  399f8c: e5c25000     	strb	r5, [r2]
  399f90: e3a01010     	mov	r1, #16
  399f94: e5843760     	str	r3, [r4, #0x760]
  399f98: e5843764     	str	r3, [r4, #0x764]
  399f9c: e584674c     	str	r6, [r4, #0x74c]
  399fa0: ebfdddb5     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x8892c
  399fa4: e1a03004     	mov	r3, r4
  399fa8: e5b32760     	ldr	r2, [r3, #0x760]!
  399fac: e3a01010     	mov	r1, #16
  399fb0: e283300c     	add	r3, r3, #12
  399fb4: e5c25000     	strb	r5, [r2]
  399fb8: e1a00003     	mov	r0, r3
  399fbc: e584377c     	str	r3, [r4, #0x77c]
  399fc0: e5843780     	str	r3, [r4, #0x780]
  399fc4: ebfdddac     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x88950
  399fc8: e594277c     	ldr	r2, [r4, #0x77c]
  399fcc: e3a03001     	mov	r3, #1
  399fd0: e1a00004     	mov	r0, r4
  399fd4: e5c25000     	strb	r5, [r2]
  399fd8: e5c45084     	strb	r5, [r4, #0x84]
  399fdc: e5c43085     	strb	r3, [r4, #0x85]
  399fe0: e5c45784     	strb	r5, [r4, #0x784]
  399fe4: e5845788     	str	r5, [r4, #0x788]
  399fe8: e5c43028     	strb	r3, [r4, #0x28]
  399fec: e8bd8070     	pop	{r4, r5, r6, pc}
  399ff0: 7c ab 5f 00  	.word	0x005fab7c
  399ff4: 64 2b 00 00  	.word	0x00002b64


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00399d68 <non-virtual thunk to TriggerObject::DeclareProperties()>:
  399d68: e2400004     	sub	r0, r0, #4
  399d6c: eaffffff     	b	0x399d70 <TriggerObject::DeclareProperties()> @ imm = #-0x4
