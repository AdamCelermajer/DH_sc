
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

004591f0 <ScriptManager::GetIDFromName(char const*, bool) const>:
  4591f0: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  4591f4: e5904024     	ldr	r4, [r0, #0x24]
  4591f8: e5907028     	ldr	r7, [r0, #0x28]
  4591fc: e3520000     	cmp	r2, #0
  459200: 05905008     	ldreq	r5, [r0, #0x8]
  459204: e0647007     	rsb	r7, r4, r7
  459208: e1a07147     	asr	r7, r7, #2
  45920c: 13a05000     	movne	r5, #0
  459210: e1550007     	cmp	r5, r7
  459214: e1a08001     	mov	r8, r1
  459218: aa00000c     	bge	0x459250 <ScriptManager::GetIDFromName(char const*, bool) const+0x60> @ imm = #0x30
  45921c: e1a06105     	lsl	r6, r5, #2
  459220: ea000003     	b	0x459234 <ScriptManager::GetIDFromName(char const*, bool) const+0x44> @ imm = #0xc
  459224: e2855001     	add	r5, r5, #1
  459228: e1550007     	cmp	r5, r7
  45922c: e2866004     	add	r6, r6, #4
  459230: 0a000006     	beq	0x459250 <ScriptManager::GetIDFromName(char const*, bool) const+0x60> @ imm = #0x18
  459234: e7940006     	ldr	r0, [r4, r6]
  459238: e1a01008     	mov	r1, r8
  45923c: ebfad529     	bl	0x30e6e8 <.plt+0x974>   @ imm = #-0x14ab5c
  459240: e3500000     	cmp	r0, #0
  459244: 1afffff6     	bne	0x459224 <ScriptManager::GetIDFromName(char const*, bool) const+0x34> @ imm = #-0x28
  459248: e1a00005     	mov	r0, r5
  45924c: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  459250: e3e05000     	mvn	r5, #0
  459254: e1a00005     	mov	r0, r5
  459258: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
