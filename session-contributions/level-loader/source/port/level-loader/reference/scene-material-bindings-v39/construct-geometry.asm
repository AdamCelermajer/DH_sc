
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0061aeb8 <glitch::collada::CColladaDatabase::constructGeometry(glitch::video::IVideoDriver*, glitch::collada::SInstanceGeometry*, glitch::collada::CRootSceneNode*) const>:
  61aeb8: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  61aebc: e1a09000     	mov	r9, r0
  61aec0: e3a00000     	mov	r0, #0
  61aec4: e5890000     	str	r0, [r9]
  61aec8: e1a06003     	mov	r6, r3
  61aecc: e5933000     	ldr	r3, [r3]
  61aed0: e596c004     	ldr	r12, [r6, #0x4]
  61aed4: e24dd034     	sub	sp, sp, #52
  61aed8: e1530000     	cmp	r3, r0
  61aedc: e1a05001     	mov	r5, r1
  61aee0: e28cc001     	add	r12, r12, #1
  61aee4: e58d2010     	str	r2, [sp, #0x10]
  61aee8: 0a00005e     	beq	0x61b068 <glitch::collada::CColladaDatabase::constructGeometry(glitch::video::IVideoDriver*, glitch::collada::SInstanceGeometry*, glitch::collada::CRootSceneNode*) const+0x1b0> @ imm = #0x178
  61aeec: e28d002c     	add	r0, sp, #44
  61aef0: e58dc000     	str	r12, [sp]
  61aef4: ebfffef9     	bl	0x61aae0 <glitch::collada::CColladaDatabase::constructGeometry(glitch::video::IVideoDriver*, char const*, char const*) const> @ imm = #-0x41c
  61aef8: e59d302c     	ldr	r3, [sp, #0x2c]
  61aefc: e3530000     	cmp	r3, #0
  61af00: 15932004     	ldrne	r2, [r3, #0x4]
  61af04: 12822001     	addne	r2, r2, #1
  61af08: 15832004     	strne	r2, [r3, #0x4]
  61af0c: e5990000     	ldr	r0, [r9]
  61af10: e5893000     	str	r3, [r9]
  61af14: e3500000     	cmp	r0, #0
  61af18: 0a000000     	beq	0x61af20 <glitch::collada::CColladaDatabase::constructGeometry(glitch::video::IVideoDriver*, glitch::collada::SInstanceGeometry*, glitch::collada::CRootSceneNode*) const+0x68> @ imm = #0x0
  61af1c: ebf40998     	bl	0x31d584 <glitch::IReferenceCounted::drop() const> @ imm = #-0x2fd9a0
  61af20: e59d002c     	ldr	r0, [sp, #0x2c]
  61af24: e3500000     	cmp	r0, #0
  61af28: 0a000000     	beq	0x61af30 <glitch::collada::CColladaDatabase::constructGeometry(glitch::video::IVideoDriver*, glitch::collada::SInstanceGeometry*, glitch::collada::CRootSceneNode*) const+0x78> @ imm = #0x0
  61af2c: ebf40994     	bl	0x31d584 <glitch::IReferenceCounted::drop() const> @ imm = #-0x2fd9b0
  61af30: e5993000     	ldr	r3, [r9]
  61af34: e3530000     	cmp	r3, #0
  61af38: 0a000047     	beq	0x61b05c <glitch::collada::CColladaDatabase::constructGeometry(glitch::video::IVideoDriver*, glitch::collada::SInstanceGeometry*, glitch::collada::CRootSceneNode*) const+0x1a4> @ imm = #0x11c
  61af3c: e596300c     	ldr	r3, [r6, #0xc]
  61af40: e3530000     	cmp	r3, #0
  61af44: da000044     	ble	0x61b05c <glitch::collada::CColladaDatabase::constructGeometry(glitch::video::IVideoDriver*, glitch::collada::SInstanceGeometry*, glitch::collada::CRootSceneNode*) const+0x1a4> @ imm = #0x110
  61af48: e3a07000     	mov	r7, #0
  61af4c: e28d001c     	add	r0, sp, #28
  61af50: e1a04007     	mov	r4, r7
  61af54: e1a08007     	mov	r8, r7
  61af58: e28da024     	add	r10, sp, #36
  61af5c: e28db020     	add	r11, sp, #32
  61af60: e58d0014     	str	r0, [sp, #0x14]
  61af64: e1a07006     	mov	r7, r6
  61af68: ea000030     	b	0x61b030 <glitch::collada::CColladaDatabase::constructGeometry(glitch::video::IVideoDriver*, glitch::collada::SInstanceGeometry*, glitch::collada::CRootSceneNode*) const+0x178> @ imm = #0xc0
  61af6c: e5962004     	ldr	r2, [r6, #0x4]
  61af70: e2822001     	add	r2, r2, #1
  61af74: ebffff43     	bl	0x61ac88 <glitch::collada::CColladaDatabase::getMaterial(char const*, char const*) const> @ imm = #-0x2f4
  61af78: e1a02000     	mov	r2, r0
  61af7c: e1a0000a     	mov	r0, r10
  61af80: e59d1058     	ldr	r1, [sp, #0x58]
  61af84: e59d3010     	ldr	r3, [sp, #0x10]
  61af88: eb0106db     	bl	0x65cafc <glitch::collada::CRootSceneNode::getMaterial(glitch::collada::SMaterial*, glitch::video::IVideoDriver*)> @ imm = #0x41b6c
  61af8c: e595e004     	ldr	lr, [r5, #0x4]
  61af90: e599c000     	ldr	r12, [r9]
  61af94: e1a03006     	mov	r3, r6
  61af98: e1a0100e     	mov	r1, lr
  61af9c: e59ee000     	ldr	lr, [lr]
  61afa0: e35c0000     	cmp	r12, #0
  61afa4: e1a0000b     	mov	r0, r11
  61afa8: e59e6024     	ldr	r6, [lr, #0x24]
  61afac: e58dc01c     	str	r12, [sp, #0x1c]
  61afb0: 159ce004     	ldrne	lr, [r12, #0x4]
  61afb4: e1a02005     	mov	r2, r5
  61afb8: e288803c     	add	r8, r8, #60
  61afbc: 128ee001     	addne	lr, lr, #1
  61afc0: 158ce004     	strne	lr, [r12, #0x4]
  61afc4: e59dc014     	ldr	r12, [sp, #0x14]
  61afc8: e58d4008     	str	r4, [sp, #0x8]
  61afcc: e58da004     	str	r10, [sp, #0x4]
  61afd0: e58dc000     	str	r12, [sp]
  61afd4: e3a0c000     	mov	r12, #0
  61afd8: e58dc00c     	str	r12, [sp, #0xc]
  61afdc: e12fff36     	blx	r6
  61afe0: e59d001c     	ldr	r0, [sp, #0x1c]
  61afe4: e3500000     	cmp	r0, #0
  61afe8: 0a000000     	beq	0x61aff0 <glitch::collada::CColladaDatabase::constructGeometry(glitch::video::IVideoDriver*, glitch::collada::SInstanceGeometry*, glitch::collada::CRootSceneNode*) const+0x138> @ imm = #0x0
  61afec: ebf40964     	bl	0x31d584 <glitch::IReferenceCounted::drop() const> @ imm = #-0x2fda70
  61aff0: e599c000     	ldr	r12, [r9]
  61aff4: e1a01004     	mov	r1, r4
  61aff8: e1a0300b     	mov	r3, r11
  61affc: e1a0200a     	mov	r2, r10
  61b000: e1a0000c     	mov	r0, r12
  61b004: e59cc000     	ldr	r12, [r12]
  61b008: e1a0e00f     	mov	lr, pc
  61b00c: e59cf020     	ldr	pc, [r12, #0x20]
  61b010: e1a0000b     	mov	r0, r11
  61b014: ebfd7c94     	bl	0x57a26c <boost::intrusive_ptr<glitch::video::CMaterialVertexAttributeMap>::~intrusive_ptr()> @ imm = #-0xa0db0
  61b018: e1a0000a     	mov	r0, r10
  61b01c: ebf3d6f1     	bl	0x310be8 <boost::intrusive_ptr<glitch::video::CMaterial>::~intrusive_ptr()> @ imm = #-0x30a43c
  61b020: e597300c     	ldr	r3, [r7, #0xc]
  61b024: e2844001     	add	r4, r4, #1
  61b028: e1540003     	cmp	r4, r3
  61b02c: aa00000a     	bge	0x61b05c <glitch::collada::CColladaDatabase::constructGeometry(glitch::video::IVideoDriver*, glitch::collada::SInstanceGeometry*, glitch::collada::CRootSceneNode*) const+0x1a4> @ imm = #0x28
  61b030: e5976010     	ldr	r6, [r7, #0x10]
  61b034: e1a00005     	mov	r0, r5
  61b038: e7961008     	ldr	r1, [r6, r8]
  61b03c: e0866008     	add	r6, r6, r8
  61b040: e3510000     	cmp	r1, #0
  61b044: 1affffc8     	bne	0x61af6c <glitch::collada::CColladaDatabase::constructGeometry(glitch::video::IVideoDriver*, glitch::collada::SInstanceGeometry*, glitch::collada::CRootSceneNode*) const+0xb4> @ imm = #-0xe0
  61b048: e5961008     	ldr	r1, [r6, #0x8]
  61b04c: e1a00005     	mov	r0, r5
  61b050: ebffccea     	bl	0x60e400 <glitch::collada::CColladaDatabase::getMaterial(int) const> @ imm = #-0xcc58
  61b054: e1a02000     	mov	r2, r0
  61b058: eaffffc7     	b	0x61af7c <glitch::collada::CColladaDatabase::constructGeometry(glitch::video::IVideoDriver*, glitch::collada::SInstanceGeometry*, glitch::collada::CRootSceneNode*) const+0xc4> @ imm = #-0xe4
  61b05c: e1a00009     	mov	r0, r9
  61b060: e28dd034     	add	sp, sp, #52
  61b064: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  61b068: e1a0300c     	mov	r3, r12
  61b06c: e28d0028     	add	r0, sp, #40
  61b070: ebfffe8c     	bl	0x61aaa8 <glitch::collada::CColladaDatabase::constructGeometry(glitch::video::IVideoDriver*, char const*) const> @ imm = #-0x5d0
  61b074: e59d3028     	ldr	r3, [sp, #0x28]
  61b078: e3530000     	cmp	r3, #0
  61b07c: 15932004     	ldrne	r2, [r3, #0x4]
  61b080: 12822001     	addne	r2, r2, #1
  61b084: 15832004     	strne	r2, [r3, #0x4]
  61b088: e5990000     	ldr	r0, [r9]
  61b08c: e5893000     	str	r3, [r9]
  61b090: e3500000     	cmp	r0, #0
  61b094: 0a000000     	beq	0x61b09c <glitch::collada::CColladaDatabase::constructGeometry(glitch::video::IVideoDriver*, glitch::collada::SInstanceGeometry*, glitch::collada::CRootSceneNode*) const+0x1e4> @ imm = #0x0
  61b098: ebf40939     	bl	0x31d584 <glitch::IReferenceCounted::drop() const> @ imm = #-0x2fdb1c
  61b09c: e59d0028     	ldr	r0, [sp, #0x28]
  61b0a0: e3500000     	cmp	r0, #0
  61b0a4: 1affffa0     	bne	0x61af2c <glitch::collada::CColladaDatabase::constructGeometry(glitch::video::IVideoDriver*, glitch::collada::SInstanceGeometry*, glitch::collada::CRootSceneNode*) const+0x74> @ imm = #-0x180
  61b0a8: eaffffa0     	b	0x61af30 <glitch::collada::CColladaDatabase::constructGeometry(glitch::video::IVideoDriver*, glitch::collada::SInstanceGeometry*, glitch::collada::CRootSceneNode*) const+0x78> @ imm = #-0x180
