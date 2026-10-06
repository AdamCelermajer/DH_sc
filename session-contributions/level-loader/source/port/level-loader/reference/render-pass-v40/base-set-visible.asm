
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00596ec4 <glitch::scene::ISceneNode::setVisible(bool)>:
  596ec4: e92d4070     	push	{r4, r5, r6, lr}
  596ec8: e5d03120     	ldrb	r3, [r0, #0x120]
  596ecc: e1a05000     	mov	r5, r0
  596ed0: e1530001     	cmp	r3, r1
  596ed4: 0a000019     	beq	0x596f40 <glitch::scene::ISceneNode::setVisible(bool)+0x7c> @ imm = #0x64
  596ed8: e590311c     	ldr	r3, [r0, #0x11c]
  596edc: e3510000     	cmp	r1, #0
  596ee0: e5c01120     	strb	r1, [r0, #0x120]
  596ee4: e2032001     	and	r2, r3, #1
  596ee8: 1a000015     	bne	0x596f44 <glitch::scene::ISceneNode::setVisible(bool)+0x80> @ imm = #0x54
  596eec: e3c33001     	bic	r3, r3, #1
  596ef0: e585311c     	str	r3, [r5, #0x11c]
  596ef4: e2033001     	and	r3, r3, #1
  596ef8: e1520003     	cmp	r2, r3
  596efc: 0a00000f     	beq	0x596f40 <glitch::scene::ISceneNode::setVisible(bool)+0x7c> @ imm = #0x3c
  596f00: e1a06005     	mov	r6, r5
  596f04: e5b640f4     	ldr	r4, [r6, #0xf4]!
  596f08: ea000009     	b	0x596f34 <glitch::scene::ISceneNode::setVisible(bool)+0x70> @ imm = #0x24
  596f0c: e595111c     	ldr	r1, [r5, #0x11c]
  596f10: e3540000     	cmp	r4, #0
  596f14: 01a03004     	moveq	r3, r4
  596f18: 12443004     	subne	r3, r4, #4
  596f1c: e1a00003     	mov	r0, r3
  596f20: e2011001     	and	r1, r1, #1
  596f24: e5933000     	ldr	r3, [r3]
  596f28: e1a0e00f     	mov	lr, pc
  596f2c: e593f0ec     	ldr	pc, [r3, #0xec]
  596f30: e5944000     	ldr	r4, [r4]
  596f34: e1560004     	cmp	r6, r4
  596f38: 1afffff3     	bne	0x596f0c <glitch::scene::ISceneNode::setVisible(bool)+0x48> @ imm = #-0x34
  596f3c: e8bd8070     	pop	{r4, r5, r6, pc}
  596f40: e8bd8070     	pop	{r4, r5, r6, pc}
  596f44: e5d01121     	ldrb	r1, [r0, #0x121]
  596f48: e3510000     	cmp	r1, #0
  596f4c: 0affffe6     	beq	0x596eec <glitch::scene::ISceneNode::setVisible(bool)+0x28> @ imm = #-0x68
  596f50: e3833001     	orr	r3, r3, #1
  596f54: e580311c     	str	r3, [r0, #0x11c]
  596f58: eaffffe5     	b	0x596ef4 <glitch::scene::ISceneNode::setVisible(bool)+0x30> @ imm = #-0x6c
