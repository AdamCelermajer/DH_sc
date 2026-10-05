
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0050a504 <AssetManager::loadSceneNode(char const*, char const*, bool, int)>:
  50a504: e59f002c     	ldr	r0, [pc, #0x2c]         @ 0x50a538 <AssetManager::loadSceneNode(char const*, char const*, bool, int)+0x34>
  50a508: e59f302c     	ldr	r3, [pc, #0x2c]         @ 0x50a53c <AssetManager::loadSceneNode(char const*, char const*, bool, int)+0x38>
  50a50c: e52d4004     	str	r4, [sp, #-0x4]!
  50a510: e08f0000     	add	r0, pc, r0
  50a514: e7904003     	ldr	r4, [r0, r3]
  50a518: e252c000     	subs	r12, r2, #0
  50a51c: 13a0c001     	movne	r12, #1
  50a520: e3a03000     	mov	r3, #0
  50a524: e5940010     	ldr	r0, [r4, #0x10]
  50a528: e590001c     	ldr	r0, [r0, #0x1c]
  50a52c: e58dc004     	str	r12, [sp, #0x4]
  50a530: e8bd0010     	ldm	sp!, {r4}
  50a534: eaf93c6f     	b	0x3596f8 <SceneManager::LoadScene(char const*, char const*, bool, bool)> @ imm = #-0x1b0e44
  50a538: 80 a5 48 00  	.word	0x0048a580
  50a53c: f4 37 00 00  	.word	0x000037f4
