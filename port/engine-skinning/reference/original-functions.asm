
# _ZN6glitch7collada6detail29CColladaSoftwareSkinTechnique4skinERNS0_11SSkinBufferEPNS_5scene11CMeshBufferE
0066ff48: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0066ff4c: sub      sp, sp, #0xbc
0066ff50: str      r0, [sp, #0x64]
0066ff54: mov      r7, r2
0066ff58: ldr      r3, [r0]
0066ff5c: mov      r5, r1
0066ff60: mov      lr, pc
0066ff64: ldr      pc, [r3, #0x10]
0066ff68: ldr      r0, [r7, #0x14]
0066ff6c: mov      r1, #1
0066ff70: str      r0, [sp, #0x8c]
0066ff74: ldr      r4, [r7, #0x24]
0066ff78: ldr      r7, [r7, #0x28]
0066ff7c: add      r6, r0, #0x14
0066ff80: ldr      r0, [r0, #0x14]
0066ff84: str      r7, [sp, #0x70]
0066ff88: ldrh     r2, [r6, #0xe]
0066ff8c: str      r2, [sp, #0x7c]
0066ff90: bl       #0x5a1adc
0066ff94: ldr      r3, [sp, #0x8c]
0066ff98: ldr      r1, [r6, #4]
0066ff9c: ldr      r2, [r3, #4]
0066ffa0: ldrh     r3, [r6, #0xe]
0066ffa4: add      r1, r0, r1
0066ffa8: tst      r2, #0x20000
0066ffac: mla      r3, r4, r3, r1
0066ffb0: ldrne    ip, [sp, #0x8c]
0066ffb4: str      r3, [sp, #0x58]
0066ffb8: str      r1, [sp, #0xa8]
0066ffbc: ldreq    r0, [sp, #0x8c]
0066ffc0: ldrbne   r3, [ip, #0xc]
0066ffc4: mov      r1, #4
0066ffc8: ldreq    r0, [r0, #0x10]
0066ffcc: addne    r3, r3, #1
0066ffd0: addne    r3, r6, r3, lsl #4
0066ffd4: strne    r3, [sp, #0x90]
0066ffd8: streq    r0, [sp, #0x90]
0066ffdc: ldr      r3, [r5]
0066ffe0: ldr      r3, [r3, #0x14]
0066ffe4: str      r3, [sp, #0x84]
0066ffe8: ldr      r0, [r3, #0x14]
0066ffec: bl       #0x5a19f0
0066fff0: ldr      r1, [sp, #0x64]
0066fff4: ldr      r2, [sp, #0x84]
0066fff8: ldr      ip, [sp, #0x84]
0066fffc: ldr      r3, [r1, #0xc]
00670000: add      r2, r2, #0x14
00670004: str      r2, [sp, #0x88]
00670008: ldr      r1, [r2, #4]
0067000c: ldrb     r2, [r3, #0x98]
00670010: ldr      r3, [ip, #4]
00670014: ldr      ip, [sp, #0x88]
00670018: add      r1, r0, r1
0067001c: add      r2, r2, #1
00670020: ldrh     ip, [ip, #0xe]
00670024: lsl      r2, r2, #2
00670028: ands     r3, r3, #0x20000
0067002c: mla      r0, r4, ip, r1
00670030: str      ip, [sp, #0x74]
00670034: str      r1, [sp, #0xa4]
00670038: str      r2, [sp, #0x78]
0067003c: str      r0, [sp, #0x50]
00670040: bne      #0x670974
00670044: ldr      r1, [sp, #0x84]
00670048: ldrb     r2, [r1, #0xc]
0067004c: str      r3, [sp, #0xac]
00670050: add      r2, r2, #1
00670054: ldr      ip, [sp, #0x84]
00670058: ldr      r0, [sp, #0x88]
0067005c: mov      r1, #0x12
00670060: ldr      r3, [ip, #0x10]
00670064: add      r2, r0, r2, lsl #4
00670068: mov      r0, ip
0067006c: bl       #0x5a0af0
00670070: ldrb     r5, [r5, #0x12]
00670074: ldr      r2, [sp, #0x88]
00670078: mov      r1, #1
0067007c: str      r5, [sp, #0x94]
00670080: ldr      r0, [r2, r5, lsl #4]
00670084: bl       #0x5a1adc
00670088: ldr      ip, [sp, #0x88]
0067008c: ldr      r5, [sp, #0x94]
00670090: add      r3, ip, r5, lsl #4
00670094: ldr      r2, [r3, #4]
00670098: ldr      ip, [sp, #0x8c]
0067009c: ldrh     r3, [r3, #0xe]
006700a0: add      r2, r0, r2
006700a4: ldr      r1, [ip, #0x10]
006700a8: mla      r3, r4, r3, r2
006700ac: ldr      r0, [sp, #0x90]
006700b0: add      r3, r3, #4
006700b4: str      r2, [sp, #0xa0]
006700b8: cmp      r0, r1
006700bc: str      r3, [sp, #0x54]
006700c0: beq      #0x6705b8
006700c4: ldr      r2, [sp, #0x90]
006700c8: ldr      r3, [r2]
006700cc: cmp      r3, #0
006700d0: beq      #0x6705b8
006700d4: ldr      r3, [sp, #0xac]
006700d8: ldr      r0, [r3]
006700dc: cmp      r0, #0
006700e0: beq      #0x6705b8
006700e4: mov      r1, #4
006700e8: bl       #0x5a19f0
006700ec: ldr      ip, [sp, #0xac]
006700f0: mov      r1, #1
006700f4: ldr      r3, [ip, #4]
006700f8: ldrh     r2, [ip, #0xe]
006700fc: add      r3, r0, r3
00670100: str      r3, [sp, #0xb4]
00670104: ldr      r3, [sp, #0x90]
00670108: str      r2, [sp, #0x9c]
0067010c: ldr      r0, [r3]
00670110: bl       #0x5a1adc
00670114: ldr      ip, [sp, #0x90]
00670118: ldr      r1, [sp, #0x70]
0067011c: ldr      r3, [ip, #4]
00670120: ldrh     r2, [ip, #0xe]
00670124: cmp      r4, r1
00670128: add      r3, r0, r3
0067012c: str      r2, [sp, #0x98]
00670130: str      r3, [sp, #0xb0]
00670134: bhs      #0x67082c
00670138: ldr      r3, [sp, #0x9c]
0067013c: ldr      ip, [sp, #0xb4]
00670140: ldr      r0, [sp, #0xb0]
00670144: str      r4, [sp, #0x80]
00670148: mla      r3, r4, r3, ip
0067014c: mla      r4, r4, r2, r0
00670150: str      r3, [sp, #0x68]
00670154: str      r4, [sp, #0x6c]
00670158: ldr      r1, [sp, #0x64]
0067015c: ldr      r2, [sp, #0x54]
00670160: ldr      r3, [r1, #0xc]
00670164: sub      ip, r2, #4
00670168: str      r2, [sp, #0x4c]
0067016c: str      ip, [sp, #0x60]
00670170: ldrb     r3, [r3, #0x98]
00670174: cmp      r3, #0
00670178: str      r3, [sp, #0x5c]
0067017c: beq      #0x6709dc
00670180: ldr      r5, [r2]
00670184: mov      r1, #0
00670188: mov      r0, r5
0067018c: bl       #0x30df8c
00670190: cmp      r0, #0
00670194: bne      #0x6709dc
00670198: ldr      r2, [sp, #0x6c]
0067019c: mov      r0, #0
006701a0: ldr      r1, [sp, #0x64]
006701a4: str      r0, [sp, #0x30]
006701a8: ldr      ip, [r2, #4]
006701ac: ldr      r3, [r1, #0x10]
006701b0: ldr      r7, [r2]
006701b4: str      ip, [sp, #0x18]
006701b8: ldr      r3, [r3, #4]
006701bc: ldr      r1, [sp, #0x58]
006701c0: ldr      ip, [sp, #0x30]
006701c4: str      r3, [sp, #0x48]
006701c8: ldr      r0, [r2, #8]
006701cc: ldr      r2, [sp, #0x58]
006701d0: ldr      r3, [sp, #0x58]
006701d4: str      r0, [sp, #0x14]
006701d8: ldr      r1, [r1]
006701dc: mov      r6, #1
006701e0: str      r1, [sp, #0x10]
006701e4: ldr      r2, [r2, #4]
006701e8: str      r2, [sp, #0xc]
006701ec: ldr      r3, [r3, #8]
006701f0: str      ip, [sp, #0x40]
006701f4: str      ip, [sp, #0x44]
006701f8: str      r3, [sp, #8]
006701fc: str      ip, [sp, #0x34]
00670200: mov      r3, #0
00670204: str      ip, [sp, #0x38]
00670208: str      ip, [sp, #0x3c]
0067020c: b        #0x67023c
00670210: ldr      r1, [sp, #0x4c]
00670214: add      r4, r6, #1
00670218: ldr      r5, [r1, #4]!
0067021c: str      r1, [sp, #0x4c]
00670220: mov      r0, r5
00670224: mov      r1, #0
00670228: bl       #0x30df8c
0067022c: cmp      r0, #0
00670230: bne      #0x670514
00670234: mov      r3, r6
00670238: mov      r6, r4
0067023c: ldr      r2, [sp, #0x60]
00670240: ldr      ip, [sp, #0x48]
00670244: ldr      r1, [sp, #0x10]
00670248: ldrb     r4, [r2, r3]
0067024c: mov      r3, #0x44
00670250: mul      r4, r3, r4
00670254: ldr      fp, [ip, r4]
00670258: add      r4, ip, r4
0067025c: ldr      sb, [r4, #0x10]
00670260: mov      r0, fp
00670264: bl       #0x30ed6c
00670268: ldr      r1, [sp, #0xc]
0067026c: mov      r8, r0
00670270: mov      r0, sb
00670274: bl       #0x30ed6c
00670278: mov      r1, r0
0067027c: mov      r0, r8
00670280: bl       #0x30eba4
00670284: ldr      sl, [r4, #0x20]
00670288: mov      r8, r0
0067028c: ldr      r1, [sp, #8]
00670290: mov      r0, sl
00670294: bl       #0x30ed6c
00670298: mov      r1, r0
0067029c: mov      r0, r8
006702a0: bl       #0x30eba4
006702a4: ldr      r1, [r4, #0x30]
006702a8: bl       #0x30eba4
006702ac: mov      r1, r0
006702b0: mov      r0, r5
006702b4: bl       #0x30ed6c
006702b8: mov      r1, r0
006702bc: ldr      r0, [sp, #0x30]
006702c0: bl       #0x30eba4
006702c4: ldr      r8, [r4, #4]
006702c8: str      r0, [sp, #0x30]
006702cc: ldr      r2, [r4, #0x14]
006702d0: ldr      r1, [sp, #0x10]
006702d4: mov      r0, r8
006702d8: str      r2, [sp, #0x1c]
006702dc: bl       #0x30ed6c
006702e0: ldr      r1, [sp, #0xc]
006702e4: mov      r3, r0
006702e8: ldr      r0, [sp, #0x1c]
006702ec: str      r3, [sp, #4]
006702f0: bl       #0x30ed6c
006702f4: ldr      r3, [sp, #4]
006702f8: mov      r1, r0
006702fc: mov      r0, r3
00670300: ldr      r3, [r4, #0x24]
00670304: str      r3, [sp, #0x20]
00670308: bl       #0x30eba4
0067030c: ldr      r1, [sp, #8]
00670310: mov      r3, r0
00670314: ldr      r0, [sp, #0x20]
00670318: str      r3, [sp, #4]
0067031c: bl       #0x30ed6c
00670320: ldr      r3, [sp, #4]
00670324: mov      r1, r0
00670328: mov      r0, r3
0067032c: bl       #0x30eba4
00670330: ldr      r1, [r4, #0x34]
00670334: bl       #0x30eba4
00670338: mov      r1, r0
0067033c: mov      r0, r5
00670340: bl       #0x30ed6c
00670344: ldr      ip, [r4, #8]
00670348: mov      r1, r0
0067034c: ldr      r0, [sp, #0x40]
00670350: str      ip, [sp, #0x24]
00670354: bl       #0x30eba4
00670358: str      r0, [sp, #0x40]
0067035c: ldr      r2, [r4, #0x18]
00670360: ldr      r1, [sp, #0x10]
00670364: ldr      r0, [sp, #0x24]
00670368: str      r2, [sp, #0x28]
0067036c: bl       #0x30ed6c
00670370: ldr      r1, [sp, #0xc]
00670374: mov      r3, r0
00670378: ldr      r0, [sp, #0x28]
0067037c: str      r3, [sp, #4]
00670380: bl       #0x30ed6c
00670384: ldr      r3, [sp, #4]
00670388: mov      r1, r0
0067038c: mov      r0, r3
00670390: ldr      r3, [r4, #0x28]
00670394: str      r3, [sp, #0x2c]
00670398: bl       #0x30eba4
0067039c: ldr      r1, [sp, #8]
006703a0: mov      r3, r0
006703a4: ldr      r0, [sp, #0x2c]
006703a8: str      r3, [sp, #4]
006703ac: bl       #0x30ed6c
006703b0: ldr      r3, [sp, #4]
006703b4: mov      r1, r0
006703b8: mov      r0, r3
006703bc: bl       #0x30eba4
006703c0: ldr      r1, [r4, #0x38]
006703c4: bl       #0x30eba4
006703c8: mov      r1, r0
006703cc: mov      r0, r5
006703d0: bl       #0x30ed6c
006703d4: mov      r1, r0
006703d8: ldr      r0, [sp, #0x44]
006703dc: bl       #0x30eba4
006703e0: mov      r1, r7
006703e4: str      r0, [sp, #0x44]
006703e8: mov      r0, fp
006703ec: bl       #0x30ed6c
006703f0: ldr      r1, [sp, #0x18]
006703f4: mov      r4, r0
006703f8: mov      r0, sb
006703fc: bl       #0x30ed6c
00670400: mov      r1, r0
00670404: mov      r0, r4
00670408: bl       #0x30eba4
0067040c: ldr      r1, [sp, #0x14]
00670410: mov      r4, r0
00670414: mov      r0, sl
00670418: bl       #0x30ed6c
0067041c: mov      r1, r0
00670420: mov      r0, r4
00670424: bl       #0x30eba4
00670428: mov      r1, r0
0067042c: mov      r0, r5
00670430: bl       #0x30ed6c
00670434: mov      r1, r0
00670438: ldr      r0, [sp, #0x34]
0067043c: bl       #0x30eba4
00670440: mov      r1, r7
00670444: str      r0, [sp, #0x34]
00670448: mov      r0, r8
0067044c: bl       #0x30ed6c
00670450: ldr      r1, [sp, #0x18]
00670454: mov      r4, r0
00670458: ldr      r0, [sp, #0x1c]
0067045c: bl       #0x30ed6c
00670460: mov      r1, r0
00670464: mov      r0, r4
00670468: bl       #0x30eba4
0067046c: ldr      r1, [sp, #0x14]
00670470: mov      r4, r0
00670474: ldr      r0, [sp, #0x20]
00670478: bl       #0x30ed6c
0067047c: mov      r1, r0
00670480: mov      r0, r4
00670484: bl       #0x30eba4
00670488: mov      r1, r0
0067048c: mov      r0, r5
00670490: bl       #0x30ed6c
00670494: mov      r1, r0
00670498: ldr      r0, [sp, #0x38]
0067049c: bl       #0x30eba4
006704a0: mov      r1, r7
006704a4: str      r0, [sp, #0x38]
006704a8: ldr      r0, [sp, #0x24]
006704ac: bl       #0x30ed6c
006704b0: ldr      r1, [sp, #0x18]
006704b4: mov      r4, r0
006704b8: ldr      r0, [sp, #0x28]
006704bc: bl       #0x30ed6c
006704c0: mov      r1, r0
006704c4: mov      r0, r4
006704c8: bl       #0x30eba4
006704cc: ldr      r1, [sp, #0x14]
006704d0: mov      r4, r0
006704d4: ldr      r0, [sp, #0x2c]
006704d8: bl       #0x30ed6c
006704dc: mov      r1, r0
006704e0: mov      r0, r4
006704e4: bl       #0x30eba4
006704e8: mov      r1, r0
006704ec: mov      r0, r5
006704f0: bl       #0x30ed6c
006704f4: mov      r1, r0
006704f8: ldr      r0, [sp, #0x3c]
006704fc: bl       #0x30eba4
00670500: ldr      ip, [sp, #0x5c]
00670504: uxtb     r3, r6
00670508: str      r0, [sp, #0x3c]
0067050c: cmp      ip, r3
00670510: bhi      #0x670210
00670514: ldr      r1, [sp, #0x80]
00670518: ldr      r3, [sp, #0x30]
0067051c: ldr      ip, [sp, #0x50]
00670520: add      r1, r1, #1
00670524: ldr      r2, [sp, #0x70]
00670528: str      r1, [sp, #0x80]
0067052c: str      r3, [ip]
00670530: ldr      r0, [sp, #0x40]
00670534: cmp      r1, r2
00670538: str      r0, [ip, #4]
0067053c: ldr      r1, [sp, #0x44]
00670540: str      r1, [ip, #8]
00670544: ldr      r2, [sp, #0x34]
00670548: ldr      r3, [sp, #0x68]
0067054c: str      r2, [r3]
00670550: ldr      ip, [sp, #0x38]
00670554: str      ip, [r3, #4]
00670558: ldr      r0, [sp, #0x3c]
0067055c: str      r0, [r3, #8]
00670560: bhs      #0x67082c
00670564: ldr      r1, [sp, #0x54]
00670568: ldr      r2, [sp, #0x78]
0067056c: ldr      r3, [sp, #0x50]
00670570: ldr      ip, [sp, #0x74]
00670574: add      r1, r1, r2
00670578: ldr      r0, [sp, #0x68]
0067057c: str      r1, [sp, #0x54]
00670580: ldr      r1, [sp, #0x9c]
00670584: add      r3, r3, ip
00670588: ldr      r2, [sp, #0x58]
0067058c: add      r0, r0, r1
00670590: ldr      ip, [sp, #0x6c]
00670594: str      r3, [sp, #0x50]
00670598: str      r0, [sp, #0x68]
0067059c: ldr      r3, [sp, #0x7c]
006705a0: ldr      r0, [sp, #0x98]
006705a4: add      r2, r2, r3
006705a8: add      ip, ip, r0
006705ac: str      r2, [sp, #0x58]
006705b0: str      ip, [sp, #0x6c]
006705b4: b        #0x670158
006705b8: ldr      r1, [sp, #0x70]
006705bc: cmp      r4, r1
006705c0: strlo    r4, [sp, #0x20]
006705c4: bhs      #0x6708bc
006705c8: ldr      r0, [sp, #0x64]
006705cc: ldr      r1, [sp, #0x54]
006705d0: ldr      r3, [r0, #0xc]
006705d4: sub      r2, r1, #4
006705d8: str      r1, [sp, #0x14]
006705dc: str      r2, [sp, #0x1c]
006705e0: ldrb     r3, [r3, #0x98]
006705e4: cmp      r3, #0
006705e8: str      r3, [sp, #0x18]
006705ec: beq      #0x670964
006705f0: ldr      r5, [r1]
006705f4: mov      r1, #0
006705f8: mov      r0, r5
006705fc: bl       #0x30df8c
00670600: cmp      r0, #0
00670604: bne      #0x670964
00670608: ldr      ip, [sp, #0x64]
0067060c: ldr      r0, [sp, #0x58]
00670610: mov      sb, #0
00670614: ldr      r3, [ip, #0x10]
00670618: ldr      sl, [r0]
0067061c: ldr      r8, [r0, #4]
00670620: ldr      r3, [r3, #4]
00670624: mov      r6, #1
00670628: str      r3, [sp, #0x10]
0067062c: ldr      r7, [r0, #8]
00670630: mov      r3, #0
00670634: str      sb, [sp, #8]
00670638: str      sb, [sp, #0xc]
0067063c: b        #0x67066c
00670640: ldr      ip, [sp, #0x14]
00670644: mov      r1, #0
00670648: add      r4, r6, #1
0067064c: ldr      r5, [ip, #4]!
00670650: mov      r0, r5
00670654: str      ip, [sp, #0x14]
00670658: bl       #0x30df8c
0067065c: cmp      r0, #0
00670660: bne      #0x6707cc
00670664: mov      r3, r6
00670668: mov      r6, r4
0067066c: ldr      r0, [sp, #0x1c]
00670670: mov      r1, #0x44
00670674: ldr      r2, [sp, #0x10]
00670678: ldrb     r4, [r0, r3]
0067067c: mov      r0, sl
00670680: mul      r4, r1, r4
00670684: ldr      r1, [r2, r4]
00670688: add      r4, r2, r4
0067068c: bl       #0x30ed6c
00670690: ldr      r1, [r4, #0x10]
00670694: mov      fp, r0
00670698: mov      r0, r8
0067069c: bl       #0x30ed6c
006706a0: mov      r1, r0
006706a4: mov      r0, fp
006706a8: bl       #0x30eba4
006706ac: ldr      r1, [r4, #0x20]
006706b0: mov      fp, r0
006706b4: mov      r0, r7
006706b8: bl       #0x30ed6c
006706bc: mov      r1, r0
006706c0: mov      r0, fp
006706c4: bl       #0x30eba4
006706c8: ldr      r1, [r4, #0x30]
006706cc: bl       #0x30eba4
006706d0: mov      r1, r0
006706d4: mov      r0, r5
006706d8: bl       #0x30ed6c
006706dc: mov      r1, r0
006706e0: ldr      r0, [sp, #8]
006706e4: bl       #0x30eba4
006706e8: str      r0, [sp, #8]
006706ec: ldr      r1, [r4, #4]
006706f0: mov      r0, sl
006706f4: bl       #0x30ed6c
006706f8: ldr      r1, [r4, #0x14]
006706fc: mov      fp, r0
00670700: mov      r0, r8
00670704: bl       #0x30ed6c
00670708: mov      r1, r0
0067070c: mov      r0, fp
00670710: bl       #0x30eba4
00670714: ldr      r1, [r4, #0x24]
00670718: mov      fp, r0
0067071c: mov      r0, r7
00670720: bl       #0x30ed6c
00670724: mov      r1, r0
00670728: mov      r0, fp
0067072c: bl       #0x30eba4
00670730: ldr      r1, [r4, #0x34]
00670734: bl       #0x30eba4
00670738: mov      r1, r0
0067073c: mov      r0, r5
00670740: bl       #0x30ed6c
00670744: mov      r1, r0
00670748: mov      r0, sb
0067074c: bl       #0x30eba4
00670750: ldr      r1, [r4, #8]
00670754: mov      sb, r0
00670758: mov      r0, sl
0067075c: bl       #0x30ed6c
00670760: ldr      r1, [r4, #0x18]
00670764: mov      fp, r0
00670768: mov      r0, r8
0067076c: bl       #0x30ed6c
00670770: mov      r1, r0
00670774: mov      r0, fp
00670778: bl       #0x30eba4
0067077c: ldr      r1, [r4, #0x28]
00670780: mov      fp, r0
00670784: mov      r0, r7
00670788: bl       #0x30ed6c
0067078c: mov      r1, r0
00670790: mov      r0, fp
00670794: bl       #0x30eba4
00670798: ldr      r1, [r4, #0x38]
0067079c: bl       #0x30eba4
006707a0: mov      r1, r0
006707a4: mov      r0, r5
006707a8: bl       #0x30ed6c
006707ac: mov      r1, r0
006707b0: ldr      r0, [sp, #0xc]
006707b4: bl       #0x30eba4
006707b8: str      r0, [sp, #0xc]
006707bc: ldr      ip, [sp, #0x18]
006707c0: uxtb     r3, r6
006707c4: cmp      r3, ip
006707c8: blo      #0x670640
006707cc: ldr      r0, [sp, #0x20]
006707d0: ldr      r2, [sp, #8]
006707d4: ldr      r3, [sp, #0x50]
006707d8: add      r0, r0, #1
006707dc: ldr      r1, [sp, #0x70]
006707e0: str      r0, [sp, #0x20]
006707e4: str      r2, [r3]
006707e8: str      sb, [r3, #4]
006707ec: ldr      ip, [sp, #0xc]
006707f0: cmp      r0, r1
006707f4: str      ip, [r3, #8]
006707f8: bhs      #0x6708bc
006707fc: ldr      r2, [sp, #0x74]
00670800: ldr      r0, [sp, #0x54]
00670804: ldr      r1, [sp, #0x78]
00670808: add      r3, r3, r2
0067080c: str      r3, [sp, #0x50]
00670810: ldr      ip, [sp, #0x7c]
00670814: ldr      r3, [sp, #0x58]
00670818: add      r0, r0, r1
0067081c: str      r0, [sp, #0x54]
00670820: add      r3, r3, ip
00670824: str      r3, [sp, #0x58]
00670828: b        #0x6705c8
0067082c: ldr      r0, [sp, #0xb0]
00670830: cmp      r0, #0
00670834: beq      #0x670874
00670838: ldr      r1, [sp, #0x90]
0067083c: ldr      r4, [r1]
00670840: ldrb     r3, [r4, #0x13]
00670844: and      r2, r3, #0x1f
00670848: cmp      r2, #1
0067084c: bhi      #0x670a10
00670850: ldrb     r3, [r4, #0x12]
00670854: tst      r3, #0x20
00670858: beq      #0x67086c
0067085c: ldr      r3, [r4]
00670860: mov      r0, r4
00670864: mov      lr, pc
00670868: ldr      pc, [r3, #0x18]
0067086c: mov      r3, #0
00670870: strb     r3, [r4, #0x13]
00670874: ldr      r2, [sp, #0xb4]
00670878: cmp      r2, #0
0067087c: beq      #0x6708bc
00670880: ldr      r3, [sp, #0xac]
00670884: ldr      r4, [r3]
00670888: ldrb     r3, [r4, #0x13]
0067088c: and      r2, r3, #0x1f
00670890: cmp      r2, #1
00670894: bhi      #0x6709fc
00670898: ldrb     r3, [r4, #0x12]
0067089c: tst      r3, #0x20
006708a0: beq      #0x6708b4
006708a4: ldr      r3, [r4]
006708a8: mov      r0, r4
006708ac: mov      lr, pc
006708b0: ldr      pc, [r3, #0x18]
006708b4: mov      r3, #0
006708b8: strb     r3, [r4, #0x13]
006708bc: ldr      r1, [sp, #0xa0]
006708c0: cmp      r1, #0
006708c4: beq      #0x6708f4
006708c8: ldr      r2, [sp, #0x94]
006708cc: ldr      r3, [sp, #0x88]
006708d0: ldr      r4, [r3, r2, lsl #4]
006708d4: ldrb     r3, [r4, #0x13]
006708d8: and      r2, r3, #0x1f
006708dc: cmp      r2, #1
006708e0: bls      #0x6709ac
006708e4: sub      r2, r2, #1
006708e8: bic      r3, r3, #0x1f
006708ec: orr      r3, r2, r3
006708f0: strb     r3, [r4, #0x13]
006708f4: ldr      ip, [sp, #0xa4]
006708f8: cmp      ip, #0
006708fc: beq      #0x670928
00670900: ldr      r0, [sp, #0x84]
00670904: ldr      r4, [r0, #0x14]
00670908: ldrb     r3, [r4, #0x13]
0067090c: and      r2, r3, #0x1f
00670910: cmp      r2, #1
00670914: bls      #0x670994
00670918: sub      r2, r2, #1
0067091c: bic      r3, r3, #0x1f
00670920: orr      r3, r2, r3
00670924: strb     r3, [r4, #0x13]
00670928: ldr      r1, [sp, #0xa8]
0067092c: cmp      r1, #0
00670930: beq      #0x67095c
00670934: ldr      r2, [sp, #0x8c]
00670938: ldr      r4, [r2, #0x14]
0067093c: ldrb     r3, [r4, #0x13]
00670940: and      r2, r3, #0x1f
00670944: cmp      r2, #1
00670948: bls      #0x6709c4
0067094c: sub      r2, r2, #1
00670950: bic      r3, r3, #0x1f
00670954: orr      r3, r2, r3
00670958: strb     r3, [r4, #0x13]
0067095c: add      sp, sp, #0xbc
00670960: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00670964: mov      sb, #0
00670968: str      sb, [sp, #8]
0067096c: str      sb, [sp, #0xc]
00670970: b        #0x6707cc
00670974: ldr      r3, [sp, #0x84]
00670978: ldr      ip, [sp, #0x88]
0067097c: ldrb     r2, [r3, #0xc]
00670980: add      r2, r2, #1
00670984: uxtb     r3, r2
00670988: add      r3, ip, r3, lsl #4
0067098c: str      r3, [sp, #0xac]
00670990: b        #0x670054
00670994: ldrb     r3, [r4, #0x12]
00670998: tst      r3, #0x20
0067099c: bne      #0x670a38
006709a0: mov      r3, #0
006709a4: strb     r3, [r4, #0x13]
006709a8: b        #0x670928
006709ac: ldrb     r3, [r4, #0x12]
006709b0: tst      r3, #0x20
006709b4: bne      #0x670a24
006709b8: mov      r3, #0
006709bc: strb     r3, [r4, #0x13]
006709c0: b        #0x6708f4
006709c4: ldrb     r3, [r4, #0x12]
006709c8: tst      r3, #0x20
006709cc: bne      #0x670a4c
006709d0: mov      r3, #0
006709d4: strb     r3, [r4, #0x13]
006709d8: b        #0x67095c
006709dc: mov      r0, #0
006709e0: str      r0, [sp, #0x30]
006709e4: str      r0, [sp, #0x40]
006709e8: str      r0, [sp, #0x44]
006709ec: str      r0, [sp, #0x34]
006709f0: str      r0, [sp, #0x38]
006709f4: str      r0, [sp, #0x3c]
006709f8: b        #0x670514
006709fc: sub      r2, r2, #1
00670a00: bic      r3, r3, #0x1f
00670a04: orr      r3, r2, r3
00670a08: strb     r3, [r4, #0x13]
00670a0c: b        #0x6708bc
00670a10: sub      r2, r2, #1
00670a14: bic      r3, r3, #0x1f
00670a18: orr      r3, r2, r3
00670a1c: strb     r3, [r4, #0x13]
00670a20: b        #0x670874
00670a24: ldr      r3, [r4]
00670a28: mov      r0, r4
00670a2c: mov      lr, pc
00670a30: ldr      pc, [r3, #0x18]
00670a34: b        #0x6709b8
00670a38: ldr      r3, [r4]
00670a3c: mov      r0, r4
00670a40: mov      lr, pc
00670a44: ldr      pc, [r3, #0x18]
00670a48: b        #0x6709a0
00670a4c: ldr      r3, [r4]
00670a50: mov      r0, r4
00670a54: mov      lr, pc
00670a58: ldr      pc, [r3, #0x18]
00670a5c: b        #0x6709d0

# _ZN6glitch7collada6detail21IColladaSkinTechnique15initProxyBufferEPNS_5scene11CMeshBufferERNS0_11SSkinBufferERNS0_5SSkinEPNS_5video12IVideoDriverE
00670ec8: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00670ecc: sub      sp, sp, #0x5c
00670ed0: str      r2, [sp, #0x18]
00670ed4: ldr      r2, [r2]
00670ed8: ldr      r4, [pc, #0x540]
00670edc: str      r0, [sp, #0x1c]
00670ee0: cmp      r2, #0
00670ee4: add      r4, pc, r4
00670ee8: str      r2, [sp, #0x14]
00670eec: mov      r6, r1
00670ef0: mov      r5, r3
00670ef4: beq      #0x67130c
00670ef8: ldr      r0, [r6, #0x14]
00670efc: ldr      r2, [sp, #0x14]
00670f00: ldrb     r8, [r0, #0xc]
00670f04: ldr      r4, [r2, #0x14]
00670f08: cmp      r8, #0
00670f0c: beq      #0x670fc8
00670f10: mov      r3, #0x24
00670f14: mov      r2, #0
00670f18: mov      sl, #8
00670f1c: b        #0x670f90
00670f20: ldrh     r7, [r4, #0xe]
00670f24: ldr      lr, [r4, #0x10]
00670f28: add      r2, r2, #1
00670f2c: orr      ip, ip, r7
00670f30: strh     ip, [r4, #0xe]
00670f34: ldr      sb, [r0, #0x10]
00670f38: add      ip, lr, r3
00670f3c: add      r7, sb, r3
00670f40: ldr      fp, [sb, r3]
00670f44: ldr      sb, [r7, #8]
00670f48: ldr      r7, [r7, #4]
00670f4c: str      fp, [lr, r3]
00670f50: str      sb, [ip, #8]
00670f54: str      r7, [ip, #4]
00670f58: ldr      r7, [r0, #0x10]
00670f5c: ldr      lr, [r4, #0x10]
00670f60: add      r3, r3, #0x18
00670f64: ldr      fp, [r7, r1]
00670f68: add      ip, r7, r1
00670f6c: ldr      sb, [ip, #8]
00670f70: ldr      r7, [ip, #4]
00670f74: add      ip, lr, r1
00670f78: str      fp, [lr, r1]
00670f7c: uxtb     r1, r2
00670f80: cmp      r8, r1
00670f84: str      sb, [ip, #8]
00670f88: str      r7, [ip, #4]
00670f8c: bls      #0x670fc4
00670f90: lsl      ip, sl, r2
00670f94: ldrh     lr, [r0, #0xe]
00670f98: sub      r1, r3, #0xc
00670f9c: tst      ip, lr
00670fa0: bne      #0x670f20
00670fa4: ldrh     r1, [r4, #0xe]
00670fa8: add      r2, r2, #1
00670fac: add      r3, r3, #0x18
00670fb0: bic      ip, r1, ip
00670fb4: uxtb     r1, r2
00670fb8: cmp      r8, r1
00670fbc: strh     ip, [r4, #0xe]
00670fc0: bhi      #0x670f90
00670fc4: ldr      r0, [r6, #0x14]
00670fc8: cmp      r0, #0
00670fcc: str      r0, [sp, #0x4c]
00670fd0: ldrne    r3, [r0]
00670fd4: addne    r3, r3, #1
00670fd8: strne    r3, [r0]
00670fdc: ldrne    r0, [sp, #0x4c]
00670fe0: ldr      r7, [r0, #8]
00670fe4: add      r0, sp, #0x4c
00670fe8: bl       #0x35eb90
00670fec: str      r7, [r4, #8]
00670ff0: mvn      r2, #0x30000000
00670ff4: mov      r3, #0
00670ff8: mov      ip, #1
00670ffc: mov      r0, r4
00671000: add      r1, r6, #0x14
00671004: str      ip, [sp]
00671008: bl       #0x5a0c60
0067100c: ldr      r3, [sp, #0x18]
00671010: ldrb     r7, [r5, #0x98]
00671014: ldr      r2, [r5, #0x94]
00671018: ldrb     r8, [r3, #0x12]
0067101c: add      r7, r7, #1
00671020: add      r3, r4, #0x14
00671024: cmp      r2, #0
00671028: add      r8, r3, r8, lsl #4
0067102c: lsl      r7, r7, #2
00671030: beq      #0x671088
00671034: ldr      r3, [r6, #0x14]
00671038: ldr      sl, [r2, #0xc]
0067103c: add      r0, sp, #0x48
00671040: cmp      r3, #0
00671044: str      r3, [sp, #0x48]
00671048: ldrne    r2, [r3]
0067104c: addne    r2, r2, #1
00671050: strne    r2, [r3]
00671054: ldrne    r3, [sp, #0x48]
00671058: ldr      sb, [r3, #8]
0067105c: bl       #0x35eb90
00671060: mul      sb, sb, r7
00671064: cmp      sb, sl
00671068: bhi      #0x671088
0067106c: ldr      r1, [sp, #0x1c]
00671070: ldr      r3, [r1, #8]
00671074: cmp      r3, #0
00671078: beq      #0x671088
0067107c: ldr      r3, [r3]
00671080: cmp      r3, #0
00671084: bne      #0x6711c0
00671088: ldr      r2, [sp, #0x1c]
0067108c: ldrb     r3, [r2, #4]
00671090: cmp      r3, #0
00671094: beq      #0x671278
00671098: ldr      sl, [r5, #0x80]
0067109c: cmp      sl, #0
006710a0: ldrne    r3, [sl]
006710a4: addne    r3, r3, #2
006710a8: strne    r3, [sl]
006710ac: ldr      r3, [sp, #0x1c]
006710b0: ldr      sb, [r3, #8]
006710b4: cmp      sb, #0
006710b8: beq      #0x6710e8
006710bc: ldr      r3, [sb]
006710c0: sub      r3, r3, #1
006710c4: cmp      r3, #0
006710c8: str      r3, [sb]
006710cc: bne      #0x6710e8
006710d0: ldr      r0, [sb, #0xc]
006710d4: cmp      r0, #0
006710d8: beq      #0x6710e0
006710dc: bl       #0x30e0b8
006710e0: mov      r3, #0
006710e4: str      r3, [sb, #0xc]
006710e8: ldr      r1, [sp, #0x1c]
006710ec: cmp      sl, #0
006710f0: str      sl, [r1, #8]
006710f4: beq      #0x671124
006710f8: ldr      r3, [sl]
006710fc: sub      r3, r3, #1
00671100: cmp      r3, #0
00671104: str      r3, [sl]
00671108: bne      #0x671124
0067110c: ldr      r0, [sl, #0xc]
00671110: cmp      r0, #0
00671114: beq      #0x67111c
00671118: bl       #0x30e0b8
0067111c: mov      r3, #0
00671120: str      r3, [sl, #0xc]
00671124: ldr      r1, [sp, #0x80]
00671128: ldr      r3, [r6, #0x14]
0067112c: add      r0, sp, #0x44
00671130: ldr      r2, [r1]
00671134: cmp      r3, #0
00671138: ldr      r6, [r2, #0x78]
0067113c: str      r3, [sp, #0x44]
00671140: ldrne    r2, [r3]
00671144: addne    r2, r2, #1
00671148: strne    r2, [r3]
0067114c: ldrne    r3, [sp, #0x44]
00671150: ldr      sl, [r3, #8]
00671154: bl       #0x35eb90
00671158: ldr      r2, [sp, #0x1c]
0067115c: mul      sl, sl, r7
00671160: ldr      r3, [r2, #8]
00671164: str      sl, [sp]
00671168: mov      r2, #0
0067116c: ldr      r3, [r3, #0xc]
00671170: add      r0, sp, #0x54
00671174: str      r2, [sp, #8]
00671178: str      r3, [sp, #4]
0067117c: ldr      r1, [sp, #0x80]
00671180: mov      r3, #4
00671184: blx      r6
00671188: ldr      r3, [sp, #0x54]
0067118c: cmp      r3, #0
00671190: ldrne    r2, [r3, #4]
00671194: addne    r2, r2, #1
00671198: strne    r2, [r3, #4]
0067119c: ldr      r0, [r5, #0x94]
006711a0: str      r3, [r5, #0x94]
006711a4: cmp      r0, #0
006711a8: beq      #0x6711b0
006711ac: bl       #0x31d584
006711b0: ldr      r0, [sp, #0x54]
006711b4: cmp      r0, #0
006711b8: beq      #0x6711c0
006711bc: bl       #0x31d584
006711c0: ldr      r3, [r5, #0x94]
006711c4: mov      r0, r4
006711c8: uxth     r7, r7
006711cc: cmp      r3, #0
006711d0: str      r3, [sp, #0x30]
006711d4: ldrne    r2, [r3, #4]
006711d8: mov      r1, r8
006711dc: addne    r2, r2, #1
006711e0: strne    r2, [r3, #4]
006711e4: mov      r3, #0
006711e8: str      r3, [sp, #0x34]
006711ec: mov      r3, #1
006711f0: str      r3, [sp, #0x38]
006711f4: add      r2, sp, #0x30
006711f8: mov      r3, #4
006711fc: strh     r3, [sp, #0x3c]
00671200: strh     r7, [sp, #0x3e]
00671204: bl       #0x670d64
00671208: ldr      r0, [sp, #0x30]
0067120c: cmp      r0, #0
00671210: beq      #0x671218
00671214: bl       #0x31d584
00671218: ldr      r2, [r5, #0x94]
0067121c: ldrb     r3, [r5, #0x98]
00671220: mov      ip, #4
00671224: cmp      r2, #0
00671228: str      r2, [sp, #0x20]
0067122c: ldrne    r0, [r2, #4]
00671230: sub      r1, r8, #0x10
00671234: addne    r0, r0, #1
00671238: strne    r0, [r2, #4]
0067123c: mov      r0, r4
00671240: str      ip, [sp, #0x24]
00671244: add      r2, sp, #0x20
00671248: mov      ip, #6
0067124c: str      ip, [sp, #0x28]
00671250: strh     r3, [sp, #0x2c]
00671254: strh     r7, [sp, #0x2e]
00671258: bl       #0x670d64
0067125c: ldr      r0, [sp, #0x20]
00671260: cmp      r0, #0
00671264: beq      #0x67126c
00671268: bl       #0x31d584
0067126c: ldr      r0, [sp, #0x14]
00671270: add      sp, sp, #0x5c
00671274: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00671278: ldr      r1, [sp, #0x80]
0067127c: ldr      r3, [r6, #0x14]
00671280: add      r0, sp, #0x40
00671284: ldr      r2, [r1]
00671288: cmp      r3, #0
0067128c: ldr      r6, [r2, #0x78]
00671290: str      r3, [sp, #0x40]
00671294: ldrne    r2, [r3]
00671298: addne    r2, r2, #1
0067129c: strne    r2, [r3]
006712a0: ldrne    r3, [sp, #0x40]
006712a4: ldr      sl, [r3, #8]
006712a8: bl       #0x35eb90
006712ac: mul      sl, sl, r7
006712b0: ldr      r3, [r5, #0x80]
006712b4: mov      r2, #0
006712b8: str      r2, [sp, #8]
006712bc: str      r3, [sp, #4]
006712c0: add      r0, sp, #0x50
006712c4: mov      r3, #4
006712c8: str      sl, [sp]
006712cc: ldr      r1, [sp, #0x80]
006712d0: blx      r6
006712d4: ldr      r3, [sp, #0x50]
006712d8: cmp      r3, #0
006712dc: ldrne    r2, [r3, #4]
006712e0: addne    r2, r2, #1
006712e4: strne    r2, [r3, #4]
006712e8: ldr      r0, [r5, #0x94]
006712ec: str      r3, [r5, #0x94]
006712f0: cmp      r0, #0
006712f4: beq      #0x6712fc
006712f8: bl       #0x31d584
006712fc: ldr      r0, [sp, #0x50]
00671300: cmp      r0, #0
00671304: bne      #0x6711bc
00671308: b        #0x6711c0
0067130c: ldr      r3, [r1, #0x14]
00671310: mov      r0, #0x38
00671314: mov      r1, r2
00671318: ldr      r7, [r3, #4]
0067131c: bl       #0x5341ac
00671320: ldr      r2, [pc, #0xfc]
00671324: mov      r3, #0
00671328: str      r0, [sp, #0x14]
0067132c: ldr      r2, [r4, r2]
00671330: str      r3, [r0, #0x10]
00671334: ldr      r1, [sp, #0x14]
00671338: add      r2, r2, #8
0067133c: orr      r7, r7, #0x30000000
00671340: str      r3, [r1, #4]
00671344: str      r3, [r1, #8]
00671348: str      r3, [r1, #0xc]
0067134c: str      r2, [r1]
00671350: ldr      r2, [sp, #0x14]
00671354: orr      r7, r7, #0x40000
00671358: mov      r1, r7
0067135c: add      r0, r2, #0x14
00671360: bl       #0x5a135c
00671364: ldr      r3, [r6, #0x18]
00671368: ldr      r1, [sp, #0x14]
0067136c: cmp      r3, #0
00671370: str      r3, [r1, #0x18]
00671374: ldrne    r2, [r3, #4]
00671378: addne    r2, r2, #1
0067137c: strne    r2, [r3, #4]
00671380: ldr      r2, [r6, #0x1c]
00671384: ldr      r1, [sp, #0x14]
00671388: str      r2, [r1, #0x1c]
0067138c: ldr      r2, [r6, #0x20]
00671390: ldr      r3, [r1, #4]
00671394: str      r2, [r1, #0x20]
00671398: ldr      r2, [r6, #0x24]
0067139c: add      r3, r3, #1
006713a0: str      r2, [r1, #0x24]
006713a4: ldr      r2, [r6, #0x28]
006713a8: str      r2, [r1, #0x28]
006713ac: ldrh     r2, [r6, #0x2c]
006713b0: strh     r2, [r1, #0x2c]
006713b4: ldrh     r2, [r6, #0x2e]
006713b8: str      r3, [r1, #4]
006713bc: strh     r2, [r1, #0x2e]
006713c0: mov      r2, #0
006713c4: str      r2, [r1, #0x30]
006713c8: mov      r2, #1
006713cc: strb     r2, [r1, #0x34]
006713d0: ldr      r3, [sp, #0x18]
006713d4: ldr      r0, [r3]
006713d8: str      r1, [r3]
006713dc: cmp      r0, #0
006713e0: beq      #0x6713e8
006713e4: bl       #0x31d584
006713e8: ldr      r1, [sp, #0x14]
006713ec: ldr      r0, [r1, #0x14]
006713f0: mov      r1, #0x1d
006713f4: add      r2, r0, #0x14
006713f8: ldr      r3, [r0, #0x10]
006713fc: bl       #0x5a0af0
00671400: ldr      r2, [sp, #0x14]
00671404: ldr      r1, [sp, #0x18]
00671408: ldr      r3, [r2, #0x14]
0067140c: add      r3, r3, #0x14
00671410: rsb      r3, r3, r0
00671414: asr      r3, r3, #4
00671418: strb     r3, [r1, #0x12]
0067141c: b        #0x670ef8
00671420: eorseq   r3, r2, ip, lsr #23
00671424: andeq    r0, r0, r4, asr ip

# _ZN6glitch7collada12CSkinnedMesh15instanciateMeshEPNS_5video12IVideoDriverEPNS0_14CRootSceneNodeE
00664af8: push     {r4, r5, r6, r7, r8, sl, lr}
00664afc: ldr      r3, [r0, #0x1c]
00664b00: sub      sp, sp, #0x2c
00664b04: mov      r6, r1
00664b08: ldr      r5, [r3, #0x70]
00664b0c: add      r7, r0, #0xc
00664b10: mov      r4, r0
00664b14: add      r5, r5, #1
00664b18: mov      r8, r2
00664b1c: add      r0, sp, #0x24
00664b20: mov      r1, r7
00664b24: mov      r2, r6
00664b28: mov      r3, r5
00664b2c: bl       #0x61aaa8
00664b30: ldr      sl, [sp, #0x24]
00664b34: cmp      sl, #0
00664b38: beq      #0x664c38
00664b3c: ldr      r3, [sl, #4]
00664b40: add      r3, r3, #1
00664b44: str      r3, [sl, #4]
00664b48: ldr      r0, [sp, #0x24]
00664b4c: cmp      r0, #0
00664b50: beq      #0x664b58
00664b54: bl       #0x31d584
00664b58: cmp      sl, #0
00664b5c: beq      #0x664c38
00664b60: ldr      r3, [sl, #4]
00664b64: add      r3, r3, #1
00664b68: str      r3, [sl, #4]
00664b6c: ldr      r0, [r4, #0x68]
00664b70: str      sl, [r4, #0x68]
00664b74: cmp      r0, #0
00664b78: moveq    r3, sl
00664b7c: beq      #0x664b88
00664b80: bl       #0x31d584
00664b84: ldr      r3, [r4, #0x68]
00664b88: mov      r0, r3
00664b8c: ldr      r3, [r3]
00664b90: mov      lr, pc
00664b94: ldr      pc, [r3, #0x24]
00664b98: ldr      r3, [r0]
00664b9c: str      r3, [r4, #0x24]
00664ba0: ldr      r3, [r0, #4]
00664ba4: str      r3, [r4, #0x28]
00664ba8: ldr      r3, [r0, #8]
00664bac: str      r3, [r4, #0x2c]
00664bb0: ldr      r3, [r0, #0xc]
00664bb4: str      r3, [r4, #0x30]
00664bb8: ldr      r3, [r0, #0x10]
00664bbc: str      r3, [r4, #0x34]
00664bc0: ldr      r3, [r0, #0x14]
00664bc4: str      r3, [r4, #0x38]
00664bc8: ldr      r3, [r4, #0x68]
00664bcc: add      r5, sp, #0xc
00664bd0: add      r4, r4, #0x5c
00664bd4: mov      r0, r3
00664bd8: ldr      r3, [r3]
00664bdc: mov      lr, pc
00664be0: ldr      pc, [r3, #0x10]
00664be4: mov      r3, #0
00664be8: mvn      ip, #0
00664bec: mov      r1, r0
00664bf0: mov      r2, r5
00664bf4: mov      r0, r4
00664bf8: str      r3, [sp, #0x18]
00664bfc: strb     ip, [sp, #0x1e]
00664c00: str      r3, [sp, #0xc]
00664c04: str      r3, [sp, #0x10]
00664c08: str      r3, [sp, #0x14]
00664c0c: strb     ip, [sp, #0x1c]
00664c10: strb     ip, [sp, #0x1d]
00664c14: bl       #0x664a90
00664c18: mov      r0, r5
00664c1c: bl       #0x6645f8
00664c20: cmp      sl, #0
00664c24: beq      #0x664c30
00664c28: mov      r0, sl
00664c2c: bl       #0x31d584
00664c30: add      sp, sp, #0x2c
00664c34: pop      {r4, r5, r6, r7, r8, sl, pc}
00664c38: mov      r1, r7
00664c3c: mov      r2, r6
00664c40: mov      r3, r5
00664c44: add      r0, sp, #0x20
00664c48: str      r8, [sp]
00664c4c: bl       #0x61aa00
00664c50: ldr      sl, [sp, #0x20]
00664c54: cmp      sl, #0
00664c58: beq      #0x664bc8
00664c5c: ldr      r3, [sl, #4]
00664c60: add      r3, r3, #1
00664c64: str      r3, [sl, #4]
00664c68: ldr      r0, [sp, #0x20]
00664c6c: cmp      r0, #0
00664c70: beq      #0x664c78
00664c74: bl       #0x31d584
00664c78: cmp      sl, #0
00664c7c: beq      #0x664bc8
00664c80: b        #0x664b60

# _ZN6glitch4coremlIfNS_7collada7SMatrixEEENS0_8CMatrix4IT_EERKS6_RKT0_
006651a0: push     {r4, r5, r6, r7, r8, lr}
006651a4: mov      r3, #0
006651a8: strb     r3, [r0, #0x40]
006651ac: mov      r5, r1
006651b0: ldrb     r1, [r1, #0x40]
006651b4: mov      r4, r0
006651b8: mov      r6, r2
006651bc: cmp      r1, r3
006651c0: beq      #0x6651e8
006651c4: mov      r1, r3
006651c8: strb     r1, [r4, #0x40]
006651cc: ldr      r2, [r6, r3]
006651d0: str      r2, [r4, r3]
006651d4: add      r3, r3, #4
006651d8: cmp      r3, #0x40
006651dc: bne      #0x6651c8
006651e0: mov      r0, r4
006651e4: pop      {r4, r5, r6, r7, r8, pc}
006651e8: ldr      r1, [r2]
006651ec: ldr      r0, [r5]
006651f0: bl       #0x30ed6c
006651f4: ldr      r1, [r6, #4]
006651f8: mov      r7, r0
006651fc: ldr      r0, [r5, #0x10]
00665200: bl       #0x30ed6c
00665204: mov      r1, r0
00665208: mov      r0, r7
0066520c: bl       #0x30eba4
00665210: ldr      r1, [r6, #8]
00665214: mov      r7, r0
00665218: ldr      r0, [r5, #0x20]
0066521c: bl       #0x30ed6c
00665220: mov      r1, r0
00665224: mov      r0, r7
00665228: bl       #0x30eba4
0066522c: str      r0, [r4]
00665230: ldr      r1, [r6]
00665234: ldr      r0, [r5, #4]
00665238: bl       #0x30ed6c
0066523c: ldr      r1, [r6, #4]
00665240: mov      r7, r0
00665244: ldr      r0, [r5, #0x14]
00665248: bl       #0x30ed6c
0066524c: mov      r1, r0
00665250: mov      r0, r7
00665254: bl       #0x30eba4
00665258: ldr      r1, [r6, #8]
0066525c: mov      r7, r0
00665260: ldr      r0, [r5, #0x24]
00665264: bl       #0x30ed6c
00665268: mov      r1, r0
0066526c: mov      r0, r7
00665270: bl       #0x30eba4
00665274: str      r0, [r4, #4]
00665278: ldr      r1, [r6]
0066527c: ldr      r0, [r5, #8]
00665280: bl       #0x30ed6c
00665284: ldr      r1, [r6, #4]
00665288: mov      r7, r0
0066528c: ldr      r0, [r5, #0x18]
00665290: bl       #0x30ed6c
00665294: mov      r1, r0
00665298: mov      r0, r7
0066529c: bl       #0x30eba4
006652a0: ldr      r1, [r6, #8]
006652a4: mov      r7, r0
006652a8: ldr      r0, [r5, #0x28]
006652ac: bl       #0x30ed6c
006652b0: mov      r1, r0
006652b4: mov      r0, r7
006652b8: bl       #0x30eba4
006652bc: mov      r7, #0
006652c0: str      r0, [r4, #8]
006652c4: str      r7, [r4, #0xc]
006652c8: ldr      r1, [r6, #0x10]
006652cc: ldr      r0, [r5]
006652d0: bl       #0x30ed6c
006652d4: ldr      r1, [r6, #0x14]
006652d8: mov      r8, r0
006652dc: ldr      r0, [r5, #0x10]
006652e0: bl       #0x30ed6c
006652e4: mov      r1, r0
006652e8: mov      r0, r8
006652ec: bl       #0x30eba4
006652f0: ldr      r1, [r6, #0x18]
006652f4: mov      r8, r0
006652f8: ldr      r0, [r5, #0x20]
006652fc: bl       #0x30ed6c
00665300: mov      r1, r0
00665304: mov      r0, r8
00665308: bl       #0x30eba4
0066530c: str      r0, [r4, #0x10]
00665310: ldr      r1, [r6, #0x10]
00665314: ldr      r0, [r5, #4]
00665318: bl       #0x30ed6c
0066531c: ldr      r1, [r6, #0x14]
00665320: mov      r8, r0
00665324: ldr      r0, [r5, #0x14]
00665328: bl       #0x30ed6c
0066532c: mov      r1, r0
00665330: mov      r0, r8
00665334: bl       #0x30eba4
00665338: ldr      r1, [r6, #0x18]
0066533c: mov      r8, r0
00665340: ldr      r0, [r5, #0x24]
00665344: bl       #0x30ed6c
00665348: mov      r1, r0
0066534c: mov      r0, r8
00665350: bl       #0x30eba4
00665354: str      r0, [r4, #0x14]
00665358: ldr      r1, [r6, #0x10]
0066535c: ldr      r0, [r5, #8]
00665360: bl       #0x30ed6c
00665364: ldr      r1, [r6, #0x14]
00665368: mov      r8, r0
0066536c: ldr      r0, [r5, #0x18]
00665370: bl       #0x30ed6c
00665374: mov      r1, r0
00665378: mov      r0, r8
0066537c: bl       #0x30eba4
00665380: ldr      r1, [r6, #0x18]
00665384: mov      r8, r0
00665388: ldr      r0, [r5, #0x28]
0066538c: bl       #0x30ed6c
00665390: mov      r1, r0
00665394: mov      r0, r8
00665398: bl       #0x30eba4
0066539c: str      r0, [r4, #0x18]
006653a0: str      r7, [r4, #0x1c]
006653a4: ldr      r1, [r6, #0x20]
006653a8: ldr      r0, [r5]
006653ac: bl       #0x30ed6c
006653b0: ldr      r1, [r6, #0x24]
006653b4: mov      r8, r0
006653b8: ldr      r0, [r5, #0x10]
006653bc: bl       #0x30ed6c
006653c0: mov      r1, r0
006653c4: mov      r0, r8
006653c8: bl       #0x30eba4
006653cc: ldr      r1, [r6, #0x28]
006653d0: mov      r8, r0
006653d4: ldr      r0, [r5, #0x20]
006653d8: bl       #0x30ed6c
006653dc: mov      r1, r0
006653e0: mov      r0, r8
006653e4: bl       #0x30eba4
006653e8: str      r0, [r4, #0x20]
006653ec: ldr      r1, [r6, #0x20]
006653f0: ldr      r0, [r5, #4]
006653f4: bl       #0x30ed6c
006653f8: ldr      r1, [r6, #0x24]
006653fc: mov      r8, r0
00665400: ldr      r0, [r5, #0x14]
00665404: bl       #0x30ed6c
00665408: mov      r1, r0
0066540c: mov      r0, r8
00665410: bl       #0x30eba4
00665414: ldr      r1, [r6, #0x28]
00665418: mov      r8, r0
0066541c: ldr      r0, [r5, #0x24]
00665420: bl       #0x30ed6c
00665424: mov      r1, r0
00665428: mov      r0, r8
0066542c: bl       #0x30eba4
00665430: str      r0, [r4, #0x24]
00665434: ldr      r1, [r6, #0x20]
00665438: ldr      r0, [r5, #8]
0066543c: bl       #0x30ed6c
00665440: ldr      r1, [r6, #0x24]
00665444: mov      r8, r0
00665448: ldr      r0, [r5, #0x18]
0066544c: bl       #0x30ed6c
00665450: mov      r1, r0
00665454: mov      r0, r8
00665458: bl       #0x30eba4
0066545c: ldr      r1, [r6, #0x28]
00665460: mov      r8, r0
00665464: ldr      r0, [r5, #0x28]
00665468: bl       #0x30ed6c
0066546c: mov      r1, r0
00665470: mov      r0, r8
00665474: bl       #0x30eba4
00665478: str      r0, [r4, #0x28]
0066547c: str      r7, [r4, #0x2c]
00665480: ldr      r1, [r6, #0x30]
00665484: ldr      r0, [r5]
00665488: bl       #0x30ed6c
0066548c: ldr      r1, [r6, #0x34]
00665490: mov      r7, r0
00665494: ldr      r0, [r5, #0x10]
00665498: bl       #0x30ed6c
0066549c: mov      r1, r0
006654a0: mov      r0, r7
006654a4: bl       #0x30eba4
006654a8: ldr      r1, [r6, #0x38]
006654ac: mov      r7, r0
006654b0: ldr      r0, [r5, #0x20]
006654b4: bl       #0x30ed6c
006654b8: mov      r1, r0
006654bc: mov      r0, r7
006654c0: bl       #0x30eba4
006654c4: ldr      r1, [r5, #0x30]
006654c8: bl       #0x30eba4
006654cc: str      r0, [r4, #0x30]
006654d0: ldr      r1, [r6, #0x30]
006654d4: ldr      r0, [r5, #4]
006654d8: bl       #0x30ed6c
006654dc: ldr      r1, [r6, #0x34]
006654e0: mov      r7, r0
006654e4: ldr      r0, [r5, #0x14]
006654e8: bl       #0x30ed6c
006654ec: mov      r1, r0
006654f0: mov      r0, r7
006654f4: bl       #0x30eba4
006654f8: ldr      r1, [r6, #0x38]
006654fc: mov      r7, r0
00665500: ldr      r0, [r5, #0x24]
00665504: bl       #0x30ed6c
00665508: mov      r1, r0
0066550c: mov      r0, r7
00665510: bl       #0x30eba4
00665514: ldr      r1, [r5, #0x34]
00665518: bl       #0x30eba4
0066551c: str      r0, [r4, #0x34]
00665520: ldr      r1, [r6, #0x30]
00665524: ldr      r0, [r5, #8]
00665528: bl       #0x30ed6c
0066552c: ldr      r1, [r6, #0x34]
00665530: mov      r7, r0
00665534: ldr      r0, [r5, #0x18]
00665538: bl       #0x30ed6c
0066553c: mov      r1, r0
00665540: mov      r0, r7
00665544: bl       #0x30eba4
00665548: ldr      r1, [r6, #0x38]
0066554c: mov      r7, r0
00665550: ldr      r0, [r5, #0x28]
00665554: bl       #0x30ed6c
00665558: mov      r1, r0
0066555c: mov      r0, r7
00665560: bl       #0x30eba4
00665564: ldr      r1, [r5, #0x38]
00665568: bl       #0x30eba4
0066556c: mov      r3, #0x3f800000
00665570: str      r0, [r4, #0x38]
00665574: str      r3, [r4, #0x3c]
00665578: mov      r0, r4
0066557c: pop      {r4, r5, r6, r7, r8, pc}

# _ZN6glitch7collada6detail29CColladaSoftwareSkinTechnique15preparePtrCacheEv
0066fab4: push     {r4, r5, r6, r7, r8, lr}
0066fab8: mov      r4, r0
0066fabc: ldr      r0, [r0, #0x10]
0066fac0: sub      sp, sp, #8
0066fac4: ldr      r3, [r0]
0066fac8: tst      r3, #0x10000
0066facc: bne      #0x66fad8
0066fad0: add      sp, sp, #8
0066fad4: pop      {r4, r5, r6, r7, r8, pc}
0066fad8: ldr      r3, [r4, #0xc]
0066fadc: add      r2, sp, #8
0066fae0: mov      r5, #0
0066fae4: ldr      r1, [r3, #0x74]
0066fae8: add      r0, r0, #0x10
0066faec: str      r5, [r2, #-4]!
0066faf0: bl       #0x66c500
0066faf4: ldr      r3, [r4, #0xc]
0066faf8: ldr      r7, [r3, #0x74]
0066fafc: cmp      r7, r5
0066fb00: bgt      #0x66fb0c
0066fb04: b        #0x66fb50
0066fb08: ldr      r3, [r4, #0xc]
0066fb0c: ldr      r3, [r3, #0x78]
0066fb10: ldr      r0, [r4, #0x14]
0066fb14: lsl      r6, r5, #2
0066fb18: ldr      r1, [r3, r5, lsl #2]
0066fb1c: bl       #0x59840c
0066fb20: ldr      r3, [r4, #0x10]
0066fb24: subs     r2, r0, #0
0066fb28: moveq    r0, r2
0066fb2c: ldr      r8, [r3, #0x10]
0066fb30: beq      #0x66fb40
0066fb34: ldr      r3, [r2]
0066fb38: mov      lr, pc
0066fb3c: ldr      pc, [r3, #0x38]
0066fb40: add      r5, r5, #1
0066fb44: cmp      r5, r7
0066fb48: str      r0, [r8, r6]
0066fb4c: bne      #0x66fb08
0066fb50: ldr      r3, [r4, #0x10]
0066fb54: ldr      r2, [r3]
0066fb58: bic      r2, r2, #0x10000
0066fb5c: str      r2, [r3]
0066fb60: b        #0x66fad0

# _ZN6glitch7collada6detail29CColladaSoftwareSkinTechnique4initERNS0_11SSkinBufferEPNS_5scene11CMeshBufferEPNS_5video12IVideoDriverEb
0066f8ec: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0066f8f0: ldr      ip, [r0, #0xc]
0066f8f4: mov      r7, r1
0066f8f8: sub      sp, sp, #0x40
0066f8fc: mov      r1, r2
0066f900: mov      r6, r3
0066f904: mov      r2, r7
0066f908: mov      r3, ip
0066f90c: mov      r5, r0
0066f910: str      r6, [sp]
0066f914: ldrb     sb, [sp, #0x60]
0066f918: bl       #0x670ec8
0066f91c: ldr      r8, [r0, #0x14]
0066f920: mov      r3, #0
0066f924: mov      ip, #6
0066f928: add      sl, r8, #0x14
0066f92c: strh     r3, [sp, #0x3a]
0066f930: str      r3, [sp, #0x2c]
0066f934: str      r3, [sp, #0x30]
0066f938: mov      r4, r0
0066f93c: mov      r3, #3
0066f940: mov      r0, r8
0066f944: mov      r1, sl
0066f948: add      r2, sp, #0x2c
0066f94c: str      ip, [sp, #0x34]
0066f950: strh     r3, [sp, #0x38]
0066f954: bl       #0x66f884
0066f958: ldr      r0, [sp, #0x2c]
0066f95c: cmp      r0, #0
0066f960: beq      #0x66f968
0066f964: bl       #0x31d584
0066f968: ldr      r3, [r8, #4]
0066f96c: tst      r3, #0x20000
0066f970: moveq    r2, #1
0066f974: beq      #0x66f9c4
0066f978: mov      r3, #0
0066f97c: strh     r3, [sp, #0x2a]
0066f980: str      r3, [sp, #0x1c]
0066f984: str      r3, [sp, #0x20]
0066f988: mov      r2, #6
0066f98c: mov      r3, #3
0066f990: str      r2, [sp, #0x24]
0066f994: strh     r3, [sp, #0x28]
0066f998: ldrb     r1, [r8, #0xc]
0066f99c: mov      r0, r8
0066f9a0: add      r2, sp, #0x1c
0066f9a4: add      r1, r1, #1
0066f9a8: add      r1, sl, r1, lsl #4
0066f9ac: bl       #0x66f884
0066f9b0: ldr      r0, [sp, #0x1c]
0066f9b4: cmp      r0, #0
0066f9b8: beq      #0x66f9c0
0066f9bc: bl       #0x31d584
0066f9c0: mov      r2, #2
0066f9c4: ldrb     r3, [r5, #0x18]
0066f9c8: cmp      r3, #0
0066f9cc: beq      #0x66fa18
0066f9d0: ldrb     r1, [r8, #0xc]
0066f9d4: mov      r3, #0
0066f9d8: mov      r0, r8
0066f9dc: add      r2, r2, r1
0066f9e0: add      r1, sl, r2, lsl #4
0066f9e4: mov      ip, #6
0066f9e8: strh     r3, [sp, #0x1a]
0066f9ec: str      r3, [sp, #0xc]
0066f9f0: str      r3, [sp, #0x10]
0066f9f4: add      r2, sp, #0xc
0066f9f8: mov      r3, #4
0066f9fc: str      ip, [sp, #0x14]
0066fa00: strh     r3, [sp, #0x18]
0066fa04: bl       #0x66f884
0066fa08: ldr      r0, [sp, #0xc]
0066fa0c: cmp      r0, #0
0066fa10: beq      #0x66fa18
0066fa14: bl       #0x31d584
0066fa18: cmp      sb, #0
0066fa1c: movne    r0, #0
0066fa20: bne      #0x66faac
0066fa24: ldr      r1, [r7, #4]
0066fa28: ldrb     r7, [r5, #0x18]
0066fa2c: mov      r2, #0x20000
0066fa30: mov      r3, #0x60000
0066fa34: cmp      r7, #0
0066fa38: add      r2, r2, #1
0066fa3c: add      r3, r3, #1
0066fa40: mov      r0, r1
0066fa44: moveq    r7, r2
0066fa48: movne    r7, r3
0066fa4c: ldr      r5, [r1, #4]
0066fa50: bl       #0x5c5d34
0066fa54: ldr      r2, [r5, #0x18]
0066fa58: mov      r1, #0xc
0066fa5c: add      r3, sp, #0x40
0066fa60: mla      r2, r1, r0, r2
0066fa64: mov      r5, #1
0066fa68: ldr      r2, [r2, #8]
0066fa6c: mov      r0, r6
0066fa70: mov      r1, r5
0066fa74: ldr      r2, [r2, #0x20]
0066fa78: ldr      r2, [r2, #0x38]
0066fa7c: str      r4, [r3, #-4]!
0066fa80: ldr      ip, [r4, #4]
0066fa84: and      r2, r7, r2
0066fa88: add      ip, ip, r5
0066fa8c: str      ip, [r4, #4]
0066fa90: str      r5, [sp]
0066fa94: bl       #0x6498a0
0066fa98: ldr      r0, [sp, #0x3c]
0066fa9c: cmp      r0, #0
0066faa0: beq      #0x66faa8
0066faa4: bl       #0x31d584
0066faa8: mov      r0, r5
0066faac: add      sp, sp, #0x40
0066fab0: pop      {r4, r5, r6, r7, r8, sb, sl, pc}

# _ZN6glitch7collada12CSkinnedMeshC1ERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverERNS0_11SControllerEPNS0_14CRootSceneNodeE
006664f0: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
006664f4: ldr      r5, [pc, #0x4e4]
006664f8: ldr      ip, [pc, #0x4e4]
006664fc: mov      r4, r0
00666500: add      r5, pc, r5
00666504: ldr      ip, [r5, ip]
00666508: mov      r0, #0
0066650c: str      r0, [r4, #4]
00666510: add      ip, ip, #8
00666514: str      ip, [r4]
00666518: ldr      r0, [r1]
0066651c: mov      r7, r1
00666520: sub      sp, sp, #0x24
00666524: str      r0, [r4, #0xc]
00666528: ldr      r1, [r1, #4]
0066652c: cmp      r0, #0
00666530: str      r1, [r4, #0x10]
00666534: beq      #0x666548
00666538: ldr      r1, [r0, #4]
0066653c: cmp      r1, #0
00666540: addne    r1, r1, #1
00666544: strne    r1, [r0, #4]
00666548: ldr      ip, [pc, #0x498]
0066654c: ldr      r0, [pc, #0x498]
00666550: mov      r1, #0
00666554: ldr      ip, [r5, ip]
00666558: ldr      r0, [r5, r0]
0066655c: str      r1, [r4, #0x14]
00666560: add      ip, ip, #4
00666564: str      ip, [r4, #8]
00666568: add      r0, r0, #8
0066656c: mov      ip, #1
00666570: strb     ip, [r4, #0x18]
00666574: str      r0, [r4]
00666578: ldr      r6, [r3, #8]
0066657c: mov      ip, #0xbf000000
00666580: add      ip, ip, #0x800000
00666584: mov      r0, #0x3f800000
00666588: add      lr, r4, #0x44
0066658c: str      r6, [r4, #0x1c]
00666590: str      ip, [r4, #0x2c]
00666594: str      r0, [r4, #0x38]
00666598: strb     r1, [r4, #0x20]
0066659c: strb     r1, [r4, #0x22]
006665a0: strb     r1, [r4, #0x23]
006665a4: str      ip, [r4, #0x24]
006665a8: str      ip, [r4, #0x28]
006665ac: str      r0, [r4, #0x30]
006665b0: str      r0, [r4, #0x34]
006665b4: str      r1, [r4, #0x3c]
006665b8: str      r1, [r4, #0x40]
006665bc: str      r1, [r4, #0x44]
006665c0: str      r1, [lr, #4]
006665c4: str      r1, [r4, #0x4c]
006665c8: str      r1, [r4, #0x50]
006665cc: str      r1, [r4, #0x54]
006665d0: str      r1, [r4, #0x58]
006665d4: str      r1, [r4, #0x5c]
006665d8: str      r1, [r4, #0x60]
006665dc: str      r1, [r4, #0x64]
006665e0: str      r1, [r4, #0x68]
006665e4: str      r1, [r4, #0x6c]
006665e8: str      r1, [r4, #0x74]
006665ec: str      r1, [r4, #0x78]
006665f0: str      r1, [r4, #0x7c]
006665f4: str      r1, [r4, #0x80]
006665f8: str      r1, [r4, #0x98]
006665fc: str      r1, [r4, #0x84]
00666600: str      r1, [r4, #0x88]
00666604: str      r1, [r4, #0x8c]
00666608: str      r1, [r4, #0x90]
0066660c: str      r1, [r4, #0x94]
00666610: ldr      r3, [r3, #4]
00666614: mov      r1, r2
00666618: mov      r0, r4
0066661c: ldr      r2, [sp, #0x48]
00666620: str      r3, [r4, #8]
00666624: bl       #0x664af8
00666628: ldr      r3, [r7]
0066662c: ldr      r3, [r3, #0x24]
00666630: ldr      r3, [r3, #0x20]
00666634: ldr      sb, [r3, #4]
00666638: ldr      r6, [r3, #0x64]
0066663c: cmp      r6, #0
00666640: movle    r6, #0
00666644: movgt    r6, #1
00666648: cmp      sb, #0
0066664c: beq      #0x666680
00666650: ldr      r3, [pc, #0x398]
00666654: ldr      r1, [sb, #0x14]
00666658: ldr      r8, [r5, r3]
0066665c: ldr      r3, [r8]
00666660: ldr      r3, [r3, #0x20]
00666664: ldr      r3, [r3, #0x34]
00666668: mov      r0, r3
0066666c: ldr      r3, [r3]
00666670: mov      lr, pc
00666674: ldr      pc, [r3, #0xc]
00666678: subs     sb, r0, #0
0066667c: beq      #0x6669a8
00666680: ldr      r3, [pc, #0x36c]
00666684: cmp      r6, #0
00666688: str      sb, [sp, #0x10]
0066668c: ldr      r3, [r5, r3]
00666690: add      r3, r3, #8
00666694: str      r3, [sp, #0xc]
00666698: bne      #0x6667bc
0066669c: cmp      sb, #0
006666a0: beq      #0x6666ac
006666a4: mov      r0, sb
006666a8: bl       #0x31d584
006666ac: mov      r1, #0
006666b0: mov      r0, #0x38
006666b4: bl       #0x5341ac
006666b8: add      r5, r4, #0x70
006666bc: mov      r3, r6
006666c0: ldr      r1, [r4, #0x1c]
006666c4: mov      r2, r5
006666c8: mov      r7, r0
006666cc: bl       #0x66e714
006666d0: ldr      r3, [r4, #0x3c]
006666d4: str      r7, [r4, #0x3c]
006666d8: cmp      r3, #0
006666dc: beq      #0x6666f0
006666e0: mov      r0, r3
006666e4: ldr      r3, [r3]
006666e8: mov      lr, pc
006666ec: ldr      pc, [r3, #4]
006666f0: mov      r1, #0
006666f4: mov      r0, #0x30
006666f8: bl       #0x5341ac
006666fc: mov      r3, r6
00666700: ldr      r1, [r4, #0x1c]
00666704: mov      r2, r5
00666708: mov      r7, r0
0066670c: bl       #0x66b8dc
00666710: ldr      r3, [r4, #0x40]
00666714: str      r7, [r4, #0x40]
00666718: cmp      r3, #0
0066671c: beq      #0x666730
00666720: mov      r0, r3
00666724: ldr      r3, [r3]
00666728: mov      lr, pc
0066672c: ldr      pc, [r3, #4]
00666730: mov      r1, #0
00666734: mov      r0, #0x30
00666738: bl       #0x5341ac
0066673c: mov      r3, r6
00666740: ldr      r1, [r4, #0x1c]
00666744: mov      r2, r5
00666748: mov      r7, r0
0066674c: bl       #0x66d01c
00666750: ldr      r3, [r4, #0x44]
00666754: str      r7, [r4, #0x44]
00666758: cmp      r3, #0
0066675c: beq      #0x666770
00666760: mov      r0, r3
00666764: ldr      r3, [r3]
00666768: mov      lr, pc
0066676c: ldr      pc, [r3, #4]
00666770: mov      r1, #0
00666774: mov      r0, #0x34
00666778: bl       #0x5341ac
0066677c: mov      r3, r6
00666780: mov      r2, r5
00666784: ldr      r1, [r4, #0x1c]
00666788: mov      r7, r0
0066678c: bl       #0x66f658
00666790: ldr      r3, [r4, #0x48]
00666794: str      r7, [r4, #0x48]
00666798: cmp      r3, #0
0066679c: beq      #0x6667b0
006667a0: mov      r0, r3
006667a4: ldr      r3, [r3]
006667a8: mov      lr, pc
006667ac: ldr      pc, [r3, #4]
006667b0: mov      r0, r4
006667b4: add      sp, sp, #0x24
006667b8: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
006667bc: ldr      r3, [r4, #0x1c]
006667c0: add      sl, sp, #0xc
006667c4: mov      r2, sl
006667c8: ldr      r1, [r3, #0x80]
006667cc: add      r0, sp, #0x1c
006667d0: bl       #0x663bd0
006667d4: ldr      r3, [sp, #0x1c]
006667d8: cmp      r3, #0
006667dc: ldrne    r2, [r3]
006667e0: addne    r2, r2, #1
006667e4: strne    r2, [r3]
006667e8: ldr      r5, [r4, #0x4c]
006667ec: cmp      r5, #0
006667f0: beq      #0x666820
006667f4: ldr      r3, [r5]
006667f8: sub      r3, r3, #1
006667fc: cmp      r3, #0
00666800: str      r3, [r5]
00666804: bne      #0x666820
00666808: ldr      r0, [r5, #0xc]
0066680c: cmp      r0, #0
00666810: beq      #0x666818
00666814: bl       #0x30e0b8
00666818: mov      r3, #0
0066681c: str      r3, [r5, #0xc]
00666820: ldr      r5, [sp, #0x1c]
00666824: cmp      r5, #0
00666828: str      r5, [r4, #0x4c]
0066682c: beq      #0x666864
00666830: ldr      r3, [r5]
00666834: sub      r3, r3, #1
00666838: cmp      r3, #0
0066683c: str      r3, [r5]
00666840: bne      #0x66685c
00666844: ldr      r0, [r5, #0xc]
00666848: cmp      r0, #0
0066684c: beq      #0x666854
00666850: bl       #0x30e0b8
00666854: mov      r3, #0
00666858: str      r3, [r5, #0xc]
0066685c: mov      r3, #0
00666860: str      r3, [sp, #0x1c]
00666864: ldr      r3, [r4, #0x1c]
00666868: add      r2, sp, #0x20
0066686c: add      r0, r4, #0x50
00666870: ldr      r1, [r3, #0x84]
00666874: mov      r3, #0
00666878: str      r3, [r2, #-8]!
0066687c: bl       #0x665f98
00666880: ldr      r5, [sp, #0x18]
00666884: cmp      r5, #0
00666888: beq      #0x6668a8
0066688c: ldr      r3, [r5]
00666890: sub      r3, r3, #1
00666894: cmp      r3, #0
00666898: str      r3, [r5]
0066689c: beq      #0x66698c
006668a0: mov      r3, #0
006668a4: str      r3, [sp, #0x18]
006668a8: ldr      r3, [r4, #0x1c]
006668ac: ldr      r2, [r3, #0x84]
006668b0: cmp      r2, #0
006668b4: ble      #0x66669c
006668b8: mov      r5, #0
006668bc: add      fp, sp, #0x14
006668c0: mov      r8, r5
006668c4: ldr      r3, [r3, #0x88]
006668c8: mov      r2, sl
006668cc: mov      r0, fp
006668d0: add      r3, r3, r5, lsl #3
006668d4: ldr      r1, [r3, #4]
006668d8: ldr      r7, [r4, #0x50]
006668dc: bl       #0x663c3c
006668e0: ldr      r3, [sp, #0x14]
006668e4: lsl      r2, r5, #2
006668e8: cmp      r3, #0
006668ec: ldrne    r1, [r3]
006668f0: addne    r1, r1, #1
006668f4: strne    r1, [r3]
006668f8: ldr      r3, [r7, r2]
006668fc: cmp      r3, #0
00666900: beq      #0x666934
00666904: ldr      r1, [r3]
00666908: sub      r1, r1, #1
0066690c: cmp      r1, #0
00666910: str      r1, [r3]
00666914: bne      #0x666934
00666918: ldr      r0, [r3, #0xc]
0066691c: cmp      r0, #0
00666920: beq      #0x666930
00666924: stm      sp, {r2, r3}
00666928: bl       #0x30e0b8
0066692c: ldm      sp, {r2, r3}
00666930: str      r8, [r3, #0xc]
00666934: ldr      r3, [sp, #0x14]
00666938: str      r3, [r7, r2]
0066693c: ldr      r7, [sp, #0x14]
00666940: cmp      r7, #0
00666944: beq      #0x666974
00666948: ldr      r3, [r7]
0066694c: sub      r3, r3, #1
00666950: cmp      r3, #0
00666954: str      r3, [r7]
00666958: bne      #0x666970
0066695c: ldr      r0, [r7, #0xc]
00666960: cmp      r0, #0
00666964: beq      #0x66696c
00666968: bl       #0x30e0b8
0066696c: str      r8, [r7, #0xc]
00666970: str      r8, [sp, #0x14]
00666974: ldr      r3, [r4, #0x1c]
00666978: add      r5, r5, #1
0066697c: ldr      r2, [r3, #0x84]
00666980: cmp      r5, r2
00666984: blt      #0x6668c4
00666988: b        #0x66669c
0066698c: ldr      r0, [r5, #0xc]
00666990: cmp      r0, #0
00666994: beq      #0x66699c
00666998: bl       #0x30e0b8
0066699c: mov      r3, #0
006669a0: str      r3, [r5, #0xc]
006669a4: b        #0x6668a0
006669a8: ldr      r2, [r7]
006669ac: ldr      r3, [r8]
006669b0: ldr      r2, [r2, #0x24]
006669b4: ldr      r3, [r3, #0x20]
006669b8: ldr      r2, [r2, #0x20]
006669bc: ldr      r3, [r3, #0x34]
006669c0: ldr      r2, [r2, #4]
006669c4: mov      r0, r3
006669c8: ldr      r3, [r3]
006669cc: ldr      r1, [r2, #0x14]
006669d0: mov      lr, pc
006669d4: ldr      pc, [r3, #0xc]
006669d8: mov      sb, r0
006669dc: b        #0x666680
006669e0: mlaseq   r2, r0, r5, lr
006669e4: andeq    r0, r0, r0, asr #20
006669e8: strheq   r1, [r0], -r4
006669ec: andeq    r1, r0, r4, lsl r3
006669f0: andeq    r4, r0, r8, asr #8
006669f4: strdeq   r4, r5, [r0], -ip

# _ZN6glitch7collada6detail29CColladaSoftwareSkinTechnique12prepareCacheEv
0066fe34: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0066fe38: ldr      r3, [r0, #0x10]
0066fe3c: sub      sp, sp, #0xd0
0066fe40: mov      r5, r0
0066fe44: ldr      r3, [r3]
0066fe48: tst      r3, #1
0066fe4c: bne      #0x66fe58
0066fe50: add      sp, sp, #0xd0
0066fe54: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0066fe58: bl       #0x66fab4
0066fe5c: ldr      r3, [r5, #0xc]
0066fe60: ldr      r8, [r5, #0x10]
0066fe64: add      r4, sp, #0x8c
0066fe68: ldr      r7, [r3, #0x74]
0066fe6c: mov      r6, #0
0066fe70: mov      r1, r6
0066fe74: mov      r2, #0x40
0066fe78: mov      r0, r4
0066fe7c: add      r8, r8, #4
0066fe80: bl       #0x30e460
0066fe84: mov      r3, #0x3f800000
0066fe88: mov      r2, r4
0066fe8c: mov      ip, #1
0066fe90: mov      r0, r8
0066fe94: mov      r1, r7
0066fe98: str      r3, [sp, #0xc8]
0066fe9c: str      r3, [sp, #0x8c]
0066fea0: str      r3, [sp, #0xa0]
0066fea4: str      r3, [sp, #0xb4]
0066fea8: strb     ip, [sp, #0xcc]
0066feac: bl       #0x66cc9c
0066feb0: ldr      r3, [r5, #0x10]
0066feb4: ldr      r2, [r3, #0x10]
0066feb8: ldr      sb, [r3, #0x14]
0066febc: rsb      sb, r2, sb
0066fec0: asrs     sb, sb, #2
0066fec4: beq      #0x66ff38
0066fec8: mov      r4, r6
0066fecc: add      r8, sp, #0x48
0066fed0: add      r7, sp, #4
0066fed4: b        #0x66fee0
0066fed8: ldr      r3, [r5, #0x10]
0066fedc: ldr      r2, [r3, #0x10]
0066fee0: ldr      r0, [r5, #0xc]
0066fee4: ldr      r1, [r2, r4, lsl #2]
0066fee8: ldr      sl, [r3, #4]
0066feec: ldr      r2, [r0, #4]
0066fef0: mov      r0, r8
0066fef4: add      sl, sl, r6
0066fef8: add      r2, r2, r4, lsl #6
0066fefc: bl       #0x6651a0
0066ff00: ldr      r2, [r5, #0xc]
0066ff04: mov      r0, r7
0066ff08: mov      r1, r8
0066ff0c: add      r2, r2, #0x10
0066ff10: bl       #0x6651a0
0066ff14: add      r4, r4, #1
0066ff18: mov      r0, sl
0066ff1c: mov      r1, r7
0066ff20: mov      r2, #0x41
0066ff24: bl       #0x30e868
0066ff28: cmp      r4, sb
0066ff2c: add      r6, r6, #0x44
0066ff30: bne      #0x66fed8
0066ff34: ldr      r3, [r5, #0x10]
0066ff38: ldr      r2, [r3]
0066ff3c: bic      r2, r2, #1
0066ff40: str      r2, [r3]
0066ff44: b        #0x66fe50

# _ZNK6glitch7collada16CColladaDatabase19constructControllerEPNS_5video12IVideoDriverEPNS0_19SInstanceControllerEPNS0_14CRootSceneNodeEb
0061ace8: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0061acec: mov      r6, r3
0061acf0: sub      sp, sp, #0x2c
0061acf4: ldr      r3, [r3, #4]
0061acf8: ldr      ip, [sp, #0x50]
0061acfc: ldrb     lr, [sp, #0x54]
0061ad00: mov      r5, r0
0061ad04: add      r3, r3, #1
0061ad08: str      ip, [sp]
0061ad0c: mov      sl, r1
0061ad10: mov      fp, r2
0061ad14: str      lr, [sp, #0x14]
0061ad18: bl       #0x61aa00
0061ad1c: ldr      r3, [r5]
0061ad20: cmp      r3, #0
0061ad24: beq      #0x61aeac
0061ad28: ldr      r2, [r6, #0xc]
0061ad2c: cmp      r2, #0
0061ad30: ble      #0x61ade0
0061ad34: mov      r7, #0
0061ad38: mov      r8, r7
0061ad3c: add      r4, sp, #0x20
0061ad40: add      sb, sp, #0x24
0061ad44: b        #0x61adb0
0061ad48: ldr      r2, [r3, #4]
0061ad4c: add      r2, r2, #1
0061ad50: bl       #0x61ac88
0061ad54: mov      r2, r0
0061ad58: mov      r0, r4
0061ad5c: ldr      r1, [sp, #0x50]
0061ad60: mov      r3, fp
0061ad64: bl       #0x65cafc
0061ad68: ldr      r0, [r5]
0061ad6c: mov      lr, #0
0061ad70: mov      r1, r8
0061ad74: ldr      ip, [r0]
0061ad78: mov      r3, sb
0061ad7c: mov      r2, r4
0061ad80: ldr      ip, [ip, #0x20]
0061ad84: str      lr, [sp, #0x24]
0061ad88: blx      ip
0061ad8c: mov      r0, sb
0061ad90: bl       #0x57a26c
0061ad94: mov      r0, r4
0061ad98: bl       #0x310be8
0061ad9c: ldr      r3, [r6, #0xc]
0061ada0: add      r8, r8, #1
0061ada4: add      r7, r7, #0x3c
0061ada8: cmp      r8, r3
0061adac: bge      #0x61addc
0061adb0: ldr      r3, [r6, #0x10]
0061adb4: mov      r0, sl
0061adb8: ldr      r1, [r3, r7]
0061adbc: add      r3, r3, r7
0061adc0: cmp      r1, #0
0061adc4: bne      #0x61ad48
0061adc8: ldr      r1, [r3, #8]
0061adcc: mov      r0, sl
0061add0: bl       #0x60e400
0061add4: mov      r2, r0
0061add8: b        #0x61ad58
0061addc: ldr      r3, [r5]
0061ade0: mov      r0, r3
0061ade4: mov      r1, fp
0061ade8: ldr      r3, [r3]
0061adec: ldr      r2, [sp, #0x14]
0061adf0: mov      lr, pc
0061adf4: ldr      pc, [r3, #0x40]
0061adf8: ldr      r3, [r6, #0xc]
0061adfc: cmp      r3, #0
0061ae00: ble      #0x61aeac
0061ae04: mov      r8, #0
0061ae08: mov      r7, r8
0061ae0c: add      r4, sp, #0x20
0061ae10: add      sb, sp, #0x1c
0061ae14: mov      fp, r8
0061ae18: ldr      r3, [r5]
0061ae1c: mov      r2, r7
0061ae20: mov      r0, r4
0061ae24: mov      r1, r3
0061ae28: ldr      r3, [r3]
0061ae2c: mov      lr, pc
0061ae30: ldr      pc, [r3, #0x18]
0061ae34: ldr      r2, [sl, #4]
0061ae38: ldr      r3, [r6, #0x10]
0061ae3c: mov      r0, sb
0061ae40: ldr      ip, [r2]
0061ae44: mov      r1, r2
0061ae48: add      r3, r3, r8
0061ae4c: str      r7, [sp, #8]
0061ae50: mov      r2, sl
0061ae54: str      r5, [sp]
0061ae58: str      r4, [sp, #4]
0061ae5c: str      fp, [sp, #0xc]
0061ae60: mov      lr, pc
0061ae64: ldr      pc, [ip, #0x24]
0061ae68: ldr      ip, [r5]
0061ae6c: mov      r1, r7
0061ae70: mov      r3, sb
0061ae74: mov      r2, r4
0061ae78: mov      r0, ip
0061ae7c: ldr      ip, [ip]
0061ae80: mov      lr, pc
0061ae84: ldr      pc, [ip, #0x20]
0061ae88: mov      r0, sb
0061ae8c: bl       #0x57a26c
0061ae90: mov      r0, r4
0061ae94: bl       #0x310be8
0061ae98: ldr      r3, [r6, #0xc]
0061ae9c: add      r7, r7, #1
0061aea0: add      r8, r8, #0x3c
0061aea4: cmp      r7, r3
0061aea8: blt      #0x61ae18
0061aeac: mov      r0, r5
0061aeb0: add      sp, sp, #0x2c
0061aeb4: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZNK6glitch7collada16CColladaDatabase19constructControllerEPNS_5video12IVideoDriverEPNS0_11SControllerEPNS0_14CRootSceneNodeE
0060fa24: push     {r4, r5, lr}
0060fa28: ldr      ip, [r3]
0060fa2c: sub      sp, sp, #0xc
0060fa30: mov      r4, r0
0060fa34: cmp      ip, #0
0060fa38: ldr      r5, [sp, #0x18]
0060fa3c: bne      #0x60fa54
0060fa40: str      r5, [sp]
0060fa44: bl       #0x60f924
0060fa48: mov      r0, r4
0060fa4c: add      sp, sp, #0xc
0060fa50: pop      {r4, r5, pc}
0060fa54: cmp      ip, #1
0060fa58: movne    r3, #0
0060fa5c: strne    r3, [r0]
0060fa60: bne      #0x60fa48
0060fa64: str      r5, [sp]
0060fa68: bl       #0x60f9a4
0060fa6c: b        #0x60fa48

# _ZN6glitch7collada12CSkinnedMesh4skinEj
00664ee0: push     {r4, r5, r6, r7, r8, sl, lr}
00664ee4: mov      r5, #0x14
00664ee8: mul      r5, r5, r1
00664eec: mov      r6, r1
00664ef0: ldr      r1, [r0, #0x5c]
00664ef4: mov      r4, r0
00664ef8: sub      sp, sp, #0x14
00664efc: add      r1, r1, r5
00664f00: bl       #0x66383c
00664f04: ldr      r7, [r4, #0x5c]
00664f08: add      r7, r7, r5
00664f0c: ldrb     r2, [r7, #0x10]
00664f10: ldrb     r3, [r7, #0x11]
00664f14: cmp      r2, r3
00664f18: beq      #0x664fb4
00664f1c: ldr      r8, [r7, #0xc]
00664f20: ldr      r3, [r4, #0x68]
00664f24: add      r0, sp, #0xc
00664f28: ldr      ip, [r8]
00664f2c: mov      r1, r3
00664f30: mov      r2, r6
00664f34: ldr      r3, [r3]
00664f38: ldr      sl, [ip, #0x14]
00664f3c: mov      lr, pc
00664f40: ldr      pc, [r3, #0x14]
00664f44: ldr      r3, [r4, #0x5c]
00664f48: ldrb     ip, [r4, #0x20]
00664f4c: mov      r1, r7
00664f50: add      r3, r3, r5
00664f54: ldr      r3, [r3, #4]
00664f58: ldr      r2, [sp, #0xc]
00664f5c: mov      r0, r8
00664f60: ldr      r3, [r3, #4]
00664f64: and      r7, r6, #0x1f
00664f68: ldr      r3, [r3, #4]
00664f6c: str      ip, [sp]
00664f70: blx      sl
00664f74: ldr      r3, [r4, #0x14]
00664f78: cmp      r0, #0
00664f7c: mov      r2, #1
00664f80: orrne    r7, r3, r2, lsl r7
00664f84: biceq    r7, r3, r2, lsl r7
00664f88: str      r7, [r4, #0x14]
00664f8c: ldr      r0, [sp, #0xc]
00664f90: cmp      r0, #0
00664f94: beq      #0x664f9c
00664f98: bl       #0x31d584
00664f9c: ldr      r3, [r4, #0x5c]
00664fa0: add      r3, r3, r5
00664fa4: ldrb     r2, [r3, #0x10]
00664fa8: strb     r2, [r3, #0x11]
00664fac: ldr      r7, [r4, #0x5c]
00664fb0: add      r7, r7, r5
00664fb4: ldr      r5, [r7, #0xc]
00664fb8: ldr      r3, [r4, #0x68]
00664fbc: mov      r2, r6
00664fc0: ldr      ip, [r5]
00664fc4: mov      r1, r3
00664fc8: add      r0, sp, #8
00664fcc: ldr      r3, [r3]
00664fd0: ldr      r4, [ip, #0x18]
00664fd4: mov      lr, pc
00664fd8: ldr      pc, [r3, #0x14]
00664fdc: mov      r0, r5
00664fe0: mov      r1, r7
00664fe4: ldr      r2, [sp, #8]
00664fe8: blx      r4
00664fec: ldr      r0, [sp, #8]
00664ff0: cmp      r0, #0
00664ff4: beq      #0x664ffc
00664ff8: bl       #0x31d584
00664ffc: add      sp, sp, #0x14
00665000: pop      {r4, r5, r6, r7, r8, sl, pc}
