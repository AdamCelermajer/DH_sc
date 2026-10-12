
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00356c9c <SceneManager::~SceneManager()>:
  356c9c: e92d4010     	push	{r4, lr}
  356ca0: e1a04000     	mov	r4, r0
  356ca4: ebffffd0     	bl	0x356bec <SceneManager::~SceneManager()> @ imm = #-0xc0
  356ca8: e1a00004     	mov	r0, r4
  356cac: ebfee5e3     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x46874
  356cb0: e1a00004     	mov	r0, r4
  356cb4: e8bd8010     	pop	{r4, pc}
