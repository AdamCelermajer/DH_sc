
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003625d0 <SkyBoxMeshSceneNode::onRegisterSceneNode()>:
  3625d0: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  3625d4: e5903134     	ldr	r3, [r0, #0x134]
  3625d8: e24dd01c     	sub	sp, sp, #28
  3625dc: e1a05000     	mov	r5, r0
  3625e0: e3530000     	cmp	r3, #0
  3625e4: 0a000042     	beq	0x3626f4 <SkyBoxMeshSceneNode::onRegisterSceneNode()+0x124> @ imm = #0x108
  3625e8: e5902110     	ldr	r2, [r0, #0x110]
  3625ec: e5929014     	ldr	r9, [r2, #0x14]
  3625f0: e3590000     	cmp	r9, #0
  3625f4: 0a00003e     	beq	0x3626f4 <SkyBoxMeshSceneNode::onRegisterSceneNode()+0x124> @ imm = #0xf8
  3625f8: e1a00003     	mov	r0, r3
  3625fc: e5933000     	ldr	r3, [r3]
  362600: e1a0e00f     	mov	lr, pc
  362604: e593f010     	ldr	pc, [r3, #0x10]
  362608: e2507000     	subs	r7, r0, #0
  36260c: 0a000038     	beq	0x3626f4 <SkyBoxMeshSceneNode::onRegisterSceneNode()+0x124> @ imm = #0xe0
  362610: e3a04001     	mov	r4, #1
  362614: e28da014     	add	r10, sp, #20
  362618: e28d6010     	add	r6, sp, #16
  36261c: e3a0b000     	mov	r11, #0
  362620: e1a08007     	mov	r8, r7
  362624: ea000006     	b	0x362644 <SkyBoxMeshSceneNode::onRegisterSceneNode()+0x74> @ imm = #0x18
  362628: e3500005     	cmp	r0, #5
  36262c: 0a000033     	beq	0x362700 <SkyBoxMeshSceneNode::onRegisterSceneNode()+0x130> @ imm = #0xcc
  362630: e1a00006     	mov	r0, r6
  362634: ebfeb96b     	bl	0x310be8 <boost::intrusive_ptr<glitch::video::CMaterial>::~intrusive_ptr()> @ imm = #-0x51a54
  362638: e1580004     	cmp	r8, r4
  36263c: e2844001     	add	r4, r4, #1
  362640: 9a00002b     	bls	0x3626f4 <SkyBoxMeshSceneNode::onRegisterSceneNode()+0x124> @ imm = #0xac
  362644: e5953134     	ldr	r3, [r5, #0x134]
  362648: e2447001     	sub	r7, r4, #1
  36264c: e1a0000a     	mov	r0, r10
  362650: e1a01003     	mov	r1, r3
  362654: e1a02007     	mov	r2, r7
  362658: e5933000     	ldr	r3, [r3]
  36265c: e1a0e00f     	mov	lr, pc
  362660: e593f014     	ldr	pc, [r3, #0x14]
  362664: e59d3014     	ldr	r3, [sp, #0x14]
  362668: e2530000     	subs	r0, r3, #0
  36266c: 0afffff1     	beq	0x362638 <SkyBoxMeshSceneNode::onRegisterSceneNode()+0x68> @ imm = #-0x3c
  362670: ebfeebc3     	bl	0x31d584 <glitch::IReferenceCounted::drop() const> @ imm = #-0x450f4
  362674: e5953134     	ldr	r3, [r5, #0x134]
  362678: e1a00006     	mov	r0, r6
  36267c: e1a02007     	mov	r2, r7
  362680: e1a01003     	mov	r1, r3
  362684: e5933000     	ldr	r3, [r3]
  362688: e1a0e00f     	mov	lr, pc
  36268c: e593f018     	ldr	pc, [r3, #0x18]
  362690: e595c134     	ldr	r12, [r5, #0x134]
  362694: e1a03007     	mov	r3, r7
  362698: e1a02009     	mov	r2, r9
  36269c: e1a0000c     	mov	r0, r12
  3626a0: e3a01000     	mov	r1, #0
  3626a4: e59cc000     	ldr	r12, [r12]
  3626a8: e1a0e00f     	mov	lr, pc
  3626ac: e59cf038     	ldr	pc, [r12, #0x38]
  3626b0: e3500004     	cmp	r0, #4
  3626b4: 13500010     	cmpne	r0, #16
  3626b8: 1affffda     	bne	0x362628 <SkyBoxMeshSceneNode::onRegisterSceneNode()+0x58> @ imm = #-0x98
  3626bc: e5953110     	ldr	r3, [r5, #0x110]
  3626c0: e1a01005     	mov	r1, r5
  3626c4: e1a02006     	mov	r2, r6
  3626c8: e593c000     	ldr	r12, [r3]
  3626cc: e1a00003     	mov	r0, r3
  3626d0: e3a03002     	mov	r3, #2
  3626d4: e58d3000     	str	r3, [sp]
  3626d8: e3e03102     	mvn	r3, #-2147483648
  3626dc: e58d3008     	str	r3, [sp, #0x8]
  3626e0: e58db004     	str	r11, [sp, #0x4]
  3626e4: e1a03004     	mov	r3, r4
  3626e8: e1a0e00f     	mov	lr, pc
  3626ec: e59cf024     	ldr	pc, [r12, #0x24]
  3626f0: eaffffce     	b	0x362630 <SkyBoxMeshSceneNode::onRegisterSceneNode()+0x60> @ imm = #-0xc8
  3626f4: e3a00001     	mov	r0, #1
  3626f8: e28dd01c     	add	sp, sp, #28
  3626fc: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  362700: e5953134     	ldr	r3, [r5, #0x134]
  362704: e1a00003     	mov	r0, r3
  362708: e5933000     	ldr	r3, [r3]
  36270c: e1a0e00f     	mov	lr, pc
  362710: e593f024     	ldr	pc, [r3, #0x24]
  362714: eaffffc5     	b	0x362630 <SkyBoxMeshSceneNode::onRegisterSceneNode()+0x60> @ imm = #-0xec
