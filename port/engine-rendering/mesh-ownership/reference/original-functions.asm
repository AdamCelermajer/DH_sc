; Exact-range ARM32 listings from the APK ELF member. Not assembler-ready source.
; APK SHA-256: 32c2d027b585a42547311cd95da6a3975fdb3174e663e513d42a7f49d1a4c200
; ELF SHA-256: 36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80
; Every listed code row is emitted by llvm-objdump from the hash-matched shared ELF.

; FUNCTION collada_mesh_constructor: _ZN6glitch7collada5CMeshC1ERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverERKNS0_9SGeometryERKNS0_13SBufferConfigESD_b
; demangled: glitch::collada::CMesh constructor; creates and stores primitive mesh buffers
; elf_va=0x00645588 size=1504 file_offset=0x00645588 sha256=f84f873b135b3507caae483dfa74e54d829dd0a31f9a517049e3c999d13cea0a
; decoder: llvm-objdump ARM mode; exact range byte-backed

00645588 <_ZN6glitch7collada5CMeshC1ERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverERKNS0_9SGeometryERKNS0_13SBufferConfigESD_b>:
  645588: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  64558c: e59f45bc     	ldr	r4, [pc, #0x5bc]        @ 0x645b50 <_ZN6glitch7collada5CMeshC1ERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverERKNS0_9SGeometryERKNS0_13SBufferConfigESD_b+0x5c8>
  645590: e59fc5bc     	ldr	r12, [pc, #0x5bc]       @ 0x645b54 <_ZN6glitch7collada5CMeshC1ERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverERKNS0_9SGeometryERKNS0_13SBufferConfigESD_b+0x5cc>
  645594: e1a0b000     	mov	r11, r0
  645598: e08f4004     	add	r4, pc, r4
  64559c: e794c00c     	ldr	r12, [r4, r12]
  6455a0: e3a00000     	mov	r0, #0
  6455a4: e58b0004     	str	r0, [r11, #0x4]
  6455a8: e28cc008     	add	r12, r12, #8
  6455ac: e58bc000     	str	r12, [r11]
  6455b0: e1a05001     	mov	r5, r1
  6455b4: e5911000     	ldr	r1, [r1]
  6455b8: e24dd074     	sub	sp, sp, #116
  6455bc: e58d202c     	str	r2, [sp, #0x2c]
  6455c0: e58d3034     	str	r3, [sp, #0x34]
  6455c4: e58b100c     	str	r1, [r11, #0xc]
  6455c8: e5953004     	ldr	r3, [r5, #0x4]
  6455cc: e1510000     	cmp	r1, r0
  6455d0: e58b3010     	str	r3, [r11, #0x10]
  6455d4: e5dd20a0     	ldrb	r2, [sp, #0xa0]
  6455d8: e58d2010     	str	r2, [sp, #0x10]
  6455dc: 0a000003     	beq	0x6455f0 <_ZN6glitch7collada5CMeshC1ERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverERKNS0_9SGeometryERKNS0_13SBufferConfigESD_b+0x68> @ imm = #0xc
  6455e0: e5913004     	ldr	r3, [r1, #0x4]
  6455e4: e1530000     	cmp	r3, r0
  6455e8: 12833001     	addne	r3, r3, #1
  6455ec: 15813004     	strne	r3, [r1, #0x4]
  6455f0: e59f0560     	ldr	r0, [pc, #0x560]        @ 0x645b58 <_ZN6glitch7collada5CMeshC1ERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverERKNS0_9SGeometryERKNS0_13SBufferConfigESD_b+0x5d0>
  6455f4: e59f3560     	ldr	r3, [pc, #0x560]        @ 0x645b5c <_ZN6glitch7collada5CMeshC1ERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverERKNS0_9SGeometryERKNS0_13SBufferConfigESD_b+0x5d4>
  6455f8: e3a014bf     	mov	r1, #-1090519040
  6455fc: e7940000     	ldr	r0, [r4, r0]
  645600: e7943003     	ldr	r3, [r4, r3]
  645604: e3a025fe     	mov	r2, #1065353216
  645608: e2811502     	add	r1, r1, #8388608
  64560c: e283c008     	add	r12, r3, #8
  645610: e2800004     	add	r0, r0, #4
  645614: e3a03000     	mov	r3, #0
  645618: e58b0008     	str	r0, [r11, #0x8]
  64561c: e58b102c     	str	r1, [r11, #0x2c]
  645620: e58b2038     	str	r2, [r11, #0x38]
  645624: e58b1024     	str	r1, [r11, #0x24]
  645628: e58b1028     	str	r1, [r11, #0x28]
  64562c: e58b2030     	str	r2, [r11, #0x30]
  645630: e58b2034     	str	r2, [r11, #0x34]
  645634: e58bc000     	str	r12, [r11]
  645638: e58b3020     	str	r3, [r11, #0x20]
  64563c: e58b3014     	str	r3, [r11, #0x14]
  645640: e58b3018     	str	r3, [r11, #0x18]
  645644: e58b301c     	str	r3, [r11, #0x1c]
  645648: e59dc034     	ldr	r12, [sp, #0x34]
  64564c: e28b1018     	add	r1, r11, #24
  645650: e1a00001     	mov	r0, r1
  645654: e59c3000     	ldr	r3, [r12]
  645658: e58d101c     	str	r1, [sp, #0x1c]
  64565c: e58b3008     	str	r3, [r11, #0x8]
  645660: e59c300c     	ldr	r3, [r12, #0xc]
  645664: e593300c     	ldr	r3, [r3, #0xc]
  645668: e1a01003     	mov	r1, r3
  64566c: e58d3018     	str	r3, [sp, #0x18]
  645670: ebfffeb2     	bl	0x645140 <_ZNSt6vectorIN6glitch7collada5CMesh7SBufferENS0_4core10SAllocatorIS3_LNS0_6memory13E_MEMORY_HINTE0EEEE7reserveEj> @ imm = #-0x538
  645674: e5953000     	ldr	r3, [r5]
  645678: e59d2034     	ldr	r2, [sp, #0x34]
  64567c: e5933024     	ldr	r3, [r3, #0x24]
  645680: e592700c     	ldr	r7, [r2, #0xc]
  645684: e5932020     	ldr	r2, [r3, #0x20]
  645688: e5923004     	ldr	r3, [r2, #0x4]
  64568c: e5922064     	ldr	r2, [r2, #0x64]
  645690: e3520000     	cmp	r2, #0
  645694: d3a02000     	movle	r2, #0
  645698: c3a02001     	movgt	r2, #1
  64569c: e3530000     	cmp	r3, #0
  6456a0: e58d2014     	str	r2, [sp, #0x14]
  6456a4: 058d3038     	streq	r3, [sp, #0x38]
  6456a8: 0a00000a     	beq	0x6456d8 <_ZN6glitch7collada5CMeshC1ERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverERKNS0_9SGeometryERKNS0_13SBufferConfigESD_b+0x150> @ imm = #0x28
  6456ac: e5931014     	ldr	r1, [r3, #0x14]
  6456b0: e59f34a8     	ldr	r3, [pc, #0x4a8]        @ 0x645b60 <_ZN6glitch7collada5CMeshC1ERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverERKNS0_9SGeometryERKNS0_13SBufferConfigESD_b+0x5d8>
  6456b4: e7943003     	ldr	r3, [r4, r3]
  6456b8: e5933000     	ldr	r3, [r3]
  6456bc: e5933020     	ldr	r3, [r3, #0x20]
  6456c0: e5933034     	ldr	r3, [r3, #0x34]
  6456c4: e1a00003     	mov	r0, r3
  6456c8: e5933000     	ldr	r3, [r3]
  6456cc: e1a0e00f     	mov	lr, pc
  6456d0: e593f00c     	ldr	pc, [r3, #0xc]
  6456d4: e58d0038     	str	r0, [sp, #0x38]
  6456d8: e59f3484     	ldr	r3, [pc, #0x484]        @ 0x645b64 <_ZN6glitch7collada5CMeshC1ERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverERKNS0_9SGeometryERKNS0_13SBufferConfigESD_b+0x5dc>
  6456dc: e59dc014     	ldr	r12, [sp, #0x14]
  6456e0: e59d1038     	ldr	r1, [sp, #0x38]
  6456e4: e7943003     	ldr	r3, [r4, r3]
  6456e8: e35c0000     	cmp	r12, #0
  6456ec: e58d1058     	str	r1, [sp, #0x58]
  6456f0: e2833008     	add	r3, r3, #8
  6456f4: e58d3054     	str	r3, [sp, #0x54]
  6456f8: 0a000002     	beq	0x645708 <_ZN6glitch7collada5CMeshC1ERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverERKNS0_9SGeometryERKNS0_13SBufferConfigESD_b+0x180> @ imm = #0x8
  6456fc: e5973000     	ldr	r3, [r7]
  645700: e3530000     	cmp	r3, #0
  645704: 1a0000f3     	bne	0x645ad8 <_ZN6glitch7collada5CMeshC1ERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverERKNS0_9SGeometryERKNS0_13SBufferConfigESD_b+0x550> @ imm = #0x3cc
  645708: e3a01000     	mov	r1, #0
  64570c: e58d1030     	str	r1, [sp, #0x30]
  645710: e59d2018     	ldr	r2, [sp, #0x18]
  645714: e3520000     	cmp	r2, #0
  645718: 0a0000c6     	beq	0x645a38 <_ZN6glitch7collada5CMeshC1ERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverERKNS0_9SGeometryERKNS0_13SBufferConfigESD_b+0x4b0> @ imm = #0x318
  64571c: e28dc05c     	add	r12, sp, #92
  645720: e28d1060     	add	r1, sp, #96
  645724: e3a08000     	mov	r8, #0
  645728: e28d3048     	add	r3, sp, #72
  64572c: e58dc020     	str	r12, [sp, #0x20]
  645730: e58d1024     	str	r1, [sp, #0x24]
  645734: e28d2064     	add	r2, sp, #100
  645738: e28dc054     	add	r12, sp, #84
  64573c: e28d1068     	add	r1, sp, #104
  645740: e58db044     	str	r11, [sp, #0x44]
  645744: e1a0a008     	mov	r10, r8
  645748: e1a09008     	mov	r9, r8
  64574c: e58d203c     	str	r2, [sp, #0x3c]
  645750: e58dc028     	str	r12, [sp, #0x28]
  645754: e58d1040     	str	r1, [sp, #0x40]
  645758: e1a0b003     	mov	r11, r3
  64575c: ea000071     	b	0x645928 <_ZN6glitch7collada5CMeshC1ERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverERKNS0_9SGeometryERKNS0_13SBufferConfigESD_b+0x3a0> @ imm = #0x1c4
  645760: e5973010     	ldr	r3, [r7, #0x10]
  645764: e30fcfff     	movw	r12, #0xffff
  645768: e0833008     	add	r3, r3, r8
  64576c: e5932024     	ldr	r2, [r3, #0x24]
  645770: e152000c     	cmp	r2, r12
  645774: da000092     	ble	0x6459c4 <_ZN6glitch7collada5CMeshC1ERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverERKNS0_9SGeometryERKNS0_13SBufferConfigESD_b+0x43c> @ imm = #0x248
  645778: e593102c     	ldr	r1, [r3, #0x2c]
  64577c: e59d0040     	ldr	r0, [sp, #0x40]
  645780: e59d2028     	ldr	r2, [sp, #0x28]
  645784: ebfffecc     	bl	0x6452bc <_ZN6glitch3res8onDemandIjE3getERNS0_14onDemandReaderE> @ imm = #-0x4d0
  645788: e59d3068     	ldr	r3, [sp, #0x68]
  64578c: e3530000     	cmp	r3, #0
  645790: 0a000067     	beq	0x645934 <_ZN6glitch7collada5CMeshC1ERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverERKNS0_9SGeometryERKNS0_13SBufferConfigESD_b+0x3ac> @ imm = #0x19c
  645794: e5932000     	ldr	r2, [r3]
  645798: e2822001     	add	r2, r2, #1
  64579c: e5832000     	str	r2, [r3]
  6457a0: e59d5068     	ldr	r5, [sp, #0x68]
  6457a4: e3550000     	cmp	r5, #0
  6457a8: 0a000062     	beq	0x645938 <_ZN6glitch7collada5CMeshC1ERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverERKNS0_9SGeometryERKNS0_13SBufferConfigESD_b+0x3b0> @ imm = #0x188
  6457ac: e5953000     	ldr	r3, [r5]
  6457b0: e2433001     	sub	r3, r3, #1
  6457b4: e3530000     	cmp	r3, #0
  6457b8: e5853000     	str	r3, [r5]
  6457bc: 1a000005     	bne	0x6457d8 <_ZN6glitch7collada5CMeshC1ERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverERKNS0_9SGeometryERKNS0_13SBufferConfigESD_b+0x250> @ imm = #0x14
  6457c0: e595000c     	ldr	r0, [r5, #0xc]
  6457c4: e3500000     	cmp	r0, #0
  6457c8: 0a000000     	beq	0x6457d0 <_ZN6glitch7collada5CMeshC1ERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverERKNS0_9SGeometryERKNS0_13SBufferConfigESD_b+0x248> @ imm = #0x0
  6457cc: ebf32239     	bl	0x30e0b8 <_ZdaPv@plt>   @ imm = #-0x33771c
  6457d0: e3a01000     	mov	r1, #0
  6457d4: e585100c     	str	r1, [r5, #0xc]
  6457d8: e58d9068     	str	r9, [sp, #0x68]
  6457dc: e1a06009     	mov	r6, r9
  6457e0: e59d3010     	ldr	r3, [sp, #0x10]
  6457e4: e3530000     	cmp	r3, #0
  6457e8: 0a000056     	beq	0x645948 <_ZN6glitch7collada5CMeshC1ERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverERKNS0_9SGeometryERKNS0_13SBufferConfigESD_b+0x3c0> @ imm = #0x158
  6457ec: e5973010     	ldr	r3, [r7, #0x10]
  6457f0: e0833008     	add	r3, r3, r8
  6457f4: e5934034     	ldr	r4, [r3, #0x34]
  6457f8: e3540000     	cmp	r4, #0
  6457fc: 0a000051     	beq	0x645948 <_ZN6glitch7collada5CMeshC1ERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverERKNS0_9SGeometryERKNS0_13SBufferConfigESD_b+0x3c0> @ imm = #0x144
  645800: e5943004     	ldr	r3, [r4, #0x4]
  645804: e2833001     	add	r3, r3, #1
  645808: e5843004     	str	r3, [r4, #0x4]
  64580c: e3540000     	cmp	r4, #0
  645810: e58d9060     	str	r9, [sp, #0x60]
  645814: e58d905c     	str	r9, [sp, #0x5c]
  645818: e58d4048     	str	r4, [sp, #0x48]
  64581c: 058d404c     	streq	r4, [sp, #0x4c]
  645820: 0a000008     	beq	0x645848 <_ZN6glitch7collada5CMeshC1ERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverERKNS0_9SGeometryERKNS0_13SBufferConfigESD_b+0x2c0> @ imm = #0x20
  645824: e5943004     	ldr	r3, [r4, #0x4]
  645828: e2833001     	add	r3, r3, #1
  64582c: e5843004     	str	r3, [r4, #0x4]
  645830: e59d3060     	ldr	r3, [sp, #0x60]
  645834: e3530000     	cmp	r3, #0
  645838: e58d304c     	str	r3, [sp, #0x4c]
  64583c: 15932000     	ldrne	r2, [r3]
  645840: 12822001     	addne	r2, r2, #1
  645844: 15832000     	strne	r2, [r3]
  645848: e59d305c     	ldr	r3, [sp, #0x5c]
  64584c: e1a0100b     	mov	r1, r11
  645850: e3530000     	cmp	r3, #0
  645854: e58d3050     	str	r3, [sp, #0x50]
  645858: 15932000     	ldrne	r2, [r3]
  64585c: 12822001     	addne	r2, r2, #1
  645860: 15832000     	strne	r2, [r3]
  645864: e59d001c     	ldr	r0, [sp, #0x1c]
  645868: ebfffec9     	bl	0x645394 <_ZNSt6vectorIN6glitch7collada5CMesh7SBufferENS0_4core10SAllocatorIS3_LNS0_6memory13E_MEMORY_HINTE0EEEE9push_backERKS3_> @ imm = #-0x4dc
  64586c: e1a0000b     	mov	r0, r11
  645870: ebfffd04     	bl	0x644c88 <_ZN6glitch7collada5CMesh7SBufferD1Ev> @ imm = #-0xbf0
  645874: e59d0020     	ldr	r0, [sp, #0x20]
  645878: ebfcd27b     	bl	0x57a26c <_ZN5boost13intrusive_ptrIN6glitch5video27CMaterialVertexAttributeMapEED1Ev> @ imm = #-0xcb614
  64587c: e59d0024     	ldr	r0, [sp, #0x24]
  645880: ebf32cd8     	bl	0x310be8 <_ZN5boost13intrusive_ptrIN6glitch5video9CMaterialEED1Ev> @ imm = #-0x334ca0
  645884: e59d1010     	ldr	r1, [sp, #0x10]
  645888: e3510000     	cmp	r1, #0
  64588c: 0a000004     	beq	0x6458a4 <_ZN6glitch7collada5CMeshC1ERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverERKNS0_9SGeometryERKNS0_13SBufferConfigESD_b+0x31c> @ imm = #0x10
  645890: e5973010     	ldr	r3, [r7, #0x10]
  645894: e0833008     	add	r3, r3, r8
  645898: e5932034     	ldr	r2, [r3, #0x34]
  64589c: e3520000     	cmp	r2, #0
  6458a0: 0a000039     	beq	0x64598c <_ZN6glitch7collada5CMeshC1ERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverERKNS0_9SGeometryERKNS0_13SBufferConfigESD_b+0x404> @ imm = #0xe4
  6458a4: e3540000     	cmp	r4, #0
  6458a8: 1a000042     	bne	0x6459b8 <_ZN6glitch7collada5CMeshC1ERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverERKNS0_9SGeometryERKNS0_13SBufferConfigESD_b+0x430> @ imm = #0x108
  6458ac: e3550000     	cmp	r5, #0
  6458b0: 0a00000a     	beq	0x6458e0 <_ZN6glitch7collada5CMeshC1ERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverERKNS0_9SGeometryERKNS0_13SBufferConfigESD_b+0x358> @ imm = #0x28
  6458b4: e5953000     	ldr	r3, [r5]
  6458b8: e2433001     	sub	r3, r3, #1
  6458bc: e3530000     	cmp	r3, #0
  6458c0: e5853000     	str	r3, [r5]
  6458c4: 1a000005     	bne	0x6458e0 <_ZN6glitch7collada5CMeshC1ERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverERKNS0_9SGeometryERKNS0_13SBufferConfigESD_b+0x358> @ imm = #0x14
  6458c8: e595000c     	ldr	r0, [r5, #0xc]
  6458cc: e3500000     	cmp	r0, #0
  6458d0: 0a000000     	beq	0x6458d8 <_ZN6glitch7collada5CMeshC1ERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverERKNS0_9SGeometryERKNS0_13SBufferConfigESD_b+0x350> @ imm = #0x0
  6458d4: ebf321f7     	bl	0x30e0b8 <_ZdaPv@plt>   @ imm = #-0x337824
  6458d8: e3a02000     	mov	r2, #0
  6458dc: e585200c     	str	r2, [r5, #0xc]
  6458e0: e3560000     	cmp	r6, #0
  6458e4: 0a00000a     	beq	0x645914 <_ZN6glitch7collada5CMeshC1ERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverERKNS0_9SGeometryERKNS0_13SBufferConfigESD_b+0x38c> @ imm = #0x28
  6458e8: e5963000     	ldr	r3, [r6]
  6458ec: e2433001     	sub	r3, r3, #1
  6458f0: e3530000     	cmp	r3, #0
  6458f4: e5863000     	str	r3, [r6]
  6458f8: 1a000005     	bne	0x645914 <_ZN6glitch7collada5CMeshC1ERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverERKNS0_9SGeometryERKNS0_13SBufferConfigESD_b+0x38c> @ imm = #0x14
  6458fc: e596000c     	ldr	r0, [r6, #0xc]
  645900: e3500000     	cmp	r0, #0
  645904: 0a000000     	beq	0x64590c <_ZN6glitch7collada5CMeshC1ERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverERKNS0_9SGeometryERKNS0_13SBufferConfigESD_b+0x384> @ imm = #0x0
  645908: ebf321ea     	bl	0x30e0b8 <_ZdaPv@plt>   @ imm = #-0x337858
  64590c: e3a03000     	mov	r3, #0
  645910: e586300c     	str	r3, [r6, #0xc]
  645914: e59dc018     	ldr	r12, [sp, #0x18]
  645918: e28aa001     	add	r10, r10, #1
  64591c: e2888038     	add	r8, r8, #56
  645920: e15c000a     	cmp	r12, r10
  645924: 0a000042     	beq	0x645a34 <_ZN6glitch7collada5CMeshC1ERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverERKNS0_9SGeometryERKNS0_13SBufferConfigESD_b+0x4ac> @ imm = #0x108
  645928: e59d2014     	ldr	r2, [sp, #0x14]
  64592c: e3520000     	cmp	r2, #0
  645930: 1affff8a     	bne	0x645760 <_ZN6glitch7collada5CMeshC1ERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverERKNS0_9SGeometryERKNS0_13SBufferConfigESD_b+0x1d8> @ imm = #-0x1d8
  645934: e3a05000     	mov	r5, #0
  645938: e59d3010     	ldr	r3, [sp, #0x10]
  64593c: e1a06005     	mov	r6, r5
  645940: e3530000     	cmp	r3, #0
  645944: 1affffa8     	bne	0x6457ec <_ZN6glitch7collada5CMeshC1ERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverERKNS0_9SGeometryERKNS0_13SBufferConfigESD_b+0x264> @ imm = #-0x160
  645948: e3a01000     	mov	r1, #0
  64594c: e3a00038     	mov	r0, #56
  645950: ebfbba15     	bl	0x5341ac <_ZnwjN6glitch6memory13E_MEMORY_HINTE> @ imm = #-0x1117ac
  645954: e59dc098     	ldr	r12, [sp, #0x98]
  645958: e1a04000     	mov	r4, r0
  64595c: e59d102c     	ldr	r1, [sp, #0x2c]
  645960: e58dc000     	str	r12, [sp]
  645964: e59dc09c     	ldr	r12, [sp, #0x9c]
  645968: e1a02007     	mov	r2, r7
  64596c: e1a0300a     	mov	r3, r10
  645970: e58dc004     	str	r12, [sp, #0x4]
  645974: e59dc014     	ldr	r12, [sp, #0x14]
  645978: e58dc008     	str	r12, [sp, #0x8]
  64597c: eb01e01d     	bl	0x6bd9f8 <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b> @ imm = #0x78074
  645980: e3540000     	cmp	r4, #0
  645984: 1affff9d     	bne	0x645800 <_ZN6glitch7collada5CMeshC1ERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverERKNS0_9SGeometryERKNS0_13SBufferConfigESD_b+0x278> @ imm = #-0x18c
  645988: eaffff9f     	b	0x64580c <_ZN6glitch7collada5CMeshC1ERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverERKNS0_9SGeometryERKNS0_13SBufferConfigESD_b+0x284> @ imm = #-0x184
  64598c: e3540000     	cmp	r4, #0
  645990: 05834034     	streq	r4, [r3, #0x34]
  645994: 0affffc4     	beq	0x6458ac <_ZN6glitch7collada5CMeshC1ERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverERKNS0_9SGeometryERKNS0_13SBufferConfigESD_b+0x324> @ imm = #-0xf0
  645998: e5942004     	ldr	r2, [r4, #0x4]
  64599c: e2822001     	add	r2, r2, #1
  6459a0: e5842004     	str	r2, [r4, #0x4]
  6459a4: e5930034     	ldr	r0, [r3, #0x34]
  6459a8: e5834034     	str	r4, [r3, #0x34]
  6459ac: e3500000     	cmp	r0, #0
  6459b0: 0a000000     	beq	0x6459b8 <_ZN6glitch7collada5CMeshC1ERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverERKNS0_9SGeometryERKNS0_13SBufferConfigESD_b+0x430> @ imm = #0x0
  6459b4: ebf35ef2     	bl	0x31d584 <_ZNK6glitch17IReferenceCounted4dropEv> @ imm = #-0x328438
  6459b8: e1a00004     	mov	r0, r4
  6459bc: ebf35ef0     	bl	0x31d584 <_ZNK6glitch17IReferenceCounted4dropEv> @ imm = #-0x328440
  6459c0: eaffffb9     	b	0x6458ac <_ZN6glitch7collada5CMeshC1ERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverERKNS0_9SGeometryERKNS0_13SBufferConfigESD_b+0x324> @ imm = #-0x11c
  6459c4: e593102c     	ldr	r1, [r3, #0x2c]
  6459c8: e59d003c     	ldr	r0, [sp, #0x3c]
  6459cc: e59d2028     	ldr	r2, [sp, #0x28]
  6459d0: ebfffe54     	bl	0x645328 <_ZN6glitch3res8onDemandItE3getERNS0_14onDemandReaderE> @ imm = #-0x6b0
  6459d4: e59d3064     	ldr	r3, [sp, #0x64]
  6459d8: e3530000     	cmp	r3, #0
  6459dc: 0affffd4     	beq	0x645934 <_ZN6glitch7collada5CMeshC1ERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverERKNS0_9SGeometryERKNS0_13SBufferConfigESD_b+0x3ac> @ imm = #-0xb0
  6459e0: e5932000     	ldr	r2, [r3]
  6459e4: e2822001     	add	r2, r2, #1
  6459e8: e5832000     	str	r2, [r3]
  6459ec: e59d6064     	ldr	r6, [sp, #0x64]
  6459f0: e3560000     	cmp	r6, #0
  6459f4: 01a05006     	moveq	r5, r6
  6459f8: 0affff78     	beq	0x6457e0 <_ZN6glitch7collada5CMeshC1ERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverERKNS0_9SGeometryERKNS0_13SBufferConfigESD_b+0x258> @ imm = #-0x220
  6459fc: e5963000     	ldr	r3, [r6]
  645a00: e2433001     	sub	r3, r3, #1
  645a04: e3530000     	cmp	r3, #0
  645a08: e5863000     	str	r3, [r6]
  645a0c: 1a000005     	bne	0x645a28 <_ZN6glitch7collada5CMeshC1ERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverERKNS0_9SGeometryERKNS0_13SBufferConfigESD_b+0x4a0> @ imm = #0x14
  645a10: e596000c     	ldr	r0, [r6, #0xc]
  645a14: e3500000     	cmp	r0, #0
  645a18: 0a000000     	beq	0x645a20 <_ZN6glitch7collada5CMeshC1ERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverERKNS0_9SGeometryERKNS0_13SBufferConfigESD_b+0x498> @ imm = #0x0
  645a1c: ebf321a5     	bl	0x30e0b8 <_ZdaPv@plt>   @ imm = #-0x33796c
  645a20: e3a02000     	mov	r2, #0
  645a24: e586200c     	str	r2, [r6, #0xc]
  645a28: e58d9064     	str	r9, [sp, #0x64]
  645a2c: e1a05009     	mov	r5, r9
  645a30: eaffff6a     	b	0x6457e0 <_ZN6glitch7collada5CMeshC1ERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverERKNS0_9SGeometryERKNS0_13SBufferConfigESD_b+0x258> @ imm = #-0x258
  645a34: e59db044     	ldr	r11, [sp, #0x44]
  645a38: e59d1038     	ldr	r1, [sp, #0x38]
  645a3c: e3510000     	cmp	r1, #0
  645a40: 0a000001     	beq	0x645a4c <_ZN6glitch7collada5CMeshC1ERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverERKNS0_9SGeometryERKNS0_13SBufferConfigESD_b+0x4c4> @ imm = #0x4
  645a44: e1a00001     	mov	r0, r1
  645a48: ebf35ecd     	bl	0x31d584 <_ZNK6glitch17IReferenceCounted4dropEv> @ imm = #-0x3284cc
  645a4c: e59d2034     	ldr	r2, [sp, #0x34]
  645a50: e59dc030     	ldr	r12, [sp, #0x30]
  645a54: e592300c     	ldr	r3, [r2, #0xc]
  645a58: e35c0000     	cmp	r12, #0
  645a5c: e2831014     	add	r1, r3, #20
  645a60: e2832020     	add	r2, r3, #32
  645a64: e5914008     	ldr	r4, [r1, #0x8]
  645a68: e593c020     	ldr	r12, [r3, #0x20]
  645a6c: e5935014     	ldr	r5, [r3, #0x14]
  645a70: e5920008     	ldr	r0, [r2, #0x8]
  645a74: e5911004     	ldr	r1, [r1, #0x4]
  645a78: e5923004     	ldr	r3, [r2, #0x4]
  645a7c: e58b5024     	str	r5, [r11, #0x24]
  645a80: e58b1028     	str	r1, [r11, #0x28]
  645a84: e58b402c     	str	r4, [r11, #0x2c]
  645a88: e58bc030     	str	r12, [r11, #0x30]
  645a8c: e58b3034     	str	r3, [r11, #0x34]
  645a90: e58b0038     	str	r0, [r11, #0x38]
  645a94: 0a00000c     	beq	0x645acc <_ZN6glitch7collada5CMeshC1ERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverERKNS0_9SGeometryERKNS0_13SBufferConfigESD_b+0x544> @ imm = #0x30
  645a98: e59d1030     	ldr	r1, [sp, #0x30]
  645a9c: e5913000     	ldr	r3, [r1]
  645aa0: e2433001     	sub	r3, r3, #1
  645aa4: e3530000     	cmp	r3, #0
  645aa8: e5813000     	str	r3, [r1]
  645aac: 1a000006     	bne	0x645acc <_ZN6glitch7collada5CMeshC1ERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverERKNS0_9SGeometryERKNS0_13SBufferConfigESD_b+0x544> @ imm = #0x18
  645ab0: e591000c     	ldr	r0, [r1, #0xc]
  645ab4: e3500000     	cmp	r0, #0
  645ab8: 0a000000     	beq	0x645ac0 <_ZN6glitch7collada5CMeshC1ERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverERKNS0_9SGeometryERKNS0_13SBufferConfigESD_b+0x538> @ imm = #0x0
  645abc: ebf3217d     	bl	0x30e0b8 <_ZdaPv@plt>   @ imm = #-0x337a0c
  645ac0: e59d2030     	ldr	r2, [sp, #0x30]
  645ac4: e3a03000     	mov	r3, #0
  645ac8: e582300c     	str	r3, [r2, #0xc]
  645acc: e1a0000b     	mov	r0, r11
  645ad0: e28dd074     	add	sp, sp, #116
  645ad4: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  645ad8: e5973008     	ldr	r3, [r7, #0x8]
  645adc: e28d2054     	add	r2, sp, #84
  645ae0: e28d006c     	add	r0, sp, #108
  645ae4: e5931024     	ldr	r1, [r3, #0x24]
  645ae8: ebfffdd8     	bl	0x645250 <_ZN6glitch3res8onDemandIcE3getERNS0_14onDemandReaderE> @ imm = #-0x8a0
  645aec: e59d206c     	ldr	r2, [sp, #0x6c]
  645af0: e3520000     	cmp	r2, #0
  645af4: 0affff03     	beq	0x645708 <_ZN6glitch7collada5CMeshC1ERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverERKNS0_9SGeometryERKNS0_13SBufferConfigESD_b+0x180> @ imm = #-0x3f4
  645af8: e5923000     	ldr	r3, [r2]
  645afc: e2833001     	add	r3, r3, #1
  645b00: e5823000     	str	r3, [r2]
  645b04: e59d206c     	ldr	r2, [sp, #0x6c]
  645b08: e3520000     	cmp	r2, #0
  645b0c: e58d2030     	str	r2, [sp, #0x30]
  645b10: 0afffefe     	beq	0x645710 <_ZN6glitch7collada5CMeshC1ERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverERKNS0_9SGeometryERKNS0_13SBufferConfigESD_b+0x188> @ imm = #-0x408
  645b14: e5923000     	ldr	r3, [r2]
  645b18: e2433001     	sub	r3, r3, #1
  645b1c: e3530000     	cmp	r3, #0
  645b20: e5823000     	str	r3, [r2]
  645b24: 1a000006     	bne	0x645b44 <_ZN6glitch7collada5CMeshC1ERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverERKNS0_9SGeometryERKNS0_13SBufferConfigESD_b+0x5bc> @ imm = #0x18
  645b28: e592000c     	ldr	r0, [r2, #0xc]
  645b2c: e3500000     	cmp	r0, #0
  645b30: 0a000000     	beq	0x645b38 <_ZN6glitch7collada5CMeshC1ERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverERKNS0_9SGeometryERKNS0_13SBufferConfigESD_b+0x5b0> @ imm = #0x0
  645b34: ebf3215f     	bl	0x30e0b8 <_ZdaPv@plt>   @ imm = #-0x337a84
  645b38: e59dc030     	ldr	r12, [sp, #0x30]
  645b3c: e3a03000     	mov	r3, #0
  645b40: e58c300c     	str	r3, [r12, #0xc]
  645b44: e3a03000     	mov	r3, #0
  645b48: e58d306c     	str	r3, [sp, #0x6c]
  645b4c: eafffeef     	b	0x645710 <_ZN6glitch7collada5CMeshC1ERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverERKNS0_9SGeometryERKNS0_13SBufferConfigESD_b+0x188> @ imm = #-0x444
  645b50: f8 f4 34 00  	.word	0x0034f4f8
  645b54: 40 0a 00 00  	.word	0x00000a40
  645b58: b4 17 00 00  	.word	0x000017b4
  645b5c: c0 3d 00 00  	.word	0x00003dc0
  645b60: 48 44 00 00  	.word	0x00004448
  645b64: fc 46 00 00  	.word	0x000046fc

; FUNCTION meshbuffer_constructor_c1: _ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b
; demangled: scene::CMeshBuffer C1 constructor called by the Collada mesh constructor
; elf_va=0x006bd9f8 size=2680 file_offset=0x006bd9f8 sha256=175e56defbf9666c5cb012f976f63d79989b717a03b354c6543a0ab70b48a551
; decoder: llvm-objdump ARM mode; exact range byte-backed

006bd9f8 <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b>:
  6bd9f8: e59fca64     	ldr	r12, [pc, #0xa64]       @ 0x6be464 <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0xa6c>
  6bd9fc: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  6bda00: e59fea60     	ldr	lr, [pc, #0xa60]        @ 0x6be468 <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0xa70>
  6bda04: e08fc00c     	add	r12, pc, r12
  6bda08: e1a04000     	mov	r4, r0
  6bda0c: e79ce00e     	ldr	lr, [r12, lr]
  6bda10: e3a00000     	mov	r0, #0
  6bda14: e5840014     	str	r0, [r4, #0x14]
  6bda18: e28ee008     	add	lr, lr, #8
  6bda1c: e584e000     	str	lr, [r4]
  6bda20: e5840004     	str	r0, [r4, #0x4]
  6bda24: e5840008     	str	r0, [r4, #0x8]
  6bda28: e584000c     	str	r0, [r4, #0xc]
  6bda2c: e5840010     	str	r0, [r4, #0x10]
  6bda30: e3a05038     	mov	r5, #56
  6bda34: e0050395     	mul	r5, r5, r3
  6bda38: e1a06002     	mov	r6, r2
  6bda3c: e5922010     	ldr	r2, [r2, #0x10]
  6bda40: e1a09001     	mov	r9, r1
  6bda44: e59f3a20     	ldr	r3, [pc, #0xa20]        @ 0x6be46c <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0xa74>
  6bda48: e0821005     	add	r1, r2, r5
  6bda4c: e5910024     	ldr	r0, [r1, #0x24]
  6bda50: e7922005     	ldr	r2, [r2, r5]
  6bda54: e24dd044     	sub	sp, sp, #68
  6bda58: e08f3003     	add	r3, pc, r3
  6bda5c: e3500801     	cmp	r0, #65536
  6bda60: e793e102     	ldr	lr, [r3, r2, lsl #2]
  6bda64: e59d706c     	ldr	r7, [sp, #0x6c]
  6bda68: e5dd8070     	ldrb	r8, [sp, #0x70]
  6bda6c: ba00015c     	blt	0x6bdfe4 <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x5ec> @ imm = #0x570
  6bda70: e591c028     	ldr	r12, [r1, #0x28]
  6bda74: e3a02002     	mov	r2, #2
  6bda78: e5913030     	ldr	r3, [r1, #0x30]
  6bda7c: e5911020     	ldr	r1, [r1, #0x20]
  6bda80: e2800001     	add	r0, r0, #1
  6bda84: e3530000     	cmp	r3, #0
  6bda88: e5843018     	str	r3, [r4, #0x18]
  6bda8c: 1593a004     	ldrne	r10, [r3, #0x4]
  6bda90: 128aa001     	addne	r10, r10, #1
  6bda94: 1583a004     	strne	r10, [r3, #0x4]
  6bda98: e3a03000     	mov	r3, #0
  6bda9c: e584c020     	str	r12, [r4, #0x20]
  6bdaa0: e5841024     	str	r1, [r4, #0x24]
  6bdaa4: e5840028     	str	r0, [r4, #0x28]
  6bdaa8: e1c422bc     	strh	r2, [r4, #44]
  6bdaac: e1c4e2be     	strh	lr, [r4, #46]
  6bdab0: e5c43034     	strb	r3, [r4, #0x34]
  6bdab4: e584301c     	str	r3, [r4, #0x1c]
  6bdab8: e5843030     	str	r3, [r4, #0x30]
  6bdabc: e5963010     	ldr	r3, [r6, #0x10]
  6bdac0: e0835005     	add	r5, r3, r5
  6bdac4: e595a030     	ldr	r10, [r5, #0x30]
  6bdac8: e35a0000     	cmp	r10, #0
  6bdacc: 0a000151     	beq	0x6be018 <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x620> @ imm = #0x544
  6bdad0: e597b000     	ldr	r11, [r7]
  6bdad4: e5da3011     	ldrb	r3, [r10, #0x11]
  6bdad8: e15b0003     	cmp	r11, r3
  6bdadc: 0a00000b     	beq	0x6bdb10 <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x118> @ imm = #0x2c
  6bdae0: e5da3012     	ldrb	r3, [r10, #0x12]
  6bdae4: e3130008     	tst	r3, #8
  6bdae8: 1a000185     	bne	0x6be104 <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x70c> @ imm = #0x614
  6bdaec: e6efb07b     	uxtb	r11, r11
  6bdaf0: e35b0004     	cmp	r11, #4
  6bdaf4: e5cab011     	strb	r11, [r10, #0x11]
  6bdaf8: 0a000004     	beq	0x6bdb10 <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x118> @ imm = #0x10
  6bdafc: e59a3008     	ldr	r3, [r10, #0x8]
  6bdb00: e3530000     	cmp	r3, #0
  6bdb04: 15da3012     	ldrbne	r3, [r10, #0x12]
  6bdb08: 13833002     	orrne	r3, r3, #2
  6bdb0c: 15ca3012     	strbne	r3, [r10, #0x12]
  6bdb10: e5d73004     	ldrb	r3, [r7, #0x4]
  6bdb14: e3530000     	cmp	r3, #0
  6bdb18: 1a00016c     	bne	0x6be0d0 <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x6d8> @ imm = #0x5b0
  6bdb1c: e5963000     	ldr	r3, [r6]
  6bdb20: e3530000     	cmp	r3, #0
  6bdb24: 0a000018     	beq	0x6bdb8c <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x194> @ imm = #0x60
  6bdb28: e5967008     	ldr	r7, [r6, #0x8]
  6bdb2c: e597a028     	ldr	r10, [r7, #0x28]
  6bdb30: e35a0000     	cmp	r10, #0
  6bdb34: 0a0001b0     	beq	0x6be1fc <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x804> @ imm = #0x6c0
  6bdb38: e59d0068     	ldr	r0, [sp, #0x68]
  6bdb3c: e5da3011     	ldrb	r3, [r10, #0x11]
  6bdb40: e5907000     	ldr	r7, [r0]
  6bdb44: e1570003     	cmp	r7, r3
  6bdb48: 0a00000b     	beq	0x6bdb7c <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x184> @ imm = #0x2c
  6bdb4c: e5da3012     	ldrb	r3, [r10, #0x12]
  6bdb50: e3130008     	tst	r3, #8
  6bdb54: 1a0001f9     	bne	0x6be340 <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x948> @ imm = #0x7e4
  6bdb58: e6ef7077     	uxtb	r7, r7
  6bdb5c: e3570004     	cmp	r7, #4
  6bdb60: e5ca7011     	strb	r7, [r10, #0x11]
  6bdb64: 0a000004     	beq	0x6bdb7c <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x184> @ imm = #0x10
  6bdb68: e59a3008     	ldr	r3, [r10, #0x8]
  6bdb6c: e3530000     	cmp	r3, #0
  6bdb70: 15da3012     	ldrbne	r3, [r10, #0x12]
  6bdb74: 13833002     	orrne	r3, r3, #2
  6bdb78: 15ca3012     	strbne	r3, [r10, #0x12]
  6bdb7c: e59d1068     	ldr	r1, [sp, #0x68]
  6bdb80: e5d13004     	ldrb	r3, [r1, #0x4]
  6bdb84: e3530000     	cmp	r3, #0
  6bdb88: 1a0001d9     	bne	0x6be2f4 <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x8fc> @ imm = #0x764
  6bdb8c: e3a00e1e     	mov	r0, #480
  6bdb90: ebf9da97     	bl	0x5345f4 <_ZN6glitch4core18allocProcessBufferEi> @ imm = #-0x1895a4
  6bdb94: e59d8068     	ldr	r8, [sp, #0x68]
  6bdb98: e1d530dc     	ldrsb	r3, [r5, #12]
  6bdb9c: e1a07000     	mov	r7, r0
  6bdba0: e3a0b000     	mov	r11, #0
  6bdba4: e1a00009     	mov	r0, r9
  6bdba8: e1a01006     	mov	r1, r6
  6bdbac: e1a02005     	mov	r2, r5
  6bdbb0: e58d8008     	str	r8, [sp, #0x8]
  6bdbb4: e88d0880     	stm	sp, {r7, r11}
  6bdbb8: ebfffc4c     	bl	0x6bccf0 <_ZN6glitch5scene12_GLOBAL__N_19addStreamEPNS_5video12IVideoDriverERNS_7collada5SMeshERNS5_11SMeshBufferEcPNS2_17SVertexStreamDataEhRKNS5_13SBufferConfigE> @ imm = #-0xed0
  6bdbbc: e3a0a001     	mov	r10, #1
  6bdbc0: e1a08000     	mov	r8, r0
  6bdbc4: e58d5014     	str	r5, [sp, #0x14]
  6bdbc8: e58d5010     	str	r5, [sp, #0x10]
  6bdbcc: e59d0010     	ldr	r0, [sp, #0x10]
  6bdbd0: e1a01006     	mov	r1, r6
  6bdbd4: e1a02005     	mov	r2, r5
  6bdbd8: e1d0c1d0     	ldrsb	r12, [r0, #16]
  6bdbdc: e1a00009     	mov	r0, r9
  6bdbe0: e35c0000     	cmp	r12, #0
  6bdbe4: e1a0300c     	mov	r3, r12
  6bdbe8: ba00000d     	blt	0x6bdc24 <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x22c> @ imm = #0x34
  6bdbec: e58d8004     	str	r8, [sp, #0x4]
  6bdbf0: e59d8068     	ldr	r8, [sp, #0x68]
  6bdbf4: e3a0c002     	mov	r12, #2
  6bdbf8: e18aab1c     	orr	r10, r10, r12, lsl r11
  6bdbfc: e58d8008     	str	r8, [sp, #0x8]
  6bdc00: e58d7000     	str	r7, [sp]
  6bdc04: ebfffc39     	bl	0x6bccf0 <_ZN6glitch5scene12_GLOBAL__N_19addStreamEPNS_5video12IVideoDriverERNS_7collada5SMeshERNS5_11SMeshBufferEcPNS2_17SVertexStreamDataEhRKNS5_13SBufferConfigE> @ imm = #-0xf1c
  6bdc08: e59dc010     	ldr	r12, [sp, #0x10]
  6bdc0c: e28bb001     	add	r11, r11, #1
  6bdc10: e35b0004     	cmp	r11, #4
  6bdc14: e28cc001     	add	r12, r12, #1
  6bdc18: e1a08000     	mov	r8, r0
  6bdc1c: e58dc010     	str	r12, [sp, #0x10]
  6bdc20: 1affffe9     	bne	0x6bdbcc <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x1d4> @ imm = #-0x5c
  6bdc24: e1d530dd     	ldrsb	r3, [r5, #13]
  6bdc28: e3530000     	cmp	r3, #0
  6bdc2c: ba000009     	blt	0x6bdc58 <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x260> @ imm = #0x24
  6bdc30: e58d8004     	str	r8, [sp, #0x4]
  6bdc34: e59d8068     	ldr	r8, [sp, #0x68]
  6bdc38: e1a00009     	mov	r0, r9
  6bdc3c: e1a01006     	mov	r1, r6
  6bdc40: e1a02005     	mov	r2, r5
  6bdc44: e58d8008     	str	r8, [sp, #0x8]
  6bdc48: e58d7000     	str	r7, [sp]
  6bdc4c: ebfffc27     	bl	0x6bccf0 <_ZN6glitch5scene12_GLOBAL__N_19addStreamEPNS_5video12IVideoDriverERNS_7collada5SMeshERNS5_11SMeshBufferEcPNS2_17SVertexStreamDataEhRKNS5_13SBufferConfigE> @ imm = #-0xf64
  6bdc50: e38aa802     	orr	r10, r10, #131072
  6bdc54: e1a08000     	mov	r8, r0
  6bdc58: e1d530de     	ldrsb	r3, [r5, #14]
  6bdc5c: e3530000     	cmp	r3, #0
  6bdc60: ba000007     	blt	0x6bdc84 <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x28c> @ imm = #0x1c
  6bdc64: e59dc068     	ldr	r12, [sp, #0x68]
  6bdc68: e1a00009     	mov	r0, r9
  6bdc6c: e1a01006     	mov	r1, r6
  6bdc70: e1a02005     	mov	r2, r5
  6bdc74: e88d1180     	stm	sp, {r7, r8, r12}
  6bdc78: ebfffc1c     	bl	0x6bccf0 <_ZN6glitch5scene12_GLOBAL__N_19addStreamEPNS_5video12IVideoDriverERNS_7collada5SMeshERNS5_11SMeshBufferEcPNS2_17SVertexStreamDataEhRKNS5_13SBufferConfigE> @ imm = #-0xf90
  6bdc7c: e38aa701     	orr	r10, r10, #262144
  6bdc80: e1a08000     	mov	r8, r0
  6bdc84: e1d530df     	ldrsb	r3, [r5, #15]
  6bdc88: e3530000     	cmp	r3, #0
  6bdc8c: ba000009     	blt	0x6bdcb8 <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x2c0> @ imm = #0x24
  6bdc90: e58d8004     	str	r8, [sp, #0x4]
  6bdc94: e59d8068     	ldr	r8, [sp, #0x68]
  6bdc98: e1a00009     	mov	r0, r9
  6bdc9c: e1a01006     	mov	r1, r6
  6bdca0: e1a02005     	mov	r2, r5
  6bdca4: e58d8008     	str	r8, [sp, #0x8]
  6bdca8: e58d7000     	str	r7, [sp]
  6bdcac: ebfffc0f     	bl	0x6bccf0 <_ZN6glitch5scene12_GLOBAL__N_19addStreamEPNS_5video12IVideoDriverERNS_7collada5SMeshERNS5_11SMeshBufferEcPNS2_17SVertexStreamDataEhRKNS5_13SBufferConfigE> @ imm = #-0xfc4
  6bdcb0: e38aa702     	orr	r10, r10, #524288
  6bdcb4: e1a08000     	mov	r8, r0
  6bdcb8: e58d5010     	str	r5, [sp, #0x10]
  6bdcbc: e3a0b000     	mov	r11, #0
  6bdcc0: e59d0010     	ldr	r0, [sp, #0x10]
  6bdcc4: e1a01006     	mov	r1, r6
  6bdcc8: e1a02005     	mov	r2, r5
  6bdccc: e1d0c1d8     	ldrsb	r12, [r0, #24]
  6bdcd0: e1a00009     	mov	r0, r9
  6bdcd4: e35c0000     	cmp	r12, #0
  6bdcd8: e1a0300c     	mov	r3, r12
  6bdcdc: ba00000d     	blt	0x6bdd18 <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x320> @ imm = #0x34
  6bdce0: e58d8004     	str	r8, [sp, #0x4]
  6bdce4: e59d8068     	ldr	r8, [sp, #0x68]
  6bdce8: e3a0c601     	mov	r12, #1048576
  6bdcec: e18aab1c     	orr	r10, r10, r12, lsl r11
  6bdcf0: e58d8008     	str	r8, [sp, #0x8]
  6bdcf4: e58d7000     	str	r7, [sp]
  6bdcf8: ebfffbfc     	bl	0x6bccf0 <_ZN6glitch5scene12_GLOBAL__N_19addStreamEPNS_5video12IVideoDriverERNS_7collada5SMeshERNS5_11SMeshBufferEcPNS2_17SVertexStreamDataEhRKNS5_13SBufferConfigE> @ imm = #-0x1010
  6bdcfc: e59dc010     	ldr	r12, [sp, #0x10]
  6bdd00: e28bb001     	add	r11, r11, #1
  6bdd04: e35b0004     	cmp	r11, #4
  6bdd08: e28cc001     	add	r12, r12, #1
  6bdd0c: e1a08000     	mov	r8, r0
  6bdd10: e58dc010     	str	r12, [sp, #0x10]
  6bdd14: 1affffe9     	bne	0x6bdcc0 <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x2c8> @ imm = #-0x5c
  6bdd18: e58d5010     	str	r5, [sp, #0x10]
  6bdd1c: e3a0b000     	mov	r11, #0
  6bdd20: e59d0010     	ldr	r0, [sp, #0x10]
  6bdd24: e1a01006     	mov	r1, r6
  6bdd28: e1a02005     	mov	r2, r5
  6bdd2c: e1d0c1d4     	ldrsb	r12, [r0, #20]
  6bdd30: e1a00009     	mov	r0, r9
  6bdd34: e35c0000     	cmp	r12, #0
  6bdd38: e1a0300c     	mov	r3, r12
  6bdd3c: ba00000d     	blt	0x6bdd78 <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x380> @ imm = #0x34
  6bdd40: e58d8004     	str	r8, [sp, #0x4]
  6bdd44: e59d8068     	ldr	r8, [sp, #0x68]
  6bdd48: e3a0c401     	mov	r12, #16777216
  6bdd4c: e18aab1c     	orr	r10, r10, r12, lsl r11
  6bdd50: e58d8008     	str	r8, [sp, #0x8]
  6bdd54: e58d7000     	str	r7, [sp]
  6bdd58: ebfffbe4     	bl	0x6bccf0 <_ZN6glitch5scene12_GLOBAL__N_19addStreamEPNS_5video12IVideoDriverERNS_7collada5SMeshERNS5_11SMeshBufferEcPNS2_17SVertexStreamDataEhRKNS5_13SBufferConfigE> @ imm = #-0x1070
  6bdd5c: e59dc010     	ldr	r12, [sp, #0x10]
  6bdd60: e28bb001     	add	r11, r11, #1
  6bdd64: e35b0004     	cmp	r11, #4
  6bdd68: e28cc001     	add	r12, r12, #1
  6bdd6c: e1a08000     	mov	r8, r0
  6bdd70: e58dc010     	str	r12, [sp, #0x10]
  6bdd74: 1affffe9     	bne	0x6bdd20 <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x328> @ imm = #-0x5c
  6bdd78: e1d531dc     	ldrsb	r3, [r5, #28]
  6bdd7c: e3530000     	cmp	r3, #0
  6bdd80: ba000009     	blt	0x6bddac <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x3b4> @ imm = #0x24
  6bdd84: e58d8004     	str	r8, [sp, #0x4]
  6bdd88: e59d8068     	ldr	r8, [sp, #0x68]
  6bdd8c: e1a00009     	mov	r0, r9
  6bdd90: e1a01006     	mov	r1, r6
  6bdd94: e1a02005     	mov	r2, r5
  6bdd98: e58d8008     	str	r8, [sp, #0x8]
  6bdd9c: e58d7000     	str	r7, [sp]
  6bdda0: ebfffbd2     	bl	0x6bccf0 <_ZN6glitch5scene12_GLOBAL__N_19addStreamEPNS_5video12IVideoDriverERNS_7collada5SMeshERNS5_11SMeshBufferEcPNS2_17SVertexStreamDataEhRKNS5_13SBufferConfigE> @ imm = #-0x10b8
  6bdda4: e38aa201     	orr	r10, r10, #268435456
  6bdda8: e1a08000     	mov	r8, r0
  6bddac: e1d531dd     	ldrsb	r3, [r5, #29]
  6bddb0: e3530000     	cmp	r3, #0
  6bddb4: ba000007     	blt	0x6bddd8 <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x3e0> @ imm = #0x1c
  6bddb8: e59dc068     	ldr	r12, [sp, #0x68]
  6bddbc: e1a00009     	mov	r0, r9
  6bddc0: e1a01006     	mov	r1, r6
  6bddc4: e1a02005     	mov	r2, r5
  6bddc8: e88d1180     	stm	sp, {r7, r8, r12}
  6bddcc: ebfffbc7     	bl	0x6bccf0 <_ZN6glitch5scene12_GLOBAL__N_19addStreamEPNS_5video12IVideoDriverERNS_7collada5SMeshERNS5_11SMeshBufferEcPNS2_17SVertexStreamDataEhRKNS5_13SBufferConfigE> @ imm = #-0x10e4
  6bddd0: e38aa202     	orr	r10, r10, #536870912
  6bddd4: e1a08000     	mov	r8, r0
  6bddd8: e1a0100a     	mov	r1, r10
  6bdddc: e28d0018     	add	r0, sp, #24
  6bdde0: ebfb8d5d     	bl	0x5a135c <_ZN6glitch5video14CVertexStreams8allocateEj> @ imm = #-0x11ca8c
  6bdde4: e59d3018     	ldr	r3, [sp, #0x18]
  6bdde8: e3530000     	cmp	r3, #0
  6bddec: 15932000     	ldrne	r2, [r3]
  6bddf0: 12822001     	addne	r2, r2, #1
  6bddf4: 15832000     	strne	r2, [r3]
  6bddf8: e594a014     	ldr	r10, [r4, #0x14]
  6bddfc: e5843014     	str	r3, [r4, #0x14]
  6bde00: e35a0000     	cmp	r10, #0
  6bde04: 0a000004     	beq	0x6bde1c <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x424> @ imm = #0x10
  6bde08: e59a3000     	ldr	r3, [r10]
  6bde0c: e2433001     	sub	r3, r3, #1
  6bde10: e3530000     	cmp	r3, #0
  6bde14: e58a3000     	str	r3, [r10]
  6bde18: 0a000079     	beq	0x6be004 <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x60c> @ imm = #0x1e4
  6bde1c: e59da018     	ldr	r10, [sp, #0x18]
  6bde20: e35a0000     	cmp	r10, #0
  6bde24: 0a000004     	beq	0x6bde3c <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x444> @ imm = #0x10
  6bde28: e59a3000     	ldr	r3, [r10]
  6bde2c: e2433001     	sub	r3, r3, #1
  6bde30: e3530000     	cmp	r3, #0
  6bde34: e58a3000     	str	r3, [r10]
  6bde38: 0a00006c     	beq	0x6bdff0 <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x5f8> @ imm = #0x1b0
  6bde3c: e3e02000     	mvn	r2, #0
  6bde40: e3a03000     	mov	r3, #0
  6bde44: e5940014     	ldr	r0, [r4, #0x14]
  6bde48: e1a01007     	mov	r1, r7
  6bde4c: ebfb8e4e     	bl	0x5a178c <_ZN6glitch5video14CVertexStreams12setupStreamsEPKNS0_17SVertexStreamDataEjb> @ imm = #-0x11c6c8
  6bde50: e5963000     	ldr	r3, [r6]
  6bde54: e5942014     	ldr	r2, [r4, #0x14]
  6bde58: e3530000     	cmp	r3, #0
  6bde5c: 15963004     	ldrne	r3, [r6, #0x4]
  6bde60: e5823008     	str	r3, [r2, #0x8]
  6bde64: e5963000     	ldr	r3, [r6]
  6bde68: e5d5200c     	ldrb	r2, [r5, #0xc]
  6bde6c: e3530000     	cmp	r3, #0
  6bde70: 0a00001a     	beq	0x6bdee0 <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x4e8> @ imm = #0x68
  6bde74: e5963008     	ldr	r3, [r6, #0x8]
  6bde78: e6af2072     	sxtb	r2, r2
  6bde7c: e5933020     	ldr	r3, [r3, #0x20]
  6bde80: e7933102     	ldr	r3, [r3, r2, lsl #2]
  6bde84: e3530000     	cmp	r3, #0
  6bde88: 0a000014     	beq	0x6bdee0 <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x4e8> @ imm = #0x50
  6bde8c: e5942014     	ldr	r2, [r4, #0x14]
  6bde90: e5931004     	ldr	r1, [r3, #0x4]
  6bde94: e5930008     	ldr	r0, [r3, #0x8]
  6bde98: e5922010     	ldr	r2, [r2, #0x10]
  6bde9c: e593c000     	ldr	r12, [r3]
  6bdea0: e5820008     	str	r0, [r2, #0x8]
  6bdea4: e582c000     	str	r12, [r2]
  6bdea8: e5821004     	str	r1, [r2, #0x4]
  6bdeac: e5942014     	ldr	r2, [r4, #0x14]
  6bdeb0: e5930014     	ldr	r0, [r3, #0x14]
  6bdeb4: e593c00c     	ldr	r12, [r3, #0xc]
  6bdeb8: e5922010     	ldr	r2, [r2, #0x10]
  6bdebc: e5931010     	ldr	r1, [r3, #0x10]
  6bdec0: e282300c     	add	r3, r2, #12
  6bdec4: e582c00c     	str	r12, [r2, #0xc]
  6bdec8: e5830008     	str	r0, [r3, #0x8]
  6bdecc: e5831004     	str	r1, [r3, #0x4]
  6bded0: e5943014     	ldr	r3, [r4, #0x14]
  6bded4: e1d320be     	ldrh	r2, [r3, #14]
  6bded8: e3822004     	orr	r2, r2, #4
  6bdedc: e1c320be     	strh	r2, [r3, #14]
  6bdee0: e59d0014     	ldr	r0, [sp, #0x14]
  6bdee4: e3a02024     	mov	r2, #36
  6bdee8: e3a01000     	mov	r1, #0
  6bdeec: e3a0b008     	mov	r11, #8
  6bdef0: e58d7010     	str	r7, [sp, #0x10]
  6bdef4: e58d8014     	str	r8, [sp, #0x14]
  6bdef8: e1d031d0     	ldrsb	r3, [r0, #16]
  6bdefc: e3530000     	cmp	r3, #0
  6bdf00: ba000023     	blt	0x6bdf94 <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x59c> @ imm = #0x8c
  6bdf04: e596c000     	ldr	r12, [r6]
  6bdf08: e242700c     	sub	r7, r2, #12
  6bdf0c: e35c0000     	cmp	r12, #0
  6bdf10: 0a00001a     	beq	0x6bdf80 <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x588> @ imm = #0x68
  6bdf14: e596c008     	ldr	r12, [r6, #0x8]
  6bdf18: e59cc020     	ldr	r12, [r12, #0x20]
  6bdf1c: e79c3103     	ldr	r3, [r12, r3, lsl #2]
  6bdf20: e3530000     	cmp	r3, #0
  6bdf24: 0a000015     	beq	0x6bdf80 <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x588> @ imm = #0x54
  6bdf28: e594c014     	ldr	r12, [r4, #0x14]
  6bdf2c: e593a008     	ldr	r10, [r3, #0x8]
  6bdf30: e5939004     	ldr	r9, [r3, #0x4]
  6bdf34: e59c5010     	ldr	r5, [r12, #0x10]
  6bdf38: e5938000     	ldr	r8, [r3]
  6bdf3c: e085c007     	add	r12, r5, r7
  6bdf40: e7858007     	str	r8, [r5, r7]
  6bdf44: e58ca008     	str	r10, [r12, #0x8]
  6bdf48: e58c9004     	str	r9, [r12, #0x4]
  6bdf4c: e594c014     	ldr	r12, [r4, #0x14]
  6bdf50: e5935014     	ldr	r5, [r3, #0x14]
  6bdf54: e593700c     	ldr	r7, [r3, #0xc]
  6bdf58: e59cc010     	ldr	r12, [r12, #0x10]
  6bdf5c: e593a010     	ldr	r10, [r3, #0x10]
  6bdf60: e08c3002     	add	r3, r12, r2
  6bdf64: e78c7002     	str	r7, [r12, r2]
  6bdf68: e5835008     	str	r5, [r3, #0x8]
  6bdf6c: e583a004     	str	r10, [r3, #0x4]
  6bdf70: e5943014     	ldr	r3, [r4, #0x14]
  6bdf74: e1d3c0be     	ldrh	r12, [r3, #14]
  6bdf78: e18cc11b     	orr	r12, r12, r11, lsl r1
  6bdf7c: e1c3c0be     	strh	r12, [r3, #14]
  6bdf80: e2811001     	add	r1, r1, #1
  6bdf84: e3510004     	cmp	r1, #4
  6bdf88: e2800001     	add	r0, r0, #1
  6bdf8c: e2822018     	add	r2, r2, #24
  6bdf90: 1affffd8     	bne	0x6bdef8 <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x500> @ imm = #-0xa0
  6bdf94: e59d7010     	ldr	r7, [sp, #0x10]
  6bdf98: e59d8014     	ldr	r8, [sp, #0x14]
  6bdf9c: e0878208     	add	r8, r7, r8, lsl #4
  6bdfa0: e1570008     	cmp	r7, r8
  6bdfa4: 11a05007     	movne	r5, r7
  6bdfa8: 0a000006     	beq	0x6bdfc8 <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x5d0> @ imm = #0x18
  6bdfac: e5950000     	ldr	r0, [r5]
  6bdfb0: e2855010     	add	r5, r5, #16
  6bdfb4: e3500000     	cmp	r0, #0
  6bdfb8: 0a000000     	beq	0x6bdfc0 <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x5c8> @ imm = #0x0
  6bdfbc: ebf17d70     	bl	0x31d584 <_ZNK6glitch17IReferenceCounted4dropEv> @ imm = #-0x3a0a40
  6bdfc0: e1580005     	cmp	r8, r5
  6bdfc4: 1afffff8     	bne	0x6bdfac <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x5b4> @ imm = #-0x20
  6bdfc8: e3570000     	cmp	r7, #0
  6bdfcc: 0a000001     	beq	0x6bdfd8 <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x5e0> @ imm = #0x4
  6bdfd0: e1a00007     	mov	r0, r7
  6bdfd4: ebf9d9ab     	bl	0x534688 <_ZN6glitch4core20releaseProcessBufferEPv> @ imm = #-0x189954
  6bdfd8: e1a00004     	mov	r0, r4
  6bdfdc: e28dd044     	add	sp, sp, #68
  6bdfe0: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  6bdfe4: e591c028     	ldr	r12, [r1, #0x28]
  6bdfe8: e3a02001     	mov	r2, #1
  6bdfec: eafffea1     	b	0x6bda78 <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x80> @ imm = #-0x57c
  6bdff0: e1a0000a     	mov	r0, r10
  6bdff4: ebfb8a88     	bl	0x5a0a1c <_ZN6glitch5video14CVertexStreamsD1Ev> @ imm = #-0x11d5e0
  6bdff8: e1a0000a     	mov	r0, r10
  6bdffc: ebf140ab     	bl	0x30e2b0 <_ZdlPv@plt>   @ imm = #-0x3afd54
  6be000: eaffff8d     	b	0x6bde3c <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x444> @ imm = #-0x1cc
  6be004: e1a0000a     	mov	r0, r10
  6be008: ebfb8a83     	bl	0x5a0a1c <_ZN6glitch5video14CVertexStreamsD1Ev> @ imm = #-0x11d5f4
  6be00c: e1a0000a     	mov	r0, r10
  6be010: ebf140a6     	bl	0x30e2b0 <_ZdlPv@plt>   @ imm = #-0x3afd68
  6be014: eaffff80     	b	0x6bde1c <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x424> @ imm = #-0x200
  6be018: e3580000     	cmp	r8, #0
  6be01c: 1a000041     	bne	0x6be128 <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x730> @ imm = #0x104
  6be020: e5951024     	ldr	r1, [r5, #0x24]
  6be024: e5992000     	ldr	r2, [r9]
  6be028: e5973000     	ldr	r3, [r7]
  6be02c: e3510801     	cmp	r1, #65536
  6be030: e592c078     	ldr	r12, [r2, #0x78]
  6be034: e5951028     	ldr	r1, [r5, #0x28]
  6be038: e595202c     	ldr	r2, [r5, #0x2c]
  6be03c: e28d0024     	add	r0, sp, #36
  6be040: a1a01101     	lslge	r1, r1, #2
  6be044: b1a01081     	lsllt	r1, r1, #1
  6be048: e58d2004     	str	r2, [sp, #0x4]
  6be04c: e3a02000     	mov	r2, #0
  6be050: e58d1000     	str	r1, [sp]
  6be054: e58d2008     	str	r2, [sp, #0x8]
  6be058: e1a01009     	mov	r1, r9
  6be05c: e3a02001     	mov	r2, #1
  6be060: e12fff3c     	blx	r12
  6be064: e59d3024     	ldr	r3, [sp, #0x24]
  6be068: e3530000     	cmp	r3, #0
  6be06c: 15932004     	ldrne	r2, [r3, #0x4]
  6be070: 12822001     	addne	r2, r2, #1
  6be074: 15832004     	strne	r2, [r3, #0x4]
  6be078: e5950030     	ldr	r0, [r5, #0x30]
  6be07c: e5853030     	str	r3, [r5, #0x30]
  6be080: e3500000     	cmp	r0, #0
  6be084: 0a000000     	beq	0x6be08c <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x694> @ imm = #0x0
  6be088: ebf17d3d     	bl	0x31d584 <_ZNK6glitch17IReferenceCounted4dropEv> @ imm = #-0x3a0b0c
  6be08c: e59d0024     	ldr	r0, [sp, #0x24]
  6be090: e3500000     	cmp	r0, #0
  6be094: 0a000000     	beq	0x6be09c <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x6a4> @ imm = #0x0
  6be098: ebf17d39     	bl	0x31d584 <_ZNK6glitch17IReferenceCounted4dropEv> @ imm = #-0x3a0b1c
  6be09c: e5953030     	ldr	r3, [r5, #0x30]
  6be0a0: e3530000     	cmp	r3, #0
  6be0a4: 15932004     	ldrne	r2, [r3, #0x4]
  6be0a8: 12822001     	addne	r2, r2, #1
  6be0ac: 15832004     	strne	r2, [r3, #0x4]
  6be0b0: e5940018     	ldr	r0, [r4, #0x18]
  6be0b4: e5843018     	str	r3, [r4, #0x18]
  6be0b8: e3500000     	cmp	r0, #0
  6be0bc: 0afffe93     	beq	0x6bdb10 <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x118> @ imm = #-0x5b4
  6be0c0: ebf17d2f     	bl	0x31d584 <_ZNK6glitch17IReferenceCounted4dropEv> @ imm = #-0x3a0b44
  6be0c4: e5d73004     	ldrb	r3, [r7, #0x4]
  6be0c8: e3530000     	cmp	r3, #0
  6be0cc: 0afffe92     	beq	0x6bdb1c <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x124> @ imm = #-0x5b8
  6be0d0: e5953030     	ldr	r3, [r5, #0x30]
  6be0d4: e5d71005     	ldrb	r1, [r7, #0x5]
  6be0d8: e5d32012     	ldrb	r2, [r3, #0x12]
  6be0dc: e3120008     	tst	r2, #8
  6be0e0: 1a000093     	bne	0x6be334 <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x93c> @ imm = #0x24c
  6be0e4: e5d32011     	ldrb	r2, [r3, #0x11]
  6be0e8: e3520004     	cmp	r2, #4
  6be0ec: 0afffe8a     	beq	0x6bdb1c <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x124> @ imm = #-0x5d8
  6be0f0: e1a00003     	mov	r0, r3
  6be0f4: e5933000     	ldr	r3, [r3]
  6be0f8: e1a0e00f     	mov	lr, pc
  6be0fc: e593f00c     	ldr	pc, [r3, #0xc]
  6be100: eafffe85     	b	0x6bdb1c <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x124> @ imm = #-0x5ec
  6be104: e6efb07b     	uxtb	r11, r11
  6be108: e59a3000     	ldr	r3, [r10]
  6be10c: e1a0000a     	mov	r0, r10
  6be110: e1a0e00f     	mov	lr, pc
  6be114: e593f010     	ldr	pc, [r3, #0x10]
  6be118: e35b0004     	cmp	r11, #4
  6be11c: e5cab011     	strb	r11, [r10, #0x11]
  6be120: 1afffe75     	bne	0x6bdafc <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x104> @ imm = #-0x62c
  6be124: eafffe79     	b	0x6bdb10 <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x118> @ imm = #-0x61c
  6be128: e5953024     	ldr	r3, [r5, #0x24]
  6be12c: e3530801     	cmp	r3, #65536
  6be130: e5953028     	ldr	r3, [r5, #0x28]
  6be134: a1a03103     	lslge	r3, r3, #2
  6be138: b1a03083     	lsllt	r3, r3, #1
  6be13c: e3530801     	cmp	r3, #65536
  6be140: ba000083     	blt	0x6be354 <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x95c> @ imm = #0x20c
  6be144: e595302c     	ldr	r3, [r5, #0x2c]
  6be148: e28da03c     	add	r10, sp, #60
  6be14c: e1a0100a     	mov	r1, r10
  6be150: e3530000     	cmp	r3, #0
  6be154: e58d303c     	str	r3, [sp, #0x3c]
  6be158: 15932000     	ldrne	r2, [r3]
  6be15c: e284000c     	add	r0, r4, #12
  6be160: e3a0b000     	mov	r11, #0
  6be164: 12822001     	addne	r2, r2, #1
  6be168: 15832000     	strne	r2, [r3]
  6be16c: ebfff97e     	bl	0x6bc76c <_ZN6glitch3res15onDemandPointerIjEaSERKS2_> @ imm = #-0x1a08
  6be170: e1a0000a     	mov	r0, r10
  6be174: ebfff968     	bl	0x6bc71c <_ZN6glitch3res15onDemandPointerIjED1Ev> @ imm = #-0x1a60
  6be178: e2850030     	add	r0, r5, #48
  6be17c: e5952024     	ldr	r2, [r5, #0x24]
  6be180: e5993000     	ldr	r3, [r9]
  6be184: e58d0010     	str	r0, [sp, #0x10]
  6be188: e5951028     	ldr	r1, [r5, #0x28]
  6be18c: e3520801     	cmp	r2, #65536
  6be190: e594200c     	ldr	r2, [r4, #0xc]
  6be194: a1a01101     	lslge	r1, r1, #2
  6be198: b1a01081     	lsllt	r1, r1, #1
  6be19c: e593c078     	ldr	r12, [r3, #0x78]
  6be1a0: e5973000     	ldr	r3, [r7]
  6be1a4: e58d1000     	str	r1, [sp]
  6be1a8: e592200c     	ldr	r2, [r2, #0xc]
  6be1ac: e28da038     	add	r10, sp, #56
  6be1b0: e1a0000a     	mov	r0, r10
  6be1b4: e58d2004     	str	r2, [sp, #0x4]
  6be1b8: e1a01009     	mov	r1, r9
  6be1bc: e3a02001     	mov	r2, #1
  6be1c0: e58db008     	str	r11, [sp, #0x8]
  6be1c4: e12fff3c     	blx	r12
  6be1c8: e1a0100a     	mov	r1, r10
  6be1cc: e59d0010     	ldr	r0, [sp, #0x10]
  6be1d0: ebfff9ad     	bl	0x6bc88c <_ZN5boost13intrusive_ptrIN6glitch5video7IBufferEEaSERKS4_> @ imm = #-0x194c
  6be1d4: e1a0000a     	mov	r0, r10
  6be1d8: e28da040     	add	r10, sp, #64
  6be1dc: ebfde66a     	bl	0x637b8c <_ZN5boost13intrusive_ptrIN6glitch5video7IBufferEED1Ev> @ imm = #-0x86658
  6be1e0: e52ab00c     	str	r11, [r10, #-0xc]!
  6be1e4: e2840008     	add	r0, r4, #8
  6be1e8: e1a0100a     	mov	r1, r10
  6be1ec: ebfff98c     	bl	0x6bc824 <_ZN6glitch3res15onDemandPointerItEaSERKS2_> @ imm = #-0x19d0
  6be1f0: e1a0000a     	mov	r0, r10
  6be1f4: ebfff976     	bl	0x6bc7d4 <_ZN6glitch3res15onDemandPointerItED1Ev> @ imm = #-0x1a28
  6be1f8: eaffffa7     	b	0x6be09c <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x6a4> @ imm = #-0x164
  6be1fc: e3580000     	cmp	r8, #0
  6be200: 0a000081     	beq	0x6be40c <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0xa14> @ imm = #0x204
  6be204: e5977024     	ldr	r7, [r7, #0x24]
  6be208: e3570000     	cmp	r7, #0
  6be20c: 15973000     	ldrne	r3, [r7]
  6be210: 12833002     	addne	r3, r3, #2
  6be214: 15873000     	strne	r3, [r7]
  6be218: e5948010     	ldr	r8, [r4, #0x10]
  6be21c: e3580000     	cmp	r8, #0
  6be220: 0a00000a     	beq	0x6be250 <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x858> @ imm = #0x28
  6be224: e5983000     	ldr	r3, [r8]
  6be228: e2433001     	sub	r3, r3, #1
  6be22c: e3530000     	cmp	r3, #0
  6be230: e5883000     	str	r3, [r8]
  6be234: 1a000005     	bne	0x6be250 <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x858> @ imm = #0x14
  6be238: e598000c     	ldr	r0, [r8, #0xc]
  6be23c: e3500000     	cmp	r0, #0
  6be240: 0a000000     	beq	0x6be248 <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x850> @ imm = #0x0
  6be244: ebf13f9b     	bl	0x30e0b8 <_ZdaPv@plt>   @ imm = #-0x3b0194
  6be248: e3a03000     	mov	r3, #0
  6be24c: e588300c     	str	r3, [r8, #0xc]
  6be250: e3570000     	cmp	r7, #0
  6be254: e5847010     	str	r7, [r4, #0x10]
  6be258: 0a00000a     	beq	0x6be288 <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x890> @ imm = #0x28
  6be25c: e5973000     	ldr	r3, [r7]
  6be260: e2433001     	sub	r3, r3, #1
  6be264: e3530000     	cmp	r3, #0
  6be268: e5873000     	str	r3, [r7]
  6be26c: 1a000005     	bne	0x6be288 <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x890> @ imm = #0x14
  6be270: e597000c     	ldr	r0, [r7, #0xc]
  6be274: e3500000     	cmp	r0, #0
  6be278: 0a000000     	beq	0x6be280 <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x888> @ imm = #0x0
  6be27c: ebf13f8d     	bl	0x30e0b8 <_ZdaPv@plt>   @ imm = #-0x3b01cc
  6be280: e3a03000     	mov	r3, #0
  6be284: e587300c     	str	r3, [r7, #0xc]
  6be288: e5961000     	ldr	r1, [r6]
  6be28c: e5962008     	ldr	r2, [r6, #0x8]
  6be290: e5993000     	ldr	r3, [r9]
  6be294: e3510000     	cmp	r1, #0
  6be298: 15961004     	ldrne	r1, [r6, #0x4]
  6be29c: e2828028     	add	r8, r2, #40
  6be2a0: 15922000     	ldrne	r2, [r2]
  6be2a4: e59d0068     	ldr	r0, [sp, #0x68]
  6be2a8: e593c078     	ldr	r12, [r3, #0x78]
  6be2ac: 10010291     	mulne	r1, r1, r2
  6be2b0: e5942010     	ldr	r2, [r4, #0x10]
  6be2b4: e5903000     	ldr	r3, [r0]
  6be2b8: e58d1000     	str	r1, [sp]
  6be2bc: e592000c     	ldr	r0, [r2, #0xc]
  6be2c0: e28d7020     	add	r7, sp, #32
  6be2c4: e3a01000     	mov	r1, #0
  6be2c8: e1a02001     	mov	r2, r1
  6be2cc: e98d0003     	stmib	sp, {r0, r1}
  6be2d0: e1a00007     	mov	r0, r7
  6be2d4: e1a01009     	mov	r1, r9
  6be2d8: e12fff3c     	blx	r12
  6be2dc: e1a00008     	mov	r0, r8
  6be2e0: e1a01007     	mov	r1, r7
  6be2e4: ebfff968     	bl	0x6bc88c <_ZN5boost13intrusive_ptrIN6glitch5video7IBufferEEaSERKS4_> @ imm = #-0x1a60
  6be2e8: e1a00007     	mov	r0, r7
  6be2ec: ebfde626     	bl	0x637b8c <_ZN5boost13intrusive_ptrIN6glitch5video7IBufferEED1Ev> @ imm = #-0x86768
  6be2f0: eafffe21     	b	0x6bdb7c <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x184> @ imm = #-0x77c
  6be2f4: e5963008     	ldr	r3, [r6, #0x8]
  6be2f8: e5d11005     	ldrb	r1, [r1, #0x5]
  6be2fc: e5933028     	ldr	r3, [r3, #0x28]
  6be300: e5d32012     	ldrb	r2, [r3, #0x12]
  6be304: e3120008     	tst	r2, #8
  6be308: 0a000001     	beq	0x6be314 <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x91c> @ imm = #0x4
  6be30c: e3120002     	tst	r2, #2
  6be310: 0afffe1d     	beq	0x6bdb8c <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x194> @ imm = #-0x78c
  6be314: e5d32011     	ldrb	r2, [r3, #0x11]
  6be318: e3520004     	cmp	r2, #4
  6be31c: 0afffe1a     	beq	0x6bdb8c <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x194> @ imm = #-0x798
  6be320: e1a00003     	mov	r0, r3
  6be324: e5933000     	ldr	r3, [r3]
  6be328: e1a0e00f     	mov	lr, pc
  6be32c: e593f00c     	ldr	pc, [r3, #0xc]
  6be330: eafffe15     	b	0x6bdb8c <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x194> @ imm = #-0x7ac
  6be334: e3120002     	tst	r2, #2
  6be338: 0afffdf7     	beq	0x6bdb1c <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x124> @ imm = #-0x824
  6be33c: eaffff68     	b	0x6be0e4 <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x6ec> @ imm = #-0x260
  6be340: e59a3000     	ldr	r3, [r10]
  6be344: e1a0000a     	mov	r0, r10
  6be348: e1a0e00f     	mov	lr, pc
  6be34c: e593f010     	ldr	pc, [r3, #0x10]
  6be350: eafffe00     	b	0x6bdb58 <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x160> @ imm = #-0x800
  6be354: e595302c     	ldr	r3, [r5, #0x2c]
  6be358: e28da030     	add	r10, sp, #48
  6be35c: e1a0100a     	mov	r1, r10
  6be360: e3530000     	cmp	r3, #0
  6be364: e58d3030     	str	r3, [sp, #0x30]
  6be368: 15932000     	ldrne	r2, [r3]
  6be36c: e2840008     	add	r0, r4, #8
  6be370: e3a0b000     	mov	r11, #0
  6be374: 12822001     	addne	r2, r2, #1
  6be378: 15832000     	strne	r2, [r3]
  6be37c: ebfff928     	bl	0x6bc824 <_ZN6glitch3res15onDemandPointerItEaSERKS2_> @ imm = #-0x1b60
  6be380: e1a0000a     	mov	r0, r10
  6be384: ebfff912     	bl	0x6bc7d4 <_ZN6glitch3res15onDemandPointerItED1Ev> @ imm = #-0x1bb8
  6be388: e2851030     	add	r1, r5, #48
  6be38c: e5952024     	ldr	r2, [r5, #0x24]
  6be390: e5993000     	ldr	r3, [r9]
  6be394: e58d1010     	str	r1, [sp, #0x10]
  6be398: e5951028     	ldr	r1, [r5, #0x28]
  6be39c: e3520801     	cmp	r2, #65536
  6be3a0: e5942008     	ldr	r2, [r4, #0x8]
  6be3a4: a1a01101     	lslge	r1, r1, #2
  6be3a8: b1a01081     	lsllt	r1, r1, #1
  6be3ac: e593c078     	ldr	r12, [r3, #0x78]
  6be3b0: e5973000     	ldr	r3, [r7]
  6be3b4: e58d1000     	str	r1, [sp]
  6be3b8: e592200c     	ldr	r2, [r2, #0xc]
  6be3bc: e28da02c     	add	r10, sp, #44
  6be3c0: e1a0000a     	mov	r0, r10
  6be3c4: e58d2004     	str	r2, [sp, #0x4]
  6be3c8: e1a01009     	mov	r1, r9
  6be3cc: e3a02001     	mov	r2, #1
  6be3d0: e58db008     	str	r11, [sp, #0x8]
  6be3d4: e12fff3c     	blx	r12
  6be3d8: e1a0100a     	mov	r1, r10
  6be3dc: e59d0010     	ldr	r0, [sp, #0x10]
  6be3e0: ebfff929     	bl	0x6bc88c <_ZN5boost13intrusive_ptrIN6glitch5video7IBufferEEaSERKS4_> @ imm = #-0x1b5c
  6be3e4: e1a0000a     	mov	r0, r10
  6be3e8: e28da040     	add	r10, sp, #64
  6be3ec: ebfde5e6     	bl	0x637b8c <_ZN5boost13intrusive_ptrIN6glitch5video7IBufferEED1Ev> @ imm = #-0x86868
  6be3f0: e52ab018     	str	r11, [r10, #-0x18]!
  6be3f4: e284000c     	add	r0, r4, #12
  6be3f8: e1a0100a     	mov	r1, r10
  6be3fc: ebfff8da     	bl	0x6bc76c <_ZN6glitch3res15onDemandPointerIjEaSERKS2_> @ imm = #-0x1c98
  6be400: e1a0000a     	mov	r0, r10
  6be404: ebfff8c4     	bl	0x6bc71c <_ZN6glitch3res15onDemandPointerIjED1Ev> @ imm = #-0x1cf0
  6be408: eaffff23     	b	0x6be09c <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x6a4> @ imm = #-0x374
  6be40c: e5962004     	ldr	r2, [r6, #0x4]
  6be410: e5971000     	ldr	r1, [r7]
  6be414: e59dc068     	ldr	r12, [sp, #0x68]
  6be418: e28da01c     	add	r10, sp, #28
  6be41c: e0010291     	mul	r1, r1, r2
  6be420: e59c3000     	ldr	r3, [r12]
  6be424: e58d1000     	str	r1, [sp]
  6be428: e5971024     	ldr	r1, [r7, #0x24]
  6be42c: e58d8008     	str	r8, [sp, #0x8]
  6be430: e1a02008     	mov	r2, r8
  6be434: e58d1004     	str	r1, [sp, #0x4]
  6be438: e1a0000a     	mov	r0, r10
  6be43c: e1a01009     	mov	r1, r9
  6be440: e599c000     	ldr	r12, [r9]
  6be444: e1a0e00f     	mov	lr, pc
  6be448: e59cf078     	ldr	pc, [r12, #0x78]
  6be44c: e2870028     	add	r0, r7, #40
  6be450: e1a0100a     	mov	r1, r10
  6be454: ebfff90c     	bl	0x6bc88c <_ZN5boost13intrusive_ptrIN6glitch5video7IBufferEEaSERKS4_> @ imm = #-0x1bd0
  6be458: e1a0000a     	mov	r0, r10
  6be45c: ebfde5ca     	bl	0x637b8c <_ZN5boost13intrusive_ptrIN6glitch5video7IBufferEED1Ev> @ imm = #-0x868d8
  6be460: eafffdc5     	b	0x6bdb7c <_ZN6glitch5scene11CMeshBufferC1EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b+0x184> @ imm = #-0x8ec
  6be464: 8c 70 2d 00  	.word	0x002d708c
  6be468: 54 0c 00 00  	.word	0x00000c54
  6be46c: d8 d8 22 00  	.word	0x0022d8d8

; FUNCTION meshbuffer_add_stream: _ZN6glitch5scene12_GLOBAL__N_19addStreamEPNS_5video12IVideoDriverERNS_7collada5SMeshERNS5_11SMeshBufferEcPNS2_17SVertexStreamDataEhRKNS5_13SBufferConfigE
; demangled: builds stream records and dispatches buffer creation or cached-buffer reuse
; elf_va=0x006bccf0 size=656 file_offset=0x006bccf0 sha256=6b31fed2a17e981495a87a9a3dcb4ab7a65aefcf249972cb1590dedf1dc58034
; decoder: llvm-objdump ARM mode; exact range byte-backed

006bccf0 <_ZN6glitch5scene12_GLOBAL__N_19addStreamEPNS_5video12IVideoDriverERNS_7collada5SMeshERNS5_11SMeshBufferEcPNS2_17SVertexStreamDataEhRKNS5_13SBufferConfigE>:
  6bccf0: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  6bccf4: e1a05001     	mov	r5, r1
  6bccf8: e5911000     	ldr	r1, [r1]
  6bccfc: e59f6274     	ldr	r6, [pc, #0x274]        @ 0x6bcf78 <_ZN6glitch5scene12_GLOBAL__N_19addStreamEPNS_5video12IVideoDriverERNS_7collada5SMeshERNS5_11SMeshBufferEcPNS2_17SVertexStreamDataEhRKNS5_13SBufferConfigE+0x288>
  6bcd00: e24dd024     	sub	sp, sp, #36
  6bcd04: e3510000     	cmp	r1, #0
  6bcd08: e08f6006     	add	r6, pc, r6
  6bcd0c: e1a0c000     	mov	r12, r0
  6bcd10: e1a08003     	mov	r8, r3
  6bcd14: e59da048     	ldr	r10, [sp, #0x48]
  6bcd18: e5dd704c     	ldrb	r7, [sp, #0x4c]
  6bcd1c: 0a000038     	beq	0x6bce04 <_ZN6glitch5scene12_GLOBAL__N_19addStreamEPNS_5video12IVideoDriverERNS_7collada5SMeshERNS5_11SMeshBufferEcPNS2_17SVertexStreamDataEhRKNS5_13SBufferConfigE+0x114> @ imm = #0xe0
  6bcd20: e5952008     	ldr	r2, [r5, #0x8]
  6bcd24: e5924028     	ldr	r4, [r2, #0x28]
  6bcd28: e3540000     	cmp	r4, #0
  6bcd2c: 0a000023     	beq	0x6bcdc0 <_ZN6glitch5scene12_GLOBAL__N_19addStreamEPNS_5video12IVideoDriverERNS_7collada5SMeshERNS5_11SMeshBufferEcPNS2_17SVertexStreamDataEhRKNS5_13SBufferConfigE+0xd0> @ imm = #0x8c
  6bcd30: e5943004     	ldr	r3, [r4, #0x4]
  6bcd34: e2833001     	add	r3, r3, #1
  6bcd38: e5843004     	str	r3, [r4, #0x4]
  6bcd3c: e595c000     	ldr	r12, [r5]
  6bcd40: e35c0000     	cmp	r12, #0
  6bcd44: 1a00001c     	bne	0x6bcdbc <_ZN6glitch5scene12_GLOBAL__N_19addStreamEPNS_5video12IVideoDriverERNS_7collada5SMeshERNS5_11SMeshBufferEcPNS2_17SVertexStreamDataEhRKNS5_13SBufferConfigE+0xcc> @ imm = #0x70
  6bcd48: e3a03014     	mov	r3, #20
  6bcd4c: e5952008     	ldr	r2, [r5, #0x8]
  6bcd50: e0080893     	mul	r8, r3, r8
  6bcd54: e59f1220     	ldr	r1, [pc, #0x220]        @ 0x6bcf7c <_ZN6glitch5scene12_GLOBAL__N_19addStreamEPNS_5video12IVideoDriverERNS_7collada5SMeshERNS5_11SMeshBufferEcPNS2_17SVertexStreamDataEhRKNS5_13SBufferConfigE+0x28c>
  6bcd58: e7923008     	ldr	r3, [r2, r8]
  6bcd5c: e0828008     	add	r8, r2, r8
  6bcd60: e7961001     	ldr	r1, [r6, r1]
  6bcd64: e5982004     	ldr	r2, [r8, #0x4]
  6bcd68: e3540000     	cmp	r4, #0
  6bcd6c: e7d10003     	ldrb	r0, [r1, r3]
  6bcd70: e6ff1072     	uxth	r1, r2
  6bcd74: e78a4207     	str	r4, [r10, r7, lsl #4]
  6bcd78: e0020092     	mul	r2, r2, r0
  6bcd7c: e08aa207     	add	r10, r10, r7, lsl #4
  6bcd80: e6ff2072     	uxth	r2, r2
  6bcd84: 0a000019     	beq	0x6bcdf0 <_ZN6glitch5scene12_GLOBAL__N_19addStreamEPNS_5video12IVideoDriverERNS_7collada5SMeshERNS5_11SMeshBufferEcPNS2_17SVertexStreamDataEhRKNS5_13SBufferConfigE+0x100> @ imm = #0x64
  6bcd88: e594e004     	ldr	lr, [r4, #0x4]
  6bcd8c: e1a00004     	mov	r0, r4
  6bcd90: e28ee001     	add	lr, lr, #1
  6bcd94: e584e004     	str	lr, [r4, #0x4]
  6bcd98: e1ca20be     	strh	r2, [r10, #14]
  6bcd9c: e58ac004     	str	r12, [r10, #0x4]
  6bcda0: e58a3008     	str	r3, [r10, #0x8]
  6bcda4: e1ca10bc     	strh	r1, [r10, #12]
  6bcda8: ebf181f5     	bl	0x31d584 <_ZNK6glitch17IReferenceCounted4dropEv> @ imm = #-0x39f82c
  6bcdac: e2870001     	add	r0, r7, #1
  6bcdb0: e6ef0070     	uxtb	r0, r0
  6bcdb4: e28dd024     	add	sp, sp, #36
  6bcdb8: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  6bcdbc: e5952008     	ldr	r2, [r5, #0x8]
  6bcdc0: e5921018     	ldr	r1, [r2, #0x18]
  6bcdc4: e5920008     	ldr	r0, [r2, #0x8]
  6bcdc8: e5923010     	ldr	r3, [r2, #0x10]
  6bcdcc: e7911108     	ldr	r1, [r1, r8, lsl #2]
  6bcdd0: e3540000     	cmp	r4, #0
  6bcdd4: e790c108     	ldr	r12, [r0, r8, lsl #2]
  6bcdd8: e7933108     	ldr	r3, [r3, r8, lsl #2]
  6bcddc: e1d220b0     	ldrh	r2, [r2]
  6bcde0: e6ff1071     	uxth	r1, r1
  6bcde4: e78a4207     	str	r4, [r10, r7, lsl #4]
  6bcde8: e08aa207     	add	r10, r10, r7, lsl #4
  6bcdec: 1affffe5     	bne	0x6bcd88 <_ZN6glitch5scene12_GLOBAL__N_19addStreamEPNS_5video12IVideoDriverERNS_7collada5SMeshERNS5_11SMeshBufferEcPNS2_17SVertexStreamDataEhRKNS5_13SBufferConfigE+0x98> @ imm = #-0x6c
  6bcdf0: e1ca20be     	strh	r2, [r10, #14]
  6bcdf4: e58ac004     	str	r12, [r10, #0x4]
  6bcdf8: e58a3008     	str	r3, [r10, #0x8]
  6bcdfc: e1ca10bc     	strh	r1, [r10, #12]
  6bce00: eaffffe9     	b	0x6bcdac <_ZN6glitch5scene12_GLOBAL__N_19addStreamEPNS_5video12IVideoDriverERNS_7collada5SMeshERNS5_11SMeshBufferEcPNS2_17SVertexStreamDataEhRKNS5_13SBufferConfigE+0xbc> @ imm = #-0x5c
  6bce04: e3a01014     	mov	r1, #20
  6bce08: e5953008     	ldr	r3, [r5, #0x8]
  6bce0c: e0090891     	mul	r9, r1, r8
  6bce10: e083b009     	add	r11, r3, r9
  6bce14: e59b4010     	ldr	r4, [r11, #0x10]
  6bce18: e3540000     	cmp	r4, #0
  6bce1c: 0a00001a     	beq	0x6bce8c <_ZN6glitch5scene12_GLOBAL__N_19addStreamEPNS_5video12IVideoDriverERNS_7collada5SMeshERNS5_11SMeshBufferEcPNS2_17SVertexStreamDataEhRKNS5_13SBufferConfigE+0x19c> @ imm = #0x68
  6bce20: e59d3050     	ldr	r3, [sp, #0x50]
  6bce24: e593b000     	ldr	r11, [r3]
  6bce28: e5d43011     	ldrb	r3, [r4, #0x11]
  6bce2c: e15b0003     	cmp	r11, r3
  6bce30: 0a00000e     	beq	0x6bce70 <_ZN6glitch5scene12_GLOBAL__N_19addStreamEPNS_5video12IVideoDriverERNS_7collada5SMeshERNS5_11SMeshBufferEcPNS2_17SVertexStreamDataEhRKNS5_13SBufferConfigE+0x180> @ imm = #0x38
  6bce34: e5d43012     	ldrb	r3, [r4, #0x12]
  6bce38: e3130008     	tst	r3, #8
  6bce3c: 1a000048     	bne	0x6bcf64 <_ZN6glitch5scene12_GLOBAL__N_19addStreamEPNS_5video12IVideoDriverERNS_7collada5SMeshERNS5_11SMeshBufferEcPNS2_17SVertexStreamDataEhRKNS5_13SBufferConfigE+0x274> @ imm = #0x120
  6bce40: e6efb07b     	uxtb	r11, r11
  6bce44: e35b0004     	cmp	r11, #4
  6bce48: e5c4b011     	strb	r11, [r4, #0x11]
  6bce4c: 0a000004     	beq	0x6bce64 <_ZN6glitch5scene12_GLOBAL__N_19addStreamEPNS_5video12IVideoDriverERNS_7collada5SMeshERNS5_11SMeshBufferEcPNS2_17SVertexStreamDataEhRKNS5_13SBufferConfigE+0x174> @ imm = #0x10
  6bce50: e5943008     	ldr	r3, [r4, #0x8]
  6bce54: e3530000     	cmp	r3, #0
  6bce58: 15d43012     	ldrbne	r3, [r4, #0x12]
  6bce5c: 13833002     	orrne	r3, r3, #2
  6bce60: 15c43012     	strbne	r3, [r4, #0x12]
  6bce64: e5953008     	ldr	r3, [r5, #0x8]
  6bce68: e0833009     	add	r3, r3, r9
  6bce6c: e5934010     	ldr	r4, [r3, #0x10]
  6bce70: e59d2050     	ldr	r2, [sp, #0x50]
  6bce74: e5d23004     	ldrb	r3, [r2, #0x4]
  6bce78: e3530000     	cmp	r3, #0
  6bce7c: 1a000027     	bne	0x6bcf20 <_ZN6glitch5scene12_GLOBAL__N_19addStreamEPNS_5video12IVideoDriverERNS_7collada5SMeshERNS5_11SMeshBufferEcPNS2_17SVertexStreamDataEhRKNS5_13SBufferConfigE+0x230> @ imm = #0x9c
  6bce80: e3540000     	cmp	r4, #0
  6bce84: 1affffa9     	bne	0x6bcd30 <_ZN6glitch5scene12_GLOBAL__N_19addStreamEPNS_5video12IVideoDriverERNS_7collada5SMeshERNS5_11SMeshBufferEcPNS2_17SVertexStreamDataEhRKNS5_13SBufferConfigE+0x40> @ imm = #-0x15c
  6bce88: eaffffab     	b	0x6bcd3c <_ZN6glitch5scene12_GLOBAL__N_19addStreamEPNS_5video12IVideoDriverERNS_7collada5SMeshERNS5_11SMeshBufferEcPNS2_17SVertexStreamDataEhRKNS5_13SBufferConfigE+0x4c> @ imm = #-0x154
  6bce8c: e1d200dc     	ldrsb	r0, [r2, #12]
  6bce90: e3052556     	movw	r2, #0x5556
  6bce94: e3452555     	movt	r2, #0x5555
  6bce98: e0213091     	mla	r1, r1, r0, r3
  6bce9c: e59f00d8     	ldr	r0, [pc, #0xd8]         @ 0x6bcf7c <_ZN6glitch5scene12_GLOBAL__N_19addStreamEPNS_5video12IVideoDriverERNS_7collada5SMeshERNS5_11SMeshBufferEcPNS2_17SVertexStreamDataEhRKNS5_13SBufferConfigE+0x28c>
  6bcea0: e5911008     	ldr	r1, [r1, #0x8]
  6bcea4: e7933009     	ldr	r3, [r3, r9]
  6bcea8: e7960000     	ldr	r0, [r6, r0]
  6bceac: e0c2e192     	smull	lr, r2, r2, r1
  6bceb0: e59be004     	ldr	lr, [r11, #0x4]
  6bceb4: e0422fc1     	sub	r2, r2, r1, asr #31
  6bceb8: e7d00003     	ldrb	r0, [r0, r3]
  6bcebc: e00e029e     	mul	lr, lr, r2
  6bcec0: e59d2050     	ldr	r2, [sp, #0x50]
  6bcec4: e00e0e90     	mul	lr, r0, lr
  6bcec8: e5923000     	ldr	r3, [r2]
  6bcecc: e28d201c     	add	r2, sp, #28
  6bced0: e58d2014     	str	r2, [sp, #0x14]
  6bced4: e58de000     	str	lr, [sp]
  6bced8: e59b100c     	ldr	r1, [r11, #0xc]
  6bcedc: e58d4008     	str	r4, [sp, #0x8]
  6bcee0: e1a02004     	mov	r2, r4
  6bcee4: e58d1004     	str	r1, [sp, #0x4]
  6bcee8: e59d0014     	ldr	r0, [sp, #0x14]
  6bceec: e1a0100c     	mov	r1, r12
  6bcef0: e59cc000     	ldr	r12, [r12]
  6bcef4: e1a0e00f     	mov	lr, pc
  6bcef8: e59cf078     	ldr	pc, [r12, #0x78]
  6bcefc: e59d1014     	ldr	r1, [sp, #0x14]
  6bcf00: e28b0010     	add	r0, r11, #16
  6bcf04: ebfffe60     	bl	0x6bc88c <_ZN5boost13intrusive_ptrIN6glitch5video7IBufferEEaSERKS4_> @ imm = #-0x680
  6bcf08: e59d0014     	ldr	r0, [sp, #0x14]
  6bcf0c: ebfdeb1e     	bl	0x637b8c <_ZN5boost13intrusive_ptrIN6glitch5video7IBufferEED1Ev> @ imm = #-0x85388
  6bcf10: e5953008     	ldr	r3, [r5, #0x8]
  6bcf14: e0833009     	add	r3, r3, r9
  6bcf18: e5934010     	ldr	r4, [r3, #0x10]
  6bcf1c: eaffffd3     	b	0x6bce70 <_ZN6glitch5scene12_GLOBAL__N_19addStreamEPNS_5video12IVideoDriverERNS_7collada5SMeshERNS5_11SMeshBufferEcPNS2_17SVertexStreamDataEhRKNS5_13SBufferConfigE+0x180> @ imm = #-0xb4
  6bcf20: e5d43012     	ldrb	r3, [r4, #0x12]
  6bcf24: e5d21005     	ldrb	r1, [r2, #0x5]
  6bcf28: e3130008     	tst	r3, #8
  6bcf2c: 0a000001     	beq	0x6bcf38 <_ZN6glitch5scene12_GLOBAL__N_19addStreamEPNS_5video12IVideoDriverERNS_7collada5SMeshERNS5_11SMeshBufferEcPNS2_17SVertexStreamDataEhRKNS5_13SBufferConfigE+0x248> @ imm = #0x4
  6bcf30: e3130002     	tst	r3, #2
  6bcf34: 0affffd1     	beq	0x6bce80 <_ZN6glitch5scene12_GLOBAL__N_19addStreamEPNS_5video12IVideoDriverERNS_7collada5SMeshERNS5_11SMeshBufferEcPNS2_17SVertexStreamDataEhRKNS5_13SBufferConfigE+0x190> @ imm = #-0xbc
  6bcf38: e5d43011     	ldrb	r3, [r4, #0x11]
  6bcf3c: e3530004     	cmp	r3, #4
  6bcf40: 0affffce     	beq	0x6bce80 <_ZN6glitch5scene12_GLOBAL__N_19addStreamEPNS_5video12IVideoDriverERNS_7collada5SMeshERNS5_11SMeshBufferEcPNS2_17SVertexStreamDataEhRKNS5_13SBufferConfigE+0x190> @ imm = #-0xc8
  6bcf44: e5943000     	ldr	r3, [r4]
  6bcf48: e1a00004     	mov	r0, r4
  6bcf4c: e1a0e00f     	mov	lr, pc
  6bcf50: e593f00c     	ldr	pc, [r3, #0xc]
  6bcf54: e5953008     	ldr	r3, [r5, #0x8]
  6bcf58: e0839009     	add	r9, r3, r9
  6bcf5c: e5994010     	ldr	r4, [r9, #0x10]
  6bcf60: eaffffc6     	b	0x6bce80 <_ZN6glitch5scene12_GLOBAL__N_19addStreamEPNS_5video12IVideoDriverERNS_7collada5SMeshERNS5_11SMeshBufferEcPNS2_17SVertexStreamDataEhRKNS5_13SBufferConfigE+0x190> @ imm = #-0xe8
  6bcf64: e5943000     	ldr	r3, [r4]
  6bcf68: e1a00004     	mov	r0, r4
  6bcf6c: e1a0e00f     	mov	lr, pc
  6bcf70: e593f010     	ldr	pc, [r3, #0x10]
  6bcf74: eaffffb1     	b	0x6bce40 <_ZN6glitch5scene12_GLOBAL__N_19addStreamEPNS_5video12IVideoDriverERNS_7collada5SMeshERNS5_11SMeshBufferEcPNS2_17SVertexStreamDataEhRKNS5_13SBufferConfigE+0x150> @ imm = #-0x13c
  6bcf78: 88 7d 2d 00  	.word	0x002d7d88
  6bcf7c: 08 11 00 00  	.word	0x00001108

; FUNCTION scene_mesh_add_buffer: _ZN6glitch5scene5CMesh13addMeshBufferERKN5boost13intrusive_ptrINS0_11CMeshBufferEEERKNS3_INS_5video9CMaterialEEERKNS3_INS8_27CMaterialVertexAttributeMapEEE
; demangled: stores mesh-buffer, material, and attribute-map intrusive references
; elf_va=0x006bc664 size=116 file_offset=0x006bc664 sha256=fcb68af2a1cf66c15681f00f3df53abc8e2bb43b0925a700d9af59016516de3b
; decoder: llvm-objdump ARM mode; exact range byte-backed

006bc664 <_ZN6glitch5scene5CMesh13addMeshBufferERKN5boost13intrusive_ptrINS0_11CMeshBufferEEERKNS3_INS_5video9CMaterialEEERKNS3_INS8_27CMaterialVertexAttributeMapEEE>:
  6bc664: e92d4010     	push	{r4, lr}
  6bc668: e5911000     	ldr	r1, [r1]
  6bc66c: e24dd010     	sub	sp, sp, #16
  6bc670: e3510000     	cmp	r1, #0
  6bc674: 0a000015     	beq	0x6bc6d0 <_ZN6glitch5scene5CMesh13addMeshBufferERKN5boost13intrusive_ptrINS0_11CMeshBufferEEERKNS3_INS_5video9CMaterialEEERKNS3_INS8_27CMaterialVertexAttributeMapEEE+0x6c> @ imm = #0x54
  6bc678: e58d1004     	str	r1, [sp, #0x4]
  6bc67c: e591c004     	ldr	r12, [r1, #0x4]
  6bc680: e28d4004     	add	r4, sp, #4
  6bc684: e2800008     	add	r0, r0, #8
  6bc688: e28cc001     	add	r12, r12, #1
  6bc68c: e581c004     	str	r12, [r1, #0x4]
  6bc690: e5922000     	ldr	r2, [r2]
  6bc694: e3520000     	cmp	r2, #0
  6bc698: e58d2008     	str	r2, [sp, #0x8]
  6bc69c: 15921000     	ldrne	r1, [r2]
  6bc6a0: 12811001     	addne	r1, r1, #1
  6bc6a4: 15821000     	strne	r1, [r2]
  6bc6a8: e5933000     	ldr	r3, [r3]
  6bc6ac: e1a01004     	mov	r1, r4
  6bc6b0: e3530000     	cmp	r3, #0
  6bc6b4: e58d300c     	str	r3, [sp, #0xc]
  6bc6b8: 15932000     	ldrne	r2, [r3]
  6bc6bc: 12822001     	addne	r2, r2, #1
  6bc6c0: 15832000     	strne	r2, [r3]
  6bc6c4: ebffff69     	bl	0x6bc470 <_ZNSt6vectorIN6glitch5scene5CMesh7SBufferENS0_4core10SAllocatorIS3_LNS0_6memory13E_MEMORY_HINTE0EEEE9push_backERKS3_> @ imm = #-0x25c
  6bc6c8: e1a00004     	mov	r0, r4
  6bc6cc: ebfffdd1     	bl	0x6bbe18 <_ZN6glitch5scene5CMesh7SBufferD1Ev> @ imm = #-0x8bc
  6bc6d0: e28dd010     	add	sp, sp, #16
  6bc6d4: e8bd8010     	pop	{r4, pc}

; FUNCTION scene_mesh_sbuffer_destructor: _ZN6glitch5scene5CMesh7SBufferD1Ev
; demangled: releases one scene::CMesh buffer/material/attribute record
; elf_va=0x006bbe18 size=48 file_offset=0x006bbe18 sha256=6720b6c92e718ebdf6548bc62834855e6dcc80737a44d8f1258526c61e31b012
; decoder: llvm-objdump ARM mode; exact range byte-backed

006bbe18 <_ZN6glitch5scene5CMesh7SBufferD1Ev>:
  6bbe18: e92d4010     	push	{r4, lr}
  6bbe1c: e1a04000     	mov	r4, r0
  6bbe20: e2800008     	add	r0, r0, #8
  6bbe24: ebfaf910     	bl	0x57a26c <_ZN5boost13intrusive_ptrIN6glitch5video27CMaterialVertexAttributeMapEED1Ev> @ imm = #-0x141bc0
  6bbe28: e2840004     	add	r0, r4, #4
  6bbe2c: ebf1536d     	bl	0x310be8 <_ZN5boost13intrusive_ptrIN6glitch5video9CMaterialEED1Ev> @ imm = #-0x3ab24c
  6bbe30: e5940000     	ldr	r0, [r4]
  6bbe34: e3500000     	cmp	r0, #0
  6bbe38: 0a000000     	beq	0x6bbe40 <_ZN6glitch5scene5CMesh7SBufferD1Ev+0x28> @ imm = #0x0
  6bbe3c: ebf185d0     	bl	0x31d584 <_ZNK6glitch17IReferenceCounted4dropEv> @ imm = #-0x39e8c0
  6bbe40: e1a00004     	mov	r0, r4
  6bbe44: e8bd8010     	pop	{r4, pc}

; FUNCTION scene_mesh_destructor: _ZN6glitch5scene5CMeshD1Ev
; demangled: destroys the scene mesh and its buffer vector
; elf_va=0x006bc248 size=52 file_offset=0x006bc248 sha256=d8900f426d8dff34aa828fac9e80de676a79c09d6665054775744edef608b495
; decoder: llvm-objdump ARM mode; exact range byte-backed

006bc248 <_ZN6glitch5scene5CMeshD1Ev>:
  6bc248: e59f3024     	ldr	r3, [pc, #0x24]         @ 0x6bc274 <_ZN6glitch5scene5CMeshD1Ev+0x2c>
  6bc24c: e59f2024     	ldr	r2, [pc, #0x24]         @ 0x6bc278 <_ZN6glitch5scene5CMeshD1Ev+0x30>
  6bc250: e92d4010     	push	{r4, lr}
  6bc254: e08f3003     	add	r3, pc, r3
  6bc258: e7932002     	ldr	r2, [r3, r2]
  6bc25c: e1a04000     	mov	r4, r0
  6bc260: e2822008     	add	r2, r2, #8
  6bc264: e4802008     	str	r2, [r0], #8
  6bc268: ebffffe5     	bl	0x6bc204 <_ZNSt6vectorIN6glitch5scene5CMesh7SBufferENS0_4core10SAllocatorIS3_LNS0_6memory13E_MEMORY_HINTE0EEEED1Ev> @ imm = #-0x6c
  6bc26c: e1a00004     	mov	r0, r4
  6bc270: e8bd8010     	pop	{r4, pc}
  6bc274: 3c 88 2d 00  	.word	0x002d883c
  6bc278: 9c 10 00 00  	.word	0x0000109c

; FUNCTION scene_mesh_get_buffer_count: _ZNK6glitch5scene5CMesh18getMeshBufferCountEv
; demangled: scene mesh buffer-count accessor
; elf_va=0x006bbb38 size=40 file_offset=0x006bbb38 sha256=e17dbfb88a307d7813f7e2bc7f1f479f631821e26c0d11e9d940b013996cfcc8
; decoder: llvm-objdump ARM mode; exact range byte-backed

006bbb38 <_ZNK6glitch5scene5CMesh18getMeshBufferCountEv>:
  6bbb38: e5903008     	ldr	r3, [r0, #0x8]
  6bbb3c: e590200c     	ldr	r2, [r0, #0xc]
  6bbb40: e0633002     	rsb	r3, r3, r2
  6bbb44: e1a03143     	asr	r3, r3, #2
  6bbb48: e0830103     	add	r0, r3, r3, lsl #2
  6bbb4c: e0800200     	add	r0, r0, r0, lsl #4
  6bbb50: e0800400     	add	r0, r0, r0, lsl #8
  6bbb54: e0800800     	add	r0, r0, r0, lsl #16
  6bbb58: e0830080     	add	r0, r3, r0, lsl #1
  6bbb5c: e12fff1e     	bx	lr

; FUNCTION scene_mesh_get_buffer: _ZNK6glitch5scene5CMesh13getMeshBufferEj
; demangled: scene mesh-buffer accessor
; elf_va=0x006bbb60 size=88 file_offset=0x006bbb60 sha256=1fc6a695254e1ce5ec878026cdb90695982f65e320c83038d86272ebb7a9a572
; decoder: llvm-objdump ARM mode; exact range byte-backed

006bbb60 <_ZNK6glitch5scene5CMesh13getMeshBufferEj>:
  6bbb60: e5913008     	ldr	r3, [r1, #0x8]
  6bbb64: e591c00c     	ldr	r12, [r1, #0xc]
  6bbb68: e063c00c     	rsb	r12, r3, r12
  6bbb6c: e1a0c14c     	asr	r12, r12, #2
  6bbb70: e08c110c     	add	r1, r12, r12, lsl #2
  6bbb74: e0811201     	add	r1, r1, r1, lsl #4
  6bbb78: e0811401     	add	r1, r1, r1, lsl #8
  6bbb7c: e0811801     	add	r1, r1, r1, lsl #16
  6bbb80: e08cc081     	add	r12, r12, r1, lsl #1
  6bbb84: e152000c     	cmp	r2, r12
  6bbb88: 23a03000     	movhs	r3, #0
  6bbb8c: 25803000     	strhs	r3, [r0]
  6bbb90: 212fff1e     	bxhs	lr
  6bbb94: e3a0100c     	mov	r1, #12
  6bbb98: e0020291     	mul	r2, r1, r2
  6bbb9c: e7933002     	ldr	r3, [r3, r2]
  6bbba0: e3530000     	cmp	r3, #0
  6bbba4: e5803000     	str	r3, [r0]
  6bbba8: 15932004     	ldrne	r2, [r3, #0x4]
  6bbbac: 12822001     	addne	r2, r2, #1
  6bbbb0: 15832004     	strne	r2, [r3, #0x4]
  6bbbb4: e12fff1e     	bx	lr

; FUNCTION scene_mesh_get_material: _ZNK6glitch5scene5CMesh11getMaterialEj
; demangled: scene material accessor
; elf_va=0x006bbbb8 size=88 file_offset=0x006bbbb8 sha256=d8fc507d73d5953a8f9b2d1d3a1698ca550b1fdbe9ba8df9751e7ce3d2a69f93
; decoder: llvm-objdump ARM mode; exact range byte-backed

006bbbb8 <_ZNK6glitch5scene5CMesh11getMaterialEj>:
  6bbbb8: e5913008     	ldr	r3, [r1, #0x8]
  6bbbbc: e591c00c     	ldr	r12, [r1, #0xc]
  6bbbc0: e063c00c     	rsb	r12, r3, r12
  6bbbc4: e1a0c14c     	asr	r12, r12, #2
  6bbbc8: e08c110c     	add	r1, r12, r12, lsl #2
  6bbbcc: e0811201     	add	r1, r1, r1, lsl #4
  6bbbd0: e0811401     	add	r1, r1, r1, lsl #8
  6bbbd4: e0811801     	add	r1, r1, r1, lsl #16
  6bbbd8: e08cc081     	add	r12, r12, r1, lsl #1
  6bbbdc: e152000c     	cmp	r2, r12
  6bbbe0: 23a03000     	movhs	r3, #0
  6bbbe4: 25803000     	strhs	r3, [r0]
  6bbbe8: 212fff1e     	bxhs	lr
  6bbbec: e3a0100c     	mov	r1, #12
  6bbbf0: e0233291     	mla	r3, r1, r2, r3
  6bbbf4: e5933004     	ldr	r3, [r3, #0x4]
  6bbbf8: e3530000     	cmp	r3, #0
  6bbbfc: e5803000     	str	r3, [r0]
  6bbc00: 15932000     	ldrne	r2, [r3]
  6bbc04: 12822001     	addne	r2, r2, #1
  6bbc08: 15832000     	strne	r2, [r3]
  6bbc0c: e12fff1e     	bx	lr

; FUNCTION scene_mesh_get_attribute_map: _ZNK6glitch5scene5CMesh29getMaterialVertexAttributeMapEj
; demangled: scene material attribute-map accessor
; elf_va=0x006bbc10 size=88 file_offset=0x006bbc10 sha256=c9245420c10f17163ab33c35d20dc57d804cd57560d8a2e21cd28d9313cb9481
; decoder: llvm-objdump ARM mode; exact range byte-backed

006bbc10 <_ZNK6glitch5scene5CMesh29getMaterialVertexAttributeMapEj>:
  6bbc10: e5913008     	ldr	r3, [r1, #0x8]
  6bbc14: e591c00c     	ldr	r12, [r1, #0xc]
  6bbc18: e063c00c     	rsb	r12, r3, r12
  6bbc1c: e1a0c14c     	asr	r12, r12, #2
  6bbc20: e08c110c     	add	r1, r12, r12, lsl #2
  6bbc24: e0811201     	add	r1, r1, r1, lsl #4
  6bbc28: e0811401     	add	r1, r1, r1, lsl #8
  6bbc2c: e0811801     	add	r1, r1, r1, lsl #16
  6bbc30: e08cc081     	add	r12, r12, r1, lsl #1
  6bbc34: e152000c     	cmp	r2, r12
  6bbc38: 23a03000     	movhs	r3, #0
  6bbc3c: 25803000     	strhs	r3, [r0]
  6bbc40: 212fff1e     	bxhs	lr
  6bbc44: e3a0100c     	mov	r1, #12
  6bbc48: e0233291     	mla	r3, r1, r2, r3
  6bbc4c: e5933008     	ldr	r3, [r3, #0x8]
  6bbc50: e3530000     	cmp	r3, #0
  6bbc54: e5803000     	str	r3, [r0]
  6bbc58: 15932000     	ldrne	r2, [r3]
  6bbc5c: 12822001     	addne	r2, r2, #1
  6bbc60: 15832000     	strne	r2, [r3]
  6bbc64: e12fff1e     	bx	lr

; FUNCTION scene_mesh_set_material: _ZN6glitch5scene5CMesh11setMaterialEjRKN5boost13intrusive_ptrINS_5video9CMaterialEEERKNS3_INS4_27CMaterialVertexAttributeMapEEE
; demangled: replaces a scene mesh buffer material and attribute map
; elf_va=0x006bbd6c size=172 file_offset=0x006bbd6c sha256=9765f5e6c82e85c653a63b3f2be4c9a1e1d7ca2359ad86a84d368f52a041020d
; decoder: llvm-objdump ARM mode; exact range byte-backed

006bbd6c <_ZN6glitch5scene5CMesh11setMaterialEjRKN5boost13intrusive_ptrINS_5video9CMaterialEEERKNS3_INS4_27CMaterialVertexAttributeMapEEE>:
  6bbd6c: e92d4030     	push	{r4, r5, lr}
  6bbd70: e590c00c     	ldr	r12, [r0, #0xc]
  6bbd74: e5900008     	ldr	r0, [r0, #0x8]
  6bbd78: e1a05003     	mov	r5, r3
  6bbd7c: e24dd00c     	sub	sp, sp, #12
  6bbd80: e060c00c     	rsb	r12, r0, r12
  6bbd84: e1a0c14c     	asr	r12, r12, #2
  6bbd88: e08c310c     	add	r3, r12, r12, lsl #2
  6bbd8c: e0833203     	add	r3, r3, r3, lsl #4
  6bbd90: e0833403     	add	r3, r3, r3, lsl #8
  6bbd94: e0833803     	add	r3, r3, r3, lsl #16
  6bbd98: e08cc083     	add	r12, r12, r3, lsl #1
  6bbd9c: e151000c     	cmp	r1, r12
  6bbda0: 2a00001a     	bhs	0x6bbe10 <_ZN6glitch5scene5CMesh11setMaterialEjRKN5boost13intrusive_ptrINS_5video9CMaterialEEERKNS3_INS4_27CMaterialVertexAttributeMapEEE+0xa4> @ imm = #0x68
  6bbda4: e5923000     	ldr	r3, [r2]
  6bbda8: e3a0400c     	mov	r4, #12
  6bbdac: e0240194     	mla	r4, r4, r1, r0
  6bbdb0: e58d3004     	str	r3, [sp, #0x4]
  6bbdb4: e3530000     	cmp	r3, #0
  6bbdb8: 15932000     	ldrne	r2, [r3]
  6bbdbc: e28d0008     	add	r0, sp, #8
  6bbdc0: 12822001     	addne	r2, r2, #1
  6bbdc4: 15832000     	strne	r2, [r3]
  6bbdc8: e5942004     	ldr	r2, [r4, #0x4]
  6bbdcc: 159d3004     	ldrne	r3, [sp, #0x4]
  6bbdd0: e5202004     	str	r2, [r0, #-0x4]!
  6bbdd4: e5843004     	str	r3, [r4, #0x4]
  6bbdd8: ebf15382     	bl	0x310be8 <_ZN5boost13intrusive_ptrIN6glitch5video9CMaterialEED1Ev> @ imm = #-0x3ab1f8
  6bbddc: e5953000     	ldr	r3, [r5]
  6bbde0: e28d0008     	add	r0, sp, #8
  6bbde4: e58d3000     	str	r3, [sp]
  6bbde8: e3530000     	cmp	r3, #0
  6bbdec: 15932000     	ldrne	r2, [r3]
  6bbdf0: 12822001     	addne	r2, r2, #1
  6bbdf4: 15832000     	strne	r2, [r3]
  6bbdf8: 159d3000     	ldrne	r3, [sp]
  6bbdfc: e5942008     	ldr	r2, [r4, #0x8]
  6bbe00: e5202008     	str	r2, [r0, #-0x8]!
  6bbe04: e5843008     	str	r3, [r4, #0x8]
  6bbe08: e1a0000d     	mov	r0, sp
  6bbe0c: ebfaf916     	bl	0x57a26c <_ZN5boost13intrusive_ptrIN6glitch5video27CMaterialVertexAttributeMapEED1Ev> @ imm = #-0x141ba8
  6bbe10: e28dd00c     	add	sp, sp, #12
  6bbe14: e8bd8030     	pop	{r4, r5, pc}

; FUNCTION collada_mesh_get_buffer_count: _ZNK6glitch7collada5CMesh18getMeshBufferCountEv
; demangled: Collada mesh buffer-count accessor
; elf_va=0x00644a44 size=40 file_offset=0x00644a44 sha256=52a08bc76416efe96ce9d7388fac9d095ee59a9c5e4b6f7a66cc922486332ae9
; decoder: llvm-objdump ARM mode; exact range byte-backed

00644a44 <_ZNK6glitch7collada5CMesh18getMeshBufferCountEv>:
  644a44: e5903018     	ldr	r3, [r0, #0x18]
  644a48: e590201c     	ldr	r2, [r0, #0x1c]
  644a4c: e0633002     	rsb	r3, r3, r2
  644a50: e1a03143     	asr	r3, r3, #2
  644a54: e0830103     	add	r0, r3, r3, lsl #2
  644a58: e0800200     	add	r0, r0, r0, lsl #4
  644a5c: e0800400     	add	r0, r0, r0, lsl #8
  644a60: e0800800     	add	r0, r0, r0, lsl #16
  644a64: e0830080     	add	r0, r3, r0, lsl #1
  644a68: e12fff1e     	bx	lr

; FUNCTION collada_mesh_get_buffer: _ZNK6glitch7collada5CMesh13getMeshBufferEj
; demangled: Collada mesh-buffer accessor
; elf_va=0x00644a6c size=92 file_offset=0x00644a6c sha256=6d3c180342aae9983be5783acad1b45a9b591a7cca38a8e0799f7aaa37cd876b
; decoder: llvm-objdump ARM mode; exact range byte-backed

00644a6c <_ZNK6glitch7collada5CMesh13getMeshBufferEj>:
  644a6c: e591c01c     	ldr	r12, [r1, #0x1c]
  644a70: e5913018     	ldr	r3, [r1, #0x18]
  644a74: e063c00c     	rsb	r12, r3, r12
  644a78: e1a0c14c     	asr	r12, r12, #2
  644a7c: e08c110c     	add	r1, r12, r12, lsl #2
  644a80: e0811201     	add	r1, r1, r1, lsl #4
  644a84: e0811401     	add	r1, r1, r1, lsl #8
  644a88: e0811801     	add	r1, r1, r1, lsl #16
  644a8c: e08cc081     	add	r12, r12, r1, lsl #1
  644a90: e152000c     	cmp	r2, r12
  644a94: 2a000008     	bhs	0x644abc <_ZNK6glitch7collada5CMesh13getMeshBufferEj+0x50> @ imm = #0x20
  644a98: e3a0100c     	mov	r1, #12
  644a9c: e0020291     	mul	r2, r1, r2
  644aa0: e7933002     	ldr	r3, [r3, r2]
  644aa4: e3530000     	cmp	r3, #0
  644aa8: e5803000     	str	r3, [r0]
  644aac: 15932004     	ldrne	r2, [r3, #0x4]
  644ab0: 12822001     	addne	r2, r2, #1
  644ab4: 15832004     	strne	r2, [r3, #0x4]
  644ab8: e12fff1e     	bx	lr
  644abc: e3a03000     	mov	r3, #0
  644ac0: e5803000     	str	r3, [r0]
  644ac4: e12fff1e     	bx	lr

; FUNCTION collada_mesh_get_material: _ZNK6glitch7collada5CMesh11getMaterialEj
; demangled: Collada material accessor
; elf_va=0x00644ac8 size=92 file_offset=0x00644ac8 sha256=96873dc610a0f2dd9700edf88075da4e63dac10c67b8d8913fe1339af46244ac
; decoder: llvm-objdump ARM mode; exact range byte-backed

00644ac8 <_ZNK6glitch7collada5CMesh11getMaterialEj>:
  644ac8: e591c01c     	ldr	r12, [r1, #0x1c]
  644acc: e5913018     	ldr	r3, [r1, #0x18]
  644ad0: e063c00c     	rsb	r12, r3, r12
  644ad4: e1a0c14c     	asr	r12, r12, #2
  644ad8: e08c110c     	add	r1, r12, r12, lsl #2
  644adc: e0811201     	add	r1, r1, r1, lsl #4
  644ae0: e0811401     	add	r1, r1, r1, lsl #8
  644ae4: e0811801     	add	r1, r1, r1, lsl #16
  644ae8: e08cc081     	add	r12, r12, r1, lsl #1
  644aec: e152000c     	cmp	r2, r12
  644af0: 2a000008     	bhs	0x644b18 <_ZNK6glitch7collada5CMesh11getMaterialEj+0x50> @ imm = #0x20
  644af4: e3a0100c     	mov	r1, #12
  644af8: e0233291     	mla	r3, r1, r2, r3
  644afc: e5933004     	ldr	r3, [r3, #0x4]
  644b00: e3530000     	cmp	r3, #0
  644b04: e5803000     	str	r3, [r0]
  644b08: 15932000     	ldrne	r2, [r3]
  644b0c: 12822001     	addne	r2, r2, #1
  644b10: 15832000     	strne	r2, [r3]
  644b14: e12fff1e     	bx	lr
  644b18: e3a03000     	mov	r3, #0
  644b1c: e5803000     	str	r3, [r0]
  644b20: e12fff1e     	bx	lr

; FUNCTION collada_mesh_get_attribute_map: _ZNK6glitch7collada5CMesh29getMaterialVertexAttributeMapEj
; demangled: Collada material attribute-map accessor
; elf_va=0x00644b24 size=92 file_offset=0x00644b24 sha256=b2e6831366ce593ad138509b5f3585398410b953169add830342f16feddf95f3
; decoder: llvm-objdump ARM mode; exact range byte-backed

00644b24 <_ZNK6glitch7collada5CMesh29getMaterialVertexAttributeMapEj>:
  644b24: e591c01c     	ldr	r12, [r1, #0x1c]
  644b28: e5913018     	ldr	r3, [r1, #0x18]
  644b2c: e063c00c     	rsb	r12, r3, r12
  644b30: e1a0c14c     	asr	r12, r12, #2
  644b34: e08c110c     	add	r1, r12, r12, lsl #2
  644b38: e0811201     	add	r1, r1, r1, lsl #4
  644b3c: e0811401     	add	r1, r1, r1, lsl #8
  644b40: e0811801     	add	r1, r1, r1, lsl #16
  644b44: e08cc081     	add	r12, r12, r1, lsl #1
  644b48: e152000c     	cmp	r2, r12
  644b4c: 2a000008     	bhs	0x644b74 <_ZNK6glitch7collada5CMesh29getMaterialVertexAttributeMapEj+0x50> @ imm = #0x20
  644b50: e3a0100c     	mov	r1, #12
  644b54: e0233291     	mla	r3, r1, r2, r3
  644b58: e5933008     	ldr	r3, [r3, #0x8]
  644b5c: e3530000     	cmp	r3, #0
  644b60: e5803000     	str	r3, [r0]
  644b64: 15932000     	ldrne	r2, [r3]
  644b68: 12822001     	addne	r2, r2, #1
  644b6c: 15832000     	strne	r2, [r3]
  644b70: e12fff1e     	bx	lr
  644b74: e3a03000     	mov	r3, #0
  644b78: e5803000     	str	r3, [r0]
  644b7c: e12fff1e     	bx	lr

; FUNCTION collada_mesh_set_material: _ZN6glitch7collada5CMesh11setMaterialEjRKN5boost13intrusive_ptrINS_5video9CMaterialEEERKNS3_INS4_27CMaterialVertexAttributeMapEEE
; demangled: replaces a Collada mesh material and attribute map
; elf_va=0x00644bdc size=172 file_offset=0x00644bdc sha256=76c8842f932be0929cb30a07de16b85f38510e58f584c98cbeeb18342a0e6161
; decoder: llvm-objdump ARM mode; exact range byte-backed

00644bdc <_ZN6glitch7collada5CMesh11setMaterialEjRKN5boost13intrusive_ptrINS_5video9CMaterialEEERKNS3_INS4_27CMaterialVertexAttributeMapEEE>:
  644bdc: e92d4030     	push	{r4, r5, lr}
  644be0: e590c01c     	ldr	r12, [r0, #0x1c]
  644be4: e5900018     	ldr	r0, [r0, #0x18]
  644be8: e1a05003     	mov	r5, r3
  644bec: e24dd00c     	sub	sp, sp, #12
  644bf0: e060c00c     	rsb	r12, r0, r12
  644bf4: e1a0c14c     	asr	r12, r12, #2
  644bf8: e08c310c     	add	r3, r12, r12, lsl #2
  644bfc: e0833203     	add	r3, r3, r3, lsl #4
  644c00: e0833403     	add	r3, r3, r3, lsl #8
  644c04: e0833803     	add	r3, r3, r3, lsl #16
  644c08: e08cc083     	add	r12, r12, r3, lsl #1
  644c0c: e151000c     	cmp	r1, r12
  644c10: 2a00001a     	bhs	0x644c80 <_ZN6glitch7collada5CMesh11setMaterialEjRKN5boost13intrusive_ptrINS_5video9CMaterialEEERKNS3_INS4_27CMaterialVertexAttributeMapEEE+0xa4> @ imm = #0x68
  644c14: e5923000     	ldr	r3, [r2]
  644c18: e3a0400c     	mov	r4, #12
  644c1c: e0240194     	mla	r4, r4, r1, r0
  644c20: e58d3004     	str	r3, [sp, #0x4]
  644c24: e3530000     	cmp	r3, #0
  644c28: 15932000     	ldrne	r2, [r3]
  644c2c: e28d0008     	add	r0, sp, #8
  644c30: 12822001     	addne	r2, r2, #1
  644c34: 15832000     	strne	r2, [r3]
  644c38: e5942004     	ldr	r2, [r4, #0x4]
  644c3c: 159d3004     	ldrne	r3, [sp, #0x4]
  644c40: e5202004     	str	r2, [r0, #-0x4]!
  644c44: e5843004     	str	r3, [r4, #0x4]
  644c48: ebf32fe6     	bl	0x310be8 <_ZN5boost13intrusive_ptrIN6glitch5video9CMaterialEED1Ev> @ imm = #-0x334068
  644c4c: e5953000     	ldr	r3, [r5]
  644c50: e28d0008     	add	r0, sp, #8
  644c54: e58d3000     	str	r3, [sp]
  644c58: e3530000     	cmp	r3, #0
  644c5c: 15932000     	ldrne	r2, [r3]
  644c60: 12822001     	addne	r2, r2, #1
  644c64: 15832000     	strne	r2, [r3]
  644c68: 159d3000     	ldrne	r3, [sp]
  644c6c: e5942008     	ldr	r2, [r4, #0x8]
  644c70: e5202008     	str	r2, [r0, #-0x8]!
  644c74: e5843008     	str	r3, [r4, #0x8]
  644c78: e1a0000d     	mov	r0, sp
  644c7c: ebfcd57a     	bl	0x57a26c <_ZN5boost13intrusive_ptrIN6glitch5video27CMaterialVertexAttributeMapEED1Ev> @ imm = #-0xcaa18
  644c80: e28dd00c     	add	sp, sp, #12
  644c84: e8bd8030     	pop	{r4, r5, pc}

; FUNCTION collada_mesh_sbuffer_destructor: _ZN6glitch7collada5CMesh7SBufferD1Ev
; demangled: releases one Collada mesh-buffer/material/attribute record
; elf_va=0x00644c88 size=48 file_offset=0x00644c88 sha256=a318e6e9a99323630dc26c2f80cb2a3c37965d865dd6eb1b0e24a6ebb7822de3
; decoder: llvm-objdump ARM mode; exact range byte-backed

00644c88 <_ZN6glitch7collada5CMesh7SBufferD1Ev>:
  644c88: e92d4010     	push	{r4, lr}
  644c8c: e1a04000     	mov	r4, r0
  644c90: e2800008     	add	r0, r0, #8
  644c94: ebfcd574     	bl	0x57a26c <_ZN5boost13intrusive_ptrIN6glitch5video27CMaterialVertexAttributeMapEED1Ev> @ imm = #-0xcaa30
  644c98: e2840004     	add	r0, r4, #4
  644c9c: ebf32fd1     	bl	0x310be8 <_ZN5boost13intrusive_ptrIN6glitch5video9CMaterialEED1Ev> @ imm = #-0x3340bc
  644ca0: e5940000     	ldr	r0, [r4]
  644ca4: e3500000     	cmp	r0, #0
  644ca8: 0a000000     	beq	0x644cb0 <_ZN6glitch7collada5CMesh7SBufferD1Ev+0x28> @ imm = #0x0
  644cac: ebf36234     	bl	0x31d584 <_ZNK6glitch17IReferenceCounted4dropEv> @ imm = #-0x327730
  644cb0: e1a00004     	mov	r0, r4
  644cb4: e8bd8010     	pop	{r4, pc}

; FUNCTION collada_mesh_destructor: _ZN6glitch7collada5CMeshD1Ev
; demangled: destroys Collada buffer records and database subobject
; elf_va=0x00644cfc size=80 file_offset=0x00644cfc sha256=52b53beea4cd0c689e3c9f127e824fd59b13c473c57f51ba665d50bc7f7948b9
; decoder: llvm-objdump ARM mode; exact range byte-backed

00644cfc <_ZN6glitch7collada5CMeshD1Ev>:
  644cfc: e92d4070     	push	{r4, r5, r6, lr}
  644d00: e59f4038     	ldr	r4, [pc, #0x38]         @ 0x644d40 <_ZN6glitch7collada5CMeshD1Ev+0x44>
  644d04: e59f3038     	ldr	r3, [pc, #0x38]         @ 0x644d44 <_ZN6glitch7collada5CMeshD1Ev+0x48>
  644d08: e1a05000     	mov	r5, r0
  644d0c: e08f4004     	add	r4, pc, r4
  644d10: e7943003     	ldr	r3, [r4, r3]
  644d14: e2833008     	add	r3, r3, #8
  644d18: e4803018     	str	r3, [r0], #24
  644d1c: ebffffe5     	bl	0x644cb8 <_ZNSt6vectorIN6glitch7collada5CMesh7SBufferENS0_4core10SAllocatorIS3_LNS0_6memory13E_MEMORY_HINTE0EEEED1Ev> @ imm = #-0x6c
  644d20: e59f3020     	ldr	r3, [pc, #0x20]         @ 0x644d48 <_ZN6glitch7collada5CMeshD1Ev+0x4c>
  644d24: e1a00005     	mov	r0, r5
  644d28: e7943003     	ldr	r3, [r4, r3]
  644d2c: e2833008     	add	r3, r3, #8
  644d30: e480300c     	str	r3, [r0], #12
  644d34: ebff51ce     	bl	0x619474 <_ZN6glitch7collada16CColladaDatabaseD1Ev> @ imm = #-0x2b8c8
  644d38: e1a00005     	mov	r0, r5
  644d3c: e8bd8070     	pop	{r4, r5, r6, pc}
  644d40: 84 fd 34 00  	.word	0x0034fd84
  644d44: c0 3d 00 00  	.word	0x00003dc0
  644d48: 04 37 00 00  	.word	0x00003704

; FUNCTION scene_mesh_node_render: _ZN6glitch5scene14CMeshSceneNode6renderEPv
; demangled: scene mesh-node material selection and buffer draw calls
; elf_va=0x00585370 size=612 file_offset=0x00585370 sha256=956cb761a5060b3f9e8991ee531a93bb0592abe2521bd639405e81f3d42327f2
; decoder: llvm-objdump ARM mode; exact range byte-backed

00585370 <_ZN6glitch5scene14CMeshSceneNode6renderEPv>:
  585370: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  585374: e5902130     	ldr	r2, [r0, #0x130]
  585378: e5903110     	ldr	r3, [r0, #0x110]
  58537c: e24dd02c     	sub	sp, sp, #44
  585380: e3520000     	cmp	r2, #0
  585384: e1a05000     	mov	r5, r0
  585388: e5938014     	ldr	r8, [r3, #0x14]
  58538c: 0a000079     	beq	0x585578 <_ZN6glitch5scene14CMeshSceneNode6renderEPv+0x208> @ imm = #0x1e4
  585390: e3580000     	cmp	r8, #0
  585394: 0a000077     	beq	0x585578 <_ZN6glitch5scene14CMeshSceneNode6renderEPv+0x208> @ imm = #0x1dc
  585398: e5902134     	ldr	r2, [r0, #0x134]
  58539c: e5939174     	ldr	r9, [r3, #0x174]
  5853a0: e1a00008     	mov	r0, r8
  5853a4: e2823001     	add	r3, r2, #1
  5853a8: e5853134     	str	r3, [r5, #0x134]
  5853ac: e5983000     	ldr	r3, [r8]
  5853b0: e3a01001     	mov	r1, #1
  5853b4: e2852024     	add	r2, r5, #36
  5853b8: e1a0e00f     	mov	lr, pc
  5853bc: e593f06c     	ldr	pc, [r3, #0x6c]
  5853c0: e28d301c     	add	r3, sp, #28
  5853c4: e58d300c     	str	r3, [sp, #0xc]
  5853c8: e28d3014     	add	r3, sp, #20
  5853cc: e58d3008     	str	r3, [sp, #0x8]
  5853d0: e5953130     	ldr	r3, [r5, #0x130]
  5853d4: e3a04000     	mov	r4, #0
  5853d8: e3590008     	cmp	r9, #8
  5853dc: 13a09000     	movne	r9, #0
  5853e0: 03a09001     	moveq	r9, #1
  5853e4: e1a00003     	mov	r0, r3
  5853e8: e5933000     	ldr	r3, [r3]
  5853ec: e1a0e00f     	mov	lr, pc
  5853f0: e593f010     	ldr	pc, [r3, #0x10]
  5853f4: e1540000     	cmp	r4, r0
  5853f8: e28d7024     	add	r7, sp, #36
  5853fc: e28d6020     	add	r6, sp, #32
  585400: e3a0a00c     	mov	r10, #12
  585404: e28db018     	add	r11, sp, #24
  585408: 2a00005a     	bhs	0x585578 <_ZN6glitch5scene14CMeshSceneNode6renderEPv+0x208> @ imm = #0x168
  58540c: e5953130     	ldr	r3, [r5, #0x130]
  585410: e1a00007     	mov	r0, r7
  585414: e1a02004     	mov	r2, r4
  585418: e1a01003     	mov	r1, r3
  58541c: e5933000     	ldr	r3, [r3]
  585420: e1a0e00f     	mov	lr, pc
  585424: e593f014     	ldr	pc, [r3, #0x14]
  585428: e59d3024     	ldr	r3, [sp, #0x24]
  58542c: e3530000     	cmp	r3, #0
  585430: 0a000048     	beq	0x585558 <_ZN6glitch5scene14CMeshSceneNode6renderEPv+0x1e8> @ imm = #0x120
  585434: e5953130     	ldr	r3, [r5, #0x130]
  585438: e1a02004     	mov	r2, r4
  58543c: e1a00006     	mov	r0, r6
  585440: e1a01003     	mov	r1, r3
  585444: e5933000     	ldr	r3, [r3]
  585448: e1a0e00f     	mov	lr, pc
  58544c: e593f018     	ldr	pc, [r3, #0x18]
  585450: e59d3020     	ldr	r3, [sp, #0x20]
  585454: e1a00003     	mov	r0, r3
  585458: e58d3004     	str	r3, [sp, #0x4]
  58545c: eb010234     	bl	0x5c5d34 <_ZNK6glitch5video9CMaterial12getTechniqueEv> @ imm = #0x408d0
  585460: e59d3004     	ldr	r3, [sp, #0x4]
  585464: e5932004     	ldr	r2, [r3, #0x4]
  585468: e5953110     	ldr	r3, [r5, #0x110]
  58546c: e5922018     	ldr	r2, [r2, #0x18]
  585470: e5d3328a     	ldrb	r3, [r3, #0x28a]
  585474: e022209a     	mla	r2, r10, r0, r2
  585478: e3530000     	cmp	r3, #0
  58547c: e5923008     	ldr	r3, [r2, #0x8]
  585480: e5933004     	ldr	r3, [r3, #0x4]
  585484: 0a00003d     	beq	0x585580 <_ZN6glitch5scene14CMeshSceneNode6renderEPv+0x210> @ imm = #0xf4
  585488: e5953130     	ldr	r3, [r5, #0x130]
  58548c: e1a02004     	mov	r2, r4
  585490: e59d000c     	ldr	r0, [sp, #0xc]
  585494: e1a01003     	mov	r1, r3
  585498: e5933000     	ldr	r3, [r3]
  58549c: e1a0e00f     	mov	lr, pc
  5854a0: e593f01c     	ldr	pc, [r3, #0x1c]
  5854a4: e59d301c     	ldr	r3, [sp, #0x1c]
  5854a8: e1a00008     	mov	r0, r8
  5854ac: e1a01006     	mov	r1, r6
  5854b0: e3530000     	cmp	r3, #0
  5854b4: e58d3018     	str	r3, [sp, #0x18]
  5854b8: 15932000     	ldrne	r2, [r3]
  5854bc: 12822001     	addne	r2, r2, #1
  5854c0: 15832000     	strne	r2, [r3]
  5854c4: e1a0200b     	mov	r2, r11
  5854c8: ebf76590     	bl	0x35eb10 <_ZN6glitch5video12IVideoDriver11setMaterialERKN5boost13intrusive_ptrINS0_9CMaterialEEERKNS3_IKNS0_27CMaterialVertexAttributeMapEEE> @ imm = #-0x2269c0
  5854cc: e59d3018     	ldr	r3, [sp, #0x18]
  5854d0: e3530000     	cmp	r3, #0
  5854d4: 0a000004     	beq	0x5854ec <_ZN6glitch5scene14CMeshSceneNode6renderEPv+0x17c> @ imm = #0x10
  5854d8: e5932000     	ldr	r2, [r3]
  5854dc: e2422001     	sub	r2, r2, #1
  5854e0: e3520000     	cmp	r2, #0
  5854e4: e5832000     	str	r2, [r3]
  5854e8: 0a000028     	beq	0x585590 <_ZN6glitch5scene14CMeshSceneNode6renderEPv+0x220> @ imm = #0xa0
  5854ec: e59d301c     	ldr	r3, [sp, #0x1c]
  5854f0: e3530000     	cmp	r3, #0
  5854f4: 0a000004     	beq	0x58550c <_ZN6glitch5scene14CMeshSceneNode6renderEPv+0x19c> @ imm = #0x10
  5854f8: e5932000     	ldr	r2, [r3]
  5854fc: e2422001     	sub	r2, r2, #1
  585500: e3520000     	cmp	r2, #0
  585504: e5832000     	str	r2, [r3]
  585508: 0a00002a     	beq	0x5855b8 <_ZN6glitch5scene14CMeshSceneNode6renderEPv+0x248> @ imm = #0xa8
  58550c: e59d3024     	ldr	r3, [sp, #0x24]
  585510: e1a00008     	mov	r0, r8
  585514: e3530000     	cmp	r3, #0
  585518: e58d3014     	str	r3, [sp, #0x14]
  58551c: 15932004     	ldrne	r2, [r3, #0x4]
  585520: 12822001     	addne	r2, r2, #1
  585524: 15832004     	strne	r2, [r3, #0x4]
  585528: e59d1008     	ldr	r1, [sp, #0x8]
  58552c: ebf765a7     	bl	0x35ebd0 <_ZN6glitch5video12IVideoDriver14drawMeshBufferERKN5boost13intrusive_ptrIKNS_5scene11CMeshBufferEEE> @ imm = #-0x226964
  585530: e59d0014     	ldr	r0, [sp, #0x14]
  585534: e3500000     	cmp	r0, #0
  585538: 0a000000     	beq	0x585540 <_ZN6glitch5scene14CMeshSceneNode6renderEPv+0x1d0> @ imm = #0x0
  58553c: ebf66010     	bl	0x31d584 <_ZNK6glitch17IReferenceCounted4dropEv> @ imm = #-0x267fc0
  585540: e1a00006     	mov	r0, r6
  585544: ebf62da7     	bl	0x310be8 <_ZN5boost13intrusive_ptrIN6glitch5video9CMaterialEED1Ev> @ imm = #-0x274964
  585548: e59d0024     	ldr	r0, [sp, #0x24]
  58554c: e3500000     	cmp	r0, #0
  585550: 0a000000     	beq	0x585558 <_ZN6glitch5scene14CMeshSceneNode6renderEPv+0x1e8> @ imm = #0x0
  585554: ebf6600a     	bl	0x31d584 <_ZNK6glitch17IReferenceCounted4dropEv> @ imm = #-0x267fd8
  585558: e5953130     	ldr	r3, [r5, #0x130]
  58555c: e2844001     	add	r4, r4, #1
  585560: e1a00003     	mov	r0, r3
  585564: e5933000     	ldr	r3, [r3]
  585568: e1a0e00f     	mov	lr, pc
  58556c: e593f010     	ldr	pc, [r3, #0x10]
  585570: e1540000     	cmp	r4, r0
  585574: 3affffa4     	blo	0x58540c <_ZN6glitch5scene14CMeshSceneNode6renderEPv+0x9c> @ imm = #-0x170
  585578: e28dd02c     	add	sp, sp, #44
  58557c: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  585580: e7e03853     	ubfx	r3, r3, #0x10, #0x1
  585584: e1590003     	cmp	r9, r3
  585588: 1affffec     	bne	0x585540 <_ZN6glitch5scene14CMeshSceneNode6renderEPv+0x1d0> @ imm = #-0x50
  58558c: eaffffbd     	b	0x585488 <_ZN6glitch5scene14CMeshSceneNode6renderEPv+0x118> @ imm = #-0x10c
  585590: e1a00003     	mov	r0, r3
  585594: e58d3004     	str	r3, [sp, #0x4]
  585598: eb01686d     	bl	0x5df754 <_ZN6glitch5video27CMaterialVertexAttributeMapD1Ev> @ imm = #0x5a1b4
  58559c: e59d3004     	ldr	r3, [sp, #0x4]
  5855a0: e1a00003     	mov	r0, r3
  5855a4: ebf62341     	bl	0x30e2b0 <_ZdlPv@plt>   @ imm = #-0x2772fc
  5855a8: e59d301c     	ldr	r3, [sp, #0x1c]
  5855ac: e3530000     	cmp	r3, #0
  5855b0: 1affffd0     	bne	0x5854f8 <_ZN6glitch5scene14CMeshSceneNode6renderEPv+0x188> @ imm = #-0xc0
  5855b4: eaffffd4     	b	0x58550c <_ZN6glitch5scene14CMeshSceneNode6renderEPv+0x19c> @ imm = #-0xb0
  5855b8: e1a00003     	mov	r0, r3
  5855bc: e58d3004     	str	r3, [sp, #0x4]
  5855c0: eb016863     	bl	0x5df754 <_ZN6glitch5video27CMaterialVertexAttributeMapD1Ev> @ imm = #0x5a18c
  5855c4: e59d3004     	ldr	r3, [sp, #0x4]
  5855c8: e1a00003     	mov	r0, r3
  5855cc: ebf62337     	bl	0x30e2b0 <_ZdlPv@plt>   @ imm = #-0x277324
  5855d0: eaffffcd     	b	0x58550c <_ZN6glitch5scene14CMeshSceneNode6renderEPv+0x19c> @ imm = #-0xcc

; FUNCTION collada_mesh_node_render: _ZN6glitch7collada14CMeshSceneNode6renderEPv
; demangled: Collada mesh-node material selection and buffer draw calls
; elf_va=0x00646730 size=504 file_offset=0x00646730 sha256=b10c961f8baaeb25d47c50528b3a9d867df7bf41981555654c31a6c08b11114f
; decoder: llvm-objdump ARM mode; exact range byte-backed

00646730 <_ZN6glitch7collada14CMeshSceneNode6renderEPv>:
  646730: e92d45f0     	push	{r4, r5, r6, r7, r8, r10, lr}
  646734: e5903134     	ldr	r3, [r0, #0x134]
  646738: e5902110     	ldr	r2, [r0, #0x110]
  64673c: e24dd01c     	sub	sp, sp, #28
  646740: e3530000     	cmp	r3, #0
  646744: e1a04000     	mov	r4, r0
  646748: e1a06001     	mov	r6, r1
  64674c: e5925014     	ldr	r5, [r2, #0x14]
  646750: 0a000061     	beq	0x6468dc <_ZN6glitch7collada14CMeshSceneNode6renderEPv+0x1ac> @ imm = #0x184
  646754: e3550000     	cmp	r5, #0
  646758: 0a00005f     	beq	0x6468dc <_ZN6glitch7collada14CMeshSceneNode6renderEPv+0x1ac> @ imm = #0x17c
  64675c: e1a00003     	mov	r0, r3
  646760: e1a01005     	mov	r1, r5
  646764: e5933000     	ldr	r3, [r3]
  646768: e2842024     	add	r2, r4, #36
  64676c: e1a0e00f     	mov	lr, pc
  646770: e593f044     	ldr	pc, [r3, #0x44]
  646774: e3560000     	cmp	r6, #0
  646778: 0a000057     	beq	0x6468dc <_ZN6glitch7collada14CMeshSceneNode6renderEPv+0x1ac> @ imm = #0x15c
  64677c: e5943134     	ldr	r3, [r4, #0x134]
  646780: e2466001     	sub	r6, r6, #1
  646784: e28d0014     	add	r0, sp, #20
  646788: e1a01003     	mov	r1, r3
  64678c: e1a02006     	mov	r2, r6
  646790: e5933000     	ldr	r3, [r3]
  646794: e1a0e00f     	mov	lr, pc
  646798: e593f014     	ldr	pc, [r3, #0x14]
  64679c: e59d3014     	ldr	r3, [sp, #0x14]
  6467a0: e3530000     	cmp	r3, #0
  6467a4: 0a00004c     	beq	0x6468dc <_ZN6glitch7collada14CMeshSceneNode6renderEPv+0x1ac> @ imm = #0x130
  6467a8: e5943134     	ldr	r3, [r4, #0x134]
  6467ac: e206001f     	and	r0, r6, #31
  6467b0: e3a01001     	mov	r1, #1
  6467b4: e5932014     	ldr	r2, [r3, #0x14]
  6467b8: e0122011     	ands	r2, r2, r1, lsl r0
  6467bc: 13a08000     	movne	r8, #0
  6467c0: 0a000047     	beq	0x6468e4 <_ZN6glitch7collada14CMeshSceneNode6renderEPv+0x1b4> @ imm = #0x11c
  6467c4: e28d7010     	add	r7, sp, #16
  6467c8: e1a01003     	mov	r1, r3
  6467cc: e1a00007     	mov	r0, r7
  6467d0: e1a02006     	mov	r2, r6
  6467d4: e5933000     	ldr	r3, [r3]
  6467d8: e1a0e00f     	mov	lr, pc
  6467dc: e593f018     	ldr	pc, [r3, #0x18]
  6467e0: e5943134     	ldr	r3, [r4, #0x134]
  6467e4: e28d0008     	add	r0, sp, #8
  6467e8: e1a02006     	mov	r2, r6
  6467ec: e1a01003     	mov	r1, r3
  6467f0: e5933000     	ldr	r3, [r3]
  6467f4: e1a0e00f     	mov	lr, pc
  6467f8: e593f01c     	ldr	pc, [r3, #0x1c]
  6467fc: e59d3008     	ldr	r3, [sp, #0x8]
  646800: e3530000     	cmp	r3, #0
  646804: e58d300c     	str	r3, [sp, #0xc]
  646808: 0a00000e     	beq	0x646848 <_ZN6glitch7collada14CMeshSceneNode6renderEPv+0x118> @ imm = #0x38
  64680c: e5932000     	ldr	r2, [r3]
  646810: e2822001     	add	r2, r2, #1
  646814: e5832000     	str	r2, [r3]
  646818: e59da008     	ldr	r10, [sp, #0x8]
  64681c: e35a0000     	cmp	r10, #0
  646820: 0a000008     	beq	0x646848 <_ZN6glitch7collada14CMeshSceneNode6renderEPv+0x118> @ imm = #0x20
  646824: e59a3000     	ldr	r3, [r10]
  646828: e2433001     	sub	r3, r3, #1
  64682c: e3530000     	cmp	r3, #0
  646830: e58a3000     	str	r3, [r10]
  646834: 1a000003     	bne	0x646848 <_ZN6glitch7collada14CMeshSceneNode6renderEPv+0x118> @ imm = #0xc
  646838: e1a0000a     	mov	r0, r10
  64683c: ebfe63c4     	bl	0x5df754 <_ZN6glitch5video27CMaterialVertexAttributeMapD1Ev> @ imm = #-0x670f0
  646840: e1a0000a     	mov	r0, r10
  646844: ebf31e99     	bl	0x30e2b0 <_ZdlPv@plt>   @ imm = #-0x33859c
  646848: e28d200c     	add	r2, sp, #12
  64684c: e1a00005     	mov	r0, r5
  646850: e1a01007     	mov	r1, r7
  646854: ebf460ad     	bl	0x35eb10 <_ZN6glitch5video12IVideoDriver11setMaterialERKN5boost13intrusive_ptrINS0_9CMaterialEEERKNS3_IKNS0_27CMaterialVertexAttributeMapEEE> @ imm = #-0x2e7d4c
  646858: e59d3014     	ldr	r3, [sp, #0x14]
  64685c: e1a00005     	mov	r0, r5
  646860: e28d1004     	add	r1, sp, #4
  646864: e3530000     	cmp	r3, #0
  646868: e58d3004     	str	r3, [sp, #0x4]
  64686c: 15932004     	ldrne	r2, [r3, #0x4]
  646870: 12822001     	addne	r2, r2, #1
  646874: 15832004     	strne	r2, [r3, #0x4]
  646878: ebf460d4     	bl	0x35ebd0 <_ZN6glitch5video12IVideoDriver14drawMeshBufferERKN5boost13intrusive_ptrIKNS_5scene11CMeshBufferEEE> @ imm = #-0x2e7cb0
  64687c: e59d0004     	ldr	r0, [sp, #0x4]
  646880: e3500000     	cmp	r0, #0
  646884: 0a000000     	beq	0x64688c <_ZN6glitch7collada14CMeshSceneNode6renderEPv+0x15c> @ imm = #0x0
  646888: ebf35b3d     	bl	0x31d584 <_ZNK6glitch17IReferenceCounted4dropEv> @ imm = #-0x32930c
  64688c: e3580000     	cmp	r8, #0
  646890: 1a00001c     	bne	0x646908 <_ZN6glitch7collada14CMeshSceneNode6renderEPv+0x1d8> @ imm = #0x70
  646894: e59d400c     	ldr	r4, [sp, #0xc]
  646898: e3540000     	cmp	r4, #0
  64689c: 0a000008     	beq	0x6468c4 <_ZN6glitch7collada14CMeshSceneNode6renderEPv+0x194> @ imm = #0x20
  6468a0: e5943000     	ldr	r3, [r4]
  6468a4: e2433001     	sub	r3, r3, #1
  6468a8: e3530000     	cmp	r3, #0
  6468ac: e5843000     	str	r3, [r4]
  6468b0: 1a000003     	bne	0x6468c4 <_ZN6glitch7collada14CMeshSceneNode6renderEPv+0x194> @ imm = #0xc
  6468b4: e1a00004     	mov	r0, r4
  6468b8: ebfe63a5     	bl	0x5df754 <_ZN6glitch5video27CMaterialVertexAttributeMapD1Ev> @ imm = #-0x6716c
  6468bc: e1a00004     	mov	r0, r4
  6468c0: ebf31e7a     	bl	0x30e2b0 <_ZdlPv@plt>   @ imm = #-0x338618
  6468c4: e1a00007     	mov	r0, r7
  6468c8: ebf328c6     	bl	0x310be8 <_ZN5boost13intrusive_ptrIN6glitch5video9CMaterialEED1Ev> @ imm = #-0x335ce8
  6468cc: e59d0014     	ldr	r0, [sp, #0x14]
  6468d0: e3500000     	cmp	r0, #0
  6468d4: 0a000000     	beq	0x6468dc <_ZN6glitch7collada14CMeshSceneNode6renderEPv+0x1ac> @ imm = #0x0
  6468d8: ebf35b29     	bl	0x31d584 <_ZNK6glitch17IReferenceCounted4dropEv> @ imm = #-0x32935c
  6468dc: e28dd01c     	add	sp, sp, #28
  6468e0: e8bd85f0     	pop	{r4, r5, r6, r7, r8, r10, pc}
  6468e4: e1a00003     	mov	r0, r3
  6468e8: e593c000     	ldr	r12, [r3]
  6468ec: e1a02005     	mov	r2, r5
  6468f0: e1a03006     	mov	r3, r6
  6468f4: e1a0e00f     	mov	lr, pc
  6468f8: e59cf038     	ldr	pc, [r12, #0x38]
  6468fc: e5943134     	ldr	r3, [r4, #0x134]
  646900: e2008004     	and	r8, r0, #4
  646904: eaffffae     	b	0x6467c4 <_ZN6glitch7collada14CMeshSceneNode6renderEPv+0x94> @ imm = #-0x148
  646908: e5943134     	ldr	r3, [r4, #0x134]
  64690c: e1a01005     	mov	r1, r5
  646910: e1a02006     	mov	r2, r6
  646914: e1a00003     	mov	r0, r3
  646918: e5933000     	ldr	r3, [r3]
  64691c: e1a0e00f     	mov	lr, pc
  646920: e593f03c     	ldr	pc, [r3, #0x3c]
  646924: eaffffda     	b	0x646894 <_ZN6glitch7collada14CMeshSceneNode6renderEPv+0x164> @ imm = #-0x98

; FUNCTION meshbuffer_destructor: _ZN6glitch5scene11CMeshBufferD1Ev
; demangled: releases CMeshBuffer-owned stream, primitive, and related references
; elf_va=0x006bc8c4 size=344 file_offset=0x006bc8c4 sha256=46db43a9d0dab9e34267ff8310ed3f0ceb5a34a1dd253d84c33c9a669f6b24e5
; decoder: llvm-objdump ARM mode; exact range byte-backed

006bc8c4 <_ZN6glitch5scene11CMeshBufferD1Ev>:
  6bc8c4: e92d4070     	push	{r4, r5, r6, lr}
  6bc8c8: e59f3144     	ldr	r3, [pc, #0x144]        @ 0x6bca14 <_ZN6glitch5scene11CMeshBufferD1Ev+0x150>
  6bc8cc: e59f2144     	ldr	r2, [pc, #0x144]        @ 0x6bca18 <_ZN6glitch5scene11CMeshBufferD1Ev+0x154>
  6bc8d0: e5901030     	ldr	r1, [r0, #0x30]
  6bc8d4: e08f3003     	add	r3, pc, r3
  6bc8d8: e7932002     	ldr	r2, [r3, r2]
  6bc8dc: e3510000     	cmp	r1, #0
  6bc8e0: e1a04000     	mov	r4, r0
  6bc8e4: e2822008     	add	r2, r2, #8
  6bc8e8: e5802000     	str	r2, [r0]
  6bc8ec: 0a000005     	beq	0x6bc908 <_ZN6glitch5scene11CMeshBufferD1Ev+0x44> @ imm = #0x14
  6bc8f0: e5913000     	ldr	r3, [r1]
  6bc8f4: e1a00001     	mov	r0, r1
  6bc8f8: e1a0e00f     	mov	lr, pc
  6bc8fc: e593f004     	ldr	pc, [r3, #0x4]
  6bc900: e3a03000     	mov	r3, #0
  6bc904: e5843030     	str	r3, [r4, #0x30]
  6bc908: e5940018     	ldr	r0, [r4, #0x18]
  6bc90c: e3500000     	cmp	r0, #0
  6bc910: 0a000000     	beq	0x6bc918 <_ZN6glitch5scene11CMeshBufferD1Ev+0x54> @ imm = #0x0
  6bc914: ebf1831a     	bl	0x31d584 <_ZNK6glitch17IReferenceCounted4dropEv> @ imm = #-0x39f398
  6bc918: e5945014     	ldr	r5, [r4, #0x14]
  6bc91c: e3550000     	cmp	r5, #0
  6bc920: 0a000004     	beq	0x6bc938 <_ZN6glitch5scene11CMeshBufferD1Ev+0x74> @ imm = #0x10
  6bc924: e5953000     	ldr	r3, [r5]
  6bc928: e2433001     	sub	r3, r3, #1
  6bc92c: e3530000     	cmp	r3, #0
  6bc930: e5853000     	str	r3, [r5]
  6bc934: 0a000031     	beq	0x6bca00 <_ZN6glitch5scene11CMeshBufferD1Ev+0x13c> @ imm = #0xc4
  6bc938: e5945010     	ldr	r5, [r4, #0x10]
  6bc93c: e3550000     	cmp	r5, #0
  6bc940: 0a00000c     	beq	0x6bc978 <_ZN6glitch5scene11CMeshBufferD1Ev+0xb4> @ imm = #0x30
  6bc944: e5953000     	ldr	r3, [r5]
  6bc948: e2433001     	sub	r3, r3, #1
  6bc94c: e3530000     	cmp	r3, #0
  6bc950: e5853000     	str	r3, [r5]
  6bc954: 1a000005     	bne	0x6bc970 <_ZN6glitch5scene11CMeshBufferD1Ev+0xac> @ imm = #0x14
  6bc958: e595000c     	ldr	r0, [r5, #0xc]
  6bc95c: e3500000     	cmp	r0, #0
  6bc960: 0a000000     	beq	0x6bc968 <_ZN6glitch5scene11CMeshBufferD1Ev+0xa4> @ imm = #0x0
  6bc964: ebf145d3     	bl	0x30e0b8 <_ZdaPv@plt>   @ imm = #-0x3ae8b4
  6bc968: e3a03000     	mov	r3, #0
  6bc96c: e585300c     	str	r3, [r5, #0xc]
  6bc970: e3a03000     	mov	r3, #0
  6bc974: e5843010     	str	r3, [r4, #0x10]
  6bc978: e594500c     	ldr	r5, [r4, #0xc]
  6bc97c: e3550000     	cmp	r5, #0
  6bc980: 0a00000c     	beq	0x6bc9b8 <_ZN6glitch5scene11CMeshBufferD1Ev+0xf4> @ imm = #0x30
  6bc984: e5953000     	ldr	r3, [r5]
  6bc988: e2433001     	sub	r3, r3, #1
  6bc98c: e3530000     	cmp	r3, #0
  6bc990: e5853000     	str	r3, [r5]
  6bc994: 1a000005     	bne	0x6bc9b0 <_ZN6glitch5scene11CMeshBufferD1Ev+0xec> @ imm = #0x14
  6bc998: e595000c     	ldr	r0, [r5, #0xc]
  6bc99c: e3500000     	cmp	r0, #0
  6bc9a0: 0a000000     	beq	0x6bc9a8 <_ZN6glitch5scene11CMeshBufferD1Ev+0xe4> @ imm = #0x0
  6bc9a4: ebf145c3     	bl	0x30e0b8 <_ZdaPv@plt>   @ imm = #-0x3ae8f4
  6bc9a8: e3a03000     	mov	r3, #0
  6bc9ac: e585300c     	str	r3, [r5, #0xc]
  6bc9b0: e3a03000     	mov	r3, #0
  6bc9b4: e584300c     	str	r3, [r4, #0xc]
  6bc9b8: e5945008     	ldr	r5, [r4, #0x8]
  6bc9bc: e3550000     	cmp	r5, #0
  6bc9c0: 0a00000c     	beq	0x6bc9f8 <_ZN6glitch5scene11CMeshBufferD1Ev+0x134> @ imm = #0x30
  6bc9c4: e5953000     	ldr	r3, [r5]
  6bc9c8: e2433001     	sub	r3, r3, #1
  6bc9cc: e3530000     	cmp	r3, #0
  6bc9d0: e5853000     	str	r3, [r5]
  6bc9d4: 1a000005     	bne	0x6bc9f0 <_ZN6glitch5scene11CMeshBufferD1Ev+0x12c> @ imm = #0x14
  6bc9d8: e595000c     	ldr	r0, [r5, #0xc]
  6bc9dc: e3500000     	cmp	r0, #0
  6bc9e0: 0a000000     	beq	0x6bc9e8 <_ZN6glitch5scene11CMeshBufferD1Ev+0x124> @ imm = #0x0
  6bc9e4: ebf145b3     	bl	0x30e0b8 <_ZdaPv@plt>   @ imm = #-0x3ae934
  6bc9e8: e3a03000     	mov	r3, #0
  6bc9ec: e585300c     	str	r3, [r5, #0xc]
  6bc9f0: e3a03000     	mov	r3, #0
  6bc9f4: e5843008     	str	r3, [r4, #0x8]
  6bc9f8: e1a00004     	mov	r0, r4
  6bc9fc: e8bd8070     	pop	{r4, r5, r6, pc}
  6bca00: e1a00005     	mov	r0, r5
  6bca04: ebfb9004     	bl	0x5a0a1c <_ZN6glitch5video14CVertexStreamsD1Ev> @ imm = #-0x11bff0
  6bca08: e1a00005     	mov	r0, r5
  6bca0c: ebf14627     	bl	0x30e2b0 <_ZdlPv@plt>   @ imm = #-0x3ae764
  6bca10: eaffffc8     	b	0x6bc938 <_ZN6glitch5scene11CMeshBufferD1Ev+0x74> @ imm = #-0xe0
  6bca14: bc 81 2d 00  	.word	0x002d81bc
  6bca18: 54 0c 00 00  	.word	0x00000c54

; FUNCTION vertex_streams_destructor: _ZN6glitch5video14CVertexStreamsD1Ev
; demangled: drops each stream-record IBuffer reference
; elf_va=0x005a0a1c size=60 file_offset=0x005a0a1c sha256=4c248aae757b3bf8699f3747e28501a046d0f6fb43b622d7212b2eaf991cfc10
; decoder: llvm-objdump ARM mode; exact range byte-backed

005a0a1c <_ZN6glitch5video14CVertexStreamsD1Ev>:
  5a0a1c: e92d4070     	push	{r4, r5, r6, lr}
  5a0a20: e5905010     	ldr	r5, [r0, #0x10]
  5a0a24: e1a06000     	mov	r6, r0
  5a0a28: e2804014     	add	r4, r0, #20
  5a0a2c: e1540005     	cmp	r4, r5
  5a0a30: 0a000006     	beq	0x5a0a50 <_ZN6glitch5video14CVertexStreamsD1Ev+0x34> @ imm = #0x18
  5a0a34: e5940000     	ldr	r0, [r4]
  5a0a38: e2844010     	add	r4, r4, #16
  5a0a3c: e3500000     	cmp	r0, #0
  5a0a40: 0afffff9     	beq	0x5a0a2c <_ZN6glitch5video14CVertexStreamsD1Ev+0x10> @ imm = #-0x1c
  5a0a44: ebf5f2ce     	bl	0x31d584 <_ZNK6glitch17IReferenceCounted4dropEv> @ imm = #-0x2834c8
  5a0a48: e1540005     	cmp	r4, r5
  5a0a4c: 1afffff8     	bne	0x5a0a34 <_ZN6glitch5video14CVertexStreamsD1Ev+0x18> @ imm = #-0x20
  5a0a50: e1a00006     	mov	r0, r6
  5a0a54: e8bd8070     	pop	{r4, r5, r6, pc}

; DATA scene_mesh_vtable_get_buffer_count: vtable pointer slot -> scene::CMesh::getMeshBufferCount
; elf_va=0x00988070 size=4 file_offset=0x00987070 sha256=0eb2f276e3d13c8d647c39c7ba5e5b54405c3a10187e30af54b7c7e326c0583f
00988070  38 bb 6b 00  .word 0x006bbb38

; DATA scene_mesh_vtable_get_buffer: vtable pointer slot -> scene::CMesh::getMeshBuffer
; elf_va=0x00988074 size=4 file_offset=0x00987074 sha256=e326caffcb71723aa2935134a51e451b4762b55d76db7921fb4731d53d8523e2
00988074  60 bb 6b 00  .word 0x006bbb60

; DATA scene_mesh_vtable_get_material: vtable pointer slot -> scene::CMesh::getMaterial
; elf_va=0x00988078 size=4 file_offset=0x00987078 sha256=9f7b15645b9ce33ec3fa886f254c31c02a87ae853da46922c752efa379439fec
00988078  b8 bb 6b 00  .word 0x006bbbb8

; DATA scene_mesh_vtable_get_attribute_map: vtable pointer slot -> scene::CMesh::getMaterialVertexAttributeMap
; elf_va=0x0098807c size=4 file_offset=0x0098707c sha256=0207f4ea9fcd547e4cb3f366714caf020b3e4e5ec473b9a5026b9e9c991fd4a0
0098807c  10 bc 6b 00  .word 0x006bbc10

; DATA collada_mesh_vtable_get_buffer_count: vtable pointer slot -> collada::CMesh::getMeshBufferCount
; elf_va=0x0097f078 size=4 file_offset=0x0097e078 sha256=6dc4cb0ac6c5c46b612a130c9db744c1e258c6771c375f0a80b0b66466a6c38f
0097f078  44 4a 64 00  .word 0x00644a44

; DATA collada_mesh_vtable_get_buffer: vtable pointer slot -> collada::CMesh::getMeshBuffer
; elf_va=0x0097f07c size=4 file_offset=0x0097e07c sha256=9119da75669b4f3645104d68849d48076687054b4ea74727ae44aeb06da67adf
0097f07c  6c 4a 64 00  .word 0x00644a6c

; DATA collada_mesh_vtable_get_material: vtable pointer slot -> collada::CMesh::getMaterial
; elf_va=0x0097f080 size=4 file_offset=0x0097e080 sha256=55b2384d8055b23251e1d9b4652f45f96c564afa214edb95fb5042c595a73a85
0097f080  c8 4a 64 00  .word 0x00644ac8

; DATA collada_mesh_vtable_get_attribute_map: vtable pointer slot -> collada::CMesh::getMaterialVertexAttributeMap
; elf_va=0x0097f084 size=4 file_offset=0x0097e084 sha256=20bf21ed5db38419ee25eb36aed3163e7ca8c13d61f419d17e561e3f7b53e11e
0097f084  24 4b 64 00  .word 0x00644b24

; DATA collada_mesh_vtable_set_transform: vtable pointer slot -> collada::IMesh::setTransform
; elf_va=0x0097f0ac size=4 file_offset=0x0097e0ac sha256=a62f3345d80044e535f41cf1c6572c33869de5d27388706504a89dcd90fd6a77
0097f0ac  c8 76 66 00  .word 0x006676c8

; DATA opengles2_driver_vtable_create_buffer: vtable pointer slot -> CCommonGLDriver::createBuffer
; elf_va=0x00977b98 size=4 file_offset=0x00976b98 sha256=e661eab6d53d2a6cc64a8a8312f4f8eb67ec01274403199111adb9c72b9c1c99
00977b98  f4 13 5b 00  .word 0x005b13f4
