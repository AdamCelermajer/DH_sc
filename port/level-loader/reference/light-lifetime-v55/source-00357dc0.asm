
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00357dc0 <SceneManager::clearRenderLists()>:
  357dc0: e92d40f0     	push	{r4, r5, r6, r7, lr}
  357dc4: e24dd05c     	sub	sp, sp, #92
  357dc8: e3a04000     	mov	r4, #0
  357dcc: e1a05000     	mov	r5, r0
  357dd0: e28d1050     	add	r1, sp, #80
  357dd4: e280003c     	add	r0, r0, #60
  357dd8: e58d4050     	str	r4, [sp, #0x50]
  357ddc: e58d4054     	str	r4, [sp, #0x54]
  357de0: e3a06000     	mov	r6, #0
  357de4: ebffec41     	bl	0x352ef0 <std::vector<glitch::scene::CSceneManager::SUnsortedNodeEntry, glitch::core::SAllocator<glitch::scene::CSceneManager::SUnsortedNodeEntry, (glitch::memory::E_MEMORY_HINT)0>>::resize(unsigned int, glitch::scene::CSceneManager::SUnsortedNodeEntry const&) (.clone.6)> @ imm = #-0x4efc
  357de8: e3a07000     	mov	r7, #0
  357dec: e28d2028     	add	r2, sp, #40
  357df0: e2850048     	add	r0, r5, #72
  357df4: e1a01004     	mov	r1, r4
  357df8: e1cd63f0     	strd	r6, r7, [sp, #48]
  357dfc: e58d4028     	str	r4, [sp, #0x28]
  357e00: e28d6018     	add	r6, sp, #24
  357e04: e58d402c     	str	r4, [sp, #0x2c]
  357e08: ebffe72a     	bl	0x351ab8 <std::vector<glitch::scene::CSceneManager::SDistanceNodeEntry, glitch::core::SAllocator<glitch::scene::CSceneManager::SDistanceNodeEntry, (glitch::memory::E_MEMORY_HINT)0>>::resize(unsigned int, glitch::scene::CSceneManager::SDistanceNodeEntry const&)> @ imm = #-0x6358
  357e0c: e285006c     	add	r0, r5, #108
  357e10: e28d1048     	add	r1, sp, #72
  357e14: e58d4048     	str	r4, [sp, #0x48]
  357e18: e58d404c     	str	r4, [sp, #0x4c]
  357e1c: ebffec33     	bl	0x352ef0 <std::vector<glitch::scene::CSceneManager::SUnsortedNodeEntry, glitch::core::SAllocator<glitch::scene::CSceneManager::SUnsortedNodeEntry, (glitch::memory::E_MEMORY_HINT)0>>::resize(unsigned int, glitch::scene::CSceneManager::SUnsortedNodeEntry const&) (.clone.6)> @ imm = #-0x4f34
  357e20: e1a01006     	mov	r1, r6
  357e24: e2850078     	add	r0, r5, #120
  357e28: e58d4018     	str	r4, [sp, #0x18]
  357e2c: e58d401c     	str	r4, [sp, #0x1c]
  357e30: e58d4020     	str	r4, [sp, #0x20]
  357e34: e58d4024     	str	r4, [sp, #0x24]
  357e38: ebfffe3d     	bl	0x357734 <std::vector<glitch::scene::CSceneManager::SDefaultNodeEntry, glitch::core::SAllocator<glitch::scene::CSceneManager::SDefaultNodeEntry, (glitch::memory::E_MEMORY_HINT)0>>::resize(unsigned int, glitch::scene::CSceneManager::SDefaultNodeEntry const&) (.clone.2)> @ imm = #-0x70c
  357e3c: e2860008     	add	r0, r6, #8
  357e40: ebffe7fd     	bl	0x351e3c <boost::intrusive_ptr<glitch::video::CMaterial const>::~intrusive_ptr()> @ imm = #-0x600c
  357e44: e2850054     	add	r0, r5, #84
  357e48: e28d1040     	add	r1, sp, #64
  357e4c: e28d6004     	add	r6, sp, #4
  357e50: e58d4040     	str	r4, [sp, #0x40]
  357e54: e58d4044     	str	r4, [sp, #0x44]
  357e58: ebffedcf     	bl	0x35359c <std::vector<glitch::scene::CSceneManager::SRenderDataSortNodeEntry, glitch::core::SAllocator<glitch::scene::CSceneManager::SRenderDataSortNodeEntry, (glitch::memory::E_MEMORY_HINT)0>>::resize(unsigned int, glitch::scene::CSceneManager::SRenderDataSortNodeEntry const&) (.clone.13)> @ imm = #-0x48c4
  357e5c: e2850060     	add	r0, r5, #96
  357e60: e28d1038     	add	r1, sp, #56
  357e64: e58d4038     	str	r4, [sp, #0x38]
  357e68: e58d403c     	str	r4, [sp, #0x3c]
  357e6c: ebffedca     	bl	0x35359c <std::vector<glitch::scene::CSceneManager::SRenderDataSortNodeEntry, glitch::core::SAllocator<glitch::scene::CSceneManager::SRenderDataSortNodeEntry, (glitch::memory::E_MEMORY_HINT)0>>::resize(unsigned int, glitch::scene::CSceneManager::SRenderDataSortNodeEntry const&) (.clone.13)> @ imm = #-0x48d8
  357e70: e2850084     	add	r0, r5, #132
  357e74: e3a03000     	mov	r3, #0
  357e78: e1a01006     	mov	r1, r6
  357e7c: e58d4010     	str	r4, [sp, #0x10]
  357e80: e58d3014     	str	r3, [sp, #0x14]
  357e84: e58d4004     	str	r4, [sp, #0x4]
  357e88: e58d4008     	str	r4, [sp, #0x8]
  357e8c: e58d400c     	str	r4, [sp, #0xc]
  357e90: ebffffb2     	bl	0x357d60 <std::vector<glitch::scene::CSceneManager::STransparentNodeEntry, glitch::core::SAllocator<glitch::scene::CSceneManager::STransparentNodeEntry, (glitch::memory::E_MEMORY_HINT)0>>::resize(unsigned int, glitch::scene::CSceneManager::STransparentNodeEntry const&) (.clone.3)> @ imm = #-0x138
  357e94: e2860008     	add	r0, r6, #8
  357e98: ebffe7e7     	bl	0x351e3c <boost::intrusive_ptr<glitch::video::CMaterial const>::~intrusive_ptr()> @ imm = #-0x6064
  357e9c: e28dd05c     	add	sp, sp, #92
  357ea0: e8bd80f0     	pop	{r4, r5, r6, r7, pc}
