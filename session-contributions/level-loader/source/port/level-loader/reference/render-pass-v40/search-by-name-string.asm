
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00352b74 <SceneManager::SearchByName(glitch::scene::ISceneNode*, std::string const&, bool)>:
  352b74: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  352b78: e2514000     	subs	r4, r1, #0
  352b7c: e1a08000     	mov	r8, r0
  352b80: e1a05002     	mov	r5, r2
  352b84: e1a07003     	mov	r7, r3
  352b88: 0a00001d     	beq	0x352c04 <SceneManager::SearchByName(glitch::scene::ISceneNode*, std::string const&, bool)+0x90> @ imm = #0x74
  352b8c: e3530000     	cmp	r3, #0
  352b90: 1a00001d     	bne	0x352c0c <SceneManager::SearchByName(glitch::scene::ISceneNode*, std::string const&, bool)+0x98> @ imm = #0x74
  352b94: e5943000     	ldr	r3, [r4]
  352b98: e1a00004     	mov	r0, r4
  352b9c: e1a0e00f     	mov	lr, pc
  352ba0: e593f024     	ldr	pc, [r3, #0x24]
  352ba4: e5951014     	ldr	r1, [r5, #0x14]
  352ba8: ebfeeddb     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0x44894
  352bac: e3500000     	cmp	r0, #0
  352bb0: 0a000013     	beq	0x352c04 <SceneManager::SearchByName(glitch::scene::ISceneNode*, std::string const&, bool)+0x90> @ imm = #0x4c
  352bb4: e1a00004     	mov	r0, r4
  352bb8: eb091182     	bl	0x5971c8 <glitch::scene::ISceneNode::getChildren() const> @ imm = #0x244608
  352bbc: e1a06000     	mov	r6, r0
  352bc0: e5b64004     	ldr	r4, [r6, #0x4]!
  352bc4: e1540006     	cmp	r4, r6
  352bc8: 03a04000     	moveq	r4, #0
  352bcc: 0a00000c     	beq	0x352c04 <SceneManager::SearchByName(glitch::scene::ISceneNode*, std::string const&, bool)+0x90> @ imm = #0x30
  352bd0: e3540000     	cmp	r4, #0
  352bd4: 01a01004     	moveq	r1, r4
  352bd8: 12441004     	subne	r1, r4, #4
  352bdc: e1a00008     	mov	r0, r8
  352be0: e1a02005     	mov	r2, r5
  352be4: e1a03007     	mov	r3, r7
  352be8: ebffffe1     	bl	0x352b74 <SceneManager::SearchByName(glitch::scene::ISceneNode*, std::string const&, bool)> @ imm = #-0x7c
  352bec: e5944000     	ldr	r4, [r4]
  352bf0: e1560004     	cmp	r6, r4
  352bf4: 0a000001     	beq	0x352c00 <SceneManager::SearchByName(glitch::scene::ISceneNode*, std::string const&, bool)+0x8c> @ imm = #0x4
  352bf8: e3500000     	cmp	r0, #0
  352bfc: 0afffff3     	beq	0x352bd0 <SceneManager::SearchByName(glitch::scene::ISceneNode*, std::string const&, bool)+0x5c> @ imm = #-0x34
  352c00: e1a04000     	mov	r4, r0
  352c04: e1a00004     	mov	r0, r4
  352c08: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  352c0c: e5943000     	ldr	r3, [r4]
  352c10: e1a00004     	mov	r0, r4
  352c14: e1a0e00f     	mov	lr, pc
  352c18: e593f024     	ldr	pc, [r3, #0x24]
  352c1c: e5951014     	ldr	r1, [r5, #0x14]
  352c20: e5952010     	ldr	r2, [r5, #0x10]
  352c24: e0612002     	rsb	r2, r1, r2
  352c28: ebfef013     	bl	0x30ec7c <.plt+0xf08>   @ imm = #-0x43fb4
  352c2c: e3500000     	cmp	r0, #0
  352c30: 1affffdf     	bne	0x352bb4 <SceneManager::SearchByName(glitch::scene::ISceneNode*, std::string const&, bool)+0x40> @ imm = #-0x84
  352c34: e1a00004     	mov	r0, r4
  352c38: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
