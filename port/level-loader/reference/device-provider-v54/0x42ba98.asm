
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0042ba98 <MenuMainMenu::GotFocus()>:
  42ba98: e59f3038     	ldr	r3, [pc, #0x38]         @ 0x42bad8 <MenuMainMenu::GotFocus()+0x40>
  42ba9c: e59f2038     	ldr	r2, [pc, #0x38]         @ 0x42badc <MenuMainMenu::GotFocus()+0x44>
  42baa0: e52d4004     	str	r4, [sp, #-0x4]!
  42baa4: e08f3003     	add	r3, pc, r3
  42baa8: e793c002     	ldr	r12, [r3, r2]
  42baac: e59f202c     	ldr	r2, [pc, #0x2c]         @ 0x42bae0 <MenuMainMenu::GotFocus()+0x48>
  42bab0: e3a04001     	mov	r4, #1
  42bab4: e58c4000     	str	r4, [r12]
  42bab8: e7930002     	ldr	r0, [r3, r2]
  42babc: e59f2020     	ldr	r2, [pc, #0x20]         @ 0x42bae4 <MenuMainMenu::GotFocus()+0x4c>
  42bac0: e7931002     	ldr	r1, [r3, r2]
  42bac4: e3a02000     	mov	r2, #0
  42bac8: e5c02000     	strb	r2, [r0]
  42bacc: e5c12000     	strb	r2, [r1]
  42bad0: e8bd0010     	ldm	sp!, {r4}
  42bad4: e12fff1e     	bx	lr
  42bad8: ec 8f 56 00  	.word	0x00568fec
  42badc: 50 38 00 00  	.word	0x00003850
  42bae0: dc 2b 00 00  	.word	0x00002bdc
  42bae4: d4 29 00 00  	.word	0x000029d4

0042bae8 <MenuMainMenu::LostFocus()>:
  42bae8: e12fff1e     	bx	lr

0042baec <MenuMainMenu::DestroyAvatarCamera()>:
  42baec: e59f3048     	ldr	r3, [pc, #0x48]         @ 0x42bb3c <MenuMainMenu::DestroyAvatarCamera()+0x50>
  42baf0: e59f2048     	ldr	r2, [pc, #0x48]         @ 0x42bb40 <MenuMainMenu::DestroyAvatarCamera()+0x54>
  42baf4: e92d4010     	push	{r4, lr}
  42baf8: e08f3003     	add	r3, pc, r3
  42bafc: e7934002     	ldr	r4, [r3, r2]
  42bb00: e5943000     	ldr	r3, [r4]
  42bb04: e3530000     	cmp	r3, #0
