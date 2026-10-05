
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00520b40 <PFFloor::_LoadNavMesh(glitch::scene::IMeshSceneNode*)>:
  520b40: e92d47f0     	push	{r4, r5, r6, r7, r8, r9, r10, lr}
  520b44: e1a04000     	mov	r4, r0
  520b48: e24dd038     	sub	sp, sp, #56
  520b4c: e1a00001     	mov	r0, r1
  520b50: e1a05001     	mov	r5, r1
  520b54: eb01d9cd     	bl	0x597290 <glitch::scene::ISceneNode::getParent() const> @ imm = #0x76734
  520b58: e5903000     	ldr	r3, [r0]
  520b5c: e1a0e00f     	mov	lr, pc
  520b60: e593f0ac     	ldr	pc, [r3, #0xac]
  520b64: e59fa400     	ldr	r10, [pc, #0x400]       @ 0x520f6c <PFFloor::_LoadNavMesh(glitch::scene::IMeshSceneNode*)+0x42c>
  520b68: e28d8008     	add	r8, sp, #8
  520b6c: e1a01000     	mov	r1, r0
  520b70: e28d7038     	add	r7, sp, #56
  520b74: e08fa00a     	add	r10, pc, r10
  520b78: e1a00008     	mov	r0, r8
  520b7c: ebf7e175     	bl	0x319158 <UserProperties::UserProperties(char const*)> @ imm = #-0x207a2c
  520b80: e2889004     	add	r9, r8, #4
  520b84: e527a008     	str	r10, [r7, #-0x8]!
  520b88: e1a00009     	mov	r0, r9
  520b8c: e1a01007     	mov	r1, r7
  520b90: ebfff108     	bl	0x51cfb8 <std::priv::_Rb_tree_node_base* std::priv::_Rb_tree<std::string, std::less<std::string>, std::pair<std::string const, std::string>, std::priv::_Select1st<std::pair<std::string const, std::string>>, std::priv::_MapTraitsT<std::pair<std::string const, std::string>>, std::allocator<std::pair<std::string const, std::string>>>::_M_find<char const*>(char const* const&) const> @ imm = #-0x3be0
  520b94: e59f63d4     	ldr	r6, [pc, #0x3d4]        @ 0x520f70 <PFFloor::_LoadNavMesh(glitch::scene::IMeshSceneNode*)+0x430>
  520b98: e1590000     	cmp	r9, r0
  520b9c: e08f6006     	add	r6, pc, r6
  520ba0: 0a00000b     	beq	0x520bd4 <PFFloor::_LoadNavMesh(glitch::scene::IMeshSceneNode*)+0x94> @ imm = #0x2c
  520ba4: e1a01007     	mov	r1, r7
  520ba8: e1a00009     	mov	r0, r9
  520bac: e58da030     	str	r10, [sp, #0x30]
  520bb0: ebfff100     	bl	0x51cfb8 <std::priv::_Rb_tree_node_base* std::priv::_Rb_tree<std::string, std::less<std::string>, std::pair<std::string const, std::string>, std::priv::_Select1st<std::pair<std::string const, std::string>>, std::priv::_MapTraitsT<std::pair<std::string const, std::string>>, std::allocator<std::pair<std::string const, std::string>>>::_M_find<char const*>(char const* const&) const> @ imm = #-0x3c00
  520bb4: e590903c     	ldr	r9, [r0, #0x3c]
  520bb8: e284a028     	add	r10, r4, #40
  520bbc: e1a00009     	mov	r0, r9
  520bc0: ebf7b4a3     	bl	0x30de54 <.plt+0xe0>    @ imm = #-0x212d74
  520bc4: e1a01009     	mov	r1, r9
  520bc8: e0892000     	add	r2, r9, r0
  520bcc: e1a0000a     	mov	r0, r10
  520bd0: ebf7bf82     	bl	0x3109e0 <std::string::_M_assign(char const*, char const*)> @ imm = #-0x2101f8
  520bd4: e594903c     	ldr	r9, [r4, #0x3c]
  520bd8: e59f1394     	ldr	r1, [pc, #0x394]        @ 0x520f74 <PFFloor::_LoadNavMesh(glitch::scene::IMeshSceneNode*)+0x434>
  520bdc: e1a00009     	mov	r0, r9
  520be0: e08f1001     	add	r1, pc, r1
  520be4: ebf7b7fa     	bl	0x30ebd4 <.plt+0xe60>   @ imm = #-0x212018
  520be8: e594a024     	ldr	r10, [r4, #0x24]
  520bec: e59f1384     	ldr	r1, [pc, #0x384]        @ 0x520f78 <PFFloor::_LoadNavMesh(glitch::scene::IMeshSceneNode*)+0x438>
  520bf0: e3500000     	cmp	r0, #0
  520bf4: 138aa401     	orrne	r10, r10, #16777216
  520bf8: 1584a024     	strne	r10, [r4, #0x24]
  520bfc: e08f1001     	add	r1, pc, r1
  520c00: e1a00009     	mov	r0, r9
  520c04: ebf7b7f2     	bl	0x30ebd4 <.plt+0xe60>   @ imm = #-0x212038
  520c08: e59f136c     	ldr	r1, [pc, #0x36c]        @ 0x520f7c <PFFloor::_LoadNavMesh(glitch::scene::IMeshSceneNode*)+0x43c>
  520c0c: e3500000     	cmp	r0, #0
  520c10: 138aa402     	orrne	r10, r10, #33554432
  520c14: 1584a024     	strne	r10, [r4, #0x24]
  520c18: e08f1001     	add	r1, pc, r1
  520c1c: e1a00009     	mov	r0, r9
  520c20: ebf7b7eb     	bl	0x30ebd4 <.plt+0xe60>   @ imm = #-0x212054
  520c24: e59f1354     	ldr	r1, [pc, #0x354]        @ 0x520f80 <PFFloor::_LoadNavMesh(glitch::scene::IMeshSceneNode*)+0x440>
  520c28: e3500000     	cmp	r0, #0
  520c2c: 138aa001     	orrne	r10, r10, #1
  520c30: 1584a024     	strne	r10, [r4, #0x24]
  520c34: e08f1001     	add	r1, pc, r1
  520c38: e1a00009     	mov	r0, r9
  520c3c: ebf7b7e4     	bl	0x30ebd4 <.plt+0xe60>   @ imm = #-0x212070
  520c40: e3500000     	cmp	r0, #0
  520c44: 138aa002     	orrne	r10, r10, #2
  520c48: 1584a024     	strne	r10, [r4, #0x24]
  520c4c: e31a0403     	tst	r10, #50331648
  520c50: 15943020     	ldrne	r3, [r4, #0x20]
  520c54: e1a00005     	mov	r0, r5
  520c58: 13833407     	orrne	r3, r3, #117440512
  520c5c: 15843020     	strne	r3, [r4, #0x20]
  520c60: eb01d98a     	bl	0x597290 <glitch::scene::ISceneNode::getParent() const> @ imm = #0x76628
  520c64: e3500000     	cmp	r0, #0
  520c68: 0a0000aa     	beq	0x520f18 <PFFloor::_LoadNavMesh(glitch::scene::IMeshSceneNode*)+0x3d8> @ imm = #0x2a8
  520c6c: e1a00005     	mov	r0, r5
  520c70: eb01d986     	bl	0x597290 <glitch::scene::ISceneNode::getParent() const> @ imm = #0x76618
  520c74: e3500000     	cmp	r0, #0
  520c78: 0a000008     	beq	0x520ca0 <PFFloor::_LoadNavMesh(glitch::scene::IMeshSceneNode*)+0x160> @ imm = #0x20
  520c7c: e5953000     	ldr	r3, [r5]
  520c80: e28d6024     	add	r6, sp, #36
  520c84: e1a00006     	mov	r0, r6
  520c88: e1a01005     	mov	r1, r5
  520c8c: e593a0a4     	ldr	r10, [r3, #0xa4]
  520c90: eb01d93a     	bl	0x597180 <glitch::scene::ISceneNode::getAbsolutePosition() const> @ imm = #0x764e8
  520c94: e1a00005     	mov	r0, r5
  520c98: e1a01006     	mov	r1, r6
  520c9c: e12fff3a     	blx	r10
  520ca0: e1a00005     	mov	r0, r5
  520ca4: ebffbafc     	bl	0x50f89c <CopyMeshSceneNode(glitch::scene::IMeshSceneNode*)> @ imm = #-0x11410
  520ca8: e5840040     	str	r0, [r4, #0x40]
  520cac: e3a01000     	mov	r1, #0
  520cb0: e1a00005     	mov	r0, r5
  520cb4: e5953000     	ldr	r3, [r5]
  520cb8: e1a0e00f     	mov	lr, pc
  520cbc: e593f048     	ldr	pc, [r3, #0x48]
  520cc0: e1a00005     	mov	r0, r5
  520cc4: e5953000     	ldr	r3, [r5]
  520cc8: e1a0e00f     	mov	lr, pc
  520ccc: e593f068     	ldr	pc, [r3, #0x68]
  520cd0: e5943040     	ldr	r3, [r4, #0x40]
  520cd4: e1a00003     	mov	r0, r3
  520cd8: e5933000     	ldr	r3, [r3]
  520cdc: e1a0e00f     	mov	lr, pc
  520ce0: e593f0a0     	ldr	pc, [r3, #0xa0]
  520ce4: e5901000     	ldr	r1, [r0]
  520ce8: e1a03000     	mov	r3, r0
  520cec: e5942040     	ldr	r2, [r4, #0x40]
  520cf0: e584105c     	str	r1, [r4, #0x5c]
  520cf4: e5901004     	ldr	r1, [r0, #0x4]
  520cf8: e1a00002     	mov	r0, r2
  520cfc: e5841060     	str	r1, [r4, #0x60]
  520d00: e5933008     	ldr	r3, [r3, #0x8]
  520d04: e5843064     	str	r3, [r4, #0x64]
  520d08: e5923000     	ldr	r3, [r2]
  520d0c: e1a0e00f     	mov	lr, pc
  520d10: e593f034     	ldr	pc, [r3, #0x34]
  520d14: e5903000     	ldr	r3, [r0]
  520d18: e3a01311     	mov	r1, #1140850688
  520d1c: e281187a     	add	r1, r1, #7995392
  520d20: e5843044     	str	r3, [r4, #0x44]
  520d24: e5903004     	ldr	r3, [r0, #0x4]
  520d28: e5843048     	str	r3, [r4, #0x48]
  520d2c: e5905008     	ldr	r5, [r0, #0x8]
  520d30: e584504c     	str	r5, [r4, #0x4c]
  520d34: e590300c     	ldr	r3, [r0, #0xc]
  520d38: e5843050     	str	r3, [r4, #0x50]
  520d3c: e5903010     	ldr	r3, [r0, #0x10]
  520d40: e5843054     	str	r3, [r4, #0x54]
  520d44: e5900014     	ldr	r0, [r0, #0x14]
  520d48: ebf7b795     	bl	0x30eba4 <.plt+0xe30>   @ imm = #-0x2121ac
  520d4c: e3a01311     	mov	r1, #1140850688
  520d50: e5840058     	str	r0, [r4, #0x58]
  520d54: e281187a     	add	r1, r1, #7995392
  520d58: e1a00005     	mov	r0, r5
  520d5c: ebf7b592     	bl	0x30e3ac <.plt+0x638>   @ imm = #-0x2129b8
  520d60: e5943040     	ldr	r3, [r4, #0x40]
  520d64: e584004c     	str	r0, [r4, #0x4c]
  520d68: e28d0034     	add	r0, sp, #52
  520d6c: e1a01003     	mov	r1, r3
  520d70: e5933000     	ldr	r3, [r3]
  520d74: e1a0e00f     	mov	lr, pc
  520d78: e593f0f8     	ldr	pc, [r3, #0xf8]
  520d7c: e3a01000     	mov	r1, #0
  520d80: e3a000b8     	mov	r0, #184
  520d84: e59d5034     	ldr	r5, [sp, #0x34]
  520d88: eb004d07     	bl	0x5341ac <operator new(unsigned int, glitch::memory::E_MEMORY_HINT)> @ imm = #0x1341c
  520d8c: e5942040     	ldr	r2, [r4, #0x40]
  520d90: e3a0c001     	mov	r12, #1
  520d94: e1a01005     	mov	r1, r5
  520d98: e3a0300f     	mov	r3, #15
  520d9c: e1a06000     	mov	r6, r0
  520da0: e58dc000     	str	r12, [sp]
  520da4: eb019e2a     	bl	0x588654 <glitch::scene::COctTreeTriangleSelector::COctTreeTriangleSelector(glitch::scene::IMesh const*, glitch::scene::ISceneNode const*, int, bool)> @ imm = #0x678a8
  520da8: e59d0034     	ldr	r0, [sp, #0x34]
  520dac: e3500000     	cmp	r0, #0
  520db0: 0a000000     	beq	0x520db8 <PFFloor::_LoadNavMesh(glitch::scene::IMeshSceneNode*)+0x278> @ imm = #0x0
  520db4: ebf7f1f2     	bl	0x31d584 <glitch::IReferenceCounted::drop() const> @ imm = #-0x203838
  520db8: e5943040     	ldr	r3, [r4, #0x40]
  520dbc: e1a01006     	mov	r1, r6
  520dc0: e1a00003     	mov	r0, r3
  520dc4: e5933000     	ldr	r3, [r3]
  520dc8: e1a0e00f     	mov	lr, pc
  520dcc: e593f0b4     	ldr	pc, [r3, #0xb4]
  520dd0: e1a00006     	mov	r0, r6
  520dd4: ebf7f1ea     	bl	0x31d584 <glitch::IReferenceCounted::drop() const> @ imm = #-0x203858
  520dd8: e5963000     	ldr	r3, [r6]
  520ddc: e1a00006     	mov	r0, r6
  520de0: e1a0e00f     	mov	lr, pc
  520de4: e593f00c     	ldr	pc, [r3, #0xc]
  520de8: e3500000     	cmp	r0, #0
  520dec: e1a0a000     	mov	r10, r0
  520df0: e58d0030     	str	r0, [sp, #0x30]
  520df4: d3a05000     	movle	r5, #0
  520df8: da00001f     	ble	0x520e7c <PFFloor::_LoadNavMesh(glitch::scene::IMeshSceneNode*)+0x33c> @ imm = #0x7c
  520dfc: e3a00024     	mov	r0, #36
  520e00: e3a01000     	mov	r1, #0
  520e04: e0000a90     	mul	r0, r0, r10
  520e08: ebf7bdd7     	bl	0x31056c <operator new[](unsigned int, MemoryHintState)> @ imm = #-0x2108a4
  520e0c: e3a02000     	mov	r2, #0
  520e10: e1a05000     	mov	r5, r0
  520e14: e1a03000     	mov	r3, r0
  520e18: e3a01000     	mov	r1, #0
  520e1c: ea000000     	b	0x520e24 <PFFloor::_LoadNavMesh(glitch::scene::IMeshSceneNode*)+0x2e4> @ imm = #0x0
  520e20: e2833024     	add	r3, r3, #36
  520e24: e2811001     	add	r1, r1, #1
  520e28: e15a0001     	cmp	r10, r1
  520e2c: e5832000     	str	r2, [r3]
  520e30: e5832004     	str	r2, [r3, #0x4]
  520e34: e5832008     	str	r2, [r3, #0x8]
  520e38: e583200c     	str	r2, [r3, #0xc]
  520e3c: e5832010     	str	r2, [r3, #0x10]
  520e40: e5832014     	str	r2, [r3, #0x14]
  520e44: e5832018     	str	r2, [r3, #0x18]
  520e48: e583201c     	str	r2, [r3, #0x1c]
  520e4c: e5832020     	str	r2, [r3, #0x20]
  520e50: 1afffff2     	bne	0x520e20 <PFFloor::_LoadNavMesh(glitch::scene::IMeshSceneNode*)+0x2e0> @ imm = #-0x38
  520e54: e3a01000     	mov	r1, #0
  520e58: e596c000     	ldr	r12, [r6]
  520e5c: e1a00006     	mov	r0, r6
  520e60: e58d1000     	str	r1, [sp]
  520e64: e59d2030     	ldr	r2, [sp, #0x30]
  520e68: e1a03007     	mov	r3, r7
  520e6c: e1a01005     	mov	r1, r5
  520e70: e1a0e00f     	mov	lr, pc
  520e74: e59cf010     	ldr	pc, [r12, #0x10]
  520e78: e59da030     	ldr	r10, [sp, #0x30]
  520e7c: e1a0200a     	mov	r2, r10
  520e80: e1a00004     	mov	r0, r4
  520e84: e1a01005     	mov	r1, r5
  520e88: ebfffdbe     	bl	0x520588 <PFFloor::_CreateNodes(glitch::core::triangle3d<float>*, unsigned int)> @ imm = #-0x908
  520e8c: e59d3030     	ldr	r3, [sp, #0x30]
  520e90: e5845068     	str	r5, [r4, #0x68]
  520e94: e3530000     	cmp	r3, #0
  520e98: e584306c     	str	r3, [r4, #0x6c]
  520e9c: 0a000019     	beq	0x520f08 <PFFloor::_LoadNavMesh(glitch::scene::IMeshSceneNode*)+0x3c8> @ imm = #0x64
  520ea0: e3a06000     	mov	r6, #0
  520ea4: e1a07006     	mov	r7, r6
  520ea8: ea000000     	b	0x520eb0 <PFFloor::_LoadNavMesh(glitch::scene::IMeshSceneNode*)+0x370> @ imm = #0x0
  520eac: e5945068     	ldr	r5, [r4, #0x68]
  520eb0: e0855006     	add	r5, r5, r6
  520eb4: e5950008     	ldr	r0, [r5, #0x8]
  520eb8: e3a015fe     	mov	r1, #1065353216
  520ebc: ebf7b738     	bl	0x30eba4 <.plt+0xe30>   @ imm = #-0x212320
  520ec0: e5850008     	str	r0, [r5, #0x8]
  520ec4: e5945068     	ldr	r5, [r4, #0x68]
  520ec8: e3a015fe     	mov	r1, #1065353216
  520ecc: e2877001     	add	r7, r7, #1
  520ed0: e0855006     	add	r5, r5, r6
  520ed4: e5950014     	ldr	r0, [r5, #0x14]
  520ed8: ebf7b731     	bl	0x30eba4 <.plt+0xe30>   @ imm = #-0x21233c
  520edc: e5850014     	str	r0, [r5, #0x14]
  520ee0: e5945068     	ldr	r5, [r4, #0x68]
  520ee4: e3a015fe     	mov	r1, #1065353216
  520ee8: e0855006     	add	r5, r5, r6
  520eec: e5950020     	ldr	r0, [r5, #0x20]
  520ef0: ebf7b72b     	bl	0x30eba4 <.plt+0xe30>   @ imm = #-0x212354
  520ef4: e5850020     	str	r0, [r5, #0x20]
  520ef8: e594306c     	ldr	r3, [r4, #0x6c]
  520efc: e2866024     	add	r6, r6, #36
  520f00: e1530007     	cmp	r3, r7
  520f04: 8affffe8     	bhi	0x520eac <PFFloor::_LoadNavMesh(glitch::scene::IMeshSceneNode*)+0x36c> @ imm = #-0x60
  520f08: e1a00008     	mov	r0, r8
  520f0c: ebf7dc99     	bl	0x318178 <UserProperties::~UserProperties()> @ imm = #-0x208d9c
  520f10: e28dd038     	add	sp, sp, #56
  520f14: e8bd87f0     	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
  520f18: e59f3064     	ldr	r3, [pc, #0x64]         @ 0x520f84 <PFFloor::_LoadNavMesh(glitch::scene::IMeshSceneNode*)+0x444>
  520f1c: e7963003     	ldr	r3, [r6, r3]
  520f20: e5933000     	ldr	r3, [r3]
  520f24: e3530002     	cmp	r3, #2
  520f28: 05800000     	streq	r0, [r0]
  520f2c: 0affff4e     	beq	0x520c6c <PFFloor::_LoadNavMesh(glitch::scene::IMeshSceneNode*)+0x12c> @ imm = #-0x2c8
  520f30: e3530001     	cmp	r3, #1
  520f34: 1affff4c     	bne	0x520c6c <PFFloor::_LoadNavMesh(glitch::scene::IMeshSceneNode*)+0x12c> @ imm = #-0x2d0
  520f38: e59f0048     	ldr	r0, [pc, #0x48]         @ 0x520f88 <PFFloor::_LoadNavMesh(glitch::scene::IMeshSceneNode*)+0x448>
  520f3c: e59f1048     	ldr	r1, [pc, #0x48]         @ 0x520f8c <PFFloor::_LoadNavMesh(glitch::scene::IMeshSceneNode*)+0x44c>
  520f40: e59f2048     	ldr	r2, [pc, #0x48]         @ 0x520f90 <PFFloor::_LoadNavMesh(glitch::scene::IMeshSceneNode*)+0x450>
  520f44: e7960000     	ldr	r0, [r6, r0]
  520f48: e59f3044     	ldr	r3, [pc, #0x44]         @ 0x520f94 <PFFloor::_LoadNavMesh(glitch::scene::IMeshSceneNode*)+0x454>
  520f4c: e3a0c08a     	mov	r12, #138
  520f50: e08f1001     	add	r1, pc, r1
  520f54: e08f2002     	add	r2, pc, r2
  520f58: e08f3003     	add	r3, pc, r3
  520f5c: e28000a8     	add	r0, r0, #168
  520f60: e58dc000     	str	r12, [sp]
  520f64: ebf7b426     	bl	0x30e004 <.plt+0x290>   @ imm = #-0x212f68
  520f68: eaffff3f     	b	0x520c6c <PFFloor::_LoadNavMesh(glitch::scene::IMeshSceneNode*)+0x12c> @ imm = #-0x304
  520f6c: c4 bd 3b 00  	.word	0x003bbdc4
  520f70: f4 3e 47 00  	.word	0x00473ef4
  520f74: 10 ab 3e 00  	.word	0x003eab10
  520f78: 4c bd 3b 00  	.word	0x003bbd4c
  520f7c: 38 bd 3b 00  	.word	0x003bbd38
  520f80: 24 bd 3b 00  	.word	0x003bbd24
  520f84: c0 39 00 00  	.word	0x000039c0
  520f88: c0 19 00 00  	.word	0x000019c0
  520f8c: 88 d4 39 00  	.word	0x0039d488
  520f90: 0c ba 3b 00  	.word	0x003bba0c
  520f94: 78 b9 3b 00  	.word	0x003bb978
