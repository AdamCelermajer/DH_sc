
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00455be0 <ScriptManager::StopSkipping()>:
  455be0: e3e03000     	mvn	r3, #0
  455be4: e5803000     	str	r3, [r0]
  455be8: e12fff1e     	bx	lr

00455bec <ScriptManager::IsScriptRunning(int) const>:
  455bec: e590300c     	ldr	r3, [r0, #0xc]
  455bf0: e3a0200c     	mov	r2, #12
  455bf4: e0233192     	mla	r3, r2, r1, r3
  455bf8: e5930008     	ldr	r0, [r3, #0x8]
  455bfc: e2500002     	subs	r0, r0, #2
  455c00: 13a00001     	movne	r0, #1
  455c04: e12fff1e     	bx	lr

00455c08 <Script_ExecScript::IsBlocking() const>:
  455c08: e590200c     	ldr	r2, [r0, #0xc]
  455c0c: e59f3024     	ldr	r3, [pc, #0x24]         @ 0x455c38 <Script_ExecScript::IsBlocking() const+0x30>
  455c10: e5d22018     	ldrb	r2, [r2, #0x18]
  455c14: e08f3003     	add	r3, pc, r3
  455c18: e3520000     	cmp	r2, #0
  455c1c: 1a000001     	bne	0x455c28 <Script_ExecScript::IsBlocking() const+0x20> @ imm = #0x4
  455c20: e1a00002     	mov	r0, r2
  455c24: e12fff1e     	bx	lr
  455c28: e59f200c     	ldr	r2, [pc, #0xc]          @ 0x455c3c <Script_ExecScript::IsBlocking() const+0x34>
  455c2c: e5901010     	ldr	r1, [r0, #0x10]
  455c30: e7930002     	ldr	r0, [r3, r2]
