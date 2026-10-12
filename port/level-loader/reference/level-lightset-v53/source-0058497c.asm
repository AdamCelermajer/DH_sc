
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0058497c <glitch::scene::CLightSceneNode::~CLightSceneNode()>:
  58497c: e92d4070     	push	{r4, r5, r6, lr}
  584980: e5913000     	ldr	r3, [r1]
  584984: e1a04000     	mov	r4, r0
  584988: e59f60a0     	ldr	r6, [pc, #0xa0]         @ 0x584a30 <glitch::scene::CLightSceneNode::~CLightSceneNode()+0xb4>
  58498c: e5803000     	str	r3, [r0]
  584990: e513301c     	ldr	r3, [r3, #-0x1c]
  584994: e5912010     	ldr	r2, [r1, #0x10]
  584998: e08f6006     	add	r6, pc, r6
  58499c: e1a05001     	mov	r5, r1
  5849a0: e7802003     	str	r2, [r0, r3]
  5849a4: e5903000     	ldr	r3, [r0]
  5849a8: e5912014     	ldr	r2, [r1, #0x14]
  5849ac: e513300c     	ldr	r3, [r3, #-0xc]
  5849b0: e7802003     	str	r2, [r0, r3]
  5849b4: e5900134     	ldr	r0, [r0, #0x134]
  5849b8: e5903000     	ldr	r3, [r0]
  5849bc: e3530001     	cmp	r3, #1
  5849c0: 9a000002     	bls	0x5849d0 <glitch::scene::CLightSceneNode::~CLightSceneNode()+0x54> @ imm = #0x8
  5849c4: e5940130     	ldr	r0, [r4, #0x130]
  5849c8: ebffff4a     	bl	0x5846f8 <glitch::video::STransformationSource::detach()> @ imm = #-0x2d8
  5849cc: e5940134     	ldr	r0, [r4, #0x134]
  5849d0: e3500000     	cmp	r0, #0
  5849d4: 0a000010     	beq	0x584a1c <glitch::scene::CLightSceneNode::~CLightSceneNode()+0xa0> @ imm = #0x40
  5849d8: e5903000     	ldr	r3, [r0]
  5849dc: e2433001     	sub	r3, r3, #1
  5849e0: e3530000     	cmp	r3, #0
  5849e4: e5803000     	str	r3, [r0]
  5849e8: 1a00000b     	bne	0x584a1c <glitch::scene::CLightSceneNode::~CLightSceneNode()+0xa0> @ imm = #0x2c
  5849ec: e5d03054     	ldrb	r3, [r0, #0x54]
  5849f0: e3530000     	cmp	r3, #0
  5849f4: 1a000005     	bne	0x584a10 <glitch::scene::CLightSceneNode::~CLightSceneNode()+0x94> @ imm = #0x14
  5849f8: e59f3034     	ldr	r3, [pc, #0x34]         @ 0x584a34 <glitch::scene::CLightSceneNode::~CLightSceneNode()+0xb8>
  5849fc: e5902050     	ldr	r2, [r0, #0x50]
  584a00: e7963003     	ldr	r3, [r6, r3]
  584a04: e5931000     	ldr	r1, [r3]
  584a08: e5821000     	str	r1, [r2]
  584a0c: e5832000     	str	r2, [r3]
  584a10: e3a03000     	mov	r3, #0
  584a14: e5803050     	str	r3, [r0, #0x50]
  584a18: ebf62624     	bl	0x30e2b0 <.plt+0x53c>   @ imm = #-0x276770
  584a1c: e2851004     	add	r1, r5, #4
  584a20: e1a00004     	mov	r0, r4
  584a24: eb0050a4     	bl	0x598cbc <glitch::scene::ISceneNode::~ISceneNode()> @ imm = #0x14290
  584a28: e1a00004     	mov	r0, r4
  584a2c: e8bd8070     	pop	{r4, r5, r6, pc}
  584a30: f8 00 41 00  	.word	0x004100f8
  584a34: c0 3c 00 00  	.word	0x00003cc0
