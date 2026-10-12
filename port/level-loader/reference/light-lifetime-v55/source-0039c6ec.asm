
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0039c6ec <TriggerZoneExitLevel::~TriggerZoneExitLevel()>:
  39c6ec: e92d4010     	push	{r4, lr}
  39c6f0: e1a04000     	mov	r4, r0
  39c6f4: ebffffe4     	bl	0x39c68c <TriggerZoneExitLevel::~TriggerZoneExitLevel()> @ imm = #-0x70
  39c6f8: e1a00004     	mov	r0, r4
  39c6fc: ebfdcf4f     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x8c2c4
  39c700: e1a00004     	mov	r0, r4
  39c704: e8bd8010     	pop	{r4, pc}
