
# _ZNK6glitch5scene24COctTreeTriangleSelector26getTrianglesFromOctTreeBoxEPNS1_12SOctTreeNodeE
005870fc: push     {r4, r5, r6, lr}
00587100: mov      r6, r0
00587104: mov      r5, r1
00587108: ldr      r0, [r1, #0x2c]
0058710c: ldr      r1, [r6, #0x50]
00587110: bl       #0x30e9ac
00587114: cmp      r0, #0
00587118: beq      #0x5871c4
0058711c: ldr      r0, [r5, #0x30]
00587120: ldr      r1, [r6, #0x54]
00587124: bl       #0x30e9ac
00587128: cmp      r0, #0
0058712c: beq      #0x5871c4
00587130: ldr      r0, [r5, #0x34]
00587134: ldr      r1, [r6, #0x58]
00587138: bl       #0x30e9ac
0058713c: cmp      r0, #0
00587140: beq      #0x5871c4
00587144: ldr      r0, [r5, #0x38]
00587148: ldr      r1, [r6, #0x44]
0058714c: bl       #0x30e4b4
00587150: cmp      r0, #0
00587154: beq      #0x5871c4
00587158: ldr      r0, [r5, #0x3c]
0058715c: ldr      r1, [r6, #0x48]
00587160: bl       #0x30e4b4
00587164: cmp      r0, #0
00587168: beq      #0x5871c4
0058716c: ldr      r0, [r5, #0x40]
00587170: ldr      r1, [r6, #0x4c]
00587174: bl       #0x30e4b4
00587178: cmp      r0, #0
0058717c: beq      #0x5871c4
00587180: mov      r0, r6
00587184: mov      r1, r5
00587188: bl       #0x586ec8
0058718c: ldr      r2, [r6, #0xa8]
00587190: ldr      r3, [r6, #0xa4]
00587194: cmp      r2, r3
00587198: beq      #0x5871c4
0058719c: mov      r4, #0
005871a0: ldr      r1, [r5, #0xc]
005871a4: add      r4, r4, #1
005871a8: mov      r0, r6
005871ac: cmp      r1, #0
005871b0: beq      #0x5871b8
005871b4: bl       #0x5870fc
005871b8: cmp      r4, #8
005871bc: add      r5, r5, #4
005871c0: bne      #0x5871a0
005871c4: pop      {r4, r5, r6, pc}

# _ZNK6glitch5scene17CTriangleSelector11TestWithBoxERSt6vectorINS_4core10triangle3dIfEENS3_10SAllocatorIS5_LNS_6memory13E_MEMORY_HINTE0EEEE
00586ec8: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00586ecc: ldr      r3, [r1]
00586ed0: ldr      r2, [r1, #4]
00586ed4: sub      sp, sp, #0x1c
00586ed8: str      r0, [sp, #0xc]
00586edc: rsb      r2, r3, r2
00586ee0: asr      r2, r2, #2
00586ee4: mov      sl, r1
00586ee8: lsl      r1, r2, #3
00586eec: ldr      r7, [r0, #0x44]
00586ef0: ldr      r8, [r0, #0x50]
00586ef4: rsb      r1, r2, r1
00586ef8: ldr      r0, [r0, #0x48]
00586efc: add      r1, r1, r1, lsl #6
00586f00: str      r0, [sp, #4]
00586f04: add      r1, r2, r1, lsl #3
00586f08: ldr      r0, [sp, #0xc]
00586f0c: lsl      sb, r1, #0xf
00586f10: rsb      r1, r1, sb
00586f14: ldr      r0, [r0, #0x54]
00586f18: add      sb, r2, r1, lsl #3
00586f1c: ldr      r1, [sp, #0xc]
00586f20: str      r0, [sp, #8]
00586f24: ldr      r2, [sp, #0xc]
00586f28: ldr      r1, [r1, #0x4c]
00586f2c: cmp      sb, #0
00586f30: str      r1, [sp, #0x10]
00586f34: ldr      r2, [r2, #0x58]
00586f38: str      r2, [sp, #0x14]
00586f3c: ble      #0x5870dc
00586f40: mov      r4, #0
00586f44: mov      r5, r4
00586f48: mov      r6, r3
00586f4c: ldr      fp, [r6, r4]
00586f50: mov      r1, r7
00586f54: add      r6, r6, r4
00586f58: mov      r0, fp
00586f5c: bl       #0x30e70c
00586f60: cmp      r0, #0
00586f64: beq      #0x586f90
00586f68: ldr      r0, [r6, #0xc]
00586f6c: mov      r1, r7
00586f70: bl       #0x30e70c
00586f74: cmp      r0, #0
00586f78: beq      #0x586f90
00586f7c: ldr      r0, [r6, #0x18]
00586f80: mov      r1, r7
00586f84: bl       #0x30e70c
00586f88: cmp      r0, #0
00586f8c: bne      #0x587048
00586f90: mov      r1, fp
00586f94: mov      r0, r8
00586f98: bl       #0x30e70c
00586f9c: cmp      r0, #0
00586fa0: beq      #0x586fcc
00586fa4: ldr      r0, [r6, #0xc]
00586fa8: mov      r1, r8
00586fac: bl       #0x30e2f8
00586fb0: cmp      r0, #0
00586fb4: beq      #0x586fcc
00586fb8: ldr      r0, [r6, #0x18]
00586fbc: mov      r1, r8
00586fc0: bl       #0x30e2f8
00586fc4: cmp      r0, #0
00586fc8: bne      #0x587048
00586fcc: ldr      fp, [r6, #4]
00586fd0: ldr      r1, [sp, #4]
00586fd4: mov      r0, fp
00586fd8: bl       #0x30e70c
00586fdc: cmp      r0, #0
00586fe0: beq      #0x58700c
00586fe4: ldr      r0, [r6, #0x10]
00586fe8: ldr      r1, [sp, #4]
00586fec: bl       #0x30e70c
00586ff0: cmp      r0, #0
00586ff4: beq      #0x58700c
00586ff8: ldr      r0, [r6, #0x1c]
00586ffc: ldr      r1, [sp, #4]
00587000: bl       #0x30e70c
00587004: cmp      r0, #0
00587008: bne      #0x587048
0058700c: mov      r1, fp
00587010: ldr      r0, [sp, #8]
00587014: bl       #0x30e70c
00587018: cmp      r0, #0
0058701c: beq      #0x587060
00587020: ldr      r0, [r6, #0x10]
00587024: ldr      r1, [sp, #8]
00587028: bl       #0x30e2f8
0058702c: cmp      r0, #0
00587030: beq      #0x587060
00587034: ldr      r0, [r6, #0x1c]
00587038: ldr      r1, [sp, #8]
0058703c: bl       #0x30e2f8
00587040: cmp      r0, #0
00587044: beq      #0x587060
00587048: add      r5, r5, #1
0058704c: cmp      r5, sb
00587050: add      r4, r4, #0x24
00587054: beq      #0x5870dc
00587058: ldr      r6, [sl]
0058705c: b        #0x586f4c
00587060: ldr      fp, [r6, #8]
00587064: ldr      r1, [sp, #0x10]
00587068: mov      r0, fp
0058706c: bl       #0x30e70c
00587070: cmp      r0, #0
00587074: beq      #0x58708c
00587078: ldr      r0, [r6, #0x14]
0058707c: ldr      r1, [sp, #0x10]
00587080: bl       #0x30e70c
00587084: cmp      r0, #0
00587088: bne      #0x5870e4
0058708c: mov      r1, fp
00587090: ldr      r0, [sp, #0x14]
00587094: bl       #0x30e70c
00587098: cmp      r0, #0
0058709c: beq      #0x5870c8
005870a0: ldr      r0, [r6, #0x14]
005870a4: ldr      r1, [sp, #0x14]
005870a8: bl       #0x30e2f8
005870ac: cmp      r0, #0
005870b0: beq      #0x5870c8
005870b4: ldr      r0, [r6, #0x20]
005870b8: ldr      r1, [sp, #0x14]
005870bc: bl       #0x30e2f8
005870c0: cmp      r0, #0
005870c4: bne      #0x587048
005870c8: mov      r1, r6
005870cc: ldr      r0, [sp, #0xc]
005870d0: bl       #0x586acc
005870d4: cmp      r0, #0
005870d8: beq      #0x587048
005870dc: add      sp, sp, #0x1c
005870e0: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
005870e4: ldr      r0, [r6, #0x20]
005870e8: ldr      r1, [sp, #0x10]
005870ec: bl       #0x30e70c
005870f0: cmp      r0, #0
005870f4: bne      #0x587048
005870f8: b        #0x58708c

# _ZNK6glitch4core10triangle3dIfE16isTotalInsideBoxERKNS0_8aabbox3dIfEE
005857ac: push     {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
005857b0: ldr      r6, [r0]
005857b4: ldr      r7, [r1]
005857b8: mov      r5, r0
005857bc: mov      r4, r1
005857c0: mov      r0, r6
005857c4: mov      r1, r7
005857c8: bl       #0x30e4b4
005857cc: cmp      r0, #0
005857d0: beq      #0x585968
005857d4: ldr      r8, [r4, #0xc]
005857d8: mov      r0, r6
005857dc: mov      r1, r8
005857e0: bl       #0x30e9ac
005857e4: cmp      r0, #0
005857e8: beq      #0x585968
005857ec: ldr      sl, [r5, #4]
005857f0: ldr      r6, [r4, #4]
005857f4: mov      r0, sl
005857f8: mov      r1, r6
005857fc: bl       #0x30e4b4
00585800: cmp      r0, #0
00585804: beq      #0x585968
00585808: ldr      sb, [r4, #0x10]
0058580c: mov      r0, sl
00585810: mov      r1, sb
00585814: bl       #0x30e9ac
00585818: cmp      r0, #0
0058581c: beq      #0x585968
00585820: ldr      fp, [r5, #8]
00585824: ldr      sl, [r4, #8]
00585828: mov      r0, fp
0058582c: mov      r1, sl
00585830: bl       #0x30e4b4
00585834: cmp      r0, #0
00585838: beq      #0x585968
0058583c: ldr      r4, [r4, #0x14]
00585840: mov      r0, fp
00585844: mov      r1, r4
00585848: bl       #0x30e9ac
0058584c: cmp      r0, #0
00585850: beq      #0x585968
00585854: ldr      fp, [r5, #0xc]
00585858: mov      r0, r7
0058585c: mov      r1, fp
00585860: bl       #0x30e9ac
00585864: cmp      r0, #0
00585868: beq      #0x585968
0058586c: mov      r1, fp
00585870: mov      r0, r8
00585874: bl       #0x30e4b4
00585878: cmp      r0, #0
0058587c: beq      #0x585968
00585880: ldr      fp, [r5, #0x10]
00585884: mov      r0, r6
00585888: mov      r1, fp
0058588c: bl       #0x30e9ac
00585890: cmp      r0, #0
00585894: beq      #0x585968
00585898: mov      r1, fp
0058589c: mov      r0, sb
005858a0: bl       #0x30e4b4
005858a4: cmp      r0, #0
005858a8: beq      #0x585968
005858ac: ldr      fp, [r5, #0x14]
005858b0: mov      r0, sl
005858b4: mov      r1, fp
005858b8: bl       #0x30e9ac
005858bc: cmp      r0, #0
005858c0: beq      #0x585968
005858c4: mov      r1, fp
005858c8: mov      r0, r4
005858cc: bl       #0x30e4b4
005858d0: cmp      r0, #0
005858d4: beq      #0x585968
005858d8: ldr      fp, [r5, #0x18]
005858dc: mov      r0, r7
005858e0: mov      r1, fp
005858e4: bl       #0x30e9ac
005858e8: cmp      r0, #0
005858ec: beq      #0x585968
005858f0: mov      r0, r8
005858f4: mov      r1, fp
005858f8: bl       #0x30e4b4
005858fc: cmp      r0, #0
00585900: beq      #0x585968
00585904: ldr      r7, [r5, #0x1c]
00585908: mov      r0, r6
0058590c: mov      r1, r7
00585910: bl       #0x30e9ac
00585914: cmp      r0, #0
00585918: beq      #0x585968
0058591c: mov      r0, sb
00585920: mov      r1, r7
00585924: bl       #0x30e4b4
00585928: cmp      r0, #0
0058592c: beq      #0x585968
00585930: ldr      r5, [r5, #0x20]
00585934: mov      r0, sl
00585938: mov      r1, r5
0058593c: bl       #0x30e9ac
00585940: cmp      r0, #0
00585944: beq      #0x585968
00585948: mov      r0, r4
0058594c: mov      r1, r5
00585950: bl       #0x30e4b4
00585954: cmp      r0, #0
00585958: mov      r0, #0
0058595c: movne    r0, #1
00585960: uxtb     r0, r0
00585964: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
00585968: mov      r0, #0
0058596c: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZNK6glitch4core8aabbox3dIfE8getEdgesEPNS0_8vector3dIfEE
00585638: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0058563c: ldr      fp, [r0, #0xc]
00585640: sub      sp, sp, #0xc
00585644: mov      r4, r1
00585648: mov      r5, r0
0058564c: ldr      r1, [r0]
00585650: mov      r0, fp
00585654: bl       #0x30eba4
00585658: mov      r1, #0x3f000000
0058565c: bl       #0x30ed6c
00585660: ldr      r7, [r5, #0x10]
00585664: ldr      r1, [r5, #4]
00585668: mov      r8, r0
0058566c: mov      r0, r7
00585670: bl       #0x30eba4
00585674: mov      r1, #0x3f000000
00585678: bl       #0x30ed6c
0058567c: ldr      r6, [r5, #0x14]
00585680: ldr      r1, [r5, #8]
00585684: mov      sb, r0
00585688: mov      r0, r6
0058568c: bl       #0x30eba4
00585690: mov      r1, #0x3f000000
00585694: bl       #0x30ed6c
00585698: mov      r1, fp
0058569c: mov      sl, r0
005856a0: mov      r0, r8
005856a4: bl       #0x30e3ac
005856a8: mov      r1, r7
005856ac: str      r0, [sp]
005856b0: mov      r0, sb
005856b4: bl       #0x30e3ac
005856b8: mov      r1, r6
005856bc: mov      fp, r0
005856c0: mov      r0, sl
005856c4: bl       #0x30e3ac
005856c8: ldr      r1, [sp]
005856cc: str      r0, [sp, #4]
005856d0: mov      r0, r8
005856d4: bl       #0x30eba4
005856d8: mov      r1, fp
005856dc: mov      r7, r0
005856e0: mov      r0, sb
005856e4: bl       #0x30eba4
005856e8: ldr      r1, [sp, #4]
005856ec: mov      r5, r0
005856f0: mov      r0, sl
005856f4: bl       #0x30eba4
005856f8: mov      r1, fp
005856fc: mov      r6, r0
00585700: str      r0, [r4, #8]
00585704: str      r7, [r4]
00585708: mov      r0, sb
0058570c: str      r5, [r4, #4]
00585710: bl       #0x30e3ac
00585714: add      r3, r4, #0xc
00585718: str      r7, [r4, #0xc]
0058571c: str      r6, [r3, #8]
00585720: ldr      r1, [sp, #4]
00585724: mov      sb, r0
00585728: str      r0, [r3, #4]
0058572c: mov      r0, sl
00585730: bl       #0x30e3ac
00585734: add      r2, r4, #0x18
00585738: add      r3, r4, #0x24
0058573c: str      r7, [r4, #0x18]
00585740: str      r0, [r2, #8]
00585744: str      r5, [r2, #4]
00585748: str      r7, [r4, #0x24]
0058574c: str      r0, [r3, #8]
00585750: str      sb, [r3, #4]
00585754: ldr      r1, [sp]
00585758: mov      sl, r0
0058575c: mov      r0, r8
00585760: bl       #0x30e3ac
00585764: add      ip, r4, #0x30
00585768: add      r1, r4, #0x3c
0058576c: add      r2, r4, #0x48
00585770: add      r3, r4, #0x54
00585774: str      r0, [r4, #0x30]
00585778: str      r6, [ip, #8]
0058577c: str      r5, [ip, #4]
00585780: str      r0, [r4, #0x3c]
00585784: str      r6, [r1, #8]
00585788: str      sb, [r1, #4]
0058578c: str      r0, [r4, #0x48]
00585790: str      r5, [r2, #4]
00585794: str      sl, [r2, #8]
00585798: str      r0, [r4, #0x54]
0058579c: str      sl, [r3, #8]
005857a0: str      sb, [r3, #4]
005857a4: add      sp, sp, #0xc
005857a8: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZNSt6vectorIN6glitch4core10triangle3dIfEENS1_10SAllocatorIS3_LNS0_6memory13E_MEMORY_HINTE0EEEE22_M_insert_overflow_auxEPS3_RKS3_RKSt12__false_typejb
00587ac4: push     {r4, r5, r6, r7, r8, sb, sl, lr}
00587ac8: ldr      r5, [sp, #0x20]
00587acc: mov      r4, r1
00587ad0: mov      r6, r2
00587ad4: mov      r1, r5
00587ad8: mov      r7, r0
00587adc: ldrb     sl, [sp, #0x24]
00587ae0: bl       #0x586494
00587ae4: mov      r8, #0x24
00587ae8: mul      r8, r8, r0
00587aec: mov      r1, #0
00587af0: mov      r0, r8
00587af4: bl       #0x310568
00587af8: ldr      r3, [r7]
00587afc: mov      sb, r0
00587b00: rsb      r2, r3, r4
00587b04: asr      r2, r2, #2
00587b08: lsl      r1, r2, #3
00587b0c: rsb      r1, r2, r1
00587b10: add      r1, r1, r1, lsl #6
00587b14: add      r1, r2, r1, lsl #3
00587b18: lsl      ip, r1, #0xf
00587b1c: rsb      ip, r1, ip
00587b20: add      ip, r2, ip, lsl #3
00587b24: cmp      ip, #0
00587b28: movle    r3, r0
00587b2c: ble      #0x587b98
00587b30: mov      r1, ip
00587b34: mov      r2, r0
00587b38: ldr      r0, [r3]
00587b3c: subs     r1, r1, #1
00587b40: str      r0, [r2]
00587b44: ldr      r0, [r3, #4]
00587b48: str      r0, [r2, #4]
00587b4c: ldr      r0, [r3, #8]
00587b50: str      r0, [r2, #8]
00587b54: ldr      r0, [r3, #0xc]
00587b58: str      r0, [r2, #0xc]
00587b5c: ldr      r0, [r3, #0x10]
00587b60: str      r0, [r2, #0x10]
00587b64: ldr      r0, [r3, #0x14]
00587b68: str      r0, [r2, #0x14]
00587b6c: ldr      r0, [r3, #0x18]
00587b70: str      r0, [r2, #0x18]
00587b74: ldr      r0, [r3, #0x1c]
00587b78: str      r0, [r2, #0x1c]
00587b7c: ldr      r0, [r3, #0x20]
00587b80: add      r3, r3, #0x24
00587b84: str      r0, [r2, #0x20]
00587b88: add      r2, r2, #0x24
00587b8c: bne      #0x587b38
00587b90: mov      r3, #0x24
00587b94: mla      r3, r3, ip, sb
00587b98: cmp      r5, #1
00587b9c: beq      #0x587d30
00587ba0: mov      r2, #0x24
00587ba4: mla      r5, r2, r5, r3
00587ba8: rsb      r1, r3, r5
00587bac: asr      r1, r1, #2
00587bb0: lsl      r2, r1, #3
00587bb4: rsb      r2, r1, r2
00587bb8: add      r2, r2, r2, lsl #6
00587bbc: add      r2, r1, r2, lsl #3
00587bc0: lsl      r0, r2, #0xf
00587bc4: rsb      r2, r2, r0
00587bc8: add      r2, r1, r2, lsl #3
00587bcc: cmp      r2, #0
00587bd0: bgt      #0x587bdc
00587bd4: b        #0x587c2c
00587bd8: add      r3, r3, #0x24
00587bdc: ldr      r1, [r6]
00587be0: subs     r2, r2, #1
00587be4: str      r1, [r3]
00587be8: ldr      r1, [r6, #4]
00587bec: str      r1, [r3, #4]
00587bf0: ldr      r1, [r6, #8]
00587bf4: str      r1, [r3, #8]
00587bf8: ldr      r1, [r6, #0xc]
00587bfc: str      r1, [r3, #0xc]
00587c00: ldr      r1, [r6, #0x10]
00587c04: str      r1, [r3, #0x10]
00587c08: ldr      r1, [r6, #0x14]
00587c0c: str      r1, [r3, #0x14]
00587c10: ldr      r1, [r6, #0x18]
00587c14: str      r1, [r3, #0x18]
00587c18: ldr      r1, [r6, #0x1c]
00587c1c: str      r1, [r3, #0x1c]
00587c20: ldr      r1, [r6, #0x20]
00587c24: str      r1, [r3, #0x20]
00587c28: bne      #0x587bd8
00587c2c: cmp      sl, #0
00587c30: beq      #0x587c94
00587c34: ldr      r0, [r7, #4]
00587c38: ldr      r3, [r7]
00587c3c: cmp      r3, r0
00587c40: beq      #0x587c80
00587c44: sub      r2, r0, #0x24
00587c48: rsb      r3, r3, r2
00587c4c: lsr      r3, r3, #2
00587c50: lsl      r2, r3, #3
00587c54: rsb      r2, r3, r2
00587c58: add      r2, r2, r2, lsl #6
00587c5c: add      r2, r3, r2, lsl #3
00587c60: lsl      r1, r2, #0xf
00587c64: rsb      r2, r2, r1
00587c68: add      r3, r3, r2, lsl #3
00587c6c: bic      r3, r3, #0xc0000000
00587c70: mvn      r2, #0x23
00587c74: mul      r3, r2, r3
00587c78: add      r3, r3, r2
00587c7c: add      r0, r0, r3
00587c80: add      r8, sb, r8
00587c84: bl       #0x310450
00587c88: stmib    r7, {r5, r8}
00587c8c: str      sb, [r7]
00587c90: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
00587c94: ldr      r0, [r7, #4]
00587c98: rsb      r3, r4, r0
00587c9c: asr      r3, r3, #2
00587ca0: lsl      r2, r3, #3
00587ca4: rsb      r2, r3, r2
00587ca8: add      r2, r2, r2, lsl #6
00587cac: add      r2, r3, r2, lsl #3
00587cb0: lsl      ip, r2, #0xf
00587cb4: rsb      r2, r2, ip
00587cb8: add      ip, r3, r2, lsl #3
00587cbc: cmp      ip, #0
00587cc0: ble      #0x587c38
00587cc4: mov      r2, ip
00587cc8: mov      r3, r5
00587ccc: ldr      r1, [r4]
00587cd0: subs     r2, r2, #1
00587cd4: str      r1, [r3]
00587cd8: ldr      r1, [r4, #4]
00587cdc: str      r1, [r3, #4]
00587ce0: ldr      r1, [r4, #8]
00587ce4: str      r1, [r3, #8]
00587ce8: ldr      r1, [r4, #0xc]
00587cec: str      r1, [r3, #0xc]
00587cf0: ldr      r1, [r4, #0x10]
00587cf4: str      r1, [r3, #0x10]
00587cf8: ldr      r1, [r4, #0x14]
00587cfc: str      r1, [r3, #0x14]
00587d00: ldr      r1, [r4, #0x18]
00587d04: str      r1, [r3, #0x18]
00587d08: ldr      r1, [r4, #0x1c]
00587d0c: str      r1, [r3, #0x1c]
00587d10: ldr      r1, [r4, #0x20]
00587d14: add      r4, r4, #0x24
00587d18: str      r1, [r3, #0x20]
00587d1c: add      r3, r3, #0x24
00587d20: bne      #0x587ccc
00587d24: mov      r3, #0x24
00587d28: mla      r5, r3, ip, r5
00587d2c: b        #0x587c34
00587d30: ldr      r2, [r6]
00587d34: cmp      sl, #0
00587d38: add      r5, r3, #0x24
00587d3c: str      r2, [r3]
00587d40: ldr      r2, [r6, #4]
00587d44: str      r2, [r3, #4]
00587d48: ldr      r2, [r6, #8]
00587d4c: str      r2, [r3, #8]
00587d50: ldr      r2, [r6, #0xc]
00587d54: str      r2, [r3, #0xc]
00587d58: ldr      r2, [r6, #0x10]
00587d5c: str      r2, [r3, #0x10]
00587d60: ldr      r2, [r6, #0x14]
00587d64: str      r2, [r3, #0x14]
00587d68: ldr      r2, [r6, #0x18]
00587d6c: str      r2, [r3, #0x18]
00587d70: ldr      r2, [r6, #0x1c]
00587d74: str      r2, [r3, #0x1c]
00587d78: ldr      r2, [r6, #0x20]
00587d7c: str      r2, [r3, #0x20]
00587d80: bne      #0x587c34
00587d84: b        #0x587c94

# _ZN6glitch5scene24COctTreeTriangleSelector16constructOctTreeEPNS1_12SOctTreeNodeE
00587e60: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00587e64: sub      sp, sp, #0x10c
00587e68: str      r0, [sp, #0x14]
00587e6c: ldr      r3, [r0, #0xb0]
00587e70: mov      r4, r1
00587e74: add      r3, r3, #1
00587e78: str      r3, [r0, #0xb0]
00587e7c: ldr      sb, [r1]
00587e80: ldr      r3, [r1, #4]
00587e84: ldr      r1, [sb]
00587e88: rsb      r3, sb, r3
00587e8c: asr      r3, r3, #2
00587e90: str      r1, [r4, #0x38]
00587e94: ldr      r7, [sb, #4]
00587e98: lsl      r2, r3, #3
00587e9c: rsb      r2, r3, r2
00587ea0: str      r7, [r4, #0x3c]
00587ea4: ldr      r5, [sb, #8]
00587ea8: add      r2, r2, r2, lsl #6
00587eac: str      r5, [r4, #0x40]
00587eb0: ldr      r0, [sb]
00587eb4: add      r2, r3, r2, lsl #3
00587eb8: str      r0, [r4, #0x2c]
00587ebc: ldr      r8, [sb, #4]
00587ec0: lsl      ip, r2, #0xf
00587ec4: rsb      r2, r2, ip
00587ec8: str      r8, [r4, #0x30]
00587ecc: ldr      r6, [sb, #8]
00587ed0: adds     r2, r3, r2, lsl #3
00587ed4: str      r2, [sp, #0xc]
00587ed8: str      r6, [r4, #0x34]
00587edc: beq      #0x5880ac
00587ee0: mov      r6, #0
00587ee4: mov      fp, r6
00587ee8: b        #0x587ef0
00587eec: ldr      r1, [r4, #0x38]
00587ef0: ldr      r8, [sb, r6]
00587ef4: add      r3, sb, r6
00587ef8: ldr      r5, [r3, #8]
00587efc: mov      r0, r8
00587f00: ldr      r7, [r3, #4]
00587f04: bl       #0x30e2f8
00587f08: cmp      r0, #0
00587f0c: ldr      r1, [r4, #0x3c]
00587f10: strne    r8, [r4, #0x38]
00587f14: mov      r0, r7
00587f18: bl       #0x30e2f8
00587f1c: cmp      r0, #0
00587f20: ldr      r1, [r4, #0x40]
00587f24: strne    r7, [r4, #0x3c]
00587f28: mov      r0, r5
00587f2c: bl       #0x30e2f8
00587f30: cmp      r0, #0
00587f34: ldr      r1, [r4, #0x2c]
00587f38: strne    r5, [r4, #0x40]
00587f3c: mov      r0, r8
00587f40: bl       #0x30e70c
00587f44: cmp      r0, #0
00587f48: ldr      r1, [r4, #0x30]
00587f4c: strne    r8, [r4, #0x2c]
00587f50: mov      r0, r7
00587f54: bl       #0x30e70c
00587f58: cmp      r0, #0
00587f5c: ldr      r1, [r4, #0x34]
00587f60: strne    r7, [r4, #0x30]
00587f64: mov      r0, r5
00587f68: bl       #0x30e70c
00587f6c: cmp      r0, #0
00587f70: strne    r5, [r4, #0x34]
00587f74: add      r5, sb, r6
00587f78: ldr      sl, [r5, #0xc]
00587f7c: ldr      r1, [r4, #0x38]
00587f80: ldr      r8, [r5, #0x10]
00587f84: mov      r0, sl
00587f88: bl       #0x30e2f8
00587f8c: cmp      r0, #0
00587f90: ldr      r7, [r5, #0x14]
00587f94: ldr      r1, [r4, #0x3c]
00587f98: strne    sl, [r4, #0x38]
00587f9c: mov      r0, r8
00587fa0: bl       #0x30e2f8
00587fa4: cmp      r0, #0
00587fa8: ldr      r1, [r4, #0x40]
00587fac: strne    r8, [r4, #0x3c]
00587fb0: mov      r0, r7
00587fb4: bl       #0x30e2f8
00587fb8: cmp      r0, #0
00587fbc: ldr      r1, [r4, #0x2c]
00587fc0: strne    r7, [r4, #0x40]
00587fc4: mov      r0, sl
00587fc8: bl       #0x30e70c
00587fcc: cmp      r0, #0
00587fd0: ldr      r1, [r4, #0x30]
00587fd4: strne    sl, [r4, #0x2c]
00587fd8: mov      r0, r8
00587fdc: bl       #0x30e70c
00587fe0: cmp      r0, #0
00587fe4: ldr      r1, [r4, #0x34]
00587fe8: strne    r8, [r4, #0x30]
00587fec: mov      r0, r7
00587ff0: bl       #0x30e70c
00587ff4: cmp      r0, #0
00587ff8: strne    r7, [r4, #0x34]
00587ffc: ldr      r8, [r5, #0x18]
00588000: ldr      r1, [r4, #0x38]
00588004: ldr      r7, [r5, #0x20]
00588008: mov      r0, r8
0058800c: bl       #0x30e2f8
00588010: ldr      r5, [r5, #0x1c]
00588014: cmp      r0, #0
00588018: strne    r8, [r4, #0x38]
0058801c: mov      r0, r5
00588020: ldr      r1, [r4, #0x3c]
00588024: bl       #0x30e2f8
00588028: cmp      r0, #0
0058802c: ldr      r1, [r4, #0x40]
00588030: strne    r5, [r4, #0x3c]
00588034: mov      r0, r7
00588038: bl       #0x30e2f8
0058803c: cmp      r0, #0
00588040: ldr      r1, [r4, #0x2c]
00588044: strne    r7, [r4, #0x40]
00588048: mov      r0, r8
0058804c: bl       #0x30e70c
00588050: cmp      r0, #0
00588054: ldr      r1, [r4, #0x30]
00588058: strne    r8, [r4, #0x2c]
0058805c: mov      r0, r5
00588060: bl       #0x30e70c
00588064: cmp      r0, #0
00588068: strne    r5, [r4, #0x30]
0058806c: ldr      r1, [r4, #0x34]
00588070: mov      r0, r7
00588074: bl       #0x30e70c
00588078: cmp      r0, #0
0058807c: strne    r7, [r4, #0x34]
00588080: ldr      r0, [sp, #0xc]
00588084: add      fp, fp, #1
00588088: add      r6, r6, #0x24
0058808c: cmp      fp, r0
00588090: bne      #0x587eec
00588094: ldr      r0, [r4, #0x2c]
00588098: ldr      r1, [r4, #0x38]
0058809c: ldr      r8, [r4, #0x30]
005880a0: ldr      r7, [r4, #0x3c]
005880a4: ldr      r6, [r4, #0x34]
005880a8: ldr      r5, [r4, #0x40]
005880ac: bl       #0x30eba4
005880b0: mov      r1, #0x3f000000
005880b4: bl       #0x30ed6c
005880b8: mov      r1, r7
005880bc: mov      fp, r0
005880c0: mov      r0, r8
005880c4: bl       #0x30eba4
005880c8: mov      r1, #0x3f000000
005880cc: bl       #0x30ed6c
005880d0: mov      r1, r5
005880d4: str      r0, [sp, #0xc]
005880d8: mov      r0, r6
005880dc: bl       #0x30eba4
005880e0: mov      r1, #0x3f000000
005880e4: bl       #0x30ed6c
005880e8: add      sb, sp, #0x34
005880ec: str      r0, [sp, #0x10]
005880f0: mov      r2, #0
005880f4: add      r3, sb, #0xc
005880f8: add      r1, sb, #0x6c
005880fc: str      r2, [r3, #-0xc]
00588100: str      r2, [r3, #-8]
00588104: str      r2, [r3, #-4]
00588108: add      r3, r3, #0xc
0058810c: cmp      r3, r1
00588110: bne      #0x5880fc
00588114: add      r0, r4, #0x2c
00588118: mov      r1, sb
0058811c: bl       #0x585638
00588120: ldr      r6, [r4, #0x2c]
00588124: movw     r1, #0x37bd
00588128: mov      r3, #0
0058812c: movt     r1, #0x3586
00588130: mov      r0, r6
00588134: ldr      r5, [r4, #0x38]
00588138: str      r3, [sp, #0xfc]
0058813c: str      r3, [sp, #0xf4]
00588140: str      r3, [sp, #0xf8]
00588144: bl       #0x30eba4
00588148: mov      r1, r0
0058814c: mov      r0, r5
00588150: bl       #0x30e9ac
00588154: cmp      r0, #0
00588158: beq      #0x588228
0058815c: movw     r1, #0x37bd
00588160: movt     r1, #0x3586
00588164: mov      r0, r6
00588168: bl       #0x30e3ac
0058816c: mov      r1, r0
00588170: mov      r0, r5
00588174: bl       #0x30e4b4
00588178: cmp      r0, #0
0058817c: beq      #0x588228
00588180: ldr      r6, [r4, #0x30]
00588184: movw     r1, #0x37bd
00588188: movt     r1, #0x3586
0058818c: mov      r0, r6
00588190: bl       #0x30eba4
00588194: ldr      r5, [r4, #0x3c]
00588198: mov      r1, r0
0058819c: mov      r0, r5
005881a0: bl       #0x30e9ac
005881a4: cmp      r0, #0
005881a8: beq      #0x588228
005881ac: movw     r1, #0x37bd
005881b0: movt     r1, #0x3586
005881b4: mov      r0, r6
005881b8: bl       #0x30e3ac
005881bc: mov      r1, r0
005881c0: mov      r0, r5
005881c4: bl       #0x30e4b4
005881c8: cmp      r0, #0
005881cc: beq      #0x588228
005881d0: ldr      r6, [r4, #0x34]
005881d4: movw     r1, #0x37bd
005881d8: movt     r1, #0x3586
005881dc: mov      r0, r6
005881e0: bl       #0x30eba4
005881e4: ldr      r5, [r4, #0x40]
005881e8: mov      r1, r0
005881ec: mov      r0, r5
005881f0: bl       #0x30e9ac
005881f4: cmp      r0, #0
005881f8: beq      #0x588228
005881fc: movw     r1, #0x37bd
00588200: movt     r1, #0x3586
00588204: mov      r0, r6
00588208: bl       #0x30e3ac
0058820c: mov      r1, r0
00588210: mov      r0, r5
00588214: bl       #0x30e4b4
00588218: cmp      r0, #0
0058821c: beq      #0x588228
00588220: add      sp, sp, #0x10c
00588224: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00588228: ldr      r2, [r4, #4]
0058822c: ldr      r3, [r4]
00588230: ldr      ip, [sp, #0x14]
00588234: rsb      r3, r3, r2
00588238: asr      r3, r3, #2
0058823c: ldr      r1, [ip, #0xb4]
00588240: lsl      r2, r3, #3
00588244: rsb      r2, r3, r2
00588248: add      r2, r2, r2, lsl #6
0058824c: add      r2, r3, r2, lsl #3
00588250: lsl      r0, r2, #0xf
00588254: rsb      r2, r2, r0
00588258: add      r3, r3, r2, lsl #3
0058825c: cmp      r1, r3
00588260: bge      #0x588220
00588264: add      r0, sb, #0x68
00588268: add      r2, sp, #0xf4
0058826c: str      r0, [sp, #0x1c]
00588270: str      r2, [sp, #0x18]
00588274: add      r3, sp, #0xb8
00588278: add      ip, sp, #0x94
0058827c: add      r0, sp, #0x100
00588280: add      r2, sp, #0x104
00588284: add      sb, sb, #8
00588288: mov      sl, r4
0058828c: str      r3, [sp, #0x24]
00588290: str      ip, [sp, #0x20]
00588294: str      r0, [sp, #0x28]
00588298: str      r2, [sp, #0x2c]
0058829c: mov      r7, #0
005882a0: ldr      r2, [sp, #0xc]
005882a4: ldr      r3, [sp, #0x10]
005882a8: str      fp, [sp, #0xe8]
005882ac: str      r2, [sp, #0xec]
005882b0: str      r3, [sp, #0xf0]
005882b4: str      r2, [sp, #0xe0]
005882b8: str      r3, [sp, #0xe4]
005882bc: str      fp, [sp, #0xdc]
005882c0: ldr      r8, [sb, #-8]
005882c4: mov      r0, fp
005882c8: ldr      r6, [sb, #-4]
005882cc: mov      r1, r8
005882d0: bl       #0x30e70c
005882d4: mov      r1, r6
005882d8: cmp      r0, #0
005882dc: ldr      r0, [sp, #0xc]
005882e0: ldr      r5, [sb]
005882e4: strne    r8, [sp, #0xe8]
005882e8: bl       #0x30e70c
005882ec: ldr      r1, [sp, #0xf0]
005882f0: cmp      r0, #0
005882f4: mov      r0, r5
005882f8: strne    r6, [sp, #0xec]
005882fc: bl       #0x30e2f8
00588300: ldr      r1, [sp, #0xdc]
00588304: cmp      r0, #0
00588308: mov      r0, r8
0058830c: strne    r5, [sp, #0xf0]
00588310: bl       #0x30e70c
00588314: ldr      r1, [sp, #0xe0]
00588318: cmp      r0, #0
0058831c: mov      r0, r6
00588320: strne    r8, [sp, #0xdc]
00588324: bl       #0x30e70c
00588328: ldr      r1, [sp, #0xe4]
0058832c: cmp      r0, #0
00588330: mov      r0, r5
00588334: strne    r6, [sp, #0xe0]
00588338: bl       #0x30e70c
0058833c: mov      r1, #0
00588340: cmp      r0, #0
00588344: mov      r0, #0x44
00588348: strne    r5, [sp, #0xe4]
0058834c: bl       #0x5341ac
00588350: mov      r2, #0
00588354: mov      ip, #0xbf000000
00588358: add      ip, ip, #0x800000
0058835c: mov      r3, r2
00588360: str      r2, [r0]
00588364: str      r2, [r0, #4]
00588368: str      r2, [r0, #8]
0058836c: mov      r2, #0x3f800000
00588370: str      r2, [r0, #0x38]
00588374: str      r2, [r0, #0x3c]
00588378: str      r2, [r0, #0x40]
0058837c: str      ip, [r0, #0x2c]
00588380: str      ip, [r0, #0x30]
00588384: str      ip, [r0, #0x34]
00588388: mov      r2, r0
0058838c: mov      r5, r3
00588390: add      r3, r3, #1
00588394: cmp      r3, #8
00588398: str      r5, [r2, #0xc]
0058839c: add      r2, r2, #4
005883a0: bne      #0x588390
005883a4: str      r0, [sl, #0xc]
005883a8: ldm      r4, {r0, r3}
005883ac: rsb      r3, r0, r3
005883b0: cmp      r3, #0x23
005883b4: ble      #0x5884ec
005883b8: mov      r6, r5
005883bc: add      r8, sp, #0xdc
005883c0: b        #0x588468
005883c4: ldr      r0, [sl, #0xc]
005883c8: ldr      r3, [r4]
005883cc: ldmib    r0, {r1, ip}
005883d0: add      r2, r3, r5
005883d4: cmp      r1, ip
005883d8: beq      #0x5885c8
005883dc: ldr      r3, [r3, r5]
005883e0: str      r3, [r1]
005883e4: ldr      r3, [r2, #4]
005883e8: str      r3, [r1, #4]
005883ec: ldr      r3, [r2, #8]
005883f0: str      r3, [r1, #8]
005883f4: ldr      r3, [r2, #0xc]
005883f8: str      r3, [r1, #0xc]
005883fc: ldr      r3, [r2, #0x10]
00588400: str      r3, [r1, #0x10]
00588404: ldr      r3, [r2, #0x14]
00588408: str      r3, [r1, #0x14]
0058840c: ldr      r3, [r2, #0x18]
00588410: str      r3, [r1, #0x18]
00588414: ldr      r3, [r2, #0x1c]
00588418: str      r3, [r1, #0x1c]
0058841c: ldr      r3, [r2, #0x20]
00588420: str      r3, [r1, #0x20]
00588424: ldr      r3, [r0, #4]
00588428: add      r3, r3, #0x24
0058842c: str      r3, [r0, #4]
00588430: ldm      r4, {r0, r3}
00588434: add      r6, r6, #1
00588438: add      r5, r5, #0x24
0058843c: rsb      r3, r0, r3
00588440: asr      r3, r3, #2
00588444: lsl      r2, r3, #3
00588448: rsb      r2, r3, r2
0058844c: add      r2, r2, r2, lsl #6
00588450: add      r2, r3, r2, lsl #3
00588454: lsl      r1, r2, #0xf
00588458: rsb      r2, r2, r1
0058845c: add      r2, r3, r2, lsl #3
00588460: cmp      r6, r2
00588464: bge      #0x5884ec
00588468: add      r0, r0, r5
0058846c: mov      r1, r8
00588470: bl       #0x5857ac
00588474: cmp      r0, #0
00588478: bne      #0x5883c4
0058847c: ldr      r2, [sp, #0xfc]
00588480: ldr      r1, [sp, #0xf8]
00588484: ldr      r3, [r4]
00588488: cmp      r1, r2
0058848c: add      r2, r3, r5
00588490: beq      #0x5885e0
00588494: ldr      r3, [r3, r5]
00588498: str      r3, [r1]
0058849c: ldr      r3, [r2, #4]
005884a0: str      r3, [r1, #4]
005884a4: ldr      r3, [r2, #8]
005884a8: str      r3, [r1, #8]
005884ac: ldr      r3, [r2, #0xc]
005884b0: str      r3, [r1, #0xc]
005884b4: ldr      r3, [r2, #0x10]
005884b8: str      r3, [r1, #0x10]
005884bc: ldr      r3, [r2, #0x14]
005884c0: str      r3, [r1, #0x14]
005884c4: ldr      r3, [r2, #0x18]
005884c8: str      r3, [r1, #0x18]
005884cc: ldr      r3, [r2, #0x1c]
005884d0: str      r3, [r1, #0x1c]
005884d4: ldr      r3, [r2, #0x20]
005884d8: str      r3, [r1, #0x20]
005884dc: ldr      r3, [sp, #0xf8]
005884e0: add      r3, r3, #0x24
005884e4: str      r3, [sp, #0xf8]
005884e8: b        #0x588430
005884ec: ldr      r1, [sp, #0xf4]
005884f0: ldr      r3, [sp, #0xf8]
005884f4: rsb      r3, r1, r3
005884f8: asr      r3, r3, #2
005884fc: lsl      r2, r3, #3
00588500: rsb      r2, r3, r2
00588504: add      r2, r2, r2, lsl #6
00588508: add      r2, r3, r2, lsl #3
0058850c: lsl      ip, r2, #0xf
00588510: rsb      r2, r2, ip
00588514: adds     r2, r3, r2, lsl #3
00588518: moveq    r1, r2
0058851c: bne      #0x5885fc
00588520: mov      r0, r4
00588524: ldr      r2, [sp, #0x24]
00588528: str      r7, [sp, #0xb8]
0058852c: str      r7, [sp, #0xbc]
00588530: str      r7, [sp, #0xc0]
00588534: str      r7, [sp, #0xc4]
00588538: str      r7, [sp, #0xc8]
0058853c: str      r7, [sp, #0xcc]
00588540: str      r7, [sp, #0xd0]
00588544: str      r7, [sp, #0xd4]
00588548: str      r7, [sp, #0xd8]
0058854c: bl       #0x587dfc
00588550: ldr      r2, [sp, #0x20]
00588554: ldr      r0, [sp, #0x18]
00588558: mov      r1, #0
0058855c: str      r7, [sp, #0x94]
00588560: str      r7, [sp, #0x98]
00588564: str      r7, [sp, #0x9c]
00588568: str      r7, [sp, #0xa0]
0058856c: str      r7, [sp, #0xa4]
00588570: str      r7, [sp, #0xa8]
00588574: str      r7, [sp, #0xac]
00588578: str      r7, [sp, #0xb0]
0058857c: str      r7, [sp, #0xb4]
00588580: bl       #0x587dfc
00588584: ldr      r5, [sl, #0xc]
00588588: ldm      r5, {r2, r3}
0058858c: cmp      r2, r3
00588590: beq      #0x588638
00588594: mov      r1, r5
00588598: ldr      r0, [sp, #0x14]
0058859c: bl       #0x587e60
005885a0: ldr      r0, [sp, #0x1c]
005885a4: add      sb, sb, #0xc
005885a8: add      sl, sl, #4
005885ac: cmp      sb, r0
005885b0: bne      #0x5882a0
005885b4: ldr      r0, [sp, #0xf4]
005885b8: cmp      r0, #0
005885bc: beq      #0x588220
005885c0: bl       #0x310450
005885c4: b        #0x588220
005885c8: mov      ip, #1
005885cc: ldr      r3, [sp, #0x2c]
005885d0: str      ip, [sp]
005885d4: str      ip, [sp, #4]
005885d8: bl       #0x587ac4
005885dc: b        #0x588430
005885e0: mov      ip, #1
005885e4: ldr      r0, [sp, #0x18]
005885e8: ldr      r3, [sp, #0x28]
005885ec: str      ip, [sp]
005885f0: str      ip, [sp, #4]
005885f4: bl       #0x587ac4
005885f8: b        #0x588430
005885fc: mov      r3, #0x24
00588600: mul      r2, r3, r2
00588604: bl       #0x30e868
00588608: ldr      r2, [sp, #0xf8]
0058860c: ldr      r3, [sp, #0xf4]
00588610: rsb      r3, r3, r2
00588614: asr      r3, r3, #2
00588618: lsl      r2, r3, #3
0058861c: rsb      r2, r3, r2
00588620: add      r2, r2, r2, lsl #6
00588624: add      r2, r3, r2, lsl #3
00588628: lsl      r1, r2, #0xf
0058862c: rsb      r2, r2, r1
00588630: add      r1, r3, r2, lsl #3
00588634: b        #0x588520
00588638: mov      r0, r5
0058863c: bl       #0x5865d0
00588640: mov      r0, r5
00588644: bl       #0x30e2b0
00588648: mov      r3, #0
0058864c: str      r3, [sl, #0xc]
00588650: b        #0x5885a0

# _ZNSt6vectorIN6glitch4core10triangle3dIfEENS1_10SAllocatorIS3_LNS0_6memory13E_MEMORY_HINTE0EEEE6resizeEjRKS3_
00587dfc: push     {r4, r5, r6, r7}
00587e00: ldr      r4, [r0, #4]
00587e04: ldr      r5, [r0]
00587e08: mov      r3, r2
00587e0c: rsb      r2, r5, r4
00587e10: asr      r2, r2, #2
00587e14: lsl      r6, r2, #3
00587e18: rsb      r6, r2, r6
00587e1c: add      r6, r6, r6, lsl #6
00587e20: add      r6, r2, r6, lsl #3
00587e24: lsl      r7, r6, #0xf
00587e28: rsb      r6, r6, r7
00587e2c: add      r2, r2, r6, lsl #3
00587e30: cmp      r1, r2
00587e34: bhs      #0x587e50
00587e38: mov      r3, #0x24
00587e3c: mla      r5, r3, r1, r5
00587e40: cmp      r5, r4
00587e44: strne    r5, [r0, #4]
00587e48: pop      {r4, r5, r6, r7}
00587e4c: bx       lr
00587e50: rsb      r2, r2, r1
00587e54: mov      r1, r4
00587e58: pop      {r4, r5, r6, r7}
00587e5c: b        #0x587d88

# _ZNK6glitch5scene17CTriangleSelector9AddResultERKNS_4core10triangle3dIfEE
00586acc: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00586ad0: ldr      r3, [r0, #0xa8]
00586ad4: mov      r5, #0x24
00586ad8: ldr      ip, [r1]
00586adc: mul      r3, r5, r3
00586ae0: mov      r4, r0
00586ae4: ldr      r0, [r0, #0xa0]
00586ae8: sub      sp, sp, #0xc
00586aec: str      ip, [r0, r3]
00586af0: add      r2, r0, r3
00586af4: ldr      r3, [r1, #4]
00586af8: str      r3, [r2, #4]
00586afc: ldr      r3, [r1, #8]
00586b00: str      r3, [r2, #8]
00586b04: ldr      r3, [r1, #0xc]
00586b08: str      r3, [r2, #0xc]
00586b0c: ldr      r3, [r1, #0x10]
00586b10: str      r3, [r2, #0x10]
00586b14: ldr      r3, [r1, #0x14]
00586b18: str      r3, [r2, #0x14]
00586b1c: ldr      r3, [r1, #0x18]
00586b20: str      r3, [r2, #0x18]
00586b24: ldr      r3, [r1, #0x1c]
00586b28: str      r3, [r2, #0x1c]
00586b2c: ldr      r3, [r1, #0x20]
00586b30: str      r3, [r2, #0x20]
00586b34: ldrb     r3, [r4, #0x9c]
00586b38: cmp      r3, #0
00586b3c: bne      #0x586ea4
00586b40: ldr      r7, [r4, #0xa8]
00586b44: ldr      r8, [r4, #0xa0]
00586b48: ldr      r1, [r4, #0x60]
00586b4c: mul      r7, r5, r7
00586b50: ldr      fp, [r8, r7]
00586b54: add      r6, r8, r7
00586b58: ldr      sb, [r6, #4]
00586b5c: mov      r0, fp
00586b60: bl       #0x30ed6c
00586b64: ldr      r1, [r4, #0x70]
00586b68: mov      r3, r0
00586b6c: mov      r0, sb
00586b70: ldr      sl, [r6, #8]
00586b74: str      r3, [sp, #4]
00586b78: bl       #0x30ed6c
00586b7c: ldr      r3, [sp, #4]
00586b80: mov      r1, r0
00586b84: mov      r0, r3
00586b88: bl       #0x30eba4
00586b8c: ldr      r1, [r4, #0x80]
00586b90: mov      r3, r0
00586b94: mov      r0, sl
00586b98: str      r3, [sp, #4]
00586b9c: bl       #0x30ed6c
00586ba0: ldr      r3, [sp, #4]
00586ba4: mov      r1, r0
00586ba8: mov      r0, r3
00586bac: bl       #0x30eba4
00586bb0: ldr      r1, [r4, #0x90]
00586bb4: bl       #0x30eba4
00586bb8: ldr      r1, [r4, #0x64]
00586bbc: mov      r2, r0
00586bc0: mov      r0, fp
00586bc4: str      r2, [sp]
00586bc8: bl       #0x30ed6c
00586bcc: ldr      r1, [r4, #0x74]
00586bd0: mov      r3, r0
00586bd4: mov      r0, sb
00586bd8: str      r3, [sp, #4]
00586bdc: bl       #0x30ed6c
00586be0: ldr      r3, [sp, #4]
00586be4: mov      r1, r0
00586be8: mov      r0, r3
00586bec: bl       #0x30eba4
00586bf0: ldr      r1, [r4, #0x84]
00586bf4: mov      r3, r0
00586bf8: mov      r0, sl
00586bfc: str      r3, [sp, #4]
00586c00: bl       #0x30ed6c
00586c04: ldr      r3, [sp, #4]
00586c08: mov      r1, r0
00586c0c: mov      r0, r3
00586c10: bl       #0x30eba4
00586c14: ldr      r1, [r4, #0x94]
00586c18: bl       #0x30eba4
00586c1c: ldr      r1, [r4, #0x5c]
00586c20: mov      r3, r0
00586c24: mov      r0, fp
00586c28: str      r3, [sp, #4]
00586c2c: bl       #0x30ed6c
00586c30: ldr      r1, [r4, #0x6c]
00586c34: mov      fp, r0
00586c38: mov      r0, sb
00586c3c: bl       #0x30ed6c
00586c40: mov      r1, r0
00586c44: mov      r0, fp
00586c48: bl       #0x30eba4
00586c4c: ldr      r1, [r4, #0x7c]
00586c50: mov      sb, r0
00586c54: mov      r0, sl
00586c58: bl       #0x30ed6c
00586c5c: mov      r1, r0
00586c60: mov      r0, sb
00586c64: bl       #0x30eba4
00586c68: ldr      r1, [r4, #0x8c]
00586c6c: bl       #0x30eba4
00586c70: str      r0, [r8, r7]
00586c74: ldr      r3, [sp, #4]
00586c78: str      r3, [r6, #8]
00586c7c: ldr      r2, [sp]
00586c80: str      r2, [r6, #4]
00586c84: ldr      r3, [r4, #0xa0]
00586c88: ldr      r6, [r4, #0xa8]
00586c8c: ldr      r1, [r4, #0x60]
00586c90: mla      r6, r5, r6, r3
00586c94: ldr      sl, [r6, #0xc]
00586c98: ldr      r8, [r6, #0x10]
00586c9c: ldr      r7, [r6, #0x14]
00586ca0: mov      r0, sl
00586ca4: bl       #0x30ed6c
00586ca8: ldr      r1, [r4, #0x70]
00586cac: mov      sb, r0
00586cb0: mov      r0, r8
00586cb4: bl       #0x30ed6c
00586cb8: mov      r1, r0
00586cbc: mov      r0, sb
00586cc0: bl       #0x30eba4
00586cc4: ldr      r1, [r4, #0x80]
00586cc8: mov      sb, r0
00586ccc: mov      r0, r7
00586cd0: bl       #0x30ed6c
00586cd4: mov      r1, r0
00586cd8: mov      r0, sb
00586cdc: bl       #0x30eba4
00586ce0: ldr      r1, [r4, #0x90]
00586ce4: bl       #0x30eba4
00586ce8: ldr      r1, [r4, #0x64]
00586cec: mov      sb, r0
00586cf0: mov      r0, sl
00586cf4: bl       #0x30ed6c
00586cf8: ldr      r1, [r4, #0x74]
00586cfc: mov      fp, r0
00586d00: mov      r0, r8
00586d04: bl       #0x30ed6c
00586d08: mov      r1, r0
00586d0c: mov      r0, fp
00586d10: bl       #0x30eba4
00586d14: ldr      r1, [r4, #0x84]
00586d18: mov      fp, r0
00586d1c: mov      r0, r7
00586d20: bl       #0x30ed6c
00586d24: mov      r1, r0
00586d28: mov      r0, fp
00586d2c: bl       #0x30eba4
00586d30: ldr      r1, [r4, #0x94]
00586d34: bl       #0x30eba4
00586d38: ldr      r1, [r4, #0x5c]
00586d3c: mov      fp, r0
00586d40: mov      r0, sl
00586d44: bl       #0x30ed6c
00586d48: ldr      r1, [r4, #0x6c]
00586d4c: mov      sl, r0
00586d50: mov      r0, r8
00586d54: bl       #0x30ed6c
00586d58: mov      r1, r0
00586d5c: mov      r0, sl
00586d60: bl       #0x30eba4
00586d64: ldr      r1, [r4, #0x7c]
00586d68: mov      r8, r0
00586d6c: mov      r0, r7
00586d70: bl       #0x30ed6c
00586d74: mov      r1, r0
00586d78: mov      r0, r8
00586d7c: bl       #0x30eba4
00586d80: ldr      r1, [r4, #0x8c]
00586d84: bl       #0x30eba4
00586d88: str      r0, [r6, #0xc]
00586d8c: str      sb, [r6, #0x10]
00586d90: str      fp, [r6, #0x14]
00586d94: ldr      r2, [r4, #0xa8]
00586d98: ldr      r3, [r4, #0xa0]
00586d9c: ldr      r1, [r4, #0x60]
00586da0: mla      r5, r5, r2, r3
00586da4: ldr      r8, [r5, #0x18]
00586da8: ldr      r7, [r5, #0x1c]
00586dac: ldr      r6, [r5, #0x20]
00586db0: mov      r0, r8
00586db4: bl       #0x30ed6c
00586db8: ldr      r1, [r4, #0x70]
00586dbc: mov      sl, r0
00586dc0: mov      r0, r7
00586dc4: bl       #0x30ed6c
00586dc8: mov      r1, r0
00586dcc: mov      r0, sl
00586dd0: bl       #0x30eba4
00586dd4: ldr      r1, [r4, #0x80]
00586dd8: mov      sl, r0
00586ddc: mov      r0, r6
00586de0: bl       #0x30ed6c
00586de4: mov      r1, r0
00586de8: mov      r0, sl
00586dec: bl       #0x30eba4
00586df0: ldr      r1, [r4, #0x90]
00586df4: bl       #0x30eba4
00586df8: ldr      r1, [r4, #0x64]
00586dfc: mov      sl, r0
00586e00: mov      r0, r8
00586e04: bl       #0x30ed6c
00586e08: ldr      r1, [r4, #0x74]
00586e0c: mov      sb, r0
00586e10: mov      r0, r7
00586e14: bl       #0x30ed6c
00586e18: mov      r1, r0
00586e1c: mov      r0, sb
00586e20: bl       #0x30eba4
00586e24: ldr      r1, [r4, #0x84]
00586e28: mov      sb, r0
00586e2c: mov      r0, r6
00586e30: bl       #0x30ed6c
00586e34: mov      r1, r0
00586e38: mov      r0, sb
00586e3c: bl       #0x30eba4
00586e40: ldr      r1, [r4, #0x94]
00586e44: bl       #0x30eba4
00586e48: ldr      r1, [r4, #0x5c]
00586e4c: mov      sb, r0
00586e50: mov      r0, r8
00586e54: bl       #0x30ed6c
00586e58: ldr      r1, [r4, #0x6c]
00586e5c: mov      r8, r0
00586e60: mov      r0, r7
00586e64: bl       #0x30ed6c
00586e68: mov      r1, r0
00586e6c: mov      r0, r8
00586e70: bl       #0x30eba4
00586e74: ldr      r1, [r4, #0x7c]
00586e78: mov      r7, r0
00586e7c: mov      r0, r6
00586e80: bl       #0x30ed6c
00586e84: mov      r1, r0
00586e88: mov      r0, r7
00586e8c: bl       #0x30eba4
00586e90: ldr      r1, [r4, #0x8c]
00586e94: bl       #0x30eba4
00586e98: str      r0, [r5, #0x18]
00586e9c: str      sb, [r5, #0x20]
00586ea0: str      sl, [r5, #0x1c]
00586ea4: ldr      r3, [r4, #0xa8]
00586ea8: ldr      r0, [r4, #0xa4]
00586eac: add      r3, r3, #1
00586eb0: cmp      r3, r0
00586eb4: movne    r0, #0
00586eb8: moveq    r0, #1
00586ebc: str      r3, [r4, #0xa8]
00586ec0: add      sp, sp, #0xc
00586ec4: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
