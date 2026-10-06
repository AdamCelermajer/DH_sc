
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0060fc70 <glitch::collada::CColladaDatabase::constructImage(glitch::collada::SImage*, glitch::collada::CRootSceneNode*) const>:
  60fc70: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  60fc74: e2526000     	subs	r6, r2, #0
  60fc78: e1a05000     	mov	r5, r0
  60fc7c: e1a07001     	mov	r7, r1
  60fc80: 05806000     	streq	r6, [r0]
  60fc84: 0a00000b     	beq	0x60fcb8 <glitch::collada::CColladaDatabase::constructImage(glitch::collada::SImage*, glitch::collada::CRootSceneNode*) const+0x48> @ imm = #0x2c
  60fc88: e3a01000     	mov	r1, #0
  60fc8c: e3a0001c     	mov	r0, #28
  60fc90: ebfc9145     	bl	0x5341ac <operator new(unsigned int, glitch::memory::E_MEMORY_HINT)> @ imm = #-0xdbaec
  60fc94: e1a01007     	mov	r1, r7
  60fc98: e1a02006     	mov	r2, r6
  60fc9c: e1a04000     	mov	r4, r0
  60fca0: ebfff94b     	bl	0x60e1d4 <glitch::collada::CImage::CImage(glitch::collada::CColladaDatabase const&, glitch::collada::SImage&)> @ imm = #-0x1ad4
  60fca4: e3540000     	cmp	r4, #0
  60fca8: e5854000     	str	r4, [r5]
  60fcac: 15943004     	ldrne	r3, [r4, #0x4]
  60fcb0: 12833001     	addne	r3, r3, #1
  60fcb4: 15843004     	strne	r3, [r4, #0x4]
  60fcb8: e1a00005     	mov	r0, r5
  60fcbc: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}

0060fcc0 <glitch::collada::CColladaDatabase::constructImage(int, glitch::collada::CRootSceneNode*) const>:
  60fcc0: e92d4070     	push	{r4, r5, r6, lr}
  60fcc4: e1a05001     	mov	r5, r1
  60fcc8: e1a04000     	mov	r4, r0
  60fccc: e1a01002     	mov	r1, r2
  60fcd0: e1a00005     	mov	r0, r5
  60fcd4: e1a06003     	mov	r6, r3
  60fcd8: ebfff9ba     	bl	0x60e3c8 <glitch::collada::CColladaDatabase::getImage(int) const> @ imm = #-0x1918
  60fcdc: e1a01005     	mov	r1, r5
  60fce0: e1a02000     	mov	r2, r0
  60fce4: e1a03006     	mov	r3, r6
  60fce8: e1a00004     	mov	r0, r4
  60fcec: ebffffdf     	bl	0x60fc70 <glitch::collada::CColladaDatabase::constructImage(glitch::collada::SImage*, glitch::collada::CRootSceneNode*) const> @ imm = #-0x84
  60fcf0: e1a00004     	mov	r0, r4
  60fcf4: e8bd8070     	pop	{r4, r5, r6, pc}

0060fcf8 <int glitch::collada::CEventsManager::getEventTimeFromEventNameEx<int, 1000>(char const*)>:
  60fcf8: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  60fcfc: e5909014     	ldr	r9, [r0, #0x14]
  60fd00: e24dd00c     	sub	sp, sp, #12
  60fd04: e1a07001     	mov	r7, r1
  60fd08: e5992010     	ldr	r2, [r9, #0x10]
  60fd0c: e3520000     	cmp	r2, #0
  60fd10: e58d2004     	str	r2, [sp, #0x4]
  60fd14: d3e0b000     	mvnle	r11, #0
  60fd18: da00001c     	ble	0x60fd90 <int glitch::collada::CEventsManager::getEventTimeFromEventNameEx<int, 1000>(char const*)+0x98> @ imm = #0x70
  60fd1c: e5993014     	ldr	r3, [r9, #0x14]
  60fd20: e3a08000     	mov	r8, #0
  60fd24: e3e0b000     	mvn	r11, #0
  60fd28: e58d3000     	str	r3, [sp]
  60fd2c: e59d3000     	ldr	r3, [sp]
  60fd30: e7935188     	ldr	r5, [r3, r8, lsl #3]
  60fd34: e0833188     	add	r3, r3, r8, lsl #3
  60fd38: e3550000     	cmp	r5, #0
  60fd3c: da00000f     	ble	0x60fd80 <int glitch::collada::CEventsManager::getEventTimeFromEventNameEx<int, 1000>(char const*)+0x88> @ imm = #0x3c
  60fd40: e5936004     	ldr	r6, [r3, #0x4]
  60fd44: e1a0a108     	lsl	r10, r8, #2
  60fd48: e3a04000     	mov	r4, #0
  60fd4c: e7961104     	ldr	r1, [r6, r4, lsl #2]
  60fd50: e1a00007     	mov	r0, r7
