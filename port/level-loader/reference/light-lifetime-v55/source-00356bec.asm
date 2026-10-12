
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00356bec <SceneManager::~SceneManager()>:
  356bec: e92d4070     	push	{r4, r5, r6, lr}
  356bf0: e59f5098     	ldr	r5, [pc, #0x98]         @ 0x356c90 <SceneManager::~SceneManager()+0xa4>
  356bf4: e59f3098     	ldr	r3, [pc, #0x98]         @ 0x356c94 <SceneManager::~SceneManager()+0xa8>
  356bf8: e1a04000     	mov	r4, r0
  356bfc: e08f5005     	add	r5, pc, r5
  356c00: e590047c     	ldr	r0, [r0, #0x47c]
  356c04: e7953003     	ldr	r3, [r5, r3]
  356c08: e3500000     	cmp	r0, #0
  356c0c: e28320c0     	add	r2, r3, #192
  356c10: e283301c     	add	r3, r3, #28
  356c14: e5843000     	str	r3, [r4]
  356c18: e584248c     	str	r2, [r4, #0x48c]
  356c1c: 0a000000     	beq	0x356c24 <SceneManager::~SceneManager()+0x38> @ imm = #0x0
  356c20: ebfee60a     	bl	0x310450 <GlitchFree(void*)> @ imm = #-0x467d8
  356c24: e2840e47     	add	r0, r4, #1136
  356c28: ebffed29     	bl	0x3520d4 <std::vector<glitch::scene::CSceneManager::SDefaultNodeEntry, glitch::core::SAllocator<glitch::scene::CSceneManager::SDefaultNodeEntry, (glitch::memory::E_MEMORY_HINT)0>>::~vector()> @ imm = #-0x4b5c
  356c2c: e2840e46     	add	r0, r4, #1120
  356c30: e2800004     	add	r0, r0, #4
  356c34: ebffed26     	bl	0x3520d4 <std::vector<glitch::scene::CSceneManager::SDefaultNodeEntry, glitch::core::SAllocator<glitch::scene::CSceneManager::SDefaultNodeEntry, (glitch::memory::E_MEMORY_HINT)0>>::~vector()> @ imm = #-0x4b68
  356c38: e594044c     	ldr	r0, [r4, #0x44c]
  356c3c: e2843d11     	add	r3, r4, #1088
  356c40: e283300c     	add	r3, r3, #12
  356c44: e3500000     	cmp	r0, #0
  356c48: 0a000005     	beq	0x356c64 <SceneManager::~SceneManager()+0x78> @ imm = #0x14
  356c4c: e5931008     	ldr	r1, [r3, #0x8]
  356c50: e0601001     	rsb	r1, r0, r1
  356c54: e3c11003     	bic	r1, r1, #3
  356c58: e3510080     	cmp	r1, #128
  356c5c: 8a000009     	bhi	0x356c88 <SceneManager::~SceneManager()+0x9c> @ imm = #0x24
  356c60: eb0ec8a6     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x3b2298
  356c64: e2840fa5     	add	r0, r4, #660
  356c68: eb02d818     	bl	0x40ccd0 <LightSetManager::~LightSetManager()> @ imm = #0xb6060
  356c6c: e59f1024     	ldr	r1, [pc, #0x24]         @ 0x356c98 <SceneManager::~SceneManager()+0xac>
  356c70: e1a00004     	mov	r0, r4
  356c74: e7951001     	ldr	r1, [r5, r1]
  356c78: e2811004     	add	r1, r1, #4
  356c7c: eb08e0a6     	bl	0x58ef1c <glitch::scene::CSceneManager::~CSceneManager()> @ imm = #0x238298
  356c80: e1a00004     	mov	r0, r4
  356c84: e8bd8070     	pop	{r4, r5, r6, pc}
  356c88: ebfee5ec     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x46850
  356c8c: eafffff4     	b	0x356c64 <SceneManager::~SceneManager()+0x78> @ imm = #-0x30
  356c90: 94 de 63 00  	.word	0x0063de94
  356c94: 00 42 00 00  	.word	0x00004200
  356c98: 40 31 00 00  	.word	0x00003140
