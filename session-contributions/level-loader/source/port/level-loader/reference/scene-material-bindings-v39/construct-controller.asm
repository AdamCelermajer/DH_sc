
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0061ace8 <glitch::collada::CColladaDatabase::constructController(glitch::video::IVideoDriver*, glitch::collada::SInstanceController*, glitch::collada::CRootSceneNode*, bool) const>:
  61ace8: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  61acec: e1a06003     	mov	r6, r3
  61acf0: e24dd02c     	sub	sp, sp, #44
  61acf4: e5933004     	ldr	r3, [r3, #0x4]
  61acf8: e59dc050     	ldr	r12, [sp, #0x50]
  61acfc: e5dde054     	ldrb	lr, [sp, #0x54]
  61ad00: e1a05000     	mov	r5, r0
  61ad04: e2833001     	add	r3, r3, #1
  61ad08: e58dc000     	str	r12, [sp]
  61ad0c: e1a0a001     	mov	r10, r1
  61ad10: e1a0b002     	mov	r11, r2
  61ad14: e58de014     	str	lr, [sp, #0x14]
  61ad18: ebffff38     	bl	0x61aa00 <glitch::collada::CColladaDatabase::constructController(glitch::video::IVideoDriver*, char const*, glitch::collada::CRootSceneNode*) const> @ imm = #-0x320
  61ad1c: e5953000     	ldr	r3, [r5]
  61ad20: e3530000     	cmp	r3, #0
  61ad24: 0a000060     	beq	0x61aeac <glitch::collada::CColladaDatabase::constructController(glitch::video::IVideoDriver*, glitch::collada::SInstanceController*, glitch::collada::CRootSceneNode*, bool) const+0x1c4> @ imm = #0x180
  61ad28: e596200c     	ldr	r2, [r6, #0xc]
  61ad2c: e3520000     	cmp	r2, #0
  61ad30: da00002a     	ble	0x61ade0 <glitch::collada::CColladaDatabase::constructController(glitch::video::IVideoDriver*, glitch::collada::SInstanceController*, glitch::collada::CRootSceneNode*, bool) const+0xf8> @ imm = #0xa8
  61ad34: e3a07000     	mov	r7, #0
  61ad38: e1a08007     	mov	r8, r7
  61ad3c: e28d4020     	add	r4, sp, #32
  61ad40: e28d9024     	add	r9, sp, #36
  61ad44: ea000019     	b	0x61adb0 <glitch::collada::CColladaDatabase::constructController(glitch::video::IVideoDriver*, glitch::collada::SInstanceController*, glitch::collada::CRootSceneNode*, bool) const+0xc8> @ imm = #0x64
  61ad48: e5932004     	ldr	r2, [r3, #0x4]
  61ad4c: e2822001     	add	r2, r2, #1
  61ad50: ebffffcc     	bl	0x61ac88 <glitch::collada::CColladaDatabase::getMaterial(char const*, char const*) const> @ imm = #-0xd0
  61ad54: e1a02000     	mov	r2, r0
  61ad58: e1a00004     	mov	r0, r4
  61ad5c: e59d1050     	ldr	r1, [sp, #0x50]
  61ad60: e1a0300b     	mov	r3, r11
  61ad64: eb010764     	bl	0x65cafc <glitch::collada::CRootSceneNode::getMaterial(glitch::collada::SMaterial*, glitch::video::IVideoDriver*)> @ imm = #0x41d90
  61ad68: e5950000     	ldr	r0, [r5]
  61ad6c: e3a0e000     	mov	lr, #0
  61ad70: e1a01008     	mov	r1, r8
  61ad74: e590c000     	ldr	r12, [r0]
  61ad78: e1a03009     	mov	r3, r9
  61ad7c: e1a02004     	mov	r2, r4
  61ad80: e59cc020     	ldr	r12, [r12, #0x20]
  61ad84: e58de024     	str	lr, [sp, #0x24]
  61ad88: e12fff3c     	blx	r12
  61ad8c: e1a00009     	mov	r0, r9
  61ad90: ebfd7d35     	bl	0x57a26c <boost::intrusive_ptr<glitch::video::CMaterialVertexAttributeMap>::~intrusive_ptr()> @ imm = #-0xa0b2c
  61ad94: e1a00004     	mov	r0, r4
  61ad98: ebf3d792     	bl	0x310be8 <boost::intrusive_ptr<glitch::video::CMaterial>::~intrusive_ptr()> @ imm = #-0x30a1b8
  61ad9c: e596300c     	ldr	r3, [r6, #0xc]
  61ada0: e2888001     	add	r8, r8, #1
  61ada4: e287703c     	add	r7, r7, #60
  61ada8: e1580003     	cmp	r8, r3
  61adac: aa00000a     	bge	0x61addc <glitch::collada::CColladaDatabase::constructController(glitch::video::IVideoDriver*, glitch::collada::SInstanceController*, glitch::collada::CRootSceneNode*, bool) const+0xf4> @ imm = #0x28
  61adb0: e5963010     	ldr	r3, [r6, #0x10]
  61adb4: e1a0000a     	mov	r0, r10
  61adb8: e7931007     	ldr	r1, [r3, r7]
  61adbc: e0833007     	add	r3, r3, r7
  61adc0: e3510000     	cmp	r1, #0
  61adc4: 1affffdf     	bne	0x61ad48 <glitch::collada::CColladaDatabase::constructController(glitch::video::IVideoDriver*, glitch::collada::SInstanceController*, glitch::collada::CRootSceneNode*, bool) const+0x60> @ imm = #-0x84
  61adc8: e5931008     	ldr	r1, [r3, #0x8]
  61adcc: e1a0000a     	mov	r0, r10
  61add0: ebffcd8a     	bl	0x60e400 <glitch::collada::CColladaDatabase::getMaterial(int) const> @ imm = #-0xc9d8
  61add4: e1a02000     	mov	r2, r0
  61add8: eaffffde     	b	0x61ad58 <glitch::collada::CColladaDatabase::constructController(glitch::video::IVideoDriver*, glitch::collada::SInstanceController*, glitch::collada::CRootSceneNode*, bool) const+0x70> @ imm = #-0x88
  61addc: e5953000     	ldr	r3, [r5]
  61ade0: e1a00003     	mov	r0, r3
  61ade4: e1a0100b     	mov	r1, r11
  61ade8: e5933000     	ldr	r3, [r3]
  61adec: e59d2014     	ldr	r2, [sp, #0x14]
  61adf0: e1a0e00f     	mov	lr, pc
  61adf4: e593f040     	ldr	pc, [r3, #0x40]
  61adf8: e596300c     	ldr	r3, [r6, #0xc]
  61adfc: e3530000     	cmp	r3, #0
  61ae00: da000029     	ble	0x61aeac <glitch::collada::CColladaDatabase::constructController(glitch::video::IVideoDriver*, glitch::collada::SInstanceController*, glitch::collada::CRootSceneNode*, bool) const+0x1c4> @ imm = #0xa4
  61ae04: e3a08000     	mov	r8, #0
  61ae08: e1a07008     	mov	r7, r8
  61ae0c: e28d4020     	add	r4, sp, #32
  61ae10: e28d901c     	add	r9, sp, #28
  61ae14: e1a0b008     	mov	r11, r8
  61ae18: e5953000     	ldr	r3, [r5]
  61ae1c: e1a02007     	mov	r2, r7
  61ae20: e1a00004     	mov	r0, r4
  61ae24: e1a01003     	mov	r1, r3
  61ae28: e5933000     	ldr	r3, [r3]
  61ae2c: e1a0e00f     	mov	lr, pc
  61ae30: e593f018     	ldr	pc, [r3, #0x18]
  61ae34: e59a2004     	ldr	r2, [r10, #0x4]
  61ae38: e5963010     	ldr	r3, [r6, #0x10]
  61ae3c: e1a00009     	mov	r0, r9
  61ae40: e592c000     	ldr	r12, [r2]
  61ae44: e1a01002     	mov	r1, r2
  61ae48: e0833008     	add	r3, r3, r8
  61ae4c: e58d7008     	str	r7, [sp, #0x8]
  61ae50: e1a0200a     	mov	r2, r10
  61ae54: e58d5000     	str	r5, [sp]
  61ae58: e58d4004     	str	r4, [sp, #0x4]
  61ae5c: e58db00c     	str	r11, [sp, #0xc]
  61ae60: e1a0e00f     	mov	lr, pc
  61ae64: e59cf024     	ldr	pc, [r12, #0x24]
  61ae68: e595c000     	ldr	r12, [r5]
  61ae6c: e1a01007     	mov	r1, r7
  61ae70: e1a03009     	mov	r3, r9
  61ae74: e1a02004     	mov	r2, r4
  61ae78: e1a0000c     	mov	r0, r12
  61ae7c: e59cc000     	ldr	r12, [r12]
  61ae80: e1a0e00f     	mov	lr, pc
  61ae84: e59cf020     	ldr	pc, [r12, #0x20]
  61ae88: e1a00009     	mov	r0, r9
  61ae8c: ebfd7cf6     	bl	0x57a26c <boost::intrusive_ptr<glitch::video::CMaterialVertexAttributeMap>::~intrusive_ptr()> @ imm = #-0xa0c28
  61ae90: e1a00004     	mov	r0, r4
  61ae94: ebf3d753     	bl	0x310be8 <boost::intrusive_ptr<glitch::video::CMaterial>::~intrusive_ptr()> @ imm = #-0x30a2b4
  61ae98: e596300c     	ldr	r3, [r6, #0xc]
  61ae9c: e2877001     	add	r7, r7, #1
  61aea0: e288803c     	add	r8, r8, #60
  61aea4: e1570003     	cmp	r7, r3
  61aea8: baffffda     	blt	0x61ae18 <glitch::collada::CColladaDatabase::constructController(glitch::video::IVideoDriver*, glitch::collada::SInstanceController*, glitch::collada::CRootSceneNode*, bool) const+0x130> @ imm = #-0x98
  61aeac: e1a00005     	mov	r0, r5
  61aeb0: e28dd02c     	add	sp, sp, #44
  61aeb4: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
