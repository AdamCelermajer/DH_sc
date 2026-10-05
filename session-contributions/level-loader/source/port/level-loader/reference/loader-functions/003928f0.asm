
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003928f0 <GameObject::_SetPosition(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)>:
  3928f0: e92d40f0     	push	{r4, r5, r6, r7, lr}
  3928f4: e5903004     	ldr	r3, [r0, #0x4]
  3928f8: e1a04002     	mov	r4, r2
  3928fc: e59f5274     	ldr	r5, [pc, #0x274]        @ 0x392b78 <GameObject::_SetPosition(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)+0x288>
  392900: e8930006     	ldm	r3, {r1, r2}
  392904: e08f5005     	add	r5, pc, r5
  392908: e24dd024     	sub	sp, sp, #36
  39290c: e0613002     	rsb	r3, r1, r2
  392910: e1a03243     	asr	r3, r3, #4
  392914: e1a06000     	mov	r6, r0
  392918: e0832183     	add	r2, r3, r3, lsl #3
  39291c: e0822302     	add	r2, r2, r2, lsl #6
  392920: e0832182     	add	r2, r3, r2, lsl #3
  392924: e0822782     	add	r2, r2, r2, lsl #15
  392928: e0833182     	add	r3, r3, r2, lsl #3
  39292c: e2633000     	rsb	r3, r3, #0
  392930: e3530001     	cmp	r3, #1
  392934: 0a00001a     	beq	0x3929a4 <GameObject::_SetPosition(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)+0xb4> @ imm = #0x68
  392938: e3530003     	cmp	r3, #3
  39293c: 0a000001     	beq	0x392948 <GameObject::_SetPosition(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)+0x58> @ imm = #0x4
  392940: e28dd024     	add	sp, sp, #36
  392944: e8bd80f0     	pop	{r4, r5, r6, r7, pc}
  392948: e5913004     	ldr	r3, [r1, #0x4]
  39294c: e3530003     	cmp	r3, #3
  392950: 1afffffa     	bne	0x392940 <GameObject::_SetPosition(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)+0x50> @ imm = #-0x18
  392954: e3a01001     	mov	r1, #1
  392958: ebffa466     	bl	0x37baf8 <sfc::script::lua::Arguments::operator[](unsigned int) const> @ imm = #-0x16e68
  39295c: e5903004     	ldr	r3, [r0, #0x4]
  392960: e3530003     	cmp	r3, #3
  392964: 1afffff5     	bne	0x392940 <GameObject::_SetPosition(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)+0x50> @ imm = #-0x2c
  392968: e1a00006     	mov	r0, r6
  39296c: e3a01002     	mov	r1, #2
  392970: ebffa460     	bl	0x37baf8 <sfc::script::lua::Arguments::operator[](unsigned int) const> @ imm = #-0x16e80
  392974: e5903004     	ldr	r3, [r0, #0x4]
  392978: e3530003     	cmp	r3, #3
  39297c: 1affffef     	bne	0x392940 <GameObject::_SetPosition(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)+0x50> @ imm = #-0x44
  392980: e5962004     	ldr	r2, [r6, #0x4]
  392984: e3063db7     	movw	r3, #0x6db7
  392988: e34b36db     	movt	r3, #0xb6db
  39298c: e5921004     	ldr	r1, [r2, #0x4]
  392990: e5922000     	ldr	r2, [r2]
  392994: e0622001     	rsb	r2, r2, r1
  392998: e1a02242     	asr	r2, r2, #4
  39299c: e0030293     	mul	r3, r3, r2
  3929a0: ea000032     	b	0x392a70 <GameObject::_SetPosition(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)+0x180> @ imm = #0xc8
  3929a4: e5913004     	ldr	r3, [r1, #0x4]
  3929a8: e3530004     	cmp	r3, #4
  3929ac: 1a000019     	bne	0x392a18 <GameObject::_SetPosition(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)+0x128> @ imm = #0x64
  3929b0: e1a00006     	mov	r0, r6
  3929b4: e3a01000     	mov	r1, #0
  3929b8: ebffa44e     	bl	0x37baf8 <sfc::script::lua::Arguments::operator[](unsigned int) const> @ imm = #-0x16ec8
  3929bc: e5903004     	ldr	r3, [r0, #0x4]
  3929c0: e3530004     	cmp	r3, #4
  3929c4: 0a000057     	beq	0x392b28 <GameObject::_SetPosition(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)+0x238> @ imm = #0x15c
  3929c8: e3a01000     	mov	r1, #0
  3929cc: e1a00006     	mov	r0, r6
  3929d0: ebffa448     	bl	0x37baf8 <sfc::script::lua::Arguments::operator[](unsigned int) const> @ imm = #-0x16ee0
  3929d4: ebfe22f1     	bl	0x31b5a0 <sfc::script::lua::Value::getUserData() const> @ imm = #-0x7743c
  3929d8: e1a05000     	mov	r5, r0
  3929dc: e3550000     	cmp	r5, #0
  3929e0: 0affffd6     	beq	0x392940 <GameObject::_SetPosition(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)+0x50> @ imm = #-0xa8
  3929e4: e1a00004     	mov	r0, r4
  3929e8: e2851e16     	add	r1, r5, #352
  3929ec: e3a02001     	mov	r2, #1
  3929f0: eb0004ef     	bl	0x393db4 <GameObject::SetPosition(Point3D<float> const&, bool)> @ imm = #0x13bc
  3929f4: e5953160     	ldr	r3, [r5, #0x160]
  3929f8: e1a00004     	mov	r0, r4
  3929fc: e58431e0     	str	r3, [r4, #0x1e0]
  392a00: e5953164     	ldr	r3, [r5, #0x164]
  392a04: e58431e4     	str	r3, [r4, #0x1e4]
  392a08: e5953168     	ldr	r3, [r5, #0x168]
  392a0c: e58431e8     	str	r3, [r4, #0x1e8]
  392a10: eb00051e     	bl	0x393e90 <GameObject::ForceUpdatePosition()> @ imm = #0x1478
  392a14: eaffffc9     	b	0x392940 <GameObject::_SetPosition(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)+0x50> @ imm = #-0xdc
  392a18: e3a01000     	mov	r1, #0
  392a1c: ebffa435     	bl	0x37baf8 <sfc::script::lua::Arguments::operator[](unsigned int) const> @ imm = #-0x16f2c
  392a20: e5903004     	ldr	r3, [r0, #0x4]
  392a24: e3530007     	cmp	r3, #7
  392a28: 0a000032     	beq	0x392af8 <GameObject::_SetPosition(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)+0x208> @ imm = #0xc8
  392a2c: e1a00006     	mov	r0, r6
  392a30: e3a01000     	mov	r1, #0
  392a34: ebffa42f     	bl	0x37baf8 <sfc::script::lua::Arguments::operator[](unsigned int) const> @ imm = #-0x16f44
  392a38: e5903004     	ldr	r3, [r0, #0x4]
  392a3c: e3530002     	cmp	r3, #2
  392a40: 1affffbe     	bne	0x392940 <GameObject::_SetPosition(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)+0x50> @ imm = #-0x108
  392a44: e5963004     	ldr	r3, [r6, #0x4]
  392a48: e5932004     	ldr	r2, [r3, #0x4]
  392a4c: e5933000     	ldr	r3, [r3]
  392a50: e0633002     	rsb	r3, r3, r2
  392a54: e1a03243     	asr	r3, r3, #4
  392a58: e0832183     	add	r2, r3, r3, lsl #3
  392a5c: e0822302     	add	r2, r2, r2, lsl #6
  392a60: e0832182     	add	r2, r3, r2, lsl #3
  392a64: e0822782     	add	r2, r2, r2, lsl #15
  392a68: e0833182     	add	r3, r3, r2, lsl #3
  392a6c: e2633000     	rsb	r3, r3, #0
  392a70: e3530001     	cmp	r3, #1
  392a74: 0affffcd     	beq	0x3929b0 <GameObject::_SetPosition(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)+0xc0> @ imm = #-0xcc
  392a78: e3530003     	cmp	r3, #3
  392a7c: 1affffaf     	bne	0x392940 <GameObject::_SetPosition(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)+0x50> @ imm = #-0x144
  392a80: e3a01000     	mov	r1, #0
  392a84: e1a00006     	mov	r0, r6
  392a88: ebffa41a     	bl	0x37baf8 <sfc::script::lua::Arguments::operator[](unsigned int) const> @ imm = #-0x16f98
  392a8c: ebfe2457     	bl	0x31bbf0 <sfc::script::lua::Value::getNumber() const> @ imm = #-0x76ea4
  392a90: e3a01001     	mov	r1, #1
  392a94: e1a07000     	mov	r7, r0
  392a98: e1a00006     	mov	r0, r6
  392a9c: ebffa415     	bl	0x37baf8 <sfc::script::lua::Arguments::operator[](unsigned int) const> @ imm = #-0x16fac
  392aa0: ebfe2452     	bl	0x31bbf0 <sfc::script::lua::Value::getNumber() const> @ imm = #-0x76eb8
  392aa4: e3a01002     	mov	r1, #2
  392aa8: e1a05000     	mov	r5, r0
  392aac: e1a00006     	mov	r0, r6
  392ab0: ebffa410     	bl	0x37baf8 <sfc::script::lua::Arguments::operator[](unsigned int) const> @ imm = #-0x16fc0
  392ab4: ebfe244d     	bl	0x31bbf0 <sfc::script::lua::Value::getNumber() const> @ imm = #-0x76ecc
  392ab8: e28d1008     	add	r1, sp, #8
  392abc: e58d0010     	str	r0, [sp, #0x10]
  392ac0: e3a02001     	mov	r2, #1
  392ac4: e1a00004     	mov	r0, r4
  392ac8: e58d7008     	str	r7, [sp, #0x8]
  392acc: e58d500c     	str	r5, [sp, #0xc]
  392ad0: eb0004b7     	bl	0x393db4 <GameObject::SetPosition(Point3D<float> const&, bool)> @ imm = #0x12dc
  392ad4: e59d200c     	ldr	r2, [sp, #0xc]
  392ad8: e59d3010     	ldr	r3, [sp, #0x10]
  392adc: e59d1008     	ldr	r1, [sp, #0x8]
  392ae0: e1a00004     	mov	r0, r4
  392ae4: e58421e4     	str	r2, [r4, #0x1e4]
  392ae8: e58411e0     	str	r1, [r4, #0x1e0]
  392aec: e58431e8     	str	r3, [r4, #0x1e8]
  392af0: eb0004e6     	bl	0x393e90 <GameObject::ForceUpdatePosition()> @ imm = #0x1398
  392af4: eaffff91     	b	0x392940 <GameObject::_SetPosition(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)+0x50> @ imm = #-0x1bc
  392af8: e5963004     	ldr	r3, [r6, #0x4]
  392afc: e5932004     	ldr	r2, [r3, #0x4]
  392b00: e5933000     	ldr	r3, [r3]
  392b04: e0632002     	rsb	r2, r3, r2
  392b08: e1a02242     	asr	r2, r2, #4
  392b0c: e0823182     	add	r3, r2, r2, lsl #3
  392b10: e0833303     	add	r3, r3, r3, lsl #6
  392b14: e0823183     	add	r3, r2, r3, lsl #3
  392b18: e0833783     	add	r3, r3, r3, lsl #15
  392b1c: e0823183     	add	r3, r2, r3, lsl #3
  392b20: e2633000     	rsb	r3, r3, #0
  392b24: eaffffd1     	b	0x392a70 <GameObject::_SetPosition(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)+0x180> @ imm = #-0xbc
  392b28: e59f304c     	ldr	r3, [pc, #0x4c]         @ 0x392b7c <GameObject::_SetPosition(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)+0x28c>
  392b2c: e3a01000     	mov	r1, #0
  392b30: e1a00006     	mov	r0, r6
  392b34: e7953003     	ldr	r3, [r5, r3]
  392b38: e28d5014     	add	r5, sp, #20
  392b3c: e5936038     	ldr	r6, [r3, #0x38]
  392b40: ebffa3ec     	bl	0x37baf8 <sfc::script::lua::Arguments::operator[](unsigned int) const> @ imm = #-0x17050
  392b44: ebfe2654     	bl	0x31c49c <sfc::script::lua::Value::getString() const> @ imm = #-0x766b0
  392b48: e3a0c000     	mov	r12, #0
  392b4c: e1a02000     	mov	r2, r0
  392b50: e1a01006     	mov	r1, r6
  392b54: e1a00005     	mov	r0, r5
  392b58: e3e03000     	mvn	r3, #0
  392b5c: e58dc004     	str	r12, [sp, #0x4]
  392b60: e58dc000     	str	r12, [sp]
  392b64: ebfee04d     	bl	0x34aca0 <ObjectManager::GetObjectByName(char const*, int, bool, char const*)> @ imm = #-0x47ecc
  392b68: e1a00005     	mov	r0, r5
  392b6c: ebfeb4dc     	bl	0x33fee4 <ObjectHandle::operator GameObject*()> @ imm = #-0x52c90
  392b70: e1a05000     	mov	r5, r0
  392b74: eaffff98     	b	0x3929dc <GameObject::_SetPosition(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)+0xec> @ imm = #-0x1a0
  392b78: 8c 21 60 00  	.word	0x0060218c
  392b7c: f4 37 00 00  	.word	0x000037f4
