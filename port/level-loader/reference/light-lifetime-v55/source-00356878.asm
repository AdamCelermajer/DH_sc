
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00356878 <SceneManager::~SceneManager()>:
  356878: e92d4070     	push	{r4, r5, r6, lr}
  35687c: e5913000     	ldr	r3, [r1]
  356880: e1a04000     	mov	r4, r0
  356884: e1a05001     	mov	r5, r1
  356888: e5803000     	str	r3, [r0]
  35688c: e513301c     	ldr	r3, [r3, #-0x1c]
  356890: e5912010     	ldr	r2, [r1, #0x10]
  356894: e7802003     	str	r2, [r0, r3]
  356898: e5903000     	ldr	r3, [r0]
  35689c: e5912014     	ldr	r2, [r1, #0x14]
  3568a0: e513300c     	ldr	r3, [r3, #-0xc]
  3568a4: e7802003     	str	r2, [r0, r3]
  3568a8: e590047c     	ldr	r0, [r0, #0x47c]
  3568ac: e3500000     	cmp	r0, #0
  3568b0: 0a000000     	beq	0x3568b8 <SceneManager::~SceneManager()+0x40> @ imm = #0x0
  3568b4: ebfee6e5     	bl	0x310450 <GlitchFree(void*)> @ imm = #-0x4646c
  3568b8: e2840e47     	add	r0, r4, #1136
  3568bc: ebffee04     	bl	0x3520d4 <std::vector<glitch::scene::CSceneManager::SDefaultNodeEntry, glitch::core::SAllocator<glitch::scene::CSceneManager::SDefaultNodeEntry, (glitch::memory::E_MEMORY_HINT)0>>::~vector()> @ imm = #-0x47f0
  3568c0: e2840e46     	add	r0, r4, #1120
  3568c4: e2800004     	add	r0, r0, #4
  3568c8: ebffee01     	bl	0x3520d4 <std::vector<glitch::scene::CSceneManager::SDefaultNodeEntry, glitch::core::SAllocator<glitch::scene::CSceneManager::SDefaultNodeEntry, (glitch::memory::E_MEMORY_HINT)0>>::~vector()> @ imm = #-0x47fc
  3568cc: e594044c     	ldr	r0, [r4, #0x44c]
  3568d0: e2843d11     	add	r3, r4, #1088
  3568d4: e283300c     	add	r3, r3, #12
  3568d8: e3500000     	cmp	r0, #0
  3568dc: 0a000005     	beq	0x3568f8 <SceneManager::~SceneManager()+0x80> @ imm = #0x14
  3568e0: e5931008     	ldr	r1, [r3, #0x8]
  3568e4: e0601001     	rsb	r1, r0, r1
  3568e8: e3c11003     	bic	r1, r1, #3
  3568ec: e3510080     	cmp	r1, #128
  3568f0: 8a000007     	bhi	0x356914 <SceneManager::~SceneManager()+0x9c> @ imm = #0x1c
  3568f4: eb0ec981     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x3b2604
  3568f8: e2840fa5     	add	r0, r4, #660
  3568fc: eb02d8f3     	bl	0x40ccd0 <LightSetManager::~LightSetManager()> @ imm = #0xb63cc
  356900: e1a00004     	mov	r0, r4
  356904: e2851004     	add	r1, r5, #4
  356908: eb08e183     	bl	0x58ef1c <glitch::scene::CSceneManager::~CSceneManager()> @ imm = #0x23860c
  35690c: e1a00004     	mov	r0, r4
  356910: e8bd8070     	pop	{r4, r5, r6, pc}
  356914: ebfee6c9     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x464dc
  356918: eafffff6     	b	0x3568f8 <SceneManager::~SceneManager()+0x80> @ imm = #-0x28
