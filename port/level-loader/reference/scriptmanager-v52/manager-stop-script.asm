
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00455bb8 <ScriptManager::StopScript(int, bool)>:
  455bb8: e3a0300c     	mov	r3, #12
  455bbc: e0030193     	mul	r3, r3, r1
  455bc0: e590200c     	ldr	r2, [r0, #0xc]
  455bc4: e3a01002     	mov	r1, #2
  455bc8: e0822003     	add	r2, r2, r3
  455bcc: e5821008     	str	r1, [r2, #0x8]
  455bd0: e590200c     	ldr	r2, [r0, #0xc]
  455bd4: e3e01000     	mvn	r1, #0
  455bd8: e7821003     	str	r1, [r2, r3]
  455bdc: e12fff1e     	bx	lr
