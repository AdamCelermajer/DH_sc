
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00496ba4 <VisualFXManager::_BuildAnimLibrary()>:
  496ba4: e92d4010     	push	{r4, lr}
  496ba8: e5902028     	ldr	r2, [r0, #0x28]
  496bac: e590302c     	ldr	r3, [r0, #0x2c]
  496bb0: e1a04000     	mov	r4, r0
  496bb4: e1520003     	cmp	r2, r3
  496bb8: 0a000000     	beq	0x496bc0 <VisualFXManager::_BuildAnimLibrary()+0x1c> @ imm = #0x0
  496bbc: e8bd8010     	pop	{r4, pc}
  496bc0: e3a03001     	mov	r3, #1
  496bc4: e5c03004     	strb	r3, [r0, #0x4]
  496bc8: ebfff9e5     	bl	0x495364 <VisualFXManager::_BuildAnimFXDict()> @ imm = #-0x186c
  496bcc: e1a00004     	mov	r0, r4
  496bd0: e8bd4010     	pop	{r4, lr}
  496bd4: eaffff5e     	b	0x496954 <VisualFXManager::_BuildAnimFXSets()> @ imm = #-0x288
