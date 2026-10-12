
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0045a1c8 <ScriptManager::UnLoadAllScripts()>:
  45a1c8: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  45a1cc: e1a05000     	mov	r5, r0
  45a1d0: e595301c     	ldr	r3, [r5, #0x1c]
  45a1d4: e5900018     	ldr	r0, [r0, #0x18]
  45a1d8: e0603003     	rsb	r3, r0, r3
  45a1dc: e1a03143     	asr	r3, r3, #2
  45a1e0: e0837103     	add	r7, r3, r3, lsl #2
  45a1e4: e0877207     	add	r7, r7, r7, lsl #4
  45a1e8: e0877407     	add	r7, r7, r7, lsl #8
  45a1ec: e0877807     	add	r7, r7, r7, lsl #16
  45a1f0: e0837087     	add	r7, r3, r7, lsl #1
  45a1f4: e3570000     	cmp	r7, #0
  45a1f8: da000009     	ble	0x45a224 <ScriptManager::UnLoadAllScripts()+0x5c> @ imm = #0x24
  45a1fc: e3a04000     	mov	r4, #0
  45a200: e1a06004     	mov	r6, r4
  45a204: ea000000     	b	0x45a20c <ScriptManager::UnLoadAllScripts()+0x44> @ imm = #0x0
  45a208: e5950018     	ldr	r0, [r5, #0x18]
  45a20c: e0800004     	add	r0, r0, r4
  45a210: e2866001     	add	r6, r6, #1
  45a214: ebfff044     	bl	0x45632c <ScriptManager::ScriptCmds::Free()> @ imm = #-0x3ef0
  45a218: e1560007     	cmp	r6, r7
  45a21c: e284400c     	add	r4, r4, #12
  45a220: 1afffff8     	bne	0x45a208 <ScriptManager::UnLoadAllScripts()+0x40> @ imm = #-0x20
  45a224: e5952028     	ldr	r2, [r5, #0x28]
  45a228: e5953024     	ldr	r3, [r5, #0x24]
  45a22c: e0636002     	rsb	r6, r3, r2
  45a230: e1a06146     	asr	r6, r6, #2
  45a234: e3560000     	cmp	r6, #0
  45a238: da00000c     	ble	0x45a270 <ScriptManager::UnLoadAllScripts()+0xa8> @ imm = #0x30
  45a23c: e3a04000     	mov	r4, #0
  45a240: e1a07004     	mov	r7, r4
  45a244: ea000000     	b	0x45a24c <ScriptManager::UnLoadAllScripts()+0x84> @ imm = #0x0
  45a248: e5953024     	ldr	r3, [r5, #0x24]
  45a24c: e7930104     	ldr	r0, [r3, r4, lsl #2]
  45a250: ebfad87a     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x149e18
  45a254: e5953024     	ldr	r3, [r5, #0x24]
  45a258: e7837104     	str	r7, [r3, r4, lsl #2]
  45a25c: e2844001     	add	r4, r4, #1
  45a260: e1540006     	cmp	r4, r6
  45a264: 1afffff7     	bne	0x45a248 <ScriptManager::UnLoadAllScripts()+0x80> @ imm = #-0x24
  45a268: e5953024     	ldr	r3, [r5, #0x24]
  45a26c: e5952028     	ldr	r2, [r5, #0x28]
  45a270: e1530002     	cmp	r3, r2
  45a274: 15853028     	strne	r3, [r5, #0x28]
  45a278: e595201c     	ldr	r2, [r5, #0x1c]
  45a27c: e5953018     	ldr	r3, [r5, #0x18]
  45a280: e1a00005     	mov	r0, r5
  45a284: e1530002     	cmp	r3, r2
  45a288: 1585301c     	strne	r3, [r5, #0x1c]
  45a28c: e5952010     	ldr	r2, [r5, #0x10]
  45a290: e595300c     	ldr	r3, [r5, #0xc]
  45a294: e1530002     	cmp	r3, r2
  45a298: 15853010     	strne	r3, [r5, #0x10]
  45a29c: ebffee4f     	bl	0x455be0 <ScriptManager::StopSkipping()> @ imm = #-0x46c4
  45a2a0: e3a03000     	mov	r3, #0
  45a2a4: e5853008     	str	r3, [r5, #0x8]
  45a2a8: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
