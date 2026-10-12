
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

005848a4 <glitch::scene::CLightSceneNode::~CLightSceneNode()>:
  5848a4: e92d4070     	push	{r4, r5, r6, lr}
  5848a8: e59f50a0     	ldr	r5, [pc, #0xa0]         @ 0x584950 <glitch::scene::CLightSceneNode::~CLightSceneNode()+0xac>
  5848ac: e59f30a0     	ldr	r3, [pc, #0xa0]         @ 0x584954 <glitch::scene::CLightSceneNode::~CLightSceneNode()+0xb0>
  5848b0: e1a04000     	mov	r4, r0
  5848b4: e08f5005     	add	r5, pc, r5
  5848b8: e7953003     	ldr	r3, [r5, r3]
  5848bc: e5900134     	ldr	r0, [r0, #0x134]
  5848c0: e2832e12     	add	r2, r3, #288
  5848c4: e283301c     	add	r3, r3, #28
  5848c8: e5843000     	str	r3, [r4]
  5848cc: e5842154     	str	r2, [r4, #0x154]
  5848d0: e5903000     	ldr	r3, [r0]
  5848d4: e3530001     	cmp	r3, #1
  5848d8: 9a000002     	bls	0x5848e8 <glitch::scene::CLightSceneNode::~CLightSceneNode()+0x44> @ imm = #0x8
  5848dc: e5940130     	ldr	r0, [r4, #0x130]
  5848e0: ebffff84     	bl	0x5846f8 <glitch::video::STransformationSource::detach()> @ imm = #-0x1f0
  5848e4: e5940134     	ldr	r0, [r4, #0x134]
  5848e8: e3500000     	cmp	r0, #0
  5848ec: 0a000010     	beq	0x584934 <glitch::scene::CLightSceneNode::~CLightSceneNode()+0x90> @ imm = #0x40
  5848f0: e5903000     	ldr	r3, [r0]
  5848f4: e2433001     	sub	r3, r3, #1
  5848f8: e3530000     	cmp	r3, #0
  5848fc: e5803000     	str	r3, [r0]
  584900: 1a00000b     	bne	0x584934 <glitch::scene::CLightSceneNode::~CLightSceneNode()+0x90> @ imm = #0x2c
  584904: e5d03054     	ldrb	r3, [r0, #0x54]
  584908: e3530000     	cmp	r3, #0
  58490c: 1a000005     	bne	0x584928 <glitch::scene::CLightSceneNode::~CLightSceneNode()+0x84> @ imm = #0x14
  584910: e59f3040     	ldr	r3, [pc, #0x40]         @ 0x584958 <glitch::scene::CLightSceneNode::~CLightSceneNode()+0xb4>
  584914: e5902050     	ldr	r2, [r0, #0x50]
  584918: e7953003     	ldr	r3, [r5, r3]
  58491c: e5931000     	ldr	r1, [r3]
  584920: e5821000     	str	r1, [r2]
  584924: e5832000     	str	r2, [r3]
  584928: e3a03000     	mov	r3, #0
  58492c: e5803050     	str	r3, [r0, #0x50]
  584930: ebf6265e     	bl	0x30e2b0 <.plt+0x53c>   @ imm = #-0x276688
  584934: e59f1020     	ldr	r1, [pc, #0x20]         @ 0x58495c <glitch::scene::CLightSceneNode::~CLightSceneNode()+0xb8>
  584938: e1a00004     	mov	r0, r4
  58493c: e7951001     	ldr	r1, [r5, r1]
  584940: e2811004     	add	r1, r1, #4
  584944: eb0050dc     	bl	0x598cbc <glitch::scene::ISceneNode::~ISceneNode()> @ imm = #0x14370
  584948: e1a00004     	mov	r0, r4
  58494c: e8bd8070     	pop	{r4, r5, r6, pc}
  584950: dc 01 41 00  	.word	0x004101dc
  584954: 58 1a 00 00  	.word	0x00001a58
  584958: c0 3c 00 00  	.word	0x00003cc0
  58495c: e0 10 00 00  	.word	0x000010e0
