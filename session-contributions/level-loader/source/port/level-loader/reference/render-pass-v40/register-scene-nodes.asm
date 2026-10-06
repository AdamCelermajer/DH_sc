
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00357ea4 <SceneManager::_registerSceneNodes(glitch::scene::ISceneNode*)>:
  357ea4: e92d4070     	push	{r4, r5, r6, lr}
  357ea8: e1a04000     	mov	r4, r0
  357eac: e5900440     	ldr	r0, [r0, #0x440]
  357eb0: e59f213c     	ldr	r2, [pc, #0x13c]        @ 0x357ff4 <SceneManager::_registerSceneNodes(glitch::scene::ISceneNode*)+0x150>
  357eb4: e59f313c     	ldr	r3, [pc, #0x13c]        @ 0x357ff8 <SceneManager::_registerSceneNodes(glitch::scene::ISceneNode*)+0x154>
  357eb8: e2800001     	add	r0, r0, #1
  357ebc: e5840440     	str	r0, [r4, #0x440]
  357ec0: e08f2002     	add	r2, pc, r2
  357ec4: e5d2200c     	ldrb	r2, [r2, #0xc]
  357ec8: e08f3003     	add	r3, pc, r3
  357ecc: e1a06001     	mov	r6, r1
  357ed0: e3520000     	cmp	r2, #0
  357ed4: 0a00003f     	beq	0x357fd8 <SceneManager::_registerSceneNodes(glitch::scene::ISceneNode*)+0x134> @ imm = #0xfc
  357ed8: e59f211c     	ldr	r2, [pc, #0x11c]        @ 0x357ffc <SceneManager::_registerSceneNodes(glitch::scene::ISceneNode*)+0x158>
  357edc: e7932002     	ldr	r2, [r3, r2]
  357ee0: e3a01000     	mov	r1, #0
  357ee4: e5841440     	str	r1, [r4, #0x440]
  357ee8: e5d22030     	ldrb	r2, [r2, #0x30]
  357eec: e1520001     	cmp	r2, r1
  357ef0: 13a01001     	movne	r1, #1
  357ef4: 0a00003c     	beq	0x357fec <SceneManager::_registerSceneNodes(glitch::scene::ISceneNode*)+0x148> @ imm = #0xf0
  357ef8: e59f2100     	ldr	r2, [pc, #0x100]        @ 0x358000 <SceneManager::_registerSceneNodes(glitch::scene::ISceneNode*)+0x15c>
  357efc: e08f2002     	add	r2, pc, r2
  357f00: e5c2100c     	strb	r1, [r2, #0xc]
  357f04: e59f20f8     	ldr	r2, [pc, #0xf8]         @ 0x358004 <SceneManager::_registerSceneNodes(glitch::scene::ISceneNode*)+0x160>
  357f08: e7930002     	ldr	r0, [r3, r2]
  357f0c: ebff1da0     	bl	0x31f594 <Application::GetCurrentLevel() const> @ imm = #-0x38980
  357f10: e5d43289     	ldrb	r3, [r4, #0x289]
  357f14: e1a05000     	mov	r5, r0
  357f18: e3530000     	cmp	r3, #0
  357f1c: 1a00001f     	bne	0x357fa0 <SceneManager::_registerSceneNodes(glitch::scene::ISceneNode*)+0xfc> @ imm = #0x7c
  357f20: e5940440     	ldr	r0, [r4, #0x440]
  357f24: e5941444     	ldr	r1, [r4, #0x444]
  357f28: ebfeda75     	bl	0x30e904 <.plt+0xb90>   @ imm = #-0x4962c
  357f2c: e3510000     	cmp	r1, #0
  357f30: 0a00001a     	beq	0x357fa0 <SceneManager::_registerSceneNodes(glitch::scene::ISceneNode*)+0xfc> @ imm = #0x68
  357f34: e3550000     	cmp	r5, #0
  357f38: 0a000006     	beq	0x357f58 <SceneManager::_registerSceneNodes(glitch::scene::ISceneNode*)+0xb4> @ imm = #0x18
  357f3c: e5953158     	ldr	r3, [r5, #0x158]
  357f40: e3530000     	cmp	r3, #0
  357f44: 0a000003     	beq	0x357f58 <SceneManager::_registerSceneNodes(glitch::scene::ISceneNode*)+0xb4> @ imm = #0xc
  357f48: e5930034     	ldr	r0, [r3, #0x34]
  357f4c: e3500000     	cmp	r0, #0
  357f50: 0a000000     	beq	0x357f58 <SceneManager::_registerSceneNodes(glitch::scene::ISceneNode*)+0xb4> @ imm = #0x0
  357f54: eb06d5f0     	bl	0x50d71c <batch::GOBatchSceneNode::updateSegmentVisibility()> @ imm = #0x1b57c0
  357f58: e594347c     	ldr	r3, [r4, #0x47c]
  357f5c: e5946480     	ldr	r6, [r4, #0x480]
  357f60: e0636006     	rsb	r6, r3, r6
  357f64: e1a06146     	asr	r6, r6, #2
  357f68: e3560000     	cmp	r6, #0
  357f6c: da00000a     	ble	0x357f9c <SceneManager::_registerSceneNodes(glitch::scene::ISceneNode*)+0xf8> @ imm = #0x28
  357f70: e3a05000     	mov	r5, #0
  357f74: ea000000     	b	0x357f7c <SceneManager::_registerSceneNodes(glitch::scene::ISceneNode*)+0xd8> @ imm = #0x0
  357f78: e594347c     	ldr	r3, [r4, #0x47c]
  357f7c: e7933105     	ldr	r3, [r3, r5, lsl #2]
  357f80: e2855001     	add	r5, r5, #1
  357f84: e1a00003     	mov	r0, r3
  357f88: e5933000     	ldr	r3, [r3]
  357f8c: e1a0e00f     	mov	lr, pc
  357f90: e593f000     	ldr	pc, [r3]
  357f94: e1550006     	cmp	r5, r6
  357f98: 1afffff6     	bne	0x357f78 <SceneManager::_registerSceneNodes(glitch::scene::ISceneNode*)+0xd4> @ imm = #-0x28
  357f9c: e8bd8070     	pop	{r4, r5, r6, pc}
  357fa0: e1a00004     	mov	r0, r4
  357fa4: ebffff85     	bl	0x357dc0 <SceneManager::clearRenderLists()> @ imm = #-0x1ec
  357fa8: e594347c     	ldr	r3, [r4, #0x47c]
  357fac: e5942480     	ldr	r2, [r4, #0x480]
  357fb0: e1a01006     	mov	r1, r6
  357fb4: e1a00004     	mov	r0, r4
  357fb8: e1530002     	cmp	r3, r2
  357fbc: 15843480     	strne	r3, [r4, #0x480]
  357fc0: eb08ce31     	bl	0x58b88c <glitch::scene::CSceneManager::registerSceneNodes(glitch::scene::ISceneNode*)> @ imm = #0x2338c4
  357fc4: e3a03000     	mov	r3, #0
  357fc8: e5c43448     	strb	r3, [r4, #0x448]
  357fcc: e5843440     	str	r3, [r4, #0x440]
  357fd0: e5c43289     	strb	r3, [r4, #0x289]
  357fd4: e8bd8070     	pop	{r4, r5, r6, pc}
  357fd8: e59f201c     	ldr	r2, [pc, #0x1c]         @ 0x357ffc <SceneManager::_registerSceneNodes(glitch::scene::ISceneNode*)+0x158>
  357fdc: e7931002     	ldr	r1, [r3, r2]
  357fe0: e5d11030     	ldrb	r1, [r1, #0x30]
  357fe4: e3510000     	cmp	r1, #0
  357fe8: 1affffbb     	bne	0x357edc <SceneManager::_registerSceneNodes(glitch::scene::ISceneNode*)+0x38> @ imm = #-0x114
  357fec: e5d41448     	ldrb	r1, [r4, #0x448]
  357ff0: eaffffc0     	b	0x357ef8 <SceneManager::_registerSceneNodes(glitch::scene::ISceneNode*)+0x54> @ imm = #-0x100
  357ff4: 28 a0 64 00  	.word	0x0064a028
  357ff8: c8 cb 63 00  	.word	0x0063cbc8
  357ffc: 20 1a 00 00  	.word	0x00001a20
  358000: ec 9f 64 00  	.word	0x00649fec
  358004: f4 37 00 00  	.word	0x000037f4
