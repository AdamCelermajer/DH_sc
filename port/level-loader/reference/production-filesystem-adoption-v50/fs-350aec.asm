
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00350aec <IrrFactory::createFileSystem()>:
  350aec: e92d4070     	push	{r4, r5, r6, lr}
  350af0: e3a01000     	mov	r1, #0
  350af4: e1a04000     	mov	r4, r0
  350af8: e3a00d15     	mov	r0, #1344
  350afc: ebfefe9b     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x40594
  350b00: e1a05000     	mov	r5, r0
  350b04: ebfffafa     	bl	0x34f6f4 <FileSystemWin32::FileSystemWin32()> @ imm = #-0x1418
  350b08: e3550000     	cmp	r5, #0
  350b0c: e5845000     	str	r5, [r4]
  350b10: 15953004     	ldrne	r3, [r5, #0x4]
  350b14: e1a00004     	mov	r0, r4
  350b18: 12833001     	addne	r3, r3, #1
  350b1c: 15853004     	strne	r3, [r5, #0x4]
  350b20: e8bd8070     	pop	{r4, r5, r6, pc}
