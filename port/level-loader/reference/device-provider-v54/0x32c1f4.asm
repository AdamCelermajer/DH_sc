
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0032c1f4 <Application::GoToMainMenu(GoToMainMenuEvent)>:
  32c1f4: e92d47f0     	push	{r4, r5, r6, r7, r8, r9, r10, lr}
  32c1f8: e59f4208     	ldr	r4, [pc, #0x208]        @ 0x32c408 <Application::GoToMainMenu(GoToMainMenuEvent)+0x214>
  32c1fc: e59f3208     	ldr	r3, [pc, #0x208]        @ 0x32c40c <Application::GoToMainMenu(GoToMainMenuEvent)+0x218>
  32c200: e59f6208     	ldr	r6, [pc, #0x208]        @ 0x32c410 <Application::GoToMainMenu(GoToMainMenuEvent)+0x21c>
  32c204: e08f4004     	add	r4, pc, r4
  32c208: e7942003     	ldr	r2, [r4, r3]
  32c20c: e59f3200     	ldr	r3, [pc, #0x200]        @ 0x32c414 <Application::GoToMainMenu(GoToMainMenuEvent)+0x220>
  32c210: e794a006     	ldr	r10, [r4, r6]
  32c214: e1a05000     	mov	r5, r0
  32c218: e794c003     	ldr	r12, [r4, r3]
  32c21c: e3a03000     	mov	r3, #0
  32c220: e1a0000a     	mov	r0, r10
  32c224: e5cc3000     	strb	r3, [r12]
  32c228: e5c23000     	strb	r3, [r2]
  32c22c: e1a07001     	mov	r7, r1
  32c230: ebffccd7     	bl	0x31f594 <Application::GetCurrentLevel() const> @ imm = #-0xcca4
  32c234: e2508000     	subs	r8, r0, #0
  32c238: 0a000002     	beq	0x32c248 <Application::GoToMainMenu(GoToMainMenuEvent)+0x54> @ imm = #0x8
  32c23c: e5983130     	ldr	r3, [r8, #0x130]
  32c240: e3530026     	cmp	r3, #38
  32c244: 0a000049     	beq	0x32c370 <Application::GoToMainMenu(GoToMainMenuEvent)+0x17c> @ imm = #0x124
  32c248: eb04020f     	bl	0x42ca8c <MenuManager::GetInstance()> @ imm = #0x10083c
  32c24c: e1a08000     	mov	r8, r0
  32c250: eb040539     	bl	0x42d73c <MenuManager::Reset()> @ imm = #0x1014e4
  32c254: e59f01bc     	ldr	r0, [pc, #0x1bc]        @ 0x32c418 <Application::GoToMainMenu(GoToMainMenuEvent)+0x224>
  32c258: e08f0000     	add	r0, pc, r0
  32c25c: ebffdfac     	bl	0x324114 <_DEBUG_OUT(char const*, ...)> @ imm = #-0x8150
  32c260: e59f11b4     	ldr	r1, [pc, #0x1b4]        @ 0x32c41c <Application::GoToMainMenu(GoToMainMenuEvent)+0x228>
