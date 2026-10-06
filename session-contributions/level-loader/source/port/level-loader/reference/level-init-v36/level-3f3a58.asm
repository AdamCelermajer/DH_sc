
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003f3a58 <Level::_LoadScripts()>:
  3f3a58: e92d4070     	push	{r4, r5, r6, lr}
  3f3a5c: e59f50b8     	ldr	r5, [pc, #0xb8]         @ 0x3f3b1c <Level::_LoadScripts()+0xc4>
  3f3a60: e59f30b8     	ldr	r3, [pc, #0xb8]         @ 0x3f3b20 <Level::_LoadScripts()+0xc8>
  3f3a64: e24dd008     	sub	sp, sp, #8
  3f3a68: e08f5005     	add	r5, pc, r5
  3f3a6c: e7956003     	ldr	r6, [r5, r3]
  3f3a70: e1a04000     	mov	r4, r0
  3f3a74: e1a00006     	mov	r0, r6
  3f3a78: eb0199d2     	bl	0x45a1c8 <ScriptManager::UnLoadAllScripts()> @ imm = #0x66748
  3f3a7c: e59f10a0     	ldr	r1, [pc, #0xa0]         @ 0x3f3b24 <Level::_LoadScripts()+0xcc>
  3f3a80: e1a00006     	mov	r0, r6
  3f3a84: e3a02001     	mov	r2, #1
  3f3a88: e08f1001     	add	r1, pc, r1
  3f3a8c: eb019df4     	bl	0x45b264 <ScriptManager::LoadScriptFile(char const*, bool)> @ imm = #0x677d0
  3f3a90: e59f1090     	ldr	r1, [pc, #0x90]         @ 0x3f3b28 <Level::_LoadScripts()+0xd0>
  3f3a94: e1a00006     	mov	r0, r6
  3f3a98: e3a02001     	mov	r2, #1
  3f3a9c: e08f1001     	add	r1, pc, r1
  3f3aa0: eb019ca6     	bl	0x45ad40 <ScriptManager::LoadScriptFileNames(char const*, bool)> @ imm = #0x67298
  3f3aa4: e5941038     	ldr	r1, [r4, #0x38]
  3f3aa8: e3510000     	cmp	r1, #0
  3f3aac: 0a000004     	beq	0x3f3ac4 <Level::_LoadScripts()+0x6c> @ imm = #0x10
  3f3ab0: e1a00004     	mov	r0, r4
  3f3ab4: e2811e15     	add	r1, r1, #336
  3f3ab8: e28dd008     	add	sp, sp, #8
  3f3abc: e8bd4070     	pop	{r4, r5, r6, lr}
  3f3ac0: eaffff76     	b	0x3f38a0 <Level::_LoadScriptFile(std::string&)> @ imm = #-0x228
  3f3ac4: e59f3060     	ldr	r3, [pc, #0x60]         @ 0x3f3b2c <Level::_LoadScripts()+0xd4>
  3f3ac8: e7953003     	ldr	r3, [r5, r3]
  3f3acc: e5933000     	ldr	r3, [r3]
  3f3ad0: e3530002     	cmp	r3, #2
  3f3ad4: 05811000     	streq	r1, [r1]
  3f3ad8: 0afffff4     	beq	0x3f3ab0 <Level::_LoadScripts()+0x58> @ imm = #-0x30
  3f3adc: e3530001     	cmp	r3, #1
  3f3ae0: 1afffff2     	bne	0x3f3ab0 <Level::_LoadScripts()+0x58> @ imm = #-0x38
  3f3ae4: e59f0044     	ldr	r0, [pc, #0x44]         @ 0x3f3b30 <Level::_LoadScripts()+0xd8>
  3f3ae8: e59f1044     	ldr	r1, [pc, #0x44]         @ 0x3f3b34 <Level::_LoadScripts()+0xdc>
  3f3aec: e59f2044     	ldr	r2, [pc, #0x44]         @ 0x3f3b38 <Level::_LoadScripts()+0xe0>
  3f3af0: e7950000     	ldr	r0, [r5, r0]
  3f3af4: e59f3040     	ldr	r3, [pc, #0x40]         @ 0x3f3b3c <Level::_LoadScripts()+0xe4>
  3f3af8: e08f1001     	add	r1, pc, r1
  3f3afc: e3a0cf77     	mov	r12, #476
  3f3b00: e28000a8     	add	r0, r0, #168
  3f3b04: e08f2002     	add	r2, pc, r2
  3f3b08: e08f3003     	add	r3, pc, r3
  3f3b0c: e58dc000     	str	r12, [sp]
  3f3b10: ebfc693b     	bl	0x30e004 <.plt+0x290>   @ imm = #-0xe5b14
  3f3b14: e5941038     	ldr	r1, [r4, #0x38]
  3f3b18: eaffffe4     	b	0x3f3ab0 <Level::_LoadScripts()+0x58> @ imm = #-0x70
  3f3b1c: 28 10 5a 00  	.word	0x005a1028
  3f3b20: 20 1a 00 00  	.word	0x00001a20
  3f3b24: d8 2c 4d 00  	.word	0x004d2cd8
  3f3b28: ec 2c 4d 00  	.word	0x004d2cec
  3f3b2c: c0 39 00 00  	.word	0x000039c0
  3f3b30: c0 19 00 00  	.word	0x000019c0
  3f3b34: e0 a8 4c 00  	.word	0x004ca8e0
  3f3b38: ec 20 4d 00  	.word	0x004d20ec
  3f3b3c: 50 e1 4c 00  	.word	0x004ce150
