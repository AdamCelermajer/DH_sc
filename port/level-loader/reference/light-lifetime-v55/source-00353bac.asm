
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00353bac <SceneManager::_RegisterAutomacticLights()>:
  353bac: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  353bb0: e5903048     	ldr	r3, [r0, #0x48]
  353bb4: e590204c     	ldr	r2, [r0, #0x4c]
  353bb8: e5906428     	ldr	r6, [r0, #0x428]
  353bbc: e24dd024     	sub	sp, sp, #36
  353bc0: e1530002     	cmp	r3, r2
  353bc4: 1580304c     	strne	r3, [r0, #0x4c]
  353bc8: e5903424     	ldr	r3, [r0, #0x424]
  353bcc: e1a04000     	mov	r4, r0
  353bd0: e0636006     	rsb	r6, r3, r6
  353bd4: e1a06146     	asr	r6, r6, #2
  353bd8: e3560000     	cmp	r6, #0
  353bdc: da000024     	ble	0x353c74 <SceneManager::_RegisterAutomacticLights()+0xc8> @ imm = #0x90
  353be0: e3a05000     	mov	r5, #0
  353be4: e2809048     	add	r9, r0, #72
  353be8: e28d8008     	add	r8, sp, #8
  353bec: e1a07005     	mov	r7, r5
  353bf0: e28db01c     	add	r11, sp, #28
  353bf4: e3a0a001     	mov	r10, #1
  353bf8: ea000008     	b	0x353c20 <SceneManager::_RegisterAutomacticLights()+0x74> @ imm = #0x20
  353bfc: e898000f     	ldm	r8, {r0, r1, r2, r3}
  353c00: e88c000f     	stm	r12, {r0, r1, r2, r3}
  353c04: e594304c     	ldr	r3, [r4, #0x4c]
  353c08: e2855001     	add	r5, r5, #1
  353c0c: e1550006     	cmp	r5, r6
  353c10: e2833010     	add	r3, r3, #16
  353c14: e584304c     	str	r3, [r4, #0x4c]
  353c18: 0a000015     	beq	0x353c74 <SceneManager::_RegisterAutomacticLights()+0xc8> @ imm = #0x54
  353c1c: e5943424     	ldr	r3, [r4, #0x424]
  353c20: e7933105     	ldr	r3, [r3, r5, lsl #2]
  353c24: e1a00005     	mov	r0, r5
  353c28: e5933120     	ldr	r3, [r3, #0x120]
  353c2c: e58d700c     	str	r7, [sp, #0xc]
  353c30: e58d3008     	str	r3, [sp, #0x8]
  353c34: ebfeec3d     	bl	0x30ed30 <.plt+0xfbc>   @ imm = #-0x44f0c
  353c38: e594c04c     	ldr	r12, [r4, #0x4c]
  353c3c: e5943050     	ldr	r3, [r4, #0x50]
  353c40: e1cd01f0     	strd	r0, r1, [sp, #16]
  353c44: e15c0003     	cmp	r12, r3
  353c48: 1affffeb     	bne	0x353bfc <SceneManager::_RegisterAutomacticLights()+0x50> @ imm = #-0x54
  353c4c: e1a0100c     	mov	r1, r12
  353c50: e1a00009     	mov	r0, r9
  353c54: e1a02008     	mov	r2, r8
  353c58: e1a0300b     	mov	r3, r11
  353c5c: e2855001     	add	r5, r5, #1
  353c60: e58da000     	str	r10, [sp]
  353c64: e58da004     	str	r10, [sp, #0x4]
  353c68: ebfff72c     	bl	0x351920 <std::vector<glitch::scene::CSceneManager::SDistanceNodeEntry, glitch::core::SAllocator<glitch::scene::CSceneManager::SDistanceNodeEntry, (glitch::memory::E_MEMORY_HINT)0>>::_M_insert_overflow_aux(glitch::scene::CSceneManager::SDistanceNodeEntry*, glitch::scene::CSceneManager::SDistanceNodeEntry const&, std::__false_type const&, unsigned int, bool)> @ imm = #-0x2350
  353c6c: e1550006     	cmp	r5, r6
  353c70: 1affffe9     	bne	0x353c1c <SceneManager::_RegisterAutomacticLights()+0x70> @ imm = #-0x5c
  353c74: e28dd024     	add	sp, sp, #36
  353c78: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
