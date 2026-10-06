
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003b9084 <Character::_SetMaster(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)>:
  3b9084: e92d4010     	push	{r4, lr}
  3b9088: e5903004     	ldr	r3, [r0, #0x4]
  3b908c: e8930003     	ldm	r3, {r0, r1}
  3b9090: e0603001     	rsb	r3, r0, r1
  3b9094: e1a03243     	asr	r3, r3, #4
  3b9098: e0831183     	add	r1, r3, r3, lsl #3
  3b909c: e0811301     	add	r1, r1, r1, lsl #6
  3b90a0: e0831181     	add	r1, r3, r1, lsl #3
  3b90a4: e0811781     	add	r1, r1, r1, lsl #15
  3b90a8: e0833181     	add	r3, r3, r1, lsl #3
  3b90ac: e3530000     	cmp	r3, #0
  3b90b0: 1a000000     	bne	0x3b90b8 <Character::_SetMaster(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)+0x34> @ imm = #0x0
  3b90b4: e8bd8010     	pop	{r4, pc}
  3b90b8: e5903004     	ldr	r3, [r0, #0x4]
  3b90bc: e3530002     	cmp	r3, #2
  3b90c0: 0a000001     	beq	0x3b90cc <Character::_SetMaster(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)+0x48> @ imm = #0x4
  3b90c4: e3530007     	cmp	r3, #7
  3b90c8: 1afffff9     	bne	0x3b90b4 <Character::_SetMaster(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)+0x30> @ imm = #-0x1c
  3b90cc: e2824ff2     	add	r4, r2, #968
  3b90d0: ebfd8932     	bl	0x31b5a0 <sfc::script::lua::Value::getUserData() const> @ imm = #-0x9db38
  3b90d4: e1a01000     	mov	r1, r0
  3b90d8: e1a00004     	mov	r0, r4
  3b90dc: e8bd4010     	pop	{r4, lr}
  3b90e0: ea006f26     	b	0x3d4d80 <CharAI::AI_SetMaster(Character*)> @ imm = #0x1bc98
